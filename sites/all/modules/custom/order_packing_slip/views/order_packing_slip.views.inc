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
  return $data;
}

