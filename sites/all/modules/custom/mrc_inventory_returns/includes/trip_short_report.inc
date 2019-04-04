<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
function trip_short_excess_form($form, &$form_state) {
  //$form['#attached']['js'][] = drupal_get_path('module', 'mrc_inventory_returns') . '/js/tripshort.js';
  $form['#attached']['css'][] = drupal_get_path('module', 'mrc_inventory_returns') . '/css/vrs_confirmation.css';

  if (isset($_GET['trip_id'])) {
    $trip_id = $_GET['trip_id'];
  }

  //$dispatch_date = date('Y-m-d');
  if (isset($_GET['dispatch_date'])) {
    $dispatch_date = $_GET['dispatch_date'];
  }
  //$confirm_date = date('Y-m-d');
  if (isset($_GET['confirm_date'])) {
    $confirm_date = $_GET['confirm_date'];
  }
  if (isset($_GET['trip_status'])) {
    $trip_status = $_GET['trip_status'];
  }
  $form['trip_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Trip ID*'),
    '#size' => 20,
    '#default_value' => $trip_id,
    '#maxlength' => 50,
    '#attributes' => array('class' => array('trip-id'), 'id' => 'trip-id'),
    '#prefix' => '<div class="form_row form_row2" id="trip_id"><div class="col_3">',
    '#suffix' => '</div></div>',
  );


  $form['dispatch_date'] = array(
    '#type' => 'date_popup',
    '#title' => t('Dispatch Date'),
    '#date_format' => 'Y-m-d',
    '#attributes' => array('class' => array('dispatch_date')),
    '#date_label_position' => '',
    '#date_year_range' => '-20:+2',
    '#default_value' => $dispatch_date,
    '#size' => 30,
    '#prefix' => '<div class="col_3" id="dis_date">',
    '#suffix' => '</div>',
  );

  $trip_type_field = field_info_field('field_vrs_status');
  $options = list_allowed_values($trip_type_field);
  $form['trip_status'] = array(
    '#type' => 'select',
    '#title' => t('Trip Status'),
    '#options' => array('' => 'Select') + $options,
    '#attributes' => array('class' => array('dispatch_date')),
    '#default_value' => $trip_status,
    '#prefix' => '<div class="stat_3">',
    '#suffix' => '</div>'
  );

  $form['confirm_date'] = array(
    '#type' => 'date_popup',
    '#title' => t('Delivery Confirm Date'),
    '#date_format' => 'Y-m-d',
    '#date_label_position' => '',
    '#date_year_range' => '-20:+2',
    '#size' => 30,
    '#default_value' => $confirm_date,
    '#prefix' => '<div class="col_3">',
    '#suffix' => '</div>'
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Apply'),
    '#validate' => array('trip_short_excess_form_validate'),
    '#submit' => array('trip_short_excess_form_submit'),
  );
  return $form;
}

function trip_short_excess_form_validate($form, &$form_state) {
  $dispatch_date = $form_state['values']['dispatch_date'];
  $trip_id = $form_state['values']['trip_id'];
  $trip_status = $form_state['values']['trip_status'];
  $confirm_date = $form_state['values']['confirm_date'];
  if (empty($dispatch_date) && empty($trip_id) && (empty($trip_status) && empty($confirm_date))) {
    /* $error_elements = array('trip_id', 'dispatch_date');
      foreach ($error_elements as $element) :
      form_set_error($element);
      endforeach; */
    drupal_set_message(t('Please select any one filter'), 'error');
  }
}

function trip_short_excess_form_submit($form, &$form_state) {
  $dispatch_date = $form_state['values']['dispatch_date'];
  $trip_id = $form_state['values']['trip_id'];
  $trip_status = $form_state['values']['trip_status'];
  $confirm_date = $form_state['values']['confirm_date'];

  if (($form_state['values']["op"] == "Apply") && (arg(0) == 'trip-short-excess-report')) {
    drupal_goto('trip-short-excess-report', array('query' => array('dispatch_date' => $dispatch_date, 'trip_id' => $trip_id, 'trip_status' => $trip_status, 'confirm_date' => $confirm_date)));
  }
}

