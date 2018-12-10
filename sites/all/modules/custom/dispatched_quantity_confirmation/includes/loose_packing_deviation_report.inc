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
function loose_packing_deviation_form($form, &$form_state) {
  $form['#attached']['js'][] = drupal_get_path('module', 'leanbox_reports') . '/js/pp_non_pp.js';
  if (isset($_GET['trip_id'])) {
    $trip_id = $_GET['trip_id'];
  }
  if (isset($_GET['trip_creation_start_date'])) {
    $trip_creation_start_date = $_GET['trip_creation_start_date'];
  }
  if (isset($_GET['trip_creation_end_date'])) {
    $trip_creation_end_date = $_GET['trip_creation_end_date'];
  }
  
  $form['trip_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Trip ID'),
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

  $form['trip_creation_start_date'] = array(
    '#type' => 'date_popup',
    '#title' => t('Trip Creation Start Date*'),
    '#date_format' => 'Y-m-d',
    '#attributes' => array('class' => array('trip_creation_start_date')),
    '#date_label_position' => '',
    '#date_year_range' => '-20:+2',
    '#size' => 30,
    '#prefix' => '<div class="form_row form_row2"><div class="col_3">',
    '#suffix' => '</div>'
  );
  $form['trip_creation_end_date'] = array(
    '#type' => 'date_popup',
    '#title' => t('Trip Creation End Date*'),
    '#date_format' => 'Y-m-d',
    '#attributes' => array('class' => array('trip_creation_end_date')),
    '#date_label_position' => '',
    '#date_year_range' => '-20:+2',
    '#size' => 30,
    '#prefix' => '<div class="form_row form_row2"><div class="col_3">',
    '#suffix' => '</div></div>'
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Apply'),
    '#submit' => array('loose_packing_deviation_form_submit'),
    '#validate' => array('loose_packing_deviation_report_form_validate'),
  );

  if (!empty($form_state['html'])) {
    $form['loose-picking-deviation'] = array(
      '#markup' => $form_state['html'],
    );
    cache_set('loose_packing_deviation' . session_id(), $form_state['html'], 'cache', time() + 60 * 10);
    $rr = l('Download', "loose-packing-deviation-report/export", array('attributes' => array("target" => "_blank"), 'query' => array('trip_creation_start_date' => $form_state['values']['trip_creation_start_date'], 'trip_creation_end_date' => $form_state['values']['trip_creation_end_date'])));
    $form['download_mark'] = array(
      '#type' => 'markup',
      '#markup' => $rr,
    );
  }
  
  
  return $form;
}

