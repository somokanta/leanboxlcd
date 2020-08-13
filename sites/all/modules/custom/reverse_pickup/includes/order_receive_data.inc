<?php

/**
 * 
 * @param type $data
 * @return string
 */
function order_receive_data_callback($data) {
    $results['flag'] = '0';
    $error_msg = '';
    watchdog('order_recive_api', '<pre>' . print_r($data, TRUE) . '</pre>');
    foreach ($data['Order Data'] as $value) {
        $distributor_id = get_distributor_by_title($value['Parent Party Code'], 'hul_code_master');
        //if ($value['Order Type'] == 'Forward') {
            if(!($value['Order Type'] == 'Forward' || $value['Order Type'] == 'Reverse')){
                $error_msg .= "Order type value should be Forward or Reverse <br>";
            }
            $bill_number = validate_content_title($value['Parent Bill No'], 'sales_register_data');;
            if (empty($bill_number)) {
                $error_msg .= "Bill Number is not present in System:".$value['Parent Bill No']."<br>";
            } else {
                $nid = check_bill_exists_with_party($value['Parent Bill No'], 'sales_register_data', $value['Party Code']);
                if (empty($nid)) {
                    $error_msg .= "Bill Number: ".$value['Parent Bill No']." Parent Party Code is different in the system <br>";
                }else{
                    $child_order_id = trim($value['Child Order ID']);
                    $child_order_res = check_child_order_in_order_receive($child_order_id, $distributor_id);
                    if ($child_order_res) {
                        $error_msg .= "Child Order $child_order_id already exist<br>";
                    }else{
                        $child_order_sales_register = check_child_order_in_sales_register($child_order_id, $distributor_id);
                        if ($child_order_sales_register) {
                            $error_msg .= "Child Order $child_order_id already exist<br>";
                        }
                    }
                    //Child level Validation
                    foreach ($value['Order Line'] as $val) {
                        $delivered_qty = get_delivered_qty_from_bill_sku($value['Parent Bill No'], $val['SKU7'],$distributor_id);
                        if($delivered_qty === FALSE){
                            $error_msg .= "Bill: ".$value['Parent Bill No']. " SKu: ".$val['SKU7']." combination is not present in System <br>";
                        }else{
                            if($val['Return Quantity'] > $delivered_qty){
                                $error_msg .= "Return Quantity can not be grater than Actual Delivered Quantity for Bill: ".$value['Parent Bill No']. " SKu: ".$val['SKU7']."<br>";
                            }
                        }
                    }
                }
            }
        //}
        $hul = validate_content_title($value['Parent Party Code'],'hul_code_master');
        if(empty($hul)){
            $error_msg .= "Parent party Code ".$value['Parent Party Code']. " does not exist in HUL Code Master <br>";
        }
        
    }
    if (empty($error_msg)) {
        foreach ($data['Order Data'] as $value) {
            
            $order_creation_date = trim($value['Order Creation Date']);
            $parent_bill_number = trim($value['Parent Bill No']);
            $child_order_id = trim($value['Child Order ID']);
            $order_type = trim($value['Order Type']);
            $party_code = trim($value['Party Code']);
            $beat_name = trim($value['Beat Name']);
            $parent_party_code = trim($value['Parent Party Code']);
            foreach ($value['Order Line'] as $val) {
                $lb_sku_id = trim($val['LBSKUID']);
                $pkd = trim($val['PKD']); 
                $sku7_code = trim($val['SKU7']); 
                $product_name = trim($val['Product Name']); 
                $mrp = trim($val['MRP']); 
                $return_qty = trim($val['Return Quantity']); 

                $distributor_id = get_distributor_by_title($parent_party_code, 'hul_code_master');
                $result = db_insert('order_receive_data')
                        ->fields(array(
                            'order_creation_date' => $order_creation_date ? strtotime($order_creation_date) : 0,
                            'parent_bill_number' => $parent_bill_number ? $parent_bill_number : '',
                            'child_order_id' => $child_order_id ? $child_order_id : '',
                            'order_type' => $order_type ? $order_type : '',
                            'parent_party_code' => $parent_party_code ? $parent_party_code : '',
                            'party_code' => $party_code ? $party_code : '',
                            'beat_name' => $beat_name ? $beat_name : '',
                            'lb_sku_id' => $lb_sku_id ? $lb_sku_id : '',
                            'pkd' => $pkd ? $pkd : '',
                            'sku7_code' => $sku7_code ? $sku7_code : '',                    
                            'product_name' => $product_name ? $product_name : '',                    
                            'mrp' => $mrp ? $mrp : '',                    
                            'return_qty' => $return_qty ? $return_qty : '',                    
                            'distributor_id' => $distributor_id,
                            'type' => 0, //API
                            'created_time' => REQUEST_TIME
                        ))
                        ->execute();
            }
            $results['flag'] = '1';
            $results['result'] = "Updated Successfully";
        }
    } else {
        $results['result'] = $error_msg;
    }
    return $results;
}
