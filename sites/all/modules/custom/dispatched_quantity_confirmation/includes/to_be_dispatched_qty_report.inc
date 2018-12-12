<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function to_be_dispatched_qty_form($form, &$form_state) {

  if (isset($_GET['trip_id'])) {
    $trip_id = $_GET['trip_id'];
  }
  //$trip_creation_from_date  = date('Y-m-d');
  if (isset($_GET['trip_creation_from_date'])) {
    $trip_creation_from_date = $_GET['trip_creation_from_date'];
  }
  //$trip_creation_to_date  = date('Y-m-d');
  if (isset($_GET['trip_creation_to_date'])) {
    $trip_creation_to_date = $_GET['trip_creation_to_date'];
  }
  //$trip_execution_from_date  = date('Y-m-d');
  if (isset($_GET['trip_execution_from_date'])) {
    $trip_execution_from_date = $_GET['trip_execution_from_date'];
  }
  //$trip_execution_to_date  = date('Y-m-d');
  if (isset($_GET['trip_execution_to_date'])) {
    $trip_execution_to_date = $_GET['trip_execution_to_date'];
  }
  $bills_edited = 1;
  if (isset($_GET['bills_edited'])) {
    $bills_edited = $_GET['bills_edited'];
  }

  $form['trip_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Trip ID*'),
    '#size' => 20,
    '#default_value' => $trip_id,
    '#maxlength' => 50,
  );
  $form['trip_creation_from_date'] = array(
    '#type' => 'date_popup',
    '#title' => t('Trip Creation From Date*'),
    '#date_format' => 'Y-m-d',
    '#attributes' => array('class' => array('trip_creation_from_date')),
    '#date_label_position' => '',
    '#date_year_range' => '-20:+2',
    '#size' => 30,
    '#default_value' => $trip_creation_from_date,
    '#prefix' => '<div class="form_row form_row2"><div class="col_3">',
    '#suffix' => '</div>'
  );
  $form['trip_creation_to_date'] = array(
    '#type' => 'date_popup',
    '#title' => t('Trip Creation To Date*'),
    '#date_format' => 'Y-m-d',
    '#attributes' => array('class' => array('trip_creation_to_date')),
    '#date_label_position' => '',
    '#date_year_range' => '-20:+2',
    '#size' => 30,
    '#default_value' => $trip_creation_to_date,
    '#prefix' => '<div class="form_row form_row2"><div class="col_3">',
    '#suffix' => '</div></div>'
  );

  $form['trip_execution_from_date'] = array(
    '#type' => 'date_popup',
    '#title' => t('Trip Execution From Date*'),
    '#date_format' => 'Y-m-d',
    '#attributes' => array('class' => array('trip_execution_from_date')),
    '#date_label_position' => '',
    '#date_year_range' => '-20:+2',
    '#size' => 30,
    '#default_value' => $trip_execution_from_date,
    '#prefix' => '<div class="form_row form_row2"><div class="col_3">',
    '#suffix' => '</div>'
  );
  $form['trip_execution_to_date'] = array(
    '#type' => 'date_popup',
    '#title' => t('Trip Execution To Date*'),
    '#date_format' => 'Y-m-d',
    '#attributes' => array('class' => array('trip_execution_from_date')),
    '#date_label_position' => '',
    '#date_year_range' => '-20:+2',
    '#size' => 30,
    '#default_value' => $trip_execution_to_date,
    '#prefix' => '<div class="form_row form_row2"><div class="col_3">',
    '#suffix' => '</div></div>'
  );

  $form['bills_edited'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Bills with edited quantity only'),
    '#default_value' => $bills_edited,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Apply'),
    '#submit' => array('to_be_dispatched_qty_form_submit'),
  );

  return $form;
}

