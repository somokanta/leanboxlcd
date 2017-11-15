<?php

/**
 * @file
 * Definition of example_handler_custom_field
 */

/**
 * Provides a custom views field.
 */
class header_ptl_loose_custom_field extends views_handler_field {

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
    global $user;
    $user_id = $user->uid;
    $total_result = $this->view->result;
    //dpm($total_result);
    foreach ($total_result as $key => $value) {
      $units += $value->ptl_data_configuration_loose;
    }

    $this->view->custom_footer = $units;

    $name_line_1 = 'LeanBox Logistics Solution Pvt Ltd';
    $name_line_2 = 'Loose PICKING SHEET';

    $trip_id = $this->view->args[0];
    $trip_date = ptl_trip_date_by_trip_id($trip_id);
    $trip_date = date('d-M-y', $trip_date);
    $loading_sheet = ptl_loading_sheet_by_trip_id($trip_id);
    $no_of_bills = ptl_no_of_bills_by_trip_id($trip_id);


    if (!empty($no_of_bills)) {

      return '<table width = "100%" cellspacing = "0" style = "font-size: 13px; color:#000; font-family:arial" cellpadding = "5" border = "0" align = "center" class = "preview_table">
      <tr>
      <td style = "font-size: 24px; font-weight: bold; text-align: center">' . $name_line_1 . '</td>
      </tr>
      <tr>
      <td style = "font-size: 24px; font-weight: bold; text-align: center">' . $name_line_2 . '</td>
      </tr>      
      </table><br>
      <table class = "preview_table" width = "100%" cellspacing = "0" style = "font-family:arial; font-size: 13px; color:#000;" cellpadding = "5" border = "0" align = "center">
      <tbody>
      <tr>
      <td></td>
      <td>Trip Id:</td>
      <td align = "left" style="color:#666">' . $trip_id . '</td>
      <td></td>
      <td></td>
      <td>Trip Date</td>
      <td align = "left" style="color:#666">' . $trip_date . '</td>
      <td></td>
      </tr>
      <tr>
      <td></td>
      <td>Loading Sheet</td>
      <td align = "left" style="color:#666">' . $loading_sheet . '</td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      </tr>
      <tr>
      <td></td>
      <td>No. of Bills</td>
      <td align = "left" style="color:#666">' . $no_of_bills . '</td>
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
