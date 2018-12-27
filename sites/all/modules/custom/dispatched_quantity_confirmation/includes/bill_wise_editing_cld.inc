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
function bill_wise_editing_cld_form($form, &$form_state) {
  $form['#attached']['js'][] = drupal_get_path('module', 'dispatched_quantity_confirmation') . '/js/bill_wise_editing_cld.js';
  $form['#attached']['css'][] = drupal_get_path('module', 'dispatched_quantity_confirmation') . '/css/dispatched_quantity_confirmation.css';
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);

  $form['#prefix'] = '<div id="itemlist-div">';
  $form['#suffix'] = '</div>';

  $form['trip_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Trip ID'),
    '#required' => TRUE,
    '#ajax' => array(
      'callback' => 'bill_wise_editing_cld_callback',
      'wrapper' => 'itemlist-div',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    '#autocomplete_path' => 'get-all-trips/autocomplete',
  );
  $trip_number = $form_state['input']['trip_number'];

  if ($form_state['triggering_element']['#title'] === 'Trip ID' && $form_state['entered_trip_id'] != $form_state['input']['trip_number']) {
    $form_state['bill_wise_editing_cld']['skus'] = array();
  }
  $form_state['entered_trip_id'] = $form_state['input']['trip_number'];

  $form['bill_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Bill Number'),
    '#autocomplete_path' => "ptl-retail-cld-bills/autocomplete/$trip_number",
    '#ajax' => array(
      'callback' => 'bill_wise_editing_cld_callback',
      'wrapper' => 'itemlist-div',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    //'#required' => TRUE,
  );
  $bill_number = $form_state['input']['bill_number'];

  $form['sku_code'] = array(
    '#type' => 'textfield',
    '#title' => t('SKU Code'),
    //'#required' => TRUE,
    '#autocomplete_path' => "ptl-cld-skus/autocomplete/$trip_number",
  );
  $sku_code = $form_state['input']['sku_code'];

  $form['apply'] = array(
    '#type' => 'submit',
    '#value' => 'Apply',
    '#ajax' => array(
      'callback' => 'bill_wise_editing_cld_callback',
      'wrapper' => 'itemlist-div',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    '#validate' => array('trip_id_validate'),
    '#submit' => array('bill_wise_editing_cld_submit'),
  );

  $header = array('SKU Code',
    'Product Name',
    'Original CLD Qty',
    'Confirmed CLD Qty',
    'CLD Short Reason',
    'Bill Numbers');

  if (!isset($form_state['bill_wise_editing_cld']['skus'])) {
    $form_state['bill_wise_editing_cld']['skus'] = array();
  }
  $options = $form_state['bill_wise_editing_cld']['skus'];

  if (empty($form_state['trip_no_error'])) {
    if (!empty($options)) {
      $form['bill_details'] = array(
        '#theme' => 'table',
        '#tree' => TRUE,
        '#header' => $header,
        '#rows' => $options,
        '#prefix' => '<div class="custom_middle_table">',
        '#suffix' => '</div>',
      );
                  // Dispatch Supervisor
      $incharge_name_option = array('' => 'Select');
      $resource_type = 'Bill Editor';
      $dispatch_supervisor_name_option = array();
      $dispatch_supervisor_options = get_ptl_supervisor_list($dis_id, $resource_type);
      if (!empty($dispatch_supervisor_options)) {
        $dispatch_supervisor_name_option = $incharge_name_option + $dispatch_supervisor_options;
        $field = 'field_bill_cld_confirm_resource';
        $default_value_superviser = get_auditor_name($dis_id,$trip_number,$field);
      }
      $form['bill_editor'] = array(
        '#title' => t('Edited By*'),
        '#type' => 'select',
        '#prefix'=>'<div class="packing_auditor_outer"><div class="packing_auditor_name">',
        '#suffix'=>'</div></div>',
        '#default_value'=>$default_value_superviser,
        '#options' => $dispatch_supervisor_name_option,
      ); 
      $form['save'] = array(
        '#type' => 'submit',
        '#value' => 'Save',
        '#validate' => array('billwise_editing_cld_validate'),
        '#submit' => array('billwise_save_submit'),
      );

      $form['submit'] = array(
        '#type' => 'submit',
        '#value' => 'Submit',
        '#validate' => array('billwise_editing_cld_validate'),
        '#submit' => array('billwise_final_submit'),
      );
    }

    $header2 = array('Bill Number', 'Party Name', 'Original CLD Qty', 'Confirmed CLD Qty', 'CLD Short Reason', 'To Be Dispatched CLD Qty');

    foreach ($options as $key => $val) {
      $skus = $val['sku7_code'];
      $form['bill_details']['#rows'][$key][] = array('data' => l('Show Bills', 'javascript:', array('external' => TRUE)), 'class' => 'show-bills', 'sku_code' => $val['sku7_code']);


      //Popup part
      $form["sku_bill_table_$skus"] = array(
        '#theme' => 'table',
        '#tree' => TRUE,
        '#header' => $header2,
        '#rows' => array(),
        '#sticky' => FALSE,
        '#caption' => "<b>Product Name: </b>" . $val['product_name'],
        '#prefix' => '<div id ="pop_up_bill_' . $skus . '" class="pop-up_table" style="display:none;"><div class="popup_overlay"></div><div class="popup_wrap"><div class="pop_content"><div class="popup_border">'
        . '<img sku_key="' . $skus . '" class="close-pop-up-button" id="close_pop_up_bill_' . $skus . '" src="sites/all/modules/custom/mrc_inventory_returns/images/close_btn.png">',
        '#suffix' => '</div></div></div></div>',
      );
//
      $popup_bill_wise = get_bill_wise_cld_pop_up_table($trip_number, $skus, $dis_id);
//
      foreach ($popup_bill_wise as $val1) {

        $get_qty = get_qty_default_values_confirmed($trip_number, $val1['bill_number'], $skus, $dis_id);

        $form["sku_bill_table_$skus"]['#rows'][$val1['bill_number']]['bill_number'] = $val1['bill_number'];
        $form["sku_bill_table_$skus"]['#rows'][$val1['bill_number']]['field_party_name_value'] = $val1['field_party_name_value'];
        $form["sku_bill_table_$skus"]['#rows'][$val1['bill_number']]['original_cld_qty'] = array('data' => $val1['original_cld_qty'], 'class' => 'og_qty');
        $cld_confirmed_qty = array(
          '#type' => 'textfield',
          '#attributes' => array('class' => array('cld-confirmed-qty')),
          '#size' => 10,
          '#default_value' => $get_qty['cld_confirmed_qty'] ? $get_qty['cld_confirmed_qty'] : ($get_qty['cld_short_reason'] ? 0 : ''),
        );
        $form["sku_bill_table_$skus"][$val1['bill_number']]["cld_confirmed_qty"] = &$cld_confirmed_qty;
        $form["sku_bill_table_$skus"]['#rows'][$val1['bill_number']][]['data']["cld_confirmed_qty"] = &$cld_confirmed_qty;
//
        $reason_options = array('' => 'Select');
        $dispatch_short_reason_options = get_short_reason_list($dis_id, 'CLD');
        if (!empty($dispatch_short_reason_options)) {
          $reason_options = $reason_options + $dispatch_short_reason_options;
        }
        $cld_short_reason = array(
          '#type' => 'select',
          '#options' => $reason_options,
          '#attributes' => array('class' => array('cld-short-reason')),
          '#default_value' => $get_qty['cld_short_reason'],
        );
        $form["sku_bill_table_$skus"][$val1['bill_number']]["cld_short_reason"] = &$cld_short_reason;
        $form["sku_bill_table_$skus"]['#rows'][$val1['bill_number']]["cld_short_reason"]['data']["cld_short_reason"] = &$cld_short_reason;
        $to_be_dispatch = array(
          '#type' => 'textfield',
          '#attributes' => array('class' => array('to-be-dispatch-qty'), 'readonly' => 'readonly'),
          '#size' => 10,
          '#default_value' => $get_qty['dispatch_cld_qty'] ? $get_qty['dispatch_cld_qty'] : ($get_qty['cld_short_reason'] ? 0 : ''),
        );
        $form["sku_bill_table_$skus"][$val1['bill_number']]["to_be_dispatch"] = &$to_be_dispatch;
        $form["sku_bill_table_$skus"]['#rows'][$val1['bill_number']]["to_be_dispatch"]['data']["to_be_dispatch"] = &$to_be_dispatch;
//
        unset($original_cld_qty);
        unset($cld_confirmed_qty);
        unset($cld_short_reason);
        unset($to_be_dispatch);
      }

      //Popup part
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
function bill_wise_editing_cld_callback($form, &$form_state) {
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
  $sku_code = $form_state['values']['sku_code'];
  $form_state['values']['trip_type'] = get_trip_type($trip_number, $dis_id);
  $type = $form_state['values']['trip_type'];
  
  if ($type == 'Master PTL') {
    $link = l('Master PTL CLD confirmation', '/master-ptl-cld-confirmation',array('attributes'=>array("target" => "_blank")));
    drupal_set_message(t('Trip Id '.$trip_number.' is of type Master PTL. Please go to '.$link), 'warning');
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
    drupal_set_message(t('Please enter valid Trip ID.'), 'error');
    $form_state['trip_no_error'] = 1;
  }
  else {
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
    $query->fields('dqc', array('cld_close_flag'));
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

  $query = db_select('dispatch_qty_confirmation', 'mpd');
  $query->condition('mpd.trip_id', $trip_number);
  $query->condition('mpd.distributor_id', $dis_id);
  $query->fields('mpd', array('cld_retail_confirmed_flag'));
  $result = $query->execute()->fetchField();
  if (empty($result)) {
    drupal_set_message(t('CLD confirmation is pending for trip id ' . $trip_number), 'error');
    $form_state['trip_no_error'] = 1;
  }
  if ($type == 'Master PTL') {
    drupal_set_message(t("Trip ID $trip_number entered is of type Master PTL"), 'warning');
    $form_state['trip_no_error'] = 1;
  }
}

function bill_wise_editing_cld_submit($form, &$form_state) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);
  $trip_number = $form_state['values']['trip_number'];
  $sku_code = $form_state['values']['sku_code'];
  $bill_number = $form_state['values']['bill_number'];
  $all_parent_sku = get_all_parent_cld_skus_query_wise($trip_number, $dis_id, $bill_number, $sku_code);
  $child_sku = get_child_cld_sku_query_wise($all_parent_sku, $dis_id);

  if (empty($form_state['trip_no_error'])) {
    $query = db_select('dispatch_qty_confirmation', 'dqc');
    $query->condition('dqc.trip_id', $trip_number);
    $query->condition('dqc.distributor_id', $dis_id);
    if (!empty($sku_code)) {
      $query->condition('dqc.sku_code', $sku_code);
    }
    if (!empty($bill_number)) {
      $query->condition('dqc.bill_number', $bill_number);
    }
    //exclude child sku by default
    if (!empty($child_sku['child_sku'])) {
      $query->condition('dqc.sku_code', $child_sku['child_sku'], 'NOT IN');
    }
    $query->condition('dqc.original_cld_qty', 0, '>');
    $query->addExpression('dqc.sku_code', 'sku_code');
    $query->addExpression('dqc.sku_code', 'sku7_code');
    $query->addExpression('dqc.product_name', 'product_name');
    $query->addExpression('SUM(dqc.original_cld_qty)', 'cld');
    $query->addExpression('SUM(dqc.cld_confirmed_qty)', 'con_cld');
    $query->addExpression("GROUP_CONCAT(DISTINCT(if (dqc.cld_short_reason='', null, dqc.cld_short_reason)))", 'cld_short_reason');
    $query->addExpression('SUM(dqc.original_cld_qty) - SUM(dqc.cld_confirmed_qty)', 'final_qty');
    $query->havingCondition('final_qty', 0, '>');
    $query->groupBy('dqc.sku_code');
    $result = $query->execute()->fetchAll(PDO::FETCH_GROUP | PDO::FETCH_UNIQUE | PDO::FETCH_ASSOC);
    foreach ($result as $key => &$value) {
      unset($value['final_qty']);
    }
    $form_state['bill_wise_editing_cld']['skus'] = $result;
    if (empty($result)) {
      drupal_set_message(t("Trip ID $trip_number does not have any SKUs with CLD Short and therefore has been closed"), 'error');
    }
  }
  $form_state['rebuild'] = TRUE;
}

function billwise_editing_cld_validate($form, &$form_state) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);
  $trip_id = $form_state['values']['trip_number'];
  foreach ($form_state['bill_wise_editing_cld']['skus'] as $key => $value) {
    $con_cld = $value['con_cld'];
    $cld_confirmed_qty_sum = 0;
    foreach ($form_state['input']["sku_bill_table_" . $value['sku7_code']] as $bill => $val) {
      if ($val['cld_confirmed_qty'] === '') {
        drupal_set_message(t("CLD confirmed qty cannot be empty for bill $bill and sku: $key."), 'error');
        $form_state['rebuild'] = TRUE;
      }
      if (!empty($val['cld_confirmed_qty']) || $val['cld_confirmed_qty'] === '0') {
        if (!is_numeric($val['cld_confirmed_qty'])) {
          drupal_set_message(t("CLD confirmed qty should be numeric for: $key."), 'error');
          $form_state['rebuild'] = TRUE;
        }
        $cld_confirmed_qty = $val['cld_confirmed_qty'];
        $original_cld_qty = $form["sku_bill_table_" . $value['sku7_code']]['#rows'][$bill]['original_cld_qty']['data'];
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
        if (!empty($val['cld_confirmed_qty'])) {
          $cld_confirmed_qty_sum += $val['cld_confirmed_qty'];
        }
      }
    }
    if ($con_cld != $cld_confirmed_qty_sum && $cld_confirmed_qty_sum != 0) {
      drupal_set_message(t("Confirmed Qty is not equal to Sum of confirmed qty for sku: " . $value['sku7_code']), 'error');
      $form_state['rebuild'] = TRUE;
    }
  }
  if(empty($form_state['values']['bill_editor'])) {
      drupal_set_message(t('Edited By field is required'), 'error');
      $form_state['rebuild'] = TRUE;
  }
}

