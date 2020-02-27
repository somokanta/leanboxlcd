<?php
// upload form for billing data
module_load_include('inc', 'leanbox_address_confirm', 'includes/address_confirm');
function change_feeds_import_sales_register_sibling_form($form, $form_state) {
    global $user;
    $user_id = $user->uid;
    // This is sibling central distributor ID only sibling central can upload sales register 
    $sibling_central_distributor_id = change_feeds_import_get_distributor_id($user_id);
    //add distributor id columns for central distributor
    //add distributor id columns for central distributor
    if (in_array('sibling_central_distributor', $user->roles)) {
        $template = 'sales_register_template_central.csv';
        $form_state['central_distributor_id'] = $distributor_id;

        // check distributor incompleted upload
        $query = db_select('custom_sales_data', 'sd');
        $query->condition('sd.created_by_distributor_id', $sibling_central_distributor_id, '=');
        $query->fields('sd', array('fid'));
        $fid = $query->execute()->fetchField();

        if (!empty($fid)) {
            drupal_goto("sales-confirm/$fid");
        }

        $form['file'] = array(
          '#type' => 'file',
          '#title' => t('Choose a CSV file for sales data upload'),
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
        drupal_goto("sales-register/import");
    }
    return $form;
}


function change_feeds_import_sales_register_sibling_form_validate($form, &$form_state) {
    global $user;
    $file = file_save_upload('file', array(
      'file_validate_extensions' => array('csv'),
    ));
    $header_array_from_xsd = array(
      'BILL_NUMBER',
      'BILL_DATE',
      'PARTY_CODE',
      'BILL_VALUE',
      'DISTRIBUTOR NAME',
    );

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
            }
            else { // Full else part is added for TP to Host qty feature
                foreach ($data as $val) {
                    $all_bills[$val[0]] = $val[0]; //Bill Number is at zero position(means first)
                }
//        $existing_bills = check_bill_exists_in_custom_billing($all_bills);
//        if(!empty($existing_bills)){
//          form_set_error('file_upload', "Custom billing data has been already uploaded for bills: <BR>".$existing_bills);
//        }
            }
        }
        else {
            form_set_error('file_upload', t('Failed to write the uploaded file the site\'s file folder.'));
        }
        $mess = '';
        foreach ($data as $key => $value) {
                $distributor_name = strtolower(trim($value[4]));
                $sibling_distributor_ids = validate_distributor_name($distributor_name);
                if (empty($sibling_distributor_ids)) {
                    $mess .= t('Invalid Distributor Name: ' . $distributor_name) . '<br>';
                }
                // valid Distributor Name
                else {
                    $party_code = trim($value[2]);
                    // distributor id of party code
                    $party_code_count = check_same_party_code_exist_for_sibling($party_code, $sibling_distributor_ids);
                    if ($party_code_count>1) {
                       $mess .= t('Party Code : ' . $party_code. ' exist for multiple Sibling Distributor Location.') . '<br>';  
                    }
                    $distributor_id = get_distributor_id_of_party_code($party_code, $sibling_distributor_ids);
                   if (empty($distributor_id)) {
                        $mess .= t('Invalid Party Code : ' . $party_code) . '<br>';
                    }
                    else {
                        // this for array identification of party-code-distributor name
                        // partycode is unique in sibling distributor level.
                        $party_code_distributor_id_key = $party_code . $distributor_name;
                    }
                    $form_state['distributor_data'][$party_code_distributor_id_key] = $distributor_id;
                    $form_state['central_distributor_data'][$party_code_distributor_id_key] = get_central_distributor_id($distributor_id);
                }
            
        }
        if (!empty($mess)) {
            form_set_error('form', $mess);
        }
        else {
            //print_r($form_state['distributor_data']);die;
        }
    }
    else {
        form_set_error('file', t('No file was uploaded.'));
    }
}

function get_number_of_days_of_sales_register($central_distributor_id) {
    if (!empty($central_distributor_id)) {
        $query = db_select('node', 'n');
        $query->condition('n.type', 'van_stop_master');
        $query->condition('n.title', $central_distributor_id);
        $query->join('field_data_field_no_of_days_for_sales_reg', 'no_da', 'no_da.entity_id = n.nid');
        $query->fields('no_da', array('field_no_of_days_for_sales_reg_value'));
        $query->condition('n.uid',$central_distributor_id);
        $no_of_days = $query->execute()->fetchField();
    }
    return $no_of_days;
}

