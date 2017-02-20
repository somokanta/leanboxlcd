<?php

// when validate is clicked
function change_feeds_import_validate_content($id) {
//  module_load_include('inc', 'change_feeds_import', 'form/upload_billing_data.inc');
  require_once drupal_get_path('module', 'change_feeds_import') . '/form/upload_billing_data.inc';

  // select from stage table which has error = 1 
  $result = db_select('stage_billing_data', 'bd')
          ->condition('bd.error_id', $id, '=')
          ->condition('bd.error', 1, '=')
          ->fields('bd', array(
            'vehicle',
            'loading_date',
            'party_code',
            'BEAT_NAME',
            'bill_number',
            'bill_date',
            'BASEPACK_CODE',
            'sku7_code',
            'product_name',
            'PRICELIST_GROUP',
            'PRODUCT_DIVISION',
            'PRODUCT_CATEGORY',
            'PRODUCT_SUB_CATEGORY',
            'mrp',
            'tur',
            'Sum_of_TOTAL_SALES_QTY',
           // 'Sum_of_TOTAL_SALES_RET_QTY',
            'Sum_of_NET_SALES_QTY',
            'Sum_of_SALES_FREE_QTY',
          //  'Sum_of_SALES_RET_FREE_QTY',
          //  'Sum_of_SALES_VALUE',
          //  'Sum_of_SALES_RET_VALUE',
             'Sum_of_NET_SALES_VALUE',
            'Sum_of_NET_SALES_CASES',
            'Sum_of_NET_SALES_KGS',
            'Sum_of_SCHEME_DISCOUNT',
            'Sum_of_OTHER_DISCOUNT',
            'Sum_of_TAX_PERCENTAGE',
          ))
          ->execute()->fetchAll(PDO::FETCH_ASSOC);

  $party_code_result = check_party_code_exist_or_not();
  $sku_code_result = check_sku7_code_exist_or_not();
  $bill_result = check_bill_number_exist();
  $master_result = get_node_titles(array('pricelist_group_master', 'product_division_master', 'sales_register_data'));
  
  $counter = 0;
  $bill_unique_count = 0;
  $got_error = 0;
  $error = array();
  $errors[] = $header = get_csv_file_header();

  foreach ($result as $key => $value) {
    $error_bool = 0;
    $total_qty = $value['Sum_of_TOTAL_SALES_QTY'];
    $party_code = $value['party_code'];
    $bill_number = $value['bill_number'];

    foreach ($value as $k => $v) {
      $flag = 0;
      $error_text = '';

      $v = trim($v);
      if (empty($v) && $v !== '0') {
        $flag = 1;
        $error_text = "$k can not be null";
      }
      else {
        
        if ($k == 'vehicle') {
          if ($v == "N.A.") {
            $error_text = $v . " Inavlid Vehicle";
            $flag = 1;
          }
        }
        
        if ($k == 'party_code') {
          if (!array_key_exists($v, $party_code_result)) {
            $error_text = $v . " PARTY_CODE is not present in PARTY_CODE Master";
            $flag = 1;
          }
        }
        elseif ($k == 'BEAT_NAME') {
          if (!empty($party_code)) {
            if ($party_code_result[$party_code] != $v) {
              $error_text = $v . " BEAT_NAME does not match with OUTLET_MASTER";
              $flag = 1;
            }
          }
        }
        elseif ($k == 'sku7_code') {
          if (!array_key_exists($v, $sku_code_result)) {
            $error_text = $v . " SKU7_CODE is not present in SKU7_CODE Master";
            $flag = 1;
          }
        }
        elseif ($k == 'bill_number') {
          if (array_key_exists($v, $bill_result)) {
            $error_text = $v . " BILL_NUMBER Already exist in Billing Data Table";
            $flag = 1;
          }
          if (!isset($master_result['sales_register_data'][$v])) {
            $error_text = $v . " is not present in SALES_REGISTER Master";
            $flag = 1;
          }
        }
        elseif ($k == 'PRICELIST_GROUP') {
          if (!isset($master_result['pricelist_group_master'][$v])) {
            $error_text = $v . " is not present in PRICELIST_GROUP Master";
            $flag = 1;
          }
        }
        elseif ($k == 'PRODUCT_DIVISION') {
          if (!isset($master_result['product_division_master'][$v])) {
            $error_text = $v . " is not present in PRODUCT_DIVISION Master";
            $flag = 1;
          }
        }
        elseif ($k == 'Sum_of_TOTAL_SALES_QTY') {
          $context['results']['sum_of_total_sales_qty'] += $total_qty;
        }
      }

      if ($flag) {
        $error[$k] = $error_text;
        $error_bool = 1;
      }
      else {
        $error[$k] = $v;
      }
    }

    $errors[] = $error;

    if ($error_bool) {
      $base_path = getcwd();

      global $user;
      $user_id = $user->uid;
      $distributor_id = change_feeds_import_get_distributor_id($user_id);
      $csv_file = $distributor_id . "_error.csv";
      convert_array_to_csv_export($errors, $base_path . "/sites/default/files/billing_data/$csv_file");
      $got_error = 1;
    }

    if (!isset($res[$bill_number])) {
      $res[$bill_number] = array();
      $bill_unique_count++;
    }

    $counter++;
  }

  //$bill_uploaded = $bill_unique_count;
  //$total_lines = ($counter - 1);
  if ($got_error == 1) {
    global $base_url;
    //$error_link = $base_url . '/sites/default/files/filter_error.csv';
    $status = "Error";
  }
  else {
    //$error_link = "";
    $status = "Confirm";
  }

  // change status of error billing data table
  change_feeds_import_update_data_in_error_billing_table($status, $id);
  if ($status == "Confirm") {
    // delete data from stage table after confirm clicked and no error occured 
    change_feeds_import_update_data_in_staging_billing_table($result, $id);
  }
  drupal_goto("confirm/" . $id);
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

function change_feeds_import_update_data_in_staging_billing_table($result, $id) {
  foreach ($result as $key => $value) {
    // change status in error_billing_data
    $status_updated = db_update('stage_billing_data') // Table name no longer needs {}
        ->fields(array(
          'error' => 0,
        ))
        ->condition('error_id', $id, '=')
        ->execute();
  }
}
