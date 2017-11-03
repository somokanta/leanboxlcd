<?php
/**
 * @file
 * collection_feature.features.field_base.inc
 */

/**
 * Implements hook_field_default_field_bases().
 */
function collection_feature_field_default_field_bases() {
  $field_bases = array();

  // Exported field_base: 'field_assigned_to'.
  $field_bases['field_assigned_to'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_assigned_to',
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(
      'max_length' => 255,
    ),
    'translatable' => 0,
    'type' => 'text',
  );

  // Exported field_base: 'field_collection_cash_amt'.
  $field_bases['field_collection_cash_amt'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_collection_cash_amt',
    'indexes' => array(),
    'locked' => 0,
    'module' => 'number',
    'settings' => array(
      'decimal_separator' => '.',
      'precision' => 10,
      'scale' => 2,
    ),
    'translatable' => 0,
    'type' => 'number_decimal',
  );

  // Exported field_base: 'field_collection_ch_amt'.
  $field_bases['field_collection_ch_amt'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_collection_ch_amt',
    'indexes' => array(),
    'locked' => 0,
    'module' => 'number',
    'settings' => array(
      'decimal_separator' => '.',
      'precision' => 10,
      'scale' => 2,
    ),
    'translatable' => 0,
    'type' => 'number_decimal',
  );

  // Exported field_base: 'field_collection_ch_date'.
  $field_bases['field_collection_ch_date'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_collection_ch_date',
    'indexes' => array(),
    'locked' => 0,
    'module' => 'date',
    'settings' => array(
      'cache_count' => 4,
      'cache_enabled' => 0,
      'granularity' => array(
        'day' => 'day',
        'hour' => 'hour',
        'minute' => 'minute',
        'month' => 'month',
        'second' => 0,
        'year' => 'year',
      ),
      'timezone_db' => 'UTC',
      'todate' => '',
      'tz_handling' => 'site',
    ),
    'translatable' => 0,
    'type' => 'datetime',
  );

  // Exported field_base: 'field_collection_cheque_no'.
  $field_bases['field_collection_cheque_no'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_collection_cheque_no',
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(
      'max_length' => 255,
    ),
    'translatable' => 0,
    'type' => 'text',
  );

  // Exported field_base: 'field_payment_collection_ref'.
  $field_bases['field_payment_collection_ref'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_payment_collection_ref',
    'indexes' => array(
      'nid' => array(
        0 => 'nid',
      ),
    ),
    'locked' => 0,
    'module' => 'node_reference',
    'settings' => array(
      'referenceable_types' => array(
        'area_hook_group_master' => 0,
        'area_hook_master' => 0,
        'article' => 0,
        'billing_data' => 0,
        'ccfot' => 0,
        'channel_master' => 0,
        'collection_history' => 0,
        'day_van_pl_gp_area_hook_grouping' => 0,
        'dispatch_data' => 0,
        'egir' => 0,
        'godown_master' => 0,
        'hht_master' => 0,
        'holiday_master' => 0,
        'hul_code_master' => 0,
        'impact_cron_mobile_data' => 0,
        'loading' => 0,
        'outlet_master' => 0,
        'outstanding_activity' => 0,
        'page' => 0,
        'party_pack_master' => 0,
        'party_packing' => 0,
        'payment_collection' => 'payment_collection',
        'pdd_master' => 0,
        'pick_confirm' => 0,
        'picking' => 0,
        'pl_group_master' => 0,
        'pricelist_group_master' => 0,
        'product_division_master' => 0,
        'resource_master' => 0,
        'resource_type' => 0,
        'return_bill' => 0,
        'sales_delivery_day_master' => 0,
        'sales_register_data' => 0,
        'sku7_code_master' => 0,
        'user_manual' => 0,
        'van_pl_group_master' => 0,
        'van_stop_master' => 0,
        'vehicle_master' => 0,
      ),
      'view' => array(
        'args' => array(),
        'display_name' => '',
        'view_name' => '',
      ),
    ),
    'translatable' => 0,
    'type' => 'node_reference',
  );

  return $field_bases;
}
