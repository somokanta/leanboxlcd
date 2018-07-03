<?php

/*
  Implementation in Hook View Data
 * dispatch_planning Logs
 */

function dispatch_planning_views_data() {

  $data = array();
  
   // Start of MASTER-PTL_PUT_QTY
  $data['ptl_put_qty']['table'] = array(
    'group' => t('ptl_put_qty data'),
    'title' => t('ptl_put_qty data'),
    'help' => t('ptl_put_qty data.'),
  );

  // Define this as a base table.
  $data['ptl_put_qty']['table']['base'] = array(
    'field' => 'id',
    'title' => t('ptl_put_qty'),
    'help' => t("unique identifiers."),
    // 'weight' => -10,
  );

  $data['ptl_put_qty']['id'] = array(
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
  
  // ptl_job ID
  $data['ptl_put_qty']['ptl_job'] = array(
    'title' => t('ptl_job'),
    'help' => t('ptl_job'),
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
  $base_table = 'ptl_put_qty';
  $data[$base_table]['date_filter'] = array(
    'group' => t('Date'),
    'title' => t('Date (!base_table)', array('!base_table' => $base_table)),
    'help' => t('Filter any Views !base_table date field.', array('!base_table' => $base_table)),
    'filter' => array(
      'handler' => 'date_views_filter_handler',
      'empty field name' => t('Undated'),
      'is date' => TRUE,
    //'skip base' => $base_table,
    ),
  );
  
  $data['ptl_put_qty']['line_count'] = array(
    'title' => t('line_count'),
    'help' => t('line_count'),
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
  
  $data['ptl_put_qty']['put_qty_sum'] = array(
    'title' => t('put_qty_sum'),
    'help' => t('put_qty_sum'),
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
  
  $data['ptl_put_qty']['rs_code'] = array(
    'title' => t('rs_code'),
    'help' => t('rs_code'),
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
  
  $data['ptl_put_qty']['created'] = array(
    'title' => t('created'),
    'help' => t('created'),
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
  
  
  $data['ptl_put_qty']['type'] = array(
    'title' => t('type'),
    'help' => t('type'),
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

  
  $data['ptl_put_qty']['unique_id'] = array(
    'title' => t('Unique ID'),
    'help' => t('Unique ID'),
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
  
  $data['ptl_put_qty']['distributor_id'] = array(
    'title' => t('Distributor Id'),
    'help' => t('Distributor Id'),
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
  // end of MASTER-PTL_PUT_QTY
  

  // Add custom field
  $data['views']['field_van_loading_date'] = array(
    'title' => t('van_loading_date'),
    'help' => t('Provides a custom van_loading_date field.'),
    'field' => array(
      'handler' => 'field_van_loading_date',
    ),
  );
  
   // Add custom field for box id in master ptl
  $data['views']['field_box_id_master_ptl'] = array(
    'title' => t('box_id_master_ptl'),
    'help' => t('Provides a custom box id field for master ptl.'),
    'field' => array(
      'handler' => 'field_box_id_master_ptl',
    ),
  );
  
   // Add custom field for box id in master ptl
  $data['views']['field_res_status'] = array(
    'title' => t('field_res_status'),
    'help' => t('Provides a custom res status for billvs trip.'),
    'field' => array(
      'handler' => 'field_res_status',
    ),
  );
  
   // Add custom field for box id in ptl for drs
  $data['views']['field_box_id_ptl'] = array(
    'title' => t('box_id_ptl'),
    'help' => t('Provides a custom box id field for ptl.'),
    'field' => array(
      'handler' => 'field_box_id_ptl',
    ),
  );
  
  // Add custom field for count of box id in ptl for drs
  $data['views']['field_box_id_ptl_count'] = array(
    'title' => t('box_id_ptl_count'),
    'help' => t('Provides count for custom box id field for ptl.'),
    'field' => array(
      'handler' => 'field_box_id_ptl_count',
    ),
  );
  
  // Add custom field for count of box id in master ptl
  $data['views']['field_box_id_master_ptl_count'] = array(
    'title' => t('box_id_master_ptl_count'),
    'help' => t('Provides count for custom box id field for master ptl.'),
    'field' => array(
      'handler' => 'field_box_id_master_ptl_count',
    ),
  );
  
    // Add custom field for count of box id in master ptl
  $data['views']['dispatch_planning_inventory_return_qty'] = array(
    'title' => t('dispatch_planning_inventory_return_qty'),
    'help' => t('Provides return count for dispatch_planning_inventory_return_qty.'),
    'field' => array(
      'handler' => 'dispatch_planning_inventory_return_qty',
    ),
  );
  
  // Add footer for count of box id in master ptl
  $data['views']['footer_master_ptl_custom_box_count'] = array(
    'title' => t('Master PTL Custom Footer for Box Count'),
    'help' => t('Just a field created for  Master PTL Custom Footer for Box Count.'),
    'area' => array(
      'handler' => 'footer_master_ptl_custom_box_count',
    ),
  );
  
  // Add footer for count of box id in ptl
  $data['views']['footer_ptl_custom_box_count'] = array(
    'title' => t('PTL Custom Footer for Box Count'),
    'help' => t('Just a field created for PTL Custom Footer for Box Count.'),
    'area' => array(
      'handler' => 'footer_ptl_custom_box_count',
    ),
  );

  $data['views']['header_ptl_loose_custom'] = array(
    'title' => t('PTL Loose Header'),
    'help' => t('Just a field created for PTL Loose Header.'),
    'area' => array(
      'handler' => 'header_ptl_loose_custom_field',
    ),
  );

  $data['views']['footer_ptl_loose_custom'] = array(
    'title' => t('PTL Loose Footer'),
    'help' => t('Just a field created for PTL Loose Footer.'),
    'area' => array(
      'handler' => 'footer_ptl_loose_custom_field',
    ),
  );
  $data['views']['header_ptl_cld_custom'] = array(
    'title' => t('PTL CLD Header'),
    'help' => t('Just a field created for PTL CLD Header.'),
    'area' => array(
      'handler' => 'header_ptl_cld_custom_field',
    ),
  );

  $data['views']['footer_ptl_cld_custom'] = array(
    'title' => t('PTL CLD Footer'),
    'help' => t('Just a field created for PTL CLD Footer.'),
    'area' => array(
      'handler' => 'footer_ptl_cld_custom_field',
    ),
  );

  //Master PTL
  $data['views']['header_master_ptl_loose_custom'] = array(
    'title' => t('Master PTL Loose Header'),
    'help' => t('Just a field created for Master PTL Loose Header.'),
    'area' => array(
      'handler' => 'header_master_ptl_loose_custom_field',
    ),
  );

  $data['views']['footer_master_ptl_loose_custom'] = array(
    'title' => t('Master PTL Loose Footer'),
    'help' => t('Just a field created for Master PTL Loose Footer.'),
    'area' => array(
      'handler' => 'footer_master_ptl_loose_custom_field',
    ),
  );
  $data['views']['header_master_ptl_cld_custom'] = array(
    'title' => t('Master PTL CLD Header'),
    'help' => t('Just a field created for Master PTL CLD Header.'),
    'area' => array(
      'handler' => 'header_master_ptl_cld_custom_field',
    ),
  );

  $data['views']['footer_master_ptl_cld_custom'] = array(
    'title' => t('Master PTL CLD Footer'),
    'help' => t('Just a field created for Master PTL CLD Footer.'),
    'area' => array(
      'handler' => 'footer_master_ptl_cld_custom_field',
    ),
  );

  //Dispatch Panning API Log
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

  $data['dispatch_planning_api_log']['distributor_id'] = array(
    'title' => t('Distributor id in dispatch api'),
    'help' => t('Distributor id in dispatch api'),
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
  $data['dispatch_planning_api_log']['distributor_id']['relationship'] = array(
    'title' => t('Rs relation in dispatch api'),
    'help' => t("Rs relation in dispatch api"),
    'handler' => 'views_handler_relationship',
    'base' => 'users',
    //'entity type' => 'file',
    'base field' => 'uid',
    'label' => t('Rs relation !field_name', array('!field_name' => 'distributor_id')),
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

  // Adding custom handler for PTL Report data
  $data['ptl_data_configuration']['table'] = array(
    'group' => t('PTL data configuration'),
    'title' => t('PTL data configuration'),
    'help' => t('PTL data configuration.'),
  );

  // Define this as a base table.
  $data['ptl_data_configuration']['table']['base'] = array(
    'field' => 'id',
    'title' => t('PTL data configuration'),
    'help' => t("unique identifiers."),
    // 'weight' => -10,
  );
  // serial No
  $data['ptl_data_configuration']['id'] = array(
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
  // ptl_job
  $data['ptl_data_configuration']['ptl_job'] = array(
    'title' => t('ptl_job'),
    'help' => t('Ptl Job'),
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
  // 	rs_code
  $data['ptl_data_configuration']['rs_code'] = array(
    'title' => t('RS Code'),
    'help' => t('RS Code'),
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
  // 	trip_id
  $data['ptl_data_configuration']['trip_id'] = array(
    'title' => t('trip_id'),
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
  // vehicle
  $data['ptl_data_configuration']['vehicle'] = array(
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
  // bill_number
  $data['ptl_data_configuration']['bill_number'] = array(
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
    'relationship' => array(
      'title' => t('PTL Bill reference.'),
      'help' => t("PTL Bill reference."),
      'handler' => 'views_handler_relationship',
      'base' => 'node',
      'base field' => 'title',
        'extra' => array(
        array('field' => 'type', 'value' => 'sales_register_data'),
        array('field' => 'uid', 'left_field' => 'distributor_id'),
      ),
    ),
  );
  // loading_date
  $data['ptl_data_configuration']['loading_date'] = array(
    'title' => t('loading_date'),
    'help' => t('loading_date'),
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
  // sequence_no
  $data['ptl_data_configuration']['sequence_no'] = array(
    'title' => t('sequence_no'),
    'help' => t('sequence_no'),
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
  // basepack_code
  $data['ptl_data_configuration']['basepack_code'] = array(
    'title' => t('basepack_code'),
    'help' => t('basepack_code'),
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
  // sku_code
  $data['ptl_data_configuration']['sku_code'] = array(
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
    'relationship' => array(
      'title' => t('PTL Sku reference.'),
      'help' => t("PTL Sku reference."),
      'handler' => 'views_handler_relationship',
      'base' => 'node',
      'base field' => 'title',
        'extra' => array(
        array('field' => 'type', 'value' => 'sku7_code_master'),
        array('field' => 'uid', 'left_field' => 'distributor_id'),
      ),
    ),
  );
  // fieldExecutive
  $data['ptl_data_configuration']['fieldExecutive'] = array(
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
  // product_name
  $data['ptl_data_configuration']['product_name'] = array(
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
  // MRP
  $data['ptl_data_configuration']['MRP'] = array(
    'title' => t('MRP'),
    'help' => t('MRP'),
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
  // TUR
  $data['ptl_data_configuration']['TUR'] = array(
    'title' => t('TUR'),
    'help' => t('TUR'),
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
  // sum_sales_qty
  $data['ptl_data_configuration']['sum_sales_qty'] = array(
    'title' => t('sum_sales_qty'),
    'help' => t('sum_sales_qty'),
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
  // sum_sales_qty
  $data['ptl_data_configuration']['fetch_time'] = array(
    'title' => t('Fetch Time'),
    'help' => t('Fetch Time'),
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
  // sum_sales_qty
  $data['ptl_data_configuration']['update_time'] = array(
    'title' => t('Update Time'),
    'help' => t('Update Time'),
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
  // sum_sales_qty
  $data['ptl_data_configuration']['api_flag'] = array(
    'title' => t('API Flag'),
    'help' => t('API Flag'),
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
  // created_time
  $data['ptl_data_configuration']['created_time'] = array(
    'title' => t('Created Time'),
    'help' => t('Created Time'),
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
  // cld
  $data['ptl_data_configuration']['cld'] = array(
    'title' => t('CLD'),
    'help' => t('CLD'),
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
  // loose
  $data['ptl_data_configuration']['loose'] = array(
    'title' => t('Loose'),
    'help' => t('Loose'),
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
  // UPC
  $data['ptl_data_configuration']['upc'] = array(
    'title' => t('UPC'),
    'help' => t('UPC'),
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
  // Put Qty
  $data['ptl_data_configuration']['put_qty'] = array(
    'title' => t('Put Qty'),
    'help' => t('Put Qty'),
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
  // put_qty_time
  $data['ptl_data_configuration']['put_qty_time'] = array(
    'title' => t('Put Qty Time'),
    'help' => t('Put Qty Time'),
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
  $data['ptl_data_configuration']['sum_of_sales_free_qty'] = array(
    'title' => t('Sum of Sales Free QTY'),
    'help' => t('Sum of Sales Free QTY'),
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
  $data['ptl_data_configuration']['free_sku_description'] = array(
    'title' => t('FREE SKU DESCRIPTION'),
    'help' => t('FREE SKU DESCRIPTION'),
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
  $data['ptl_data_configuration']['cp'] = array(
    'title' => t('CP'),
    'help' => t('CP'),
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
  // cld
  $data['ptl_data_configuration']['free_cld'] = array(
    'title' => t('Free CLD'),
    'help' => t('Free CLD'),
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
  // loose
  $data['ptl_data_configuration']['free_loose'] = array(
    'title' => t('Free Loose'),
    'help' => t('Free Loose'),
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
  // drived free qty
  $data['ptl_data_configuration']['derived_free_qty'] = array(
    'title' => t('Derived Free QTY'),
    'help' => t('Derived Free QTY'),
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
  // drived loose
  $data['ptl_data_configuration']['derived_loose'] = array(
    'title' => t('Derived Loose'),
    'help' => t('Derived Loose'),
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
  // drived CLD
  $data['ptl_data_configuration']['derived_cld'] = array(
    'title' => t('Derived CLD'),
    'help' => t('Derived CLD'),
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
  // drived UPC
  $data['ptl_data_configuration']['derived_upc'] = array(
    'title' => t('Derived UPC'),
    'help' => t('Derived UPC'),
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
  // drived EAN
  $data['ptl_data_configuration']['ean'] = array(
    'title' => t('EAN'),
    'help' => t('EAN'),
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
  // drived EAN
  $data['ptl_data_configuration']['ean1'] = array(
    'title' => t('EAN1'),
    'help' => t('EAN1'),
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
  // drived EAN
  $data['ptl_data_configuration']['ean2'] = array(
    'title' => t('EAN2'),
    'help' => t('EAN2'),
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
  // drived EAN
  $data['ptl_data_configuration']['ean3'] = array(
    'title' => t('EAN3'),
    'help' => t('EAN3'),
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
  $data['ptl_data_configuration']['put_operator_id'] = array(
    'title' => t('Put Operator ID'),
    'help' => t('Put Operator ID'),
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
  // put_qty_time
  $data['ptl_data_configuration']['put_time'] = array(
    'title' => t('Put Time'),
    'help' => t('Put Time'),
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
  // Put Qty
  $data['ptl_data_configuration']['close_operator_id'] = array(
    'title' => t('Close Operator ID'),
    'help' => t('Close Operator ID'),
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
  // put_qty_time
  $data['ptl_data_configuration']['close_time'] = array(
    'title' => t('Close Time'),
    'help' => t('Close Time'),
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
  $data['ptl_data_configuration']['box_id'] = array(
    'title' => t('Box ID'),
    'help' => t('Box ID'),
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
  $data['ptl_data_configuration']['zone'] = array(
    'title' => t('Zone'),
    'help' => t('Zone'),
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
  
  $data['ptl_data_configuration']['distributor_id'] = array(
    'title' => t('Distributor Id'),
    'help' => t('Distributor Id'),
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
  
  $base_table = 'ptl_data_configuration';
  $data[$base_table]['date_filter'] = array(
    'group' => t('Date'),
    'title' => t('Date (!base_table)', array('!base_table' => $base_table)),
    'help' => t('Filter any Views !base_table date field.', array('!base_table' => $base_table)),
    'filter' => array(
      'handler' => 'date_views_filter_handler',
      'empty field name' => t('Undated'),
      'is date' => TRUE,
    //'skip base' => $base_table,
    ),
  );

  // Adding custom handler for Master PTL Report data
  $data['master_ptl_data']['table'] = array(
    'group' => t('Master PTL data configuration'),
    'title' => t('Master PTL data configuration'),
    'help' => t('Master PTL data configuration.'),
  );

  // Define this as a base table.
  $data['master_ptl_data']['table']['base'] = array(
    'field' => 'id',
    'title' => t('Master PTL data configuration'),
    'help' => t("unique identifiers."),
    // 'weight' => -10,
  );
  // serial No
  $data['master_ptl_data']['id'] = array(
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
  // ptl_job
  $data['master_ptl_data']['ptl_job'] = array(
    'title' => t('ptl_job'),
    'help' => t('Ptl Job'),
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
  // 	rs_code
  $data['master_ptl_data']['rs_code'] = array(
    'title' => t('RS Code'),
    'help' => t('RS Code'),
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
  // 	trip_id
  $data['master_ptl_data']['trip_id'] = array(
    'title' => t('trip_id'),
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
  // vehicle
  $data['master_ptl_data']['vehicle'] = array(
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
  // bill_number
  $data['master_ptl_data']['bill_number'] = array(
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
  // loading_date
  $data['master_ptl_data']['loading_date'] = array(
    'title' => t('loading_date'),
    'help' => t('loading_date'),
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
  // sequence_no
  $data['master_ptl_data']['sequence_no'] = array(
    'title' => t('sequence_no'),
    'help' => t('sequence_no'),
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
  // basepack_code
  $data['master_ptl_data']['basepack_code'] = array(
    'title' => t('basepack_code'),
    'help' => t('basepack_code'),
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
  // sku_code
  $data['master_ptl_data']['sku_code'] = array(
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
    'relationship' => array(
      'title' => t('PTL Sku reference.'),
      'help' => t("PTL Sku reference."),
      'handler' => 'views_handler_relationship',
      'base' => 'node',
      'base field' => 'title',
        'extra' => array(
        array('field' => 'type', 'value' => 'sku7_code_master'),
        array('field' => 'uid', 'left_field' => 'distributor_id'),
      ),
    ),
  );
  // fieldExecutive
  $data['master_ptl_data']['fieldExecutive'] = array(
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
  // product_name
  $data['master_ptl_data']['product_name'] = array(
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
  // MRP
  $data['master_ptl_data']['MRP'] = array(
    'title' => t('MRP'),
    'help' => t('MRP'),
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
  // TUR
  $data['master_ptl_data']['TUR'] = array(
    'title' => t('TUR'),
    'help' => t('TUR'),
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
  // sum_sales_qty
  $data['master_ptl_data']['sum_sales_qty'] = array(
    'title' => t('sum_sales_qty'),
    'help' => t('sum_sales_qty'),
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
  // sum_sales_qty
  $data['master_ptl_data']['fetch_time'] = array(
    'title' => t('Fetch Time'),
    'help' => t('Fetch Time'),
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
  // sum_sales_qty
  $data['master_ptl_data']['update_time'] = array(
    'title' => t('Update Time'),
    'help' => t('Update Time'),
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
  // sum_sales_qty
  $data['master_ptl_data']['api_flag'] = array(
    'title' => t('API Flag'),
    'help' => t('API Flag'),
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
  // created_time
  $data['master_ptl_data']['created_time'] = array(
    'title' => t('Created Time'),
    'help' => t('Created Time'),
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
  // cld
  $data['master_ptl_data']['cld'] = array(
    'title' => t('CLD'),
    'help' => t('CLD'),
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
  // loose
  $data['master_ptl_data']['loose'] = array(
    'title' => t('Loose'),
    'help' => t('Loose'),
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
  // UPC
  $data['master_ptl_data']['upc'] = array(
    'title' => t('UPC'),
    'help' => t('UPC'),
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
  // Put Qty
  $data['master_ptl_data']['put_qty'] = array(
    'title' => t('Put Qty'),
    'help' => t('Put Qty'),
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
  // put_qty_time
  $data['master_ptl_data']['put_qty_time'] = array(
    'title' => t('Put Qty Time'),
    'help' => t('Put Qty Time'),
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
  $data['master_ptl_data']['sum_of_sales_free_qty'] = array(
    'title' => t('Sum of Sales Free QTY'),
    'help' => t('Sum of Sales Free QTY'),
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
  $data['master_ptl_data']['free_sku_description'] = array(
    'title' => t('FREE SKU DESCRIPTION'),
    'help' => t('FREE SKU DESCRIPTION'),
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
  $data['master_ptl_data']['cp'] = array(
    'title' => t('CP'),
    'help' => t('CP'),
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
  // cld
  $data['master_ptl_data']['free_cld'] = array(
    'title' => t('Free CLD'),
    'help' => t('Free CLD'),
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
  // loose
  $data['master_ptl_data']['free_loose'] = array(
    'title' => t('Free Loose'),
    'help' => t('Free Loose'),
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
  // drived free qty
  $data['master_ptl_data']['derived_free_qty'] = array(
    'title' => t('Derived Free QTY'),
    'help' => t('Derived Free QTY'),
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
  // drived loose
  $data['master_ptl_data']['derived_loose'] = array(
    'title' => t('Derived Loose'),
    'help' => t('Derived Loose'),
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
  // drived CLD
  $data['master_ptl_data']['derived_cld'] = array(
    'title' => t('Derived CLD'),
    'help' => t('Derived CLD'),
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
  // drived UPC
  $data['master_ptl_data']['derived_upc'] = array(
    'title' => t('Derived UPC'),
    'help' => t('Derived UPC'),
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
  // drived EAN
  $data['master_ptl_data']['ean'] = array(
    'title' => t('EAN'),
    'help' => t('EAN'),
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
  $data['master_ptl_data']['ean1'] = array(
    'title' => t('EAN1'),
    'help' => t('EAN1'),
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
  $data['master_ptl_data']['ean2'] = array(
    'title' => t('EAN2'),
    'help' => t('EAN2'),
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
  $data['master_ptl_data']['ean3'] = array(
    'title' => t('EAN3'),
    'help' => t('EAN3'),
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
  // Put Qty
  $data['master_ptl_data']['put_operator_id'] = array(
    'title' => t('Put Operator ID'),
    'help' => t('Put Operator ID'),
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
  // put_qty_time
  $data['master_ptl_data']['put_time'] = array(
    'title' => t('Put Time'),
    'help' => t('Put Time'),
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
  // Put Qty
  $data['master_ptl_data']['close_operator_id'] = array(
    'title' => t('Close Operator ID'),
    'help' => t('Close Operator ID'),
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
  // put_qty_time
  $data['master_ptl_data']['close_time'] = array(
    'title' => t('Close Time'),
    'help' => t('Close Time'),
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
  $data['master_ptl_data']['box_id'] = array(
    'title' => t('Box ID'),
    'help' => t('Box ID'),
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
    $data['master_ptl_data']['zone'] = array(
    'title' => t('Zone'),
    'help' => t('Zone'),
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
  
  $data['master_ptl_data']['distributor_id'] = array(
    'title' => t('Distributor Id'),
    'help' => t('Distributor Id'),
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

  $base_table = 'master_ptl_data';
  $data[$base_table]['date_filter'] = array(
    'group' => t('Date'),
    'title' => t('Date (!base_table)', array('!base_table' => $base_table)),
    'help' => t('Filter any Views !base_table date field.', array('!base_table' => $base_table)),
    'filter' => array(
      'handler' => 'date_views_filter_handler',
      'empty field name' => t('Undated'),
      'is date' => TRUE,
    //'skip base' => $base_table,
    ),
  );
    
  // Table for farey exposed api
  $data['dispatch_planning_api_data']['table'] = array(
    'group' => t('dispatch_planning data'),
    'title' => t('dispatch_planning data'),
    'help' => t('dispatch_planning data.'),
  );

  // Define this as a base table.
  $data['dispatch_planning_api_data']['table']['base'] = array(
    'field' => 'id',
    'title' => t('Dispatch Planning Api Data'),
    'help' => t("unique identifiers."),
    // 'weight' => -10,
  );
  $data['dispatch_planning_api_data']['id'] = array(
    'title' => t('DP ID'),
    'help' => t('Dispatch Planning Data id'),
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
    'relationship' => array(
      'title' => t('Id to Trip_bill_reference.'),
      'help' => t("Id to Trip_bill_reference in dispatch_planning_item_details."),
      'handler' => 'views_handler_relationship',
      'base' => 'dispatch_planning_item_details',
      'base field' => 'trip_bill_reference',
      'label' => t('Id to !field_name', array('!field_name' => 'trip_bill_reference')),
    ),
  );

  $data['dispatch_planning_api_data']['jobType'] = array(
    'title' => t('DP jobType'),
    'help' => t('Dispatch Planning jobType'),
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

  $data['dispatch_planning_api_data']['referenceNo'] = array(
    'title' => t('DP referenceNo'),
    'help' => t('Dispatch Planning referenceNo'),
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

  $data['dispatch_planning_api_data']['date'] = array(
    'title' => t('DP date'),
    'help' => t('Dispatch Planning date'),
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
  $data['dispatch_planning_api_data']['city'] = array(
    'title' => t('DP city'),
    'help' => t('Dispatch Planning date'),
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

  $data['dispatch_planning_api_data']['hub'] = array(
    'title' => t('DP hub'),
    'help' => t('Dispatch Planning Rs Code'),
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

  $data['dispatch_planning_api_data']['res_reason_for_failure'] = array(
    'title' => t('DP res_reason_for_failure'),
    'help' => t('Dispatch Planning res_reason_for_failure'),
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

  $data['dispatch_planning_api_data']['fieldExecutive'] = array(
    'title' => t('DP fieldExecutive'),
    'help' => t('Dispatch Planning fieldExecutive'),
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

  $data['dispatch_planning_api_data']['fieldExecutiveName'] = array(
    'title' => t('DP fieldExecutiveName'),
    'help' => t('Dispatch Planning fieldExecutiveName'),
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
    'title' => t('DP vehicle'),
    'help' => t('Dispatch Planning vehicle'),
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

  $data['dispatch_planning_api_data']['res_cash_amount'] = array(
    'title' => t('DP res_cash_amount'),
    'help' => t('Dispatch Planning res_cash_amount'),
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
  $data['dispatch_planning_api_data']['res_to_be_collection'] = array(
    'title' => t('DP res_to_be_collection'),
    'help' => t('Dispatch Planning res_to_be_collection'),
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
  $data['dispatch_planning_api_data']['res_signed_bill_partial'] = array(
    'title' => t('DP res_signed_bill_partial'),
    'help' => t('Dispatch Planning res_signed_bill_partial'),
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
  
  $data['dispatch_planning_api_data']['res_signed_bill_partial_edit'] = array(
    'title' => t('DP res_signed_bill_partial_edit'),
    'help' => t('Dispatch Planning res_signed_bill_partial'),
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
    
  $data['dispatch_planning_api_data']['res_signed_bill_delivery_edit'] = array(
    'title' => t('DP res_signed_bill_delivery_edit'),
    'help' => t('Dispatch Planning res_signed_bill_delivery'),
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
    'title' => t('DP party_hll_code'),
    'help' => t('Dispatch Planning party_hll_code'),
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
    'title' => t('DP lat_longlatitude'),
    'help' => t('Dispatch Planning lat_longlatitude'),
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
    'title' => t('DP lat_longlongitude'),
    'help' => t('Dispatch Planning lat_longlongitude'),
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
    'title' => t('DP bill_value'),
    'help' => t('Dispatch Planning bill_value'),
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
    'title' => t('DP bill_date'),
    'help' => t('Dispatch Planning bill_date'),
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

  $data['dispatch_planning_api_data']['locality'] = array(
    'title' => t('DP locality'),
    'help' => t('Dispatch Planning locality'),
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
    'title' => t('DP store_sequence_no'),
    'help' => t('Dispatch Planning store_sequence_no'),
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
    'title' => t('DP party_name'),
    'help' => t('Dispatch Planning party_name'),
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
    'title' => t('DP beat_name'),
    'help' => t('Dispatch Planning beat_name'),
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
    'title' => t('DP party_code'),
    'help' => t('Dispatch Planning party_code'),
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
    'title' => t('DP bill_number'),
    'help' => t('Dispatch Planning bill_number'),
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
    'title' => t('DP runsheet_number'),
    'help' => t('Dispatch Planning Trip Id'),
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
  $data['dispatch_planning_api_data']['distributor_id'] = array(
    'title' => t('DP distributor_id'),
    'help' => t('Dispatch Planning distributor_id'),
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
  $data['dispatch_planning_api_data']['res_runsheetNo'] = array(
    'title' => t('DP res_runsheetNo'),
    'help' => t('Dispatch Planning res_runsheetNo'),
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
  $data['dispatch_planning_api_data']['res_actualAmount'] = array(
    'title' => t('DP res_actualAmount'),
    'help' => t('Dispatch Planning res_actualAmount'),
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
  $data['dispatch_planning_api_data']['res_originalAmount'] = array(
    'title' => t('DP res_originalAmount'),
    'help' => t('Dispatch Planning res_originalAmount'),
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
  $data['dispatch_planning_api_data']['res_moneyTransactionType'] = array(
    'title' => t('DP res_moneyTransactionType'),
    'help' => t('Dispatch Planning res_moneyTransactionType'),
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
  $data['dispatch_planning_api_data']['res_latitude'] = array(
    'title' => t('DP res_latitude'),
    'help' => t('Dispatch Planning res_latitude'),
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
  $data['dispatch_planning_api_data']['res_longitude'] = array(
    'title' => t('DP res_longitude'),
    'help' => t('Dispatch Planning res_longitude'),
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
  $data['dispatch_planning_api_data']['res_jobMasterId'] = array(
    'title' => t('DP res_jobMasterId'),
    'help' => t('Dispatch Planning res_jobMasterId'),
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

  $data['dispatch_planning_api_data']['res_attemptCount'] = array(
    'title' => t('DP res_attemptCount'),
    'help' => t('Dispatch Planning res_attemptCount'),
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
  $data['dispatch_planning_api_data']['res_companyId'] = array(
    'title' => t('DP res_companyId'),
    'help' => t('Dispatch Planning res_companyId'),
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
  $data['dispatch_planning_api_data']['res_jobType'] = array(
    'title' => t('DP res_jobType'),
    'help' => t('Dispatch Planning res_jobType'),
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
  $data['dispatch_planning_api_data']['res_jobId'] = array(
    'title' => t('DP res_jobId'),
    'help' => t('Dispatch Planning res_jobId'),
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
  $data['dispatch_planning_api_data']['res_employeeCode'] = array(
    'title' => t('DP res_employeeCode'),
    'help' => t('Dispatch Planning res_employeeCode'),
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
  $data['dispatch_planning_api_data']['res_status'] = array(
    'title' => t('DP res_status'),
    'help' => t('Dispatch Planning res_status'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'dispatch_planning_bill_status',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  
   $data['dispatch_planning_api_data']['res_delivery_boy'] = array(
    'title' => t('DP res_delivery_boy'),
    'help' => t('Dispatch Planning res_delivery_boy'),
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
  
  $data['views']['total_sales_quantity'] = array(
        'title' => t('Total Sales Quantity'),
        'help' => t('Total Sales Quantity.'),
        'real field' => 'id',
        'field' => array(
            'handler' => 'bill_returns_report',
        ),
    );
  
  //Godown short res_godown_short in item level
    $data['views']['res_godown_short'] = array(
        'title' => t('Godown Short'),
        'help' => t('Godown Short'),
        'real field' => 'id',
        'field' => array(
            'handler' => 'bill_returns_report',
        ),
    );
    //res_cashier_short in item level
    $data['views']['cashier_short'] = array(
        'title' => t('Cashier Short'),
        'help' => t('Cashier Short'),
        'real field' => 'id',
        'field' => array(
            'handler' => 'bill_returns_report',
        ),
    );
    //Dispatched Quantity dispatch_qty in item level
    $data['views']['dispatched_qty'] = array(
        'title' => t('Dispatched Quantity'),
        'help' => t('Dispatched Quantity'),
        'real field' => 'id',
        'field' => array(
            'handler' => 'bill_returns_report',
        ),
    );
    //Returned Qty return_qty in item level
    $data['views']['return_qty'] = array(
        'title' => t('Returned Quantity'),
        'help' => t('Returned Quantity'),
        'real field' => 'id',
        'field' => array(
            'handler' => 'bill_returns_report',
        ),
    );
    //Actual Deliverd Qty .res_sku_actual_quantity1
    $data['views']['actual_delivered_qty'] = array(
        'title' => t('Actual Delivered Quantity'),
        'help' => t('Actual Delivered Quantity'),
        'real field' => 'id',
        'field' => array(
            'handler' => 'bill_returns_report',
        ),
    );
    // Delivery app delivered quantity  
    $data['views']['delivery_app_delivered_qty'] = array(
        'title' => t('Delivery App Delivered Quantity'),
        'help' => t('Delivery App Delivered Quantity'),
        'real field' => 'id',
        'field' => array(
            'handler' => 'bill_returns_report',
        ),
    );
    
  $data['dispatch_planning_api_data']['res_trackHalt'] = array(
    'title' => t('DP res_trackHalt'),
    'help' => t('Dispatch Planning res_trackHalt'),
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
  $data['dispatch_planning_api_data']['res_trackKm'] = array(
    'title' => t('DP res_trackKm'),
    'help' => t('Dispatch Planning res_trackKm'),
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
  $data['dispatch_planning_api_data']['res_trackTransactionTimeSpent'] = array(
    'title' => t('DP res_trackTransactionTimeSpent'),
    'help' => t('Dispatch Planning res_trackTransactionTimeSpent'),
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
  $data['dispatch_planning_api_data']['res_merchantCode'] = array(
    'title' => t('DP res_merchantCode'),
    'help' => t('Dispatch Planning res_merchantCode'),
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
  $data['dispatch_planning_api_data']['res_transactionDate'] = array(
    'title' => t('DP res_transactionDate'),
    'help' => t('Dispatch Planning res_transactionDate'),
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
  $data['dispatch_planning_api_data']['res_erpPushTime'] = array(
    'title' => t('DP res_erpPushTime'),
    'help' => t('Dispatch Planning res_erpPushTime'),
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
  $data['dispatch_planning_api_data']['res_lastTransactionTime'] = array(
    'title' => t('DP res_lastTransactionTime'),
    'help' => t('Dispatch Planning res_lastTransactionTime'),
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
  $data['dispatch_planning_api_data']['res_battery'] = array(
    'title' => t('DP res_battery'),
    'help' => t('Dispatch Planning res_battery'),
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
  $data['dispatch_planning_api_data']['res_confirm_signed_bill_amount'] = array(
    'title' => t('DP res_confirm_signed_bill_amount'),
    'help' => t('Dispatch Planning res_confirm_signed_bill_amount'),
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
  $data['dispatch_planning_api_data']['res_bank_name'] = array(
    'title' => t('DP res_bank_name'),
    'help' => t('Dispatch Planning res_bank_name'),
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
  
  $data['dispatch_planning_api_data']['updated_flag'] = array(
    'title' => t('DP updated_flag'),
    'help' => t('Dispatch Planning updated_flag'),
    'field' => array(
      'handler' => 'dispatch_planning_updated_flag_data',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'dispatch_planning_updated_flag',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  
  $data['dispatch_planning_api_data']['res_photo_of_signed_bill_delivery'] = array(
    'title' => t('DP res_photo_of_signed_bill_delivery'),
    'help' => t('Dispatch Planning res_photo_of_signed_bill_delivery'),
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
  $data['dispatch_planning_api_data']['res_total_amount'] = array(
    'title' => t('DP res_total_amount'),
    'help' => t('Dispatch Planning res_total_amount'),
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
  $data['dispatch_planning_api_data']['res_remarks'] = array(
    'title' => t('DP res_remarks'),
    'help' => t('Dispatch Planning res_remarks'),
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
  $data['dispatch_planning_api_data']['res_cheque_date'] = array(
    'title' => t('DP res_cheque_date'),
    'help' => t('Dispatch Planning res_cheque_date'),
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
  $data['dispatch_planning_api_data']['res_photo_of_cheque'] = array(
    'title' => t('DP res_photo_of_cheque'),
    'help' => t('Dispatch Planning res_photo_of_cheque'),
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
  $data['dispatch_planning_api_data']['res_cheque_amount'] = array(
    'title' => t('DP res_cheque_amount'),
    'help' => t('Dispatch Planning res_cheque_amount'),
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
  $data['dispatch_planning_api_data']['res_money_collect2'] = array(
    'title' => t('DP res_money_collect2'),
    'help' => t('Dispatch Planning res_money_collect2'),
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
  $data['dispatch_planning_api_data']['res_signed_bill_delivery'] = array(
    'title' => t('DP res_signed_bill_delivery'),
    'help' => t('Dispatch Planning res_signed_bill_delivery'),
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
  
  $data['dispatch_planning_api_data']['salesman'] = array(
    'title' => t('DP salesman'),
    'help' => t('Dispatch Planning salesman'),
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
  
  $data['dispatch_planning_api_data']['created_time'] = array(
    'title' => t('DP Created Time'),
    'help' => t('DP Created Time'),
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
   
  $data['dispatch_planning_api_data']['updated_time'] = array(
    'title' => t('DP Updated Time'),
    'help' => t('DP Updated Time'),
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
  
     
  $data['dispatch_planning_api_data']['res_updated_time'] = array(
    'title' => t('DP FAREY Updated Time'),
    'help' => t('DP FAREY Updated Time'),
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
   $data['dispatch_planning_api_data']['res_cheque_date_edit'] = array(
    'title' => t('DP res_cheque_date_edit'),
    'help' => t('Dispatch Planning res_cheque_date'),
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
  $data['dispatch_planning_api_data']['res_photo_of_cheque_edit'] = array(
    'title' => t('DP res_photo_of_cheque_edit'),
    'help' => t('Dispatch Planning res_photo_of_cheque'),
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
  $data['dispatch_planning_api_data']['res_cheque_amount_edit'] = array(
    'title' => t('DP res_cheque_amount_edit'),
    'help' => t('Dispatch Planning res_cheque_amount'),
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
  $data['dispatch_planning_api_data']['res_money_collect2_edit'] = array(
    'title' => t('DP res_money_collect2_edit'),
    'help' => t('Dispatch Planning res_money_collect2'),
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
   $data['dispatch_planning_api_data']['res_confirm_signed_bill_amount_edit'] = array(
    'title' => t('DP res_confirm_signed_bill_amount_edit'),
    'help' => t('Dispatch Planning res_confirm_signed_bill_amount'),
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
  $data['dispatch_planning_api_data']['res_bank_name_edit'] = array(
    'title' => t('DP res_bank_name_edit'),
    'help' => t('Dispatch Planning res_bank_name'),
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
  $data['dispatch_planning_api_data']['res_photo_of_signed_bill_delivery_edit'] = array(
    'title' => t('DP res_photo_of_signed_bill_delivery_edit'),
    'help' => t('Dispatch Planning res_photo_of_signed_bill_delivery'),
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
    $data['dispatch_planning_api_data']['res_cash_amount_edit'] = array(
    'title' => t('DP res_cash_amount_edit'),
    'help' => t('Dispatch Planning res_cash_amount'),
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
  
  $data['dispatch_planning_api_data']['res_cheque_no'] = array(
    'title' => t('DP res_cheque_no'),
    'help' => t('Dispatch Planning res_cheque_no'),
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
   $data['dispatch_planning_api_data']['res_cheque_no_edit'] = array(
    'title' => t('DP res_cheque_no_edit'),
    'help' => t('Dispatch Planning res_cheque_no'),
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
    
  //Dispatch Panning API Log
  $data['dispatch_planning_item_details']['table'] = array(
    'group' => t('dispatch_planning_item_details lines'),
    'title' => t('dispatch_planning_item_details '),
    'help' => t('dispatch_planning_item_details .'),
  );

  // Define this as a base table.
  $data['dispatch_planning_item_details']['table']['base'] = array(
    'field' => 'id',
    'title' => t('dispatch_planning_item_details lines'),
    'help' => t("unique identifiers."),
    // 'weight' => -10,
  );
  $data['dispatch_planning_item_details']['id'] = array(
    'title' => t('DP lines id'),
    'help' => t('Dispatch Planning lines id'),
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
  $data['dispatch_planning_item_details']['tur'] = array(
    'title' => t('DP lines tur'),
    'help' => t('Dispatch Planning lines tur'),
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
    'title' => t('DP lines mrp'),
    'help' => t('Dispatch Planning lines mrp'),
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
    'title' => t('DP lines product_name'),
    'help' => t('Dispatch Planning lines product_name'),
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

  $data['dispatch_planning_item_details']['res_sku_reason'] = array(
    'title' => t('DP lines res_sku_reason'),
    'help' => t('Dispatch Planning lines res_sku_reason'),
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
  $data['dispatch_planning_item_details']['res_godown_short'] = array(
    'title' => t('DP lines res_godown_short'),
    'help' => t('Dispatch Planning lines res_godown_short'),
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
  $data['dispatch_planning_item_details']['res_cashier_short'] = array(
    'title' => t('DP lines res_cashier_short'),
    'help' => t('Dispatch Planning lines res_cashier_short'),
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
    'title' => t('DP lines sum_net_sales_cases'),
    'help' => t('Dispatch Planning lines sum_net_sales_cases'),
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
    'title' => t('DP lines sum_net_sales_quantity'),
    'help' => t('Dispatch Planning lines sum_net_sales_quantity'),
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
    'title' => t('DP lines sku_code'),
    'help' => t('Dispatch Planning lines sku_code'),
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
    'title' => t('DP lines sum_net_sales_kgs'),
    'help' => t('Dispatch Planning lines sum_net_sales_kgs'),
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
    'title' => t('DP lines sum_net_sales_value'),
    'help' => t('Dispatch Planning lines sum_net_sales_value'),
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
    'title' => t('DP lines sku_description'),
    'help' => t('Dispatch Planning lines sku_description'),
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
    'title' => t('DP lines trip_bill_reference'),
    'help' => t('Dispatch Planning lines trip_bill_reference'),
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

  $data['dispatch_planning_item_details']['res_sku_actual_quantity1'] = array(
    'title' => t('DP lines res_sku_actual_quantity1'),
    'help' => t('Dispatch Planning lines res_sku_actual_quantity1'),
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
  
  $data['dispatch_planning_item_details']['farey_delivered_qty'] = array(
    'title' => t('DP lines farey_delivered_qty'),
    'help' => t('Dispatch Planning lines farey_delivered_qty'),
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
    
  $data['dispatch_planning_item_details']['dispatch_qty'] = array(
    'title' => t('DP dispatch_qty'),
    'help' => t('Dispatch Planning lines dispatch_qty'),
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
   $data['dispatch_planning_item_details']['return_qty'] = array(
    'title' => t('DP return_qty'),
    'help' => t('Dispatch Planning lines return_qty'),
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
   
   $data['dispatch_planning_item_details']['line_level_status'] = array(
    'title' => t('DP Line level status'),
    'help' => t('Dispatch Planning lines level status'),
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

  $base_table = 'dispatch_planning_api_data';
  $data[$base_table]['date_filter'] = array(
    'group' => t('Date'),
    'title' => t('Date (!base_table)', array('!base_table' => $base_table)),
    'help' => t('Filter any Views !base_table date field.', array('!base_table' => $base_table)),
    'filter' => array(
      'handler' => 'date_views_filter_handler',
      'empty field name' => t('Undated'),
      'is date' => TRUE,
    ),
  );


  // Adding custom handler for PTL Header Report
  $data['ptl_header_data']['table'] = array(
    'group' => t('PTL Header data'),
    'title' => t('PTL Header data'),
    'help' => t('PTL Header data'),
  );

  // Define this as a base table.
  $data['ptl_header_data']['table']['base'] = array(
    'field' => 'id',
    'title' => t('PTL Header data'),
    'help' => t("unique identifiers."),
    // 'weight' => -10,
  );
  // serial No
  $data['ptl_header_data']['id'] = array(
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
  // ptl_job
  $data['ptl_header_data']['ptl_job'] = array(
    'title' => t('ptl_job'),
    'help' => t('Ptl Job'),
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
  // 	rs_code
  $data['ptl_header_data']['rs_code'] = array(
    'title' => t('RS Code'),
    'help' => t('RS Code'),
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
  // 	trip_id
  $data['ptl_header_data']['trip_id'] = array(
    'title' => t('trip_id'),
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
  // type
  $data['ptl_header_data']['type'] = array(
    'title' => t('Type'),
    'help' => t('Type'),
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
  // vehicle
  $data['ptl_header_data']['vehicle'] = array(
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
  // bill_lines
  $data['ptl_header_data']['bill_lines'] = array(
    'title' => t('Bill Lines'),
    'help' => t('Bill Lines'),
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
  // res_bill_lines
  $data['ptl_header_data']['res_bill_lines'] = array(
    'title' => t('RES Bill Lines'),
    'help' => t('RES Bill Lines'),
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
  // total_qty
  $data['ptl_header_data']['total_qty'] = array(
    'title' => t('Total QTY'),
    'help' => t('Total QTY'),
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
  // res_total_qty
  $data['ptl_header_data']['res_total_qty'] = array(
    'title' => t('RES Total QTY'),
    'help' => t('RES Total QTY'),
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
  // operator_id
  $data['ptl_header_data']['operator_id'] = array(
    'title' => t('Operator Id'),
    'help' => t('Operator Id'),
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
  // start_time
  $data['ptl_header_data']['start_time'] = array(
    'title' => t('Start Time'),
    'help' => t('Start Time'),
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
  // end_time
  $data['ptl_header_data']['end_time'] = array(
    'title' => t('End Time'),
    'help' => t('End Time'),
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
  // created_time
  $data['ptl_header_data']['created_time'] = array(
    'title' => t('Created Time'),
    'help' => t('Created Time'),
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
  // updated_time
  $data['ptl_header_data']['updated_time'] = array(
    'title' => t('Updated Time'),
    'help' => t('Updated Time'),
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
  
  $data['ptl_header_data']['distributor_id'] = array(
    'title' => t('Distributor Id'),
    'help' => t('Distributor Id'),
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


  // VRS TABLE
  //Dispatch Panning API Log
  $data['dispatch_delivered_vrs_details']['table'] = array(
    'group' => t('dispatch_delivered_vrs_details'),
    'title' => t('dispatch_delivered_vrs_details'),
    'help' => t('dispatch_delivered_vrs_details.'),
  );

  // Define this as a base table.
  $data['dispatch_delivered_vrs_details']['table']['base'] = array(
    'field' => 'id',
    'title' => t('dispatch_delivered_vrs_details'),
    'help' => t("unique identifiers."),
    // 'weight' => -10,
  );

  // Trip ID
  $data['dispatch_delivered_vrs_details']['trip_id'] = array(
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

  // ID
  $data['dispatch_delivered_vrs_details']['id'] = array(
    'title' => t('vrs ID'),
    'help' => t('vrs ID'),
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

  $data['dispatch_delivered_vrs_details']['sb_bill_count'] = array(
    'title' => t('sb_bill_count'),
    'help' => t('sb_bill_count'),
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

  $data['dispatch_delivered_vrs_details']['sb_bill_value'] = array(
    'title' => t('sb_bill_value'),
    'help' => t('sb_bill_value'),
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

  $data['dispatch_delivered_vrs_details']['sb_app_value'] = array(
    'title' => t('sb_app_value'),
    'help' => t('sb_app_value'),
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

  $data['dispatch_delivered_vrs_details']['sb_actual_collected'] = array(
    'title' => t('sb_actual_collected'),
    'help' => t('sb_actual_collected'),
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

  $data['dispatch_delivered_vrs_details']['sb_difference'] = array(
    'title' => t('sb_difference'),
    'help' => t('sb_difference'),
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

  $data['dispatch_delivered_vrs_details']['ch_bill_count'] = array(
    'title' => t('ch_bill_count'),
    'help' => t('ch_bill_count'),
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
  $data['dispatch_delivered_vrs_details']['ch_bill_value'] = array(
    'title' => t('ch_bill_value'),
    'help' => t('ch_bill_value'),
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
  $data['dispatch_delivered_vrs_details']['ch_app_value'] = array(
    'title' => t('ch_app_value'),
    'help' => t('ch_app_value'),
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
  $data['dispatch_delivered_vrs_details']['ch_actual_collected'] = array(
    'title' => t('ch_actual_collected'),
    'help' => t('ch_actual_collected'),
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
  $data['dispatch_delivered_vrs_details']['ch_difference'] = array(
    'title' => t('ch_difference'),
    'help' => t('ch_difference'),
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
  $data['dispatch_delivered_vrs_details']['cash_bill_count'] = array(
    'title' => t('cash_bill_count'),
    'help' => t('cash_bill_count'),
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
  $data['dispatch_delivered_vrs_details']['cash_bill_value'] = array(
    'title' => t('cash_bill_value'),
    'help' => t('cash_bill_value'),
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
  $data['dispatch_delivered_vrs_details']['cash_app_value'] = array(
    'title' => t('cash_app_value'),
    'help' => t('cash_app_value'),
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
  $data['dispatch_delivered_vrs_details']['cash_actual_collected'] = array(
    'title' => t('cash_actual_collected'),
    'help' => t('cash_actual_collected'),
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
  $data['dispatch_delivered_vrs_details']['cash_difference'] = array(
    'title' => t('cash_difference'),
    'help' => t('cash_difference'),
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
  $data['dispatch_delivered_vrs_details']['tbr_bill_count'] = array(
    'title' => t('tbr_bill_count'),
    'help' => t('tbr_bill_count'),
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
  $data['dispatch_delivered_vrs_details']['tbr_bill_value'] = array(
    'title' => t('tbr_bill_value'),
    'help' => t('tbr_bill_value'),
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
  $data['dispatch_delivered_vrs_details']['tbr_app_value'] = array(
    'title' => t('tbr_app_value'),
    'help' => t('tbr_app_value'),
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
  $data['dispatch_delivered_vrs_details']['tbr_actual_collected'] = array(
    'title' => t('tbr_actual_collected'),
    'help' => t('tbr_actual_collected'),
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
  $data['dispatch_delivered_vrs_details']['tbr_difference'] = array(
    'title' => t('tbr_difference'),
    'help' => t('tbr_difference'),
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
  $data['dispatch_delivered_vrs_details']['van_bill_value'] = array(
    'title' => t('van_bill_value'),
    'help' => t('van_bill_value'),
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
  $data['dispatch_delivered_vrs_details']['cash_debit'] = array(
    'title' => t('cash_debit'),
    'help' => t('cash_debit'),
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
  $data['dispatch_delivered_vrs_details']['inventory_debit'] = array(
    'title' => t('inventory_debit'),
    'help' => t('inventory_debit'),
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
  $data['dispatch_delivered_vrs_details']['total_debit'] = array(
    'title' => t('total_debit'),
    'help' => t('total_debit'),
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
  $data['dispatch_delivered_vrs_details']['total_bill_count'] = array(
    'title' => t('total_bill_count'),
    'help' => t('total_bill_count'),
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
  $data['dispatch_delivered_vrs_details']['total_bill_value'] = array(
    'title' => t('total_bill_value'),
    'help' => t('total_bill_value'),
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
  $data['dispatch_delivered_vrs_details']['fr_bill_count'] = array(
    'title' => t('fr_bill_count'),
    'help' => t('fr_bill_count'),
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
  $data['dispatch_delivered_vrs_details']['fr_bill_value'] = array(
    'title' => t('fr_bill_value'),
    'help' => t('fr_bill_value'),
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
  $data['dispatch_delivered_vrs_details']['ws_bill_count'] = array(
    'title' => t('ws_bill_count'),
    'help' => t('ws_bill_count'),
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
  $data['dispatch_delivered_vrs_details']['ws_bill_value'] = array(
    'title' => t('ws_bill_value'),
    'help' => t('ws_bill_value'),
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
  $data['dispatch_delivered_vrs_details']['dis_id'] = array(
    'title' => t('vrs dis_id'),
    'help' => t('vrs dis_id'),
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

  // Adding custom handler for Godown Data
  $data['godown_data']['table'] = array(
    'group' => t('Godown data'),
    'title' => t('Godown data'),
    'help' => t('Godown data'),
  );

  // Define this as a base table.
  $data['godown_data']['table']['base'] = array(
    'field' => 'id',
    'title' => t('Godown data'),
    'help' => t("unique identifiers."),
  );

  // serial No
  $data['godown_data']['id'] = array(
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
  $data['godown_data']['trip_id'] = array(
    'title' => t('Trip Id'),
    'help' => t('Trip Id'),
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
  $data['godown_data']['bill_number'] = array(
    'title' => t('Bill Number'),
    'help' => t('Bill Number'),
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
  $data['godown_data']['sku_code'] = array(
    'title' => t('SKU Code'),
    'help' => t('SKU Code'),
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
  $data['godown_data']['batch_code'] = array(
    'title' => t('Batch Code'),
    'help' => t('Batch Code'),
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
  $data['godown_data']['mrp'] = array(
    'title' => t('MRP'),
    'help' => t('MRP'),
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
  $data['godown_data']['sku_desc'] = array(
    'title' => t('SKU Description'),
    'help' => t('SKU Description'),
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
  $data['godown_data']['sum_sales_qty'] = array(
    'title' => t('Sum of Net Sales Quantity'),
    'help' => t('Sum of Net Sales Quantity'),
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
  $data['godown_data']['godown_short'] = array(
    'title' => t('Godown Short'),
    'help' => t('Godown Short'),
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
  
  $data['views']['header_area_full_printout'] = array(
    'title' => t('Header field'),
    'help' => t('Just a field created for header purposes for full printout.'),
    'area' => array(
      'handler' => 'header_handler_full_printout',
    ),
  );
  
  $data['views']['header_area_full_printout_popup'] = array(
    'title' => t('Header field'),
    'help' => t('Just a field created for header purposes for full printout popup.'),
    'area' => array(
      'handler' => 'header_handler_full_printout_popup',
    ),
  );
  
  $data['views']['header_area_partial_printout'] = array(
    'title' => t('Header field'),
    'help' => t('Just a field created for header purposes for partial printout.'),
    'area' => array(
      'handler' => 'header_handler_partial_printout',
    ),
  );
  
  $data['views']['header_area_partial_printout_popup'] = array(
    'title' => t('Header field'),
    'help' => t('Just a field created for header purposes for partial printout popup.'),
    'area' => array(
      'handler' => 'header_handler_partial_printout_popup',
    ),
  );
  
  $data['views']['field_collection_value'] = array(
    'title' => t('Collection value handler'),
    'help' => t('Collection value handler.'),
    'field' => array(
      'handler' => 'field_collection_value',
    ),
  );
  
  $data['views']['field_collection_mode'] = array(
    'title' => t('Collection mode handler'),
    'help' => t('Collection mode handler.'),
    'field' => array(
      'handler' => 'field_collection_mode',
    ),
  );
  
  return $data;
}

/**
 * Field handler to translate a Status into its readable form.
 */
class dispatch_planning_updated_flag_data extends views_handler_field {

  function render($values) {
    //dpm($values);
    $updated_flag = $values->dispatch_planning_api_data_updated_flag;
      $res = '';

      if ($updated_flag == 1) {
         $res = 'Manual';
      }
      if ($updated_flag == 2) {
         $res = 'Api';
      }
    return $res;
  }

}