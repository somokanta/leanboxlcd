<?php

/**
 * @file
 * Definition of example_handler_custom_field
 */

/**
 * Provides a custom views field.
 */
class footer_ptl_custom_box_count extends views_handler_field {

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

  function render($empty = FALSE) {
    $bill_val = $this->view->custom_footer;
    $trip_id = $this->view->args[0];
    $query1 = db_select('ptl_data_configuration', 'ptl');
    $query1->condition('ptl.trip_id', $trip_id, "=");
    $query1->addExpression("COUNT(DISTINCT ptl.box_id)", 'box_id');
    $query1->condition('ptl.box_id', '', "!=");
    $box_id = $query1->execute()->fetchField();
    
    return "<table border='0' width='100%' cellpadding='6'><tr><td style='font-weight:bold; text-align:right'>Total Box Id Count $box_id</td><td style='font-weight:bold; text-align:right;'>Total $bill_val</td></tr></table>";
  }

}
