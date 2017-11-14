<?php
/**
 * @file
 * leanbox_petty_cash_feature.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function leanbox_petty_cash_feature_user_default_roles() {
  $roles = array();

  // Exported role: leanbox_petty_cash.
  $roles['leanbox_petty_cash'] = array(
    'name' => 'leanbox_petty_cash',
    'weight' => 13,
  );

  return $roles;
}
