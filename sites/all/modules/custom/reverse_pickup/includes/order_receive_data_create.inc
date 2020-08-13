<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function order_receive_data_create_callback() {
    $query = db_select('order_receive_data', 'a');
    $query->fields('a');
    $query->condition('flag', 0);
    $res = $query->groupBy('parent_bill_number')->range(0, 5)->execute()->fetchAll(PDO::FETCH_ASSOC);

    foreach ($res as $data) {
        insert_tester_order_data_in_sales_register($data);
    }
    return "1";
}

function insert_tester_order_data_in_sales_register($data) {
    $distributor_id = get_distributor_by_title($data['parent_party_code'], 'hul_code_master');

    $query = db_select('node', 'n');
    $query->condition('n.type', 'sales_register_data');
    $query->leftJoin('field_data_field_parent_bill_number', 'pb', 'n.nid = pb.entity_id');
    $query->condition('pb.field_parent_bill_number_value', $data['parent_bill_number']);
    $query->condition('n.uid', $distributor_id);
    $query->fields('n', array('nid'));
    $seq_result = $query->execute()->fetchCol();

    $seq_num = $seq_record ? (count($seq_result) + 1) : 1;
    $seq = str_pad($seq_num, 2, '0', STR_PAD_LEFT);
    $new_bill_number = "R" . $data['parent_bill_number'] . $data['parent_party_code'] . $seq;

    $node = new stdClass();
    $node->title = $new_bill_number;
    $node->type = "sales_register_data";
    node_object_prepare($node); // Sets some defaults. Invokes hook_prepare() and hook_node_prepare().
    $node->language = LANGUAGE_NONE; // Or e.g. 'en' if locale is enabled
    $node->uid = $distributor_id;
    $node->status = 1; //(1 or 0): published or not
    $node->comment = 0; // 0 = comments disabled, 1 = read only, 2 = read/write

    $node->field_sr_new_bill_date[LANGUAGE_NONE][0]['value'] = $data['order_creation_date'];
    $node->field_sr_bill_date[LANGUAGE_NONE][0]['value'] = $data['order_creation_date'];
    $node->field_billing_loading_date[LANGUAGE_NONE][0]['value'] = $data['order_creation_date'];
    $node->field_bill_value[LANGUAGE_NONE][0]['value'] = 0;
    $node->field_sales_party_code[LANGUAGE_NONE][0]['value'] = $data['party_code'];
    $node->field_sales_updated_date[LANGUAGE_NONE][0]['value'] = time();
    //$node->field_crate_id['und'][0]['value'] = $data['box_id'] ? $data['box_id'] : '';
    $node->field_outlet_order_no['und'][0]['value'] = $data['child_order_id'];
    $node->field_billing_van[LANGUAGE_NONE][0]['value'] = 'Van - 1';
    $node->field_bill_net_sales[LANGUAGE_NONE][0]['value'] = 0;

    $node->field_parent_bill_number[LANGUAGE_NONE][0]['value'] = $data['parent_bill_number'];
    $node->field_order_type[LANGUAGE_NONE][0]['value'] = 1;

    //$node->field_promised_date[LANGUAGE_NONE][0]['value'] = $pdd;
    node_save($node);

    //Now insert Custom Billing Detailss
    insert_tester_order_data_in_custom_billing_table($new_bill_number, $data, $node->nid);
}