function billwise_save_submit($form, &$form_state) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);
  $trip_id = $form_state['values']['trip_number'];
  $form_state['values']['trip_type'] = get_trip_type($trip_id, $dis_id);
  $type = $form_state['values']['trip_type'];

  foreach ($form_state['bill_wise_editing_cld']['skus'] as $key => $value) {
    $parent_upc = get_sku_unit_case($value['sku7_code'], $dis_id);
    $child_sku = get_child_detials_by_parent_sku($value['sku7_code'], $dis_id);
    $child_ratio = $child_sku['child_qty'];
    $child_upc = get_sku_unit_case($child_sku['child_sku'], $dis_id);

    foreach ($form_state['input']["sku_bill_table_" . $value['sku7_code']] as $bill => $val) {
      $original_cld_qty = $form["sku_bill_table_" . $value['sku7_code']]['#rows'][$bill]['original_cld_qty']['data'];
      $cld_confirmed_qty = $val['cld_confirmed_qty'];
      $cld_short_reason = $val['cld_short_reason'];
      $to_be_dispatched_qty = $val['to_be_dispatch'];

      if ($to_be_dispatched_qty !== '') {
        db_update('dispatch_qty_confirmation')
          ->fields(array(
            'cld_confirmed_qty' => (!empty($cld_confirmed_qty)) ? $cld_confirmed_qty : 0,
            'cld_short_reason' => ($cld_confirmed_qty != $original_cld_qty) ? $cld_short_reason : '',
            'dispatch_cld_qty' => (!empty($to_be_dispatched_qty) || $to_be_dispatched_qty === '0') ? $to_be_dispatched_qty : $cld_confirmed_qty,
            'type' => $type,
          ))
          ->condition('trip_id', $trip_id, '=')
          ->condition('sku_code', $value['sku7_code'], '=')
          ->condition('bill_number', $bill, '=')
          ->execute();

//         if ($child_sku) {
//              $parent_qty = $cld_confirmed_qty * $parent_upc;
//              $child_total_qty = $parent_qty * $child_ratio;
//              $child_cld = (int) ($child_total_qty / $child_upc);
//              $child_loose = (int) ($child_total_qty % $child_upc);
//              db_update('dispatch_qty_confirmation')
//                ->fields(array(
//                  'cld_confirmed_qty' => $child_cld,
//                  'cld_loose_child_qty' => $child_loose,
//                  'cld_short_reason' => $cld_short_reason ? $cld_short_reason : '',
//                  'dispatch_cld_qty' => $child_cld,
//                  'type' => $type,
//                ))
//                ->condition('bill_number', $bill, '=')
//                ->condition('sku_code', $child_sku['child_sku'], '=')
//                ->execute();
//            }        
      }
    }
  }
  $supervisor_resource_nid = $form_state['values']['bill_editor'];
  $field = 'field_bill_cld_confirm_resource';
  save_dispatch_qty_supervisor_name($trip_id,$supervisor_resource_nid,$dis_id,$field);
  drupal_set_message(t("Data saved successfully."));
}

