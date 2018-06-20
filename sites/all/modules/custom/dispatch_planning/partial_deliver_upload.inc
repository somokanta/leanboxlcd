<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function partial_deliver_upload_form($form, $form_state) {

  $form['file'] = array(
    '#type' => 'file',
    '#title' => t('Choose a CSV file for Partial Delivered data upload'),
  );
  $form['template_download'] = array(
    '#type' => 'markup',
    '#prefix' => '<div id="template_wrapper">',
    '#suffix' => '</div>',
    '#markup' => l("Download Template", drupal_get_path('module', 'dispatch_planning') . '/partial_deliver_upload_template.csv'),
  );

  $res_data_new = _get_partial_reason_result();
  $text = "<br>Please enter the below values in Reason field:";

  $form['title'] = array(
    '#markup' => $text,
  );
  $form['final_text'] = array(
    '#theme' => 'item_list',
    '#items' => $res_data_new,
  );


  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Upload'),
  );
  return $form;
}

function partial_deliver_upload_form_validate($form, &$form_state) {
  $file = file_save_upload('file', array(
    'file_validate_extensions' => array('csv'),
  ));

  $header_array_from_xsd = array('Trip Id', 'Bill Number', 'SKU Code', 'Returned Quantity', 'Cashier Short', 'Reason');
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
        $reason = $val['Reason'];
        $sku = $val['SKU Code'];
        //$batch_code = $val['Batch Code'];
        $returned_qty = $val['Returned Quantity'];
        $cashier_short = $val['Cashier Short'];
//        if ($status == 'undeliver' && empty($reason)) {
//          form_set_error('form', t("Reason cannot be empty for bill number $bill_Number"));
//        }
        if (empty($trip_id)) {
          form_set_error('form', t("Trip Id cannot be empty for sku $sku"));
        }
        if (empty($bill_Number)) {
          form_set_error('form', t("Bill Number cannot be empty for sku $sku"));
        }
        if (empty($sku)) {
          form_set_error('form', t("Sku cannot be empty for Batch Code $batch_code"));
        }
//        if (empty($batch_code)) {
//          form_set_error('form', t("Batch Code cannot be empty for sku $sku"));
//        }
//        if (empty($actual_qty) && $actual_qty !=="0") {
//          form_set_error('form', t("Actual Delivered quantity cannot be empty for sku $sku"));
//        }

            if (!empty($returned_qty) || !empty($cashier_short)) {
               $res_data_reason = _get_partial_reason_result();
               foreach ($res_data_reason as $key => $val) {
                  $res_data_reason[$key] = strtolower($val);
               }
               $reason = strtolower($reason);
               if (!in_array($reason, $res_data_reason)) {
                  form_set_error('form', t("$reason Not a valid reason for sku $sku"));
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
          form_set_error('form', t("Bill not attached to a trip or Bill status is not blank for sku $sku"));
        }
        // Blank and attached to trip validation

        $reference_no = $trip_id . '_' . $rs_code . '_' . $bill_Number;
        $bill_row_id = _get_bill_row_id_using_reference_no($reference_no);

        $query1 = db_select('dispatch_planning_item_details', 'it');
        $query1->fields('it', array('dispatch_qty'));
        $query1->condition('it.trip_bill_reference', $bill_row_id);
        $query1->condition('it.sku_code', $sku);
        //$query1->condition('it.batch_code', $batch_code);
        $res1 = $query1->execute()->fetchField();
        if (empty($res1) && $res1 !== "0") {
          form_set_error('form', t("SKU Code doesn't exist for sku $sku"));
        }
        if ($res1 < $returned_qty) {
          form_set_error('form', t("Entered Quantity should be less then dispatch quantity for sku $sku"));
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

function partial_deliver_upload_form_submit($form, &$form_state) {

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
    $operations[] = array('partial_deliver_upload_form_process', array($data));
  }
  $batch = array(
    'title' => t('Uploading Partial Delivery Data Csv'),
    'operations' => $operations,
    'init_message' => t("Please wait..."),
    'progress_message' => t('Processed @current out of @total.'),
    'error_message' => t('Upload data processing has encountered an error.'),
    'finished' => 'partial_deliver_upload_form_finished',
    'file' => drupal_get_path('module', 'dispatch_planning') . '/partial_deliver_upload.inc',
  );
  batch_set($batch);
}

function partial_deliver_upload_form_process($data, &$context) {
  update_data_in_dispatch_data_partial_table($data);
}

function partial_deliver_upload_form_finished($success, $results, $operations) {
  if ($success) {
    drupal_set_message('All Data are Uploaded successfully');
    $_GET['destination'] = "partial-deliver/import";
  }
  else {
    drupal_set_message('There is Error Occured in batch process');
  }
}

function update_data_in_dispatch_data_partial_table($data) {
  global $user;
  $rs_code = dispatch_planning_get_rscode_using_uid($user->uid);

  $unique_bills_array = array();
  foreach ($data as $val) {
    $bill_number = $val['Bill Number'];
    $trip_id = $val['Trip Id'];
    $unique_bills_array[$trip_id][$bill_number] = $bill_number; //update status for unique bills
    $status = $val['Status'];
    //$batch_code = $val['Batch Code'];
    $reason = $val['Reason'];
    $cashier_short = $val['Cashier Short'];
    $returned_qty = $val['Returned Quantity'];
    $sku = $val['SKU Code'];
    $returned_qty = $returned_qty ? $returned_qty : 0;
    $reference_no = $trip_id . '_' . $rs_code . '_' . $bill_number;
    $bill_row_id = _get_bill_row_id_using_reference_no($reference_no);

    $query = db_select('dispatch_planning_item_details', 'ad');
    $query->fields('ad', array('dispatch_qty','res_godown_short'));
    $query->condition('ad.trip_bill_reference', $bill_row_id);
    $query->condition('ad.sku_code', $sku);
    //$query->condition('ad.batch_code', $batch_code, '=');
    $data = $query->execute()->fetch(PDO::FETCH_ASSOC);
    
    $dispatch_qty = $data['dispatch_qty'];
    $godown_short = $data['res_godown_short'];
    $deliver_qty = $dispatch_qty - $returned_qty - $cashier_short;
    
    $ret_cash = $returned_qty + $cashier_short; //Return Qty + Cashier Short  
    $ret_cash_godown = $ret_cash + $godown_short; //Return Qty + Cashier Short + Godown Short
    $line_status = '';
    if ($ret_cash == $dispatch_qty) {
      $line_status = 'line_undeliver';
    }else if ($ret_cash_godown > 0) {
      $line_status = 'line_partial_deliver';
    }else if ($ret_cash_godown == 0) {
      $line_status = 'line_deliver';
    }
    
      db_update('dispatch_planning_item_details') // Table name no longer needs {}
          ->fields(array(
            'res_sku_actual_quantity1' => $deliver_qty,
            'res_sku_reason' => $reason,
            'res_cashier_short' => $cashier_short,
            'line_level_status'=> $line_status,
            'return_qty' => $returned_qty,
          ))
          ->condition('trip_bill_reference', $bill_row_id, '=')
          ->condition('sku_code', $sku, '=')
          //->condition('batch_code', $batch_code, '=')
          ->execute();
  }

  foreach ($unique_bills_array as $key => $value) {
    foreach ($value as $v) {
      db_update('dispatch_planning_api_data') // Table name no longer needs {}
          ->fields(array(
            'res_status' => 'partial_deliver',
            'updated_flag' => 1, //manual
            'updated_time' => REQUEST_TIME,  
          ))
          ->condition('bill_number', $v, '=')
          ->condition('runsheet_number', $key, '=')
          ->condition('hub', $rs_code, '=')
          ->execute();
    }
  }
}

function _get_partial_reason_result() {
  $query = db_select('node', 'n');
  $query->join('field_data_field_reason_type', 'rt', 'rt.entity_id = n.nid');
  $query->join('field_data_field_dp_reason', 'dr', 'dr.entity_id = n.nid');
  $query->condition('rt.field_reason_type_value', 'partial_deliver');
  $query->fields('dr', array('field_dp_reason_value'));
  $query->addTag('node_distributor_access');
  $res_data = $query->execute()->fetchAllKeyed(0, 0);
  return $res_data;
}
