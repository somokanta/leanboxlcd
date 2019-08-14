<?php

function custom_order_box_id_views_data_alter(&$data) {
  // ======= Custom Order Box Id View Start =======
  $data[' outlet_order_crate']['table']['group'] = t('Custom Order Box Id');
  $data[' outlet_order_crate']['table']['base'] = array(
    'title' => t('Outlet Order Box Id'),
    'help' => t('Outlet Order Box Id'),
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
  return $data;
}
