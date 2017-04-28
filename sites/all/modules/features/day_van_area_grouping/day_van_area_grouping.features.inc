<?php
/**
 * @file
 * day_van_area_grouping.features.inc
 */

/**
 * Implements hook_node_info().
 */
function day_van_area_grouping_node_info() {
  $items = array(
    'area_hook_group_master' => array(
      'name' => t('Area Hook Group Master'),
      'base' => 'node_content',
      'description' => t('Area Hook Group Master'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'day_van_pl_gp_area_hook_grouping' => array(
      'name' => t('Day-Van-PL-Gp-Area Hook Grouping'),
      'base' => 'node_content',
      'description' => t('Day-Van-PL-Gp-Area Hook Grouping'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
