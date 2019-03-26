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
      $dp_bills_table = $footer_table = '';
      if($this->view->current_display == 'drs_report2'){
        global $user;
        $user_id = $user->uid;
        $distributor_id = change_feeds_import_get_distributor_id($user_id);
        $trip_id = $this->view->args[0];
        
        $sub_query1 = db_select('dispatch_planning_item_details', 'dpi');
        $sub_query1->fields('dpi', array('trip_bill_reference'));
        $sub_query1->addExpression('(SUM(dpi.sum_net_sales_value * dpi.res_sku_actual_quantity1))', 'delivered_value');
        $sub_query1->join('dispatch_planning_api_data', 'dp_header', 'dpi.trip_bill_reference = dp_header.id AND dp_header.distributor_id ='.$distributor_id);    
        $sub_query1->condition('dp_header.runsheet_number', $trip_id, '=');
        $sub_query1->groupBy('dpi.trip_bill_reference');
        
        $query = db_select('dispatch_planning_api_data', 'ad');
        $query->condition('ad.runsheet_number', $trip_id, '=');
        $query->condition('ad.distributor_id', $distributor_id, '=');
        $query->condition('ad.delivery_pending_flag', 1, '=');
        $query->join($sub_query1, 'item_details', 'item_details.trip_bill_reference = ad.id');
        $query->join('node', 'n', 'n.title = ad.bill_number AND n.uid = ad.distributor_id');
        $query->condition('n.type', 'sales_register_data', '=');
        $query->leftjoin('field_data_field_bill_sequence', 'fbs', 'fbs.entity_id = n.nid');
        $query->join('field_data_field_bill_level_discount', 'bld', 'bld.entity_id = n.nid');
        $query->join('field_data_field_pod_flag', 'pod', 'pod.entity_id = n.nid');
        $query->join('custom_billing_data', 'cbd', 'cbd.bill_number = ad.bill_number AND cbd.distributor_id = ad.distributor_id');
        $query->join('node', 'n1', 'n1.title = ad.party_hll_code  AND n1.uid = ad.distributor_id');
        $query->condition('n1.type', 'hul_code_master');
        $query->join('field_data_field_updated_address', 'fua', 'n1.nid = fua.entity_id');
        $query->join('node', 'n2', 'n2.title = ad.party_code AND n2.uid = ad.distributor_id');
        $query->condition('n2.type', 'outlet_master');
        $query->leftjoin('field_data_field_payment_terms', 'pt', 'n2.nid = pt.entity_id');
        $query->leftjoin('field_data_field_credit_days', 'cd', 'n2.nid = cd.entity_id');
        $query->fields('fbs', array('field_bill_sequence_value'));
        $query->addExpression("ROUND(SUM(cbd.Sum_of_TOTAL_SALES_QTY * cbd.line_level_value),2)", 'dispatched_value'); 
        $query->addExpression("CONCAT(fua.field_updated_address_locality, ',', fua.field_updated_address_dependent_locality,',', fua.field_updated_address_sub_premise)", 'area');
        $query->addExpression("concat(cbd.vehicle ,' , ', DATE_FORMAT(CONVERT_TZ(FROM_UNIXTIME(cbd.loading_date),'+00:00','+05:30'), '%d/%m/%y'))", 'van_date');
        $query->fields('ad', array('beat_name', 'bill_number', 'party_name', 'bill_value'));
        $query->fields('bld', array('field_bill_level_discount_value'));
        $query->fields('pt', array('field_payment_terms_value'));
        $query->fields('cd', array('field_credit_days_value'));
        $query->addExpression("CONCAT(pt.field_payment_terms_value ,'-', cd.field_credit_days_value)", 'pt_ct');
        $query->addField('ad', 'res_status', 'res_status');
        $query->addExpression("ROUND((item_details.delivered_value),2)", 'delivered_value');
        $query->addExpression("(CASE WHEN (ad.res_status IS NOT NULL AND item_details.delivered_value <= bld.field_bill_level_discount_value) THEN CONCAT(ad.bill_value,' Collect:',(ROUND((ad.bill_value + bld.field_bill_level_discount_value - item_details.delivered_value),2))) ELSE ad.bill_value END)", 'net_amt');
        $query->fields('pod', array('field_pod_flag_value'));
        $query->fields('bld', array('field_bill_level_discount_value'));
        $query->groupBy('ad.bill_number');
        $query->orderBY('fbs.field_bill_sequence_value');
        $result = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
        $row = array();
        $total = array();
        $sr_no = 0;
        if (!empty($result)) {

          foreach ($result as $value) {
            $sr_no++;
            $row['sr_no'] = $sr_no;
            $total['sr_no'] = '';
            $row['field_bill_sequence_value'] = $value['field_bill_sequence_value'];
            $total['field_bill_sequence_value'] = '';
            $row['dispatched_value'] = $value['dispatched_value'];
            $total['dispatched_value'] = '';
            $row['area'] = $value['area'];
            $total['area'] = '';
            $row['van_date'] = $value['van_date'];
            $total['van_date'] = '';
            $row['beat_name'] = $value['beat_name'];
            $total['beat_name'] = '';
            $row['bill_number'] = $value['bill_number'];
            $total['bill_number'] = '';
            $row['party_name'] = $value['party_name'];
            $total['party_name'] = '';
            $row['field_bill_level_discount_value'] = $value['field_bill_level_discount_value'];
            $total['field_bill_level_discount_value'] = '<b>Total</b>';
            $row['net_amt'] = "<b>".$value['net_amt']. "</b>"; 
            $additon_amt = $value['net_amt'];
            if(strpos($value['net_amt'], 'Collect:')){
              $net_amt_arr = explode("Collect:", $value['net_amt']);
              $additon_amt = $net_amt_arr[1];
            }
            $total['net_amt'] += $additon_amt; 
            if(!empty($value['field_payment_terms_value']) && !empty($value['field_credit_days_value'])) {
              $row['pt_ct'] = $value['pt_ct'];
            }
            else if(empty($value['field_payment_terms_value']) && !empty($value['field_credit_days_value'])){
              $row['pt_ct'] = $value['field_credit_days_value'];
            }
            else if(!empty($value['field_payment_terms_value']) && empty($value['field_credit_days_value'])){
             $row['pt_ct'] = $value['field_payment_terms_value']; 
            }
            else {
              $row['pt_ct'] = '-';
            }
            $total['pt_ct'] = '';
            $row['payment'] = '';
            $total['payment'] = '';
            if($value['field_pod_flag_value'] == 1) {
              $row['field_pod_flag_value'] = 'POD';
            }
            else{
              $row['field_pod_flag_value'] = '';
            }
            $total['field_pod_flag_value'] = '';
            $rows[] = $row;

          }
          $rows[] = $total;
          if(!empty($rows)){
            $title = "<table border='0' width='100%' cellpadding='0'><tr> <td style='font-weight:bold; text-align:center'>DP Bills</td></tr></table>";
            $header = array('Sr. No.', 'Seq', 'Dispatched Value', 'Area', 'VAN Date', 'Sales Beat', 'Bill No.', 'PARTY', 'Bill Level Discount', 'Net Amt.', 'PT-CT', 'Payment', 'POD');
            $dp_bills_table = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('width' => "100%", 'cellspacing' => "0", 'style' => "font-size: 13px; color:#000; font-family:arial", 'cellpadding' => "5", 'border' => "1", 'align' => "center", 'class' => "preview_table")));
          }
        }
        $excess_table = drs_excess_table($trip_id, $distributor_id);
        $drs_table = drs_invoice_vs_actual_dispatched_report_table($trip_id, $distributor_id);
        //$manual_vrs_table = manual_vrs_report_table($trip_id, $distributor_id);
        if(!empty($dp_bills_table)){
          $footer_table = $dp_bills_table;
        } 
        // Page Break required in Excess Table.
        if(!empty($excess_table)){
          $footer_table = $footer_table. "<pagebreak />". $excess_table;
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
