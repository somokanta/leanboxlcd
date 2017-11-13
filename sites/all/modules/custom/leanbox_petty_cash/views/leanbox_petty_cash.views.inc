<?php

/*
  Implementation in Hook View Data
 * dispatch_planning Logs
 */

function leanbox_petty_cash_views_data() {
   $data['petty_cash']['table'] = array(
     'group' => t('petty cash'),
     'title' => t('petty cash'),
     'help' => t('petty cash details.'),
   );

   // Define this as a base table.
   $data['petty_cash']['table']['base'] = array(
     'field' => 'id',
     'title' => t('petty cash'),
     'help' => t("unique identifiers."),
       // 'weight' => -10,
   );


   // Order ID
   $data['petty_cash']['id'] = array(
     'title' => t('Serial ID'),
     'help' => t('Petty cash ID'),
     'field' => array(
       'handler' => 'views_handler_field',
       'click sortable' => TRUE,
     ),
     'sort' => array(
       'handler' => 'views_handler_sort',
     ),
     'filter' => array(
       'handler' => 'views_handler_filter_string',
     ),
     'argument' => array(
       'handler' => 'views_handler_argument_string',
     ),
   );
   $data['petty_cash']['type'] = array(
     'title' => t('Type'),
     'help' => t('Petty cash Type'),
     'field' => array(
       'handler' => 'views_handler_field',
       'click sortable' => TRUE,
     ),
     'sort' => array(
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
   $data['petty_cash']['head'] = array(
     'title' => t('Head'),
     'help' => t('Petty cash head Data'),
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
   $data['petty_cash']['subhead'] = array(
     'title' => t('Subhead'),
     'help' => t('Petty cash subhead Data'),
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
   $data['petty_cash']['description'] = array(
     'title' => t('description'),
     'help' => t('Petty cash description'),
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
   $data['petty_cash']['bill_available'] = array(
     'title' => t('Bill Available'),
     'help' => t('Petty cash bill available flag'),
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
//  $data['petty_cash']['table'] = array(
//    'group' => t('dispatch_planning_api_data'),
//    'title' => t('dispatch_planning_api_data'),
//    'help' => t('Dispatch planning api data.'),
//  );
   // Define this as a base table.
//  $data['petty_cash']['table']['base'] = array(
//    'field' => 'id',
//    'title' => t('Dispatch planning api data'),
//    'help' => t("unique identifiers for api data"),
//  );
   $data['petty_cash']['bill_no'] = array(
     'title' => t('Bill no'),
     'help' => t('Petty cash bill_no'),
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
   // jobType
   $data['petty_cash']['amount'] = array(
     'title' => t('Amount'),
     'help' => t('Petty cash amount'),
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
   // referenceNo
   $data['petty_cash']['person_name'] = array(
     'title' => t('Person name'),
     'help' => t('Petty cash person name'),
     'field' => array(
       'handler' => 'views_handler_field',
       'click sortable' => TRUE,
     ),
     'sort' => array(
       'handler' => 'views_handler_sort',
     ),
     'filter' => array(
       'handler' => 'views_handler_filter_string',
     ),
     'argument' => array(
       'handler' => 'views_handler_argument_string',
     ),
   );
   $data['petty_cash']['deleted'] = array(
     'title' => t('Deleted'),
     'help' => t('Petty cash deleted'),
     'field' => array(
       'handler' => 'views_handler_field',
       'click sortable' => TRUE,
     ),
     'sort' => array(
       'handler' => 'views_handler_sort',
     ),
     'filter' => array(
       'handler' => 'views_handler_filter_string',
     ),
     'argument' => array(
       'handler' => 'views_handler_argument_string',
     ),
   );
   $data['petty_cash']['voucher_reference_no'] = array(
     'title' => t('Voucher reference no'),
     'help' => t('Petty cash voucher reference no'),
     'field' => array(
       'handler' => 'views_handler_field',
       'click sortable' => TRUE,
     ),
     'sort' => array(
       'handler' => 'views_handler_sort',
     ),
     'filter' => array(
       'handler' => 'views_handler_filter_string',
     ),
     'argument' => array(
       'handler' => 'views_handler_argument_string',
     ),
   );
   $data['petty_cash']['voucher_no'] = array(
     'title' => t('Voucher no'),
     'help' => t('Petty cash voucher no'),
     'field' => array(
       'handler' => 'views_handler_field',
       'click sortable' => TRUE,
     ),
     'sort' => array(
       'handler' => 'views_handler_sort',
     ),
     'filter' => array(
       'handler' => 'views_handler_filter_string',
     ),
     'argument' => array(
       'handler' => 'views_handler_argument_string',
     ),
   );
   $data['petty_cash']['voucher_date'] = array(
     'title' => t('Voucher date'),
     'help' => t('Petty cash voucher date'),
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
   // fieldExecutiveName
   $data['petty_cash']['file_date'] = array(
     'title' => t('File date'),
     'help' => t('Petty cash file date'),
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
// vehicle
   $data['petty_cash']['balance'] = array(
     'title' => t('Balance'),
     'help' => t('Petty cash balance'),
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
   //party_hll_code
   $data['petty_cash']['file'] = array(
     'title' => t('File'),
     'help' => t('Petty cash file'),
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
   $data['petty_cash']['file']['relationship'] = array(
     'title' => t('file'),
     'help' => t("file"),
     'handler' => 'views_handler_relationship',
     'base' => 'file_managed',
     //'entity type' => 'file',
     'base field' => 'fid',
     'label' => t('file from !field_name', array('!field_name' => 'file')),
   );

   $data['petty_cash']['bill_file'] = array(
     'title' => t('Bill File'),
     'help' => t('Petty cash bill file'),
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
   $data['petty_cash']['bill_file']['relationship'] = array(
     'title' => t('Bill file'),
     'help' => t(" bill file"),
     'handler' => 'views_handler_relationship',
     'base' => 'file_managed',
     //'entity type' => 'file',
     'base field' => 'fid',
     'label' => t('bill file from !field_name', array('!field_name' => 'bill_file')),
   );

   $data['petty_cash']['distributor_id'] = array(
     'title' => t('Distributor_id'),
     'help' => t('Petty cash distributor id'),
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
   $data['petty_cash']['date_filter'] = array(
     'group' => t('Date'),
     'title' => t('Date (!base_table)', array('!base_table' => 'petty_cash')),
     'help' => t('Filter any Views !base_table date field.', array('!base_table' => 'petty_cash')),
     'filter' => array(
       'handler' => 'date_views_filter_handler',
       'empty field name' => t('Undated'),
       'is date' => TRUE,
     //'skip base' => $base_table,
     ),
   );
   return $data;
}
