<?php

/**
 * 
 * @param type $data
 * @return string
 */
function order_receive_data_callback($data) {
//    $results = array();
//    $error_msg = '';
    watchdog('order_recive_api', '<pre>' . print_r($data, TRUE) . '</pre>');

    foreach ($data['Order Data'] as $value) {
        $results = array();
        $order_creation_date = trim($value['Order Creation Date']);
        $parent_bill_number = trim($value['Parent Bill No']);
        $child_order_id = trim($value['Child Order ID']);
        $order_type = trim($value['Order Type']);
        $party_code = trim($value['Party Code']);
        $beat_name = trim($value['Beat Name']);
        $parent_party_code = trim($value['Parent Party Code']);
        $external_dis_code = trim($value['External Distributor Code']);

        foreach ($value['Order Line'] as $k => $val) {
            $lb_sku_id = trim($val['LBSKUID']);
            $pkd = trim($val['PKD']);
            $sku7_code = trim($val['SKU7']);
            $product_name = trim($val['Product Name']);
            $mrp = trim($val['MRP']);
            $return_qty = trim($val['Return Quantity']);
            $ean_code = trim($val['EAN Code']);
            $crate_id = trim($val['Crate ID']);

            $distributor_id = get_distributor_by_title($parent_party_code, 'hul_code_master');


            if (!($value['Order Type'] == 'Forward' || $value['Order Type'] == 'Reverse')) {
                $res[$k]['flag'] = '0';
                $res[$k]['message'] .= "Order type value should be Forward or Reverse <br>";
                $res[$k]['Child Order ID'] = $child_order_id;
                $res[$k]['Parent Bill No'] = $parent_bill_number;
                $res[$k]['LBSKUID'] = $lb_sku_id;
                $res[$k]['PKD'] = $pkd;
            }
            $bill_number = validate_content_title($value['Parent Bill No'], 'sales_register_data');

            if (($value['Order Type'] == 'Reverse')) {
                if (empty($bill_number)) {
                    $res[$k]['flag'] = '0';
                    $res[$k]['message'] .= "Bill Number is not present in System:" . $value['Parent Bill No'] . "<br>";
                    $res[$k]['Child Order ID'] = $child_order_id;
                    $res[$k]['Parent Bill No'] = $parent_bill_number;
                    $res[$k]['LBSKUID'] = $lb_sku_id;
                    $res[$k]['PKD'] = $pkd;
                } else {
                    $nid = check_bill_exists_with_party($value['Parent Bill No'], 'sales_register_data', $value['Party Code']);
                    if (empty($nid)) {
                        $res[$k]['flag'] = '0';
                        $res[$k]['message'] .= "Bill Number: " . $value['Parent Bill No'] . " Parent Party Code is different in the system <br>";
                        $res[$k]['Child Order ID'] = $child_order_id;
                        $res[$k]['Parent Bill No'] = $parent_bill_number;
                        $res[$k]['LBSKUID'] = $lb_sku_id;
                        $res[$k]['PKD'] = $pkd;
                    } else {
                        $child_order_id = trim($value['Child Order ID']);
                        $child_order_res = check_child_order_in_order_receive($child_order_id, $distributor_id);
                        if ($child_order_res) {
                            $res[$k]['flag'] = '0';
                            $res[$k]['message'] .= "Child Order $child_order_id already exist<br>";
                            $res[$k]['Child Order ID'] = $child_order_id;
                            $res[$k]['Parent Bill No'] = $parent_bill_number;
                            $res[$k]['LBSKUID'] = $lb_sku_id;
                            $res[$k]['PKD'] = $pkd;
                        } else {
                            $child_order_sales_register = check_child_order_in_sales_register($child_order_id, $distributor_id);
                            if ($child_order_sales_register) {
                                $res[$k]['flag'] = '0';
                                $res[$k]['message'] .= "Child Order $child_order_id already exist<br>";
                                $res[$k]['Child Order ID'] = $child_order_id;
                                $res[$k]['Parent Bill No'] = $parent_bill_number;
                                $res[$k]['LBSKUID'] = $lb_sku_id;
                                $res[$k]['PKD'] = $pkd;
                            }
                        }

                        $delivered_qty = get_delivered_qty_from_bill_sku($value['Parent Bill No'], $val['SKU7'], $distributor_id);
                        if ($delivered_qty === FALSE) {
                            $res[$k]['flag'] = '0';
                            $res[$k]['message'] .= "Bill: " . $value['Parent Bill No'] . " SKu: " . $val['SKU7'] . " combination is not present in System <br>";
                            $res[$k]['Child Order ID'] = $child_order_id;
                            $res[$k]['Parent Bill No'] = $parent_bill_number;
                            $res[$k]['LBSKUID'] = $lb_sku_id;
                            $res[$k]['PKD'] = $pkd;
                        } else {
                            if ($val['Return Quantity'] > $delivered_qty) {
                                $res[$k]['flag'] = '0';
                                $res[$k]['message'] .= "Return Quantity can not be grater than Actual Delivered Quantity for Bill: " . $value['Parent Bill No'] . " SKu: " . $val['SKU7'] . "<br>";
                                $res[$k]['Child Order ID'] = $child_order_id;
                                $res[$k]['Parent Bill No'] = $parent_bill_number;
                                $res[$k]['LBSKUID'] = $lb_sku_id;
                                $res[$k]['PKD'] = $pkd;
                            }
                        }
                    }
                }
                //}
                $hul = validate_content_title($value['Parent Party Code'], 'hul_code_master');
                if (empty($hul)) {
                    $res[$k]['flag'] = '0';
                    $res[$k]['message'] .= "Parent party Code " . $value['Parent Party Code'] . " does not exist in HUL Code Master <br>";
                    $res[$k]['Child Order ID'] = $child_order_id;
                    $res[$k]['Parent Bill No'] = $parent_bill_number;
                    $res[$k]['LBSKUID'] = $lb_sku_id;
                    $res[$k]['PKD'] = $pkd;
                }
            } else {
                $child_order_res = check_child_order_in_order_receive($child_order_id, $distributor_id);
                if ($child_order_res) {
                    $res[$k]['flag'] = '0';
                    $res[$k]['message'] .= "Child Order $child_order_id already exist in order receive table<br>";
                    $res[$k]['Child Order ID'] = $child_order_id;
                    $res[$k]['Parent Bill No'] = $parent_bill_number;
                    $res[$k]['LBSKUID'] = $lb_sku_id;
                    $res[$k]['PKD'] = $pkd;
                }
            }
            if ($res[$k]['flag'] != '0') {
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
                            'ean_code' => $ean_code ? $ean_code : '',
                            'crate_id' => $crate_id ? $crate_id : '',
                            'distributor_id' => $distributor_id,
                            'external_dis_id' => $external_dis_code ? $external_dis_code : 0,
                            'type' => 0, //API
                            'created_time' => REQUEST_TIME
                        ))
                        ->execute();
                $res[$k]['flag'] = '1';
                $res[$k]['message'] .= "Updated Successfully";
            }

            $res[$k]['Child Order ID'] = $child_order_id;
            $res[$k]['Parent Bill No'] = $parent_bill_number ? $parent_bill_number : '';
            $res[$k]['LBSKUID'] = $lb_sku_id;
            $res[$k]['PKD'] = $pkd;
            $results[] = $res[$k];
        }
    }
    return $results;
}
