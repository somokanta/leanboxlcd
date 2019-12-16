<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function update_tester_order_data($data) {

    $results['flag'] = '0';
    $results['result'] = "0 Row Updated";
    watchdog('tester order data', '<pre>' . print_r($data, TRUE) . '</pre>');
    foreach ($data as $value) {
        $result = db_merge('tester_order_data')
                ->key(array('tester_order_number' => $value['tester_order_number'], 'distributor_id' => $value['distributor_id'], 'facility_code' => $value['lcd_facility_code']))
                ->fields(array(
                    'tester_order_number' => $value['tester_order_number'],
                    'order_date' => $value['order_date'],
                    'party_code' => $value['party_code'] ? $value['party_code'] : '',
                    'party_name' => $value['party_name'] ? $value['party_name'] : '',
                    'beat_name' => $value['beat_name'] ? $value['beat_name'] : '',
                    'basepack_code' => $value['basepack_code'],
                    'shade_code' => $value['shade_code'] ? $value['shade_code'] : '',
                    'shade_name' => $value['shade_name'] ? $value['shade_name'] : '',
                    'product_description' => $value['product_description'] ? $value['product_description'] : '',
                    'tester_qty' => $value['picked_qty'] ? $value['picked_qty'] : 0,// rest all key is same
                    'seal_number' => $value['seal_number'] ? $value['seal_number'] : 0,
                    'box_id' => $value['box_id'] ? $value['box_id'] : 0,
                    'distributor_id' => $value['distributor_id'],
                    'facility_code' => $value['lcd_facility_code'],
                    'created_time' => REQUEST_TIME
                ))
                ->execute();
        $results['flag'] = '1';
        $results['result'] = "Updated Successfully";
    }
    //Once data inserted then create custom billing and sales register data
    insert_tester_order_data_in_sales_register($data);
    insert_tester_order_data_in_custom_billing_table($data);

    return $results;
}


function insert_tester_order_data_in_sales_register($tester_order_data) {
    watchdog('tester order data sales register', '<pre>' . print_r($data, TRUE) . '</pre>');
    foreach ($tester_order_data as $key => $data) {
        if(!empty($data['tester_order_number'])){
            $distributor_id = $data['distributor_id'];

            $query = db_select('node', 'n');
            $query->condition('n.type', 'sales_register_data');
            $query->condition('n.title', $data['tester_order_number']);
            $query->condition('n.uid', $distributor_id);
            $query->fields('n', array('nid'));
            $nid = $query->execute()->fetchField();

            if (empty($nid)) {
                $node = new stdClass();
                $node->title = $data['tester_order_number'];
                $node->type = "sales_register_data";
                node_object_prepare($node); // Sets some defaults. Invokes hook_prepare() and hook_node_prepare().
                $node->language = LANGUAGE_NONE; // Or e.g. 'en' if locale is enabled
                $node->uid = $distributor_id;
                $node->status = 1; //(1 or 0): published or not
                $node->comment = 0; // 0 = comments disabled, 1 = read only, 2 = read/write

                $node->field_sr_new_bill_date[LANGUAGE_NONE][0]['value'] = $data['order_date'];
                $node->field_bill_value[LANGUAGE_NONE][0]['value'] = 0;
                $node->field_sales_party_code[LANGUAGE_NONE][0]['value'] = $data['party_code'];
                $node->field_sales_updated_date[LANGUAGE_NONE][0]['value'] = time();
                $node->field_crate_id['und'][0]['value'] = $value['box_id'] ? $value['box_id'] : '';
                $node->field_outlet_order_no['und'][0]['value'] = $data['tester_order_number'];
                node_save($node);
            } else {
                $node = node_load($nid);
                $node->field_sr_new_bill_date[LANGUAGE_NONE][0]['value'] = $data['order_date'];
                $node->field_bill_value[LANGUAGE_NONE][0]['value'] = 0;
                $node->field_sales_party_code[LANGUAGE_NONE][0]['value'] = $data['party_code'];
                $node->field_sales_updated_date[LANGUAGE_NONE][0]['value'] = time();
                $node->field_crate_id['und'][0]['value'] = $value['box_id'] ? $value['box_id'] : '';
                $node->field_outlet_order_no['und'][0]['value'] = $data['tester_order_number'];
            }
        }
    }
}


