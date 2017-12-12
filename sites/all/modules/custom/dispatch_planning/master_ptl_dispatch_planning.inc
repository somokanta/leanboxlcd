<?php

/*
 * it will call from, "ptl-api-data" services
 * 
 */

function fetch_master_ptl_data($rs_code, $api_time) {
  $time = time() - 60 * 5; // send data which is created 5min befor, to avoid data loose of trip

  $query = db_select('master_ptl_data', 'cb');
  $query->addExpression('MAX(id)', 'max');
  $query->condition('api_flag', '0');
  $query->condition('rs_code', $rs_code, '=');
  $query->condition('created_time', $time, '<');
  $max_ptl_data = $query->execute()->fetchField();

  $query = db_select('master_ptl_data', 'p');
  $query->fields('p', array('ptl_job', 'rs_code'));
  $query->addExpression('COUNT(id)', 'bill_lines'); // send loose value in sum_sales_qty
  $query->addExpression('SUM(loose)', 'total_qty');
  $query->condition('p.id', $max_ptl_data, '<=');
  $query->condition('p.api_flag', '0', '=');
  $query->condition('p.rs_code', $rs_code, '=');
  $query->condition('p.loose', 1, '>=');
  $query->groupBy('rs_code,ptl_job');
  $header_result = $query->execute()->fetchAll();

  $data['header'] = $header_result;

  $query = db_select('master_ptl_data', 'p');
  $query->fields('p', array('id', 'ptl_job', 'rs_code', 'trip_id', 'vehicle', 'bill_number', 'loading_date', 'sequence_no', 'basepack_code', 'sku_code', 'product_name', 'MRP', 'TUR', 'free_sku_description', 'cp'));
  $query->addExpression('loose', 'sum_sales_qty'); // send loose value in sum_sales_qty
  $query->condition('p.id', $max_ptl_data, '<=');
  $query->condition('p.api_flag', '0', '=');
  $query->condition('p.rs_code', $rs_code, '=');
  $query->condition('p.loose', 1, '>=');
  $result = $query->execute()->fetchAll();

  $data['data'] = $result;

  if (!empty($result)) {
    $results['result'] = json_encode($data);
    $results['flag'] = '1';

    db_update('master_ptl_data') // Table name no longer needs {}
      ->fields(array(
        'fetch_time' => $api_time,
      ))
      ->condition('id', $max_ptl_data, '<=')
      ->condition('api_flag', '0', '=')
      ->condition('rs_code', $rs_code, '=')
      ->condition('loose', 1, '>=')
      ->execute();
  }
  else {
    $results['flag'] = '0';
    $results['result'] = 'No Result';
  }
  // log entry and is remaining

  return $results;
}

/*
 * it will call from, "ptl-api-data" services to update the time
 * 
 */

function update_master_ptl_data($unique_id, $update_time) {
  $query = db_select('master_ptl_data', 'cb');
  $query->addExpression('MAX(id)', 'max');
  $query->condition('id', $unique_id, 'IN');
  $query->condition('loose', 1, '>=');
  $max_ptl_data = $query->execute()->fetchField();

  $results['flag'] = '0';
  $results['result'] = 'Data not found. Please check RS Code and Trip Id';

  if ($max_ptl_data) {
    db_update('master_ptl_data') // Table name no longer needs {}
      ->fields(array(
        'update_time' => $update_time,
        'api_flag' => 1,
      ))
      ->condition('id', $unique_id, 'IN')
      ->condition('loose', 1, '>=')
      ->execute();

    $results['flag'] = '1';
    $results['result'] = 'Updated Successfully';
  }
  return $results;
}

/**
 * 
 * @param type $id
 * @param type $put_qty
 * @return string
 * Put QTY will be updated in our system by API
 * They will pass id and put_qty.
 */
