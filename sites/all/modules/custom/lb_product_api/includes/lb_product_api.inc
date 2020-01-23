<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function get_lb_product_api_data($data) {
    if (!empty($data)) {
        watchdog('d8 lb product master', '<pre>' . print_r($data, TRUE) . '</pre>');
        $uid = get_uid_by_username('subadmin');
        foreach ($data as $val) {
            try {
                $nid = check_content_node_title_exists($val['field_lb_sku_id'], 'lb_product_master');
                if (empty($nid)) {
                    $node = new stdClass();
                    $node->title = $val['field_lb_sku_id'];
                    $node->type = "lb_product_master";
                    node_object_prepare($node); // Sets some defaults. Invokes hook_prepare() and hook_node_prepare().
                    $node->language = LANGUAGE_NONE; // Or e.g. 'en' if locale is enabled
                    $node->uid = $uid;
                    $node->status = 1; //(1 or 0): published or not
                    $node->comment = 0; // 0 = comments disabled, 1 = read only, 2 = read/write
                } else {
                    $node = node_load($nid);
                }
                $node->field_lb_sku7_code[LANGUAGE_NONE][0]['value'] = $val['title'];
                $node->field_lb_basepack_code[LANGUAGE_NONE][0]['value'] = $val['field_basepack_code'];
                $node->field_lb_brand[LANGUAGE_NONE][0]['value'] = $val['field_brand'];
                $node->field_lb_category[LANGUAGE_NONE][0]['value'] = $val['field_category1'];
                $node->field_lb_cbu_code[LANGUAGE_NONE][0]['value'] = $val['field_cbu_code'];
                $node->field_company[LANGUAGE_NONE][0]['value'] = $val['field_company'];
                $node->field_lb_ean[LANGUAGE_NONE][0]['value'] = $val['field_ean'];
                $node->field_lb_icld_volume[LANGUAGE_NONE][0]['value'] = $val['field_icld_volume'];
                $node->field_lb_sku_product_name[LANGUAGE_NONE][0]['value'] = $val['field_lb_sku_product_name'];
                $node->field_lb_mrp[LANGUAGE_NONE][0]['value'] = $val['field_mrp'];
                $node->field_lb_offer[LANGUAGE_NONE][0]['value'] = $val['field_offer'];
                $node->field_lb_pack_size_uom[LANGUAGE_NONE][0]['value'] = $val['field_pack_size_uom'];
                $node->field_lb_price_lot[LANGUAGE_NONE][0]['value'] = $val['field_price_lot'];
                $node->field_lb_pack_size[LANGUAGE_NONE][0]['value'] = $val['field_pack_size'];
                $node->field_lb_sku5code[LANGUAGE_NONE][0]['value'] = $val['field_sku5code'];
                $node->field_lb_storage_type[LANGUAGE_NONE][0]['value'] = $val['field_storage_type'];
                $node->field_lb_subcategory[LANGUAGE_NONE][0]['value'] = $val['field_subcategory'];
                $node->field_lb_tester[LANGUAGE_NONE][0]['value'] = $val['field_tester'];
                $node->field_lb_tester_ratio[LANGUAGE_NONE][0]['value'] = $val['field_tester_ratio'];
                $node->field_lb_uom[LANGUAGE_NONE][0]['value'] = $val['field_uom'];
                $node->field_lb_upc[LANGUAGE_NONE][0]['value'] = $val['field_upc'];
                $node->field_lb_volume[LANGUAGE_NONE][0]['value'] = $val['field_volume'];
                $node->field_lb_weight[LANGUAGE_NONE][0]['value'] = $val['field_weight'];
                $node->field_lb_sub_brand[LANGUAGE_NONE][0]['value'] = $val['field_sub_brand1'];
                $node->field_icld_ean[LANGUAGE_NONE][0]['value'] = $val['field_icld_ean'];
                $node->status = $val['status'];
                $sku7_code_nid = check_sku7_code_exists($val['title']);
                // Create SKU7 code master
                if (empty($sku7_code_nid)) {
                    create_sku7code_master($val,$uid);
                }
                node_save($node);
            } catch (Exception $ex) {
                watchdog('lb product master api error', '<pre>' . print_r($ex->getMessage(), TRUE) . '</pre>');
            }
        }

        $results['flag'] = '1';
        $results['result'] = 'Data updated Sucessfully.';
    } else {
        $results['flag'] = '0';
        $results['result'] = 'No data was updated.';
    }

    return $results;
}

function check_content_node_title_exists($title, $type) {
    $query1 = db_select('node', 'n');
    $query1->condition('n.type', $type);
    $query1->condition('n.title', $title);
    $query1->fields('n', array('nid'));
    $nid = $query1->execute()->fetchField();
    return $nid;
}

function check_sku7_code_exists($title) {
    if (!empty($title)) {
        $query1 = db_select('node', 'n');
        $query1->condition('n.type', 'sku7_code_master');
        $query1->condition('n.title', $title);
        $query1->fields('n', array('nid'));
        $nid = $query1->execute()->fetchField();
    }
    return $nid;
}

function create_sku7code_master($val = array(),$uid) {
    if (!empty($val)) {
        $node = new stdClass();
        $node->title = $val['title'];
        $node->type = "sku7_code_master";
        node_object_prepare($node); // Sets some defaults. Invokes hook_prepare() and hook_node_prepare().
        $node->language = LANGUAGE_NONE; // Or e.g. 'en' if locale is enabled
        $node->uid = $uid;
        $node->status = $val['status']; //(1 or 0): published or not
        $node->field_unit_case_units_[LANGUAGE_NONE][0]['value'] = $val['field_upc'];
        $node->field_product_name[LANGUAGE_NONE][0]['value'] = $val['field_lb_sku_product_name'];
        $node->field_sku7_code[LANGUAGE_NONE][0]['value'] = $val['title'];
        $node->comment = 0; // 0 = comments disabled, 1 = read only, 2 = read/write
        $brand_nid = get_node_nid($val['field_brand'], 'brand_master');
        $sucate_nid = get_node_nid($val['field_subcategory'], 'product_sub_category_master');
        if (!empty($brand_nid)) {
            $node->field_brand[LANGUAGE_NONE][0]['nid'] = $brand_nid;
        }
        if (!empty($sucate_nid)) {
            $node->field_sub_category[LANGUAGE_NONE][0]['nid'] = $sucate_nid;
        }
        node_save($node);
    }
}


function get_node_nid($title, $type) {
    $query1 = db_select('node', 'n');
    $query1->condition('n.type', $type);
    $query1->condition('n.title', $title);
    $query1->fields('n', array('nid'));
    $nid = $query1->execute()->fetchField();
    return $nid;
}
