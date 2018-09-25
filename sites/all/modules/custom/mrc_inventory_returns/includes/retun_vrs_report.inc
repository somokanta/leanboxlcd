<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function retun_vrs_form($form, &$form_state) {

  if (isset($_GET['trip_id'])) {
    $trip_id = $_GET['trip_id'];
  }
  
  $dispatch_date = date('Y-m-d');
  if (isset($_GET['dispatch_date'])) {
    $dispatch_date = $_GET['dispatch_date'];
  }

  $form['trip_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Trip ID*'),
    '#size' => 20,
    '#default_value' => $trip_id,
    '#maxlength' => 50,
    '#attributes' => array('class' => array('trip-id'), 'id' => 'trip-id'),
    '#prefix' => '<div class="form_row form_row2"><div class="col_3">',
  );
  $form['trip_id_wrapper']['trip_id'] = array(
    '#type' => 'markup',
    '#title' => t('OR'),
    '#prefix' => '<div id="after_trip_id" class="text-center font-weight-bold or_txt or_txt1"> OR',
    '#suffix' => '</div></div>'
  );
  $form['dispatch_date'] = array(
    '#type' => 'date_popup',
    '#title' => t('Dispatch Date'),
    '#date_format' => 'Y-m-d',
    '#attributes' => array('class' => array('dispatch_date')),
    '#date_label_position' => '',
    '#date_year_range' => '-20:+2',
    '#size' => 30,
    '#default_value' => $dispatch_date,
    '#prefix' => '<div class="col_3">',
    '#suffix' => '</div>'
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Apply'),
    '#submit' => array('retun_vrs_form_submit'),
  );

  return $form;
}

function retun_vrs_form_submit($form, &$form_state) {
  $dispatch_date = $form_state['values']['dispatch_date'];
  $trip_id = $form_state['values']['trip_id'];

  if (($form_state['values']["op"] == "Apply") && (arg(0) == 'return-vrs-time-report')) {
    drupal_goto('return-vrs-time-report', array('query' => array('dispatch_date' => $dispatch_date, 'trip_id' => $trip_id)));
  }
}

