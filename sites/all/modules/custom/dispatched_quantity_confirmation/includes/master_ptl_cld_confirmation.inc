<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * 
 * @param array $form
 * @param type $form_state
 * @return array
 */
module_load_include('inc', 'dispatched_quantity_confirmation', 'includes/dispatch_qty_common');
function master_ptl_cld_confirmation_form($form, &$form_state) {
  $form['#attached']['js'][] = drupal_get_path('module', 'dispatched_quantity_confirmation') . '/js/ptl_retail_cld_confirmation.js';
  $form['#attached']['css'][] = drupal_get_path('module', 'dispatched_quantity_confirmation') . '/css/dispatched_quantity_confirmation.css';
  
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);

  $form['#prefix'] = '<div id="itemlist-div">';
  $form['#suffix'] = '</div>';

  $form['trip_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Trip ID'),
    '#required' => TRUE,
    '#autocomplete_path' => 'get-all-trips/autocomplete',
    '#ajax' => array(
      'callback' => 'master_ptl_cld_callback',
      'wrapper' => 'itemlist-div',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );

  $trip_id = $form_state['input']['trip_number'];
  if (isset($form_state['triggering_element']['#title']) || !empty($form_state['input']['trip_number'])) {
    if ($form_state['triggering_element']['#title'] === 'Trip ID' && $form_state['entered_trip_id'] != $form_state['input']['trip_number']) {
      $form_state['master_ptl_cld_confirmation']['bills'] = array();
    }
    $form_state['entered_trip_id'] = $form_state['input']['trip_number'];

    $form['sku'] = array(
      '#type' => 'textfield',
      '#title' => t('SKU Code'),
      '#autocomplete_path' => "ptl-cld-skus/autocomplete/$trip_id",
    );
  }
  $form['apply'] = array(
    '#type' => 'submit',
    '#value' => 'Apply',
    '#ajax' => array(
      'callback' => 'master_ptl_cld_callback',
      'wrapper' => 'itemlist-div',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    '#validate' => array('trip_id_validate'),
    '#submit' => array('master_ptl_cld_confirmation_apply'),
  );


  $header = array(
    'SKU Code',
    'Product Name',
    'Original CLD Qty',
    'Confirmed CLD Qty',
    'CLD Short Reason',
    'To Be Dispatched Qty - CLD Qty'
  );

  if (!isset($form_state['master_ptl_cld_confirmation']['bills'])) {
    $form_state['master_ptl_cld_confirmation']['bills'] = array();
  }
  $options = $form_state['master_ptl_cld_confirmation']['bills'];

  if (empty($form_state['trip_no_error'])) {
    if (!empty($options)) {
      $form['bill_details'] = array(
        '#theme' => 'table',
        '#tree' => TRUE,
        '#header' => $header,
        '#rows' => array(),
        '#prefix' => '<div class="custom_middle_table">',
        '#suffix' => '</div>',
      );
      
      // Dispatch Supervisor
      $incharge_name_option = array('' => 'Select');
      $resource_type = 'Packing Auditor';
      $dispatch_supervisor_name_option = array();
      $dispatch_supervisor_options = get_ptl_supervisor_list($dis_id, $resource_type);
      if (!empty($dispatch_supervisor_options)) {
        $dispatch_supervisor_name_option = $incharge_name_option + $dispatch_supervisor_options;
        $field = 'field_packing_auditor_cld';
        $default_value_superviser = get_auditor_name($dis_id, $trip_id, $field);
      }
      $form['packing_auditor_name'] = array(
        '#title' => t('Packing Auditor Name'),
        '#required' => TRUE,
        '#type' => 'select',
        '#default_value' => $default_value_superviser,
        '#prefix' => '<div class="packing_auditor_outer"><div class="packing_auditor_name">',
        '#suffix' => '</div></div>',
        '#options' => $dispatch_supervisor_name_option,
        '#attributes' => array('class' => array('return-incharge-name')),
      );

      $form['save'] = array(
        '#type' => 'submit',
        '#value' => 'Save',
        '#ajax' => array(
          'callback' => 'master_ptl_cld_callback',
          'wrapper' => 'itemlist-div',
          'method' => 'replace',
          'effect' => 'fade',
        ),
        '#validate' => array('master_ptl_cld_confirmation_save_validate'),
        '#submit' => array('save_master_ptl_cld_confirmation_submit'),
      );

      $form['submit'] = array(
        '#type' => 'submit',
        '#value' => 'Close Trip',
        '#validate' => array('master_ptl_cld_confirmation_save_validate'),
        '#submit' => array('master_ptl_cld_confirmation_submit'),
      );
    }

    foreach ($options as $key => $val) {
      $form['bill_details']['#rows'][$key][] = $val['sku7_code'];
      $form['bill_details']['#rows'][$key][] = $val['product_name'];

      $form['bill_details']['#rows'][$key][] = array('data' => $val['cld'], 'class' => 'cld_qty_' . $key);

      // Fetch default value from "dispatch_qty_confirmation" table
      $get_qty = get_qty_default_values_cld($trip_id, $val['sku7_code'], $dis_id);

      $cld_confirmed_qty = array(
        '#type' => 'textfield',
        '#attributes' => array('bill_sku' => $val['sku7_code'], 'class' => array('cld-confirmed-qty')),
        '#size' => 10,
        '#default_value' => $get_qty['cld_confirmed_qty'] ? $get_qty['cld_confirmed_qty'] : ($get_qty['cld_short_reason'] ? 0 : ''),
      );

      $reason_options = array('' => 'Select');
      $dispatch_short_reason_options = get_short_reason_list($dis_id, 'CLD');
      if (!empty($dispatch_short_reason_options)) {
        $reason_options = $reason_options + $dispatch_short_reason_options;
      }
      $cld_short_reason = array(
        '#type' => 'select',
        '#options' => $reason_options,
        '#attributes' => array('class' => array('cld-short-reason_' . $key)),
        '#default_value' => $get_qty['cld_short_reason'],
      );

      $to_be_dispatched_qty = array(
        '#type' => 'textfield',
        '#attributes' => array('class' => array('to-be-dispatched-qty_' . $key), 'readonly' => 'readonly'),
        '#size' => 10,
        '#default_value' => $get_qty['dispatch_cld_qty'] ? $get_qty['dispatch_cld_qty'] : ($get_qty['cld_short_reason'] ? 0 : ''),
      );

      $form['bill_details'][$key]["cld_confirmed_qty"] = &$cld_confirmed_qty;
      $form['bill_details'][$key]["cld_short_reason"] = &$cld_short_reason;
      $form['bill_details'][$key]["to_be_dispatched_qty"] = &$to_be_dispatched_qty;

      $form['bill_details']['#rows'][$key][]['data']["cld_confirmed_qty"] = &$cld_confirmed_qty;
      $form['bill_details']['#rows'][$key][]['data']["cld_short_reason"] = &$cld_short_reason;
      $form['bill_details']['#rows'][$key][]['data']["to_be_dispatched_qty"] = &$to_be_dispatched_qty;

      unset($cld_confirmed_qty);
      unset($cld_short_reason);
      unset($to_be_dispatched_qty);
    }
  }
  return $form;
}

