<?php

/**
 * @file
 * Definition of example_handler_custom_field
 */

/**
 * Provides a custom views field.
 */
class field_item_details_common_handler extends views_handler_field {

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
    $user_id = $user->uid;
    $distributor_id = change_feeds_import_get_distributor_id($user_id);
    $trip_id = $data->dispatch_qty_confirmation_trip_id;
    $bill_number = $data->dispatch_qty_confirmation_bill_number;
    $sku_code = $data->dispatch_qty_confirmation_sku_code;
    
    $query = db_select('dispatch_planning_item_details', 'dpid');
    $query->join('dispatch_planning_api_data', 'ad', 'ad.id = dpid.trip_bill_reference');
    $query->condition('ad.runsheet_number', $trip_id, '=');
    $query->condition('ad.bill_number', $bill_number, '=');
    $query->condition('dpid.sku_code', $sku_code, '=');
    $query->condition('ad.distributor_id', $distributor_id, '=');
    $query->fields('dpid', array('res_godown_short', 'claimed_godown_short', 'return_qty', 'res_cashier_short'));
    $result = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

    $data->res_godown_short =  $result[0]['res_godown_short'];
    $data->claimed_godown_short =  $result[0]['claimed_godown_short'];
    $data->return_qty =  $result[0]['return_qty'];
    $data->res_cashier_short =  $result[0]['res_cashier_short'];
  }
}

