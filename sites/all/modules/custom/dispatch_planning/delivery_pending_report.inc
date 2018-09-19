<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function delivery_pending_report_form($form, &$form_state) {

  if (isset($_GET['from_dispatch_date'])) {
    $from_dispatch_date = $_GET['from_dispatch_date'];
  }
  if (isset($_GET['to_dispatch_date'])) {
    $to_dispatch_date = $_GET['to_dispatch_date'];
  }
  if (isset($_GET['from_bill_date'])) {
    $from_bill_date = $_GET['from_bill_date'];
  }
  if (isset($_GET['to_bill_date'])) {
    $to_bill_date = $_GET['to_bill_date'];
  }
  if (isset($_GET['bill_num'])) {
    $bill_num = $_GET['bill_num'];
  }
  if (isset($_GET['pending'])) {
    $pending = $_GET['pending'];
  }

  $form['from_dispatch_date'] = array(
    '#type' => 'date_popup',
    '#title' => 'Dispatch From date',
    '#date_label_position' => 'within',
    '#date_format' => 'Y-m-d',
    '#default_value' => $from_dispatch_date,
  );

  $form['to_dispatch_date'] = array(
    '#type' => 'date_popup',
    '#title' => 'Dispatch To date',
    '#date_label_position' => 'within',
    '#date_format' => 'Y-m-d',
    '#default_value' => $to_dispatch_date,
  );

  $form['from_bill_date'] = array(
    '#type' => 'date_popup',
    '#title' => 'Bill From date',
    '#date_label_position' => 'within',
    '#date_format' => 'Y-m-d',
    '#default_value' => $from_bill_date,
  );

  $form['to_bill_date'] = array(
    '#type' => 'date_popup',
    '#title' => 'Bill To date',
    '#date_label_position' => 'within',
    '#date_format' => 'Y-m-d',
    '#default_value' => $to_bill_date,
  );

  $form['bill_num'] = array(
    '#type' => 'textfield',
    '#title' => t('Bill Number'),
    '#size' => 20,
    '#default_value' => $bill_num,
    '#maxlength' => 50,
  );

  $form['pending'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show pending bills'),
    '#default_value' => $pending,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Apply'),
    '#submit' => array('delivery_pending_report_form_submit'),
  );

  return $form;
}

function delivery_pending_report_form_submit($form, &$form_state) {
  $from_dispatch_date = $form_state['values']['from_dispatch_date'];
  $to_dispatch_date = $form_state['values']['to_dispatch_date'];
  $from_bill_date = $form_state['values']['from_bill_date'];
  $to_bill_date = $form_state['values']['to_bill_date'];
  $bill_num = $form_state['values']['bill_num'];
  $pending = $form_state['values']['pending'];

  if (($form_state['values']["op"] == "Apply") && (arg(0) == 'delivery-pending-report')) {
    drupal_goto('delivery-pending-report', array('query' => array('from_dispatch_date' => $from_dispatch_date, 'to_dispatch_date' => $to_dispatch_date, 'from_bill_date' => $from_bill_date, 'to_bill_date' => $to_bill_date, 'bill_num' => $bill_num, 'pending' => $pending)));
  }
}

