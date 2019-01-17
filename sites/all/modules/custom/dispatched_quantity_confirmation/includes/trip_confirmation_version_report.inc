<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function trip_confirmation_version_form($form, &$form_state) {

  if (isset($_GET['trip_id'])) {
    $trip_id = $_GET['trip_id'];
  }

  $dispatch_from_date = $dispatch_to_date = date('Y-m-d');
  if (isset($_GET['dispatch_from_date'])) {
    $dispatch_from_date = $_GET['dispatch_from_date'];
  }
  
  if (isset($_GET['dispatch_to_date'])) {
    $dispatch_to_date = $_GET['dispatch_to_date'];
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
  $form['dispatch_from_date'] = array(
    '#type' => 'date_popup',
    '#title' => t('Dispatch From Date'),
    '#date_format' => 'Y-m-d',
    '#attributes' => array('class' => array('dispatch_date')),
    '#date_label_position' => '',
    '#date_year_range' => '-20:+2',
    '#size' => 30,
    '#default_value' => $dispatch_from_date,
    '#prefix' => '<div class="col_3">',
    '#suffix' => '</div>'
  );

  $form['dispatch_to_date'] = array(
    '#type' => 'date_popup',
    '#title' => t('Dispatch To Date'),
    '#date_format' => 'Y-m-d',
    '#attributes' => array('class' => array('dispatch_date')),
    '#date_label_position' => '',
    '#date_year_range' => '-20:+2',
    '#size' => 30,
    '#default_value' => $dispatch_to_date,
    '#prefix' => '<div class="col_3">',
    '#suffix' => '</div>'
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Apply'),
    '#submit' => array('trip_confirmation_version_form_submit'),
  );

  return $form;
}

function trip_confirmation_version_form_submit($form, &$form_state) {
  $dispatch_from_date = $form_state['values']['dispatch_from_date'];
  $dispatch_to_date = $form_state['values']['dispatch_to_date'];
  $trip_id = $form_state['values']['trip_id'];

  if (($form_state['values']["op"] == "Apply") && (arg(0) == 'trip-confirmation-version-report')) {
    drupal_goto('trip-confirmation-version-report', array('query' => array('dispatch_from_date' => $dispatch_from_date, 'dispatch_to_date' => $dispatch_to_date, 'trip_id' => $trip_id)));
  }
}

function trip_confirmation_version_callback() {

  global $user;
  $distributor_id = change_feeds_import_get_distributor_id($user->uid);
  $dispatch_from_date = $dispatch_to_date = date('Y-m-d');

  if (isset($_GET['trip_id'])) {
    $trip_id = $_GET['trip_id'];
  }

  if (isset($_GET['dispatch_from_date'])) {
    $dispatch_from_date = $_GET['dispatch_from_date'];
  }

  if (isset($_GET['dispatch_to_date'])) {
    $dispatch_to_date = $_GET['dispatch_to_date'];
  }

  if (arg(1) != 'export') {
    $form = drupal_render(drupal_get_form('trip_confirmation_version_form'));
  }

  ######################
  $trips = get_trips_from_dispatch_date($dispatch_from_date, $dispatch_to_date, $distributor_id);
  if (!empty($trip_id) || !empty($trips)) {
    $query = db_select('dispatch_qty_confirmation', 'dqc');
    $query->condition('dqc.distributor_id', $distributor_id, '=');
    if (!empty($trip_id)) {
      $query->condition('dqc.trip_id', $trip_id, '=');
    }
    else if (!empty($trips)) {
      $query->condition('dqc.trip_id', $trips, 'IN');
    }
    $query->leftjoin('dispatch_qty_confirmation_versioning', 'dqcv', "dqc.trip_id = dqcv.trip_id and dqc.bill_number = dqcv.bill_number and dqc.sku_code = dqcv.sku_code and dqc.distributor_id = dqcv.distributor_id");
    $query->fields('dqc', array('trip_id'));
    $query->addexpression("MAX(CASE WHEN dqcv.version_type = 'Loose' THEN dqcv.version_no END)", 'loose_version_no');
    $query->addexpression("MAX(CASE WHEN dqcv.version_type = 'Loose' AND dqcv.version_created_time != 0 THEN dqcv.version_created_time ELSE dqc.loose_created_date END)", 'loose_version_time');
    $query->addexpression("MAX(CASE WHEN dqcv.version_type = 'CLD' THEN dqcv.version_no END)", 'cld_version_no');
    $query->addexpression("MAX(CASE WHEN dqcv.version_type = 'CLD' AND dqcv.version_created_time != 0 THEN dqcv.version_created_time ELSE dqc.cld_created_date END)", 'cld_version_time');
    $query->leftjoin('difference_table_versioning', 'diff', "dqc.trip_id = diff.trip_id AND dqc.bill_number = diff.bill_number and dqc.sku_code = diff.sku_code and dqc.distributor_id = diff.distributor_id");
    $query->join('node', 'n', "n.title = dqc.trip_id and dqc.distributor_id = n.uid");
    $query->condition('n.type','dispatch_data');
    $query->leftjoin('field_data_field_confirm_bill_edit_time', 'conf', "n.nid = conf.entity_id");
    $query->addexpression("MAX(diff.version_no)", 'version_no');
    $query->addexpression("MAX(CASE WHEN diff.version_created_time != 0 THEN diff.version_created_time ELSE conf.field_confirm_bill_edit_time_value END)", 'diff_version_time');
    $query->leftjoin('excess_qty_versioning', 'exc', "dqc.trip_id = exc.trip_id and dqc.bill_number = exc.bill_number and dqc.sku_code = exc.sku_code and dqc.distributor_id = exc.distributor_id");
    $query->addexpression("MAX(exc.version_no)", 'exc_version_no');
    $query->addexpression("MAX(CASE WHEN exc.version_created_time != 0 THEN exc.version_created_time ELSE conf.field_confirm_bill_edit_time_value END)", 'exc_version_time');
    $query->groupBy('dqc.trip_id');
    $result = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
  }

  foreach ($result as $val) {
    $row['trip_id'] = $val['trip_id'];
    $row['loose_version_no'] = $val['loose_version_no'] ? $val['loose_version_no'] : 'V_1.0';
    if (!empty($val['loose_version_time'])) {
      $row['loose_version_time'] = date('d-M-y H:i:s', $val['loose_version_time']);
    }
    else {
      $row['loose_version_time'] = '';
    }
    $row['cld_version_no'] = $val['cld_version_no'] ? $val['cld_version_no'] : 'V_1.0';
    if (!empty($val['cld_version_time'])) {
      $row['cld_version_time'] = date('d-M-y H:i:s', $val['cld_version_time']);
    }
    else {
      $row['cld_version_time'] = '';
    }
    $row['version_no'] = $val['version_no'] ? $val['version_no'] : 'V_1.0';
    if (!empty($val['diff_version_time'])) {
      $row['diff_version_time'] = date('d-M-y H:i:s', $val['diff_version_time']);
    }
    else {
      $row['diff_version_time'] = '';
    }
    $row['exc_version_no'] = $val['exc_version_no'] ? $val['exc_version_no'] : 'V_1.0';
    if (!empty($val['exc_version_time'])) {
      $row['exc_version_time'] = date('d-M-y H:i:s', $val['exc_version_time']);
    }
    else {
      $row['exc_version_time'] = '';
    }
    $rows[] = $row;
  }
  ######################
  $header = array('Trip ID', 'Loose Version', 'Loose Version Time', 'CLD Version', 'CLD Version Time', 'Difference Qty Version', 'Difference Qty Version Time', 'Excess Qty Version', 'Excess Qty Version Time');
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

  cache_set('trip-confirmation-version-report' . session_id(), $html, 'cache', time() + 60 * 10);
  $output .= l(t('XLS'), 'trip-confirmation-version-report/export', array('query' => array($data)));
  return $form . $output;
}

function trip_confirmation_version_export() {
  $file = "Trip-Confirmation-Version-Report.xls";
  $data = cache_get('trip-confirmation-version-report' . session_id());
  if (empty($data)) {
    $table = trip_confirmation_version_callback();
  }
  else {
    $table = $data->data;
  }
  header("Content-type: application/vnd.ms-excel");
  header("Content-Disposition: attachment; filename=$file");
  echo $table;
}

function get_trips_from_dispatch_date($dispatch_from_date, $dispatch_to_date, $distributor_id) {
 if (!empty($dispatch_from_date) && !empty($dispatch_to_date)) {  
  $query = db_select('node', 'n');
  $query->condition('n.type', 'dispatch_data');
  $query->condition('n.uid', $distributor_id);
  $query->join('field_data_field_dispatch_date', 'dd', 'dd.entity_id = n.nid');
  $query->condition('dd.field_dispatch_date_value', array($dispatch_from_date, $dispatch_to_date), 'BETWEEN');
  $query->fields('n', array('title'));
  $trips = $query->execute()->fetchCol();
 }
  return $trips;
}