function put_qty_master_ptl_data($id_qty) {

  $update_time = REQUEST_TIME;

  $results['flag'] = '0';
  $total = 0;

  foreach ($id_qty as $value) {
    $count = db_update('master_ptl_data') // Table name no longer needs {}
      ->fields(array(
        'put_qty' => $value['qty'],
        'put_qty_time' => $update_time,
      ))
      ->condition('id', $value['id'], '=')
      ->execute();
    $total += $count;
  }
  if ($total) {
    $results['flag'] = '1';
  }
  $results['result'] = "$total Row Updated";
  return $results;
}

/**
 * PTL Loose PDF REPORT
 * @param type $form
 * @param type $form_state
 * @return type
 */
function master_ptl_loose_sheet_form($form, &$form_state) {
  $form['#prefix'] = '<div id="itemlist-div">';
  $form['#suffix'] = '</div>';

  $form['master_trip_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Master Trip Id'),
    '#size' => 10,
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('submit'),
    '#ajax' => array(
      'wrapper' => 'itemlist-div',
      'callback' => 'get_data_of_master_ptl_sheet_form_submit',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    '#submit' => array('master_ptl_loose_sheet_form_submit'),
  );
  if (!empty($form_state['views_data'])) {
    $form['hhh'] = array(
      '#type' => 'markup',
      '#markup' => $form_state['views_data'],
    );
    $master_trip_id = $form_state['values']['master_trip_id'];
    $rr = l('Open PDF', "/download-master-loose-picking/$master_trip_id");
    $form['dowmload_mark'] = array(
      '#type' => 'markup',
      '#markup' => $rr,
    );
  }
  return $form;
}

function get_data_of_master_ptl_sheet_form_submit($form, &$form_state) {
  return $form;
}

function master_ptl_loose_sheet_form_submit($form, &$form_state) {
  $master_trip_id = $form_state['values']['master_trip_id'];
  $view_name = "master_ptl_loose_sheet";
  $view = views_get_view($view_name, '');
  $view->set_arguments(array($master_trip_id));
  $rendered_html = $view->render();
  $form_state['views_data'] = $rendered_html;
  $form_state['rebuild'] = TRUE;
}

function download_master_ptl_loose_sheet($args) {
  $view_name = "master_ptl_loose_sheet";
  $view = views_get_view($view_name, '');
  $view->set_arguments(array($args));
  $rendered_html = $view->render();
  pdf_using_mpdf_api($rendered_html);
}

//*************PTL PDF CLD REPORT***************
function master_ptl_cld_sheet_form($form, &$form_state) {
  $form['#prefix'] = '<div id="itemlist-div">';
  $form['#suffix'] = '</div>';

  $form['master_trip_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Master Trip Id'),
    '#size' => 10,
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('submit'),
    '#ajax' => array(
      'wrapper' => 'itemlist-div',
      'callback' => 'get_data_of_master_ptl_sheet_form_submit',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    '#submit' => array('master_ptl_cld_sheet_form_submit'),
  );
  if (!empty($form_state['views_data'])) {
    $form['hhh'] = array(
      '#type' => 'markup',
      '#markup' => $form_state['views_data'],
    );
    $master_trip_id = $form_state['values']['master_trip_id'];
    $rr = l('Open PDF', "/download-master-cld-picking/$master_trip_id");
    $form['dowmload_mark'] = array(
      '#type' => 'markup',
      '#markup' => $rr,
    );
  }
  return $form;
}

function master_ptl_cld_sheet_form_submit($form, &$form_state) {
  $master_trip_id = $form_state['values']['master_trip_id'];
  $view_name = "master_ptl_cld_sheet";
  $view = views_get_view($view_name, '');
  $view->set_arguments(array($master_trip_id));
  $rendered_html = $view->render();
  $form_state['views_data'] = $rendered_html;
  $form_state['rebuild'] = TRUE;
}

function download_master_ptl_cld_sheet($args) {
  $view_name = "master_ptl_cld_sheet";
  $view = views_get_view($view_name, '');
  $view->set_arguments(array($args));
  $rendered_html = $view->render();
  pdf_using_mpdf_api($rendered_html);
}

/**
 * 
 */
