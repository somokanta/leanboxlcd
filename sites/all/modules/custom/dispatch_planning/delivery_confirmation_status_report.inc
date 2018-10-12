<?php

function delivery_confirmation_status_callback() {
 
  // Show last 3 days data.
  for ($i = 1; $i <= 3; $i++) {
    $yesterday_date = date('Y-m-d', strtotime("-$i days"));
    $nex_load = get_next_day_load_count($i);
    $query = db_select('node', 'n');
    $query->condition('n.type', 'dispatch_data');
    $query->join('field_data_field_vrs_status', 'fdfvs', 'fdfvs.entity_id = n.nid');
    $query->join('field_data_field_dispatch_date', 'fdfdd', 'fdfdd.entity_id = n.nid');
    $query->join('field_data_field_cashier_name', 'fcn', 'fcn.entity_id = n.nid');
    $query->condition('fdfdd.field_dispatch_date_value', $yesterday_date, '=');
    $query->addExpression("COUNT(n.title)", 'count_of_trips_created');
    $query->addExpression("COUNT(CASE WHEN fdfvs.field_vrs_status_value > -1 THEN n.title END)", 'delivery_confirmation_count');
    $query->addExpression("COUNT(CASE WHEN fdfvs.field_vrs_status_value = -1 THEN n.title END)", 'delivery_confirmation_pending_count');
    $result = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
    $row = array();
    foreach ($result as $value) {
      if (!empty($yesterday_date)) {
        $row['field_dispatch_date_value'] = date('d-M-y, l', strtotime($yesterday_date));
      }
      else {
        $row['field_dispatch_date_value'] = '';
      }
      $row['Day'] = 'N -' . $i;
      $row['count_of_trips_created'] = $value['count_of_trips_created'];
      $row['delivery_confirmation_count'] = $value['delivery_confirmation_count'];
      $row['delivery_confirmation_pending_count'] = $value['delivery_confirmation_pending_count'];
      $row['next_load'] = $nex_load;
      $rows[] = $row;
    }
  }

  // Show 10 days data less than last 3 days.
  $start_date = date('Y-m-d', strtotime("-4 days"));
  $last_date = date('Y-m-d', strtotime("-10 days"));
  $query = db_select('node', 'n');
  $query->condition('n.type', 'dispatch_data');
  $query->join('field_data_field_vrs_status', 'fdfvs', 'fdfvs.entity_id = n.nid');
  $query->join('field_data_field_dispatch_date', 'fdfdd', 'fdfdd.entity_id = n.nid');
  $query->join('field_data_field_cashier_name', 'fcn', 'fcn.entity_id = n.nid');
  $query->condition('fdfdd.field_dispatch_date_value', array($last_date, $start_date), 'BETWEEN');
  $query->addExpression("COUNT(n.title)", 'count_of_trips_created');
  $query->addExpression("COUNT(CASE WHEN fdfvs.field_vrs_status_value > -1 THEN n.title END)", 'delivery_confirmation_count');
  $query->addExpression("COUNT(CASE WHEN fdfvs.field_vrs_status_value = -1 THEN n.title END)", 'delivery_confirmation_pending_count');
  $query->addExpression("COUNT(distinct CASE WHEN fdfvs.field_vrs_status_value = -1 AND field_cashier_name_value != ' ' AND  fdfdd.field_dispatch_date_value > $yesterday_date THEN field_cashier_name_value END)", 'next_load');
  $result = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
  $row = array();
  $nex_load = get_next_day_load_count(10);
  foreach ($result as $value) {
    if (!empty($start_date)) {
      $row['field_dispatch_date_value'] = '<' . ' ' . date('d-M-y, l', strtotime($start_date)) . ' ' . '(Last 10 Days)';
    }
    else {
      $row['field_dispatch_date_value'] = '';
    }
    $row['Day'] = 'N -' . $i;
    $row['count_of_trips_created'] = $value['count_of_trips_created'];
    $row['delivery_confirmation_count'] = $value['delivery_confirmation_count'];
    $row['delivery_confirmation_pending_count'] = $value['delivery_confirmation_pending_count'];
    
    $row['next_load'] = $nex_load;
    $rows[] = $row;
  }
  $header = array('Dispatch Date', 'Day', 'Count of Trips created', 'Delivery Confirmed Count', 'Delivery Confirmation Pending Count', 'Next Load given despite Delivery Confirmation Pending');
  $html = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('width' => '100%')));
  cache_set('delivery-confirmation-status-report' . session_id(), $html, 'cache', time() + 60 * 10);
  $output .= l(t('XLS'), 'delivery-confirmation-status-report/export');
  return $html.$output;
}

