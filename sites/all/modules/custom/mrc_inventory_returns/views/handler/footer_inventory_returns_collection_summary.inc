<?php

/**
 * @file
 * Definition of example_handler_custom_field
 */

/**
 * Provides a custom views field.
 */
class footer_inventory_returns_collection_summary extends views_handler_field {

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
    $query->addExpression('COUNT(CASE WHEN (CONVERT(ad.res_cash_amount,SIGNED) > 0) THEN 1 END)', 'cash_count');
    $query->addExpression('SUM(ad.res_cash_amount)', 'cash_amt');
    $query->addExpression('COUNT(CASE WHEN (CONVERT(ad.res_cheque_amount,SIGNED) > 0) THEN 1 END)', 'cheque_count');
    $query->addExpression('SUM(ad.res_cheque_amount)', 'cheque_amt');
    $query->addExpression('COUNT(CASE WHEN (CONVERT(ad.res_confirm_signed_bill_amount,SIGNED) > 0) THEN 1 END)', 'signed_bill_count');
    $query->addExpression('SUM(ad.res_confirm_signed_bill_amount)', 'signed_bill_amt');
    $results = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

    foreach ($results as $val) {
      $row['cash']['name'] = 'Cash';
      $row['cash']['count'] = $val['cash_count'];
      $row['cash']['value'] = number_format($val['cash_amt']);
      $row['cheque']['name'] = 'Cheque';
      $row['cheque']['count'] = $val['cheque_count'];
      $row['cheque']['value'] = number_format($val['cheque_amt']);
      $row['signed']['name'] = 'Signed Bill';
      $row['signed']['count'] = $val['signed_bill_count'];
      $row['signed']['value'] = number_format($val['signed_bill_amt']);
    }
    $header = array('', 'Count', 'Value');
    $output = theme('table', array('header' => $header, 'rows' => $row, 'attributes' => array('border' => '1', 'cellpadding' => '5', 'cellspacing' => '0')));
    
    return $output."<br/>\n";
  }
}
