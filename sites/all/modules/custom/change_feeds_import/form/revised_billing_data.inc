<?php

/**
 * Upload form for revised billing data
 * @global type $user
 * @param type $form
 * @param type $form_state
 * @return string
 */
function change_feeds_import_revised_billing_data_form($form, $form_state) {
  global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);

  $did = check_distributor_incompleted_upload($distributor_id);
  if (!empty($did)) {
    drupal_goto("confirm/$did");
  }

  $form['file'] = array(
    '#type' => 'file',
    '#title' => t('Choose a CSV file for revised billing data upload'),
  );
  $form['template_download'] = array(
    '#type' => 'markup',
    '#prefix' => '<div id="template_wrapper">',
    '#suffix' => '</div>',
    '#markup' => l("Download Template", drupal_get_path('module', 'change_feeds_import') . '/revised_billing_data_template.csv'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Upload'),
  );

  return $form;
}

/**
 * 
 * @param type $form
 * @param type $form_state
 */
function change_feeds_import_revised_billing_data_form_validate($form, &$form_state) {
  $file = file_save_upload('file', array(
    'file_validate_extensions' => array('csv'),
  ));
  //used same function because same file is for both billing and revised billing upload
  $header_array_from_xsd = get_csv_file_header_for_revised_billing();

  if ($file) {
    if ($file = file_move($file, 'public://')) {
      $form_state['values']['file'] = $file;
      $filepath = $form_state['values']['file']->uri;
      $data = array_map("str_getcsv", file($filepath, FILE_SKIP_EMPTY_LINES));
      $header_from_csv = array_shift($data);
      if ($header_array_from_xsd != $header_from_csv) {
        $type = 'error';
        $message = 'Header mismatch.';
        form_set_error('file_upload', t($message));
        // log
      }
      if (count($data) > 15001) {
        // Count includes header count as well
        $message = 'File Row Count should not be greater than 15000';
        form_set_error('file_upload', t($message));
      }
      else {
        if (!isset($res['unique_combination'])) {
          $res['unique_combination'] = array();
        }

        foreach ($data as $key => $value) {
          $bill_no = $value[4];
          $sku = $value[7];
          $unique_comb = $bill_no . $sku;
          if (array_key_exists($unique_comb, $res['unique_combination'])) {
            form_set_error('duplicate_data', t('Duplicate Data Found , Please Remove Duplicate Data and Upload Again.'));
          }
          $res['unique_combination'][$unique_comb] = $unique_comb;
        }
      }
    }
    else {
      form_set_error('file_upload', t('Failed to write the uploaded file the site\'s file folder.'));
    }
  }
  else {
    form_set_error('file', t('No file was uploaded.'));
  }
}

/**
 * 
 * @global type $user
 * @param type $form
 * @param type $form_state
 */