/**
 * 
 * @param type $form
 * @param type $form_state
 * @return array
 */
function master_ptl_cld_callback($form, &$form_state) {
  //return $form;
  $commands[] = ajax_command_replace('#itemlist-div', render($form));
  $commands[] = ajax_command_replace('.alert-danger', '');
  $commands[] = ajax_command_replace('.alert-success', '');
  $commands[] = ajax_command_prepend('#itemlist-div', theme('status_messages'));
  return array('#type' => 'ajax', '#commands' => $commands);
}

function trip_id_validate($form, &$form_state) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);

  $trip_number = $form_state['values']['trip_number'];
  $sku_code = $form_state['values']['sku'];
  $form_state['trip_no_error'] = '';
  if (!empty($trip_number)) {

    $type = get_trip_type($trip_number, $dis_id);
    if ($type != 'Master PTL') {
      drupal_set_message(t('Trip Id not present in Master PTL'), 'error');
      $form_state['trip_no_error'] = 1;
    }

    $bypass_flag = get_loose_cld_flag_trip_level($trip_number, $dis_id);
    if ($bypass_flag['cld'] == 1) {
      drupal_set_message(t("Trip ID $trip_number is already closed."), 'error');
      $form_state['trip_no_error'] = 1;
    }

    $query = db_select('node', 'n');
    $query->condition('n.title', $trip_number, '=');
    $query->condition('n.type', 'dispatch_data');
    $query->condition('n.status', 1);
    $query->leftjoin('field_data_field_dispatch_status', 'ds', 'ds.entity_id = n.nid');
    $db_or = db_or();
    $db_or->isNull('ds.field_dispatch_status_value');
    $db_or->condition('ds.field_dispatch_status_value', 0);
    $query->condition($db_or);
    $query->fields('n', array('nid'));
    $query->condition('n.uid', $dis_id);
    $result = $query->execute()->fetchField();
    if (empty($result)) {
      drupal_set_message(t('Please enter valid trip number.'), 'error');
      $form_state['trip_no_error'] = 1;
    }
    else {
      //validation for trip present in dispatch but cron is not run
      $query = db_select('dispatch_qty_confirmation', 'dqp');
      $query->fields('dqp', array('trip_id'));
      $query->condition('dqp.distributor_id', $dis_id);
      $query->condition('dqp.trip_id', $trip_number);
      $res = $query->execute()->fetchField();
      if (empty($res)) {
        drupal_set_message(t("Trip ID $trip_number is being processed, please try again in 4 mins"), 'error');
        $form_state['trip_no_error'] = 1;
      }
      
      $query = db_select('master_ptl_data', 'mpd');
      $query->condition('mpd.trip_id', $trip_number);
      $query->condition('mpd.distributor_id', $dis_id);
      $query->fields('mpd', array('trip_id'));
      $result = $query->execute()->fetchField();
      if (empty($result)) {
        drupal_set_message(t('Trip Id not present in Master PTL'), 'error');
        $form_state['trip_no_error'] = 1;
      }
      
      //check dp bills in trip
      $dp_bills = check_dp_bills_in_trip($result);
      if (!empty($dp_bills)) {
        drupal_set_message(t("Trip ID $trip_number contains DP Bills"), 'error');
        $form_state['trip_no_error'] = 1;
      }

      if (!empty($sku_code)) {
        $query = db_select('node', 'n');
        $query->condition('n.type', 'sku7_code_master');
        $query->condition('n.uid', $dis_id);
        $query->condition('n.title', $sku_code);
        $query->join('field_data_field_child_sku', 'csk', 'n.nid = csk.field_child_sku_nid');
        $query->fields('n', array('title'));
        $row = $query->execute()->fetchField();
        if (!empty($row)) {
          drupal_set_message(t("Entered sku $sku_code is child sku"), 'error');
          $form_state['trip_no_error'] = 1;
        }
      }

      $query = db_select('dispatch_qty_confirmation', 'dqc');
      $query->condition('dqc.trip_id', $trip_number, '=');
      $query->condition('dqc.distributor_id', $dis_id, '=');
      $query->fields('dqc', array('cld_retail_confirmed_flag'));
      $result1 = $query->execute()->fetchField();
      if (!empty($result1)) {
        //extra check for auto closed
        $query = db_select('dispatch_qty_confirmation', 'dqp');
        $query->fields('dqp', array('id'));
        $query->condition('dqp.distributor_id', $dis_id);
        $query->condition('dqp.trip_id', $trip_number);
        $query->condition('dqp.original_cld_qty', 0, '>');
        $original_cld_qty = $query->execute()->fetchField();
        if (empty($original_cld_qty)) {
          drupal_set_message(t("Trip ID $trip_number does not have any CLD quantities"), 'error');
        }
        else {
          drupal_set_message(t("Trip ID $trip_number is already closed."), 'error');
        }
        $form_state['trip_no_error'] = 1;
      }
    }
  }
}

