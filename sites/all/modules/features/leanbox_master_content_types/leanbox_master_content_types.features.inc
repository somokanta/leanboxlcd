<?php
/**
 * @file
 * leanbox_master_content_types.features.inc
 */

/**
 * Implements hook_node_info().
 */
function leanbox_master_content_types_node_info() {
  $items = array(
    'area_hook_master' => array(
      'name' => t('Area Hook Master'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Area Hook'),
      'help' => '',
    ),
    'hul_code_master' => array(
      'name' => t('Hul Code Master'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Hul Code'),
      'help' => '',
    ),
    'outlet_master' => array(
      'name' => t('Outlet Master'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'party_pack_master' => array(
      'name' => t('PARTY PACK Master'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'pricelist_group_master' => array(
      'name' => t('Pricelist Group Master'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Pricelist Group Code'),
      'help' => '',
    ),
    'product_division_master' => array(
      'name' => t('Product Division Master'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Product Division Code'),
      'help' => '',
    ),
    'resource_type' => array(
      'name' => t('RESOURCE TYPE'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Resource Type'),
      'help' => '',
    ),
    'sales_register_data' => array(
      'name' => t('Sales register data'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Bill Number'),
      'help' => '',
    ),
    'vehicle_master' => array(
      'name' => t('Vehicle Master'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Vehicle Number'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
