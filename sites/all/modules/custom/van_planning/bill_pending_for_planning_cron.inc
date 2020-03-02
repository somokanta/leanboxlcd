<?php

function bill_pending_for_planning_cron() {
    $query = db_select('field_data_field_facility_code', 'facility_code');
    $query->join('taxonomy_term_data', 'term_data', 'term_data.tid = facility_code.field_facility_code_tid');
    $query->join('field_data_field_facility_name', 'facility_name', 'facility_name.entity_id = term_data.tid');
    $query->fields('term_data', array('name'));
    $query->fields('facility_name', array('field_facility_name_value'));
    $all_facility = $query->execute()->fetchAllKeyed(0, 1);

    $total_counter = 0;
    foreach ($all_facility as $crossdoc_facility_code => $crossdoc_facility_name) {

        $json_cross_doc[$crossdoc_facility_code]['Crossdock_Facility_code'] = $crossdoc_facility_code;
        $json_cross_doc[$crossdoc_facility_code]['Crossdock_Facility_Name'] = $crossdoc_facility_name;
        $json_cross_doc[$crossdoc_facility_code]['Transaction_Datetime'] = time();
        //query to find out all bills in that $crossdoc_facility_code
        $facility_bills = get_all_bills_under_facilty_code($crossdoc_facility_code);
        $crossdoc_counter = 0;
        $bill_arr = array();
        foreach ($facility_bills as $val) {
            $bill_arr[$crossdoc_counter]['Merchant_Code'] = 'Lakme';
            $bill_arr[$crossdoc_counter]['Bill_Number'] = $val['bill_no'];
            $bill_arr[$crossdoc_counter]['Bill_Value'] = $val['bill_value'];
            $bill_arr[$crossdoc_counter]['Bill_Date'] = date("Y-m-d", $val['field_sr_bill_date_value']);
            $bill_arr[$crossdoc_counter]['Parent_Party_Code'] = $val['hul_code'];
            $bill_arr[$crossdoc_counter]['Party_Code'] = $val['party_code_val'];
            $bill_arr[$crossdoc_counter]['Party_Name'] = $val['party_name_val'];
            $bill_arr[$crossdoc_counter]['Beat_Name'] = '10-M G ROAD';
            $bill_arr[$crossdoc_counter]['Area_Code'] = $val['area_code'];
            $bill_arr[$crossdoc_counter]['Sales_Day'] = $val['field_sales_day_value'];
            $bill_arr[$crossdoc_counter]['Planned_Dispatch_Date'] = date("Y-m-d");
            $bill_arr[$crossdoc_counter]['Latitude'] = $val['field_hul_updated_lat_value'];
            $bill_arr[$crossdoc_counter]['Longitude'] = $val['field_hul_updated_lat_value'];
            $crossdoc_counter++;
            $total_counter++;
        }

        $json_cross_doc[$crossdoc_facility_code]['Crossdock_Pending_Bills'] = $crossdoc_counter;
        $json_cross_doc[$crossdoc_facility_code]['Order_Details'] = array();
        foreach ($bill_arr as $value) {
            $json_cross_doc[$crossdoc_facility_code]['Order_Details'][] = $value;
        }
    }

    $json['Total_Pending_Bills'] = $total_counter;
    foreach ($json_cross_doc as $value) {
        $json['Bills_Pending_for_Planning'][] = $value;
    }

    dpm($json);
    dpm($bill_pending_for_planning = json_encode($json));
    watchdog('bill_pending_for_planning', '<pre>' . print_r($bill_pending_for_planning, TRUE) . '</pre>');
}

