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
function master_ptl_loose_confirmation_form($form, &$form_state) {
  $form['#attached']['js'][] = drupal_get_path('module', 'dispatched_quantity_confirmation') . '/js/master_ptl_loose_confirmation.js';
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
  );
  // IF YOU ADD ANY FILTER THEN YOU HAVE TO CHANGE THE SUBMIT. LIKE OTHER SCREENS
  // Its look like illogical but its true
  $trip_number = $form_state['input']['trip_number'];
  
  $form['apply'] = array(
    '#type' => 'submit',
    '#value' => 'Apply',
    '#prefix' => "<div><div style='float:left';width:50%>",
    '#suffix' => '</div>',
    '#ajax' => array(
      'callback' => 'master_ptl_loose_confirmation_callback',
      'wrapper' => 'itemlist-div',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    '#validate' => array('trip_id_validate'),
    '#submit' => array('master_ptl_loose_confirmation_submit'),
  );

  $form['allocate'] = array(
    '#type' => 'submit',
    '#value' => 'Auto-assign Bills',
    '#prefix' => "<div style='float:left';width:50%>",
    '#suffix' => '</div></div>',
    '#ajax' => array(
      'callback' => 'master_ptl_loose_confirmation_callback',
      'wrapper' => 'itemlist-div',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    //'#validate' => array('trip_id_validate'),
    '#submit' => array('master_ptl_loose_autoallocate'),
  );

  $header = array('SKU Code',
    'Product Name',
    //'Original Qty',
    'Original Loose Qty',
    'Master PTL Put Quantity',
    'Short Quantity',
    'Bill Numbers');

  if (!isset($form_state['master_ptl_loose_confirmation']['skus'])) {
    $form_state['master_ptl_loose_confirmation']['skus'] = array();
  }
  $options = $form_state['master_ptl_loose_confirmation']['skus'];

  if (empty($form_state['trip_no_error'])) {
    if (!empty($options)) {
      $form['sku_details'] = array(
        '#theme' => 'table',
        '#tree' => TRUE,
        '#header' => $header,
        '#rows' => array(),
        '#prefix' => '<div class="custom_middle_table" style="float:left;clear:both;width:100%">',
        '#suffix' => '</div>',
      );
      // Dispatch Supervisor
        $incharge_name_option = array('' => 'Select');
        $resource_type = 'Packing Auditor';
        $dispatch_supervisor_name_option = array();
        $dispatch_supervisor_options = get_ptl_supervisor_list($dis_id,$resource_type);
        if (!empty($dispatch_supervisor_options)) {
          $dispatch_supervisor_name_option = $incharge_name_option + $dispatch_supervisor_options;
          $field = 'field_packing_auditor_loose';
          $default_value_superviser = get_auditor_name($dis_id,$trip_id,$field);
        }
        $form['packing_auditor_name'] = array(
          '#title' => t('Packing Auditor Name'.'<span class="form-required" title="This field is required.">*</span>'),
          //'#required' => TRUE,
          '#type' => 'select',
          '#default_value'=>$default_value_superviser,
          '#prefix'=>'<div class="packing_auditor_outer"><div class="packing_auditor_name">',
          '#suffix'=>'</div></div>',
          '#options' => $dispatch_supervisor_name_option,
          '#attributes' => array('class' => array('return-incharge-name')),
        );

      $form['submit'] = array(
        '#type' => 'submit',
        '#value' => 'Submit',
        '#prefix' => "<div style='float:left';width:50%>",
        '#suffix' => '</div></div>',
        '#validate' => array('_masterptl_validate'),
        '#submit' => array('_masterptl_submit'),
      );
    }

    $header2 = array('Bill Number', 'Party Name', 'Original Loose Qty', 'Short Qty', 'Short Reason', 'To be Dispatched Qty- Loose');

    foreach ($options as $key => $val) {
      $sku_code = $val['sku_code'];
      //$child_sku = get_child_detials_by_parent_sku($sku_code, $dis_id);

      $form['sku_details']['#rows'][$key]["sku_code"] = $val['sku_code'];
      $form['sku_details']['#rows'][$key]['product_name'] = $val['product_name'];
      //$form['sku_details']['#rows'][$key]['og'] = $val['og'];
      $form['sku_details']['#rows'][$key]['looses'] = $val['looses'];
      $form['sku_details']['#rows'][$key]['put_qty'] = $val['put_qty'];
      //Put qty changes
//              $put_qty = array(
//              '#type' => 'textfield',
//              '#attributes' => array('readonly' => 'readonly','class' => array('put_qty','put-qty_' . $sku_code)),
////              '#attributes' => array('child-sku'=> $child_sku['child_sku'], 'child-qty'=> $child_sku['child_qty'],'readonly' => 'readonly','class' => array('put_qty','put-qty_' . $sku_code)),
//              '#size' => 10,
//              '#default_value' => $val['put_qty'],
//              );
//              $form['sku_details'][$key]["put_qty"] = &$put_qty;
//              $form['sku_details']['#rows'][$key][]['data']["put_qty"] = &$put_qty;
//               unset($put_qty);
      //
            
            $form['sku_details']['#rows'][$key]['short_qty'] = $val['short_qty'];

      $form['sku_details']['#rows'][$key][] = array('data' => l('Show Bills', 'javascript:', array('external' => TRUE)), 'class' => 'show-bills skus-' . $val['sku_code'], 'sku_code' => $val['sku_code']);
//            $form['sku_details']['#rows'][$key][] = array('child-sku'=> $child_sku['child_sku'], 'child-qty'=> $child_sku['child_qty'],'data' => l('Show Bills', 'javascript:', array('external' => TRUE)), 'class' => 'show-bills skus-'.$val['sku_code'], 'sku_code' => $val['sku_code']);
      //Popup part
      $form["sku_bill_table_$sku_code"] = array(
        '#theme' => 'table',
        '#tree' => TRUE,
        '#header' => $header2,
        '#rows' => array(),
        '#sticky' => FALSE,
        '#caption' => "<b>Product Name: </b>" . $val['product_name'],
        '#prefix' => '<div id ="pop_up_bill_' . $sku_code . '" class="pop-up_table" style="display:none;"><div class="popup_overlay"></div><div class="popup_wrap"><div class="pop_content"><div class="popup_border">'
        . '<img sku_key="' . $sku_code . '" class="close-pop-up-button" id="close_pop_up_bill_' . $sku_code . '" src="sites/all/modules/custom/mrc_inventory_returns/images/close_btn.png">',
        '#suffix' => '</div></div></div></div>',
      );

      $all_bills = get_master_ptl_pop_up_table($trip_number, $sku_code, $dis_id);

      foreach ($all_bills as $val1) {
        $get_qty = get_qty_default_values_mptl($trip_number, $val1['bill_number'], $sku_code, $dis_id);
        $form["sku_bill_table_$sku_code"]['#rows'][$val1['bill_number']][] = array('data' => $val1['bill_number'], 'class' => 'bill_num');
        $form["sku_bill_table_$sku_code"]['#rows'][$val1['bill_number']][] = $val1['field_party_name_value'];
        $form["sku_bill_table_$sku_code"]['#rows'][$val1['bill_number']][] = array('data' => $val1['looses'], 'class' => 'og_qty');
//      $form["sku_bill_table_$sku_code"]['#rows'][$val1['bill_number']]['short_qty'] = array('data' => $val1['short_qty'], 'class' => 'short_qty');
        $pplc_short_qty = array(
          '#type' => 'textfield',
//                    '#attributes' => array('child-sku'=> $child_sku['child_sku'], 'child-qty'=> $child_sku['child_qty'],'class' => array('pplc-short-qty','psq-'.$val1['bill_number'].'-'.$sku_code)),
          '#attributes' => array('class' => array('pplc-short-qty', 'psq-' . $val1['bill_number'] . '-' . $sku_code)),
          '#size' => 10,
          '#default_value' => $get_qty['masterptl_short_qty'] ? $get_qty['masterptl_short_qty'] : 0,
        );
        $form["sku_bill_table_$sku_code"][$val1['bill_number']]["pplc_short_qty"] = &$pplc_short_qty;
        $form["sku_bill_table_$sku_code"]['#rows'][$val1['bill_number']][]['data']["pplc_short_qty"] = &$pplc_short_qty;
//
        $reason_options = array('' => 'Select');
        $dispatch_short_reason_options = get_short_reason_list($dis_id, 'Master PTL');
        if (!empty($dispatch_short_reason_options)) {
          $reason_options = $reason_options + $dispatch_short_reason_options;
        }
        $master_short_reason = array(
          '#type' => 'select',
          '#options' => $reason_options,
          '#default_value' => $get_qty['short_reason'],
//                    '#attributes' => array('child-sku'=> $child_sku['child_sku'],'class' => array('msh-res-'.$val1['bill_number'].'-'.$sku_code,'master-pplc-short-reason')),
          '#attributes' => array('class' => array('msh-res-' . $val1['bill_number'] . '-' . $sku_code, 'master-pplc-short-reason')),
        );
        $form["sku_bill_table_$sku_code"][$val1['bill_number']]["master_short_reason"] = &$master_short_reason;
        $form["sku_bill_table_$sku_code"]['#rows'][$val1['bill_number']]["master_short_reason"]['data']["master_short_reason"] = &$master_short_reason;
        $to_be_dispatch = array(
          '#type' => 'textfield',
          '#attributes' => array('class' => array('to-be-dispatch-qty', 'disp-' . $val1['bill_number'] . '-' . $sku_code), 'readonly' => 'readonly'),
          '#size' => 10,
          '#default_value' => $get_qty['dispatch_loose_qty'] ? $get_qty['dispatch_loose_qty'] : 0,
        );
        $form["sku_bill_table_$sku_code"][$val1['bill_number']]["to_be_dispatch"] = &$to_be_dispatch;
        $form["sku_bill_table_$sku_code"]['#rows'][$val1['bill_number']]["to_be_dispatch"]['data']["to_be_dispatch"] = &$to_be_dispatch;
//
        unset($pplc_short_qty);
        unset($master_short_reason);
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
function master_ptl_loose_confirmation_callback($form, &$form_state) {
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
  $form_state['trip_no_error'] = 0;

  $bypass_flag = get_loose_cld_flag_trip_level($trip_number, $dis_id);
  if ($bypass_flag['loose'] == 1) {
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
    //check dp bills in trip
    $dp_bills = check_dp_bills_in_trip($result);
    if (!empty($dp_bills)) {
      drupal_set_message(t("Trip ID $trip_number contains DP Bills"), 'error');
      $form_state['trip_no_error'] = 1;
    }

    $query = db_select('dispatch_qty_confirmation', 'dqc');
    $query->condition('dqc.trip_id', $trip_number, '=');
    $query->condition('dqc.distributor_id', $dis_id, '=');
    $query->fields('dqc', array('loose_close_flag'));
    $result1 = $query->execute()->fetchField();
    if (!empty($result1)) {
      drupal_set_message(t('This trip is already closed.'), 'error');
      $form_state['trip_no_error'] = 1;
    }
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
}

function master_ptl_loose_confirmation_submit($form, &$form_state) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);
  $trip_number = $form_state['values']['trip_number'];
  $child_sku = get_child_sku_mptl_query_wise($trip_number, $dis_id);

  if (empty($form_state['trip_no_error'])) {
    $query = db_select('master_ptl_data', 'mpd');
    $query->condition('mpd.trip_id', $trip_number);
    $query->condition('mpd.distributor_id', $dis_id);
    $query->condition('mpd.short_put_qty', 0, '>');
    if (!empty($child_sku['child_sku'])) {
      $query->condition('mpd.sku_code', $child_sku['child_sku'], 'NOT IN');
    }
    $query->join('dispatch_qty_confirmation', 'dqc', ' dqc.trip_id = mpd.trip_id AND dqc.sku_code = mpd.sku_code AND mpd.distributor_id = dqc.distributor_id');
    $query->condition('dqc.original_loose_qty', 0, '>');
    $query->addExpression('mpd.sku_code', 'sku');
    $query->addExpression('mpd.sku_code', 'sku_code');
    $query->addExpression('mpd.product_name', 'product_name');
    $query->addExpression('mpd.loose', 'looses');
    $query->addExpression('mpd.put_qty', 'put_qty');
    $query->addExpression("mpd.short_put_qty", 'short_qty');
    $query->groupBy('mpd.sku_code');

    $result = $query->execute()->fetchAll(PDO::FETCH_GROUP | PDO::FETCH_UNIQUE | PDO::FETCH_ASSOC);
    if (empty($result)) {
      //when there is no skus with short use this function also child sku handled
      assign_master_ptl_loose_noshort($trip_number, $dis_id);
    }
    $form_state['master_ptl_loose_confirmation']['skus'] = $result;
  }
  $form_state['rebuild'] = TRUE;
}

function _masterptl_validate($form, &$form_state) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);
  $trip_number = $form_state['values']['trip_number'];
  $form_state['mptl_error'] = 0;
  //$child_sku = get_child_sku_mptl_query_wise($trip_number,$dis_id);
  foreach ($form_state['master_ptl_loose_confirmation']['skus'] as $key => $value) {
    $short_qty = $value['short_qty'];
    $pplc_short_qty_sum = 0;
    foreach ($form_state['input']["sku_bill_table_" . $value['sku_code']] as $bill => $val) {
      $original_loose_qty = $form["sku_bill_table_" . $value['sku_code']]['#rows'][$bill][2]['data'];
      if (!empty($val['pplc_short_qty'])) {
        $pplc_short_qty_sum += $val['pplc_short_qty'];
      }
      if ($val['to_be_dispatch'] < 0) {
        drupal_set_message(t("To be Dispatched Qty cannot be negative for sku: " . $value['sku_code'] . " and bill $bill"), 'error');
        $form_state['mptl_error'] = 1;
      }
      if ($val['pplc_short_qty'] != '' && $val['pplc_short_qty'] != 0 && $val['master_short_reason'] == '') {
        drupal_set_message(t("Enter Short Reason for sku: " . $value['sku_code'] . " and bill $bill"), 'error');
        $form_state['mptl_error'] = 1;
      }
      if ($original_loose_qty < $val['pplc_short_qty']) {
        drupal_set_message(t("Short qty cannot be greater then Original Loose Qty for sku " . $value['sku_code'] . " and bill $bill"), 'error');
        $form_state['mptl_error'] = 1;
      }
    }

//        if ($short_qty != $pplc_short_qty_sum && $pplc_short_qty_sum != 0) {
    // dont run this validation for child skus
//        if ($short_qty != $pplc_short_qty_sum && !in_array($value['sku_code'],$child_sku['child_sku'])) {
    if ($short_qty != $pplc_short_qty_sum) {
      drupal_set_message(t("Short qty is not equal to sum of PPLC Short Qty for sku: " . $value['sku_code']), 'error');
      $form_state['mptl_error'] = 1;
    }
  }
  if(empty($form_state['values']['packing_auditor_name'])) {
      drupal_set_message(t('Packing Auditor Name field is required'), 'error');
      $form_state['mptl_error'] = 1;
  }
  if ($form_state['mptl_error'] == 1) {
    $form_state['rebuild'] = TRUE;
  }
}

function _masterptl_submit($form, &$form_state) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);
  $trip_number = $form_state['values']['trip_number'];

  if (empty($form_state['mptl_error'])) {
    $child_sku = get_child_sku_mptl_query_wise($trip_number, $dis_id);
    $query = db_select('master_ptl_data', 'mpd');
    $query->join('dispatch_qty_confirmation', 'dqc', ' dqc.trip_id = mpd.trip_id AND dqc.sku_code = mpd.sku_code AND mpd.distributor_id = dqc.distributor_id');
    $query->condition('mpd.trip_id', $trip_number);
    $query->condition('mpd.distributor_id', $dis_id);
    if (!empty($child_sku['child_sku'])) {
      $query->condition('mpd.sku_code', $child_sku['child_sku'], 'NOT IN');
    }
    $query->addExpression('mpd.sku_code', 'sku_code');
    $query->addExpression('mpd.put_qty', 'put_qty');
    $query->groupBy('dqc.sku_code');

    $result = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
    foreach ($result as $key => $value) {
      $sku_code = $value['sku_code'];
      $put_qty = $value['put_qty'];
      $bills = get_bills_loose_from_trip_sku($trip_number, $sku_code, $dis_id);
      foreach ($bills as $bill_details) {

        $original_loose_qty = $bill_details->original_loose_qty;
        $bill = $bill_details->bill_number;
        $pplc_short_qty = $form_state['values']["sku_bill_table_" . $sku_code][$bill]['pplc_short_qty'];
        $to_be_dispatched_qty = $form_state['values']["sku_bill_table_" . $sku_code][$bill]['to_be_dispatch'];
        $master_short_reason = $form_state['input']["sku_bill_table_" . $sku_code][$bill]['master_short_reason'];
        if ($to_be_dispatched_qty !== '' && !is_null($to_be_dispatched_qty)) {
          if ($original_loose_qty <= ($put_qty + $pplc_short_qty)) {
            $dispatch_loose_qty = $original_loose_qty - $pplc_short_qty;
            db_update('dispatch_qty_confirmation')
              ->fields(array(
                'masterptl_short_qty' => $pplc_short_qty,
                'short_reason' => !empty($pplc_short_qty) ?  $master_short_reason : '',
                'dispatch_loose_qty' => $dispatch_loose_qty,
              ))
              ->condition('trip_id', $trip_number, '=')
              ->condition('bill_number', $bill, '=')
              ->condition('sku_code', $sku_code, '=')
              ->execute();
            $put_qty -= $dispatch_loose_qty;
          }
          else {
            $dispatch_loose_qty = $put_qty;
            db_update('dispatch_qty_confirmation')
              ->fields(array(
                'masterptl_short_qty' => $pplc_short_qty,
                'short_reason' => !empty($pplc_short_qty) ?  $master_short_reason : '',
                'dispatch_loose_qty' => $dispatch_loose_qty,
              ))
              ->condition('trip_id', $trip_number, '=')
              ->condition('bill_number', $bill, '=')
              ->condition('sku_code', $sku_code, '=')
              ->execute();
            $put_qty -= $dispatch_loose_qty;
          }
        }
        else {
          //no short qty. which are not in form
          if ($bill_details->original_loose_qty < $put_qty) {
            db_update('dispatch_qty_confirmation')
              ->fields(array(
                //'ptl_put_qty' => $put_qty,
                'dispatch_loose_qty' => $bill_details->original_loose_qty,
              ))
              ->condition('trip_id', $trip_number, '=')
              ->condition('bill_number', $bill_details->bill_number, '=')
              ->condition('sku_code', $sku_code, '=')
              ->execute();
            $put_qty -= $bill_details->original_loose_qty;
          }
          else {
            db_update('dispatch_qty_confirmation')
              ->fields(array(
                //'ptl_put_qty' => $put_qty,
                'dispatch_loose_qty' => $put_qty,
              ))
              ->condition('trip_id', $trip_number, '=')
              ->condition('bill_number', $bill_details->bill_number, '=')
              ->condition('sku_code', $sku_code, '=')
              ->execute();
            $put_qty = 0;
          }
        }
      }
    }

    //update flag and type for trip confirmed so that loose flag is close.dispatch_loose_qty is updated for those whose lose flag is 1 so that dispatch qty is zero also handled 
    db_update('dispatch_qty_confirmation')
      //->expression('dispatch_loose_qty', 'CASE WHEN loose_close_flag = 1 THEN dispatch_loose_qty ELSE ptl_put_qty END')
      ->fields(array(
        'loose_close_flag' => 1,
        'type' => 'Master PTL',
        'loose_created_date' => REQUEST_TIME
      ))
      ->condition('trip_id', $trip_number, '=')
//      ->condition('original_loose_qty', 0, '>')
//      ->condition('loose_close_flag', 0, '=')
//      ->condition('type', 'Master PTL', '=')     
      ->execute();

    update_mptl_child_loose($trip_number, $dis_id, 'Master PTL');
    $flag = check_loose_cld_close_flag($trip_number, $dis_id);
    if ($flag['loose'] == 1 && $flag['cld'] == 1) {
      sync_mptl_loose_cld($trip_number, $dis_id);
      update_mptl_child_cld($trip_number, $dis_id, 'Master PTL');
    }

    //Sync To be Dispatch Qty
    db_update('dispatch_qty_confirmation') // Table name no longer needs {}
      ->expression('to_be_dispatch_qty', "dispatch_loose_qty + (dispatch_cld_qty * upc) + cld_loose_child_qty")
      ->condition('trip_id', $trip_number, '=')
      ->execute();
    //End
    //update dispatch data
    $nid = get_nid_from_title($trip_number, 'dispatch_data');
    $node = node_load($nid);
    $node->field_loose_confirmation['und'][0]['value'] = 1;
    node_save($node);
    $supervisor_resource_nid = $form_state['values']['packing_auditor_name'];
    $field = 'field_packing_auditor_loose';
    save_dispatch_qty_supervisor_name($trip_number,$supervisor_resource_nid,$dis_id,$field);

    drupal_set_message(t("Data submitted successfully."));
  }
}

