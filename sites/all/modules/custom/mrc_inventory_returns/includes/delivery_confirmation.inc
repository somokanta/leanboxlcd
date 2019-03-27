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
function delivery_confirmation_form($form, &$form_state) {
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
      'callback' => 'delivery_confirmation_callback',
      'wrapper' => 'itemlist-div',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    '#attributes' => array('class' => array('trip-submit')),
    '#submit' => array('delivery_confirmation_form_submit'),
  );

  $header = array('Trip ID', 'Dispatch Date', 'Cashier Name', 'Bill Count', 'Full Delivered Bill Count', 'Full Returned Bill Count', 'Partial Delivered Bill Count','Reattempt Bill Count','Pending Bill Count', 'Vehicle Arrival Date', 'Vehicle Arrival Time', 'Delivery Incharge Name','Asset Summary','Delivery Summary','Action');

  if (!isset($form_state['delivery_confirmation']['bills'])) {
    $form_state['delivery_confirmation']['bills'] = array();
  }
  $options = $form_state['delivery_confirmation']['bills'];

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

  //$form['bill_details']['#rows'] = $options;
  foreach ($options as $key => &$val) {
    $bill_count = $val['bill_count'];
    $full_delivered_bill_count = $val['full_delivered_bill_count'];
    $full_returned_bill_count = $val['full_returned_bill_count'];
    $partial_delivered_bill_count = $val['partial_delivered_bill_count'];
    $delivery_pending_bill_count = $val['delivery_pending_bill_count'];
    $vehicle_arrival_date_value = $val['vehicle_arrival_date'];
    $vehicle_arrival_time_value = $val['vehicle_arrival_time'];
    $delivery_incharge_name_value = $val['delivery_incharge_name'];
    //$pending_bill_count = array(
//      '#markup' => $bill_count - ($full_delivered_bill_count + $full_returned_bill_count + $partial_delivered_bill_count + $delivery_pending_bill_count),
//      '#name' => "pending_bill_count_$key"
//    );
    $pending_bill_count = $bill_count - ($full_delivered_bill_count + $full_returned_bill_count + $partial_delivered_bill_count + $delivery_pending_bill_count);
   
    $vehicle_arrival_date = array(
      '#type' => 'date_popup',
      '#date_label_position' => 'within',
      '#date_format' => 'Y-m-d',
      '#default_value' => !empty($vehicle_arrival_date_value) ? date('Y-m-d', $vehicle_arrival_date_value) : date('Y-m-d'),
    );

    $vehicle_arrival_time = array(
      '#type' => 'textfield',
      '#size' => 20,
      '#attributes' => array('class' => array('vehicle-arrival-time')),
      '#default_value' => $vehicle_arrival_time_value,
    );
    
    $delivery_incharge_name = array(
      '#type' => 'textfield',
       '#size' => 10,
      '#attributes' => array('class' => array('delivery-incharge-name')),
      '#default_value' => $delivery_incharge_name_value,
    );
    
    $link = l('Received Assets', "received-assets-screen?title=$key", array('html' => TRUE, 'attributes' => array('target' => '_blank')));
//    $asset_summary = array(
//      '#markup' => rawurldecode($link),
//      '#name' => "asset_summary_$key",
//    );
    $asset_summary = rawurldecode($link);
    
//    $delivery_summary_printout = array(
//      '#markup' => l(t('Delivery Summary'), "delivery-summary-popup/nojs/$key", array('attributes' => array('class' => 'ctools-use-modal'))),
//      '#name' => "delivery_summary_$key",
//        //'#attributes' => array('class' => array('full-return')),
//    );
    $delivery_summary_printout = l(t('Delivery Summary'), "delivery-summary-popup/nojs/$key", array('attributes' => array('class' => 'ctools-use-modal')));

    $confirm_btn = array(
      '#type' => 'submit',
      '#value' => t('Confirm'),
      '#ajax' => array(
        'callback' => 'delivery_confirmation_callback',
        'wrapper' => 'itemlist-div',
        'method' => 'replace',
        'effect' => 'fade',
      ),
      '#name' => "confirm_submit_$key",
      '#validate' => array('delivery_confirm_validate'),
      '#submit' => array('delivery_confirm_submit'),
      '#attributes' => array('class' => array('confirm-remove')),
    );
    $form['bill_details']['#rows'][$key][] = $val['n_title'];
    $form['bill_details']['#rows'][$key][] = $val['field_dispatch_date_value'];
    $form['bill_details']['#rows'][$key][] = $val['field_cashier_name_value'];
    $form['bill_details']['#rows'][$key][] = $val['bill_count'];
    $form['bill_details']['#rows'][$key][] = $val['full_delivered_bill_count'];
    $form['bill_details']['#rows'][$key][] = $val['full_returned_bill_count'];
    $form['bill_details']['#rows'][$key][] = $val['partial_delivered_bill_count'];
    $form['bill_details']['#rows'][$key][] = $val['delivery_pending_bill_count'];
    $form['bill_details']['#rows'][$key][] = $pending_bill_count;

    //$form['bill_details'][$key]["pending_bill_count_{$key}"] = &$pending_bill_count;
    $form['bill_details'][$key]["vehicle_arrival_date"] = &$vehicle_arrival_date;
    $form['bill_details'][$key]["vehicle_arrival_time"] = &$vehicle_arrival_time;
    $form['bill_details'][$key]["delivery_incharge_name"] = &$delivery_incharge_name;
    //$form['bill_details'][$key]["asset_summary_{$key}"] = &$asset_summary;
    //$form['bill_details'][$key]["delivery_summary_{$key}"] = &$delivery_summary_printout;
    $form['bill_details'][$key]["confirm_submit"] = &$confirm_btn;
    
    
    $form['bill_details']['#rows'][$key][]['data']["vehicle_arrival_date"] = &$vehicle_arrival_date;
    $form['bill_details']['#rows'][$key][]['data']["vehicle_arrival_time"] = &$vehicle_arrival_time;
    $form['bill_details']['#rows'][$key][]['data']["delivery_incharge_name"] = &$delivery_incharge_name;
    $form['bill_details']['#rows'][$key][] = $asset_summary;
    $form['bill_details']['#rows'][$key][] = $delivery_summary_printout;
    $form['bill_details']['#rows'][$key][]['data']["confirm_btn"] = &$confirm_btn;

    //unset($pending_bill_count);
    unset($vehicle_arrival_date);
    unset($vehicle_arrival_time);
    unset($delivery_incharge_name);
    //unset($asset_summary);
    //unset($delivery_summary_printout);
    unset($confirm_btn);
  }
  return $form;
}

