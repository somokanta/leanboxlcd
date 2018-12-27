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
function godown_short_approval_delete_form($form, &$form_state) {
  $form['#attached']['css'][] = drupal_get_path('module', 'leanbox_new_sku_wise_return') . '/css/leanbox_new_sku_wise_return.css';

  $form['#prefix'] = '<div id="itemlist-div">';
  $form['#suffix'] = '</div>';

  $form['trip_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Trip ID'),
    '#required' => TRUE,
    '#autocomplete_path' => 'sku-wise-trip/autocomplete',
    '#prefix' => '<div class="form_row form_row2"><div class="trip_id_outer_width">',
    '#suffix' => '</div>'
  );

  $form['delete'] = array(
    '#type' => 'submit',
    '#value' => 'Delete',
    '#prefix' => '<div class="apply_btn">',
    '#suffix' => '</div>',
    '#submit' => array('godown_short_approval_delete_submit'),
    '#validate' => array('trip_id_status_validation'),
  );
  return $form;
}

// Check Trip status.
function trip_id_status_validation($form, &$form_state) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);
  $form_state['trip_no_error'] = '';
  $trip_number = $form_state['input']['trip_number'];
  $form_state['trip_no_error'] = 0;
  $trip_detail = check_trip_id_exists_or_not($dis_id, $trip_number);
  $vrs_status = $trip_detail->field_vrs_status_value;
  $declared_claimed_godown_short = declared_claimed_godown_short($trip_number, $dis_id);
  if (empty($declared_claimed_godown_short)) {
    drupal_set_message(t('You cannot delete because Declared Claimed Godown Short is 0.'), 'error');
    $form_state['trip_no_error'] = 1;
  }
  if ($vrs_status == 2) {
    drupal_set_message(t('You cannot delete because this trip status is VRS confirmed.'), 'error');
    $form_state['trip_no_error'] = 1;
  }
}

function godown_short_approval_delete_submit($form, &$form_state) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);
  $trip_id = $form_state['values']['trip_number'];
  if (empty($form_state['trip_no_error'])) {
    $query = db_select('dispatch_planning_api_data', 'dpi');
    $query->condition('dpi.distributor_id', $dis_id, '=');
    $query->condition('dpi.runsheet_number', $trip_id, '=');
    $query->join('dispatch_planning_item_details', 'ad', 'ad.trip_bill_reference = dpi.id');
    $query->condition('ad.declared_claimed_godown_short', 0, '>');
    $query->addExpression('ad.declared_claimed_godown_short', 'declared_claimed_godown_short');
    $query->addExpression('ad.claimed_godown_short', 'claimed_godown_short');
    $query->addExpression('ad.godown_short_reason', 'godown_short_reason');
    $query->addExpression('ad.res_cashier_short', 'cashier_short');
    $query->addExpression('ad.res_godown_short', 'godown_short');
    $query->addExpression('dpi.id', 'dpi_id');
    $query->addExpression('dpi.bill_number', 'bill_number');
    $query->addExpression('ad.sku_code', 'sku_code');
    $query->addExpression('ad.approved_by', 'approved_by');
    $query->addExpression('ad.id', 'item_id');
    $result = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

    foreach ($result as $value) {
      $claimed_godown_short = $value['declared_claimed_godown_short'];
      $item_id = $value['item_id'];
      $godown_short_reason = '';
      $cashier_short = 0;
      $godown_short = 0;
      $sku_code = $value['sku_code'];
      db_update('dispatch_planning_item_details') // Table name no longer needs {}
        ->fields(array(
          'res_cashier_short' => $cashier_short,
          'res_godown_short' => $godown_short,
          'claimed_godown_short' => $claimed_godown_short,
          'godown_short_reason' => $godown_short_reason,
          'approved_quantity' => 0,
          'approved_by' => '',
          'approval_flag' => '',
        ))
        ->condition('sku_code', $sku_code, '=')
        ->condition('id', $item_id, '=')
        ->execute();
    }
    drupal_set_message(t("Data deleted successfully."));
  }
}

// Check Declared Claimed Godown Short is 0.
function declared_claimed_godown_short($trip_id, $dis_id) {
  $query = db_select('dispatch_planning_api_data', 'dpi');
  $query->condition('dpi.distributor_id', $dis_id, '=');
  $query->condition('dpi.runsheet_number', $trip_id, '=');
  $query->join('dispatch_planning_item_details', 'ad', 'ad.trip_bill_reference = dpi.id');
  $query->condition('ad.declared_claimed_godown_short', 0, '>');
  $query->addExpression('ad.declared_claimed_godown_short', 'declared_claimed_godown_short');
  $result = $query->execute()->fetchField();
  
  return $result;
}
