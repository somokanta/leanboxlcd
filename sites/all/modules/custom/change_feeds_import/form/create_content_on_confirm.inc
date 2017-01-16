<?php

// when confirm button is clicked
function change_feeds_import_create_confirm_content($id) {
 
  // select data from stage table
  $billing_data_result = db_select('stage_billing_data', 'sbd')
          ->condition('sbd.error_id', $id, '=')
          ->fields('sbd')
          ->execute()->fetchAll(PDO::FETCH_ASSOC);
  
  foreach ($billing_data_result as $key => $value) {
    $vehicle = $value['vehicle'];
    $loading_date = $value['loading_date'];
    $total_qty = $value['Sum_of_TOTAL_SALES_QTY'];
    $sku = $value['sku7_code'];

    if (isset($res[$vehicle . "/" . $loading_date])) {
      $res[$vehicle . "/" . $loading_date]['sku_count'] ++;
      $res[$vehicle . "/" . $loading_date]['total_count'] += $total_qty;
      $unique_sku_string = $vehicle . "" . $loading_date . "" . $sku;
      if ($temp_unique_sku_string != $unique_sku_string) {
        $res[$vehicle . "/" . $loading_date]['unique_count'] ++;
        $temp_unique_sku_string = $vehicle . "" . $loading_date . "" . $sku;
      }
    }
    else {
      $res[$vehicle . "/" . $loading_date] = array();
      $res[$vehicle . "/" . $loading_date]['sku_count'] = 1;
      $res[$vehicle . "/" . $loading_date]['total_count'] = $total_qty;
      $temp_unique_sku_string = $vehicle . "" . $loading_date . "" . $sku;
      $res[$vehicle . "/" . $loading_date]['unique_count'] = 1;
    }

    $distint_sku_count = $res[$vehicle . "/" . $loading_date]['unique_count'];
    $sku_count = $res[$vehicle . "/" . $loading_date]['sku_count'];
    $total_qty_count = $res[$vehicle . "/" . $loading_date]['total_count'];

    $created_date = date("Y-m-d h:i:s", $value['date']);
    $party_packing_data[$vehicle . "/" . $loading_date]['VEHICLE'] = $vehicle;
    $party_packing_data[$vehicle . "/" . $loading_date]['LOADING_DATE'] = $loading_date;
    $party_packing_data[$vehicle . "/" . $loading_date]['party_packing_date'] = $created_date;
    $party_packing_data[$vehicle . "/" . $loading_date]['loading_sheet_line'] = $distint_sku_count;
    $party_packing_data[$vehicle . "/" . $loading_date]['bill_lines'] = $sku_count;
    $party_packing_data[$vehicle . "/" . $loading_date]['each_units'] = $total_qty_count;
  }

  foreach ($party_packing_data as $key => $val) {
    $party_packing_data_new[] = $val;
  }


  // 3 btach operation for creating billing , party picking and picking data
  $batch = array(
    'operations' => array(
      array('change_feeds_import_create_billing_data', array($billing_data_result))
    ),
    'finished' => 'change_feeds_import_finished',
    'title' => t('Creating Billing Data'),
    'init_message' => t('Billing Data Batch Creation is starting.'),
    'progress_message' => t('Processed 0 out of 3.'),
    'error_message' => t('Billing Data Batch has encountered an error.'),
    'file' => drupal_get_path('module', 'change_feeds_import') . '/form/create_content_on_confirm.inc'
  );
  batch_set($batch);
  $batch = array(
    'operations' => array(
      array('change_feeds_import_create_party_packing', array($party_packing_data_new))
    ),
    'finished' => 'change_feeds_import_finished',
    'title' => t('Creating Party Packing Data'),
    'init_message' => t('Billing Data Batch Creation is starting.'),
    'progress_message' => t('Processed 1 out of 3.'),
    'error_message' => t('Party Packing Data Batch has encountered an error.'),
    'file' => drupal_get_path('module', 'change_feeds_import') . '/form/create_content_on_confirm.inc'
  );
  batch_set($batch);
  $batch = array(
    'operations' => array(
      array('change_feeds_import_create_picking', array())
    ),
    'finished' => 'change_feeds_import_finished',
    'title' => t('Creating Picking Data'),
    'init_message' => t('Picking Data Batch Creation is starting.'),
    'progress_message' => t('Processed 2 out of 3.'),
    'error_message' => t('Picking Data Batch has encountered an error.'),
    'file' => drupal_get_path('module', 'change_feeds_import') . '/form/create_content_on_confirm.inc'
  );
  batch_set($batch);
  // delete data from stage_billing_data.
  db_delete('stage_billing_data')
      ->condition('error_id', $id, '=')
      ->execute();

  require_once drupal_get_path('module', 'change_feeds_import').'/form/validate_content.inc';
  $status = "Completed";
  // change status in error billing data
  change_feeds_import_update_data_in_error_billing_table($status, $id);
  $message = "Data is Created Successfully";
  drupal_set_message($message);
  $path = 'public://filter_error.csv';
  if (file_exists($path) ){
    drupal_unlink($path);//delete the generated error file
  }
  batch_process('/billing_data/import');
}

