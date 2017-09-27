<?php
/**
 * @file
 * pick_confirm.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function pick_confirm_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'pick_confirm_form';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Pick Confirm Form';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Pick Confirm Form';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Field: Content: VAN ID */
  $handler->display->display_options['fields']['field_pc_van_id']['id'] = 'field_pc_van_id';
  $handler->display->display_options['fields']['field_pc_van_id']['table'] = 'field_data_field_pc_van_id';
  $handler->display->display_options['fields']['field_pc_van_id']['field'] = 'field_pc_van_id';
  /* Field: Content: Loading Date */
  $handler->display->display_options['fields']['field_pc_loading_date']['id'] = 'field_pc_loading_date';
  $handler->display->display_options['fields']['field_pc_loading_date']['table'] = 'field_data_field_pc_loading_date';
  $handler->display->display_options['fields']['field_pc_loading_date']['field'] = 'field_pc_loading_date';
  $handler->display->display_options['fields']['field_pc_loading_date']['alter']['trim_whitespace'] = TRUE;
  $handler->display->display_options['fields']['field_pc_loading_date']['alter']['strip_tags'] = TRUE;
  $handler->display->display_options['fields']['field_pc_loading_date']['settings'] = array(
    'format_type' => 'short',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_remaining_days' => 0,
  );
  /* Field: Content: Pick Confirm Type */
  $handler->display->display_options['fields']['field_pc_picking_type']['id'] = 'field_pc_picking_type';
  $handler->display->display_options['fields']['field_pc_picking_type']['table'] = 'field_data_field_pc_picking_type';
  $handler->display->display_options['fields']['field_pc_picking_type']['field'] = 'field_pc_picking_type';
  /* Field: Content: Pick Confirm Date */
  $handler->display->display_options['fields']['field_pc_picking_date']['id'] = 'field_pc_picking_date';
  $handler->display->display_options['fields']['field_pc_picking_date']['table'] = 'field_data_field_pc_picking_date';
  $handler->display->display_options['fields']['field_pc_picking_date']['field'] = 'field_pc_picking_date';
  $handler->display->display_options['fields']['field_pc_picking_date']['alter']['trim_whitespace'] = TRUE;
  $handler->display->display_options['fields']['field_pc_picking_date']['alter']['strip_tags'] = TRUE;
  $handler->display->display_options['fields']['field_pc_picking_date']['settings'] = array(
    'format_type' => 'short',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_remaining_days' => 0,
  );
  /* Field: Content: PICK CONFIRM LINES */
  $handler->display->display_options['fields']['field_pc_pick_lines']['id'] = 'field_pc_pick_lines';
  $handler->display->display_options['fields']['field_pc_pick_lines']['table'] = 'field_data_field_pc_pick_lines';
  $handler->display->display_options['fields']['field_pc_pick_lines']['field'] = 'field_pc_pick_lines';
  $handler->display->display_options['fields']['field_pc_pick_lines']['settings'] = array(
    'thousand_separator' => '',
    'prefix_suffix' => 1,
  );
  /* Field: Content: PICK CONFIRM UNITS */
  $handler->display->display_options['fields']['field_pc_pick_units']['id'] = 'field_pc_pick_units';
  $handler->display->display_options['fields']['field_pc_pick_units']['table'] = 'field_data_field_pc_pick_units';
  $handler->display->display_options['fields']['field_pc_pick_units']['field'] = 'field_pc_pick_units';
  $handler->display->display_options['fields']['field_pc_pick_units']['settings'] = array(
    'thousand_separator' => '',
    'prefix_suffix' => 1,
  );
  /* Field: Content: PICK CONFIRM CLDs */
  $handler->display->display_options['fields']['field_pc_pick_clds']['id'] = 'field_pc_pick_clds';
  $handler->display->display_options['fields']['field_pc_pick_clds']['table'] = 'field_data_field_pc_pick_clds';
  $handler->display->display_options['fields']['field_pc_pick_clds']['field'] = 'field_pc_pick_clds';
  $handler->display->display_options['fields']['field_pc_pick_clds']['settings'] = array(
    'thousand_separator' => '',
    'prefix_suffix' => 1,
  );
  /* Field: Content: PICK CONFIRM SUPERVISOR */
  $handler->display->display_options['fields']['field_pc_pick_supervisor']['id'] = 'field_pc_pick_supervisor';
  $handler->display->display_options['fields']['field_pc_pick_supervisor']['table'] = 'field_data_field_pc_pick_supervisor';
  $handler->display->display_options['fields']['field_pc_pick_supervisor']['field'] = 'field_pc_pick_supervisor';
  /* Field: Content: PICK CONFIRM LINES */
  $handler->display->display_options['fields']['field_pc_pick_lines_1']['id'] = 'field_pc_pick_lines_1';
  $handler->display->display_options['fields']['field_pc_pick_lines_1']['table'] = 'field_data_field_pc_pick_lines';
  $handler->display->display_options['fields']['field_pc_pick_lines_1']['field'] = 'field_pc_pick_lines';
  $handler->display->display_options['fields']['field_pc_pick_lines_1']['label'] = 'LOADING SHEET LINE';
  $handler->display->display_options['fields']['field_pc_pick_lines_1']['settings'] = array(
    'thousand_separator' => '',
    'prefix_suffix' => 1,
  );
  /* Field: Content: PICKER NAME */
  $handler->display->display_options['fields']['field_pc_picker_name']['id'] = 'field_pc_picker_name';
  $handler->display->display_options['fields']['field_pc_picker_name']['table'] = 'field_data_field_pc_picker_name';
  $handler->display->display_options['fields']['field_pc_picker_name']['field'] = 'field_pc_picker_name';
  /* Field: Content: PICK CONFIRM TEAM COUNT */
  $handler->display->display_options['fields']['field_pc_team_count']['id'] = 'field_pc_team_count';
  $handler->display->display_options['fields']['field_pc_team_count']['table'] = 'field_data_field_pc_team_count';
  $handler->display->display_options['fields']['field_pc_team_count']['field'] = 'field_pc_team_count';
  $handler->display->display_options['fields']['field_pc_team_count']['settings'] = array(
    'thousand_separator' => '',
    'decimal_separator' => '.',
    'scale' => '2',
    'prefix_suffix' => 1,
  );
  /* Field: Content: Pick Confirm Start Time */
  $handler->display->display_options['fields']['field_pc_start_time']['id'] = 'field_pc_start_time';
  $handler->display->display_options['fields']['field_pc_start_time']['table'] = 'field_data_field_pc_start_time';
  $handler->display->display_options['fields']['field_pc_start_time']['field'] = 'field_pc_start_time';
  /* Field: Content: Pick Confirm End time */
  $handler->display->display_options['fields']['field_pc_end_time']['id'] = 'field_pc_end_time';
  $handler->display->display_options['fields']['field_pc_end_time']['table'] = 'field_data_field_pc_end_time';
  $handler->display->display_options['fields']['field_pc_end_time']['field'] = 'field_pc_end_time';
  /* Field: Content: Status */
  $handler->display->display_options['fields']['field_pc_status']['id'] = 'field_pc_status';
  $handler->display->display_options['fields']['field_pc_status']['table'] = 'field_data_field_pc_status';
  $handler->display->display_options['fields']['field_pc_status']['field'] = 'field_pc_status';
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'pick_confirm' => 'pick_confirm',
  );
  /* Filter criterion: Content: Status (field_pc_status) */
  $handler->display->display_options['filters']['field_pc_status_value']['id'] = 'field_pc_status_value';
  $handler->display->display_options['filters']['field_pc_status_value']['table'] = 'field_data_field_pc_status';
  $handler->display->display_options['filters']['field_pc_status_value']['field'] = 'field_pc_status_value';
  $handler->display->display_options['filters']['field_pc_status_value']['operator'] = 'not';
  $handler->display->display_options['filters']['field_pc_status_value']['value'] = array(
    1 => '1',
    2 => '2',
  );
  /* Filter criterion: Content: Loading Date (field_pc_loading_date) */
  $handler->display->display_options['filters']['field_pc_loading_date_value']['id'] = 'field_pc_loading_date_value';
  $handler->display->display_options['filters']['field_pc_loading_date_value']['table'] = 'field_data_field_pc_loading_date';
  $handler->display->display_options['filters']['field_pc_loading_date_value']['field'] = 'field_pc_loading_date_value';
  $handler->display->display_options['filters']['field_pc_loading_date_value']['operator'] = 'between';
  $handler->display->display_options['filters']['field_pc_loading_date_value']['form_type'] = 'date_text';
  $handler->display->display_options['filters']['field_pc_loading_date_value']['default_date'] = '-7 day';
  $handler->display->display_options['filters']['field_pc_loading_date_value']['default_to_date'] = 'now';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'pick-confirm-form';
  $export['pick_confirm_form'] = $view;

  return $export;
}
