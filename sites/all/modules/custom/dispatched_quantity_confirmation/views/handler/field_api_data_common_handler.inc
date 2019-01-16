<?php

/**
 * @file
 * Definition of example_handler_custom_field
 */

/**
 * Provides a custom views field.
 */
class field_api_data_common_handler extends views_handler_field {

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
    
    $query = db_select('dispatch_planning_api_data', 'ad');
    $query->join('dispatch_qty_confirmation', 'dqc', 'dqc.trip_id = ad.runsheet_number AND dqc.bill_number = ad.bill_number');
    $query->leftjoin('node', 'n', 'ad.party_code = n.title');
    $query->condition('n.type', 'outlet_master');
    $query->leftjoin('field_data_field_payment_terms', 'pt', 'pt.entity_id = n.nid ');
    $query->leftjoin('field_data_field_credit_days', 'cd', 'cd.entity_id = n.nid');
    $query->condition('ad.runsheet_number', $trip_id, '=');
    $query->condition('ad.bill_number', $bill_number, '=');
    $query->condition('dqc.sku_code', $sku_code, '=');
    $query->condition('dqc.distributor_id', $distributor_id, '=');
    $query->fields('ad', array('res_money_collect2', 'res_money_collect2_edit', 'res_signed_bill_delivery', 'res_signed_bill_delivery_edit', 'res_signed_bill_partial', 'res_signed_bill_partial_edit', 'res_status'));
    $query->fields('pt', array('field_payment_terms_value'));
    $query->fields('cd', array('field_credit_days_value'));
    $result = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

    $data->res_money_collect2 =  $result[0]['res_money_collect2'];
    $data->res_money_collect2_edit =  $result[0]['res_money_collect2_edit'];
    $data->res_signed_bill_delivery =  $result[0]['res_signed_bill_delivery'];
    $data->res_signed_bill_delivery_edit =  $result[0]['res_signed_bill_delivery_edit'];
    $data->res_signed_bill_partial =  $result[0]['res_signed_bill_partial'];
    $data->res_signed_bill_partial_edit =  $result[0]['res_signed_bill_partial_edit'];
    $data->field_payment_terms_value =  $result[0]['field_payment_terms_value'];
    $data->field_credit_days_value =  $result[0]['field_credit_days_value'];
    $data->res_status =  $result[0]['res_status'];
  }
}
