<?php
/**
 * @file
 * new_custom_menus.features.inc
 */

/**
 * Implements hook_node_info().
 */
function new_custom_menus_node_info() {
  $items = array(
    'impact_cron_mobile_data' => array(
      'name' => t('impact cron mobile data'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Bill Number'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
