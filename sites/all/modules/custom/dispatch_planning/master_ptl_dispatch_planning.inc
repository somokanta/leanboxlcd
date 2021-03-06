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
  $query->fields('p', array('id', 'ptl_job', 'rs_code', 'trip_id', 'vehicle', 'loading_date', 'sequence_no', 'basepack_code', 'sku_code', 'product_name', 'MRP', 'TUR', 'free_sku_description', 'cp','ean','ean1','ean2','ean3'));
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
    
    insert_master_ptl_header_data($header_result);
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
        'put_operator_id' => $value['put_operator_id'],
        'put_time'=>$value['put_time'],
        'close_operator_id'=>$value['close_operator_id'],
        'zone'=>$value['zone'],
        'close_time'=>$value['close_time'],
        'box_id'=>$value['box_id'],
      ))
      ->expression('short_put_qty', "loose - ".$value['qty'])
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
    $rr = l('Open PDF', "/download-master-loose-picking/$master_trip_id", array('attributes' => array('target'=> '_blank')));
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
  $form['trip_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Trip Id'),
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
    $trip_id = $form_state['values']['trip_id'];
    $rr = l('Open PDF', "/download-master-cld-picking/$master_trip_id/$trip_id");
    $form['dowmload_mark'] = array(
      '#type' => 'markup',
      '#markup' => $rr,
    );
  }
  return $form;
}

function master_ptl_cld_sheet_form_validate($form, &$form_state) {
  $master_trip_id = $form_state['values']['master_trip_id'];
  $trip_id = $form_state['values']['trip_id'];
  if(empty($master_trip_id)){
    form_set_error('master_ptl_id', 'Please enter Master PTL ID');
  }
  if(empty($trip_id)){
    form_set_error('trip_id', 'Please enter trip ID');
  }
}

function master_ptl_cld_sheet_form_submit($form, &$form_state) {
  $master_trip_id = $form_state['values']['master_trip_id'];
  $trip_id = $form_state['values']['trip_id'];
  $view_name = "master_ptl_cld_sheet";
  $view = views_get_view($view_name, '');
  $view->set_arguments(array($master_trip_id,$trip_id));
  $rendered_html = $view->render();
  $form_state['views_data'] = $rendered_html;
  $form_state['rebuild'] = TRUE;
}

