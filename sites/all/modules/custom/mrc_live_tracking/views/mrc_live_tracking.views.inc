<?php

/*
  Implementation in Hook View Data
 * mrc_live_tracking
 */

function mrc_live_tracking_views_data() {

  $data = array();

  $data['van_live_tracking']['table'] = array(
    'group' => t('van_live_tracking data'),
    'title' => t('van_live_tracking data'),
    'help' => t('van_live_tracking data.'),
  );

  // Define this as a base table.
  $data['van_live_tracking']['table']['base'] = array(
    'field' => 'id',
    'title' => t('van_live_tracking'),
    'help' => t("unique identifiers."),
  );

  $data['van_live_tracking']['id'] = array(
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

  $data['van_live_tracking']['employeecode'] = array(
    'title' => t('employeecode'),
    'help' => t('employeecode'),
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
  $data['van_live_tracking']['tracktime'] = array(
    'title' => t('tracktime'),
    'help' => t('tracktime'),
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
  $data['van_live_tracking']['latitude'] = array(
    'title' => t('latitude'),
    'help' => t('latitude'),
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
  $data['van_live_tracking']['longitude'] = array(
    'title' => t('longitude'),
    'help' => t('longitude'),
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
  $data['van_live_tracking']['gpssignal'] = array(
    'title' => t('gpssignal'),
    'help' => t('gpssignal'),
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

  $data['van_live_tracking']['speed'] = array(
    'title' => t('speed'),
    'help' => t('speed'),
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
  $data['van_live_tracking']['battery'] = array(
    'title' => t('battery'),
    'help' => t('battery'),
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
  $data['van_live_tracking']['signalaccuracy'] = array(
    'title' => t('signalaccuracy'),
    'help' => t('signalaccuracy'),
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
  $data['van_live_tracking']['altitude'] = array(
    'title' => t('altitude'),
    'help' => t('altitude'),
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
  $data['van_live_tracking']['bearing'] = array(
    'title' => t('bearing'),
    'help' => t('bearing'),
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
  $data['van_live_tracking']['extra1'] = array(
    'title' => t('extra1'),
    'help' => t('extra1'),
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
  $data['van_live_tracking']['extra2'] = array(
    'title' => t('extra2'),
    'help' => t('extra2'),
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
  $data['van_live_tracking']['extra3'] = array(
    'title' => t('extra3'),
    'help' => t('extra3'),
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
  $data['van_live_tracking']['distributor_id'] = array(
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
  $data['van_live_tracking']['created_time'] = array(
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
  $van_live_tracking = 'van_live_tracking';
  $data[$van_live_tracking]['date_filter'] = array(
    'group' => t('Date'),
    'title' => t('Date (!base_table)', array('!base_table' => $van_live_tracking)),
    'help' => t('Filter any Views !base_table date field.', array('!base_table' => $van_live_tracking)),
    'filter' => array(
      'handler' => 'date_views_filter_handler',
      'empty field name' => t('Undated'),
      'is date' => TRUE,
    ),
  );
  return $data;
}
