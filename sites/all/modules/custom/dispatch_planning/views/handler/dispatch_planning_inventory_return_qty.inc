<?php

/**
 * @file
 * Definition of example_handler_custom_field
 */

/**
 * Provides a custom views field.
 */
class dispatch_planning_inventory_return_qty extends views_handler_field {

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
     $data->dispatch_planning_item_details_dispatch_planning_api_data_su;
     $data->dispatch_planning_item_details_dispatch_planning_api_data_re;
     $return_qty = '';
     if($data->dispatch_planning_item_details_dispatch_planning_api_data_re){
       $return_qty = $data->dispatch_planning_item_details_dispatch_planning_api_data_su-  $data->dispatch_planning_item_details_dispatch_planning_api_data_re;
     }
    return $return_qty;
   }
}
