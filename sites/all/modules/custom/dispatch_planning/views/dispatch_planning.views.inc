<?php

/*
  Implementation in Hook View Data
 * dispatch_planning Logs
 */

function dispatch_planning_views_data() {
  $data['dispatch_planning_api_log']['table'] = array(
    'group' => t('dispatch_planning Logs'),
    'title' => t('dispatch_planning Logs'),
    'help' => t('dispatch_planning Logs.'),
  );

  // Define this as a base table.
  $data['dispatch_planning_api_log']['table']['base'] = array(
    'field' => 'id',
    'title' => t('dispatch_planning_log'),
    'help' => t("unique identifiers."),
    // 'weight' => -10,
  );


  // Order ID
  $data['dispatch_planning_api_log']['trip_id'] = array(
    'title' => t('Trip ID'),
    'help' => t('Trip ID'),
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
  $data['dispatch_planning_api_log']['api_type'] = array(
    'title' => t('API Type'),
    'help' => t('API Type'),
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


  //Request Data of API
  $data['dispatch_planning_api_log']['request_data'] = array(
    'title' => t('Request Data'),
    'help' => t('Request Data'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  //Response Data of API
  $data['dispatch_planning_api_log']['response_data'] = array(
    'title' => t('Response Data'),
    'help' => t('Response Data'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // API Request Time
  $data['dispatch_planning_api_log']['request_time'] = array(
    'title' => t('Request Time'),
    'help' => t('Request Time'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
  );

  return $data;
}