function get_master_ptl_pop_up_table($trip_number, $sku_code, $dis_id) {

  //$child_sku = get_child_sku_mptl_query_wise($trip_number,$dis_id);
  $query = db_select('master_ptl_data', 'mpd');
  $query->condition('mpd.trip_id', $trip_number);
  $query->condition('mpd.distributor_id', $dis_id);
  $query->condition('mpd.sku_code', $sku_code);
  $query->join('dispatch_qty_confirmation', 'dqc', ' dqc.trip_id = mpd.trip_id AND dqc.sku_code = mpd.sku_code AND mpd.distributor_id = dqc.distributor_id');
  $query->condition('dqc.original_loose_qty', 0, '>');
  $query->fields('dqc', array('bill_number'));
  $query->join('node', 'n', 'n.title = dqc.bill_number AND n.uid = dqc.distributor_id');
  $query->condition('n.type', 'sales_register_data', '=');
  $query->join('field_data_field_party_code_reference', 'pc', 'pc.entity_id = n.nid');
  $query->join('node', 'oun', 'oun.nid = pc.field_party_code_reference_nid');
  $query->join('field_data_field_party_name', 'pn', 'oun.nid = pn.entity_id');
  $query->fields('pn', array('field_party_name_value'));
  $query->addExpression('dqc.original_loose_qty', 'looses');
  $query->condition('mpd.short_put_qty', 0, '>');
  //append child sku bills to popup
//    if (!empty($child_sku['child_sku'])) {
//      $db_and1 = db_and();
//      $db_and1->condition('mpd.short_put_qty', 0, '>');
//      $db_and2 = db_and();
//      $db_and2->condition('mpd.sku_code', $child_sku['child_sku'], 'IN');
//      $db_or = db_or();
//      $db_or->condition($db_and1);
//      $db_or->condition($db_and2);
//      $query->condition($db_or);
//    }
//    else {
//      $query->condition('mpd.short_put_qty', 0, '>');
//    }
  $query->addExpression("mpd.short_put_qty", 'short_qty');
  $query->orderBy('dqc.original_loose_qty', 'DESC');
  $query->groupBy('dqc.bill_number');
  $result = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
  return $result;
}

