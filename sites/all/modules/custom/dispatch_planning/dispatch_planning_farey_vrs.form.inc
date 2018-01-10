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
dpm($trip_amt_details, '$trip_amt_details');
      if (!empty($trip_details)) {
         $trip_details_header = array('', 'Bill count', 'Bill value');
         $trip_details_amt_header = array('', 'Bill count', 'Bill value', 'App value', 'Actual value', 'Difference');

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
      }
      else {
         drupal_set_message(t('No result Found'), 'error');
      }
   }
   $form['actual_collected'] = array('#type' => 'value');
   $form['difference'] = array('#type' => 'value');
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
   dpm($data, 'data1');


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
   dpm($data, 'data2');

   return $data;
}

function tripwise_vrs_details_remove_callback($form, &$form_state) {
   return $form;
}
