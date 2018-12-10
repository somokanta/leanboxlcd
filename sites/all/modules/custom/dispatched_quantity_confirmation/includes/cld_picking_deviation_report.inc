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
function cld_picking_deviation_form($form, &$form_state) {
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

  $form['trip_creation_start_date'] = array(
    '#type' => 'date_popup',
    '#title' => t('Trip Creation Start Date*'),
    '#date_format' => 'Y-m-d',
    '#attributes' => array('class' => array('trip_creation_start_date')),
    '#date_label_position' => '',
    '#date_year_range' => '-20:+2',
    '#size' => 30,
    '#default_value' => $trip_creation_start_date,
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
    '#default_value' => $trip_creation_end_date,
    '#prefix' => '<div class="form_row form_row2"><div class="col_3">',
    '#suffix' => '</div></div>'
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Apply'),
    '#submit' => array('cld_picking_deviation_form_submit'),
    '#validate' => array('cld_picking_deviation_report_form_validate'),
  );

  if (!empty($form_state['html'])) {
    $form['cld-picking-deviation'] = array(
      '#markup' => $form_state['html'],
    );
    cache_set('cld_picking_deviation' . session_id(), $form_state['html'], 'cache', time() + 60 * 10);
    $rr = l('Download', "cld-picking-deviation-report/export", array('attributes' => array("target" => "_blank"), 'query' => array('trip_creation_start_date' => $form_state['values']['trip_creation_start_date'], 'trip_creation_end_date' => $form_state['values']['trip_creation_end_date'])));
    $form['download_mark'] = array(
      '#type' => 'markup',
      '#markup' => $rr,
    );
  }

  return $form;
}

function cld_picking_deviation_form_submit($form, &$form_state, $export = 0) {
  global $user;
  $row = array();
  $user_id = $user->uid;
  $total_sku_count = 0;
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
    $query->condition('dqc.cld_retail_confirmed_flag', 1, '=');
    $query->condition('dqc.original_cld_qty', 0, '>');
    $query->fields('dqc', array('trip_id', 'sku_code'));
    $query->addExpression("SUM(dqc.original_cld_qty)", 'total_original_cld_qty');
    $query->addExpression("SUM(dqc.original_cld_qty - dqc.cld_confirmed_qty)", 'cld_short_qty');
    $query->addExpression("SUM((dqc.original_cld_qty - dqc.cld_confirmed_qty))*cbd.line_level_value", 'cld_short_value');
    $query->groupBy('dqc.sku_code');
    $result = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
    
    if (!empty($result)) {
      $sku_count = 0;
      foreach ($result as $value) {
        if ($value['sku_code']) {
          $sku_count++;
          $total_sku_count++;
        }
        $row['trip_id'] = $value['trip_id'];
        $sub_total['trip_id'] = 'Trip Sub Total';
        $grand_total['trip_id'] = 'Grand Total';
        $row['sku_code'] = $value['sku_code'];
        $sub_total['sku_code'] = $sku_count;
        $grand_total['sku_code'] = $total_sku_count;
        $row['total_original_cld_qty'] = $value['total_original_cld_qty'];
        $sub_total['total_original_cld_qty'] += $value['total_original_cld_qty'];
        $grand_total['total_original_cld_qty'] += $value['total_original_cld_qty'];
        $row['cld_short_qty'] = $value['cld_short_qty'];
        $sub_total['cld_short_qty'] += $value['cld_short_qty'];
        $grand_total['cld_short_qty'] += $value['cld_short_qty'];
        $row['cld_short_value'] = $value['cld_short_value'];
        $sub_total['cld_short_value'] += $value['cld_short_value'];
        $grand_total['cld_short_value'] += $value['cld_short_value'];

        $rows[] = $row;
      }
      $rows[] = $sub_total;
    }
  }
  $rows[] = $grand_total;
  $header = array('Trip ID', 'SKU Code', 'Original CLD Qty', 'CLD Short Qty', 'CLD Short Value');
  $html = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('width' => '100%')));
  if ($export == 1) {
    return $html;
  }
  else {
    $form_state['html'] = $html;
    $form_state['rebuild'] = TRUE;
  }
}

function cld_picking_deviation_report_form_validate($form, &$form_state) {
  global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);
  $trip_id = $form_state['values']['trip_id'];
  $trip_creation_start_date = strtotime($form_state['values']['trip_creation_start_date']);
  $trip_creation_end_date = strtotime($form_state['values']['trip_creation_end_date']);

  if (!empty($trip_id)) {
    
    $query1 = db_select('dispatch_qty_confirmation', 'dqc');
    $query1->condition('dqc.trip_id', $trip_id, '=');
    $query1->condition('dqc.distributor_id', $distributor_id, '=');
    $query1->addExpression("(SUM(dqc.original_cld_qty)) > 0", 'original_cld_qty');
    $result1 = $query1->execute()->fetchField();
    if (empty($result1)) {
      form_set_error('form', "Trip ID $trip_id does not have any CLDs");
      drupal_goto('cld-picking-deviation-report');
    }
    
    $query = db_select('dispatch_qty_confirmation', 'dqc');
    $query->condition('dqc.trip_id', $trip_id, '=');
    $query->condition('dqc.distributor_id', $distributor_id, '=');
    $query->condition('dqc.cld_retail_confirmed_flag', 1, '=');
    $query->fields('dqc', array('cld_retail_confirmed_flag'));
    $result = $query->execute()->fetchField();
    if (empty($result)) {
      form_set_error('form', "CLDs for Trip ID $trip_id have not been confirmed");
      drupal_goto('cld-picking-deviation-report');
    }
  }

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

function cld_picking_deviation_export() {
  $query_parameters = drupal_get_query_parameters();
  $file = "cld_picking_deviation_report" . ".xls";
  $data = cache_get('cld_picking_deviation' . session_id());
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
