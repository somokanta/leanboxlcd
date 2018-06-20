<?php
function get_current_vrs_version($trip_id, $dis_id) {
    if (!empty($trip_id) && !empty($dis_id)) {
        $query = db_select('node', 'disp_data');
        $query->condition('disp_data.type', 'dispatch_data');
        $query->condition('disp_data.title', $trip_id);
        $query->condition('disp_data.status', 1);
        $query->condition('disp_data .uid', $dis_id);
        $query->leftjoin('field_data_field_vrs_version', 'vc', 'vc.entity_id = disp_data.nid');
        $query->fields('vc', array('field_vrs_version_value'));
        $vrs_closure_id = $query->execute()->fetchField();
        return $vrs_closure_id;
    }
}
// function to fetch vrs bill items
function get_vrs_total_items($trip_id, $distributor_id, $vrs_version) {
    $res = array();
    if (!empty($trip_id) && !empty($distributor_id) && !empty($vrs_version)) {
        $query = db_select('vrs_trip_total', 'total');
        $query->condition('total.trip_id', $trip_id);
        $query->condition('total.vrs_version_no', $vrs_version);
        $query->condition('total.distributor_id', $distributor_id);
        $query->fields('total', array('trip_id', 'bill_count', 'total_to_be_collected_sales_register', 'total_to_be_collected_amt'
            , 'total_actual_cash_amount', 'total_actual_cheque_amount', 'total_actual_signed_bill_amount',
            'total_short_amt', 'total_cashier_debit_value', 'cheque_count', 'sb_count'
            , 'vrs_version_no', 'id', 'trip_id', 'total_cashier_cash_short', 'vrs_created_date'));
        $res = $query->execute()->fetchAll(PDO::FETCH_UNIQUE | PDO::FETCH_ASSOC);
    }
    return $res;
}

// function to fetch vrs bill items
function get_vrs_bill_items($trip_id, $distributor_id, $vrs_version) {
    $res = array();
    if (!empty($trip_id) && !empty($distributor_id) && !empty($vrs_version)) {
        $query = db_select('vrs_trip_items', 'bills');
        $query->condition('bills.trip_id', $trip_id);
        $query->condition('bills.vrs_version_no', $vrs_version);
        $query->condition('bills.distributor_id', $distributor_id);
        $query->fields('bills', array('bill_number', 'bill_status', 'to_be_collected_amt_sales_register'
            , 'to_be_collected_amt', 'actual_cash_amount', 'actual_cheque_amount',
            'actual_signed_bill_amount', 'cheque_number', 'cheque_date', 'bank_name'
            , 'short_amt', 'reason_code', 'vrs_version_no', 'trip_id', 'remarks', 'non_billable', 'photo_cheque', 'photo_signed_bill','non_bank_reason_code'));
        $res = $query->execute()->fetchAll(PDO::FETCH_UNIQUE | PDO::FETCH_ASSOC);
    }
    return $res;
}


function get_bill_details($bill_numbers, $trip_id) {
    if (!empty($bill_numbers) && !empty($trip_id)) {
        global $user;
        $dis_id = change_feeds_import_get_distributor_id($user->uid);
        $query = db_select('dispatch_planning_api_data', 'ad');
        $query->condition('ad.runsheet_number', $trip_id, '=');
        $query->condition('ad.distributor_id', $dis_id, '=');
        $query->condition('ad.bill_number', $bill_numbers, 'IN');
        $query->fields('ad', array('bill_number', 'res_status'));
        $res = $query->execute()->fetchAll(PDO::FETCH_UNIQUE | PDO::FETCH_ASSOC);
        return $res;
    }
}
// function to fetch vrs bill items vrs_trip_denominations
function get_vrs_total_denominations($trip_id, $vrs_total_id, $distributor_id) {
    $res = array();
    if (!empty($trip_id) && !empty($distributor_id)) {
        $query = db_select('vrs_trip_denominations', 'deno');
        $query->condition('deno.trip_id', $trip_id);
        $query->condition('deno.vrs_total_id', $vrs_total_id);
        $query->condition('deno.distributor_id', $distributor_id);
        $query->fields('deno', array('denominations_nid', 'no_of_notes', 'trip_id', 'vrs_total_id', 'coins'));
        $res = $query->execute()->fetchAll(PDO::FETCH_UNIQUE | PDO::FETCH_ASSOC);
    }
    return $res;
}



// function to fetch received assets count

