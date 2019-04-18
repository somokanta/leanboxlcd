<?php

/**
 * @file
 * Definition of example_handler_custom_field
 */

/**
 * Provides a custom views field.
 */
class field_cheque_date_handler extends views_handler_field {

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
    $res_cheque_date = $data->dispatch_planning_api_data_res_cheque_date ? $data->dispatch_planning_api_data_res_cheque_date : '';
    $res_cheque_date_edit = $data->dispatch_planning_api_data_res_cheque_date_edit ? $data->dispatch_planning_api_data_res_cheque_date_edit : '';
    $derived_payment_outcome = $data->dispatch_planning_api_data_derived_payment_outcome;
    if ($derived_payment_outcome == 'Cheque' ||  $derived_payment_outcome == 'Cheque,Cheque'  ||  $derived_payment_outcome == 'Cheque,Cash'  ||  $derived_payment_outcome == 'Cash,Cheque') {
      if (!empty($res_cheque_date_edit)) {
        return date('d-M-y', strtotime($res_cheque_date_edit));
      }
      else {
        return date('d-M-y', $res_cheque_date);
      }
    }
  }
}
