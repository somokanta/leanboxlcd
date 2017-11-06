<?php
/**
 * @file
 * leanbox_payment_collection_feature.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function leanbox_payment_collection_feature_user_default_roles() {
  $roles = array();

  // Exported role: leanbox_collection.
  $roles['leanbox_collection'] = array(
    'name' => 'leanbox_collection',
    'weight' => 12,
  );

  return $roles;
}
