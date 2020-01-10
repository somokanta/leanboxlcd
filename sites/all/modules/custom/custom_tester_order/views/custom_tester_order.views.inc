<?php

function custom_tester_order_views_data() {
    $data['tester_order_data']['table'] = array(
        'group' => t('Tester Order Data'),
        'title' => t('Tester Order Data'),
        'help' => t('Tester Order data.'),
    );
    // Define this as a base table.
    $data['tester_order_data']['table']['base'] = array(
        'field' => 'id',
        'title' => t('Tester Order Data'),
        'help' => t("Tester Order Data unique identifiers."),
            // 'weight' => -10,
    );
    $data['tester_order_data']['id'] = array(
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
    $data['tester_order_data']['tester_order_number'] = array(
        'title' => t('Tester Order Number'),
        'help' => t('Tester Order Number.'),
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
    $data['tester_order_data']['order_date'] = array(
        'title' => t('Order Date'),
        'help' => t('Order Date'),
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
    $data['tester_order_data']['party_code'] = array(
        'title' => t('Party Code Value'),
        'help' => t('Party Code Value.'),
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
    $data['tester_order_data']['party_name'] = array(
        'title' => t('Party Name'),
        'help' => t('Party Name'),
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
    $data['tester_order_data']['beat_name'] = array(
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
    $data['tester_order_data']['basepack_code'] = array(
        'title' => t('Basepack Code'),
        'help' => t('Basepack Code'),
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
    $data['tester_order_data']['shade_code'] = array(
        'title' => t('Shade Code'),
        'help' => t('Shade Code'),
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
    $data['tester_order_data']['shade_name'] = array(
        'title' => t('Shade Name'),
        'help' => t('Shade Name.'),
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
    $data['tester_order_data']['product_description'] = array(
        'title' => t('Product Description'),
        'help' => t('Product Description.'),
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
    $data['tester_order_data']['tester_qty'] = array(
        'title' => t('Tester QTY'),
        'help' => t('Tester QTY'),
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
    $data['tester_order_data']['seal_number'] = array(
        'title' => t('Seal Number'),
        'help' => t('Seal_number'),
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
    $data['tester_order_data']['box_id'] = array(
        'title' => t('Box Id'),
        'help' => t('Box Id'),
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
    $data['tester_order_data']['distributor_id'] = array(
        'title' => t('Distributor Id'),
        'help' => t('Distributor Id'),
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
    $data['tester_order_data']['facility_code'] = array(
        'title' => t('Facility Code'),
        'help' => t('Facility Code'),
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
    $data['tester_order_data']['created_time'] = array(
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


    $data['tester_order_data']['date_filter'] = array(
        'group' => t('Date'),
        'title' => t('Date (!base_table)', array('!base_table' => 'tester_order_data')),
        'help' => t('Filter any Views !base_table date field.', array('!base_table' => 'tester_order_data')),
        'filter' => array(
            'handler' => 'date_views_filter_handler',
            'empty field name' => t('Undated'),
            'is date' => TRUE,
        //'skip base' => $base_table,
        ),
    );

    return $data;
}
