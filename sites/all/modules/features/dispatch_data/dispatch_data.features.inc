<?php
/**
 * @file
 * dispatch_data.features.inc
 */

/**
 * Implements hook_node_info().
 */
function dispatch_data_node_info() {
  $items = array(
    'dispatch_data' => array(
      'name' => t('Dispatch Data'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'hht_master' => array(
      'name' => t('HHT Master'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('HHT id'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
