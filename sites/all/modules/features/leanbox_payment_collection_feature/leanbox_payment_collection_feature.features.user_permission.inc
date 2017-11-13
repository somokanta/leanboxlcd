<?php
/**
 * @file
 * leanbox_payment_collection_feature.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function leanbox_payment_collection_feature_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'create payment_collection content'.
  $permissions['create payment_collection content'] = array(
    'name' => 'create payment_collection content',
    'roles' => array(
      'leanbox_collection' => 'leanbox_collection',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any payment_collection content'.
  $permissions['delete any payment_collection content'] = array(
    'name' => 'delete any payment_collection content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'delete own payment_collection content'.
  $permissions['delete own payment_collection content'] = array(
    'name' => 'delete own payment_collection content',
    'roles' => array(
      'leanbox_collection' => 'leanbox_collection',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any payment_collection content'.
  $permissions['edit any payment_collection content'] = array(
    'name' => 'edit any payment_collection content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'edit own payment_collection content'.
  $permissions['edit own payment_collection content'] = array(
    'name' => 'edit own payment_collection content',
    'roles' => array(
      'leanbox_collection' => 'leanbox_collection',
    ),
    'module' => 'node',
  );

  // Exported permission: 'upload payment collection'.
  $permissions['upload payment collection'] = array(
    'name' => 'upload payment collection',
    'roles' => array(
      'administrator' => 'administrator',
      'leanbox_collection' => 'leanbox_collection',
    ),
    'module' => 'leanbox_payment_collection',
  );

  return $permissions;
}
