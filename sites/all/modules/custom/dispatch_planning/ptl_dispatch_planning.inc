<?php

/*
 * it will call from, "ptl-api-data" services
 * 
 */

function fetch_ptl_data($rs_code, $api_time) {
  $time = time() - 60 * 5; // send data which is created 5min befor, to avoid data loose of trip

  $query = db_select('ptl_data_configuration', 'cb');
  $query->addExpression('MAX(id)', 'max');
  $query->condition('api_flag', '0');
  $query->condition('rs_code', $rs_code, '=');
  $query->condition('created_time', $time, '<');
  $max_ptl_data = $query->execute()->fetchField();
  
  $query = db_select('ptl_data_configuration', 'p');
  $query->fields('p', array('ptl_job', 'rs_code'));
  $query->addExpression('COUNT(id)', 'bill_lines'); // send loose value in sum_sales_qty
  $query->addExpression('SUM(loose)', 'total_qty');
  $query->condition('p.id', $max_ptl_data, '<=');
  $query->condition('p.api_flag', '0', '=');
  $query->condition('p.rs_code', $rs_code, '=');
  $query->condition('p.loose', 1, '>=');
  $query->groupBy('rs_code,ptl_job');
  $header_result = $query->execute()->fetchAll();
  
  $data['header'] =  $header_result;
  
  $query = db_select('ptl_data_configuration', 'p');
  $query->fields('p', array('id', 'ptl_job', 'rs_code', 'trip_id', 'vehicle', 'bill_number', 'loading_date', 'sequence_no', 'basepack_code', 'sku_code', 'product_name', 'MRP', 'TUR','free_sku_description','cp','ean','ean1','ean2','ean3'));
  $query->addExpression('loose', 'sum_sales_qty'); // send loose value in sum_sales_qty
  $query->condition('p.id', $max_ptl_data, '<=');
  $query->condition('p.api_flag', '0', '=');
  $query->condition('p.rs_code', $rs_code, '=');
  $query->condition('p.loose', 1, '>=');
  $result = $query->execute()->fetchAll();

  $data['data'] =  $result;
  
  if (!empty($result)) {
    $results['result'] = json_encode($data);
    $results['flag'] = '1';

    db_update('ptl_data_configuration') // Table name no longer needs {}
      ->fields(array(
        'fetch_time' => $api_time,
      ))
      ->condition('id', $max_ptl_data, '<=')
      ->condition('api_flag', '0', '=')
      ->condition('rs_code', $rs_code, '=')
      ->condition('loose', 1, '>=')
      ->execute();
    
    insert_ptl_header_data($header_result);
  }
  else {
    $results['flag'] = '0';
    $results['result'] = 'No Result';
  }
  // log entry and is remaining

  return $results;
}
/**
 * 
 * @param type $rs_code
 * @param type $api_time
 * @return string
 */
