<?php

/**
 * @file
 * Definition of views_handler_field_bill_value.
 */

/**
 * Returns crate count
 * @ingroup views_field_handlers
 */
class bill_vs_trip_report extends views_handler_field {

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

        $bill_number = $data->node_title;
        $trip_id = $data->node_field_data_field_trip_id_reference_title;
        $distributor_id = $data->node_uid;
        $views_current_field = $this->field;

        if (!empty($bill_number) && !empty($trip_id)) {
            $query = db_select('dispatch_planning_api_data', 'ad');
            $query->condition('ad.distributor_id', $distributor_id, '=');
            $query->condition('ad.runsheet_number', $trip_id, '=');
            $query->condition('ad.bill_number', $bill_number, '=');
            $query->join('dispatch_planning_item_details', 'dpi', 'dpi.trip_bill_reference = ad.id');
            $query->addExpression('(SUM(dpi.return_qty * dpi.sum_net_sales_value))', 'returned_value');
            $query->addExpression('(SUM(dpi.sum_net_sales_value))', 'sum_net_sales_value');
            $query->addExpression('(CASE WHEN (ad.res_status = \'undeliver\') THEN ad.res_reason_for_failure END)', 'reason_for_failure');
            $bill_trip_report = $query->execute()->fetch(PDO::FETCH_ASSOC);

            //print_r($to_be_collected);die;
            if (!empty($views_current_field)) {
                switch ($views_current_field) {

                    case 'billtrip_return_value';
                        $return_value = $bill_trip_report['returned_value'] ? round($bill_trip_report['returned_value'], 2) : 0;
                        return $return_value;
                        break;
                    case 'billtrip_net_sales_value';
                        $sum_net_sales_value = $bill_trip_report['sum_net_sales_value'] ? round($bill_trip_report['sum_net_sales_value'], 2) : 0;
                        return $sum_net_sales_value;
                        break;
                    case 'billtrip_reason_for_failure';
                        return $bill_trip_report['reason_for_failure'];
                        break;
                    case 'first_attempt_date';
                        $query = db_select('dispatch_planning_api_data', 'ad');
                        $query->condition('ad.distributor_id', $distributor_id, '=');
                        $query->condition('ad.bill_number', $bill_number, '=');
                        $query->addExpression('MIN(ad.dispatch_date)', 'first_dispatch_date');
                        $first_dispatch_date = $query->execute()->fetchField();
                        $first_dispatch_date = !empty($first_dispatch_date) ? date('d-M-y', $first_dispatch_date) : '';
                        return $first_dispatch_date;                      
                        break;

                    default:
                        return '';
                }
            }
        }

        return $bill_trip_report;
    }

}
