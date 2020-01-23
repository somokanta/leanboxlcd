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
                ->key(array('tester_order_number' => $value['tester_order_number'], 'basepack_code' => $value['basepack_code'], 'distributor_id' => $value['lcd_distributor_id'], 'facility_code' => $value['lcd_facility_code']))
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
                    'distributor_id' => $value['lcd_distributor_id'],
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
    $test_order_arr = array();
    foreach ($tester_order_data as $key => $data) {
        if(!empty($data['tester_order_number'])){
            if (!array_key_exists($data['tester_order_number'], $test_order_arr)) {
                $test_order_arr[$data['tester_order_number']] = $data['tester_order_number'];
                watchdog('tester order data sales register', '<pre>' . print_r($data, TRUE) . '</pre>');
                $distributor_id = $data['lcd_distributor_id'];

                $query = db_select('node', 'n');
                $query->condition('n.type', 'sales_register_data');
                $query->condition('n.title', $data['tester_order_number']);
                $query->condition('n.uid', $distributor_id);
                $query->fields('n', array('nid'));
                $nid = $query->execute()->fetchField();

                $data['order_date'] = get_modified_order_date_tester($data['order_date'], $distributor_id); //as discussed with kusha
                
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
                    $node->field_sr_bill_date[LANGUAGE_NONE][0]['value'] = $data['order_date'];
                    $node->field_billing_loading_date[LANGUAGE_NONE][0]['value'] = $data['order_date'];
                    $node->field_bill_value[LANGUAGE_NONE][0]['value'] = 0;
                    $node->field_sales_party_code[LANGUAGE_NONE][0]['value'] = $data['party_code'];
                    $node->field_sales_updated_date[LANGUAGE_NONE][0]['value'] = time();
                    $node->field_crate_id['und'][0]['value'] = $data['box_id'] ? $data['box_id'] : '';
                    $node->field_outlet_order_no['und'][0]['value'] = $data['tester_order_number'];
                    $node->field_billing_van[LANGUAGE_NONE][0]['value'] = 'Van - 1';
                    $node->field_bill_net_sales[LANGUAGE_NONE][0]['value'] = 0;
                    
                    //PDD calculation
                    $pdd = get_promised_delivery_date_tester($data['order_date'], $distributor_id);
                    $node->field_promised_date[LANGUAGE_NONE][0]['value'] = $pdd;
                    node_save($node);
                } else {
                    $node = node_load($nid);
                    $node->field_sr_new_bill_date[LANGUAGE_NONE][0]['value'] = $data['order_date'];
                    $node->field_sr_bill_date[LANGUAGE_NONE][0]['value'] = $data['order_date'];
                    $node->field_billing_loading_date[LANGUAGE_NONE][0]['value'] = $data['order_date'];
                    $node->field_bill_value[LANGUAGE_NONE][0]['value'] = 0;
                    $node->field_sales_party_code[LANGUAGE_NONE][0]['value'] = $data['party_code'];
                    $node->field_sales_updated_date[LANGUAGE_NONE][0]['value'] = time();
                    $node->field_crate_id['und'][0]['value'] = $data['box_id'] ? $data['box_id'] : '';
                    $node->field_outlet_order_no['und'][0]['value'] = $data['tester_order_number'];
                    $node->field_billing_van[LANGUAGE_NONE][0]['value'] = 'Van - 1';
                    $node->field_bill_net_sales[LANGUAGE_NONE][0]['value'] = 0;
                    
                    //PDD calculation
                    $pdd = get_promised_delivery_date_tester($data['order_date'], $distributor_id);
                    $node->field_promised_date[LANGUAGE_NONE][0]['value'] = $pdd;
                    node_save($node);
                }
            }
        }
    }
}


