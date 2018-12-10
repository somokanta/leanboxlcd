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
function godown_short_approval_form($form, &$form_state) {
  $form['#attached']['js'][] = drupal_get_path('module', 'leanbox_new_sku_wise_return') . '/js/leanbox_new_sku_wise_return.js';
  $form['#attached']['css'][] = drupal_get_path('module', 'leanbox_new_sku_wise_return') . '/css/leanbox_new_sku_wise_return.css';
  global $user;
  $distributor_id = change_feeds_import_get_distributor_id($user->uid);

  $form['#prefix'] = '<div id="itemlist-div">';
  $form['#suffix'] = '</div>';

  $form['trip_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Trip ID'),
    '#required' => TRUE,
    '#autocomplete_path' => 'sku-wise-trip/autocomplete',
    '#ajax' => array(
      'wrapper' => 'itemlist-div',
      'callback' => 'short_approval_callback',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    '#prefix'=>'<div class="form_row form_row2"><div class="trip_id_outer_width">',
    '#validate' => array('trip_id_validation'),
    '#suffix'=>'</div>'
  );
  $link = $rr = l('View Pending for Approval', "claimed-gs-report", array('attributes' => array("target" => "_blank"),));
  $form['view_pending_gs_approval_link'] = array(
    '#type' => 'markup',
    '#prefix'=>'<div class="gs_approval_link">'.$link,
    '#suffix'=>'</div></div>'
  );
  $form['apply'] = array(
    '#type' => 'submit',
    '#value' => 'Apply',
    '#ajax' => array(
      'callback' => 'short_approval_callback',
      'wrapper' => 'itemlist-div',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    '#prefix'=>'<div class="apply_btn">',
    '#suffix'=>'</div>',
    '#submit' => array('godown_short_approval_submit'),
  );
  $header = array('Bill Number',
    'Product Name',
    'SKU Code',
    'MRP',
    'Total Sales Qty',
    'Godown Short',
    'Dispatch Qty',
    'Returned Qty',
    'Cashier Short',
    'Claimed Godown Short Qty',
    'Approved Quantity',
    'Approval Flag',
    'Reason',
    'Approved By',
    'Remarks',
    'bill_details' => array('data' => t('bill_details'), 'class' => array('bill_no_details hide-col'))
    );

  if (!isset($form_state['godown_short_approval']['bills'])) {
    $form_state['godown_short_approval']['bills'] = array();
  }
  $options = $form_state['godown_short_approval']['bills'];

  if (!empty($options)) {
    $form['bill_details'] = array(
      '#theme' => 'table',
      '#tree' => TRUE,
      '#header' => $header,
      '#rows' => array(),
      '#prefix' => '<div class="custom_middle_table">',
      '#suffix' => '</div>',
    );

    /*   $form['save'] = array(
      '#type' => 'submit',
      '#value' => 'Save',
      //      '#ajax' => array(
      //        'callback' => 'short_approval_callback',
      //        'wrapper' => 'itemlist-div',
      //        'method' => 'replace',
      //        'effect' => 'fade',
      //      ),
      '#validate' => array('save_godown_short_validate'),
      '#submit' => array('save_godown_short_submit'),
      );

     */

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => 'Submit',
//      '#ajax' => array(
//        'callback' => 'short_approval_callback',
//        'wrapper' => 'itemlist-div',
//        'method' => 'replace',
//        'effect' => 'fade',
//      ),
      '#prefix'=>'<div class="apply_btn">',
      '#suffix'=>'</div>',
      '#validate' => array('godown_short_validate'),
      '#submit' => array('godown_short_submit'),
    );
  }

  foreach ($options as $key => $val) {
    $form['bill_details']['#rows'][$key][] = $val['bill_number'];
    $form['bill_details']['#rows'][$key][] = $val['product_name'];
    $form['bill_details']['#rows'][$key][] = $val['sku_code'];
    $form['bill_details']['#rows'][$key][] = $val['mrp'];
    $form['bill_details']['#rows'][$key][] = $val['sum_net_sales_quantity'];
    //$form['bill_details']['#rows'][$key][] = $val['res_godown_short'];
    $res_godown_short = array(
      '#type' => 'textfield',
      '#size' => 10,
      '#attributes' => array('class' => array("res-godown-short_$key"), 'readonly' => 'readonly'),
      '#default_value' => $val['res_godown_short'],
    );
    $form['bill_details'][$key]["res_godown_short"] = &$res_godown_short;
    $form['bill_details']['#rows'][$key][]['data']["res_godown_short"] = &$res_godown_short;

    $dispatch_qty = array(
      '#type' => 'textfield',
      '#size' => 10,
      '#attributes' => array('class' => array("dispatch-qty_$key"), 'readonly' => 'readonly'),
      '#default_value' => $val['dispatch_qty'],
    );
    $form['bill_details'][$key]["dispatch_qty"] = &$dispatch_qty;
    $form['bill_details']['#rows'][$key][]['data']["dispatch_qty"] = &$dispatch_qty;

    //$form['bill_details']['#rows'][$key][] = $val['dispatch_qty'];

    $form['bill_details']['#rows'][$key][] = $val['return_qty'];
    //$form['bill_details']['#rows'][$key][] = $val['res_cashier_short'];
    $res_cashier_short = array(
      '#type' => 'textfield',
      '#size' => 10,
      '#attributes' => array('class' => array("res-cashier-short_$key"), 'readonly' => 'readonly'),
      '#default_value' => $val['res_cashier_short'],
    );

    $claimed_godown_short = array(
      '#type' => 'textfield',
      '#size' => 10,
      '#attributes' => array('class' => array("claimed-godown-short_$key"), 'readonly' => 'readonly'),
      '#default_value' => $val['claimed_godown_short'],
    );

    $flag_value = $form_state['values']['bill_details'][$key]['approval_flag'];

    if ($val['approval_flag'] == 'no' && $flag_value != 'yes') {
      $approved_qty = array(
        '#type' => 'textfield',
        '#attributes' => array('class' => array("approved-qty_$key",'gs_approved_qty'), 'readonly' => 'readonly','data-key'=>$key),
        '#size' => 10,
        //'#default_value' => $val['approved_quantity'],
      );
    }
    else {
      $approved_qty = array(
        '#type' => 'textfield',
        '#attributes' => array('class' => array("approved-qty_$key",'gs_approved_qty'),'data-key'=>$key),
        '#size' => 10,
        //'#default_value' => $val['approved_quantity'],
      );
    }

    $approval_flag = array(
      '#type' => 'select',
      '#options' => array('' => 'Select', 'yes' => 'Yes', 'no' => 'No'),
      //'#default_value' => $val['approval_flag'],
      '#ajax' => array(
        'callback' => 'short_approval_callback_select',
        'wrapper' => 'itemlist-div',
        'method' => 'replace',
        'effect' => 'fade',
      ),
      '#attributes' => array('class' => array("approval_flag flag_data_$key"), 'data' => $key),
    );

    $initial_option = array('' => 'Select');
    if ($flag_value == 'yes') {
      $flag = 'yes';
      $reason_list = get_godown_short_reason_list($distributor_id, $flag);
      $reason = array(
        '#type' => 'select',
        '#options' => $initial_option + $reason_list,
      );
    }
    else if ($flag_value == 'no') {
      $flag = 'no';
      $reason_list = get_godown_short_reason_list($distributor_id, $flag);
      $reason = array(
        '#type' => 'select',
        '#options' => $initial_option + $reason_list,
      );
    }
//    else if($val['approval_flag'] == 'yes') {
//      $flag = 'yes';
//      $reason_list = get_godown_short_reason_list($distributor_id, $flag);
//      $yes_default_value = $val['godown_short_reason'];
//      $reason = array(
//        '#type' => 'select',
//        '#options' => $initial_option + $reason_list,
//        //'#default_value' => $yes_default_value,
//      );
//    }   
//    else if($val['approval_flag'] == 'no') {
//      $flag = 'no';
//      $reason_list = get_godown_short_reason_list($distributor_id, $flag);
//      $no_default_value = $val['godown_short_reason'];
//      $reason = array(
//        '#type' => 'select',
//        '#options' => $initial_option + $reason_list,
//        //'#default_value' => $no_default_value,
//      );
//    }

    $approved_by = array(
      '#type' => 'textfield',
      '#attributes' => array('class' => array('approved-by')),
      '#size' => 10,
      //'#default_value' => $val['approved_by'],
    );

    $remarks = array(
      '#type' => 'textfield',
      '#attributes' => array('class' => array('remarks')),
      '#size' => 10,
      //'#default_value' => $val['remarks'],
    );

    $form['bill_details'][$key]["res_cashier_short"] = &$res_cashier_short;
    $form['bill_details'][$key]["claimed_godown_short"] = &$claimed_godown_short;
    $form['bill_details'][$key]["approved_qty"] = &$approved_qty;
    $form['bill_details'][$key]["approval_flag"] = &$approval_flag;
    $form['bill_details'][$key]["reason"] = &$reason;
    $form['bill_details'][$key]["approved_by"] = &$approved_by;
    $form['bill_details'][$key]["remarks"] = &$remarks;

    $form['bill_details']['#rows'][$key][]['data']["res_cashier_short"] = &$res_cashier_short;
    $form['bill_details']['#rows'][$key][]['data']["claimed_godown_short"] = &$claimed_godown_short;
    $form['bill_details']['#rows'][$key][]['data']["approved_qty"] = &$approved_qty;
    $form['bill_details']['#rows'][$key][]['data']["approval_flag"] = &$approval_flag;
    $form['bill_details']['#rows'][$key][]['data']["reason"] = &$reason;
    $form['bill_details']['#rows'][$key][]['data']["approved_by"] = &$approved_by;
    $form['bill_details']['#rows'][$key][]['data']["remarks"] = &$remarks;

    $form['bill_details']['#rows'][$key]['bill_no']['data'] = '';
    $form['bill_details']['#rows'][$key]['bill_no']['class'] = array("bill_no_details_$key hide-col");
    $form['bill_details']['#rows'][$key]['bill_no']['data-value'] = json_encode($val);

    unset($res_godown_short);
    unset($dispatch_qty);
    unset($res_cashier_short);
    unset($claimed_godown_short);
    unset($approved_qty);
    unset($approval_flag);
    unset($reason);
    unset($approved_by);
    unset($remarks);
  }

  if ($form_state['triggering_element']['#value'] == 'Apply') {
    $form_state['apply_clicked'] = 1;
  }

  if (empty($options) && ($form_state['apply_clicked'] == 1)) {
    drupal_set_message(t("No Result Found"), 'error');
  }

  return $form;
}

