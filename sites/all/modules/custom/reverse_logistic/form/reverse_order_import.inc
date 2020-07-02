<?php

/**
 * Upload form for custom order box id
 * @global type $user
 * @param type $form
 * @param type $form_state
 * @return string
 */
function reverse_order_import_form($form, &$form_state) {
    global $user;
    $user_id = $user->uid;
    $distributor_id = change_feeds_import_get_distributor_id($user_id);

    $template = 'reverse_order_import_template.csv';


    $form['file'] = array(
        '#type' => 'file',
        '#title' => t('Choose a CSV file for order vs bill upload'),
    );
    $form['template_download'] = array(
        '#type' => 'markup',
        '#prefix' => '<div id="template_wrapper">',
        '#suffix' => '</div>',
        '#markup' => l("Download Template", drupal_get_path('module', 'reverse_logistic') . '/' . $template),
    );
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Upload'),
    );

    return $form;
}

function reverse_order_import_form_validate($form, &$form_state) {
    global $user;
    $file = file_save_upload('file', array(
        'file_validate_extensions' => array('csv'),
    ));

    $header_array_from_xsd = array(
        'Bill Number',
        'Party Code',
        'Parent Party Code',
        'SKU7 Code',
        'To be returned Qty',
        'PKM',
        'Reverse Order Date',
        'Order type'
    );

    if ($file) {
        if ($file = file_move($file, 'public://')) {// move from source to destination source = file object & destination = 'public dir'
            $form_state['values']['file'] = $file; // move file object to form state
            $filepath = $form_state['values']['file']->uri;
            $data = array_map("str_getcsv", file($filepath, FILE_SKIP_EMPTY_LINES));
            $header_from_csv = array_shift($data);
            if ($header_array_from_xsd != $header_from_csv) {
                $type = 'error';
                $error_message = '';
                $message = 'Header mismatch.';
                form_set_error('file_upload', t($message));
                // log
            }
            if (count($data) > 15001) {
                // Count includes header count as well
                $message = 'File Row Count should not be greater than 15000';
                form_set_error('file_upload', t($message));
            } else {
                foreach ($data as $key => $value) {

                    $bill_no = $value[0];
                    $party_code = $value[1];
                    $parent_party_code = $value[2];
                    $sku7_code = $value[3];
                    $to_be_return_qty = $value[4];
                    $pkm = $value[5];
                    $reverse_order_date = $value[6];
                    $order_type = $value[7];


                    if ($order_type == 'Regular') {
                        if (empty($bill_no)) {
                            $error_message .= "Bill Number should not be empty at line " . ($key + 2) . '<br>';
                        } else {
                            $bill_number = validate_content_title($bill_no, 'sales_register_data');
                            if (empty($bill_number)) {
                                $error_message .= "Bill Number does not exist in Sale Register at line" . ($key + 2) . '<br>';
                            } else {
                                $nid = check_bill_exists_with_party($bill_number, 'sales_register_data', $party_code);
                                if (empty($nid)) {
                                    $error_msg = "Bill Number and Parent Party Code is different in system for line" . ($key + 2) . '<br>';
                                }
                            }
                        }
                    }
                    if (empty($party_code)) {
                        $error_message .= "Party Code should not be empty at line " . ($key + 2) . '<br>';
                    }
                    if (empty($parent_party_code)) {
                        $error_message .= "Parent party Code should not be empty at line " . ($key + 2) . '<br>';
                    } else {
                        $hul_code = validate_content_title($parent_party_code, 'hul_code_master');
                        if (empty($hul_code)) {
                            $error_message .= "Parent party Code does not exist in HUL Code Master at line" . ($key + 2) . '<br>';
                        }
                    }
                    if (empty($sku7_code)) {
                        $error_message .= "Sku7 code should not be empty at line " . ($key + 2) . '<br>';
                    }
                    if ($to_be_return_qty < 1) {
                        $error_message .= "To be return qty should not be greater than zero at line " . ($key + 2) . '<br>';
                    }else{
                        $delivered_qty = get_delivered_qty_from_bill_sku($bill_no, $sku7_code);
                        if($delivered_qty === FALSE){
                            $error_msg .= "Bill SKu combination is not present in System at line " . ($key + 2) . '<br>';
                        }else{
                            if($to_be_return_qty > $delivered_qty){
                                $error_msg .= "To be return Quantity can not be grater than Actual Delivered Quantity at line " . ($key + 2) . '<br>';
                            }
                        }
                    }
                    
                    if (empty($pkm)) {
                        $error_message .= "PKM should not be empty at line " . ($key + 2) . '<br>';
                    }
                    if (empty($reverse_order_date)) {
                        $error_message .= "Reverse Order Date should not be empty at line " . ($key + 2) . '<br>';
                    }
                    if (empty($order_type)) {
                        $error_message .= "Order Type should not be empty at line " . ($key + 2) . '<br>';
                    }

                    $unique_comb = $bill_no . $parent_party_code . $sku7_code . $pkm . $order_type;
                    if (array_key_exists($unique_comb, $res['unique_combination'])) {
                        form_set_error('duplicate_data', t('Duplicate Data Found , Please Remove Duplicate Data and Upload Again.'));
                    }
                    $res['unique_combination'][$unique_comb] = $unique_comb;
                }
            }
        } else {
            form_set_error('file_upload', t('Failed to write the uploaded file the site\'s file folder.'));
        }
    } else {
        form_set_error('file', t('No file was uploaded.'));
    }
    if (!empty($error_message)) {
        form_set_error('file', t($error_message));
    }
}

