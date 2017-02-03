<?php

// when validate is clicked
function change_feeds_import_validate_content($id) {
  print 1;
 //require_once drupal_get_path('module', 'change_feeds_import').'/form/upload_billing_data.inc';
  print 1;

 // select from stage table which has error = 1 
  $result = db_select('stage_billing_data', 'bd')
          ->condition('bd.error_id', $id, '=')
          ->condition('bd.error', 1, '=')
          ->fields('bd')
          ->execute()->fetchAll(PDO::FETCH_ASSOC);



  $party_code_result = check_party_code_exist_or_not();
  $sku_code_result = check_sku7_code_exist_or_not();
  $bill_result = check_bill_number_exist();
  print 1;
  $counter = 0;
  $bill_unique_count = 0;
  $got_error = 0;
  $error = array();
  $billing_data = array();
  $party_packing_data = array();
  $error[0] = $header = array('VEHICLE', 'LOADING_DATE', 'PARTY_CODE', 'PARTY_NAME', 'BILL_NUMBER', 'BILL_DATE', 'PRODUCT_NAME', 'SKU7_CODE', 'MRP', 'TUR', 'Sum of TOTAL_SALES_QTY', 'Sum of TOTAL_SALES_RET_QTY', 'Sum of NET_SALES_QTY', 'Sum of SALES_FREE_QTY', 'Sum of SALES_RET_FREE_QTY', 'Sum of SALES_VALUE', 'Sum of SALES_RET_VALUE', 'Sum of NET_SALES_VALUE');


  foreach ($result as $key => $value) {
    $error_bool = 0;
    $vehicle = $value['vehicle'];
    $loading_date = $value['loading_date'];
    $total_qty = $value['Sum_of_TOTAL_SALES_QTY'];
    $sku = $value['sku7_code'];
    $party_code = $value['party_code'];
    $party_name = $value['party_name'];
    $bill_number = $value['bill_number'];
    $bill_date = $value['bill_date'];
    $product_name = $value['product_name'];
    $mrp = $value['mrp'];
    $tur = $value['tur'];
    $sum_of_total_sales_ret_qty = $value['Sum_of_TOTAL_SALES_RET_QTY'];
    $sum_of_net_sales_qty = $value['Sum_of_NET_SALES_QTY'];
    $sum_of_sales_free_qty = $value['Sum_of_SALES_FREE_QTY'];
    $sum_of_sales_ret_free_qty = $value['Sum_of_SALES_RET_FREE_QTY'];
    $sum_of_sales_value = $value['Sum_of_SALES_VALUE'];

    $sum_of_sales_ret_value = $value['Sum_of_SALES_RET_VALUE'];
    $Sum_of_NET_SALES_VALUE = $value['Sum_of_NET_SALES_VALUE'];

    if (empty($vehicle)) {
      $error[$counter][0] = "vehicle can not be null";
      $error_bool = 1;
    }
    else {
      $error[$counter][0] = $vehicle;
    }

    if (empty($loading_date)) {
      $error[$counter][1] = "Loading Date can not be null";
      $error_bool = 1;
    }
    else {
      $error[$counter][1] = $loading_date;
    }

    if (empty($party_code)) {
      $error[$counter][2] = "PARTY_CODE can not be null";
      $error_bool = 1;
    }
    else {
      if (!array_key_exists($party_code, $party_code_result)) {
        $error[$counter][2] = $party_code." PARTY_CODE is not present in PARTY_CODE Master";
        $error_bool = 1;
      }
      else {
        $error[$counter][2] = $party_code;
      }
    }

    if (empty($party_name)) {
      $error[$counter][3] = "PARTY_NAME can not be null";
      $error_bool = 1;
    }
    else {
      $error[$counter][3] = $party_name;
    }
    if (empty($bill_number)) {
      $error[$counter][4] = "BILL_NUMBER can not be null";
      $error_bool = 1;
    }
    else {
      if (array_key_exists($bill_number, $bill_result)) {
        $error[$counter][4] = $bill_number." BILL_NUMBER Already exist in Billing Data Table";
        $error_bool = 1;
      }
      else {
        $error[$counter][4] = $bill_number;
      }
    }
    if (empty($bill_date)) {
      $error[$counter][5] = "BILL_DATE can not be null";
      $error_bool = 1;
    }
    else {
      $error[$counter][5] = $bill_date;
    }

    if (empty($product_name)) {
      $error[$counter][6] = "PRODUCT_NAME can not be null";
      $error_bool = 1;
    }
    else {
      $error[$counter][6] = $product_name;
    }
    if (empty($sku)) {
      $error[$counter][7] = "SKU7_CODE can not be null";
      $error_bool = 1;
    }
    else {
      if (!array_key_exists($sku, $sku_code_result)) {
        $error[$counter][7] = $sku." SKU7_CODE is not present in SKU7_CODE Master";
        $error_bool = 1;
      }
      else {
        $error[$counter][7] = $sku;
      }
    }
    if (empty($mrp)) {
      $error[$counter][8] = "MRP can not be null";
      $error_bool = 1;
    }
    else {
      if (!is_numeric($mrp)) {
        $error[$counter][8] = $mrp." MRP is not Numeric";
        $error_bool = 1;
      }
      else {
        $error[$counter][8] = $mrp;
      }
    }

    if (empty($tur)) {
      $error[$counter][9] = "TUR can not be null";
      $error_bool = 1;
    }
    else {
      if (!is_numeric($tur)) {
        $error[$counter][9] = $tur." TUR is not Numeric";
        $error_bool = 1;
      }
      else {
        $error[$counter][9] = $tur;
      }
    }

    if (!is_numeric($total_qty)) {
      $error[$counter][10] = $total_qty." Sum of TOTAL_SALES_QTY is not Numeric";
      $error_bool = 1;
    }
    else {
      $error[$counter][10] = $total_qty;
      $sum_of_total_sales_qty += $total_qty;
    }

    if (!is_numeric($sum_of_total_sales_ret_qty)) {
      $error[$counter][11] = $sum_of_total_sales_ret_qty." Sum of TOTAL_SALES_RET_QTY is not Numeric";
      $error_bool = 1;
    }
    else {
      $error[$counter][11] = $sum_of_total_sales_ret_qty;
    }


    if (!is_numeric($sum_of_net_sales_qty)) {
      $error[$counter][12] = $sum_of_net_sales_qty." Sum of NET_SALES_QTY is not Numeric";
      $error_bool = 1;
    }
    else {
      $error[$counter][12] = $sum_of_net_sales_qty;
    }

    if (!is_numeric($sum_of_sales_free_qty)) {
      $error[$counter][13] = $sum_of_sales_free_qty." Sum of SALES_FREE_QTY is not Numeric";
      $error_bool = 1;
    }
    else {
      $error[$counter][13] = $sum_of_sales_free_qty;
    }


    if (!is_numeric($sum_of_sales_ret_free_qty)) {
      $error[$counter][14] = $sum_of_sales_ret_free_qty." Sum of SALES_RET_FREE_QTY is not Numeric";
      $error_bool = 1;
    }
    else {
      $error[$counter][14] = $sum_of_sales_ret_free_qty;
    }

    if (!is_numeric($sum_of_sales_value)) {
      $error[$counter][15] = $sum_of_sales_value." Sum of SALES_VALUE is not Numeric";
      $error_bool = 1;
    }
    else {
      $error[$counter][15] = $sum_of_sales_value;
    }

    if (!is_numeric($sum_of_sales_ret_value)) {
      $error[$counter][16] = $sum_of_sales_ret_value." Sum of SALES_RET_VALUE is not Numeric";
      $error_bool = 1;
    }
    else {
      $error[$counter][16] = $sum_of_sales_ret_value;
    }

    if (!is_numeric($Sum_of_NET_SALES_VALUE)) {
      $error[$counter][17] = $Sum_of_NET_SALES_VALUE." Sum of NET_SALES_VALUE is not Numeric";
      $error_bool = 1;
    }
    else {
      $error[$counter][17] = $Sum_of_NET_SALES_VALUE;
    }


    if ($error_bool) {
      global $base_url;
      $base_path = getcwd();
      convert_array_to_csv_export($error, $base_path . '/sites/default/files/filter_error.csv');
      $got_error = 1;
    }
    else {
      unset($error[$counter]);
    }

    if ($counter != 0) {
      if (isset($res[$bill_number])) {
        // do nothing
      }
      else {
        $res[$bill_number] = array();
        $bill_unique_count++;
      }
    }

    $counter++;
  }

  $bill_uploaded = $bill_unique_count;
  $total_lines = ($counter - 1);
  if ($got_error == 1) {
    global $base_url;
    $error_link = $base_url . '/sites/default/files/filter_error.csv';
    $status = "Error";
  }
  else {
    $error_link = "";
    $status = "Confirm";
  }

  // change status of error billing data table
  change_feeds_import_update_data_in_error_billing_table($status, $id);
  if ($status == "Confirm") {
   // delete data from stage table after confirm clicked and no error occured 
    change_feeds_import_update_data_in_staging_billing_table($result,$id);
  }
  drupal_goto("confirm/".$id);
  
}

function change_feeds_import_update_data_in_error_billing_table($status, $id) {
  // change status in error_billing_data
  $status_updated = db_update('error_billing_data') // Table name no longer needs {}
      ->fields(array(
        'status' => $status,
      ))
      ->condition('id', $id, '=')
      ->execute();
}

function change_feeds_import_update_data_in_staging_billing_table($result,$id) {
  foreach ($result as $key=>$value) {
     // change status in error_billing_data
  $status_updated = db_update('stage_billing_data') // Table name no longer needs {}
      ->fields(array(
        'error' => 0,
      ))
      ->condition('error_id', $id, '=')
      ->execute();
  }
}


