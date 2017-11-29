<?php

/**
 * @file
 * Definition of example_handler_custom_field
 */

/**
 * Provides a custom views field.
 */
class footer_total_custom_field extends views_handler_field {

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
    
    $total_result = $this->view->result;
    foreach ($total_result as $key => $value) {
      $total_pending_for_coll += $value->field_field_field_amnt_difference[0]['raw']['value'];
    }
    if (!empty($total_pending_for_coll)) {
      return "<table border='0' width='100%' cellpadding='6'><tr> <td style='font-weight:bold; text-align:right'>Total Amt Pending For Collection:</td><td style='font-weight:bold; text-align:left; width:140px' width='140'> $total_pending_for_coll</td></tr></table>";
    }
    else {
      return "";
    }
  }
}