function master_ptl_cld_confirmation_apply($form, &$form_state) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);
  $trip_number = $form_state['values']['trip_number'];
  $sku_code = $form_state['values']['sku'];
  $all_parent_sku = get_all_parent_cld_skus_query_wise($trip_number, $dis_id, $sku_code);
  $child_sku = get_child_cld_sku_query_wise($all_parent_sku, $dis_id);

  if (empty($form_state['trip_no_error'])) {
    $query = db_select('master_ptl_data', 'mpd');
    $query->condition('mpd.trip_id', $trip_number);
    $query->condition('mpd.distributor_id', $dis_id);
    if (!empty($sku_code)) {
      $query->condition('mpd.sku_code', $sku_code);
    }
    //exclude child sku by default
    if (!empty($child_sku['child_sku'])) {
      $query->condition('mpd.sku_code', $child_sku['child_sku'], 'NOT IN');
    }
    $query->condition('mpd.cld', 0, '>');
    $query->addExpression('mpd.sku_code', 'sku_code');
    $query->addExpression('mpd.sku_code', 'sku7_code');
    $query->addExpression('mpd.product_name', 'product_name');
    $query->addExpression('mpd.cld', 'cld');
//    $query->groupBy('mpd.sku_code');
    $result = $query->execute()->fetchAll(PDO::FETCH_GROUP | PDO::FETCH_UNIQUE | PDO::FETCH_ASSOC);
    $form_state['master_ptl_cld_confirmation']['bills'] = $result;
  }
  $form_state['rebuild'] = TRUE;
}

