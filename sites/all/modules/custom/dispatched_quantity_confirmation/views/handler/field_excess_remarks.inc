<?php

/**
 * @file
 * Definition of example_handler_custom_field
 */

/**
 * Provides a custom views field.
 */
class field_excess_remarks extends views_handler_field {

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
    global $user;
    $dis_id = change_feeds_import_get_distributor_id($user->uid);
    $trip_id = $data->dispatch_qty_confirmation_trip_id;
    $bill_number = $data->dispatch_qty_confirmation_bill_number;
    $sku_code = $data->dispatch_qty_confirmation_sku_code;

    $query = db_select('excess_qty', 'exc');
    $query->condition('exc.trip_id', $trip_id);
    $query->condition('exc.bill_number', $bill_number);
    $query->condition('exc.sku_code', $sku_code);
    $query->condition('exc.distributor_id', $dis_id);
    $query->fields('exc', array('excess_remarks'));
    $res = $query->execute()->fetchField();
    
    return $res;
  }
}

