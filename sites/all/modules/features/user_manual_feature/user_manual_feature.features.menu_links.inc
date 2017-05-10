<?php
/**
 * @file
 * user_manual_feature.features.menu_links.inc
 */

/**
 * Implements hook_menu_default_menu_links().
 */
function user_manual_feature_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: menu-custom-menus_area-hook-wise-outlet:area-hook-confirm-view.
  $menu_links['menu-custom-menus_area-hook-wise-outlet:area-hook-confirm-view'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'area-hook-confirm-view',
    'router_path' => 'area-hook-confirm-view',
    'link_title' => 'Area Hook Wise Outlet',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_area-hook-wise-outlet:area-hook-confirm-view',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -48,
    'customized' => 1,
    'parent_identifier' => 'menu-custom-menus_transactions:node/595899',
  );
  // Exported menu link: menu-custom-menus_user-manual:user-manual.
  $menu_links['menu-custom-menus_user-manual:user-manual'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'user-manual',
    'router_path' => 'user-manual',
    'link_title' => 'User Manual',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_user-manual:user-manual',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Area Hook Wise Outlet');
  t('User Manual');

  return $menu_links;
}
