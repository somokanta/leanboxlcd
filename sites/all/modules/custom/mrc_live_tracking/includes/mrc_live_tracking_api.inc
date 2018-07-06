<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function get_live_tracking_van_api_data($data) {
  if (!empty($data)) {
    foreach ($data as $value) {
      $rs_code_string = explode("_", $value['employeeCode']);
      $rs_code = $rs_code_string[0];
      $distributor_id = get_distributor_id_using_rscode($rs_code);
      if (!empty($distributor_id)) {
//        db_merge('van_live_tracking')
//          ->key(array('employeecode' => $value['employeeCode']))
          db_insert('van_live_tracking')//ForGraph
          ->fields(array(
            'employeecode' => $value['employeeCode'],
            'tracktime' => strtotime($value['trackTime']),
            'latitude' => $value['latitude'],
            'longitude' => $value['longitude'],
            'gpssignal' => $value['gpsSignal'],
            'speed' => $value['speed'],
            'battery' => $value['battery'],
            'signalaccuracy' => $value['signalAccuracy'],
            'altitude' => $value['altitude'],
            'bearing' => $value['bearing'],
            'extra1' => $value['extra1'],
            'extra2' => $value['extra2'],
            'extra3' => $value['extra3'],
            'distributor_id' => $distributor_id ? $distributor_id : 0, // hardcoded value because no parameter in api
            'created_time' => REQUEST_TIME,
          ))
          ->execute();
      }
    }

    $results['flag'] = '1';
    $results['result'] = 'Data updated Sucessfully.';
  }
  else {
    $results['flag'] = '0';
    $results['result'] = 'No data was updated.';
  }

  return $results;
}
