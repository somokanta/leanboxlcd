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

//  $link = l(t('Master Upload Imports'), 'content/master-upload'); // all master upload link.
//  $form['template'] = array(
//    '#markup' => $link,
//  );

  $form['file'] = array(
    '#type' => 'file',
    '#title' => t('Choose a CSV file for billing data upload'),
  );
  $form['template_download'] = array(
    '#type' => 'markup',
    '#prefix' => '<div id="template_wrapper">',
    '#suffix' => '</div>',
    '#markup' => l("Download Template", drupal_get_path('module', 'change_feeds_import') . '/billing_data_template.csv'),
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

  $header_array_from_xsd = get_csv_file_header();

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
      dpm($data,"data"); 
      if (count($data) > 15001) {
        // Count includes header count as well
        $message = 'File Row Count should not be greater than 15000';
        form_set_error('file_upload', t($message));
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

//  $party_code_result = check_party_code_exist_or_not(); // check party code present or not.
//  $sku_code_result = check_sku7_code_exist_or_not(); // check sku7 code is present or not.
//  $bill_result = check_bill_number_exist(); // check bill number is not exist.
//  $master_result = get_node_titles(array('pricelist_group_master', 'product_division_master','sales_register_data'));
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
    $operations[] = array('change_feeds_import_batch_fix_process', array($max, $data, $distributor_id, $file_name, $party_code_result, $sku_code_result, $bill_result, $master_result));
  }
  $batch = array(
    'title' => t('Reading Billing Data Csv'),
    'operations' => $operations,
    'init_message' => t("Batch is starting."),
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
//      if (!is_dir($base_url . '/sites/default/files/billing_data')) {
//        mkdir($base_url . '/sites/default/files/billing_data');
//      }
      $directory = 'public://billing_data';
      file_prepare_directory($directory, FILE_CREATE_DIRECTORY);

      $base_path = getcwd();
      $csv_file = $results['csv_file'];
      $error_link = $base_url . "/sites/default/files/billing_data/$csv_file";
      $status = "Error";
      $header = get_csv_file_header();
      array_unshift($results['error'], $header);
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
    change_feeds_import_update_data_in_error_billing_table_after_confirm($last_id, $bill_uploaded, $total_lines, $sum_of_total_sales_qty, $error_link, $status);

    drupal_goto("confirm/" . $last_id);
  }
  else {
    drupal_set_message('There is Error Occured in batch process');
  }
}