function download_master_ptl_cld_sheet($args,$args2) {
  $view_name = "master_ptl_cld_sheet";
  $view = views_get_view($view_name, '');
  $view->set_arguments(array($args,$args2));
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

/**
 * 
 * @global type $user
 * @param type $form
 * @param type $form_state
 */
function delete_fetch_data_master_ptl_trip_form_validate($form, &$form_state) {
  $master_trip_id = $form_state['values']['master_trip_id'];

  global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);

  //A Trip ID has been sent to PTL (i.e. data exists in PTL Complete report for that Trip ID)
  $master_ptl_data_exist = check_trip_exist_in_master_ptl($master_trip_id, $distributor_id);
  if (empty($master_ptl_data_exist)) {// empty data not found
    form_set_error('trip_id', "Master Trip ID: $master_trip_id is not exist in Master PTL!");
  }
  
  // check trip wise in loop
  $all_trip_id = get_all_trip_by_master_ptl($master_trip_id, $distributor_id);
  foreach ($all_trip_id as $trip_id) {
    //PTL Loose confirmation has been done for that Trip ID
    $loose_completed = check_ptl_loose_confirmation_for_trip($trip_id, $distributor_id);
    if (!empty($loose_completed)) {
      form_set_error('trip_id', "PTL Loose confirmation has been done for Trip ID: $trip_id");
    }

    //CLD Confirmation has been done for that Trip ID
    $cld_completed = check_cld_confirmation_for_trip($trip_id, $distributor_id);
    if (!empty($cld_completed)) {
      form_set_error('trip_id', "CLD confirmation has been done for Trip ID: $trip_id");
    }

    //CLD Confirmation has been done for that Trip ID
    $cld_retail_confirmed = check_cld_retailer_confirmation_for_trip($trip_id,$distributor_id);
    if (!empty($cld_retail_confirmed)) {
      form_set_error('trip_id', "CLD retailer confirmation has been done for Trip ID: $trip_id");
    }    

    //Re-upload of custom billing data has been done
    $bill_reupload = check_reupload_for_any_bills_in_trip($trip_id, $distributor_id);
    if (!empty($bill_reupload)) {
      form_set_error('trip_id', "Re-upload of custom billing data has been done for the bills: $bill_reupload for a Trip ID: $trip_id");
    }
  }
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
      
      $pending_bills = master_ptl_get_pending_bill_title_by_master_trip($master_trip_id_nid);
      if($pending_bills){
        drupal_set_message("Did not send DP Bill Numbers: $pending_bills",'warning');
      }
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
        $eans = get_sku_ean($sku7_code, $api_data['rs_code']);
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
//            $derived_cld = (int) ($derived_free_qty / $derived_unit_case); //petty
//            $derived_loose = (int) ($derived_free_qty % $derived_unit_case); //loose
            $derived_cld = number_format((($cld * $sku_unit_case * $child_detail['child_qty']) / $derived_unit_case), 2, '.', ''); //petty
            $derived_loose = $loose * $child_detail['child_qty']; //loose            
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
          'short_put_qty'=>$loose,
          'upc' => $sku_unit_case ? $sku_unit_case : 0,
          'sum_of_sales_free_qty' => $sum_of_sales_free_qty, //below are newly added
          'free_sku_description' => $free_sku_description,
          'cp' => $cp ? $cp : '',
          'free_cld' => $free_cld,
          'free_loose' => $free_loose,
          'derived_free_qty' => $derived_free_qty,
          'derived_cld' => $derived_cld,
          'derived_loose' => $derived_loose,
          'derived_upc' => $derived_unit_case,
          'ean' => $eans[0]->ean ? $eans[0]->ean : '',
          'ean1' => $eans[0]->ean1 ? $eans[0]->ean1 : '',
          'ean2' => $eans[0]->ean2 ? $eans[0]->ean2 : '',
          'ean3' => $eans[0]->ean3 ? $eans[0]->ean3 : '',
          'distributor_id' => $distributor_id ? $distributor_id : 0,
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
  $res = $query->groupBy('sku7_code','tur', 'mrp')->execute()->fetchAll(PDO::FETCH_ASSOC);
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
    $query->condition('tr.field_trip_id_reference_nid', "$dispatch_data_nid");
    $query->join('node', 'n', 'n.nid = tr.entity_id');
    $query->addExpression('GROUP_CONCAT(n.title)', 'titles');
    $query->condition('tr.bundle', "sales_register_data");
    //don't send pending bills. means which picking is done against previous trip attached
    $query->leftJoin('field_data_field_previous_trip', 'dp', 'tr.entity_id = dp.entity_id');
    $query->isNull('dp.field_previous_trip_value');
    $sales_register_titles = $query->execute()->fetchField();
    $bills = explode(',', $sales_register_titles);
  return $bills;
}

/**
 * it will call to insert the header data in header table
 */
function insert_master_ptl_header_data($header_result) {
  $distributor_id = get_distributor_id_using_rscode($header_result[0]->rs_code);
  foreach ($header_result as $value) {
    $result = db_merge('ptl_header_data')
      ->key(array('ptl_job' => $value->ptl_job, 'rs_code' => $value->rs_code, 'type' => 'MASTER_TRIP'))
      ->fields(array(
        'ptl_job' => $value->ptl_job,
        'bill_lines' => $value->bill_lines,
        'total_qty' => $value->total_qty,
        'rs_code' => $value->rs_code,
        'type' => 'MASTER_TRIP', // same table, diff is trip and master trip
        'created_time' => REQUEST_TIME,
        'distributor_id' => $distributor_id ? $distributor_id : 0
      ))
      ->execute();
  }
}
/**
 * it will call for update the header data
 * $ptl_job ,$trip_id, $rs_code, $vehicle,$bill_lines,$total_qty,$operator_id,$start_time,$end_time
 */