function change_feeds_import_sales_register_sibling_form_submit($form, &$form_state) {
    global $user;
    $user_id = $user->uid;
    $sibling_distributor_id = change_feeds_import_get_distributor_id($user_id);
    $file = $form_state['values']['file'];
    $file_name = $file->filename;
    $fid = $file->fid;
    $filepath = $file->uri;

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
        $operations[] = array('change_feeds_import_sales_register_sibling_batch_fix_process', array($data, $sibling_distributor_id, $file_name, $fid, $no_of_days, $form_state['distributor_data'],$form_state['central_distributor_data']));
    }
    $batch = array(
      'title' => t('Reading sales Data Csv'),
      'operations' => $operations,
      'init_message' => t("Please wait..."),
      'progress_message' => t('Processed @current out of @total.'),
      'error_message' => t('Upload data processing has encountered an error.'),
      'finished' => 'sales_register_sibling_data_csv_finished',
      'file' => drupal_get_path('module', 'change_feeds_import') . '/form/sales_register_data_sibling.inc',
    );
    batch_set($batch);
}

function change_feeds_import_sales_register_sibling_batch_fix_process($data, $distributor_id, $file_name, $fid, $no_of_days, $csv_distributor_ids,$central_distributor_ids, &$context) {
    global $user;
    require_once drupal_get_path('module', 'change_feeds_import') . '/form/upload_billing_data.inc';
    $value['error_message'] = '';
    if (!isset($context['results']['unique_combination'])) {
        $context['results']['unique_combination'] = array();
    }
    foreach ($data as $key => $value) {
            if (empty($value['DISTRIBUTOR NAME'])) {
                $value['error_message'] .= "Distributor Name Column Missing for bill number " . $value['BILL_NUMBER'] . ".";
            }
            else {
                //$party_code_distributor_id_key = $party_code . $distributor_name;
                $party_code = $value['PARTY_CODE'];
                $distributor_name = strtolower($value['DISTRIBUTOR NAME']);
                $party_code_distributor_id_key = $party_code . $distributor_name;
                $csv_dist_id = $csv_distributor_ids[$party_code_distributor_id_key];
                if (empty($csv_dist_id)) {
                    $value['error_message'] .= "Invalid Distributor Name:" . $value['DISTRIBUTOR NAME'];
                }
            }
        

        $dis_id = $csv_dist_id ;
        $sibling_distributor_ids = get_sibling_distributor_id($dis_id);
        foreach ($value as $k => $v) {
            $error_text = '';
            $v = trim($v);
            if ($k == 'BILL_VALUE') {
                if ((empty($v) && ($v != '0') ) || $v < '0') {
                    $value['error_message'] .= $k . " can not be empty or  less than 0(Zero) for bill no " . $value['BILL_NUMBER'] . ".";
                }
            }
            else if ($k != 'Crate_ID') {
                if (empty($v)) {
                    $value['error_message'] .= $k . " can not be empty for bill no " . $value['BILL_NUMBER'] . ".";
                }
            }
        }

        if (isset($value['BILL_DATE'])) {
            // $value['BILL_DATE'] = str_replace('/', '-', $value['BILL_DATE']);
            // $date_arr = explode('-', $value['BILL_DATE']);
            // $converted_date = $date_arr[2] . '-' . $date_arr[1] . '-' . $date_arr[0];
            $is_not_valid_date = validate_input_date(trim($value['BILL_DATE']));
            if ($is_not_valid_date === 1) {
                $value['error_message'] .= " Please Enter  Bill Date for Bill No:" . $value['BILL_NUMBER'] . " in corect format. 
                                             Acceptable formats are e.g. 2-4-18 or 2/4/18 or 2-Apr-18 or 2/Apr/18 or 2-4-2018 or 2/4/2018 .";
            }
            else {
                $value['BILL_DATE'] = date('d-m-Y', strtotime($is_not_valid_date));
            }
            $value['BILL_DATE'] = strtotime($value['BILL_DATE']);
            $no_of_days = get_number_of_days_of_sales_register($central_distributor_ids[$party_code_distributor_id_key]);
            if (!empty($value['BILL_DATE'])) {
                $check_date = $value['BILL_DATE'];
                $from = strtotime(date('Y-m-d', strtotime("-$no_of_days days")));
                $to = strtotime(date('Y-m-d'));
                if (($check_date >= $from) && ($check_date <= $to)) {
                    //Check sales register_sibling is already updated then validate with field_sr_bill_date
                    $sr_bill_date = get_sales_register_sibling_sr_bill_date(trim($value['BILL_NUMBER']),$dis_id);
                    if (!empty($sr_bill_date) && $sr_bill_date != $value['BILL_DATE']) {
                        $value['error_message'] .= "Bill Date should be equal to previously uploaded bill date for Bill No " . $value['BILL_NUMBER'] . ".";
                    }
                }
                else {
                    $value['error_message'] .= "Enter correct Bill Date for Bill No " . $value['BILL_NUMBER'] . ". </br>";
                }
            }
        }
        if (!empty($value['BILL_NUMBER'])) {
            $unique_comb = $value['BILL_NUMBER'] ;
            $bill_distributor_id = get_distributor_id_of_bill(trim($value['BILL_NUMBER']),$sibling_distributor_ids);
            if (!empty($bill_distributor_id) && $bill_distributor_id!=$dis_id) {
                $value['error_message'] .= "Bill Number: " . $value['BILL_NUMBER'] . ' already exist' . ". </br>";
            }
            if (in_array($unique_comb, $context['results']['unique_combination'])) {
                $value['error_message'] .= "BILL_NUMBER " . $value['BILL_NUMBER'] . " is Repeated.  ";
            }
            else {
                $context['results']['unique_combination'][$unique_comb] = $unique_comb;
            }
        }
        else {
            $value['error_message'] .= "BILL_NUMBER Can't be blank. ";
        }
//      $value['BILL_NUMBER'] = trim($value['BILL_NUMBER']);
//      // Check if bill no already created as a node in content type sales_register_sibling_data
//      $title1 = check_node_title_exists($value['BILL_NUMBER'], 'sales_register_sibling_data');
//      if (!empty($title1)) {
//       
//        $title2 = check_node_title_exists($value['BILL_NUMBER'], 'sales_register_sibling_data', TRUE);
//        if (!empty($title2)) {
//          // will have to ovveride the node in this case
//        }
//        else {
//          $value['error_message'] .= " Bill no " . $value['BILL_NUMBER'] . ' already exist in our system.';
//        }
//      }
//    }
        if (isset($value['BILL_VALUE'])) {

            // Check if bill no already created as a node in content type sales_register_sibling_data
            if (!is_numeric($value['BILL_VALUE'])) {
                $value['error_message'] .= " Bill no " . $value['BILL_NUMBER'] . ' should have numeric bill value.';
            }
        }
        if (isset($value['PARTY_CODE'])) {
            $value['PARTY_CODE'] = trim($value['PARTY_CODE']);
            // Check if PARTY_CODE doesnt exist in outlet master
            $party_code = check_node_title_exist_of_distributor($value['PARTY_CODE'], 'outlet_master', FALSE, $dis_id);
            if (empty($party_code)) {
                $value['error_message'] .= " Party code " . $value['PARTY_CODE'] . ' does not exist in our system.';
            }
            // checking hul_address confirmed for that outlet
            if (user_access('hul_address_confirm_upload') && !empty($value['PARTY_CODE'])) {
                global $base_url;
                $page_url = $base_url . '/address/confirm';
                $hul_details = check_hul_address_confirmed_or_not($value['PARTY_CODE'], $dis_id); //returns hul_code, 
                if (!empty($hul_details)) {
                    foreach ($hul_details as $key => $details) {
                        if (!in_array($details['party_code'], $party_codes)) {
                            $party_codes[] = $details['party_code'];

                            $arugments = '?pl_group=' . urlencode($details['pl_group']) . '&sales_day=' . urlencode($details['sales_day']) . '&beat_name=' . urlencode($details['beat_name']) . '&hul_status=' . urlencode(0) . '&distributor_id=' . urlencode($dis_id);
                            $url = $page_url . $arugments;
                            $value['error_message'] = t('Address not confirmed for Party Code: ' . l($details['party_code'] . '- (HUL Code:' . $details['hul_code'] . ')', $url, array('attributes' => array('target' => '_blank'))));
                        }
                    }
                }
            }
        }
        /* if (empty($distributor_id)) {
          $value['error_message'] .= " Please check your Distributor code";
          } */

        if (variable_get('party_bill_check') == 1) {
            if (isset($value['PARTY_CODE']) && isset($value['BILL_NUMBER']) && isset($value['BILL_DATE'])) {
                $unique_comb = $value['PARTY_CODE'] . $value['BILL_DATE'];
                $context['results']['unique_party_billdate'][$unique_comb][$value['BILL_NUMBER']] = $value['BILL_NUMBER'];
                $config_party_code = explode(',', variable_get('party_code_check'));
                if (!in_array($value['PARTY_CODE'], $config_party_code)) {
                    if (count($context['results']['unique_party_billdate'][$unique_comb]) > 1) {
                        $value['error_message'] .= "Duplicate Bills are found for party code " . $value['PARTY_CODE'];
                    }
                    else {
                        $query = db_select('node', 'n');
                        $query->condition('n.type', 'sales_register_sibling_data');
                        $query->condition('n.uid', $dis_id, '=');
                        $query->condition('n.title', $value['BILL_NUMBER'], '!=');
                        $query->join('field_data_field_sr_new_bill_date', 'bd', 'n.nid = bd.entity_id');
                        $query->condition('bd.field_sr_new_bill_date_value', $value['BILL_DATE'], '=');
                        $query->join('field_data_field_party_code_reference', 'pc', 'pc.entity_id = n.nid');
                        $query->join('node', 'n1', 'n1.nid = pc.field_party_code_reference_nid');
                        $query->condition('n1.title', $value['PARTY_CODE'], '=');
                        $query->fields('n1', array('title'));
                        $result = $query->execute()->fetchField();
                        if (!empty($result)) {
                            $value['error_message'] .= "Duplicate Bills are found for party code " . $result;
                        }
                    }
                }
            }
        }

        // insert into custom sales table
        change_feeds_import_insert_data_in_sibling_custom_sales_table($value, $fid, $dis_id);

        $context['results']['fid'] = $fid;
    }
}

