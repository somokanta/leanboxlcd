<?php

/**
 * @file
 * Definition of example_handler_custom_field
 */

/**
 * Provides a custom views field.
 */
class custom_for_picking_type extends views_handler_field {

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
    if (is_numeric($data->field_field_combined_nid[0]['raw']['value'])) {
      return $data->_field_data['nid']['entity']->field_picking_picking_type['und'][0]['value'];
    }
    else {
      if ($data->field_field_combined_nid[0]['raw']['value'] == null) {
        return $data->_field_data['nid']['entity']->field_picking_picking_type['und'][0]['value'];
      }
      else {
        return "combined";
      }
    }
  }

}