function update_master_ptl_header_data($ptl_job ,$trip_id, $rs_code, $vehicle,$bill_lines,$total_qty,$operator_id,$start_time,$end_time) {

  $results['flag'] = '0';
  $results['result'] = 'Data not found. Please check RS Code and PTL Job';

   $result =  db_update('ptl_header_data') // Table name no longer needs {}
      ->fields(array(
        'trip_id' => $trip_id,
        'vehicle' => $vehicle,
        'res_bill_lines' => $bill_lines,
        'res_total_qty' => $total_qty,
        'operator_id' => $operator_id,
        'start_time' => $start_time,
        'end_time' => $end_time,
        'updated_time' => time(),
      ))
      ->condition('ptl_job', $ptl_job, '=')
      ->condition('type', 'MASTER_TRIP', '=')
      ->condition('rs_code', $rs_code, '=')
      ->execute();
   
  if($result){
    $results['flag'] = '1';
    $results['result'] = 'Updated Successfully';
  }
  return $results;
}

/**
 * 
 * @param type $rs_code
 * @param type $api_time
 * @return string
 */
function fetch_master_ptl_sku_ean_mrp_data($rs_code, $api_time) {
  $time = time() - 60 * 1; // send data which is created 1min befor, to avoid data loose of trip

  $query = db_select('sku_ean_mrp', 'p');
  $query->addField('p', 'sku', 'sku_code');
  $query->fields('p', array('ean', 'mrp','ean1', 'mrp1','ean2', 'mrp2','ean3', 'mrp3','rs_code'));
  $query->condition('created_time', $time, '<');
  $query->condition('p.master_flag', '0', '=');
  $query->condition('p.rs_code', $rs_code, '=');
  $result = $query->execute()->fetchAll();
  
  if (!empty($result)) {
    $results['result'] = json_encode($result);
    $results['flag'] = '1';

    db_update('sku_ean_mrp') // Table name no longer needs {}
      ->fields(array(
        'master_api_fetch_time' => $api_time,
        'master_flag' => 1
      ))
      ->condition('created_time', $time, '<')
      ->condition('master_flag', '0', '=')
      ->condition('rs_code', $rs_code, '=')
      ->execute();
  }
  else {
    $results['flag'] = '0';
    $results['result'] = 'No Result';
  }
  // log entry and is remaining

  return $results;
}

//Master trip Box -ID
function master_trip_box_id_form($form, &$form_state) {
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
    '#submit' => array('master_trip_box_id_form_submit'),
  );
  if (!empty($form_state['views_data'])) {
    $form['hhh'] = array(
      '#type' => 'markup',
      '#markup' => $form_state['views_data'],
    );
    $master_trip_id = $form_state['values']['master_trip_id'];
    $rr = l('Open PDF', "/download-master-trip-box-id/$master_trip_id",array('attributes'=>array("target" => "_blank")));
    $form['dowmload_mark'] = array(
      '#type' => 'markup',
      '#markup' => $rr,
    );
  }
  return $form;
}

