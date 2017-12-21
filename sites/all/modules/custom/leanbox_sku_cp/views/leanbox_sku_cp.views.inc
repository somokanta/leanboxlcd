<?php

/*
  Implementation in Hook View Data
 * dispatch_planning Logs
 */

function leanbox_sku_cp_views_data() {

   $data = array();
  
   // Adding custom handler for PTL Header Report
   $data['sku_ean_mrp']['table'] = array(
     'group' => t('SKU EAN MRP data'),
     'title' => t('SKU EAN MRP data'),
     'help' => t('SKU EAN MRP data'),
   );

   // Define this as a base table.
   $data['sku_ean_mrp']['table']['base'] = array(
     'field' => 'id',
     'title' => t('SKU EAN MRP data'),
     'help' => t("unique identifiers."),
       // 'weight' => -10,
   );
   // serial No
   $data['sku_ean_mrp']['id'] = array(
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
   // sku
   $data['sku_ean_mrp']['sku'] = array(
     'title' => t('SKU7_Code'),
     'help' => t('SKU7_Code'),
     'field' => array(
       'handler' => 'views_handler_field',
       'click sortable' => TRUE,
     ),
     'sort' => array(
       'handler' => 'views_handler_sort',
     ),
     'filter' => array(
       'handler' => 'views_handler_filter_string',
     ),
     'argument' => array(
       'handler' => 'views_handler_argument_string',
     ),
   );
   // 	ean
   $data['sku_ean_mrp']['ean'] = array(
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
   // 	mrp
   $data['sku_ean_mrp']['mrp'] = array(
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
   // flag
   $data['sku_ean_mrp']['flag'] = array(
     'title' => t('Flag'),
     'help' => t('Flag'),
     'field' => array(
       'handler' => 'views_handler_field',
       'click sortable' => TRUE,
     ),
     'sort' => array(
       'handler' => 'views_handler_sort',
     ),
     'filter' => array(
       'handler' => 'views_handler_filter_string',
     ),
     'argument' => array(
       'handler' => 'views_handler_argument_string',
     ),
   );
   // deleted
   $data['sku_ean_mrp']['deleted'] = array(
     'title' => t('Deleted'),
     'help' => t('Deleted'),
     'field' => array(
       'handler' => 'views_handler_field',
       'click sortable' => TRUE,
     ),
     'sort' => array(
       'handler' => 'views_handler_sort',
     ),
     'filter' => array(
       'handler' => 'views_handler_filter_string',
     ),
     'argument' => array(
       'handler' => 'views_handler_argument_string',
     ),
   );    
   // rs_code
   $data['sku_ean_mrp']['rs_code'] = array(
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
  

   // created_time
   $data['sku_ean_mrp']['created_time'] = array(
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
   // api_fetch_time
   $data['sku_ean_mrp']['api_fetch_time'] = array(
     'title' => t('API Fetch Time'),
     'help' => t('API Fetch Time'),
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
