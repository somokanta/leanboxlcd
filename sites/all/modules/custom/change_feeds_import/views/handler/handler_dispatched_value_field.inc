<?php

/**
 * @file
 * Definition of example_handler_custom_field
 */

/**
 * Provides a custom views field.
 */
class handler_dispatched_value_field extends views_handler_field {

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
    $trip_id = $this->view->args[0];
    $bill_number = $data->node_title;

    $query = db_select('custom_billing_data', 'cbd');
    $query->condition('cbd.bill_number', $bill_number, '=');
    $query->condition('cbd.distributor_id', $distributor_id, '=');
    $query->addExpression("SUM(cbd.Sum_of_TOTAL_SALES_QTY * cbd.line_level_value)", 'dispatched_value');
    $result = $query->execute()->fetchField();
    return $result;
  }

}

