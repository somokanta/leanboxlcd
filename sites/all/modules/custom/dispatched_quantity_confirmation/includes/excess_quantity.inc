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
function excess_qty_form($form, &$form_state) {
   global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);
    
  $form['trip_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Trip ID'),
    '#required' => TRUE,
  );

  $form['bill_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Bill Number'),
    '#required' => TRUE,
  );
  $form['sku_prod_name'] = array(
    '#type' => 'textfield',
    '#title' => t('SKU code/Product name'),
    '#required' => TRUE,
  );
  $reason_options = array('' => 'Select');
  $dispatch_short_reason_options = get_excess_reason_list($dis_id);;
  if (!empty($dispatch_short_reason_options)) {
    $reason_options = $reason_options + $dispatch_short_reason_options;
  }
  $form['reason_code'] = array(
    '#type' => 'select',
    '#title' => t('Reason Code'),
    '#options' => $reason_options,
  );
  $form['excess_qty'] = array(
    '#type' => 'textfield',
    '#title' => t('Excess Qty'),
  );
  
  $form['save'] = array(
    '#type' => 'submit',
    '#value' => 'Save',    
  );
  return $form;
}

function excess_qty_form_submit($form, $form_state) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);
  $trip_number = $form_state['values']['trip_number'];
  $bill_number = $form_state['values']['bill_number'];
  $sku_prod_name = $form_state['values']['sku_prod_name'];
  $reason_code = $form_state['values']['reason_code'];
  $excess_qty = $form_state['values']['excess_qty'];
  
  db_insert('excess_qty')
    //->key(array('trip_id' => $trip_number, 'bill_number' => $bill, 'sku_prod_name' => $sku_prod_name))
    ->fields(array(
      'trip_id' => $trip_number,
      'bill_number' => $bill_number,
      'sku_prod_name' => $sku_prod_name,
      'reason_code' => $reason_code,
      'excess_qty ' => $excess_qty,
      'created_date ' => REQUEST_TIME,
      'distributor_id' => $dis_id,
    ))
    ->execute();

  drupal_set_message(t("Data saved successfully."));
}