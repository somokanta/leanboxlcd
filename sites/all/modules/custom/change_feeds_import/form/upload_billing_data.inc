<?php

// upload form for billing data
function change_feeds_import_billing_data_form($form, $form_state) {
  global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);

  $did = check_distributor_incompleted_upload($distributor_id);
  if (!empty($did)) {
    drupal_goto("confirm/$did");
  }

  $link = l(t('Master Upload Imports'), 'content/master-upload'); // all master upload link.
  $form['template'] = array(
    '#markup' => $link,
  );

  $form['file'] = array(
    '#type' => 'file',
    '#title' => t('Choose a CSV file for billing data upload'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Upload'),
  );



  return $form;
}

function change_feeds_import_billing_data_form_validate($form, &$form_state) {
  $file = file_save_upload('file', array(
    'file_validate_extensions' => array('csv'),
  ));
  if ($file) {
    if ($file = file_move($file, 'public://')) {
      $form_state['values']['file'] = $file;
    }
    else {
      form_set_error('file', t("Failed to write the uploaded file to the site's file folder."));
    }
  }
  else {
    form_set_error('file', t('No file was uploaded.'));
  }
}

function change_feeds_import_billing_data_form_submit($form, &$form_state) {
  global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);
  $_SESSION['distributor_id'] = $distributor_id;
  $file = $form_state['values']['file'];
  $file_name = $file->filename;
  $_SESSION['billing_file_name'] = $file_name;


  $party_code_result = check_party_code_exist_or_not(); // check party code present or not.
  $sku_code_result = check_sku7_code_exist_or_not(); // check sku7 code is present or not.
  $bill_result = check_bill_number_exist(); // check bill number is not exist.

  $operations[] = array('change_feeds_import_batch_fix_process', array($file, $party_code_result, $sku_code_result, $bill_result));
  $batch = array(
    'operations' => $operations,
    'title' => t('Reading Billing Data Csv'),
    'finished' => 'billing_data_csv_finished',
    'init_message' => t('Batch is starting...'),
    'error_message' => t('Batch has encountered an error.'),
    'file' => drupal_get_path('module', 'change_feeds_import') . '/form/upload_billing_data.inc',
  );

  batch_set($batch);
}

function billing_data_csv_finished($success, $results, $operations) {
  $file_name = $_SESSION['billing_file_name'];
  $distributor_id = $_SESSION['distributor_id'];
  $date = strtotime(date('Y-m-d H:i:s'));
  $sum_of_total_sales_qty = $results['sum_of_total_sales_qty'];
  $bill_unique_count = $results['bill_unique_count'];
  $bill_uploaded = $bill_unique_count;
  $total_lines = $results['total_lines'];
  $error_link = $results['error_link'];
  $status = $results['status'];
  $pass_data = $results['pass_data'];
  // insert data in error billing data table 
  change_feeds_import_insert_data_in_error_billing_table($file_name, $date, $bill_uploaded, $total_lines, $sum_of_total_sales_qty, $error_link, $status, $distributor_id);
  $last_id = db_query('SELECT MAX(id) FROM {error_billing_data}')->fetchField();
  // insert data in stage billing data table 
  change_feeds_import_insert_data_in_staging_billing_table($pass_data, $last_id, $date);
  drupal_goto("confirm/" . $last_id);
}