function to_be_dispatched_qty_form_validate($form, &$form_state) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);
  $trip_id = $form_state['values']['trip_id'];
  $trip_creation_from_date = strtotime($form_state['values']['trip_creation_from_date']);
  $trip_creation_to_date = strtotime($form_state['values']['trip_creation_to_date']);
  $trip_execution_from_date = strtotime($form_state['values']['trip_execution_from_date']);
  $trip_execution_to_date = strtotime($form_state['values']['trip_execution_to_date']);
  $bills_edited = $form_state['values']['bills_edited'];

  if (!empty($trip_creation_from_date) && empty($trip_creation_to_date)) {
    form_set_error('form', 'Please enter Trip Creation To Date');
    drupal_goto('to-be-dispatched-qty-report');
  }
  if (!empty($trip_creation_to_date) && empty($trip_creation_from_date)) {
    form_set_error('form', 'Please enter Trip Creation From Date');
    drupal_goto('to-be-dispatched-qty-report');
  }
  if (!empty($trip_execution_from_date) && empty($trip_execution_to_date)) {
    form_set_error('form', 'Please enter Trip Execution To Date');
    drupal_goto('to-be-dispatched-qty-report');
  }
  if (!empty($trip_execution_to_date) && empty($trip_execution_from_date)) {
    form_set_error('form', 'Please enter Trip Execution From Date');
    drupal_goto('to-be-dispatched-qty-report');
  }
  if (empty($trip_id) && empty($trip_creation_from_date) && empty($trip_creation_to_date) && empty($trip_execution_from_date) && empty($trip_execution_to_date)) {
    form_set_error('form', "Please fill any of the fields");
    drupal_goto('to-be-dispatched-qty-report');
  }
  if ((!empty($trip_creation_from_date) && !empty($trip_creation_to_date)) || (!empty($trip_execution_from_date) && !empty($trip_execution_to_date))) {
    $trips = get_trips_from_trip_creation_execution_date($trip_creation_from_date, $trip_creation_to_date, $trip_execution_from_date, $trip_execution_to_date, $dis_id);
  }
  if (!empty($trip_id) || !empty($trips)) {
    $query = db_select('dispatch_qty_confirmation', 'dqc');
    if (!empty($trip_id)) {
      $query->condition('dqc.trip_id', $trip_id);
    }
    if (!empty($trips)) {
      $query->condition('dqc.trip_id', $trips, 'IN');
    }
    $query->condition('dqc.distributor_id', $dis_id);
    
    $db_or = db_or();
    $db_or->condition('dqc.cld_close_flag', 1, '!=');
    $db_or->condition('dqc.loose_close_flag', 1, '!=');
    $query->condition($db_or);
    
    $query->fields('dqc', array('trip_id'))->distinct();
    $result = $query->execute()->fetchCol();
    if (!empty($result)) {
      $trips_res = implode(',', $result);
      form_set_error('form', "Loose/CLD Confirmation is pending for trip id :" . $trips_res);
      drupal_goto('to-be-dispatched-qty-report');
    }
  }
  else {
    form_set_error('form', "No data found");
    drupal_goto('to-be-dispatched-qty-report');
  }
}

function to_be_dispatched_qty_form_submit($form, &$form_state) {
  $trip_id = $form_state['values']['trip_id'];
  $trip_creation_from_date = $form_state['values']['trip_creation_from_date'];
  $trip_creation_to_date = $form_state['values']['trip_creation_to_date'];
  $trip_execution_from_date = $form_state['values']['trip_execution_from_date'];
  $trip_execution_to_date = $form_state['values']['trip_execution_to_date'];
  $bills_edited = $form_state['values']['bills_edited'];

  if (($form_state['values']["op"] == "Apply") && (arg(0) == 'to-be-dispatched-qty-report')) {
    drupal_goto('to-be-dispatched-qty-report', array('query' => array('trip_id' => $trip_id, 'trip_creation_from_date' => $trip_creation_from_date, 'trip_creation_to_date' => $trip_creation_to_date, 'trip_execution_from_date' => $trip_execution_from_date, 'trip_execution_to_date' => $trip_execution_to_date, 'bills_edited' => $bills_edited)));
  }
}

