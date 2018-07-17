<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * 
 * @param type $form
 * @param type $form_state
 * @return array
 */
function returns_confirmation_form($form, &$form_state) {
  $form = array();

  $arr['ajaxConfirm']['confirm-remove'] = array('title' => 'Confirmation Dialog', 'text' => 'Are you sure you want to confirm this Trip ID?', 'buttons' => array('button_confirm' => array('text' => 'Yes'), 'button_reject' => array('text' => 'No')));
  drupal_add_js($arr, 'setting');
  drupal_add_library('system', 'ui.dialog');
  $form['#attached']['js'][] = drupal_get_path('module', 'mrc_inventory_returns') . '/js/returns_confirmation_form.js';

  $form['#prefix'] = '<div id="itemlist-div" class="custom_form_wrapper">';
  $form['#suffix'] = '</div>';

  $form['trip_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Trip ID*'),
    '#size' => 20,
    '#maxlength' => 50,
    '#prefix' => '<div class="form_row"><div class="col_2">',
    '#suffix' => '</div></div>'
  );

  $form['trip_id_wrapper']['trip_id'] = array(
    '#type' => 'markup',
    '#title' => t('OR'),
    '#prefix' => '<div class="form_row"><div class="col_2"><div id="after_trip_id" class="text-center font-weight-bold or_txt"> OR',
    '#suffix' => '</div></div></div>'
  );

  $form['dispatch_from_date'] = array(
    '#type' => 'date_popup',
    '#title' => t('Dispatch From Date*'),
    '#date_format' => 'Y-m-d',
    '#date_label_position' => '',
    '#date_year_range' => '-20:+2',
    '#size' => 30,
    '#prefix' => '<div class="form_row"><div class="col_2">',
    '#suffix' => '</div>'
  );

  $form['dispatch_to_date'] = array(
    '#type' => 'date_popup',
    '#title' => t('Dispatch To Date*'),
    '#date_format' => 'Y-m-d',
    '#date_label_position' => '',
    '#date_year_range' => '-20:+2',
    '#size' => 30,
    '#prefix' => '<div class="col_2"> ',
    '#suffix' => '</div></div>'
  );


  $form['cashier_name'] = array(
    '#type' => 'select',
    '#title' => 'Cashier Name',
    '#options' => array('' => 'Select') + get_all_cashier_names(),
    '#prefix' => '<div class="form_row"><div class="col_2"> ',
    '#suffix' => '</div></div>'
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Show',
    '#name' => 'show',
    '#ajax' => array(
      'callback' => 'returns_confirmation_callback',
      'wrapper' => 'itemlist-div',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    '#attributes' => array('class' => array('trip-submit')),
    '#submit' => array('returns_confirmation_form_submit'),
  );

  $header = array('Trip ID', 'Dispatch Date', 'Cashier Name', 'Bill Count', 'Full Delivered Bill Count', 'Full Returned Bill Count', 'Partial Delivered Bill Count', 'Pending Bill Count', 'Summary Printout', 'Action');

  if (!isset($form_state['returns_confirmation']['bills'])) {
    $form_state['returns_confirmation']['bills'] = array();
  }
  $options = $form_state['returns_confirmation']['bills'];


  if (!empty($options)) {
    $form['bill_details'] = array(
      '#theme' => 'table',
      '#tree' => TRUE,
      '#header' => $header,
      '#rows' => array(),
      '#prefix' => '<div class="custom_middle_table">',
      '#suffix' => '</div>',
    );
  }

  $form['bill_details']['#rows'] = $options;
  foreach ($options as $key => $val) {
    $bill_count = $val['bill_count'];
    $full_delivered_bill_count = $val['full_delivered_bill_count'];
    $full_returned_bill_count = $val['full_returned_bill_count'];
    $partial_delivered_bill_count = $val['partial_delivered_bill_count'];
    
    $pending_bill_count = array(
      '#markup' => $bill_count - ($full_delivered_bill_count + $full_returned_bill_count + $partial_delivered_bill_count),
      '#name' => "pending_bill_count_$key"
    );
     
    $overall_summary_printout = array(
      '#markup' => l(t('Summary Printout'), "overall-summary-popup/nojs/$key", array('attributes' => array('class' => 'ctools-use-modal'))),
      '#name' => "overall_summary_$key",
        //'#attributes' => array('class' => array('full-return')),
    );

    $confirm_btn = array(
      '#type' => 'submit',
      '#value' => t('Confirm'),
      '#ajax' => array(
        'callback' => 'returns_confirmation_callback',
        'wrapper' => 'itemlist-div',
        'method' => 'replace',
        'effect' => 'fade',
      ),
      '#name' => "confirm_submit_$key",
      '#validate' => array('return_confirm_validate'),
      '#submit' => array('return_confirm_submit'),
      '#attributes' => array('class' => array('confirm-remove')),
    );
    
    $form['bill_details'][$key]["pending_bill_count_{$key}"] = &$pending_bill_count;
    $form['bill_details'][$key]["overall_summary_{$key}"] = &$overall_summary_printout;
    $form['bill_details'][$key]["confirm_submit_{$key}"] = &$confirm_btn;

    $form['bill_details']['#rows'][$key][]['data']["pending_bill_count_{$key}"] = &$pending_bill_count;
    $form['bill_details']['#rows'][$key][]['data']["overall_summary_{$key}"] = &$overall_summary_printout;
    $form['bill_details']['#rows'][$key][]['data']["confirm_submit_{$key}"] = &$confirm_btn;
    
    unset($pending_bill_count);
    unset($overall_summary_printout);
    unset($confirm_btn);
  }
  return $form;
}

