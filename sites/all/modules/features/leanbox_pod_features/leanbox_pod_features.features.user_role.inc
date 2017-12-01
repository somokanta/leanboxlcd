<?php
/**
 * @file
 * leanbox_pod_features.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function leanbox_pod_features_user_default_roles() {
  $roles = array();

  // Exported role: leanbox_pod.
  $roles['leanbox_pod'] = array(
    'name' => 'leanbox_pod',
    'weight' => 14,
  );

  return $roles;
}
