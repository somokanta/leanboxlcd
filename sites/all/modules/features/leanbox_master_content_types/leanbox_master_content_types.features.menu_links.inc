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
    'weight' => -50,
    'customized' => 1,
    'parent_identifier' => 'menu-custom-menus_master-reports:node/553239',
  );
  // Exported menu link: menu-custom-menus_day-van-pl-gp-area-hook-grouping:day-van-area/import.
  $menu_links['menu-custom-menus_day-van-pl-gp-area-hook-grouping:day-van-area/import'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'day-van-area/import',
    'router_path' => 'day-van-area/import',
    'link_title' => 'Day-Van-PL-Gp-Area Hook Grouping',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_day-van-pl-gp-area-hook-grouping:day-van-area/import',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -45,
    'customized' => 1,
    'parent_identifier' => 'menu-custom-menus_master-upload:node/24596',
  );
  // Exported menu link: menu-custom-menus_lean-outlet-sequence:import/outlet_sequence.
  $menu_links['menu-custom-menus_lean-outlet-sequence:import/outlet_sequence'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'import/outlet_sequence',
    'router_path' => 'import/%',
    'link_title' => 'Lean Outlet Sequence',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_lean-outlet-sequence:import/outlet_sequence',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -46,
    'customized' => 1,
    'parent_identifier' => 'menu-custom-menus_master-upload:node/24596',
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
    'weight' => -42,
    'customized' => 1,
    'parent_identifier' => 'menu-custom-menus_master-upload:node/24596',
  );
  // Exported menu link: menu-custom-menus_static-beat-creation:delivery-beat.
  $menu_links['menu-custom-menus_static-beat-creation:delivery-beat'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'delivery-beat',
    'router_path' => 'delivery-beat',
    'link_title' => 'Static Beat Creation',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_static-beat-creation:delivery-beat',
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
  // Exported menu link: menu-custom-menus_user-defined-outlet-sequence:import/user_defined_outlet_sequence.
  $menu_links['menu-custom-menus_user-defined-outlet-sequence:import/user_defined_outlet_sequence'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'import/user_defined_outlet_sequence',
    'router_path' => 'import/%',
    'link_title' => 'User Defined Outlet Sequence',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_user-defined-outlet-sequence:import/user_defined_outlet_sequence',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -33,
    'customized' => 1,
    'parent_identifier' => 'menu-custom-menus_master-upload:node/24596',
  );
  // Exported menu link: menu-custom-menus_van-stop-master:van-stop-master.
  $menu_links['menu-custom-menus_van-stop-master:van-stop-master'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'van-stop-master',
    'router_path' => 'van-stop-master',
    'link_title' => 'Van-Stop Master',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_van-stop-master:van-stop-master',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -31,
    'customized' => 1,
    'parent_identifier' => 'menu-custom-menus_master-upload:node/24596',
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Area Hook Group Report');
  t('Day-Van-PL-Gp-Area Hook Grouping');
  t('Lean Outlet Sequence');
  t('Outlet Area Hook');
  t('Static Beat Creation');
  t('User Defined Outlet Sequence');
  t('Van-Stop Master');

  return $menu_links;
}
