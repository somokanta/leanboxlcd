<?php

/*
 * it will call from, "farey_data" services
 * 
 */

function fetch_dispatch_planning_api_data($data) {
   watchdog('farey data', '<pre>' . print_r($data,TRUE). '</pre>');
   if (!empty($data)) {

      // first entry in log table
      // get dis id using rs code
      $dis_id = leanbox_mobile_get_distributor_using_rscode($data['hubCode']);
      $json_data = json_encode($data);
      dispatch_planning_api_log($json_data, $data['jobData']['runsheet_number'], 'farey_sales_deliver', $dis_id, 1);
      
      $bill_row_id = _get_bill_row_id_using_reference_no($data['referenceNumber']);
      
      $updated_bill_row_id = db_update('dispatch_planning_api_data') // Table name no longer needs {}
          ->fields(array(
            'res_runsheetNo' => isset($data['runsheetNo']) ? $data['runsheetNo'] : '' ,
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
            'res_money_collect2' => isset($data['fieldData']['money_collect2']) ? $data['fieldData']['money_collect2'] : '',
            'res_signed_bill_delivery' => isset($data['fieldData']['signed_bill_delivery']) ? $data['fieldData']['signed_bill_delivery'] : '',
          ))
          ->condition('referenceNo', $data['referenceNumber'], '=')
          ->execute();

      // add trip id condition also in db_update
      //       ->condition('referenceNo', $data[''referenceNumber], '=')


   watchdog('farey data1', '<pre>' . print_r($updated_bill_row_id,TRUE). '</pre>');

watchdog('farey data1', '<pre>' . print_r($updated_bill_row_id,TRUE). '</pre>');


      if (!empty($bill_row_id) && isset($data['fieldData']['item_details1']) && !empty($data['fieldData']['item_details1'])) {
         foreach ($data['fieldData']['item_details1'] as $key => $value) {

            // ignore if value is not in form of array
            if (!is_array($value)) {
              continue;
            }
            db_update('dispatch_planning_item_details') // Table name no longer needs {}
                ->fields(array(
                  'res_sku_actual_quantity1' => isset($value['sku_actual_quantity1']) ? $value['sku_actual_quantity1'] : 0,
                ))
                ->condition('trip_bill_reference', $bill_row_id, '=')
                ->condition('sku_code', $value['sku_code1'], '=')
                ->execute();
         }
      }

      if (!empty($updated_bill_row_id) && isset($data['fieldData']['item_details']) && !empty($data['fieldData']['item_details'])) {
        watchdog('farey data1', '<pre>' . print_r($data['fieldData']['item_details'],TRUE). '</pre>');
 

        foreach ($data['fieldData']['item_details'] as $key => $value) {

            // ignore if value is not in form of array
            if (!is_array($value)) {
              continue;
            }
            db_update('dispatch_planning_item_details') // Table name no longer needs {}
                ->fields(array(
                  'res_sku_actual_quantity1' => isset($value['sku_actual_quantity']) ? $value['sku_actual_quantity'] : 0,
                ))
                ->condition('trip_bill_reference', $bill_row_id, '=')
                ->condition('sku_code', $value['sku_code'], '=')
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
 * @param type $ref_no
 * @return type
 */
function _get_bill_row_id_using_reference_no($ref_no) {
   $query = db_select('dispatch_planning_api_data', 'dp');
   $query->fields('dp', array('id'));
   $query->condition('dp.referenceNo', $ref_no);
   $res = $query->execute()->fetchField();
   return $res;
}