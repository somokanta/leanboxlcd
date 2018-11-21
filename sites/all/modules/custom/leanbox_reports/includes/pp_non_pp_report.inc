<?php

/*
 * Van planning pp-non pp report export.
 */

function pp_report_form($form, &$form_state) {
  $form = array();
  $form['#attached']['css'][] = drupal_get_path('module', 'deliverycost_calculation') . '/css/vehicle_resource_daily_status.css';
  $form['#attached']['css'][] = drupal_get_path('module', 'leanbox_reports') . '/css/pp_non_pp.css';
  $form['#prefix'] = '<div id="itemlist-div" class="custom_form_wrapper">';
  $form['#suffix'] = '</div>';
  $form['trip_creation_start_date'] = array(
    '#type' => 'date_popup',
    '#date_format' => 'm/d/y',
    '#title' => t('Trip Creation Start Date'),
    '#date_label_position' => '',
    '#date_year_range' => '-20:+1',
    '#required' => TRUE,
    '#prefix' => '<div class="form_row form_row2"><div class="col_3" style="width:200px">',
    '#suffix' => '</div>'
  );
  $form['trip_creation_end_date'] = array(
    '#type' => 'date_popup',
    '#date_format' => 'm/d/y',
    '#title' => t('Trip Creation End Date'),
    '#date_label_position' => '',
    '#date_year_range' => '-20:+1',
    '#required' => TRUE,
    '#prefix' => '<div class="col_3" style="width:200px">',
    '#suffix' => '</div></div>'
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Show',
    '#name' => 'show',
    '#ajax' => array(
      'callback' => 'pp_report_callback',
      'wrapper' => 'itemlist-div',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    '#attributes' => array('class' => array('vrs-trip-submit')),
    '#submit' => array('get_pp_report_submit'),
    '#validate' => array('validate_pp_report'),
    '#prefix' => '<div style="clear:both;float:left;align:center;">',
    '#suffix' => '</div></div>'
  );
  $is_user_selected_non_pp = arg(1);
  $pp = 'pp';
  if ($is_user_selected_non_pp == 'non_pp') {
    $form_state['non_pp'] = 1;
    $pp = 'non_pp';
  }
  if ($form_state['non_pp'] == 1) {
    $pp = 'non_pp';
  }
  $options = $form_state['output'];
  if (!empty($options)) {
    cache_set('pp_report' . session_id(), $options, 'cache', time() + 60 * 10);
    $form['data'] = array(
      '#type' => 'markup',
      '#markup' => $options,
      '#prefix' => '<div style="margin-bottom:15px;"></div>'
    );
    $rr = l('Download', "pp-non-pp-report-export/$pp", array('attributes' => array("target" => "_blank"), 'query' => array('trip_creation_start_date' => $form_state['values']['trip_creation_start_date'], 'trip_creation_end_date' => $form_state['values']['trip_creation_end_date'])));
    $form['download_mark'] = array(
      '#type' => 'markup',
      '#prefix' => '<div style="margin-top:15px;"></div>',
      '#markup' => $rr,
    );
  }
  return $form;
}

function pp_report_callback(&$form, &$form_state) {
  $commands[] = ajax_command_replace('#itemlist-div', render($form));
  $commands[] = ajax_command_replace('.alert-danger', '');
  $commands[] = ajax_command_replace('.alert-success', '');
  $commands[] = ajax_command_prepend('#itemlist-div', theme('status_messages'));
  return array('#type' => 'ajax', '#commands' => $commands);
}

function pp_report_export($form, $form_state) {
  $arg1 = arg(1);
  $label = $arg1;
  $cache_name = '';
  $file = "hul_pp_report" . '_' . date('d-M-Y') . '_' . time() . ".xls";
  $data = cache_get('pp_report' . session_id());
  if (empty($data)) {
    $paramenters = drupal_get_query_parameters();
    if (!empty($paramenters)) {
      if (!empty($paramenters['trip_creation_end_date'])) {
        $form_state['values']['trip_creation_end_date'] = $paramenters['trip_creation_end_date'];
      }
      elseif (!empty($paramenters['trip_creation_start_date'])) {
        $form_state['values']['trip_creation_start_date'] = $paramenters['trip_creation_start_date'];
      }
      if ($arg1 == 'non_pp') {
        $form_state['non_pp'] = 1;
      }
      $table = get_pp_report_submit($form, $form_state, $export = 1);
      $table = render($table);
    }
  }
  else {
    $table = render($data->data);
  }
  header("Content-type: application/vnd.ms-excel");
  header("Content-Disposition: attachment; filename=$file");
  echo $table;
}

function get_pp_report_submit($form, &$form_state, $export = 0) {
  global $user;
  $round_off = 2;
  $distributor_id = change_feeds_import_get_distributor_id($user->uid);
  $non_pp = $form_state['non_pp'];
  if (empty($non_pp)) {
    $van_type = t('PP');
  }
  else {
    $van_type = t('Non-PP');
  }
  $start_date = $form_state['values']['trip_creation_start_date'];
  $end_date = $form_state['values']['trip_creation_end_date'];
  if (!empty($start_date) && !empty($end_date)) {
    $start_date = strtotime($form_state['values']['trip_creation_start_date']);
    $end_date = strtotime($form_state['values']['trip_creation_end_date']);
    $result = get_pp_trips($start_date, $end_date);
    if (!empty($result)) {
      $table1 = '<table class ="van-plan-report-table"  cellpadding="0" cellspacing="0" width="100%">';
      $table1 .= '<tr><td>' . '<b>' . t('Trip ID') . '</b>' . '</td>';
      $table1 .= '<td>' . '<b>' . t('Bill date') . '</b>' . '</td>';
      $table1 .= '<td >' . '<b>' . t('TP Van no') . '</b>' . '</td>';
      $table1 .= '<td >' . '<b>' . t('Van type (PP/Non-PP)') . '</b>' . '</td>';
      $table1 .= '<td >' . '<b>' . t('Pl group') . '</b>' . '</td>';
      $table1 .= '<td >' . '<b>' . t('Pick lines loose') . '</b>' . '</td>';
      $table1 .= '<td >' . '<b>' . t('Loose value(in lakhs)') . '</b>' . '</td>';
      $table1 .= '<td >' . '<b>' . t('Loose value %age by PL group') . '</b>' . '</td>';
      $table1 .= '<td >' . '<b>' . t('CLD Lines') . '</b>' . '</td>';
      $table1 .= '<td >' . '<b>' . t('No. of CLDs (Pack/Retail)') . '</b>' . '</td>';
      $table1 .= '<td >' . '<b>' . t('CLD value (in lakhs)') . '</b>' . '</td>';
      $table1 .= '</tr>';
      foreach ($result as $trip_id => $value) {
        $out_put_data = get_pp_report_data($value['sales_nid'], $distributor_id);
        foreach ($value['bill_date'] as $bill_date) {
          $b_date = $bill_date . ',';
        }
        $b_date = rtrim($b_date, ',');
        $table1 .= '<tr><td>' . $trip_id . '</td>';
        $table1 .= '<td>' . $b_date . '</td>';
        $table1 .= '<td>' . $value['prevan'] . '</td>';
        $table1 .= '<td>' . $van_type . '</td>';
        $table1 .= '<td>';
        $table1 .= '<table  cellpadding="0" cellspacing="0" width="100%">';
        foreach ($out_put_data as $cat => $value) {
          $table1 .= '<tr><td>' . $cat . '</td></tr>';
        }
        $table1 .= '</table>';
        $table1 .= '</td>';

        // Pick Lines loose
        $table1 .= '<td>';
        $table1 .= '<table  cellpadding="0" cellspacing="0" width="100%">';
        foreach ($out_put_data as $cat => $value) {
          $table1 .= '<tr><td>' . $value['loose_lines'] . '</td></tr>';
        }
        $table1 .= '</table>';
        $table1 .= '</td>';
        // Loose value
        $table1 .= '<td>';
        $table1 .= '<table  cellpadding="0" cellspacing="0" width="100%">';
        foreach ($out_put_data as $cat => $value) {
          $table1 .= '<tr><td>' . round($value['loose_value'], $round_off) . '</td></tr>';
        }
        $table1 .= '</table>';
        $table1 .= '</td>';
        // Loose value percentage by group
        $table1 .= '<td>';
        $table1 .= '<table  cellpadding="0" cellspacing="0" width="100%">';
        foreach ($out_put_data as $cat => $value) {
          $table1 .= '<tr><td>' . $value['loose_value_percentage_by_group'] . '</td></tr>';
        }
        $table1 .= '</table>';
        $table1 .= '</td>';

        // CLD lines
        $table1 .= '<td>';
        $table1 .= '<table  cellpadding="0" cellspacing="0" width="100%">';
        foreach ($out_put_data as $cat => $value) {
          $table1 .= '<tr><td>' . $value['cld_lines'] . '</td></tr>';
        }
        $table1 .= '</table>';
        $table1 .= '</td>';

        // No of cld pack
        $table1 .= '<td>';
        $table1 .= '<table  cellpadding="0" cellspacing="0" width="100%">';
        foreach ($out_put_data as $cat => $value) {
          $table1 .= '<tr><td>' . $value['no_of_clds'] . '</td></tr>';
        }
        $table1 .= '</table>';
        $table1 .= '</td>';
        // CLD value
        $table1 .= '<td>';
        $table1 .= '<table  cellpadding="0" cellspacing="0" width="100%">';
        foreach ($out_put_data as $cat => $value) {
          $table1 .= '<tr><td>' . round($value['cld_value'], $round_off) . '</td></tr>';
        }
        $table1 .= '</table>';
        $table1 .= '</td>';
        $table1 .= '</tr>';
      }
    }
    $table1 .= '</table>';
    if ($export == 1) {
      $table = render($table1);
      return $table;
    }
    $form_state['output'] = render($table1);
    $form_state['rebuild'] = TRUE;
  }
}

function get_pp_report_data($total_sales_nid_array, $distributor_id) {
  $lak = 100000;
  $round_off = 3;
  $res1 = array();
  $query = db_select('custom_billing_data', 'bd');
  $query->addfield('bd', 'Sum_of_TOTAL_SALES_QTY', 'total_qty');
  $query->addfield('bd', 'sku7_code', 'sku_code');
  $query->addfield('bd', 'bill_number', 'bill_number');
  $query->addExpression('ROUND(((Sum_of_NET_SALES_VALUE - ((Sum_of_OTHER_DISCOUNT + Sum_of_SCHEME_DISCOUNT)*(1 + (Sum_of_TAX_PERCENTAGE/100))))/Sum_of_TOTAL_SALES_QTY), 2)', 'sum_net_sales_value');
  $query->groupBy('bd.sku7_code');
  $query->groupBy('bd.bill_number');
  $query->condition('bd.distributor_id', $distributor_id);
  $query->join('node', 'n', 'n.title = bd.sku7_code AND bd.distributor_id = n.uid');
  $query->condition('n.type', 'sku7_code_master');
  $query->join('field_data_field_unit_case_units_', 'fucu', 'fucu.entity_id = n.nid');
  $query->fields('fucu', array('field_unit_case_units__value'));
  //subcategory join
  $query->leftjoin('field_data_field_sub_category', 'fc', 'fc.entity_id = n.nid');
  $query->leftjoin('node', 'sub_category', "sub_category.nid = fc.field_sub_category_nid and sub_category.type = 'product_sub_category_master' ");
  // Join with product category
  $query->leftjoin('field_data_field_product_category', 'category_field', "category_field.entity_id = sub_category.nid ");
  $query->leftjoin('node', 'category', "category.nid = category_field.field_product_category_nid and category.type = 'product_category_master' ");
  $query->join('node', 'n1', 'n1.title = bd.bill_number AND bd.distributor_id = n1.uid');
  $query->condition('n1.type', 'sales_register_data');
  $query->addfield('n1', 'nid', 'sales_nid');
  $query->addfield('n1', 'title', 'bill_no');
  $query->addfield('bd', 'party_code', 'party_code');
  $query->addfield('category', 'title', 'category');
  $query->addfield('sub_category', 'title', 'sub_category');
  $query->condition('n1.nid', $total_sales_nid_array, 'IN');
  $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
  $unique_sku = $sku_array = $bill_wise_unique_sku = $unique_sku_co = array();
  $res1 = array();
  foreach ($res as &$val) {
    if (empty($val['category'])) {
      $val['category'] = 'NA';
    }
    if (!empty($val['category'])) {
      if (!isset($unique_sku_co[$val['category']][$val['sku_code']])) {
        $unique_sku_co[$val['category']][$val['sku_code']] = 1;
        $res1[$val['category']]['sku_count'] += 1;
      }
      $cld = (int) ($val['total_qty'] / $val['field_unit_case_units__value']);
      $loose = $val['total_qty'] % $val['field_unit_case_units__value'];
      $res1[$val['category']]['cld'] += 0;
      $res1[$val['category']]['no_of_clds'] += 0;
      $res1[$val['category']]['loose'] += 0;
      $res1[$val['category']]['no_loose_count'] += 0;
      $res1[$val['category']]['loose_value'] += 0;
      $res1[$val['category']]['cld_value'] += 0;
      if ($cld >= 1) {
        if (!isset($unique_sku[$val['category']]['cld'][$val['sku_code']])) {
          $unique_sku[$val['category']]['cld'][$val['sku_code']] = 1;
          $res1[$val['category']]['cld'] += 1;
          $res1[$val['category']]['no_of_clds'] += $cld;
        }
        $res1[$val['category']]['cld_value'] += ($val['sum_net_sales_value'] * $cld * $val['field_unit_case_units__value']) / $lak;
        $unique_sku[$val['category']]['cld']['total'] = $res1[$val['category']]['cld_value'];
      }
      if ($loose >= 1) {
        if (!isset($unique_sku[$val['category']]['loose'][$val['sku_code']])) {
          $unique_sku[$val['category']]['loose'][$val['sku_code']] = 1;
          $res1[$val['category']]['loose'] += 1;
          $res1[$val['category']]['no_loose_count'] += $loose;
        }
        if (!isset($bill_wise_unique_sku['bill_wise_loose'][$val['category']][$val['sales_nid']][$val['sku_code']])) {
          $bill_wise_unique_sku['bill_wise_loose'][$val['category']][$val['sales_nid']][$val['sku_code']] = 1;
          $res1[$val['category']]['bill_wise_loose'] += 1;
        }
        $res1[$val['category']]['loose_value'] += ($val['sum_net_sales_value'] * $loose) / $lak;
        $unique_sku[$val['category']]['loose']['total'] = $res1[$val['category']]['loose_value'];
      }
      $res1[$val['category']]['cld_lines'] = ($res1[$val['category']]['cld']);
      $res1[$val['category']]['loose_lines'] = $res1[$val['category']]['loose'];
      $res1[$val['category']]['no_of_clds'] = ($res1[$val['category']]['no_of_clds']);
    }
  }
  $total_loose_value = 0;
  $total_cld_value = 0;
  if (!empty($res1)) {
    foreach ($res1 as $cat_key => $value) {
      $total_loose_value += $value['loose_value'];
      $total_cld_value += $value['cld_value'];
    }
    //$report['total']['total_loose_value'] = $total_loose_value;
    //$report['total']['total_cld_value'] = $total_cld_value;
    //for calculating percentage
    foreach ($res1 as $cat_key => &$value) {
      $loose_value_percentage_by_group = round($value['loose_value'] / $total_loose_value * 100);
      $value['loose_value_percentage_by_group'] = $loose_value_percentage_by_group;
    }
  }
  return $res1;
}

function get_non_pp_report_data_trip_wise($bill_numbers, $distributor_id) {
  $lak = 100000;
  $round_off = 3;
  $res1 = array();
  $query = db_select('custom_billing_data', 'bd');
  $query->addExpression('SUM(Sum_of_TOTAL_SALES_QTY)', 'total_qty');
  $query->addfield('bd', 'sku7_code', 'sku_code');
  $query->addExpression('ROUND(((Sum_of_NET_SALES_VALUE - ((Sum_of_OTHER_DISCOUNT + Sum_of_SCHEME_DISCOUNT)*(1 + (Sum_of_TAX_PERCENTAGE/100))))/Sum_of_TOTAL_SALES_QTY), 2)', 'sum_net_sales_value');
  $query->groupBy('bd.sku7_code');
  $query->condition('bd.distributor_id', $distributor_id);
  $query->join('node', 'n', 'n.title = bd.sku7_code AND bd.distributor_id = n.uid');
  $query->condition('n.type', 'sku7_code_master');
  $query->join('field_data_field_unit_case_units_', 'fucu', 'fucu.entity_id = n.nid');
  $query->fields('fucu', array('field_unit_case_units__value'));
  //subcategory join
  $query->leftjoin('field_data_field_sub_category', 'fc', 'fc.entity_id = n.nid');
  $query->leftjoin('node', 'sub_category', "sub_category.nid = fc.field_sub_category_nid and sub_category.type = 'product_sub_category_master' ");
  // Join with product category
  $query->leftjoin('field_data_field_product_category', 'category_field', "category_field.entity_id = sub_category.nid ");
  $query->leftjoin('node', 'category', "category.nid = category_field.field_product_category_nid and category.type = 'product_category_master' ");
  $query->addfield('category', 'title', 'category');
  $query->addfield('sub_category', 'title', 'sub_category');
  $query->condition('bd.bill_number', $bill_numbers, 'IN');
  $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
  $unique_sku = $sku_array = $bill_wise_unique_sku = $unique_sku_co = array();
  $res1 = array();
  foreach ($res as &$val) {
    if (empty($val['category'])) {
      $val['category'] = 'NA';
    }
    if (!empty($val['category'])) {
      if (!isset($unique_sku_co[$val['category']][$val['sku_code']])) {
        $unique_sku_co[$val['category']][$val['sku_code']] = 1;
        $res1[$val['category']]['sku_count'] += 1;
      }
      $cld = (int) ($val['total_qty'] / $val['field_unit_case_units__value']);
      $loose = $val['total_qty'] % $val['field_unit_case_units__value'];
      $res1[$val['category']]['cld'] += 0;
      $res1[$val['category']]['no_of_clds'] += 0;
      $res1[$val['category']]['loose'] += 0;
      $res1[$val['category']]['no_loose_count'] += 0;
      $res1[$val['category']]['loose_value'] += 0;
      $res1[$val['category']]['cld_value'] += 0;
      if ($cld >= 1) {
        if (!isset($unique_sku[$val['category']]['cld'][$val['sku_code']])) {
          $unique_sku[$val['category']]['cld'][$val['sku_code']] = 1;
          $res1[$val['category']]['cld'] += 1;
          $res1[$val['category']]['no_of_clds'] += $cld;
        }
        $res1[$val['category']]['cld_value'] += ($val['sum_net_sales_value'] * $cld * $val['field_unit_case_units__value']) / $lak;
        $unique_sku[$val['category']]['cld']['total'] = $res1[$val['category']]['cld_value'];
      }
      if ($loose >= 1) {
        if (!isset($unique_sku[$val['category']]['loose'][$val['sku_code']])) {
          $unique_sku[$val['category']]['loose'][$val['sku_code']] = 1;
          $res1[$val['category']]['loose'] += 1;
          $res1[$val['category']]['no_loose_count'] += $loose;
        }
        if (!isset($bill_wise_unique_sku['bill_wise_loose'][$val['category']][$val['sales_nid']][$val['sku_code']])) {
          $bill_wise_unique_sku['bill_wise_loose'][$val['category']][$val['sales_nid']][$val['sku_code']] = 1;
          $res1[$val['category']]['bill_wise_loose'] += 1;
        }
        $res1[$val['category']]['loose_value'] += ($val['sum_net_sales_value'] * $loose) / $lak;
        $unique_sku[$val['category']]['loose']['total'] = $res1[$val['category']]['loose_value'];
      }
      $res1[$val['category']]['cld_lines'] = ($res1[$val['category']]['cld']);
      $res1[$val['category']]['loose_lines'] = $res1[$val['category']]['loose'];
      $res1[$val['category']]['no_of_clds'] = ($res1[$val['category']]['no_of_clds']);
    }
  }

  return $res1;
}
function get_pp_trips($start_date, $end_date) {
  $result = array();
  if (!empty($start_date) && !empty($end_date)) {
    $query = db_select('node', 'n');
    $query->condition('n.type', 'dispatch_data');
    $query->join('ptl_data_configuration', 'ptl', 'ptl.trip_id = n.title');
    $query->join('field_data_field_trip_creation_date', 'trip_created_date', 'trip_created_date.entity_id = n.nid');
    $query->condition('trip_created_date.field_trip_creation_date_value', array($start_date, $end_date), 'BETWEEN');
    $query->addTag('node_distributor_access');
    $query->join('field_data_field_trip_id_reference', 'trip_bill_ref', 'trip_bill_ref.field_trip_id_reference_nid = n.nid');
    $query->join('node', 'sales_register', 'sales_register.nid = trip_bill_ref.entity_id');
    $query->condition('sales_register.type', 'sales_register_data');
    $query->join('field_data_field_sr_new_bill_date', 'bd', 'bd.entity_id = sales_register.nid');
    $query->addExpression("DATE_FORMAT(CONVERT_TZ(FROM_UNIXTIME(bd.field_sr_new_bill_date_value),'+00:00','+05:30'), '%d-%m-%Y')", 'bill_date');
    $query->fields('sales_register', array('nid'));
    $query->fields('n', array('title'));
    $query->leftjoin('field_data_field_entered_prevan', 'pvan', 'pvan.entity_id = n.nid');
    $query->fields('pvan', array('field_entered_prevan_value'));
    $res = $query->execute()->fetchAll(PDO::FETCH_UNIQUE | PDO::FETCH_ASSOC);
    if (!empty($res)) {
      foreach ($res as $bill_number => $value) {
        $result[$value['title']]['sales_nid'][$bill_number] = $bill_number;
        $result[$value['title']]['bill_date'][$value['bill_date']] = $value['bill_date'];
        $result[$value['title']]['prevan'] = $value['field_entered_prevan_value'];
      }
    }
  }
  return $result;
}

function validate_pp_report($form, &$form_state) {
  global $user;
  $round_off = 2;
  $distributor_id = change_feeds_import_get_distributor_id($user->uid);
  $non_pp = $form_state['non_pp'];
  if (empty($non_pp)) {
    $van_type = t('PP');
  }
  else {
    $van_type = t('Non-PP');
  }
  $start_date = $form_state['values']['trip_creation_start_date'];
  $end_date = $form_state['values']['trip_creation_end_date'];
  if (!empty($start_date) && !empty($end_date)) {
    $start_date = strtotime($form_state['values']['trip_creation_start_date']);
    $end_date = strtotime($form_state['values']['trip_creation_end_date']);
    $result = get_pp_trips($start_date, $end_date);
    if (empty($result)) {
      form_set_error('form', t('No records found.'));
    }
  }
}

// Non pp report

function non_pp_report_form($form, &$form_state) {
  $form = array();
  $form['#attached']['css'][] = drupal_get_path('module', 'deliverycost_calculation') . '/css/vehicle_resource_daily_status.css';
  $form['#attached']['css'][] = drupal_get_path('module', 'leanbox_reports') . '/css/pp_non_pp.css';
  $form['#prefix'] = '<div id="itemlist-div" class="custom_form_wrapper">';
  $form['#suffix'] = '</div>';
  $form['trip_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Master Trip ID'),
    '#required' => TRUE,
    '#prefix' => '<div class="form_row form_row2"><div class="col_3" style="width:200px">',
    '#suffix' => '</div>'
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Show',
    '#name' => 'show',
    '#ajax' => array(
      'callback' => 'non_pp_report_callback',
      'wrapper' => 'itemlist-div',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    '#attributes' => array('class' => array('vrs-trip-submit')),
    '#submit' => array('get_non_pp_report_submit'),
    '#validate' => array('validate_non_pp_report'),
    '#prefix' => '<div style="clear:both;float:left;align:center;margin-left:25px;">',
    '#suffix' => '</div></div>'
  );
  $options = $form_state['output'];
  if (!empty($options)) {
    cache_set('non_pp_report' . session_id(), $options, 'cache', time() + 60 * 10);
    $form['data'] = array(
      '#type' => 'markup',
      '#markup' => $options,
      '#prefix' => '<div style="margin-bottom:15px;"></div>'
    );
    $rr = l('Download', "non-pp-report-export/$pp", array('attributes' => array("target" => "_blank"), 'query' => array('trip_id' => $form_state['values']['trip_id'])));
    $form['download_mark'] = array(
      '#type' => 'markup',
      '#prefix' => '<div style="margin-top:15px;"></div>',
      '#markup' => $rr,
    );
  }
  return $form;
}

function non_pp_report_callback(&$form, &$form_state) {
  $commands[] = ajax_command_replace('#itemlist-div', render($form));
  $commands[] = ajax_command_replace('.alert-danger', '');
  $commands[] = ajax_command_replace('.alert-success', '');
  $commands[] = ajax_command_prepend('#itemlist-div', theme('status_messages'));
  return array('#type' => 'ajax', '#commands' => $commands);
}

function non_pp_report_export($form, $form_state) {
  $arg1 = arg(1);
  $label = $arg1;
  $cache_name = '';
  $file = "hul_non_pp_report" . '_' . date('d-M-Y') . '_' . time() . ".xls";
  $data = cache_get('non_pp_report' . session_id());
  if (empty($data)) {
    $paramenters = drupal_get_query_parameters();
    if (!empty($paramenters)) {
      if (!empty($paramenters['trip_id'])) {
        $form_state['values']['trip_id'] = $paramenters['trip_id'];
      }
      
      $table = get_non_pp_report_submit($form, $form_state, $export = 1);
      $table = render($table);
    }
  }
  else {
    $table = render($data->data);
  }
  header("Content-type: application/vnd.ms-excel");
  header("Content-Disposition: attachment; filename=$file");
  echo $table;
}

function get_non_pp_report_submit($form, &$form_state, $export = 0) {
  global $user;
  $round_off = 2;
  $distributor_id = change_feeds_import_get_distributor_id($user->uid);
  $van_type = t('Non-PP');
  $master_trip_id = $form_state['values']['trip_id'];
  if (!empty($master_trip_id)) {
    $result = get_non_pp_trips($master_trip_id);
    //print_r($result['trip_wise']);
    //watchdog('non_pp_trips', '<pre>' . print_r($result, TRUE) . '</pre>');

    if (!empty($result['master_trip_data']['bill_numbers'])) {
      $table1 = '<table class ="van-plan-report-table"  cellpadding="0" cellspacing="0" width="100%">';
      $table1 .= '<tr><td>' . '<b>' . t('Master trip') . '</b>' . '</td>';
      $table1 .= '<td>' . '<b>' . t('Bill date') . '</b>' . '</td>';
      $table1 .= '<td>' . '<b>' . t('Trip ID') . '</b>' . '</td>';
      $table1 .= '<td >' . '<b>' . t('Van no') . '</b>' . '</td>';
      $table1 .= '<td >' . '<b>' . t('Van type (PP/Non-PP)') . '</b>' . '</td>';
      $table1 .= '<td >' . '<b>' . t('Pl group') . '</b>' . '</td>';
      $table1 .= '<td >' . '<b>' . t('Pick lines loose') . '</b>' . '</td>';
      $table1 .= '<td >' . '<b>' . t('Openable CLDs') . '</b>' . '</td>';
      $table1 .= '<td >' . '<b>' . t('Loose value(in lakhs)') . '</b>' . '</td>';
      $table1 .= '<td >' . '<b>' . t('Loose value %age by PL group') . '</b>' . '</td>';
      $table1 .= '</tr>';
      //$out_put_data = get_non_pp_report_data($value['sales_nid'], $distributor_id);
      $out_put_data = get_non_pp_report_data($result['master_trip_data']['bill_numbers'], $distributor_id);
      $b_date = rtrim($b_date, ',');
      $table1 .= '<tr><td>' . $master_trip_id . '</td>';
      $table1 .= '<td>' . $result['master_trip_data']['bill_dates'] . '</td>';
      $table1 .= '<td>' . $result['master_trip_data']['trip_ids'] . '</td>';
      $table1 .= '<td>' . $result['master_trip_data']['prevan'] . '</td>';
      $table1 .= '<td>' . $van_type . '</td>';
      $table1 .= '<td>';
      $table1 .= '<table  cellpadding="0" cellspacing="0" width="100%">';
      foreach ($out_put_data as $cat => $value) {
        $table1 .= '<tr><td>' . $cat . '</td></tr>';
      }
      $table1 .= '</table>';
      $table1 .= '</td>';

      // Pick Lines loose
      $table1 .= '<td>';
      $table1 .= '<table  cellpadding="0" cellspacing="0" width="100%">';
      foreach ($out_put_data as $cat => $value) {
        $table1 .= '<tr><td>' . $value['loose_lines'] . '</td></tr>';
      }
      $table1 .= '</table>';
      $table1 .= '</td>';
      // Loose value
      $table1 .= '<td>';
      $table1 .= '<table  cellpadding="0" cellspacing="0" width="100%">';
      foreach ($out_put_data as $cat => $value) {
        $table1 .= '<tr><td>' . $value['total_openable_cld'] . '</td></tr>';
      }
      $table1 .= '</table>';
      $table1 .= '</td>';
      // Loose value percentage by group
      $table1 .= '<td>';
      $table1 .= '<table  cellpadding="0" cellspacing="0" width="100%">';
      foreach ($out_put_data as $cat => $value) {
        $table1 .= '<tr><td>' . round($value['loose_value'], $round_off) . '</td></tr>';
      }
      $table1 .= '</table>';
      $table1 .= '</td>';
      // Loose value percentage by group
      $table1 .= '<td>';
      $table1 .= '<table  cellpadding="0" cellspacing="0" width="100%">';
      foreach ($out_put_data as $cat => $value) {
        $table1 .= '<tr><td>' . $value['loose_value_percentage_by_group'] . '</td></tr>';
      }
      $table1 .= '</table>';
      $table1 .= '</td>';
      $table1 .= '</tr>';
    }
  }
  if (!empty($result['master_trip_data']['bill_numbers'])) {
    // Second table row
    $table1 .= '<tr><td colspan="10"></td></tr>';
    $table1 .= '<tr><td colspan="10"></td></tr>';
    $table1 .= '<tr><td colspan="10"></td></tr>';
    $table1 .= '<tr><td>' . '<b>' . t('Master trip') . '</b>' . '</td>';
    $table1 .= '<td>' . '<b>' . t('Bill date') . '</b>' . '</td>';
    $table1 .= '<td>' . '<b>' . t('Trip ID') . '</b>' . '</td>';
    $table1 .= '<td >' . '<b>' . t('Van no') . '</b>' . '</td>';
    $table1 .= '<td >' . '<b>' . t('Van type (PP/Non-PP)') . '</b>' . '</td>';
    $table1 .= '<td >' . '<b>' . t('Pl group') . '</b>' . '</td>';
    $table1 .= '<td >' . '<b>' . t('No. of CLDs') . '</b>' . '</td>';
    $table1 .= '<td >' . '<b>' . t('CLD lines') . '</b>' . '</td>';
    $table1 .= '<td >' . '<b>' . t('CLD value (in lakhs))') . '</b>' . '</td>';
    $table1 .= '</tr>';
    /* $table1 .= '<tr><td>' . $master_trip_id . '</td>';
      $table1 .= '<td>' . $result['master_trip_data']['bill_dates'] . '</td>';
      $table1 .= '<td>' . $result['master_trip_data']['trip_ids'] . '</td>';
      $table1 .= '<td>' . $result['master_trip_data']['prevan'] . '</td>';
      $table1 .= '<td>' . $van_type . '</td>';
      $table1 .= '<td>'; */
    foreach ($result['trip_wise'] as $trip_id => $value) {
      $out_put_data = get_non_pp_report_data_trip_wise($value['sales_nid'], $distributor_id);
      $table1 .= '<tr><td>' . $master_trip_id . '</td>';
      $table1 .= '<td>' . $result['master_trip_data']['bill_dates'] . '</td>';
      $table1 .= '<td>' . $trip_id . '</td>';
      $table1 .= '<td>' . $value['prevan'] . '</td>';
      $table1 .= '<td>' . $van_type . '</td>';
      $table1 .= '<td>';
      $table1 .= '<table  cellpadding="0" cellspacing="0" width="100%">';
      foreach ($out_put_data as $cat => $value) {
        $table1 .= '<tr><td>' . $cat . '</td></tr>';
      }
      $table1 .= '</table>';
      $table1 .= '</td>';

      // No of cld pack
      $table1 .= '<td>';
      $table1 .= '<table  cellpadding="0" cellspacing="0" width="100%">';
      foreach ($out_put_data as $cat => $value) {
        $table1 .= '<tr><td>' . $value['no_of_clds'] . '</td></tr>';
      }
      $table1 .= '</table>';
      $table1 .= '</td>';

      // CLD lines
      $table1 .= '<td>';
      $table1 .= '<table  cellpadding="0" cellspacing="0" width="100%">';
      foreach ($out_put_data as $cat => $value) {
        $table1 .= '<tr><td>' . $value['cld_lines'] . '</td></tr>';
      }
      $table1 .= '</table>';
      $table1 .= '</td>';
      // CLD value
      $table1 .= '<td>';
      $table1 .= '<table  cellpadding="0" cellspacing="0" width="100%">';
      foreach ($out_put_data as $cat => $value) {
        $table1 .= '<tr><td>' . round($value['cld_value'], $round_off) . '</td></tr>';
      }
      $table1 .= '</table>';
      $table1 .= '</td>';
      $table1 .= '</tr>';
    }
    $table1 .= '</table>';
  }
  if ($export == 1) {
    $table = render($table1);
    return $table;
  }
  $form_state['output'] = render($table1);
  $form_state['rebuild'] = TRUE;
}

function get_non_pp_report_data($bill_numbers, $distributor_id) {
  $lak = 100000;
  $round_off = 3;
  $res1 = array();
  $query = db_select('custom_billing_data', 'bd');
  //$query->addfield('bd', 'Sum_of_TOTAL_SALES_QTY', 'total_qty');
  $query->addExpression('SUM(Sum_of_TOTAL_SALES_QTY)', 'total_qty');
  $query->addfield('bd', 'sku7_code', 'sku_code');
  //$query->addfield('bd', 'bill_number', 'bill_number');
  $query->addExpression('ROUND(((Sum_of_NET_SALES_VALUE - ((Sum_of_OTHER_DISCOUNT + Sum_of_SCHEME_DISCOUNT)*(1 + (Sum_of_TAX_PERCENTAGE/100))))/Sum_of_TOTAL_SALES_QTY), 2)', 'sum_net_sales_value');
  $query->groupBy('bd.sku7_code');
  //$query->groupBy('bd.bill_number');
  $query->condition('bd.distributor_id', $distributor_id);
  $query->join('node', 'n', 'n.title = bd.sku7_code AND bd.distributor_id = n.uid');
  $query->condition('n.type', 'sku7_code_master');
  $query->join('field_data_field_unit_case_units_', 'fucu', 'fucu.entity_id = n.nid');
  $query->fields('fucu', array('field_unit_case_units__value'));
  //subcategory join
  $query->leftjoin('field_data_field_sub_category', 'fc', 'fc.entity_id = n.nid');
  $query->leftjoin('node', 'sub_category', "sub_category.nid = fc.field_sub_category_nid and sub_category.type = 'product_sub_category_master' ");
  // Join with product category
  $query->leftjoin('field_data_field_product_category', 'category_field', "category_field.entity_id = sub_category.nid ");
  $query->leftjoin('node', 'category', "category.nid = category_field.field_product_category_nid and category.type = 'product_category_master' ");
  //$query->join('node', 'n1', 'n1.title = bd.bill_number AND bd.distributor_id = n1.uid');
  //$query->condition('n1.type', 'sales_register_data');
  //$query->addfield('n1', 'nid', 'sales_nid');
  //$query->addfield('n1', 'title', 'bill_no');
  //$query->addfield('bd', 'party_code', 'party_code');
  $query->addfield('category', 'title', 'category');
  $query->addfield('sub_category', 'title', 'sub_category');
  //$query->condition('n1.nid', $total_sales_nid_array, 'IN');
  $query->condition('bd.bill_number', $bill_numbers, 'IN');
  $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
  watchdog('$res', '<pre>' . print_r($res, TRUE) . '</pre>');
  $unique_sku = $sku_array = $bill_wise_unique_sku = $unique_sku_co = array();
  $res1 = array();
  foreach ($res as &$val) {
    if (empty($val['category'])) {
      $val['category'] = 'NA';
    }
    if (!empty($val['category'])) {
      if (!isset($unique_sku_co[$val['category']][$val['sku_code']])) {
        $unique_sku_co[$val['category']][$val['sku_code']] = 1;
        $res1[$val['category']]['sku_count'] += 1;
      }
      $cld = (int) ($val['total_qty'] / $val['field_unit_case_units__value']);
      $loose = (int) ($val['total_qty'] % $val['field_unit_case_units__value']);
      $res1[$val['category']]['cld'] += 0;
      $res1[$val['category']]['no_of_clds'] += 0;
      $res1[$val['category']]['loose'] += 0;
      $res1[$val['category']]['no_loose_count'] += 0;
      $res1[$val['category']]['loose_value'] += 0;
      $res1[$val['category']]['cld_value'] += 0;
      if ($cld >= 1) {
        if (!isset($unique_sku[$val['category']]['cld'][$val['sku_code']])) {
          $unique_sku[$val['category']]['cld'][$val['sku_code']] = 1;
          $res1[$val['category']]['cld'] += 1;
          $res1[$val['category']]['no_of_clds'] += $cld;
        }
        $res1[$val['category']]['cld_value'] += ($val['sum_net_sales_value'] * $cld * $val['field_unit_case_units__value']) / $lak;
        $unique_sku[$val['category']]['cld']['total'] = $res1[$val['category']]['cld_value'];
        
      }
      if ($loose >= 1) {
         if (!isset($unique_sku[$val['category']]['loose'][$val['sku_code']])) {
          $unique_sku[$val['category']]['loose'][$val['sku_code']] = 1;
          $res1[$val['category']]['loose'] += 1;
          
        }
        if (!isset($bill_wise_unique_sku['bill_wise_loose'][$val['category']][$val['sales_nid']][$val['sku_code']])) {
          $bill_wise_unique_sku['bill_wise_loose'][$val['category']][$val['sales_nid']][$val['sku_code']] = 1;
          $res1[$val['category']]['bill_wise_loose'] += 1;
        }
        $res1[$val['category']]['loose_value'] += ($val['sum_net_sales_value'] * $loose) / $lak;
        $unique_sku[$val['category']]['loose']['total'] = $res1[$val['category']]['loose_value'];
        // not required
        //$res1[$val['category']]['no_loose_count'] += $loose;
        $res1[$val['category']]['openable_cld'][$val['sku_code']]['no_loose_count'] += (int) ($loose);
        
           // watchdog('loose', '<pre>' . print_r($loose, TRUE) . '</pre>');
      }
      $res1[$val['category']]['cld_lines'] = ($res1[$val['category']]['cld']);
      $res1[$val['category']]['loose_lines'] = $res1[$val['category']]['loose'];
      $res1[$val['category']]['no_of_clds'] = ($res1[$val['category']]['no_of_clds']);
    }
  }

  //print_r($res1[$val['category']]['openable_cld']);
  //$cld = (int) ($val['total_qty'] / $val['field_unit_case_units__value']);
  $total_loose_value = 0;
  $total_cld_value = 0;
  if (!empty($res1)) {
    foreach ($res1 as $cat_key => &$value) {
      $total_loose_value += $value['loose_value'];
      $total_cld_value += $value['cld_value'];
      if (!empty($value['openable_cld'])) {
         foreach($value['openable_cld'] as $sku => $sku_loose_value) {
           $upc = (int) get_sku_upc($sku);
            if (!empty($upc) && !empty($sku_loose_value['no_loose_count'])) {
             $openable_cld = (int) ($sku_loose_value['no_loose_count']/$upc); 
             $res1[$cat_key]['total_openable_cld'] += $openable_cld;
           }
         }
      }
    }
      watchdog('openable_cld', '<pre>' . print_r($res1, TRUE) . '</pre>');
    //watchdog('openable_cld', '<pre>' . print_r($res1, TRUE) . '</pre>');
    //$report['total']['total_loose_value'] = $total_loose_value;
    //$report['total']['total_cld_value'] = $total_cld_value;
    //for calculating percentage
    foreach ($res1 as $cat_key => &$value) {
      $loose_value_percentage_by_group = round($value['loose_value'] / $total_loose_value * 100);
      $value['loose_value_percentage_by_group'] = $loose_value_percentage_by_group;
    }
  }
  return $res1;
}

function get_non_pp_trips($mater_trip_id) {
  $result = array();
  if (!empty($mater_trip_id)) {
    $query = db_select('node', 'n');
    $query->condition('n.type', 'master_dispatch_data');
    $query->condition('n.title', $mater_trip_id);
    $query->join('master_ptl_data', 'ptl', 'ptl.ptl_job = n.title');
    $query->addTag('node_distributor_access');
    $query->join('field_data_field_master_trip_id_reference', 'master_trip_bill_ref', 'master_trip_bill_ref.field_master_trip_id_reference_nid = n.nid');
    $query->join('node', 'dispatch_data', 'dispatch_data.nid = master_trip_bill_ref.entity_id');
    $query->condition('dispatch_data.type', 'dispatch_data');
    $query->join('field_data_field_trip_id_reference', 'trip_bill_ref', 'trip_bill_ref.field_trip_id_reference_nid = dispatch_data.nid');
    $query->join('node', 'sales_register', 'sales_register.nid = trip_bill_ref.entity_id');
    $query->condition('sales_register.type', 'sales_register_data');
    $query->join('field_data_field_sr_new_bill_date', 'bd', 'bd.entity_id = sales_register.nid');
    $query->addExpression("GROUP_CONCAT(DISTINCT(DATE_FORMAT(CONVERT_TZ(FROM_UNIXTIME(bd.field_sr_new_bill_date_value),'+00:00','+05:30'), '%d-%m-%Y')))", 'bill_date');
    //$query->fields('sales_register', array('title'));
    $query->addfield('sales_register', 'title', 'bill_number');
    $query->addfield('sales_register', 'nid', 'nid');
    $query->fields('dispatch_data', array('title'));
    $query->leftjoin('field_data_field_entered_prevan', 'pvan', 'pvan.entity_id = dispatch_data.nid');
    $query->fields('pvan', array('field_entered_prevan_value'));
    $query->groupBy('sales_register.title');
    $res = $query->execute()->fetchAll(PDO::FETCH_UNIQUE | PDO::FETCH_ASSOC);
    //watchdog('non_pp_trips', '<pre>' . print_r($res, TRUE) . '</pre>');
    $master_ptl_result = array();
    if (!empty($res)) {
      foreach ($res as $bill_number => $value) {
        $result[$value['title']]['sales_nid'][$bill_number] = $bill_number;
        $result[$value['title']]['bill_date'][$value['bill_date']] = $value['bill_date'];
        $result[$value['title']]['prevan'] = $value['field_entered_prevan_value'];
        $trip_ids[$value['title']] = $value['title'];
        $bills[$bill_number] = $bill_number;
        $sales_nids[$value['nid']] = $value['nid'];
        $prevan[$value['field_entered_prevan_value']] = $value['field_entered_prevan_value'];
        $bill_date[$value['bill_date']] = $value['bill_date'];
      }
      $trip_ids = implode(',', $trip_ids);
      $prevan_data = implode(',', $prevan);
      $bill_dates = implode(',', $bill_date);
      $master_ptl_result['trip_ids'] = $trip_ids;
      $master_ptl_result['bill_numbers'] = $bills;
      $master_ptl_result['prevan'] = $prevan_data;
      $master_ptl_result['bill_dates'] = $bill_dates;
      $master_ptl_result['sales_nids'] = $sales_nids;
      //  watchdog('$master_ptl_result', '<pre>' . print_r($master_ptl_result, TRUE) . '</pre>');
      $total_result['trip_wise'] = $result;
      $total_result['master_trip_data'] = $master_ptl_result;
      // watchdog('$total_result', '<pre>' . print_r($total_result, TRUE) . '</pre>');
    }
  }
  return $total_result;
}

function validate_non_pp_report($form,&$form_state) {
    $mater_trip_id = $form_state['values']['trip_id'];
    $query = db_select('node', 'n');
    $query->condition('n.type', 'master_dispatch_data');
    $query->condition('n.title', $mater_trip_id);
    $query->join('master_ptl_data', 'ptl', 'ptl.ptl_job = n.title');
    $query->addTag('node_distributor_access');
    //
    $query->join('field_data_field_master_trip_id_reference', 'master_trip_bill_ref', 'master_trip_bill_ref.field_master_trip_id_reference_nid = n.nid');
    $query->join('node', 'dispatch_data', 'dispatch_data.nid = master_trip_bill_ref.entity_id');
    $query->condition('dispatch_data.type', 'dispatch_data');
    //
    $query->join('field_data_field_trip_id_reference', 'trip_bill_ref', 'trip_bill_ref.field_trip_id_reference_nid = dispatch_data.nid');
    $query->join('node', 'sales_register', 'sales_register.nid = trip_bill_ref.entity_id');
    $query->condition('sales_register.type', 'sales_register_data');
    $query->join('field_data_field_sr_new_bill_date', 'bd', 'bd.entity_id = sales_register.nid');
    $query->addExpression("GROUP_CONCAT(DISTINCT(DATE_FORMAT(CONVERT_TZ(FROM_UNIXTIME(bd.field_sr_new_bill_date_value),'+00:00','+05:30'), '%d-%m-%Y')))", 'bill_date');
    $query->fields('sales_register', array('nid'));
    $query->fields('dispatch_data', array('title'));
    $query->leftjoin('field_data_field_entered_prevan', 'pvan', 'pvan.entity_id = dispatch_data.nid');
    $query->fields('pvan', array('field_entered_prevan_value'));
    $query->groupBy('sales_register.title');
    $res = $query->execute()->fetchAll(PDO::FETCH_UNIQUE | PDO::FETCH_ASSOC);
    if (empty($res)) {
      form_set_error('form',t('No record found'));
    }
}

function get_sku_upc($sku_code) {
  $upc = 0;
  if (!empty($sku_code)) {
    global $user;
    $distributor_id = change_feeds_import_get_distributor_id($user->uid);
    $query = db_select('node', 'n');
    $query->condition('n.uid', $distributor_id);
    $query->condition('n.title', $sku_code);
    $query->condition('n.type', 'sku7_code_master');
    $query->join('field_data_field_unit_case_units_', 'fucu', 'fucu.entity_id = n.nid');
    $query->fields('fucu', array('field_unit_case_units__value'));
    $upc = $query->execute()->fetchField();
  }
  return $upc;
}