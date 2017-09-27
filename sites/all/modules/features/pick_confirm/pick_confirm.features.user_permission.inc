<?php
/**
 * @file
 * pick_confirm.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function pick_confirm_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'access_for_api'.
  $permissions['access_for_api'] = array(
    'name' => 'access_for_api',
    'roles' => array(
      'DFE' => 'DFE',
      'Distributor' => 'Distributor',
      'RS Admin' => 'RS Admin',
      'RS Operator' => 'RS Operator',
      'Uploader' => 'Uploader',
      'administrator' => 'administrator',
      'leanbox' => 'leanbox',
    ),
    'module' => 'picking_type_combined',
  );

  // Exported permission: 'create pick_confirm content'.
  $permissions['create pick_confirm content'] = array(
    'name' => 'create pick_confirm content',
    'roles' => array(
      'leanbox' => 'leanbox',
    ),
    'module' => 'node',
  );

  // Exported permission: 'create resource_master content'.
  $permissions['create resource_master content'] = array(
    'name' => 'create resource_master content',
    'roles' => array(
      'DFE' => 'DFE',
      'Distributor' => 'Distributor',
      'RS Admin' => 'RS Admin',
      'RS Operator' => 'RS Operator',
      'Uploader' => 'Uploader',
      'administrator' => 'administrator',
      'leanbox' => 'leanbox',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own pick_confirm content'.
  $permissions['edit own pick_confirm content'] = array(
    'name' => 'edit own pick_confirm content',
    'roles' => array(
      'leanbox' => 'leanbox',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own resource_master content'.
  $permissions['edit own resource_master content'] = array(
    'name' => 'edit own resource_master content',
    'roles' => array(
      'DFE' => 'DFE',
      'Distributor' => 'Distributor',
      'RS Admin' => 'RS Admin',
      'RS Operator' => 'RS Operator',
      'Uploader' => 'Uploader',
      'administrator' => 'administrator',
      'leanbox' => 'leanbox',
    ),
    'module' => 'node',
  );

  return $permissions;
}
