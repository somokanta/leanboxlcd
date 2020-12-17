<?php

/**
 * Upload form for custom order box id
 * @global type $user
 * @param type $form
 * @param type $form_state
 * @return string
 */
function custom_order_box_id_upload_sibling_form($form, &$form_state) {
    global $user;
    $user_id = $user->uid;
    $sibling_central_distributor_id = change_feeds_import_get_distributor_id($user_id);
    $form_state['sibling_central_distributor_id'] = $sibling_central_distributor_id;
    //add distributor id columns for central distributor
    if (in_array('sibling_central_distributor', $user->roles)) {
        $template = 'order_vs_bill_import_template_central.csv';
        $form['file'] = array(
          '#type' => 'file',
          '#title' => t('Choose a CSV file for order vs bill upload'),
        );
        $form['template_download'] = array(
          '#type' => 'markup',
          '#prefix' => '<div id="template_wrapper">',
          '#suffix' => '</div>',
          '#markup' => l("Download Template", drupal_get_path('module', 'custom_order_box_id') . '/' . $template),
        );
        $form['submit'] = array(
          '#type' => 'submit',
          '#value' => t('Upload'),
        );
    }
    else {
        drupal_goto("order-vs-bill-import");
    }

    return $form;
}

function custom_order_box_id_upload_sibling_form_validate($form, &$form_state) {
    $sibling_central_distributor_id =  $form_state['sibling_central_distributor_id'];
    $file = file_save_upload('file', array(
      'file_validate_extensions' => array('csv'),
    ));

    $header_array_from_xsd = array(
      'Order Number',
      'Order Date',
      'Bill Ref. No',
      'Hul Code',
      'Distributor Name'
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
            //$data = array_map("str_getcsv", file($filepath, FILE_SKIP_EMPTY_LINES));
            //$data1 = array_shift($data);
           if (count($data) > 15001) {
                // Count includes header count as well
                $message = 'File Row Count should not be greater than 15000';
                form_set_error('file_upload', t($message));
            }
            else {

                $sibling_child_distributor_ids = get_sibling_child_distributor_ids($sibling_central_distributor_id);
                foreach ($data as $key => $value) {
                    $dist_name = strtolower(trim($value[4]));
                    $distributor_names_array[$dist_name] = $dist_name;
                    $bill_no = trim($value[2]);
                    $bills_array[$bill_no] = $bill_no;
                    $hul_code = trim($value[3]);
                    $hul_code_array[$hul_code] = $hul_code;
                    $outlet_order_no = trim($value[0]);
                    $outlet_order_numbers[$outlet_order_no] = $outlet_order_no;
                }
                $bill_distributor_ids = get_distributor_id_of_bills($bills_array, $sibling_child_distributor_ids);
                $hul_code_distributor_ids = get_distributor_id_of_hul_codes($hul_code_array, $sibling_child_distributor_ids);
                $distributor_names = validate_distributor_names($distributor_names_array);
                $form_state['unique_distributor_ids'][] = $sibling_child_distributor_ids;
                $form_state['distributor_data']['bills_data'] = $bill_distributor_ids;
                $form_state['distributor_data']['hulcode_data'] = $hul_code_distributor_ids;
                $crate_ids = array();
                foreach ($data as $key => $value) {
                    $outlet_order_no = $value[0];
                    $order_date = $value[1];
                    $bill_no = trim($value[2]);
                    $hul_code = trim($value[3]);
                    $distributor_name = trim(strtolower($value[4]));
                    if (empty($distributor_names[$distributor_name])) {
                        $error_message .= t('Invalid Distributor Name: ' . $distributor_name) . ' at line No:'. ($key + 2) . '<br>';
                    }
                    if (empty($hul_code_distributor_ids[$hul_code]) && !empty($hul_code)) {
                        $error_message .= t('Invalid Hul Code: ' . $hul_code) . ($key + 2) . '<br>';
                    }
                    if (empty($bill_distributor_ids[$bill_no]) && !empty($bill_no)) {
                        $error_message .= "Bill Number Ref $bill_no does not exist at line No: " . ($key + 2) . '<br>';
                    }
                    if (empty($order_date)) {
                        $error_message .= "Order Date should not be empty at line No: " . ($key + 2) . '<br>';
                    }
                    if (empty($outlet_order_no)) {
                        $error_message .= "Order Number should not be empty at line No " . ($key + 2) . '<br>';
                    }
                    if (empty($hul_code)) {
                        $error_message .= "Hul code should not be empty at line No " . ($key + 2) . '<br>';
                    }
                    $unique_comb = $outlet_order_no . $bill_no;
                    if (array_key_exists($unique_comb, $res['unique_combination'])) {
                        form_set_error('duplicate_data', t('Duplicate Data Found , Please Remove Duplicate Data and Upload Again.'));
                    }
                    $res['unique_combination'][$unique_comb] = $unique_comb;
                    
                    $crate_ids[$bill_no] = check_crate_id_exist_sibling($outlet_order_no, $bill_no, $sibling_child_distributor_ids, $hul_code);
                    if (empty($crate_ids[$bill_no])) {
                        $error_message .= "Invalid Order Number/Bill Number mapping Or Invalid Hul code Or Crate Id for Bill Number: $bill_no does not exist at line " . ($key + 2) . '<br>';
                    }
                    if (!empty($bill_distributor_ids[$bill_no])) {
                         $distributor_id = $bill_distributor_ids[$bill_no];
                    }
                    $order_party_code = get_party_code_of_order_no_sibling($outlet_order_no, $distributor_id);
                    $bill_party_code = get_party_code_of_bill_no_sibling($bill_no, $distributor_id);
                    $order_party_code = strtolower($order_party_code);
                    $bill_party_code = strtolower($bill_party_code);
                    if ($order_party_code !== $bill_party_code) {
                        $error_message .= "Order Number $outlet_order_no has Party Code $order_party_code whereas its Bill Number $bill_no has Party Code $bill_party_code at line" . ($key + 2) . '<br>';
                    }
                    // same bill number should not be present in sibling location coz bill number is unique in sibling location
                    $bill_number_count = check_same_bill_number_exist_for_sibling($bill_no, $sibling_child_distributor_ids);
                    if ($bill_number_count>1) {
                       $mess .= t('Bill Number : ' . $bill_no. ' exist for multiple Sibling Distributor Location.') . '<br>';  
                    }
                }
                    $form_state['distributor_data']['crate_ids'] = $crate_ids;
            }
        }
        else {
            form_set_error('file_upload', t('Failed to write the uploaded file the site\'s file folder.'));
        }
    }
    else {
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
function custom_order_box_id_upload_sibling_form_submit($form, &$form_state) {
    $sibling_child_distributor_ids = $form_state['unique_distributor_ids'];
    $bill_distributor_ids = $form_state['distributor_data']['bills_data'] ;
    $hul_code_distributor_ids = $form_state['distributor_data']['hulcode_data'] ;
    $crate_ids = $form_state['distributor_data']['crate_ids'] ;
    $file = $form_state['values']['file'];
    $file_name = $file->filename;
    $filepath = $file->uri;
    $max = count(file($file->uri)); // count of row
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
    $data_chunk = array_chunk($datas, 1);

    foreach ($data_chunk as $data) {

        $operations[] = array('custom_order_box_id_process_sibling', array($max, $data, $file_name, $bill_distributor_ids,$hul_code_distributor_ids,$crate_ids));
    }

    $batch = array(
      'title' => t('Reading CSV Data'),
      'operations' => $operations,
      'init_message' => t("Please wait..."),
      'progress_message' => t('Processed @current out of @total.'),
      'error_message' => t('Upload data processing has encountered an error.'),
      'finished' => 'order_vs_bill_data_csv_finished_sibling',
      'file' => drupal_get_path('module', 'custom_order_box_id') . '/form/custom_order_box_id_sibling.inc',
    );
    batch_set($batch);
}

function order_vs_bill_data_csv_finished_sibling($success, $results, $operations) {
    if ($success) {
        if (!empty($results)) {
            drupal_set_message('Sales Register Data Upload Completed successfully');
        }
    }
    else {
        drupal_set_message('There is Error Occured in batch process');
    }
}

function custom_order_box_id_process_sibling($max, $data, $file_name, $bill_distributor_ids,$hul_code_distributor_ids,$crate_ids, &$context) {
    foreach ($data as $key => &$value) {
        $outlet_order_ref = trim($value['Order Number']); //Order Number//
        $order_date = trim($value['Order Date']); //Order Date//
        $bill_number = trim($value['Bill Ref. No']); //Bill Ref. No
        $distributor_id = $bill_distributor_ids[$bill_number];
        $crate_id = $crate_ids[$bill_number];
        if (!empty($outlet_order_ref)) {
            $query = db_select('node', 'n');
            $query->condition('n.title', $bill_number);
            $query->condition('n.type', 'sales_register_data');
            $query->condition('n.uid', $distributor_id);
            $query->fields('n', array('nid'));
            $sales_register_data_nid = $query->execute()->fetchField();
            $node = node_load($sales_register_data_nid);
            if (!empty($node)) {
                $node->field_crate_id['und'][0]['value'] = $crate_id ? $crate_id : '';
                $node->field_outlet_order_no['und'][0]['value'] = $outlet_order_ref;
                node_save($node);
                $context['results'][] = $node->nid;
                if (empty($crate_id)) {
                    drupal_set_message("Crate Id is not present for  Order Number  : $outlet_order_ref");
                }
            }
            // update order_upload_datetime
            db_update('outlet_order_crate')
              ->fields(array('order_upload_datetime' => REQUEST_TIME))
              ->condition('outlet_order_no', $outlet_order_ref, '=')
              ->condition('distributor_id', $distributor_id, '=')
              ->execute();
        }
        else {
            drupal_set_message("Order Number can't be blank for Bill Number Ref : $bill_number", 'warning');
        }
    }
}

function check_crate_id_exist_sibling($outlet_order_nos, $bill_numbers, $sibling_distributor_ids, $hul_codes) {
    $results = array();
    if (!empty($outlet_order_nos) && !empty($sibling_distributor_ids) && !empty($bill_numbers) && !empty($hul_codes)) {
        $query = db_select('outlet_order_crate', 'ooc');
        $query->condition('ooc.outlet_order_no', $outlet_order_nos, '=');
        $query->join('node', 'outlet_master', 'outlet_master.title = ooc.party_code AND outlet_master.uid = ooc.distributor_id'); //outlet node
        $query->join('field_data_field_party_code_reference', 'party_ref', 'party_ref.field_party_code_reference_nid = outlet_master.nid');
        $query->join('field_data_field_hul_code_reference', 'hul_code_ref', 'outlet_master.nid = hul_code_ref.entity_id');
        $query->join('node', 'hul_code', 'hul_code.nid = hul_code_ref.field_hul_code_reference_nid'); //hul node
        $query->join('node', 'sales_register', 'sales_register.nid = party_ref.entity_id'); // sales node
        $query->condition('sales_register.type', 'sales_register_data');
        $query->condition('sales_register.uid', $sibling_distributor_ids, 'IN');
        $query->condition('ooc.distributor_id', $sibling_distributor_ids, 'IN');
        $query->condition('hul_code.uid', $sibling_distributor_ids, 'IN');
        $query->condition('ooc.hul_code', $hul_codes, '=');
        $query->condition('hul_code.title', $hul_codes, '=');
        $query->condition('sales_register.title', $bill_numbers, '=');
//        $query->fields('sales_register', array('title'));
        $query->fields('ooc', array('crate_id'));
        $results = $query->execute()->fetchField();
    }
    return $results;
}

function get_party_code_of_order_no_sibling($outlet_order_no, $sibling_distributor_id) {
    $party_code = '';
    if (!empty($outlet_order_no) && !empty($sibling_distributor_id)) {
        $query = db_select('outlet_order_crate', 'ooc');
        $query->condition('ooc.outlet_order_no', $outlet_order_no);
        $query->condition('ooc.distributor_id', $sibling_distributor_id);
        $query->fields('ooc', array('party_code'));
        $party_code = $query->execute()->fetchField();
    }
    return $party_code;
}

function get_party_code_of_bill_no_sibling($bill_no, $sibling_distributor_id) {
    $party_code = '';
    if (!empty($bill_no) && !empty($sibling_distributor_id)) {
        $query = db_select('node', 'sales_data');
        $query->condition('sales_data.title', $bill_no);
        $query->condition('sales_data.uid', $sibling_distributor_id);
        $query->condition('sales_data.type', 'sales_register_data');
        $query->join('field_data_field_party_code_reference', 'party_ref', 'party_ref.entity_id = sales_data.nid');
        $query->join('node', 'outlet_master', 'outlet_master.nid = party_ref.field_party_code_reference_nid');
        $query->condition('outlet_master.type', 'outlet_master');
        $query->condition('outlet_master.uid', $sibling_distributor_id);
        $query->fields('outlet_master', array('title'));
        $party_code = $query->execute()->fetchField();
    }
    return $party_code;
}

