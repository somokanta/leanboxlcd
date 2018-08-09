<?php

/**
 * @file
 * Definition of views_handler_field_store_count.
 */

/**
 * Returns store count
 * @ingroup views_field_handlers
 */
class views_handler_field_sequence extends views_handler_field {

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
    $trip_id = $this->view->args[0];
    $sku_code = $data->ptl_data_configuration_sku_code;
    $rs_code = $data->ptl_data_configuration_rs_code;
    $query1 = db_select('ptl_data_configuration', 'p');
    $query1->condition('p.trip_id', $trip_id, "=");
    $query1->condition('p.sku_code', $sku_code, "=");
    $query1->condition('p.rs_code', $rs_code, "=");
    $query1->addExpression("GROUP_CONCAT(CONCAT(p.sequence_no, '-', p.loose) ORDER BY p.sequence_no SEPARATOR ', ')", "sequence_quantity");
    $output = $query1->execute()->fetchField();
    return $output;
  }
}
