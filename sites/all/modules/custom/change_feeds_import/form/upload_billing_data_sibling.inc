<?php

// upload form for billing data
function change_feeds_import_billing_data_sibling_form($form, $form_state) {
    global $user;
    $user_id = $user->uid;
    $sibling_central_distributor_id = change_feeds_import_get_distributor_id($user_id);
    if (in_array('sibling_central_distributor', $user->roles)) {
        $did = check_distributor_incompleted_upload_sibling($sibling_central_distributor_id);
        if (!empty($did)) {
            drupal_goto("confirm/$did");
        }
        $template = 'billing_data_template_central.csv';
        $form['file'] = array(
          '#type' => 'file',
          '#title' => t('Choose a CSV file for billing data upload'),
        );
        $form['template_download'] = array(
          '#type' => 'markup',
          '#prefix' => '<div id="template_wrapper">',
          '#suffix' => '</div>',
          '#markup' => l("Download Template", drupal_get_path('module', 'change_feeds_import') . '/' . $template),
        );
        $form['submit'] = array(
          '#type' => 'submit',
          '#value' => t('Upload'),
        );
    }
    else {
          drupal_goto("billing_data/import");
        
    }

    return $form;
}

function change_feeds_import_billing_data_sibling_form_validate($form, &$form_state) {
    global $user;
    $sibling_central_distributor_id = change_feeds_import_get_distributor_id($user->uid);
    $file = file_save_upload('file', array(
      'file_validate_extensions' => array('csv'),
    ));

    $header_array_from_xsd = get_csv_file_header_sibling();
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
                $mess = '';
                $sibling_child_distributor_ids = get_sibling_child_distributor_ids($sibling_central_distributor_id);
                foreach ($data as $key => $value) {
                    $bill_no = trim($value[4]);
                    $sku = $value[7];
                    $bills_array[$bill_no] = $bill_no;
                    $dist_name = strtolower(trim($value[24]));
                    $distributor_names_array[$dist_name] = $dist_name;
                }
                $bill_distributor_ids = get_distributor_id_of_bills($bills_array, $sibling_child_distributor_ids);
                $distributor_names = validate_distributor_names($distributor_names_array);
                foreach ($data as $key => $value) {
                    $bill_no = trim($value[4]);
                    $sku = $value[7];
                    // passing distributor_name
                    $distributor_name = strtolower(trim($value[24]));
                    //$sibling_distributor_id = validate_distributor_name($distributor_name);
                    if (empty($distributor_names[$distributor_name])) {
                        $mess .= t('Invalid Distributor Name: ' . $distributor_name) . '<br>';
                    }
                    else {
                        $unique_comb = $bill_no . $sku;
                        if (array_key_exists($unique_comb, $res['unique_combination'])) {
                            form_set_error('duplicate_data', t('Duplicate Data Found , Please Remove Duplicate Data and Upload Again.'));
                        }
                        $res['unique_combination'][$unique_comb] = $unique_comb;
                        $bill_distributor_id = $bill_distributor_ids[$bill_no];
                        if (empty($bill_distributor_id)) {
                            $mess .= t('Invalid Bill Number: ' . $bill_no) . '<br>';
                        }
                        else {
                            $form_state['distributor_data'][$bill_no] = $bill_distributor_id;
                           // $form_state['central_distributor_data'][$bill_no] = get_central_distributor_id($bill_distributor_id);
                           $form_state['sibling_distributor_ids'][$bill_distributor_id] = $bill_distributor_id;
                        }
                        
                    }
                }
                if (!empty($mess)) {
                    form_set_error('form', $mess);
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

function change_feeds_import_billing_data_sibling_form_submit($form, &$form_state) {
    global $user;
    $user_id = $user->uid;
    $sibling_central_distributor_id = change_feeds_import_get_distributor_id($user_id);
    $file = $form_state['values']['file'];
    $file_name = $file->filename;
    $bill_distributor_ids = $form_state['sibling_distributor_ids'];
    // $form_state['distributor_data']
    // $form_state['central_distributor_data']

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
    $unique_bills = array_unique(array_map(function($elem) {
          return $elem['BILL_NUMBER'];
      }, $datas));
    $unique_skus = array_unique(array_map(function($elem) {
          return $elem['SKU7_CODE'];
      }, $datas));
    $unique_party_code = array_unique(array_map(function($elem) {
          return $elem['PARTY_CODE'];
      }, $datas));
    $party_code_result = sibling_check_party_code_exist_or_not($bill_distributor_ids, $unique_party_code);
    // check sku7 code is present or not.
    $sku_code_result = sibling_check_sku7_code_exist_or_not($unique_skus);
    // check bill number is not exist.
    $bill_result = sibling_check_distributor_bill_number_exist($bill_distributor_ids, $unique_bills);
    $operations = array();
    $data_chunk = array_chunk($datas, 1000);
    foreach ($data_chunk as $data) {
        $operations[] = array('change_feeds_import_batch_fix_process_sibling', array($max, $data, $sibling_central_distributor_id, $file_name, $party_code_result, $sku_code_result, $bill_result, $master_result, $form_state['distributor_data']));
    }
    $batch = array(
      'title' => t('Reading Billing Data Csv'),
      'operations' => $operations,
      'init_message' => t("Batch is starting."),
      'progress_message' => t('Processed @current out of @total.'),
      'error_message' => t('Upload data processing has encountered an error.'),
      'finished' => 'billing_data_csv_finished_sibling',
      'file' => drupal_get_path('module', 'change_feeds_import') . '/form/upload_billing_data_sibling.inc',
    );

    batch_set($batch);
}

function billing_data_csv_finished_sibling($success, $results, $operations) {

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
            $header = get_csv_file_header_sibling();
            array_unshift($results['error'], $header);
            $error = $results['error'];
            convert_array_to_csv_export_sibling($error, $base_path . "/sites/default/files/billing_data/$csv_file");
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
        change_feeds_import_update_data_in_error_billing_table_after_confirm_sibling($last_id, $bill_uploaded, $total_lines, $sum_of_total_sales_qty, $error_link, $status);

        //drupal_goto("confirm/" . $last_id);
        $_GET['destination'] = "confirm/" . $last_id;
    }
    else {
        drupal_set_message('There is Error Occured in batch process');
    }
}

function change_feeds_import_update_data_in_error_billing_table_after_confirm_sibling($last_id, $bill_uploaded, $total_lines, $sum_of_total_sales_qty, $error_link, $status) {
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

// fetch sales register bill date
function sibling_get_sales_register_bill_date($distributor_ids) {
    $sr_bill_dates = array();
    $time = strtotime('-30 days');  // last 1 month
    if (!empty($distributor_ids)) {
        $query = db_select('node', 'n');
        $query->leftjoin('field_data_field_sr_new_bill_date', 'sr_bill', 'sr_bill.entity_id = n.nid');
        $query->condition('n.uid', $distributor_ids, 'IN');
        $query->condition('n.type', 'sales_register_data');
        $query->addExpression("CONCAT(n.title,n.uid)", 'bill_number_key');
        $query->addExpression('sr_bill.field_sr_new_bill_date_value', 'field_sr_new_bill_date_value');
        //$query->fields('n', array('title'));
        $query->condition('n.created', $time, '>');
        $sr_bill_dates = $query->execute()->fetchAllKeyed(0, 1);
    }
    return $sr_bill_dates;
}

function change_feeds_import_batch_fix_process_sibling($max, $data, $distributor_id, $file_name, $party_code_result, $sku_code_result, $bill_result, $master_result, $csv_distributor_ids,&$context) {
    if (!isset($context['results']['last_id'])) {
        $context['results']['csv_file'] = $distributor_id . "_" . time() . "_error.csv";
        $date = time();
        $bill_uploaded = NULL;
        $total_lines = NULL;
        $sum_of_total_sales_qty = NULL;
        $error_link = NULL;
        $status = NULL;
        $last_id = change_feeds_import_insert_data_in_error_billing_table_sibling($file_name, $date, $bill_uploaded, $total_lines, $sum_of_total_sales_qty, $error_link, $status, $distributor_id);
        $distributor_ids = $csv_distributor_ids;
        $context['results']['last_id'] = $last_id;
        $context['results']['max'] = $max;
        $context['results']['total_lines'] = 0;
        $context['results']['bill_unique_count'] = 0;
        $context['results']['party_code_result'] = $party_code_result; // check party code present or not.
        $context['results']['sku_code_result'] = $sku_code_result; // check sku7 code is present or not.
        $context['results']['bill_result'] = $bill_result; // check bill number is not exist.
        $context['results']['master_result'] = sibling_get_node_titles(array('pricelist_group_master', 'product_division_master'), $distributor_ids);
        $context['results']['sr_bill_dates'] = sibling_get_sales_register_bill_date($distributor_ids);
      }

    $last_id = $context['results']['last_id'];
    $csv_file = $context['sandbox']['csv_file'];
    $created_by_distributor_id = $distributor_id;
    $pass_data = array();

    foreach ($data as $key => &$value) {
        $billnumber = trim($value['BILL_NUMBER']);
        $distributor_id = $csv_distributor_ids[$billnumber];
        $error_bool = 0;
        $total_qty = $value['Sum of TOTAL_SALES_QTY'];
        $net_sales_qty = $value['Sum of NET_SALES_QTY'];
        $sales_free_qty = $value['Sum of SALES_FREE_QTY'];

        $party_code = $value['PARTY_CODE'];
        $bill_number = trim($value['BILL_NUMBER']);

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
                        if (!array_key_exists($v, $context['results']['party_code_result'][$distributor_id])) {
                            $error_text = $v . " PARTY_CODE is not present in PARTY_CODE Master";
                            $flag = 1;
                        }
                        $hul_code = get_blocked_hul_code_from_party_code($v, $distributor_id);
                        if ($hul_code) {
                            $error_text = " Party code " . $v . " consist of HUL Code ".$hul_code['hul_code']." is blocked due to ".$hul_code['block_reason'];
                            $flag = 1;
                        }
                        break;

                    case 'BEAT_NAME':
                        if (!empty($party_code)) {
                            if ($context['results']['party_code_result'][$distributor_id][$party_code] != $v) {
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

                        $unique_comb = $value['BILL_NUMBER'] . '-' . $value['SKU7_CODE'] ;
                        if (in_array($unique_comb, $context['results']['unique_combination'])) {
                            $flag = 1;
                            $error_text = "BILL_NUMBER " . $value['BILL_NUMBER'] . " and SKU7_CODE " . $value['SKU7_CODE'] . " is Repeated.  ";
                        }
                        else {
                            $context['results']['unique_combination'][] = $unique_comb;
                        }

                        if (array_key_exists($v, $context['results']['bill_result'][$distributor_id])) {
                            $error_text = $v . " BILL_NUMBER Already exist in Billing Data Table";
                            $flag = 1;
                        }
                        if (!isset($context['results']['master_result'][$distributor_id]['sales_register_data'][$v])) {
                            $error_text .= $v . " is not present in SALES_REGISTER Master";
                            $flag = 1;
                        }

                        break;

                    case 'PRICELIST_GROUP':
                        $v = strtolower($v);
                        if (!isset($context['results']['master_result']['pricelist_group_master'][$v])) {
                            $error_text = $v . " is not present in PRICELIST_GROUP Master";
                            $flag = 1;
                        }

                        break;

                    case 'PRODUCT_DIVISION':
                        $v = strtolower($v);
                        if (!isset($context['results']['master_result']['product_division_master'][$v])) {
                            $error_text = $v . " is not present in PRODUCT_DIVISION Master";
                            $flag = 1;
                        }

                        break;

                    case 'BILL_DATE':
                    case 'LOADING_DATE':
                        /* if (empty(strtotime($v))) {
                          $flag = 1;
                          $error_text = "$v Not a valid date";
                          } */
                        $is_not_valid_date = validate_input_date(trim($v));
                        if ($is_not_valid_date === 1) {
                            $flag = 1;
                            $error_text = "$v Not a valid date format.Acceptable formats are e.g. 2-4-18 or 2/4/18 or 2-Apr-18 or 2/Apr/18 or 2-4-2018 or 2/4/2018 .";
                        }
                        else {
                            $sr_new_bill_date = strtotime($is_not_valid_date);
                        }
                        if (!empty($sr_new_bill_date)) {
                            //$context['results']['sr_bill_dates'][$bill_number]);one month bill _number dates
                            //b123<br>1233333 [billnumber<br>billdate
                            $bill_date_key = trim($bill_number) . trim($distributor_id);
                            //print_r(array($bill_date_key));
                            if ($sr_new_bill_date != $context['results']['sr_bill_dates'][$bill_date_key]) {
                                $flag = 1;
                                $error_text = "Loading/Billing Date - $v : Does not match with Sales register Billing Date.";
                            }
                            else {
                                // strtotitme value
                                $value[$k] = date('d-m-Y', $sr_new_bill_date);
                            }
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

                            if ($total_qty != $net_sales_qty + $sales_free_qty) {
                                $flag = 1;
                                $error_text = "$total_qty in $k should be Sum of NET_SALES_QTY + Sum of SALES_FREE_QTY";
                            }
                            if ($total_qty < 0) {
                                $flag = 1;
                                $error_text = "$total_qty in $k cannot be negative";
                            }
                            if (floatval($total_qty) == 0) {
                                $flag = 1;
                                $error_text = "$total_qty in $k cannot be zero";
                            }
                            $context['results']['sum_of_total_sales_qty'] += $total_qty;
                        }
                        if ($k == 'Sum of NET_SALES_QTY' && $net_sales_qty < 0) {
                            $flag = 1;
                            $error_text = "$net_sales_qty in $k cannot be negative";
                        }
                        if ($k == 'Sum of SALES_FREE_QTY' && $sales_free_qty < 0) {
                            $flag = 1;
                            $error_text = "$sales_free_qty in $k cannot be negative";
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

        if (!isset($context['results']['check_bills'][$distributor_id][$bill_number])) {
            $context['results']['check_bills'][$distributor_id][$bill_number] = $bill_number;
            $context['results']['bill_unique_count']++;
        }

        $pass_data = $value;

        try {
            change_feeds_import_insert_data_in_staging_billing_table_sibling($pass_data, $error_bool, $last_id, REQUEST_TIME, $distributor_id, $created_by_distributor_id);
        }
        catch (Exception $ex) {
            
        }

        $context['results']['total_lines']++;
    }
}

function convert_array_to_csv_export_sibling($array, $filename) {
    unlink($filename);
    $fp = fopen($filename, 'w');
    foreach ($array as $fields) {
        fputcsv($fp, $fields);
    }
    fclose($fp);
}

function change_feeds_import_insert_data_in_error_billing_table_sibling($file_name, $date, $bill_uploaded, $total_lines, $sum_of_total_sales_qty, $error_link, $status, $distributor_id) {
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
        'created_by_distributor_id' => $distributor_id,
      ))
      ->execute();

    return $error_billing_data;
}

function change_feeds_import_insert_data_in_staging_billing_table_sibling($value, $error_boolean, $last_id, $date, $distributor_id, $created_by_distributor_id) {
    if (empty($last_id)) {
        global $user;
        $created_by_distributor_id = change_feeds_import_get_distributor_id($user->uid);
        $st_query = db_select('error_billing_data', 'err');
        $st_query->condition('err.created_by_distributor_id', $created_by_distributor_id);
        $st_query->addExpression('Max(id)', 'max');
        $last_id = $st_query->execute()->fetchField();
    }

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
        'distributor_id' => $distributor_id,
      ))
      ->execute();
}

function sibling_check_party_code_exist_or_not($distributor_ids, $csv_party_code) {
    $party_code_result = array();
    if (!empty($distributor_ids)) {
        $query1 = db_select('field_data_field_party_code', 'pc');
        $query1->join('node', 'n', 'n.nid = pc.entity_id');
        $query1->join('field_data_field_beat_name', 'bn', 'bn.entity_id = pc.entity_id');
        $query1->condition('pc.bundle', "outlet_master");
        $query1->fields('pc', array('field_party_code_value'));
        $query1->fields('bn', array('field_beat_name_value'));
        $query1->fields('n', array('uid'));
        //$query1->addTag('node_uid');
        $query1->condition('uid', $distributor_ids, 'IN');
        $query1->condition('pc.field_party_code_value', $csv_party_code, 'IN');
        //$party_code_result = $query1->execute()->fetchAllKeyed(0, 1);
        $result = $query1->execute()->fetchAll(PDO::FETCH_ASSOC);
        if (!empty($result)) {
            foreach ($result as $key => $value) {
                $party_code = ($value['field_party_code_value']);
                $party_code_result[$value['uid']][$party_code] = ($value['field_beat_name_value']);
            }
        }
    }
    return $party_code_result;
}

function sibling_check_sku7_code_exist_or_not($csv_skus) {
    $csv_skus1 = array_map('trim', $csv_skus);
    $query2 = db_select('field_data_field_sku7_code', 'sc');
    $query2->join('node', 'n', 'n.nid = sc.entity_id');
    $query2->fields('sc', array('field_sku7_code_value'));
    $query2->condition('n.title', $csv_skus1, 'IN');
    $query2->condition('sc.bundle', 'sku7_code_master', '=');
    $sku_code_result = $query2->execute()->fetchAllKeyed(0, 0);
    return $sku_code_result;
}

function check_distributor_incompleted_upload_sibling($distributor_id) {
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

function get_csv_file_header_sibling() {
    global $user;
    $header = array(
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
      'Distributor Name'
    );
     return $header;
}
function sibling_get_node_titles($types, $distributor_ids = array()) {
    if (!empty($distributor_ids)) {
        $time = strtotime('-30 days');  // last 1 month
        $type = array('sales_register_data');
        $query1 = db_select('node', 'n');
        $query1->condition('n.type', $type, '=');
        $query1->condition('n.created', $time, '>');
        $query1->fields('n', array('type', 'title', 'uid'));
        //$query1->addTag('node_uid');
        $query1->condition('n.uid', $distributor_ids, 'IN');
        $titles = $query1->execute()->fetchAll();
        foreach ($titles as $k => $v) {
            $result[$v->uid][$v->type][$v->title] = $v->title;
        }
        $query1 = db_select('node', 'n');
        $query1->condition('n.type', $types, 'IN');
        //$query1->condition('n.created', $time, '>');
        $query1->fields('n', array('type', 'title'));
        $titles = $query1->execute()->fetchAll();
        foreach ($titles as $k => $v) {
            $node_title = strtolower($v->title);
            $result[$v->type][$node_title] = $node_title;
        }
    }
    return $result;
}
//This returns bill numbers of distributor
//@$csv_bill_numbers = bill numbers present in the csv file
function sibling_check_distributor_bill_number_exist($distributor_ids, $csv_bill_numbers) {
    $bill_result = array();
    if (!empty($distributor_ids) && !empty($csv_bill_numbers)) {
        $query = db_select('custom_billing_data', 'cb');
        $query->condition('cb.distributor_id', $distributor_ids, 'IN');
        $query->condition('cb.bill_number', $csv_bill_numbers, 'IN');
        $query->fields('cb', array('bill_number', 'distributor_id'))->distinct();
        $res = $query->execute()->fetchAll();
        foreach ($res as $k => $v) {
            $bill_result[$v->distributor_id][$v->bill_number] = $v->bill_number;
        }
    }
    return $bill_result;
}