function change_feeds_import_create_picking(&$context) {
  if (!isset($context['sandbox']['progress'])) {
    global $user;
    $user_id = $user->uid;
    $distributor_id = change_feeds_import_get_distributor_id($user_id);
    $query = db_select('field_data_field_vehicle', 'v');
    $query->condition('v.bundle', 'billing_data', '=');
    $query->join('field_data_field_loading_date', 'date', 'date.entity_id = v.entity_id');
    $query->condition('date.bundle', 'billing_data', '=');
    $query->join('field_data_field_sku7_code', 'sc', 'sc.entity_id = v.entity_id');
    $query->condition('sc.bundle', 'billing_data', '=');
    $query->join('field_data_field_sku7_code', 'gaisc', 'gaisc.field_sku7_code_value = sc.field_sku7_code_value');
    $query->condition('gaisc.bundle', 'sku7_code_master', '=');
    $query->join('field_data_field_godown_area_id', 'gai', 'gaisc.entity_id = gai.entity_id');
    $query->condition('gai.bundle', 'sku7_code_master', '=');
    $query->join('field_data_field_sum_of_total_sales_qty', 'stsq', 'stsq.entity_id = v.entity_id');
    $query->condition('stsq.bundle', 'billing_data', '=');
    $query->join('field_data_field_pack_size', 'fps', 'fps.entity_id = gaisc.entity_id');
    $query->condition('fps.bundle', 'sku7_code_master', '=');
    $query->join('field_data_field_unit_case_units_', 'fucu', 'fucu.entity_id = gaisc.entity_id');
    $query->condition('fucu.bundle', 'sku7_code_master', '=');
    $query->fields('v', array('field_vehicle_value'));
    $query->fields('date', array('field_loading_date_value'));
    $query->fields('sc', array('field_sku7_code_value'));
    $query->fields('gai', array('field_godown_area_id_value'));
    $query->fields('fps', array('field_pack_size_value'));
    $query->fields('fucu', array('field_unit_case_units__value'));
    $query->addExpression('SUM(stsq.field_sum_of_total_sales_qty_value)', 'total_sales_qty');
    $query->addExpression('floor(SUM(stsq.field_sum_of_total_sales_qty_value) / field_unit_case_units__value)', 'cld');
    $query->addExpression('SUM(stsq.field_sum_of_total_sales_qty_value) - (field_unit_case_units__value * floor(SUM(stsq.field_sum_of_total_sales_qty_value) / field_unit_case_units__value))', 'eaches');
    $query->groupBy('field_vehicle_value,field_loading_date_value,field_sku7_code_value,field_godown_area_id_value');
    $subquery = db_select($query, 'middle');
    $subquery->fields('middle');
    $subquery->addExpression('COUNT(cld)', 'cld_lines');
    $subquery->addExpression('COUNT(eaches)', 'eaches_lines');
    $subquery->addExpression('SUM(cld)', 'total_cld');
    $subquery->addExpression('SUM(eaches)', 'total_eaches');
    $subquery->groupBy('field_vehicle_value,field_loading_date_value,field_godown_area_id_value');
    $results = $subquery->execute()->fetchAll();
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['current_node'] = 0;
    $context['sandbox']['max'] = count($results);
    $context['sandbox']['result'] = $results;
  }
  global $user;
  $res = $context['sandbox']['result'];

  for ($i = $context['sandbox']['progress']; $i < $context['sandbox']['progress'] + 50 && $i < $context['sandbox']['max']; $i++) {
    $key = $i;
    $val = $res[$key];
    if ($val->total_cld > 0 || $val->total_eaches > 0) {
      $node = new stdClass();
      $node->title = $val->field_vehicle_value . format_date(time());
      $node->type = "picking";
      node_object_prepare($node); // Sets some defaults. Invokes hook_prepare() and hook_node_prepare().
      $node->language = LANGUAGE_NONE; // Or e.g. 'en' if locale is enabled
      $node->uid = $user->uid;
      $node->status = 1; //(1 or 0): published or not
      $node->comment = 1; // 0 = comments disabled, 1 = read only, 2 = read/write

      $node->field_picking_van_id[$node->language][0]['value'] = $val->field_vehicle_value;
      $node->field_picking_loading_date[$node->language][0]['value'] = $val->field_loading_date_value;
      $node->field_picking_godown_area_id[$node->language][0]['value'] = $val->field_godown_area_id_value;

      $node->field_pick_units[$node->language][0]['value'] = $val->total_eaches;
      $node->field_pick_clds[$node->language][0]['value'] = $val->total_cld;
      $node->field_distributor_uid['und'][0]['value'] = $distributor_id;
      $node_copy = clone $node;
      $node_copy->is_new = true;
      if ($val->total_eaches > 0) {
        $node->field_picking_picking_type[$node->language][0]['value'] = 'Loose';
        $node->field_pick_lines[$node->language][0]['value'] = $val->eaches_lines;
        $node->field_pick_clds[$node->language][0]['value'] = 0;
        node_save($node);
      }
      if ($val->total_cld > 0) {
        $node_copy->field_picking_picking_type[$node->language][0]['value'] = 'Bulk';
        $node_copy->field_pick_lines[$node->language][0]['value'] = $val->cld_lines;
        $node_copy->field_pick_units[$node->language][0]['value'] = 0;
        node_save($node_copy);
      }
    }
  }
  $context['sandbox']['progress'] = $i;

  if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
  else {
    $context['finished'] = 1;
  }
}