/**
 * 
 * @param type $form
 * @param type $form_state
 * @return array
 */
function short_approval_callback($form, &$form_state) {
  return $form;
}

function short_approval_callback_select($form, &$form_state) {
  $trigered_element = $form_state['triggering_element']['#name'];
  $trigered_element = str_replace("]", "[", $trigered_element);
  $trigered_element = explode('[', $trigered_element);

  // Used for removing drupal set message on submit.
  $commands[] = ajax_command_replace('#itemlist-div', render($form));
  $commands[] = array
    (
    'command' => 'claimed_short_to_approval',
    'selectedValue' => $trigered_element[1],
  );
  return array('#type' => 'ajax', '#commands' => $commands);
}

function trip_id_validation($form, &$form_state) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);
  $form_state['trip_no_error'] = '';
  $trip_number = $form_state['input']['trip_number'];

  $trip_detail = check_trip_id_exists_or_not($dis_id, $trip_number);
  $vrs_status = $trip_detail->field_vrs_status_value;

  if ($vrs_status == 2) {
    drupal_set_message(t('You cannot edit because this trip is VRS confirmed.'), 'error');
    $form_state['trip_no_error'] = 1;
  }
  /*else if ($vrs_status == 3) {
    drupal_set_message(t('You cannot edit because this trip is VRS pending.'), 'error');
    $form_state['trip_no_error'] = 1;
  }*/
  else if ($vrs_status == 4) {
    drupal_set_message(t('You cannot edit because this trip is Handover pending.'), 'error');
    $form_state['trip_no_error'] = 1;
  }
  else if ($vrs_status == 5) {
    drupal_set_message(t('You cannot edit because this trip is Handover confirmed.'), 'error');
    $form_state['trip_no_error'] = 1;
  }
}

