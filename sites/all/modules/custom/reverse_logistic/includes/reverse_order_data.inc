<?php

/**
 * 
 * @param type $data
 * @return string
 */
function reverse_order_data_callback($data) {
    $results['flag'] = '0';
    $error_msg = '';
    watchdog('reverse_order', '<pre>' . print_r($data, TRUE) . '</pre>');
    foreach ($data as $value) {
        if ($value['order_type'] == 'Regular') {
            $nid = check_bill_exists_with_party($value['bill_number'], 'sales_register_data', '');
            if (empty($nid)) {
                $error_msg .= "Bill Number is not present in System:".$value['bill_number'].'<br>';
            } else {
                $nid = check_bill_exists_with_party($value['bill_number'], 'sales_register_data', $value['party_code']);
                if (empty($nid)) {
                    $error_msg .= "Bill Number should belong to the same Parent Party Code".$value['bill_number'].'<br>';
                }else{
                    $delivered_qty = get_delivered_qty_from_bill_sku($value['bill_number'], $value['sku7_code']);
                    if($delivered_qty === FALSE){
                        $error_msg .= "Bill:".$value['bill_number']. " SKu:".$value['sku7_code']." combination is not present in System <br>";
                    }else{
                        if($value['to_be_returned_qty'] > $delivered_qty){
                            $error_msg .= 'To be return Quantity can not be grater than Actual Delivered Quantity for '."Bill:".$value['bill_number']. " SKu:".$value['sku7_code']."<br>";
                        }
                    }
                }
            }
        }
        $hul = validate_content_title($value['parent_party_code'],'hul_code_master');
        if(empty($hul)){
            $error_msg .= 'Parent party Code'.$value['parent_party_code']. 'does not exist in HUL Code Master <br>';
        }
        
    }
    if (empty($error_msg)) {
        foreach ($data as $value) {
            $result = db_merge('reverse_order_data')
                    ->key(array('transaction_id' => $value['transaction_id'], 'transaction_line_id' => $value['transaction_line_id']))
                    ->fields(array(
                        'transaction_id' => $value['transaction_id'] ? $value['transaction_id'] : '',
                        'transaction_line_id' => $value['transaction_line_id'] ? $value['transaction_line_id'] : '',
                        'bill_number' => $value['bill_number'] ? $value['bill_number'] : '',
                        'party_code' => $value['party_code'] ? $value['party_code'] : '',
                        'parent_party_code' => $value['parent_party_code'] ? $value['parent_party_code'] : '',
                        'sku7_code' => $value['sku7_code'] ? $value['sku7_code'] : '',
                        'to_be_returned_qty' => $value['to_be_returned_qty'] ? $value['to_be_returned_qty'] : 0,
                        'pkm' => $value['pkm'] ? $value['pkm'] : '',
                        'reverse_order_date' => $value['reverse_order_date'] ? $value['reverse_order_date'] : 0,
                        'order_type' => $value['order_type'] ? $value['order_type'] : '',
                        'distributor_id' => 0,
                        'received_through' => 0, //API
                        'status' => 0, //Pending
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
