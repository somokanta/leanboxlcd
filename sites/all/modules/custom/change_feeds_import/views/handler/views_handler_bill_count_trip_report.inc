<?php

/**
 * @file
 * Definition of example_handler_custom_field
 */

/**
 * Provides a custom views field.
 */
class views_handler_bill_count_trip_report extends views_handler_field {

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
    $van = $data->field_field_billing_van[0]['raw']['value'];
    $trip_id = $data->node_title;
    $loading_date = $data->field_field_billing_loading_date[0]['raw']['value'];
    $query1 = db_select('node', 'n');
    $query1->condition('n.type', 'sales_register_data');
    $query1->join('field_data_field_billing_van', 'bv', 'bv.entity_id = n.nid');
    $query1->condition('bv.field_billing_van_value', $van);
    $query1->join('field_data_field_billing_loading_date', 'bld', 'bld.entity_id = n.nid');
    $query1->condition('bld.field_billing_loading_date_value', $loading_date);
    $query1->join('field_data_field_trip_id_reference', 'tr', 'tr.entity_id = n.nid');
    $query1->leftjoin('node', 'disn', 'disn.nid = tr.field_trip_id_reference_nid');
    $query1->condition('disn.title', $trip_id);
    $query1->groupBy('tr.field_trip_id_reference_nid');
    $query1->addExpression('COUNT(n.title)', 'bill_count');
    $count = $query1->execute()->fetchField();
    return $count;
  }

}
