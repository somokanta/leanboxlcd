<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * 
 * @param array $form
 * @param type $form_state
 * @return array
 */
function bill_editing_exception_form($form, &$form_state) {
  $form['#attached']['js'][] = drupal_get_path('module', 'leanbox_reports') . '/js/pp_non_pp.js';
  if (isset($_GET['trip_id'])) {
    $trip_id = $_GET['trip_id'];
  }
  if (isset($_GET['trip_creation_from_date'])) {
    $trip_creation_from_date = $_GET['trip_creation_from_date'];
  }
  if (isset($_GET['trip_creation_to_date'])) {
    $trip_creation_to_date = $_GET['trip_creation_to_date'];
  }
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
    '#attributes' => array('class' => array('trip_id')),
  );

  $form['markup'] = array(
    '#type' => 'markup',
    '#prefix' => '<div class="col_3" style="width:100px">',
    '#suffix' => 'OR</div>'
  );

  $form['trip_creation_from_date'] = array(
    '#type' => 'date_popup',
    '#title' => t('Trip Creation From Date*'),
    '#date_format' => 'Y-m-d',
    '#attributes' => array('class' => array('trip_creation_start_date')),
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
    '#attributes' => array('class' => array('trip_creation_end_date')),
    '#date_label_position' => '',
    '#date_year_range' => '-20:+2',
    '#size' => 30,
    '#default_value' => $trip_creation_to_date,
    '#prefix' => '<div class="form_row form_row2"><div class="col_3">',
    '#suffix' => '</div></div>'
  );

  $form['dispatch_from_date'] = array(
    '#type' => 'date_popup',
    '#title' => t('Dispatch From Date*'),
    '#date_format' => 'Y-m-d',
    '#attributes' => array('class' => array('trip_creation_start_date')),
    '#date_label_position' => '',
    '#date_year_range' => '-20:+2',
    '#size' => 30,
    '#default_value' => $dispatch_from_date,
    '#prefix' => '<div class="form_row form_row2"><div class="col_3">',
    '#suffix' => '</div>'
  );
  $form['dispatch_to_date'] = array(
    '#type' => 'date_popup',
    '#title' => t('Dispatch To Date*'),
    '#date_format' => 'Y-m-d',
    '#attributes' => array('class' => array('trip_creation_end_date')),
    '#date_label_position' => '',
    '#date_year_range' => '-20:+2',
    '#size' => 30,
    '#default_value' => $dispatch_to_date,
    '#prefix' => '<div class="form_row form_row2"><div class="col_3">',
    '#suffix' => '</div></div>'
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Apply'),
    '#submit' => array('bill_editing_exception_form_submit'),
    '#validate' => array('bill_editing_exception_report_form_validate'),
  );

  if (!empty($form_state['html'])) {
    $form['bill-editing-exception'] = array(
      '#markup' => $form_state['html'],
    );
    cache_set('bill_editing_exception' . session_id(), $form_state['html'], 'cache', time() + 60 * 10);
    $rr = l('Download', "bill-editing-exception-report/export", array('attributes' => array("target" => "_blank")));
    $form['download_mark'] = array(
      '#type' => 'markup',
      '#markup' => $rr,
    );
  }
  return $form;
}

