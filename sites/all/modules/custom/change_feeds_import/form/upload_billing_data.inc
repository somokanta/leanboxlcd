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

  $header_array_from_xsd = array('VEHICLE', 'LOADING_DATE', 'PARTY_CODE', 'PARTY_NAME', 'BILL_NUMBER', 'BILL_DATE', 'PRODUCT_NAME', 'SKU7_CODE', 'MRP', 'TUR', 'Sum of TOTAL_SALES_QTY', 'Sum of TOTAL_SALES_RET_QTY', 'Sum of NET_SALES_QTY', 'Sum of SALES_FREE_QTY', 'Sum of SALES_RET_FREE_QTY', 'Sum of SALES_VALUE', 'Sum of SALES_RET_VALUE', 'Sum of NET_SALES_VALUE');
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
    }
    else {
      form_set_error('file_upload', t('Failed to write the uploaded file the site\'s file folder.'));
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

  $operations = array();
  $data_chunk = array_chunk($datas, 1000);
  foreach ($data_chunk as $data) {
    $operations[] = array('change_feeds_import_batch_fix_process', array($max, $data, $distributor_id, $file_name, $party_code_result, $sku_code_result, $bill_result));
  }
  $batch = array(
    'title' => t('Reading Billing Data Csv'),
    'operations' => $operations,
    'init_message' => t("Please wait..."),
    'progress_message' => t('Processed @current out of @total.'),
    'error_message' => t('Upload data processing has encountered an error.'),
    'finished' => 'billing_data_csv_finished',
    'file' => drupal_get_path('module', 'change_feeds_import') . '/form/upload_billing_data.inc',
  );
  batch_set($batch);

}

function billing_data_csv_finished($success, $results, $operations) {
  if ($success) {
    $_SESSION['hard'] = $results;

    if (!empty($results['error'])) {
      global $base_url;
      if (!is_dir($base_url . '/sites/default/files/billing_data')) {
        mkdir($base_url . '/sites/default/files/billing_data');
      }

      $base_path = getcwd();
      $csv_file = $results['csv_file'];
      $error_link = $base_url . "/sites/default/files/billing_data/$csv_file";
      $status = "Error";
      $error = $results['error'];
      convert_array_to_csv_export($error, $base_path . "/sites/default/files/billing_data/$csv_file");
      $_SESSION['error_link'] = $error_link;
    }
    else {
      $error_link = "";
      $status = "Confirm";
    }

    $sum_of_total_sales_qty = $results['sum_of_total_sales_qty'];
    $bill_unique_count = $results['bill_unique_count'];
    $bill_uploaded = $bill_unique_count;
    $total_lines = ($results['max'] - 1);
    $last_id = $results['last_id'];
    // insert data in error billing data table 
    change_feeds_import_update_data_in_error_billing_table($last_id, $bill_uploaded, $total_lines, $sum_of_total_sales_qty, $error_link, $status);
    drupal_goto("confirm/" . $last_id);
  }
  else {
    
  }
}

function change_feeds_import_update_data_in_error_billing_table($last_id, $bill_uploaded, $total_lines, $sum_of_total_sales_qty, $error_link, $status) {
  $status_updated = db_update('error_billing_data') // update status in error_billing_data condition based on (Distributor Id )
      ->fields(array(
        'total_bill_uploaded' => $bill_uploaded,
        'total_line_uploaded' => $total_lines,
        'Sum_of_TOTAL_SALES_QTY' => $sum_of_total_sales_qty,
        'error_file' => $error_link,
        'status' => $status,
      ))
      ->condition('id', $last_id, '=')
      ->execute();
}

