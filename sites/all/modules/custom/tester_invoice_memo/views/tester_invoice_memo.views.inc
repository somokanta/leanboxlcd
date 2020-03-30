<?php

function tester_invoice_memo_views_data() {
    $data['tester_invoice_memo']['table'] = array(
          'group' => t('Tester Invoice Memo Api Data'),
          'title' => t('Tester Invoice Memo Data'),
          'help' => t('Tester Invoice Memo Data'),
      );
    // Define this as a base table.
    $data['tester_invoice_memo']['table']['base'] = array(
          'field' => 'id',
          'title' => t('Tester Invoice Memo Data'),
          'help' => t("Tester Invoice Memo Data."),
              // 'weight' => -10,
      );

    $data['tester_invoice_memo']['id'] = array(
          'title' => t('ID'),
          'help' => t('The record ID.'),
          'field' => array(
              'handler' => 'views_handler_field_numeric',
          ),
          'sort' => array(
              'handler' => 'views_handler_sort',
          ),
          'filter' => array(
              'handler' => 'views_handler_filter_string',
          ),
      );

    $data['tester_invoice_memo']['order_no'] = array(
          'title' => t('Order Number'),
          'help' => t('Order Number.'),
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

    $data['tester_invoice_memo']['order_date'] = array(
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

    $data['tester_invoice_memo']['party_code'] = array(
        'title' => t('Party Code Value'),
        'help' => t('The Party Code Value.'),
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
    $data['tester_invoice_memo']['party_name'] = array(
        'title' => t('Party Name Value'),
        'help' => t('The Party Name Value.'),
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

    $data['tester_invoice_memo']['beat_name'] = array(
        'title' => t('Beat Name Value'),
        'help' => t('The Beat Name Value.'),
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

    $data['tester_invoice_memo']['box_id'] = array(
        'title' => t('Box Id Value'),
        'help' => t('The Box Id Value.'),
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

    $data['tester_invoice_memo']['ptl_id'] = array(
        'title' => t('Ptl Id Value'),
        'help' => t('The Ptl Id Value.'),
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

    $data['tester_invoice_memo']['total_picked_qty'] = array(
        'title' => t('Total Picked Qty Value'),
        'help' => t('The Total Picked Qty Value.'),
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

    $data['tester_invoice_memo']['header_data'] = array(
        'title' => t('Header Data'),
        'help' => t('Header Data'),
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

    $data['tester_invoice_memo']['row_data'] = array(
        'title' => t('Row Data'),
        'help' => t('Row Data'),
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

    $data['tester_invoice_memo']['facility_code'] = array(
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

    $data['tester_invoice_memo']['distributor_id'] = array(
        'title' => t('Distributor ID'),
        'help' => t('The record distributor_id.'),
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

    $data['tester_invoice_memo']['request_time'] = array(
        'title' => t('Datetime'),
        'help' => t(' Datetime.'),
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

    $data['tester_invoice_memo']['date_filter'] = array(
        'group' => t('Date'),
        'title' => t('Date (!base_table)', array('!base_table' => 'tester_invoice_memo')),
        'help' => t('Filter any Views !base_table date field.', array('!base_table' => 'tester_invoice_memo')),
        'filter' => array(
          'handler' => 'date_views_filter_handler',
          'empty field name' => t('Undated'),
          'is date' => TRUE,
        ),
    );  
  return $data;
}

