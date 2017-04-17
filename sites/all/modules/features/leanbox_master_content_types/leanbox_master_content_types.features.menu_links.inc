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
  // Exported menu link: menu-custom-menus_multiple-area-group-confirmation:area-confirm.
  $menu_links['menu-custom-menus_multiple-area-group-confirmation:area-confirm'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'area-confirm',
    'router_path' => 'area-confirm',
    'link_title' => 'Multiple Area Group Confirmation',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_multiple-area-group-confirmation:area-confirm',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -47,
    'customized' => 1,
    'parent_identifier' => 'menu-custom-menus_transactions:node/595899',
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
  // Exported menu link: menu-custom-menus_view-area-group-confirmation:area-confirm-view.
  $menu_links['menu-custom-menus_view-area-group-confirmation:area-confirm-view'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'area-confirm-view',
    'router_path' => 'area-confirm-view',
    'link_title' => 'View Area Group Confirmation',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_view-area-group-confirmation:area-confirm-view',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -43,
    'customized' => 1,
    'parent_identifier' => 'menu-custom-menus_transactions:node/595899',
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Area Hook Group Report');
  t('Multiple Area Group Confirmation');
  t('Outlet Area Hook');
  t('Outlet Sequence');
  t('View Area Group Confirmation');

  return $menu_links;
}
