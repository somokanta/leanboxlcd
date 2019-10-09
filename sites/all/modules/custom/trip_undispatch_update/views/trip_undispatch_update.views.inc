<?php

/**
 * Implements hook_views_data().
 */
function trip_undispatch_update_views_data() {
  $data = array();
  
  $data['delivery_app_update_undispatch_api_log']['table']['group'] = t('Trip Update/Undispatch Log');
  $data['delivery_app_update_undispatch_api_log']['table']['base'] = array(
    'title' => t('Trip Update/Undispatch Log'),
    'help' => t('Trip Update/Undispatch Log'), 
  );
  
  $data['delivery_app_update_undispatch_api_log']['id'] = array(
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

  $data['delivery_app_update_undispatch_api_log']['trip_id'] = array(
    'title' => t('Trip Id'),
    'help' => t('Trip Id'),
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

  $data['delivery_app_update_undispatch_api_log']['request_data'] = array(
    'title' => t('Request Data'),
    'help' => t('Request Data'),
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


  $data['delivery_app_update_undispatch_api_log']['response_data'] = array(
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

  $data['delivery_app_update_undispatch_api_log']['request_time'] = array(
    'title' => t('Request Time'),
    'help' => t('Request Time'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
    'argument' => array(
      'handler' => 'views_handler_filter_date',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
  );

  $data['delivery_app_update_undispatch_api_log']['api_type'] = array(
    'title' => t('Api Type'),
    'help' => t('Api Type'),
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

  $data['delivery_app_update_undispatch_api_log']['distributor_id'] = array(
    'title' => t('Distributor ID'),
    'help' => t('Distributor ID'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_arument_numeric',
    ),
  );

  return $data;
}