function godown_short_approval_submit($form, &$form_state) {
  if ($form_state['trip_no_error'] != 1) {
    global $user;
    $dis_id = change_feeds_import_get_distributor_id($user->uid);

    $trip_id = $form_state['values']['trip_number'];

    $query = db_select('dispatch_planning_item_details', 'dpi');
    $query->join('dispatch_planning_api_data', 'ad', 'dpi.trip_bill_reference = ad.id');
    $query->condition('ad.delivery_pending_flag', 1, '!=');
    $query->condition('ad.runsheet_number', $trip_id);
    $query->condition('ad.distributor_id', $dis_id);
    $query->condition('dpi.line_level_status', 'line_deliver', '!=');
    $query->condition('dpi.claimed_godown_short', 0, '>');
    $query->fields('ad', array('bill_number'));
    $query->fields('dpi', array('product_name'));
    $query->fields('dpi', array('sku_code'));
    $query->fields('dpi', array('mrp'));
    $query->fields('dpi', array('sum_net_sales_quantity'));
    $query->fields('dpi', array('res_godown_short'));
    $query->fields('dpi', array('dispatch_qty'));
    $query->fields('dpi', array('return_qty'));
    $query->fields('dpi', array('res_cashier_short'));
    $query->fields('dpi', array('claimed_godown_short'));
    $query->fields('dpi', array('approved_quantity'));
    $query->fields('dpi', array('approval_flag'));
    $query->fields('dpi', array('godown_short_reason'));
    $query->fields('dpi', array('approved_by'));
    $query->fields('dpi', array('remarks'));
    $result = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

    $form_state['godown_short_approval']['bills'] = $result;
  }

  $form_state['rebuild'] = TRUE;
}

