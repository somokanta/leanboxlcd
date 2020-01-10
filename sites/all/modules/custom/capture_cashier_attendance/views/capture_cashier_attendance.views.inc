<?php

/**
 * Implements hook_views_data().
 */
function capture_cashier_attendance_views_data() {
  $data = array();
  
  $data['cashier_attendance_api_log']['table']['group'] = t('Cashier Attendance Api Log');
  $data['cashier_attendance_api_log']['table']['base'] = array(
    'title' => t('Cashier Attendance Api Log'),
    'help' => t('Cashier Attendance Api Log'), 
  );
  
  $data['cashier_attendance_api_log']['id'] = array(
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

  $data['cashier_attendance_api_log']['received_data'] = array(
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

  $data['cashier_attendance_api_log']['response_flag'] = array(
    'title' => t('Response Flag'),
    'help' => t('Response Flag'),
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

  $data['cashier_attendance_api_log']['request_time'] = array(
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

  $data['cashier_attendance_api_log']['distributor_id'] = array(
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
  
  $data['cashier_attendance_api_log']['facility_code'] = array(
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

  return $data;
}
