<?php

/*
  Implementation in Hook View Data
 * mrc_live_tracking
 */

function parkstop_calculation_views_data() {

  $data = array();

  $data['parkstop_api_log']['table'] = array(
    'group' => t('parkstop_api_log data'),
    'title' => t('parkstop_api_log data'),
    'help' => t('parkstop_api_log data.'),
  );

  // Define this as a base table.
  $data['parkstop_api_log']['table']['base'] = array(
    'field' => 'id',
    'title' => t('parkstop_api_log'),
    'help' => t("unique identifiers."),
  );

  $data['parkstop_api_log']['id'] = array(
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

  $data['parkstop_api_log']['prevan'] = array(
    'title' => t('prevan'),
    'help' => t('prevan'),
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
  $data['parkstop_api_log']['trip_id'] = array(
    'title' => t('trip_id'),
    'help' => t('trip_id'),
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
  $data['parkstop_api_log']['api_request'] = array(
    'title' => t('api_request'),
    'help' => t('api_request'),
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
  $data['parkstop_api_log']['api_response'] = array(
    'title' => t('api_response'),
    'help' => t('api_response'),
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
  
  $data['parkstop_api_log']['created_time'] = array(
    'title' => t('created_time'),
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

  $data['parkstop_api_log']['distributor_id'] = array(
    'title' => t('distributor_id'),
    'help' => t('distributor_id'),
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
  $parkstop_api_log = 'parkstop_api_log';
  $data[$parkstop_api_log]['date_filter'] = array(
    'group' => t('Date'),
    'title' => t('Date (!base_table)', array('!base_table' => $parkstop_api_log)),
    'help' => t('Filter any Views !base_table date field.', array('!base_table' => $parkstop_api_log)),
    'filter' => array(
      'handler' => 'date_views_filter_handler',
      'empty field name' => t('Undated'),
      'is date' => TRUE,
    ),
  );
  return $data;
}
