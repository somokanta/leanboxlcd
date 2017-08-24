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
  //Response Data of API
  $data['dispatch_planning_api_log']['api_success'] = array(
    'title' => t('API Success'),
    'help' => t('API Success'),
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
  //Response Data of API
  $data['dispatch_planning_api_log']['request_flag'] = array(
    'title' => t('Request Flag'),
    'help' => t('Request Flag'),
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
  $data['dispatch_planning_api_data']['table'] = array(
    'group' => t('dispatch_planning_api_data'),
    'title' => t('dispatch_planning_api_data'),
    'help' => t('Dispatch planning api data.'),
  );

  // Define this as a base table.
  $data['dispatch_planning_api_data']['table']['base'] = array(
    'field' => 'id',
    'title' => t('Dispatch planning api data'),
    'help' => t("unique identifiers for api data"),
  );
  $data['dispatch_planning_api_data']['id'] = array(
    'title' => t('Id'),
    'help' => t('Id'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );
  // jobType
  $data['dispatch_planning_api_data']['jobType'] = array(
    'title' => t('jobType'),
    'help' => t('jobType'),
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
  // jobType
  $data['dispatch_planning_api_data']['referenceNo'] = array(
    'title' => t('referenceNo'),
    'help' => t('referenceNo'),
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
  // jobType
  $data['dispatch_planning_api_data']['date'] = array(
    'title' => t('date'),
    'help' => t('date'),
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
  // jobType
  $data['dispatch_planning_api_data']['city'] = array(
    'title' => t('city'),
    'help' => t('city'),
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
  // jobType
  $data['dispatch_planning_api_data']['hub'] = array(
    'title' => t('hub'),
    'help' => t('hub'),
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
  // jobType
  $data['dispatch_planning_api_data']['fieldExecutive'] = array(
    'title' => t('fieldExecutive'),
    'help' => t('fieldExecutive'),
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
  // jobType
  $data['dispatch_planning_api_data']['fieldExecutiveName'] = array(
    'title' => t('fieldExecutiveName'),
    'help' => t('fieldExecutiveName'),
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

  $data['dispatch_planning_api_data']['vehicle'] = array(
    'title' => t('vehicle'),
    'help' => t('vehicle'),
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
  $data['dispatch_planning_api_data']['party_hll_code'] = array(
    'title' => t('party_hll_code'),
    'help' => t('party_hll_code'),
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
  $data['dispatch_planning_api_data']['lat_longlatitude'] = array(
    'title' => t('lat_longlatitude'),
    'help' => t('lat_longlatitude'),
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
  $data['dispatch_planning_api_data']['lat_longlongitude'] = array(
    'title' => t('lat_longlongitude'),
    'help' => t('lat_longlongitude'),
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
  $data['dispatch_planning_api_data']['bill_value'] = array(
    'title' => t('bill_value'),
    'help' => t('bill_value'),
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
  $data['dispatch_planning_api_data']['bill_date'] = array(
    'title' => t('bill_date'),
    'help' => t('bill_date'),
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
  $data['dispatch_planning_api_data']['locality'] = array(
    'title' => t('locality'),
    'help' => t('locality'),
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
  $data['dispatch_planning_api_data']['store_sequence_no'] = array(
    'title' => t('store_sequence_no'),
    'help' => t('store_sequence_no'),
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
  $data['dispatch_planning_api_data']['party_name'] = array(
    'title' => t('party_name'),
    'help' => t('party_name'),
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
  $data['dispatch_planning_api_data']['beat_name'] = array(
    'title' => t('beat_name'),
    'help' => t('beat_name'),
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
  $data['dispatch_planning_api_data']['party_code'] = array(
    'title' => t('party_code'),
    'help' => t('party_code'),
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
  $data['dispatch_planning_api_data']['bill_number'] = array(
    'title' => t('bill_number'),
    'help' => t('bill_number'),
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
  $data['dispatch_planning_api_data']['runsheet_number'] = array(
    'title' => t('runsheet_number'),
    'help' => t('runsheet_number'),
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
  $data['dispatch_planning_item_details']['table'] = array(
    'group' => t('dispatch_planning_item_details'),
    'title' => t('dispatch_planning_item_details'),
    'help' => t('Dispatch planning items details.'),
  );

  // Define this as a base table.
  $data['dispatch_planning_item_details']['table']['base'] = array(
    'field' => 'id',
    'title' => t('Dispatch planning items details'),
    'help' => t("unique identifiers for items data"),
  );
  $data['dispatch_planning_item_details']['tur'] = array(
    'title' => t('tur'),
    'help' => t('tur'),
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
  $data['dispatch_planning_item_details']['mrp'] = array(
    'title' => t('mrp'),
    'help' => t('mrp'),
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
  $data['dispatch_planning_item_details']['product_name'] = array(
    'title' => t('product_name'),
    'help' => t('product_name'),
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
  $data['dispatch_planning_item_details']['sum_net_sales_cases'] = array(
    'title' => t('sum_net_sales_cases'),
    'help' => t('sum_net_sales_cases'),
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
  $data['dispatch_planning_item_details']['sum_net_sales_quantity'] = array(
    'title' => t('sum_net_sales_quantity'),
    'help' => t('sum_net_sales_quantity'),
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
  $data['dispatch_planning_item_details']['sku_code'] = array(
    'title' => t('sku_code'),
    'help' => t('sku_code'),
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
  $data['dispatch_planning_item_details']['sum_net_sales_kgs'] = array(
    'title' => t('sum_net_sales_kgs'),
    'help' => t('sum_net_sales_kgs'),
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
  $data['dispatch_planning_item_details']['sum_net_sales_value'] = array(
    'title' => t('sum_net_sales_value'),
    'help' => t('sum_net_sales_value'),
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
  $data['dispatch_planning_item_details']['sku_description'] = array(
    'title' => t('sku_description'),
    'help' => t('sku_description'),
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
  $data['dispatch_planning_item_details']['trip_bill_reference'] = array(
    'title' => t('Dispatch planning items id'),
    'help' => t('Dispatch planning items id.'),
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
      'base' => 'dispatch_planning_api_data',
      'base field' => 'id',
      'field' => 'trip_bill_reference',
      'handler' => 'views_handler_relationship',
      'label' => t('id'),
    ),
  );
  return $data;
}