/*
  function save_godown_short_validate($form, &$form_state) {
  $form_state['approval_error'] = 0;
  foreach ($form_state['input']['bill_details'] as $k => $value) {
  $approved_by = $value['approved_by'];
  $approved_flag = $value['approval_flag'];
  $claimed_godown_short = $value['claimed_godown_short'];
  $approved_qty = $value['approved_qty'];
  $bill_number = $form_state['godown_short_approval']['bills'][$k]['bill_number'];

  if (empty($approved_flag)) {
  drupal_set_message(t("Please enter the approved flag for bill no : $bill_number"), 'error');
  $form_state['approval_error'] = 1;
  }

  if (empty($approved_by)) {
  drupal_set_message(t("Please enter the approved by name for bill no : $bill_number"), 'error');
  $form_state['approval_error'] = 1;
  }

  if ($approved_qty > $claimed_godown_short) {
  drupal_set_message(t("Approved qty cannot be greater than claimed godown short for bill no : $bill_number"), 'error');
  $form_state['approval_error'] = 1;
  }
  }

  if ($form_state['approval_error'] == 1) {
  $form_state['rebuild'] = TRUE;
  }
  }

  function save_godown_short_submit($form, &$form_state) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);

  $trip_id = $form_state['values']['trip_number'];

  foreach ($form_state['godown_short_approval']['bills'] as $key => $value) {
  $bill_number = $value['bill_number'];
  $sku_code = $value['sku_code'];

  $claimed_godown_short = $form_state['input']['bill_details'][$key]['claimed_godown_short'];
  $approved_qty = $form_state['input']['bill_details'][$key]['approved_qty'];
  $approval_flag = $form_state['input']['bill_details'][$key]['approval_flag'];
  $reason = $form_state['input']['bill_details'][$key]['reason'];
  $approved_by = $form_state['input']['bill_details'][$key]['approved_by'];
  $remarks = $form_state['input']['bill_details'][$key]['remarks'];

  $query = db_select('dispatch_planning_item_details', 'dpi');
  $query->join('dispatch_planning_api_data', 'ad', 'dpi.trip_bill_reference = ad.id');
  $query->addfield('ad', 'res_status', 'res_status');
  $query->fields('dpi', array('trip_bill_reference'));
  $query->fields('dpi', array('id'));
  $query->fields('dpi', array('res_cashier_short'));
  $query->fields('dpi', array('res_godown_short'));
  $query->condition('dpi.sku_code', $sku_code, '=');
  $query->condition('ad.bill_number', $bill_number, '=');
  $query->condition('ad.distributor_id', $dis_id, '=');
  $query->condition('ad.runsheet_number', $trip_id, '=');
  $itdatas = $query->execute()->fetchAll();
  $itdata = $itdatas[0];

  if ($approval_flag == 'yes') {
  if ($claimed_godown_short == $approved_qty) {
  $godown_short = $approved_qty;
  }
  else {
  $godown_short = $approved_qty;
  $cashier_short = ($claimed_godown_short - $approved_qty);
  }
  }
  else if ($approval_flag == 'no') {
  $cashier_short = $approved_qty;
  }

  $final_godown_short = ($itdata->res_godown_short + $godown_short);
  $final_cashier_short = ($itdata->res_cashier_short + $cashier_short);

  if ($itdata->res_status == 'undeliver') {
  $ret_cash = $value['return_qty'] + $final_cashier_short; //Return Qty + Cashier Short
  $ret_cash_godown = $ret_cash + $final_godown_short; //Return Qty + Cashier Short + claimed Godown Short
  $line_status = '';
  if ($ret_cash_godown == $value['dispatch_qty']) {
  $line_status = 'line_undeliver';
  }
  else if ($ret_cash_godown > 0) {
  $line_status = 'line_partial_deliver';
  }
  else if ($ret_cash_godown == 0) {
  $line_status = 'line_deliver';
  }

  db_update('dispatch_planning_item_details') // Table name no longer needs {}
  ->fields(array(
  'res_sku_actual_quantity1' => $value['dispatch_qty'] - $value['return_qty'] - $final_cashier_short - $final_godown_short,
  'return_qty' => $value['return_qty'],
  'dispatch_qty' => $value['sum_net_sales_quantity'] - $final_godown_short,
  'res_cashier_short' => $final_cashier_short,
  'res_godown_short' => $final_godown_short,
  'claimed_godown_short' => 0,
  'approved_quantity' => $approved_qty,
  'approval_flag' => $approval_flag,
  'godown_short_reason' => $reason,
  'approved_by' => $approved_by,
  'remarks' => $remarks,
  'line_level_status' => $line_status, // line level status
  ))
  ->condition('sku_code', $sku_code, '=')
  ->condition('id', $itdata->id, '=')
  ->execute();

  if ($line_status == 'line_undeliver') {
  db_update('dispatch_planning_api_data') // Table name no longer needs {}
  ->fields(array(
  'updated_flag' => 1,
  'updated_time' => REQUEST_TIME,
  ))
  ->condition('runsheet_number', $trip_id, '=')
  ->condition('id', $itdata->trip_bill_reference, '=')
  ->execute();
  }
  else {
  db_update('dispatch_planning_api_data') // Table name no longer needs {}
  ->fields(array(
  'res_status' => 'partial_deliver',
  'updated_flag' => 1,
  'updated_time' => REQUEST_TIME,
  ))
  ->condition('runsheet_number', $trip_id, '=')
  ->condition('id', $itdata->trip_bill_reference, '=')
  ->execute();
  }

  db_update('godown_data') // Table name no longer needs {}
  ->fields(array(
  'godown_short' => $final_godown_short,
  ))
  ->condition('trip_id', $trip_id, '=')
  ->condition('bill_number', $bill_number, '=')
  ->condition('sku_desc', $value['product_name'], '=')
  ->execute();
  }
  else if ($itdata->res_status == 'partial_deliver') {

  $ret_cash = $value['return_qty'] + $final_cashier_short; //Return Qty + Cashier Short
  $ret_cash_godown = $ret_cash + $final_godown_short; //Return Qty + Cashier Short + claimed Godown Short
  $line_status = '';
  if ($ret_cash_godown == $value['dispatch_qty']) {
  $line_status = 'line_undeliver';
  }
  else if ($ret_cash_godown > 0) {
  $line_status = 'line_partial_deliver';
  }
  else if ($ret_cash_godown == 0) {
  $line_status = 'line_deliver';
  }
  db_update('dispatch_planning_api_data') // Table name no longer needs {}
  ->fields(array(
  'updated_flag' => 1,
  'updated_time' => REQUEST_TIME,
  ))
  ->condition('runsheet_number', $trip_id, '=')
  ->condition('id', $itdata->trip_bill_reference, '=')
  ->execute();

  db_update('dispatch_planning_item_details') // Table name no longer needs {}
  ->fields(array(
  'res_sku_actual_quantity1' => $value['dispatch_qty'] - $value['return_qty'] - $final_cashier_short - $final_godown_short,
  'return_qty' => $value['return_qty'],
  'dispatch_qty' => $value['sum_net_sales_quantity'] - $final_godown_short,
  'res_cashier_short' => $final_cashier_short,
  'res_godown_short' => $final_godown_short,
  'claimed_godown_short' => 0,
  'approved_quantity' => $approved_qty,
  'approval_flag' => $approval_flag,
  'godown_short_reason' => $reason,
  'approved_by' => $approved_by,
  'remarks' => $remarks,
  'line_level_status' => $line_status, // line level status
  ))
  ->condition('sku_code', $sku_code, '=')
  ->condition('id', $itdata->id, '=')
  ->execute();

  db_update('godown_data') // Table name no longer needs {}
  ->fields(array(
  'godown_short' => $final_godown_short,
  ))
  ->condition('trip_id', $trip_id, '=')
  ->condition('bill_number', $bill_number, '=')
  ->condition('sku_desc', $value['product_name'], '=')
  ->execute();
  }
  }
  drupal_set_message(t("Data saved successfully."));
  //ajax_redirect('godown-short-approval');
  }
 * */