function change_feeds_import_batch_fix_process($file, $party_code_result, $sku_code_result, $bill_result, &$context) {
  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['progrss'] = 0;
    $context['sandbox']['handle'] = fopen($file->uri, "r");
    $context['sandbox']['counter'] = 0;
    $context['sandbox']['error'][0] = array('VEHICLE', 'LOADING_DATE', 'PARTY_CODE', 'PARTY_NAME', 'BILL_NUMBER', 'BILL_DATE', 'PRODUCT_NAME', 'SKU7_CODE', 'MRP', 'TUR', 'Sum of TOTAL_SALES_QTY', 'Sum of TOTAL_SALES_RET_QTY', 'Sum of NET_SALES_QTY', 'Sum of SALES_FREE_QTY', 'Sum of SALES_RET_FREE_QTY', 'Sum of SALES_VALUE', 'Sum of SALES_RET_VALUE', 'Sum of NET_SALES_VALUE');
  }
  $handle = $context['sandbox']['handle'];
  $counter = $context['sandbox']['counter'];
  $error = $context['sandbox']['error'];

  if (($handle) !== FALSE) { // take the url of uploaded file
    for ($i = $context['sandbox']['progrss']; $i < $context['sandbox']['progrss'] + 50; $i++) {
      $got_error = 0;
      if (($data = fgetcsv($handle, 0, ",")) !== FALSE) {
        if ($counter == 0) {
          $header_check = array_diff($data, $error[0]);
          if (count($header_check) > 0) {
            form_set_error('file', 'Wrong template');
            $context['finished'] = 1;
            drupal_goto('billing_data/import');
          }
        }
        else {

          $error_bool = 0;
          $vehicle = $data[0];
          $loading_date = $data[1];
          $total_qty = $data[10];
          $sku = $data[7];

          $party_code = $data[2];
          $party_name = $data[3];
          $bill_number = $data[4];
          $bill_date = $data[5];
          $product_name = $data[6];
          $mrp = $data[8];
          $tur = $data[9];
          $sum_of_total_sales_ret_qty = $data[11];
          $sum_of_net_sales_qty = $data[12];
          $sum_of_sales_free_qty = $data[13];
          $sum_of_sales_ret_free_qty = $data[14];
          $sum_of_sales_value = $data[15];
          $sum_of_sales_ret_value = $data[16];
          $sum_of_net_sales_value = $data[17];

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
              $error[$counter][2] = $party_code . " PARTY_CODE is not present in PARTY_CODE Master";
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
              $error[$counter][4] = $bill_number . " BILL_NUMBER Already exist in Billing Data Table";
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
              $error[$counter][7] = $sku . " SKU7_CODE is not present in SKU7_CODE Master";
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
              $error[$counter][8] = $mrp . " MRP is not Numeric";
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
              $error[$counter][9] = $tur . " TUR is not Numeric";
              $error_bool = 1;
            }
            else {
              $error[$counter][9] = $tur;
            }
          }

          if (!is_numeric($total_qty)) {
            $error[$counter][10] = $total_qty . " Sum of TOTAL_SALES_QTY is not Numeric";
            $error_bool = 1;
          }
          else {
            $error[$counter][10] = $total_qty;
            $context['results']['sum_of_total_sales_qty'] += $total_qty;
          }

          if (!is_numeric($sum_of_total_sales_ret_qty)) {
            $error[$counter][11] = $sum_of_total_sales_ret_qty . " Sum of TOTAL_SALES_RET_QTY is not Numeric";
            $error_bool = 1;
          }
          else {
            $error[$counter][11] = $sum_of_total_sales_ret_qty;
          }

          if (!is_numeric($sum_of_net_sales_qty)) {
            $error[$counter][12] = $sum_of_net_sales_qty . " Sum of NET_SALES_QTY is not Numeric";
            $error_bool = 1;
          }
          else {
            $error[$counter][12] = $sum_of_net_sales_qty;
          }

          if (!is_numeric($sum_of_sales_free_qty)) {
            $error[$counter][13] = $sum_of_sales_free_qty . " Sum of SALES_FREE_QTY is not Numeric";
            $error_bool = 1;
          }
          else {
            $error[$counter][13] = $sum_of_sales_free_qty;
          }


          if (!is_numeric($sum_of_sales_ret_free_qty)) {
            $error[$counter][14] = $sum_of_sales_ret_free_qty . " Sum of SALES_RET_FREE_QTY is not Numeric";
            $error_bool = 1;
          }
          else {
            $error[$counter][14] = $sum_of_sales_ret_free_qty;
          }

          if (!is_numeric($sum_of_sales_value)) {
            $error[$counter][15] = $sum_of_sales_value . " Sum of SALES_VALUE is not Numeric";
            $error_bool = 1;
          }
          else {
            $error[$counter][15] = $sum_of_sales_value;
          }

          if (!is_numeric($sum_of_sales_ret_value)) {
            $error[$counter][16] = $sum_of_sales_ret_value . " Sum of SALES_RET_VALUE is not Numeric";
            $error_bool = 1;
          }
          else {
            $error[$counter][16] = $sum_of_sales_ret_value;
          }

          if (!is_numeric($sum_of_net_sales_value)) {
            $error[$counter][17] = $sum_of_net_sales_value . " Sum of NET_SALES_VALUE is not Numeric";
            $error_bool = 1;
          }
          else {
            $error[$counter][17] = $sum_of_net_sales_value;
          }

          if ($error_bool) { // if error occured generate error file
            form_set_error();
            global $base_url;
            $base_path = getcwd();
            convert_array_to_csv_export($error, $base_path . '/sites/default/files/filter_error.csv');
            $got_error = 1;
          }
          else {
            unset($error[$counter]);
          }

          $pass_data[$counter]['error_boolean'] = $error_bool;
          $context['results']['pass_data'][] = $data;

          $bill_number = $data[4];
          if ($counter != 0) {
            if (isset($_SESSION[$bill_number])) {
              // do nothing
            }
            else {
              $res[$bill_number] = array();
              $_SESSION[$bill_number] = $bill_number;
              $context['results']['bill_unique_count'] ++;
            }
          }

          if ($got_error == 1) {
            global $base_url;
            $error_link = $base_url . '/sites/default/files/filter_error.csv';
            $status = "Error";
          }
          else {
            $error_link = "";
            $status = "Confirm";
          }

          $context['results']['error_link'] = $error_link;
          $context['results']['status'] = $status;
          $context['results']['total_lines'] = $counter;
        }
        $counter++;
      }
      else {
        $context['finished'] = 1;
      }
    }
    $context['sandbox']['progrss'] = $i;
    $context['sandbox']['handle'] = $handle;
    $context['sandbox']['counter'] = $counter;
    $context['sandbox']['error'] = $error;
    
  }
  else {
    form_set_error('file', 'File not readable, Please contact site admin');
  }
}

