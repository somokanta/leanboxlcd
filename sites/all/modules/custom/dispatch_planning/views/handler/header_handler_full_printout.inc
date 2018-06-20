<?php

/**
 * @file
 * Definition of example_handler_custom_field
 */

/**
 * Provides a custom views field.
 */
class header_handler_full_printout extends views_handler_field {

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
    $trip_id = $this->view->args[0];
    $result = get_full_return_data_from_trip_id($trip_id);
    $result1 = get_trip_exec_date_from_trip_id($trip_id);

    if (!empty($result)) {
      $bill_count = $result['bill_number'];
      $bill_number = $result['bill_number_2'];
      $cashier_name = $result['fieldExecutiveName'];
      $trip_execution_date = $result1['field_trip_executon_date_value'];
      $name= get_distrubutor_name_full_from_user_id($user_id);;

      return '<table width = "100%" cellspacing = "0" style = "font-size: 13px; color:#000; font-family:arial" cellpadding = "5" border = "0" align = "center" class = "preview_table">
      <tr>
      <td style = "font-size: 24px; font-weight: bold; text-align: center">'.$name.'</td>
      </tr>
      </table><br>
      <table class = "preview_table" width = "100%" cellspacing = "0" style = "font-family:arial; font-size: 13px; color:#000;" cellpadding = "5" border = "0" align = "center">
      <tbody>
      <tr>
      <td></td>
      <td>Bill Number:</td>
      <td colspan="7" align = "left" style="color:#666;">'.$bill_number.'</td>
      <tr>
      <td></td>
      <td>Cashier Name:</td>
      <td align = "left" style="color:#666">'.$cashier_name.'</td>
      <td></td>
      <td>Trip Execution Date:</td>
      <td align = "left" style="color:#666">'.date('d-M-y', $trip_execution_date).'</td>
      <td></td>
      <td>Bill Count:</td>
      <td align = "left" style="color:#666">'.$bill_count.'</td>       
      </tr>
      </tr>
      </tbody>
      </table><br>';
    }
    else {
      return "";
    }
  }

}

function get_full_return_data_from_trip_id($trip_id) {
  $user_id = $GLOBALS['user']->uid;
  $rs_code = dispatch_planning_get_rscode_using_uid($user_id);
  $query1 = db_select('dispatch_planning_api_data', 'ad');
  $query1->condition('ad.runsheet_number', $trip_id, "=");
  $query1->condition('ad.res_status','undeliver', "=");
  $query1->condition('ad.hub',$rs_code, "=");
  $query1->addExpression("COUNT(DISTINCT ad.bill_number)", 'bill_number');
  $query1->addExpression("GROUP_CONCAT(DISTINCT ad.bill_number ORDER BY ad.bill_number separator ', ' )", 'bill_number');
  $query1->fields('ad', array('fieldExecutiveName'));
  $trip_id_result = $query1->execute()->fetchAssoc();
  return $trip_id_result;
}

function get_distrubutor_name_full_from_user_id($user_id) {
  $query = db_select('users', 'u');
  $query->innerjoin('field_data_field_distributor', 'd', 'd.entity_id = u.uid');
  $query->condition('d.entity_id', $user_id, '=');
  $query->fields('d', array('field_distributor_value'));
  $query->innerjoin('field_data_field_name', 'n', 'n.entity_id = d.field_distributor_value');
  $query->fields('n', array('field_name_value'));
  $result = $query->execute()->fetchAssoc();
  $name = $result['field_name_value'];
  return $name;
}