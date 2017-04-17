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

  // Exported menu link: menu-custom-menus_address-confirmation:hul-locations.
  $menu_links['menu-custom-menus_address-confirmation:hul-locations'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'hul-locations',
    'router_path' => 'hul-locations',
    'link_title' => 'Address Confirmation',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_address-confirmation:hul-locations',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -50,
    'customized' => 1,
  );
  // Exported menu link: menu-custom-menus_area-hook-group-import:area-hook-group/import.
  $menu_links['menu-custom-menus_area-hook-group-import:area-hook-group/import'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'area-hook-group/import',
    'router_path' => 'area-hook-group/import',
    'link_title' => 'Area Hook Group Import',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_area-hook-group-import:area-hook-group/import',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
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
    'weight' => 0,
    'customized' => 1,
  );
  // Exported menu link: menu-custom-menus_area-hook-master-import:import/area_hook_master.
  $menu_links['menu-custom-menus_area-hook-master-import:import/area_hook_master'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'import/area_hook_master',
    'router_path' => 'import/%',
    'link_title' => 'Area Hook Master Import',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_area-hook-master-import:import/area_hook_master',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
  );
  // Exported menu link: menu-custom-menus_channel-pl-group-master-report:import/party_pack_master.
  $menu_links['menu-custom-menus_channel-pl-group-master-report:import/party_pack_master'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'import/party_pack_master',
    'router_path' => 'import/%',
    'link_title' => 'Channel PL Group Master Report',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_channel-pl-group-master-report:import/party_pack_master',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -46,
    'customized' => 1,
  );
  // Exported menu link: menu-custom-menus_delivery-per-van-upload:delivery-per-van/import.
  $menu_links['menu-custom-menus_delivery-per-van-upload:delivery-per-van/import'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'delivery-per-van/import',
    'router_path' => 'delivery-per-van/import',
    'link_title' => 'Delivery Per van Upload',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_delivery-per-van-upload:delivery-per-van/import',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
  );
  // Exported menu link: menu-custom-menus_multiple-address-confirmation:hul-locations-multi.
  $menu_links['menu-custom-menus_multiple-address-confirmation:hul-locations-multi'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'hul-locations-multi',
    'router_path' => 'hul-locations-multi',
    'link_title' => 'Multiple Address Confirmation',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_multiple-address-confirmation:hul-locations-multi',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -48,
    'customized' => 1,
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
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Address Confirmation');
  t('Area Hook Group Import');
  t('Area Hook Group Report');
  t('Area Hook Master Import');
  t('Channel PL Group Master Report');
  t('Delivery Per van Upload');
  t('Multiple Address Confirmation');
  t('Multiple Area Group Confirmation');
  t('Outlet Area Hook');
  t('Outlet Sequence');
  t('View Area Group Confirmation');

  return $menu_links;
}
