<?php

/**
 * Files containing VRS Screen form for Trip returned from Farey Delivered Trip API
 */

/**
 * 
 * @param array $form
 * @param type $form_state
 * @return array
 */
function _delivered_tripwise_vrs_data_callback($form, &$form_state) {
   $form = array();
   $form['#prefix'] = '<div id="itemlist-div">';
   $form['#suffix'] = '</div>';
   $form['#attached']['js'][] = drupal_get_path('module', 'dispatch_planning') . '/js/dispatch_planning_vrs.js';


   // get logged in users rs id
   $user_id = $GLOBALS['user']->uid;
   $rs_code = dispatch_planning_get_rscode_using_uid($user_id);
   $dis_id = change_feeds_import_get_distributor_id($user_id);

   $form['trip_number'] = array(
     '#prefix' => '<div class="textfield-wrapper">',
     '#type' => 'textfield',
     '#title' => t('Trip'),
     '#required' => TRUE,
     '#autocomplete_path' => 'trip/autocomplete',
     '#ajax' => array(
       'wrapper' => 'itemlist-div',
       'callback' => 'tripwise_vrs_details_remove_callback',
       'method' => 'replace',
       'effect' => 'fade',
     ),
   );

   $form['rs_code'] = array(
     '#type' => 'value',
     '#value' => $rs_code,
   );
   $form['dis_id'] = array(
     '#type' => 'value',
     '#value' => $dis_id,
   );


   $trip_number = $form_state['input']['trip_number'];

   if (!empty($trip_number) && !empty($rs_code)) {
      $trip_details = _get_farey_delivered_tripwise_vrs_details($trip_number, $rs_code);
      $trip_amt_details = _get_farey_delivered_tripwise_vrs_amt_details($trip_number, $rs_code);
      $ret_trip_amt_details = _get_farey_returned_delivered_trip_details($trip_number, $rs_code);

      if (!empty($trip_details)) {
         $trip_details_header = array('', 'Bill count', 'Bill value');
         $trip_details_amt_header = array('', 'Bill count', 'Bill value', 'App value', 'Actual collected', 'Difference');
         $returned_details_amt_header = array('To be returned Inventory', 'Sku count', 'Bill value', 'App Ret Inventory Value', 'Actual Ret Inventory Value', 'Difference');

         $form['trip_details'] = array(
           '#theme' => 'table',
           '#header' => $trip_details_header,
           '#rows' => $trip_details,
         );

         $form['trip_amt_details'] = array(
           '#theme' => 'table',
           '#header' => $trip_details_amt_header,
           '#rows' => $trip_amt_details,
         );

         $form['ret_trip_details'] = array(
           '#theme' => 'table',
           '#header' => $returned_details_amt_header,
           '#rows' => $ret_trip_amt_details,
         );
         $form['van_bill_value'] = array(
           '#type' => 'textfield',
           '#title' => t('van bill value'),
           '#default_value' => $trip_details['Total Bills'][2],
           '#disabled' => TRUE,
           '#attributes' => array('class' => array("van_bill_value")),
         );
         $form['cash_debit'] = array(
           '#type' => 'textfield',
           '#title' => t('Total collection'),
           '#attributes' => array('class' => array("cash_debit"), 'readonly' => 'readonly'),
         );
         $form['inventory_detail'] = array(
           '#type' => 'textfield',
           '#title' => t('Actual ret inventory value'),
           '#attributes' => array('class' => array("inventory_detail"), 'readonly' => 'readonly'),
         );
         $form['total_debit'] = array(
           '#type' => 'textfield',
           '#title' => t('Total van debit'),
           '#attributes' => array('class' => array("total_debit"), 'readonly' => 'readonly'),
         );
         $form['submit'] = array(
           '#type' => 'submit',
           '#value' => 'submit',
         );
      }
      else {
         drupal_set_message(t('No result Found'), 'error');
      }
   }
   $form['actual_collected'] = array('#type' => 'value');
   $form['difference'] = array('#type' => 'value');
   $form['ret_actual_collected'] = array('#type' => 'value');
   $form['ret_difference'] = array('#type' => 'value');


   if (!isset($form_state['bills']['rows'])) {
      $form_state['bills']['rows'] = array();
   }
   return $form;
}

