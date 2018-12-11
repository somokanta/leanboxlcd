<?php

function drs_invoice_vs_actual_dispatched_report_form($form, &$form_state) {

  $form['#prefix'] = '<div id="itemlist-div">';
  $form['#suffix'] = '</div>';

  $form['trip_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Trip Id'),
    '#size' => 10,
    '#id' => 'drs-report2'
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('submit'),
    '#submit' => array('drs_invoice_vs_actual_dispatched_report_form_submit'),
    '#validate' => array('drs_invoice_vs_actual_dispatched_report_form_validate'),
  );
  
  if (!empty($form_state['html'])) {
    $form['drs-invoice-vs-actual-dispatched-report-form'] = array(
      '#markup' => $form_state['html'],
    );
  }
  
  return $form;
}

function drs_invoice_vs_actual_dispatched_report_form_validate($form, &$form_state) {
  
  $trip_id = $form_state['values']['trip_id'];
  $nid = get_nid_from_title($trip_id, 'dispatch_data');
  $node = node_load($nid);
  
  if(empty($trip_id)) {
    form_set_error('form', t("Trip Id field is required."));
  }
  if($node){
    if ($node->field_confirm_bill_edit_status['und'][0]['value'] != 1) {
      form_set_error('form', t("Bill Confirmation status is not completed for trip: " . $trip_id));
      drupal_goto('drs-invoice-vs-actual-dispatched-report');
    }
  }else{
    form_set_error('form', t("Trip not exists: " . $trip_id));
    drupal_goto('drs-invoice-vs-actual-dispatched-report');
  }
}

function drs_invoice_vs_actual_dispatched_report_form_submit($form, &$form_state) { 
  global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);
  $trip_id = $form_state['values']['trip_id'];
  $drs_table = drs_invoice_vs_actual_dispatched_report_table($trip_id, $distributor_id);
 
  $form_state['html'] = $drs_table;
  $form_state['rebuild'] = TRUE;
}