function master_ptl_cld_confirmation_save_validate($form, &$form_state) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);
  $trip_id = $form_state['values']['trip_number'];
  $form_state['values']['trip_type'] = get_trip_type($trip_id, $dis_id);
  foreach ($form_state['values']['bill_details'] as $key => $val) {
    if ($val['cld_confirmed_qty'] === '') {
      drupal_set_message(t("CLD confirmed qty cannot be empty for sku: $key."), 'error');
      $form_state['rebuild'] = TRUE;
    }
    if (!empty($val['cld_confirmed_qty']) || $val['cld_confirmed_qty'] === '0') {
      if (!is_numeric($val['cld_confirmed_qty'])) {
        drupal_set_message(t("CLD confirmed qty should be numeric for: $key."), 'error');
        $form_state['rebuild'] = TRUE;
      }
      $cld_confirmed_qty = $val['cld_confirmed_qty'];
      $original_cld_qty = $form_state['master_ptl_cld_confirmation']['bills'][$key]['cld'];
      if ($cld_confirmed_qty > $original_cld_qty) {
        drupal_set_message(t("CLD confirmed qty cannot be greater than original cld qty for: $key."), 'error');
        $form_state['rebuild'] = TRUE;
      }
      if ($cld_confirmed_qty != $original_cld_qty) {
        if (empty($val['cld_short_reason'])) {
          drupal_set_message(t("CLD short reason should be mandatory for: $key."), 'error');
          $form_state['rebuild'] = TRUE;
        }
      }
    }
    else {
      if ($form_state['triggering_element']['#value'] == 'Close Trip' && $form_state['values']['trip_type'] == 'non-ptl') {
        drupal_set_message(t("CLD confirmed qty is mandatory for: $key."), 'error');
        $form_state['rebuild'] = TRUE;
      }
    }
  }
}

