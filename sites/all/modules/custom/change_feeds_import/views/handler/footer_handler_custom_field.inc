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
        $query->join('node', 'n', 'n.title = e.bill_number AND n.uid = e.distributor_id');
        $query->condition('n.type', 'sales_register_data', '=');
        $query->join('field_data_field_party_code_reference', 'pc', 'pc.entity_id = n.nid');
        $query->join('node', 'oun', 'oun.nid = pc.field_party_code_reference_nid');
        $query->join('field_data_field_party_name', 'pn', 'oun.nid = pn.entity_id');
        $query->join('custom_billing_data', 'cbd', 'e.bill_number = cbd.bill_number and e.sku_code = cbd.sku7_code and e.distributor_id = cbd.distributor_id');
        $query->condition('e.distributor_id', $distributor_id, '=');
        $query->condition('e.trip_id', $trip_id);
        $query->fields('e', array('bill_number'));
        $query->fields('pn', array('field_party_name_value'));
        $query->fields('e', array('sku_code'));
        $query->fields('cbd', array('product_name'));
        $query->fields('e', array('excess_qty','reason_code'));
        $rows = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
        
        $header = array('Bill No', 'Party Name', 'Sku', 'Product Name', 'Excess Qty','Reason');
        if(!empty($rows)){
          $title = "<table border='0' width='100%' cellpadding='0'><tr> <td style='font-weight:bold; text-align:center'>Excess Report</td></tr></table>";
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
      return "<table border='0' width='100%' cellpadding='6'><tr> <td style='font-weight:bold; text-align:right'>Total</td><td style='font-weight:bold; text-align:left; width:140px' width='140'> $bill_val</td></tr></table>".$title.$footer_table;
    }
    else {
      return "";
    }
  }
}
