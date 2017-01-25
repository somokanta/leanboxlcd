<?php
/**
 * @file
 * loading.features.inc
 */

/**
 * Implements hook_node_info().
 */
function loading_node_info() {
  $items = array(
    'loading' => array(
      'name' => t('Loading'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
