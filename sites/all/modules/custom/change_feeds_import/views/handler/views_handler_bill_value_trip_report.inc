<?php

/**
 * @file
 * Definition of example_handler_custom_field
 */

/**
 * Provides a custom views field.
 */
class views_handler_bill_value_trip_report extends views_handler_field {

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
    dpm($van, "van");
    $trip_id = $data->node_title;
    dpm($trip_id, "trip_id");
    $loading_date = $data->field_field_billing_loading_date[0]['raw']['value'];
    dpm($loading_date, "loading_date");

    return 1;
  }

}
