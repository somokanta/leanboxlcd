<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function mrc_inventory_returns_views_data() {

  $data = array();

  // Adding custom handler for VRS Handover Report
  $data['vrs_handover']['table'] = array(
    'group' => t('VRS Handover'),
    'title' => t('VRS Handover'),
    'help' => t('VRS Handover'),
  );

  // Define this as a base table.
  $data['vrs_handover']['table']['base'] = array(
    'field' => 'id',
    'title' => t('VRS Handover'),
    'help' => t("unique identifiers."),
  );

  // serial No
  $data['vrs_handover']['id'] = array(
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
  $data['vrs_handover']['trip_id'] = array(
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
  $data['vrs_handover']['vrs_id'] = array(
    'title' => t('VRS ID'),
    'help' => t('VRS ID'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['vrs_handover']['collection_type'] = array(
    'title' => t('Collection Type'),
    'help' => t('Collection Type'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['vrs_handover']['total_cash_amt'] = array(
    'title' => t('Total Cash Collected'),
    'help' => t('Total Cash Collected'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['vrs_handover']['handover_to'] = array(
    'title' => t('Handover To'),
    'help' => t('Handover To'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
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
      'base' => 'node',
      'base field' => 'title',
      'handler' => 'views_handler_relationship',
      'help' => t('Entity Name from Handover Master'),
      'extra' => array(
        array('field' => 'type', 'value' => 'handover_master'),
        array('field' => 'uid', 'left_field' => 'distributor_id'),
      ),
    ),
  );
  $data['vrs_handover']['handover_date'] = array(
    'title' => t('Handover Date'),
    'help' => t('Handover Date'),
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
      'handler' => 'views_handler_argument_date',
    ),
  );
  $data['vrs_handover']['handover_amt'] = array(
    'title' => t('Handover Amount'),
    'help' => t('Handover Amount'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['vrs_handover']['petty_cash_amt'] = array(
    'title' => t('Petty Cash Amount'),
    'help' => t('Petty Cash Amount'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['vrs_handover']['total_cheque_count'] = array(
    'title' => t('Total Cheque Count'),
    'help' => t('Total Cheque Count'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['vrs_handover']['cheque_amt'] = array(
    'title' => t('Total Cheque Amount Collected'),
    'help' => t('Total Cheque Amount Collected'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['vrs_handover']['signed_bills_count'] = array(
    'title' => t('Signed Bills Count'),
    'help' => t('Signed Bills Count'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['vrs_handover']['signed_bills_amt'] = array(
    'title' => t('Signed Bills Amount'),
    'help' => t('Signed Bills Amount'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['vrs_handover']['remarks'] = array(
    'title' => t('Remarks'),
    'help' => t('Remarks'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['vrs_handover']['handover_id'] = array(
    'title' => t('Handover ID'),
    'help' => t('Handover ID'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  
  $data['vrs_handover']['handover_cheque_id'] = array(
    'title' => t('Cheque Handover ID'),
    'help' => t('Cheque Handover ID'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  
  $data['vrs_handover']['handover_signedbill_id'] = array(
    'title' => t('Signed Bill Handover ID'),
    'help' => t('Signed Bill Handover ID'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  
  $data['vrs_handover']['file'] = array(
    'title' => t('File'),
    'help' => t('Handover Acknowldegement file'),
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
  $data['vrs_handover']['file']['relationship'] = array(
    'title' => t('file'),
    'help' => t("file"),
    'handler' => 'views_handler_relationship',
    'base' => 'file_managed',
    //'entity type' => 'file',
    'base field' => 'fid',
    'label' => t('file from !field_name', array('!field_name' => 'file')),
  );


  $vrs_handover = 'vrs_handover';
  $data[$vrs_handover]['date_filter'] = array(
    'group' => t('Date'),
    'title' => t('Date (!base_table)', array('!base_table' => $vrs_handover)),
    'help' => t('Filter any Views !base_table date field.', array('!base_table' => $vrs_handover)),
    'filter' => array(
      'handler' => 'date_views_filter_handler',
      'empty field name' => t('Undated'),
      'is date' => TRUE,
    //'skip base' => $base_table,
    ),
  );
  
  
    // Add custom field for footer_inventory_returns_bills_summary
  $data['views']['footer_inventory_returns_bills_summary'] = array(
    'title' => t('mrc_inventory_returns_bills_summary'),
    'help' => t('Provides count for mrc_inventory_returns_bills_summary.'),
    'area' => array(
      'handler' => 'footer_inventory_returns_bills_summary',
    ),
  );
  
    // Add custom field for footer_inventory_returns_collection_summary
  $data['views']['footer_collection_summary'] = array(
    'title' => t('mrc_inventory_returns_collection_summary'),
    'help' => t('Provides count for mrc_inventory_returns_collection_summary.'),
    'area' => array(
      'handler' => 'footer_inventory_returns_collection_summary',
    ),
  );
  
  // Add custom field for footer_inventory_returns_collection_mode_discrepancy
  $data['views']['footer_collection_mode_discrepancy'] = array(
    'title' => t('mrc_inventory_returns_collection_mode_discrepancy'),
    'help' => t('Provides count for mrc_inventory_returns_collection_mode_discrepancy.'),
    'area' => array(
      'handler' => 'footer_inventory_returns_collection_mode_discrepancy',
    ),
  );
  
  // Adding custom handler for VRS Trip Total Report
  $data['vrs_trip_total']['table'] = array(
    'group' => t('VRS Trip Total'),
    'title' => t('VRS Trip Total'),
    'help' => t('VRS Trip Total'),
  );

  // Define this as a base table.
  $data['vrs_trip_total']['table']['base'] = array(
    'field' => 'id',
    'title' => t('VRS Trip Total'),
    'help' => t("unique identifiers."),
  );

  // serial No
  $data['vrs_trip_total']['id'] = array(
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
  $data['vrs_trip_total']['trip_id'] = array(
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
    'relationship' => array(
      'base' => 'node',
      'base field' => 'title',
      'handler' => 'views_handler_relationship',
      'help' => t('Trip ID from Dispatch Data'),
      'extra' => array(
        array('field' => 'type', 'value' => 'dispatch_data'),
        array('field' => 'uid', 'left_field' => 'distributor_id'),
      ),
    ),
  );
  
  $data['vrs_trip_total']['vrs_id'] = array(
    'title' => t('VRS ID'),
    'help' => t('VRS ID'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );
  $data['vrs_trip_total']['vrs_version_no'] = array(
    'title' => t('VRS Version No.'),
    'help' => t('VRS Version No.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['vrs_trip_total']['bill_count'] = array(
    'title' => t('Bill Count'),
    'help' => t('Bill Count'),
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
      'handler' => 'views_handler_argument_numeric',
    ),
  );
  
   //Custom field handler for Crate Count for Order Summary
  $data['views']['total_bill_value'] = array(
    'title' => t('Sales Register Total Bill Value'),
    'help' => t('Just a field created for display sales register bill value.'),
    'real field' => 'id',
    'field' => array(
      'handler' => 'vrs_printout_total_bill_value',
    ),
  );
  
  $data['vrs_trip_total']['total_to_be_collected_sales_register'] = array(
    'title' => t('Total To Be Collected Sales Register'),
    'help' => t('Total To Be Collected Sales Register'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['vrs_trip_total']['rto_charges'] = array(
    'title' => t('RTO Charges'),
    'help' => t('RTO Charges'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  
  $data['vrs_trip_total']['total_to_be_collected_amt'] = array(
    'title' => t('Total To Be Collected'),
    'help' => t('Total To Be Collected'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['vrs_trip_total']['total_actual_cash_amount'] = array(
    'title' => t('Total Actual Cash Amount'),
    'help' => t('Total Actual Cash Amount'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['vrs_trip_total']['total_actual_cheque_amount'] = array(
    'title' => t('Total Actual Cheque Amount'),
    'help' => t('Total Actual Cheque Amount'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['vrs_trip_total']['total_actual_signed_bill_amount'] = array(
    'title' => t('Total Actual Signed Bill Amount'),
    'help' => t('Total Actual Signed Bill Amount'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['vrs_trip_total']['total_short_amt'] = array(
    'title' => t('Total Short Amount'),
    'help' => t('Total Short Amount'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['vrs_trip_total']['total_cashier_debit_value'] = array(
    'title' => t('Total Cashier Debit Value'),
    'help' => t('Total Cashier Debit Value'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['vrs_trip_total']['cheque_count'] = array(
    'title' => t('Cheque Count'),
    'help' => t('Cheque Count'),
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

  $data['vrs_trip_total']['sb_count'] = array(
    'title' => t('Remarks'),
    'help' => t('Remarks'),
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
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  $data['vrs_trip_total']['vrs_created_date'] = array(
    'title' => t('VRS Created Date'),
    'help' => t('VRS Created Date'),
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
      'handler' => 'views_handler_argument_date',
    ),
  );

  $data['vrs_trip_total']['vrs_confirm_date'] = array(
    'title' => t('VRS Confirm Date'),
    'help' => t('VRS Confirm Date'),
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
      'handler' => 'views_handler_argument_date',
    ),
  );
  
  $data['vrs_trip_total']['total_cashier_cash_short'] = array(
    'title' => t('Total Cashier Cash Short'),
    'help' => t('Total Cashier Cash Short'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  
  $vrs_trip_total = 'vrs_trip_total';
  $data[$vrs_trip_total]['date_filter'] = array(
    'group' => t('Date'),
    'title' => t('Date (!base_table)', array('!base_table' => $vrs_trip_total)),
    'help' => t('Filter any Views !base_table date field.', array('!base_table' => $vrs_trip_total)),
    'filter' => array(
      'handler' => 'date_views_filter_handler',
      'empty field name' => t('Undated'),
      'is date' => TRUE,
    //'skip base' => $base_table,
    ),
  );
  
  
  // Table for versioning history of api data
  $data['inventory_versioning_api_data']['table'] = array(
    'group' => t('Inventory Versioning api data'),
    'title' => t('Inventory Versioning api data'),
    'help' => t('Inventory Versioning api data'),
  );

  // Define this as a base table.
  $data['inventory_versioning_api_data']['table']['base'] = array(
    'field' => 'id',
    'title' => t('Inventory Versioning api data'),
    'help' => t("unique identifiers."),
    // 'weight' => -10,
  );
  $data['inventory_versioning_api_data']['id'] = array(
    'title' => t('IV ID'),
    'help' => t('Inventory Versioning Data id'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
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
      'help' => t("Id to Trip_bill_reference in inventory_versioning_item_details."),
      'handler' => 'views_handler_relationship',
      'base' => 'inventory_versioning_item_details',
      'base field' => 'trip_bill_reference',
      'label' => t('Id to !field_name', array('!field_name' => 'trip_bill_reference')),
    ),
  );

  $data['inventory_versioning_api_data']['jobType'] = array(
    'title' => t('IV jobType'),
    'help' => t('Inventory Versioning jobType'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['inventory_versioning_api_data']['referenceNo'] = array(
    'title' => t('IV referenceNo'),
    'help' => t('Inventory Versioning referenceNo'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['inventory_versioning_api_data']['date'] = array(
    'title' => t('IV date'),
    'help' => t('Inventory Versioning date'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['inventory_versioning_api_data']['city'] = array(
    'title' => t('IV city'),
    'help' => t('Inventory Versioning date'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['inventory_versioning_api_data']['hub'] = array(
    'title' => t('IV hub'),
    'help' => t('Inventory Versioning Rs Code'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['inventory_versioning_api_data']['fieldExecutive'] = array(
    'title' => t('IV fieldExecutive'),
    'help' => t('Inventory Versioning fieldExecutive'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['inventory_versioning_api_data']['fieldExecutiveName'] = array(
    'title' => t('IV fieldExecutiveName'),
    'help' => t('Inventory Versioning fieldExecutiveName'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['inventory_versioning_api_data']['vehicle'] = array(
    'title' => t('IV vehicle'),
    'help' => t('Inventory Versioning vehicle'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  
  $data['inventory_versioning_api_data']['party_hll_code'] = array(
    'title' => t('IV party_hll_code'),
    'help' => t('Inventory Versioning party_hll_code'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  
  $data['inventory_versioning_api_data']['lat_longlatitude'] = array(
    'title' => t('IV lat_longlatitude'),
    'help' => t('Inventory Versioning lat_longlatitude'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['inventory_versioning_api_data']['lat_longlongitude'] = array(
    'title' => t('IV lat_longlongitude'),
    'help' => t('Inventory Versioning lat_longlongitude'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  
  $data['inventory_versioning_api_data']['bill_value'] = array(
    'title' => t('IV bill_value'),
    'help' => t('Inventory Versioning bill_value'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['inventory_versioning_api_data']['bill_date'] = array(
    'title' => t('IV bill_date'),
    'help' => t('Inventory Versioning bill_date'),
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

  $data['inventory_versioning_api_data']['locality'] = array(
    'title' => t('IV locality'),
    'help' => t('Inventory Versioning locality'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['inventory_versioning_api_data']['store_sequence_no'] = array(
    'title' => t('IV store_sequence_no'),
    'help' => t('Inventory Versioning store_sequence_no'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['inventory_versioning_api_data']['party_name'] = array(
    'title' => t('IV party_name'),
    'help' => t('Inventory Versioning party_name'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['inventory_versioning_api_data']['beat_name'] = array(
    'title' => t('IV beat_name'),
    'help' => t('Inventory Versioning beat_name'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['inventory_versioning_api_data']['party_code'] = array(
    'title' => t('IV party_code'),
    'help' => t('Inventory Versioning party_code'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['inventory_versioning_api_data']['bill_number'] = array(
    'title' => t('IV bill_number'),
    'help' => t('Inventory Versioning bill_number'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['inventory_versioning_api_data']['runsheet_number'] = array(
    'title' => t('IV runsheet_number'),
    'help' => t('Inventory Versioning Trip Id'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['inventory_versioning_api_data']['distributor_id'] = array(
    'title' => t('IV distributor_id'),
    'help' => t('Inventory Versioning distributor_id'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['inventory_versioning_api_data']['res_runsheetNo'] = array(
    'title' => t('IV res_runsheetNo'),
    'help' => t('Inventory Versioning res_runsheetNo'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['inventory_versioning_api_data']['res_actualAmount'] = array(
    'title' => t('IV res_actualAmount'),
    'help' => t('Inventory Versioning res_actualAmount'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['inventory_versioning_api_data']['res_originalAmount'] = array(
    'title' => t('IV res_originalAmount'),
    'help' => t('Inventory Versioning res_originalAmount'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['inventory_versioning_api_data']['res_moneyTransactionType'] = array(
    'title' => t('IV res_moneyTransactionType'),
    'help' => t('Inventory Versioning res_moneyTransactionType'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['inventory_versioning_api_data']['res_latitude'] = array(
    'title' => t('IV res_latitude'),
    'help' => t('Inventory Versioning res_latitude'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['inventory_versioning_api_data']['res_longitude'] = array(
    'title' => t('IV res_longitude'),
    'help' => t('Inventory Versioning res_longitude'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['inventory_versioning_api_data']['res_jobMasterId'] = array(
    'title' => t('IV res_jobMasterId'),
    'help' => t('Inventory Versioning res_jobMasterId'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['inventory_versioning_api_data']['res_attemptCount'] = array(
    'title' => t('IV res_attemptCount'),
    'help' => t('Inventory Versioning res_attemptCount'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['inventory_versioning_api_data']['res_companyId'] = array(
    'title' => t('IV res_companyId'),
    'help' => t('Inventory Versioning res_companyId'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  
  $data['inventory_versioning_api_data']['res_jobType'] = array(
    'title' => t('IV res_jobType'),
    'help' => t('Inventory Versioning res_jobType'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['inventory_versioning_api_data']['res_jobId'] = array(
    'title' => t('IV res_jobId'),
    'help' => t('Inventory Versioning res_jobId'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['inventory_versioning_api_data']['res_employeeCode'] = array(
    'title' => t('IV res_employeeCode'),
    'help' => t('Inventory Versioning res_employeeCode'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['inventory_versioning_api_data']['res_status'] = array(
    'title' => t('IV res_status'),
    'help' => t('Inventory Versioning res_status'),
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
  
 // Total sales quantity sum_net_sales_quantity in item level
    $data['views']['versioning_total_sales_quantity'] = array(
        'title' => t('Total Sales Quantity'),
        'help' => t('Total Sales Quantity.'),
        'real field' => 'id',
        'field' => array(
            'handler' => 'inventory_versioning_bill_returns_report',
        ),
    );
    //Godown short res_godown_short in item level
    $data['views']['versioning_res_godown_short'] = array(
        'title' => t('Godown Short'),
        'help' => t('Godown Short'),
        'real field' => 'id',
        'field' => array(
            'handler' => 'inventory_versioning_bill_returns_report',
        ),
    );
    //res_cashier_short in item level
    $data['views']['versioning_cashier_short'] = array(
        'title' => t('Cashier Short'),
        'help' => t('Cashier Short'),
        'real field' => 'id',
        'field' => array(
            'handler' => 'inventory_versioning_bill_returns_report',
        ),
    );
    //Dispatched Quantity dispatch_qty in item level
    $data['views']['versioning_dispatched_qty'] = array(
        'title' => t('Dispatched Quantity'),
        'help' => t('Dispatched Quantity'),
        'real field' => 'id',
        'field' => array(
            'handler' => 'inventory_versioning_bill_returns_report',
        ),
    );
    //Returned Qty return_qty in item level
    $data['views']['versioning_return_qty'] = array(
        'title' => t('Returned Quantity'),
        'help' => t('Returned Quantity'),
        'real field' => 'id',
        'field' => array(
            'handler' => 'inventory_versioning_bill_returns_report',
        ),
    );
    //Actual Deliverd Qty .res_sku_actual_quantity1
    $data['views']['versioning_actual_delivered_qty'] = array(
        'title' => t('Actual Delivered Quantity'),
        'help' => t('Actual Delivered Quantity'),
        'real field' => 'id',
        'field' => array(
            'handler' => 'inventory_versioning_bill_returns_report',
        ),
    );
    // Delivery app delivered quantity  
    $data['views']['versioning_delivery_app_delivered_qty'] = array(
        'title' => t('Delivery App Delivered Quantity'),
        'help' => t('Delivery App Delivered Quantity'),
        'real field' => 'id',
        'field' => array(
            'handler' => 'inventory_versioning_bill_returns_report',
        ),
    );

    $data['inventory_versioning_api_data']['res_trackHalt'] = array(
    'title' => t('IV res_trackHalt'),
    'help' => t('Inventory Versioning res_trackHalt'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['inventory_versioning_api_data']['res_trackKm'] = array(
    'title' => t('IV res_trackKm'),
    'help' => t('Inventory Versioning res_trackKm'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['inventory_versioning_api_data']['res_trackTransactionTimeSpent'] = array(
    'title' => t('IV res_trackTransactionTimeSpent'),
    'help' => t('Inventory Versioning res_trackTransactionTimeSpent'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['inventory_versioning_api_data']['res_merchantCode'] = array(
    'title' => t('IV res_merchantCode'),
    'help' => t('Inventory Versioning res_merchantCode'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['inventory_versioning_api_data']['res_transactionDate'] = array(
    'title' => t('IV res_transactionDate'),
    'help' => t('Inventory Versioning res_transactionDate'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['inventory_versioning_api_data']['res_erpPushTime'] = array(
    'title' => t('IV res_erpPushTime'),
    'help' => t('Inventory Versioning res_erpPushTime'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['inventory_versioning_api_data']['res_lastTransactionTime'] = array(
    'title' => t('IV res_lastTransactionTime'),
    'help' => t('Inventory Versioning res_lastTransactionTime'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['inventory_versioning_api_data']['res_battery'] = array(
    'title' => t('IV res_battery'),
    'help' => t('Inventory Versioning res_battery'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['inventory_versioning_api_data']['res_confirm_signed_bill_amount'] = array(
    'title' => t('IV res_confirm_signed_bill_amount'),
    'help' => t('Inventory Versioning res_confirm_signed_bill_amount'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['inventory_versioning_api_data']['res_bank_name'] = array(
    'title' => t('IV res_bank_name'),
    'help' => t('Inventory Versioning res_bank_name'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['inventory_versioning_api_data']['updated_flag'] = array(
    'title' => t('IV updated_flag'),
    'help' => t('Inventory Versioning updated_flag'),
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
  $data['inventory_versioning_api_data']['res_photo_of_signed_bill_delivery'] = array(
    'title' => t('IV res_photo_of_signed_bill_delivery'),
    'help' => t('Inventory Versioning res_photo_of_signed_bill_delivery'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['inventory_versioning_api_data']['res_total_amount'] = array(
    'title' => t('IV res_total_amount'),
    'help' => t('Inventory Versioning res_total_amount'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['inventory_versioning_api_data']['res_remarks'] = array(
    'title' => t('IV res_remarks'),
    'help' => t('Inventory Versioning res_remarks'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['inventory_versioning_api_data']['res_cheque_date'] = array(
    'title' => t('IV res_cheque_date'),
    'help' => t('Inventory Versioning res_cheque_date'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['inventory_versioning_api_data']['res_photo_of_cheque'] = array(
    'title' => t('IV res_photo_of_cheque'),
    'help' => t('Inventory Versioning res_photo_of_cheque'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['inventory_versioning_api_data']['res_cheque_amount'] = array(
    'title' => t('IV res_cheque_amount'),
    'help' => t('Inventory Versioning res_cheque_amount'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['inventory_versioning_api_data']['res_money_collect2'] = array(
    'title' => t('IV res_money_collect2'),
    'help' => t('Inventory Versioning res_money_collect2'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  
  $data['inventory_versioning_api_data']['res_signed_bill_delivery'] = array(
    'title' => t('IV res_signed_bill_delivery'),
    'help' => t('Inventory Versioning res_signed_bill_delivery'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  
  $data['inventory_versioning_api_data']['res_reason_for_failure'] = array(
    'title' => t('IV res_reason_for_failure'),
    'help' => t('Inventory Versioning res_reason_for_failure'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  
  $data['inventory_versioning_api_data']['res_cash_amount'] = array(
    'title' => t('IV res_cash_amount'),
    'help' => t('Inventory Versioning res_cash_amount'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['inventory_versioning_api_data']['res_to_be_collection'] = array(
    'title' => t('IV res_to_be_collection'),
    'help' => t('Inventory Versioning res_to_be_collection'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['inventory_versioning_api_data']['res_signed_bill_partial'] = array(
    'title' => t('IV res_signed_bill_partial'),
    'help' => t('Inventory Versioning res_signed_bill_partial'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  
  
  $data['inventory_versioning_api_data']['salesman'] = array(
    'title' => t('IV salesman'),
    'help' => t('Inventory Versioning salesman'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  
   $data['inventory_versioning_api_data']['created_time'] = array(
    'title' => t('IV Created Time'),
    'help' => t('Inventory Versioning Created Time'),
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
   
  $data['inventory_versioning_api_data']['updated_time'] = array(
    'title' => t('IV Updated Time'),
    'help' => t('Inventory Versioning Updated Time'),
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
     
  $data['inventory_versioning_api_data']['res_updated_time'] = array(
    'title' => t('IV FAREY Updated Time'),
    'help' => t('Inventory Versioning FAREY Updated Time'),
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
  
  $data['inventory_versioning_api_data']['version_created_time'] = array(
    'title' => t('IV Version Created Time'),
    'help' => t('Inventory Versioning Version Created Time'),
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
  
  $data['inventory_versioning_api_data']['version_no'] = array(
    'title' => t('IV Version No'),
    'help' => t('Inventory Versioning version_no'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  
  $data['inventory_versioning_api_data']['res_cheque_date_edit'] = array(
    'title' => t('IV res_cheque_date_edit'),
    'help' => t('Inventory Versioning res_cheque_date'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['inventory_versioning_api_data']['res_photo_of_cheque_edit'] = array(
    'title' => t('IV res_photo_of_cheque_edit'),
    'help' => t('Inventory Versioning res_photo_of_cheque'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['inventory_versioning_api_data']['res_cheque_amount_edit'] = array(
    'title' => t('IV res_cheque_amount_edit'),
    'help' => t('Inventory Versioning res_cheque_amount'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['inventory_versioning_api_data']['res_money_collect2_edit'] = array(
    'title' => t('IV res_money_collect2_edit'),
    'help' => t('Inventory Versioning res_money_collect2'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
   $data['inventory_versioning_api_data']['res_confirm_signed_bill_amount_edit'] = array(
    'title' => t('IV res_confirm_signed_bill_amount_edit'),
    'help' => t('Inventory Versioning res_confirm_signed_bill_amount'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['inventory_versioning_api_data']['res_bank_name_edit'] = array(
    'title' => t('IV res_bank_name_edit'),
    'help' => t('Inventory Versioning res_bank_name'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['inventory_versioning_api_data']['res_photo_of_signed_bill_delivery_edit'] = array(
    'title' => t('IV res_photo_of_signed_bill_delivery_edit'),
    'help' => t('Inventory Versioning res_photo_of_signed_bill_delivery'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
    $data['inventory_versioning_api_data']['res_cash_amount_edit'] = array(
    'title' => t('IV res_cash_amount_edit'),
    'help' => t('Inventory Versioning res_cash_amount'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  
  $data['inventory_versioning_api_data']['res_cheque_no'] = array(
    'title' => t('IV res_cheque_no'),
    'help' => t('Inventory Versioning res_cheque_no'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
   $data['inventory_versioning_api_data']['res_cheque_no_edit'] = array(
    'title' => t('IV res_cheque_no_edit'),
    'help' => t('Inventory Versioning res_cheque_no'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  
   $data['inventory_versioning_api_data']['res_signed_bill_delivery_edit'] = array(
    'title' => t('IV res_signed_bill_delivery_edit'),
    'help' => t('Inventory Versioning res_signed_bill_delivery'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
 
  $data['inventory_versioning_api_data']['res_signed_bill_partial_edit'] = array(
    'title' => t('IV res_signed_bill_partial_edit'),
    'help' => t('Inventory Versioning res_signed_bill_partial'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  ); 
 
    //inventory_versioning_item_details Item Details
  $data['inventory_versioning_item_details']['table'] = array(
    'group' => t('inventory_versioning_item_details lines'),
    'title' => t('inventory_versioning_item_details '),
    'help' => t('inventory_versioning_item_details .'),
  );

  // Define this as a base table.
  $data['inventory_versioning_item_details']['table']['base'] = array(
    'field' => 'id',
    'title' => t('inventory_versioning_item_details lines'),
    'help' => t("unique identifiers."),
    // 'weight' => -10,
  );
  $data['inventory_versioning_item_details']['id'] = array(
    'title' => t('IV lines id'),
    'help' => t('Inventory Versioning lines id'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['inventory_versioning_item_details']['tur'] = array(
    'title' => t('IV lines tur'),
    'help' => t('Inventory Versioning lines tur'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['inventory_versioning_item_details']['mrp'] = array(
    'title' => t('IV lines mrp'),
    'help' => t('Inventory Versioning lines mrp'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['inventory_versioning_item_details']['product_name'] = array(
    'title' => t('IV lines product_name'),
    'help' => t('Inventory Versioning lines product_name'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['inventory_versioning_item_details']['res_sku_reason'] = array(
    'title' => t('IV lines res_sku_reason'),
    'help' => t('Inventory Versioning lines res_sku_reason'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['inventory_versioning_item_details']['res_godown_short'] = array(
    'title' => t('IV lines res_godown_short'),
    'help' => t('Inventory Versioning lines res_godown_short'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['inventory_versioning_item_details']['res_cashier_short'] = array(
    'title' => t('IV lines res_cashier_short'),
    'help' => t('Inventory Versioning lines res_cashier_short'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['inventory_versioning_item_details']['sum_net_sales_cases'] = array(
    'title' => t('IV lines sum_net_sales_cases'),
    'help' => t('Inventory Versioning lines sum_net_sales_cases'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['inventory_versioning_item_details']['sum_net_sales_quantity'] = array(
    'title' => t('IV lines sum_net_sales_quantity'),
    'help' => t('Inventory Versioning lines sum_net_sales_quantity'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['inventory_versioning_item_details']['sku_code'] = array(
    'title' => t('IV lines sku_code'),
    'help' => t('Inventory Versioning lines sku_code'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['inventory_versioning_item_details']['sum_net_sales_kgs'] = array(
    'title' => t('IV lines sum_net_sales_kgs'),
    'help' => t('Inventory Versioning lines sum_net_sales_kgs'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['inventory_versioning_item_details']['sum_net_sales_value'] = array(
    'title' => t('IV lines sum_net_sales_value'),
    'help' => t('Inventory Versioning lines sum_net_sales_value'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['inventory_versioning_item_details']['sku_description'] = array(
    'title' => t('IV lines sku_description'),
    'help' => t('Inventory Versioning lines sku_description'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['inventory_versioning_item_details']['trip_bill_reference'] = array(
    'title' => t('IV lines trip_bill_reference'),
    'help' => t('Inventory Versioning lines trip_bill_reference'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['inventory_versioning_item_details']['res_sku_actual_quantity1'] = array(
    'title' => t('IV lines res_sku_actual_quantity1'),
    'help' => t('Inventory Versioning lines res_sku_actual_quantity1'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  
  $data['inventory_versioning_item_details']['farey_delivered_qty'] = array(
    'title' => t('IV lines farey_delivered_qty'),
    'help' => t('Inventory Versioning lines farey_delivered_qty'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
    
  $data['inventory_versioning_item_details']['dispatch_qty'] = array(
    'title' => t('IV dispatch_qty'),
    'help' => t('Inventory Versioning lines dispatch_qty'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
   $data['inventory_versioning_item_details']['return_qty'] = array(
    'title' => t('IV return_qty'),
    'help' => t('Inventory Versioning lines return_qty'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
   
   $data['inventory_versioning_item_details']['line_level_status'] = array(
    'title' => t('IV Line level status'),
    'help' => t('Inventory Versioning lines level status'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  
  $inventory_versioning_api_data = 'inventory_versioning_api_data';
  $data[$inventory_versioning_api_data]['date_filter'] = array(
    'group' => t('Date'),
    'title' => t('Date (!base_table)', array('!base_table' => $inventory_versioning_api_data)),
    'help' => t('Filter any Views !base_table date field.', array('!base_table' => $inventory_versioning_api_data)),
    'filter' => array(
      'handler' => 'date_views_filter_handler',
      'empty field name' => t('Undated'),
      'is date' => TRUE,
    //'skip base' => $base_table,
    ),
  );
  

  // Adding custom handler for VRS Handover History Report
  $data['vrs_handover_history']['table'] = array(
    'group' => t('VRS Handover History'),
    'title' => t('VRS Handover History'),
    'help' => t('VRS Handover History'),
  );

  // Define this as a base table.
  $data['vrs_handover_history']['table']['base'] = array(
    'field' => 'id',
    'title' => t('VRS Handover History'),
    'help' => t("unique identifiers."),
  );

  // serial No
  $data['vrs_handover_history']['id'] = array(
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
  $data['vrs_handover_history']['trip_id'] = array(
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
  $data['vrs_handover_history']['vrs_id'] = array(
    'title' => t('VRS ID'),
    'help' => t('VRS ID'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['vrs_handover_history']['collection_type'] = array(
    'title' => t('Collection Type'),
    'help' => t('Collection Type'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['vrs_handover_history']['total_cash_amt'] = array(
    'title' => t('Total Cash Collected'),
    'help' => t('Total Cash Collected'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['vrs_handover_history']['handover_to'] = array(
    'title' => t('Handover To'),
    'help' => t('Handover To'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
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
      'base' => 'node',
      'base field' => 'title',
      'handler' => 'views_handler_relationship',
      'help' => t('Entity Name from Handover Master'),
      'extra' => array(
        array('field' => 'type', 'value' => 'handover_master'),
        array('field' => 'uid', 'left_field' => 'distributor_id'),
      ),
    ),
  );
  $data['vrs_handover_history']['handover_date'] = array(
    'title' => t('Handover Date'),
    'help' => t('Handover Date'),
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
      'handler' => 'views_handler_argument_date',
    ),
  );
  $data['vrs_handover_history']['handover_amt'] = array(
    'title' => t('Handover Amount'),
    'help' => t('Handover Amount'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['vrs_handover_history']['petty_cash_amt'] = array(
    'title' => t('Petty Cash Amount'),
    'help' => t('Petty Cash Amount'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['vrs_handover_history']['total_cheque_count'] = array(
    'title' => t('Total Cheque Count'),
    'help' => t('Total Cheque Count'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['vrs_handover_history']['cheque_amt'] = array(
    'title' => t('Total Cheque Amount Collected'),
    'help' => t('Total Cheque Amount Collected'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['vrs_handover_history']['signed_bills_count'] = array(
    'title' => t('Signed Bills Count'),
    'help' => t('Signed Bills Count'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['vrs_handover_history']['signed_bills_amt'] = array(
    'title' => t('Signed Bills Amount'),
    'help' => t('Signed Bills Amount'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['vrs_handover_history']['remarks'] = array(
    'title' => t('Remarks'),
    'help' => t('Remarks'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['vrs_handover_history']['handover_id'] = array(
    'title' => t('Handover ID'),
    'help' => t('Handover ID'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  
  $data['vrs_handover_history']['handover_cheque_id'] = array(
    'title' => t('Cheque Handover ID'),
    'help' => t('Cheque Handover ID'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  
  $data['vrs_handover_history']['handover_signedbill_id'] = array(
    'title' => t('Signed Bill Handover ID'),
    'help' => t('Signed Bill Handover ID'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  
  $data['vrs_handover_history']['file'] = array(
    'title' => t('File'),
    'help' => t('Handover Acknowldegement file'),
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
  $data['vrs_handover_history']['file']['relationship'] = array(
    'title' => t('file'),
    'help' => t("file"),
    'handler' => 'views_handler_relationship',
    'base' => 'file_managed',
    //'entity type' => 'file',
    'base field' => 'fid',
    'label' => t('file from !field_name', array('!field_name' => 'file')),
  );


  $vrs_handover_history = 'vrs_handover_history';
  $data[$vrs_handover_history]['date_filter'] = array(
    'group' => t('Date'),
    'title' => t('Date (!base_table)', array('!base_table' => $vrs_handover_history)),
    'help' => t('Filter any Views !base_table date field.', array('!base_table' => $vrs_handover_history)),
    'filter' => array(
      'handler' => 'date_views_filter_handler',
      'empty field name' => t('Undated'),
      'is date' => TRUE,
    //'skip base' => $base_table,
    ),
  );
  
  $data['vrs_handover_history']['cash_version_no'] = array(
    'title' => t('Cash Version No'),
    'help' => t('Cash Version No'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  
  $data['vrs_handover_history']['cheque_version_no'] = array(
    'title' => t('Cheque Version No'),
    'help' => t('Cheque Version No'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  
  $data['vrs_handover_history']['signed_version_no'] = array(
    'title' => t('Signed Version No'),
    'help' => t('Signed Version No'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  
  return $data;
}
