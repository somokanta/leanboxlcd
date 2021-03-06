<?php

/**
 * @file
 * Definition of views_handler_field_bill_value.
 */

/**
 * Returns crate count
 * @ingroup views_field_handlers
 */
class bill_returns_report extends views_handler_field {

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

  function render(&$data) {
        $distributor_id = $data->dispatch_planning_api_data_distributor_id;
        $dispatch_planning_id = $data->id;
        $res_status = $data->dispatch_planning_api_data_res_status;
        $delivery_pending = $data->dispatch_planning_api_data_delivery_pending_flag;
        $bill_number =  $data->dispatch_planning_api_data_bill_number;
        // Get views current field value
        $views_current_field = $this->field;
        //$sales_bill_value = $data->_field_data['node_dispatch_planning_api_data_1_nid']['entity']->field_bill_value[LANGUAGE_NONE][0]['value'];
        $bill_level_discount = $data->_field_data['node_dispatch_planning_api_data_1_nid']['entity']->field_bill_level_discount[LANGUAGE_NONE][0]['value'];
        // Bill value from farery header table
        $sales_bill_value = $data->dispatch_planning_api_data_bill_value;
        if (!empty($dispatch_planning_id) && !empty($distributor_id)) {
            $query = db_select('dispatch_planning_api_data', 'dpi');
            $query->condition('dpi.distributor_id', $distributor_id, '=');
            $query->condition('dpi.id', $dispatch_planning_id, '=');
            $query->join('dispatch_planning_item_details', 'ad', 'ad.trip_bill_reference = dpi.id');
            $query->addExpression('(SUM(ad.sum_net_sales_quantity))', 'total_sales_quantity');
            $query->addExpression('(SUM(ad.res_sku_actual_quantity1))', 'actual_delivered_qty');
            $query->addExpression("SUM(ad.claimed_godown_short * ad.sum_net_sales_value)", 'claimed_godown_short_value');
            $query->addExpression('(SUM(ad.res_cashier_short))', 'cashier_short');
            $query->addExpression('(SUM(ad.farey_delivered_qty))', 'farey_delivered_qty');
            $query->addExpression('(SUM(ad.dispatch_qty))', 'dispatch_qty');
            $query->addExpression('(SUM(ad.res_godown_short))', 'godown_short');
            $query->addExpression('(SUM(ad.return_qty))', 'return_qty');
            $query->addExpression('(SUM(ad.sum_net_sales_value * ad.res_sku_actual_quantity1))', 'delivered_value');
            $query->addExpression('(SUM(ad.return_qty * ad.sum_net_sales_value))', 'returned_value');
            $query->addExpression("SUM(ad.res_cashier_short * ad.sum_net_sales_value)", 'cashier_short_value');
            $query->addExpression("SUM(ad.res_godown_short * ad.sum_net_sales_value)", 'godown_short_value');
            $query->addExpression('(CASE WHEN (dpi.res_status = \'undeliver\') THEN dpi.res_reason_for_failure END)', 'reason_for_failure');
            $bill_return_report =$query->execute()->fetch(PDO::FETCH_ASSOC);
            if (!empty($sales_bill_value)) {
                $to_be_collected = round($sales_bill_value - $bill_return_report['returned_value'] - $bill_return_report['cashier_short_value']- $bill_return_report['godown_short_value'] - $bill_return_report['claimed_godown_short_value']);
                if($to_be_collected<0) {
                  $to_be_collected = 0;
                }
                
                if ($data->dispatch_planning_api_data_res_status !== 'undeliver') {
                    $data->sales_to_be_collected = $to_be_collected;
                }
                else {
                    $data->sales_to_be_collected = 0;
                }
                if (!empty($bill_level_discount) && !empty($bill_return_report['delivered_value']) && $bill_level_discount>$bill_return_report['delivered_value']) {
                    $data->sales_to_be_collected = 0;
                }
                 
                
            }
            //print_r($to_be_collected);die;
            if (!empty($views_current_field)) {
                switch ($views_current_field) {
                    case 'total_sales_quantity' :
                        return $bill_return_report['total_sales_quantity'];
                        break;
                    case 'res_godown_short';
                        return $bill_return_report['godown_short'];
                        break;
                    case 'cashier_short';
                        return $bill_return_report['cashier_short'];
                        break;
                    case 'actual_delivered_qty';
                        return $bill_return_report['actual_delivered_qty'];
                        break;
                   case 'dispatched_qty';
                        return $bill_return_report['dispatch_qty'];
                        break;
                    case 'return_qty';
                        return $bill_return_report['return_qty'];
                        break;
                    case 'delivery_app_delivered_qty';
                        return $bill_return_report['farey_delivered_qty'];
                        break;
                    case 'delivered_value';
                        return round($bill_return_report['delivered_value'],2);
                        break;
                    case 'returned_value';
                        return round($bill_return_report['returned_value'],2);
                        break;
                    case 'reason_for_failure';
                        return $bill_return_report['reason_for_failure'];
                        break;
                    case 'to_be_collected_amount';
                        return $data->sales_to_be_collected;
                        break;
                    case 'global_delivery_boy':
                        $res_delivery_boy = $data->dispatch_planning_api_data_res_delivery_boy ? $data->dispatch_planning_api_data_res_delivery_boy : '';
                        $res_delivery_boy_edit = $data->dispatch_planning_api_data_res_delivery_boy_edit ? $data->dispatch_planning_api_data_res_delivery_boy_edit : '';
                        if (!empty($res_delivery_boy_edit)) {
                            return $res_delivery_boy_edit;
                        }
                        else {
                            return $res_delivery_boy;
                        }
                        break;
                    case 'global_facility_name':
                        return get_facility_name($distributor_id);
                        break;
                    case 'delivery_status';
                        //return $res_status;
                        if ($delivery_pending == 1) {
                            $res_status = 'delivery_pending';
                        }
                        return get_res_status_value($res_status);

                    default:
                        return '';
                }
            }
        }

        return $bill_return_report;
    }

}