/**
 * 
 * @global type $user
 * @param type $form
 * @param type $form_state
 */
function reverse_order_import_form_submit($form, &$form_state) {
    $file = $form_state['values']['file'];
    $file_name = $file->filename;
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
    $data_chunk = array_chunk($datas, 300);
    foreach ($data_chunk as $data) {
        $operations[] = array('reverse_order_process', array($data, $file_name));
    }

    $batch = array(
        'title' => t('Reading CSV Data'),
        'operations' => $operations,
        'init_message' => t("Please wait..."),
        'progress_message' => t('Processed @current out of @total.'),
        'error_message' => t('Upload data processing has encountered an error.'),
        'finished' => 'reverse_order_process_finished',
        'file' => drupal_get_path('module', 'reverse_logistic') . '/form/reverse_order_import.inc',
    );
    batch_set($batch);
}

function reverse_order_process_finished($success, $results, $operations) {
    if ($success) {
        if (!empty($results)) {
            drupal_set_message('Reverse Order Data Upload Completed successfully');
        }
    } else {
        drupal_set_message('There is Error Occured in batch process');
    }
}

function reverse_order_process($data, $file_name, &$context) {

    foreach ($data as $key => &$value) {
        $bill_number = trim($value['Bill Number']); //Order Number//
        $party_code = trim($value['Party Code']); //Order Date//
        $parent_party_code = trim($value['Parent Party Code']); //Bill Ref. No
        $sku7_code = trim($value['SKU7 Code']); //Bill Ref. No
        $to_be_return_qty = trim($value['To be returned Qty']); //Bill Ref. No
        $pkm = trim($value['PKM']); //Bill Ref. No
        $reverse_order_date = trim($value['Reverse Order Date']); //Bill Ref. No
        $order_type = trim($value['Order type']); //Bill Ref. No

        $distributor_id = get_distributor_by_title($parent_party_code, 'hul_code_master');
        $result = db_insert('reverse_order_data')
                ->fields(array(
                    'transaction_id' => time(),
                    'transaction_line_id' => time() . '_' . $key,
                    'bill_number' => $bill_number ? $bill_number : '',
                    'party_code' => $party_code ? $party_code : '',
                    'parent_party_code' => $parent_party_code ? $parent_party_code : '',
                    'sku7_code' => $sku7_code ? $sku7_code : '',
                    'to_be_returned_qty' => $to_be_return_qty ? $to_be_return_qty : 0,
                    'pkm' => $pkm ? $pkm : '',
                    'reverse_order_date' => $reverse_order_date ? strtotime($reverse_order_date) : 0,
                    'order_type' => $order_type ? $order_type : '',
                    'distributor_id' => $distributor_id,
                    'received_through' => 1, //Uploader
                    'status' => 0, //Pending
                    'created_time' => REQUEST_TIME
                ))
                ->execute();
    }
}