function returns_confirmation_callback($form, &$form_state) {
  return $form;
}

/**
 * 
 * @param array $form
 * @param array $form_state
 * @return array
 */
function returns_confirmation_form_validate($form, &$form_state) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);

  if (empty($form_state['values']['trip_id']) && empty($form_state['values']['dispatch_from_date'])) {
    drupal_set_message(t("Please select either trip id or dispatch date."), 'error');
    $error_flag = 1;
  }

  if (!empty($form_state['values']['trip_id']) && !empty($form_state['values']['dispatch_from_date'])) {
    drupal_set_message(t("Please select either trip id or dispatch date."), 'error');
    $error_flag = 1;
  }

  if (!empty($form_state['values']['trip_id'])) {
    $check_trip_id = check_trip_id_exists_or_not($dis_id, $form_state['values']['trip_id']);
    if (empty($check_trip_id->nid)) {
      drupal_set_message(t("Trip id does not exists."), 'error');
      $error_flag = 1;
    }
    // check for returned confirmed or not
    $check_returns_flag = check_trip_id_exists_or_not($dis_id, $form_state['values']['trip_id']);
    if ($check_returns_flag->field_vrs_status_value >= 1) {
      unset($form_state['returns_confirmation']['bills']);
      drupal_set_message(t("This Trip status is already returns confirmed."), 'error');
      $error_flag = 1;
    }
  }

  if (!empty($form_state['values']['dispatch_from_date']) || !empty($form_state['values']['dispatch_to_date'])) {
    $dispatch_from_date = strtotime($form_state['values']['dispatch_from_date']);
    $dispatch_to_date = strtotime($form_state['values']['dispatch_to_date']);
    if (empty($dispatch_from_date)) {
      drupal_set_message(t("Dispatch From Date should not be empty."), 'error');
      $error_flag = 1;
    }
    if (empty($dispatch_to_date)) {
      drupal_set_message(t("Dispatch To Date should not be empty."), 'error');
      $error_flag = 1;
    }
    if ($dispatch_to_date < $dispatch_from_date) {
      drupal_set_message(t("Dispatch To Date should not be less than Dispatch From Date."), 'error');
      $error_flag = 1;
    }
  }
  $form_state['error_flag'] = $error_flag;
}

/**
 * 
 * @param array $form
 * @param array $form_state
 * @return array
 */
