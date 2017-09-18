<?php

module_load_include('inc', 'change_feeds_import', 'form/upload_billing_data');

// when confirm button is clicked
function change_feeds_import_create_confirm_content($id) {
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
  batch_process('/billing_data/import');
  dpm("batch end");
  //background_batch_process_batch('/billing_data/import');
}

function change_feeds_import_create_billing_data($max_billing_id) {
  global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);
  $query = db_select('custom_billing_data', 'cb');

  $query->addField('cb', 'bill_number');
  $query->addField('cb', 'vehicle', 'vehicle');
  $query->addField('cb', 'loading_date', 'loading_date');
  $query->addField('cb', 'bill_date', 'bill_date');
  $query->addExpression('SUM(cb.Sum_of_NET_SALES_VALUE)', 'sum_net_sales_value');

  $query->condition('cb.distributor_id', $distributor_id);
  $query->condition('cb.id', $max_billing_id, '>');

  $query->groupBy('cb.bill_number');
  $results = $query->execute()->fetchAll();
  return $results;
}

function change_feeds_import_create_picking_data($max_billing_id) {
  global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);
  $query = db_select('custom_billing_data', 'cb');
  $query->join('field_data_field_sku7_code', 'gaisc', 'gaisc.field_sku7_code_value = cb.sku7_code');
  $query->condition('gaisc.bundle', 'sku7_code_master', '=');
  $query->join('node', 'nd', 'nd.nid = gaisc.entity_id');
  $query->condition('nd.uid', $distributor_id, '=');
//    $query->join('field_data_field_godown_area_id', 'gai', 'gaisc.entity_id = gai.entity_id');
//    $query->condition('gai.bundle', 'sku7_code_master', '=');
//    $query->join('field_data_field_pack_size', 'fps', 'fps.entity_id = gaisc.entity_id');
//    $query->condition('fps.bundle', 'sku7_code_master', '=');
  $query->join('field_data_field_unit_case_units_', 'fucu', 'fucu.entity_id = gaisc.entity_id');
  $query->condition('fucu.bundle', 'sku7_code_master', '=');
  $query->fields('cb', array('sku7_code'));
  $query->addField('cb', 'vehicle', 'field_vehicle_value');
  $query->addField('cb', 'loading_date', 'field_loading_date_value');
  $query->addField('cb', 'PRICELIST_GROUP', 'PRICELIST_GROUP');
  //$query->fields('gai', array('field_godown_area_id_value'));
  //$query->fields('fps', array('field_pack_size_value'));
  $query->fields('fucu', array('field_unit_case_units__value'));
  $query->addExpression('SUM(cb.Sum_of_TOTAL_SALES_QTY)', 'total_sales_qty');
  $query->addExpression('floor(SUM(cb.Sum_of_TOTAL_SALES_QTY) / field_unit_case_units__value)', 'cld');
  $query->addExpression('SUM(cb.Sum_of_TOTAL_SALES_QTY) - (field_unit_case_units__value * floor(SUM(cb.Sum_of_TOTAL_SALES_QTY) / field_unit_case_units__value))', 'eaches');

  $query->condition('cb.distributor_id', $distributor_id);
  $query->condition('cb.id', $max_billing_id, '>');

  $query->groupBy('field_vehicle_value,field_loading_date_value,sku7_code,PRICELIST_GROUP');
  $subquery = db_select($query, 'middle');
  $subquery->fields('middle');
  //$subquery->addExpression('COUNT(cld)', 'cld_lines');
  //$subquery->addExpression('COUNT(eaches)', 'eaches_lines');

  $subquery->addExpression('SUM(if(cld > 0, 1, 0))', 'cld_lines');
  $subquery->addExpression('SUM(if(eaches > 0, 1, 0))', 'eaches_lines');

  $subquery->addExpression('SUM(cld)', 'total_cld');
  $subquery->addExpression('SUM(eaches)', 'total_eaches');
  $subquery->groupBy('field_vehicle_value,field_loading_date_value,PRICELIST_GROUP');
  $results = $subquery->execute()->fetchAll();
  return $results;
}

