<?php

function change_feeds_import_views_data_alter(&$data) {
  $data['field_data_field_picking_picking_type']['field_picking_picking_type']['field']['handler'] = 'custom_for_picking_type';
  $data['field_data_field_combined_nid']['field_combined_nid']['field']['handler'] = 'custom_for_combine_nid';
}


/**
 * Implements hook_views_data().
 */
function change_feeds_import_views_data() {
  $data = array();
  // Add Example group
  $data['ccfot']['table']['group'] = t('ccfot');
  $data['ccfot']['table']['join'] = array(
    // #global is a special flag which let's a table appear all the time.
    '#global' => array(),
  );
  // Add custom field
  $data['ccfot']['Order_Basepack'] = array(
    'title' => t('Order Number - Basepack Code'),
    'help' => t('Provides a custom field.'),
    'field' => array(
      'handler' => 'order_basepack_handler_custom_field',
    ),
  );
  return $data;
}