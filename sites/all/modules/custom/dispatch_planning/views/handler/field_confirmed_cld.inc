<?php

/**
 * @file
 * Definition of example_handler_custom_field
 */

/**
 * Provides a custom views field.
 */
class field_confirmed_cld extends views_handler_field {

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
    $user_id = $user->uid;
    $distributor_id = change_feeds_import_get_distributor_id($user_id);
    $trip_id = $this->view->args[0];
    $sku_code = $data->ptl_data_configuration_sku_code;
    
    $query = db_select('dispatch_qty_confirmation', 'dqc');
    $query->condition('dqc.trip_id', $trip_id, '=');
    $query->condition('dqc.sku_code', $sku_code, '=');
    $query->addExpression("SUM(dqc.cld_confirmed_qty)", 'total_cld_confirmed_qty');
    $res = $query->execute()->fetchField();
    $data->total_confirmed_cld += (int)$res;
    return $res;
  }
}