function change_feeds_import_create_party_packing($party_packing_data_new, &$context) {
  if (!isset($context['sandbox']['progress'])) {
    global $user;
    $user_id = $user->uid;
    $distributor_id = change_feeds_import_get_distributor_id($user_id);
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['current_node'] = 0;
    $context['sandbox']['max'] = count($party_packing_data_new);
    $context['sandbox']['result'] = $party_packing_data_new;
  }
  $res = $context['sandbox']['result'];
  global $user;
  for ($i = $context['sandbox']['progress']; $i < $context['sandbox']['progress'] + 50 && $i < $context['sandbox']['max']; $i++) {
    $key = $i;
    $val = $res[$key];

    $loading_date = date("Y-m-d", ($val['LOADING_DATE']));
    $node = new stdClass();
    $node->title = $val['VEHICLE'] . format_date(time());
    $node->type = "party_packing";
    node_object_prepare($node); // Sets some defaults. Invokes hook_prepare() and hook_node_prepare().
    $node->language = LANGUAGE_NONE; // Or e.g. 'en' if locale is enabled
    $node->uid = $user->uid;
    $node->status = 1; //(1 or 0): published or not
    $node->comment = 1; // 0 = comments disabled, 1 = read only, 2 = read/write
    $node->field_party_packing_vehicle[$node->language][0]['value'] = $val['VEHICLE'];
    $node->field_party_packing_loading_date[$node->language][0]['value'] = $loading_date;
    $node->field_party_packing_date[$node->language][0]['value'] = $val['party_packing_date'];
    $node->field_loading_sheet_line[$node->language][0]['value'] = $val['loading_sheet_line'];
    $node->field_bill_lines[$node->language][0]['value'] = $val['bill_lines'];
    $node->field_each_units[$node->language][0]['value'] = $val['each_units'];
    $node->field_distributor_uid['und'][0]['value'] = $distributor_id ? $distributor_id : 0;
    $node = node_submit($node); // Prepare node for saving
    node_save($node);
  }
  $context['sandbox']['progress'] = $i;

  if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
  else {
    $context['finished'] = 1;
  }
}

