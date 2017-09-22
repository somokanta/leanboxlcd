<?php
/**
 * @file
 * pick_confirm.features.inc
 */

/**
 * Implements hook_node_info().
 */
function pick_confirm_node_info() {
  $items = array(
    'pick_confirm' => array(
      'name' => t('Pick Confirm'),
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
