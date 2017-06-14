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

  // Exported menu link: menu-custom-menus_area-hook-group-master:area-hook-grp/import.
  $menu_links['menu-custom-menus_area-hook-group-master:area-hook-grp/import'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'area-hook-grp/import',
    'router_path' => 'area-hook-grp/import',
    'link_title' => 'Area Hook Group Master',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_area-hook-group-master:area-hook-grp/import',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -50,
    'customized' => 1,
    'parent_identifier' => 'menu-custom-menus_master-upload:node/24596',
  );
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
    'weight' => -48,
    'customized' => 1,
    'parent_identifier' => 'menu-custom-menus_master-reports:node/553239',
  );
  // Exported menu link: menu-custom-menus_drs-printout-2:drs-report2.
  $menu_links['menu-custom-menus_drs-printout-2:drs-report2'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'drs-report2',
    'router_path' => 'drs-report2',
    'link_title' => 'DRS Printout-2',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_drs-printout-2:drs-report2',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -48,
    'customized' => 1,
    'parent_identifier' => 'menu-custom-menus_transaction-report:node/648810',
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
    'weight' => -42,
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

  // Translatables
  // Included for use with string extractors like potx.
  t('Area Hook Group Master');
  t('Area Hook Group Report');
  t('DRS Printout-2');
  t('Outlet Area Hook');
  t('Static Beat Creation');
  t('User Defined Outlet Sequence');

  return $menu_links;
}