function delete_fetch_data_master_ptl_trip_form($form, &$form_state) {
  $form['master_trip_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Master Trip Id'),
    '#size' => 10,
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  return $form;
}

function delete_fetch_data_master_ptl_trip_form_submit($form, &$form_state) {
  global $user;

  $master_trip_id = $form_state['values']['master_trip_id'];
  $rs_code = master_ptl_get_rs_code_by_uid($user->uid);

  $count = db_delete('master_ptl_data')
    ->condition('ptl_job', $master_trip_id, '=')
    ->condition('rs_code', $rs_code, '=')
    ->execute();

  if ($count) {
    drupal_set_message("Master Trip $master_trip_id Fetch data is deleted successfully!");
  }
  else {
    drupal_set_message("Fetch data is not found for a master trip $master_trip_id");
  }
}

function create_fetch_data_master_ptl_trip_form($form, &$form_state) {
  $form['master_trip_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Master Trip Id'),
    '#size' => 10,
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  return $form;
}

function create_fetch_data_master_ptl_trip_form_submit($form, &$form_state) {
  global $user;
  $count = 0;
  $user_id = $user->uid;

  $master_trip_id = $form_state['values']['master_trip_id'];
  $distributor_id = change_feeds_import_get_distributor_id($user_id);
  $rs_code = master_ptl_get_rs_code_by_uid($user_id);

  $query = db_select('master_ptl_data', 'cb');
  $query->addExpression('MAX(id)', 'max');
  $query->condition('ptl_job', $master_trip_id, '=');
  $query->condition('rs_code', $rs_code, '=');
  $ptl_data_exist = $query->execute()->fetchField();

  if (!$ptl_data_exist) {
    $query = db_select('node', 'n');
    $query->addField('n', 'nid');
    $query->condition('n.title', $master_trip_id);
    $query->condition('n.uid', $distributor_id);
    $query->condition('n.type', 'master_dispatch_data');
    $master_trip_id_nid = $query->execute()->fetchField();

    $ptl_authentication_flag = ptl_check_api_authentication($distributor_id);

    if (!empty($ptl_authentication_flag) && $master_trip_id_nid) {
      $dispatch_data_nid = master_ptl_get_all_trip_by_master_trip($master_trip_id_nid);
      foreach ($dispatch_data_nid as $nid) {
        $count++;
        master_ptl_api_data_create($nid->entity_id, $master_trip_id, $count, $distributor_id);
      }
    }

    if ($count) {
      drupal_set_message("Master Trip $master_trip_id PTL data is created successfully!");
    }
    else if (empty($master_trip_id_nid)) {
      drupal_set_message("Master Trip $master_trip_id is not created!", 'warning');
    }
    else if (empty($ptl_authentication_flag)) {
      drupal_set_message("Master PTL data is not created because flag is not enabled!", 'warning');
    }
  }
  else {
    drupal_set_message("Master Trip $master_trip_id data is already created!", 'warning');
  }
}

/**
 * 
 * @param type $nid
 * @param type $trip_id
 */
function master_ptl_api_data_create($dispatch_data_nid, $master_trip_id, $sequence, $distributor_id) {
  $api_data = array();

  $query = db_select('node', 'n');
  $query->condition('n.nid', $dispatch_data_nid);
  $query->fields('n', array('title'));
  $trip_id = $query->execute()->fetchField();

  // find first sales ragister nid from trip
  $bill_numbers = get_all_bills_of_trip_id($dispatch_data_nid);

  if (!empty($bill_numbers)) {
    $rs_code = dispatch_planning_get_rscode_using_uid($distributor_id);

    //$store_sequence = (int) $node->field_drs_sequence[LANGUAGE_NONE][0]['value'];
    $store_sequence = $sequence; // for master trip asc of trip id

    $billing_data_details = master_ptl_get_billing_data($distributor_id, $bill_numbers);

    $api_data['ptl_job'] = $master_trip_id; //newly added for master trip
    $api_data['trip_id'] = $trip_id;
    $api_data['rs_code'] = $rs_code;
    $api_data['store_sequence_no'] = $store_sequence;

    $api_data['jobData']['item_details'] = $billing_data_details; // array

    master_ptl_data_insert_into_table($api_data, $distributor_id);
  }
}