function convert_array_to_csv_export($array, $filename) {
  unlink($filename);
  $fp = fopen($filename, 'w');
  foreach ($array as $fields) {
    fputcsv($fp, $fields);
  }
  fclose($fp);
}

function change_feeds_import_insert_data_in_error_billing_table($file_name, $date, $bill_uploaded, $total_lines, $sum_of_total_sales_qty, $error_link, $status, $distributor_id) {
  $error_billing_data = db_insert('error_billing_data') // Table name no longer needs {}
      ->fields(array(
        'file_name' => $file_name,
        'date' => $date,
        'total_bill_uploaded' => $bill_uploaded,
        'total_line_uploaded' => $total_lines,
        'Sum_of_TOTAL_SALES_QTY' => $sum_of_total_sales_qty,
        'error_file' => $error_link,
        'status' => $status,
        'distributor_id' => $distributor_id,
      ))
      ->execute();
}

function change_feeds_import_insert_data_in_staging_billing_table($pass_data, $last_id, $date) {
  foreach ($pass_data as $data) {
    $stage_billing_data = db_insert('stage_billing_data') // Table name no longer needs {}
        ->fields(array(
          'vehicle' => $data[0],
          'loading_date' => strtotime($data[1]),
          'party_code' => $data[2],
          'party_name' => $data[3],
          'bill_number' => $data[4],
          'bill_date' => strtotime($data[5]),
          'product_name' => $data[6],
          'sku7_code' => $data[7],
          'mrp' => $data[8],
          'tur' => $data[9],
          'Sum_of_TOTAL_SALES_QTY' => $data[10],
          'Sum_of_TOTAL_SALES_RET_QTY' => $data[11],
          'Sum_of_NET_SALES_QTY' => $data[12],
          'Sum_of_SALES_FREE_QTY' => $data[13],
          'Sum_of_SALES_RET_FREE_QTY' => $data[14],
          'Sum_of_SALES_VALUE' => $data[15],
          'Sum_of_SALES_RET_VALUE' => $data[16],
          'Sum_of_NET_SALES_VALUE' => $data[17],
          'error' => $data['error_boolean'],
          'error_id' => $last_id,
          'date' => $date,
        ))
        ->execute();
  }
}

function check_party_code_exist_or_not() {
  $query1 = db_select('node', 'n');
  $query1->condition('n.type', "outlet_master", '=');
  $query1->join('field_data_field_party_code', 'pc', 'pc.entity_id = n.nid');
  $query1->fields('pc', array('field_party_code_value'));
  $query1->fields('n', array('nid'));
  $party_code_result = $query1->execute()->fetchAllKeyed(0, 1);
  return $party_code_result;
}

function check_sku7_code_exist_or_not() {
  $query2 = db_select('field_data_field_sku7_code', 'sc');
  $query2->fields('sc', array('field_sku7_code_value'));
  $sku_code_result = $query2->execute()->fetchAllKeyed(0, 0);
  return $sku_code_result;
}

function check_bill_number_exist() {
  $query3 = db_select('node', 'n');
  $query3->condition('n.type', "billing_data", '=');
  $query3->join('field_data_field_bill_number', 'bn', 'bn.entity_id = n.nid');
  $query3->fields('bn', array('field_bill_number_value'));
  $bill_result = $query3->execute()->fetchAllKeyed(0, 0);
  return $bill_result;
}

function check_distributor_incompleted_upload($distributor_id) {
  $result = db_select('error_billing_data', 'bd')
      ->fields('bd', array('id'))
      ->condition('status', 'Completed', '!=')
      ->condition('status', 'Stopped', '!=')
      ->condition('distributor_id', $distributor_id, '=')
      ->execute()
      ->fetchAssoc();

  $did = $result['id'];
  return $did;
}
