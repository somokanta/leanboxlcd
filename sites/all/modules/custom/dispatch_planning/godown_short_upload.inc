<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function godown_short_upload_form($form, $form_state) {

  $form['file'] = array(
    '#type' => 'file',
    '#title' => t('Choose a CSV file for Godown Short data upload'),
  );

  $form['template_download'] = array(
    '#type' => 'markup',
    '#prefix' => '<div id="template_wrapper">',
    '#suffix' => '</div>',
    '#markup' => l("Download Template", drupal_get_path('module', 'dispatch_planning') . '/godown_short_upload_template.csv'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Upload'),
  );
  return $form;
}

function godown_short_upload_form_validate($form, &$form_state) {
  $file = file_save_upload('file', array(
    'file_validate_extensions' => array('csv'),
  ));

  $header_array_from_xsd = array('Trip ID', 'Bill Number', 'SKU Code', 'Godown Short Quantity');
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
      $dis_id = change_feeds_import_get_distributor_id($user->uid);
      $all_bills = array();
      foreach ($datas as $key => $val) {
        $trip_id = $val['Trip ID'];
        $bill_Number = $val['Bill Number'];
        $sku = $val['SKU Code'];
        //$batch_code = $val['Batch Code'];
        $godown_short_qty = $val['Godown Short Quantity'];

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

        if (!isset($all_bills[$trip_id])) {
          $query = db_select('node', 'n');
          $query->Join('field_data_field_trip_id_reference', 'ft', 'ft.entity_id = n.nid');
          $query->Join('node', 'n1', 'n1.nid = ft.field_trip_id_reference_nid');
          $query->condition('n.type', 'sales_register_data');
          $query->condition('n1.title', $trip_id);
          $query->fields('n', array('title'));
          $query->addTag('node_distributor_access');
          $all_bills[$trip_id] = $query->execute()->fetchAllKeyed(0, 0);
          if (empty($all_bills[$trip_id])) {
            form_set_error('form', t("Trip Id $trip_id Not exist"));
          }
          else {
            // check trip if it is Returns Confirmed
            // check trip if it is VRS Confirmed
            $trip_detail = check_trip_id_exists_or_not($dis_id, $trip_id);
            $vrs_status = $trip_detail->field_vrs_status_value;

            if($vrs_status == 1) {
              form_set_error('form', t("You cannot edit because this trip:$trip_id is returns confirmed."));
            }
            else if($vrs_status == 2) {
              form_set_error('form', t("You cannot edit because this trip:$trip_id is VRS confirmed."));
            }
            else if($vrs_status == 3) {
              form_set_error('form', t("You cannot edit because this trip:$trip_id is VRS pending."));
            }
            else if($vrs_status == 4) {
              form_set_error('form', t("You cannot edit because this trip:$trip_id is Handover pending."));
            }
            else if($vrs_status == 5) {
              form_set_error('form', t("You cannot edit because this trip:$trip_id is Handover confirmed."));
            }
          }
        }

        if (!in_array($bill_Number, $all_bills[$trip_id])) {
          form_set_error('form', t("Bill $bill_Number not attached to a trip $trip_id"));
        }

        $bill_present = check_bill_exists_or_not($dis_id, $trip_id, $bill_Number);

        if (empty($bill_present)) {

          $billing_data = get_billing_data_using_bill_no($dis_id, $bill_Number);

          foreach ($billing_data as $v) {
            db_insert('godown_data')
              ->fields(array(
                'trip_id' => $trip_id,
                'bill_number' => $bill_Number,
                'sku_code' => $v['sku7_code'],
                //'batch_code' => $v['BASEPACK_CODE'],
                'mrp' => $v['mrp'],
                'sku_desc' => $v['product_name'],
                'sum_sales_qty' => $v['Sum_of_TOTAL_SALES_QTY'],
                'distributor_id' => $dis_id,
                'created_date' => time(),
              ))
              ->execute();
          }
        }

        $query = db_select('godown_data', 'gd');
        $query->condition('gd.distributor_id', $dis_id);
        $query->condition('gd.trip_id', $trip_id);
        $query->condition('gd.bill_number', $bill_Number);
        //$query->condition('gd.batch_code', $batch_code);
        $query->condition('gd.sku_code', $sku);
        $query->fields('gd', array('id'));
        $check_sku_batch_code = $query->execute()->fetchField();


        if (empty($check_sku_batch_code)) {
          form_set_error('form', t("SKU Code doesn't exist for sku $sku"));
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

function godown_short_upload_form_submit($form, &$form_state) {

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
  $data_chunk = array_chunk($datas, 1000);
  foreach ($data_chunk as $data) {
    $operations[] = array('godown_short_upload_form_process', array($data));
  }
  $batch = array(
    'title' => t('Uploading Godown Short Data Csv'),
    'operations' => $operations,
    'init_message' => t("Please wait..."),
    'progress_message' => t('Processed @current out of @total.'),
    'error_message' => t('Upload data processing has encountered an error.'),
    'finished' => 'godown_short_upload_form_finished',
    'file' => drupal_get_path('module', 'dispatch_planning') . '/godown_short_upload.inc',
  );
  batch_set($batch);
}

function godown_short_upload_form_process($data, &$context) {
  update_data_in_dispatch_data_godown_table($data);
}

function godown_short_upload_form_finished($success, $results, $operations) {
  if ($success) {
    drupal_set_message('All Data are Uploaded successfully');
    $_GET['destination'] = "godown-short/import";
  }
  else {
    drupal_set_message('There is Error Occured in batch process');
  }
}

function update_data_in_dispatch_data_godown_table($data) {
  global $user;
  $rs_code = dispatch_planning_get_rscode_using_uid($user->uid);
  $distributor_id = change_feeds_import_get_distributor_id($user->uid);
  $farey_status_exist = 0;
  $farey_status_msg = 0;
  foreach ($data as $val) {
    $bill_number = $val['Bill Number'];
    $trip_id = $val['Trip ID'];
    //$batch_code = $val['Batch Code'];
    $sku = $val['SKU Code'];
    $godown_short_qty = $val['Godown Short Quantity'];


    db_update('godown_data') // Table name no longer needs {}
      ->fields(array(
        'godown_short' => $godown_short_qty,
        'created_date' => time()
      ))
      ->condition('trip_id', $trip_id, '=')
      ->condition('bill_number', $bill_number, '=')
      ->condition('sku_code', $sku, '=')
      //->condition('batch_code', $batch_code, '=')
      ->condition('distributor_id', $distributor_id, '=')
      ->execute();

    //newly added for sync in farey table
    $reference_no = $trip_id . '_' . $rs_code . '_' . $bill_number;
    $bill_row_id = _get_bill_row_id_using_reference_no($reference_no);
    if ($bill_row_id) {
      $godown_short_qty = $godown_short_qty ? $godown_short_qty : 0;
      $farey_status_exist = db_query("select res_status from dispatch_planning_api_data where id = $bill_row_id ")->fetchField();
      
      if (!empty($farey_status_exist)) {
        $farey_status_msg = 1;
        $query = db_select('dispatch_planning_item_details', 'ad');
        $query->fields('ad', array('sum_net_sales_quantity','res_cashier_short','return_qty'));
        $query->condition('ad.trip_bill_reference', $bill_row_id);
        $query->condition('ad.sku_code', $sku);
        //$query->condition('ad.batch_code', $batch_code, '=');
        $res_update = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
        
        $sum_net_sales = ($res_update[0]['sum_net_sales_quantity'])?$res_update[0]['sum_net_sales_quantity'] : 0;
        $cashier_short = ($res_update[0]['res_cashier_short'])?$res_update[0]['res_cashier_short'] : 0;
        $returned_qty = ($res_update[0]['return_qty'])?$res_update[0]['return_qty'] : 0;
        $dispatch_qty = $sum_net_sales - $godown_short_qty;
        
        $deliver_qty = $dispatch_qty - $returned_qty - $cashier_short;

        db_update('dispatch_planning_item_details') // Table name no longer needs {}
          ->fields(array(
            'res_godown_short' => $godown_short_qty,
            'dispatch_qty'=>$dispatch_qty,
            'res_sku_actual_quantity1' => $deliver_qty,
          ))
          ->condition('trip_bill_reference', $bill_row_id, '=')
          ->condition('sku_code', $sku, '=')
          //->condition('batch_code', $batch_code, '=')
          ->execute();
      }
      else {
        db_update('dispatch_planning_item_details') // Table name no longer needs {}
          ->fields(array(
            'res_godown_short' => $godown_short_qty
          ))
          ->expression('dispatch_qty', "sum_net_sales_quantity - $godown_short_qty")
          ->condition('trip_bill_reference', $bill_row_id, '=')
          ->condition('sku_code', $sku, '=')
          //->condition('batch_code', $batch_code, '=')
          ->execute();
      }
    }
  }
  if (!empty($farey_status_msg)) {
    drupal_set_message("Please update Return Manual Edit Screen for Trip and Bill you uploaded", 'warning');
  }
}

function check_bill_exists_or_not($dis_id, $trip_id, $bill_Number) {
  $query = db_select('godown_data', 'gd');
  $query->condition('gd.distributor_id', $dis_id);
  $query->condition('gd.trip_id', $trip_id);
  $query->condition('gd.bill_number', $bill_Number);
  $query->fields('gd', array('id'));
  $result = $query->execute()->fetchField();

  return $result;
}

function get_billing_data_using_bill_no($dis_id, $bill_Number) {
  $query = db_select('custom_billing_data', 'cbd');
  $query->condition('cbd.distributor_id', $dis_id);
  $query->condition('cbd.bill_number', $bill_Number);
  $query->fields('cbd', array('sku7_code', 'product_name', 'mrp', 'Sum_of_TOTAL_SALES_QTY'));
  $results = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

  return $results;
}
