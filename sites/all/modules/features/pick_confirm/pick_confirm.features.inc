<?php
/**
 * @file
 * pick_confirm.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function pick_confirm_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "services" && $api == "services") {
    return array("version" => "3");
  }
}

/**
 * Implements hook_views_api().
 */
function pick_confirm_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

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
