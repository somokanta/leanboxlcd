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

  if (isset($_GET['trip_id'])) {
    $trip_id = $_GET['trip_id'];
  }

  if (isset($_GET['dispatch_date'])) {
    $dispatch_date = $_GET['dispatch_date'];
  }

  if (arg(1) != 'export') {
    $form = drupal_render(drupal_get_form('retun_vrs_form'));
  }

  $query = db_select('dispatch_planning_api_data', 'ad');
  $query->condition('ad.distributor_id', $distributor_id, '=');
  $query->condition('ad.updated_flag', 1, '=');
  if (!empty($dispatch_date)) {
    $query->condition('ad.date', $dispatch_date, '=');
  }
  if (!empty($trip_id)) {
    $query->condition('ad.runsheet_number', $trip_id, '=');
  }

  $query->fields('ad', array('runsheet_number'));
  $query->addExpression('MIN(updated_time)', 'min_updated_time');
  $query->addExpression('MAX(updated_time)', 'max_updated_time');
  $query->addexpression("MAX(updated_time) - MIN(updated_time)", 'total_return_time');
  $query->groupBy('ad.runsheet_number');
  $query->join('dispatch_planning_item_details', 'idt', 'ad.id = idt.trip_bill_reference');
  $query->addexpression("SUM(idt.return_qty * idt.sum_net_sales_value)", 'return_value');
  $query->addexpression("((SUM(idt.return_qty * idt.sum_net_sales_value) * 10) / ((MAX(updated_time) - MIN(updated_time))/60))", 'return_prod');
  $query->join('vrs_trip_total_history', 'vtth', 'ad.runsheet_number = vtth.trip_id AND ad.distributor_id = vtth.distributor_id');
  $query->condition('vtth.vrs_version_no', 'V1.0', '=');
  $query->fields('vtth', array('vrs_start_time'));
  $query->join('vrs_trip_total', 'vtt', 'ad.runsheet_number = vtt.trip_id AND ad.distributor_id = vtt.distributor_id');
  $query->fields('vtt', array('vrs_end_time'));
  $query->addexpression("vtt.vrs_end_time - vtth.vrs_start_time", 'vrs_time');
  $query->fields('vtt', array('total_actual_cash_amount', 'total_actual_cheque_amount', 'total_actual_signed_bill_amount'));
  $result = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
  $row = array();
  foreach ($result as $val) {
    $row['trip_id'] = $val['runsheet_number'];
    $row['min_updated_time'] = date('d-M-y H:i:s', $val['min_updated_time']);
    $row['max_updated_time'] = date('d-M-y H:i:s', $val['max_updated_time']);
    $row['total_return_time'] = gmdate('H:i:s', $val['total_return_time']);
    $row['return_value'] = round($val['return_value']);
    $row['return_prod'] = round($val['return_prod']);
    $row['vrs_start_time'] = date('d-M-y H:i:s', $val['vrs_start_time']);
    $row['vrs_end_time'] = date('d-M-y H:i:s', $val['vrs_end_time']);
    $row['vrs_time'] = gmdate('H:i:s', $val['vrs_time']);
    $row['total_actual_cash_amount'] = $val['total_actual_cash_amount'];
    $row['total_actual_cheque_amount'] = $val['total_actual_cheque_amount'];
    $row['total_actual_signed_bill_amount'] = $val['total_actual_signed_bill_amount'];
    $rows[] = $row;
  }

  $header = array('Trip ID', 'Returns Start Time', 'Returns End Time', 'Total Returns Time taken', 'Total Returned Value', 'Returns Productivity (Value/10 min)', 'VRS Start Time', 'VRS End Time', 'Total VRS Time taken', 'Cash Value', 'Cheque Value', 'Signed Bill Value');
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