function assign_master_ptl_loose_noshort($trip_number, $dis_id) {

  $child_sku = get_child_sku_mptl_query_wise($trip_number, $dis_id);
  $query = db_select('master_ptl_data', 'mpd');
  $query->join('dispatch_qty_confirmation', 'dqc', ' dqc.trip_id = mpd.trip_id AND dqc.sku_code = mpd.sku_code AND mpd.distributor_id = dqc.distributor_id');
  $query->condition('mpd.trip_id', $trip_number);
  if (!empty($child_sku['child_sku'])) {
    $query->condition('mpd.sku_code', $child_sku['child_sku'], 'NOT IN');
  }
  //$query->condition('mpd.put_qty', 0, '>'); //for put qty > zero mean we selecting only parent sku
  $query->condition('mpd.distributor_id', $dis_id);
  $query->addExpression('mpd.sku_code', 'sku_code');
  $query->addExpression('mpd.put_qty', 'put_qty');
  //$query->addExpression('dqc.bill_number', 'bill_number');
  $result = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

  foreach ($result as $key => $value) {
    $sku_code = $value['sku_code'];
    $bills = get_bills_loose_from_trip_sku($trip_number, $sku_code, $dis_id);
    $put_qty = $value['put_qty'];
    foreach ($bills as $bill_details) {

      if ($bill_details->original_loose_qty < $put_qty) {
        db_update('dispatch_qty_confirmation')
          ->fields(array(
            //'ptl_put_qty' => $put_qty,
            'dispatch_loose_qty' => $bill_details->original_loose_qty,
          ))
          ->condition('trip_id', $trip_number, '=')
          ->condition('bill_number', $bill_details->bill_number, '=')
          ->condition('sku_code', $sku_code, '=')
          ->execute();
        $put_qty -= $bill_details->original_loose_qty;
      }
      else {
        db_update('dispatch_qty_confirmation')
          ->fields(array(
            //'ptl_put_qty' => $put_qty,
            'dispatch_loose_qty' => $put_qty,
          ))
          ->condition('trip_id', $trip_number, '=')
          ->condition('bill_number', $bill_details->bill_number, '=')
          ->condition('sku_code', $sku_code, '=')
          ->execute();
        $put_qty = 0;
      }
    }
  }
  // child logic end
  //for all in noshort as in child also original loose qty in custom billing upload is already multiplied with ratio
//  db_update('dispatch_qty_confirmation')
//    ->expression('dispatch_loose_qty', 'original_loose_qty')
//    ->fields(array(
//      //'ptl_put_qty' => (!empty($put_qty)) ? $put_qty : 0,
//      'loose_close_flag' => 1,
//    ))
//    ->condition('trip_id', $trip_number, '=')
//    ->execute();
  //Sync To be Dispatch Qty   
  db_update('dispatch_qty_confirmation') // Table name no longer needs {}
    //->expression('to_be_dispatch_qty', "dispatch_loose_qty + (dispatch_cld_qty * upc) + cld_loose_child_qty")
    ->fields(array(
      'loose_close_flag' => 1,
      'type' => 'Master PTL',
      'loose_created_date' => REQUEST_TIME,
    ))
    ->condition('trip_id', $trip_number, '=')
    ->execute();
  //End

  update_mptl_child_loose($trip_number, $dis_id, 'Master PTL');
  $flag = check_loose_cld_close_flag($trip_number, $dis_id);
  if ($flag['loose'] == 1 && $flag['cld'] == 1) {
    sync_mptl_loose_cld($trip_number, $dis_id);
    update_mptl_child_cld($trip_number, $dis_id, 'Master PTL');
  }

  //Sync To be Dispatch Qty
  db_update('dispatch_qty_confirmation') // Table name no longer needs {}
    ->expression('to_be_dispatch_qty', "dispatch_loose_qty + (dispatch_cld_qty * upc) + cld_loose_child_qty")
    ->condition('trip_id', $trip_number, '=')
    ->execute();

  //update dispatch data
  $nid = get_nid_from_title($trip_number, 'dispatch_data');
  $node = node_load($nid);
  $node->field_loose_confirmation['und'][0]['value'] = 1;
  node_save($node);

  drupal_set_message("Trip ID $trip_number does not have any Master PTL Short Qty", 'warning');
}

