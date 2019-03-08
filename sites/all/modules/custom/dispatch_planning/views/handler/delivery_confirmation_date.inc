<?php

/**
* @file
* Definition of example_handler_custom_field
*/

/**
* Provides a custom views field.
*/
class delivery_confirmation_date extends views_handler_field {
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

 function render(&$data) {
   global $user;
   $distributor_id = change_feeds_import_get_distributor_id($user->uid);
   $trip_id = $data->dispatch_planning_api_data_runsheet_number;
   
   $query = db_select('inventory_versioning_api_data', 'vad');
   $query->fields('vad', array('version_created_time'));
   $query->orderBy('version_created_time', 'DESC');
   $query->condition('vad.runsheet_number', $trip_id, '=');
   $query->condition('vad.distributor_id', $distributor_id, '=');
   $query->range(0, 1);
   $result = $query->execute()->fetchField();
   
   $delivery_confirmation_date = date('d-M-y h:i', $result);
   return $delivery_confirmation_date;
 }

}
