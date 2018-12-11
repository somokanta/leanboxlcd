<?php

function change_feeds_import_reupload_sales_register_form($form, $form_state) {
  global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);

  // check distributor incompleted upload
  $query = db_select('custom_sales_data', 'sd');
  $query->condition('sd.distributor_id', $distributor_id, '=');
  $query->fields('sd', array('fid'));
  $fid = $query->execute()->fetchField();

  if (!empty($fid)) {
    drupal_goto("sales-confirm/$fid");
  }

  $form['file'] = array(
    '#type' => 'file',
    '#title' => t('Choose a CSV file for sales data reupload'),
  );
  $form['template_download'] = array(
    '#type' => 'markup',
    '#prefix' => '<div id="template_wrapper">',
    '#suffix' => '</div>',
    '#markup' => l("Download Template", drupal_get_path('module', 'change_feeds_import') . '/reupload_sales_register_template.csv'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Upload'),
  );



  return $form;
}

function change_feeds_import_reupload_sales_register_form_validate($form, &$form_state) {
  $file = file_save_upload('file', array(
    'file_validate_extensions' => array('csv'),
  ));

  $header_array_from_xsd = array(
    'BILL_NUMBER',
    'BILL_DATE',
    'PARTY_CODE',
    'BILL_VALUE',
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
        $existing_bills = check_bill_exists_in_sales_register($all_bills);
        $not_exist_bills = array_diff($all_bills, $existing_bills);
        if (!empty($not_exist_bills)) {
          $not_exist_bill = implode($not_exist_bills, ',');
          form_set_error('file_upload', "Below bills are not present is sales register: <BR>" . $not_exist_bill);
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

function change_feeds_import_reupload_sales_register_form_submit($form, &$form_state) {
  global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);


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

  $error = change_feeds_import_reupload_sales_register_batch_fix_process($datas);
  if (!empty($error)) {
    // show error here
    $error_line = implode("<br>", $error);
    drupal_set_message("The following error occured while processing: <br>" . $error_line, 'error');
  }
  else {

    $operations = array();
    $data_chunk = array_chunk($datas, 200);
    foreach ($data_chunk as $data) {
      $operations[] = array('change_feeds_import_reupload_sales_register_node', array($data, $distributor_id));
    }

    $batch = array(
      'operations' => $operations,
      'finished' => 'reupload_sales_register_data_csv_finished',
      'title' => t('Creating Sales register Data'),
      'init_message' => t('Sales Data Batch Creation is starting.'),
      'progress_message' => t('Processed @current out of @total.'),
      'error_message' => t('Sales Data Batch has encountered an error.'),
      'file' => drupal_get_path('module', 'change_feeds_import') . '/form/reupload_sales_register_data.inc'
    );
    batch_set($batch);
  }
}

function change_feeds_import_reupload_sales_register_batch_fix_process($data) {
  $error = array();
  foreach ($data as $key => $value) {
    $error_text = '';
    if (isset($value['BILL_NUMBER'])) {
      $unique_comb = $value['BILL_NUMBER'];
      if (in_array($unique_comb, $context['unique_combination'])) {
        $error_text = "BILL_NUMBER " . $value['BILL_NUMBER'] . " is Repeated.  ";
      }
      else {
        $context['unique_combination'][] = $unique_comb;
      }
    }

    if ((empty($value['BILL_VALUE']) && ($value['BILL_VALUE'] != '0') ) || $value['BILL_VALUE'] < '0') {
      $error_text = $k . " can not be empty or  less than 0(Zero) for bill no " . $value['BILL_NUMBER'] . ".";
    }

    if (isset($value['BILL_VALUE'])) {
      if (!is_numeric($value['BILL_VALUE'])) {
        $error_text = " Bill no " . $value['BILL_NUMBER'] . ' should have numeric bill value.';
      }
    }

    if (!empty($error_text)) {
      $error[] = $error_text;
    }
  }
  return $error;
}

function reupload_sales_register_data_csv_finished($success, $results, $operations) {
  if ($success) {
    drupal_set_message('Updated successfully');
    $_GET['destination'] = "reupload_sales_register/import";
  }
  else {
    drupal_set_message('There is Error Occured in batch process');
  }
}

function change_feeds_import_reupload_sales_register_node($sales_data_result, $distributor_id, &$context) {
  if (!empty($sales_data_result)) {
    foreach ($sales_data_result as $key => $val) {
      $cld_loose_pending = check_billwise_cld_loose_confirmation_pending($val['BILL_NUMBER'], $distributor_id);
      $bill_edit_conf = check_bill_edit_confirmation_complete($val['BILL_NUMBER'], $distributor_id);
      
      // CLD and Loose should be complete. if pending then don't update. because it should done after that
      //Bill wise edit confirmation should not complete. if completed then change not allowed
      // IMPORTANT cld loose pending should be empty and bill edit confirmation should not complete
      if(empty($cld_loose_pending) && empty($bill_edit_conf)){
        $nid = check_node_title_exists($val['BILL_NUMBER'], 'sales_register_data');
        $node = node_load($nid);
        if($node){
          $node->field_revised_host_bill_value[LANGUAGE_NONE][0]['value'] = $val['BILL_VALUE'];
          $node->field_reupload_sales_flag[LANGUAGE_NONE][0]['value'] = 1;
          $node->field_sales_reuploaded_date[LANGUAGE_NONE][0]['value'] = REQUEST_TIME;
          node_save($node);
        }
      }
    }
  }
}

function check_bill_exists_in_sales_register($all_bills) {
  global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);

  $query = db_select('node', 'n');
  $query->condition('n.uid', $distributor_id);
  $query->condition('n.type', 'sales_register_data');
  $query->condition('n.title', $all_bills, 'IN');
  $query->fields('n', array('title'));

  $results = $query->execute()->fetchCol();

  return $results;
}


/**
 * 
 * @param type $unique_bills
 * @param type $distributor_id
 * @return type
 */
function check_billwise_cld_loose_confirmation_pending($bill_number, $distributor_id) {
  $query = db_select('dispatch_qty_confirmation', 'n');
  $query->condition('n.bill_number', $bill_number);
  $query->condition('n.distributor_id', "$distributor_id");
  
  $db_or = db_or();
  $db_or->condition('n.loose_close_flag', 0, '=');
  $db_or->condition('n.cld_close_flag', 0, '=');
  $query->condition($db_or);
  
  $query->addField('n', 'bill_number');
  $bill_num = $query->execute()->fetchField();
  return $bill_num;
}

/**
 * 
 * @param type $unique_bills
 * @param type $distributor_id
 */
function check_bill_edit_confirmation_complete($bill_number, $distributor_id) {
  $query = db_select('node', 'n');
  $query->condition('n.type', "sales_register_data");
  $query->condition('n.uid', "$distributor_id");
  $query->condition('n.title', $bill_number);
  $query->join('field_data_field_trip_id_reference', 'tir', 'tir.entity_id = n.nid');
  $query->join('node', 'n2', 'n2.nid = tir.field_trip_id_reference_nid');
  $query->leftjoin('field_data_field_confirm_bill_edit_status', 'bds', 'bds.entity_id = n2.nid');
  $query->condition('bds.field_confirm_bill_edit_status_value', 1, '=');
  $query->addField('n', 'title');
  $confirmed_bill = $query->execute()->fetchField();
  return $confirmed_bill;
}