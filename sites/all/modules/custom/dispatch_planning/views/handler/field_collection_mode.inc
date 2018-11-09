<?php

/**
 * @file
 * Definition of example_handler_custom_field
 */

/**
 * Provides a custom views field.
 */
class field_collection_mode extends views_handler_field {

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
    $res_money_collect2 = $data->dispatch_planning_api_data_res_money_collect2; 
    $res_money_collect2_edit = $data->dispatch_planning_api_data_res_money_collect2_edit; 
    $res_signed_bill_delivery = $data->dispatch_planning_api_data_res_signed_bill_delivery;
    $res_signed_bill_delivery_edit = $data->dispatch_planning_api_data_res_signed_bill_delivery_edit;
    $res_signed_bill_partial = $data->dispatch_planning_api_data_res_signed_bill_partial;
    $res_signed_bill_partial_edit = $data->dispatch_planning_api_data_res_signed_bill_partial_edit;
    
    if (!empty($res_money_collect2_edit)) {
      return ucfirst($res_money_collect2_edit);
    }
    else if ((!empty($res_money_collect2)) && ($res_money_collect2 != 'NA')) {
       return $res_money_collect2;
    }
    else if ($res_signed_bill_delivery_edit == 'Yes') {
      return 'Signed';
    }
    else if ($res_signed_bill_delivery == 'Yes') {
      return 'Signed';
    }
    else if ($res_signed_bill_partial_edit == 'Yes') {
      return 'Signed';
    }
    else if ($res_signed_bill_partial == 'Yes') {
      return 'Signed';
    }
  }
}