/**
 * 
 * @param type $trip_number
 * @param type $rs_code
 */
function _get_farey_delivered_tripwise_vrs_details($trip_number, $rs_code) {
   global $user;
   $data = array();
   $query = db_select('dispatch_planning_api_data', 'ad');
   $query->condition('ad.runsheet_number', $trip_number, '=');
   $query->condition('ad.hub', $rs_code, '=');
   $query->addExpression('Count(ad.id)', 'Count_of_bills');
   $query->addExpression('SUM(ad.bill_value)', 'Sum_of_bills');

   $query->addExpression('SUM(CASE WHEN (res_status = \'undeliver\') THEN 1 ELSE 0 END)', 'Count_Returned_bills');
   $query->addExpression('SUM(CASE WHEN (res_status =  \'undeliver\') THEN ad.bill_value ELSE 0 END)', 'Sum_Returned_bills');

   $query->addExpression('SUM(CASE WHEN (res_status IS NULL) THEN 1 ELSE 0 END)', 'Count_Returned_bills');
   $query->addExpression('SUM(CASE WHEN (res_status IS NULL) THEN ad.bill_value ELSE 0 END)', 'Sum_Returned_bills');

   $rows = $query->execute()->fetch(PDO::FETCH_ASSOC);
   $first_row = array('Total Bills', 'Full Returned Bills', 'Without Status Bills');
   foreach ($first_row as $val) {
      $k = 0;
      $data[$val][] = $val;
      foreach ($rows as $key1 => $val1) {
         if ($k == 2) {
            continue;
         }
         $data[$val][] = $val1;
         $k = $k + 1;
         $rows1 = array_shift($rows);
      }
   }
   return $data;
}

/**
 * 
 * @param type $trip_number
 * @param type $rs_code
 */
function _get_farey_delivered_tripwise_vrs_amt_details($trip_number, $rs_code) {
   global $user;
   $data = array();

   $query = db_select('dispatch_planning_api_data', 'ad');
   $query->condition('ad.runsheet_number', $trip_number, '=');
   $query->condition('ad.hub', $rs_code, '=');

   $query->addExpression('SUM(CASE WHEN (res_confirm_signed_bill_amount IS NOT NULL AND res_confirm_signed_bill_amount != \'NA\' AND res_confirm_signed_bill_amount != \'\') THEN 1 ELSE 0 END)', 'Count_signed_bills');
   $query->addExpression('SUM(CASE WHEN (res_confirm_signed_bill_amount IS NOT NULL AND res_confirm_signed_bill_amount != \'NA\'  AND res_confirm_signed_bill_amount != \'\') THEN bill_value ELSE 0 END)', 'Sum_signed_bills');
   $query->addExpression('SUM(CASE WHEN (res_confirm_signed_bill_amount IS NOT NULL AND res_confirm_signed_bill_amount != \'NA\'  AND res_confirm_signed_bill_amount != \'\') THEN res_to_be_collection ELSE 0 END)', 'app_signed_bills');

   $query->addExpression('SUM(CASE WHEN (res_cheque_amount IS NOT NULL AND res_cheque_amount != \'NA\'  AND res_cheque_amount != \'\') THEN 1 ELSE 0 END)', 'Count_cheque_bills');
   $query->addExpression('SUM(CASE WHEN (res_cheque_amount IS NOT NULL AND res_cheque_amount != \'NA\'  AND res_cheque_amount != \'\') THEN bill_value ELSE 0 END)', 'Sum_cheque_bills');
   $query->addExpression('SUM(CASE WHEN (res_cheque_amount IS NOT NULL AND res_cheque_amount != \'NA\'  AND res_cheque_amount != \'\') THEN res_to_be_collection ELSE 0 END)', 'app_cheque_bills');

   $query->addExpression('SUM(CASE WHEN (res_cash_amount IS NOT NULL AND res_cash_amount != \'NA\'  AND res_cash_amount != \'\') THEN 1 ELSE 0 END)', 'Count_cash_bills');
   $query->addExpression('SUM(CASE WHEN (res_cash_amount IS NOT NULL AND res_cash_amount != \'NA\'  AND res_cash_amount != \'\') THEN bill_value ELSE 0 END)', 'Sum_cash_bills');
   $query->addExpression('SUM(CASE WHEN (res_cash_amount IS NOT NULL AND res_cash_amount != \'NA\'  AND res_cash_amount != \'\') THEN res_to_be_collection ELSE 0 END)', 'app_cash_bills');
   $rows = $query->execute()->fetch(PDO::FETCH_ASSOC);

   $first_row = array('Signed_Bills', 'Cheque_Bills', 'Cash_Bills');
   foreach ($first_row as $val) {
      $k = 0;
      $data[$val][] = $val;
      foreach ($rows as $key1 => $val1) {
         if ($k == 3) {
            continue;
         }
         $data[$val][] = array('data' => $val1, 'data-value' => $val1, 'class' => array("$val-$key1-val"));
         //$data[$val][] = array('data' => 100, 'data-value' => 100, 'class' => array("$val-$key1-val"));
         $k = $k + 1;
         $rows1 = array_shift($rows);
      }
   }

   foreach ($data as $key => $value) {

      $data[$key]['actual_collected']['data'] = array(
        '#type' => 'textfield',
        '#title' => t('Actual Collected'),
        '#title_display' => 'invisible',
        '#name' => "actual_collected[$key]",
        '#attributes' => array('class' => array("$key-collected")),
        '#required' => TRUE,
      );
      $data[$key]['difference']['data'] = array(
        '#type' => 'textfield',
        '#title' => t('Difference'),
        '#title_display' => 'invisible',
        '#name' => "difference[$key]",
        '#disabled' => TRUE,
        '#attributes' => array('class' => array("$key-difference"), 'readonly' => 'readonly'),
        '#required' => TRUE,
      );
   }

   return $data;
}

