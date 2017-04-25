<?php
/**
 * @file
 * leanbox_dash_changes.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function leanbox_dash_changes_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'taxonomy_term-productivity_target-field_target'.
  $field_instances['taxonomy_term-productivity_target-field_target'] = array(
    'bundle' => 'productivity_target',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'number',
        'settings' => array(
          'decimal_separator' => '.',
          'prefix_suffix' => TRUE,
          'scale' => 0,
          'thousand_separator' => '',
        ),
        'type' => 'number_integer',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_target',
    'label' => 'Target',
    'required' => 0,
    'settings' => array(
      'max' => '',
      'min' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'number',
      'settings' => array(),
      'type' => 'number',
      'weight' => 31,
    ),
  );

  // Exported field_instance:
  // 'taxonomy_term-productivity_target-field_target_distributor'.
  $field_instances['taxonomy_term-productivity_target-field_target_distributor'] = array(
    'bundle' => 'productivity_target',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'user_reference',
        'settings' => array(),
        'type' => 'user_reference_default',
        'weight' => 1,
      ),
    ),
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_target_distributor',
    'label' => 'Target Distributor',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'user_reference',
      'settings' => array(
        'autocomplete_match' => 'contains',
        'autocomplete_path' => 'user_reference/autocomplete',
        'size' => 60,
      ),
      'type' => 'user_reference_autocomplete',
      'weight' => 32,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Target');
  t('Target Distributor');

  return $field_instances;
}