function insert_tester_order_data_in_custom_billing_table($tester_order_data) {
    watchdog('tester order data custom billing', '<pre>' . print_r($data, TRUE) . '</pre>');
    foreach ($tester_order_data as $key => $data) {
        if(!empty($data['tester_order_number'])){
            $distributor_id = $data['distributor_id'];

            //Fetch SKU7_CODE
            $query = db_select('field_data_field_lb_sku7_code', 'sku');
            $query->leftjoin('field_data_field_lb_basepack_code', 'bc', 'bc.entity_id = sku.entity_id');
            $query->leftjoin('node', 'n', 'n.nid = sku.entity_id');
            $query->condition('n.type', 'lb_product_master');
            $query->condition('n.status', 1);
            $query->condition('bc.field_lb_basepack_code_value', $data['basepack_code']);
            $query->addField('sku', 'field_lb_sku7_code_value', 'sku7_code');
            $data['sku7_code'] = $query->execute()->fetchField();

            // For weight and volume calculation
            $sku_wt = 0;
            $sku_vl = 0;
            //TP to host calculation for line level value
            $line_level_value = 0;

    //Fetch SKU7 code – based on basepack code (since multiple SKU7 codes will be found, fetch anyone)
            $stage_billing_data = db_insert('custom_billing_data') // Table name no longer needs {}
                    ->fields(array(
                        //'vehicle' => $data['vehicle'],
                        'loading_date' => $data['order_date'], //T
                        'party_code' => $data['party_code'], //T
                        'BEAT_NAME' => $data['beat_name'], //T
                        'bill_number' => $data['tester_order_number'], //T
                        'bill_date' => $data['order_date'], //T
                        'BASEPACK_CODE' => $data['basepack_code'], //T
                        'product_name' => $data['product_description'], //T
                        'sku7_code' => $data['sku7_code'], //T If needed fetch below based on sku7 master
                        //'PRICELIST_GROUP' => $data['PRICELIST_GROUP'],
                        //'PRODUCT_DIVISION' => $data['PRODUCT_DIVISION'],
                        //'PRODUCT_CATEGORY' => $data['PRODUCT_CATEGORY'],
                        //'PRODUCT_SUB_CATEGORY' => $data['PRODUCT_SUB_CATEGORY'],
                        'mrp' => 0, //T
                        //'tur' => $data['tur'],
                        'Sum_of_TOTAL_SALES_QTY' => $data['picked_qty'], //T
                        'Sum_of_NET_SALES_QTY' => $data['picked_qty'], //T
                        //'Sum_of_SALES_FREE_QTY' => $data['Sum_of_SALES_FREE_QTY'],
                        'Sum_of_NET_SALES_VALUE' => 0, //T
                        //'Sum_of_NET_SALES_CASES' => $data['Sum_of_NET_SALES_CASES'],
                        //'Sum_of_NET_SALES_KGS' => $data['Sum_of_NET_SALES_KGS'],
                        //'Sum_of_SCHEME_DISCOUNT' => $data['Sum_of_SCHEME_DISCOUNT'],
                        //'Sum_of_OTHER_DISCOUNT' => $data['Sum_of_OTHER_DISCOUNT'],
                        //'Sum_of_TAX_PERCENTAGE' => $data['Sum_of_TAX_PERCENTAGE'],
                        //'Derived_NET_SALES_KGS' => $sku_wt ? round($sku_wt,2) : 0,
                        //'Derived_NET_SALES_VOLUME' => $sku_vl ? round($sku_vl,2) : 0,
                        'distributor_id' => $distributor_id,
                        'created_date' => REQUEST_TIME,
                        //TP to HOST entry fields
                        'original_Sum_of_TOTAL_SALES_QTY' => $data['picked_qty'], //T
                        'line_level_value' => $line_level_value //T
                            //END
                    ))
                    ->execute();
        }
    }
}
