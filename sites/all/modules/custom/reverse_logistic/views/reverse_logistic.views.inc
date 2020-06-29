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
      'handler' => 'views_handler_filter_string',
    ),
  );

  $data[' reverse_order_data']['outlet_order_no'] = array(
    'title' => t('Outlet Order Number'),
    'help' => t('The Outlet Order Number.'),
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
  $data[' reverse_order_data']['crate_id'] = array(
    'title' => t('Crate Id Value'),
    'help' => t('The Crate Id Value.'),
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
  $data[' reverse_order_data']['order_date'] = array(
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
  $data[' reverse_order_data']['party_name'] = array(
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
  $data[' reverse_order_data']['facility_code'] = array(
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
    $data[' reverse_order_data']['party_code'] = array(
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
    
  $data[' reverse_order_data']['hul_code'] = array(
    'title' => t('Hul Code Value'),
    'help' => t('The Hul Code Value.'),
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
  $data[' reverse_order_data']['order_upload_datetime'] = array(
  'title' => t('Order Upload Datetime'),
  'help' => t('Order Upload Datetime.'),
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
