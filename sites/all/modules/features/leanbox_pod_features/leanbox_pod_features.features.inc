<?php
/**
 * @file
 * leanbox_pod_features.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function leanbox_pod_features_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "feeds" && $api == "feeds_importer_default") {
    return array("version" => "1");
  }
  if ($module == "feeds_tamper" && $api == "feeds_tamper_default") {
    return array("version" => "2");
  }
}

/**
 * Implements hook_node_info().
 */
function leanbox_pod_features_node_info() {
  $items = array(
    'pod_history' => array(
      'name' => t('pod history'),
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
