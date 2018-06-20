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

  function render($data) {
        $distributor_id = $data->dispatch_planning_api_data_distributor_id;
        $dispatch_planning_id = $data->id;
        // Get views current field value
        $views_current_field = $this->field;
        if (!empty($dispatch_planning_id) && !empty($distributor_id)) {
            $query = db_select('dispatch_planning_api_data', 'dpi');
            $query->condition('dpi.distributor_id', $distributor_id, '=');
            $query->condition('dpi.id', $dispatch_planning_id, '=');
            $query->join('dispatch_planning_item_details', 'ad', 'ad.trip_bill_reference = dpi.id');
            $query->addExpression('(SUM(ad.sum_net_sales_quantity))', 'total_sales_quantity');
            $query->addExpression('(SUM(ad.res_sku_actual_quantity1))', 'actual_delivered_qty');
            $query->addExpression('(SUM(ad.res_cashier_short))', 'cashier_short');
            $query->addExpression('(SUM(ad.farey_delivered_qty))', 'farey_delivered_qty');
            $query->addExpression('(SUM(ad.dispatch_qty))', 'dispatch_qty');
            $query->addExpression('(SUM(ad.res_godown_short))', 'godown_short');
            $query->addExpression('(SUM(ad.return_qty))', 'return_qty');
            $bill_return_report =$query->execute()->fetch(PDO::FETCH_ASSOC);
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
                    default:
                        return '';
                }
            }
        }

        return $bill_return_report;
    }

}