function billwise_final_submit($form, &$form_state) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);
  $trip_id = $form_state['values']['trip_number'];
  $form_state['values']['trip_type'] = get_trip_type($trip_id, $dis_id);
  $type = $form_state['values']['trip_type'];
  $all_parent_sku = get_all_parent_cld_skus_query_wise($trip_id, $dis_id);
  $child_sku = get_child_cld_sku_query_wise($all_parent_sku, $dis_id);

  foreach ($form_state['bill_wise_editing_cld']['skus'] as $key => $value) {
    //$parent_upc = get_sku_unit_case($value['sku7_code'], $dis_id);
    //$child_sku = get_child_detials_by_parent_sku($value['sku7_code'], $dis_id);
    //$child_ratio = $child_sku['child_qty'];
    //$child_upc = get_sku_unit_case($child_sku['child_sku'], $dis_id);

    foreach ($form_state['input']["sku_bill_table_" . $value['sku7_code']] as $bill => $val) {
      $original_cld_qty = $form["sku_bill_table_" . $value['sku7_code']]['#rows'][$bill]['original_cld_qty']['data'];
      $cld_confirmed_qty = $val['cld_confirmed_qty'];
      $cld_short_reason = $val['cld_short_reason'];
      $to_be_dispatched_qty = $val['to_be_dispatch'];

      if ($to_be_dispatched_qty !== '') {
        db_update('dispatch_qty_confirmation')
          ->fields(array(
            'cld_confirmed_qty' => (!empty($cld_confirmed_qty)) ? $cld_confirmed_qty : 0,
            'cld_short_reason' => ($cld_confirmed_qty != $original_cld_qty) ? $cld_short_reason : '',
            'dispatch_cld_qty' => (!empty($to_be_dispatched_qty) || $to_be_dispatched_qty === '0') ? $to_be_dispatched_qty : $cld_confirmed_qty,
            'type' => $type,
          ))
          ->condition('trip_id', $trip_id, '=')
          ->condition('sku_code', $value['sku7_code'], '=')
          ->condition('bill_number', $bill, '=')
          ->execute();

        //child sku calculation
//          if ($child_sku) {
//              $parent_qty = $cld_confirmed_qty * $parent_upc;
//              $child_total_qty = $parent_qty * $child_ratio;
//              $child_cld = (int) ($child_total_qty / $child_upc);
//              $child_loose = (int) ($child_total_qty % $child_upc);
//               db_update('dispatch_qty_confirmation')
//                ->expression('cld_confirmed_qty', 'cld_confirmed_qty + :ccq', array(':ccq' => $child_cld))
//                ->expression('cld_loose_child_qty', 'cld_loose_child_qty + :clc', array(':clc' => $child_loose))
//                ->expression('dispatch_cld_qty', 'dispatch_cld_qty + :dis', array(':dis' => $child_cld))
//                ->fields(array(
//                  //'cld_confirmed_qty' => $child_cld,
//                  //'cld_loose_child_qty' => $child_loose,
//                  'cld_short_reason' => $cld_short_reason ? $cld_short_reason : '',
//                  //'dispatch_cld_qty' => $child_cld,
//                  'type' => $type,
//                )) 
//                ->condition('trip_id', $trip_id, '=')
//                ->condition('bill_number', $bill, '=')
//                ->condition('sku_code', $child_sku['child_sku'], '=')
//                ->execute();
//          }    
      }
    }
  }

  //convert_cld_child_greater_then_upc($trip_id, $dis_id);
  //validation for all skus in trip
  $validate = check_validation_trip_data_cld_bill($trip_id, $dis_id);
  $skus = implode(',', $validate['bill_sku']);
  if ($validate['flag'] == 1) {
    $query = db_select('dispatch_qty_confirmation', 'dqc');
    $query->condition('dqc.trip_id', $trip_id);
    $query->condition('dqc.distributor_id', $dis_id);
    $query->condition('dqc.original_cld_qty', 0, '>');
    if (!empty($child_sku['child_sku'])) {
      $query->condition('dqc.sku_code', $child_sku['child_sku'], 'NOT IN');
    }
    $query->addExpression("CONCAT(dqc.bill_number, '-', dqc.sku_code)", "grp_concat");
    $query->addExpression("dqc.bill_number", "bill_number");
    $query->addExpression('dqc.sku_code', 'sku_code');
    $query->addExpression('dqc.cld_confirmed_qty', 'cld_confirmed_qty');
    $query->addExpression('dqc.cld_short_reason', 'cld_short_reason');
    $query->addExpression('dqc.upc', 'upc');
    //$query->groupBy('dqc.sku_code');
    $result = $query->execute()->fetchAll(PDO::FETCH_GROUP | PDO::FETCH_UNIQUE | PDO::FETCH_ASSOC);
    foreach ($result as $value) {
      $child_sku_exist = get_child_detials_by_parent_sku($value['sku_code'], $dis_id);
      if ($child_sku_exist) {
        $cld_confirmed_qty = $value['cld_confirmed_qty'];
        $parent_upc = $value['upc'];
        $bill = $value['bill_number'];
        $cld_short_reason = $value['cld_short_reason'];
        $child_ratio = $child_sku_exist['child_qty'];
        $child_upc = get_sku_unit_case($child_sku_exist['child_sku'], $dis_id);
        $parent_qty = $cld_confirmed_qty * $parent_upc;
        $child_total_qty = $parent_qty * $child_ratio;
        $child_cld = (int) ($child_total_qty / $child_upc);
        $child_loose = (int) ($child_total_qty % $child_upc);
        db_update('dispatch_qty_confirmation')
          ->expression('cld_confirmed_qty', 'cld_confirmed_qty + :ccq', array(':ccq' => $child_cld))
          ->expression('cld_loose_child_qty', 'cld_loose_child_qty + :clc', array(':clc' => $child_loose))
          ->expression('dispatch_cld_qty', 'dispatch_cld_qty + :dis', array(':dis' => $child_cld))
          ->fields(array(
            //'cld_confirmed_qty' => $child_cld,
            //'cld_loose_child_qty' => $child_loose,
            'cld_short_reason' => $cld_short_reason ? $cld_short_reason : '',
            //'dispatch_cld_qty' => $child_cld,
            'type' => $type,
          ))
          ->condition('trip_id', $trip_id, '=')
          ->condition('bill_number', $bill, '=')
          ->condition('sku_code', $child_sku_exist['child_sku'], '=')
          ->execute();
      }
    }

    convert_cld_child_greater_then_upc($trip_id, $dis_id);
    //update flag and type for all sku of that trip whose flag is zero and loose > 0 
    db_update('dispatch_qty_confirmation')
    // i think in cld dispatch_cld qty is confirm cld qty directly abhihektp
    //and all the places find child sku based on all sku not where cld>0. because parent
    // sku might don't have cld but child have due to difference in upc
    // put child sku in cron preprocess auto close show that no parent has cld/loose then trip flag is closed
      ->expression('dispatch_cld_qty', "CASE WHEN cld_confirmed_qty = 0 and cld_short_reason = '' AND cld_loose_child_qty = 0 THEN original_cld_qty ELSE cld_confirmed_qty END")
      ->fields(array(
        'cld_close_flag' => 1,
        'type' => $type,
        'cld_created_date' => REQUEST_TIME
      ))
      ->condition('trip_id', $trip_id, '=')
//          ->condition('original_cld_qty', 0, '>')
      //->condition('type', $type, '=')
      ->execute();
    
    //for child avoid above expression.  
    if($child_sku['child_sku']){
      db_update('dispatch_qty_confirmation')
        ->expression('dispatch_cld_qty', "cld_confirmed_qty")
        ->condition('trip_id', $trip_id, '=')
        ->condition('sku_code', $child_sku['child_sku'], 'IN')
        ->execute();
    }

    //Sync To be Dispatch Qty
    db_update('dispatch_qty_confirmation') // Table name no longer needs {}
      ->expression('to_be_dispatch_qty', "dispatch_loose_qty + (dispatch_cld_qty * upc) + cld_loose_child_qty")
      ->condition('trip_id', $trip_id, '=')
      ->execute();
    //End
    //update dispatch data      // From CLD screen
    $nid = get_nid_from_title($trip_id, 'dispatch_data');
    $node = node_load($nid);
    $node->field_cld_confirmation['und'][0]['value'] = 1;
    node_save($node);
    $supervisor_resource_nid = $form_state['values']['bill_editor'];
    $field = 'field_bill_cld_confirm_resource';
    save_dispatch_qty_supervisor_name($trip_id,$supervisor_resource_nid,$dis_id,$field);
    drupal_set_message(t("Data closed successfully."));
  }
  else {
    drupal_set_message(t('Confirmed CLD Qty	is mandatory for ' . $skus), 'error');
    $form_state['rebuild'] = TRUE;
  }
}

