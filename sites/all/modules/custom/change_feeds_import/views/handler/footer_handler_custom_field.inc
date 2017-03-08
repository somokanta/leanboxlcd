<?php

/**
 * @file
 * Definition of example_handler_custom_field
 */

/**
 * Provides a custom views field.
 */
class footer_handler_custom_field extends views_handler_field {

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
    if (!empty($bill_val)) {
      return "<table border='0' width='100%' cellpadding='6'><tr> <td style='font-weight:bold; text-align:right'>Total</td><td style='font-weight:bold; text-align:left; width:140px' width='140'> $bill_val</td></tr></table>";
    }
    else {
      return "";
    }
  }

}