function sales_register_sibling_data_csv_finished($success, $results, $operations) {
    if ($success) {
        $fid = $results['fid'];
        //drupal_goto("sales-confirm/" . $fid);
        $_GET['destination'] = "sales-confirm/" . $fid;
    }
    else {
        drupal_set_message('There is Error Occured in batch process');
    }
}

function change_feeds_import_insert_data_in_sibling_custom_sales_table($value, $fid, $dis_id) {
   global $user; 
   $created_by_distributor_id =   change_feeds_import_get_distributor_id($user->uid);
    $sales_data = db_insert('custom_sales_data') // Table name no longer needs {}
      ->fields(array(
        'bill_no' => trim($value['BILL_NUMBER']),
        'date' => trim($value['BILL_DATE']),
        'bill_value' => trim($value['BILL_VALUE']),
        'created' => time(),
        'error_message' => trim($value['error_message']),
        'fid' => $fid,
        'party_code' => trim($value['PARTY_CODE']),
        'distributor_id' => $dis_id,
        'created_by_distributor_id' => $created_by_distributor_id,
//        'outlet_order_no'=> $value['Outlet_Order_No'],
//        'crate_id'=> $value['Crate_ID'],
      ))
      ->execute();
}

function get_sales_register_sibling_sr_bill_date($bill_number, $distributor_id) {
    $bill_number = trim($bill_number);
    $query = db_select('node', 'n');
    $query->condition('n.uid', $distributor_id);
    $query->condition('n.type', 'sales_register_data');
    $query->condition('n.title', $bill_number, '=');
    $query->join('field_data_field_sr_bill_date', 'sr_bill', 'sr_bill.entity_id = n.nid');
    $query->fields('sr_bill', array('field_sr_bill_date_value'));
    $sr_bill_dates = $query->execute()->fetchField();
    return $sr_bill_dates;
}