function change_feeds_import_create_billing_data($billing_data_result, &$context) {
  global $user;
  if (!isset($context['sandbox']['progress'])) {
    $user_id = $user->uid;
    $distributor_id = change_feeds_import_get_distributor_id($user_id);
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['current_node'] = 0;
    $context['sandbox']['max'] = count($billing_data_result);
    $context['sandbox']['result'] = $billing_data_result;
  }
  $res = $context['sandbox']['result'];
  for ($i = $context['sandbox']['progress']; $i < $context['sandbox']['progress'] + 50 && $i < $context['sandbox']['max']; $i++) {
    $key = $i;
    $val = $res[$key];
    $loading_date = date("Y-m-d", ($val['loading_date']));
    $bill_date = date("Y-m-d", ($val['bill_date']));

    $node = new stdClass();
    $node->title = $val['vehicle'] . format_date(time());
    $node->type = "billing_data";
    node_object_prepare($node); // Sets some defaults. Invokes hook_prepare() and hook_node_prepare().
    $node->language = LANGUAGE_NONE; // Or e.g. 'en' if locale is enabled
    $node->uid = $user->uid;
    $node->status = 1; //(1 or 0): published or not
    $node->comment = 0; // 0 = comments disabled, 1 = read only, 2 = read/write
    $node->field_vehicle[$node->language][0]['value'] = $val['vehicle'];
    $node->field_loading_date[$node->language][0]['value'] = $loading_date;
    $node->field_party_code[$node->language][0]['value'] = $val['party_code'];
    $node->field_party_name[$node->language][0]['value'] = $val['party_name'];
    $node->field_bill_number[$node->language][0]['value'] = $val['bill_number'];
    $node->field_bill_date[$node->language][0]['value'] = $bill_date;
    $node->field_product_name[$node->language][0]['value'] = $val['product_name'];
    $node->field_sku7_code[$node->language][0]['value'] = $val['sku7_code'];
    $node->field_mrp[$node->language][0]['value'] = $val['mrp'];
    $node->field_tur[$node->language][0]['value'] = $val['tur'];
    $node->field_sum_of_total_sales_qty[$node->language][0]['value'] = $val['Sum_of_TOTAL_SALES_QTY'];
    $node->field_sum_of_total_sales_ret_qty[$node->language][0]['value'] = $val['Sum_of_TOTAL_SALES_RET_QTY'];
    $node->field_sum_of_net_sales_qty[$node->language][0]['value'] = $val['Sum_of_NET_SALES_QTY'];
    $node->field_sum_of_sales_free_qty[$node->language][0]['value'] = $val['Sum_of_SALES_FREE_QTY'];
    $node->field_sum_of_sales_ret_free_qty[$node->language][0]['value'] = $val['Sum_of_SALES_RET_FREE_QTY'];
    $node->field_sum_of_sales_value[$node->language][0]['value'] = $val['Sum_of_SALES_VALUE'];
    $node->field_sum_of_sales_ret_value[$node->language][0]['value'] = $val['Sum_of_SALES_RET_VALUE'];
    $node->field_sum_of_net_sales_value[$node->language][0]['value'] = $val['Sum_of_NET_SALES_VALUE'];
    $node = node_submit($node); // Prepare node for saving
    node_save($node);
  }
  $context['sandbox']['progress'] = $i;

  if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
  else {
    $context['finished'] = 1;
  }
}
