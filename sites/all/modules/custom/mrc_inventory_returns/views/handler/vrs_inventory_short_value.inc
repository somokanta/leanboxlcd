<?php
module_load_include('inc', 'mrc_inventory_returns', 'includes/vrs_common');
/**
 * @file
 * Definition of vrs_inventory_short_value.
 */

class vrs_inventory_short_value extends views_handler_field{
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
  
  function render(&$data){
    $views_current_field = $this->field;
    $trip_number = $data->vrs_trip_total_trip_id; //$this->view->args[0];
    $distributor_id = $data->vrs_trip_total_distributor_id;
    if(empty($data->trip_id{$trip_number})){
      $data->trip_id{$trip_number} = get_short_values($trip_number, $distributor_id);
    }
    if (!empty($views_current_field)) {
      switch ($views_current_field) {
        case 'inventory_short_value':
          //$res = get_short_values($trip_number, $distributor_id);
          return round($data->trip_id{$trip_number}['inventory_short_value'], 2); 
          break;
        case 'asset_short_value':
          $asset = received_assets_count($trip_number, $distributor_id);
          return $asset['value'];
          break;
        case 'pending_claimed_gs_value':
          //$res = get_short_values($trip_number, $distributor_id);
          return round($data->trip_id{$trip_number}['pending_claimed_gs_value'], 2); 
          break;
        default:
          return '';
      }
    }
  }
}

function get_short_values($trip_number, $distributor_id){
  $query = db_select('dispatch_planning_api_data', 'ad');
  $query->leftjoin('vrs_trip_total', 'vtt', 'ad.runsheet_number = vtt.trip_id AND ad.distributor_id = vtt.distributor_id');
  $query->join('dispatch_planning_item_details', 'dit', 'ad.id = dit.trip_bill_reference');
  $query->addExpression("SUM(distinct(CASE WHEN dit.res_cashier_short != '' THEN dit.res_cashier_short*dit.sum_net_sales_value END))", 'inventory_short_value'); //formula changes (EARLIER: dit.res_cashier_short*dit.sum_net_sales_value)
  $query->addExpression("SUM(distinct(CASE WHEN dit.claimed_godown_short > 0 THEN dit.claimed_godown_short*dit.sum_net_sales_value END))", 'pending_claimed_gs_value'); //formula changes (EARLIER: dit.claimed_godown_short*dit.sum_net_sales_value)
  $query->condition('ad.distributor_id', $distributor_id, '=');
  $query->condition('ad.runsheet_number', $trip_number, '=');
  $query->groupBy('ad.runsheet_number');
  $res = $query->execute()->fetchAssoc();
  return $res;
}