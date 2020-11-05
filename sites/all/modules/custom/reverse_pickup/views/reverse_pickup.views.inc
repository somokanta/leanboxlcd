<?php

function reverse_pickup_views_data() {
    $data['order_receive_data']['table'] = array(
        'group' => t('Order Receive Data'),
        'title' => t('Order Receive Data'),
        'help' => t('Order Receive Data.'),
    );
    // Define this as a base table.
    $data['order_receive_data']['table']['base'] = array(
        'field' => 'id',
        'title' => t('Order Receive Data'),
        'help' => t("Order Receive Data unique identifiers."),
            // 'weight' => -10,
    );

    $data['order_receive_data']['id'] = array(
        'title' => t('ID'),
        'help' => t('The record ID.'),
        'field' => array(
            'handler' => 'views_handler_field_numeric',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_numeric',
        ),
    );

    $data['order_receive_data']['order_creation_date'] = array(
        'title' => t('Order Creation Date'),
        'help' => t('Order Creation Date'),
        'field' => array(
            'handler' => 'views_handler_field_date',
            'click sortable' => TRUE,
        ),
        'sort' => array(
            'handler' => 'views_handler_sort_date',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_date',
        ),
        'argument' => array(
            'handler' => 'views_handler_filter_date',
        ),
    );


    $data['order_receive_data']['parent_bill_number'] = array(
        'title' => t('Parent Bill No.'),
        'help' => t('The Bill Number.'),
        'field' => array(
            'handler' => 'views_handler_field',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_string',
        ),
    );

    $data['order_receive_data']['child_order_id'] = array(
        'title' => t('Child Order ID'),
        'help' => t('The Bill Number.'),
        'field' => array(
            'handler' => 'views_handler_field',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_string',
        ),
    );

    $data['order_receive_data']['party_code'] = array(
        'title' => t('Party Code'),
        'help' => t('The Party Code.'),
        'field' => array(
            'handler' => 'views_handler_field',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_string',
        ),
    );
    $data['order_receive_data']['parent_party_code'] = array(
        'title' => t('Parent Party Code'),
        'help' => t('The Parent Party Code.'),
        'field' => array(
            'handler' => 'views_handler_field',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_string',
        ),
    );
    $data['order_receive_data']['sku7_code'] = array(
        'title' => t('SKU7 Code'),
        'help' => t('The SKU7 Code.'),
        'field' => array(
            'handler' => 'views_handler_field',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_string',
        ),
    );
    $data['order_receive_data']['return_qty'] = array(
        'title' => t('Return QTY'),
        'help' => t('Return QTY.'),
        'field' => array(
            'handler' => 'views_handler_field_numeric',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_numeric',
        ),
    );
    $data['order_receive_data']['pkd'] = array(
        'title' => t('PKD'),
        'help' => t('PKD'),
        'field' => array(
            'handler' => 'views_handler_field',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_string',
        ),
    );

    $data['order_receive_data']['lb_sku_id'] = array(
        'title' => t('LB SKU ID'),
        'help' => t('LB SKU ID'),
        'field' => array(
            'handler' => 'views_handler_field',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_string',
        ),
    );

    $data['order_receive_data']['product_name'] = array(
        'title' => t('Product Name'),
        'help' => t('Product Name'),
        'field' => array(
            'handler' => 'views_handler_field',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_string',
        ),
    );

    $data['order_receive_data']['mrp'] = array(
        'title' => t('MRP'),
        'help' => t('MRP'),
        'field' => array(
            'handler' => 'views_handler_field',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_string',
        ),
    );


    $data['order_receive_data']['order_type'] = array(
        'title' => t('Order Type'),
        'help' => t('The Order Type.'),
        'field' => array(
            'handler' => 'views_handler_field',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_string',
        ),
    );

    $data['order_receive_data']['created_time'] = array(
        'title' => t('Created Time'),
        'help' => t('created_time'),
        'field' => array(
            'handler' => 'views_handler_field_date',
            'click sortable' => TRUE,
        ),
        'sort' => array(
            'handler' => 'views_handler_sort_date',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_date',
        ),
        'argument' => array(
            'handler' => 'views_handler_filter_date',
        ),
    );
    
    $data['order_receive_data']['beat_name'] = array(
        'title' => t('Beat Name'),
        'help' => t('Beat Name'),
        'field' => array(
            'handler' => 'views_handler_field',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_string',
        ),
    );


    $data['order_receive_data']['flag'] = array(
        'title' => t('Flag'),
        'help' => t('Flag'),
        'field' => array(
            'handler' => 'views_handler_field',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_string',
        ),
    );
    
    
    $data['order_receive_data']['date_filter'] = array(
        'group' => t('Date'),
        'title' => t('Date (!base_table)', array('!base_table' => 'order_receive_data')),
        'help' => t('Filter any Views !base_table date field.', array('!base_table' => 'order_receive_data')),
        'filter' => array(
            'handler' => 'date_views_filter_handler',
            'empty field name' => t('Undated'),
            'is date' => TRUE,
        //'skip base' => $base_table,
        ),
    );
    $data['order_receive_data']['distributor_id'] = array(
        'title' => t('Distributor Id'),
        'help' => t('The Distributor Id.'),
        'field' => array(
            'handler' => 'views_handler_field_numeric',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_numeric',
        ),
        'relationship' => array(
            'title' => t('Distributor ID to Distributor Name'),
            'help' => t("Distributor ID to Distributor Name"),
            'handler' => 'views_handler_relationship',
            'base' => 'users',
            //'entity type' => 'file',
            'base field' => 'uid',
            'label' => t('Distributor Name relation !field_name', array('!field_name' => 'distributor_id')),
        )
    );

    $data['order_receive_data']['type'] = array(
        'title' => t('Type'),
        'help' => t('Type'),
        'field' => array(
            'handler' => 'views_handler_field_numeric',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_numeric',
        ),
    );
    
    $data['order_receive_data']['ean_code'] = array(
        'title' => t('EAN Code'),
        'help' => t('EAN Code'),
        'field' => array(
            'handler' => 'views_handler_field',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_string',
        ),
    );    
        
    $data['order_receive_data']['crate_id'] = array(
        'title' => t('Crate ID'),
        'help' => t('Crate ID'),
        'field' => array(
            'handler' => 'views_handler_field',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_string',
        ),
    );
    
    $data['oms_api_log']['table'] = array(
        'group' => t('OMS API LOG'),
        'title' => t('OMS API LOG'),
        'help' => t('OMS API LOG.'),
    );
    
    // Define this as a base table.
    $data['oms_api_log']['table']['base'] = array(
        'field' => 'id',
        'title' => t('OMS API Log'),
        'help' => t("OMS API Log"),
            // 'weight' => -10,
    );
    
    $data['oms_api_log']['id'] = array(
        'title' => t('ID'),
        'help' => t('The record ID.'),
        'field' => array(
            'handler' => 'views_handler_field_numeric',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_numeric',
        ),
    );

    $data['oms_api_log']['received_data'] = array(
        'title' => t('Received Data'),
        'help' => t('Received Data'),
        'field' => array(
            'handler' => 'views_handler_field',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_string',
        ),
    );

    $data['oms_api_log']['response_data'] = array(
        'title' => t('Response Data'),
        'help' => t('Response Data'),
        'field' => array(
            'handler' => 'views_handler_field',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_string',
        ),
    );

    $data['oms_api_log']['request_time'] = array(
        'title' => t('Request Time'),
        'help' => t('Request Time'),
        'field' => array(
            'handler' => 'views_handler_field_date',
            'click sortable' => TRUE,
        ),
        'sort' => array(
            'handler' => 'views_handler_sort_date',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_date',
        ),
        'argument' => array(
            'handler' => 'views_handler_filter_date',
        ),
    );    

    $data['oms_api_log']['date_filter'] = array(
        'group' => t('Date'),
        'title' => t('Date (!base_table)', array('!base_table' => 'oms_api_log')),
        'help' => t('Filter any Views !base_table date field.', array('!base_table' => 'oms_api_log')),
        'filter' => array(
            'handler' => 'date_views_filter_handler',
            'empty field name' => t('Undated'),
            'is date' => TRUE,
        //'skip base' => $base_table,
        ),
    );    

    return $data;
}