function tripwise_vrs_details_remove_callback($form, &$form_state) {
   return $form;
}

function _get_farey_returned_delivered_trip_details($trip_number, $rs_code) {
   global $user;
   $dis_id = change_feeds_import_get_distributor_id($user->uid);

   $query = db_select('dispatch_planning_item_details', 'id');
   $query->join('node', 'n', 'n.title = id.sku_code');
   $query->condition('n.uid', $dis_id);
   $query->condition('n.type', 'sku7_code_master');
   $query->leftjoin('field_data_field_child_sku', 'chs', 'chs.entity_id = n.nid');
   $query->leftjoin('node', 'n1', 'n1.nid = chs.field_child_sku_nid');
   $query->join('dispatch_planning_api_data', 'ad', 'id.trip_bill_reference = ad.id');
   $query->condition('ad.runsheet_number', $trip_number, '=');
   $query->condition('ad.hub', $rs_code, '=');
   $query->condition('id.og_sum_net_sales_quantity', 0, '!=');
   $query->condition('id.res_sku_actual_quantity1', '', '!=');
   $query->condition('id.res_sku_actual_quantity1', 'NULL', '!=');

   $query->addExpression('(SUM(og_sum_net_sales_quantity) - SUM(res_sku_actual_quantity1))', 'Returned_qty');
   $query->addExpression('SUM((og_sum_net_sales_quantity - res_sku_actual_quantity1)*sum_net_sales_value)', 'Returned_val');

   $query->havingCondition('Returned_qty', 0, '>');
   $rows = $query->execute()->fetch(PDO::FETCH_ASSOC);

   $data[0]['to_be_ret'] = '';
   $data[0]['Returned_qty'] = $rows['Returned_qty'];
   $data[0]['del_val'] = '';
   $data[0]['Returned_val'] = array('data' => $rows['Returned_val'],'data-value' => $rows['Returned_val'],  'class' => array("ret_val"));
   //$data[0]['Returned_val'] = array('data' => 100, 'data-value' => 100, 'class' => array("ret_val"));

   $data[0]['ret_actual_collected']['data'] = array(
     '#type' => 'textfield',
     '#title' => t('Actual Collected'),
     '#title_display' => 'invisible',
     '#name' => "ret_actual_collected",
     '#attributes' => array('class' => array("ret_collected")),
     '#required' => TRUE,
   );
   $data[0]['ret_difference']['data'] = array(
     '#type' => 'textfield',
     '#title' => t('Difference'),
     '#title_display' => 'invisible',
     '#name' => "ret_difference",
     '#disabled' => TRUE,
     '#attributes' => array('class' => array("ret_difference"), 'readonly' => 'readonly'),
     '#required' => TRUE,
   );

   return $data;
}