function change_feeds_import_batch_fix_process($max, $data, $distributor_id, $file_name, $party_code_result, $sku_code_result, $bill_result, &$context) {
  if (!isset($context['results']['last_id'])) {
    $context['sandbox']['counter'] = 0;
    $context['sandbox']['got_error'] = 0;
    $context['results']['csv_file'] = $distributor_id . "_error.csv";

    $date = strtotime(date('Y-m-d H:i:s'));
    $bill_uploaded = NULL;
    $total_lines = NULL;
    $sum_of_total_sales_qty = NULL;
    $error_link = NULL;
    $status = NULL;
    change_feeds_import_insert_data_in_error_billing_table($file_name, $date, $bill_uploaded, $total_lines, $sum_of_total_sales_qty, $error_link, $status, $distributor_id);
    $last_id = db_query('SELECT MAX(id) FROM {error_billing_data}')->fetchField();
    $context['results']['last_id'] = $last_id;
    $context['results']['max'] = $max;
  }
  $last_id = $context['results']['last_id'];
  $got_error = $context['sandbox']['got_error'];
  $csv_file = $context['sandbox']['csv_file'];

  foreach ($data as $key => $value) {

    $error_bool = 0;
    $vehicle = $value['VEHICLE'];
    $loading_date = $value['LOADING_DATE'];
    $total_qty = $value['Sum of TOTAL_SALES_QTY'];
    $sku = $value['SKU7_CODE'];

    $party_code = $value['PARTY_CODE'];
    $party_name = $value['PARTY_NAME'];
    $bill_number = $value['BILL_NUMBER'];
    $bill_date = $value['BILL_DATE'];
    $product_name = $value['PRODUCT_NAME'];
    $mrp = $value['MRP'];
    $tur = $value['TUR'];
    $sum_of_total_sales_ret_qty = $value['Sum of TOTAL_SALES_RET_QTY'];
    $sum_of_net_sales_qty = $value['Sum of NET_SALES_QTY'];
    $sum_of_sales_free_qty = $value['Sum of SALES_FREE_QTY'];
    $sum_of_sales_ret_free_qty = $value['Sum of SALES_RET_FREE_QTY'];
    $sum_of_sales_value = $value['Sum of SALES_VALUE'];
    $sum_of_sales_ret_value = $value['Sum of SALES_RET_VALUE'];
    $sum_of_net_sales_value = $value['Sum of NET_SALES_VALUE'];

    if (empty($vehicle)) {
      $error[0] = "vehicle can not be null";
      $error_bool = 1;
    }
    else {
      $error[0] = $vehicle;
    }

    if (empty($loading_date)) {
      $error[1] = "Loading Date can not be null";
      $error_bool = 1;
    }
    else {
      $error[1] = $loading_date;
    }

    if (empty($party_code)) {
      $error[2] = "PARTY_CODE can not be null";
      $error_bool = 1;
    }
    else {
      if (!array_key_exists($party_code, $party_code_result)) {
        $error[2] = $party_code . " PARTY_CODE is not present in PARTY_CODE Master";
        $error_bool = 1;
      }
      else {
        $error[2] = $party_code;
      }
    }

    if (empty($party_name)) {
      $error[3] = "PARTY_NAME can not be null";
      $error_bool = 1;
    }
    else {
      $error[3] = $party_name;
    }
    if (empty($bill_number)) {
      $error[4] = "BILL_NUMBER can not be null";
      $error_bool = 1;
    }
    else {
      if (array_key_exists($bill_number, $bill_result)) {
        $error[4] = $bill_number . " BILL_NUMBER Already exist in Billing Data Table";
        $error_bool = 1;
      }
      else {
        $error[4] = $bill_number;
      }
    }
    if (empty($bill_date)) {
      $error[5] = "BILL_DATE can not be null";
      $error_bool = 1;
    }
    else {
      $error[5] = $bill_date;
    }

    if (empty($product_name)) {
      $error[6] = "PRODUCT_NAME can not be null";
      $error_bool = 1;
    }
    else {
      $error[6] = $product_name;
    }
    if (empty($sku)) {
      $error[7] = "SKU7_CODE can not be null";
      $error_bool = 1;
    }
    else {
      if (!array_key_exists($sku, $sku_code_result)) {
        $error[7] = $sku . " SKU7_CODE is not present in SKU7_CODE Master";
        $error_bool = 1;
      }
      else {
        $error[7] = $sku;
      }
    }

    if (!is_numeric($mrp)) {
      $error[8] = $mrp . " MRP is not Numeric";
      $error_bool = 1;
    }
    else {
      $error[8] = $mrp;
    }


    if (!is_numeric($tur)) {
      $error[9] = $tur . " TUR is not Numeric";
      $error_bool = 1;
    }
    else {
      $error[9] = $tur;
    }


    if (!is_numeric($total_qty)) {
      $error[10] = $total_qty . " Sum of TOTAL_SALES_QTY is not Numeric";
      $error_bool = 1;
    }
    else {
      $error[10] = $total_qty;
      $context['results']['sum_of_total_sales_qty'] += $total_qty;
    }

    if (!is_numeric($sum_of_total_sales_ret_qty)) {
      $error[11] = $sum_of_total_sales_ret_qty . " Sum of TOTAL_SALES_RET_QTY is not Numeric";
      $error_bool = 1;
    }
    else {
      $error[11] = $sum_of_total_sales_ret_qty;
    }

    if (!is_numeric($sum_of_net_sales_qty)) {
      $error[12] = $sum_of_net_sales_qty . " Sum of NET_SALES_QTY is not Numeric";
      $error_bool = 1;
    }
    else {
      $error[12] = $sum_of_net_sales_qty;
    }

    if (!is_numeric($sum_of_sales_free_qty)) {
      $error[13] = $sum_of_sales_free_qty . " Sum of SALES_FREE_QTY is not Numeric";
      $error_bool = 1;
    }
    else {
      $error[13] = $sum_of_sales_free_qty;
    }


    if (!is_numeric($sum_of_sales_ret_free_qty)) {
      $error[14] = $sum_of_sales_ret_free_qty . " Sum of SALES_RET_FREE_QTY is not Numeric";
      $error_bool = 1;
    }
    else {
      $error[14] = $sum_of_sales_ret_free_qty;
    }

    if (!is_numeric($sum_of_sales_value)) {
      $error[15] = $sum_of_sales_value . " Sum of SALES_VALUE is not Numeric";
      $error_bool = 1;
    }
    else {
      $error[15] = $sum_of_sales_value;
    }

    if (!is_numeric($sum_of_sales_ret_value)) {
      $error[16] = $sum_of_sales_ret_value . " Sum of SALES_RET_VALUE is not Numeric";
      $error_bool = 1;
    }
    else {
      $error[16] = $sum_of_sales_ret_value;
    }

    if (!is_numeric($sum_of_net_sales_value)) {
      $error[17] = $sum_of_net_sales_value . " Sum of NET_SALES_VALUE is not Numeric";
      $error_bool = 1;
    }
    else {
      $error[17] = $sum_of_net_sales_value;
    }

    if ($error_bool) { // if error occured generate error file
      $context['results']['error'][] = $error;
      $got_error = 1;
    }
    else {
      unset($error);
    }

    $error_boolean = $error_bool;
    $pass_data = $value;
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



    $context['results']['error_link'] = $error_link;
    $context['results']['status'] = $status;
    $context['sandbox']['got_error'] = $got_error;
    change_feeds_import_insert_data_in_staging_billing_table($pass_data, $error_boolean, $last_id, $date);

    $counter++;
    $context['results']['total_lines'] = $counter;
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

function change_feeds_import_insert_data_in_staging_billing_table($value, $error_boolean, $last_id, $date) {
  $stage_billing_data = db_insert('stage_billing_data') // Table name no longer needs {}
      ->fields(array(
        'vehicle' => $value['VEHICLE'],
        'loading_date' => strtotime($value['LOADING_DATE']) ? strtotime($value['LOADING_DATE']) : NULL,
        'party_code' => $value['PARTY_CODE'],
        'party_name' => $value['PARTY_NAME'],
        'bill_number' => $value['BILL_NUMBER'],
        'bill_date' => strtotime($value['BILL_DATE']) ? strtotime($value['BILL_DATE']) : NULL,
        'product_name' => $value['PRODUCT_NAME'],
        'sku7_code' => $value['SKU7_CODE'],
        'mrp' => $value['MRP'] ? $value['MRP'] : NULL,
        'tur' => $value['TUR'] ? $value['TUR'] : NULL,
        'Sum_of_TOTAL_SALES_QTY' => isset($value['Sum of TOTAL_SALES_QTY']) ? $value['Sum of TOTAL_SALES_QTY'] : NULL,
        'Sum_of_TOTAL_SALES_RET_QTY' => isset($value['Sum of TOTAL_SALES_RET_QTY']) ? $value['Sum of TOTAL_SALES_RET_QTY'] : NULL,
        'Sum_of_NET_SALES_QTY' => isset($value['Sum of NET_SALES_QTY']) ? $value['Sum of NET_SALES_QTY'] : NULL,
        'Sum_of_SALES_FREE_QTY' => isset($value['Sum of SALES_FREE_QTY']) ? $value['Sum of SALES_FREE_QTY'] : NULL,
        'Sum_of_SALES_RET_FREE_QTY' => isset($value['Sum of SALES_RET_FREE_QTY']) ? $value['Sum of SALES_RET_FREE_QTY'] : NULL,
        'Sum_of_SALES_VALUE' => isset($value['Sum of SALES_VALUE']) ? $value['Sum of SALES_VALUE'] : NULL,
        'Sum_of_SALES_RET_VALUE' => isset($value['Sum of SALES_RET_VALUE']) ? $value['Sum of SALES_RET_VALUE'] : NULL,
        'Sum_of_NET_SALES_VALUE' => isset($value['Sum of NET_SALES_VALUE']) ? $value['Sum of NET_SALES_VALUE'] : NULL,
        'error' => $error_boolean,
        'error_id' => $last_id,
        'date' => $date,
      ))
      ->execute();
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
  $query2->condition('sc.bundle', 'sku7_code_master', '=');
  $sku_code_result = $query2->execute()->fetchAllKeyed(0, 0);
  return $sku_code_result;
}

function check_bill_number_exist() {
  $query3 = db_select('custom_billing_data', 'cb');
  $query3->fields('cb', array('bill_number'));
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
