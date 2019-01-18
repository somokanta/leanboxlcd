<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

// Get Resource Type Name.
module_load_include('inc', 'dispatched_quantity_confirmation', 'includes/dispatch_qty_common');

function confirm_bill_editing_status_form($form, &$form_state) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);
  $form['#attached']['js'][] = drupal_get_path('module', 'dispatched_quantity_confirmation') . '/js/confirm_bill_editing_status.js';
  $form['#attached']['css'][] = drupal_get_path('module', 'dispatched_quantity_confirmation') . '/css/dispatched_quantity_confirmation.css';
  
  $form['#prefix'] = '<div id="itemlist-div">';
  $form['#suffix'] = '</div>';

  $form['trip_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Trip ID'),
    '#required' => TRUE,
    //'#autocomplete_path' => 'get-trips-not-dispatched/autocomplete',
  );
  $trip_number = $form_state['input']['trip_number'];

  $form['apply'] = array(
    '#type' => 'submit',
    '#value' => 'Apply',
    '#attributes' => array('class' => array('confirm-apply')),
    '#ajax' => array(
      'callback' => 'confirm_bill_editing_status_callback',
      'wrapper' => 'itemlist-div',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    '#validate' => array('trip_bill_edit_validate'),
    '#submit' => array('confirm_bill_editing_status_submit'),
  );

  $header = array('Bill Number',
    'SKU',
    'To Be Dispatched Qty',
    'IKEA Qty',
    'Excess Qty',
    'Excess Reason',
    'Excess Remarks');
  
  $header1 = array('Bill Number',
    'SKU',
    'To Be Dispatched Qty',
    'IKEA Qty',
    'Difference Qty',
    'Difference Reason',
    'Difference Remarks');

  if (!isset($form_state['confirm_bill_editing_status']['bills_sku'])) {
    $form_state['confirm_bill_editing_status']['bills_sku'] = array();
  }
  $options = $form_state['confirm_bill_editing_status']['bills_sku'];

  if (!isset($form_state['confirm_bill_editing_status']['bills_sku_new'])) {
    $form_state['confirm_bill_editing_status']['bills_sku_new'] = array();
  }
  $options1 = $form_state['confirm_bill_editing_status']['bills_sku_new'];

  $bill_status = get_confirm_bill_status_from_trip($trip_number, $dis_id);
  if ($bill_status == 1 && empty($form_state['trip_no_error'])) {
    $form['data'] = array(
      '#type' => 'markup',
      '#prefix' => '<div id ="pop_up_bill_" class="pop-up_table" style="display:none;"><div class="popup_overlay"></div><div class="popup_wrap"><div class="pop_content"><div class="popup_border">'
      . '<img sku_key="" class="close-pop-up-button" id="close_pop_up_bill_" src="sites/all/modules/custom/mrc_inventory_returns/images/close_btn.png">',
      '#markup' => "Trip ID $trip_number already confirmed. Do you want to undo confirmation? ",
    );
    $form['undo_yes'] = array(
      '#type' => 'submit',
      '#value' => 'Yes',
      //'#validate' => array('dispatched_qty_confirmation_save_validate'),
      '#submit' => array('confirm_bill_editing_status_undo_submit'),
    );
    $form['undo_no'] = array(
      '#type' => 'submit',
      '#value' => 'No',
      '#suffix' => '</div></div></div></div>'
    );
  }
  else if (empty($form_state['trip_no_error']) && $bill_status === '0') {

      $field_ikea_bill_editing_status = field_info_field('field_ikea_bill_editing_status');
      $field_ikea_bill_editing_status = list_allowed_values($field_ikea_bill_editing_status);
      $field_ikea_bill_editing_status = array('' => 'Select') + $field_ikea_bill_editing_status;

      $form['ikea_bill_editing_status'] = array(
        '#type' => 'select',
        '#title' => t('IKEA Bill Editing Status:'),
        '#options' => $field_ikea_bill_editing_status,
        //'#required' => TRUE,
      );
      
    // Confirmed By Field.
    $person_name_option = array('' => 'Select');
    $resource_type = 'Bill Editor';
    $resource_type_options = get_ptl_supervisor_list($dis_id, $resource_type);
    if (!empty($resource_type_options)) {
      $resource_type_name_option = $person_name_option + $resource_type_options;
    }
    $form['confirmed_by'] = array(
      '#title' => t('Confirmed By'),
      '#required' => TRUE,
      '#type' => 'select',
      '#options' => $resource_type_name_option,
    );

    if (!empty($options)) {
      $form['bill_sku_details'] = array(
        '#theme' => 'table',
        '#tree' => TRUE,
        '#header' => $header,
        '#rows' => $options,
        '#caption' => "<b>Excess Table: </b>",
        '#prefix' => '<div class="custom_middle_table">',
        '#suffix' => '</div>',
      );
    }
    
    // Common Reason field for Difference Table.
    if (!empty($options1)) {
      $option = array('' => 'Select');
      $bill_not_edited_reason_list = get_bill_not_edited_reason_list($dis_id);
      if (!empty($bill_not_edited_reason_list)) {
        $bill_not_edited_reason = $option + $bill_not_edited_reason_list;
      }
      $form['reason'] = array(
        '#title' => t('Difference Reason'),
        '#type' => 'select',
        '#options' => $bill_not_edited_reason,
        //'#required' => TRUE,
        '#attributes' => array('class' => array('reason_all'),'id'=>'reason_all'),
        '#prefix' => '<div class="common_reason difference_table_reason">',
        '#suffix' => '</div>',
      );
    }
    if (!empty($options1)) {
       $form['bill_sku_details_new'] = array(
        '#theme' => 'table',
        '#tree' => TRUE,
        '#header' => $header1,
        '#rows' => $options1,
        '#caption' => "<b>Difference Table: </b>",
        '#prefix' => '<div class="custom_middle_table difference_table">',
        '#suffix' => '</div>',
      );
    }

      $form['submit'] = array(
        '#type' => 'submit',
        '#value' => 'Confirm & Close Trip',
        '#validate' => array('confirm_bill_editing_status_final_validate'),
        '#submit' => array('confirm_bill_editing_status_final_submit'),
      );

    foreach ($options as $key => $val) {
      $get_excess_reason_default = get_excess_reason_default($trip_number, $val['bill_number'], $val['sku_code'], $dis_id);

      $reason_options = array('' => 'Select');
      $excess_reason_list = get_excess_reason_list($dis_id);
      if (!empty($excess_reason_list)) {
        $reason_options = $reason_options + $excess_reason_list;
      }

      $excess_reason = array(
        '#type' => 'select',
        '#options' => $reason_options,
        '#attributes' => array('class' => array('excess-reason_' . $key)),
        '#default_value' => $get_excess_reason_default,
      );
      $form['bill_sku_details'][$key]["excess_reason"] = &$excess_reason;
      $form['bill_sku_details']['#rows'][$key][]['data']["excess_reason"] = &$excess_reason;
      unset($excess_reason);
      
      $excess_remark = array(
        '#type' => 'textfield',
        '#attributes' => array('class' => array('remarks')),
        '#size' => 20,
      );
      $form['bill_sku_details'][$key]["excess_remark"] = &$excess_remark;
      $form['bill_sku_details']['#rows'][$key][]['data']["excess_remark"] = &$excess_remark;
      unset($excess_remark);
    }
    
    // Bill not edited reason for difference table.
    foreach ($options1 as $key1 => $val1) {
      $get_bill_not_edited_reason_default = get_bill_not_edited_reason_default($trip_number, $val['bill_number'], $val['sku_code'], $dis_id);

      $reason_option = array('' => 'Select');
      $bill_not_edited_reason_list = get_bill_not_edited_reason_list($dis_id);
      if (!empty($bill_not_edited_reason_list)) {
        $bill_not_edited_reason_option = $reason_option + $bill_not_edited_reason_list;
      }

      $bill_not_edited_reason = array(
        '#type' => 'select',
        '#options' => $bill_not_edited_reason_option,
        '#attributes' => array('class' => array('excess-reason_' . $key1, 'reason_bills')),
        '#default_value' => $get_bill_not_edited_reason_default,
      );
      $form['bill_sku_details_new'][$key1]["reason"] = &$bill_not_edited_reason;
      $form['bill_sku_details_new']['#rows'][$key1][]['data']["reason"] = &$bill_not_edited_reason;
      unset($bill_not_edited_reason);
      
      $difference_remark = array(
        '#type' => 'textfield',
        '#attributes' => array('class' => array('remarks')),
        '#size' => 20,
      );
      $form['bill_sku_details_new'][$key1]["difference_remark"] = &$difference_remark;
      $form['bill_sku_details_new']['#rows'][$key1][]['data']["difference_remark"] = &$difference_remark;
      unset($difference_remark);
    }
  }

  return $form;
}