function get_all_bills_under_facilty_code($crossdoc_facility_code) {

    $query = db_select('field_data_field_distributor', 'dis_id');
    $query->innerjoin('field_data_field_facility_code', 'facility_code', 'dis_id.entity_id = facility_code.entity_id');
    $query->join('taxonomy_term_data', 'term_data', 'term_data.tid = facility_code.field_facility_code_tid');
    $query->fields('dis_id', array('field_distributor_value'));
    $query->condition('term_data.name', $crossdoc_facility_code);
    $all_distributor = $query->execute()->fetchAllKeyed(0, 0);

    $query = db_select('field_data_field_facility_code', 'facility_code');
    $query->join('taxonomy_term_data', 'term_data', 'term_data.tid = facility_code.field_facility_code_tid');
    $query->join('users_roles', 'ur', 'ur.uid = facility_code.entity_id');
    $query->join('role', 'role', 'role.rid = ur.rid');
    $query->condition('role.name', 'central_distributor', '=');
    $query->fields('ur', array('uid'));
    $query->condition('term_data.name', $crossdoc_facility_code);
    $central_distributor_id = $query->execute()->fetchField();

    $facility_bills = array();
    $no_of_days = variable_get('van_planning_dates', 15);
    $billing_start_date = strtotime(date('Y-m-d') . "- $no_of_days days");
    $billing_end_date = strtotime(date("Y-m-d"));

    if (!empty($all_distributor) && !empty($central_distributor_id)) {
        $query = db_select('node', 'n');
        $query->condition('n.type', 'hul_code_master');
        $query->addField('n', 'title', 'hul_code');
        $query->leftjoin('field_data_field_area_code', 'ac', 'ac.entity_id = n.nid');
        $query->condition('n.uid', $all_distributor, 'IN');
        $query->leftjoin('node', 'n1', "n1.nid = ac.field_area_code_nid and n1.type ='area_code_master' and n1.uid = $central_distributor_id ");
        $query->addExpression("CASE WHEN (n1.title IS NOT NULL) THEN n1.title ELSE 'NA' END", 'area_code');
        $query->join('field_data_field_hul_code_reference', 'hcr', 'hcr.field_hul_code_reference_nid = n.nid');
        $query->join('node', 'n2', 'n2.nid = hcr.entity_id');
        $query->condition('n2.type', 'outlet_master');
        $query->condition('n2.uid', $all_distributor, 'IN');
        $query->join('field_data_field_party_code', 'fpc', 'fpc.entity_id = n2.nid');
        $query->join('field_data_field_party_name', 'fpn', 'fpn.entity_id = n2.nid');
        $query->join('field_data_field_sales_day', 'fsd', 'fsd.entity_id = n2.nid');
        $query->join('field_data_field_beat_name', 'fbn', 'fbn.entity_id = n2.nid');
        $query->addField('fsd', 'field_sales_day_value', 'field_sales_day_value');
        $query->addField('fbn', 'field_beat_name_value', 'field_beat_name_value');
        $query->join('field_data_field_hul_updated_lat', 'lat', 'lat.entity_id = n.nid');
        $query->join('field_data_field_hul_updated_long', 'lng', 'lng.entity_id = n.nid');
        $query->addField('lat', 'field_hul_updated_lat_value', 'field_hul_updated_lat_value');
        $query->addField('lng', 'field_hul_updated_long_value', 'field_hul_updated_long_value');
        $query->join('field_data_field_party_code_reference', 'pcr', 'pcr.field_party_code_reference_nid = n2.nid');
        $query->join('node', 'n3', 'n3.nid = pcr.entity_id');
        $query->condition('n3.type', 'sales_register_data');
        $query->condition('n3.uid', $all_distributor, 'IN');
        $query->leftJoin('field_data_field_trip_id_reference', 'ft', 'ft.entity_id = n3.nid');
        $query->leftJoin('field_data_field_delivery_pending', 'dp', 'n3.nid = dp.entity_id');
        $query->isNull('ft.field_trip_id_reference_nid');
        $query->leftJoin('field_data_field_bill_status', 'bs', 'bs.entity_id = n3.nid');
        $query->isNull('bs.field_bill_status_value');
        $query->join('field_data_field_sr_bill_date', 'sbd', 'sbd.entity_id = n3.nid');
        $query->condition('sbd.field_sr_bill_date_value', array($billing_start_date, $billing_end_date), 'BETWEEN');
        $query->addField('sbd', 'field_sr_bill_date_value', 'field_sr_bill_date_value');
        $query->join('field_data_field_bill_net_sales', 'bv', 'bv.entity_id = n3.nid');
        $query->addField('n3', 'title', 'bill_no');
        $query->addExpression('ROUND(bv.field_bill_net_sales_value)', 'bill_value');
        $query->addExpression('fpc.field_party_code_value', 'party_code_val');
        $query->addExpression('fpn.field_party_name_value', 'party_name_val');
        $facility_bills = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
    }
    return $facility_bills;
}