function fetch_sku_ean_mrp_data($rs_code, $api_time) {
  $time = time() - 60 * 1; // send data which is created 5min befor, to avoid data loose of trip

  $query = db_select('sku_ean_mrp', 'p');
  $query->addField('p', 'sku', 'sku_code');
  $query->fields('p', array('ean', 'mrp','ean1', 'mrp1','ean2', 'mrp2','ean3', 'mrp3','rs_code'));
  $query->condition('created_time', $time, '<');
  $query->condition('p.flag', '0', '=');
  $query->condition('p.rs_code', $rs_code, '=');
  $result = $query->execute()->fetchAll();
  
  if (!empty($result)) {
    $results['result'] = json_encode($result);
    $results['flag'] = '1';

    db_update('sku_ean_mrp') // Table name no longer needs {}
      ->fields(array(
        'api_fetch_time' => $api_time,
        'flag' => 1
      ))
      ->condition('created_time', $time, '<')
      ->condition('flag', '0', '=')
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

/*
 * it will call from, "ptl-api-data" services to update the time
 * 
 */

function update_ptl_data($unique_id, $update_time) {
  $query = db_select('ptl_data_configuration', 'cb');
  $query->addExpression('MAX(id)', 'max');
  $query->condition('id', $unique_id, 'IN');
  $query->condition('loose', 1, '>=');
  $max_ptl_data = $query->execute()->fetchField();

  $results['flag'] = '0';
  $results['result'] = 'Data not found. Please check RS Code and Trip Id';

  if ($max_ptl_data) {
    db_update('ptl_data_configuration') // Table name no longer needs {}
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
function put_qty_ptl_data($id_qty) {

  $update_time = REQUEST_TIME;

  $results['flag'] = '0';
  $total = 0;

  foreach ($id_qty as $value) {
    $count = db_update('ptl_data_configuration') // Table name no longer needs {}
      ->fields(array(
        'put_qty' => $value['qty'],
        'put_qty_time' => $update_time,
        'put_operator_id' => $value['put_operator_id'],
        'put_time'=>$value['put_time'],
        'close_operator_id'=>$value['close_operator_id'],
        'zone'=>$value['zone'],
        'close_time'=>$value['close_time'],
        'box_id'=>$value['box_id'],
        'reason_code'=>$value['reason_code'] ? $value['reason_code'] : "",
      ))
      ->expression('short_put_qty', "loose - ".$value['qty'])
      ->condition('id', $value['id'], '=')
      ->execute();
    $total += $count;
  }
  if($total){
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
function ptl_loose_sheet_form($form, &$form_state) {
  $form['#prefix'] = '<div id="itemlist-div">';
  $form['#suffix'] = '</div>';

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
      'callback' => 'get_data_of_ptl_sheet_form_submit',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    '#submit' => array('ptl_loose_sheet_form_submit'),
  );
  if (!empty($form_state['views_data'])) {
    $form['hhh'] = array(
      '#type' => 'markup',
      '#markup' => $form_state['views_data'],
    );
    $trip_id = $form_state['values']['trip_id'];
    $rr = l('Open PDF', "/download-loose-picking/$trip_id", array('attributes' => array('target'=> '_blank')));
    $form['dowmload_mark'] = array(
      '#type' => 'markup',
      '#markup' => $rr,
    );
  }
  return $form;
}

function get_data_of_ptl_sheet_form_submit($form, &$form_state) {
  return $form;
}

function ptl_loose_sheet_form_submit($form, &$form_state) {
  $trip_id = $form_state['values']['trip_id'];
  $view_name = "ptl_loose_sheet";
  $view = views_get_view($view_name, '');
  $view->set_arguments(array($trip_id));
  $rendered_html = $view->render();
  $form_state['views_data'] = $rendered_html;
  $form_state['rebuild'] = TRUE;
}

function download_ptl_loose_sheet($args) {
  $view_name = "ptl_loose_sheet";
  $view = views_get_view($view_name, '');
  $view->set_arguments(array($args));
  $rendered_html = $view->render();
  pdf_using_mpdf_api($rendered_html);
}

//*************PTL PDF CLD REPORT***************
function ptl_cld_sheet_form($form, &$form_state) {
  $form['#prefix'] = '<div id="itemlist-div">';
  $form['#suffix'] = '</div>';

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
      'callback' => 'get_data_of_ptl_sheet_form_submit',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    '#submit' => array('ptl_cld_sheet_form_submit'),
  );
  if (!empty($form_state['views_data'])) {
    $form['hhh'] = array(
      '#type' => 'markup',
      '#markup' => $form_state['views_data'],
    );
    $trip_id = $form_state['values']['trip_id'];
    $rr = l('Open PDF', "/download-cld-picking/$trip_id");
    $form['dowmload_mark'] = array(
      '#type' => 'markup',
      '#markup' => $rr,
    );
  }
  return $form;
}

function ptl_cld_sheet_form_submit($form, &$form_state) {
  $trip_id = $form_state['values']['trip_id'];
  $view_name = "ptl_cld_sheet";
  $view = views_get_view($view_name, '');
  $view->set_arguments(array($trip_id));
  $rendered_html = $view->render();
  $form_state['views_data'] = $rendered_html;
  $form_state['rebuild'] = TRUE;
}

function download_ptl_cld_sheet($args) {
  $view_name = "ptl_cld_sheet";
  $view = views_get_view($view_name, '');
  $view->set_arguments(array($args));
  $rendered_html = $view->render();
  pdf_using_mpdf_api($rendered_html);
}

/**
 * 
 */
function delete_fetch_data_ptl_trip_form($form, &$form_state){
  $form['trip_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Trip Id'),
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
function delete_fetch_data_ptl_trip_form_validate($form, &$form_state) {
  $trip_id = $form_state['values']['trip_id'];
  
  global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);

  //A Trip ID has been sent to PTL (i.e. data exists in PTL Complete report for that Trip ID)
  $ptl_data_exist = check_trip_exist_in_ptl($trip_id,$distributor_id);
  if (empty($ptl_data_exist)) {// empty data not found
    form_set_error('trip_id', "Trip ID: $trip_id is not exist in PTL!");
  }
  
  //PTL Loose confirmation has been done for that Trip ID
  $loose_completed = check_ptl_loose_confirmation_for_trip($trip_id,$distributor_id);
  if (!empty($loose_completed)) {
    form_set_error('trip_id', "PTL Loose confirmation has been done for Trip ID: $trip_id");
  }

  //CLD Confirmation has been done for that Trip ID
  $cld_completed = check_cld_confirmation_for_trip($trip_id,$distributor_id);
  if (!empty($cld_completed)) {
    form_set_error('trip_id', "CLD confirmation has been done for Trip ID: $trip_id");
  }
  
  //CLD Confirmation has been done for that Trip ID
  $cld_retail_confirmed = check_cld_retailer_confirmation_for_trip($trip_id,$distributor_id);
  if (!empty($cld_retail_confirmed)) {
    form_set_error('trip_id', "CLD retailer confirmation has been done for Trip ID: $trip_id");
  }

  //Re-upload of custom billing data has been done
  $bill_reupload = check_reupload_for_any_bills_in_trip($trip_id,$distributor_id);
  if (!empty($bill_reupload)) {
    form_set_error('trip_id', "Re-upload of custom billing data has been done for the bills: $bill_reupload");
  }

}

function delete_fetch_data_ptl_trip_form_submit($form, &$form_state) {
  global $user;
  
  $trip_id = $form_state['values']['trip_id'];
  $rs_code = ptl_get_rs_code_by_uid($user->uid);

  $count = db_delete('ptl_data_configuration')
    ->condition('trip_id', $trip_id, '=')
    ->condition('rs_code', $rs_code, '=')
    ->execute();
  
  if ($count) {
    drupal_set_message("Trip $trip_id Fetch data is deleted successfully!");
    $nid = get_nid_from_title($trip_id, 'dispatch_data');
    $node = node_load($nid);
    $node->field_trip_type['und'][0]['value'] = 0; // make NON PTL
    node_save($node);
  }
  else {
    drupal_set_message("Fetch data is not found for a trip $trip_id");
  }
}

function create_fetch_data_ptl_trip_form($form, &$form_state){
  $form['trip_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Trip Id'),
    '#size' => 10,
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  return $form;
}

function create_fetch_data_ptl_trip_form_validate($form, &$form_state) {
  global $user;
  $user_id = $user->uid;
  $trip_id = $form_state['values']['trip_id'];
  $distributor_id = change_feeds_import_get_distributor_id($user_id);
  $node_created_time = get_node_created_time($trip_id, 'dispatch_data', $distributor_id);
  $diff_minutes = abs(time() - $node_created_time) / 60;
    // Should be  atleast 2 minutes delay after creating Trip otherwise all bill related fields values would not update.
   if ($diff_minutes < 2) {
       form_set_error('trip_id', "Please wait for 2 minutes to send Trip ID:$trip_id in PTL station.");
   }

    $query = db_select('node', 'n');
    $query->condition('n.title', $trip_id);
    $query->condition('n.uid', $distributor_id);
    $query->condition('n.type', 'dispatch_data');
    $query->join('field_data_field_master_trip_id_reference', 'mt', 'mt.entity_id = n.nid');
    $query->join('node', 'n1', 'mt.field_master_trip_id_reference_nid = n1.nid');
    $query->condition('n1.uid', $distributor_id);
    $query->condition('n1.type', 'master_dispatch_data');
    $query->addField('n1', 'title');
    $master_trip_id = $query->execute()->fetchField();

  if(!empty($master_trip_id)){
    form_set_error('trip_id', "Trip ID $trip_id has already been attached to a Master Trip ID $master_trip_id");
  }
  
/*
  $rs_code = dispatch_planning_get_rscode_using_uid($distributor_id);

  $query = db_select('node', 'n');
  $query->addField('n', 'nid');
  $query->condition('n.title', $trip_id);
  $query->condition('n.uid', $distributor_id);
  $query->condition('n.type', 'dispatch_data');
  $trip_id_nid = $query->execute()->fetchField();

  $query = db_select('node', 'n');
  $query->condition('n.type', "sales_register_data");
  $query->addField('n', 'title');
  $query->join('field_data_field_trip_id_reference', 'tir', 'tir.entity_id = n.nid');
  $query->condition('field_trip_id_reference_nid', "$trip_id_nid");
  $bill_numbers = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

  $query = db_select('custom_billing_data', 'bd');
  $query->addField('bd', 'sku7_code');
  $query->condition('bd.distributor_id', $distributor_id);
  $query->condition('bd.Sum_of_NET_SALES_QTY', 1, '>=');
  $query->condition('bd.bill_number', $bill_numbers, 'IN');
  $query->groupBy('sku7_code');
  $sku7_codes = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

  $sku_codes = array();
  foreach ($sku7_codes as $sku) {
    $sku_codes[] = $sku['sku7_code'];
  }

  $query = db_select('sku_ean_mrp', 'p');
  $query->addField('p', 'sku');
  $query->condition('p.rs_code', $rs_code, '=');
  $query->condition('p.sku', $sku_codes, 'IN');
  $exist_sku7s = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

  $exist_skus = array();
  foreach ($exist_sku7s as $sku) {
    $exist_skus[] = $sku['sku'];
  }

  $not_ean_sku = array_diff($sku_codes, $exist_skus);

  if (!empty($not_ean_sku)) {
    form_set_error('trip_id', 'EAN is not maintained for SKUs: ' . implode(' , ', $not_ean_sku));
  }
 * 
 */
}

function create_fetch_data_ptl_trip_form_submit($form, &$form_state) {
  global $user;
  $count = 0;
  $user_id = $user->uid;
  
  $trip_id = $form_state['values']['trip_id'];
  $distributor_id = change_feeds_import_get_distributor_id($user_id);
  $rs_code = ptl_get_rs_code_by_uid($user_id);
  
  $query = db_select('ptl_data_configuration', 'cb');
  $query->addExpression('MAX(id)', 'max');
  $query->condition('trip_id', $trip_id, '=');
  $query->condition('rs_code', $rs_code, '=');
  $ptl_data_exist = $query->execute()->fetchField();
  
  if (!$ptl_data_exist) {
    $query = db_select('node', 'n');
    $query->addField('n', 'nid');
    $query->condition('n.title', $trip_id);
    $query->condition('n.uid', $distributor_id);
    $query->condition('n.type', 'dispatch_data');
    $trip_id_nid = $query->execute()->fetchField();

    $ptl_authentication_flag = ptl_check_api_authentication($distributor_id);

    if (!empty($ptl_authentication_flag) && $trip_id_nid) {
      $sales_register_nid = ptl_get_sales_register_nid_by_trip($trip_id_nid);
      if (count($sales_register_nid) < 69) {
        foreach ($sales_register_nid as $nid) {
          $count = 1;
          ptl_api_data_create($nid->entity_id, $trip_id);
        }
      }else{
        drupal_set_message("Trip contains more than 68 Bills!",'warning');
      }
    }

    if ($count) {
      drupal_set_message("Trip $trip_id PTL data is created successfully!");
      // Show pending bill attached in this trip
      $pending_bills = ptl_get_pending_bill_title_by_trip($trip_id_nid);
      if($pending_bills){
        drupal_set_message("Did not send DP Bill Numbers: $pending_bills",'warning');
      }
      // Assign trip type
      $nid = get_nid_from_title($trip_id, 'dispatch_data');
      $node = node_load($nid);
      $node->field_trip_type['und'][0]['value'] = 1; // PTL
      node_save($node);
    }
    else if(empty($trip_id_nid)){  
      drupal_set_message("Trip $trip_id is not created!",'warning');
    }
    else if(empty($ptl_authentication_flag)){
      drupal_set_message("PTL data is not created because flag is not enabled!",'warning');
    }else{
      drupal_set_message("Trip $trip_id has no bills!",'warning');
    }   
  }
  else {
    drupal_set_message("Trip $trip_id data is already created!", 'warning');
  }
}

/**
 * 
 * @param type $nid
 * @param type $trip_id
 */
function ptl_api_data_create($nid, $trip_id) {
  $api_data = array();

  $node = node_load($nid);
  $node_uid = $node->uid;
  if (!empty($node)) {
    $rs_code = dispatch_planning_get_rscode_using_uid($node_uid);
    $vehicle = $node->field_billing_van[LANGUAGE_NONE][0]['value'];
    $bill_number = $node->title;
    $loading_data = $node->field_billing_loading_date[LANGUAGE_NONE][0]['value'];
    $store_sequence = (int) $node->field_drs_sequence[LANGUAGE_NONE][0]['value'];

    $billing_data_details = ptl_get_billing_data($node_uid, $bill_number);

    $api_data['jobData']['trip_id'] = $trip_id;
    $api_data['rs_code'] = $rs_code;
    $api_data['jobData']['vehicle'] = $vehicle;
    $api_data['jobData']['bill_number'] = $bill_number;
    $api_data['jobData']['loading_data'] = $loading_data;
    $api_data['jobData']['store_sequence_no'] = $store_sequence;

    $api_data['jobData']['item_details'] = $billing_data_details; // array

    ptl_data_insert_into_table($api_data, $node_uid);
  }
}

/**
 * Function for  ptl_configuration_data_insert($api_data);
 */
function ptl_data_insert_into_table($api_data, $distributor_id) {
    if (!empty($api_data)) {
        $exist_sku_unit = array();
        $exist_child_arr = array();
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
//                        $derived_cld = (int) ($derived_free_qty / $derived_unit_case); //petty
//                        $derived_loose = (int) ($derived_free_qty % $derived_unit_case); //loose
                        $derived_cld = number_format((($cld * $sku_unit_case * $child_detail['child_qty']) / $derived_unit_case), 2, '.', ''); //petty
                        $derived_loose = $loose * $child_detail['child_qty']; //loose
                    }
                } else {
                    $sum_of_sales_free_qty = $item_data['sum_of_sales_free_qty'];
                    //$free_sku_description =  $item_data['product_name']; // for now
                }
                // New logic end
                $query = db_insert('ptl_data_configuration');
                $fields = array(
                    'ptl_job' => $api_data['jobData']['trip_id'],
                    'rs_code' => $api_data['rs_code'],
                    'trip_id' => $api_data['jobData']['trip_id'],
                    'vehicle' => $api_data['jobData']['vehicle'],
                    'bill_number' => $api_data['jobData']['bill_number'],
                    'loading_date' => $api_data['jobData']['loading_data'],
                    'sequence_no' => $api_data['jobData']['store_sequence_no'],
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

function ptl_get_billing_data($node_uid, $bill_number) {
  $query = db_select('custom_billing_data', 'bd');
  $query->fields('bd', array('tur', 'mrp', 'product_name'));
  $query->addfield('bd', 'Sum_of_NET_SALES_QTY', 'sum_net_sales_quantity');
  $query->addfield('bd', 'Sum_of_SALES_FREE_QTY', 'sum_of_sales_free_qty');
  $query->addfield('bd', 'sku7_code', 'sku_code');
  $query->addfield('bd', 'BASEPACK_CODE', 'basepack_code');
  $query->condition('bd.distributor_id', $node_uid);
  $query->condition('bd.bill_number', $bill_number);
  $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
  return $res;
}

function ptl_get_sales_register_nid_by_trip($trip_id_nid) {
    $query = db_select('field_data_field_trip_id_reference', 'tr');
    $query->addField('tr', 'entity_id');
    $query->condition('tr.field_trip_id_reference_nid', "$trip_id_nid");
    $query->condition('tr.bundle', "sales_register_data");
    //don't send pending bills. means which picking is done against previous trip attached
    $query->leftJoin('field_data_field_previous_trip', 'dp', 'tr.entity_id = dp.entity_id');
    $query->isNull('dp.field_previous_trip_value');
    $sales_register_nid = $query->execute()->fetchAll();
  return $sales_register_nid;
}

function ptl_get_rs_code_by_uid($uid) {
  $query = db_select('field_data_field_third_party_username', 'rs');
  $query->condition('rs.entity_id', $uid);
  $query->fields('rs', array('field_third_party_username_value'));
  $rs_code = $query->execute()->fetchField();
  return $rs_code;
}

#################DRS BOX ID#####################################3

function drs_box_id_sheet_form($form, &$form_state) {
  $form['#prefix'] = '<div id="itemlist-div">';
  $form['#suffix'] = '</div>';

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
      'callback' => 'get_data_of_drs_box_id_sheet_form_submit',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    '#submit' => array('drs_box_id_sheet_form_submit'),
    '#validate' => array('drs_box_id_sheet_form_validate'),
  );
  if (!empty($form_state['views_data'])) {
    $form['hhh'] = array(
      '#type' => 'markup',
      '#markup' => $form_state['views_data'],
    );
    $trip_id = $form_state['values']['trip_id'];
    $rr = l('Open PDF', "/download-drs-box-id/$trip_id", array('attributes'=>array("target" => "_blank")));
    $form['dowmload_mark'] = array(
      '#type' => 'markup',
      '#markup' => $rr,
    );
  }
  return $form;
}

function get_data_of_drs_box_id_sheet_form_submit($form, &$form_state) {
  return $form;
}

function drs_box_id_sheet_form_validate($form, &$form_state) {
  global $user;
  $rs_code = ptl_get_rs_code_by_uid($user->uid);
  $trip_id = $form_state['values']['trip_id'];
  if (!is_numeric($trip_id)) {
    unset($form_state['complete form']['hhh']);
    unset($form_state['complete form']['dowmload_mark']);
    form_set_error('trip_id', "Enter Correct  Integer Value");
  }
  $query1 = db_select('ptl_data_configuration', 'ptl');
  $query1->condition('ptl.trip_id', $trip_id, "=");
  $query1->condition('ptl.rs_code', $rs_code, "=");
  $query1->addExpression('SUM(ptl.put_qty)', 'put_qty');
  $put_qty = $query1->execute()->fetchField();

  $query = db_select('ptl_put_qty', 'put');
  $query->condition('put.type', 'TRIP', "=");
  $query->condition('put.rs_code', $rs_code, "=");
  $query->condition('put.ptl_job', $trip_id, "=");
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
  
  $query1 = db_select('ptl_data_configuration', 'ptl');
  $query1->condition('ptl.trip_id', $trip_id, "=");
  $query1->condition('ptl.rs_code', $rs_code, "=");
  $query1->condition('ptl.box_id', '', "!=");
  $query1->fields('ptl',array('box_id'));
  $query1->range(0, 1);
  $res = $query1->execute()->fetchField();
  
  if (!empty($error_msg) && !empty($res)) {
    unset($form_state['complete form']['hhh']);
    unset($form_state['complete form']['dowmload_mark']);
    form_set_error('master_trip_id', $error_msg);
  }
}

function drs_box_id_sheet_form_submit($form, &$form_state) {
  $trip_id = $form_state['values']['trip_id'];
  $view_name = "drs_report2";
  $view = views_get_view($view_name, '');
  $view->set_display('page_1');
  $view->set_arguments(array($trip_id));
  $rendered_html = $view->render();
  $form_state['views_data'] = $rendered_html;
  $form_state['rebuild'] = TRUE;
}

function download_drs_box_id_sheet($args) {
  $view_name = "drs_report2";
  $view = views_get_view($view_name, '');
  $view->set_display('page_1');
  $view->set_arguments(array($args));
  $rendered_html = $view->render();
  pdf_using_mpdf_api($rendered_html);
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
 * it will call to insert the header data in header table
 */
function insert_ptl_header_data($header_result) {
  $distributor_id = get_distributor_id_using_rscode($header_result[0]->rs_code);
  foreach ($header_result as $value) {
    $result = db_merge('ptl_header_data')
      ->key(array('ptl_job' => $value->ptl_job, 'rs_code' => $value->rs_code, 'type' => 'TRIP'))
      ->fields(array(
        'ptl_job' => $value->ptl_job,
        'bill_lines' => $value->bill_lines,
        'total_qty' => $value->total_qty,
        'rs_code' => $value->rs_code,
        'type' => 'TRIP', // same table diff is trip and master trip
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

function update_ptl_header_data($ptl_job ,$trip_id, $rs_code, $vehicle,$bill_lines,$total_qty,$operator_id,$start_time,$end_time) {

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
      ->condition('type', 'TRIP', '=')
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
 * @param type $ptl_job
 * @param type $rs_code
 * @param type $line_count
 * @param type $put_qty_sum
 * @param type $box_id
 */
function update_ptl_put_qty_sum_data($ptl_job, $id, $rs_code, $api_time, $line_count, $put_qty_sum, $box_id) {
   $distributor_id = get_distributor_id_using_rscode($rs_code);

//    $result = db_insert('ptl_put_qty')
//       ->fields(array(
//         'unique_id' => $id,
//         'ptl_job' => $ptl_job,
//         'line_count' => $line_count,
//         'put_qty_sum' => $put_qty_sum,
//         'rs_code' => $rs_code,
//         'type' => 'TRIP', // same table diff is trip and master trip
//         'created' => $api_time,
//         'box_id' => $box_id ? $box_id : '',
//         'distributor_id' => $distributor_id ? $distributor_id : 0
//       ))
//       ->execute();

    //Newly added merge condition has extra "created" condition for merge
   $result = db_merge('ptl_put_qty')
    ->key(array('unique_id' => $id, 'created' => $api_time, 'rs_code' => $rs_code, 'type' => 'TRIP'))
    ->fields(array(
         'unique_id' => $id,
         'ptl_job' => $ptl_job,
         'line_count' => $line_count,
         'put_qty_sum' => $put_qty_sum,
         'rs_code' => $rs_code,
         'type' => 'TRIP', // same table diff is trip and master trip
         'created' => $api_time,
         'box_id' => $box_id ? $box_id : '',
         'distributor_id' => $distributor_id ? $distributor_id : 0
       ))
       ->execute();

   $results['flag'] = '1';
   $results['result'] = 'Updated Successfully';
   return $results;
}

// PTL BILLWISE PRINTOUT
function ptl_billwise_sheet_form($form, &$form_state) {
  $form['#prefix'] = '<div id="itemlist-div">';
  $form['#suffix'] = '</div>';

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
      'callback' => 'get_data_of_ptl_sheet_billwise_form_submit',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    '#submit' => array('ptl_billwise_sheet_form_submit'),
  );
  if (!empty($form_state['views_data'])) {
    $form['hhh'] = array(
      '#type' => 'markup',
      '#markup' => $form_state['views_data'],
    );
    $trip_id = $form_state['values']['trip_id'];
    $rr = l('Open PDF', "/download-billwise-picking/$trip_id");
    $form['dowmload_mark'] = array(
      '#type' => 'markup',
      '#markup' => $rr,
    );
  }
  return $form;
}

function get_data_of_ptl_sheet_billwise_form_submit($form, &$form_state) {
  return $form;
}

function ptl_billwise_sheet_form_submit($form, &$form_state) {
  $trip_id = $form_state['values']['trip_id'];
  $view_name = "ptl_loose_sheet";
  $view = views_get_view($view_name, '');
  $view->set_display('page_1');
  $view->set_arguments(array($trip_id));
  $rendered_html = $view->render();
  $form_state['views_data'] = $rendered_html;
  $form_state['rebuild'] = TRUE;
}

function download_ptl_billwise($args) {
  $view_name = "ptl_loose_sheet";
  $view = views_get_view($view_name, '');
  $view->set_display('page_1');
  $view->set_arguments(array($args));
  $rendered_html = $view->render();
  pdf_using_mpdf_api($rendered_html);
}

function ptl_get_pending_bill_title_by_trip($trip_id_nid) {
    $query = db_select('field_data_field_trip_id_reference', 'tr');
    $query->join('node', 'n', 'n.nid = tr.entity_id');
    $query->addExpression('GROUP_CONCAT(n.title)', 'titles');
    $query->condition('tr.field_trip_id_reference_nid', "$trip_id_nid");
    $query->condition('tr.bundle', "sales_register_data");
    $query->leftJoin('field_data_field_previous_trip', 'dp', 'tr.entity_id = dp.entity_id');
    $query->isNotNull('dp.field_previous_trip_value');
    $sales_register_title = $query->execute()->fetchField();
  return $sales_register_title;
}

//*************PTL PDF CLD CONFIRMATION REPORT***************
function ptl_cld_confirmation_sheet_form($form, &$form_state) {
  $form['#prefix'] = '<div id="itemlist-div">';
  $form['#suffix'] = '</div>';

  $form['trip_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Trip Id'),
    '#size' => 10,
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('submit'),
    '#submit' => array('ptl_cld_confirmation_sheet_form_submit'),
    '#validate' => array('ptl_cld_confirmation_sheet_form_validate'),
  );
  if (!empty($form_state['views_data'])) {
    $form['hhh'] = array(
      '#type' => 'markup',
      '#markup' => $form_state['views_data'],
    );
    $trip_id = $form_state['values']['trip_id'];
    $rr = l('Open PDF', "/download-cld-confirmation/$trip_id", array('attributes'=>array("target" => "_blank")));
    $form['dowmload_mark'] = array(
      '#type' => 'markup',
      '#markup' => $rr,
    );
  }
  return $form;
}

function ptl_cld_confirmation_sheet_form_submit($form, &$form_state) {
  $trip_id = $form_state['values']['trip_id'];
  $view_name = "ptl_cld_confirmation_sheet";
  $view = views_get_view($view_name, '');
  $view->set_arguments(array($trip_id));
  $rendered_html = $view->render();
  $form_state['views_data'] = $rendered_html;
  $form_state['rebuild'] = TRUE;
}

function download_ptl_cld_confirmation_sheet($args) {
  $view_name = "ptl_cld_confirmation_sheet";
  $view = views_get_view($view_name, '');
  $view->set_arguments(array($args));
  $rendered_html = $view->render();
  pdf_using_mpdf_api($rendered_html);
}

function ptl_cld_confirmation_sheet_form_validate($form, &$form_state) {
  global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);
  $trip_id = $form_state['values']['trip_id'];

  if (!empty($trip_id)) { 
    $query = db_select('dispatch_qty_confirmation', 'dqc');
    $query->condition('dqc.trip_id', $trip_id, '=');
    $query->condition('dqc.distributor_id', $distributor_id, '=');
    $query->condition('dqc.cld_retail_confirmed_flag', 1, '=');
    $query->fields('dqc', array('cld_retail_confirmed_flag'));
    $result = $query->execute()->fetchField();
    if (empty($result)) {
      form_set_error('form', "CLDs for Trip ID $trip_id have not been confirmed");
      drupal_goto('cld-confirmation-sheet');
    }
  }
}