function loose_packing_deviation_form_submit($form, &$form_state, $export = 0) {
  global $user;
  $row = array();
  $user_id = $user->uid;
  $total_bill_count = 0;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);
  $trip = $form_state['values']['trip_id'];
  $trip_creation_start_date = strtotime($form_state['values']['trip_creation_start_date']);
  $trip_creation_end_date = strtotime($form_state['values']['trip_creation_end_date']);
  $trip_id = get_trips_from_trip_creation_date($trip_creation_start_date, $trip_creation_end_date, $distributor_id);
  if (!empty($trip)) {
    $trip_id = array($trip);
  }
  
  foreach ($trip_id as $trip_key) {
    $sub_total = array();
    $query = db_select('dispatch_qty_confirmation', 'dqc');
    $query->join('custom_billing_data', 'cbd', 'cbd.bill_number = dqc.bill_number and cbd.sku7_code = dqc.sku_code and cbd.distributor_id = dqc.distributor_id');
    $query->condition('dqc.trip_id', $trip_key, '=');
    $query->condition('dqc.distributor_id', $distributor_id, '=');
    $query->fields('dqc', array('trip_id', 'bill_number'));
    $query->addExpression("COUNT(dqc.sku_code)", 'original_sku_lines_count');
    $query->addExpression("COUNT(CASE WHEN (dqc.pplc_confirmed_qty = 0 AND dqc.short_reason != '') OR pplc_confirmed_qty != 0 THEN dqc.pplc_confirmed_qty END)", 'pplc_confirmed_lines_count');
    $query->addExpression("COUNT(CASE WHEN dqc.pplc_confirmed_qty < dqc.ptl_put_qty AND dqc.short_reason != '' THEN dqc.bill_number END)", 'pplc_short_lines_count');
    $query->addExpression("COUNT(CASE WHEN dqc.pplc_confirmed_qty > dqc.ptl_put_qty THEN dqc.bill_number END)", 'pplc_excess_lines_count');
    $query->addExpression("SUM(CASE WHEN dqc.pplc_confirmed_qty < dqc.ptl_put_qty AND dqc.short_reason != '' THEN dqc.ptl_put_qty - dqc.pplc_confirmed_qty ELSE 0 END)", 'pplc_short_qty');
    $query->addExpression("SUM(CASE WHEN dqc.pplc_confirmed_qty > dqc.ptl_put_qty THEN dqc.pplc_confirmed_qty - dqc.ptl_put_qty ELSE 0 END)", 'pplc_excess_qty');
    $query->addExpression("SUM(CASE WHEN dqc.pplc_confirmed_qty < dqc.ptl_put_qty AND dqc.short_reason != '' THEN (dqc.ptl_put_qty - dqc.pplc_confirmed_qty)*cbd.line_level_value ELSE 0 END)", 'pplc_short_value');
    $query->addExpression("SUM(CASE WHEN dqc.pplc_confirmed_qty > dqc.ptl_put_qty THEN (dqc.pplc_confirmed_qty - dqc.ptl_put_qty)*cbd.line_level_value ELSE 0 END)", 'pplc_excess_value');
    $query->groupBy('dqc.trip_id, dqc.bill_number');
    $result = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
    if (!empty($result)) {
      $bill_number_count = 0;
      foreach ($result as $value) {
        if ($value['bill_number']) {
          $bill_number_count++;
          $total_bill_count++;
        }
        $row['trip_id'] = $value['trip_id'];
        $sub_total['trip_id'] = 'Trip Sub Total';
        $grand_total['trip_id'] = 'Grand Total';
        $row['bill_number'] = $value['bill_number'];
        $sub_total['bill_number'] = $bill_number_count;
        $grand_total['bill_number'] = $total_bill_count;
        $row['original_sku_lines_count'] = $value['original_sku_lines_count'];
        $sub_total['original_sku_lines_count'] += $value['original_sku_lines_count'];
        $grand_total['original_sku_lines_count'] += $value['original_sku_lines_count'];
        $row['pplc_confirmed_lines_count'] = $value['pplc_confirmed_lines_count'];
        $sub_total['pplc_confirmed_lines_count'] += $value['pplc_confirmed_lines_count'];
        $grand_total['pplc_confirmed_lines_count'] += $value['pplc_confirmed_lines_count'];
        $row['pplc_short_lines_count'] = $value['pplc_short_lines_count'];
        $sub_total['pplc_short_lines_count'] += $value['pplc_short_lines_count'];
        $grand_total['pplc_short_lines_count'] += $value['pplc_short_lines_count'];
        $row['pplc_excess_lines_count'] = $value['pplc_excess_lines_count'];
        $sub_total['pplc_excess_lines_count'] += $value['pplc_excess_lines_count'];
        $grand_total['pplc_excess_lines_count'] += $value['pplc_excess_lines_count'];
        $row['pplc_short_qty'] = $value['pplc_short_qty'];
        $sub_total['pplc_short_qty'] += $value['pplc_short_qty'];
        $grand_total['pplc_short_qty'] += $value['pplc_short_qty'];
        $row['pplc_excess_qty'] = $value['pplc_excess_qty'];
        $sub_total['pplc_excess_qty'] += $value['pplc_excess_qty'];
        $grand_total['pplc_excess_qty'] += $value['pplc_excess_qty'];
        $row['pplc_short_value'] = $value['pplc_short_value'];
        $sub_total['pplc_short_value'] += $value['pplc_short_value'];
        $grand_total['pplc_short_value'] += $value['pplc_short_value'];
        $row['pplc_excess_value'] = $value['pplc_excess_value'];
        $sub_total['pplc_excess_value'] += $value['pplc_excess_value'];
        $grand_total['pplc_excess_value'] += $value['pplc_excess_value'];

        $rows[] = $row;
      }
      $rows[] = $sub_total;
    }
  }
  $rows[] = $grand_total;
  $header = array('Trip ID', 'Bill Number', 'Original SKU Lines', 'PPLC Confirmed Lines', 'PPLC Short Lines', 'PPLC Excess Lines', 'PPLC Short Qty', 'PPLC Excess Qty', 'PPLC Short Value', 'PPLC Excess Value');
  $html = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('width' => '100%')));
  if($export == 1){
    return $html;
  }
  else {
    $form_state['html'] = $html;
    $form_state['rebuild'] = TRUE;
  }
}

function loose_packing_deviation_report_form_validate($form, &$form_state) {

  $trip_id = $form_state['values']['trip_id'];
  $trip_creation_start_date = strtotime($form_state['values']['trip_creation_start_date']);
  $trip_creation_end_date = strtotime($form_state['values']['trip_creation_end_date']);

   if (empty($trip_id) && empty($trip_creation_start_date) && empty($trip_creation_end_date)) {
    form_set_error('form', "Please fill any of the fields");
  }
  if (!empty($trip_creation_start_date) && empty($trip_creation_end_date)) {
    form_set_error('form', 'Please enter Trip Creation End Date');
  }
  if (!empty($trip_creation_end_date) && empty($trip_creation_start_date)) {
    form_set_error('form', 'Please enter Trip Creation Start Date');
  }
}

function loose_packing_deviation_export() {
  $query_parameters = drupal_get_query_parameters();
  $file = "loose_packing_deviation_report".".xls";
  $data = cache_get('loose_packing_deviation' . session_id());
  if (empty($data->data)) {
    if (!empty($query_parameters['trip_creation_start_date']) && !empty($query_parameters['trip_creation_end_date'])) {
      $form_state['values']['trip_creation_start_date'] = $query_parameters['trip_creation_start_date'];
      $form_state['values']['trip_creation_end_date'] = $query_parameters['trip_creation_end_date'];
      $options = loose_packing_deviation_form_submit($form, $form_state, $export = 1);
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