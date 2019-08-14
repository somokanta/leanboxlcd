<?php
/**
 * Upload form for custom order box id
 * @global type $user
 * @param type $form
 * @param type $form_state
 * @return string
 */

function custom_order_box_id_upload_form($form, $form_state) {
  global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);
  $form['file'] = array(
    '#type' => 'file',
    '#title' => t('Choose a CSV file for custom order box id upload'),
  );
  $form['template_download'] = array(
    '#type' => 'markup',
    '#prefix' => '<div id="template_wrapper">',
    '#suffix' => '</div>',
    '#markup' => l("Download Template", drupal_get_path('module', 'custom_order_box_id') . '/custom_order_box_id_template.csv'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Upload'),
  );

  return $form;
}

function custom_order_box_id_upload_form_validate($form, &$form_state) {
  
  $file = file_save_upload('file', array(
    'file_validate_extensions' => array('csv'),
  ));
  $header_array_from_xsd = array(
    'BILL_NUMBER',
    'Outlet_Order_No'
  );

  if ($file) {
    if ($file = file_move($file, 'public://')) {// move from source to destination source = file object & destination = 'public dir'
      $form_state['values']['file'] = $file;// move file object to form state
      $filepath = $form_state['values']['file']->uri;
      $data = array_map("str_getcsv", file($filepath, FILE_SKIP_EMPTY_LINES));
      $header_from_csv = array_shift($data);
      //dpm($header_from_csv);
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

        foreach ($data as $key => $value) {
          $bill_no = $value[0];
          $outlet_order_no = $value[1];
          $unique_comb = $bill_no . $outlet_order_no;
          if (array_key_exists($unique_comb, $res['unique_combination'])) {
            form_set_error('duplicate_data', t('Duplicate Data Found , Please Remove Duplicate Data and Upload Again.'));
          }
          $res['unique_combination'][$unique_comb] = $unique_comb;
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

/**
 * 
 * @global type $user
 * @param type $form
 * @param type $form_state
 */
function custom_order_box_id_upload_form_submit($form, &$form_state) {
  /*
  global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);
  $_SESSION['distributor_id'] = $distributor_id;
  $file = $form_state['values']['file'];
  $file_name = $file->filename;
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

  $error = change_feeds_import_revised_billing_validate($datas, $distributor_id);
  if (!empty($error)) {
    // show error here
    $error_line = implode("<br>", $error);
    drupal_set_message("The following error occured while processing: <br>" . $error_line, 'error');
  }
  else {

    $unique_bills = array_unique(array_map(function($elem) {
        return $elem['BILL_NUMBER'];
      }, $datas));

        $bill_not_in_dis_qty_conf = find_bill_not_in_dispatch_qty_confirmation($unique_bills, $distributor_id);
        //upload only cld and loose confirmed bills
        if(is_array($bill_not_in_dis_qty_conf)){
          $bills_present = array_diff($unique_bills,$bill_not_in_dis_qty_conf);
        }else{// if no pending then upload all bills
          $bills_present = $unique_bills;
        }
        
        $cld_loose_pending_bills = check_bill_cld_loose_confirmation_pending($bills_present, $distributor_id);
        //upload only cld and loose confirmed bills
        if(is_array($cld_loose_pending_bills)){
          $bills_cl_confirmed = array_diff($bills_present,$cld_loose_pending_bills);
        }else{// if no pending then upload all bills
          $bills_cl_confirmed = $bills_present;
        }
      
      // check bill confirm edit in trip table for all bills. if its close then don't upload the data
      //we can send $unique_bills as well as $bills_cl_confirmed. because result is same
        $confirmed_bills = check_bill_confirmation_complete($bills_cl_confirmed, $distributor_id);
        if (is_array($confirmed_bills)) {
          $bills_upload = array_diff($bills_cl_confirmed, $confirmed_bills);
        }
        else {
          $bills_upload = $bills_cl_confirmed;
        }
//      if (empty($confirmed_bills)) {
        $operations = array();
        $operations[] = array('change_feeds_import_revised_updated_flag_unset', array($bills_upload, $distributor_id));
        
        $data_chunk = array_chunk($datas, 1000);
        foreach ($data_chunk as $data) {
          $operations[] = array('change_feeds_import_revised_billing_process', array($data, $bills_upload, $distributor_id));
        }

        //For bill update flag which are not present in file check isnull in function
        $operations[] = array('change_feeds_import_revised_billing_update_flag', array($bills_upload, $distributor_id));
        
        //Sales register Upload
        $unique_bills_chunk = array_chunk($bills_upload, 300);
        foreach ($unique_bills_chunk as $bills_chunk) {
          $operations[] = array('sales_register_update_create_billing', array($bills_chunk,$distributor_id));
        }
        $batch = array(
          'title' => t('Reading Billing Data Csv'),
          'operations' => $operations,
          'init_message' => t("Batch is starting."),
          'progress_message' => t('Processed @current out of @total.'),
          'error_message' => t('Upload data processing has encountered an error.'),
          'finished' => 'revised_billing_data_csv_finished',
          'file' => drupal_get_path('module', 'change_feeds_import') . '/form/revised_billing_data.inc',
        );

        batch_set($batch);
  }*/
}