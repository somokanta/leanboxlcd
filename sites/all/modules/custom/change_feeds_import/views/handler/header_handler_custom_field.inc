<?php

/**
 * @file
 * Definition of example_handler_custom_field
 */

/**
 * Provides a custom views field.
 */
class header_handler_custom_field extends views_handler_field {

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
    $trip_id = $this->view->args[0];
    $distributor_id = get_distributorid_of_trip($trip_id);
    $total_result = $this->view->result;
    foreach ($total_result as $key => $value) {
      if($this->view->current_display === 'page_1'){
        $bill_val += $value->field_field_bill_value[0]['raw']['value'];
      }else{
        // for drs2 printout only
        $bill_number = $value->node_title;
        $bill_value = round($value->field_field_bill_value[0]['raw']['value'],2);
        $bills_arr[] = $bill_number;
        $bill_discount = round($value->field_field_bill_level_discount[0]['raw']['value'],2);
        if (!empty($bill_number) && !empty($trip_id)) {
          $query = db_select('dispatch_planning_api_data', 'dpi');
          $query->condition('dpi.distributor_id', $distributor_id, '=');
          $query->condition('dpi.runsheet_number', $trip_id, '=');
          $query->condition('dpi.bill_number', $bill_number, '=');
          $query->join('dispatch_planning_item_details', 'ad', 'ad.trip_bill_reference = dpi.id');
          $query->addField('dpi', 'res_status', 'res_status');
          $query->addExpression('ROUND(SUM(ad.sum_net_sales_value * ad.res_sku_actual_quantity1),2)', 'delivered_value');
          $deliver_amt = $query->execute()->fetchAll();
        }

        if(!empty($deliver_amt[0]->res_status) && $deliver_amt[0]->delivered_value <= $bill_discount){
          $bill_val += round(($bill_value + $bill_discount - $deliver_amt[0]->delivered_value),2);
        }else{
          $bill_val += $bill_value;
        }
      }
    }
    $this->view->custom_footer = $bill_val;
    $name = get_distrubutor_name_from_user_id($distributor_id);
    $result = get_data_from_trip_id($trip_id,$distributor_id);
    $tp_van = get_tp_van_by_trip_id($trip_id,$distributor_id);

