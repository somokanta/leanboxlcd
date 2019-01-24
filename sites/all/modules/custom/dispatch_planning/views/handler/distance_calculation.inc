<?php

/**
 * @file
 * Definition of example_handler_custom_field
 */

/**
 * Provides a custom views field.
 */

class distance_calculation extends views_handler_field {
  
  function option_definition() {
      $options = parent::option_definition();
      return $options;
   }

   function options_form(&$form, &$form_state) {
      parent::options_form($form, $form_state);
   }

   function query() {
      // do nothing -- to override the parent query.
   }
   
   function render($data) {
    $latitudeFrom = $data->dispatch_planning_api_data_lat_longlatitude;
    $longitudeFrom = $data->dispatch_planning_api_data_lat_longlongitude;
    $latitudeTo = $data->dispatch_planning_api_data_res_latitude;
    $longitudeTo = $data->dispatch_planning_api_data_res_longitude;
    $earthRadius = 6371;
    
    $latFrom = deg2rad($latitudeFrom);
    $lonFrom = deg2rad($longitudeFrom);
    $latTo = deg2rad($latitudeTo);
    $lonTo = deg2rad($longitudeTo);

    $latDelta = $latTo - $latFrom;
    $lonDelta = $lonTo - $lonFrom;

    $angle = 2 * asin(sqrt(pow(sin($latDelta / 2), 2) +
          cos($latFrom) * cos($latTo) * pow(sin($lonDelta / 2), 2)));
    return $angle * $earthRadius;
  }
}