function insert_tester_order_data_in_custom_billing_table($new_bill_number, $order_data, $nid) {
    $distributor_id = get_distributor_by_title($order_data['parent_party_code'], 'hul_code_master');
    $bill_value = 0;
    $query = db_select('order_receive_data', 'a');
    $query->fields('a');
    $query->condition('a.parent_bill_number', $order_data['parent_bill_number'], '=');
    $query->condition('a.parent_party_code', $order_data['parent_party_code'], '=');
    $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

    foreach ($res as $key => $data) {
        $previous_data = db_query("select tur,BASEPACK_CODE,PRICELIST_GROUP,PRODUCT_DIVISION,PRODUCT_CATEGORY,PRODUCT_SUB_CATEGORY,line_level_value from custom_billing_data where distributor_id = $distributor_id and  bill_number = '" . $data['parent_bill_number'] . "' and  sku7_code = '" . $data['sku7_code'] . "' limit 1")->fetchAll(PDO::FETCH_ASSOC);
        $previous_details = $previous_data[0];
        // For weight and volume calculation
        $sku_wt = 0;
        $sku_vl = 0;
        //TP to host calculation for line level value
//            $line_level_value = round(
//              (($data['Sum_of_NET_SALES_VALUE'] - 
//                (($data['Sum_of_SCHEME_DISCOUNT'] + $data['Sum_of_OTHER_DISCOUNT'])*
//                 (1+($data['Sum_of_TAX_PERCENTAGE']/100)))) / $data['Sum_of_TOTAL_SALES_QTY'])
//              ,10);
        try {
            $bill_value = $bill_value + ($previous_details['tur'] * $data['return_qty']);
            db_insert('custom_billing_data')
                    ->fields(array(
                        'vehicle' => 'Van - 1',
                        'loading_date' => $data['order_creation_date'],
                        'party_code' => $data['party_code'],
                        'BEAT_NAME' => $data['beat_name'],
                        'bill_number' => $new_bill_number,
                        'bill_date' => $data['order_creation_date'],
                        'BASEPACK_CODE' => $previous_details['BASEPACK_CODE'],
                        'product_name' => $data['product_name'],
                        'sku7_code' => $data['sku7_code'],
                        'PRICELIST_GROUP' => $previous_details['PRICELIST_GROUP'],
                        'PRODUCT_DIVISION' => $previous_details['PRODUCT_DIVISION'],
                        'PRODUCT_CATEGORY' => $previous_details['PRODUCT_CATEGORY'],
                        'PRODUCT_SUB_CATEGORY' => $previous_details['PRODUCT_SUB_CATEGORY'],
                        'mrp' => $data['mrp'],
                        'tur' => $previous_details['tur'],
                        'Sum_of_TOTAL_SALES_QTY' => $data['return_qty'],
                        'Sum_of_NET_SALES_QTY' => $data['return_qty'],
                        'Sum_of_SALES_FREE_QTY' => 0,
                        'Sum_of_NET_SALES_VALUE' => ($previous_details['tur'] * $data['return_qty']),
                        'Sum_of_NET_SALES_CASES' => 0, //Pending
                        'Sum_of_NET_SALES_KGS' => 0, //Pending
                        'Sum_of_SCHEME_DISCOUNT' => 0,
                        'Sum_of_OTHER_DISCOUNT' => 0,
                        //'Sum_of_TAX_PERCENTAGE' => $data['Sum_of_TAX_PERCENTAGE'],
                        'Derived_NET_SALES_KGS' => 0,//Pending $sku_wt ? round($sku_wt,2) : 0,
                        'Derived_NET_SALES_VOLUME' => 0,//Pending $sku_vl ? round($sku_vl,2) : 0,
                        'distributor_id' => $distributor_id,
                        'created_date' => REQUEST_TIME,
                        //TP to HOST entry fields
                        'original_Sum_of_TOTAL_SALES_QTY' => $data['return_qty'],
                        'line_level_value' => $previous_details['line_level_value'],
                            //END
                    ))
                    ->execute();
        } catch (Exception $ex) {
            //you can trigger an email if required
        }
    }
    if ($bill_value) {
        $node = node_load($nid);
        $node->field_bill_value[LANGUAGE_NONE][0]['value'] = $bill_value;
        $node->field_bill_net_sales[LANGUAGE_NONE][0]['value'] = $bill_value;
        node_save($node);
    }

    db_update('order_receive_data')
    ->fields(array('flag' => 1))
    ->condition('parent_bill_number', $order_data['parent_bill_number'], '=')
    ->condition('parent_party_code', $order_data['parent_party_code'], '=')
    ->execute();
}
