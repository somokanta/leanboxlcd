<?php

/**
 * @file
 * Definition of example_handler_custom_field
 */

/**
 * Provides a custom views field.
 */
class field_photo_cheque_handler extends views_handler_field {

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
    $trip_id = $data->dispatch_planning_api_data_runsheet_number;
    $bill_number = $data->dispatch_planning_api_data_bill_number;
    $distributor_id = $data->dispatch_planning_api_data_distributor_id;
    
    $query = db_select('vrs_trip_items', 'vrs_trip');
    $query->condition('vrs_trip.trip_id', $trip_id, "=");
    $query->condition('vrs_trip.bill_number', $bill_number, "=");
    $query->condition('vrs_trip.distributor_id', $distributor_id, "=");
    $query->fields('vrs_trip', array('photo_cheque'));
    $photo_cheque = $query->execute()->fetchField();
    
    return $photo_cheque;
   }

}


