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

   // get logged in users rs id
   $user_id = $GLOBALS['user']->uid;
   $rs_code = dispatch_planning_get_rscode_using_uid($user_id);

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

   $trip_number = $form_state['input']['trip_number'];

   if (!empty($trip_number) && !empty($rs_code)) {
      $trip_details = _get_farey_delivered_tripwise_vrs_details($trip_number, $rs_code);
      $trip_amt_details = _get_farey_delivered_tripwise_vrs_amt_details($trip_number, $rs_code);
      $ret_trip_amt_details = _get_farey_returned_delivered_trip_details($trip_number, $rs_code);

      if (!empty($trip_details)) {
         $trip_details_header = array('', 'Bill count', 'Bill value');
         $trip_details_amt_header = array('', 'Bill count', 'Bill value', 'App value', 'Actual collected', 'Difference');
         $returned_details_amt_header = array('To be returned Inventory', 'Sku count', 'Bill value', 'App value', 'Actual collected', 'Difference');

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
         );
         $form['cash_debit'] = array(
           '#type' => 'textfield',
           '#title' => t('Cash debit'),
         );
         $form['inventory_detail'] = array(
           '#type' => 'textfield',
           '#title' => t('Inventory debit'),
         );
         $form['total_debit'] = array(
           '#type' => 'textfield',
           '#title' => t('Total debit'),
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

   $trip_number = 1111;
   $rs_code = 100006;

   $query = db_select('dispatch_planning_api_data', 'ad');
   $query->condition('ad.runsheet_number', $trip_number, '=');
   $query->condition('ad.hub', $rs_code, '=');

   $query->addExpression('SUM(CASE WHEN (res_confirm_signed_bill_amount IS NOT NULL) THEN 1 ELSE 0 END)', 'Count_signed_bills');
   $query->addExpression('SUM(CASE WHEN (res_confirm_signed_bill_amount IS NOT NULL) THEN bill_value ELSE 0 END)', 'Sum_signed_bills');
   $query->addExpression('SUM(CASE WHEN (res_confirm_signed_bill_amount IS NOT NULL) THEN res_to_be_collection ELSE 0 END)', 'app_signed_bills');

   $query->addExpression('SUM(CASE WHEN (res_cheque_amount IS NOT NULL) THEN 1 ELSE 0 END)', 'Count_cheque_bills');
   $query->addExpression('SUM(CASE WHEN (res_cheque_amount IS NOT NULL) THEN bill_value ELSE 0 END)', 'Sum_cheque_bills');
   $query->addExpression('SUM(CASE WHEN (res_cheque_amount IS NOT NULL) THEN res_to_be_collection ELSE 0 END)', 'app_cheque_bills');

   $query->addExpression('SUM(CASE WHEN (res_cash_amount IS NOT NULL) THEN 1 ELSE 0 END)', 'Count_cash_bills');
   $query->addExpression('SUM(CASE WHEN (res_cash_amount IS NOT NULL) THEN bill_value ELSE 0 END)', 'Sum_cash_bills');
   $query->addExpression('SUM(CASE WHEN (res_cash_amount IS NOT NULL) THEN res_to_be_collection ELSE 0 END)', 'app_cash_bills');
   $rows = $query->execute()->fetch(PDO::FETCH_ASSOC);


   $first_row = array('Signed Bills', 'Cheque Bills', 'Cash Bills');
   foreach ($first_row as $val) {
      $k = 0;
      $data[$val][] = $val;
      foreach ($rows as $key1 => $val1) {
         if ($k == 3) {
            continue;
         }
         $data[$val][] = $val1;
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
        '#required' => TRUE,
      );
      $data[$key]['difference']['data'] = array(
        '#type' => 'textfield',
        '#title' => t('Difference'),
        '#title_display' => 'invisible',
        '#name' => "difference[$key]",
        '#attributes' => array('class' => array('cheque-date-field')),
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
   // $query->fields('id', array('sku_code', 'sku_description'));
   // $query->addField('n1', 'title', 'Child squ');
   // $query->addField('id', 'sum_net_sales_value', 'value per unit');
   // $query->groupBy('id.sku_code');
   // $query->addExpression('SUM(og_sum_net_sales_quantity)', 'Sum_of_net_sales_qty');
   //$query->addExpression('SUM(res_sku_actual_quantity1)', 'Actual_qty_delivered');
   $query->addExpression('(SUM(og_sum_net_sales_quantity) - SUM(res_sku_actual_quantity1))', 'Returned_qty');
   //$query->addExpression('SUM(og_sum_net_sales_quantity*sum_net_sales_value)', 'Returned_val');
   $query->addExpression('SUM((og_sum_net_sales_quantity - res_sku_actual_quantity1)*sum_net_sales_value)', 'Returned_val');

   $query->havingCondition('Returned_qty', 0, '>');
   $rows = $query->execute()->fetch(PDO::FETCH_ASSOC);

   $data[0]['to_be_ret'] = '';
   $data[0]['Returned_qty'] = $rows['Returned_qty'];
   $data[0]['del_val'] = '';
   $data[0]['Returned_val'] = $rows['Returned_val'];

   $data[0]['ret_actual_collected']['data'] = array(
     '#type' => 'textfield',
     '#title' => t('Actual Collected'),
     '#title_display' => 'invisible',
     '#name' => "ret_actual_collected",
     '#required' => TRUE,
   );
   $data[0]['ret_difference']['data'] = array(
     '#type' => 'textfield',
     '#title' => t('Difference'),
     '#title_display' => 'invisible',
     '#name' => "ret_difference",
     '#attributes' => array('class' => array('cheque-date-field')),
     '#required' => TRUE,
   );

   return $data;
}

function _delivered_tripwise_vrs_data_callback_submit($form, &$form_state) {
   $trip_id = $form_state['values']['trip_number'];
   if (!empty($trip_id)) {
      db_merge('dispatch_delivered_vrs_details')
          ->key(array('dis_id' => $distributor_id, 'trip_id' => $trip_id))
          ->fields(array(
            'dis_id' => $distributor_id,
            'trip_id' => $trip_id,
            'sb_actual_collected' => $form_state['values']['actual_collected']['Signed Bills'],
            'sb_difference' => $form_state['values']['difference']['Signed Bills'],
            'ch_actual_collected' =>  $form_state['values']['actual_collected']['Cheque Bills'],
            'ch_difference' =>  $form_state['values']['difference']['Cheque Bills'],
            'cash_actual_collected' =>  $form_state['values']['actual_collected']['Cash Bills'],
            'cash_difference' => $form_state['values']['difference']['Cash Bills'],
            'tbr_actual_collected' =>  $form_state['values']['ret_actual_collected'],
            'tbr_difference' =>  $form_state['values']['ret_difference'],
            'van_bill_value' =>  $form_state['values']['van_bill_value'],
            'cash_debit' =>  $form_state['values']['cash_debit'],
            'inventory_debit' =>  $form_state['values']['inventory_detail'],
            'total_debit' =>  $form_state['values']['total_debit'],
          ))
          ->execute();
   }
}