function return_vrs_callback() {

  global $user;
  $distributor_id = change_feeds_import_get_distributor_id($user->uid);
  $dispatch_date = date('Y-m-d');

  if (isset($_GET['trip_id'])) {
    $trip_id = $_GET['trip_id'];
  }

  if (isset($_GET['dispatch_date'])) {
    $dispatch_date = $_GET['dispatch_date'];
  }

  if (arg(1) != 'export') {
    $form = drupal_render(drupal_get_form('retun_vrs_form'));
  }
  
  $get_subdivision_nids = check_product_validation($distributor_id);

  $query = db_select('dispatch_planning_api_data', 'ad');
  $query->condition('ad.distributor_id', $distributor_id, '=');
  //$query->condition('ad.updated_flag', 1, '=');
  if (!empty($dispatch_date)) {
    $query->condition('ad.date', $dispatch_date, '=');
  }
  if (!empty($trip_id)) {
    $query->condition('ad.runsheet_number', $trip_id, '=');
  }
  
  $query->fields('ad', array('runsheet_number'));
  $query->fields('ad', array('date'));
  $query->fields('ad', array('fieldExecutiveName'));
  $query->leftjoin('node', 'n1', "n1.title = ad.runsheet_number and n1.type ='assets_count' and n1.uid = $distributor_id");
  $query->leftjoin('field_data_field_received_asset_time', 'at', 'at.entity_id = n1.nid');
  $query->fields('at', array('field_received_asset_time_value'));
  $query->join('node', 'disp_data', 'disp_data.title = ad.runsheet_number');
  $query->condition('disp_data.type', 'dispatch_data');
  $query->condition('disp_data .uid', $distributor_id);
  $query->join('field_data_field_vrs_status', 'vs', 'vs.entity_id = disp_data.nid');
  $query->fields('vs', array('field_vrs_status_value'));
  $query->leftjoin('field_data_field_dispatch_start_time', 'dst', 'dst.entity_id = disp_data.nid');
  $query->fields('dst', array('field_dispatch_start_time_value'));
  $query->addExpression('MIN(CASE WHEN (ad.updated_flag = 1) THEN ad.updated_time END)', 'min_updated_time');
  $query->addExpression('MAX(CASE WHEN (ad.updated_flag = 1) THEN ad.updated_time END)', 'max_updated_time');
  $query->addExpression('(MAX(CASE WHEN (ad.updated_flag = 1) THEN ad.updated_time END) - MIN(CASE WHEN (ad.updated_flag = 1) THEN ad.updated_time END))', 'total_return_time');
  $query->groupBy('ad.runsheet_number');
  $query->join('dispatch_planning_item_details', 'idt', 'ad.id = idt.trip_bill_reference');
  $query->addexpression("SUM(idt.return_qty * idt.sum_net_sales_value)", 'return_value');
  $query->addexpression("(SUM(CASE WHEN (ad.updated_flag = 1) THEN (idt.return_qty * idt.sum_net_sales_value * 10) END)) / ((MAX(CASE WHEN (ad.updated_flag = 1) THEN ad.updated_time END) - MIN(CASE WHEN (ad.updated_flag = 1) THEN ad.updated_time END)) / 60)", 'return_prod');
  $query->leftjoin('vrs_trip_total_history', 'vtth', 'ad.runsheet_number = vtth.trip_id AND ad.distributor_id = vtth.distributor_id');
  $query->addExpression("CASE WHEN vtth.vrs_version_no = 'V1.0' THEN vtth.vrs_start_time END", 'vrs_start_time');
  $query->leftjoin('vrs_trip_total', 'vtt', 'ad.runsheet_number = vtt.trip_id AND ad.distributor_id = vtt.distributor_id');
  $query->fields('vtt', array('vrs_end_time'));
  $query->addexpression("vtt.vrs_end_time - (CASE WHEN vtth.vrs_version_no = 'V1.0' THEN vtth.vrs_start_time END)", 'vrs_time');
  $query->fields('vtt', array('total_actual_cash_amount', 'total_actual_cheque_amount', 'total_actual_signed_bill_amount'));
  
  $query->join('node', 'n2', "n2.title = idt.sku_code");
  $query->condition('n2.type', 'sku7_code_master', '=');
  $query->condition('n2.uid',$distributor_id);
  $query->leftjoin('field_data_field_product_devision_reference', 'pdr', 'pdr.entity_id = n2.nid');
  $query->addexpression("SUM(CASE WHEN pdr.field_product_devision_reference_nid IN ($get_subdivision_nids) THEN idt.return_qty END)", 'return_sku_qty_sachets');
  $query->addexpression("SUM(CASE WHEN pdr.field_product_devision_reference_nid NOT IN ($get_subdivision_nids) THEN idt.return_qty END)", 'return_sku_qty_wo_sachets');
  
  $result = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
  $row = array();
  foreach ($result as $val) {
    $row['trip_id'] = $val['runsheet_number'];
    $dispatch_date = strtotime($val['date']); 
    if(isset($dispatch_date)){
      $row['dispatch_date'] = date('d-M-y', $dispatch_date);
    }else{
      $row['dispatch_date'] = '';
    }
    $row['fieldExecutiveName'] = $val['fieldExecutiveName'];
    $field = field_info_field('field_vrs_status');
     $allowed_values = list_allowed_values($field);
     foreach ($allowed_values as $key => $value) {
       if ($key == $val['field_vrs_status_value']) {
         $row['field_vrs_status_value'] = $value;
       }
     }
    $row['field_dispatch_start_time_value'] = $val['field_dispatch_start_time_value'];
    if(isset($val['field_received_asset_time_value'])){
      $row['field_received_asset_time_value'] = date('d-M-y H:i:s', $val['field_received_asset_time_value']);
    }else{
      $row['field_received_asset_time_value'] = '';
    }
    
    //Delivery Confirmation time
    $query = db_select('inventory_versioning_api_data', 'iad');
    $query->condition('iad.distributor_id', $distributor_id, '=');
    $query->condition('iad.version_flag', 1, '=');
    $query->condition('iad.runsheet_number', $val['runsheet_number'], '=');
    $query->fields('iad', array('version_created_time'));
    $query->orderBy('iad.id','DESC');
    $query->range(0, 1);
    $version_created_time = $query->execute()->fetchField();

    if (!empty($version_created_time)) {
      $row['version_created_time'] = date('d-M-y H:i:s', $version_created_time);
    }
    else {
      $row['version_created_time'] = '';
    }
    //Delivery Confirmation time
    
    if(isset($val['min_updated_time'])){
      $row['min_updated_time'] = date('d-M-y H:i:s', $val['min_updated_time']);
    }else{
      $row['min_updated_time'] = '';
    }
    if(isset($val['max_updated_time'])){
    $row['max_updated_time'] = date('d-M-y H:i:s', $val['max_updated_time']);
    }else{
      $row['max_updated_time'] = '';
    }
    if(isset($val['total_return_time'])){
    $row['total_return_time'] = gmdate('H:i:s', $val['total_return_time']);
    }else{
      $row['total_return_time'] = '';
    }
    $row['return_value'] = round($val['return_value']);
    $row['return_qty_wo_sachets'] = $val['return_sku_qty_wo_sachets'];
    $row['return_qty_sachets'] = $val['return_sku_qty_sachets'];
    $row['return_prod'] = round($val['return_prod']);
    if(!empty($val['vrs_start_time'])){
    $row['vrs_start_time'] = date('d-M-y H:i:s', $val['vrs_start_time']);
    }else{
      $row['vrs_start_time'] = '';
    }
    if(!empty($val['vrs_end_time'])){
    $row['vrs_end_time'] = date('d-M-y H:i:s', $val['vrs_end_time']);
    }else{
      $row['vrs_end_time'] = '';
    }  
    if(!empty($val['vrs_time'])){
    $row['vrs_time'] = gmdate('H:i:s', $val['vrs_time']);
    }else{
      $row['vrs_time'] = '';
    }
    $row['total_actual_cash_amount'] = $val['total_actual_cash_amount'];
    $row['total_actual_cheque_amount'] = $val['total_actual_cheque_amount'];
    $row['total_actual_signed_bill_amount'] = $val['total_actual_signed_bill_amount'];
    $rows[] = $row;
  }

  $header = array('Trip ID', 'Dispatch Date', 'Cashier Name','Status', 'Dispatch Time', 'Received Assets Time', 'Delivery Confirmation Time' , 'Returns Start Time', 'Returns End Time', 'Total Returns Time taken', 'Total Returned Value','Returned SKU Quantity(w/o Sachets)','Returned Quantity of Sachets','Returns Productivity (Value/10 min)', 'VRS Start Time', 'VRS End Time', 'Total VRS Time taken', 'Cash Value', 'Cheque Value', 'Signed Bill Value');
  $per_page = 10;
  // Initialize the pager
  $current_page = pager_default_initialize(count($rows), $per_page);
  // Split your list into page sized chunks
  $chunks = array_chunk($rows, $per_page, TRUE);
  // Show the appropriate items from the list
  $output = theme('table', array('header' => $header, 'rows' => $chunks[$current_page]));
  // Show the pager
  $output .= theme('pager', array('quantity', count($rows)));

  $html = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('width' => '100%')));

  cache_set('return-vrs-time-report' . session_id(), $html, 'cache', time() + 60 * 10);
  $output .= l(t('XLS'), 'return-vrs-time-report/export', array('query' => array($data)));
  return $form . $output;
}

function return_vrs_export() {
  $file = "Return_VRS_Report.xls";
  $data = cache_get('return-vrs-time-report' . session_id());
  if (empty($data)) {
    $table = return_vrs_callback();
  }
  else {
    $table = $data->data;
  }
  header("Content-type: application/vnd.ms-excel");
  header("Content-Disposition: attachment; filename=$file");
  echo $table;
}