function change_feeds_import_revised_billing_data_form_submit($form, &$form_state) {
  global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);
  $_SESSION['distributor_id'] = $distributor_id;
  $file = $form_state['values']['file'];
  $file_name = $file->filename;
  $filepath = $file->uri;

  $max = count(file($file->uri));
  $datas = array();
  // Making data as associative array with header.
  if (($handle = fopen($filepath, "r")) !== FALSE) {
    $header = fgetcsv($handle, 0, ',', '"');

    while (($row = fgetcsv($handle) ) !== FALSE) {

      foreach ($row as $key => &$value) {
        $value = mb_convert_encoding($value, 'UTF-8', 'UTF-8');
      }

      $datas[] = array_combine($header, $row);
    }
    fclose($handle);
  }

  $error = change_feeds_import_revised_billing_validate($datas, $distributor_id);
  if (!empty($error)) {
    // show error here
    $error_line = implode("<br>", $error);
    drupal_set_message("The following error occured while processing: <br>" . $error_line, 'error');
  }
  else {

    $unique_bills = array_unique(array_map(function($elem) {
        return $elem['BILL_NUMBER'];
      }, $datas));

        $bill_not_in_dis_qty_conf = find_bill_not_in_dispatch_qty_confirmation($unique_bills, $distributor_id);
        //upload only cld and loose confirmed bills
        if(is_array($bill_not_in_dis_qty_conf)){
          $bills_present = array_diff($unique_bills,$bill_not_in_dis_qty_conf);
        }else{// if no pending then upload all bills
          $bills_present = $unique_bills;
        }
        
        $cld_loose_pending_bills = check_bill_cld_loose_confirmation_pending($bills_present, $distributor_id);
        //upload only cld and loose confirmed bills
        if(is_array($cld_loose_pending_bills)){
          $bills_cl_confirmed = array_diff($bills_present,$cld_loose_pending_bills);
        }else{// if no pending then upload all bills
          $bills_cl_confirmed = $bills_present;
        }
      
      // check bill confirm edit in trip table for all bills. if its close then don't upload the data
      //we can send $unique_bills as well as $bills_cl_confirmed. because result is same
        $confirmed_bills = check_bill_confirmation_complete($bills_cl_confirmed, $distributor_id);
        if (is_array($confirmed_bills)) {
          $bills_upload = array_diff($bills_cl_confirmed, $confirmed_bills);
        }
        else {
          $bills_upload = $bills_cl_confirmed;
        }
//      if (empty($confirmed_bills)) {
        $operations = array();
        $operations[] = array('change_feeds_import_revised_updated_flag_unset', array($bills_upload, $distributor_id));
        
        $data_chunk = array_chunk($datas, 1000);
        foreach ($data_chunk as $data) {
          $operations[] = array('change_feeds_import_revised_billing_process', array($data, $bills_upload, $distributor_id));
        }

        //For bill update flag which are not present in file check isnull in function
        $operations[] = array('change_feeds_import_revised_billing_update_flag', array($bills_upload, $distributor_id));
        
        //Sales register Upload
        $unique_bills_chunk = array_chunk($bills_upload, 300);
        foreach ($unique_bills_chunk as $bills_chunk) {
          $operations[] = array('sales_register_update_create_billing', array($bills_chunk,$distributor_id));
        }
        $batch = array(
          'title' => t('Reading Billing Data Csv'),
          'operations' => $operations,
          'init_message' => t("Batch is starting."),
          'progress_message' => t('Processed @current out of @total.'),
          'error_message' => t('Upload data processing has encountered an error.'),
          'finished' => 'revised_billing_data_csv_finished',
          'file' => drupal_get_path('module', 'change_feeds_import') . '/form/revised_billing_data.inc',
        );

        batch_set($batch);
//      }
//      else {
//        $trip_ids = implode(",", $confirmed_bills);
//        drupal_set_message("Bill confirmation status is completed for Bills: $trip_ids", 'warning');
//      }
//    }
//    else {
//      $bills = implode(",", $cld_loose_pending_bills);
//      drupal_set_message("CLD or Loose confirmation is pending for bills: $bills", 'error');
//    }
  }
}

/**
 * 
 * @param type $success
 * @param type $results
 * @param type $operations
 */
function revised_billing_data_csv_finished($success, $results, $operations) {

  if ($success) {
    drupal_set_message('Data Updated Successfully');
  }
  else {
    drupal_set_message('There is Error Occured in batch process');
  }
}

/**
 * 
 * @param type $data
 * @param type $distributor_id
 * @return string
 */
function change_feeds_import_revised_billing_validate($data, $distributor_id) {
  $bill_sku_combination = get_bill_sku_from_custom_billing($distributor_id);

  $error = array();
  foreach ($data as $key => $value) {
    $error_text = '';
    $total_qty = $value['Sum of TOTAL_SALES_QTY'];
    $net_sales_qty = $value['Sum of NET_SALES_QTY'];
    $sales_free_qty = $value['Sum of SALES_FREE_QTY'];
    
    foreach ($value as $k => $v) {
      $v = trim($v);
      if (empty($v) && $v !== '0') {
        $error_text = "$k can not be null";
      }


      if (strstr($k, 'Sum') !== FALSE) {
        if (!is_numeric($v)) {
          $error_text = "$v in $k Should be numeric";
        }
        else {
          if ($k == 'Sum of TOTAL_SALES_QTY') {

            if ($total_qty != $net_sales_qty + $sales_free_qty) {
              $error_text = "$total_qty in $k should be Sum of NET_SALES_QTY + Sum of SALES_FREE_QTY";
            }
            if ($total_qty < 0) {
              $error_text = "$total_qty in $k cannot be negative";
            }
            if (floatval($total_qty) == 0) {
              $error_text = "$total_qty in $k cannot be zero";
            }
          }
          if ($k == 'Sum of NET_SALES_QTY' && $net_sales_qty < 0) {
            $error_text = "$net_sales_qty in $k cannot be negative";
          }
          if ($k == 'Sum of SALES_FREE_QTY' && $sales_free_qty < 0) {
            $error_text = "$sales_free_qty in $k cannot be negative";
          }
        }
      }
    }

    if (!isset($context['results']['unique_combination'])) {
      $context['results']['unique_combination'] = array();
    }

    $unique_comb = $value['BILL_NUMBER'] . '-' . $value['SKU7_CODE'];

    if (in_array($unique_comb, $context['results']['unique_combination'])) {
      $error_text = "BILL_NUMBER " . $value['BILL_NUMBER'] . " and SKU7_CODE " . $value['SKU7_CODE'] . " is Repeated.  ";
    }
    else {
      $context['results']['unique_combination'][] = $unique_comb;
    }

    if (!array_key_exists($unique_comb, $bill_sku_combination)) {
      $error_text = $unique_comb . " BILL_NUMBER-SKU not exist in Billing Data Table";
    }
    if (!empty($error_text)) {
      $error[] = $error_text;
    }
  }
  return $error;
}

