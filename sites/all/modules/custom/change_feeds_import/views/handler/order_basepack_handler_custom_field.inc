<?php

/**
 * @file
 * Definition of example_handler_custom_field
 */

/**
 * Provides a custom views field.
 */
class order_basepack_handler_custom_field extends views_handler_field {

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
  $order_no =   $data->field_field_order_number[0]['raw']['value'];
  $backspace_code =   $data->field_field_basepack_code[0]['raw']['value'];
  return $order_no."-".$backspace_code;
  }

}