function trip_bill_edit_validate($form, &$form_state) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);
  $trip_number = $form_state['values']['trip_number'];

  $bill_status = get_confirm_bill_status_from_trip($trip_number, $dis_id);
  $query = db_select('node', 'n');
  $query->condition('n.title', $trip_number, '=');
  $query->condition('n.type', 'dispatch_data');
  $query->leftjoin('field_data_field_dispatch_status', 'ds', 'ds.entity_id = n.nid');
  $db_or = db_or();
  $db_or->isNull('ds.field_dispatch_status_value');
  $db_or->condition('ds.field_dispatch_status_value', 0);
  $query->condition($db_or);
  $query->fields('n', array('nid'));
  $query->condition('n.uid', $dis_id);
  $result = $query->execute()->fetchField();
  if (empty($result)) {
    drupal_set_message(t("Trip ID $trip_number has already been dispatched or Invalid"), 'error');
    $form_state['trip_no_error'] = 1;
  }
  
  $bypass_flag = get_loose_cld_flag_trip_level($trip_number, $dis_id);
    if ($bypass_flag['bill_wise'] == 2) {
      drupal_set_message(t("Dispatch validation is bypassed for Trip ID $trip_number."), 'error');
      $form_state['trip_no_error'] = 1;
    }

  //check if revised custom billing data is uploaded 
  $bills = get_bills_from_trip($trip_number, $dis_id);
  if (!empty($bills)) {
    $query = db_select('custom_billing_data', 'cb');
    $query->condition('cb.distributor_id', $dis_id);
    $query->isNull('cb.updated_flag');
    $query->condition('cb.bill_number', $bills, 'IN');
    $query->addExpression('GROUP_CONCAT(DISTINCT cb.bill_number)', 'bills');
    $result = $query->execute()->fetchField();
    if (!empty($result)) {
      drupal_set_message(t("Please upload revised custom billing data for Bill Numbers $result"), 'error');
      $form_state['trip_no_error'] = 1;
    }
  }
  
  //reupload validation
  $reuploaded_bills = check_bill_reupload_complete($bills, $dis_id);
  $not_uploaded_bills = array_diff($bills, $reuploaded_bills);
  if (!empty($not_uploaded_bills)) {
    $error_bills = implode($not_uploaded_bills, ',');
    drupal_set_message("Please re-upload sales register data for Bill Numbers: <BR>" . $error_bills, 'error');
    $form_state['trip_no_error'] = 1;
  }
}