    if (!empty($result)) {
      $trip_execution_date = $result['field_trip_executon_date_value'];
      $hht_id = $result['title'];
      $vehicle_no = $result['field_vehicle_no_value'];
      if ($this->view->current_display === 'page_1') {
        $bill_count = $result['field_bill_count_value'];
      }
      else {
        $bills = get_bill_count_nonzero($bills_arr, $distributor_id);
        $bill_count = count($bills);
      }
      $cashier_name = $result['field_cashier_name_value'];
      $seq_method = $result['field_sequence_method_value'];
      
      $total_distance = round($result['field_total_distance_value'],2);
      $total_travel_time = round($result['field_total_travel_time_value'],2);
      if(!empty($total_travel_time)) {
        $hour_minute = explode('.',$total_travel_time);
        $hour = $hour_minute[0].'H';
        $minute = str_pad($hour_minute[1], 2, "0", STR_PAD_RIGHT);
        $minute = round($minute*0.6).'M';
        $total_travel_time = $hour <= 9 ? '0'.$hour.','.$minute : $hour.','.$minute;
      }

      if($seq_method == "0" || empty($seq_method)){
        $sequence = "MANUAL";
      }else{
        $sequence = "API";
      }
      $trip_execution_date = date('d/m/y', $trip_execution_date);
      $sheet_name = 'Delivery Run Sheet';
      if($this->view->current_display === 'page_1'){
        $sheet_name = 'DRS Box ID Print Out';
      }

      return '<table width = "100%" cellspacing = "0" style = "font-size: 13px; color:#000; font-family:arial" cellpadding = "5" border = "0" align = "center" class = "preview_table">
      <tr>
      <td style = "font-size: 24px; font-weight: bold; text-align: center">'.$name.'</td>
      </tr>
      <tr>
      <td style = "text-align: center; font-size: 18px; font-weight: bold;">'.$sheet_name.'</td>
      </tr>
      </table><br>
      <table class = "preview_table" width = "100%" cellspacing = "0" style = "font-family:arial; font-size: 13px; color:#000;" cellpadding = "5" border = "0" align = "center">
      <tbody>
      <tr>
      <td></td>
      <td>Trip Id:</td>
      <td align = "left" style="color:#666">'.$trip_id.'</td>
      <td></td>
      <td></td>
      <td>Bill Count</td>
      <td align = "left" style="color:#666">'.$bill_count.'</td>
      <td></td>
      </tr>
      <tr>
      <td></td>
      <td>Trip Execution Date</td>
      <td align = "left" style="color:#666">'.$trip_execution_date.'</td>
      <td></td>
      <td></td>
      <td>Bill Value</td>
      <td align = "left" style="color:#666">'.$bill_val.'</td>
      <td></td>
      </tr>
      <tr>
      <td></td>
      <td>HHT Id</td>
      <td align = "left" style="color:#666">'.$hht_id.'</td>
      <td></td>
      <td></td>
      <td>Cashier Name</td>
      <td align = "left" style="color:#666">'.$cashier_name.'</td>
      <td></td>
      </tr>
      <tr>
      <td></td>
      <td>Vehicle No</td>
      <td align = "left" style="color:#666">'.$vehicle_no.'</td>
      <td></td>
      <td></td>
      <td>Seq/Kms/Time(Hrs)</td>
      <td align = "left" style="color:#666">'.$sequence.'/'. $total_distance.'/'.$total_travel_time.'</td>
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

function get_data_from_trip_id($trip_id,$distributor_id) {

  $query1 = db_select('node', 'n');
  $query1->condition('n.title', $trip_id, "=");
  $query1->condition('n.type', 'dispatch_data');
  $query1->condition('n.status', 1);
  $query1->condition('n.uid',$distributor_id);
  $query1->join('field_data_field_trip_executon_date', 'trip_exe', 'trip_exe.entity_id = n.nid');
  $query1->join('field_data_field_hht_id', 'hht', 'hht.entity_id = n.nid');
  $query1->join('node', 'dd', 'dd.nid = hht.field_hht_id_nid');
  $query1->leftjoin('field_data_field_vehicle_no', 've', 've.entity_id = n.nid');
  $query1->join('field_data_field_bill_count', 'bill', 'bill.entity_id = n.nid');
  $query1->leftjoin('field_data_field_cashier_name', 'cash', 'cash.entity_id = n.nid');
  $query1->leftjoin('field_data_field_sequence_method', 'sm', 'sm.entity_id = n.nid');
  $query1->leftjoin('field_data_field_total_distance', 'td', 'td.entity_id = n.nid');
  $query1->leftjoin('field_data_field_total_travel_time', 'ttt', 'ttt.entity_id = n.nid');
  $query1->fields('sm', array('	field_sequence_method_value'));
  $query1->fields('td', array('field_total_distance_value'));
  $query1->fields('ttt', array('field_total_travel_time_value'));
  $query1->fields('trip_exe', array('	field_trip_executon_date_value'));
  $query1->fields('hht', array('field_hht_id_nid'));
  $query1->fields('dd', array('title'));
  $query1->fields('ve', array('field_vehicle_no_value'));
  $query1->fields('bill', array('field_bill_count_value'));
  $query1->fields('cash', array('field_cashier_name_value'));
  $trip_id_result = $query1->execute()->fetchAssoc();

  return $trip_id_result;
}

function get_distrubutor_name_from_user_id($user_id) {
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

function get_bill_count_nonzero($bill_number, $distributor_id) {
  $final_bills = array();
  if (is_array($bill_number) && !empty($bill_number)) {
    $query = db_select('custom_billing_data', 'bd');
    $query->condition('bd.distributor_id', $distributor_id);
    $query->condition('bd.bill_number', $bill_number, 'IN');
    $query->addExpression("SUM(bd.Sum_of_TOTAL_SALES_QTY)", 'Sum_of_TOTAL_SALES_QTY');
    $query->havingCondition('Sum_of_TOTAL_SALES_QTY', 0, '>');
    $query->fields('bd', array('bill_number'));
    $query->groupBy('bd.bill_number');
    $final_bills = $query->execute()->fetchCol();
  }
  return $final_bills;
}
