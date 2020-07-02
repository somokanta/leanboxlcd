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
        $nid = check_bill_exists_with_party($value['bill_number'], 'sales_register_data', '');
        if(empty($nid)){
            $error_msg = "Bill Number is not present in System";
        }else{
            $nid = check_bill_exists_with_party($value['bill_number'], 'sales_register_data',$value['party_code']);
            if(empty($nid)){
                $error_msg = "Bill Number should belong to the same Parent Party Code";
            }
        }
    }
    if(empty($error_msg)){
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
    }else{
        $results['result'] = $error_msg;
    }
    return $results;
}

function check_bill_exists_with_party($title, $type, $party) {
    $query1 = db_select('node', 'n');
    $query1->condition('n.type', $type);
    $query1->condition('n.title', $title);
    $query1->fields('n', array('nid'));
    if ($party) {
        $query1->join('field_data_field_sales_party_code', 'd', 'd.entity_id = n.nid');
        $query1->condition('d.field_sales_party_code_value', $party);
    }
    $nid = $query1->execute()->fetchField();
    return $nid;
}