<?php
/**
 * @file
 * leanbox_payment_collection_feature.features.inc
 */

/**
 * Implements hook_node_info().
 */
function leanbox_payment_collection_feature_node_info() {
  $items = array(
    'assignment_history' => array(
      'name' => t('Assignment History'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Bill No'),
      'help' => '',
    ),
    'payment_collection' => array(
      'name' => t('Payment Collection'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Bill No'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