function delivery_pending_report_callback() {
  global $user;
  $distributor_id = change_feeds_import_get_distributor_id($user->uid);

  if (isset($_GET['from_dispatch_date'])) {
    $from_dispatch_date = strtotime($_GET['from_dispatch_date']);
  }
  if (isset($_GET['to_dispatch_date'])) {
    $to_dispatch_date = strtotime($_GET['to_dispatch_date']);
  }
  if (isset($_GET['from_bill_date'])) {
    $from_bill_date = strtotime($_GET['from_bill_date']);
  }
  if (isset($_GET['to_bill_date'])) {
    $to_bill_date = strtotime($_GET['to_bill_date']);
  }
  if (isset($_GET['bill_num'])) {
    $bill_num = $_GET['bill_num'];
  }
  if (isset($_GET['pending'])) {
    $pending = $_GET['pending'];
  }
  if (arg(1) != 'export') {
    $form = drupal_render(drupal_get_form('delivery_pending_report_form'));
  }

//Find all bills which are DP
  $query = db_select('dispatch_planning_api_data', 'ad');
  $query->condition('ad.distributor_id', $distributor_id, '=');
  if (!empty($bill_num)) {
    $query->condition('ad.bill_number', $bill_num, '=');
  }
  if (!empty($from_dispatch_date) && !empty($to_dispatch_date)) {
    $query->condition('ad.dispatch_date', array($from_dispatch_date, $to_dispatch_date), 'BETWEEN');
  }
  if (!empty($from_bill_date) && !empty($to_bill_date)) {
    $query->condition('ad.bill_date', array($from_bill_date, $to_bill_date), 'BETWEEN');
  }
  $query->condition('ad.delivery_pending_flag', 1, '=');
  $query->fields('ad', array('bill_number'));
  $query->groupBy('ad.bill_number');
  $bills = $query->execute()->fetchCol();

  $query = db_select('dispatch_planning_api_data', 'ad');
  $query->condition('ad.distributor_id', $distributor_id, '=');
  if (!empty($bills)) {
    $query->condition('ad.bill_number', $bills, 'IN');
  }
  $query->fields('ad', array('bill_number'));
  $query->fields('ad', array('bill_date'));
  $query->addexpression("CASE WHEN (ad.delivery_pending_flag = 0 and (ad.res_status IS NOT NULL or ad.res_status !='')) THEN 'Completed' ELSE 'Pending' END", 'attempted_status');
  $query->addexpression("CASE WHEN ad.delivery_pending_flag = 0 THEN ad.res_status END", 'current_bill_status');
  $query->addexpression("CASE WHEN ad.delivery_pending_flag = 0 AND res_status ='undeliver'  THEN ad.res_reason_for_failure END", 'reason_for_failure');
  $query->addexpression("ad.runsheet_number", 'trip_number');
  $query->addexpression("ad.dispatch_date", 'dispatch_date');
  $query->leftjoin('field_data_field_dp_reason', 'dpr', 'dpr.entity_id = ad.delivery_pending_reason');
  $query->fields('dpr', array('field_dp_reason_value'));
  $query->addexpression("ad.fieldExecutiveName", 'fieldExecutiveName');
  $query->join('node', 'n', 'n.title = ad.runsheet_number');
  $query->condition('n.type', 'dispatch_data', '=');
  $query->condition('n.uid', $distributor_id);
  $query->join('field_data_field_vrs_status', 'vs', 'vs.entity_id = n.nid');
  $query->fields('vs', array('field_vrs_status_value'));
  $query->groupBy('ad.bill_number,ad.runsheet_number');
  $query->orderBy('ad.id');
  $result = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
  $arr = array();
  $header_new = array();

  foreach ($result as $val) {
      $arr[$val['bill_number']]['bill_number'] = $val['bill_number'];
      $arr[$val['bill_number']]['bill_date'] = date('d-M-y', $val['bill_date']);
      $arr[$val['bill_number']]['attempted_status'] = $val['attempted_status'];
      $arr[$val['bill_number']]['current_bill_status'] = $val['current_bill_status'];
      $arr[$val['bill_number']]['reason_for_failure'] = $val['reason_for_failure'];
      $arr[$val['bill_number']]['trip_count'] += 1;
      $counter = $arr[$val['bill_number']]['trip_count'];
      $arr[$val['bill_number']]['trip_number' . $counter] = $val['trip_number'];
      $arr[$val['bill_number']]['dispatch_date' . $counter] = date('d-M-y', $val['dispatch_date']);
      $arr[$val['bill_number']]['delivery_pending_reason' . $counter] = $val['field_dp_reason_value'];
      $arr[$val['bill_number']]['fieldExecutiveName' . $counter] = $val['fieldExecutiveName'];
      $field = field_info_field('field_vrs_status');
      $allowed_values = list_allowed_values($field);
      foreach ($allowed_values as $key => $value) {
        if ($key == $val['field_vrs_status_value']) {
          $arr[$val['bill_number']]['vrs_status' . $counter] = $value;
        }
      }
      $header_new['trip_id' . $counter] = 'Trip ' . $counter;
      $header_new['dispatch_date' . $counter] = 'Trip ' . $counter . ' Dispatch Date';
      $header_new['dp_reason' . $counter] = 'Trip ' . $counter . ' DP Reason';
      $header_new['cashier' . $counter] = 'Trip ' . $counter . ' Cashier';
      $header_new['status' . $counter] = 'Trip ' . $counter . ' Status';
  }
  $header = array('Bill Number', 'Bill Date', 'DP Attempted Status', 'Current Bill Status', 'Reason if full returned', 'Count of Trips');
  foreach ($header_new as $val) {
    $header[] = $val;
  }

  foreach ($result as $val) {
    for ($i = 1; $i <= getMax($arr); $i++) {
      if (!isset($arr[$val['bill_number']]['trip_number' . $i])) {
        $arr[$val['bill_number']]['trip_number' . $i] = '';
      }
      if (!isset($arr[$val['bill_number']]['dispatch_date' . $i])) {
        $arr[$val['bill_number']]['dispatch_date' . $i] = '';
      }
      if (!isset($arr[$val['bill_number']]['delivery_pending_reason' . $i])) {
        $arr[$val['bill_number']]['delivery_pending_reason' . $i] = '';
      }
      if (!isset($arr[$val['bill_number']]['fieldExecutiveName' . $i])) {
        $arr[$val['bill_number']]['fieldExecutiveName' . $i] = '';
      }
      if (!isset($arr[$val['bill_number']]['vrs_status' . $i])) {
        $arr[$val['bill_number']]['vrs_status' . $i] = '';
      }
    }
  }
  // Sorting By Pending first and also pending not equal to one condition for not taking pending values of checkbox
  $res_set1 = $res_set2 = array();
  foreach($arr as $key => $val){
    if($val['attempted_status'] == 'Pending'){
      $res_set1[$key] = $val; 
    }else if ($pending != 1) {
      $res_set2[$key] = $val;
    }
  }
  $final_arr = $res_set1 + $res_set2;

  $per_page = 10;
  // Initialize the pager
  $current_page = pager_default_initialize(count($final_arr), $per_page);
  // Split your list into page sized chunks
  $chunks = array_chunk($final_arr, $per_page, TRUE);
  // Show the appropriate items from the list
  $output = theme('table', array('header' => $header, 'rows' => $chunks[$current_page]));
  // Show the pager
  $output .= theme('pager', array('quantity', count($final_arr)));

  $html = theme('table', array('header' => $header, 'rows' => $final_arr, 'attributes' => array('cellpadding' => '5', 'cellspacing' => '0'), 'empty' => t('No Records has been found')));

  cache_set('delivery-pending-report' . session_id(), $html, 'cache', time() + 60 * 10);
  $output .= l(t('XLS'), 'delivery-pending-report/export', array('query' => array($data)));
  return $form . $output;
}

function delivery_pending_export() {
  $file = "Delivery_Pending_Report.xls";
  $data = cache_get('delivery-pending-report' . session_id());
  if (empty($data)) {
    $table = delivery_pending_report_callback();
  }
  else {
    $table = $data->data;
  }
  header("Content-type: application/vnd.ms-excel");
  header("Content-Disposition: attachment; filename=$file");
  echo $table;
}

function getMax($arr) {
  $max = 0;
  foreach ($arr as $k => $v) {
    $max = max(array($max, $v['trip_count']));
  }
  return $max;
}