function change_feeds_import_create_van_cld_report_data($max_billing_id) {
  global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);

  $query = db_select('custom_billing_data', 'cb');
  $query->join('field_data_field_sku7_code', 'gaisc', 'gaisc.field_sku7_code_value = cb.sku7_code');
  $query->condition('gaisc.bundle', 'sku7_code_master', '=');
  $query->join('node', 'nd', 'nd.nid = gaisc.entity_id');
  $query->condition('nd.uid', $distributor_id, '=');
  $query->join('field_data_field_unit_case_units_', 'fucu', 'fucu.entity_id = gaisc.entity_id');
  $query->condition('fucu.bundle', 'sku7_code_master', '=');
  $query->fields('cb', array('vehicle', 'loading_date', 'sku7_code', 'product_name', 'Sum_of_TOTAL_SALES_QTY', 'mrp', 'PRICELIST_GROUP'));
  $query->fields('fucu', array('field_unit_case_units__value'));
  $query->condition('cb.distributor_id', $distributor_id);
  $query->condition('cb.id', $max_billing_id, '>');
  $query->addExpression('floor(SUM(Sum_of_TOTAL_SALES_QTY) / field_unit_case_units__value)', 'cld');
  $query->addExpression('SUM(cb.Sum_of_TOTAL_SALES_QTY) - (field_unit_case_units__value * floor(SUM(Sum_of_TOTAL_SALES_QTY) / field_unit_case_units__value))', 'eaches');
  $query->groupBy('vehicle,loading_date,sku7_code');
  $results = $query->execute()->fetchAll();

  return $results;
}

function change_feeds_import_finished($success, $results, $operations) {
  drupal_set_message("Successfully Created Content");
}

function change_feeds_import_insert_data_in_custom_billing_table($billing_data_result) {
  $created_date = time();
  global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);

  /* $query = db_select('node', 'n');
    $query->condition('n.uid', $distributor_id);
    $query->condition('n.type', 'pdd_master');
    $query->fields('n', array('nid'));
    $pdd_value_result = $query->execute()->fetchCol();
    $pdd_data = node_load($pdd_value_result[0]);
    $pdd_value = $pdd_data->field_pdd[LANGUAGE_NONE][0]['value'];
    $weekly = $pdd_data->field_day[LANGUAGE_NONE];
    foreach ($weekly as $key => $val) {
    $weeklyoff[] = $val['value'];
    }

    $query = db_select('node', 'n');
    $query->condition('n.uid', $distributor_id);
    $query->condition('n.type', 'holiday_master');
    $query->leftjoin('field_data_field_holiday', 'hday', 'hday.entity_id = n.nid');
    $query->addExpression("DATE_FORMAT(FROM_UNIXTIME(hday.field_holiday_value), '%Y-%m-%d')", 'hdate');
    $holiday_result = $query->execute()->fetchCol();
   */

  foreach ($billing_data_result as $key => $data) {
//    $billing_date_stamp = $data['bill_date'];
//    $pdd = holiday_get_working_day($billing_date_stamp, $pdd_value, $holiday_result, $weeklyoff);
// 
    $loading_date = strtotime($data['loading_date']);
    $billing_date = strtotime($data['bill_date']);
    try {
      $stage_billing_data = db_insert('custom_billing_data') // Table name no longer needs {}
          ->fields(array(
            'vehicle' => $data['vehicle'],
            'loading_date' => $loading_date,
            'party_code' => $data['party_code'],
            'BEAT_NAME' => $data['BEAT_NAME'],
            'bill_number' => $data['bill_number'],
            'bill_date' => $billing_date,
            'BASEPACK_CODE' => $data['BASEPACK_CODE'],
            'product_name' => $data['product_name'],
            'sku7_code' => $data['sku7_code'],
            'PRICELIST_GROUP' => $data['PRICELIST_GROUP'],
            'PRODUCT_DIVISION' => $data['PRODUCT_DIVISION'],
            'PRODUCT_CATEGORY' => $data['PRODUCT_CATEGORY'],
            'PRODUCT_SUB_CATEGORY' => $data['PRODUCT_SUB_CATEGORY'],
            'mrp' => $data['mrp'],
            'tur' => $data['tur'],
            'Sum_of_TOTAL_SALES_QTY' => $data['Sum_of_TOTAL_SALES_QTY'],
            //  'Sum_of_TOTAL_SALES_RET_QTY' => $data['Sum_of_TOTAL_SALES_RET_QTY'],
            'Sum_of_NET_SALES_QTY' => $data['Sum_of_NET_SALES_QTY'],
            'Sum_of_SALES_FREE_QTY' => $data['Sum_of_SALES_FREE_QTY'],
            //  'Sum_of_SALES_RET_FREE_QTY' => $data['Sum_of_SALES_RET_FREE_QTY'],
            //  'Sum_of_SALES_VALUE' => $data['Sum_of_SALES_VALUE'],
            //  'Sum_of_SALES_RET_VALUE' => $data['Sum_of_SALES_RET_VALUE'],
            'Sum_of_NET_SALES_VALUE' => $data['Sum_of_NET_SALES_VALUE'],
            'Sum_of_NET_SALES_CASES' => $data['Sum_of_NET_SALES_CASES'],
            'Sum_of_NET_SALES_KGS' => $data['Sum_of_NET_SALES_KGS'],
            'Sum_of_SCHEME_DISCOUNT' => $data['Sum_of_SCHEME_DISCOUNT'],
            'Sum_of_OTHER_DISCOUNT' => $data['Sum_of_OTHER_DISCOUNT'],
            'Sum_of_TAX_PERCENTAGE' => $data['Sum_of_TAX_PERCENTAGE'],
            //'promise_date'=> $pdd,
            'distributor_id' => $distributor_id,
            'created_date' => $created_date,
          ))
          ->execute();
    }
    catch (Exception $ex) {
      
    }
  }
}

