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

  function render($data) {
    $seq = $data->_field_data['nid']['entity']->field_bill_sequence['und'][0]['value'];
    if ($seq == "0" || empty($seq)) {
      return "NA";
    }else{
      return $seq;
    }
  }

}
