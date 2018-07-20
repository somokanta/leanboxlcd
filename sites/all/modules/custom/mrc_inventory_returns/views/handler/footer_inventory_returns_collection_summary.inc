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
    
//    $query->addExpression('COUNT(CASE WHEN (CONVERT(ad.res_cash_amount,SIGNED) > 0) THEN 1 END)', 'cash_count');
//    $query->addExpression('SUM(ad.res_cash_amount)', 'cash_amt');
//    $query->addExpression('COUNT(CASE WHEN (CONVERT(ad.res_cheque_amount,SIGNED) > 0) THEN 1 END)', 'cheque_count');
//    $query->addExpression('SUM(ad.res_cheque_amount)', 'cheque_amt');
//    $query->addExpression('COUNT(CASE WHEN (CONVERT(ad.res_confirm_signed_bill_amount,SIGNED) > 0) THEN 1 END)', 'signed_bill_count');
//    $query->addExpression('SUM(ad.res_confirm_signed_bill_amount)', 'signed_bill_amt');
    
    $query->fields('ad', array('res_money_collect2'));
    $query->fields('ad', array('res_money_collect2_edit'));
    $query->fields('ad', array('res_cash_amount'));
    $query->fields('ad', array('res_cash_amount_edit'));
    $query->fields('ad', array('res_cheque_amount'));
    $query->fields('ad', array('res_cheque_amount_edit'));
    $query->fields('ad', array('res_confirm_signed_bill_amount'));
    $query->fields('ad', array('res_confirm_signed_bill_amount_edit'));
    $results = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

    $cash_count = 0;
    $cheque_count = 0;
    $signed_count = 0;
    $cash_value = 0;
    $cheque_value = 0;
    $signed_value = 0;
    foreach ($results as &$val) {
//      $row['cash']['name'] = 'Cash';
//      $row['cash']['count'] = $val['cash_count'];
//      $row['cash']['value'] = number_format($val['cash_amt']);
//      $row['cheque']['name'] = 'Cheque';
//      $row['cheque']['count'] = $val['cheque_count'];
//      $row['cheque']['value'] = number_format($val['cheque_amt']);
//      $row['signed']['name'] = 'Signed Bill';
//      $row['signed']['count'] = $val['signed_bill_count'];
//      $row['signed']['value'] = number_format($val['signed_bill_amt']);
      
      // Convert into float because fareye sents "NA" in all three amount fields.
      $val['res_cash_amount_edit'] = (float)$val['res_cash_amount_edit'];
      $val['res_cheque_amount_edit'] = (float)$val['res_cheque_amount_edit'];
      $val['res_confirm_signed_bill_amount_edit'] = (float)$val['res_confirm_signed_bill_amount_edit'];
      
      $val['res_cash_amount'] = (float)$val['res_cash_amount'];
      $val['res_cheque_amount'] = (float)$val['res_cheque_amount'];
      $val['res_confirm_signed_bill_amount'] = (float)$val['res_confirm_signed_bill_amount'];
      
      $money_collect_edit = $val['res_money_collect2_edit'];
      if (!empty($money_collect_edit)) {
        switch ($money_collect_edit) {
          case 'cash':
            $cash_count++;
            $cash_value += $val['res_cash_amount_edit'];
            break;
          case 'cheque':
            $cheque_count++;
            $cheque_value += $val['res_cheque_amount_edit'];
            break;
          case 'signed':
            $signed_count++;
            $signed_value += $val['res_confirm_signed_bill_amount_edit'];
            break;
        }
      }
      else {
        if (!empty($val['res_cash_amount'])) {
          $cash_count++;
          $cash_value += $val['res_cash_amount'];

        }
        if (!empty($val['res_cheque_amount'])) {
            $cheque_count++;
            $cheque_value += $val['res_cheque_amount'];
        }
        if (!empty($val['res_confirm_signed_bill_amount'])) {
            $signed_count++;
            $signed_value += $val['res_confirm_signed_bill_amount'];
        }
      }
    }
    $row[] = array(t('Cash'), $cash_count, number_format($cash_value));
    $row[] = array(t('Cheque'), $cheque_count, number_format($cheque_value));
    $row[] = array(t('Signed Bill'), $signed_count, number_format($signed_value));
    
    $header = array('', 'Count', 'Value');
    $output = theme('table', array('header' => $header, 'rows' => $row, 'attributes' => array('border' => '1', 'cellpadding' => '5', 'cellspacing' => '0')));

    return $output . "<br/>\n";
  }

}