function returns_confirmation_form_submit($form, &$form_state) {
  if ($form_state['error_flag'] != 1) {
    global $user;
    $dis_id = change_feeds_import_get_distributor_id($user->uid);

    $trip_id = $form_state['values']['trip_id'];

    $dispatch_from_date = $form_state['values']['dispatch_from_date'];
    $dispatch_to_date = $form_state['values']['dispatch_to_date'];

    $cashier_name = $form_state['values']['cashier_name'];

    $query = db_select('node', 'n');
    $query->condition('n.type', 'sales_register_data', '=');
    $query->condition('n.uid', $dis_id, '=');
    $query->join('field_data_field_trip_id_reference', 'ti', 'ti.entity_id = n.nid');
    $query->leftjoin('node', 'disn', 'disn.nid = ti.field_trip_id_reference_nid');
    $query->condition('disn.type', 'dispatch_data');
    $query->leftjoin('field_data_field_vrs_status', 'vs', 'vs.entity_id = disn.nid');

    $db_and1 = db_and();
    $db_and1->isNULL('vs.field_vrs_status_value');

    $db_and2 = db_and();
    $db_and2->condition('vs.field_vrs_status_value', '0');

    $db_or = db_or();
    $db_or->condition($db_and1);
    $db_or->condition($db_and2);

    $query->condition($db_or);

    if (!empty($trip_id)) {
      $query->condition('disn.title', $trip_id, '=');
    }

    $query->leftjoin('field_data_field_dispatch_date', 'disp_date', 'disp_date.entity_id = disn.nid');
    if (!empty($dispatch_from_date)) {
      $query->condition('disp_date.field_dispatch_date_value', array($dispatch_from_date, $dispatch_to_date), 'BETWEEN');
    }

    $query->leftjoin('field_data_field_cashier_name', 'cn', 'cn.entity_id = disn.nid');
    if (!empty($cashier_name)) {
      $query->condition('cn.field_cashier_name_value', $cashier_name, '=');
    }

    $query->join('dispatch_planning_api_data', 'dpi', 'dpi.bill_number = n.title');
    $query->groupBy('disn.title');

    $query->fields('disn', array('title', 'title'));
    $query->addExpression("DATE_FORMAT(disp_date.field_dispatch_date_value, '%d-%b-%Y')", 'field_dispatch_date_value');
    $query->addExpression('cn.field_cashier_name_value', 'field_cashier_name_value');
    $query->addExpression('COUNT(n.title)', 'bill_count');
    $query->addExpression("SUM(CASE WHEN (dpi.res_status = 'deliver') THEN 1 ELSE 0 END)", 'full_delivered_bill_count');
    $query->addExpression("SUM(CASE WHEN (dpi.res_status = 'undeliver') THEN 1 ELSE 0 END)", 'full_returned_bill_count');
    $query->addExpression("SUM(CASE WHEN (dpi.res_status = 'partial_deliver') THEN 1 ELSE 0 END)", 'partial_delivered_bill_count');
    $res = $query->execute()->fetchAll(PDO::FETCH_UNIQUE | PDO::FETCH_ASSOC);

    $form_state['returns_confirmation']['bills'] = $res;
  }
  else {
    unset($form_state['returns_confirmation']['bills']);
  }

  $form_state['rebuild'] = TRUE;
}

function return_confirm_validate($form, $form_state) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);
  
  $get_trip_id = $form_state['triggering_element']['#name'];
  $exploded_name = explode('_', $get_trip_id);
  $trip_id = $exploded_name[2];
  
  // Validation of Total bill count should be equal to sum of Full delivered bill count + full returned bill count + partial delivered bill count
  $validation_on_bill_count = get_bill_count_validation($trip_id, $dis_id);

  $total_bill_count = $validation_on_bill_count['bill_count'];
  $sum_of_delivered_bills_count = ($validation_on_bill_count['full_delivered_bill_count'] + $validation_on_bill_count['full_returned_bill_count'] + $validation_on_bill_count['partial_delivered_bill_count']);

  if ($total_bill_count != $sum_of_delivered_bills_count) {
    form_set_error('form', "Please declare status for all bills in this Trip id:$trip_id.");
  }
  
  
  $query = db_select('dispatch_planning_api_data', 'ad');
  $query->join('dispatch_planning_item_details', 'dpi', 'ad.id = dpi.trip_bill_reference');
  $query->condition('ad.runsheet_number', $trip_id, '=');
  $query->condition('ad.distributor_id', $dis_id);
  $query->condition('ad.updated_flag', 2); //api

  $db_or = db_or();
  $db_or->condition('dpi.line_level_status', 'line_undeliver');
  $db_or->condition('dpi.line_level_status', 'line_partial_deliver');
  $query->condition($db_or);

  $query->addExpression("GROUP_CONCAT(DISTINCT dpi.product_name separator '<br>')", 'product_name');
  $res = $query->execute()->fetchField();
  if (!empty($res)) {
    //Temporary commented
    //form_set_error('form', "Please confirm SKU-wise Returns for Trip ID $trip_id and products<br>". $res); 
  }
}
/**
 * Submit function of modal pop up
 */
