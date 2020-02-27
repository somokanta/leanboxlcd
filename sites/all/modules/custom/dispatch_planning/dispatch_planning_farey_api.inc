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
      //$dis_id = leanbox_mobile_get_distributor_using_rscode($data['hubCode']);
      $referenceNumber = explode("_",$data['referenceNumber']);
      $actual_trip = $referenceNumber[0];
      $actual_rs = $referenceNumber[1];
      //$dis_id = get_distributorid_of_trip($actual_trip);
      //$dis_id = get_trip_distributor_id_using_rs_code($actual_rs,$actual_trip); 
      $dis_id = get_distributor_id_from_reference_number(trim($data['referenceNumber']));
      $json_data = json_encode($data);
      dispatch_planning_api_log($json_data, $data['jobData']['runsheet_number'], 'farey_sales_deliver', $dis_id, 1);
      $bill_row_id = _get_bill_row_id_manual_using_reference_no($data['referenceNumber']);
      if (!empty($bill_row_id)) {
      
      //payment outcome response will come from our delivery app  
      if (isset($data['fieldData']['payment_outcome'])) {
          if (is_array($data['fieldData']['payment_outcome'])) {
                $money_collect = implode(",", $data['fieldData']['payment_outcome']);
          } else {
                $money_collect = str_replace("&", ",", $data['fieldData']['payment_outcome']);
          }
      }  
        
      if (isset($data['fieldData']['money_collect2'])) {
          if (is_array($data['fieldData']['money_collect2'])) {
                $money_collect = implode(",", $data['fieldData']['money_collect2']);
          } else {
                $money_collect = str_replace("&", ",", $data['fieldData']['money_collect2']);
          }
        }
       if (isset($data['fieldData']['money_collect3'])) {
            if (is_array($data['fieldData']['money_collect3'])) {
                $money_collect = implode(",", $data['fieldData']['money_collect3']);
            } else {
                $money_collect = str_replace("&", ",", $data['fieldData']['money_collect3']);
            }
        }
        if (isset($data['fieldData']['proof_of_undelivery'])) {
            if (is_array($data['fieldData']['proof_of_undelivery'])) {
                $proof_of_undelivery = implode(",", $data['fieldData']['proof_of_undelivery']);
          } else {
                $money_collect = str_replace("&", ",", $data['fieldData']['proof_of_undelivery']);
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
            'derived_payment_outcome' => (!empty($money_collect) && ($money_collect != 'NA')) ? $money_collect  : (($data['fieldData']['signed_bill_delivery'] == 'Yes' || $data['fieldData']['signed_bill_partial'] == 'Yes') ? 'Signed' : ''),
            'payment_outcome' => isset($data['fieldData']['payment_outcome']) ? $money_collect  : '',
            'res_signed_bill_delivery' => isset($data['fieldData']['signed_bill_delivery']) ? $data['fieldData']['signed_bill_delivery'] : '',
            'res_cash_amount' => isset($data['fieldData']['cash_amount']) ? $data['fieldData']['cash_amount'] : '',
            'res_to_be_collection' => isset($data['fieldData']['to_be_collection']) ? $data['fieldData']['to_be_collection'] : '',
            'res_signed_bill_partial' => isset($data['fieldData']['signed_bill_partial']) ? $data['fieldData']['signed_bill_partial'] : '',
            'res_reason_for_failure' => isset($data['fieldData']['reason_for_failure']) ? $data['fieldData']['reason_for_failure'] : '',
            'photo_of_signed_bill_partial' => isset($data['fieldData']['photo_of_signed_bill_partial']) ? $data['fieldData']['photo_of_signed_bill_partial'] : '',
            'res_cheque_no' => isset($data['fieldData']['cheque_no']) ? $data['fieldData']['cheque_no'] : 'NA',
            'res_delivery_boy' => isset($data['fieldData']['delivery_b']['delivery_boy_value']) ? $data['fieldData']['delivery_b']['delivery_boy_value'] : '',
            'res_proof_of_undelivery' => !empty($proof_of_undelivery) ? $proof_of_undelivery : '',
            'updated_flag' => 2,
            'updated_time' => REQUEST_TIME,  
            'res_updated_time' => isset($data['transactionDate']) ? strtotime($data['transactionDate']) : 0,//REQUEST_TIME,

          ))
          ->condition('referenceNo', $data['referenceNumber'], '=')
          ->execute();

      if (!empty($bill_row_id) && isset($data['fieldData']['item_details1']) && !empty($data['fieldData']['item_details1'])) {
         watchdog('farey data 1', '<pre>' . print_r($data['fieldData']['item_details1'], TRUE) . '</pre>');

         foreach ($data['fieldData']['item_details1'] as $key => $value) {
            
            // fetch dispatch_qty here
            // ignore if value is not in form of array
            if (!is_array($value)) {
               continue;
            }
            $res_data = _get_dispatch_qty_item_details($bill_row_id, $value['sku_code1']);
            $return_qty = $res_data['dispatch_qty'] - $value['sku_actual_quantity1'] - $res_data['res_cashier_short'];
            
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
         watchdog('farey data', '<pre>' . print_r($data['fieldData']['item_details'], TRUE) . '</pre>');


         foreach ($data['fieldData']['item_details'] as $key => $value) {

            // ignore if value is not in form of array
            if (!is_array($value)) {
               continue;
            }
            
            $res_data = _get_dispatch_qty_item_details($bill_row_id, $value['sku_code']);
            $return_qty = $res_data['dispatch_qty'] - $value['sku_actual_quantity'] - $res_data['res_cashier_short'];
            $line_status = '';
            $ret_cash = $return_qty + $res_data['res_cashier_short']; //Return Qty + Cashier Short  
            $ret_cash_godown = $ret_cash + $res_data['res_godown_short']; //Return Qty + Cashier Short + Godown Short
            if ($ret_cash == $res_data['dispatch_qty'] ) {
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
                  'app_return_qty' => isset($value['sku_actual_ret_quantity']) ? $value['sku_actual_ret_quantity'] : 0, //app return qty
                  'line_level_status' => $line_status, // line level status
                ))
                ->condition('trip_bill_reference', $bill_row_id, '=')
                ->condition('sku_code', $value['sku_code'], '=')
                ->execute();
         }
      }
      if ($data['status'] == 'undeliver' && !empty($updated_bill_row_id)) {
        watchdog('farey data undeliver', '<pre>' . print_r($data['fieldData']['item_details'], TRUE) . '</pre>');
        db_update('dispatch_planning_item_details')
          ->expression('return_qty', 'dispatch_qty')
          ->fields(array(
            'res_sku_actual_quantity1' => 0,
            'farey_delivered_qty' => 0,
            'line_level_status' => 'line_undeliver',
          ))
          ->condition('trip_bill_reference', $bill_row_id, '=')
          ->execute();
        
          // new change - added on 31 Oct, 2019
        if(!empty($data['latitude']) && !empty($data['longitude']) ){
          try{
            $api_result = get_reverse_geocode_data($data['latitude'], $data['longitude'], $data['referenceNumber'],$dis_id); // call to mapmyindia api //reverse_geocode_results($data['latitude'], $data['longitude']);
            watchdog('map my india api result: ', '<pre>' . print_r($api_result, TRUE) . '</pre>');
            if(!empty($api_result)){
              $decoded_locality = $api_result[0]->locality; // area
              $decoded_sublocality = $api_result[0]->subLocality; //subarea
              $decoded_street = $api_result[0]->street;

              db_update('dispatch_planning_api_data')
                ->fields(array(
                    'rev_geocode_area' => isset($decoded_locality) ? $decoded_locality : '',
                    'rev_geocode_sub_area' => isset($decoded_sublocality) ? $decoded_sublocality : '',
                    'rev_geocode_street' => isset($decoded_street) ? $decoded_street : ''
                ))
                ->condition('referenceNo', $data['referenceNumber'], '=')
                ->execute();
            }
          }
          catch(Exception $e){
            //emailer for notification start
            global $base_url;
            $bill_ref_no = $data['referenceNumber'];
            $params['subject'] = "MapMyIndia API Exception on $base_url for Bill Reference No: $bill_ref_no";
            $params['body'] = $e->getMessage();
            $mail_to = variable_get('mapmyindia_exception_email',"abhishek.r@iksula.com,rahul.maurya@iksulaops.com"); //kusha@leanbox.in
            $from = "MIS.HELPDESK@leanbox.in";
            drupal_mail('dispatch_planning', 'bill_date_missing', $mail_to, LANGUAGE_NONE, $params, $from);
            //emailer for notification end
          }
        }

      }
      
      $bill_number = $data['jobData']['bill_number'];      
      $query = db_select('node', 'n');
      $query->condition('n.title', $bill_number);
      $query->condition('n.type', 'sales_register_data');
      $query->condition('n.uid', $dis_id);
      $query->fields('n', array('nid'));
      $sales_register_data_nid = $query->execute()->fetchField();
    

      $node = node_load($sales_register_data_nid);
      if(!empty($node)){
        $node->field_bill_status['und'][0]['value'] = $data['status'];
        node_save($node);
      }
      
      
     /**
       * New code. Create below three field on dispatch data content type
       * Inarea_Reached_DateTime
       * Inarea_Reached_Latitude
       * Inarea_Reached_Longitude
       */
      $Inarea_Reached_DateTime = $data['Inarea_Reached_DateTime'];
      $Inarea_Reached_Latitude = $data['Inarea_Reached_Latitude'];
      $Inarea_Reached_Longitude = $data['Inarea_Reached_Longitude'];
      if(!empty($Inarea_Reached_DateTime) || !empty($Inarea_Reached_Latitude) || !empty($Inarea_Reached_Longitude) ){
        $trip_id = $data['jobData']['runsheet_number'];
        
        $query = db_select('node', 'n');
        $query->condition('n.title', $trip_id);
        $query->condition('n.type', 'dispatch_data');
        $query->condition('n.uid', $dis_id);
        $query->fields('n', array('nid'));
        $dispatch_data_nid = $query->execute()->fetchField();


        $dispatch_node = node_load($dispatch_data_nid);
        if(!empty($dispatch_node)){
          $dispatch_node->field_inarea_reached_datetime['und'][0]['value'] = strtotime($Inarea_Reached_DateTime);
          $dispatch_node->field_inarea_reached_latitude['und'][0]['value'] = $Inarea_Reached_Latitude;
          $dispatch_node->field_inarea_reached_longitude['und'][0]['value'] = $Inarea_Reached_Longitude;
          node_save($dispatch_node);
        }
      }
      //END
      
      
      $results['flag'] = '1';
      $results['result'] = 'Data updated Sucessfully.';
      }else{
           $results['flag'] = '0';
          $results['result'] = 'No data was updated.';
      }
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


