<?php

/**
 * @file
 * Definition of example_handler_custom_field
 */

/**
 * Provides a custom views field.
 */
class footer_inventory_returns_bills_summary extends views_handler_field {

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

  function render($empty = FALSE) {
    $trip_number = $this->view->args[0];

    $user_id = $GLOBALS['user']->uid;
    $distributor_id = change_feeds_import_get_distributor_id($user_id);

    $query = db_select('dispatch_planning_api_data', 'ad');
    $query->condition('ad.distributor_id', $distributor_id, '=');
    $query->condition('ad.runsheet_number', $trip_number, '=');
    $query->join('dispatch_planning_item_details', 'dpi', 'dpi.trip_bill_reference = ad.id');
    $query->addExpression('COUNT(DISTINCT(CASE WHEN (ad.res_status = \'partial_deliver\') THEN ad.bill_number END))', 'bill_count_partial_deliver');
    $query->addExpression('COUNT(CASE WHEN (ad.res_status = \'partial_deliver\' AND (dpi.line_level_status = \'line_partial_deliver\'  or dpi.line_level_status = \'line_undeliver\')) THEN dpi.sku_code END)', 'bill_lines_count_partial_deliver');
    $query->addExpression('COUNT(DISTINCT(CASE WHEN (ad.res_status = \'partial_deliver\' AND (dpi.line_level_status = \'line_partial_deliver\'  or dpi.line_level_status = \'line_undeliver\')) THEN dpi.sku_code END))', 'bill_lines_count_unique_partial_deliver');
    $query->addExpression('SUM(CASE WHEN (ad.res_status = \'partial_deliver\' AND (dpi.line_level_status = \'line_partial_deliver\'  or dpi.line_level_status = \'line_undeliver\')) THEN dpi.return_qty END)', 'return_qty_partial_deliver');
    $results = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

    $query = db_select('dispatch_planning_api_data', 'ad');
    $query->condition('ad.distributor_id', $distributor_id, '=');
    $query->condition('ad.runsheet_number', $trip_number, '=');
    $query->join('dispatch_planning_item_details', 'dpi', 'dpi.trip_bill_reference = ad.id');
    $query->addExpression('COUNT(DISTINCT(CASE WHEN (ad.res_status = \'undeliver\') THEN ad.bill_number END))', 'bill_count_undeliver');
    $query->addExpression('COUNT(CASE WHEN (ad.res_status = \'undeliver\' AND (dpi.line_level_status = \'line_partial_deliver\'  or dpi.line_level_status = \'line_undeliver\')) THEN dpi.sku_code END)', 'bill_lines_count_undeliver');
    $query->addExpression('COUNT(DISTINCT(CASE WHEN (ad.res_status = \'undeliver\' AND (dpi.line_level_status = \'line_partial_deliver\'  or dpi.line_level_status = \'line_undeliver\')) THEN dpi.sku_code END))', 'bill_lines_count_unique_undeliver');
    $query->addExpression('SUM(CASE WHEN (ad.res_status = \'undeliver\' AND (dpi.line_level_status = \'line_partial_deliver\'  or dpi.line_level_status = \'line_undeliver\')) THEN dpi.return_qty END)', 'return_qty_undeliver');
    $results1 = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

    $query = db_select('dispatch_planning_api_data', 'ad');
    $query->condition('ad.distributor_id', $distributor_id, '=');
    $query->condition('ad.runsheet_number', $trip_number, '=');
    $query->join('dispatch_planning_item_details', 'dpi', 'dpi.trip_bill_reference = ad.id');
    $query->addExpression('COUNT(DISTINCT(CASE WHEN (dpi.res_cashier_short >= 1) THEN ad.bill_number END))', 'bill_count_cashier_short');
    $query->addExpression('COUNT(CASE WHEN (dpi.res_cashier_short >= 1) THEN dpi.sku_code END)', 'bill_lines_count_cashier_short');
    $query->addExpression('COUNT(DISTINCT(CASE WHEN (dpi.res_cashier_short >= 1) THEN dpi.sku_code END))', 'bill_lines_count_unique_cashier_short');
    $query->addExpression('SUM(CASE WHEN (dpi.res_cashier_short >= 1) THEN dpi.res_cashier_short END)', 'return_qty_cashier_short');
    $results2 = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

    $query = db_select('dispatch_planning_api_data', 'ad');
    $query->condition('ad.distributor_id', $distributor_id, '=');
    $query->condition('ad.runsheet_number', $trip_number, '=');
    $query->join('dispatch_planning_item_details', 'dpi', 'dpi.trip_bill_reference = ad.id');
    $query->addExpression('COUNT(DISTINCT(CASE WHEN (dpi.res_godown_short >= 1) THEN ad.bill_number END))', 'bill_count_godown_short');
    $query->addExpression('COUNT(CASE WHEN (dpi.res_godown_short >= 1) THEN dpi.sku_code END)', 'bill_lines_count_godown_short');
    $query->addExpression('COUNT(DISTINCT(CASE WHEN (dpi.res_godown_short >= 1) THEN dpi.sku_code END))', 'bill_lines_count_unique_godown_short');
    $query->addExpression('SUM(CASE WHEN (dpi.res_godown_short >= 1) THEN dpi.res_godown_short END)', 'return_qty_godown_short');
    $results3 = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

    $rows[] = array("Partial Delivered") + $results[0];
    $rows[] = array('Full Returned') + $results1[0];
    $rows[] = array('Cashier Short') + $results2[0];
    $rows[] = array('Godown Short') + $results3[0];

    $header = array('Status', 'Bill Count', 'Bill Lines Count', 'Product Count', 'Returned/Short Product Qty');
    $output = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('border' => '1', 'cellpadding' => '5', 'cellspacing' => '0')));

    return $output."<br/>\n";
  }
}
