<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function full_deliver_upload_form($form, $form_state) {
  $form['file'] = array(
    '#type' => 'file',
    '#title' => t('Choose a CSV file. Allowed Status are deliver and undeliver'),
  );
  $form['template_download'] = array(
    '#type' => 'markup',
    '#prefix' => '<div id="template_wrapper">',
    '#suffix' => '</div>',
    '#markup' => l("Download Template", drupal_get_path('module', 'dispatch_planning') . '/full_deliver_upload_template.csv'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Upload'),
  );
  return $form;
}

function full_deliver_upload_form_validate($form, &$form_state) {
  $file = file_save_upload('file', array(
    'file_validate_extensions' => array('csv'),
  ));

  $header_array_from_xsd = array('Trip Id', 'Bill Number', 'Status', 'Reason');
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

// Blank and attached to trip validation
      $datas = array();
      if (($handle = fopen($filepath, "r")) !== FALSE) {
        $header = fgetcsv($handle, 0, ',', '"');

        while (($row = fgetcsv($handle) ) !== FALSE) {

          foreach ($row as $key => &$value) {
            $value = mb_convert_encoding(trim($value), 'UTF-8', 'UTF-8');
          }

          $datas[] = array_combine($header, $row);
        }
        fclose($handle);
      }

      global $user;
      $rs_code = dispatch_planning_get_rscode_using_uid($user->uid);
      foreach ($datas as $key => $val) {
        $bill_Number = $val['Bill Number'];
        $trip_id = $val['Trip Id'];
        $status = $val['Status'];
        $reason = $val['Reason'];
        if ($status === 'deliver' || $status === 'undeliver') {
          // do nothing
        }else{
          form_set_error('form', t("Status should be deliver or undeliver"));
        }
        if ($status == 'undeliver' && empty($reason)) {
          form_set_error('form', t("Reason cannot be empty for bill number $bill_Number"));
        }
        if ($status == 'undeliver' && !empty($reason)) {
          $query = db_select('node', 'n');
          $query->join('field_data_field_reason_type', 'rt', 'rt.entity_id = n.nid');
          $query->join('field_data_field_dp_reason', 'dr', 'dr.entity_id = n.nid');
          $query->condition('rt.field_reason_type_value', 'undeliver');
          $query->fields('dr', array('field_dp_reason_value'));
          $query->addTag('node_distributor_access');
          $res_data = $query->execute()->fetchAllKeyed(0, 0);
          foreach ($res_data as $key => $val) {
            $res_data[$key] = strtolower($val);
          }
          $reason = strtolower($reason);
          if (!in_array($reason, $res_data)) {
            form_set_error('form', t("$reason Not a valid reason for bill number $bill_Number"));
          }
        }
        $query = db_select('dispatch_planning_api_data', 'ad');
        $query->fields('ad', array('bill_number'));
        $query->condition('ad.bill_number', $bill_Number);
        $query->condition('ad.runsheet_number', $trip_id);
        $query->condition('ad.hub', $rs_code);
        $query->isNull('ad.res_status');
        $res = $query->execute()->fetchField();
        if (empty($res)) {
          form_set_error('form', t("Bill not attached to a trip or Bill status is not blank for bill number $bill_Number"));
        }
      }
// Blank and attached to trip validation         
    }
    else {
      form_set_error('file_upload', t('Failed to write the uploaded file the site\'s file folder.'));
    }
  }
  else {
    form_set_error('file', t('No file was uploaded.'));
  }
}

