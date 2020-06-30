<?php

function reverse_logistic_views_data() {
    $data[' reverse_order_data']['table'] = array(
        'group' => t('Reverse Order Data'),
        'title' => t('Reverse Order Data'),
        'help' => t('Reverse Order Data.'),
    );
    // Define this as a base table.
    $data[' reverse_order_data']['table']['base'] = array(
        'field' => 'id',
        'title' => t('Reverse Order Data'),
        'help' => t("Reverse Order Data unique identifiers."),
            // 'weight' => -10,
    );

    $data[' reverse_order_data']['id'] = array(
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

    $data[' reverse_order_data']['transaction_id'] = array(
        'title' => t('Transaction ID'),
        'help' => t('The Transaction ID.'),
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
    $data[' reverse_order_data']['transaction_line_id'] = array(
        'title' => t('Transaction Line ID'),
        'help' => t('The Transaction Line ID.'),
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



    $data[' reverse_order_data']['bill_number'] = array(
        'title' => t('Bill Number'),
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
    $data[' reverse_order_data']['party_code'] = array(
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
    $data[' reverse_order_data']['parent_party_code'] = array(
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
    $data[' reverse_order_data']['sku7_code'] = array(
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
    $data[' reverse_order_data']['to_be_returned_qty'] = array(
        'title' => t('To be Returned QTY'),
        'help' => t('The To be Returned QTY.'),
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
    $data[' reverse_order_data']['pkm'] = array(
        'title' => t('PKM'),
        'help' => t('The PKM.'),
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

    $data[' reverse_order_data']['reverse_order_date'] = array(
        'title' => t('Reverse Order Date'),
        'help' => t('The Reverse Order Date'),
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
    $data[' reverse_order_data']['order_type'] = array(
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

    $data[' reverse_order_data']['created_time'] = array(
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
    $data[' reverse_order_data']['date_filter'] = array(
        'group' => t('Date'),
        'title' => t('Date (!base_table)', array('!base_table' => ' reverse_order_data')),
        'help' => t('Filter any Views !base_table date field.', array('!base_table' => ' reverse_order_data')),
        'filter' => array(
            'handler' => 'date_views_filter_handler',
            'empty field name' => t('Undated'),
            'is date' => TRUE,
        //'skip base' => $base_table,
        ),
    );
    $data[' reverse_order_data']['distributor_id'] = array(
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

    $data[' reverse_order_data']['received_through'] = array(
        'title' => t('Received Through'),
        'help' => t('The Received Through.'),
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

    $data[' reverse_order_data']['approved_qty'] = array(
        'title' => t('Approved qty'),
        'help' => t('The Approved qty.'),
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

    $data[' reverse_order_data']['status'] = array(
        'title' => t('Status'),
        'help' => t('The Status.'),
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

    return $data;
}
