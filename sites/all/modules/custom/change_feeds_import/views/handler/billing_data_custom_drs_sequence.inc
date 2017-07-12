<?php

/**
 * @file
 * Definition of example_handler_custom_field
 */

/**
 * Provides a custom views field.
 */
class billing_data_custom_drs_sequence extends views_handler_field {

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

  function render($values) {
    $bill_number = $values->custom_billing_data_bill_number;
    $dis_id = $values->custom_billing_data_distributor_id;

    $query = db_select('node', 'n');
    $query->join('field_data_field_drs_sequence', 'h', 'h.entity_id = n.nid');
    $query->fields('h', array('field_drs_sequence_value'));

    $query->condition('n.type', 'sales_register_data');
    $query->condition('n.uid', $dis_id);
    $query->condition('n.title', $bill_number);
    $res = $query->execute()->fetchField();
    return $res;
  }

}