function bill_editing_exception_form_submit($form, &$form_state, $export = 0) {
  global $user;
  $row = array();
  $user_id = $user->uid;
  $total_sku_count = 0;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);
  $trip = $form_state['values']['trip_id'];
  $trip_creation_from_date = strtotime($form_state['values']['trip_creation_from_date']);
  $trip_creation_to_date = strtotime($form_state['values']['trip_creation_to_date']);
  $dispatch_from_date = strtotime($form_state['values']['dispatch_from_date']);
  $dispatch_to_date = strtotime($form_state['values']['dispatch_to_date']);
  $trip_id = get_trips_from_trip_creation_date($trip_creation_from_date, $trip_creation_to_date, $distributor_id);
  if (!empty($trip)) {
    $trip_id = array($trip);
  }
  foreach ($trip_id as $trip_key) {
    $sub_total = array();
    $query = db_select('dispatch_qty_confirmation', 'dqc');
    $query->join('custom_billing_data', 'cbd', 'cbd.bill_number = dqc.bill_number and cbd.sku7_code = dqc.sku_code and cbd.distributor_id = dqc.distributor_id');
    $query->join('node', 'n', 'n.title = dqc.trip_id and n.uid = dqc.distributor_id');
    $query->condition('n.type', 'dispatch_data');
    if(!empty($form_state['values']['dispatch_from_date']) && !empty($form_state['values']['dispatch_to_date'])){
      $query->join('field_data_field_dispatch_date', 'fdfdd', 'fdfdd.entity_id = n.nid');
    }
    else {
      $query->leftjoin('field_data_field_dispatch_date', 'fdfdd', 'fdfdd.entity_id = n.nid');
    }
    $query->condition('dqc.trip_id', $trip_key, '=');
    $query->condition('dqc.distributor_id', $distributor_id, '=');
    $query->fields('fdfdd', array('field_dispatch_date_value'));
    $query->fields('dqc', array('trip_id'));
    $query->addExpression("COUNT(distinct(dqc.bill_number))", 'bill_number_count');
    $query->addExpression("SUM(dqc.original_qty)", 'total_original_qty');
    $query->addExpression("SUM(dqc.to_be_dispatch_qty)", 'total_to_be_dispatch_qty');
    $query->addExpression("(SUM(dqc.original_qty))-(SUM(dqc.to_be_dispatch_qty))", 'godown_short_qty');
    $query->addExpression("SUM(cbd.revised_host_quantity)", 'after_bill_editing_qty_ikea');
    $query->addExpression("(SUM(dqc.to_be_dispatch_qty))-(SUM(cbd.revised_host_quantity))", 'difference_before_vs_after');
    $query->groupBy('dqc.trip_id');
    $result = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

    if (!empty($result)) {
      foreach ($result as $value) {
        if ($value['trip_id']) {
          $total_trip_count++;
        }
        if (!empty($value['field_dispatch_date_value'])) {
          $row['field_dispatch_date_value'] = date('d-M-y', strtotime($value['field_dispatch_date_value']));
        }
        else {
          $row['field_dispatch_date_value'] = '';
        }
        $grand_total['field_dispatch_date_value'] = 'Grand Total';
        $row['trip_id'] = $value['trip_id'];
        $grand_total['trip_id'] = $total_trip_count;
        $row['bill_number_count'] = $value['bill_number_count'];
        $grand_total['bill_number_count'] += $value['bill_number_count'];
        $row['total_original_qty'] = $value['total_original_qty'];
        $grand_total['total_original_qty'] += $value['total_original_qty'];
        $row['total_to_be_dispatch_qty'] = $value['total_to_be_dispatch_qty'];
        $grand_total['total_to_be_dispatch_qty'] += $value['total_to_be_dispatch_qty'];
        $row['godown_short_qty'] = $value['godown_short_qty'];
        $grand_total['godown_short_qty'] += $value['godown_short_qty'];
        $row['after_bill_editing_qty_ikea'] = $value['after_bill_editing_qty_ikea'];
        $grand_total['after_bill_editing_qty_ikea'] += $value['after_bill_editing_qty_ikea'];
        $row['difference_before_vs_after'] = $value['difference_before_vs_after'];
        $grand_total['difference_before_vs_after'] += $value['difference_before_vs_after'];

        $rows[] = $row;
      }
    }
  }
  $rows[] = $grand_total;
  $header = array('Dispatch Date', 'Trip ID', 'Bill Count', 'Total Original Qty', 'To be Dispatched Qty', 'Godown Short Qty', 'After Bill editing Qty (IKEA)', 'Difference (Before vs After)');
  $html = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('width' => '100%')));
  if ($export == 1) {
    return $html;
  }
  else {
    $form_state['html'] = $html;
    $form_state['rebuild'] = TRUE;
  }
}

