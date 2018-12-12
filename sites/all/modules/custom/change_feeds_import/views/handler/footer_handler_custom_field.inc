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
      $excess_table = $footer_table = '';
      if($this->view->current_display == 'drs_report2'){
        global $user;
        $user_id = $user->uid;
        $distributor_id = change_feeds_import_get_distributor_id($user_id);
        $trip_id = $this->view->args[0];
        $query = db_select('excess_qty', 'e');
        $query->condition('e.distributor_id', $distributor_id, '=');
        $query->condition('e.trip_id', $trip_id);
        $query->fields('e', array('bill_number','sku_code','excess_qty','reason_code'));
        $rows = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

        $header = array('Bill No', 'Sku', 'Excess Qty','Reason');
        if(!empty($rows)){
          $excess_table = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('width' => "100%", 'cellspacing' => "0", 'style' => "font-size: 13px; color:#000; font-family:arial", 'cellpadding' => "5", 'border' => "1", 'align' => "center", 'class' => "preview_table")));
        }
        $drs_table = drs_invoice_vs_actual_dispatched_report_table($trip_id, $distributor_id);
        //$manual_vrs_table = manual_vrs_report_table($trip_id, $distributor_id);
        if(!empty($excess_table)){
          $footer_table = $excess_table;
        }       
        // Page Break required in DRS Table.
        if(!empty($drs_table)){
          $footer_table = $footer_table. "<pagebreak />".$drs_table;
        }
        // Page Break required in Manual VRS Table.
//        if(!empty($manual_vrs_table)){
//          $footer_table = $footer_table."<pagebreak />".$manual_vrs_table;
//        }
      }
      return "<table border='0' width='100%' cellpadding='6'><tr> <td style='font-weight:bold; text-align:right'>Total</td><td style='font-weight:bold; text-align:left; width:140px' width='140'> $bill_val</td></tr></table>".$footer_table;
    }
    else {
      return "";
    }
  }
}
