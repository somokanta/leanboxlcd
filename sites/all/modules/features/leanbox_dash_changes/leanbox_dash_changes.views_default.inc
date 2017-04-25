<?php
/**
 * @file
 * leanbox_dash_changes.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function leanbox_dash_changes_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'productivity_target';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'taxonomy_term_data';
  $view->human_name = 'Productivity target';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Productivity target';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    3 => '3',
  );
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'editableviews_table';
  $handler->display->display_options['style_options']['columns'] = array(
    'name' => 'name',
    'field_target' => 'field_target',
    'field_target_distributor' => 'field_target_distributor',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_target' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_target_distributor' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['style_options']['batch_size'] = '10';
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['empty'] = TRUE;
  $handler->display->display_options['header']['area']['content'] = '<a href = "/admin/structure/taxonomy/productivity_target/add">Add New</a>';
  $handler->display->display_options['header']['area']['format'] = 'filtered_html';
  /* Field: Taxonomy term: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = '';
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['name']['link_to_taxonomy'] = TRUE;
  /* Field: Taxonomy term: Target Distributor */
  $handler->display->display_options['fields']['field_target_distributor']['id'] = 'field_target_distributor';
  $handler->display->display_options['fields']['field_target_distributor']['table'] = 'field_data_field_target_distributor';
  $handler->display->display_options['fields']['field_target_distributor']['field'] = 'field_target_distributor';
  $handler->display->display_options['fields']['field_target_distributor']['label'] = 'Distributor';
  $handler->display->display_options['fields']['field_target_distributor']['type'] = 'user_reference_plain';
  /* Field: Taxonomy term: Target (editable) */
  $handler->display->display_options['fields']['field_target_editable']['id'] = 'field_target_editable';
  $handler->display->display_options['fields']['field_target_editable']['table'] = 'field_data_field_target';
  $handler->display->display_options['fields']['field_target_editable']['field'] = 'field_target_editable';
  $handler->display->display_options['fields']['field_target_editable']['label'] = 'Target';
  $handler->display->display_options['fields']['field_target_editable']['settings'] = array(
    'thousand_separator' => '',
    'prefix_suffix' => 1,
  );
  $handler->display->display_options['fields']['field_target_editable']['widget_type'] = '0';
  $handler->display->display_options['fields']['field_target_editable']['suppress_label'] = 0;
  $handler->display->display_options['fields']['field_target_editable']['suppress_description'] = 0;
  /* Filter criterion: Taxonomy vocabulary: Machine name */
  $handler->display->display_options['filters']['machine_name']['id'] = 'machine_name';
  $handler->display->display_options['filters']['machine_name']['table'] = 'taxonomy_vocabulary';
  $handler->display->display_options['filters']['machine_name']['field'] = 'machine_name';
  $handler->display->display_options['filters']['machine_name']['value'] = array(
    'productivity_target' => 'productivity_target',
  );
  /* Filter criterion: Taxonomy term: Target Distributor (field_target_distributor) */
  $handler->display->display_options['filters']['field_target_distributor_uid']['id'] = 'field_target_distributor_uid';
  $handler->display->display_options['filters']['field_target_distributor_uid']['table'] = 'field_data_field_target_distributor';
  $handler->display->display_options['filters']['field_target_distributor_uid']['field'] = 'field_target_distributor_uid';
  $handler->display->display_options['filters']['field_target_distributor_uid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_target_distributor_uid']['expose']['operator_id'] = 'field_target_distributor_uid_op';
  $handler->display->display_options['filters']['field_target_distributor_uid']['expose']['label'] = 'Distributor';
  $handler->display->display_options['filters']['field_target_distributor_uid']['expose']['operator'] = 'field_target_distributor_uid_op';
  $handler->display->display_options['filters']['field_target_distributor_uid']['expose']['identifier'] = 'field_target_distributor_uid';
  $handler->display->display_options['filters']['field_target_distributor_uid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'productivity-target';
  $export['productivity_target'] = $view;

  return $export;
}
