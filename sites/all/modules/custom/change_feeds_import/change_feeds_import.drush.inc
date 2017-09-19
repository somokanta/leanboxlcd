<?php

module_load_include('inc', 'change_feeds_import', 'form/upload_billing_data');
module_load_include('inc', 'change_feeds_import', 'form/create_content_on_confirm');

// when confirm button is clicked
function change_feeds_import_create_confirm_content_test($id) {
  dpm("hello1111");
  global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);
  $csv_file = $distributor_id . "_error.csv";
  // select data from stage table
  $billing_data_result = db_select('stage_billing_data', 'sbd')
          ->condition('sbd.error_id', $id, '=')
          ->fields('sbd')
          ->execute()->fetchAll(PDO::FETCH_ASSOC);



  $party_packing_check = get_party_packing_from_party_code(NULL, $distributor_id);

  foreach ($billing_data_result as $key => $value) {
    $vehicle = $value['vehicle'];
    $loading_date = strtotime($value['loading_date']);
    $total_qty = $value['Sum_of_TOTAL_SALES_QTY'];
    $sku = $value['sku7_code'];
    $bill_number = $value['bill_number'];
    $pricelist_group = $value['PRICELIST_GROUP'];
    if (isset($res[$vehicle . "/" . $loading_date])) {
      $total_sku = $res[$vehicle . "/" . $loading_date]['sku_count'] ++;

      if ($pricelist_group == "PP") {
        $group++;
        if (($group / $res[$vehicle . "/" . $loading_date]['sku_count']) * 100 >= 90) {
          $group_party_packing = 1;
        }
        else {
          $group_party_packing = 0;
        }
      }
      if (isset($party_packing_check[$value['party_code']]) ? $party_packing_check[$value['party_code']] : 0 == 1) {
        $check++;
        if (($check / $res[$vehicle . "/" . $loading_date]['sku_count']) * 100 >= 90) {
          $party_packing = 1;
        }
        else {
          $party_packing = 0;
        }
      }

      $res[$vehicle . "/" . $loading_date]['total_count'] += $total_qty;
      $unique_sku_string = $vehicle . "" . $loading_date . "" . $sku;
      if (!in_array("$unique_sku_string", $temp_unique_sku_string)) {
        $res[$vehicle . "/" . $loading_date]['unique_count'] ++;
        $temp_unique_sku_string[] = $vehicle . "" . $loading_date . "" . $sku;
      }
    }
    else {
      $res[$vehicle . "/" . $loading_date] = array();
      $res[$vehicle . "/" . $loading_date]['sku_count'] = 1;
      $res[$vehicle . "/" . $loading_date]['total_count'] = $total_qty;
      $temp_unique_sku_string[] = $vehicle . "" . $loading_date . "" . $sku;
      $res[$vehicle . "/" . $loading_date]['unique_count'] = 1;
      if (isset($party_packing_check[$value['party_code']]) ? $party_packing_check[$value['party_code']] : 0 == 1) {
        $party_packing = 1;
        $check = 1;
      }
      else {
        $party_packing = 0;
      }

      if ($pricelist_group == "PP") {
        $group = 1;
        $group_party_packing = 1;
      }
      else {
        $group_party_packing = 0;
      }
    }

    $distint_sku_count = $res[$vehicle . "/" . $loading_date]['unique_count'];
    $sku_count = $res[$vehicle . "/" . $loading_date]['sku_count'];
    $total_qty_count = $res[$vehicle . "/" . $loading_date]['total_count'];

    $created_date = date("Y-m-d h:i:s", $value['date']);
    $party_packing_data[$vehicle . "/" . $loading_date]['VEHICLE'] = $vehicle;
    $party_packing_data[$vehicle . "/" . $loading_date]['LOADING_DATE'] = $loading_date;
    //$party_packing_data[$vehicle . "/" . $loading_date]['party_packing_date'] = $created_date;
    $party_packing_data[$vehicle . "/" . $loading_date]['loading_sheet_line'] = $distint_sku_count;
    $party_packing_data[$vehicle . "/" . $loading_date]['bill_lines'] = $sku_count;
    $party_packing_data[$vehicle . "/" . $loading_date]['each_units'] = $total_qty_count;
    $party_packing_data[$vehicle . "/" . $loading_date]['party_check'] = $party_packing;
    $party_packing_data[$vehicle . "/" . $loading_date]['party_pack'] = $group_party_packing;
  }

  foreach ($party_packing_data as $key => $val) {
    $party_packing_data_new[] = $val;
  }


  $query = db_select('custom_billing_data', 'cb');
  $query->addExpression('MAX(id)', 'max');
  $query->condition('distributor_id', $distributor_id);
  $max_billing_id = $query->execute()->fetchField();

  $max_billing_id = $max_billing_id ? $max_billing_id : 0;

  change_feeds_import_insert_data_in_custom_billing_table($billing_data_result);
  $operations = array();
  
  $create_billing_data = change_feeds_import_create_billing_data($max_billing_id);
  dpm($create_billing_data);
  $create_billing_data_chunk = array_chunk($create_billing_data, 1000);
  foreach ($create_billing_data_chunk as $data) {
    $operations[] = array('change_feeds_import_create_billing', array($data));
  }

  $create_picking_data = change_feeds_import_create_picking_data($max_billing_id);
  dpm($create_picking_data);
  $create_picking_data_chunk = array_chunk($create_picking_data, 1000);
  foreach ($create_picking_data_chunk as $data) {
    $operations[] = array('change_feeds_import_create_picking', array($data));
  }

  $van_cld_report_data = change_feeds_import_create_van_cld_report_data($max_billing_id);
  dpm($van_cld_report_data);
  $van_cld_report_data_chunk = array_chunk($van_cld_report_data, 1000);
  foreach ($van_cld_report_data_chunk as $data) {
    $operations[] = array('change_feeds_import_create_van_cld_report', array($data));
  }
  
  $party_packing_data_new_chunk = array_chunk($party_packing_data_new, 1000);
  dpm($party_packing_data_new_chunk);
  foreach ($party_packing_data_new_chunk as $data) {
    $operations[] = array('change_feeds_import_create_party_packing', array($data));
  }
 
  $batch = array(
    'operations' => $operations,
    'finished' => 'change_feeds_import_finished',
    'title' => t('Creating Billing Data'),
    'init_message' => t('Billing Data Batch Creation is starting.'),
    'progress_message' => t('Processed @current out of @total.'),
    'error_message' => t('Billing Data Batch has encountered an error.'),
    'file' => drupal_get_path('module', 'change_feeds_import') . '/form/create_content_on_confirm.inc'
  );
  batch_set($batch);

  //delete data from stage_billing_data.
  db_delete('stage_billing_data')
      ->condition('error_id', $id, '=')
      ->execute();
 dpm("deleted");
  require_once drupal_get_path('module', 'change_feeds_import') . '/form/validate_content.inc';
  $status = "Completed";
  // change status in error billing data
  change_feeds_import_update_data_in_error_billing_table($status, $id);

  $path = "public://billing_data/$csv_file";
  if (file_exists($path)) {
    drupal_unlink($path); //delete the generated error file
  }
  dpm("batch set");
  
  $batch =& batch_get();
  $batch['progressive'] = FALSE;
  //batch_process('/billing_data/import');
  //background_batch_process_batch('/billing_data/import');
  drush_backend_batch_process();
  dpm("batch end");
}
