<?php

/**
 * @file
 * Definition of example_handler_custom_field
 */

/**
 * Provides a custom views field.
 */
class field_collection_value extends views_handler_field {

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
    $trip_id = $data->dispatch_planning_api_data_runsheet_number;
    $bill_num = $data->dispatch_planning_api_data_bill_number;
    $bill_num = preg_replace('/\s+/', '', $bill_num);
    $cash_edit = $data->dispatch_planning_api_data_res_cash_amount_edit;
    $chq_edit = $data->dispatch_planning_api_data_res_cheque_amount_edit;
    $sign_edit = $data->dispatch_planning_api_data_res_confirm_signed_bill_amount_ed;
    $cash_amt = $data->dispatch_planning_api_data_res_cash_amount;
    $chq_amt = $data->dispatch_planning_api_data_res_cheque_amount;
    $sign_amt = $data->dispatch_planning_api_data_res_confirm_signed_bill_amount;
    $money_collect2_edit = $data->dispatch_planning_api_data_res_money_collect2_edit;

    $query1 = db_select('vrs_trip_items', 'vti');
    $query1->condition('vti.trip_id', $trip_id, "=");
    $query1->condition('vti.bill_number', $bill_num, "=");
    $query1->condition('vti.distributor_id', $distributor_id, "=");
    $query1->addExpression('(vti.actual_cash_amount + vti.actual_cheque_amount + vti.actual_signed_bill_amount)', 'total_amount');
    $res = $query1->execute()->fetchField();
    if ($res != 0) {
      return $res;
    }
    else if ($money_collect2_edit) {
        $total_edit = $cash_edit + $chq_edit + $sign_edit;
      }else{
        $total_edit = $cash_amt + $chq_amt + $sign_amt;
      }
      if ($total_edit != 0) {
        return round($total_edit);
      }
    }
  }
