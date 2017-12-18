<?php

/**
 *  Filter form for pendency report
 */
function pendency_report_filter_form($form, &$form_state) {
  if (empty(arg(2))) {
    $title = 'Loading';
  }
  else {
    $title = 'Billing';
  }

  $start_date = isset($_GET['start_date']) ? $_GET['start_date'] : date('Y-m-d', strtotime("-1 days"));
  $end_date = isset($_GET['end_date']) ? $_GET['end_date'] : date('Y-m-d', strtotime("-1 days"));

  $form['start_date'] = array(
    '#type' => 'date_popup',
    '#title' => "$title From date",
    '#description' => 'Select from date to generate report',
    '#require' => TRUE,
    '#date_label_position' => 'within',
    '#date_format' => 'Y-m-d',
    '#default_value' => $start_date,
  );
  $form['end_date'] = array(
    '#type' => 'date_popup',
    '#title' => "$title To date",
    '#description' => 'Select to date to generate report',
    '#require' => TRUE,
    '#date_label_position' => 'within',
    '#date_format' => 'Y-m-d',
    '#default_value' => $end_date,
  );

  $form['save'] = array(
    '#type' => 'submit',
    '#value' => 'Save'
  );
  return $form;
}

/**
 * submit handler for pendency_report_filter_form
 */
function pendency_report_filter_form_submit($form, &$form_state) {
  $start_date = $form_state['values']['start_date'];
  $end_date = $form_state['values']['end_date'];
  if (($form_state['values']["op"] == "Save") && (arg(1) == 'reports' && empty(arg(2)))) {
    drupal_goto('pendency/reports', array('query' => array('start_date' => $start_date, 'end_date' => $end_date)));
  }
  else if (($form_state['values']["op"] == "Save") && (arg(2) == 'sales-beat-wise')){
    drupal_goto('pendency/reports/sales-beat-wise', array('query' => array('start_date' => $start_date, 'end_date' => $end_date)));
  }
  else if (($form_state['values']["op"] == "Save") && (arg(2) == 'pl-group-wise')){
    drupal_goto('pendency/reports/pl-group-wise', array('query' => array('start_date' => $start_date, 'end_date' => $end_date)));
  }
}

/**
 * Validation for pendency_report_filter_form
 */
function pendency_report_filter_form_validate($form, &$form_state) {
  $start_date = $form_state['values']['start_date'];
  $end_date = $form_state['values']['end_date']; 
  
  if (empty($start_date) || empty($end_date)) {
    form_set_error('form', 'Please select From and To dates both.'); 
  }
  if (strtotime($start_date) > strtotime($end_date)){
    form_set_error('form', 'From Date should be less than To Date.');
  }
}

function pendency_report_export() {
  $file = "PendencyReportVanWise.xls";
  $data = cache_get('pendency/reports'.session_id());
  if (empty($data)) {
    $table = pendency_report_callback();
  }
  else {
    $table = $data->data;
  }
  header("Content-type: application/vnd.ms-excel");
  header("Content-Disposition: attachment; filename=$file");
  echo $table;
}

function pendency_report_sales_beat_wise_export() {
  $file = "PendencyReportSalesBeatWise.xls";
  $data = cache_get('pendency/reports/sales-beat-wise'.session_id());
  if (empty($data)) {
    $table = pendency_report_sales_beat_wise_callback();
  }
  else {
    $table = $data->data;
  }
  header("Content-type: application/vnd.ms-excel");
  header("Content-Disposition: attachment; filename=$file");
  echo $table;
}

function pendency_report_pl_group_wise_export() {
  $file = "PendencyReportPlGroupWise.xls";
  $data = cache_get('pendency/reports/pl-group-wise'.session_id());
  if (empty($data)) {
    $table = pendency_report_pl_group_wise_callback();
  }
  else {
    $table = $data->data;
  }
  header("Content-type: application/vnd.ms-excel");
  header("Content-Disposition: attachment; filename=$file");
  echo $table;
}


/*
 * Page Callback for Pendency Report Van Wise
 */
