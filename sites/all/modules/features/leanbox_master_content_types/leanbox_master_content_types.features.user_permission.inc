<?php
/**
 * @file
 * leanbox_master_content_types.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function leanbox_master_content_types_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'create hul_code_master content'.
  $permissions['create hul_code_master content'] = array(
    'name' => 'create hul_code_master content',
    'roles' => array(
      'Distributor' => 'Distributor',
      'Uploader' => 'Uploader',
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'create pricelist_group_master content'.
  $permissions['create pricelist_group_master content'] = array(
    'name' => 'create pricelist_group_master content',
    'roles' => array(
      'Distributor' => 'Distributor',
      'Uploader' => 'Uploader',
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'create product_division_master content'.
  $permissions['create product_division_master content'] = array(
    'name' => 'create product_division_master content',
    'roles' => array(
      'Distributor' => 'Distributor',
      'Uploader' => 'Uploader',
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'create resource_type content'.
  $permissions['create resource_type content'] = array(
    'name' => 'create resource_type content',
    'roles' => array(
      'Distributor' => 'Distributor',
      'Uploader' => 'Uploader',
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'create vehicle_master content'.
  $permissions['create vehicle_master content'] = array(
    'name' => 'create vehicle_master content',
    'roles' => array(
      'Distributor' => 'Distributor',
      'Uploader' => 'Uploader',
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any hul_code_master content'.
  $permissions['delete any hul_code_master content'] = array(
    'name' => 'delete any hul_code_master content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'delete any pricelist_group_master content'.
  $permissions['delete any pricelist_group_master content'] = array(
    'name' => 'delete any pricelist_group_master content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'delete any product_division_master content'.
  $permissions['delete any product_division_master content'] = array(
    'name' => 'delete any product_division_master content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'delete any resource_type content'.
  $permissions['delete any resource_type content'] = array(
    'name' => 'delete any resource_type content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'delete any vehicle_master content'.
  $permissions['delete any vehicle_master content'] = array(
    'name' => 'delete any vehicle_master content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'delete own hul_code_master content'.
  $permissions['delete own hul_code_master content'] = array(
    'name' => 'delete own hul_code_master content',
    'roles' => array(
      'Distributor' => 'Distributor',
      'Uploader' => 'Uploader',
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own pricelist_group_master content'.
  $permissions['delete own pricelist_group_master content'] = array(
    'name' => 'delete own pricelist_group_master content',
    'roles' => array(
      'Distributor' => 'Distributor',
      'Uploader' => 'Uploader',
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own product_division_master content'.
  $permissions['delete own product_division_master content'] = array(
    'name' => 'delete own product_division_master content',
    'roles' => array(
      'Distributor' => 'Distributor',
      'Uploader' => 'Uploader',
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own resource_type content'.
  $permissions['delete own resource_type content'] = array(
    'name' => 'delete own resource_type content',
    'roles' => array(
      'Distributor' => 'Distributor',
      'Uploader' => 'Uploader',
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own vehicle_master content'.
  $permissions['delete own vehicle_master content'] = array(
    'name' => 'delete own vehicle_master content',
    'roles' => array(
      'Distributor' => 'Distributor',
      'Uploader' => 'Uploader',
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any hul_code_master content'.
  $permissions['edit any hul_code_master content'] = array(
    'name' => 'edit any hul_code_master content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'edit any pricelist_group_master content'.
  $permissions['edit any pricelist_group_master content'] = array(
    'name' => 'edit any pricelist_group_master content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'edit any product_division_master content'.
  $permissions['edit any product_division_master content'] = array(
    'name' => 'edit any product_division_master content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'edit any resource_type content'.
  $permissions['edit any resource_type content'] = array(
    'name' => 'edit any resource_type content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'edit any vehicle_master content'.
  $permissions['edit any vehicle_master content'] = array(
    'name' => 'edit any vehicle_master content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'edit own hul_code_master content'.
  $permissions['edit own hul_code_master content'] = array(
    'name' => 'edit own hul_code_master content',
    'roles' => array(
      'Distributor' => 'Distributor',
      'Uploader' => 'Uploader',
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own pricelist_group_master content'.
  $permissions['edit own pricelist_group_master content'] = array(
    'name' => 'edit own pricelist_group_master content',
    'roles' => array(
      'Distributor' => 'Distributor',
      'Uploader' => 'Uploader',
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own product_division_master content'.
  $permissions['edit own product_division_master content'] = array(
    'name' => 'edit own product_division_master content',
    'roles' => array(
      'Distributor' => 'Distributor',
      'Uploader' => 'Uploader',
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own resource_type content'.
  $permissions['edit own resource_type content'] = array(
    'name' => 'edit own resource_type content',
    'roles' => array(
      'Distributor' => 'Distributor',
      'Uploader' => 'Uploader',
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own vehicle_master content'.
  $permissions['edit own vehicle_master content'] = array(
    'name' => 'edit own vehicle_master content',
    'roles' => array(
      'Distributor' => 'Distributor',
      'Uploader' => 'Uploader',
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  return $permissions;
}