/**
 * 
 * @param type $billing_data_result
 * @param type $distributor_id
 */
function change_feeds_import_revised_updated_flag_unset($unique_bills, $distributor_id, &$context) {
    if (is_array($unique_bills)&& !empty($unique_bills)){
      db_update('custom_billing_data')
        ->fields(array(
          'updated_flag' => NULL
        ))
        ->condition('bill_number', $unique_bills, 'IN')
        ->condition('distributor_id', $distributor_id)
        ->execute();
  }
}

/**
 * 
 * @param type $billing_data_result
 * @param type $distributor_id
 */
function change_feeds_import_revised_billing_process($billing_data_result, $unique_bills, $distributor_id, &$context) {
  foreach ($billing_data_result as $key => $data) {
    if (in_array($data['BILL_NUMBER'], $unique_bills)){
      $to_be_dispatch_qty = calculte_to_be_dispatch_qty($data['BILL_NUMBER'], $data['SKU7_CODE'], $distributor_id);
      db_update('custom_billing_data')
        ->fields(array(
          //while reupload stor total sales qty in revised host qty field
          'revised_host_quantity' => $data['Sum of TOTAL_SALES_QTY'],
          //total sales qty is calculated from dispatch qty confirmation table based on bill sku
          'Sum_of_TOTAL_SALES_QTY' => $to_be_dispatch_qty ? $to_be_dispatch_qty : 0,
          'revised_Sum_of_NET_SALES_VALUE' => $data['Sum of NET_SALES_VALUE'],
          'revised_Sum_of_SCHEME_DISCOUNT' => $data['Sum of SCHEME_DISCOUNT'],
          'revised_Sum_of_OTHER_DISCOUNT' => $data['Sum of OTHER_DISCOUNT'],
          'revised_Sum_of_TAX_PERCENTAGE' => $data['Sum of TAX_PERCENTAGE'],
          'updated_date' => REQUEST_TIME,
          'updated_flag' => 1
        ))
        ->condition('bill_number', $data['BILL_NUMBER'])
        ->condition('sku7_code', $data['SKU7_CODE'])
        ->condition('distributor_id', $distributor_id)
        ->execute();
    }
  }
}

/**
 * 
 * @param type $billing_data_result
 * @param type $distributor_id
 */
function change_feeds_import_revised_billing_update_flag($unique_bills, $distributor_id, &$context) {
  if (is_array($unique_bills) && !empty($unique_bills)) {
    $billing_data_result = db_select('custom_billing_data', 'cbd')
        ->condition('cbd.bill_number', $unique_bills, 'IN')
        ->isNull('updated_flag')
        ->condition('distributor_id', $distributor_id)
        ->fields('cbd')
        ->execute()->fetchAll(PDO::FETCH_ASSOC);

    foreach ($billing_data_result as $key => $data) {
      $to_be_dispatch_qty = calculte_to_be_dispatch_qty($data['bill_number'], $data['sku7_code'], $distributor_id);
      db_update('custom_billing_data')
        ->fields(array(
          //total sales qty is calculated from dispatch qty confirmation table based on bill sku
          'Sum_of_TOTAL_SALES_QTY' => $to_be_dispatch_qty ? $to_be_dispatch_qty : 0,
          'revised_host_quantity' => 0, //it is set to zero for bills which sku is not found in csv.
          'revised_Sum_of_NET_SALES_VALUE' => 0,
          'revised_Sum_of_SCHEME_DISCOUNT' => 0,
          'revised_Sum_of_OTHER_DISCOUNT' => 0,
          'revised_Sum_of_TAX_PERCENTAGE' => 0,
          'updated_date' => REQUEST_TIME,
          'updated_flag' => 1
        ))
        ->condition('bill_number', $data['bill_number'])
        ->condition('sku7_code', $data['sku7_code'])
        ->condition('distributor_id', $distributor_id)
        ->execute();
    }
  }
}

