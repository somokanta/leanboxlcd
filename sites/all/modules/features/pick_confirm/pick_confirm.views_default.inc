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

  $view = new view();
  $view->name = 'pick_confirm_report';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Pick Confirm Report';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Pick Confirm Report';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
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
  /* Footer: Global: Result summary */
  $handler->display->display_options['footer']['result']['id'] = 'result';
  $handler->display->display_options['footer']['result']['table'] = 'views';
  $handler->display->display_options['footer']['result']['field'] = 'result';
  $handler->display->display_options['footer']['result']['content'] = '<h4>Displaying @start - @end of @total</h4>';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
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
  /* Filter criterion: Content: Loading Date (field_pc_loading_date) */
  $handler->display->display_options['filters']['field_pc_loading_date_value']['id'] = 'field_pc_loading_date_value';
  $handler->display->display_options['filters']['field_pc_loading_date_value']['table'] = 'field_data_field_pc_loading_date';
  $handler->display->display_options['filters']['field_pc_loading_date_value']['field'] = 'field_pc_loading_date_value';
  $handler->display->display_options['filters']['field_pc_loading_date_value']['operator'] = 'between';
  $handler->display->display_options['filters']['field_pc_loading_date_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_pc_loading_date_value']['expose']['operator_id'] = 'field_pc_loading_date_value_op';
  $handler->display->display_options['filters']['field_pc_loading_date_value']['expose']['label'] = 'Loading Date';
  $handler->display->display_options['filters']['field_pc_loading_date_value']['expose']['operator'] = 'field_pc_loading_date_value_op';
  $handler->display->display_options['filters']['field_pc_loading_date_value']['expose']['identifier'] = 'field_pc_loading_date_value';
  $handler->display->display_options['filters']['field_pc_loading_date_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
    6 => 0,
    7 => 0,
    8 => 0,
    9 => 0,
    10 => 0,
  );
  $handler->display->display_options['filters']['field_pc_loading_date_value']['form_type'] = 'date_popup';
  $handler->display->display_options['filters']['field_pc_loading_date_value']['default_date'] = 'now';
  $handler->display->display_options['filters']['field_pc_loading_date_value']['default_to_date'] = 'now';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['exposed_form'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'better_exposed_filters';
  $handler->display->display_options['exposed_form']['options']['bef'] = array(
    'general' => array(
      'allow_secondary' => 0,
      'secondary_label' => 'Advanced options',
      'collapsible_label' => NULL,
      'combine_rewrite' => NULL,
      'reset_label' => NULL,
      'bef_filter_description' => NULL,
      'any_label' => NULL,
      'filter_rewrite_values' => NULL,
    ),
    'field_pc_loading_date_value' => array(
      'bef_format' => 'default',
      'more_options' => array(
        'is_secondary' => 0,
        'any_label' => '',
        'bef_filter_description' => '',
        'tokens' => array(
          'available' => array(
            0 => 'global_types',
            'secondary_label' => NULL,
            'collapsible_label' => NULL,
            'combine_rewrite' => NULL,
            'reset_label' => NULL,
            'bef_filter_description' => NULL,
            'any_label' => NULL,
            'filter_rewrite_values' => NULL,
          ),
          'secondary_label' => NULL,
          'collapsible_label' => NULL,
          'combine_rewrite' => NULL,
          'reset_label' => NULL,
          'bef_filter_description' => NULL,
          'any_label' => NULL,
          'filter_rewrite_values' => NULL,
        ),
        'rewrite' => array(
          'filter_rewrite_values' => '',
          'secondary_label' => NULL,
          'collapsible_label' => NULL,
          'combine_rewrite' => NULL,
          'reset_label' => NULL,
          'bef_filter_description' => NULL,
          'any_label' => NULL,
        ),
        'secondary_label' => NULL,
        'collapsible_label' => NULL,
        'combine_rewrite' => NULL,
        'reset_label' => NULL,
        'filter_rewrite_values' => NULL,
      ),
      'secondary_label' => NULL,
      'collapsible_label' => NULL,
      'combine_rewrite' => NULL,
      'reset_label' => NULL,
      'bef_filter_description' => NULL,
      'any_label' => NULL,
      'filter_rewrite_values' => NULL,
    ),
    'field_pc_picking_type_value' => array(
      'bef_format' => 'default',
      'more_options' => array(
        'is_secondary' => 0,
        'any_label' => '',
        'bef_filter_description' => '',
        'tokens' => array(
          'available' => array(
            0 => 'global_types',
            'secondary_label' => NULL,
            'collapsible_label' => NULL,
            'combine_rewrite' => NULL,
            'reset_label' => NULL,
            'bef_filter_description' => NULL,
            'any_label' => NULL,
            'filter_rewrite_values' => NULL,
          ),
          'secondary_label' => NULL,
          'collapsible_label' => NULL,
          'combine_rewrite' => NULL,
          'reset_label' => NULL,
          'bef_filter_description' => NULL,
          'any_label' => NULL,
          'filter_rewrite_values' => NULL,
        ),
        'rewrite' => array(
          'filter_rewrite_values' => '',
          'secondary_label' => NULL,
          'collapsible_label' => NULL,
          'combine_rewrite' => NULL,
          'reset_label' => NULL,
          'bef_filter_description' => NULL,
          'any_label' => NULL,
        ),
        'secondary_label' => NULL,
        'collapsible_label' => NULL,
        'combine_rewrite' => NULL,
        'reset_label' => NULL,
        'filter_rewrite_values' => NULL,
      ),
      'secondary_label' => NULL,
      'collapsible_label' => NULL,
      'combine_rewrite' => NULL,
      'reset_label' => NULL,
      'bef_filter_description' => NULL,
      'any_label' => NULL,
      'filter_rewrite_values' => NULL,
    ),
    'field_pc_status_value' => array(
      'bef_format' => 'default',
      'more_options' => array(
        'bef_select_all_none' => FALSE,
        'bef_collapsible' => 0,
        'is_secondary' => 0,
        'any_label' => '',
        'bef_filter_description' => '',
        'tokens' => array(
          'available' => array(
            0 => 'global_types',
            'secondary_label' => NULL,
            'collapsible_label' => NULL,
            'combine_rewrite' => NULL,
            'reset_label' => NULL,
            'bef_filter_description' => NULL,
            'any_label' => NULL,
            'filter_rewrite_values' => NULL,
          ),
          'secondary_label' => NULL,
          'collapsible_label' => NULL,
          'combine_rewrite' => NULL,
          'reset_label' => NULL,
          'bef_filter_description' => NULL,
          'any_label' => NULL,
          'filter_rewrite_values' => NULL,
        ),
        'rewrite' => array(
          'filter_rewrite_values' => '',
          'secondary_label' => NULL,
          'collapsible_label' => NULL,
          'combine_rewrite' => NULL,
          'reset_label' => NULL,
          'bef_filter_description' => NULL,
          'any_label' => NULL,
        ),
        'secondary_label' => NULL,
        'collapsible_label' => NULL,
        'combine_rewrite' => NULL,
        'reset_label' => NULL,
        'filter_rewrite_values' => NULL,
      ),
      'secondary_label' => NULL,
      'collapsible_label' => NULL,
      'combine_rewrite' => NULL,
      'reset_label' => NULL,
      'bef_filter_description' => NULL,
      'any_label' => NULL,
      'filter_rewrite_values' => NULL,
    ),
    'secondary_label' => NULL,
    'collapsible_label' => NULL,
    'combine_rewrite' => NULL,
    'reset_label' => NULL,
    'bef_filter_description' => NULL,
    'any_label' => NULL,
    'filter_rewrite_values' => NULL,
  );
  $handler->display->display_options['exposed_form']['options']['input_required'] = 0;
  $handler->display->display_options['exposed_form']['options']['text_input_required_format'] = 'filtered_html';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
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
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Global: View result counter */
  $handler->display->display_options['fields']['counter']['id'] = 'counter';
  $handler->display->display_options['fields']['counter']['table'] = 'views';
  $handler->display->display_options['fields']['counter']['field'] = 'counter';
  $handler->display->display_options['fields']['counter']['label'] = 'Sr. No.';
  $handler->display->display_options['fields']['counter']['counter_start'] = '1';
  $handler->display->display_options['fields']['counter']['reverse'] = 0;
  /* Field: Content: Loading Date */
  $handler->display->display_options['fields']['field_pc_loading_date']['id'] = 'field_pc_loading_date';
  $handler->display->display_options['fields']['field_pc_loading_date']['table'] = 'field_data_field_pc_loading_date';
  $handler->display->display_options['fields']['field_pc_loading_date']['field'] = 'field_pc_loading_date';
  $handler->display->display_options['fields']['field_pc_loading_date']['alter']['trim_whitespace'] = TRUE;
  $handler->display->display_options['fields']['field_pc_loading_date']['alter']['strip_tags'] = TRUE;
  $handler->display->display_options['fields']['field_pc_loading_date']['settings'] = array(
    'format_type' => 'sales_day',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_remaining_days' => 0,
  );
  /* Field: Content: VAN ID */
  $handler->display->display_options['fields']['field_pc_van_id']['id'] = 'field_pc_van_id';
  $handler->display->display_options['fields']['field_pc_van_id']['table'] = 'field_data_field_pc_van_id';
  $handler->display->display_options['fields']['field_pc_van_id']['field'] = 'field_pc_van_id';
  /* Field: Content: Pick Confirm Type */
  $handler->display->display_options['fields']['field_pc_picking_type']['id'] = 'field_pc_picking_type';
  $handler->display->display_options['fields']['field_pc_picking_type']['table'] = 'field_data_field_pc_picking_type';
  $handler->display->display_options['fields']['field_pc_picking_type']['field'] = 'field_pc_picking_type';
  /* Field: Content: PICK CONFIRM CLDs */
  $handler->display->display_options['fields']['field_pc_pick_clds']['id'] = 'field_pc_pick_clds';
  $handler->display->display_options['fields']['field_pc_pick_clds']['table'] = 'field_data_field_pc_pick_clds';
  $handler->display->display_options['fields']['field_pc_pick_clds']['field'] = 'field_pc_pick_clds';
  $handler->display->display_options['fields']['field_pc_pick_clds']['settings'] = array(
    'thousand_separator' => '',
    'prefix_suffix' => 1,
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
  /* Field: Content: PICK CONFIRM LINES */
  $handler->display->display_options['fields']['field_pc_pick_lines_1']['id'] = 'field_pc_pick_lines_1';
  $handler->display->display_options['fields']['field_pc_pick_lines_1']['table'] = 'field_data_field_pc_pick_lines';
  $handler->display->display_options['fields']['field_pc_pick_lines_1']['field'] = 'field_pc_pick_lines';
  $handler->display->display_options['fields']['field_pc_pick_lines_1']['label'] = 'LOADING SHEET LINE';
  $handler->display->display_options['fields']['field_pc_pick_lines_1']['settings'] = array(
    'thousand_separator' => '',
    'prefix_suffix' => 1,
  );
  /* Field: Content: PICK CONFIRM SUPERVISOR */
  $handler->display->display_options['fields']['field_pc_pick_supervisor']['id'] = 'field_pc_pick_supervisor';
  $handler->display->display_options['fields']['field_pc_pick_supervisor']['table'] = 'field_data_field_pc_pick_supervisor';
  $handler->display->display_options['fields']['field_pc_pick_supervisor']['field'] = 'field_pc_pick_supervisor';
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
  /* Field: Content: Pick Confirm Date */
  $handler->display->display_options['fields']['field_pc_picking_date']['id'] = 'field_pc_picking_date';
  $handler->display->display_options['fields']['field_pc_picking_date']['table'] = 'field_data_field_pc_picking_date';
  $handler->display->display_options['fields']['field_pc_picking_date']['field'] = 'field_pc_picking_date';
  $handler->display->display_options['fields']['field_pc_picking_date']['settings'] = array(
    'format_type' => 'sales_day',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_remaining_days' => 0,
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
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Content: Loading Date (field_pc_loading_date) */
  $handler->display->display_options['sorts']['field_pc_loading_date_value']['id'] = 'field_pc_loading_date_value';
  $handler->display->display_options['sorts']['field_pc_loading_date_value']['table'] = 'field_data_field_pc_loading_date';
  $handler->display->display_options['sorts']['field_pc_loading_date_value']['field'] = 'field_pc_loading_date_value';
  $handler->display->display_options['sorts']['field_pc_loading_date_value']['order'] = 'DESC';
  /* Sort criterion: Content: VAN ID (field_pc_van_id) */
  $handler->display->display_options['sorts']['field_pc_van_id_value']['id'] = 'field_pc_van_id_value';
  $handler->display->display_options['sorts']['field_pc_van_id_value']['table'] = 'field_data_field_pc_van_id';
  $handler->display->display_options['sorts']['field_pc_van_id_value']['field'] = 'field_pc_van_id_value';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
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
  /* Filter criterion: Content: Loading Date (field_pc_loading_date) */
  $handler->display->display_options['filters']['field_pc_loading_date_value']['id'] = 'field_pc_loading_date_value';
  $handler->display->display_options['filters']['field_pc_loading_date_value']['table'] = 'field_data_field_pc_loading_date';
  $handler->display->display_options['filters']['field_pc_loading_date_value']['field'] = 'field_pc_loading_date_value';
  $handler->display->display_options['filters']['field_pc_loading_date_value']['operator'] = 'between';
  $handler->display->display_options['filters']['field_pc_loading_date_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_pc_loading_date_value']['expose']['operator_id'] = 'field_pc_loading_date_value_op';
  $handler->display->display_options['filters']['field_pc_loading_date_value']['expose']['label'] = 'Loading Date';
  $handler->display->display_options['filters']['field_pc_loading_date_value']['expose']['operator'] = 'field_pc_loading_date_value_op';
  $handler->display->display_options['filters']['field_pc_loading_date_value']['expose']['identifier'] = 'field_pc_loading_date_value';
  $handler->display->display_options['filters']['field_pc_loading_date_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
    6 => 0,
    7 => 0,
    8 => 0,
    9 => 0,
    10 => 0,
  );
  $handler->display->display_options['filters']['field_pc_loading_date_value']['form_type'] = 'date_popup';
  $handler->display->display_options['filters']['field_pc_loading_date_value']['default_date'] = 'now';
  $handler->display->display_options['filters']['field_pc_loading_date_value']['default_to_date'] = 'now';
  /* Filter criterion: Content: Pick Confirm Type (field_pc_picking_type) */
  $handler->display->display_options['filters']['field_pc_picking_type_value']['id'] = 'field_pc_picking_type_value';
  $handler->display->display_options['filters']['field_pc_picking_type_value']['table'] = 'field_data_field_pc_picking_type';
  $handler->display->display_options['filters']['field_pc_picking_type_value']['field'] = 'field_pc_picking_type_value';
  $handler->display->display_options['filters']['field_pc_picking_type_value']['operator'] = 'contains';
  $handler->display->display_options['filters']['field_pc_picking_type_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_pc_picking_type_value']['expose']['operator_id'] = 'field_pc_picking_type_value_op';
  $handler->display->display_options['filters']['field_pc_picking_type_value']['expose']['label'] = 'Pick Confirm Type';
  $handler->display->display_options['filters']['field_pc_picking_type_value']['expose']['operator'] = 'field_pc_picking_type_value_op';
  $handler->display->display_options['filters']['field_pc_picking_type_value']['expose']['identifier'] = 'field_pc_picking_type_value';
  $handler->display->display_options['filters']['field_pc_picking_type_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
    6 => 0,
    7 => 0,
    8 => 0,
    9 => 0,
    10 => 0,
  );
  /* Filter criterion: Content: Status (field_pc_status) */
  $handler->display->display_options['filters']['field_pc_status_value']['id'] = 'field_pc_status_value';
  $handler->display->display_options['filters']['field_pc_status_value']['table'] = 'field_data_field_pc_status';
  $handler->display->display_options['filters']['field_pc_status_value']['field'] = 'field_pc_status_value';
  $handler->display->display_options['filters']['field_pc_status_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_pc_status_value']['expose']['operator_id'] = 'field_pc_status_value_op';
  $handler->display->display_options['filters']['field_pc_status_value']['expose']['label'] = 'Status';
  $handler->display->display_options['filters']['field_pc_status_value']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['field_pc_status_value']['expose']['operator'] = 'field_pc_status_value_op';
  $handler->display->display_options['filters']['field_pc_status_value']['expose']['identifier'] = 'field_pc_status_value';
  $handler->display->display_options['filters']['field_pc_status_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
    6 => 0,
    7 => 0,
    8 => 0,
    9 => 0,
    10 => 0,
  );
  $handler->display->display_options['path'] = 'pick-confirm-report/view';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'View';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Pick Confirm Report (View)';
  $handler->display->display_options['tab_options']['weight'] = '0';

  /* Display: Pick Confirm Page Edit */
  $handler = $view->new_display('page', 'Pick Confirm Page Edit', 'page_1');
  $handler->display->display_options['defaults']['exposed_form'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'better_exposed_filters';
  $handler->display->display_options['exposed_form']['options']['bef'] = array(
    'general' => array(
      'allow_secondary' => 0,
      'secondary_label' => 'Advanced options',
      'collapsible_label' => NULL,
      'combine_rewrite' => NULL,
      'reset_label' => NULL,
      'bef_filter_description' => NULL,
      'any_label' => NULL,
      'filter_rewrite_values' => NULL,
    ),
    'field_pc_loading_date_value' => array(
      'bef_format' => 'default',
      'more_options' => array(
        'is_secondary' => 0,
        'any_label' => '',
        'bef_filter_description' => '',
        'tokens' => array(
          'available' => array(
            0 => 'global_types',
            'secondary_label' => NULL,
            'collapsible_label' => NULL,
            'combine_rewrite' => NULL,
            'reset_label' => NULL,
            'bef_filter_description' => NULL,
            'any_label' => NULL,
            'filter_rewrite_values' => NULL,
          ),
          'secondary_label' => NULL,
          'collapsible_label' => NULL,
          'combine_rewrite' => NULL,
          'reset_label' => NULL,
          'bef_filter_description' => NULL,
          'any_label' => NULL,
          'filter_rewrite_values' => NULL,
        ),
        'rewrite' => array(
          'filter_rewrite_values' => '',
          'secondary_label' => NULL,
          'collapsible_label' => NULL,
          'combine_rewrite' => NULL,
          'reset_label' => NULL,
          'bef_filter_description' => NULL,
          'any_label' => NULL,
        ),
        'secondary_label' => NULL,
        'collapsible_label' => NULL,
        'combine_rewrite' => NULL,
        'reset_label' => NULL,
        'filter_rewrite_values' => NULL,
      ),
      'secondary_label' => NULL,
      'collapsible_label' => NULL,
      'combine_rewrite' => NULL,
      'reset_label' => NULL,
      'bef_filter_description' => NULL,
      'any_label' => NULL,
      'filter_rewrite_values' => NULL,
    ),
    'field_pc_picking_type_value' => array(
      'bef_format' => 'default',
      'more_options' => array(
        'is_secondary' => 0,
        'any_label' => '',
        'bef_filter_description' => '',
        'tokens' => array(
          'available' => array(
            0 => 'global_types',
            'secondary_label' => NULL,
            'collapsible_label' => NULL,
            'combine_rewrite' => NULL,
            'reset_label' => NULL,
            'bef_filter_description' => NULL,
            'any_label' => NULL,
            'filter_rewrite_values' => NULL,
          ),
          'secondary_label' => NULL,
          'collapsible_label' => NULL,
          'combine_rewrite' => NULL,
          'reset_label' => NULL,
          'bef_filter_description' => NULL,
          'any_label' => NULL,
          'filter_rewrite_values' => NULL,
        ),
        'rewrite' => array(
          'filter_rewrite_values' => '',
          'secondary_label' => NULL,
          'collapsible_label' => NULL,
          'combine_rewrite' => NULL,
          'reset_label' => NULL,
          'bef_filter_description' => NULL,
          'any_label' => NULL,
        ),
        'secondary_label' => NULL,
        'collapsible_label' => NULL,
        'combine_rewrite' => NULL,
        'reset_label' => NULL,
        'filter_rewrite_values' => NULL,
      ),
      'secondary_label' => NULL,
      'collapsible_label' => NULL,
      'combine_rewrite' => NULL,
      'reset_label' => NULL,
      'bef_filter_description' => NULL,
      'any_label' => NULL,
      'filter_rewrite_values' => NULL,
    ),
    'field_pc_status_value' => array(
      'bef_format' => 'default',
      'more_options' => array(
        'bef_select_all_none' => FALSE,
        'bef_collapsible' => 0,
        'is_secondary' => 0,
        'any_label' => '',
        'bef_filter_description' => '',
        'tokens' => array(
          'available' => array(
            0 => 'global_types',
            'secondary_label' => NULL,
            'collapsible_label' => NULL,
            'combine_rewrite' => NULL,
            'reset_label' => NULL,
            'bef_filter_description' => NULL,
            'any_label' => NULL,
            'filter_rewrite_values' => NULL,
          ),
          'secondary_label' => NULL,
          'collapsible_label' => NULL,
          'combine_rewrite' => NULL,
          'reset_label' => NULL,
          'bef_filter_description' => NULL,
          'any_label' => NULL,
          'filter_rewrite_values' => NULL,
        ),
        'rewrite' => array(
          'filter_rewrite_values' => '',
          'secondary_label' => NULL,
          'collapsible_label' => NULL,
          'combine_rewrite' => NULL,
          'reset_label' => NULL,
          'bef_filter_description' => NULL,
          'any_label' => NULL,
        ),
        'secondary_label' => NULL,
        'collapsible_label' => NULL,
        'combine_rewrite' => NULL,
        'reset_label' => NULL,
        'filter_rewrite_values' => NULL,
      ),
      'secondary_label' => NULL,
      'collapsible_label' => NULL,
      'combine_rewrite' => NULL,
      'reset_label' => NULL,
      'bef_filter_description' => NULL,
      'any_label' => NULL,
      'filter_rewrite_values' => NULL,
    ),
    'secondary_label' => NULL,
    'collapsible_label' => NULL,
    'combine_rewrite' => NULL,
    'reset_label' => NULL,
    'bef_filter_description' => NULL,
    'any_label' => NULL,
    'filter_rewrite_values' => NULL,
  );
  $handler->display->display_options['exposed_form']['options']['input_required'] = 0;
  $handler->display->display_options['exposed_form']['options']['text_input_required_format'] = 'filtered_html';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'editableviews_table';
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
  $handler->display->display_options['style_options']['batch_size'] = '10';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Global: View result counter */
  $handler->display->display_options['fields']['counter']['id'] = 'counter';
  $handler->display->display_options['fields']['counter']['table'] = 'views';
  $handler->display->display_options['fields']['counter']['field'] = 'counter';
  $handler->display->display_options['fields']['counter']['label'] = 'Sr. No.';
  $handler->display->display_options['fields']['counter']['counter_start'] = '1';
  $handler->display->display_options['fields']['counter']['reverse'] = 0;
  /* Field: Content: Loading Date */
  $handler->display->display_options['fields']['field_pc_loading_date']['id'] = 'field_pc_loading_date';
  $handler->display->display_options['fields']['field_pc_loading_date']['table'] = 'field_data_field_pc_loading_date';
  $handler->display->display_options['fields']['field_pc_loading_date']['field'] = 'field_pc_loading_date';
  $handler->display->display_options['fields']['field_pc_loading_date']['alter']['trim_whitespace'] = TRUE;
  $handler->display->display_options['fields']['field_pc_loading_date']['alter']['strip_tags'] = TRUE;
  $handler->display->display_options['fields']['field_pc_loading_date']['settings'] = array(
    'format_type' => 'sales_day',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_remaining_days' => 0,
  );
  /* Field: Content: VAN ID */
  $handler->display->display_options['fields']['field_pc_van_id']['id'] = 'field_pc_van_id';
  $handler->display->display_options['fields']['field_pc_van_id']['table'] = 'field_data_field_pc_van_id';
  $handler->display->display_options['fields']['field_pc_van_id']['field'] = 'field_pc_van_id';
  /* Field: Content: Pick Confirm Type */
  $handler->display->display_options['fields']['field_pc_picking_type']['id'] = 'field_pc_picking_type';
  $handler->display->display_options['fields']['field_pc_picking_type']['table'] = 'field_data_field_pc_picking_type';
  $handler->display->display_options['fields']['field_pc_picking_type']['field'] = 'field_pc_picking_type';
  /* Field: Content: PICK CONFIRM CLDs */
  $handler->display->display_options['fields']['field_pc_pick_clds']['id'] = 'field_pc_pick_clds';
  $handler->display->display_options['fields']['field_pc_pick_clds']['table'] = 'field_data_field_pc_pick_clds';
  $handler->display->display_options['fields']['field_pc_pick_clds']['field'] = 'field_pc_pick_clds';
  $handler->display->display_options['fields']['field_pc_pick_clds']['settings'] = array(
    'thousand_separator' => '',
    'prefix_suffix' => 1,
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
  /* Field: Content: PICK CONFIRM LINES */
  $handler->display->display_options['fields']['field_pc_pick_lines_1']['id'] = 'field_pc_pick_lines_1';
  $handler->display->display_options['fields']['field_pc_pick_lines_1']['table'] = 'field_data_field_pc_pick_lines';
  $handler->display->display_options['fields']['field_pc_pick_lines_1']['field'] = 'field_pc_pick_lines';
  $handler->display->display_options['fields']['field_pc_pick_lines_1']['label'] = 'LOADING SHEET LINE';
  $handler->display->display_options['fields']['field_pc_pick_lines_1']['settings'] = array(
    'thousand_separator' => '',
    'prefix_suffix' => 1,
  );
  /* Field: Content: PICK CONFIRM SUPERVISOR (editable) */
  $handler->display->display_options['fields']['field_pc_pick_supervisor_editable']['id'] = 'field_pc_pick_supervisor_editable';
  $handler->display->display_options['fields']['field_pc_pick_supervisor_editable']['table'] = 'field_data_field_pc_pick_supervisor';
  $handler->display->display_options['fields']['field_pc_pick_supervisor_editable']['field'] = 'field_pc_pick_supervisor_editable';
  $handler->display->display_options['fields']['field_pc_pick_supervisor_editable']['label'] = 'PICK CONFIRM SUPERVISOR';
  $handler->display->display_options['fields']['field_pc_pick_supervisor_editable']['type'] = 'services';
  $handler->display->display_options['fields']['field_pc_pick_supervisor_editable']['settings'] = array(
    'data_element_key' => '',
    'skip_safe' => 0,
    'skip_empty_values' => 0,
    'skip_text_format' => 1,
  );
  $handler->display->display_options['fields']['field_pc_pick_supervisor_editable']['widget_type'] = '0';
  $handler->display->display_options['fields']['field_pc_pick_supervisor_editable']['suppress_label'] = 1;
  $handler->display->display_options['fields']['field_pc_pick_supervisor_editable']['suppress_description'] = 0;
  /* Field: Content: PICKER NAME (editable) */
  $handler->display->display_options['fields']['field_pc_picker_name_editable']['id'] = 'field_pc_picker_name_editable';
  $handler->display->display_options['fields']['field_pc_picker_name_editable']['table'] = 'field_data_field_pc_picker_name';
  $handler->display->display_options['fields']['field_pc_picker_name_editable']['field'] = 'field_pc_picker_name_editable';
  $handler->display->display_options['fields']['field_pc_picker_name_editable']['label'] = 'PICKER NAME';
  $handler->display->display_options['fields']['field_pc_picker_name_editable']['type'] = 'services';
  $handler->display->display_options['fields']['field_pc_picker_name_editable']['settings'] = array(
    'data_element_key' => '',
    'skip_safe' => 0,
    'skip_empty_values' => 0,
    'skip_text_format' => 1,
  );
  $handler->display->display_options['fields']['field_pc_picker_name_editable']['widget_type'] = '0';
  $handler->display->display_options['fields']['field_pc_picker_name_editable']['suppress_label'] = 1;
  $handler->display->display_options['fields']['field_pc_picker_name_editable']['suppress_description'] = 0;
  /* Field: Content: PICK CONFIRM TEAM COUNT (editable) */
  $handler->display->display_options['fields']['field_pc_team_count_editable']['id'] = 'field_pc_team_count_editable';
  $handler->display->display_options['fields']['field_pc_team_count_editable']['table'] = 'field_data_field_pc_team_count';
  $handler->display->display_options['fields']['field_pc_team_count_editable']['field'] = 'field_pc_team_count_editable';
  $handler->display->display_options['fields']['field_pc_team_count_editable']['label'] = 'PICK CONFIRM TEAM COUNT';
  $handler->display->display_options['fields']['field_pc_team_count_editable']['type'] = 'services';
  $handler->display->display_options['fields']['field_pc_team_count_editable']['settings'] = array(
    'data_element_key' => '',
    'skip_safe' => 0,
    'skip_empty_values' => 0,
  );
  $handler->display->display_options['fields']['field_pc_team_count_editable']['widget_type'] = '0';
  $handler->display->display_options['fields']['field_pc_team_count_editable']['suppress_label'] = 1;
  $handler->display->display_options['fields']['field_pc_team_count_editable']['suppress_description'] = 0;
  /* Field: Content: Pick Confirm Date (editable) */
  $handler->display->display_options['fields']['field_pc_picking_date_editable']['id'] = 'field_pc_picking_date_editable';
  $handler->display->display_options['fields']['field_pc_picking_date_editable']['table'] = 'field_data_field_pc_picking_date';
  $handler->display->display_options['fields']['field_pc_picking_date_editable']['field'] = 'field_pc_picking_date_editable';
  $handler->display->display_options['fields']['field_pc_picking_date_editable']['label'] = 'Pick Confirm Date';
  $handler->display->display_options['fields']['field_pc_picking_date_editable']['type'] = 'services';
  $handler->display->display_options['fields']['field_pc_picking_date_editable']['settings'] = array(
    'data_element_key' => '',
    'skip_safe' => 0,
    'skip_empty_values' => 0,
  );
  $handler->display->display_options['fields']['field_pc_picking_date_editable']['widget_type'] = 'date_popup';
  $handler->display->display_options['fields']['field_pc_picking_date_editable']['suppress_label'] = 1;
  $handler->display->display_options['fields']['field_pc_picking_date_editable']['suppress_description'] = 0;
  /* Field: Content: Pick Confirm Start Time (editable) */
  $handler->display->display_options['fields']['field_pc_start_time_editable']['id'] = 'field_pc_start_time_editable';
  $handler->display->display_options['fields']['field_pc_start_time_editable']['table'] = 'field_data_field_pc_start_time';
  $handler->display->display_options['fields']['field_pc_start_time_editable']['field'] = 'field_pc_start_time_editable';
  $handler->display->display_options['fields']['field_pc_start_time_editable']['label'] = 'Pick Confirm Start Time (Format : HH:MM)';
  $handler->display->display_options['fields']['field_pc_start_time_editable']['type'] = 'services';
  $handler->display->display_options['fields']['field_pc_start_time_editable']['settings'] = array(
    'data_element_key' => '',
    'skip_safe' => 0,
    'skip_empty_values' => 0,
    'skip_text_format' => 1,
  );
  $handler->display->display_options['fields']['field_pc_start_time_editable']['widget_type'] = '0';
  $handler->display->display_options['fields']['field_pc_start_time_editable']['suppress_label'] = 1;
  $handler->display->display_options['fields']['field_pc_start_time_editable']['suppress_description'] = 0;
  /* Field: Content: Pick Confirm End time (editable) */
  $handler->display->display_options['fields']['field_pc_end_time_editable']['id'] = 'field_pc_end_time_editable';
  $handler->display->display_options['fields']['field_pc_end_time_editable']['table'] = 'field_data_field_pc_end_time';
  $handler->display->display_options['fields']['field_pc_end_time_editable']['field'] = 'field_pc_end_time_editable';
  $handler->display->display_options['fields']['field_pc_end_time_editable']['label'] = 'Pick Confirm End time (Format : HH:MM)';
  $handler->display->display_options['fields']['field_pc_end_time_editable']['type'] = 'services';
  $handler->display->display_options['fields']['field_pc_end_time_editable']['settings'] = array(
    'data_element_key' => '',
    'skip_safe' => 0,
    'skip_empty_values' => 0,
    'skip_text_format' => 1,
  );
  $handler->display->display_options['fields']['field_pc_end_time_editable']['widget_type'] = '0';
  $handler->display->display_options['fields']['field_pc_end_time_editable']['suppress_label'] = 1;
  $handler->display->display_options['fields']['field_pc_end_time_editable']['suppress_description'] = 0;
  /* Field: Content: Status */
  $handler->display->display_options['fields']['field_pc_status']['id'] = 'field_pc_status';
  $handler->display->display_options['fields']['field_pc_status']['table'] = 'field_data_field_pc_status';
  $handler->display->display_options['fields']['field_pc_status']['field'] = 'field_pc_status';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Content: Loading Date (field_pc_loading_date) */
  $handler->display->display_options['sorts']['field_pc_loading_date_value']['id'] = 'field_pc_loading_date_value';
  $handler->display->display_options['sorts']['field_pc_loading_date_value']['table'] = 'field_data_field_pc_loading_date';
  $handler->display->display_options['sorts']['field_pc_loading_date_value']['field'] = 'field_pc_loading_date_value';
  $handler->display->display_options['sorts']['field_pc_loading_date_value']['order'] = 'DESC';
  /* Sort criterion: Content: VAN ID (field_pc_van_id) */
  $handler->display->display_options['sorts']['field_pc_van_id_value']['id'] = 'field_pc_van_id_value';
  $handler->display->display_options['sorts']['field_pc_van_id_value']['table'] = 'field_data_field_pc_van_id';
  $handler->display->display_options['sorts']['field_pc_van_id_value']['field'] = 'field_pc_van_id_value';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
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
  /* Filter criterion: Content: Loading Date (field_pc_loading_date) */
  $handler->display->display_options['filters']['field_pc_loading_date_value']['id'] = 'field_pc_loading_date_value';
  $handler->display->display_options['filters']['field_pc_loading_date_value']['table'] = 'field_data_field_pc_loading_date';
  $handler->display->display_options['filters']['field_pc_loading_date_value']['field'] = 'field_pc_loading_date_value';
  $handler->display->display_options['filters']['field_pc_loading_date_value']['operator'] = 'between';
  $handler->display->display_options['filters']['field_pc_loading_date_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_pc_loading_date_value']['expose']['operator_id'] = 'field_pc_loading_date_value_op';
  $handler->display->display_options['filters']['field_pc_loading_date_value']['expose']['label'] = 'Loading Date';
  $handler->display->display_options['filters']['field_pc_loading_date_value']['expose']['operator'] = 'field_pc_loading_date_value_op';
  $handler->display->display_options['filters']['field_pc_loading_date_value']['expose']['identifier'] = 'field_pc_loading_date_value';
  $handler->display->display_options['filters']['field_pc_loading_date_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
    6 => 0,
    7 => 0,
    8 => 0,
    9 => 0,
    10 => 0,
  );
  $handler->display->display_options['filters']['field_pc_loading_date_value']['form_type'] = 'date_popup';
  $handler->display->display_options['filters']['field_pc_loading_date_value']['default_date'] = 'now';
  $handler->display->display_options['filters']['field_pc_loading_date_value']['default_to_date'] = 'now';
  /* Filter criterion: Content: Pick Confirm Type (field_pc_picking_type) */
  $handler->display->display_options['filters']['field_pc_picking_type_value']['id'] = 'field_pc_picking_type_value';
  $handler->display->display_options['filters']['field_pc_picking_type_value']['table'] = 'field_data_field_pc_picking_type';
  $handler->display->display_options['filters']['field_pc_picking_type_value']['field'] = 'field_pc_picking_type_value';
  $handler->display->display_options['filters']['field_pc_picking_type_value']['operator'] = 'contains';
  $handler->display->display_options['filters']['field_pc_picking_type_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_pc_picking_type_value']['expose']['operator_id'] = 'field_pc_picking_type_value_op';
  $handler->display->display_options['filters']['field_pc_picking_type_value']['expose']['label'] = 'Pick Confirm Type';
  $handler->display->display_options['filters']['field_pc_picking_type_value']['expose']['operator'] = 'field_pc_picking_type_value_op';
  $handler->display->display_options['filters']['field_pc_picking_type_value']['expose']['identifier'] = 'field_pc_picking_type_value';
  $handler->display->display_options['filters']['field_pc_picking_type_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
    6 => 0,
    7 => 0,
    8 => 0,
    9 => 0,
    10 => 0,
  );
  /* Filter criterion: Content: Status (field_pc_status) */
  $handler->display->display_options['filters']['field_pc_status_value']['id'] = 'field_pc_status_value';
  $handler->display->display_options['filters']['field_pc_status_value']['table'] = 'field_data_field_pc_status';
  $handler->display->display_options['filters']['field_pc_status_value']['field'] = 'field_pc_status_value';
  $handler->display->display_options['filters']['field_pc_status_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_pc_status_value']['expose']['operator_id'] = 'field_pc_status_value_op';
  $handler->display->display_options['filters']['field_pc_status_value']['expose']['label'] = 'Status';
  $handler->display->display_options['filters']['field_pc_status_value']['expose']['operator'] = 'field_pc_status_value_op';
  $handler->display->display_options['filters']['field_pc_status_value']['expose']['identifier'] = 'field_pc_status_value';
  $handler->display->display_options['filters']['field_pc_status_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
    6 => 0,
    7 => 0,
    8 => 0,
    9 => 0,
    10 => 0,
  );
  $handler->display->display_options['path'] = 'pick-confirm-report/edit';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Edit';
  $handler->display->display_options['menu']['weight'] = '1';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Data export */
  $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
  $handler->display->display_options['defaults']['exposed_form'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'better_exposed_filters';
  $handler->display->display_options['exposed_form']['options']['bef'] = array(
    'general' => array(
      'allow_secondary' => 0,
      'secondary_label' => 'Advanced options',
      'collapsible_label' => NULL,
      'combine_rewrite' => NULL,
      'reset_label' => NULL,
      'bef_filter_description' => NULL,
      'any_label' => NULL,
      'filter_rewrite_values' => NULL,
    ),
    'field_pc_loading_date_value' => array(
      'bef_format' => 'default',
      'more_options' => array(
        'is_secondary' => 0,
        'any_label' => '',
        'bef_filter_description' => '',
        'tokens' => array(
          'available' => array(
            0 => 'global_types',
            'secondary_label' => NULL,
            'collapsible_label' => NULL,
            'combine_rewrite' => NULL,
            'reset_label' => NULL,
            'bef_filter_description' => NULL,
            'any_label' => NULL,
            'filter_rewrite_values' => NULL,
          ),
          'secondary_label' => NULL,
          'collapsible_label' => NULL,
          'combine_rewrite' => NULL,
          'reset_label' => NULL,
          'bef_filter_description' => NULL,
          'any_label' => NULL,
          'filter_rewrite_values' => NULL,
        ),
        'rewrite' => array(
          'filter_rewrite_values' => '',
          'secondary_label' => NULL,
          'collapsible_label' => NULL,
          'combine_rewrite' => NULL,
          'reset_label' => NULL,
          'bef_filter_description' => NULL,
          'any_label' => NULL,
        ),
        'secondary_label' => NULL,
        'collapsible_label' => NULL,
        'combine_rewrite' => NULL,
        'reset_label' => NULL,
        'filter_rewrite_values' => NULL,
      ),
      'secondary_label' => NULL,
      'collapsible_label' => NULL,
      'combine_rewrite' => NULL,
      'reset_label' => NULL,
      'bef_filter_description' => NULL,
      'any_label' => NULL,
      'filter_rewrite_values' => NULL,
    ),
    'secondary_label' => NULL,
    'collapsible_label' => NULL,
    'combine_rewrite' => NULL,
    'reset_label' => NULL,
    'bef_filter_description' => NULL,
    'any_label' => NULL,
    'filter_rewrite_values' => NULL,
  );
  $handler->display->display_options['exposed_form']['options']['input_required'] = 0;
  $handler->display->display_options['exposed_form']['options']['text_input_required_format'] = 'filtered_html';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_data_export_xls';
  $handler->display->display_options['style_options']['provide_file'] = 1;
  $handler->display->display_options['style_options']['parent_sort'] = 0;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Global: View result counter */
  $handler->display->display_options['fields']['counter']['id'] = 'counter';
  $handler->display->display_options['fields']['counter']['table'] = 'views';
  $handler->display->display_options['fields']['counter']['field'] = 'counter';
  $handler->display->display_options['fields']['counter']['label'] = 'Sr. No.';
  $handler->display->display_options['fields']['counter']['counter_start'] = '1';
  $handler->display->display_options['fields']['counter']['reverse'] = 0;
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
  /* Field: Content: VAN ID */
  $handler->display->display_options['fields']['field_pc_van_id']['id'] = 'field_pc_van_id';
  $handler->display->display_options['fields']['field_pc_van_id']['table'] = 'field_data_field_pc_van_id';
  $handler->display->display_options['fields']['field_pc_van_id']['field'] = 'field_pc_van_id';
  /* Field: Content: Pick Confirm Type */
  $handler->display->display_options['fields']['field_pc_picking_type']['id'] = 'field_pc_picking_type';
  $handler->display->display_options['fields']['field_pc_picking_type']['table'] = 'field_data_field_pc_picking_type';
  $handler->display->display_options['fields']['field_pc_picking_type']['field'] = 'field_pc_picking_type';
  /* Field: Content: PICK CONFIRM CLDs */
  $handler->display->display_options['fields']['field_pc_pick_clds']['id'] = 'field_pc_pick_clds';
  $handler->display->display_options['fields']['field_pc_pick_clds']['table'] = 'field_data_field_pc_pick_clds';
  $handler->display->display_options['fields']['field_pc_pick_clds']['field'] = 'field_pc_pick_clds';
  $handler->display->display_options['fields']['field_pc_pick_clds']['settings'] = array(
    'thousand_separator' => '',
    'prefix_suffix' => 1,
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
  /* Field: Content: PICK CONFIRM LINES */
  $handler->display->display_options['fields']['field_pc_pick_lines_1']['id'] = 'field_pc_pick_lines_1';
  $handler->display->display_options['fields']['field_pc_pick_lines_1']['table'] = 'field_data_field_pc_pick_lines';
  $handler->display->display_options['fields']['field_pc_pick_lines_1']['field'] = 'field_pc_pick_lines';
  $handler->display->display_options['fields']['field_pc_pick_lines_1']['label'] = 'LOADING SHEET LINE';
  $handler->display->display_options['fields']['field_pc_pick_lines_1']['settings'] = array(
    'thousand_separator' => '',
    'prefix_suffix' => 1,
  );
  /* Field: Content: PICK CONFIRM SUPERVISOR */
  $handler->display->display_options['fields']['field_pc_pick_supervisor']['id'] = 'field_pc_pick_supervisor';
  $handler->display->display_options['fields']['field_pc_pick_supervisor']['table'] = 'field_data_field_pc_pick_supervisor';
  $handler->display->display_options['fields']['field_pc_pick_supervisor']['field'] = 'field_pc_pick_supervisor';
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
  /* Field: Content: Pick Confirm Date */
  $handler->display->display_options['fields']['field_pc_picking_date']['id'] = 'field_pc_picking_date';
  $handler->display->display_options['fields']['field_pc_picking_date']['table'] = 'field_data_field_pc_picking_date';
  $handler->display->display_options['fields']['field_pc_picking_date']['field'] = 'field_pc_picking_date';
  $handler->display->display_options['fields']['field_pc_picking_date']['settings'] = array(
    'format_type' => 'short',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_remaining_days' => 0,
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
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Content: Loading Date (field_pc_loading_date) */
  $handler->display->display_options['sorts']['field_pc_loading_date_value']['id'] = 'field_pc_loading_date_value';
  $handler->display->display_options['sorts']['field_pc_loading_date_value']['table'] = 'field_data_field_pc_loading_date';
  $handler->display->display_options['sorts']['field_pc_loading_date_value']['field'] = 'field_pc_loading_date_value';
  $handler->display->display_options['sorts']['field_pc_loading_date_value']['order'] = 'DESC';
  /* Sort criterion: Content: VAN ID (field_pc_van_id) */
  $handler->display->display_options['sorts']['field_pc_van_id_value']['id'] = 'field_pc_van_id_value';
  $handler->display->display_options['sorts']['field_pc_van_id_value']['table'] = 'field_data_field_pc_van_id';
  $handler->display->display_options['sorts']['field_pc_van_id_value']['field'] = 'field_pc_van_id_value';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
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
  /* Filter criterion: Content: Loading Date (field_pc_loading_date) */
  $handler->display->display_options['filters']['field_pc_loading_date_value']['id'] = 'field_pc_loading_date_value';
  $handler->display->display_options['filters']['field_pc_loading_date_value']['table'] = 'field_data_field_pc_loading_date';
  $handler->display->display_options['filters']['field_pc_loading_date_value']['field'] = 'field_pc_loading_date_value';
  $handler->display->display_options['filters']['field_pc_loading_date_value']['operator'] = 'between';
  $handler->display->display_options['filters']['field_pc_loading_date_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_pc_loading_date_value']['expose']['operator_id'] = 'field_pc_loading_date_value_op';
  $handler->display->display_options['filters']['field_pc_loading_date_value']['expose']['label'] = 'Loading Date';
  $handler->display->display_options['filters']['field_pc_loading_date_value']['expose']['operator'] = 'field_pc_loading_date_value_op';
  $handler->display->display_options['filters']['field_pc_loading_date_value']['expose']['identifier'] = 'field_pc_loading_date_value';
  $handler->display->display_options['filters']['field_pc_loading_date_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
    6 => 0,
    7 => 0,
    8 => 0,
    9 => 0,
    10 => 0,
  );
  $handler->display->display_options['filters']['field_pc_loading_date_value']['form_type'] = 'date_popup';
  $handler->display->display_options['filters']['field_pc_loading_date_value']['default_date'] = 'now';
  $handler->display->display_options['filters']['field_pc_loading_date_value']['default_to_date'] = 'now';
  /* Filter criterion: Content: Pick Confirm Type (field_pc_picking_type) */
  $handler->display->display_options['filters']['field_pc_picking_type_value']['id'] = 'field_pc_picking_type_value';
  $handler->display->display_options['filters']['field_pc_picking_type_value']['table'] = 'field_data_field_pc_picking_type';
  $handler->display->display_options['filters']['field_pc_picking_type_value']['field'] = 'field_pc_picking_type_value';
  $handler->display->display_options['filters']['field_pc_picking_type_value']['operator'] = 'contains';
  $handler->display->display_options['path'] = 'pick-confirm-report-download';
  $handler->display->display_options['displays'] = array(
    'default' => 'default',
    'page' => 'page',
    'page_1' => 0,
  );
  $handler->display->display_options['use_batch'] = 'batch';
  $handler->display->display_options['segment_size'] = '100';
  $export['pick_confirm_report'] = $view;

  return $export;
}
