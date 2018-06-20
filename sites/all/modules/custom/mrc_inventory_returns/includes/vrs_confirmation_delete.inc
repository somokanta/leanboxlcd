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
function vrs_confirmation_delete_form($form, &$form_state) {
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

  $header = array('Trip ID', 'VRS Closure ID', 'Version Number', 'Bill Count','Full Delivered Bill Count', 'Full Returned Bill Count', 'Partial Delivered Bill Count');

  if (!isset($form_state['vrs_confirmation']['bills'])) {
    $form_state['vrs_confirmation']['bills'] = array();
  }
  $options = $form_state['vrs_confirmation']['bills'];

  if (!empty($options)) {
    $trip_number = $options[0]['title'];

    $form['bill_details'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $options,
    );

    $form['confirm_delete'] = array(
      '#type' => 'submit',
      '#value' => t('Confirm Delete'),
      '#submit' => array('vrs_confirmation_submit'),
      '#attributes' => array(
        'onclick' => 'if(!confirm("Do you want to Delete  Current VRS version of Trip ID: ' . $trip_number . '?")){return false;}',
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
function returns_confirm_edit_form_callback($form, &$form_state) {
  return $form;
}

/**
 * 
 * @param type $form
 * @param type $form_state
 * @return type
 */
function vrs_confirmation_delete_form_submit($form, &$form_state) {
  global $user;
  $vrs_status = array('3','2');
  $distributor_id = change_feeds_import_get_distributor_id($user->uid);
  $trip_number = $form_state['values']['trip_number'];

  $query = db_select('node', 'n');
  $query->condition('n.type', 'sales_register_data', '=');
  $query->condition('n.uid', $distributor_id, '=');
  $query->leftjoin('field_data_field_trip_id_reference', 'ti', 'ti.entity_id = n.nid');
  $query->leftjoin('node', 'disn', 'disn.nid = ti.field_trip_id_reference_nid');
  $query->condition('disn.type', 'dispatch_data');
  $query->leftjoin('field_data_field_vrs_status', 'vs', 'vs.entity_id = disn.nid');

  $query->condition('disn.title', $trip_number, '=');
  $query->condition('vs.field_vrs_status_value', $vrs_status, 'IN');

  $query->leftjoin('field_data_field_vrs_closure_id', 'ci', 'ci.entity_id = disn.nid');
  $query->leftjoin('field_data_field_vrs_version', 'rv', 'rv.entity_id = disn.nid');


  $query->join('dispatch_planning_api_data', 'dpi', 'dpi.bill_number = n.title');
  $query->groupBy('disn.title');

  $query->fields('disn', array('title'));
  $query->fields('ci', array('field_vrs_closure_id_value'));
  $query->fields('rv', array('field_vrs_version_value'));
  $query->addExpression('COUNT(n.title)', 'bill_count');
  $query->addExpression("SUM(CASE WHEN (dpi.res_status = 'deliver') THEN 1 ELSE 0 END)", 'full_delivered_bill_count');
  $query->addExpression("SUM(CASE WHEN (dpi.res_status = 'undeliver') THEN 1 ELSE 0 END)", 'full_returned_bill_count');
  $query->addExpression("SUM(CASE WHEN (dpi.res_status = 'partial_deliver') THEN 1 ELSE 0 END)", 'partial_delivered_bill_count');
  $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

  $form_state['vrs_confirmation']['bills'] = $res;
  $form_state['rebuild'] = TRUE;
}

/**
 * 
 * @param type $form
 * @param type $form_state
 * @return type
 */
function vrs_confirmation_delete_form_validate($form, &$form_state) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);

  $trip_id = $form_state['values']['trip_number'];

  if (!empty($trip_id)) {
        //1- Return confirmed, 3- Vrs pending
        // Checking VRS Status 
        /*0|Manual Edit
        1|Returns Confirmed
        2|VRS Confirmed
        3|VRS Pending
        4|Handover InProgress
        5|Handover Complete*/

        $check_returns_flag = check_trip_id_exists_or_not($dis_id, $trip_id);
        if (empty($check_returns_flag)) {
            form_set_error('trip_number', "Trip ID :$trip_id does not exists.");
        } 
        else if ($check_returns_flag->field_vrs_status_value == 4) {
            unset($form_state['vrs_confirmation']['bills']);
            form_set_error('trip_number', "Trip ID :$trip_id – is in Handover InProgress state.");
        }
        else if ($check_returns_flag->field_vrs_status_value == 5) {
            unset($form_state['vrs_confirmation']['bills']);
            form_set_error('trip_number', "Trip ID :$trip_id – is in Handover Complete state.");
        }
        else if ($check_returns_flag->field_vrs_status_value ==1 || empty($check_returns_flag->field_vrs_status_value) ) {
            unset($form_state['vrs_confirmation']['bills']);
            form_set_error('trip_number', "Trip ID:$trip_id-Please confirm Return Inventory first.");
        } 
       
    } else {
        form_set_error('trip_number', 'Please enter Trip ID.');
    }
  
}

/**
 * 
 * @param type $form
 * @param type $form_state
 * @return type
 */
function vrs_confirmation_submit($form, $form_state) {
  global $user;
  $distributor_id = change_feeds_import_get_distributor_id($user->uid);

  $trip_number = $form_state['values']['trip_number'];

  $trip_nid = check_trip_id_exists_or_not($distributor_id, $trip_number);
  if (!empty($trip_nid->nid)) {
    $node = node_load($trip_nid->nid);
    $node->field_vrs_status['und'][0]['value'] = 1;

    node_save($node);
  }
  drupal_set_message(t('VRS deleted successfully for Trip ID: '.$trip_number));
}
