<?php

function custom_order_box_id_views_data() {
   $data['outlet_order_crate']['table'] = array(
     'group' => t('Custom Order Box Id'),
     'title' => t('Outlet Order Box Id'),
     'help' => t('Outlet Order Box Id.'),
   );
      // Define this as a base table.
   $data['outlet_order_crate']['table']['base'] = array(
     'field' => 'id',
     'title' => t('Custom Order Box Id'),
     'help' => t("Custom Order Box Id unique identifiers."),
       // 'weight' => -10,
   );
   
  $data['outlet_order_crate']['id'] = array(
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

  $data['outlet_order_crate']['outlet_order_no'] = array(
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
  $data['outlet_order_crate']['crate_id'] = array(
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
  $data['outlet_order_crate']['order_date'] = array(
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
  $data['outlet_order_crate']['party_name'] = array(
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
  $data['outlet_order_crate']['facility_code'] = array(
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
  
  $data['outlet_order_crate']['created_time'] = array(
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
   $data['outlet_order_crate']['date_filter'] = array(
     'group' => t('Date'),
     'title' => t('Date (!base_table)', array('!base_table' => 'outlet_order_crate')),
     'help' => t('Filter any Views !base_table date field.', array('!base_table' => 'outlet_order_crate')),
     'filter' => array(
       'handler' => 'date_views_filter_handler',
       'empty field name' => t('Undated'),
       'is date' => TRUE,
     //'skip base' => $base_table,
     ),
   );
   $data['outlet_order_crate']['distributor_id'] = array(
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
    $data['outlet_order_crate']['party_code'] = array(
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
    
  $data['outlet_order_crate']['hul_code'] = array(
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
  $data['outlet_order_crate']['order_upload_datetime'] = array(
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