function return_confirm_submit($form, $form_state) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);

  $get_trip_id = $form_state['triggering_element']['#name'];
  $exploded_name = explode('_', $get_trip_id);
  $trip_id = $exploded_name[2];

  $trip_nid = check_trip_id_exists_or_not($dis_id, $trip_id);
  if (!empty($trip_nid->nid)) {
      $node = node_load($trip_nid->nid);
      // Status updated to 'Returned Confirm'
      $node->field_vrs_status['und'][0]['value'] = 1;

      $returns_version = $node->field_returned_version['und'][0]['value'];
      if (empty($returns_version)) {
        $version_no = 'V_1.0';
        $node->field_returned_version['und'][0]['value'] = $version_no;
      }
      // version increment
      else {
        $exploded_val = explode('_', $returns_version);
        $updated_version = $exploded_val[1] + 0.1;
        $version_no = "V_$updated_version";
        $node->field_returned_version['und'][0]['value'] = $version_no;
      }

      $node->field_returns_closure_id['und'][0]['value'] = "RC$trip_id";
      node_save($node);
      
      //Remove header if row count is 1
      $row_count = count($form_state['complete form']['bill_details']['#rows']);
      if($row_count == 1) {
        unset($form_state['complete form']['bill_details']['#header']);
      }
      //Remove row after confirmation
      unset($form_state['complete form']['bill_details']['#rows'][$trip_id]);

      $form_state['rebuild'] = TRUE;

      $query = db_select('dispatch_planning_api_data', 'ad');
      $query->condition('ad.runsheet_number', $trip_id);
      $query->condition('ad.distributor_id', $dis_id);
      $query->fields('ad');
      $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

      $bills_items = get_bill_items_details($trip_id, $dis_id);

      // add the header level data in new versioning header level table
      foreach ($res as $key => $val) {
        $query = db_insert('inventory_versioning_api_data');
        $cur_id = $val['id'];
        unset($val['id']);
        $val['version_created_time'] = REQUEST_TIME;
        $val['version_no'] = $version_no;
        $query->fields($val);
        $result_id = $query->execute();

        $fields = array(
          'tur',
          'mrp',
          'product_name',
          'sum_net_sales_cases',
          'sum_net_sales_quantity',
          'sku_code',
          'sum_net_sales_kgs',
          'sum_net_sales_value',
          'sku_description',
          'trip_bill_reference',
          'res_sku_actual_quantity1',
          'og_sum_net_sales_quantity',
          'res_sku_reason',
          'photo_of_signed_bill_partial',
          'updated_flag',
          'res_godown_short',
          'res_cashier_short',
          'dispatch_qty',
          'return_qty',
          'farey_delivered_qty',
          'money_collect_type',
          'line_level_status'
        );
        // add the child level data in new versioning child level table
        if (!empty($result_id)) {
          $query1 = db_insert('inventory_versioning_item_details')->fields($fields);
          foreach ($bills_items[$cur_id] as $value1) {
            unset($value1['id']);
            $value1['trip_bill_reference'] = $result_id;
            $query1->values($value1);
          }
          $query1->execute();
        }
      }

      drupal_set_message("Returns confirmation of trip id:$trip_id is done successfully.");
  }
  else {
    drupal_set_message('Trip id nid not found.');
  }
}

// Function to check the status of all bills of a trip is declared or not.
function get_bill_count_validation($trip_id, $dis_id) {
  $query = db_select('dispatch_planning_api_data', 'ad');
  $query->condition('ad.distributor_id', $dis_id);
  $query->condition('ad.runsheet_number', $trip_id, '=');
  $query->addExpression('COUNT(ad.id)', 'bill_count');
  $query->addExpression("SUM(CASE WHEN (ad.res_status = 'deliver') THEN 1 ELSE 0 END)", 'full_delivered_bill_count');
  $query->addExpression("SUM(CASE WHEN (ad.res_status = 'undeliver') THEN 1 ELSE 0 END)", 'full_returned_bill_count');
  $query->addExpression("SUM(CASE WHEN (ad.res_status = 'partial_deliver') THEN 1 ELSE 0 END)", 'partial_delivered_bill_count');
  $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

  return $res[0];
}

function get_bill_items_details($trip_id, $dis_id) {
  $query = db_select('dispatch_planning_api_data', 'ad');
  $query->condition('ad.runsheet_number', $trip_id);
  $query->condition('ad.distributor_id', $dis_id);
  $query->join('dispatch_planning_item_details', 'dpi', 'dpi.trip_bill_reference = ad.id');
  $query->fields('dpi');
  $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
  $bills_item = array();
  foreach ($res as $val) {
    $bills_item[$val['trip_bill_reference']][] = $val;
  }
  return $bills_item;
}
