<?php
/**
 * @file
 * user_manual_feature.features.inc
 */

/**
 * Implements hook_views_api().
 */
function user_manual_feature_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function user_manual_feature_node_info() {
  $items = array(
    'user_manual' => array(
      'name' => t('User Manual'),
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
