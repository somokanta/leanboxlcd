<?php
/**
 * @file
 * custom_menus.features.menu_links.inc
 */

/**
 * Implements hook_menu_default_menu_links().
 */
function custom_menus_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: menu-custom-menus_activity-reports:node/648811.
  $menu_links['menu-custom-menus_activity-reports:node/648811'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'node/648811',
    'router_path' => 'node/%',
    'link_title' => 'Activity Reports',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_activity-reports:node/648811',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 1,
    'weight' => -46,
    'customized' => 1,
  );
  // Exported menu link: menu-custom-menus_activity-upload:node/24597.
  $menu_links['menu-custom-menus_activity-upload:node/24597'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'node/24597',
    'router_path' => 'node/%',
    'link_title' => 'Activity Upload',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_activity-upload:node/24597',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 1,
    'weight' => -47,
    'customized' => 1,
  );
  // Exported menu link: menu-custom-menus_add-user:admin/people/create.
  $menu_links['menu-custom-menus_add-user:admin/people/create'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'admin/people/create',
    'router_path' => 'admin/people/create',
    'link_title' => 'Add User',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_add-user:admin/people/create',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
  );
  // Exported menu link: menu-custom-menus_billing-data:billing_data/import.
  $menu_links['menu-custom-menus_billing-data:billing_data/import'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'billing_data/import',
    'router_path' => 'billing_data/import',
    'link_title' => 'Billing Data',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_billing-data:billing_data/import',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -50,
    'customized' => 1,
    'parent_identifier' => 'menu-custom-menus_activity-upload:node/24597',
  );
  // Exported menu link: menu-custom-menus_billing-report:billing-report.
  $menu_links['menu-custom-menus_billing-report:billing-report'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'billing-report',
    'router_path' => 'billing-report',
    'link_title' => 'Billing report',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_billing-report:billing-report',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -46,
    'customized' => 1,
    'parent_identifier' => 'menu-custom-menus_transaction-report:node/648810',
  );
  // Exported menu link: menu-custom-menus_ccfot-report:ccfot-report.
  $menu_links['menu-custom-menus_ccfot-report:ccfot-report'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'ccfot-report',
    'router_path' => 'ccfot-report',
    'link_title' => 'CCFOT Report',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_ccfot-report:ccfot-report',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -50,
    'customized' => 1,
    'parent_identifier' => 'menu-custom-menus_activity-reports:node/648811',
  );
  // Exported menu link: menu-custom-menus_ccfot:import/ccfot.
  $menu_links['menu-custom-menus_ccfot:import/ccfot'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'import/ccfot',
    'router_path' => 'import/%',
    'link_title' => 'CCFOT',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_ccfot:import/ccfot',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -49,
    'customized' => 1,
    'parent_identifier' => 'menu-custom-menus_activity-upload:node/24597',
  );
  // Exported menu link: menu-custom-menus_channel-master:import/channel_master.
  $menu_links['menu-custom-menus_channel-master:import/channel_master'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'import/channel_master',
    'router_path' => 'import/%',
    'link_title' => 'CHANNEL Master',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_channel-master:import/channel_master',
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
  // Exported menu link: menu-custom-menus_channle-master-report:channle-master-report.
  $menu_links['menu-custom-menus_channle-master-report:channle-master-report'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'channle-master-report',
    'router_path' => 'channle-master-report',
    'link_title' => 'Channle Master Report',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_channle-master-report:channle-master-report',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -49,
    'customized' => 1,
    'parent_identifier' => 'menu-custom-menus_master-reports:node/553239',
  );
  // Exported menu link: menu-custom-menus_create-pricelist-group-master:node/add/pricelist-group-master.
  $menu_links['menu-custom-menus_create-pricelist-group-master:node/add/pricelist-group-master'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'node/add/pricelist-group-master',
    'router_path' => 'node/add/pricelist-group-master',
    'link_title' => 'Create Pricelist Group Master',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_create-pricelist-group-master:node/add/pricelist-group-master',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -48,
    'customized' => 1,
    'parent_identifier' => 'menu-custom-menus_master-upload:node/24596',
  );
  // Exported menu link: menu-custom-menus_create-product-division-master:node/add/product-division-master.
  $menu_links['menu-custom-menus_create-product-division-master:node/add/product-division-master'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'node/add/product-division-master',
    'router_path' => 'node/add/product-division-master',
    'link_title' => 'Create Product Division Master',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_create-product-division-master:node/add/product-division-master',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -47,
    'customized' => 1,
    'parent_identifier' => 'menu-custom-menus_master-upload:node/24596',
  );
  // Exported menu link: menu-custom-menus_create-resource-type:node/add/resource-type.
  $menu_links['menu-custom-menus_create-resource-type:node/add/resource-type'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'node/add/resource-type',
    'router_path' => 'node/add/resource-type',
    'link_title' => 'Create RESOURCE TYPE',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_create-resource-type:node/add/resource-type',
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
  // Exported menu link: menu-custom-menus_custom-billing-report:custom-billing-report.
  $menu_links['menu-custom-menus_custom-billing-report:custom-billing-report'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'custom-billing-report',
    'router_path' => 'custom-billing-report',
    'link_title' => 'Custom Billing Report',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_custom-billing-report:custom-billing-report',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -49,
    'customized' => 1,
    'parent_identifier' => 'menu-custom-menus_activity-reports:node/648811',
  );
  // Exported menu link: menu-custom-menus_dispatch-data-report:dispatch-data-report/view.
  $menu_links['menu-custom-menus_dispatch-data-report:dispatch-data-report/view'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'dispatch-data-report/view',
    'router_path' => 'dispatch-data-report/view',
    'link_title' => 'Dispatch Data Report',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_dispatch-data-report:dispatch-data-report/view',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -48,
    'customized' => 1,
    'parent_identifier' => 'menu-custom-menus_activity-reports:node/648811',
  );
  // Exported menu link: menu-custom-menus_dispatch-planning:admin/dispatch-planning.
  $menu_links['menu-custom-menus_dispatch-planning:admin/dispatch-planning'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'admin/dispatch-planning',
    'router_path' => 'admin/dispatch-planning',
    'link_title' => 'Dispatch Planning',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_dispatch-planning:admin/dispatch-planning',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -46,
    'customized' => 1,
    'parent_identifier' => 'menu-custom-menus_transactions:node/595899',
  );
  // Exported menu link: menu-custom-menus_drs-report:drs-report.
  $menu_links['menu-custom-menus_drs-report:drs-report'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'drs-report',
    'router_path' => 'drs-report',
    'link_title' => 'DRS Report',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_drs-report:drs-report',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -45,
    'customized' => 1,
    'parent_identifier' => 'menu-custom-menus_transaction-report:node/648810',
  );
  // Exported menu link: menu-custom-menus_egir:import/egir.
  $menu_links['menu-custom-menus_egir:import/egir'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'import/egir',
    'router_path' => 'import/%',
    'link_title' => 'EGIR',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_egir:import/egir',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -48,
    'customized' => 1,
    'parent_identifier' => 'menu-custom-menus_activity-upload:node/24597',
  );
  // Exported menu link: menu-custom-menus_holiday-master:import/holiday_master.
  $menu_links['menu-custom-menus_holiday-master:import/holiday_master'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'import/holiday_master',
    'router_path' => 'import/%',
    'link_title' => 'Holiday Master',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_holiday-master:import/holiday_master',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -44,
    'customized' => 1,
    'parent_identifier' => 'menu-custom-menus_master-upload:node/24596',
  );
  // Exported menu link: menu-custom-menus_home:node/24595.
  $menu_links['menu-custom-menus_home:node/24595'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'node/24595',
    'router_path' => 'node/%',
    'link_title' => 'Home',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_home:node/24595',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -50,
    'customized' => 1,
  );
  // Exported menu link: menu-custom-menus_hul-code-master:hul-code-master/view.
  $menu_links['menu-custom-menus_hul-code-master:hul-code-master/view'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'hul-code-master/view',
    'router_path' => 'hul-code-master/view',
    'link_title' => 'Hul Code Master',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_hul-code-master:hul-code-master/view',
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
  // Exported menu link: menu-custom-menus_hul-code-master:import/hul_code_master.
  $menu_links['menu-custom-menus_hul-code-master:import/hul_code_master'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'import/hul_code_master',
    'router_path' => 'import/%',
    'link_title' => 'HUL CODE MASTER',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_hul-code-master:import/hul_code_master',
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
  // Exported menu link: menu-custom-menus_impact-delivery-report:impact-delivery-report.
  $menu_links['menu-custom-menus_impact-delivery-report:impact-delivery-report'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'impact-delivery-report',
    'router_path' => 'impact-delivery-report',
    'link_title' => 'Impact Delivery Report',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_impact-delivery-report:impact-delivery-report',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -47,
    'customized' => 1,
    'parent_identifier' => 'menu-custom-menus_activity-reports:node/648811',
  );
  // Exported menu link: menu-custom-menus_impact-delivery-report:node/648810.
  $menu_links['menu-custom-menus_impact-delivery-report:node/648810'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'node/648810',
    'router_path' => 'node/%',
    'link_title' => 'Impact Delivery Report',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_impact-delivery-report:node/648810',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -50,
    'customized' => 1,
    'parent_identifier' => 'menu-custom-menus_transaction-report:node/648810',
  );
  // Exported menu link: menu-custom-menus_master-reports:node/553239.
  $menu_links['menu-custom-menus_master-reports:node/553239'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'node/553239',
    'router_path' => 'node/%',
    'link_title' => 'Master Reports',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_master-reports:node/553239',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 1,
    'weight' => -48,
    'customized' => 1,
  );
  // Exported menu link: menu-custom-menus_master-upload:node/24596.
  $menu_links['menu-custom-menus_master-upload:node/24596'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'node/24596',
    'router_path' => 'node/%',
    'link_title' => 'Master Upload',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_master-upload:node/24596',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 1,
    'weight' => -49,
    'customized' => 1,
  );
  // Exported menu link: menu-custom-menus_outlet-master-report:outlet-master-report.
  $menu_links['menu-custom-menus_outlet-master-report:outlet-master-report'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'outlet-master-report',
    'router_path' => 'outlet-master-report',
    'link_title' => 'Outlet Master Report',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_outlet-master-report:outlet-master-report',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -47,
    'customized' => 1,
    'parent_identifier' => 'menu-custom-menus_master-reports:node/553239',
  );
  // Exported menu link: menu-custom-menus_outlet-master:import/outlet_master.
  $menu_links['menu-custom-menus_outlet-master:import/outlet_master'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'import/outlet_master',
    'router_path' => 'import/%',
    'link_title' => 'Outlet Master',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_outlet-master:import/outlet_master',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -43,
    'customized' => 1,
    'parent_identifier' => 'menu-custom-menus_master-upload:node/24596',
  );
  // Exported menu link: menu-custom-menus_outstanding-activity-data:outstanding-activity/import.
  $menu_links['menu-custom-menus_outstanding-activity-data:outstanding-activity/import'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'outstanding-activity/import',
    'router_path' => 'outstanding-activity/import',
    'link_title' => 'Outstanding Activity Data',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_outstanding-activity-data:outstanding-activity/import',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -46,
    'customized' => 1,
    'parent_identifier' => 'menu-custom-menus_activity-upload:node/24597',
  );
  // Exported menu link: menu-custom-menus_party-pack-master-report:party-pack-master.
  $menu_links['menu-custom-menus_party-pack-master-report:party-pack-master'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'party-pack-master',
    'router_path' => 'party-pack-master',
    'link_title' => 'Party Pack Master Report',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_party-pack-master-report:party-pack-master',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -44,
    'customized' => 1,
    'parent_identifier' => 'menu-custom-menus_master-reports:node/553239',
  );
  // Exported menu link: menu-custom-menus_party-pack-master:import/party_pack_master.
  $menu_links['menu-custom-menus_party-pack-master:import/party_pack_master'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'import/party_pack_master',
    'router_path' => 'import/%',
    'link_title' => 'PARTY PACK Master',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_party-pack-master:import/party_pack_master',
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
  // Exported menu link: menu-custom-menus_party-pack-report:party-packing-report/view.
  $menu_links['menu-custom-menus_party-pack-report:party-packing-report/view'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'party-packing-report/view',
    'router_path' => 'party-packing-report/view',
    'link_title' => 'Party Pack Report',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_party-pack-report:party-packing-report/view',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -49,
    'customized' => 1,
    'parent_identifier' => 'menu-custom-menus_transactions:node/595899',
  );
  // Exported menu link: menu-custom-menus_pdd-master-report:pdd-master.
  $menu_links['menu-custom-menus_pdd-master-report:pdd-master'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'pdd-master',
    'router_path' => 'pdd-master',
    'link_title' => 'PDD Master Report',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_pdd-master-report:pdd-master',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -46,
    'customized' => 1,
    'parent_identifier' => 'menu-custom-menus_master-reports:node/553239',
  );
  // Exported menu link: menu-custom-menus_pdd-master:pdd/add.
  $menu_links['menu-custom-menus_pdd-master:pdd/add'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'pdd/add',
    'router_path' => 'pdd/add',
    'link_title' => 'PDD Master',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_pdd-master:pdd/add',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -37,
    'customized' => 1,
    'parent_identifier' => 'menu-custom-menus_master-upload:node/24596',
  );
  // Exported menu link: menu-custom-menus_pending-delivery-outstanding-report:pending-delivery-outstanding-report.
  $menu_links['menu-custom-menus_pending-delivery-outstanding-report:pending-delivery-outstanding-report'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'pending-delivery-outstanding-report',
    'router_path' => 'pending-delivery-outstanding-report',
    'link_title' => 'Pending Delivery Outstanding Report',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_pending-delivery-outstanding-report:pending-delivery-outstanding-report',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -49,
    'customized' => 1,
    'parent_identifier' => 'menu-custom-menus_transaction-report:node/648810',
  );
  // Exported menu link: menu-custom-menus_picking-report:picking-report/view.
  $menu_links['menu-custom-menus_picking-report:picking-report/view'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'picking-report/view',
    'router_path' => 'picking-report/view',
    'link_title' => 'Picking Report',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_picking-report:picking-report/view',
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
  // Exported menu link: menu-custom-menus_pl-group-master-report:pl-group-master-report.
  $menu_links['menu-custom-menus_pl-group-master-report:pl-group-master-report'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'pl-group-master-report',
    'router_path' => 'pl-group-master-report',
    'link_title' => 'PL Group Master Report',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_pl-group-master-report:pl-group-master-report',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -45,
    'customized' => 1,
    'parent_identifier' => 'menu-custom-menus_master-reports:node/553239',
  );
  // Exported menu link: menu-custom-menus_plgroup-master:import/pl_group_master.
  $menu_links['menu-custom-menus_plgroup-master:import/pl_group_master'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'import/pl_group_master',
    'router_path' => 'import/%',
    'link_title' => 'PL_GROUP Master',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_plgroup-master:import/pl_group_master',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -41,
    'customized' => 1,
    'parent_identifier' => 'menu-custom-menus_master-upload:node/24596',
  );
  // Exported menu link: menu-custom-menus_previous-outstanding-report:previous-outstanding-report.
  $menu_links['menu-custom-menus_previous-outstanding-report:previous-outstanding-report'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'previous-outstanding-report',
    'router_path' => 'previous-outstanding-report',
    'link_title' => 'Previous Outstanding report',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_previous-outstanding-report:previous-outstanding-report',
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
  // Exported menu link: menu-custom-menus_pricelist-group-master:pricelist-group-master/view.
  $menu_links['menu-custom-menus_pricelist-group-master:pricelist-group-master/view'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'pricelist-group-master/view',
    'router_path' => 'pricelist-group-master/view',
    'link_title' => 'Pricelist Group Master',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_pricelist-group-master:pricelist-group-master/view',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -43,
    'customized' => 1,
    'parent_identifier' => 'menu-custom-menus_master-reports:node/553239',
  );
  // Exported menu link: menu-custom-menus_product-division-master:product-division-master/view.
  $menu_links['menu-custom-menus_product-division-master:product-division-master/view'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'product-division-master/view',
    'router_path' => 'product-division-master/view',
    'link_title' => 'Product Division Master',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_product-division-master:product-division-master/view',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -42,
    'customized' => 1,
    'parent_identifier' => 'menu-custom-menus_master-reports:node/553239',
  );
  // Exported menu link: menu-custom-menus_resource-master-report:resource-master-report.
  $menu_links['menu-custom-menus_resource-master-report:resource-master-report'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'resource-master-report',
    'router_path' => 'resource-master-report',
    'link_title' => 'Resource Master Report',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_resource-master-report:resource-master-report',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -41,
    'customized' => 1,
    'parent_identifier' => 'menu-custom-menus_master-reports:node/553239',
  );
  // Exported menu link: menu-custom-menus_resource-master:import/resource_master.
  $menu_links['menu-custom-menus_resource-master:import/resource_master'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'import/resource_master',
    'router_path' => 'import/%',
    'link_title' => 'Resource Master',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_resource-master:import/resource_master',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -40,
    'customized' => 1,
    'parent_identifier' => 'menu-custom-menus_master-upload:node/24596',
  );
  // Exported menu link: menu-custom-menus_resource-type:resource-type/view.
  $menu_links['menu-custom-menus_resource-type:resource-type/view'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'resource-type/view',
    'router_path' => 'resource-type/view',
    'link_title' => 'Resource Type',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_resource-type:resource-type/view',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -40,
    'customized' => 1,
    'parent_identifier' => 'menu-custom-menus_master-reports:node/553239',
  );
  // Exported menu link: menu-custom-menus_return-bill:import/return_bill.
  $menu_links['menu-custom-menus_return-bill:import/return_bill'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'import/return_bill',
    'router_path' => 'import/%',
    'link_title' => 'Return Bill',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_return-bill:import/return_bill',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -44,
    'customized' => 1,
    'parent_identifier' => 'menu-custom-menus_activity-upload:node/24597',
  );
  // Exported menu link: menu-custom-menus_sales-register-data:sales-register/import.
  $menu_links['menu-custom-menus_sales-register-data:sales-register/import'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'sales-register/import',
    'router_path' => 'sales-register/import',
    'link_title' => 'Sales Register Data',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_sales-register-data:sales-register/import',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -45,
    'customized' => 1,
    'parent_identifier' => 'menu-custom-menus_activity-upload:node/24597',
  );
  // Exported menu link: menu-custom-menus_sku7-code-master-report:sku7-code-master-report.
  $menu_links['menu-custom-menus_sku7-code-master-report:sku7-code-master-report'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'sku7-code-master-report',
    'router_path' => 'sku7-code-master-report',
    'link_title' => 'sku7 Code Master Report',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_sku7-code-master-report:sku7-code-master-report',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -38,
    'customized' => 1,
    'parent_identifier' => 'menu-custom-menus_master-reports:node/553239',
  );
  // Exported menu link: menu-custom-menus_sku7code-master:import/sku7_code_master.
  $menu_links['menu-custom-menus_sku7code-master:import/sku7_code_master'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'import/sku7_code_master',
    'router_path' => 'import/%',
    'link_title' => 'SKU7_Code Master',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_sku7code-master:import/sku7_code_master',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -39,
    'customized' => 1,
    'parent_identifier' => 'menu-custom-menus_master-upload:node/24596',
  );
  // Exported menu link: menu-custom-menus_transaction-report:node/648810.
  $menu_links['menu-custom-menus_transaction-report:node/648810'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'node/648810',
    'router_path' => 'node/%',
    'link_title' => 'Transaction Report',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_transaction-report:node/648810',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 0,
    'weight' => -44,
    'customized' => 1,
  );
  // Exported menu link: menu-custom-menus_transaction-reports:node/648810.
  $menu_links['menu-custom-menus_transaction-reports:node/648810'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'node/648810',
    'router_path' => 'node/%',
    'link_title' => 'Transaction Reports',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_transaction-reports:node/648810',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -44,
    'customized' => 1,
    'parent_identifier' => 'menu-custom-menus_activity-reports:node/648811',
  );
  // Exported menu link: menu-custom-menus_transactions:node/595899.
  $menu_links['menu-custom-menus_transactions:node/595899'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'node/595899',
    'router_path' => 'node/%',
    'link_title' => 'Transactions',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_transactions:node/595899',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 1,
    'weight' => -45,
    'customized' => 1,
  );
  // Exported menu link: menu-custom-menus_unloading-report:unloading-form/view.
  $menu_links['menu-custom-menus_unloading-report:unloading-form/view'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'unloading-form/view',
    'router_path' => 'unloading-form/view',
    'link_title' => 'Unloading Report',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_unloading-report:unloading-form/view',
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
  // Exported menu link: menu-custom-menus_validation:admin/config/flat-table.
  $menu_links['menu-custom-menus_validation:admin/config/flat-table'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'admin/config/flat-table',
    'router_path' => 'admin/config/flat-table',
    'link_title' => 'Validation',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_validation:admin/config/flat-table',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -41,
    'customized' => 1,
  );
  // Exported menu link: menu-custom-menus_van-cld-report:van-cld-report.
  $menu_links['menu-custom-menus_van-cld-report:van-cld-report'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'van-cld-report',
    'router_path' => 'van-cld-report',
    'link_title' => 'Van Cld Report',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_van-cld-report:van-cld-report',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -43,
    'customized' => 1,
    'parent_identifier' => 'menu-custom-menus_activity-reports:node/648811',
  );
  // Exported menu link: menu-custom-menus_vehicle-master:import/vehicle_master.
  $menu_links['menu-custom-menus_vehicle-master:import/vehicle_master'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'import/vehicle_master',
    'router_path' => 'import/%',
    'link_title' => 'Vehicle Master',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_vehicle-master:import/vehicle_master',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -38,
    'customized' => 1,
    'parent_identifier' => 'menu-custom-menus_master-upload:node/24596',
  );
  // Exported menu link: menu-custom-menus_vehicle-master:vehicle-master/view.
  $menu_links['menu-custom-menus_vehicle-master:vehicle-master/view'] = array(
    'menu_name' => 'menu-custom-menus',
    'link_path' => 'vehicle-master/view',
    'router_path' => 'vehicle-master/view',
    'link_title' => 'Vehicle Master',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-custom-menus_vehicle-master:vehicle-master/view',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -39,
    'customized' => 1,
    'parent_identifier' => 'menu-custom-menus_master-reports:node/553239',
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Activity Reports');
  t('Activity Upload');
  t('Add User');
  t('Billing Data');
  t('Billing report');
  t('CCFOT');
  t('CCFOT Report');
  t('CHANNEL Master');
  t('Channle Master Report');
  t('Create Pricelist Group Master');
  t('Create Product Division Master');
  t('Create RESOURCE TYPE');
  t('Custom Billing Report');
  t('DRS Report');
  t('Dispatch Data Report');
  t('Dispatch Planning');
  t('EGIR');
  t('HUL CODE MASTER');
  t('Holiday Master');
  t('Home');
  t('Hul Code Master');
  t('Impact Delivery Report');
  t('Master Reports');
  t('Master Upload');
  t('Outlet Master');
  t('Outlet Master Report');
  t('Outstanding Activity Data');
  t('PARTY PACK Master');
  t('PDD Master');
  t('PDD Master Report');
  t('PL Group Master Report');
  t('PL_GROUP Master');
  t('Party Pack Master Report');
  t('Party Pack Report');
  t('Pending Delivery Outstanding Report');
  t('Picking Report');
  t('Previous Outstanding report');
  t('Pricelist Group Master');
  t('Product Division Master');
  t('Resource Master');
  t('Resource Master Report');
  t('Resource Type');
  t('Return Bill');
  t('SKU7_Code Master');
  t('Sales Register Data');
  t('Transaction Report');
  t('Transaction Reports');
  t('Transactions');
  t('Unloading Report');
  t('Validation');
  t('Van Cld Report');
  t('Vehicle Master');
  t('sku7 Code Master Report');

  return $menu_links;
}