function save_master_ptl_cld_confirmation_submit($form, &$form_state) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);
  $trip_id = $form_state['values']['trip_number'];
  $form_state['values']['trip_type'] = get_trip_type($trip_id, $dis_id);
  $type = $form_state['values']['trip_type'];
  foreach ($form_state['values']['bill_details'] as $key => $val) {
    //original cld qty value based on sequencing, if sequencing changed change this value also 
    $original_cld_qty = $form_state['master_ptl_cld_confirmation']['bills'][$key]['cld'];
    $cld_confirmed_qty = $form_state['values']['bill_details'][$key]['cld_confirmed_qty'];
    $cld_short_reason = ($original_cld_qty != $cld_confirmed_qty) ? $form_state['values']['bill_details'][$key]['cld_short_reason'] : '';
    $to_be_dispatched_qty = $form_state['values']['bill_details'][$key]['to_be_dispatched_qty'];
    $sku7_code = $form_state['master_ptl_cld_confirmation']['bills'][$key]['sku7_code'];

    if ($to_be_dispatched_qty !== '') {

      $cld_confirmed_qty = $cld_confirmed_qty ? $cld_confirmed_qty : 0;
      $to_be_dispatched_qty = $to_be_dispatched_qty ? $to_be_dispatched_qty : 0;
      //db_query("update dispatch_qty_confirmation set cld_confirmed_qty = $cld_confirmed_qty,cld_short_reason = '$cld_short_reason',dispatch_cld_qty = $to_be_dispatched_qty,type = '$type' where trip_id = $trip_id and sku_code = '$sku7_code' and original_cld_qty > 0 limit 1");           
      //update based on bills
      $bills = get_mptl_bills_from_trip_sku($trip_id, $sku7_code, $dis_id);

      foreach ($bills as $value) {
        if ($value->original_cld_qty < $cld_confirmed_qty && $value->original_cld_qty != 0) {
          db_update('dispatch_qty_confirmation')
            ->fields(array(
              'cld_confirmed_qty' => $value->original_cld_qty,
              'cld_short_reason' => $cld_short_reason ? $cld_short_reason : '',
              'dispatch_cld_qty' => $value->original_cld_qty,
              'type' => $type,
            ))
            ->condition('id', $value->id, '=')
            ->execute();

          $cld_confirmed_qty -= $value->original_cld_qty;
        }
        else {
          db_update('dispatch_qty_confirmation')
            ->fields(array(
              'cld_confirmed_qty' => $cld_confirmed_qty,
              'cld_short_reason' => $cld_short_reason ? $cld_short_reason : '',
              'dispatch_cld_qty' => $cld_confirmed_qty,
              'type' => $type,
            ))
            ->condition('id', $value->id, '=')
            ->execute();
          $cld_confirmed_qty = 0; // instead of break for remailining lines update 0 with reason
        }
      }
    }
  }
  $supervisor_resource_nid = $form_state['values']['packing_auditor_name'];
  $field = 'field_packing_auditor_cld';
  save_dispatch_qty_supervisor_name($trip_id,$supervisor_resource_nid,$dis_id,$field);
  drupal_set_message(t("Data saved successfully."));
  unset($form_state['master_ptl_cld_confirmation']['bills']);
  $form_state['rebuild'] = TRUE;
}

