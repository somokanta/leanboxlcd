<?php
/**
 * @file
 * user_manual_feature.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function user_manual_feature_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'create user_manual content'.
  $permissions['create user_manual content'] = array(
    'name' => 'create user_manual content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any user_manual content'.
  $permissions['delete any user_manual content'] = array(
    'name' => 'delete any user_manual content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'delete own user_manual content'.
  $permissions['delete own user_manual content'] = array(
    'name' => 'delete own user_manual content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any user_manual content'.
  $permissions['edit any user_manual content'] = array(
    'name' => 'edit any user_manual content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'edit own user_manual content'.
  $permissions['edit own user_manual content'] = array(
    'name' => 'edit own user_manual content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  return $permissions;
}
