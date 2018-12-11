<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function configurable_dispatch_qty_form($form, &$form_state) {

  $form['trip_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Trip ID'),
    '#size' => 10,
    // '#required' => TRUE,
    '#autocomplete_path' => 'get-all-trips/autocomplete',
  );

  $form['apply'] = array(
    '#type' => 'submit',
    '#value' => 'Apply',
    '#attributes' => array('class' => array('confirm-apply')),
    '#validate' => array('configurable_dispatch_qty_validate'),
    '#submit' => array('configurable_dispatch_qty_submit'),
  );

  if (!empty($form_state['trip_data'])) {

    $dispatch_validation_flag = field_info_field('field_dispatch_validation_flag');
    $dispatch_validation_flag = list_allowed_values($dispatch_validation_flag);
    $dispatch_validation_flag = array('' => 'Select') + $dispatch_validation_flag;

    $nid = get_nid_from_title($form_state['trip_data'], 'dispatch_data');
    $node = node_load($nid);
    $flag = $node->field_dispatch_validation_flag['und'][0]['value'];
    
    $form['dispatch_validation_flag'] = array(
      '#type' => 'select',
      '#title' => t('Dispatch Validation Flag'),
      '#required' => TRUE,
      '#options' => $dispatch_validation_flag,
      '#default_value' => $flag,
    );
    
    $form['msg'] = array(
      '#type' => 'markup',
      '#markup' => "By default validation will run on every trip<br>",
    );

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => 'Save',
      '#submit' => array('configurable_dispatch_qty_final_submit'),
    );
  }

  return $form;
}

function configurable_dispatch_qty_submit($form, &$form_state) {

  $trip_number = $form_state['input']['trip_number'];
  $form_state['trip_data'] = $trip_number;
  $form_state['rebuild'] = TRUE;
}

function configurable_dispatch_qty_validate($form, &$form_state) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);
  $trip_number = $form_state['values']['trip_number'];

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
    form_set_error('form', "Trip id is either invalid or dispatched.");
  }
}

function configurable_dispatch_qty_final_submit($form, &$form_state) {
  $dispatch_validation_flag = $form_state['values']['dispatch_validation_flag'];
  $trip_number = $form_state['values']['trip_number'];
  $nid = get_nid_from_title($trip_number, 'dispatch_data');
  $node = node_load($nid);
  $node->field_dispatch_validation_flag['und'][0]['value'] = $dispatch_validation_flag;
  if ($dispatch_validation_flag == 2) { //2 means bypass trip
    $node->field_loose_confirmation['und'][0]['value'] = 1;
    $node->field_cld_confirmation['und'][0]['value'] = 1;
    $node->field_confirm_bill_edit_status['und'][0]['value'] = 1;
  }
  node_save($node);
  
   drupal_set_message(t("Trip ID $trip_number - Updated Succesfully"));
}
