<?php

/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function update_crate_id_data($outlet_order_ref, $crate_id, $order_date,$party_name,$facility_code) {
   $distributor_id = get_distributor_id_of_outlet($party_name); 
   $result = db_merge('outlet_order_crate')
    ->key(array('outlet_order_no' => $outlet_order_ref))
    ->fields(array(
         'outlet_order_no' => $outlet_order_ref,
         'crate_id' => $crate_id ? $crate_id : '',
         'order_date' => $order_date ? $order_date : '',
         'party_name' => $party_name ? $party_name : '',
         'distributor_id' => $distributor_id ? $distributor_id : '',
         'facility_code' => $facility_code ? $facility_code : '',
         'created_time' => REQUEST_TIME,
       ))
       ->execute();

   $results['flag'] = '1';
   $results['result'] = 'Updated Successfully';
   return $results;
}

function get_distributor_id_of_outlet($party_name){
    if (!empty($party_name)) {
        $query = db_select('node', 'oun');
        $query->Join('field_data_field_party_name', 'pn', 'pn.entity_id = oun.nid');
        $query->fields('oun',array('uid'));
        $query->condition('pn.field_party_name_value', $party_name);
        $query->condition('oun.type', 'outlet_master');
        $distributor_id = $query->execute()->fetchField();
    }
    return $distributor_id;
}