/**
 * Function for  ptl_configuration_data_insert($api_data);
 */
function master_ptl_data_insert_into_table($api_data, $distributor_id) {
  if (!empty($api_data)) {
    if (!empty($api_data['jobData']['item_details'])) {
      foreach ($api_data['jobData']['item_details'] as $item_data) {

        $sku7_code = $item_data['sku_code'];
        $sku_unit_case = get_sku_unit_case($sku7_code, $distributor_id);

        // Conversion of cld and loose
        $cld = (int) ($item_data['sum_net_sales_quantity'] / $sku_unit_case); //petty
        $loose = (int) ($item_data['sum_net_sales_quantity'] % $sku_unit_case); //loose
        //New Logic for PTL
        $free_cld = (int) ($item_data['sum_of_sales_free_qty'] / $sku_unit_case); //petty
        $free_loose = (int) ($item_data['sum_of_sales_free_qty'] % $sku_unit_case); //loose
        $cp = '';
        $sum_of_sales_free_qty = 0;
        $derived_free_qty = 0;
        $free_sku_description = '';
        $derived_unit_case = 0;
        $derived_cld = 0;
        $derived_loose = 0;
        if (!empty($item_data['sum_net_sales_quantity'])) {
          $child_detail = get_child_desc_and_qty_by_parent_sku($sku7_code, $distributor_id);
          $derived_free_qty = $child_detail['child_qty'] * $item_data['sum_net_sales_quantity'];
          $free_sku_description = $child_detail['child_desc'];
          if (!empty($derived_free_qty) && !empty($free_sku_description)) {
            $cp = 'CP';
            $derived_unit_case = get_sku_unit_case($child_detail['child_sku'], $distributor_id);
            $derived_cld = (int) ($derived_free_qty / $derived_unit_case); //petty
            $derived_loose = (int) ($derived_free_qty % $derived_unit_case); //loose
          }
        }
        else {
          $sum_of_sales_free_qty = $item_data['sum_of_sales_free_qty'];
          //$free_sku_description =  $item_data['product_name']; // for now
        }
        // New logic end
        $query = db_insert('master_ptl_data');
        $fields = array(
          'ptl_job' => $api_data['ptl_job'], //change for master trip
          'rs_code' => $api_data['rs_code'],
          'trip_id' => $api_data['trip_id'],
          'vehicle' => $item_data['vehicle'],
          'bill_number' => $item_data['bill_number'],
          'loading_date' => $item_data['loading_date'],
          'sequence_no' => $api_data['store_sequence_no'],
          'basepack_code' => $item_data['basepack_code'],
          'sku_code' => $item_data['sku_code'],
          'product_name' => $item_data['product_name'],
          'MRP' => $item_data['mrp'],
          'TUR' => $item_data['tur'],
          'sum_sales_qty' => $item_data['sum_net_sales_quantity'],
          'created_time' => REQUEST_TIME,
          'cld' => $cld,
          'loose' => $loose,
          'upc' => $sku_unit_case ? $sku_unit_case : 0,
          'sum_of_sales_free_qty' => $sum_of_sales_free_qty, //below are newly added
          'free_sku_description' => $free_sku_description,
          'cp' => $cp ? $cp : '',
          'free_cld' => $free_cld,
          'free_loose' => $free_loose,
          'derived_free_qty' => $derived_free_qty,
          'derived_cld' => $derived_cld,
          'derived_loose' => $derived_loose,
          'derived_upc' => $derived_unit_case
        );
        $query->fields($fields);
        $result_id = $query->execute();
      }
    }
  }
}

