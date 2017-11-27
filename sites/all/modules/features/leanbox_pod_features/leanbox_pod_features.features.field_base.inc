<?php
/**
 * @file
 * leanbox_pod_features.features.field_base.inc
 */

/**
 * Implements hook_field_default_field_bases().
 */
function leanbox_pod_features_field_default_field_bases() {
  $field_bases = array();

  // Exported field_base: 'field_call_status_user'.
  $field_bases['field_call_status_user'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_call_status_user',
    'indexes' => array(
      'uid' => array(
        0 => 'uid',
      ),
    ),
    'locked' => 0,
    'module' => 'user_reference',
    'settings' => array(
      'referenceable_roles' => array(
        2 => 0,
        3 => 0,
        4 => 0,
        5 => 5,
        6 => 0,
        7 => 7,
        8 => 8,
        9 => 9,
        10 => 0,
        11 => 0,
        12 => 0,
        13 => 0,
        14 => 0,
        15 => 0,
      ),
      'referenceable_status' => array(
        0 => 0,
        1 => 0,
      ),
      'view' => array(
        'args' => array(),
        'display_name' => '',
        'view_name' => '',
      ),
    ),
    'translatable' => 0,
    'type' => 'user_reference',
  );

  // Exported field_base: 'field_delivery_status_user'.
  $field_bases['field_delivery_status_user'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_delivery_status_user',
    'indexes' => array(
      'uid' => array(
        0 => 'uid',
      ),
    ),
    'locked' => 0,
    'module' => 'user_reference',
    'settings' => array(
      'referenceable_roles' => array(
        2 => 0,
        3 => 0,
        4 => 0,
        5 => 5,
        6 => 0,
        7 => 7,
        8 => 8,
        9 => 9,
        10 => 0,
        11 => 0,
        12 => 0,
        13 => 0,
        14 => 0,
        15 => 0,
      ),
      'referenceable_status' => array(
        0 => 0,
        1 => 0,
      ),
      'view' => array(
        'args' => array(),
        'display_name' => '',
        'view_name' => '',
      ),
    ),
    'translatable' => 0,
    'type' => 'user_reference',
  );

  // Exported field_base: 'field_pod_bill_reference'.
  $field_bases['field_pod_bill_reference'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_pod_bill_reference',
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
        'assignment_history' => 0,
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
        'page' => 0,
        'party_pack_master' => 0,
        'party_packing' => 0,
        'payment_collection' => 0,
        'pdd_master' => 0,
        'pick_confirm' => 0,
        'picking' => 0,
        'pl_group_master' => 0,
        'pod_history' => 0,
        'pricelist_group_master' => 0,
        'product_division_master' => 0,
        'resource_master' => 0,
        'resource_type' => 0,
        'return_bill' => 0,
        'sales_delivery_day_master' => 0,
        'sales_register_data' => 'sales_register_data',
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

  // Exported field_base: 'field_pod_call_history'.
  $field_bases['field_pod_call_history'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_pod_call_history',
    'indexes' => array(
      'tid' => array(
        0 => 'tid',
      ),
    ),
    'locked' => 0,
    'module' => 'taxonomy',
    'settings' => array(
      'allowed_values' => array(
        0 => array(
          'vocabulary' => 'pod_call_status',
          'parent' => 0,
        ),
      ),
    ),
    'translatable' => 0,
    'type' => 'taxonomy_term_reference',
  );

  // Exported field_base: 'field_pod_delivery_history'.
  $field_bases['field_pod_delivery_history'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_pod_delivery_history',
    'indexes' => array(
      'tid' => array(
        0 => 'tid',
      ),
    ),
    'locked' => 0,
    'module' => 'taxonomy',
    'settings' => array(
      'allowed_values' => array(
        0 => array(
          'vocabulary' => 'pod_delivery_status',
          'parent' => 0,
        ),
      ),
    ),
    'translatable' => 0,
    'type' => 'taxonomy_term_reference',
  );

  // Exported field_base: 'field_remarks_history'.
  $field_bases['field_remarks_history'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_remarks_history',
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

  return $field_bases;
}