function pendency_report_callback() {
  global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);
  
  if(arg(2) != 'export'){
    $form = drupal_render(drupal_get_form('pendency_report_filter_form'));
  }
  
  if (isset($_GET['start_date']) && isset($_GET['end_date'])) {
    $start_date = strtotime($_GET['start_date']);
    $end_date = strtotime($_GET['end_date']);
  
    $query = db_select('node', 'n');
    $query->condition('n.type', 'sales_register_data');
    $query->condition('n.uid', $distributor_id, '=');
    $query->join('field_data_field_billing_van', 'bva', 'bva.entity_id = n.nid');
    $query->leftjoin('field_data_field_billing_loading_date', 'bld', 'bld.entity_id = n.nid');
    $query->condition('bld.field_billing_loading_date_value', array($start_date, $end_date), 'BETWEEN');
    $query->groupBy('field_billing_loading_date_value');
    $query->join('field_data_field_bill_value', 'bv', 'bv.entity_id = n.nid');
    $query->join('field_data_field_billing_van', 'v', 'v.entity_id = n.nid');
    $query->groupBy('field_billing_van_value');

    $query->leftjoin('field_data_field_trip_id_reference', 'ti', 'ti.entity_id = n.nid');
    $query->leftjoin('node', 'disn', 'disn.nid = ti.field_trip_id_reference_nid');
    $query->leftjoin('field_data_field_dispatch_date', 'fd', 'fd.entity_id = disn.nid');


    $query->addExpression('COUNT(n.title)', 'bill_count');
    $query->addExpression('SUM(bv.field_bill_value_value)', 'bill_value');

    $query->addExpression('SUM(if(fd.field_dispatch_date_value IS NOT NULL, 1, 0))', 'trip_dispatch_sum');
    $query->addExpression('SUM(if(fd.field_dispatch_date_value IS NOT NULL, field_bill_value_value, 0))', 'trip_dispatch_value');
    $query->fields('bld', array('field_billing_loading_date_value'));
    $query->fields('v', array('field_billing_van_value'));
    //$query->fields('n', array('nid'));

    $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

    if (!empty($res)) {
      $count = 1;
      foreach ($res as $k => $v) {
        $serial_no = $count;
        $loading_date = date('d-m-Y',$v['field_billing_loading_date_value']);
        $van = $v['field_billing_van_value'];
        $bill_count = $v['bill_count'];
        $bill_val = $v['bill_value'];
        $bill_val = round($bill_val); 
        $dispatch_bill_count = $v['trip_dispatch_sum'];
        $dispatch_bill_value = $v['trip_dispatch_value'];
        $pending_bill = ($bill_count - $dispatch_bill_count);
        $pending_bill_value = ($bill_val - $dispatch_bill_value);

        $total_bill_count += $bill_count;
        $total_bill_val += $bill_val;
        $total_trip_dispatch_sum += $dispatch_bill_count;
        $total_trip_dispatch_value += $dispatch_bill_value;
        $total_pending_bill_count += $pending_bill;
        $total_pending_bill_value += $pending_bill_value;

        $rows[] = array($serial_no,
          $loading_date,
          $van,
          $bill_count,
          $bill_val,
          $dispatch_bill_count,
          $dispatch_bill_value,
          $pending_bill,
          $pending_bill_value
        );
      $count ++;
      }
    }
    else {
      $output = "No Result Found";
    }
    if (!empty($res)) {
      $header = array('Serial No', 'Loading Date', 'Van', 'Bill Count', 'Bill Value', 'Dispatch Count', 'Dispatch Value', 'Pending Bill', 'Pending Bill Value');
      $rows[] = array('Total',
        '',
        '',
        $total_bill_count,
        $total_bill_val,
        $total_trip_dispatch_sum,
        $total_trip_dispatch_value,
        $total_pending_bill_count,
        $total_pending_bill_value
      );

      $output = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('border' => '1', 'cellpadding' => '5', 'cellspacing' => '0')));
  
    // To create the download link
    $data = array();
    $data['start_date'] = $_GET['start_date'];
    $data['end_date'] = $_GET['end_date'];

    //newly added, 2 lines
    cache_set('pendency/reports'.session_id(), $output, 'cache', time() + 60*10);
    $output .= l(t('XLS'), 'pendency/reports/export', array('query' => array($data)));
    
    }
  }
  return $form.$output;
}

/*
 * Page Callback for Pendency Report Sales Beat Wise
 */