function master_ptl_get_billing_data($node_uid, $bill_numbers) {
  $query = db_select('custom_billing_data', 'bd');
  $query->fields('bd', array('tur', 'mrp', 'product_name'));
  //$query->addfield('bd', 'Sum_of_NET_SALES_QTY', 'sum_net_sales_quantity');
  //$query->addfield('bd', 'Sum_of_SALES_FREE_QTY', 'sum_of_sales_free_qty');
  $query->addExpression('SUM(bd.Sum_of_NET_SALES_QTY)', 'sum_net_sales_quantity'); //for master trip
  $query->addExpression('SUM(bd.Sum_of_SALES_FREE_QTY)', 'sum_of_sales_free_qty');

  $query->addField('bd', 'bill_number'); // newly added for master trip
  $query->addField('bd', 'vehicle');
  $query->addField('bd', 'loading_date');

  $query->addfield('bd', 'sku7_code', 'sku_code');
  $query->addfield('bd', 'BASEPACK_CODE', 'basepack_code');
  $query->condition('bd.distributor_id', $node_uid);
  //$query->condition('bd.bill_number', $bill_number);
  $query->condition('bd.bill_number', $bill_numbers, 'IN'); //for master trip
  $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
  return $res;
}

function master_ptl_get_all_trip_by_master_trip($master_trip_id_nid) {
  $query = db_select('field_data_field_master_trip_id_reference', 'tr');
  $query->addField('tr', 'entity_id');
  $query->condition('field_master_trip_id_reference_nid', "$master_trip_id_nid");
  $query->condition('bundle', "dispatch_data");
  $query->orderBy('tr.entity_id');
  $dispatch_data_nid = $query->execute()->fetchAll();
  return $dispatch_data_nid;
}

function master_ptl_get_rs_code_by_uid($uid) {
  $query = db_select('field_data_field_third_party_username', 'rs');
  $query->condition('rs.entity_id', $uid);
  $query->fields('rs', array('field_third_party_username_value'));
  $rs_code = $query->execute()->fetchField();
  return $rs_code;
}

/**
 * 
 * @param type $sku7_code
 * @param type $distributor_id
 */
function get_child_desc_and_qty_by_parent_sku($sku7_code, $distributor_id) {
  $result = array();
  $query = db_select('node', 'n');
  $query->condition('n.type', 'sku7_code_master');
  $query->condition('n.uid', $distributor_id);
  $query->condition('n.title', $sku7_code);
  $query->leftjoin('field_data_field_child_sku', 'csk', 'csk.entity_id = n.nid');
  $query->leftjoin('node', 'n1', 'n1.nid = csk.field_child_sku_nid');

  $query->join('field_data_field_sku7_code', 'sc', 'sc.entity_id = n1.nid');
  $query->fields('sc', array('field_sku7_code_value'));

  $query->join('field_data_field_product_name', 'pn', 'pn.entity_id = n1.nid');
  $query->fields('pn', array('field_product_name_value'));
  $query->leftjoin('field_data_field_child_sku_qty‎', 'cskq', 'cskq.entity_id = n.nid');
  $query->fields('cskq', array('field_child_sku_qty_value'));
  $row = $query->execute()->fetch(PDO::FETCH_ASSOC);

  $result['child_sku'] = ($row['field_sku7_code_value']) ? $row['field_sku7_code_value'] : '';
  $result['child_qty'] = ($row['field_child_sku_qty_value']) ? $row['field_child_sku_qty_value'] : 0;
  $result['child_desc'] = ($row['field_product_name_value']) ? $row['field_product_name_value'] : '';
  return $result;
}

/**
 * 
 * @param type $dispatch_data_nid
 */
function get_all_bills_of_trip_id($dispatch_data_nid) {
  $query = db_select('field_data_field_trip_id_reference', 'tr');
  $query->condition('field_trip_id_reference_nid', "$dispatch_data_nid");
  $query->join('node', 'n', 'n.nid = tr.entity_id');
  $query->addExpression('GROUP_CONCAT(n.title)', 'titles');
  $query->condition('bundle', "sales_register_data");
  $sales_register_titles = $query->execute()->fetchField();
  $bills = explode(',', $sales_register_titles);
}
