<?php

/*
 * it will call from, "farey_data" services
 * 
 */

function fetch_dispatch_planning_api_data($json_data) {


   if (!empty($json_data)) {

      // data is in json format always so decode it
      $data = json_decode($json_data, true);
      
      // first entry in log table
      // get dis id using rs code
      $dis_id = leanbox_mobile_get_distributor_using_rscode($data['hubCode']);
      dispatch_planning_api_log($json_data, $data['LB_RunsheetNo'], 'farey_sales_deliver', $dis_id, 1);
      
      $updated_bill_row_id = db_update('dispatch_planning_api_data') // Table name no longer needs {}
          ->fields(array(
            'res_runsheetNo' => $data['runsheetNo'],
            'res_actualAmount' => $data['actualAmount'],
            'res_originalAmount' => $data['originalAmount'],
            'res_moneyTransactionType' => $data['moneyTransactionType'],
            'res_latitude' => $data['latitude'],
            'res_longitude' => $data['longitude'],
            'res_attemptCount' => $data['attemptCount'],
            'res_jobMasterId' => $data['jobMasterId'],
            'res_companyId' => $data['companyId'],
            'res_jobType' => $data['delivery1'],
            'res_jobId' => $data['jobId'],
            'res_employeeCode' => $data['employeeCode'],
            'res_status' => $data['status'],
            'res_trackHalt' => $data['trackHalt'],
            'res_trackKm' => $data['trackKm'],
            'res_trackTransactionTimeSpent' => $data['trackTransactionTimeSpent'],
            'res_merchantCode' => $data['merchantCode'],
            'res_transactionDate' => $data['transactionDate'],
            'res_erpPushTime' => $data['erpPushTime'],
            'res_lastTransactionTime' => $data['lastTransactionTime'],
            'res_battery' => $data['battery'],
            'res_confirm_signed_bill_amount' => $data['fieldData']['confirm_signed_bill_amount'],
            'res_bank_name' => $data['fieldData']['bank_name'],
            'res_photo_of_signed_bill_delivery' => $data['fieldData']['photo_of_signed_bill_delivery'],
            'res_total_amount' => $data['fieldData']['total_amount'],
            'res_remarks' => $data['fieldData']['remarks'],
            'res_cheque_date' => $data['fieldData']['cheque_date'],
            'res_photo_of_cheque' => $data['fieldData']['photo_of_cheque'],
            'res_cheque_amount' => $data['fieldData']['cheque_amount'],
            'res_money_collect2' => $data['fieldData']['money_collect2'],
            'res_signed_bill_delivery' => $data['fieldData']['signed_bill_delivery'],
          ))
          ->condition('referenceNo', "365254_GST13826", '=')
          ->execute();

      // add trip id condition also in db_update
      //       ->condition('referenceNo', $data[''referenceNumber], '=')


      if (!empty($updated_bill_row_id)) {
         foreach ($data['fieldData']['item_details1'] as $key => $value) {
            db_update('dispatch_planning_item_details') // Table name no longer needs {}
                ->fields(array(
                  'res_sku_actual_quantity1' => $value['sku_actual_quantity1'],
                ))
                ->condition('trip_bill_reference', $updated_bill_row_id, '=')
                ->condition('sku_code', $value['sku_code1'], '=')
                ->execute();
         }
      }
      $results['flag'] = '1';
      $results['result'] = 'Data updated Sucessfully.';
   }
   else {
      // make log entry here
      dispatch_planning_api_log($json_data, '', 'farey_sales_deliver', 0, 0);
      $results['flag'] = '0';
      $results['result'] = 'No data was updated.';
   }
   // log entry and is remaining
   return $results;
}