function delivery_confirmation_callback($form, &$form_state) {
  return $form;
}

/**
 * 
 * @param array $form
 * @param array $form_state
 * @return array
 */
function delivery_confirmation_form_validate($form, &$form_state) {
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
    if ($check_returns_flag->field_vrs_status_value >= 0) {
      unset($form_state['delivery_confirmation']['bills']);
      drupal_set_message(t("This Trip status is already delivery confirmed."), 'error');
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
function delivery_confirmation_form_submit($form, &$form_state) {
  if ($form_state['error_flag'] != 1) {
    global $user;
    $dis_id = change_feeds_import_get_distributor_id($user->uid);

    $trip_id = $form_state['values']['trip_id'];

    $dispatch_from_date = $form_state['values']['dispatch_from_date'];
    $dispatch_to_date = $form_state['values']['dispatch_to_date'];

    $cashier_name = $form_state['values']['cashier_name'];

    $query = db_select('node', 'n');
    $query->leftjoin('field_data_field_vehicle_arrival_date', 'vad', 'vad.entity_id = n.nid');
    $query->leftjoin('field_data_field_vehicle_arrival_time', 'vat', 'vat.entity_id = n.nid');
    $query->leftjoin('field_data_field_delivery_incharge_name', 'din', 'din.entity_id = n.nid');
    $query->condition('n.type', 'dispatch_data', '=');
    $query->condition('n.uid', $dis_id, '=');
    $query->leftjoin('field_data_field_vrs_status', 'vs', 'vs.entity_id = n.nid');
    $query->condition('vs.field_vrs_status_value', '-1', '=');
    
    if (!empty($trip_id)) {
      $query->condition('n.title', $trip_id, '=');
    }

    $query->leftjoin('field_data_field_dispatch_date', 'disp_date', 'disp_date.entity_id = n.nid');
    if (!empty($dispatch_from_date)) {
      $query->condition('disp_date.field_dispatch_date_value', array($dispatch_from_date, $dispatch_to_date), 'BETWEEN');
    }

    $query->leftjoin('field_data_field_cashier_name', 'cn', 'cn.entity_id = n.nid');
    if (!empty($cashier_name)) {
      $query->condition('cn.field_cashier_name_value', $cashier_name, '=');
    }

    $query->join('dispatch_planning_api_data', 'dpi', "dpi.runsheet_number = n.title and dpi.distributor_id = $dis_id ");
    $query->groupBy('n.title');

    $query->fields('n', array('title', 'title'));
    $query->addExpression("DATE_FORMAT(disp_date.field_dispatch_date_value, '%d-%b-%Y')", 'field_dispatch_date_value');
    $query->addExpression('cn.field_cashier_name_value', 'field_cashier_name_value');
    $query->addExpression('COUNT(dpi.bill_number)', 'bill_count');
    $query->addExpression("SUM(CASE WHEN (dpi.res_status = 'deliver') THEN 1 ELSE 0 END)", 'full_delivered_bill_count');
    $query->addExpression("SUM(CASE WHEN (dpi.res_status = 'undeliver' and dpi.delivery_pending_flag != 1) THEN 1 ELSE 0 END)", 'full_returned_bill_count');
    $query->addExpression("SUM(CASE WHEN (dpi.res_status = 'partial_deliver') THEN 1 ELSE 0 END)", 'partial_delivered_bill_count');
    $query->addExpression("SUM(CASE WHEN (dpi.delivery_pending_flag = 1) THEN 1 ELSE 0 END)", 'delivery_pending_bill_count');
    $query->addExpression('vad.field_vehicle_arrival_date_value', 'vehicle_arrival_date');
    $query->addExpression('vat.field_vehicle_arrival_time_value', 'vehicle_arrival_time');
    $query->addExpression('din.field_delivery_incharge_name_value', 'delivery_incharge_name');
    $res = $query->execute()->fetchAll(PDO::FETCH_UNIQUE | PDO::FETCH_ASSOC);
    
    $form_state['delivery_confirmation']['bills'] = $res;
  }
  else {
    unset($form_state['delivery_confirmation']['bills']);
  }

  $form_state['rebuild'] = TRUE;
}


function delivery_confirm_validate(&$form, &$form_state) {
  $mess = '';
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);
  
  $get_trip_id = $form_state['triggering_element']['#name'];
  $exploded_name = explode('_', $get_trip_id);
  $trip_id = $exploded_name[2];
  
  // Validation of Total bill count should be equal to sum of Full delivered bill count + full returned bill count + partial delivered bill count + delivery pending bill count
  $validation_on_bill_count = get_bill_count_validation($trip_id, $dis_id);

  $total_bill_count = $validation_on_bill_count['bill_count'];
  $sum_of_delivered_bills_count = ($validation_on_bill_count['full_delivered_bill_count'] + $validation_on_bill_count['full_returned_bill_count'] + $validation_on_bill_count['partial_delivered_bill_count'] + $validation_on_bill_count['delivery_pending_bill_count']);

  if ($total_bill_count != $sum_of_delivered_bills_count) {
    $form_state['rebuild']  = TRUE;
    form_set_error('form', "Please declare status for all bills in this Trip id:$trip_id.");
  }
  
  if(empty($form_state['input']['bill_details'][$trip_id]['delivery_incharge_name'])) {
    $form_state['rebuild']  = TRUE;
    form_set_error('form', "Please enter delivery incharge name for Trip id:$trip_id.");
  }
  
  if (empty($form_state['values']['bill_details'][$trip_id]['vehicle_arrival_date'])) {
    $mess .= t('Vehicle Arrival Date field is required for Trip ID:' . ' ' . $trip_id) . '<br>';
  }

  if (empty($form_state['values']['bill_details'][$trip_id]['vehicle_arrival_time'])) {
    $mess .= t('Vehicle Arrival Time field is required for Trip ID:' . ' ' . $trip_id) . '<br>';
  }
  
  // Validate Vehicle Arrival Time.
  $vehicle_arrival_date = trim($form_state['values']['bill_details'][$trip_id]['vehicle_arrival_date']);
  $vehicle_arrival_date1 = explode(" ", $vehicle_arrival_date);
  $vehicle_arrival_date = strtotime($vehicle_arrival_date1[0]);
  $vehicle_time = trim($form_state['values']['bill_details'][$trip_id]['vehicle_arrival_time']);
  
  $hrs = explode(':', trim($vehicle_time))[0];
  if($hrs > 12){ 
    $time_in_24hrs = explode(' ', trim($vehicle_time))[0];
    $final_time  = date("g:i a", strtotime($time_in_24hrs));
  }else{
    $final_time = $vehicle_time;
  }
  $vehicle_arrival_time = strtotime($vehicle_arrival_date1[0] . ' '. $final_time);

  $start_time_flag = 0;
  $current_time = strtotime(date('Y-m-d h:i a'));

  if (!empty($vehicle_time)) {
      $pre_start_time = explode(':', $vehicle_time);
      if (count($pre_start_time) != 2) {
        $start_time_flag = 1;
      }
      if (!empty($pre_start_time)) {
        $hour = (int) $pre_start_time[0];
        $minutes_array = explode(' ', $pre_start_time[1]);
        $minutes = (int) $minutes_array[0];
        $am_pm = strtolower(trim($minutes_array[1]));
      }
      if ($hour > 24 || !is_numeric($hour) || $hour <= 0) {
        $start_time_flag = 1;
      }
      if ($minutes > 60 && is_numeric($minutes)) {
        $start_time_flag = 1;
      }
      if ($minutes < 0 && !is_numeric($minutes)) {
        $start_time_flag = 1;
      }
      if ($minutes < 0) {
        $start_time_flag = 1;
      }
      if ($am_pm != 'am' && $am_pm != 'pm') {
        $start_time_flag = 1;
      }
    }

  if ($start_time_flag == 1) {
  $form_state['rebuild']  = TRUE;
    $mess .= t('Please enter valid vehicle reporting time for Trip ID:' . $trip_id . '.Allowed format is hh:mm AM/PM') . '<br>';
  }

  if ($vehicle_arrival_time > $current_time) {
  $form_state['rebuild']  = TRUE;
    $mess .= t('Vehicle Arrival Time cannot be greater than System Time for Trip ID:' . ' ' . $trip_id) . '<br>';
  }

  if (!empty($mess)) {
    form_set_error('form', $mess);
  }

  $query = db_select('node', 'n');
  $query->condition('n.type', 'assets_count');
  $query->condition('n.uid', $dis_id);
  $query->leftjoin('field_data_field_received_asset_time', 'at', 'at.entity_id = n.nid');
  $query->condition('n.title', $trip_id);
  $query->fields('at', array('field_received_asset_time_value'));
  $res = $query->execute()->fetchField();
  if(empty($res)) {
    $form_state['rebuild']  = TRUE;
    form_set_error('form', "Please confirm Received Assets first for Trip ID: $trip_id");
  }
  
}

/**
 * Submit function of modal pop up
 */
function delivery_confirm_submit($form, $form_state) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);

  $get_trip_id = $form_state['triggering_element']['#name'];
  $exploded_name = explode('_', $get_trip_id);
  $trip_id = $exploded_name[2];
  $vehicle_arrival_date = $form_state['values']['bill_details'][$trip_id]['vehicle_arrival_date'];
  $vehicle_arrival_time = $form_state['values']['bill_details'][$trip_id]['vehicle_arrival_time'];
  $delivery_incharge_name = $form_state['values']['bill_details'][$trip_id]['delivery_incharge_name'];

  $hrs = explode(':', trim($vehicle_arrival_time))[0];
  if($hrs>12){
    $time_in_24hrs = explode(' ', trim($vehicle_arrival_time))[0];
    $vehicle_arrival_time  = date("g:i a", strtotime($time_in_24hrs));
  }
  $trip_nid = check_trip_id_exists_or_not($dis_id, $trip_id);
  if (!empty($trip_nid->nid)) {
      $node = node_load($trip_nid->nid);
      // Status updated to 'Returns Confirmation Pending'
      $node->field_vrs_status['und'][0]['value'] = '0';

      $delivery_version = $node->field_delivery_version['und'][0]['value'];
      if (empty($delivery_version)) {
        $version_no = 'V_1.0';
        $node->field_delivery_version['und'][0]['value'] = $version_no;
      }
      // version increment
      else {
        $exploded_val = explode('_', $delivery_version);
        $updated_version = $exploded_val[1] + 0.1;
        $version_no = "V_$updated_version";
        $node->field_delivery_version['und'][0]['value'] = $version_no;
      }

      $node->field_vehicle_arrival_date['und'][0]['value'] = strtotime($vehicle_arrival_date);
      $node->field_vehicle_arrival_time['und'][0]['value'] = $vehicle_arrival_time;
      $node->field_delivery_incharge_name['und'][0]['value'] = $delivery_incharge_name;
      node_save($node);
      
      // Insert the Delivery Incharge Name in Fareye Table
      db_update('dispatch_planning_api_data') // Table name no longer needs {}
          ->fields(array(
            'delivery_incharge_name' => $delivery_incharge_name,
          ))
          ->condition('runsheet_number', $trip_id, '=')
          ->condition('distributor_id', $dis_id, '=')
          ->execute();
      
      //Remove header if row count is 1
      $row_count = count($form_state['complete form']['bill_details']['#rows']);
      if($row_count == 1) {
        unset($form_state['complete form']['bill_details']['#header']);
      }
      //Remove row after confirmation
      unset($form_state['complete form']['bill_details']['#rows'][$trip_id]);
      unset($form_state['complete form']['bill_details'][$trip_id]);
      
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
        $val['version_flag'] = 1;
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

      drupal_set_message("Delivery confirmation of trip id:$trip_id is done successfully.");
  }
  else {
    drupal_set_message('Trip id nid not found.');
  }
}