<?php
/**
 * 
 * @param type $data
 * @return string
 */

function reverse_order_data_callback($data) {
    $results['flag'] = '0';
    $results['result'] = "0 Row Updated";
    watchdog('reverse_order', '<pre>' . print_r($data, TRUE) . '</pre>');
    foreach ($data as $value) {
        watchdog('reverse_order inside', '<pre>' . print_r($value, TRUE) . '</pre>');
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

                    'distributor_id' => 0,
                    'received_through' => 0, //API
                    'status' => 0, //Pending
                    'created_time' => REQUEST_TIME
                ))
                ->execute();
        $results['flag'] = '1';
        $results['result'] = "Updated Successfully";
    }
    return $results;
}
