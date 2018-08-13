<?php

/**
 * @file
 * Definition of example_handler_custom_field
 */

/**
 * Provides a custom views field.
 */
class credit_terms_discrepancy extends views_handler_field {

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
        $trip_number = $this->view->args[0];
        $user_id = $user->uid;
        $distributor_id = change_feeds_import_get_distributor_id($user_id);
        $query = db_select('dispatch_planning_api_data', 'ad');
        $query->condition('ad.distributor_id', $distributor_id, '=');
        $query->condition('ad.runsheet_number', $trip_number, '=');
        $query->leftjoin('node', 'n', 'ad.party_code = n.title');
        $query->condition('n.type', 'outlet_master');
        $query->condition('n.uid', $distributor_id, '=');
        $db_or = db_or();
        $db_or->condition('ad.res_money_collect2_edit', 'cheque');
        $db_or->condition('ad.res_money_collect2', 'Cheque');
        $db_or->condition('ad.res_money_collect2', 'Cash,Cheque');
        $db_or->condition('ad.res_money_collect2', 'Cheque,Cash');
        $query->condition($db_or);
        $query->leftjoin('field_data_field_payment_terms', 'pt', 'pt.entity_id = n.nid ');
        $query->leftjoin('field_data_field_credit_days', 'credit_days', 'credit_days.entity_id = n.nid ');
        //$query->addExpression("case when pt.field_payment_terms_value !=  '' then LOWER(pt.field_payment_terms_value) else 'NA' END", "field_payment_terms_value");
        //$query->havingCondition('field_payment_terms_value', 'Cash Or Chq', '=');


        //$query->addExpression('CONVERT(ad.res_confirm_signed_bill_amount, SIGNED)', 'res_confirm_signed_bill_amount');
        //$query->havingCondition('res_confirm_signed_bill_amount', 0, '>');
        $query->fields('ad', array('bill_number'));
        $query->fields('ad', array('party_code'));
        $query->fields('ad', array('party_name'));
        $query->fields('pt', array('field_payment_terms_value'));
        $query->fields('ad', array('res_cheque_date'));
        $query->fields('ad', array('res_cheque_date_edit'));
        $query->fields('ad', array('res_money_collect2_edit'));
        $query->fields('ad', array('res_money_collect2'));
        $query->fields('ad', array('res_cheque_amount_edit'));
        $query->fields('ad', array('res_cheque_amount'));
        $query->fields('ad', array('res_updated_time'));
        $query->fields('ad', array('updated_time'));
        $query->fields('credit_days', array('field_credit_days_value'));
        $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
        $count = 1;
        $rows = array();
         foreach ($res as &$val) {
             $cheque_flag = 0;
            //$start = strtotime(date('Y-m-d', strtotime("-7 days")));
            if (strtolower($val['res_money_collect2_edit']) == 'cheque') {
                // checking edit field no need of res_cheque_date
                unset($val['res_cheque_date']);
                $cheque_flag = 1;
                if (is_numeric($val['res_cheque_date_edit']) && !empty($val['res_cheque_date_edit'])) {
                    $val['res_cheque_date'] = date('d-M-Y', $val['res_cheque_date_edit']);
                } else if (!empty($val['res_cheque_date_edit'])) {
                    $val['res_cheque_date'] = date('d-M-Y', strtotime($val['res_cheque_date_edit']));
                }

                $val['res_cheque_amount'] = $val['res_cheque_amount_edit'];
            } 
            // checking normal fields
            else if(empty($val['res_money_collect2_edit'])) {
                $cheque_flag = 1;
                if (strtolower($val['res_money_collect2']) == 'cheque') {
                    if (is_numeric($val['res_cheque_date'])) {
                        $val['res_cheque_date'] = date('d-M-Y', $val['res_cheque_date']);
                    } else if (!empty($val['res_cheque_date'])) {
                        $val['res_cheque_date'] = date('d-M-Y', strtotime($val['res_cheque_date']));
                    }
                    $val['res_cheque_amount'] = $val['res_cheque_amount'];
                }
                // Fary may contain cash,cheque
                else if ($val['res_money_collect2'] == 'Cash,Cheque' || $val['res_money_collect2'] == 'Cheque,Cash' ) {
                    $money_collect_2_array = explode(',', $val['res_money_collect2']);
                    if (strtolower($money_collect_2_array[0]) == 'cheque' || strtolower($money_collect_2_array[1]) == 'cheque') {
                        if (is_numeric($val['res_cheque_date'])) {
                            $val['res_cheque_date'] = date('d-M-Y', $val['res_cheque_date']);
                        } else if (!empty($val['res_cheque_date'])) {
                            $val['res_cheque_date'] = date('d-M-Y', strtotime($val['res_cheque_date']));
                        }
                        $val['res_cheque_amount'] = $val['res_cheque_amount'];
                    }
                }
            }
            if (!empty($val['res_updated_time'])) { // Farey updated time
                $val['res_updated_time'] = $val['res_updated_time'];
            } elseif (!empty($val['updated_time'])) { // Manual updated time . If farey updated time empty then fetch manual edit time
                $val['res_updated_time'] = $val['updated_time'];
            }
            $credit_days = $val['field_credit_days_value'];
            if (empty($credit_days)) {
                $credit_days = 0;
            }
            $cheque_date_credit_period = strtotime(date('Y-m-d', $val['res_updated_time']) . " + $credit_days days");
            if (!empty($val['res_cheque_date']))
            $cheque_time = strtotime($val['res_cheque_date']);
            // Cheque date credit period expired
            if (!empty($cheque_date_credit_period) && $cheque_time > $cheque_date_credit_period && !empty($cheque_time) && $cheque_flag == 1) {
                $row['counter'] = $count++;
                $row['bill_number'] = $val['bill_number'];
                $row['party_code'] = $val['party_code'];
                $row['party_name'] = $val['party_name'];
                $row['res_updated_time'] = date('d-M-Y',$val['res_updated_time']);
                $row['credit_days'] = $val['field_credit_days_value'];
                $row['res_cheque_date'] = $val['res_cheque_date'];
                $row['res_cheque_amount'] = $val['res_cheque_amount'];
                $total_collected_value += round($val['res_cheque_amount']);
                $rows[] = $row;
           
            }
        }

        if (!empty($rows)) {
            $rows['total'] = array('Total', '', '', '', '', '','', $total_collected_value);
            $header = array('Sr No.', 'Bill Number', 'Party Code', 'Party Name','Delivered Date', 'Credit Terms', 'Cheque Date', 'Cheque Value');
            $output = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('border' => '1', 'cellpadding' => '5', 'cellspacing' => '0')));
        } else {
            $output = '  No Result Found';
        }

        return $output . "<br/>\n";
    }

}
