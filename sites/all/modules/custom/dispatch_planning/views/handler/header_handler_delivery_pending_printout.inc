<?php

/**
 * @file
 * Definition of example_handler_custom_field
 */

/**
 * Provides a custom views field.
 */
class header_handler_delivery_pending_printout extends views_handler_field {

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
        $view_key = key($this->view->result);
        $trip_id = $this->view->args[0];
        $result = get_delivery_pending_data($trip_id);
        $result1 = get_delivery_pending_header_data($trip_id);
        $dispatch_date = $result1['date'];
        $cashier_name = $result1['fieldExecutiveName'];
        if (!empty($dispatch_date)) {
            if (is_numeric($dispatch_date)) {
                $dispatch_date = date('d-M-Y', $dispatch_date);
            } else {
                $dispatch_date = strtotime($dispatch_date);
                $dispatch_date = date('d-M-Y', $dispatch_date);
            }
        }
        if (!empty($result)) {
            $bill_count = $result['bill_count'];
            $bill_number = $result['bill_number'];
            $van_type = get_van_type($trip_id);
            $name= get_distrubutor_name_full_from_user_id($user_id);;
            return '
           <table width = "100%" cellspacing = "0" style = "font-size: 13px; color:#000; font-family:arial" cellpadding = "5" border = "0" align = "center" class = "preview_table">
      <tr>
      <td style = "font-size: 24px; font-weight: bold; text-align: center">' . $name . '</td>
      </tr>
      </table><br>
      <table class = "preview_table" width = "100%" cellspacing = "0" style = "font-family:arial; font-size: 13px; color:#000;" cellpadding = "5" border = "0" align = "center">
      <tbody>
      <tr>
      <td></td>
      <td>Trip ID:</td>
      <td colspan="5" align = "left" style="color:#666;">' . $trip_id . '</td>
      <tr>
      <tr>
      <td></td>
      <td>Bill Number:</td>
      <td colspan="5" align = "left" style="color:#666;">' . $bill_number . '</td>
      <tr>
      
      <tr>
      <td></td>
      <td>Cashier Name:</td>
      <td colspan="5" align = "left" style="color:#666;">' . $cashier_name . '</td>
      <tr>
      <tr>
      <td></td>
      <td>Van Type:</td>
      <td colspan="5" align = "left" style="color:#666;">' . $van_type . '</td>
      <tr>
      <td></td>
      <td>Dispatch Date:</td>
      <td align = "left" style="color:#666">' . $dispatch_date . '</td>
      <td></td>
      <td>Bill Count:</td>
      <td align = "left" style="color:#666">' . $bill_count . '</td>
      <td></td>      
      </tr>
      </tbody>
      </table><br>';
        } else {
            return "";
        }
    }

}
