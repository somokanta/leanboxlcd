<?php
/**
 * @file
 * new_custom_menus.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function new_custom_menus_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'Distributor Upload File'.
  $permissions['Distributor Upload File'] = array(
    'name' => 'Distributor Upload File',
    'roles' => array(
      'DFE' => 'DFE',
      'Distributor' => 'Distributor',
      'RS Admin' => 'RS Admin',
      'RS Operator' => 'RS Operator',
      'Uploader' => 'Uploader',
      'administrator' => 'administrator',
      'hul_admin' => 'hul_admin',
    ),
    'module' => 'change_feeds_import',
  );

  // Exported permission: 'Distributor Upload File DFE and RS admin wise'.
  $permissions['Distributor Upload File DFE and RS admin wise'] = array(
    'name' => 'Distributor Upload File DFE and RS admin wise',
    'roles' => array(
      'DFE' => 'DFE',
      'Distributor' => 'Distributor',
      'RS Admin' => 'RS Admin',
      'RS Operator' => 'RS Operator',
      'Uploader' => 'Uploader',
      'administrator' => 'administrator',
    ),
    'module' => 'change_feeds_import',
  );

  // Exported permission: 'dispatch planning screen'.
  $permissions['dispatch planning screen'] = array(
    'name' => 'dispatch planning screen',
    'roles' => array(
      'DFE' => 'DFE',
      'Distributor' => 'Distributor',
      'RS Admin' => 'RS Admin',
      'RS Operator' => 'RS Operator',
    ),
    'module' => 'dispatch_planning',
  );

  // Exported permission: 'dispatch planning screen-rs-admin'.
  $permissions['dispatch planning screen-rs-admin'] = array(
    'name' => 'dispatch planning screen-rs-admin',
    'roles' => array(
      'Distributor' => 'Distributor',
      'RS Admin' => 'RS Admin',
    ),
    'module' => 'dispatch_planning',
  );

  // Exported permission: 'import area_hook_master feeds'.
  $permissions['import area_hook_master feeds'] = array(
    'name' => 'import area_hook_master feeds',
    'roles' => array(
      'DFE' => 'DFE',
      'Distributor' => 'Distributor',
      'RS Admin' => 'RS Admin',
      'RS Operator' => 'RS Operator',
      'Uploader' => 'Uploader',
    ),
    'module' => 'feeds',
  );

  // Exported permission: 'import channel_master feeds'.
  $permissions['import channel_master feeds'] = array(
    'name' => 'import channel_master feeds',
    'roles' => array(
      'DFE' => 'DFE',
      'Distributor' => 'Distributor',
      'RS Admin' => 'RS Admin',
      'RS Operator' => 'RS Operator',
      'Uploader' => 'Uploader',
    ),
    'module' => 'feeds',
  );

  // Exported permission: 'import egir feeds'.
  $permissions['import egir feeds'] = array(
    'name' => 'import egir feeds',
    'roles' => array(
      'DFE' => 'DFE',
      'Distributor' => 'Distributor',
      'RS Admin' => 'RS Admin',
      'RS Operator' => 'RS Operator',
      'Uploader' => 'Uploader',
    ),
    'module' => 'feeds',
  );

  // Exported permission: 'import hht_importer feeds'.
  $permissions['import hht_importer feeds'] = array(
    'name' => 'import hht_importer feeds',
    'roles' => array(
      'DFE' => 'DFE',
      'Distributor' => 'Distributor',
      'RS Admin' => 'RS Admin',
      'RS Operator' => 'RS Operator',
      'Uploader' => 'Uploader',
    ),
    'module' => 'feeds',
  );

  // Exported permission: 'import hul_code_master feeds'.
  $permissions['import hul_code_master feeds'] = array(
    'name' => 'import hul_code_master feeds',
    'roles' => array(
      'DFE' => 'DFE',
      'Distributor' => 'Distributor',
      'RS Admin' => 'RS Admin',
      'RS Operator' => 'RS Operator',
      'Uploader' => 'Uploader',
    ),
    'module' => 'feeds',
  );

  // Exported permission: 'import outlet_master feeds'.
  $permissions['import outlet_master feeds'] = array(
    'name' => 'import outlet_master feeds',
    'roles' => array(
      'DFE' => 'DFE',
      'Distributor' => 'Distributor',
      'RS Admin' => 'RS Admin',
      'RS Operator' => 'RS Operator',
      'Uploader' => 'Uploader',
    ),
    'module' => 'feeds',
  );

  // Exported permission: 'import outlet_sequence feeds'.
  $permissions['import outlet_sequence feeds'] = array(
    'name' => 'import outlet_sequence feeds',
    'roles' => array(
      'DFE' => 'DFE',
      'Distributor' => 'Distributor',
      'RS Admin' => 'RS Admin',
      'RS Operator' => 'RS Operator',
      'Uploader' => 'Uploader',
    ),
    'module' => 'feeds',
  );

  // Exported permission: 'import party_pack_master feeds'.
  $permissions['import party_pack_master feeds'] = array(
    'name' => 'import party_pack_master feeds',
    'roles' => array(
      'DFE' => 'DFE',
      'Distributor' => 'Distributor',
      'RS Admin' => 'RS Admin',
      'RS Operator' => 'RS Operator',
      'Uploader' => 'Uploader',
    ),
    'module' => 'feeds',
  );

  // Exported permission: 'import pl_group_master feeds'.
  $permissions['import pl_group_master feeds'] = array(
    'name' => 'import pl_group_master feeds',
    'roles' => array(
      'DFE' => 'DFE',
      'Distributor' => 'Distributor',
      'RS Admin' => 'RS Admin',
      'RS Operator' => 'RS Operator',
      'Uploader' => 'Uploader',
    ),
    'module' => 'feeds',
  );

  // Exported permission: 'import resource_master feeds'.
  $permissions['import resource_master feeds'] = array(
    'name' => 'import resource_master feeds',
    'roles' => array(
      'DFE' => 'DFE',
      'Distributor' => 'Distributor',
      'RS Admin' => 'RS Admin',
      'RS Operator' => 'RS Operator',
      'Uploader' => 'Uploader',
      'leanbox' => 'leanbox',
    ),
    'module' => 'feeds',
  );

  // Exported permission: 'import sales_delivery_day_importer feeds'.
  $permissions['import sales_delivery_day_importer feeds'] = array(
    'name' => 'import sales_delivery_day_importer feeds',
    'roles' => array(
      'DFE' => 'DFE',
      'Distributor' => 'Distributor',
      'RS Admin' => 'RS Admin',
      'RS Operator' => 'RS Operator',
      'Uploader' => 'Uploader',
    ),
    'module' => 'feeds',
  );

  // Exported permission: 'import sku7_code_master feeds'.
  $permissions['import sku7_code_master feeds'] = array(
    'name' => 'import sku7_code_master feeds',
    'roles' => array(
      'DFE' => 'DFE',
      'Distributor' => 'Distributor',
      'RS Admin' => 'RS Admin',
      'RS Operator' => 'RS Operator',
      'Uploader' => 'Uploader',
    ),
    'module' => 'feeds',
  );

  // Exported permission: 'import user_defined_outlet_sequence feeds'.
  $permissions['import user_defined_outlet_sequence feeds'] = array(
    'name' => 'import user_defined_outlet_sequence feeds',
    'roles' => array(
      'DFE' => 'DFE',
      'Distributor' => 'Distributor',
      'RS Admin' => 'RS Admin',
      'RS Operator' => 'RS Operator',
      'Uploader' => 'Uploader',
    ),
    'module' => 'feeds',
  );

  // Exported permission: 'import vehicle_master feeds'.
  $permissions['import vehicle_master feeds'] = array(
    'name' => 'import vehicle_master feeds',
    'roles' => array(
      'DFE' => 'DFE',
      'Distributor' => 'Distributor',
      'RS Admin' => 'RS Admin',
      'RS Operator' => 'RS Operator',
      'Uploader' => 'Uploader',
      'administrator' => 'administrator',
    ),
    'module' => 'feeds',
  );

  // Exported permission: 'only-rs-admin-with-distributor'.
  $permissions['only-rs-admin-with-distributor'] = array(
    'name' => 'only-rs-admin-with-distributor',
    'roles' => array(
      'DFE' => 'DFE',
      'RS Admin' => 'RS Admin',
      'administrator' => 'administrator',
    ),
    'module' => 'dispatch_planning',
  );

  // Exported permission: 'unlock area_hook_master feeds'.
  $permissions['unlock area_hook_master feeds'] = array(
    'name' => 'unlock area_hook_master feeds',
    'roles' => array(
      'DFE' => 'DFE',
      'Distributor' => 'Distributor',
      'RS Admin' => 'RS Admin',
      'RS Operator' => 'RS Operator',
      'Uploader' => 'Uploader',
    ),
    'module' => 'feeds',
  );

  // Exported permission: 'unlock egir feeds'.
  $permissions['unlock egir feeds'] = array(
    'name' => 'unlock egir feeds',
    'roles' => array(
      'DFE' => 'DFE',
      'Distributor' => 'Distributor',
      'RS Admin' => 'RS Admin',
      'RS Operator' => 'RS Operator',
      'Uploader' => 'Uploader',
    ),
    'module' => 'feeds',
  );

  // Exported permission: 'unlock hht_importer feeds'.
  $permissions['unlock hht_importer feeds'] = array(
    'name' => 'unlock hht_importer feeds',
    'roles' => array(
      'DFE' => 'DFE',
      'Distributor' => 'Distributor',
      'RS Admin' => 'RS Admin',
      'RS Operator' => 'RS Operator',
      'Uploader' => 'Uploader',
    ),
    'module' => 'feeds',
  );

  // Exported permission: 'unlock hul_code_master feeds'.
  $permissions['unlock hul_code_master feeds'] = array(
    'name' => 'unlock hul_code_master feeds',
    'roles' => array(
      'DFE' => 'DFE',
      'Distributor' => 'Distributor',
      'RS Admin' => 'RS Admin',
      'RS Operator' => 'RS Operator',
      'Uploader' => 'Uploader',
    ),
    'module' => 'feeds',
  );

  // Exported permission: 'unlock outlet_master feeds'.
  $permissions['unlock outlet_master feeds'] = array(
    'name' => 'unlock outlet_master feeds',
    'roles' => array(
      'DFE' => 'DFE',
      'Distributor' => 'Distributor',
      'RS Admin' => 'RS Admin',
      'RS Operator' => 'RS Operator',
      'Uploader' => 'Uploader',
    ),
    'module' => 'feeds',
  );

  // Exported permission: 'unlock outlet_sequence feeds'.
  $permissions['unlock outlet_sequence feeds'] = array(
    'name' => 'unlock outlet_sequence feeds',
    'roles' => array(
      'DFE' => 'DFE',
      'Distributor' => 'Distributor',
      'RS Admin' => 'RS Admin',
      'RS Operator' => 'RS Operator',
      'Uploader' => 'Uploader',
    ),
    'module' => 'feeds',
  );

  // Exported permission: 'unlock party_pack_master feeds'.
  $permissions['unlock party_pack_master feeds'] = array(
    'name' => 'unlock party_pack_master feeds',
    'roles' => array(
      'DFE' => 'DFE',
      'Distributor' => 'Distributor',
      'RS Admin' => 'RS Admin',
      'RS Operator' => 'RS Operator',
      'Uploader' => 'Uploader',
    ),
    'module' => 'feeds',
  );

  // Exported permission: 'unlock resource_master feeds'.
  $permissions['unlock resource_master feeds'] = array(
    'name' => 'unlock resource_master feeds',
    'roles' => array(
      'DFE' => 'DFE',
      'Distributor' => 'Distributor',
      'RS Admin' => 'RS Admin',
      'RS Operator' => 'RS Operator',
      'Uploader' => 'Uploader',
      'leanbox' => 'leanbox',
    ),
    'module' => 'feeds',
  );

  // Exported permission: 'unlock sales_delivery_day_importer feeds'.
  $permissions['unlock sales_delivery_day_importer feeds'] = array(
    'name' => 'unlock sales_delivery_day_importer feeds',
    'roles' => array(
      'Distributor' => 'Distributor',
      'RS Admin' => 'RS Admin',
      'RS Operator' => 'RS Operator',
      'Uploader' => 'Uploader',
    ),
    'module' => 'feeds',
  );

  // Exported permission: 'unlock sku7_code_master feeds'.
  $permissions['unlock sku7_code_master feeds'] = array(
    'name' => 'unlock sku7_code_master feeds',
    'roles' => array(
      'DFE' => 'DFE',
      'Distributor' => 'Distributor',
      'RS Admin' => 'RS Admin',
      'RS Operator' => 'RS Operator',
      'Uploader' => 'Uploader',
    ),
    'module' => 'feeds',
  );

  // Exported permission: 'unlock user_defined_outlet_sequence feeds'.
  $permissions['unlock user_defined_outlet_sequence feeds'] = array(
    'name' => 'unlock user_defined_outlet_sequence feeds',
    'roles' => array(
      'DFE' => 'DFE',
      'Distributor' => 'Distributor',
      'RS Admin' => 'RS Admin',
      'RS Operator' => 'RS Operator',
      'Uploader' => 'Uploader',
    ),
    'module' => 'feeds',
  );

  // Exported permission: 'unlock vehicle_master feeds'.
  $permissions['unlock vehicle_master feeds'] = array(
    'name' => 'unlock vehicle_master feeds',
    'roles' => array(
      'DFE' => 'DFE',
      'Distributor' => 'Distributor',
      'RS Admin' => 'RS Admin',
      'RS Operator' => 'RS Operator',
      'Uploader' => 'Uploader',
      'administrator' => 'administrator',
    ),
    'module' => 'feeds',
  );

  return $permissions;
}
