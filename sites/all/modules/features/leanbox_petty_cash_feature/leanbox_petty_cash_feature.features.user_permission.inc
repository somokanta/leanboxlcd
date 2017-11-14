<?php
/**
 * @file
 * leanbox_petty_cash_feature.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function leanbox_petty_cash_feature_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'clear petty_cash_master feeds'.
  $permissions['clear petty_cash_master feeds'] = array(
    'name' => 'clear petty_cash_master feeds',
    'roles' => array(),
    'module' => 'feeds',
  );

  // Exported permission: 'edit terms in petty_cash'.
  $permissions['edit terms in petty_cash'] = array(
    'name' => 'edit terms in petty_cash',
    'roles' => array(
      'leanbox_petty_cash' => 'leanbox_petty_cash',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'import petty_cash_master feeds'.
  $permissions['import petty_cash_master feeds'] = array(
    'name' => 'import petty_cash_master feeds',
    'roles' => array(
      'leanbox_petty_cash' => 'leanbox_petty_cash',
    ),
    'module' => 'feeds',
  );

  // Exported permission: 'unlock petty_cash_master feeds'.
  $permissions['unlock petty_cash_master feeds'] = array(
    'name' => 'unlock petty_cash_master feeds',
    'roles' => array(),
    'module' => 'feeds',
  );

  // Exported permission: 'upload petty cash'.
  $permissions['upload petty cash'] = array(
    'name' => 'upload petty cash',
    'roles' => array(
      'administrator' => 'administrator',
      'leanbox_petty_cash' => 'leanbox_petty_cash',
    ),
    'module' => 'leanbox_petty_cash',
  );

  return $permissions;
}