function delivery_confirmation_status_export() {
  $file = "Delivery_Confirmation_Status_Report.xls";
  $data = cache_get('delivery-confirmation-status-report' . session_id());
  if (empty($data)) {
    $table = delivery_confirmation_status_callback();
  }
  else {
    $table = $data->data;
  }
  header("Content-type: application/vnd.ms-excel");
  header("Content-Disposition: attachment; filename=$file");
  echo $table;
}

function get_next_day_load_count($i) {

  $yesterday_date = date('Y-m-d', strtotime("-$i days"));
  $next_day = date('Y-m-d', strtotime($yesterday_date . ' +1 day'));
  $nex_day_cashiers = get_next_day_cashier_names($next_day);
  $query = db_select('node', 'n');
  $query->condition('n.type', 'dispatch_data');
  $query->join('field_data_field_vrs_status', 'fdfvs', 'fdfvs.entity_id = n.nid');
  $query->join('field_data_field_dispatch_date', 'fdfdd', 'fdfdd.entity_id = n.nid');
  $query->join('field_data_field_cashier_name', 'fcn', 'fcn.entity_id = n.nid');
  $query->condition('fdfdd.field_dispatch_date_value', $yesterday_date, '=');
  $query->fields('fcn', array(' field_cashier_name_value'));
  $cashier_names = $query->execute()->fetchAll(PDO::FETCH_UNIQUE | PDO::FETCH_ASSOC);
  if (!empty($cashier_names)) {
    $cashier_names = array_keys($cashier_names);
  }
  $common_cashiers = array_intersect($nex_day_cashiers, $cashier_names);

  $query = db_select('node', 'n');
  $query->condition('n.type', 'dispatch_data');
  $cashier_names_count = 0;
  if (!empty($common_cashiers)) {
    $cashier_names = array_keys($cashier_names);
    $query->condition('fcn.field_cashier_name_value', $common_cashiers, 'IN');
    $cashier_names_count = 1;
  }

  $query->join('field_data_field_vrs_status', 'fdfvs', 'fdfvs.entity_id = n.nid');
  $query->join('field_data_field_dispatch_date', 'fdfdd', 'fdfdd.entity_id = n.nid');
  $query->join('field_data_field_cashier_name', 'fcn', 'fcn.entity_id = n.nid');
  $query->condition('fdfdd.field_dispatch_date_value', $yesterday_date, '=');
  $query->addExpression("COUNT(CASE WHEN fdfvs.field_vrs_status_value = -1 AND $cashier_names_count !=0 AND  fdfdd.field_dispatch_date_value >  $yesterday_date THEN field_cashier_name_value END)", 'next_load');
  $query->groupBY('fdfdd.field_dispatch_date_value');
  $result = $query->execute()->fetchField();
  return $result;
}

function get_next_day_cashier_names($next_day) {
  if (!empty($next_day)) {
    $query = db_select('node', 'n');
    $query->condition('n.type', 'dispatch_data');
    $query->join('field_data_field_vrs_status', 'fdfvs', 'fdfvs.entity_id = n.nid');
    $query->join('field_data_field_dispatch_date', 'fdfdd', 'fdfdd.entity_id = n.nid');
    $query->join('field_data_field_cashier_name', 'fcn', 'fcn.entity_id = n.nid');
    $query->condition('fdfdd.field_dispatch_date_value', $next_day, '=');
    $query->fields('fcn', array(' field_cashier_name_value'));
    $cashier_names = $query->execute()->fetchAll(PDO::FETCH_UNIQUE | PDO::FETCH_ASSOC);

    if (!empty($cashier_names)) {
      $cashier_names = array_keys($cashier_names);
    }
  }
  return $cashier_names;
}
