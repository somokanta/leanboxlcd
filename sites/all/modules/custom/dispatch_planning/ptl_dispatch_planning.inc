<?php

/*
 * it will call from, "ptl-api-data" services
 * 
 */

function fetch_ptl_data($rs_code , $api_time) {
  $query = db_select('ptl_data_configuration', 'cb');
  $query->addExpression('MAX(id)', 'max');
  $query->condition('api_flag', '0');
  $query->condition('rs_code', $rs_code, '=');
  $max_ptl_data = $query->execute()->fetchField();

  $query = db_select('ptl_data_configuration', 'p');
  $query->fields('p', array('ptl_job', 'rs_code', 'trip_id', 'vehicle', 'bill_number', 'loading_date', 'sequence_no', 'basepack_code', 'sku_code', 'product_name', 'MRP', 'TUR', 'sum_sales_qty'));
  $query->condition('p.api_flag', '0', '=');
  $query->condition('p.rs_code', $rs_code, '=');
  $query->condition('p.id', $max_ptl_data, '<=');

  $result = $query->execute()->fetchAll();
  
  if (!empty($result)) {
    $results['result'] = json_encode($result);
    $results['flag'] = '1';
    
    db_update('ptl_data_configuration') // Table name no longer needs {}
      ->fields(array(
        'fetch_time' => $api_time,
      ))
      ->condition('id', $max_ptl_data, '<=')
      ->condition('api_flag', '0', '=')
      ->condition('rs_code', $rs_code, '=')
      ->execute();
  }
  else {
    $results['flag'] = '0';
    $results['result'] = 'No Result';
  }
  // log entry and is remaining

  return $results;
}

/*
 * it will call from, "ptl-api-data" services to update the time
 * 
 */

function update_ptl_data($rs_code, $trip_id, $update_time) {
  db_update('ptl_data_configuration') // Table name no longer needs {}
    ->fields(array(
      'update_time' => $update_time,
      'api_flag' => 1,
    ))
    ->condition('rs_code', $rs_code, '=')
    ->condition('trip_id', $trip_id, '=')
    ->execute();

  $results['flag'] = '1';
  $results['result'] = 'Updated Successfully';

  return $results;
}
