<?php
/**
 * @file
 * day_van_area_grouping.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function day_van_area_grouping_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'create area_hook_group_master content'.
  $permissions['create area_hook_group_master content'] = array(
    'name' => 'create area_hook_group_master content',
    'roles' => array(
      'Distributor' => 'Distributor',
      'Uploader' => 'Uploader',
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'create day_van_pl_gp_area_hook_grouping content'.
  $permissions['create day_van_pl_gp_area_hook_grouping content'] = array(
    'name' => 'create day_van_pl_gp_area_hook_grouping content',
    'roles' => array(
      'Distributor' => 'Distributor',
      'Uploader' => 'Uploader',
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any area_hook_group_master content'.
  $permissions['delete any area_hook_group_master content'] = array(
    'name' => 'delete any area_hook_group_master content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'delete any day_van_pl_gp_area_hook_grouping content'.
  $permissions['delete any day_van_pl_gp_area_hook_grouping content'] = array(
    'name' => 'delete any day_van_pl_gp_area_hook_grouping content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'delete own area_hook_group_master content'.
  $permissions['delete own area_hook_group_master content'] = array(
    'name' => 'delete own area_hook_group_master content',
    'roles' => array(
      'Distributor' => 'Distributor',
      'Uploader' => 'Uploader',
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own day_van_pl_gp_area_hook_grouping content'.
  $permissions['delete own day_van_pl_gp_area_hook_grouping content'] = array(
    'name' => 'delete own day_van_pl_gp_area_hook_grouping content',
    'roles' => array(
      'Distributor' => 'Distributor',
      'Uploader' => 'Uploader',
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any area_hook_group_master content'.
  $permissions['edit any area_hook_group_master content'] = array(
    'name' => 'edit any area_hook_group_master content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'edit any day_van_pl_gp_area_hook_grouping content'.
  $permissions['edit any day_van_pl_gp_area_hook_grouping content'] = array(
    'name' => 'edit any day_van_pl_gp_area_hook_grouping content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'edit own area_hook_group_master content'.
  $permissions['edit own area_hook_group_master content'] = array(
    'name' => 'edit own area_hook_group_master content',
    'roles' => array(
      'Distributor' => 'Distributor',
      'Uploader' => 'Uploader',
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own day_van_pl_gp_area_hook_grouping content'.
  $permissions['edit own day_van_pl_gp_area_hook_grouping content'] = array(
    'name' => 'edit own day_van_pl_gp_area_hook_grouping content',
    'roles' => array(
      'Distributor' => 'Distributor',
      'Uploader' => 'Uploader',
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  return $permissions;
}
