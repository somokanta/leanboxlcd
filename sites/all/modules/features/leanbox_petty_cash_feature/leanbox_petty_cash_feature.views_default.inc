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

  $view = new view();
  $view->name = 'petty_cash_transaction';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'petty_cash';
  $view->human_name = 'Petty cash transaction';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Petty cash transaction';
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
  $handler->display->display_options['style_plugin'] = 'table';
  /* Relationship: petty cash: file */
  $handler->display->display_options['relationships']['file']['id'] = 'file';
  $handler->display->display_options['relationships']['file']['table'] = 'petty_cash';
  $handler->display->display_options['relationships']['file']['field'] = 'file';
  /* Relationship: petty cash: Bill file */
  $handler->display->display_options['relationships']['bill_file']['id'] = 'bill_file';
  $handler->display->display_options['relationships']['bill_file']['table'] = 'petty_cash';
  $handler->display->display_options['relationships']['bill_file']['field'] = 'bill_file';
  /* Field: petty cash: Serial ID */
  $handler->display->display_options['fields']['id']['id'] = 'id';
  $handler->display->display_options['fields']['id']['table'] = 'petty_cash';
  $handler->display->display_options['fields']['id']['field'] = 'id';
  /* Field: petty cash: Amount */
  $handler->display->display_options['fields']['amount']['id'] = 'amount';
  $handler->display->display_options['fields']['amount']['table'] = 'petty_cash';
  $handler->display->display_options['fields']['amount']['field'] = 'amount';
  /* Field: petty cash: Balance */
  $handler->display->display_options['fields']['balance']['id'] = 'balance';
  $handler->display->display_options['fields']['balance']['table'] = 'petty_cash';
  $handler->display->display_options['fields']['balance']['field'] = 'balance';
  /* Field: petty cash: Bill Available */
  $handler->display->display_options['fields']['bill_available']['id'] = 'bill_available';
  $handler->display->display_options['fields']['bill_available']['table'] = 'petty_cash';
  $handler->display->display_options['fields']['bill_available']['field'] = 'bill_available';
  /* Field: petty cash: Bill no */
  $handler->display->display_options['fields']['bill_no']['id'] = 'bill_no';
  $handler->display->display_options['fields']['bill_no']['table'] = 'petty_cash';
  $handler->display->display_options['fields']['bill_no']['field'] = 'bill_no';
  /* Field: petty cash: Deleted */
  $handler->display->display_options['fields']['deleted']['id'] = 'deleted';
  $handler->display->display_options['fields']['deleted']['table'] = 'petty_cash';
  $handler->display->display_options['fields']['deleted']['field'] = 'deleted';
  /* Field: petty cash: description */
  $handler->display->display_options['fields']['description']['id'] = 'description';
  $handler->display->display_options['fields']['description']['table'] = 'petty_cash';
  $handler->display->display_options['fields']['description']['field'] = 'description';
  /* Field: petty cash: Distributor_id */
  $handler->display->display_options['fields']['distributor_id']['id'] = 'distributor_id';
  $handler->display->display_options['fields']['distributor_id']['table'] = 'petty_cash';
  $handler->display->display_options['fields']['distributor_id']['field'] = 'distributor_id';
  /* Field: petty cash: File date */
  $handler->display->display_options['fields']['file_date']['id'] = 'file_date';
  $handler->display->display_options['fields']['file_date']['table'] = 'petty_cash';
  $handler->display->display_options['fields']['file_date']['field'] = 'file_date';
  $handler->display->display_options['fields']['file_date']['date_format'] = 'long';
  $handler->display->display_options['fields']['file_date']['second_date_format'] = 'long';
  /* Field: petty cash: Head */
  $handler->display->display_options['fields']['head']['id'] = 'head';
  $handler->display->display_options['fields']['head']['table'] = 'petty_cash';
  $handler->display->display_options['fields']['head']['field'] = 'head';
  /* Field: petty cash: Person name */
  $handler->display->display_options['fields']['person_name']['id'] = 'person_name';
  $handler->display->display_options['fields']['person_name']['table'] = 'petty_cash';
  $handler->display->display_options['fields']['person_name']['field'] = 'person_name';
  /* Field: petty cash: Subhead */
  $handler->display->display_options['fields']['subhead']['id'] = 'subhead';
  $handler->display->display_options['fields']['subhead']['table'] = 'petty_cash';
  $handler->display->display_options['fields']['subhead']['field'] = 'subhead';
  /* Field: petty cash: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'petty_cash';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  /* Field: petty cash: Voucher no */
  $handler->display->display_options['fields']['voucher_no']['id'] = 'voucher_no';
  $handler->display->display_options['fields']['voucher_no']['table'] = 'petty_cash';
  $handler->display->display_options['fields']['voucher_no']['field'] = 'voucher_no';
  /* Field: petty cash: Voucher reference no */
  $handler->display->display_options['fields']['voucher_reference_no']['id'] = 'voucher_reference_no';
  $handler->display->display_options['fields']['voucher_reference_no']['table'] = 'petty_cash';
  $handler->display->display_options['fields']['voucher_reference_no']['field'] = 'voucher_reference_no';
  /* Field: petty cash: Voucher date */
  $handler->display->display_options['fields']['voucher_date']['id'] = 'voucher_date';
  $handler->display->display_options['fields']['voucher_date']['table'] = 'petty_cash';
  $handler->display->display_options['fields']['voucher_date']['field'] = 'voucher_date';
  $handler->display->display_options['fields']['voucher_date']['date_format'] = 'short';
  $handler->display->display_options['fields']['voucher_date']['second_date_format'] = 'long';
  /* Field: File: Name */
  $handler->display->display_options['fields']['filename']['id'] = 'filename';
  $handler->display->display_options['fields']['filename']['table'] = 'file_managed';
  $handler->display->display_options['fields']['filename']['field'] = 'filename';
  $handler->display->display_options['fields']['filename']['relationship'] = 'file';
  $handler->display->display_options['fields']['filename']['label'] = 'File';
  $handler->display->display_options['fields']['filename']['link_to_file'] = TRUE;
  /* Field: File: Name */
  $handler->display->display_options['fields']['filename_1']['id'] = 'filename_1';
  $handler->display->display_options['fields']['filename_1']['table'] = 'file_managed';
  $handler->display->display_options['fields']['filename_1']['field'] = 'filename';
  $handler->display->display_options['fields']['filename_1']['relationship'] = 'bill_file';
  $handler->display->display_options['fields']['filename_1']['label'] = 'Bill File';
  $handler->display->display_options['fields']['filename_1']['link_to_file'] = TRUE;
  /* Filter criterion: Date: Date (petty_cash) */
  $handler->display->display_options['filters']['date_filter_1']['id'] = 'date_filter_1';
  $handler->display->display_options['filters']['date_filter_1']['table'] = 'petty_cash';
  $handler->display->display_options['filters']['date_filter_1']['field'] = 'date_filter';
  $handler->display->display_options['filters']['date_filter_1']['operator'] = 'between';
  $handler->display->display_options['filters']['date_filter_1']['exposed'] = TRUE;
  $handler->display->display_options['filters']['date_filter_1']['expose']['operator_id'] = 'date_filter_1_op';
  $handler->display->display_options['filters']['date_filter_1']['expose']['label'] = ' File Date';
  $handler->display->display_options['filters']['date_filter_1']['expose']['operator'] = 'date_filter_1_op';
  $handler->display->display_options['filters']['date_filter_1']['expose']['identifier'] = 'date_filter_1';
  $handler->display->display_options['filters']['date_filter_1']['expose']['remember_roles'] = array(
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
  $handler->display->display_options['filters']['date_filter_1']['form_type'] = 'date_popup';
  $handler->display->display_options['filters']['date_filter_1']['date_fields'] = array(
    'petty_cash.file_date' => 'petty_cash.file_date',
  );
  /* Filter criterion: Date: Date (petty_cash) */
  $handler->display->display_options['filters']['date_filter']['id'] = 'date_filter';
  $handler->display->display_options['filters']['date_filter']['table'] = 'petty_cash';
  $handler->display->display_options['filters']['date_filter']['field'] = 'date_filter';
  $handler->display->display_options['filters']['date_filter']['operator'] = 'between';
  $handler->display->display_options['filters']['date_filter']['exposed'] = TRUE;
  $handler->display->display_options['filters']['date_filter']['expose']['operator_id'] = 'date_filter_op';
  $handler->display->display_options['filters']['date_filter']['expose']['label'] = 'Voucher Date';
  $handler->display->display_options['filters']['date_filter']['expose']['operator'] = 'date_filter_op';
  $handler->display->display_options['filters']['date_filter']['expose']['identifier'] = 'date_filter';
  $handler->display->display_options['filters']['date_filter']['expose']['remember_roles'] = array(
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
  $handler->display->display_options['filters']['date_filter']['form_type'] = 'date_popup';
  $handler->display->display_options['filters']['date_filter']['date_fields'] = array(
    'petty_cash.voucher_date' => 'petty_cash.voucher_date',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'petty-cash-transaction';

  /* Display: Data export */
  $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['style_options']['provide_file'] = 1;
  $handler->display->display_options['style_options']['parent_sort'] = 0;
  $handler->display->display_options['style_options']['quote'] = 1;
  $handler->display->display_options['style_options']['trim'] = 0;
  $handler->display->display_options['style_options']['replace_newlines'] = 0;
  $handler->display->display_options['style_options']['newline_token'] = '1';
  $handler->display->display_options['style_options']['header'] = 1;
  $handler->display->display_options['style_options']['keep_html'] = 0;
  $handler->display->display_options['path'] = 'petty-cash-transaction-download';
  $handler->display->display_options['displays'] = array(
    'default' => 'default',
    'page' => 'page',
  );
  $handler->display->display_options['use_batch'] = 'batch';
  $handler->display->display_options['segment_size'] = '100';
  $export['petty_cash_transaction'] = $view;

  return $export;
}
