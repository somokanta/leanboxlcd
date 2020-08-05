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
    foreach ($data as $value) {
        $distributor_id = get_distributor_by_title($value['Parent Party Code/ HUL Code'], 'hul_code_master');
        //if ($value['Order Type'] == 'Forward') {
            $bill_number = validate_content_title($value['Parent Bill No.'], 'sales_register_data');;
            if (empty($bill_number)) {
                $error_msg .= "Bill Number is not present in System:".$value['Parent Bill No.'].'<br>';
            } else {
                $nid = check_bill_exists_with_party($value['Parent Bill No.'], 'sales_register_data', $value['Party Code']);
                if (empty($nid)) {
                    $error_msg .= "Bill Number: ".$value['Parent Bill No.']." Parent Party Code is different in the system <br>";
                }else{
                    $delivered_qty = get_order_qty_from_bill_sku($value['Parent Bill No.'], $value['SKU7'],$distributor_id);
                    if($delivered_qty === FALSE){
                        $error_msg .= "Bill:".$value['Parent Bill No.']. " SKu:".$value['SKU7']." combination is not present in System <br>";
                    }else{
                        if($value['to_be_returned_qty'] > $delivered_qty){
                            $error_msg .= 'To be return Quantity can not be grater than Actual Delivered Quantity for '."Bill:".$value['Parent Bill No.']. " SKu:".$value['SKU7']."<br>";
                        }
                    }
                }
            }
        //}
        $hul = validate_content_title($value['Parent Party Code/ HUL Code'],'hul_code_master');
        if(empty($hul)){
            $error_msg .= 'Parent party Code'.$value['Parent Party Code/ HUL Code']. 'does not exist in HUL Code Master <br>';
        }
        
    }
    if (empty($error_msg)) {
        foreach ($data as $value) {
            
            $order_creation_date = trim($value['Order Creation Date']);
            $parent_bill_number = trim($value['Parent Bill No.']);
            $child_order_id = trim($value['Child Order ID']);
            $order_type = trim($value['Order Type']);
            $party_code = trim($value['Party Code']);
            $parent_party_code = trim($value['Parent Party Code/ HUL Code']);
            $lb_sku_id = trim($value['LBSKUID']);
            $pkd = trim($value['PKD']); 
            $sku7_code = trim($value['SKU7']); 
            $product_name = trim($value['Product Name']); 
            $mrp = trim($value['MRP']); 
            $return_qty = trim($value['Return Quantity']); 

            $distributor_id = get_distributor_by_title($parent_party_code, 'hul_code_master');
            $result = db_insert('order_receive_data')
                    ->fields(array(
                        'order_creation_date' => $order_creation_date ? strtotime($order_creation_date) : 0,
                        'parent_bill_number' => $parent_bill_number ? $parent_bill_number : '',
                        'child_order_id' => $child_order_id ? $child_order_id : '',
                        'order_type' => $order_type ? $order_type : '',
                        'parent_party_code' => $parent_party_code ? $parent_party_code : '',
                        'party_code' => $party_code ? $party_code : '',
                        'lb_sku_id' => $lb_sku_id ? $lb_sku_id : '',
                        'pkd' => $pkd ? $pkd : '',
                        'sku7_code' => $sku7_code ? $sku7_code : '',                    
                        'product_name' => $product_name ? $product_name : '',                    
                        'mrp' => $mrp ? $mrp : '',                    
                        'return_qty' => $return_qty ? $return_qty : '',                    
                        'distributor_id' => $distributor_id,
                        'type' => 1, //Uploader
                        'created_time' => REQUEST_TIME
                    ))
                    ->execute();
            
            $results['flag'] = '1';
            $results['result'] = "Updated Successfully";
        }
    } else {
        $results['result'] = $error_msg;
    }
    return $results;
}