function trip_short_excess_form_callback() {
  $output = '';
  global $user;
  $distributor_id = change_feeds_import_get_distributor_id($user->uid);

  if (isset($_GET['trip_id'])) {
    $trip_id = $_GET['trip_id'];
  }
  if (isset($_GET['dispatch_date'])) {
    $dispatch_date = $_GET['dispatch_date'];
  }
  if (isset($_GET['confirm_date'])) {
    $confirm_date = $_GET['confirm_date'];
  }

  if (isset($_GET['trip_status'])) {
    $trip_status = $_GET['trip_status'];
  }

  if (arg(1) != 'export') {
    $form = drupal_render(drupal_get_form('trip_short_excess_form'));
  }
  if (!empty($trip_id) || !empty($dispatch_date) || !empty($confirm_date) || !empty($trip_status)) {
    $query = db_select('dispatch_planning_api_data', 'ad');
    $query->condition('ad.distributor_id', $distributor_id, '=');
    if (!empty($trip_id)) {
      $query->condition('ad.runsheet_number', $trip_id, '=');
    }
    if (!empty($dispatch_date)) {
      $query->condition('ad.date', $dispatch_date, '=');
    }
    $query->leftjoin('vrs_trip_total', 'vtt', 'ad.runsheet_number = vtt.trip_id AND ad.distributor_id = vtt.distributor_id');
    $query->join('dispatch_planning_item_details', 'dit', 'ad.id = dit.trip_bill_reference');
    $query->leftjoin('excess_inventory_table', 'eit', 'ad.runsheet_number = eit.trip_id AND ad.distributor_id = eit.distributor_id');
    $query->join('node', 'n', "n.title = ad.runsheet_number AND n.uid = ad.distributor_id");
    $query->condition('n.type', 'dispatch_data');
    $query->leftjoin('field_data_field_vrs_status', 'vrs', 'n.nid = vrs.entity_id');
    if (!empty($trip_status)) {
      $query->condition('vrs.field_vrs_status_value', $trip_status, '=');
    }
    if (!empty($confirm_date)) {
      $query->leftjoin('inventory_versioning_api_data', 'vad', 'ad.runsheet_number = vad.runsheet_number and ad.distributor_id =vad.distributor_id');
      $query->addExpression("DATE_FORMAT(CONVERT_TZ(FROM_UNIXTIME(vad.version_created_time),'+00:00','+05:30'), '%Y-%m-%d')", 'version_created_time');
      $query->havingCondition('version_created_time', $confirm_date, '=');
    }
    $query->fields('ad', array('runsheet_number'));
    $query->fields('vtt', array('total_cashier_cash_short'));
    $query->addExpression("SUM(distinct(CASE WHEN dit.res_cashier_short > 0 AND dit.res_cashier_short != '' THEN dit.res_cashier_short*dit.sum_net_sales_value END))", 'Inventory_Short_Value');
    $query->addExpression("SUM(distinct(eit.excess_qty * eit.mrp * 0.9) )", 'Total_Excess_Value');
    $query->addExpression("SUM(distinct(CASE WHEN eit.credit_to_cashier = 1 THEN (eit.excess_qty * eit.mrp * 0.9) END))", 'ExcessValueCreditedtoCashier');

    $query->groupBy('ad.runsheet_number');
    $res = $query->execute()->fetchAll();
    foreach ($res as $val) {
      $inventory_short_value = $val->Inventory_Short_Value;
      $final_invent_shrt_val = round($inventory_short_value, 2);
      $row['trip_id'] = $val->runsheet_number;
      $row['Inventory_Short_Value'] = $final_invent_shrt_val;
      $row['total_cashier_cash_short'] = round($val->total_cashier_cash_short,2);
      $row['Excess_Value'] = round($val->Total_Excess_Value,2);
      $row['ExcessValueCreditedtoCashier'] = round($val->ExcessValueCreditedtoCashier,2);
      $rows[] = $row;
    }
    $header = array('Trip Id', 'Inventory Short Value', 'Cashier Cash Short', 'Excess Value', 'Excess Value Credited to Cashier');
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
    cache_set('trip-short-excess-report' . session_id(), $html, 'cache', time() + 60 * 10);
    $output .= l(t('XLS'), 'trip-short-excess-report/export', array('query' => array($data)));
  }
  return $form . $output;
  $form_state['rebuild'] = TRUE;
}

function trip_short_excess_report_export() {
  $file = "trip_short_excess_report.xls";
  $data = cache_get('trip-short-excess-report' . session_id());
  if (empty($data)) {
    $table = trip_short_excess_form_callback();
  }
  else {
    $table = $data->data;
  }
  header("Content-type: application/vnd.ms-excel");
  header("Content-Disposition: attachment; filename=$file");
  echo $table;
}
