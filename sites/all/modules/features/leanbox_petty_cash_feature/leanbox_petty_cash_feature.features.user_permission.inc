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

  // Exported permission: 'import petty_cash_master feeds'.
  $permissions['import petty_cash_master feeds'] = array(
    'name' => 'import petty_cash_master feeds',
    'roles' => array(
      'DFE' => 'DFE',
      'RS Admin' => 'RS Admin',
      'RS Operator' => 'RS Operator',
    ),
    'module' => 'feeds',
  );

  // Exported permission: 'unlock petty_cash_master feeds'.
  $permissions['unlock petty_cash_master feeds'] = array(
    'name' => 'unlock petty_cash_master feeds',
    'roles' => array(),
    'module' => 'feeds',
  );

  return $permissions;
}
