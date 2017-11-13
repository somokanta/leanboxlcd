<?php
/**
 * @file
 * leanbox_petty_cash_feature.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function leanbox_petty_cash_feature_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'petty_cash_edit';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'taxonomy_term_data';
  $view->human_name = 'Petty Cash Edit';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Petty Cash Edit';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    8 => '8',
    9 => '9',
    7 => '7',
  );
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'editableviews_table';
  /* Field: Taxonomy term: Name */
  $handler->display->display_options['fields']['name_1']['id'] = 'name_1';
  $handler->display->display_options['fields']['name_1']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['name_1']['field'] = 'name';
  /* Field: Taxonomy term: Petty Cash Head (editable) */
  $handler->display->display_options['fields']['field_petty_cash_head_editable']['id'] = 'field_petty_cash_head_editable';
  $handler->display->display_options['fields']['field_petty_cash_head_editable']['table'] = 'field_data_field_petty_cash_head';
  $handler->display->display_options['fields']['field_petty_cash_head_editable']['field'] = 'field_petty_cash_head_editable';
  $handler->display->display_options['fields']['field_petty_cash_head_editable']['label'] = 'Head';
  $handler->display->display_options['fields']['field_petty_cash_head_editable']['widget_type'] = 'text_textfield';
  $handler->display->display_options['fields']['field_petty_cash_head_editable']['suppress_label'] = 1;
  $handler->display->display_options['fields']['field_petty_cash_head_editable']['suppress_description'] = 1;
  /* Field: Taxonomy term: Petty Cash Subhead (editable) */
  $handler->display->display_options['fields']['field_petty_cash_subhead_editable']['id'] = 'field_petty_cash_subhead_editable';
  $handler->display->display_options['fields']['field_petty_cash_subhead_editable']['table'] = 'field_data_field_petty_cash_subhead';
  $handler->display->display_options['fields']['field_petty_cash_subhead_editable']['field'] = 'field_petty_cash_subhead_editable';
  $handler->display->display_options['fields']['field_petty_cash_subhead_editable']['label'] = 'Subhead';
  $handler->display->display_options['fields']['field_petty_cash_subhead_editable']['widget_type'] = 'text_textfield';
  $handler->display->display_options['fields']['field_petty_cash_subhead_editable']['suppress_label'] = 1;
  $handler->display->display_options['fields']['field_petty_cash_subhead_editable']['suppress_description'] = 0;
  /* Field: Taxonomy term: Petty Cash Type */
  $handler->display->display_options['fields']['field_petty_cash_type']['id'] = 'field_petty_cash_type';
  $handler->display->display_options['fields']['field_petty_cash_type']['table'] = 'field_data_field_petty_cash_type';
  $handler->display->display_options['fields']['field_petty_cash_type']['field'] = 'field_petty_cash_type';
  $handler->display->display_options['fields']['field_petty_cash_type']['label'] = 'Type';
  /* Filter criterion: Taxonomy vocabulary: Machine name */
  $handler->display->display_options['filters']['machine_name']['id'] = 'machine_name';
  $handler->display->display_options['filters']['machine_name']['table'] = 'taxonomy_vocabulary';
  $handler->display->display_options['filters']['machine_name']['field'] = 'machine_name';
  $handler->display->display_options['filters']['machine_name']['value'] = array(
    'petty_cash' => 'petty_cash',
  );
  /* Filter criterion: Global: Combine fields filter */
  $handler->display->display_options['filters']['combine']['id'] = 'combine';
  $handler->display->display_options['filters']['combine']['table'] = 'views';
  $handler->display->display_options['filters']['combine']['field'] = 'combine';
  $handler->display->display_options['filters']['combine']['exposed'] = TRUE;
  $handler->display->display_options['filters']['combine']['expose']['operator_id'] = 'combine_op';
  $handler->display->display_options['filters']['combine']['expose']['label'] = 'Combine fields filter';
  $handler->display->display_options['filters']['combine']['expose']['operator'] = 'combine_op';
  $handler->display->display_options['filters']['combine']['expose']['identifier'] = 'combine';
  $handler->display->display_options['filters']['combine']['expose']['remember_roles'] = array(
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
    11 => 0,
    12 => 0,
    13 => 0,
  );
  $handler->display->display_options['filters']['combine']['fields'] = array(
    'field_petty_cash_head_editable' => 'field_petty_cash_head_editable',
    'field_petty_cash_subhead_editable' => 'field_petty_cash_subhead_editable',
  );
  /* Filter criterion: Taxonomy term: Petty Cash Type (field_petty_cash_type) */
  $handler->display->display_options['filters']['field_petty_cash_type_value']['id'] = 'field_petty_cash_type_value';
  $handler->display->display_options['filters']['field_petty_cash_type_value']['table'] = 'field_data_field_petty_cash_type';
  $handler->display->display_options['filters']['field_petty_cash_type_value']['field'] = 'field_petty_cash_type_value';
  $handler->display->display_options['filters']['field_petty_cash_type_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_petty_cash_type_value']['expose']['operator_id'] = 'field_petty_cash_type_value_op';
  $handler->display->display_options['filters']['field_petty_cash_type_value']['expose']['label'] = 'Petty Cash Type';
  $handler->display->display_options['filters']['field_petty_cash_type_value']['expose']['operator'] = 'field_petty_cash_type_value_op';
  $handler->display->display_options['filters']['field_petty_cash_type_value']['expose']['identifier'] = 'field_petty_cash_type_value';
  $handler->display->display_options['filters']['field_petty_cash_type_value']['expose']['remember_roles'] = array(
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
    11 => 0,
    12 => 0,
    13 => 0,
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'petty-cash-edit';
  $export['petty_cash_edit'] = $view;

  return $export;
}