function confirm_bill_editing_status_callback($form, &$form_state) {
  $commands[] = ajax_command_replace('#itemlist-div', render($form));
  $commands[] = ajax_command_replace('.alert-danger', '');
  $commands[] = ajax_command_replace('.alert-success', '');
  $commands[] = ajax_command_prepend('#itemlist-div', theme('status_messages'));
  return array('#type' => 'ajax', '#commands' => $commands);
}

function confirm_bill_editing_status_submit($form, &$form_state) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);
  $trip_number = $form_state['values']['trip_number'];

  if (empty($form_state['trip_no_error'])) {
    
    //////////////////Fetch Data /////////////////////////
    $query = db_select('dispatch_qty_confirmation', 'dqc');
    $query->condition('dqc.trip_id', $trip_number);
    $query->condition('dqc.distributor_id', $dis_id);
    $query->join('custom_billing_data', 'cbd', 'cbd.bill_number = dqc.bill_number AND cbd.sku7_code = dqc.sku_code AND cbd.distributor_id = dqc.distributor_id');
    $query->addExpression("dqc.to_be_dispatch_qty - cbd.revised_host_quantity", 'diff_qty'); 
    $query->havingCondition('diff_qty', 0, '>'); 
    $query->fields('dqc', array('bill_number', 'sku_code'));
    $query->fields('dqc', array('to_be_dispatch_qty'));
    $query->fields('cbd', array('revised_host_quantity'));
    $result = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
    $form_state['confirm_bill_editing_status']['bills_sku'] = $result;
    
    $query = db_select('dispatch_qty_confirmation', 'dqc');
    $query->condition('dqc.trip_id', $trip_number);
    $query->condition('dqc.distributor_id', $dis_id);
    $query->join('custom_billing_data', 'cbd', 'cbd.bill_number = dqc.bill_number AND cbd.sku7_code = dqc.sku_code AND cbd.distributor_id = dqc.distributor_id');
    $query->addExpression("cbd.revised_host_quantity - dqc.to_be_dispatch_qty", 'diff_qty'); 
    $query->havingCondition('diff_qty', 0, '>'); 
    $query->fields('dqc', array('bill_number', 'sku_code'));
    $query->fields('dqc', array('to_be_dispatch_qty'));
    $query->fields('cbd', array('revised_host_quantity'));
    $result1 = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
    $form_state['confirm_bill_editing_status']['bills_sku_new'] = $result1;
  }
  $form_state['rebuild'] = TRUE;
}

