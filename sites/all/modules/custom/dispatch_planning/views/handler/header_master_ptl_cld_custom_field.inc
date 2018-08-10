<?php

/**
 * @file
 * Definition of example_handler_custom_field
 */

/**
 * Provides a custom views field.
 */
class header_master_ptl_cld_custom_field extends views_handler_field {

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
    $box = 0;
    foreach ($total_result as $key => $value) {
      $box += $value->master_ptl_data_cld;
      $derived_box += $value->master_ptl_data_derived_cld;
    }
    $rs_code = $total_result[0]->master_ptl_data_rs_code;
    $this->view->custom_footer = $box;
    
    $name_line_1 = get_rsname_using_rscode($rs_code);
    $name_line_2 = 'Master Trip Party Packing CLD Picking Sheet';

    $trip_id = $this->view->args[0];
    $trip_date = master_ptl_trip_date_by_trip_id($trip_id,$rs_code);
    $trip_dates = date('d-M-y', $trip_date);
    
    //$trip_ids = master_ptl_trip_ids_by_trip_id($trip_id,$rs_code);
    $actual_trip_id = $this->view->args[1];
    $distributor_id = change_feeds_import_get_distributor_id($user_id);
    
    $query = db_select('node', 'n');
    $query->addField('n', 'nid');
    $query->condition('n.title', $actual_trip_id);
    $query->condition('n.uid', $distributor_id);
    $query->condition('n.type', 'dispatch_data');
    $trip_id_nid = $query->execute()->fetchField();
    $van_date = _get_van_loading_combination_by_trip($trip_id_nid);
    $tp_van = get_tp_van_by_trip_id($actual_trip_id);
    if (!empty($trip_date)) {

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
      <td>Van-Date</td>
      <td align = "left" style="color:#666">' . $van_date . '</td>
      <td></td>
      <td></td>
      <td>Total Free CLDs</td>
      <td align = "left" style="color:#666">' . $derived_box . '</td>
      <td></td>
      </tr>
      <tr>
      <td></td>
      <td>TP Van</td>
      <td align = "left" style="color:#666">'.$tp_van.'</td>
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


