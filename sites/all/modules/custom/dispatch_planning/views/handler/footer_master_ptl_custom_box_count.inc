<?php

/**
 * @file
 * Definition of example_handler_custom_field
 */

/**
 * Provides a custom views field.
 */
class footer_master_ptl_custom_box_count extends views_handler_field {

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
    $ptl_job = $this->view->args[0];
    $query1 = db_select('master_ptl_data', 'mptl');
    $query1->condition('mptl.ptl_job', $ptl_job, "=");
    $query1->addExpression("COUNT(DISTINCT mptl.box_id)", 'box_id');
    $query1->condition('mptl.box_id', '', "!=");
    $box_id = $query1->execute()->fetchField();
    return "<table border='0' width='100%' cellpadding='6'><tr> <td style='font-weight:bold; text-align:center;'>Total Box ID Count: $box_id</td></tr></table>";
  }

}