function insert_tester_order_data_in_custom_billing_table($tester_order_data) {
    
    foreach ($tester_order_data as $key => $data) {
        if(!empty($data['tester_order_number'])){
            watchdog('tester order data custom billing', '<pre>' . print_r($data, TRUE) . '</pre>');
            $distributor_id = $data['lcd_distributor_id'];
            $central_distributor_id = get_central_distributor_id($distributor_id);
            $tb_id = db_query("select id from custom_billing_data where distributor_id = $distributor_id and  bill_number = '".$data['tester_order_number']."' and BASEPACK_CODE ='".$data['basepack_code']."' limit 1")->fetchField();
            if(empty($tb_id)){
                //Fetch SKU7_CODE
                $query = db_select('field_data_field_lb_sku7_code', 'sku');
                $query->leftjoin('field_data_field_lb_basepack_code', 'bc', 'bc.entity_id = sku.entity_id');
                $query->leftjoin('node', 'n', 'n.nid = sku.entity_id');
                
                //Newly added on 30th dec20. check its sku7 master should be present
                $query->leftjoin('node', 'n2', 'n2.title = sku.field_lb_sku7_code_value');
                $query->condition('n2.type', 'sku7_code_master');
                
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

                $data['order_date'] = get_modified_order_date_tester($data['order_date'], $central_distributor_id); //as discussed with kusha
                
                //Fetch SKU7 code – based on basepack code (since multiple SKU7 codes will be found, fetch anyone)
                db_insert('custom_billing_data')
                    ->fields(array(
                        'vehicle' => 'Van - 1',
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
                        'tur' => 0,
                        'Sum_of_TOTAL_SALES_QTY' => $data['picked_qty'], //T
                        'Sum_of_NET_SALES_QTY' => $data['picked_qty'], //T
                        //'Sum_of_SALES_FREE_QTY' => $data['Sum_of_SALES_FREE_QTY'],
                        'Sum_of_NET_SALES_VALUE' => 0, //T
                        'Sum_of_NET_SALES_CASES' => 0,
                        'Sum_of_NET_SALES_KGS' => 0,
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
            }else{
                //duplicate entry. hit twice so update the qty only
                db_update('custom_billing_data')
                ->fields(array(
                    'Sum_of_TOTAL_SALES_QTY' => $data['picked_qty'], //T
                    'Sum_of_NET_SALES_QTY' => $data['picked_qty'], //T
                    'updated_date' => REQUEST_TIME,
                    'updated_flag' => 1
                ))
                ->condition('id', $tb_id, '=')
                ->execute();
            }
        }
    }
}          
          
function get_promised_delivery_date_tester($date, $distributor_id) {
  $central_distributor_id = get_central_distributor_id($distributor_id);
  $query = db_select('node', 'n');
  $query->condition('n.uid', $central_distributor_id);
  $query->condition('n.type', 'pdd_master');
  $query->fields('n', array('nid'));
  $pdd_value_result = $query->execute()->fetchField();
  
  $pdd_data = node_load($pdd_value_result);
  $pdd_value = $pdd_data->field_pdd[LANGUAGE_NONE][0]['value'];
  $weekly = $pdd_data->field_day[LANGUAGE_NONE];
  foreach ($weekly as $key => $val) {
    $weeklyoff[] = $val['value'];
  }

  $query = db_select('node', 'n');
  $query->condition('n.uid', $central_distributor_id);
  $query->condition('n.type', 'holiday_master');
  $query->leftjoin('field_data_field_holiday', 'hday', 'hday.entity_id = n.nid');
  $query->addExpression("DATE_FORMAT(CONVERT_TZ(FROM_UNIXTIME(hday.field_holiday_value),'+00:00','+05:30'), '%Y-%m-%d')", 'hdate');
  $holiday_result = $query->execute()->fetchCol();

  $billing_date_stamp = $date;
  $pdd = holiday_get_working_day($billing_date_stamp, $pdd_value, $holiday_result, $weeklyoff);
  
  return strtotime($pdd);
}

function get_modified_order_date_tester($date, $central_distributor_id) {
  $query = db_select('node', 'n');
  $query->condition('n.uid', $central_distributor_id);
  $query->condition('n.type', 'pdd_master');
  $query->fields('n', array('nid'));
  $pdd_value_result = $query->execute()->fetchField();
  
  $pdd_data = node_load($pdd_value_result);
  $pdd_value = 1; //$pdd_data->field_pdd[LANGUAGE_NONE][0]['value']; Hardcoded value 1 as discussed with kusha 
  $weekly = $pdd_data->field_day[LANGUAGE_NONE];
  foreach ($weekly as $key => $val) {
    $weeklyoff[] = $val['value'];
  }

  $query = db_select('node', 'n');
  $query->condition('n.uid', $central_distributor_id);
  $query->condition('n.type', 'holiday_master');
  $query->leftjoin('field_data_field_holiday', 'hday', 'hday.entity_id = n.nid');
  $query->addExpression("DATE_FORMAT(CONVERT_TZ(FROM_UNIXTIME(hday.field_holiday_value),'+00:00','+05:30'), '%Y-%m-%d')", 'hdate');
  $holiday_result = $query->execute()->fetchCol();

  $billing_date_stamp = $date;
  $pdd = holiday_get_working_day($billing_date_stamp, $pdd_value, $holiday_result, $weeklyoff);
  
  return strtotime($pdd);
}
