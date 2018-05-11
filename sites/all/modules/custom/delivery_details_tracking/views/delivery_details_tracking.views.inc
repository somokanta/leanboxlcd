<?php

/*
  Implementation in Hook View Data
 */
function delivery_details_tracking_views_data() {

  $data = array();

  $data['delivery_details']['table'] = array(
    'group' => t('delivery_details data'),
    'title' => t('delivery_details data'),
    'help' => t('delivery_details data.'),
  );

  // Define this as a base table.
  $data['delivery_details']['table']['base'] = array(
    'field' => 'id',
    'title' => t('delivery_details'),
    'help' => t("unique identifiers."),
    // 'weight' => -10,
  );

  $data['delivery_details']['id'] = array(
    'title' => t('ID'),
    'help' => t('ID'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['delivery_details']['dispatch_date'] = array(
    'title' => t('Dispatch Date'),
    'help' => t('Dispatch Date'),
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
  $data['delivery_details']['bill_number'] = array(
    'title' => t('Bill No.'),
    'help' => t('Bill No.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'relationship' => array(
      'title' => t('Delivery details and Sales register reference'),
      'help' => t("Delivery details and Sales register reference"),
      'handler' => 'views_handler_relationship',
      'base' => 'node',
      'base field' => 'title',
        'extra' => array(
        array('field' => 'type', 'value' => 'sales_register_data'),
        array('field' => 'uid', 'left_field' => 'distributor_id'),
      ),
    ),
  );
  $data['delivery_details']['van_number'] = array(
    'title' => t('Van No.'),
    'help' => t('Van No.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['delivery_details']['trip_id'] = array(
    'title' => t('Trip Id'),
    'help' => t('Trip Id'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['delivery_details']['cashier_name'] = array(
    'title' => t('Cashier Name'),
    'help' => t('Cashier Name'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['delivery_details']['delivery_boy_name'] = array(
    'title' => t('Delivery Boy Name'),
    'help' => t('Delivery Boy Name'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['delivery_details']['status'] = array(
    'title' => t('Status'),
    'help' => t('Status'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['delivery_details']['delivery_date'] = array(
    'title' => t('Delivery Date'),
    'help' => t('Delivery Date'),
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
  $data['delivery_details']['comments'] = array(
    'title' => t('Comments'),
    'help' => t('Comments'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['delivery_details']['distributor_id'] = array(
    'title' => t('Distributor Id'),
    'help' => t('Distributor Id'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['delivery_details']['created_time'] = array(
    'title' => t('Created time'),
    'help' => t('Created time'),
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
  $delivery_details = 'delivery_details';
  $data[$delivery_details]['date_filter'] = array(
    'group' => t('Date'),
    'title' => t('Date (!base_table)', array('!base_table' => delivery_details)),
    'help' => t('Filter any Views !base_table date field.', array('!base_table' => $delivery_details)),
    'filter' => array(
      'handler' => 'date_views_filter_handler',
      'empty field name' => t('Undated'),
      'is date' => TRUE,
    //'skip base' => $base_table,
    ),
  );
  return $data;
}
