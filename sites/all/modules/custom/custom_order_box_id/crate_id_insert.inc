<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function update_crate_id_data($outlet_order_ref, $crate_id, $order_date, $party_name, $facility_code, $distributor_id,$party_code,$hul_code) {
    //$distributor_id = get_distributor_id_of_outlet($party_name); 
    if (empty($distributor_id)) {
        $distributor_id = 0;
    }
    $query = db_select('outlet_order_crate', 'ooc');
    $query->fields('ooc', array('id','crate_id'));
    $query->condition('ooc.outlet_order_no', $outlet_order_ref);
    $query->condition('ooc.distributor_id',$distributor_id);
    $exist_id_crate = $query->execute()->fetchAllKeyed(0,1);
        
    if ($exist_id_crate) {
        foreach($exist_id_crate as $id => $crate_exist){
            $combine_crate = "$crate_exist, $crate_id";
            $box_id_res = array_map('trim',array_filter(explode(',',$combine_crate)));
            $box_id_arr = array_unique($box_id_res);
            natsort($box_id_arr);
            $box_id_final = implode($box_id_arr,', ');

            $result = db_update('outlet_order_crate')
                ->fields(array(
                    'crate_id' => $box_id_final ? $box_id_final : '',
                ))
                ->condition('id', $id)
                ->execute();
        }
    } else {
        $result = db_insert('outlet_order_crate')
                ->fields(array(
                    'outlet_order_no' => $outlet_order_ref,
                    'crate_id' => $crate_id ? $crate_id : '',
                    'order_date' => $order_date ? $order_date : '',
                    'party_name' => $party_name ? $party_name : '',
                    'distributor_id' => $distributor_id ? $distributor_id : 0,
                    'facility_code' => $facility_code ? $facility_code : '',
                    'party_code' => $party_code ? $party_code : '',
                    'hul_code' => $hul_code ? $hul_code : '',
                    'created_time' => REQUEST_TIME,
                ))
                ->execute();
    }

    $results['flag'] = '1';
    $results['result'] = 'Updated Successfully';
    return $results;
}

function get_distributor_id_of_outlet($party_name) {
    if (!empty($party_name)) {
        $query = db_select('node', 'oun');
        $query->Join('field_data_field_party_name', 'pn', 'pn.entity_id = oun.nid');
        $query->fields('oun', array('uid'));
        $query->condition('pn.field_party_name_value', $party_name);
        $query->condition('oun.type', 'outlet_master');
        $distributor_id = $query->execute()->fetchField();
    }
    return $distributor_id;
}

function update_distributor_id() {
    $query = db_select('outlet_order_crate', 'pt_crate');
    $query->isNull('pt_crate.distributor_id');
    $query->fields('pt_crate', array('party_name'));
    $query->isNotNull('pt_crate.party_name');
    $party_names = $query->execute()->fetchCol();
    if (!empty($party_names)) {
        $query = db_select('node', 'n');
        $query->condition('n.type', 'outlet_master');
        $query->condition('pn.field_party_name_value', $party_names, 'IN');
        $query->join('field_data_field_party_name', 'pn', 'pn.entity_id = n.nid');
        $query->fields('pn', array('field_party_name_value'));
        $query->fields('n', array('uid'));
        $result = $query->execute()->fetchAllKeyed(0, 1);
        if (!empty($result)) {
            foreach ($result as $party_name => $distributor_id) {
                $result = db_merge('outlet_order_crate')
                        ->key(array('party_name' => $party_name))
                        ->fields(array(
                            'distributor_id' => $distributor_id ? $distributor_id : '',
                        ))
                        ->execute();
            }
        }
    }
}
