<?php

function delivery_confirmation_status_callback() {
 
  // Show last 3 days data.
  for ($i = 1; $i <= 3; $i++) {
    $yesterday_date = date('Y-m-d 0:0:0', strtotime("-$i days"));
    $query = db_select('node', 'n');
    $query->condition('n.type', 'dispatch_data');
    $query->join('field_data_field_vrs_status', 'fdfvs', 'fdfvs.entity_id = n.nid');
    $query->join('field_data_field_dispatch_date', 'fdfdd', 'fdfdd.entity_id = n.nid');
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
  $query->condition('fdfdd.field_dispatch_date_value', array($last_date, $start_date), 'BETWEEN');
  $query->addExpression("COUNT(n.title)", 'count_of_trips_created');
  $query->addExpression("COUNT(CASE WHEN fdfvs.field_vrs_status_value > -1 THEN n.title END)", 'delivery_confirmation_count');
  $query->addExpression("COUNT(CASE WHEN fdfvs.field_vrs_status_value = -1 THEN n.title END)", 'delivery_confirmation_pending_count');
  $result = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
  $row = array();
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
    $rows[] = $row;
  }
  $header = array('Dispatch Date', 'Day', 'Count of Trips created', 'Delivery Confirmed Count', 'Delivery Confirmation Pending Count');
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