function confirm_bill_editing_status_final_validate($form, &$form_state) {
  $form_state['reason_error'] = 0;
  if ($form_state['input']['ikea_bill_editing_status']== "") {
      drupal_set_message(t('Please select IKEA Bill Editing Status'), 'error');
      $form_state['reason_error'] = 1;
  }
  foreach ($form_state['confirm_bill_editing_status']['bills_sku'] as $key => $value) {
    if (empty($form_state['input']['bill_sku_details'][$key]['excess_reason'])) {
      drupal_set_message(t('Please select Excess reason for bill number ' . $value['bill_number'] . ' and sku ' . $value['sku_code']), 'error');
      $form_state['reason_error'] = 1;
    }
  }
  
  // Difference table Reason validation.
  foreach ($form_state['confirm_bill_editing_status']['bills_sku_new'] as $key2 => $value2) {
    if (empty($form_state['input']['bill_sku_details_new'][$key2]['reason'])) {
      drupal_set_message(t('Please select Difference reason for bill number ' . $value2['bill_number'] . ' and sku ' . $value2['sku_code']), 'error');
      $form_state['reason_error'] = 1;
    }
  }

  if ($form_state['reason_error'] == 1) {
    $form_state['rebuild'] = TRUE;
  }   
}

function confirm_bill_editing_status_final_submit($form, &$form_state) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);
  $trip_number = $form_state['values']['trip_number'];
  $ikea_bill_editing_status = $form_state['values']['ikea_bill_editing_status'];
  $confirmed_by = $form_state['values']['confirmed_by'];

  if (empty($form_state['reason_error'])) {
    //change status of dispatch data content type confirm bill status value
    $nid = get_nid_from_title($trip_number, 'dispatch_data');
    $node = node_load($nid);
    $node->field_confirm_bill_edit_status['und'][0]['value'] = 1;
    $node->field_ikea_bill_editing_status['und'][0]['value'] = $ikea_bill_editing_status;
    $node->field_confirmed_by['und'][0]['nid'] = $confirmed_by;
    $node->field_confirm_bill_edit_time['und'][0]['value'] = REQUEST_TIME;
    node_save($node);

    //insert in excess qty table 
    foreach ($form_state['confirm_bill_editing_status']['bills_sku'] as $key => $value) {
      db_insert('excess_qty')
        ->fields(array(
          'trip_id' => $trip_number,
          'bill_number' => $value['bill_number'],
          'sku_code' => $value['sku_code'],
          'reason_code' => $form_state['input']['bill_sku_details'][$key]['excess_reason'],
          'excess_qty ' => $value['diff_qty'],
          'created_date ' => REQUEST_TIME,
          'distributor_id' => $dis_id,
          'revised_host_qty' => $value['revised_host_quantity'],
          'to_be_dispatch_qty' => $value['to_be_dispatch_qty'],
          'excess_remarks' => $form_state['input']['bill_sku_details'][$key]['excess_remark']
        ))
        ->execute();
    }
    
    //insert in difference qty table 
    foreach ($form_state['confirm_bill_editing_status']['bills_sku_new'] as $key2 => $value2) {
      db_insert('difference_qty')
        ->fields(array(
          'trip_id' => $trip_number,
          'bill_number' => $value2['bill_number'],
          'sku_code' => $value2['sku_code'],
          'difference' => $value2['diff_qty'],
          'reason' => $form_state['input']['bill_sku_details_new'][$key2]['reason'],
          'created_date ' => REQUEST_TIME,
          'distributor_id' => $dis_id,
          'revised_host_qty' => $value2['revised_host_quantity'],
          'to_be_dispatch_qty' => $value2['to_be_dispatch_qty'],
          'remarks' => $form_state['input']['bill_sku_details_new'][$key2]['difference_remark'],
        ))
        ->execute();
    }
      drupal_set_message(t('Trip id ' . $trip_number . ' is confirmed.'));
  }
}

