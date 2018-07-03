<?php

/*
 * it will call from, "farey_data" services
 * 
 */

function fetch_dispatch_planning_api_data($data) {
   watchdog('farey data', '<pre>' . print_r($data, TRUE) . '</pre>');
   if (!empty($data)) {

      // first entry in log table
      // get dis id using rs code
      $dis_id = leanbox_mobile_get_distributor_using_rscode($data['hubCode']);
      $json_data = json_encode($data);
      dispatch_planning_api_log($json_data, $data['jobData']['runsheet_number'], 'farey_sales_deliver', $dis_id, 1);

      $bill_row_id = _get_bill_row_id_using_reference_no($data['referenceNumber']);
      
      if (isset($data['fieldData']['money_collect2'])) {
         if (is_array($data['fieldData']['money_collect2'])) {
            $money_collect = implode(",", $data['fieldData']['money_collect2']);
         }
      }
       if (isset($data['fieldData']['money_collect3'])) {
         if (is_array($data['fieldData']['money_collect3'])) {
            $money_collect = implode(",", $data['fieldData']['money_collect3']);
         }
      }
      $updated_bill_row_id = db_update('dispatch_planning_api_data') // Table name no longer needs {}
          ->fields(array(
            'res_runsheetNo' => isset($data['runsheetNo']) ? $data['runsheetNo'] : '',
            'res_actualAmount' => isset($data['actualAmount']) ? $data['actualAmount'] : '',
            'res_originalAmount' => isset($data['originalAmount']) ? $data['originalAmount'] : '',
            'res_moneyTransactionType' => isset($data['moneyTransactionType']) ? $data['moneyTransactionType'] : '',
            'res_latitude' => isset($data['latitude']) ? $data['latitude'] : '',
            'res_longitude' => isset($data['longitude']) ? $data['longitude'] : '',
            'res_attemptCount' => isset($data['attemptCount']) ? $data['attemptCount'] : '',
            'res_jobMasterId' => isset($data['jobMasterId']) ? $data['jobMasterId'] : '',
            'res_companyId' => isset($data['companyId']) ? $data['companyId'] : '',
            'res_jobType' => isset($data['delivery1']) ? $data['delivery1'] : '',
            'res_jobId' => isset($data['jobId']) ? $data['jobId'] : '',
            'res_employeeCode' => isset($data['employeeCode']) ? $data['employeeCode'] : '',
            'res_status' => isset($data['status']) ? $data['status'] : '',
            'res_trackHalt' => isset($data['trackHalt']) ? $data['trackHalt'] : '',
            'res_trackKm' => isset($data['trackKm']) ? $data['trackKm'] : '',
            'res_trackTransactionTimeSpent' => isset($data['trackTransactionTimeSpent']) ? $data['trackTransactionTimeSpent'] : '',
            'res_merchantCode' => isset($data['merchantCode']) ? $data['merchantCode'] : '',
            'res_transactionDate' => isset($data['transactionDate']) ? strtotime($data['transactionDate']) : '',
            'res_erpPushTime' => isset($data['erpPushTime']) ? $data['erpPushTime'] : '',
            'res_lastTransactionTime' => isset($data['lastTransactionTime']) ? $data['lastTransactionTime'] : '',
            'res_battery' => isset($data['battery']) ? $data['battery'] : '',
            'res_confirm_signed_bill_amount' => isset($data['fieldData']['confirm_signed_bill_amount']) ? $data['fieldData']['confirm_signed_bill_amount'] : '',
            'res_bank_name' => isset($data['fieldData']['bank_name']) ? $data['fieldData']['bank_name'] : '',
            'res_photo_of_signed_bill_delivery' => isset($data['fieldData']['photo_of_signed_bill_delivery']) ? $data['fieldData']['photo_of_signed_bill_delivery'] : '',
            'res_total_amount' => isset($data['fieldData']['total_amount']) ? $data['fieldData']['total_amount'] : '',
            'res_remarks' => isset($data['fieldData']['remarks']) ? $data['fieldData']['remarks'] : '',
            'res_cheque_date' => isset($data['fieldData']['cheque_date']) ? strtotime($data['fieldData']['cheque_date']) : '',
            'res_photo_of_cheque' => isset($data['fieldData']['photo_of_cheque']) ? $data['fieldData']['photo_of_cheque'] : '',
            'res_cheque_amount' => isset($data['fieldData']['cheque_amount']) ? $data['fieldData']['cheque_amount'] : '',
            'res_money_collect2' => !empty($money_collect) ? $money_collect : '',
            'res_signed_bill_delivery' => isset($data['fieldData']['signed_bill_delivery']) ? $data['fieldData']['signed_bill_delivery'] : '',
            'res_cash_amount' => isset($data['fieldData']['cash_amount']) ? $data['fieldData']['cash_amount'] : '',
            'res_to_be_collection' => isset($data['fieldData']['to_be_collection']) ? $data['fieldData']['to_be_collection'] : '',
            'res_signed_bill_partial' => isset($data['fieldData']['signed_bill_partial']) ? $data['fieldData']['signed_bill_partial'] : '',
            'res_reason_for_failure' => isset($data['fieldData']['reason_for_failure']) ? $data['fieldData']['reason_for_failure'] : '',
            'photo_of_signed_bill_partial' => isset($data['fieldData']['photo_of_signed_bill_partial']) ? $data['fieldData']['photo_of_signed_bill_partial'] : '',
            'res_cheque_no' => isset($data['fieldData']['cheque_no']) ? $data['fieldData']['cheque_no'] : 'NA',
            'res_delivery_boy' => isset($data['fieldData']['delivery_boy']) ? $data['fieldData']['delivery_boy'] : '',
            'updated_flag' => 2,
            'updated_time' => REQUEST_TIME,  
            'res_updated_time' => REQUEST_TIME,

          ))
          ->condition('referenceNo', $data['referenceNumber'], '=')
          ->execute();

      // add trip id condition also in db_update
      //       ->condition('referenceNo', $data[''referenceNumber], '=')


      watchdog('updated row id', '<pre>' . print_r($updated_bill_row_id, TRUE) . '</pre>');

      if (!empty($bill_row_id) && isset($data['fieldData']['item_details1']) && !empty($data['fieldData']['item_details1'])) {
         watchdog('farey data full', '<pre>' . print_r($data['fieldData']['item_details'], TRUE) . '</pre>');

         foreach ($data['fieldData']['item_details1'] as $key => $value) {
            
            // fetch dispatch_qty here
            // ignore if value is not in form of array
            if (!is_array($value)) {
               continue;
            }
            $data = _get_dispatch_qty_item_details($bill_row_id, $value['sku_code1']);
            $return_qty = $data['dispatch_qty'] - $value['sku_actual_quantity1'] - $data['res_cashier_short'];
            
            db_update('dispatch_planning_item_details') // Table name no longer needs {}
                ->fields(array(
                  'res_sku_actual_quantity1' => isset($value['sku_actual_quantity1']) ? $value['sku_actual_quantity1'] : 0,
                  'farey_delivered_qty' => isset($value['sku_actual_quantity1']) ? $value['sku_actual_quantity1'] : 0,
                  'res_sku_reason' => isset($value['sku_reason']) ? $value['sku_reason'] : '',
                  'return_qty' => $return_qty,
                  'line_level_status'=> 'line_deliver', // line level status
                ))
                ->condition('trip_bill_reference', $bill_row_id, '=')
                ->condition('sku_code', $value['sku_code1'], '=')
                ->execute();
         }
      }

      if (!empty($updated_bill_row_id) && isset($data['fieldData']['item_details']) && !empty($data['fieldData']['item_details'])) {
         watchdog('farey data partial', '<pre>' . print_r($data['fieldData']['item_details'], TRUE) . '</pre>');


         foreach ($data['fieldData']['item_details'] as $key => $value) {

            // ignore if value is not in form of array
            if (!is_array($value)) {
               continue;
            }
            
            $data = _get_dispatch_qty_item_details($bill_row_id, $value['sku_code']);
            $return_qty = $data['dispatch_qty'] - $value['sku_actual_quantity'] - $data['res_cashier_short'];
            $line_status = '';
            $ret_cash = $return_qty + $data['res_cashier_short']; //Return Qty + Cashier Short  
            $ret_cash_godown = $ret_cash + $data['res_godown_short']; //Return Qty + Cashier Short + Godown Short
            if ($ret_cash == $data['dispatch_qty'] ) {
             $line_status = 'line_undeliver';
            }else if ($ret_cash_godown > 0) {
              $line_status = 'line_partial_deliver';
            }else if ($ret_cash_godown == 0) {
              $line_status = 'line_deliver';
            }
            
            db_update('dispatch_planning_item_details') // Table name no longer needs {}
                ->fields(array(
                  'res_sku_actual_quantity1' => isset($value['sku_actual_quantity']) ? $value['sku_actual_quantity'] : 0,
                  'farey_delivered_qty' => isset($value['sku_actual_quantity']) ? $value['sku_actual_quantity'] : 0,
                  'res_sku_reason' => isset($value['sku_reason']) ? $value['sku_reason'] : '',
                  'return_qty' => $return_qty,
                  'line_level_status' => $line_status, // line level status
                ))
                ->condition('trip_bill_reference', $bill_row_id, '=')
                ->condition('sku_code', $value['sku_code'], '=')
                ->execute();
         }
      }
      if ($data['status'] == 'undeliver' && !empty($updated_bill_row_id)) {
         watchdog('farey data undeliver', '<pre>' . print_r($data['fieldData']['item_details'], TRUE) . '</pre>');
         $sku_list = _get_dispatch_qty_undeliver($bill_row_id);
         foreach ($sku_list as $key => $val) {
            $sku_code = $val['sku_code'];
            $dispatch_qty = $val['dispatch_qty'];
         
          db_update('dispatch_planning_item_details') // Table name no longer needs {}
              ->fields(array(
                'res_sku_actual_quantity1' => 0,
                'return_qty' => $dispatch_qty,
                'farey_delivered_qty' => 0,
                'line_level_status'=> 'line_undeliver', // line level status
              ))
              ->condition('trip_bill_reference', $bill_row_id, '=')
              ->condition('sku_code', $sku_code, '=')
              ->execute();
         }
      }
      $results['flag'] = '1';
      $results['result'] = 'Data updated Sucessfully.';
   }
   else {
      // make log entry here
      //dispatch_planning_api_log($json_data, '', 'farey_sales_deliver', 0, 0);
      $results['flag'] = '0';
      $results['result'] = 'No data was updated.';
   }
   // log entry and is remaining
   return $results;
}

/**
 * 
 * @param type $bill_row_id
 * @param type $sku
 * @return type
 */
function _get_dispatch_qty_item_details($bill_row_id, $sku) {
   $query1 = db_select('dispatch_planning_item_details', 'it');
   $query1->fields('it', array('dispatch_qty', 'res_cashier_short','res_godown_short'));
   $query1->condition('it.trip_bill_reference', $bill_row_id);
   $query1->condition('it.sku_code', $sku);
   $res1 = $query1->execute()->fetch(PDO::FETCH_ASSOC);
   return $res1;
}

/**
 * 
 * @param type $bill_row_id
 * @return type
 */
function _get_dispatch_qty_undeliver($bill_row_id) {
   $query = db_select('dispatch_planning_item_details', 'ad');
   $query->fields('ad', array('sku_code', 'dispatch_qty', 'res_cashier_short'));
   $query->condition('ad.trip_bill_reference', $bill_row_id);
   $sku_list = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
   return $sku_list;
}
