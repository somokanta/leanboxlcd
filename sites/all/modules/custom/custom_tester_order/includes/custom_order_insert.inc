<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function update_tester_order_data($data) {

    $results['flag'] = '0';
    $results['result'] = "0 Row Updated";

    foreach ($data as $value) {
        $result = db_merge('tester_order_data')
                ->key(array('tester_order_number' => $value['tester_order_number'], 'distributor_id' => $value['distributor_id'], 'facility_code' => $value['lcd_facility_code']))
                ->fields(array(
                    'tester_order_number' => $value['tester_order_number'],
                    'order_date' => $value['order_date'],
                    'party_code' => $value['party_code'],
                    'party_name' => $value['party_name'],
                    'sales_beat' => $value['beat_name'],
                    'basepack_code' => $value['basepack_code'],
                    'shade_code' => $value['shade_code'],
                    'shade_name' => $value['shade_name'],
                    'product_description' => $value['product_description'],
                    'tester_qty' => $value['picked_qty'],
                    'seal_number' => $value['seal_number'],
                    'box_id' => $value['box_id'],
                    'distributor_id' => $value['distributor_id'],
                    'facility_code' => $value['lcd_facility_code'],
                    'created_time' => REQUEST_TIME
                ))
                ->execute();
        $results['flag'] = '1';
        $results['result'] = "Updated Successfully";
    }
    return $results;
}
