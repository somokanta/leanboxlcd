<?php
/**
 * @file
 * leanbox_dash_changes.features.inc
 */

/**
 * Implements hook_views_api().
 */
function leanbox_dash_changes_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function leanbox_dash_changes_node_info() {
  $items = array(
    'van_stop_master' => array(
      'name' => t('Van-Stop Master'),
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
