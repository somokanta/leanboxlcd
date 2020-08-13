<?php

/**
 * Upload form for custom order box id
 * @global type $user
 * @param type $form
 * @param type $form_state
 * @return string
 */
function order_receive_import_form($form, &$form_state) {
    global $user;
    $user_id = $user->uid;
    $distributor_id = change_feeds_import_get_distributor_id($user_id);

    $template = 'order_receive_import_template.csv';


    $form['file'] = array(
        '#type' => 'file',
        '#title' => t('Choose a CSV file for Order Receive upload'),
    );
    $form['template_download'] = array(
        '#type' => 'markup',
        '#prefix' => '<div id="template_wrapper">',
        '#suffix' => '</div>',
        '#markup' => l("Download Template", drupal_get_path('module', 'reverse_pickup') . '/' . $template),
    );
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Upload'),
    );

    return $form;
}

function order_receive_import_form_validate($form, &$form_state) {
    global $user;
    $file = file_save_upload('file', array(
        'file_validate_extensions' => array('csv'),
    ));

    $header_array_from_xsd = array(
        'Order Creation Date',
        'Parent Bill No', 
        'Child Order ID', 
        'Order Type', 
        'Parent Party Code',
        'Party Code',
        'LBSKUID',
        'PKD',
        'SKU7',
        'Product Name',
        'MRP',
        'Return Quantity',
        'Beat Name'
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
                    $parent_bill_no = $value[1];
                    $child_order_id = $value[2];
                    $party_code = $value[5];
                    $parent_party_code = $value[4];
                    $sku7_code = $value[8];
                    $return_qty = $value[11];
                    $pkd = $value[7];
                    $order_date = $value[0];
                    $order_type = $value[3];
                    $beat_name = $value[12];
                    $distributor_id = get_distributor_by_title($parent_party_code, 'hul_code_master');

                   // if ($order_type == 'Regular') {
                        if (empty($parent_bill_no)) {
                            $error_message .= "Bill Number should not be empty at line " . ($key + 2) . '<br>';
                        } else {
                            $bill_number = validate_content_title($parent_bill_no, 'sales_register_data');
                            if (empty($bill_number)) {
                                $error_message .= "Bill Number does not exist in Sale Register at line" . ($key + 2) . '<br>';
                            } else {
                                $nid = check_bill_exists_with_party($parent_bill_no, 'sales_register_data', $party_code);
                                if (empty($nid)) {
                                    $error_message .= "Bill Number and Parent Party Code is different in system for line" . ($key + 2) . '<br>';
                                }
                            }
                        }
                   // }
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
                    if ($return_qty < 1) {
                        $error_message .= "Return qty should not be greater than zero at line " . ($key + 2) . '<br>';
                    }else{
                        $delivered_qty = get_delivered_qty_from_bill_sku($parent_bill_no, $sku7_code, $distributor_id);
                        if($delivered_qty === FALSE){
                            $error_message .= "Bill SKU combination is not present in System at line " . ($key + 2) . '<br>';
                        }else{
                            if($return_qty > $delivered_qty){
                                $error_message .= "Return Quantity can not be greater than Actual Delivered Quantity at line " . ($key + 2) . '<br>';
                            }
                        }
                    }
                    
                    if (empty($pkd)) {
                        $error_message .= "PKD should not be empty at line " . ($key + 2) . '<br>';
                    }
                    if (empty($order_date)) {
                        $error_message .= "Order Creation Date should not be empty at line " . ($key + 2) . '<br>';
                    }
                    if (empty($order_type)) {
                        $error_message .= "Order Type should not be empty at line " . ($key + 2) . '<br>';
                    }
                    
                    if (empty($child_order_id)) {
                        $error_message .= "Child Order should not be empty at line " . ($key + 2) . '<br>';
                    }
                    
                    $child_order_res = check_child_order_in_order_receive($child_order_id, $distributor_id);
                    if ($child_order_res) {
                        $error_message .= "Child Order $child_order_id already uploaded  at line" . ($key + 2) . '<br>';
                    }
                    
                    $child_order_sales_register = check_child_order_in_sales_register($child_order_id, $distributor_id);
                    if ($child_order_sales_register) {
                        $error_message .= "Child Order $child_order_id already present in sales register  at line" . ($key + 2) . '<br>';
                    }
                    
                    

//                    $unique_comb = $bill_no . $parent_party_code . $sku7_code . $pkm . $order_type;
//                    if (array_key_exists($unique_comb, $res['unique_combination'])) {
//                        form_set_error('duplicate_data', t('Duplicate Data Found , Please Remove Duplicate Data and Upload Again.'));
//                    }
//                    $res['unique_combination'][$unique_comb] = $unique_comb;
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
function order_receive_import_form_submit($form, &$form_state) {
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
        $operations[] = array('order_receive_process', array($data, $file_name));
    }

    $batch = array(
        'title' => t('Reading CSV Data'),
        'operations' => $operations,
        'init_message' => t("Please wait..."),
        'progress_message' => t('Processed @current out of @total.'),
        'error_message' => t('Upload data processing has encountered an error.'),
        'finished' => 'order_receive_process_finished',
        'file' => drupal_get_path('module', 'reverse_pickup') . '/form/order_receive_import.inc',
    );
    batch_set($batch);
}

function order_receive_process_finished($success, $results, $operations) {
    if ($success) {
        drupal_set_message('Order Receive Data upload completed successfully');
    } else {
        drupal_set_message('There is Error Occured in batch process');
    }
}

function order_receive_process($data, $file_name, &$context) {

    foreach ($data as $key => &$value) {
        $order_creation_date = trim($value['Order Creation Date']);
        $parent_bill_number = trim($value['Parent Bill No']);
        $child_order_id = trim($value['Child Order ID']);
        $order_type = trim($value['Order Type']);
        $party_code = trim($value['Party Code']);
        $parent_party_code = trim($value['Parent Party Code']);
        $lb_sku_id = trim($value['LBSKUID']);
        $pkd = trim($value['PKD']); 
        $sku7_code = trim($value['SKU7']); 
        $product_name = trim($value['Product Name']); 
        $mrp = trim($value['MRP']); 
        $return_qty = trim($value['Return Quantity']);
        $beat_name = trim($value['Beat Name']);

        $distributor_id = get_distributor_by_title($parent_party_code, 'hul_code_master');
        $result = db_insert('order_receive_data')
                ->fields(array(
                    'order_creation_date' => $order_creation_date ? strtotime($order_creation_date) : 0,
                    'parent_bill_number' => $parent_bill_number ? $parent_bill_number : '',
                    'child_order_id' => $child_order_id ? $child_order_id : '',
                    'order_type' => $order_type ? $order_type : '',
                    'parent_party_code' => $parent_party_code ? $parent_party_code : '',
                    'party_code' => $party_code ? $party_code : '',
                    'lb_sku_id' => $lb_sku_id ? $lb_sku_id : '',
                    'pkd' => $pkd ? $pkd : '',
                    'sku7_code' => $sku7_code ? $sku7_code : '',                    
                    'product_name' => $product_name ? $product_name : '',                    
                    'mrp' => $mrp ? $mrp : '',                    
                    'return_qty' => $return_qty ? $return_qty : '',                    
                    'beat_name' => $beat_name ? $beat_name : '',                    
                    'distributor_id' => $distributor_id,
                    'type' => 1, //Uploader
                    'created_time' => REQUEST_TIME
                ))
                ->execute();
    }
}
