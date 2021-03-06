<?php

/**
 * Upload form for custom order box id
 * @global type $user
 * @param type $form
 * @param type $form_state
 * @return string
 */
function custom_order_box_id_upload_form($form, &$form_state) {
    global $user;
    $user_id = $user->uid;
    $distributor_id = change_feeds_import_get_distributor_id($user_id);
    $form_state['distributor_id'] = $distributor_id;
    if (in_array('sibling_central_distributor', $user->roles)) {
         drupal_goto("sibling/order-vs-bill-import");
    }
    //add distributor id columns for central distributor
    if (in_array('central_distributor', $user->roles)) {
        $template = 'order_vs_bill_import_template_central.csv';
        $form_state['central_distributor_id'] = $distributor_id;
    }
    else {
        $form_state['central_distributor_id'] = get_central_distributor_id($distributor_id);
        $template = 'order_vs_bill_import_template.csv';
    }

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

    return $form;
}

function custom_order_box_id_upload_form_validate($form, &$form_state) {
    global $user;
    $file = file_save_upload('file', array(
      'file_validate_extensions' => array('csv'),
    ));
    //headers matching for different roles form diff distributors
    if (in_array('central_distributor', $user->roles)) {
        $header_array_from_xsd = array(
          'Order Number',
          'Order Date',
          'Bill Ref. No',
          'Hul Code',
          'Distributor Name'
        );
    }
    else {
        $header_array_from_xsd = array(
          'Order Number',
          'Order Date',
          'Bill Ref. No',
          'Hul Code'
        );
    }

    $distributor_id = $form_state['distributor_id'];
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
            }
            else {


                foreach ($data as $key => $value) {

                    $outlet_order_no = $value[0];
                    $order_date = $value[1];
                    $bill_no = $value[2];
                    $hul_code = $value[3];

                    if (in_array('central_distributor', $user->roles)) {
                        $distributor_name = $value[4];
                        // passing distributor_name
                        $distributor_id = validate_distributor_name($distributor_name);

                        if (empty(check_valid_distributor($distributor_id))) {
                            $error_message .= t('Invalid Distributor Name: ' . $distributor_name) . '<br>';
                        }
                        // valid Distributor Name
                        else {
                            $form_state['unique_distributor_ids'][] = $distributor_id;
                            $form_state['distributor_data'][$distributor_name] = $distributor_id;
                        }
                    }
                    if (empty($order_date)) {
                        $error_message .= "Order Date should not be empty at line " . ($key + 2) . '<br>';
                    }
                    if (empty($outlet_order_no)) {
                        $error_message .= "Order Number should not be empty at line " . ($key + 2) . '<br>';
                    }
                    if (empty($hul_code)) {
                        $error_message .= "Hul code should not be empty at line " . ($key + 2) . '<br>';
                    }
                    else {
                        $hul_code = validate_hul_code($hul_code, $distributor_id);
                        if (empty($hul_code)) {
                            $error_message .= "HUL Code does not exist in HUL Code Master" . ($key + 2) . '<br>';
                        }
                    }
                    $unique_comb = $outlet_order_no . $bill_no . $distributor_id;
                    if (array_key_exists($unique_comb, $res['unique_combination'])) {
                        form_set_error('duplicate_data', t('Duplicate Data Found , Please Remove Duplicate Data and Upload Again.'));
                    }
                    $res['unique_combination'][$unique_comb] = $unique_comb;
                    $exist_bills = check_bill_exist($bill_no, $distributor_id); // check bill number in sales register
                    if (empty($exist_bills[0])) {
                        $error_message .= "Bill Number Ref $bill_no does not exist at line " . ($key + 2) . '<br>';
                    }
                    $crate_ids = check_crate_id_exist($outlet_order_no, $bill_no, $distributor_id,$hul_code); // check crate id in outlet order crate is exist or not
                    if (empty($crate_ids[0])) {
                        $error_message .= "Invalid Order Number/Bill Number mapping Or Invalid Hul code Or Crate Id for  $bill_no does not exist at line " . ($key + 2) . '<br>';
                    }
                    $order_party_code = get_party_code_of_order_no($outlet_order_no, $distributor_id);
                    $bill_party_code = get_party_code_of_bill_no($bill_no, $distributor_id);
                    $order_party_code = strtolower($order_party_code);
                    $bill_party_code = strtolower($bill_party_code);
                    if ($order_party_code !== $bill_party_code) {
                        $error_message .= "Order Number $outlet_order_no has Party Code $order_party_code whereas its Bill Number $bill_no has Party Code $bill_party_code at line" . ($key + 2) . '<br>';
                    }
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
function custom_order_box_id_upload_form_submit($form, &$form_state) {
    global $user;
    $user_id = $user->uid;
    $distributor_id = change_feeds_import_get_distributor_id($user_id);
    $_SESSION['distributor_id'] = $distributor_id;
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
    $data_chunk = array_chunk($datas, 300);
    foreach ($data_chunk as $data) {
        $operations[] = array('custom_order_box_id_process', array($max, $data, $distributor_id, $file_name, $form_state['distributor_data']));
    }

    $batch = array(
      'title' => t('Reading CSV Data'),
      'operations' => $operations,
      'init_message' => t("Please wait..."),
      'progress_message' => t('Processed @current out of @total.'),
      'error_message' => t('Upload data processing has encountered an error.'),
      'finished' => 'order_vs_bill_data_csv_finished',
      'file' => drupal_get_path('module', 'custom_order_box_id') . '/form/custom_order_box_id.inc',
    );
    batch_set($batch);
}

function order_vs_bill_data_csv_finished($success, $results, $operations) {
    if ($success) {
        if (!empty($results)) {
            drupal_set_message('Sales Register Data Upload Completed successfully');
        }
    }
    else {
        drupal_set_message('There is Error Occured in batch process');
    }
}

function check_bill_exist($bill_number, $distributor_id) {
    if (!empty($bill_number) && !empty($distributor_id)) {
        $query = db_select('node', 'n');
        $query->condition('n.uid', $distributor_id);
        $query->condition('n.type', 'sales_register_data');
        $query->condition('n.title', $bill_number);
        $query->fields('n', array('title'));
        $results = $query->execute()->fetchCol();
    }
    return $results;
}

function custom_order_box_id_process($max, $data, $distributor_id, $file_name, $csv_distributor_ids, &$context) {
    foreach ($data as $key => &$value) {
        $outlet_order_ref = trim($value['Order Number']); //Order Number//
        $order_date = trim($value['Order Date']); //Order Date//
        $bill_number = trim($value['Bill Ref. No']); //Bill Ref. No
        $csv_dist_id = $csv_distributor_ids[trim($value['Distributor Name'])];
        $dis_id = $csv_dist_id ? $csv_dist_id : $distributor_id;
        $distributorname = $value['Distributor Name'] ? trim($value['Distributor Name']) : get_distributor_name($dis_id);
        $hul_code = trim($value['Hul Code']);
        $crate_id = check_crate_id_exist($outlet_order_ref, $bill_number, $dis_id,$hul_code);
        
        $bill_number_arr[] = trim($value['Bill Ref. No']);

        if (!empty($outlet_order_ref)) {
            $query = db_select('node', 'n');
            $query->condition('n.title', $bill_number);
            $query->condition('n.type', 'sales_register_data');
            $query->condition('n.uid', $dis_id);
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
              ->condition('distributor_id', $dis_id, '=')
              ->execute();
        }
        else {
            drupal_set_message("Order Number can't be blank for Bill Number Ref : $bill_number", 'warning');
        }
        //$context['results']['total_lines']++;
    }
        $trip_id_res = get_trip_from_bill($bill_number_arr, $dis_id);
        foreach($trip_id_res as $val) {
            $trip_id = $val->title;
            $bill_numbers = $val->bill_number;
            if (!empty($trip_id)) {
                db_insert('sdr_app_log_preprocess') // Table name no longer needs {}
                        ->fields(array(
                            'trip_id' => $trip_id,
                            'distributor_id' => $dis_id,
                            'flag' => 0,
                            'created_date' => REQUEST_TIME,
                            'bills' => $bill_numbers
                        ))
                        ->execute();
            }
        }
        
}

/* function check_crate_id_exist($outlet_order_no, $distributor_id) {

  if (!empty($outlet_order_no) && !empty($distributor_id)) {
  $query = db_select('outlet_order_crate', 'ooc');
  $query->condition('ooc.outlet_order_no', $outlet_order_no);
  $query->condition('ooc.distributor_id', $distributor_id);
  $query->fields('ooc', array('crate_id'));
  $results = $query->execute()->fetchCol();
  }
  return $results;
  } */

function check_crate_id_exist($outlet_order_no, $bill_number, $distributor_id,$hul_code) {
    if (!empty($outlet_order_no) && !empty($distributor_id) && !empty($bill_number)) {
        $query = db_select('outlet_order_crate', 'ooc');
        $query->condition('ooc.outlet_order_no', $outlet_order_no);
        // Join with party name table to validate party code of bill number
        //$query->join('field_data_field_party_name', 'party_name', 'party_name.field_party_name_value = ooc.party_name');
        // join with party code reference field in sales register
        //$query->join('field_data_field_party_code_reference', 'party_ref', 'party_ref.field_party_code_reference_nid = party_name.entity_id');
        // join with bill number
        $query->join('node', 'outlet_master', 'outlet_master.title = ooc.party_code AND outlet_master.uid = ooc.distributor_id');//outlet node
        $query->join('field_data_field_party_code_reference', 'party_ref', 'party_ref.field_party_code_reference_nid = outlet_master.nid');
        $query->join('field_data_field_hul_code_reference', 'hul_code_ref', 'outlet_master.nid = hul_code_ref.entity_id');
        $query->join('node', 'hul_code', 'hul_code.nid = hul_code_ref.field_hul_code_reference_nid');//hul node
        $query->join('node', 'sales_register', 'sales_register.nid = party_ref.entity_id');// sales node
        $query->condition('sales_register.type', 'sales_register_data');
        $query->condition('sales_register.uid', $distributor_id);
        $query->condition('ooc.distributor_id', $distributor_id);
        $query->condition('hul_code.uid', $distributor_id);
        $query->condition('ooc.hul_code', $hul_code);
        $query->condition('hul_code.title', $hul_code);
        $query->condition('sales_register.title', $bill_number);
        $query->fields('ooc', array('crate_id'));
        $results = $query->execute()->fetchCol();
    }
    return $results;
}

function get_party_code_of_order_no($outlet_order_no, $distributor_id) {
    $query = db_select('outlet_order_crate', 'ooc');
    $query->condition('ooc.outlet_order_no', $outlet_order_no);
    $query->condition('ooc.distributor_id', $distributor_id);
    $query->fields('ooc', array('party_code'));
    $party_code = $query->execute()->fetchField();
    return $party_code;
}

function get_party_code_of_bill_no($bill_no, $distributor_id) {
    $query = db_select('node', 'sales_data');
    $query->condition('sales_data.title', $bill_no);
    $query->condition('sales_data.uid', $distributor_id);
    $query->condition('sales_data.type', 'sales_register_data');
    $query->join('field_data_field_party_code_reference', 'party_ref', 'party_ref.entity_id = sales_data.nid');
    $query->join('node', 'outlet_master', 'outlet_master.nid = party_ref.field_party_code_reference_nid');
    $query->condition('outlet_master.type', 'outlet_master');
    $query->condition('outlet_master.uid', $distributor_id);
    $query->fields('outlet_master', array('title'));
    $party_code = $query->execute()->fetchField();
    return $party_code;
}
function validate_hul_code($hul_code,$distributor_id){
  $query = db_select('node', 'n');
  $query->condition('n.type', 'hul_code_master');
  $query->condition('n.title', $hul_code);
  $query->condition('n.uid', $distributor_id);
  $query->fields('n', array('title'));
  $result = $query->execute()->fetchField();
  return $result;
}

function get_trip_from_bill($bill_number, $distributor_id) {
    $query = db_select('node', 'n');
    $query->condition('n.type', 'sales_register_data');
    $query->join('field_data_field_trip_id_reference', 'tr', 'tr.entity_id = n.nid');
    $query->join('node', 'n1', 'tr.field_trip_id_reference_nid = n1.nid');
    $query->condition('n.title', $bill_number, 'IN');
    $query->condition('n.uid', $distributor_id);
    $query->fields('n1', array('title'));
    $query->addExpression("GROUP_CONCAT(DISTINCT n.title separator ', ')", 'bill_number');
    $query->groupBy('n1.title');
    $trip_id = $query->execute()->fetchAll();
            
    return $trip_id;
}