function change_feeds_import_create_van_cld_report($data, &$context) {
  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['progress'] = 0;
  }
  global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);
  foreach ($data as $key => &$value) {
    $vehicle = $value->vehicle;
    $loading_date = $value->loading_date;
    $sku7_code = $value->sku7_code;
    $product_name = $value->product_name;
    $cld = $value->cld;
    $mrp = $value->mrp;
    $pricelist_group = $value->PRICELIST_GROUP;


    if ($cld != 0) {
      $stage_billing_data = db_insert('van_cld_data_table') // Table name no longer needs {}
          ->fields(array(
            'vehicle' => $vehicle,
            'loading_date' => $loading_date,
            'sku7_code' => $sku7_code,
            'product_name' => $product_name,
            'distributor_id' => $distributor_id,
            'cld' => $cld,
            'mrp' => $mrp,
            'pricelist_group' => $pricelist_group,
          ))
          ->execute();
    }
  }
  drupal_set_message("Created Van Cld");
}

function change_feeds_import_create_picking($data, &$context) {
  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['progress'] = 0;
  }
  global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);
  foreach ($data as $key => &$value) {
    if ($value->total_cld > 0 || $value->total_eaches > 0) {
      $node = new stdClass();
      $node->title = $value->field_vehicle_value . format_date(time());
      $node->type = "picking";
      node_object_prepare($node); // Sets some defaults. Invokes hook_prepare() and hook_node_prepare().
      $node->language = LANGUAGE_NONE; // Or e.g. 'en' if locale is enabled
      $node->uid = $user->uid;
      $node->status = 1; //(1 or 0): published or not
      $node->comment = 1; // 0 = comments disabled, 1 = read only, 2 = read/write

      $node->field_picking_van_id[$node->language][0]['value'] = $value->field_vehicle_value;
      $node->field_picking_loading_date[$node->language][0]['value'] = date("Y-m-d H:i:s", $value->field_loading_date_value);
      $node->field_picking_godown_area_id[$node->language][0]['value'] = $value->PRICELIST_GROUP;

      $node->field_pick_units[$node->language][0]['value'] = $value->total_eaches;
      $node->field_pick_clds[$node->language][0]['value'] = $value->total_cld;
      $node->field_distributor_uid['und'][0]['value'] = $distributor_id;
      $node_copy = clone $node;
      $node_copy->is_new = true;
      if ($value->total_eaches > 0) {
        $node->field_picking_picking_type[$node->language][0]['value'] = 'Loose';
        $node->field_pick_lines[$node->language][0]['value'] = $value->eaches_lines;
        $node->field_pick_clds[$node->language][0]['value'] = 0;
        node_save($node);
      }
      if ($value->total_cld > 0) {
        $node_copy->field_picking_picking_type[$node->language][0]['value'] = 'Bulk';
        $node_copy->field_pick_lines[$node->language][0]['value'] = $value->cld_lines;
        $node_copy->field_pick_units[$node->language][0]['value'] = 0;
        node_save($node_copy);
      }
    }
  }
  drupal_set_message("Created Picking");
}

