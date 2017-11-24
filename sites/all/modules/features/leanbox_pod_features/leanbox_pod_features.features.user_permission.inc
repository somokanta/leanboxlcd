<?php
/**
 * @file
 * leanbox_pod_features.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function leanbox_pod_features_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'clear pod_call_status feeds'.
  $permissions['clear pod_call_status feeds'] = array(
    'name' => 'clear pod_call_status feeds',
    'roles' => array(),
    'module' => 'feeds',
  );

  // Exported permission: 'clear pod_delivery_status feeds'.
  $permissions['clear pod_delivery_status feeds'] = array(
    'name' => 'clear pod_delivery_status feeds',
    'roles' => array(),
    'module' => 'feeds',
  );

  // Exported permission: 'create pod_history content'.
  $permissions['create pod_history content'] = array(
    'name' => 'create pod_history content',
    'roles' => array(
      'leanbox_pod' => 'leanbox_pod',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any pod_history content'.
  $permissions['delete any pod_history content'] = array(
    'name' => 'delete any pod_history content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'delete own pod_history content'.
  $permissions['delete own pod_history content'] = array(
    'name' => 'delete own pod_history content',
    'roles' => array(
      'leanbox_pod' => 'leanbox_pod',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any pod_history content'.
  $permissions['edit any pod_history content'] = array(
    'name' => 'edit any pod_history content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'edit own pod_history content'.
  $permissions['edit own pod_history content'] = array(
    'name' => 'edit own pod_history content',
    'roles' => array(
      'leanbox_pod' => 'leanbox_pod',
    ),
    'module' => 'node',
  );

  // Exported permission: 'import pod_call_status feeds'.
  $permissions['import pod_call_status feeds'] = array(
    'name' => 'import pod_call_status feeds',
    'roles' => array(
      'leanbox_pod' => 'leanbox_pod',
    ),
    'module' => 'feeds',
  );

  // Exported permission: 'import pod_delivery_status feeds'.
  $permissions['import pod_delivery_status feeds'] = array(
    'name' => 'import pod_delivery_status feeds',
    'roles' => array(
      'leanbox_pod' => 'leanbox_pod',
    ),
    'module' => 'feeds',
  );

  // Exported permission: 'pod threshold'.
  $permissions['pod threshold'] = array(
    'name' => 'pod threshold',
    'roles' => array(
      'administrator' => 'administrator',
      'leanbox_pod' => 'leanbox_pod',
    ),
    'module' => 'leanbox_pod',
  );

  // Exported permission: 'unlock pod_call_status feeds'.
  $permissions['unlock pod_call_status feeds'] = array(
    'name' => 'unlock pod_call_status feeds',
    'roles' => array(),
    'module' => 'feeds',
  );

  // Exported permission: 'unlock pod_delivery_status feeds'.
  $permissions['unlock pod_delivery_status feeds'] = array(
    'name' => 'unlock pod_delivery_status feeds',
    'roles' => array(),
    'module' => 'feeds',
  );

  return $permissions;
}