function get_excess_reason_default($trip_id, $bill_number, $sku_code, $dis_id) {
  $query = db_select('excess_qty', 'eq');
  $query->condition('eq.trip_id', $trip_id);
  $query->condition('eq.bill_number', $bill_number);
  $query->condition('eq.sku_code', $sku_code);
  $query->condition('eq.distributor_id', $dis_id);
  $query->fields('eq', array('reason_code'));
  $reason_code = $query->execute()->fetchField();
  return $reason_code;
}

function get_confirm_bill_status_from_trip($trip_number, $dis_id) {
  $query = db_select('node', 'n');
  $query->condition('n.title', $trip_number);
  $query->condition('n.type', 'dispatch_data');
  $query->condition('n.uid', $dis_id);
  $query->leftjoin('field_data_field_confirm_bill_edit_status', 'bs', 'bs.entity_id = n.nid');
  $query->fields('bs', array('field_confirm_bill_edit_status_value'));
  $status = $query->execute()->fetchField();
  return $status;
}

function confirm_bill_editing_status_undo_submit($form, &$form_state) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);
  $trip_number = $form_state['values']['trip_number'];
  
 //excess table versioning 
  create_version_excess_qty_table($trip_number, $dis_id);
  
  //delete from excess qty as remaining may ledt in excess table.
  db_delete('excess_qty')
    ->condition('trip_id', $trip_number, '=')
    ->condition('distributor_id', $dis_id, '=')
    ->execute();
  
  //difference table versioning 
  create_version_difference_qty_table($trip_number, $dis_id);
  
  //delete from difference qty as remaining may ledt in difference table.
  db_delete('difference_qty')
    ->condition('trip_id', $trip_number, '=')
    ->condition('distributor_id', $dis_id, '=')
    ->execute();
  
  //unset status
  $nid = get_nid_from_title($trip_number, 'dispatch_data');
  $node = node_load($nid);
  //$status = $node->field_dispatch_status['und'][0]['value'];
  //if ($status != 1) {
    $node->field_confirm_bill_edit_status['und'][0]['value'] = 0;
    unset($node->field_ikea_bill_editing_status['und']);
    unset($node->field_confirmed_by['und']);
    node_save($node);
    drupal_set_message(t('Trip id ' . $trip_number . ' status confirmation is undone'));
  //}
//  else{
//    drupal_set_message(t('Trip id ' . $trip_number . ' status confirmation cannot be undone as trip is already dispatched'),'error');
//  }
}

function get_bills_from_trip($trip_id, $dis_id){
  $query = db_select('dispatch_qty_confirmation', 'dqp');
  $query->fields('dqp', array('bill_number'));
  $query->condition('dqp.distributor_id', $dis_id);
  $query->condition('dqp.trip_id', $trip_id);
  $bills = $query->distinct()->execute()->fetchCol();
  return $bills;
}


function check_bill_reupload_complete($bills, $distributor_id) {
  if (!empty($bills)) {
    $query = db_select('node', 'n');
    $query->condition('n.type', "sales_register_data");
    $query->condition('n.uid', "$distributor_id");
    $query->condition('n.title', $bills, 'IN');
    $query->join('field_data_field_reupload_sales_flag', 'flag', 'flag.entity_id = n.nid');
    $query->condition('flag.field_reupload_sales_flag_value', 1, '=');
    $query->addField('n', 'title');
    $reuploaded_bills = $query->execute()->fetchAllKeyed(0, 0);
  }
  return $reuploaded_bills;
}

function get_bill_not_edited_reason_default($trip_id, $bill_number, $sku_code, $dis_id) {
  $query = db_select('difference_qty', 'eq');
  $query->condition('eq.trip_id', $trip_id);
  $query->condition('eq.bill_number', $bill_number);
  $query->condition('eq.sku_code', $sku_code);
  $query->condition('eq.distributor_id', $dis_id);
  $query->fields('eq', array('reason'));
  $reason_code = $query->execute()->fetchField();
  return $reason_code;
}