function master_ptl_cld_confirmation_submit($form, &$form_state) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);
  $trip_id = $form_state['values']['trip_number'];
  $form_state['values']['trip_type'] = get_trip_type($trip_id, $dis_id);
  $all_parent_sku = get_all_parent_cld_skus_query_wise($trip_id, $dis_id);
  $child_sku = get_child_cld_sku_query_wise($all_parent_sku, $dis_id);

  $query = db_select('dispatch_qty_confirmation', 'dqc');
  $query->condition('dqc.trip_id', $trip_id);
  $query->condition('dqc.distributor_id', $dis_id);
  if (!empty($child_sku['child_sku'])) {
    $query->condition('dqc.sku_code', $child_sku['child_sku'], 'NOT IN');
  }
  $query->addExpression('dqc.sku_code', 'sku_code');
  $query->addExpression('dqc.sku_code', 'sku7_code');
  $query->addExpression('dqc.product_name', 'product_name');
  $query->addExpression('SUM(dqc.original_cld_qty)', 'cld');
  $query->groupBy('dqc.sku_code');
  $result = $query->execute()->fetchAll(PDO::FETCH_GROUP | PDO::FETCH_UNIQUE | PDO::FETCH_ASSOC);

  $type = $form_state['values']['trip_type'];

  foreach ($result as $key => $val) {
    $sku7_code = $val['sku7_code'];
    $product_name = $val['product_name'];
    $cld = $val['cld'];
    //original cld qty value based on sequencing, if sequencing changed change this value also 
    $original_cld_qty = $form_state['master_ptl_cld_confirmation']['bills'][$key]['cld'];
    $cld_confirmed_qty = $form_state['values']['bill_details'][$key]['cld_confirmed_qty'];
    $cld_short_reason = ($original_cld_qty != $cld_confirmed_qty) ? $form_state['values']['bill_details'][$key]['cld_short_reason'] : '';
    $to_be_dispatched_qty = $form_state['values']['bill_details'][$key]['to_be_dispatched_qty'];

    if ($to_be_dispatched_qty !== '' && !is_null($to_be_dispatched_qty)) {
      //store at first line only
      $cld_confirmed_qty = $cld_confirmed_qty ? $cld_confirmed_qty : 0;
      $to_be_dispatched_qty = $to_be_dispatched_qty ? $to_be_dispatched_qty : 0;
      //db_query("update dispatch_qty_confirmation set cld_confirmed_qty = $cld_confirmed_qty,cld_short_reason = '$cld_short_reason',dispatch_cld_qty = $to_be_dispatched_qty,type = '$type' where trip_id = $trip_id and sku_code = '$sku7_code' and original_cld_qty > 0 limit 1");
      //update based on bills
      $bills = get_mptl_bills_from_trip_sku($trip_id, $sku7_code, $dis_id);
      foreach ($bills as $value) {
        if ($value->original_cld_qty < $cld_confirmed_qty && $value->original_cld_qty != 0) {
          db_update('dispatch_qty_confirmation')
            ->fields(array(
              'cld_confirmed_qty' => $value->original_cld_qty,
              'cld_short_reason' => $cld_short_reason ? $cld_short_reason : '',
              'dispatch_cld_qty' => $value->original_cld_qty,
              'type' => $type,
            ))
            ->condition('id', $value->id, '=')
            ->execute();

          $cld_confirmed_qty -= $value->original_cld_qty;
        }
        else {
          db_update('dispatch_qty_confirmation')
            ->fields(array(
              'cld_confirmed_qty' => $cld_confirmed_qty,
              'cld_short_reason' => $cld_short_reason ? $cld_short_reason : '',
              'dispatch_cld_qty' => $cld_confirmed_qty,
              'type' => $type,
            ))
            ->condition('id', $value->id, '=')
            ->execute();
          $cld_confirmed_qty = 0; // instead of break for remailining lines update 0 with reason
        }
      }
    }
  }

  $validate = check_mptl_validation_trip_data_cld($trip_id, $dis_id);

  if ($validate['flag'] != 1) {
    $skus = implode(',', $validate['sku']);
    drupal_set_message(t('Confirmed CLD Qty	is mandatory for ' . $skus), 'error');
    $form_state['rebuild'] = TRUE;
  }
  else {

    //cld confirmed flag update to 1 for cld confirmation
    db_update('dispatch_qty_confirmation')
      ->fields(array(
        'type' => $type,
        'cld_retail_confirmed_flag' => 1,
        'cld_close_flag' => 1,
        'cld_created_date' => REQUEST_TIME
      ))
      ->condition('trip_id', $trip_id)
      ->condition('distributor_id', $dis_id)
      ->execute();

    $flag = check_loose_cld_close_flag($trip_id, $dis_id);
    if ($flag['loose'] == 1 && $flag['cld'] == 1) {
      sync_mptl_loose_cld($trip_id, $dis_id);
      update_mptl_child_cld($trip_id, $dis_id, 'Master PTL');
    }

    db_update('dispatch_qty_confirmation') // Table name no longer needs {}
      ->expression('to_be_dispatch_qty', "dispatch_loose_qty + (dispatch_cld_qty * upc) + cld_loose_child_qty")
      ->condition('trip_id', $trip_id, '=')
      ->execute();
    //no short in a trip then do calculation close trip automatically 
    //needs to handle child in no short
    //assign_mptl_cld_qty_no_sort($trip_id, $type, $dis_id);
    $supervisor_resource_nid = $form_state['values']['packing_auditor_name'];
    $field = 'field_packing_auditor_cld';
    save_dispatch_qty_supervisor_name($trip_id,$supervisor_resource_nid,$dis_id,$field);
    drupal_set_message(t("Trip ID $trip_id - CLD Confirmation Completed"));
  }
}

