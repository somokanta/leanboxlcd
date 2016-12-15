<?php

function change_feeds_import_views_data_alter(&$data) {
  $data['field_data_field_picking_picking_type']['field_picking_picking_type']['field']['handler'] = 'custom_for_picking_type';
  $data['field_data_field_combined_nid']['field_combined_nid']['field']['handler'] = 'custom_for_combine_nid';
}