function get_bill_wise_cld_pop_up_table($trip_number, $skus, $dis_id) {

  $query = db_select('dispatch_qty_confirmation', 'dqc');
  $query->condition('dqc.trip_id', $trip_number);
  $query->condition('dqc.distributor_id', $dis_id);
  $query->condition('dqc.sku_code', $skus);
  $query->condition('dqc.original_cld_qty', 0, '>');
  $query->fields('dqc', array('bill_number'));
  $query->join('node', 'n', 'n.title = dqc.bill_number AND n.uid = dqc.distributor_id');
  $query->condition('n.type', 'sales_register_data', '=');
  $query->join('field_data_field_party_code_reference', 'pc', 'pc.entity_id = n.nid');
  $query->join('node', 'oun', 'oun.nid = pc.field_party_code_reference_nid');
  $query->join('field_data_field_party_name', 'pn', 'oun.nid = pn.entity_id');
  $query->fields('pn', array('field_party_name_value'));
  $query->fields('dqc', array('original_cld_qty'));
  $query->groupBy('dqc.bill_number');
  $query->orderBy('dqc.original_cld_qty');
  $result = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
  return $result;
}

function get_qty_default_values_confirmed($trip_id, $bill, $sku, $dis_id) {
  $query = db_select('dispatch_qty_confirmation', 'dc');
  $query->condition('dc.trip_id', $trip_id, '=');
  $query->condition('dc.bill_number', $bill, '=');
  $query->condition('dc.sku_code', $sku, '=');
  $query->condition('dc.distributor_id', $dis_id, '=');
  $query->fields('dc', array('cld_confirmed_qty', 'cld_short_reason', 'dispatch_cld_qty'));
  $result = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

  return $result[0];
}