function received_assets_count($trip_id, $dis_id) {
    $asset = array();
    if (!empty($trip_id) && !empty($dis_id)) {
        $vocabulary = t('asset_type');
        $vid = taxonomy_vocabulary_machine_name_load($vocabulary)->vid;
        $term_name = t('Returnable');
        $term_obj = taxonomy_get_term_by_name($term_name, $vocabulary);
        if (!empty($term_obj)) {
            $returnable_term_id = key($term_obj);
        }
        //print_r($returnable_term_id);die;
        if (!empty($returnable_term_id)) {
            // Join With Asset Count content type to get total value of returnable assets
            $query = db_select('node', 'acnt');
            $query->fields('acnt', array('title'));
            $query->condition('acnt.type', 'assets_count');
            $query->condition('acnt.title', $trip_id);
            $query->condition('acnt.uid', $dis_id);
            $query->leftjoin('field_data_field_asset_type', 'atype', 'atype.entity_id = acnt.nid');
            //$query->fields('atype', array('field_asset_type_nid'));
            $query->leftjoin('field_data_field_asset_types', 'atypes', 'atypes.entity_id = atype.field_asset_type_nid');
            $query->leftjoin('field_data_field_asset_value', 'asset_val', 'asset_val.entity_id = atype.field_asset_type_nid');
           // $query->fields('asset_val', array('field_asset_value_value'));
            $query->leftjoin('field_data_field_dispatch_count', 'd_cnt', 'd_cnt.entity_id = acnt.nid');
            $query->leftjoin('field_data_field_received_count', 'r_cnt', 'r_cnt.entity_id = acnt.nid');
            //$query->fields('r_cnt', array('field_received_count_value'));
            $query->addExpression("SUM((d_cnt.field_dispatch_count_value - r_cnt.field_received_count_value) * asset_val.field_asset_value_value)", 'asset_value');
            
            $query->condition('atypes.field_asset_types_tid', $returnable_term_id);
          // $query->addExpression("SUM(r_cnt.field_received_count_value * asset_val.field_asset_value_value)", 'asset_value');
            $query->addExpression("SUM((d_cnt.field_dispatch_count_value - r_cnt.field_received_count_value))", 'asset_count');
            //$query->groupBy('atype.field_asset_type_nid');
            $res = $query->execute()->fetch(PDO::FETCH_ASSOC);
            if (!empty($res)) {
                $asset = array('received_qty' =>$res['asset_count'], 'value' =>$res['asset_value']);
            }
        }
    }
    return $asset;
}

function get_return_short($trip_id, $distributor_id) {
    $short = array();
    if (!empty($trip_id) && !empty($distributor_id)) {
        $query = db_select('dispatch_planning_api_data', 'dpi');
        $query->condition('dpi.distributor_id', $distributor_id);
        $query->condition('dpi.runsheet_number', $trip_id);
        $query->join('dispatch_planning_item_details', 'ad', 'ad.trip_bill_reference = dpi.id');
        $query->addExpression('(SUM(ad.res_godown_short))', 'sum_godown_short_qty');
        $query->addExpression('(SUM(ad.res_cashier_short))', 'sum_cashier_short_qty');
        $query->addExpression('(SUM(ad.res_godown_short * ad.sum_net_sales_value))', 'sum_godown_short_value');
        $query->addExpression('(SUM(ad.res_cashier_short * ad.sum_net_sales_value))', 'sum_cashier_short_value');
        $short = $query->execute()->fetch(PDO::FETCH_ASSOC);
    }
    return $short;
}

function received_assets_count_details($trip_id, $dis_id) {
    $asset = array();
    if (!empty($trip_id) && !empty($dis_id)) {
        $vocabulary = t('asset_type');
        $vid = taxonomy_vocabulary_machine_name_load($vocabulary)->vid;
        $term_name = t('Returnable');
        $term_obj = taxonomy_get_term_by_name($term_name, $vocabulary);
        if (!empty($term_obj)) {
            $returnable_term_id = key($term_obj);
        }
        if (!empty($returnable_term_id)) {
            // Join With Asset Count content type to get total value of returnable assets
            $query = db_select('node', 'acnt');
            $query->fields('acnt', array('title'));
            $query->condition('acnt.type', 'assets_count');
            $query->condition('acnt.title', $trip_id);
            $query->condition('acnt.uid', $dis_id);
            $query->leftjoin('field_data_field_asset_type', 'atype', 'atype.entity_id = acnt.nid');
            //$query->fields('atype', array('field_asset_type_nid'));
            $query->leftjoin('field_data_field_asset_types', 'atypes', 'atypes.entity_id = atype.field_asset_type_nid');
            $query->leftjoin('node', 'as_type', 'as_type.nid = atype.field_asset_type_nid');
            $query->fields('as_type', array('title'));
            $query->leftjoin('field_data_field_asset_value', 'asset_val', 'asset_val.entity_id = atype.field_asset_type_nid');
            $query->fields('asset_val', array('field_asset_value_value'));
            $query->leftjoin('field_data_field_dispatch_count', 'd_cnt', 'd_cnt.entity_id = acnt.nid');
            $query->leftjoin('field_data_field_received_count', 'r_cnt', 'r_cnt.entity_id = acnt.nid');
            //$query->fields('r_cnt', array('field_received_count_value'));
            $query->addExpression("SUM((d_cnt.field_dispatch_count_value - r_cnt.field_received_count_value) * asset_val.field_asset_value_value)", 'asset_value');

            $query->condition('atypes.field_asset_types_tid', $returnable_term_id);
            //$query->addExpression("SUM(r_cnt.field_received_count_value * asset_val.field_asset_value_value)", 'asset_value');
            $query->addExpression("SUM((d_cnt.field_dispatch_count_value - r_cnt.field_received_count_value))", 'asset_short_count');
            $query->groupBy('atype.field_asset_type_nid');
            $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
            if (!empty($res)) {
                // $asset = array('received_qty' =>$res['asset_count'], 'value' =>$res['asset_value']);
                foreach ($res as $key => $val) {
                    if ($val['asset_short_count']>0) {
                        $asset[$key]['asset_name'] = $val['as_type_title'];
                        $asset[$key]['field_asset_value_value'] = $val['field_asset_value_value'];
                        $asset[$key]['short_qty'] = $val['asset_short_count'];
                    }
                }
            }
        }
    }
    return $asset;
}
