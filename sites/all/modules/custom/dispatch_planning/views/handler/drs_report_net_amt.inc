<?php

/**
 * @file
 * Definition of views_handler_field_bill_value.
 */

/**
 * Returns crate count
 * @ingroup views_field_handlers
 */
class drs_report_net_amt extends views_handler_field {

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
    $distributor_id = change_feeds_import_get_distributor_id($user->uid);
    $bill_number = $data->node_title;
    $trip_id = $this->view->args[0];
    $bill_value = round($data->field_field_bill_value[0]['raw']['value'],2);
    $bill_discount = round($data->field_field_bill_level_discount[0]['raw']['value'],2);
    if (!empty($bill_number) && !empty($trip_id)) {
      $query = db_select('dispatch_planning_api_data', 'dpi');
      $query->condition('dpi.distributor_id', $distributor_id, '=');
      $query->condition('dpi.runsheet_number', $trip_id, '=');
      $query->condition('dpi.bill_number', $bill_number, '=');
      $query->join('dispatch_planning_item_details', 'ad', 'ad.trip_bill_reference = dpi.id');
      $query->addField('dpi', 'res_status', 'res_status');
      $query->addExpression('ROUND(SUM(ad.sum_net_sales_value * ad.res_sku_actual_quantity1),2)', 'delivered_value');
      $deliver_amt = $query->execute()->fetchAll();
    }

    if(!empty($deliver_amt[0]->res_status) && $deliver_amt[0]->delivered_value <= $bill_discount){
      $bill_net_amt = round(($bill_value + $bill_discount - $deliver_amt[0]->delivered_value),2);
      $return = "$bill_value Collect:$bill_net_amt";
    }else{
      $return = $bill_value;
    }
    return $return;
  }
}