function get_qty_default_values_cld($trip_id, $sku, $dis_id) {
  $query = db_select('dispatch_qty_confirmation', 'dc');
  $query->condition('dc.trip_id', $trip_id, '=');
  $query->condition('dc.sku_code', $sku, '=');
  $query->condition('dc.distributor_id', $dis_id, '=');
  //$query->condition('dc.original_cld_qty', 0, '>');
  //$query->fields('dc', array('cld_confirmed_qty', 'cld_short_reason', 'dispatch_cld_qty'));
  $query->addExpression('SUM(dc.cld_confirmed_qty)', 'cld_confirmed_qty');
  $query->addExpression('dc.cld_short_reason', 'cld_short_reason');
  $query->addExpression('SUM(dc.dispatch_cld_qty)', 'dispatch_cld_qty');
  $query->groupBy('dc.sku_code');
  $result = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

  return $result[0];
}

function check_mptl_validation_trip_data_cld($trip_id, $dis_id) {
  $all_parent_sku = get_all_parent_cld_skus_query_wise($trip_id, $dis_id);
  $child_sku = get_child_cld_sku_query_wise($all_parent_sku, $dis_id);

  $validation_pass['flag'] = 1;
  $query = db_select('dispatch_qty_confirmation', 'dc');
  $query->condition('dc.trip_id', $trip_id, '=');
  $query->condition('dc.distributor_id', $dis_id, '=');
  //$query->condition('dc.original_cld_qty', 0, '>');
  //below condition is added to exclude child sku in validation on close trip
  if (!empty($child_sku['child_sku'])) {
    $query->condition('dc.sku_code', $child_sku['child_sku'], 'NOT IN');
  }
  $query->addExpression('SUM(dc.cld_confirmed_qty)', 'cld_confirmed_qty');
  $query->addExpression('SUM(dc.original_qty)', 'original_qty');
  $query->groupBy('dc.sku_code');

  $query->fields('dc', array('sku_code', 'cld_short_reason','upc'));
  $result = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
  
  foreach ($result as $key => $value) {
    $original_qty = $value['original_qty'];
    $cld_confirmed_qty = $value['cld_confirmed_qty'];
    $cld_short_reason = $value['cld_short_reason'];
    $upc = $value['upc'];
    $original_cld_qty = (int)($original_qty / $upc);
    if ($cld_confirmed_qty != $original_cld_qty && empty($cld_short_reason)) {
      $validation_pass['flag'] = 0;
      $validation_pass['sku'][$key] = $value['sku_code'];
    }
  }
  return $validation_pass;
}

function get_mptl_bills_from_trip_sku($trip_id, $sku, $dis_id) {
  $query = db_select('dispatch_qty_confirmation', 'dqp');
  $query->fields('dqp', array('id', 'bill_number', 'original_cld_qty', 'upc'));
  $query->condition('dqp.distributor_id', $dis_id);
  $query->condition('dqp.trip_id', $trip_id);
  $query->condition('dqp.sku_code', $sku);
  //$query->condition('dqp.original_cld_qty', 0, '>');
  $query->orderBy('dqp.original_cld_qty');
  $bills = $query->execute()->fetchAll();
  return $bills;
}