/**
 * 
 * @param type $bill_number
 * @param type $sku_code
 * @param type $distributor_id
 * @return type
 */
function calculte_to_be_dispatch_qty($bill_number, $sku_code, $distributor_id) {
  $query = db_select('dispatch_qty_confirmation', 'dqc');
  $query->condition('dqc.bill_number', $bill_number);
  $query->condition('dqc.sku_code', $sku_code);
  $query->condition('dqc.distributor_id', $distributor_id);
  $query->fields('dqc', array('to_be_dispatch_qty'));
  $to_be_dispatch_qty = $query->execute()->fetchField();
  return $to_be_dispatch_qty;
}

/**
 * 
 * @return type
 */
function get_csv_file_header_for_revised_billing() {
  return array(
    'VEHICLE',
    'LOADING_DATE',
    'PARTY_CODE',
    'BEAT_NAME',
    'BILL_NUMBER',
    'BILL_DATE',
    'BASEPACK_CODE',
    'SKU7_CODE',
    'PRODUCT_NAME',
    'PRICELIST_GROUP',
    'PRODUCT_DIVISION',
    'PRODUCT_CATEGORY',
    'PRODUCT_SUB_CATEGORY',
    'MRP',
    'TUR',
    'Sum of TOTAL_SALES_QTY',
    'Sum of NET_SALES_QTY',
    'Sum of SALES_FREE_QTY',
    'Sum of NET_SALES_VALUE',
    'Sum of NET_SALES_CASES',
    'Sum of NET_SALES_KGS',
    'Sum of SCHEME_DISCOUNT',
    'Sum of OTHER_DISCOUNT',
    'Sum of TAX_PERCENTAGE',
  );
}

/**
 * Get last 20 days billing data details
 * @param type $distributor_id
 */
function get_bill_sku_from_custom_billing($distributor_id) {
  $time = strtotime('-20 days');  // last 1 month

  $query = db_select('custom_billing_data', 'cbd');
  $query->condition('cbd.distributor_id', $distributor_id);
  $query->condition('cbd.created_date', $time, '>');
  $query->addExpression("CONCAT(cbd.bill_number, '-', cbd.sku7_code)", "grp_concat");
  $bill_sku_combination = $query->execute()->fetchAllKeyed(0, 0);
  return $bill_sku_combination;
}

/**
 * 
 * @param type $unique_bills
 * @param type $distributor_id
 */
function check_bill_confirmation_complete($unique_bills, $distributor_id) {
  $confirmed_bills = array();
  if (is_array($unique_bills) && !empty($unique_bills)) {
    $query = db_select('node', 'n');
    $query->condition('n.type', "sales_register_data");
    $query->condition('n.uid', "$distributor_id");
    $query->condition('n.title', $unique_bills, 'IN');
    $query->join('field_data_field_trip_id_reference', 'tir', 'tir.entity_id = n.nid');
    $query->join('node', 'n2', 'n2.nid = tir.field_trip_id_reference_nid');
    $query->leftjoin('field_data_field_confirm_bill_edit_status', 'bds', 'bds.entity_id = n2.nid');
    $query->condition('bds.field_confirm_bill_edit_status_value', 1, '=');
    $query->addField('n', 'title');
    $confirmed_bills = $query->execute()->fetchAllKeyed(0, 0);
  }
  return $confirmed_bills;
}

/**
 * 
 * @param type $unique_bills
 * @param type $distributor_id
 * @return type
 */
function check_bill_cld_loose_confirmation_pending($unique_bills, $distributor_id) {
  $bill_numbers = array();
  if (is_array($unique_bills) && !empty($unique_bills)) {
    $query = db_select('dispatch_qty_confirmation', 'n');
    $query->condition('n.bill_number', $unique_bills, "IN");
    $query->condition('n.distributor_id', "$distributor_id");

    $db_or = db_or();
    $db_or->condition('n.loose_close_flag', 0, '=');
    $db_or->condition('n.cld_close_flag', 0, '=');
    $query->condition($db_or);

    $query->addField('n', 'bill_number');
    $bill_numbers = $query->execute()->fetchAllKeyed(0, 0);
  }
  return $bill_numbers;
}
/**
 * 
 * @param type $unique_bills
 * @param type $distributor_id
 * @return type
 */