function get_child_sku_mptl_query_wise($trip_number, $dis_id) {

  $all_parent_sku = get_all_parent_skus_mptl_query_wise($trip_number, $dis_id);
  $result = array();
  if (!empty($all_parent_sku)) {
    $query = db_select('node', 'n');
    $query->condition('n.type', 'sku7_code_master');
    $query->condition('n.uid', $dis_id);
    $query->condition('n.title', $all_parent_sku, 'IN');
    $query->leftjoin('field_data_field_child_sku', 'csk', 'csk.entity_id = n.nid');
    $query->leftjoin('node', 'n1', 'n1.nid = csk.field_child_sku_nid');

    $query->join('field_data_field_sku7_code', 'sc', 'sc.entity_id = n1.nid');
    $query->fields('sc', array('field_sku7_code_value'));

    $result['child_sku'] = $query->execute()->fetchCol();
  }
  return $result;
}

function get_all_parent_skus_mptl_query_wise($trip_number, $dis_id) {
  $query = db_select('dispatch_qty_confirmation', 'dqc');
  //$query->leftjoin('master_ptl_data', 'mpd', ' dqc.trip_id = mpd.trip_id AND dqc.sku_code = mpd.sku_code AND mpd.distributor_id = dqc.distributor_id');
  $query->condition('dqc.trip_id', $trip_number);
  $query->condition('dqc.distributor_id', $dis_id);
  //$query->condition('mpd.short_put_qty', 0, '>');
  $query->addExpression('dqc.sku_code', 'sku_code');
  $query->groupBy('dqc.sku_code');
  $result = $query->execute()->fetchCol();
  return $result;
}

