<?php

/**
 * @file
 * Definition of example_handler_custom_field
 */

/**
 * Provides a custom views field.
 */

class field_sales_returns_report_qty extends views_handler_field {
  
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
    $bill_num = $data->dispatch_planning_api_data_bill_number;
    $sku_code = $data->dispatch_planning_item_details_dispatch_planning_api_data_sk;

    $query = db_select('custom_billing_data', 'cbd');
    $query->condition('cbd.bill_number', $bill_num);
    $query->condition('cbd.sku7_code', $sku_code);
    $query->condition('cbd.distributor_id', $distributor_id);
    $query->addExpression('SUM(cbd.original_Sum_of_TOTAL_SALES_QTY)', 'original_Sum_of_TOTAL_SALES_QTY');
    $query->addExpression('SUM(cbd.Sum_of_TOTAL_SALES_QTY)', 'Sum_of_TOTAL_SALES_QTY');
    $query->addExpression('SUM(cbd.revised_host_quantity)', 'revised_host_quantity');
    $result = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

    $data->original_Sum_of_TOTAL_SALES_QTY2 =  $result[0]['original_Sum_of_TOTAL_SALES_QTY'];
    $data->Sum_of_TOTAL_SALES_QTY2 =  $result[0]['Sum_of_TOTAL_SALES_QTY'];
    $data->revised_host_quantity2 =  $result[0]['revised_host_quantity'];
  }
}