function change_feeds_import_update_data_in_error_billing_table_after_confirm($last_id, $bill_uploaded, $total_lines, $sum_of_total_sales_qty, $error_link, $status) {
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

function change_feeds_import_batch_fix_process($max, $data, $distributor_id, $file_name, $party_code_result, $sku_code_result, $bill_result, $master_result, &$context) {
  if (!isset($context['results']['last_id'])) {
    $context['results']['csv_file'] = $distributor_id . "_" . time() . "_error.csv";

    $date = time();
    $bill_uploaded = NULL;
    $total_lines = NULL;
    $sum_of_total_sales_qty = NULL;
    $error_link = NULL;
    $status = NULL;
    $last_id = change_feeds_import_insert_data_in_error_billing_table($file_name, $date, $bill_uploaded, $total_lines, $sum_of_total_sales_qty, $error_link, $status, $distributor_id);

    $context['results']['last_id'] = $last_id;
    $context['results']['max'] = $max;
    $context['results']['total_lines'] = 0;
    $context['results']['bill_unique_count'] = 0;

    $context['results']['party_code_result'] = check_party_code_exist_or_not(); // check party code present or not.
    $context['results']['sku_code_result'] = check_sku7_code_exist_or_not(); // check sku7 code is present or not.
    $context['results']['bill_result'] = check_bill_number_exist(); // check bill number is not exist.
    $context['results']['master_result'] = get_node_titles(array('pricelist_group_master', 'product_division_master', 'sales_register_data'));
  }

  $last_id = $context['results']['last_id'];
  $csv_file = $context['sandbox']['csv_file'];

  $pass_data = array();

  foreach ($data as $key => &$value) {

    $error_bool = 0;
    $total_qty = $value['Sum of TOTAL_SALES_QTY'];

    $party_code = $value['PARTY_CODE'];
    $bill_number = $value['BILL_NUMBER'];

    $error = array();

    if (!isset($context['results']['unique_combination'])) {
      $context['results']['unique_combination'] = array();
    }

    foreach ($value as $k => $v) {
      $flag = 0;
      $error_text = '';

      $v = trim($v);
      if (empty($v) && $v !== '0') {
        $flag = 1;
        $error_text = "$k can not be null";
      }
      else {

        switch ($k) {
          case 'VEHICLE':
            if ($v == "N.A.") {
              $error_text = $v . " Invalid Vehicle";
              $flag = 1;
            }
            break;

          case 'PARTY_CODE':
            if (!array_key_exists($v, $context['results']['party_code_result'])) {
              $error_text = $v . " PARTY_CODE is not present in PARTY_CODE Master";
              $flag = 1;
            }
            break;

          case 'BEAT_NAME':
            if (!empty($party_code)) {
              if ($context['results']['party_code_result'][$party_code] != $v) {
                $error_text = $v . " BEAT_NAME does not match with OUTLET_MASTER";
                $flag = 1;
              }
            }
            break;
          case 'SKU7_CODE':
            if (!array_key_exists($v, $context['results']['sku_code_result'])) {
              $error_text = $v . " SKU7_CODE is not present in SKU7_CODE Master";
              $flag = 1;
            }

            break;

          case 'BILL_NUMBER':

            $unique_comb = $value['BILL_NUMBER'] . '-' . $value['SKU7_CODE'];

            if (in_array($unique_comb, $context['results']['unique_combination'])) {
              $flag = 1;
              $error_text = "BILL_NUMBER " . $value['BILL_NUMBER'] . " and SKU7_CODE " . $value['SKU7_CODE'] . " is Repeated.  ";
            }
            else {
              $context['results']['unique_combination'][] = $unique_comb;
            }

            if (array_key_exists($v, $context['results']['bill_result'])) {
              $error_text = $v . " BILL_NUMBER Already exist in Billing Data Table";
              $flag = 1;
            }
            if (!isset($context['results']['master_result']['sales_register_data'][$v])) {
              $error_text .= $v . " is not present in SALES_REGISTER Master";
              $flag = 1;
            }

            break;

          case 'PRICELIST_GROUP':
            if (!isset($context['results']['master_result']['pricelist_group_master'][$v])) {
              $error_text = $v . " is not present in PRICELIST_GROUP Master";
              $flag = 1;
            }

            break;

          case 'PRODUCT_DIVISION':
            if (!isset($context['results']['master_result']['product_division_master'][$v])) {
              $error_text = $v . " is not present in PRODUCT_DIVISION Master";
              $flag = 1;
            }

            break;

          case 'BILL_DATE':
          case 'LOADING_DATE':
            if (empty(strtotime($v))) {
              $flag = 1;
              $error_text = "$v Not a valid date";
            }

            break;

          case 'MRP':
          case 'TUR':
            if (!is_numeric($v)) {
              $flag = 1;
              $error_text = "$v in $k Should be numeric";
            }

            break;
          default:
            break;
        }

        if (strstr($k, 'Sum') !== FALSE) {
          if (!is_numeric($v)) {
            $flag = 1;
            $error_text = "$v in $k Should be numeric";
          }
          else {
            if ($k == 'Sum of TOTAL_SALES_QTY') {
              $context['results']['sum_of_total_sales_qty'] += $total_qty;
            }
          }
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

    if ($error_bool) { // if error occured generate error file
      $context['results']['error'][] = $error;
    }

    if (!isset($context['results']['check_bills'][$bill_number])) {
      $context['results']['check_bills'][$bill_number] = $bill_number;
      $context['results']['bill_unique_count'] ++;
    }

    $pass_data = $value;

    change_feeds_import_insert_data_in_staging_billing_table($pass_data, $error_bool, $last_id, REQUEST_TIME);

    $context['results']['total_lines'] ++;
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

  return $error_billing_data;
}

function change_feeds_import_insert_data_in_staging_billing_table($value, $error_boolean, $last_id, $date) {
  $stage_billing_data = db_insert('stage_billing_data') // Table name no longer needs {}
      ->fields(array(
        'vehicle' => $value['VEHICLE'],
        'loading_date' => $value['LOADING_DATE'],
        'party_code' => $value['PARTY_CODE'],
        'BEAT_NAME' => $value['BEAT_NAME'],
        'bill_number' => $value['BILL_NUMBER'],
        'bill_date' => $value['BILL_DATE'],
        'BASEPACK_CODE' => $value['BASEPACK_CODE'],
        'product_name' => $value['PRODUCT_NAME'],
        'sku7_code' => $value['SKU7_CODE'],
        'PRICELIST_GROUP' => $value['PRICELIST_GROUP'],
        'PRODUCT_DIVISION' => $value['PRODUCT_DIVISION'],
        'PRODUCT_CATEGORY' => $value['PRODUCT_CATEGORY'],
        'PRODUCT_SUB_CATEGORY' => $value['PRODUCT_SUB_CATEGORY'],
        'mrp' => isset($value['MRP']) ? $value['MRP'] : NULL,
        'tur' => isset($value['TUR']) ? $value['TUR'] : NULL,
        'Sum_of_TOTAL_SALES_QTY' => isset($value['Sum of TOTAL_SALES_QTY']) ? $value['Sum of TOTAL_SALES_QTY'] : NULL,
        // 'Sum_of_TOTAL_SALES_RET_QTY' => isset($value['Sum of TOTAL_SALES_RET_QTY']) ? $value['Sum of TOTAL_SALES_RET_QTY'] : NULL,
        'Sum_of_NET_SALES_QTY' => isset($value['Sum of NET_SALES_QTY']) ? $value['Sum of NET_SALES_QTY'] : NULL,
        'Sum_of_SALES_FREE_QTY' => isset($value['Sum of SALES_FREE_QTY']) ? $value['Sum of SALES_FREE_QTY'] : NULL,
        //  'Sum_of_SALES_RET_FREE_QTY' => isset($value['Sum of SALES_RET_FREE_QTY']) ? $value['Sum of SALES_RET_FREE_QTY'] : NULL,
        //  'Sum_of_SALES_VALUE' => isset($value['Sum of SALES_VALUE']) ? $value['Sum of SALES_VALUE'] : NULL,
        //  'Sum_of_SALES_RET_VALUE' => isset($value['Sum of SALES_RET_VALUE']) ? $value['Sum of SALES_RET_VALUE'] : NULL,
        'Sum_of_NET_SALES_VALUE' => isset($value['Sum of NET_SALES_VALUE']) ? $value['Sum of NET_SALES_VALUE'] : NULL,
        'Sum_of_NET_SALES_CASES' => isset($value['Sum of NET_SALES_CASES']) ? $value['Sum of NET_SALES_CASES'] : NULL,
        'Sum_of_NET_SALES_KGS' => isset($value['Sum of NET_SALES_KGS']) ? $value['Sum of NET_SALES_KGS'] : NULL,
        'Sum_of_SCHEME_DISCOUNT' => isset($value['Sum of SCHEME_DISCOUNT']) ? $value['Sum of SCHEME_DISCOUNT'] : NULL,
        'Sum_of_OTHER_DISCOUNT' => isset($value['Sum of OTHER_DISCOUNT']) ? $value['Sum of OTHER_DISCOUNT'] : NULL,
        'Sum_of_TAX_PERCENTAGE' => isset($value['Sum of TAX_PERCENTAGE']) ? $value['Sum of TAX_PERCENTAGE'] : NULL,
        'error' => $error_boolean,
        'error_id' => $last_id,
        'date' => $date,
      ))
      ->execute();
}

function check_party_code_exist_or_not() {
//  $query1 = db_select('node', 'n');
//  $query1->condition('n.type', "outlet_master", '=');
//  $query1->join('field_data_field_party_code', 'pc', 'pc.entity_id = n.nid');
//  $query1->fields('pc', array('field_party_code_value'));
//  $query1->fields('n', array('nid'));
//  $party_code_result = $query1->execute()->fetchAllKeyed(0, 1);
//  return $party_code_result;


  $query1 = db_select('field_data_field_party_code', 'pc');
  $query1->join('node', 'n', 'n.nid = pc.entity_id');
  $query1->join('field_data_field_beat_name', 'bn', 'bn.entity_id = pc.entity_id');
  $query1->condition('pc.bundle', "outlet_master");
  $query1->fields('pc', array('field_party_code_value'));
  $query1->fields('bn', array('field_beat_name_value'));
  $query1->addTag('node_uid');
  $party_code_result = $query1->execute()->fetchAllKeyed(0, 1);
  return $party_code_result;
}

function check_sku7_code_exist_or_not() {
  $query2 = db_select('field_data_field_sku7_code', 'sc');
  $query2->join('node', 'n', 'n.nid = sc.entity_id');
  $query2->fields('sc', array('field_sku7_code_value'));
  $query2->condition('sc.bundle', 'sku7_code_master', '=');
  $query2->addTag('node_uid');
  $sku_code_result = $query2->execute()->fetchAllKeyed(0, 0);
  return $sku_code_result;
}

function check_bill_number_exist() {
  global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);
  $query3 = db_select('custom_billing_data', 'cb');
  $query3->condition('cb.distributor_id', $distributor_id);
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

function get_beat_name_from_party_code($party_code) {
  $query1 = db_select('field_data_field_party_code', 'pc');
  $query1->join('field_data_field_beat_name', 'bn', 'bn.entity_id = pc.entity_id');
  $query1->condition('pc.bundle', "outlet_master");
  $query1->condition('pc.field_party_code_value', $party_code);
  $query1->fields('bn', array('field_beat_name_value'));
  $beat_name = $query1->execute()->fetchField();
  return $beat_name;
}

function check_node_title_exists($title, $type, $sales_data = FALSE) {
  $query1 = db_select('node', 'n');
  $query1->condition('n.type', $type);
  $query1->condition('n.title', $title);
  $query1->fields('n', array('nid'));
  $query1->addTag('node_uid');
  if ($sales_data) {
    $date = strtotime(date('Y-m-d'));
    $query1->join('field_data_field_sr_bill_date', 'd', 'd.entity_id = n.nid');
    $query1->condition('d.field_sr_bill_date_value', $date);
  }
  $nid = $query1->execute()->fetchField();
  return $nid;
}

function check_billing_date_empty($title, $type) {
  $query1 = db_select('node', 'n');
  $query1->condition('n.type', $type);
  $query1->condition('n.title', $title);
  $query1->fields('n', array('nid'));
  $query1->addTag('node_uid');
  $query1->join('field_data_field_sr_bill_date', 'd', 'd.entity_id = n.nid');
  $nid = $query1->execute()->fetchField();
  return $nid;
}

function leanbox_check_taxonomy_exists_by_name($tname, $vname) {
  $query1 = db_select('taxonomy_term_data', 't');
  $query1->join('taxonomy_vocabulary', 'v', 'v.vid = t.vid');
  $query1->condition('t.name', $tname);
  $query1->condition('v.name', $vname);
  $query1->fields('t', array('tid'));
  // $query1->addTag('node_uid');
  $tid = $query1->execute()->fetchField();
  return $tid;
}

function get_csv_file_header() {
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
 * 
 * @param type $type
 * @return type
 */
function get_node_titles($type) {
  $query1 = db_select('node', 'n');
  $query1->condition('n.type', $type, 'IN');
  $query1->fields('n', array('type', 'title'));
  $query1->addTag('node_uid');
  $titles = $query1->execute()->fetchAll();
  foreach ($titles as $k => $v) {
    $result[$v->type][$v->title] = $v->title;
  }
  return $result;
}
