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
    $total_result = $this->view->result;
    foreach ($total_result as $value) {
      $units += $value->ptl_data_configuration_loose;
      $clds += (int)($value->ptl_data_configuration_loose/$value->ptl_data_configuration_upc);
      $derived_units += $value->ptl_data_configuration_derived_loose; 
    }
    $rs_code = $total_result[0]->ptl_data_configuration_rs_code;

    $name_line_1 = get_rsname_using_rscode($rs_code);
    $name_line_2 = 'Party Packing Loose Picking Sheet';
    if ($this->view->name === 'ptl_loose_sheet' && $this->view->current_display === 'page_1') {
        $name_line_2 = "Bill-wise Loose Picking Sheet";
    }

    $trip_id = $this->view->args[0];
    $trip_date = ptl_trip_date_by_trip_id($trip_id,$rs_code);
    $trip_date = date('d-M-y', $trip_date);
    $loading_sheet = ptl_loading_sheet_by_trip_id($trip_id,$rs_code);
    $no_of_bills = ptl_no_of_bills_by_trip_id($trip_id,$rs_code);
    $no_of_bill_lines = ptl_no_of_bill_lines_by_trip_id($trip_id,$rs_code);
    $tp_van = get_tp_van_by_trip_id($trip_id);

    if (!empty($no_of_bills)) {
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
      <td>Total Loose Qty</td>
      <td align = "left" style="color:#666">' . $units . '</td>
      <td></td>
      </tr>
      <tr>
      <td></td>
      <td>No. of Bills</td>
      <td align = "left" style="color:#666">' . $no_of_bills . '</td>
      <td></td>
      <td></td>
      <td>Total Free Qty</td>
      <td align = "left" style="color:#666">' . $derived_units . '</td>
      <td></td>
      </tr>
      <tr>
      <td></td>
      <td>Bill Lines</td>
      <td align = "left" style="color:#666">' . $no_of_bill_lines . '</td>
      <td></td>
      <td></td>
      <td>TP Van</td>
      <td align = "left" style="color:#666">' . $tp_van . '</td>
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