function change_feeds_import_create_party_packing($data, &$context) {
  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['progress'] = 0;
  }
    global $user;
    $user_id = $user->uid;
    $distributor_id = change_feeds_import_get_distributor_id($user_id);
  
  foreach ($data as $key => &$value) {
    $loading_date = date("Y-m-d", ($value['LOADING_DATE']));
    $node = new stdClass();
    $node->title = $value['VEHICLE'] . format_date(time());
    $node->type = "party_packing";
    node_object_prepare($node); // Sets some defaults. Invokes hook_prepare() and hook_node_prepare().
    $node->language = LANGUAGE_NONE; // Or e.g. 'en' if locale is enabled
    $node->uid = $user->uid;
    $node->status = 1; //(1 or 0): published or not
    $node->comment = 1; // 0 = comments disabled, 1 = read only, 2 = read/write
    $node->field_party_packing_vehicle[$node->language][0]['value'] = $value['VEHICLE'];
    $node->field_party_packing_loading_date[$node->language][0]['value'] = $loading_date;
    //$node->field_party_packing_date[$node->language][0]['value'] = $value['party_packing_date'];
    $node->field_loading_sheet_line[$node->language][0]['value'] = $value['loading_sheet_line'];
    $node->field_bill_lines[$node->language][0]['value'] = $value['bill_lines'];
    $node->field_each_units[$node->language][0]['value'] = $value['each_units'];
    $node->field_distributor_uid['und'][0]['value'] = $distributor_id ? $distributor_id : 0;
    $node->field_party_packing_check['und'][0]['value'] = $value['party_check'];
    $node->field_party_packing_type['und'][0]['value'] = $value['party_pack'];
    $node = node_submit($node); // Prepare node for saving
    node_save($node);
  }
  
  drupal_set_message("Created Party Packing");
 
}

############## IN PROGRESS FUNCTION ##################

