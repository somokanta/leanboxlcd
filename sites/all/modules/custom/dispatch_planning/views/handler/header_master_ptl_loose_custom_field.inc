<?php

/**
 * @file
 * Definition of example_handler_custom_field
 */

/**
 * Provides a custom views field.
 */
class header_master_ptl_loose_custom_field extends views_handler_field {

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
    foreach ($total_result as $value) {
      $units += $value->master_ptl_data_loose;
      $clds += (int)($value->master_ptl_data_loose/$value->ptl_data_configuration_upc);
      $derived_units += $value->master_ptl_data_derived_loose; 
    }
    $rs_code = $total_result[0]->master_ptl_data_rs_code;

    $name_line_1 = get_rsname_using_rscode($rs_code);
    $name_line_2 = 'Master Trip Party Packing Loose Picking Sheet';

    $trip_id = $this->view->args[0];
    $trip_date = master_ptl_trip_date_by_trip_id($trip_id,$rs_code);
    $trip_dates = date('d-M-y', $trip_date);
    $trip_ids = master_ptl_trip_ids_by_trip_id($trip_id,$rs_code);
    $no_of_bill_lines = master_ptl_no_of_bill_lines_by_trip_id($trip_id,$rs_code);

    if (!empty($no_of_bill_lines)) {
      $this->view->custom_footer = $clds ? $clds : 0;
      return '<table width = "100%" cellspacing = "0" style = "font-size: 13px; color:#000; font-family:arial" cellpadding = "5" border = "0" align = "center" class = "preview_table">
      <tr>
      <td style = "font-size: 24px; font-weight: bold; text-align: center">' . $name_line_1 . '</td>
      </tr>
      <tr>
      <td style = "font-size: 18px; font-weight: bold; text-align: center">' . $name_line_2 . '</td>
      </tr>      
      </table><br>
      <table class = "preview_table" width = "100%" cellspacing = "0" style = "font-family:arial; font-size: 13px; color:#000;" cellpadding = "5" border = "0" align = "center">
      <tbody>
      <tr>
      <td></td>
      <td>Master Trip Id:</td>
      <td align = "left" style="color:#666">' . $trip_id . '</td>
      <td></td>
      <td></td>
      <td>Master Trip Date</td>
      <td align = "left" style="color:#666">' . $trip_dates . '</td>
      <td></td>
      </tr>
      <tr>
      <td></td>
      <td>Trip IDs</td>
      <td align = "left" style="color:#666">' . $trip_ids . '</td>
      <td></td>
      <td></td>
      <td>Total Loose Qty</td>
      <td align = "left" style="color:#666">' . $units . '</td>
      <td></td>
      </tr>
      <tr>
      <td></td>
      <td>Bill Lines</td>
      <td align = "left" style="color:#666">' . $no_of_bill_lines . '</td>
      <td></td>
      <td></td>
      <td>Total Free Qty</td>
      <td align = "left" style="color:#666">' . $derived_units . '</td>
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
