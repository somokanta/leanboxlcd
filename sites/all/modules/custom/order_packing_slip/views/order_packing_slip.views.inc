<?php

function order_packing_slip_views_data() {
  $data['order_packing_slip_api_data']['table'] = array(
    'group' => t('Order Packing Slip Api Data'),
    'title' => t('Order Packing Slip Api Data'),
    'help' => t('Order Packing Slip Api Data'),
  );
  // Define this as a base table.
  $data['order_packing_slip_api_data']['table']['base'] = array(
    'field' => 'id',
    'title' => t('Order Packing Slip Api Data'),
    'help' => t("Order Packing Slip Api Data."),
    // 'weight' => -10,
  );

  $data['order_packing_slip_api_data']['id'] = array(
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

  $data['order_packing_slip_api_data']['order_no'] = array(
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

  $data['order_packing_slip_api_data']['order_date'] = array(
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

  $data['order_packing_slip_api_data']['party_name'] = array(
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
  $data['order_packing_slip_api_data']['box_id'] = array(
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

  $data['order_packing_slip_api_data']['seal_number'] = array(
    'title' => t('Seak Number Value'),
    'help' => t('The Seal Number Value.'),
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

  $data['order_packing_slip_api_data']['ptl_id'] = array(
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

  $data['order_packing_slip_api_data']['total_qty'] = array(
    'title' => t('Total Qty Value'),
    'help' => t('The Total Qty Value.'),
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

  $data['order_packing_slip_api_data']['header_data'] = array(
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

  $data['order_packing_slip_api_data']['row_data'] = array(
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

  $data['order_packing_slip_api_data']['facility_code'] = array(
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

  $data['order_packing_slip_api_data']['created_time'] = array(
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
  $data['order_packing_slip_api_data']['date_filter'] = array(
    'group' => t('Date'),
    'title' => t('Date (!base_table)', array('!base_table' => 'order_packing_slip_api_data')),
    'help' => t('Filter any Views !base_table date field.', array('!base_table' => 'order_packing_slip_api_data')),
    'filter' => array(
      'handler' => 'date_views_filter_handler',
      'empty field name' => t('Undated'),
      'is date' => TRUE,
    ),
  );
  $data['order_packing_slip_api_data']['distributor_id'] = array(
    'title' => t('distributor_id'),
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

  $data['order_packing_slip_api_data']['request_time'] = array(
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
  return $data;
}