function change_feeds_import_create_loading(&$context) {
  if (!isset($context['sandbox']['progress'])) {
    global $user;
    $user_id = $user->uid;
    $distributor_id = change_feeds_import_get_distributor_id($user_id);
    $query = db_select('custom_billing_data', 'cb');
    $query->join('field_data_field_sku7_code', 'gaisc', 'gaisc.field_sku7_code_value = cb.sku7_code');
    $query->condition('gaisc.bundle', 'sku7_code_master', '=');
    $query->join('field_data_field_godown_area_id', 'gai', 'gaisc.entity_id = gai.entity_id');
    $query->condition('gai.bundle', 'sku7_code_master', '=');
    $query->join('field_data_field_pack_size', 'fps', 'fps.entity_id = gaisc.entity_id');
    $query->condition('fps.bundle', 'sku7_code_master', '=');
    $query->join('field_data_field_unit_case_units_', 'fucu', 'fucu.entity_id = gaisc.entity_id');
    $query->condition('fucu.bundle', 'sku7_code_master', '=');
    $query->fields('cb', array('vehicle', 'loading_date', 'sku7_code'));
    $query->fields('gai', array('field_godown_area_id_value'));
    $query->fields('fps', array('field_pack_size_value'));
    $query->fields('fucu', array('field_unit_case_units__value'));
    //$query->addExpression('SUM(cb.Sum_of_TOTAL_SALES_QTY)', 'total_sales_qty');
    $query->addExpression('floor(SUM(cb.Sum_of_TOTAL_SALES_QTY) / field_unit_case_units__value)', 'cld');
    $query->addExpression('SUM(cb.Sum_of_TOTAL_SALES_QTY) - (field_unit_case_units__value * floor(SUM(cb.Sum_of_TOTAL_SALES_QTY) / field_unit_case_units__value))', 'eaches');
    $query->groupBy('cb.bill_number');

    $subquery = db_select($query, 'middle');
    $subquery->fields('middle');
    $subquery->addExpression('COUNT(cld)', 'cld_lines');
    $subquery->addExpression('COUNT(eaches)', 'eaches_lines');
    $subquery->addExpression('SUM(cld)', 'total_cld');
    $subquery->addExpression('SUM(eaches)', 'total_eaches');
    $subquery->groupBy('vehicle,loading_date,field_godown_area_id_value');
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
      $node->type = "loading";
      node_object_prepare($node); // Sets some defaults. Invokes hook_prepare() and hook_node_prepare().
      $node->language = LANGUAGE_NONE; // Or e.g. 'en' if locale is enabled
      $node->uid = $user->uid;
      $node->status = 1; //(1 or 0): published or not
      $node->comment = 1; // 0 = comments disabled, 1 = read only, 2 = read/write

      $node->field_loading_van_id[$node->language][0]['value'] = $val->field_vehicle_value;
      //$node->field_picking_loading_date[$node->language][0]['value'] = $val->field_loading_date_value;
      //$node->field_picking_godown_area_id[$node->language][0]['value'] = $val->field_godown_area_id_value;
      //$node->field_pick_units[$node->language][0]['value'] = $val->total_eaches;
      //$node->field_pick_clds[$node->language][0]['value'] = $val->total_cld;
      $node->field_loading_distributor_uid['und'][0]['value'] = $distributor_id;
      $node_copy = clone $node;
      $node_copy->is_new = true;
      if ($val->total_eaches > 0) {
        $node->field_loading_picking_type[$node->language][0]['value'] = 'Loose';
        $node->field_loading_lines[$node->language][0]['value'] = $val->eaches_lines;
        $node->field_pick_clds[$node->language][0]['value'] = 0;
        node_save($node);
      }
      if ($val->total_cld > 0) {
        $node_copy->field_loading_picking_type[$node->language][0]['value'] = 'Bulk';
        $node_copy->field_loading_lines[$node->language][0]['value'] = $val->cld_lines;
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

function get_party_packing_from_party_code($party_code = NULL, $distributor_id) {
  $query = db_select('field_data_field_party_code', 'pc');
  $query->condition('pc.bundle', 'outlet_master');
  $query->join('node', 'ouln', 'ouln.nid = pc.entity_id');
  $query->condition('ouln.uid', $distributor_id, '=');
  $query->join('field_data_field_channel', 'ch', 'ch.entity_id = pc.entity_id');
  $query->condition('ch.bundle', 'outlet_master');
  $query->join('field_data_field_pl_group', 'pl', 'pl.entity_id = pc.entity_id');
  $query->condition('pl.bundle', 'outlet_master');
  $query->join('field_data_field_channel', 'ppmch', 'ch.field_channel_value = ppmch.field_channel_value');
  $query->condition('ppmch.bundle', 'party_pack_master');
  $query->join('node', 'ppn', 'ppn.nid = ppmch.entity_id');
  $query->condition('ppn.uid', $distributor_id, '=');
  $query->join('field_data_field_pl_group', 'ppmpl', 'pl.field_pl_group_value = ppmpl.field_pl_group_value and ppmch.entity_id = ppmpl.entity_id');
  $query->condition('ppmch.bundle', 'party_pack_master');
  $query->join('field_data_field_party_packing', 'pp', 'pp.entity_id = ppmpl.entity_id');
  if (!is_null($party_code)) {
    $query->condition('pc.field_party_code_value', $party_code, '=');
  }
  /* $query->fields('pl', array('field_pl_group_value'));
    $query->fields('ch', array('field_channel_value')); */ //for understanding
  $query->addExpression("case when pp.field_party_packing_value = 'YES' then 1 else 0 END", "field_party_packing_value");
  $query->fields('pc', array('field_party_code_value'));
  return $query->execute()->fetchAllKeyed(0, 1);
}

function change_feeds_import_create_billing($data, &$context) {
dpm("create billing");
  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['progress'] = 0;
  }

  foreach ($data as $key => &$value) {
    $nid = check_node_title_exists($value->bill_number, 'sales_register_data');
    $node = node_load($nid);
    if ($node) {
      $node->field_billing_van[LANGUAGE_NONE][0]['value'] = $value->vehicle;
      $node->field_billing_loading_date[LANGUAGE_NONE][0]['value'] = $value->loading_date;
      $node->field_sr_bill_date[LANGUAGE_NONE][0]['value'] = $value->bill_date;
      // sum of net sales value from custom_billing_data to sales register
      $node->field_bill_net_sales‎['und'][0]['value'] = round($value->sum_net_sales_value, 2);
      node_save($node);
    }
  }
  drupal_set_message("Created Billing");
}