function pendency_report_sales_beat_wise_callback() {
  global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);
  
  if(arg(3) != 'export'){
    $form = drupal_render(drupal_get_form('pendency_report_filter_form'));
  }
    
  if (isset($_GET['start_date']) && isset($_GET['end_date'])) {
    $start_date = strtotime($_GET['start_date']);
    $end_date = strtotime($_GET['end_date']);
  
    $query = db_select('node', 'n');
    $query->condition('n.type', 'sales_register_data');
    $query->condition('n.uid', $distributor_id, '=');
    $query->join('field_data_field_sr_bill_date', 'bd', 'bd.entity_id = n.nid');
    $query->groupBy('field_sr_bill_date_value');
    $query->condition('bd.field_sr_bill_date_value', array($start_date, $end_date), 'BETWEEN');
    $query->orderBy('field_sr_bill_date_value', 'DESC');
    $query->join('field_data_field_bill_value', 'bv', 'bv.entity_id = n.nid');
    $query->join('field_data_field_party_code_reference', 'pc', 'pc.entity_id = n.nid');
    $query->join('field_data_field_beat_name', 'sb', 'sb.entity_id = pc.field_party_code_reference_nid');
    $query->groupBy('field_beat_name_value');
    $query->join('field_data_field_salesman', 'sm', 'sm.entity_id = pc.field_party_code_reference_nid');
    $query->groupBy('field_salesman_value');

    $query->leftjoin('field_data_field_trip_id_reference', 'ti', 'ti.entity_id = n.nid');
    $query->leftjoin('field_data_field_dispatch_date', 'disp_date', 'disp_date.entity_id = ti.field_trip_id_reference_nid');

    // bill counts
    $query->addExpression('SUM(CASE WHEN (disp_date.field_dispatch_date_value IS NULL) THEN 1 ELSE 0 END)', 'bill_count');

    // bill values
    $query->addExpression('SUM(CASE WHEN (disp_date.field_dispatch_date_value IS NULL) THEN field_bill_value_value ELSE 0 END)', 'bill_value');

    $query->addExpression('SUM(if(disp_date.field_dispatch_date_value IS NOT NULL, 1, 0))', 'trip_dispatch_sum');
    $query->addExpression('SUM(if(disp_date.field_dispatch_date_value IS NOT NULL, field_bill_value_value, 0))', 'trip_dispatch_value');

    $query->fields('bd', array('field_sr_bill_date_value'));
    $query->fields('sb', array('field_beat_name_value'));
    $query->fields('sm', array('field_salesman_value'));

    $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

    $row = array();
    $counter = 1;
    foreach ($res as $key => $data) {
      if ($data['bill_count'] != 0 && $data['bill_value'] != 0) {
        $row['serial_no'] = $counter++;
        $row['bill_date'] = date('d-m-Y', $data['field_sr_bill_date_value']);
        $row['beat_name'] = check_plain($data['field_beat_name_value']);
        $row['salesman'] = check_plain($data['field_salesman_value']);
        $row['bill_count'] = $data['bill_count'];
        $row['bill_value'] = $data['bill_value'];
        $row['dispatch_bill_count'] = $data['trip_dispatch_sum'];
        $row['dispatch_bill_value'] = $data['trip_dispatch_value'];
        $row['pending_bill'] = ($data['bill_count'] - $data['trip_dispatch_sum']);
        $row['pending_bill_value'] = ($data['bill_value'] - $data['trip_dispatch_value']);

        $rows[] = $row;
      }
    }

    $header = array('Sr. No.', 'Bill Date', 'Sales Beat', 'Salesman', 'Bill Count', 'Bill Value', 'Dispatch Count', 'Dispatch Value', 'Pending Bill', 'Pending Bill Value');
      
    $output = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('border' => '1', 'cellpadding' => '5', 'cellspacing' => '0'), 'empty' => t('No Records has been found')));
  
    // To create the download link
    $data = array();
    $data['start_date'] = $_GET['start_date'];
    $data['end_date'] = $_GET['end_date'];

    //newly added, 2 lines
    cache_set('pendency/reports/sales-beat-wise'.session_id(), $output, 'cache', time() + 60*10);
    $output .= l(t('XLS'), 'pendency/reports/sales-beat-wise/export', array('query' => array($data)));
    
  }
  return $form.$output;
}

/*
 * Page Callback for Pendency Report Pl Group Wise
 */
