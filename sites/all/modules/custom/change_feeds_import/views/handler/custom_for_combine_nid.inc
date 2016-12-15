<?php

/**
 * @file
 * Definition of example_handler_custom_field
 */

/**
 * Provides a custom views field.
 */
class custom_for_combine_nid extends views_handler_field {

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
    if (is_null($data->_field_data['nid']['entity']->field_combined_nid['und'][0]['value'])) {
      return ($data->nid);
    }
    else {
      return ($data->_field_data['nid']['entity']->field_combined_nid['und'][0]['value']);
    }
  }

}