function to_be_dispatched_qty_callback() {

  global $user;
  $distributor_id = change_feeds_import_get_distributor_id($user->uid);
  if (isset($_GET['trip_id'])) {
    $trip_id = $_GET['trip_id'];
  }
  //$trip_creation_from_date = date('Y-m-d');
  if (isset($_GET['trip_creation_from_date'])) {
    $trip_creation_from_date = strtotime($_GET['trip_creation_from_date']);
  }
  //$trip_creation_to_date = date('Y-m-d');
  if (isset($_GET['trip_creation_to_date'])) {
    $trip_creation_to_date = strtotime($_GET['trip_creation_to_date']);
  }
  //$trip_execution_from_date = date('Y-m-d');
  if (isset($_GET['trip_execution_from_date'])) {
    $trip_execution_from_date = strtotime($_GET['trip_execution_from_date']);
  }
  //$trip_execution_from_date = date('Y-m-d');
  if (isset($_GET['trip_execution_to_date'])) {
    $trip_execution_to_date = strtotime($_GET['trip_execution_to_date']);
  }
  if (isset($_GET['bills_edited'])) {
    $bills_edited = $_GET['bills_edited'];
  }

  if (arg(1) != 'export') {
    $form = drupal_render(drupal_get_form('to_be_dispatched_qty_form'));
  }

  if (isset($trip_id)) {
    //function to get trips from trip created date and trip execution date 
    if ((!empty($trip_creation_from_date) && !empty($trip_creation_to_date)) || (!empty($trip_execution_from_date) && !empty($trip_execution_to_date))) {
      $trips = get_trips_from_trip_creation_execution_date($trip_creation_from_date, $trip_creation_to_date, $trip_execution_from_date, $trip_execution_to_date, $distributor_id);
    }
//main query
    $query = db_select('dispatch_qty_confirmation', 'dqc');
    if (!empty($trip_id)) {
      $query->condition('dqc.trip_id', $trip_id, '=');
    }
    if (!empty($trips)) {
      $query->condition('dqc.trip_id', $trips, 'IN');
    }
    if ($bills_edited == 1) {
      $query->addExpression("dqc.original_qty - dqc.to_be_dispatch_qty", 'diff_qty');
      $query->havingCondition('diff_qty', 0, '>');
    }
    $query->condition('dqc.distributor_id', $distributor_id);
    $query->fields('dqc', array('trip_id', 'bill_number', 'sku_code', 'product_name', 'original_qty', 'to_be_dispatch_qty'));
    $result = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
    $row = array();
    foreach ($result as $val) {
      $row['trip_id'] = $val['trip_id'];
      $row['bill_number'] = $val['bill_number'];
      $row['sku_code'] = $val['sku_code'];
      $row['product_name'] = $val['product_name'];
      $row['original_qty'] = $val['original_qty'];
      $row['to_be_dispatch_qty'] = $val['to_be_dispatch_qty'];
      $rows[] = $row;
    }
    $header = array('Trip ID', 'Bill Number', 'SKU Code', 'Product Name', 'Original qty', 'To be dispatched quantity');
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

    cache_set('to-be-dispatched-qty-report' . session_id(), $html, 'cache', time() + 60 * 10);
    $output .= l(t('XLS'), 'to-be-dispatched-qty-report/export', array('query' => array($data)));
  }
  return $form . $output;
}

function to_be_dispatched_qty_export() {
  $file = "To_be_dispatched_Quantity_Report.xls";
  $data = cache_get('to-be-dispatched-qty-report' . session_id());
  if (empty($data)) {
    $table = to_be_dispatched_qty_callback();
  }
  else {
    $table = $data->data;
  }
  header("Content-type: application/vnd.ms-excel");
  header("Content-Disposition: attachment; filename=$file");
  echo $table;
}

function get_trips_from_trip_creation_execution_date($trip_creation_from_date, $trip_creation_to_date, $trip_execution_from_date, $trip_execution_to_date, $distributor_id) {
  $query = db_select('node', 'n');
  $query->condition('n.type', 'dispatch_data');
  $query->condition('n.uid', $distributor_id);
  $query->join('field_data_field_trip_creation_date', 'tc', 'tc.entity_id = n.nid');
  $query->join('field_data_field_trip_executon_date', 'te', 'te.entity_id = n.nid');
  if (!empty($trip_creation_from_date) && !empty($trip_creation_to_date)) {
    $query->condition('tc.field_trip_creation_date_value', array($trip_creation_from_date, $trip_creation_to_date), 'BETWEEN');
  }
  if (!empty($trip_execution_from_date) && !empty($trip_execution_to_date)) {
    $query->condition('te.field_trip_executon_date_value', array($trip_execution_from_date, $trip_execution_to_date), 'BETWEEN');
  }
  $query->fields('n', array('title'));
  $trips = $query->execute()->fetchCol();
  return $trips;
}
