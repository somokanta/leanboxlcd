<?php
/**
 * @file
 * reverse_geocodes.features.inc
 */

/**
 * Implements hook_node_info().
 */
function reverse_geocodes_node_info() {
  $items = array(
    'area_hook_master' => array(
      'name' => t('Area Hook Master'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Area Hook'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
