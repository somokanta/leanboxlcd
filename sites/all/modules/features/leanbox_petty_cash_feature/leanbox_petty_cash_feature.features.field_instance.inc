<?php
/**
 * @file
 * leanbox_petty_cash_feature.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function leanbox_petty_cash_feature_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance:
  // 'taxonomy_term-petty_cash-field_petty_cash_distributor'.
  $field_instances['taxonomy_term-petty_cash-field_petty_cash_distributor'] = array(
    'bundle' => 'petty_cash',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'user_reference',
        'settings' => array(),
        'type' => 'user_reference_default',
        'weight' => 3,
      ),
    ),
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_petty_cash_distributor',
    'label' => 'Petty Cash Distributor',
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
      'weight' => 34,
    ),
  );

  // Exported field_instance: 'taxonomy_term-petty_cash-field_petty_cash_head'.
  $field_instances['taxonomy_term-petty_cash-field_petty_cash_head'] = array(
    'bundle' => 'petty_cash',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 1,
      ),
    ),
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_petty_cash_head',
    'label' => 'Petty Cash Head',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'maxlength_js' => 0,
        'maxlength_js_label' => 'Content limited to @limit characters, remaining: <strong>@remaining</strong>',
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 32,
    ),
  );

  // Exported field_instance:
  // 'taxonomy_term-petty_cash-field_petty_cash_subhead'.
  $field_instances['taxonomy_term-petty_cash-field_petty_cash_subhead'] = array(
    'bundle' => 'petty_cash',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 2,
      ),
    ),
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_petty_cash_subhead',
    'label' => 'Petty Cash Subhead',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'maxlength_js' => 0,
        'maxlength_js_label' => 'Content limited to @limit characters, remaining: <strong>@remaining</strong>',
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 33,
    ),
  );

  // Exported field_instance: 'taxonomy_term-petty_cash-field_petty_cash_type'.
  $field_instances['taxonomy_term-petty_cash-field_petty_cash_type'] = array(
    'bundle' => 'petty_cash',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_petty_cash_type',
    'label' => 'Petty Cash Type',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 31,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Petty Cash Distributor');
  t('Petty Cash Head');
  t('Petty Cash Subhead');
  t('Petty Cash Type');

  return $field_instances;
}