function _delivered_tripwise_vrs_data_callback_submit($form, &$form_state) {
   $trip_id = $form_state['values']['trip_number'];

   $dis_id = $form_state['values']['dis_id'];
   $rs_code = $form_state['values']['rs_code'];

   if (!empty($trip_id) && !empty($rs_code)) {

      $trip_details = _get_farey_delivered_tripwise_vrs_details($trip_id, $rs_code);
      $trip_amt_details = _get_farey_delivered_tripwise_vrs_amt_details($trip_id, $rs_code);
      $ret_trip_amt_details = _get_farey_returned_delivered_trip_details($trip_id, $rs_code);
      
      // insert new or update if already created
      db_merge('dispatch_delivered_vrs_details')
          ->key(array('dis_id' => $dis_id, 'trip_id' => $trip_id))
          ->fields(array(
            'dis_id' => $dis_id,
            'trip_id' => $trip_id,
            'total_bill_count' => $trip_details['Total Bills'][1],
            'total_bill_value' => $trip_details['Total Bills'][2],
            'fr_bill_count' => $trip_details['Full Returned Bills'][1],
            'fr_bill_value' => $trip_details['Full Returned Bills'][2],
            'ws_bill_count' => $trip_details['Without Status Bills'][1],
            'ws_bill_value' => $trip_details['Without Status Bills'][2],
            'sb_actual_collected' => $form_state['values']['actual_collected']['Signed_Bills'],
            'sb_difference' => $form_state['values']['difference']['Signed_Bills'],
            'sb_bill_count' => $trip_amt_details['Signed_Bills'][1]['data'],
            'sb_bill_value' => $trip_amt_details['Signed_Bills'][2]['data'],
            'sb_app_value' => $trip_amt_details['Signed_Bills'][3]['data'],
            'ch_actual_collected' => $form_state['values']['actual_collected']['Cheque_Bills'],
            'ch_difference' => $form_state['values']['difference']['Cheque_Bills'],
            'ch_bill_count' => $trip_amt_details['Cheque_Bills'][1]['data'],
            'ch_bill_value' => $trip_amt_details['Cheque_Bills'][2]['data'],
            'ch_app_value' => $trip_amt_details['Cheque_Bills'][3]['data'],
            'cash_actual_collected' => $form_state['values']['actual_collected']['Cash_Bills'],
            'cash_difference' => $form_state['values']['difference']['Cash_Bills'],
            'cash_bill_count' => $trip_amt_details['Cash_Bills'][1]['data'],
            'cash_bill_value' => $trip_amt_details['Cash_Bills'][2]['data'],
            'cash_app_value' => $trip_amt_details['Cash_Bills'][3]['data'],
            'tbr_bill_count' => $ret_trip_amt_details[0]['Returned_qty'],
            'tbr_actual_collected' => $form_state['values']['ret_actual_collected'],
            'tbr_difference' => $form_state['values']['ret_difference'],
            'tbr_app_value' => $ret_trip_amt_details[0]['Returned_val']['data'],
            'van_bill_value' => (float) $form_state['values']['van_bill_value'],
            'cash_debit' => (float) $form_state['values']['cash_debit'],
            'inventory_debit' => (float) $form_state['values']['inventory_detail'],
            'total_debit' => (float) $form_state['values']['total_debit'],
          ))
          ->execute();
   }
}