function full_deliver_upload_form_submit($form, &$form_state) {

  $file = $form_state['values']['file'];
  $filepath = $file->uri;

  $datas = array();

  if (($handle = fopen($filepath, "r")) !== FALSE) {
    $header = fgetcsv($handle, 0, ',', '"');

    while (($row = fgetcsv($handle) ) !== FALSE) {

      foreach ($row as $key => &$value) {
        $value = mb_convert_encoding(trim($value), 'UTF-8', 'UTF-8');
      }

      $datas[] = array_combine($header, $row);
    }
    fclose($handle);
  }

  $operations = array();
  $data_chunk = array_chunk($datas, 100);
  foreach ($data_chunk as $data) {
    $operations[] = array('full_deliver_upload_form_process', array($data));
  }
  $batch = array(
    'title' => t('Uploading Full Delivery Data Csv'),
    'operations' => $operations,
    'init_message' => t("Please wait..."),
    'progress_message' => t('Processed @current out of @total.'),
    'error_message' => t('Upload data processing has encountered an error.'),
    'finished' => 'full_deliver_upload_form_finished',
    'file' => drupal_get_path('module', 'dispatch_planning') . '/full_deliver_upload.inc',
  );
  batch_set($batch);
}

function full_deliver_upload_form_process($data, &$context) {
  update_data_in_dispatch_data_table($data);
}

function full_deliver_upload_form_finished($success, $results, $operations) {
  if ($success) {
    drupal_set_message('All Data are Uploaded successfully');
    $_GET['destination'] = "full-deliver/import";
  }
  else {
    drupal_set_message('There is Error Occured in batch process');
  }
}

function update_data_in_dispatch_data_table($data) {
  global $user;
  $rs_code = dispatch_planning_get_rscode_using_uid($user->uid);

  foreach ($data as $v) {
    $bill_number = $v['Bill Number'];
    $trip_id = $v['Trip Id'];
    $status = $v['Status'];
    $reason = $v['Reason']; //newly added. previously missed
    $status_updated = db_update('dispatch_planning_api_data') // Table name no longer needs {}
        ->fields(array(
          'res_status' => $status,
          'updated_flag' => 1, //manual
          'updated_time' => REQUEST_TIME,
          'res_reason_for_failure' => ($status == 'undeliver') ? $reason : '',
        ))
        ->condition('bill_number', $bill_number, '=')
        ->condition('runsheet_number', $trip_id, '=')
        ->condition('hub', $rs_code, '=')
        ->execute();

    $reference_no = $trip_id . '_' . $rs_code . '_' . $bill_number;
    $bill_row_id = _get_bill_row_id_using_reference_no($reference_no);
    $query = db_select('dispatch_planning_item_details', 'ad');
    $query->fields('ad', array('sku_code', 'dispatch_qty','res_cashier_short'));
    $query->condition('ad.trip_bill_reference', $bill_row_id);
    $sku_list = $query->execute()->fetchAll();
   
    
    if ($status == 'deliver') {
      foreach ($sku_list as $key => $val) {
        $sku_code = $val->sku_code;
        $dispatch_qty = $val->dispatch_qty;
        //$batch_code = $val->batch_code;
        $deliver_qty = $dispatch_qty;

        db_update('dispatch_planning_item_details') // Table name no longer needs {}
            ->fields(array(
              'res_sku_actual_quantity1' => $deliver_qty,
              'return_qty'=> 0,
              'line_level_status'=> 'line_deliver',
              'res_cashier_short' => 0,
            ))
            ->condition('trip_bill_reference', $bill_row_id, '=')
            ->condition('sku_code', $sku_code, '=')
            //->condition('batch_code', $batch_code, '=')
            ->execute();
      }
    }
    else if ($status == 'undeliver'){
     foreach ($sku_list as $key => $val) {
       $sku_code = $val->sku_code;
       $dispatch_qty = $val->dispatch_qty;
       //$batch_code = $val->batch_code;
       $res_cashier_short = $val->res_cashier_short;
       $deliver_qty = $dispatch_qty - $res_cashier_short;
     
        db_update('dispatch_planning_item_details') // Table name no longer needs {}
            ->fields(array(
              'res_sku_actual_quantity1' => 0, // deliver quantity
              'return_qty'=> $deliver_qty,
              'line_level_status'=> 'line_undeliver',
              'res_cashier_short'=> !empty($res_cashier_short)? $res_cashier_short : 0
            ))
            ->condition('trip_bill_reference', $bill_row_id, '=')
            ->condition('sku_code', $sku_code, '=')
            //->condition('batch_code', $batch_code, '=')
            ->execute();
      }
    }
  }
}