function find_bill_not_in_dispatch_qty_confirmation($unique_bills, $distributor_id) {
  $bills_not_present = array();
  if (is_array($unique_bills) && !empty($unique_bills)) {
    $query = db_select('dispatch_qty_confirmation', 'n');
    $query->condition('n.bill_number', $unique_bills, "IN");
    $query->condition('n.distributor_id', "$distributor_id");  
    $query->addField('n', 'bill_number');
    $query->groupBy('n.bill_number');
    $bill_numbers = $query->execute()->fetchAllKeyed(0, 0);
    //find bill not present in
    if(!empty($bill_numbers)){
      $bills_not_present = array_diff($unique_bills,$bill_numbers);
    }
  }
  return $bills_not_present;
}



/**
 * 
 * @param type $unique_bills
 * @param type $distributor_id
 * @return type
 */
function sales_register_data_by_revised_billing_upload($unique_bills, $distributor_id) {
  $results = array();
  if (is_array($unique_bills) && !empty($unique_bills)) {
    $query = db_select('custom_billing_data', 'cb');
    $query->addField('cb', 'bill_number');
    $query->addExpression('SUM(cb.Sum_of_NET_SALES_VALUE)', 'sum_net_sales_value');
    $query->addExpression('SUM(Sum_of_TOTAL_SALES_QTY * line_level_value)', 'custom_bill_sales_value');
    $query->condition('cb.distributor_id', $distributor_id);
    $query->condition('cb.bill_number', $unique_bills, 'IN');
    $query->groupBy('cb.bill_number');
    $results = $query->execute()->fetchAll();
  }
  return $results;
}

/**
 * 
 * @param type $data
 * @param type $context
 */
function sales_register_update_create_billing($bills_chunk, $distributor_id, &$context) {
  
  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['progress'] = 0;
  }
  $context['message'] = t('Now Updating Billing Data');
  
  //fetch bill details
  $data = sales_register_data_by_revised_billing_upload($bills_chunk,$distributor_id);

  foreach ($data as $key => $value) {
    try {
      $nid = check_node_title_exists($value->bill_number, 'sales_register_data');
      $node = node_load($nid);
      if ($node) {
            $sum_of_net_sales_value = round($value->sum_net_sales_value, 2);
            $node->field_bill_net_sales[LANGUAGE_NONE][0]['value'] = $sum_of_net_sales_value;
            // sum of net sales value from custom_billing_data to sales register

            //new feild bill value revised. Task TP to Host entry
            $bill_discount = $node->field_bill_level_discount[LANGUAGE_NONE][0]['value'];
            $calc_bill_value = $value->custom_bill_sales_value - $bill_discount;
            if ($calc_bill_value >= 1) {
              $updated_bill_value = $calc_bill_value;
            }
            else if ($calc_bill_value < 1 && $calc_bill_value > -1) {
              $updated_bill_value = 0; // no point value. take it zero
            }
            else {
              // if value is in minus then collect full money. no discount given
              //$updated_bill_value = $value->custom_bill_sales_value; 
              
              //above logic change request. keeping above one because, might be they want to revert it
              $updated_bill_value = 0; // if negative then show zero now
            }
            $node->field_bill_value[LANGUAGE_NONE][0]['value'] = round($updated_bill_value);
            
            // logic for pod flag below
            $sales_bill_value = $node->field_bill_value[LANGUAGE_NONE][0]['value'];
            $pod_value = (($sum_of_net_sales_value - $sales_bill_value) / $sum_of_net_sales_value) * 100;
            $node_uid = $node->uid;

            // fetch pod threshold for distributor
            $user_pod_value = _get_pod_threshold_value($node_uid);

            // check if $pod_value is greater than or not $user_pod_value
            $dispatch_date = '';
            $trip_id = $node->field_trip_id_reference[LANGUAGE_NONE][0]['nid'];
            if(!empty($trip_id)){
              $dispatch_date = check_dispatch_date($trip_id);
            }
            if (!empty($user_pod_value) && empty($dispatch_date)) {
               if ($pod_value >= $user_pod_value) {
                  $node->field_pod_flag[LANGUAGE_NONE][0]['value'] = 1;
                  // make the call type 'highly discounted'
                  $node->field_call_type['und'][0]['value'] = 'hd';
               }
            }
            node_save($node);
            watchdog('testingg $node', '<pre>' . print_r($node, TRUE) . '</pre>');
         }
      }
    catch (Exception $ex) {
      //Write mail logic
    }
  }
  $context['results']['billing'] = 'Billing Created';
}