function godown_short_validate($form, &$form_state) {
  $mess = '';
  foreach ($form_state['input']['bill_details'] as $k => $value) {
    $approved_by = $value['approved_by'];
    $approved_flag = $value['approval_flag'];
    $claimed_godown_short = $value['claimed_godown_short'];
    $approved_qty = $value['approved_qty'];
    $bill_number = $form_state['godown_short_approval']['bills'][$k]['bill_number'];

    if (empty($approved_flag)) {
      $form_state['rebuild'] = TRUE;
       $mess.= "Please enter the approved flag for bill no : $bill_number" .'<br>';
    }

    if (empty($approved_by)) {
      $form_state['rebuild'] = TRUE;
      $mess.= "Please enter the approved by name for bill no : $bill_number".'<br>';
    }

    if ($approved_qty > $claimed_godown_short) {
      $form_state['rebuild'] = TRUE;
      $mess.="Approved qty cannot be greater than claimed godown short for bill no : $bill_number".'<br>';
    }
  }
  if (!empty($mess)) {
    form_set_error('form', $mess);
    $form_state['rebuild'] = TRUE;
  }
}

function godown_short_submit($form, &$form_state) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);

  $trip_id = $form_state['values']['trip_number'];

  foreach ($form_state['godown_short_approval']['bills'] as $key => $value) {
    $bill_number = $value['bill_number'];
    $sku_code = $value['sku_code'];

    $res_godown_short = $form_state['input']['bill_details'][$key]['res_godown_short'];
    $dispatch_qty = $form_state['input']['bill_details'][$key]['dispatch_qty'];
    $res_cashier_short = $form_state['input']['bill_details'][$key]['res_cashier_short'];
    $claimed_godown_short = $form_state['input']['bill_details'][$key]['claimed_godown_short'];
    $approved_qty = $form_state['input']['bill_details'][$key]['approved_qty'];
    $approval_flag = $form_state['input']['bill_details'][$key]['approval_flag'];
    $reason = $form_state['input']['bill_details'][$key]['reason'];
    $approved_by = $form_state['input']['bill_details'][$key]['approved_by'];
    $remarks = $form_state['input']['bill_details'][$key]['remarks'];

    $query = db_select('dispatch_planning_item_details', 'dpi');
    $query->join('dispatch_planning_api_data', 'ad', 'dpi.trip_bill_reference = ad.id');
    $query->addfield('ad', 'res_status', 'res_status');
    $query->fields('dpi', array('trip_bill_reference'));
    $query->fields('dpi', array('id'));
    $query->fields('dpi', array('res_cashier_short'));
    $query->fields('dpi', array('res_godown_short'));
    $query->fields('dpi', array('res_sku_reason'));
    $query->fields('dpi', array('dispatch_qty'));
    $query->condition('dpi.sku_code', $sku_code, '=');
    $query->condition('ad.bill_number', $bill_number, '=');
    $query->condition('ad.distributor_id', $dis_id, '=');
    $query->condition('ad.runsheet_number', $trip_id, '=');
    $itdatas = $query->execute()->fetchAll();
    $itdata = $itdatas[0];

//    if ($approval_flag == 'yes') {
//      if ($claimed_godown_short == $approved_qty) {
//        $godown_short = $approved_qty;
//      }
//      else if($claimed_godown_short > $approved_qty){
//        $godown_short = $approved_qty;
//        $cashier_short = ($claimed_godown_short - $approved_qty);
//      }
//    }
//    else if ($approval_flag == 'no') {
//      $cashier_short = $approved_qty;
//    }
    //$final_godown_short = ($itdata->res_godown_short + $godown_short);
    //$final_cashier_short = ($itdata->res_cashier_short + $cashier_short);

    if ($itdata->res_status == 'undeliver') {
      $ret_cash = $value['return_qty'] + $res_cashier_short; //Return Qty + Cashier Short  
      $ret_cash_godown = $ret_cash + $res_godown_short; //Return Qty + Cashier Short + Godown Short
      $line_status = '';
      if ($ret_cash_godown == $dispatch_qty) {
        $line_status = 'line_undeliver';
      }
      else if ($ret_cash_godown > 0) {
        $line_status = 'line_partial_deliver';
      }
      else if ($ret_cash_godown == 0) {
        $line_status = 'line_deliver';
      }

      db_update('dispatch_planning_item_details') // Table name no longer needs {}
        ->fields(array(
          'res_sku_actual_quantity1' => $itdata->dispatch_qty - $value['return_qty'] - $res_cashier_short - $res_godown_short,
          //$dispatch_qty replaced by $itdata->dispatch_qty
          //'return_qty' => $value['return_qty'],
          //'dispatch_qty' => $dispatch_qty, // as discussed with kusha
          'res_cashier_short' => $res_cashier_short,
          'res_godown_short' => $res_godown_short,
          'res_sku_reason' => $itdata->res_sku_reason,
          'claimed_godown_short' => 0,
          'approved_quantity' => $approved_qty,
          'approval_flag' => $approval_flag,
          'godown_short_reason' => $reason,
          'approved_by' => $approved_by,
          'remarks' => $remarks,
          //'line_level_status' => $line_status, //5DEC
        ))
        ->condition('sku_code', $sku_code, '=')
        ->condition('id', $itdata->id, '=')
        ->execute();

      if ($line_status == 'line_undeliver') {
        db_update('dispatch_planning_api_data') // Table name no longer needs {}
          ->fields(array(
            //'res_reason_for_failure' => $itdata->res_sku_reason ? $itdata->res_sku_reason : '',//5DEC
            'updated_flag' => 1,
            'updated_time' => REQUEST_TIME,
          ))
          ->condition('runsheet_number', $trip_id, '=')
          ->condition('id', $itdata->trip_bill_reference, '=')
          ->execute();
      }
      else {
        db_update('dispatch_planning_api_data') // Table name no longer needs {}
          ->fields(array(
            //'res_status' => 'partial_deliver',//5DEC
            //'res_reason_for_failure' => '',//5DEC
            'updated_flag' => 1,
            'updated_time' => REQUEST_TIME,
          ))
          ->condition('runsheet_number', $trip_id, '=')
          ->condition('id', $itdata->trip_bill_reference, '=')
          ->execute();
      }
//5DEC
//      db_update('godown_data') // Table name no longer needs {}
//        ->fields(array(
//          'godown_short' => $res_godown_short,
//        ))
//        ->condition('trip_id', $trip_id, '=')
//        ->condition('bill_number', $bill_number, '=')
//        ->condition('sku_desc', $value['product_name'], '=')
//        ->execute();
    }
    else if ($itdata->res_status == 'partial_deliver') {

      $ret_cash = $value['return_qty'] + $res_cashier_short; //Return Qty + Cashier Short  
      $ret_cash_godown = $ret_cash + $res_godown_short; //Return Qty + Cashier Short + Godown Short
      $line_status = '';
      if ($ret_cash_godown == $dispatch_qty) {
        $line_status = 'line_undeliver';
      }
      else if ($ret_cash_godown > 0) {
        $line_status = 'line_partial_deliver';
      }
      else if ($ret_cash_godown == 0) {
        $line_status = 'line_deliver';
      }
      db_update('dispatch_planning_api_data') 
        ->fields(array(
          'updated_flag' => 1,
          'updated_time' => REQUEST_TIME,
        ))
        ->condition('runsheet_number', $trip_id, '=')
        ->condition('id', $itdata->trip_bill_reference, '=')
        ->execute();

      db_update('dispatch_planning_item_details') // Table name no longer needs {}
        ->fields(array(
          'res_sku_actual_quantity1' => $itdata->dispatch_qty - $value['return_qty'] - $res_cashier_short - $res_godown_short,
          //$dispatch_qty replaced by $itdata->dispatch_qty
          //'return_qty' => $value['return_qty'],
          //'dispatch_qty' => $dispatch_qty,
          'res_cashier_short' => $res_cashier_short,
          'res_godown_short' => $res_godown_short,
          'res_sku_reason' => $itdata->res_sku_reason,
          'claimed_godown_short' => 0,
          'approved_quantity' => $approved_qty,
          'approval_flag' => $approval_flag,
          'godown_short_reason' => $reason,
          'approved_by' => $approved_by,
          'remarks' => $remarks,
          //'line_level_status' => $line_status,  //5DEC
        ))
        ->condition('sku_code', $sku_code, '=')
        ->condition('id', $itdata->id, '=')
        ->execute();

//5DEC
//      db_update('godown_data') // Table name no longer needs {}
//        ->fields(array(
//          'godown_short' => $res_godown_short,
//        ))
//        ->condition('trip_id', $trip_id, '=')
//        ->condition('bill_number', $bill_number, '=')
//        ->condition('sku_desc', $value['product_name'], '=')
//        ->execute();
    }
  }

  $get_trip_status = get_trip_status($dis_id, $trip_id);
  if ($get_trip_status == 1) {
    $trip_nid = check_trip_id_exists_or_not($dis_id, $trip_id);
    if (!empty($trip_nid->nid)) {
      $node = node_load($trip_nid->nid);
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
      node_save($node);
    }
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
      $val['version_flag'] = 2;
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
        'line_level_status',
        'claimed_godown_short',
        'claimed_godown_short_reason',
        'approved_quantity',
        'approval_flag',
        'godown_short_reason',
        'approved_by',
        'remarks',
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
  }
  drupal_set_message(t("Data submit successfully."));
  //ajax_redirect('godown-short-approval');
}
