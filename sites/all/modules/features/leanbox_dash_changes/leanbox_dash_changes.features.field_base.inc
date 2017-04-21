<?php
/**
 * @file
 * leanbox_dash_changes.features.field_base.inc
 */

/**
 * Implements hook_field_default_field_bases().
 */
function leanbox_dash_changes_field_default_field_bases() {
  $field_bases = array();

  // Exported field_base: 'field_target'.
  $field_bases['field_target'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_target',
    'indexes' => array(),
    'locked' => 0,
    'module' => 'number',
    'settings' => array(),
    'translatable' => 0,
    'type' => 'number_integer',
  );

  return $field_bases;
}
