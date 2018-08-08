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
function delivery_confirmation_edit_form($form, &$form_state) {
  $form['#prefix'] = '<div id="delete-data">';
  $form['#suffix'] = '</div>';

  $form['trip_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Trip ID'),
    '#size' => 10,
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Check'),
  );

  $header = array('Trip ID', 'Version Number', 'Bill Count', 'Full Delivered Bill Count', 'Full Returned Bill Count', 'Partial Delivered Bill Count','Reattempt Bill Count');

  if (!isset($form_state['delivery_confirmation']['bills'])) {
    $form_state['delivery_confirmation']['bills'] = array();
  }
  $options = $form_state['delivery_confirmation']['bills'];

  if (!empty($options)) {
    $trip_number = $options[0]['title'];

    $form['bill_details'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $options,
    );

    $form['submit1'] = array(
      '#type' => 'submit',
      '#value' => t('Confirm Delete'),
      '#submit' => array('delivery_confirmation_submit'),
      '#attributes' => array(
        'onclick' => 'if(!confirm("Do you want to Delete version of Trip ID: ' . $trip_number . '?")){return false;}',
        'style' => array('float: right;')
    ));
  }

  return $form;
}

/**
 * 
 * @param type $form
 * @param type $form_state
 * @return type
 */
function delivery_confirm_edit_form_callback($form, &$form_state) {
  return $form;
}

/**
 * 
 * @param type $form
 * @param type $form_state
 * @return type
 */
function delivery_confirmation_edit_form_submit($form, &$form_state) {
  global $user;
  $distributor_id = change_feeds_import_get_distributor_id($user->uid);
  $trip_number = $form_state['values']['trip_number'];

  $query = db_select('node', 'n');
  $query->condition('n.uid', $distributor_id, '=');
  $query->condition('n.type', 'dispatch_data', '=');
  $query->leftjoin('field_data_field_vrs_status', 'vs', 'vs.entity_id = n.nid');

  $query->condition('n.title', $trip_number, '=');
  $query->condition('vs.field_vrs_status_value', '0', '=');

  //$query->leftjoin('field_data_field_returns_closure_id', 'ci', 'ci.entity_id = n.nid');
  $query->leftjoin('field_data_field_delivery_version', 'dv', 'dv.entity_id = n.nid');

  $query->join('dispatch_planning_api_data', 'dpi', "dpi.runsheet_number = n.title and dpi.distributor_id = $distributor_id");
  $query->groupBy('n.title');

  $query->fields('n', array('title'));
  //$query->fields('ci', array('field_returns_closure_id_value'));
  $query->fields('dv', array('field_delivery_version_value'));
  $query->addExpression('COUNT(dpi.bill_number)', 'bill_count');
  $query->addExpression("SUM(CASE WHEN (dpi.res_status = 'deliver') THEN 1 ELSE 0 END)", 'full_delivered_bill_count');
  $query->addExpression("SUM(CASE WHEN (dpi.res_status = 'undeliver' and dpi.delivery_pending_flag != 1) THEN 1 ELSE 0 END)", 'full_returned_bill_count');
  $query->addExpression("SUM(CASE WHEN (dpi.res_status = 'partial_deliver') THEN 1 ELSE 0 END)", 'partial_delivered_bill_count');
  $query->addExpression("SUM(CASE WHEN (dpi.delivery_pending_flag = 1) THEN 1 ELSE 0 END)", 'delivery_pending_bill_count');
  $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

  $form_state['delivery_confirmation']['bills'] = $res;
  $form_state['rebuild'] = TRUE;
}

/**
 * 
 * @param type $form
 * @param type $form_state
 * @return type
 */
function delivery_confirmation_edit_form_validate($form, &$form_state) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);

  $trip_number = $form_state['values']['trip_number'];

  $check_trip_nid = check_trip_id_exists_or_not($dis_id, $trip_number);
  $vrs_status = $check_trip_nid->field_vrs_status_value;

  if ($vrs_status == -1) {
    form_set_error('trip_number', 'This trip is not confirmed yet.It is in delivery confirmation pending status.');
  }

  if ($vrs_status == 1) {
    form_set_error('trip_number', 'This trip is Returns Confirmed. So first delete from Returns Confirmed.');
  }

  if ($vrs_status == 2 ) {
    form_set_error('trip_number', 'This trip is VRS Confirmed. So first delete from VRS Confirmed.');
  }
  
  if ($vrs_status == 3 ) {
    form_set_error('trip_number', 'This trip is VRS Pending. So first delete from VRS Pending.');
  }
  
  if ($vrs_status == 4 ) {
    form_set_error('trip_number', 'This trip is Handover Pending. So first delete from Handover Pending.');
  }
  
  if ($vrs_status == 5 ) {
    form_set_error('trip_number', 'This trip is Handover Confirmed. So first delete from Handover Confirmed.');
  }
}

/**
 * 
 * @param type $form
 * @param type $form_state
 * @return type
 */
function delivery_confirmation_submit($form, $form_state) {
  global $user;
  $distributor_id = change_feeds_import_get_distributor_id($user->uid);

  $trip_number = $form_state['values']['trip_number'];

  $trip_nid = check_trip_id_exists_or_not($distributor_id, $trip_number);
  if (!empty($trip_nid->nid)) {
    $node = node_load($trip_nid->nid);
    // Status updated to 'Delivery Confirmation Pending'
    $node->field_vrs_status['und'][0]['value'] = -1;

    node_save($node);
  }
  drupal_set_message(t("Inventory delivery deleted successfully for Trip ID: $trip_number."));
}