function pendency_report_pl_group_wise_callback() {
  global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);
  
  if(arg(3) != 'export'){
    $form = drupal_render(drupal_get_form('pendency_report_filter_form'));
  }
    
  if (isset($_GET['start_date']) && isset($_GET['end_date'])) {
    $start_date = strtotime($_GET['start_date']);
    $end_date = strtotime($_GET['end_date']);
    
    $query = db_select('node', 'n');
    $query->condition('n.type', 'sales_register_data');
    $query->condition('n.uid', $distributor_id, '=');
    $query->join('field_data_field_billing_van', 'bva', 'bva.entity_id = n.nid');
    $query->join('field_data_field_sr_bill_date', 'bd', 'bd.entity_id = n.nid');
    $query->condition('bd.field_sr_bill_date_value', array($start_date, $end_date), 'BETWEEN');
    $query->join('field_data_field_bill_value', 'bv', 'bv.entity_id = n.nid');
    $query->join('field_data_field_sales_party_code', 'pc', 'pc.entity_id = n.nid');
    $query->join('node', 'oun', 'pc.field_sales_party_code_value = oun.title');
    $query->condition('oun.type', 'outlet_master');
    $query->condition('oun.uid', $distributor_id, '=');
    $query->join('field_data_field_pl_group', 'plg', 'plg.entity_id = oun.nid');
    $query->groupBy('field_pl_group_value');
    $query->leftjoin('field_data_field_trip_id_reference', 'ti', 'ti.entity_id = n.nid');
    $query->leftjoin('node', 'disn', 'disn.nid = ti.field_trip_id_reference_nid');
    $query->leftjoin('field_data_field_dispatch_date', 'fd', 'fd.entity_id = disn.nid');
 
    $query->addExpression('COUNT(n.title)', 'bill_count');
    $query->addExpression('SUM(bv.field_bill_value_value)', 'bill_value');
  
    $query->addExpression('SUM(if(fd.field_dispatch_date_value IS NOT NULL, 1, 0))', 'trip_dispatch_sum');
    $query->addExpression('SUM(if(fd.field_dispatch_date_value IS NOT NULL, field_bill_value_value, 0))', 'trip_dispatch_value');
    $query->fields('bd', array('field_sr_bill_date_value'));
    $query->fields('plg', array('field_pl_group_value'));
    //$query->fields('n', array('nid'));
 
    $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

    if (!empty($res)) {
      foreach ($res as $k => $v) {
        $pl_group = $v['field_pl_group_value'];
        $bill_val = $v['bill_value'];
        $bill_val = round($bill_val);
        $pending_bill = ($v['bill_count'] - $v['trip_dispatch_sum']);
        $pending_bill_value = ($bill_val - $v['trip_dispatch_value']);

        $total_bill_count += $v['bill_count'];
        $total_bill_val += $bill_val;
        $total_trip_dispatch_sum += $v['trip_dispatch_sum'];
        $total_trip_dispatch_value += $v['trip_dispatch_value'];
        $total_pending_bill_count += $pending_bill;
        $total_pending_bill_value += $pending_bill_value;

        $rows[] = array($pl_group,
          $v['bill_count'],
          $bill_val,
          $v['trip_dispatch_sum'],
          $v['trip_dispatch_value'],
          $pending_bill,
          $pending_bill_value
        );
      }
    }
    else {
      $output = "No Result Found";
    }
    if (!empty($res)) {
      $header = array('PL Group', 'Bill Count', 'Bill Value', 'Dispatch Count', 'Dispatch Value', 'Pending Bill', 'Pending Bill Value');

      $rows[] = array("Total",
        $total_bill_count,
        $total_bill_val,
        $total_trip_dispatch_sum,
        $total_trip_dispatch_value,
        $total_pending_bill_count,
        $total_pending_bill_value
    );

      $output = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('border' => '1', 'cellpadding' => '5', 'cellspacing' => '0')));
    
      
      // To create the download link
      $data = array();
      $data['start_date'] = $_GET['start_date'];
      $data['end_date'] = $_GET['end_date'];

      //newly added, 2 lines
      cache_set('pendency/reports/pl-group-wise'.session_id(), $output, 'cache', time() + 60*10);
      $output .= l(t('XLS'), 'pendency/reports/pl-group-wise/export', array('query' => array($data)));
      
    }
  }
  return $form.$output;
}