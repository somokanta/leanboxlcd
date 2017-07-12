<?php

/**
 * @file
 * Definition of example_handler_custom_field
 */

/**
 * Provides a custom views field.
 */
class custom_for_bill_sequence extends views_handler_field {

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
    $party_code = $values->custom_billing_data_party_code;
    $dis_id = $values->custom_billing_data_distributor_id;

    $query = db_select('node', 'n');
    $query->join('field_data_field_party_name', 'n', 'n.entity_id = n.nid');
    $query->fields('n', array('field_party_name_value'));

    $query->condition('n.type', 'outlet_master');
    $query->condition('n.distributor_id', $dis_id);
    $res = $query->execute()->fetchField();
    return $res;
  }

}