function master_trip_box_id_form_validate($form, &$form_state) {
  global $user;
  $rs_code = master_ptl_get_rs_code_by_uid($user->uid);
  $master_trip_id = $form_state['values']['master_trip_id'];
  if(empty($master_trip_id)){
    unset($form_state['complete form']['hhh']);
    unset($form_state['complete form']['dowmload_mark']);
    form_set_error('master_ptl_id', 'Please enter Master PTL ID');
  }
  $query1 = db_select('master_ptl_data', 'mptl');
  $query1->condition('mptl.ptl_job', $master_trip_id, "=");
  $query1->condition('mptl.rs_code', $rs_code, "=");
  $query1->addExpression('SUM(mptl.put_qty)', 'put_qty');
  $put_qty = $query1->execute()->fetchField();

  $query = db_select('ptl_put_qty', 'put');
  $query->condition('put.type', 'MASTER_TRIP', "=");
  $query->condition('put.rs_code', $rs_code, "=");
  $query->condition('put.ptl_job', $master_trip_id, "=");
  $query->addExpression('SUM(put.put_qty_sum)', 'put_qty_sum');
  $put_qty_sum = $query->execute()->fetchField();
  
  $error_msg = '';

  if (empty($put_qty)) {
    $error_msg = "Put Quantity is empty";
  }
  if (empty($put_qty_sum)) {
    $error_msg = "Put Incremental Quantity is empty";
  }
  if ($put_qty != $put_qty_sum) {
    $error_msg = "Put Quantity($put_qty) and Put Incremental Quantity($put_qty_sum) does not match";
  }
  
  $query1 = db_select('master_ptl_data', 'mptl');
  $query1->condition('mptl.ptl_job', $master_trip_id, "=");
  $query1->condition('mptl.rs_code', $rs_code, "=");
  $query1->condition('mptl.box_id', '', "!=");
  $query1->fields('mptl',array('box_id'));
  $query1->range(0, 1);
  $res = $query1->execute()->fetchField();
 
  if (!empty($error_msg) && !empty($res)) {
    unset($form_state['complete form']['hhh']);
    unset($form_state['complete form']['dowmload_mark']);
    form_set_error('master_trip_id', $error_msg);
  }
}

function master_trip_box_id_form_submit($form, &$form_state) {
  $master_trip_id = $form_state['values']['master_trip_id'];
  $view_name = "master_trip_box_id_report";
  $view = views_get_view($view_name, '');
  $view->set_arguments(array($master_trip_id));
  $rendered_html = $view->render();
  $form_state['views_data'] = $rendered_html;
  $form_state['rebuild'] = TRUE;
}


function download_master_trip_box_id($args) {
  $view_name = "master_trip_box_id_report";
  $view = views_get_view($view_name, '');
  $view->set_arguments(array($args));
  $rendered_html = $view->render();
  pdf_using_mpdf_api($rendered_html);
}

/**
 * 
 * @param type $ptl_job
 * @param type $rs_code
 * @param type $line_count
 * @param type $put_qty_sum
 */
function update_master_ptl_put_qty_sum_data($ptl_job, $id, $rs_code, $api_time, $line_count, $put_qty_sum) {
   $distributor_id = get_distributor_id_using_rscode($rs_code);
   $result = db_merge('ptl_put_qty')
       ->key(array('unique_id' => $id, 'rs_code' => $rs_code, 'type' => 'MASTER_TRIP'))
       ->fields(array(
         'unique_id' => $id,
         'ptl_job' => $ptl_job,
         'line_count' => $line_count,
         'put_qty_sum' => $put_qty_sum,
         'rs_code' => $rs_code,
         'type' => 'MASTER_TRIP', // same table diff is trip and master trip
         'created' => $api_time,
         'distributor_id' => $distributor_id ? $distributor_id : 0
       ))
       ->execute();
   $results['flag'] = '1';
   $results['result'] = 'Updated Successfully';
   return $results;
}

function master_ptl_get_pending_bill_title_by_master_trip($master_trip_id_nid) {
    $query = db_select('field_data_field_master_trip_id_reference', 'tm');
    $query->condition('tm.field_master_trip_id_reference_nid', "$master_trip_id_nid");
    $query->condition('tm.bundle', "dispatch_data");
    $query->leftJoin('field_data_field_trip_id_reference', 'td', 'td.field_trip_id_reference_nid = tm.entity_id');
    $query->condition('td.bundle', "sales_register_data");
    $query->join('node', 'n', 'n.nid = td.entity_id');
    $query->addExpression('GROUP_CONCAT(n.title)', 'titles');
    $query->leftJoin('field_data_field_previous_trip', 'dp', 'td.entity_id = dp.entity_id');
    $query->isNotNull('dp.field_previous_trip_value');
    $sales_register_title = $query->execute()->fetchField();
  return $sales_register_title;
}