// new change - added on 31 Oct, 2019
function get_reverse_geocode_data($latitude, $longitude, $reference_number,$dis_id=0){
  $api_result = array();
  if(!empty($latitude) && !empty($longitude)){
      $license_key = variable_get('mapmyindia_rest_key', 'z94i9hvuqcsz7amywt76aknw8dx9o15z');   
      $url = "https://apis.mapmyindia.com/advancedmaps/v1/$license_key/rev_geocode?lng=$longitude&lat=$latitude";
        $sql_ins_id = insert_map_my_india_api_log($url, $reference_number, $dis_id);
//        if (trim($longitude, 0) == 1.11 || trim($latitude, 0) == 1.11) {
        if((explode('.', $longitude)[0]) <= 1 || (explode('.', $latitude)[0]) <= 1) {
            $result = '';
        } else {
            $options = array('method' => 'GET');
            $result = drupal_http_request($url, $options);
        }
      update_map_my_india_api_log($sql_ins_id, json_encode($result));
      if($result->status_message === 'OK'){
          $decoded_data = json_decode($result->data);
          $api_result = $decoded_data->results;
      }
  }
  return $api_result;
}

function insert_map_my_india_api_log($url, $reference_number,$dis_id){
  $sql_ins_id = db_insert('map_my_india_api_log')
     ->fields(array(
        'reference_number' => $reference_number,
        'request_data' => $url,
        'request_time' => time(),
        'distributor_id'=>$dis_id,
     ))
     ->execute();

  return $sql_ins_id;
}

function update_map_my_india_api_log($sql_ins_id, $response_json){
  db_update('map_my_india_api_log')
     ->fields(array(
     'response_data' => $response_json,
     ))
     ->condition('id', $sql_ins_id, '=')
     ->execute();
}
function get_distributor_id_from_reference_number($reference_no) {
   $distributor_id = ''; 
  if (!empty($reference_no)) {
     $query = db_select('dispatch_planning_api_data', 'ad');
     $query->join('node', 'n', 'n.title = ad.runsheet_number and n.uid = ad.distributor_id');
     $query->condition('ad.referenceNo', $reference_no, '=');
     $query->condition('n.type', 'dispatch_data', '=');
     $query->fields('ad', array('distributor_id'));
     $distributor_id = $query->execute()->fetchField();
  }  
  return $distributor_id;
}
