<?php
/**
 * @file
 * collection_feature.features.inc
 */

/**
 * Implements hook_node_info().
 */
function collection_feature_node_info() {
  $items = array(
    'collection_history' => array(
      'name' => t('Collection History'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Bill No.'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