function bill_editing_exception_report_form_validate($form, &$form_state) {
  $trip_id = $form_state['values']['trip_id'];
  $trip_creation_from_date = strtotime($form_state['values']['trip_creation_from_date']);
  $trip_creation_to_date = strtotime($form_state['values']['trip_creation_to_date']);
  $dispatch_from_date = $form_state['values']['dispatch_from_date'];
  $dispatch_to_date = $form_state['values']['dispatch_to_date'];

  if (empty($trip_id) && empty($trip_creation_from_date) && empty($trip_creation_to_date) && empty($dispatch_from_date) && empty($dispatch_to_date)) {
    form_set_error('form', "Please fill any of the fields");
    drupal_goto('bill-editing-exception-report');
  }
  if (!empty($trip_creation_from_date) && empty($trip_creation_to_date)) {
    form_set_error('form', 'Please enter Trip Creation To Date');
    drupal_goto('bill-editing-exception-report');
  }
  if (!empty($trip_creation_to_date) && empty($trip_creation_from_date)) {
    form_set_error('form', 'Please enter Trip Creation From Date');
    drupal_goto('bill-editing-exception-report');
  }
  if (!empty($dispatch_from_date) && empty($dispatch_to_date)) {
    form_set_error('form', 'Please enter Dispatch To Date');
    drupal_goto('bill-editing-exception-report');
  }
  if (!empty($dispatch_to_date) && empty($dispatch_from_date)) {
    form_set_error('form', 'Please enter Dispatch From Date');
    drupal_goto('bill-editing-exception-report');
  }
}

function bill_editing_exception_export() {
  $query_parameters = drupal_get_query_parameters();
  $file = "bill_editing_exception_report" . ".xls";
  $data = cache_get('bill_editing_exception' . session_id());
  if (empty($data->data)) {
    if (!empty(($query_parameters['trip_creation_from_date']) && !empty($query_parameters['trip_creation_to_date'])) || ($query_parameters['dispatch_from_date']) && !empty($query_parameters['dispatch_to_date'])) {
      $form_state['values']['trip_creation_from_date'] = $query_parameters['trip_creation_from_date'];
      $form_state['values']['trip_creation_to_date'] = $query_parameters['trip_creation_to_date'];
      $form_state['values']['dispatch_from_date'] = $query_parameters['dispatch_from_date'];
      $form_state['values']['dispatch_to_date'] = $query_parameters['dispatch_to_date'];
      $options = bill_editing_exception_form_submit($form, $form_state, $export = 1);
      $table = render($options);
    }
  }
  else {
    $table = render($data->data);
  }
  header("Content-type: application/vnd.ms-excel");
  header("Content-Disposition: attachment; filename=$file");
  echo $table;
}

function get_trips_from_trip_creation_dispatch_date($trip_creation_from_date, $trip_creation_to_date, $dispatch_from_date, $dispatch_to_date, $distributor_id) {
  $query = db_select('node', 'n');
  $query->condition('n.type', 'dispatch_data');
  $query->condition('n.uid', $distributor_id);
  $query->join('field_data_field_trip_creation_date', 'tc', 'tc.entity_id = n.nid');
  $query->join('field_data_field_dispatch_date', 'dt', 'dt.entity_id = n.nid');
  if (!empty($trip_creation_from_date) && !empty($trip_creation_to_date)) {
    $query->condition('tc.field_trip_creation_date_value', array($trip_creation_from_date, $trip_creation_to_date), 'BETWEEN');
  }
  if (!empty($dispatch_from_date) && !empty($dispatch_to_date)) {
    $query->condition('dt.field_dispatch_date_value', array($dispatch_from_date, $dispatch_to_date), 'BETWEEN');
  }
  $query->fields('n', array('title'));
  $trips = $query->execute()->fetchCol();
  return $trips;
}