function master_ptl_loose_autoallocate($form, &$form_state) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);
  $trip_number = $form_state['values']['trip_number'];

  foreach ($form_state['master_ptl_loose_confirmation']['skus'] as $key => $value) {
    $put_qty = $value['put_qty'];
    $short_qty = $value['short_qty'];
    foreach ($form_state['input']["sku_bill_table_" . $value['sku_code']] as $bill => $val) {
      $master_short_reason = $val['master_short_reason'];
      $original_loose_qty = $form["sku_bill_table_" . $value['sku_code']]['#rows'][$bill][2]['data'];

      if ($original_loose_qty < $short_qty) {
        db_update('dispatch_qty_confirmation')
          ->fields(array(
            'masterptl_short_qty' => $original_loose_qty,
            //'short_reason' => $master_short_reason ? $master_short_reason : '',
            'dispatch_loose_qty' => 0,
            'type' => 'Master PTL',
          ))
          ->condition('trip_id', $trip_number, '=')
          ->condition('bill_number', $bill, '=')
          ->condition('sku_code', $value['sku_code'], '=')
          ->execute();

        $form_state['complete form']['sku_bill_table_' . $value['sku_code']]['#rows'][$bill][3]['data']['pplc_short_qty']['#value'] = $original_loose_qty;
        $form_state['complete form']['sku_bill_table_' . $value['sku_code']]['#rows'][$bill]['to_be_dispatch']['data']['to_be_dispatch']['#value'] = 0;
        $short_qty -= $original_loose_qty;
      }
      else {
        db_update('dispatch_qty_confirmation')
          ->fields(array(
            'masterptl_short_qty' => $short_qty,
            //'short_reason' => $master_short_reason ? $master_short_reason : '',
            'dispatch_loose_qty' => $original_loose_qty - $short_qty,
            'type' => 'Master PTL',
          ))
          ->condition('trip_id', $trip_number, '=')
          ->condition('bill_number', $bill, '=')
          ->condition('sku_code', $value['sku_code'], '=')
          ->execute();
        //break;
        $form_state['complete form']['sku_bill_table_' . $value['sku_code']]['#rows'][$bill][3]['data']['pplc_short_qty']['#value'] = $short_qty;
        $form_state['complete form']['sku_bill_table_' . $value['sku_code']]['#rows'][$bill]['to_be_dispatch']['data']['to_be_dispatch']['#value'] = $original_loose_qty - $short_qty;

        $short_qty = 0; // instead of break for remailining lines update 0 with reason
      }
    }
  }
}

function get_qty_default_values_mptl($trip_id, $bill, $sku, $dis_id) {
  $query = db_select('dispatch_qty_confirmation', 'dc');
  $query->condition('dc.trip_id', $trip_id, '=');
  $query->condition('dc.bill_number', $bill, '=');
  $query->condition('dc.sku_code', $sku, '=');
  $query->condition('dc.distributor_id', $dis_id, '=');
  $query->fields('dc', array('masterptl_short_qty', 'short_reason', 'dispatch_loose_qty'));
  $result = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

  return $result[0];
}

function get_bills_loose_from_trip_sku($trip_id, $sku, $dis_id) {
  $query = db_select('dispatch_qty_confirmation', 'dqp');
  $query->fields('dqp', array('id', 'bill_number', 'original_loose_qty', 'upc'));
  $query->condition('dqp.distributor_id', $dis_id);
  $query->condition('dqp.trip_id', $trip_id);
  $query->condition('dqp.sku_code', $sku);
  $query->condition('dqp.original_loose_qty', 0, '>');
  $bills = $query->execute()->fetchAll();
  return $bills;
}
