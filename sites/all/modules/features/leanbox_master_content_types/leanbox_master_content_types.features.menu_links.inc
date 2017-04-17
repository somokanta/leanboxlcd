<?php
/**
 * @file
 * leanbox_master_content_types.features.menu_links.inc
 */

/**
 * Implements hook_menu_default_menu_links().
 */
function leanbox_master_content_types_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: menu-custom-menus_area-hook-group-report:area-hook-grouping.
  $menu_links['menu-custom-menus_area-hook-group-report:area-hook-grouping'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'area-hook-grouping',
    'router_path' => 'area-hook-grouping',
    'link_title' => 'Area Hook Group Report',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_area-hook-group-report:area-hook-grouping',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-custom-menus_master-reports:node/553239',
  );
  // Exported menu link: menu-custom-menus_outlet-area-hook:outlet-area-hook/import.
  $menu_links['menu-custom-menus_outlet-area-hook:outlet-area-hook/import'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'outlet-area-hook/import',
    'router_path' => 'outlet-area-hook/import',
    'link_title' => 'Outlet Area Hook',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_outlet-area-hook:outlet-area-hook/import',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-custom-menus_master-upload:node/24596',
  );
  // Exported menu link: menu-custom-menus_outlet-sequence:import/outlet_sequence.
  $menu_links['menu-custom-menus_outlet-sequence:import/outlet_sequence'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'import/outlet_sequence',
    'router_path' => 'import/%',
    'link_title' => 'Outlet Sequence',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_outlet-sequence:import/outlet_sequence',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-custom-menus_master-upload:node/24596',
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Area Hook Group Report');
  t('Outlet Area Hook');
  t('Outlet Sequence');

  return $menu_links;
}
