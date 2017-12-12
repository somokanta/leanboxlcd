<?php

/*
 * it will call from, "farey_data" services
 * 
 */

function fetch_dispatch_planning_api_data($data) {
  
   //watchdog('dataaa', '<pre>' . print_r($data, TRUE), '</pre>');
  if (!empty($data)) {
    $results['result'] = 'hyghghghg';//json_encode($data);
    $results['flag'] = '1';

//    db_update('ptl_data_configuration') // Table name no longer needs {}
//      ->fields(array(
//        'fetch_time' => $api_time,
//      ))
//      ->condition('id', $max_ptl_data, '<=')
//      ->condition('api_flag', '0', '=')
//      ->condition('rs_code', $rs_code, '=')
//      ->condition('loose', 1, '>=')
//      ->execute();
  }
  else {
    $results['flag'] = '0';
    $results['result'] = 'No Result';
  }
  // log entry and is remaining
  return $results;
}