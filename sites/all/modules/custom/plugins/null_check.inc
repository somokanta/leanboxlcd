<?php

/**
 * @file
 * Do super awesome thing.
 */

$plugin = array(
  'form' => 'change_feeds_null_check_form',
  'validate' => 'change_feeds_null_check_validate',
  'callback' => 'change_feeds_null_check_callback',
  'name' => 'Null check',
  'multi' => 'loop',
  'category' => 'Other',
);

function change_feeds_null_check_form($importer, $element_key, $settings) {
  watchdog('importer_null', $importer);
  watchdog('element_key_null', $element_key);
  $form = array();
  $form['help']['#value'] = t('My plugin can do awesome things.');
  return $form;
}

function change_feeds_null_check_validate(&$settings) {
  // Validate $settings.
}

function change_feeds_null_check_callback($source, $item_key, $element_key, &$field, $settings) {
  //$field = crazy_modification($field);
  watchdog('source_null', $source);
  watchdog('element_key1_null', $element_key);
  watchdog('field_null', $field);
  watchdog('item_key_null', $item_key);
}
