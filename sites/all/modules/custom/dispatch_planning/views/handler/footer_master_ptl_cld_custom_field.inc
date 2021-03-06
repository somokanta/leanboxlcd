<?php

/**
 * @file
 * Definition of example_handler_custom_field
 */

/**
 * Provides a custom views field.
 */
class footer_master_ptl_cld_custom_field extends views_handler_field {

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
    $boxs = $this->view->custom_footer;
    if (!empty($boxs)) {
      return '<table border="0" width="100%" cellpadding="6"><tr> <td style="font-weight:bold; text-align:right">Total CLDs  '.$boxs.'</td></tr></table>
        <table class = "preview_table" width = "100%" cellspacing = "0" style = "font-family:arial; font-size: 13px; color:#000;" cellpadding = "5" border = "0" align = "center">
      <tbody>
      <tr>
      <td></td>
      <td>Picker Name:</td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td>CLD Conf Name:</td>
      <td></td>
      <td></td>
      </tr>
      <tr>
      <td></td>
      <td>Start Time:</td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td>Start Time:</td>
      <td></td>
      <td></td>
      </tr>
      <tr>
      <td></td>
      <td>End Time:</td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td>End Time:</td>
      <td></td>
      <td></td>
      </tr>
      <tr>
      <td></td>
      <td>Sign:</td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      </tr>
      </tbody>
      </table><br>';
    }
    else {
      return "";
    }
  }
}