function check_validation_trip_data_cld_bill($trip_id, $dis_id) {
  $all_parent_sku = get_all_parent_cld_skus_query_wise($trip_id, $dis_id);
  $child_sku = get_child_cld_sku_query_wise($all_parent_sku, $dis_id);
  $validation_pass['flag'] = 1;
  $query = db_select('dispatch_qty_confirmation', 'dc');
  $query->condition('dc.trip_id', $trip_id, '=');
  $query->condition('dc.distributor_id', $dis_id, '=');
  $query->condition('dc.original_cld_qty', 0, '>');
  //below condition is added to exclude child sku in validation on close trip
  if (!empty($child_sku['child_sku'])) {
    $query->condition('dc.sku_code', $child_sku['child_sku'], 'NOT IN');
  }
  $query->addExpression('dc.cld_confirmed_qty', 'cld_confirmed_qty');
  $query->addExpression('dc.original_cld_qty', 'original_cld_qty');
  $query->groupBy('dc.sku_code');

  $query->fields('dc', array('bill_number', 'sku_code', 'cld_short_reason'));
  $result = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
  foreach ($result as $key => $value) {
    $original_cld_qty = $value['original_cld_qty'];
    $cld_confirmed_qty = $value['cld_confirmed_qty'];
    $cld_short_reason = $value['cld_short_reason'];
    if ($cld_confirmed_qty != $original_cld_qty && empty($cld_short_reason)) {
      $validation_pass['flag'] = 0;
      $validation_pass['bill_sku'][$key] = $value['bill_number'] . '-' . $value['sku_code'];
    }
  }
  return $validation_pass;
}
