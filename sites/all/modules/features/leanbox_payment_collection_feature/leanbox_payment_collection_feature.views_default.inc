<?php
/**
 * @file
 * leanbox_payment_collection_feature.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function leanbox_payment_collection_feature_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'assignment_history';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Assignment History';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Assignment History';
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
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Bill No';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Assign Person */
  $handler->display->display_options['fields']['field_assign_person']['id'] = 'field_assign_person';
  $handler->display->display_options['fields']['field_assign_person']['table'] = 'field_data_field_assign_person';
  $handler->display->display_options['fields']['field_assign_person']['field'] = 'field_assign_person';
  $handler->display->display_options['fields']['field_assign_person']['label'] = 'Assign To';
  /* Field: Content: Assign date */
  $handler->display->display_options['fields']['field_assign_date']['id'] = 'field_assign_date';
  $handler->display->display_options['fields']['field_assign_date']['table'] = 'field_data_field_assign_date';
  $handler->display->display_options['fields']['field_assign_date']['field'] = 'field_assign_date';
  $handler->display->display_options['fields']['field_assign_date']['settings'] = array(
    'format_type' => 'sales_day',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_remaining_days' => 0,
  );
  /* Field: Content: Returned Date */
  $handler->display->display_options['fields']['field_returned_date']['id'] = 'field_returned_date';
  $handler->display->display_options['fields']['field_returned_date']['table'] = 'field_data_field_returned_date';
  $handler->display->display_options['fields']['field_returned_date']['field'] = 'field_returned_date';
  $handler->display->display_options['fields']['field_returned_date']['settings'] = array(
    'format_type' => 'sales_day',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_remaining_days' => 0,
  );
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
    'assignment_history' => 'assignment_history',
  );
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Bill No';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
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
    14 => 0,
    13 => 0,
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'assignment-history';

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
  $handler->display->display_options['path'] = 'assignment-history-export';
  $handler->display->display_options['displays'] = array(
    'default' => 'default',
    'page' => 'page',
  );
  $export['assignment_history'] = $view;

  $view = new view();
  $view->name = 'bills_collection';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Bills Collection';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Bills Collection';
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
  /* Relationship: Content: sales bill no ref (field_sales_bill_no_ref) */
  $handler->display->display_options['relationships']['field_sales_bill_no_ref_nid']['id'] = 'field_sales_bill_no_ref_nid';
  $handler->display->display_options['relationships']['field_sales_bill_no_ref_nid']['table'] = 'field_data_field_sales_bill_no_ref';
  $handler->display->display_options['relationships']['field_sales_bill_no_ref_nid']['field'] = 'field_sales_bill_no_ref_nid';
  $handler->display->display_options['relationships']['field_sales_bill_no_ref_nid']['delta'] = '-1';
  /* Relationship: Content: party_code_reference (field_party_code_reference) */
  $handler->display->display_options['relationships']['field_party_code_reference_nid']['id'] = 'field_party_code_reference_nid';
  $handler->display->display_options['relationships']['field_party_code_reference_nid']['table'] = 'field_data_field_party_code_reference';
  $handler->display->display_options['relationships']['field_party_code_reference_nid']['field'] = 'field_party_code_reference_nid';
  $handler->display->display_options['relationships']['field_party_code_reference_nid']['relationship'] = 'field_sales_bill_no_ref_nid';
  $handler->display->display_options['relationships']['field_party_code_reference_nid']['delta'] = '-1';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Bill No.';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Content: Bill Date */
  $handler->display->display_options['fields']['field_sr_bill_date']['id'] = 'field_sr_bill_date';
  $handler->display->display_options['fields']['field_sr_bill_date']['table'] = 'field_data_field_sr_bill_date';
  $handler->display->display_options['fields']['field_sr_bill_date']['field'] = 'field_sr_bill_date';
  $handler->display->display_options['fields']['field_sr_bill_date']['relationship'] = 'field_sales_bill_no_ref_nid';
  $handler->display->display_options['fields']['field_sr_bill_date']['settings'] = array(
    'format_type' => 'sales_day',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_remaining_days' => 0,
  );
  /* Field: Content: Flag */
  $handler->display->display_options['fields']['field_flag']['id'] = 'field_flag';
  $handler->display->display_options['fields']['field_flag']['table'] = 'field_data_field_flag';
  $handler->display->display_options['fields']['field_flag']['field'] = 'field_flag';
  $handler->display->display_options['fields']['field_flag']['label'] = 'Type';
  /* Field: Content: party_code_reference */
  $handler->display->display_options['fields']['field_party_code_reference']['id'] = 'field_party_code_reference';
  $handler->display->display_options['fields']['field_party_code_reference']['table'] = 'field_data_field_party_code_reference';
  $handler->display->display_options['fields']['field_party_code_reference']['field'] = 'field_party_code_reference';
  $handler->display->display_options['fields']['field_party_code_reference']['relationship'] = 'field_sales_bill_no_ref_nid';
  $handler->display->display_options['fields']['field_party_code_reference']['label'] = 'Party Code';
  $handler->display->display_options['fields']['field_party_code_reference']['type'] = 'node_reference_plain';
  /* Field: Content: Beat Name */
  $handler->display->display_options['fields']['field_beat_name']['id'] = 'field_beat_name';
  $handler->display->display_options['fields']['field_beat_name']['table'] = 'field_data_field_beat_name';
  $handler->display->display_options['fields']['field_beat_name']['field'] = 'field_beat_name';
  $handler->display->display_options['fields']['field_beat_name']['relationship'] = 'field_party_code_reference_nid';
  /* Field: Content: Salesman */
  $handler->display->display_options['fields']['field_salesman']['id'] = 'field_salesman';
  $handler->display->display_options['fields']['field_salesman']['table'] = 'field_data_field_salesman';
  $handler->display->display_options['fields']['field_salesman']['field'] = 'field_salesman';
  $handler->display->display_options['fields']['field_salesman']['relationship'] = 'field_party_code_reference_nid';
  /* Field: Content: Salesman Code */
  $handler->display->display_options['fields']['field_salesman_code']['id'] = 'field_salesman_code';
  $handler->display->display_options['fields']['field_salesman_code']['table'] = 'field_data_field_salesman_code';
  $handler->display->display_options['fields']['field_salesman_code']['field'] = 'field_salesman_code';
  $handler->display->display_options['fields']['field_salesman_code']['relationship'] = 'field_party_code_reference_nid';
  /* Field: Content: Bill Value */
  $handler->display->display_options['fields']['field_bill_value']['id'] = 'field_bill_value';
  $handler->display->display_options['fields']['field_bill_value']['table'] = 'field_data_field_bill_value';
  $handler->display->display_options['fields']['field_bill_value']['field'] = 'field_bill_value';
  $handler->display->display_options['fields']['field_bill_value']['relationship'] = 'field_sales_bill_no_ref_nid';
  /* Field: Content: remaining amt collection */
  $handler->display->display_options['fields']['field_remaining_amt_collection']['id'] = 'field_remaining_amt_collection';
  $handler->display->display_options['fields']['field_remaining_amt_collection']['table'] = 'field_data_field_remaining_amt_collection';
  $handler->display->display_options['fields']['field_remaining_amt_collection']['field'] = 'field_remaining_amt_collection';
  $handler->display->display_options['fields']['field_remaining_amt_collection']['label'] = 'To Be Collected';
  $handler->display->display_options['fields']['field_remaining_amt_collection']['settings'] = array(
    'thousand_separator' => '',
    'decimal_separator' => '.',
    'scale' => '2',
    'prefix_suffix' => 1,
  );
  /* Field: Content: Declaration date */
  $handler->display->display_options['fields']['field_declaration_date']['id'] = 'field_declaration_date';
  $handler->display->display_options['fields']['field_declaration_date']['table'] = 'field_data_field_declaration_date';
  $handler->display->display_options['fields']['field_declaration_date']['field'] = 'field_declaration_date';
  $handler->display->display_options['fields']['field_declaration_date']['settings'] = array(
    'format_type' => 'sales_day',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_remaining_days' => 0,
  );
  /* Field: Content: Cheque Number */
  $handler->display->display_options['fields']['field_cheque_number']['id'] = 'field_cheque_number';
  $handler->display->display_options['fields']['field_cheque_number']['table'] = 'field_data_field_cheque_number';
  $handler->display->display_options['fields']['field_cheque_number']['field'] = 'field_cheque_number';
  $handler->display->display_options['fields']['field_cheque_number']['label'] = 'Old Cheque No.';
  /* Field: Content: Cheque date */
  $handler->display->display_options['fields']['field_cheque_date']['id'] = 'field_cheque_date';
  $handler->display->display_options['fields']['field_cheque_date']['table'] = 'field_data_field_cheque_date';
  $handler->display->display_options['fields']['field_cheque_date']['field'] = 'field_cheque_date';
  $handler->display->display_options['fields']['field_cheque_date']['label'] = 'Old Cheque Date';
  $handler->display->display_options['fields']['field_cheque_date']['settings'] = array(
    'format_type' => 'sales_day',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_remaining_days' => 0,
  );
  /* Field: Content: File */
  $handler->display->display_options['fields']['field_file']['id'] = 'field_file';
  $handler->display->display_options['fields']['field_file']['table'] = 'field_data_field_file';
  $handler->display->display_options['fields']['field_file']['field'] = 'field_file';
  $handler->display->display_options['fields']['field_file']['label'] = 'File Upload Url';
  $handler->display->display_options['fields']['field_file']['click_sort_column'] = 'fid';
  /* Field: Content: File upload date */
  $handler->display->display_options['fields']['field_file_upload_date']['id'] = 'field_file_upload_date';
  $handler->display->display_options['fields']['field_file_upload_date']['table'] = 'field_data_field_file_upload_date';
  $handler->display->display_options['fields']['field_file_upload_date']['field'] = 'field_file_upload_date';
  $handler->display->display_options['fields']['field_file_upload_date']['settings'] = array(
    'format_type' => 'sales_day',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_remaining_days' => 0,
  );
  /* Field: Content: Assigned Person */
  $handler->display->display_options['fields']['field_assigned_person']['id'] = 'field_assigned_person';
  $handler->display->display_options['fields']['field_assigned_person']['table'] = 'field_data_field_assigned_person';
  $handler->display->display_options['fields']['field_assigned_person']['field'] = 'field_assigned_person';
  $handler->display->display_options['fields']['field_assigned_person']['label'] = 'Assigned To';
  /* Field: Content: Assignment date */
  $handler->display->display_options['fields']['field_assignment_date']['id'] = 'field_assignment_date';
  $handler->display->display_options['fields']['field_assignment_date']['table'] = 'field_data_field_assignment_date';
  $handler->display->display_options['fields']['field_assignment_date']['field'] = 'field_assignment_date';
  $handler->display->display_options['fields']['field_assignment_date']['settings'] = array(
    'format_type' => 'sales_day',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_remaining_days' => 0,
  );
  /* Field: Content: Collection Status */
  $handler->display->display_options['fields']['field_collection_status']['id'] = 'field_collection_status';
  $handler->display->display_options['fields']['field_collection_status']['table'] = 'field_data_field_collection_status';
  $handler->display->display_options['fields']['field_collection_status']['field'] = 'field_collection_status';
  /* Field: Content: Previous Cash Amt */
  $handler->display->display_options['fields']['field_previous_cash_amt']['id'] = 'field_previous_cash_amt';
  $handler->display->display_options['fields']['field_previous_cash_amt']['table'] = 'field_data_field_previous_cash_amt';
  $handler->display->display_options['fields']['field_previous_cash_amt']['field'] = 'field_previous_cash_amt';
  $handler->display->display_options['fields']['field_previous_cash_amt']['label'] = 'Cash Collected Amt';
  $handler->display->display_options['fields']['field_previous_cash_amt']['settings'] = array(
    'thousand_separator' => '',
    'decimal_separator' => '.',
    'scale' => '2',
    'prefix_suffix' => 1,
  );
  /* Field: Content: Previous Cheque Amt */
  $handler->display->display_options['fields']['field_previous_cheque_amt']['id'] = 'field_previous_cheque_amt';
  $handler->display->display_options['fields']['field_previous_cheque_amt']['table'] = 'field_data_field_previous_cheque_amt';
  $handler->display->display_options['fields']['field_previous_cheque_amt']['field'] = 'field_previous_cheque_amt';
  $handler->display->display_options['fields']['field_previous_cheque_amt']['label'] = 'Cheque Collected Amt';
  $handler->display->display_options['fields']['field_previous_cheque_amt']['settings'] = array(
    'thousand_separator' => '',
    'decimal_separator' => '.',
    'scale' => '2',
    'prefix_suffix' => 1,
  );
  /* Field: Global: Math expression */
  $handler->display->display_options['fields']['expression']['id'] = 'expression';
  $handler->display->display_options['fields']['expression']['table'] = 'views';
  $handler->display->display_options['fields']['expression']['field'] = 'expression';
  $handler->display->display_options['fields']['expression']['label'] = 'Total Amt Collected';
  $handler->display->display_options['fields']['expression']['precision'] = '0';
  $handler->display->display_options['fields']['expression']['expression'] = '[field_previous_cash_amt] + [field_previous_cheque_amt]';
  /* Field: Content: field_amnt_difference */
  $handler->display->display_options['fields']['field_field_amnt_difference']['id'] = 'field_field_amnt_difference';
  $handler->display->display_options['fields']['field_field_amnt_difference']['table'] = 'field_data_field_field_amnt_difference';
  $handler->display->display_options['fields']['field_field_amnt_difference']['field'] = 'field_field_amnt_difference';
  $handler->display->display_options['fields']['field_field_amnt_difference']['label'] = 'Difference Amt';
  $handler->display->display_options['fields']['field_field_amnt_difference']['settings'] = array(
    'thousand_separator' => '',
    'decimal_separator' => '.',
    'scale' => '2',
    'prefix_suffix' => 1,
  );
  /* Field: Content: Collection Date */
  $handler->display->display_options['fields']['field_collection_date']['id'] = 'field_collection_date';
  $handler->display->display_options['fields']['field_collection_date']['table'] = 'field_data_field_collection_date';
  $handler->display->display_options['fields']['field_collection_date']['field'] = 'field_collection_date';
  $handler->display->display_options['fields']['field_collection_date']['label'] = 'Latest Collection Date';
  $handler->display->display_options['fields']['field_collection_date']['settings'] = array(
    'format_type' => 'sales_day',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_remaining_days' => 0,
  );
  /* Field: Content: Returned Bill Flag */
  $handler->display->display_options['fields']['field_returned_bill_flag_1']['id'] = 'field_returned_bill_flag_1';
  $handler->display->display_options['fields']['field_returned_bill_flag_1']['table'] = 'field_data_field_returned_bill_flag';
  $handler->display->display_options['fields']['field_returned_bill_flag_1']['field'] = 'field_returned_bill_flag';
  $handler->display->display_options['fields']['field_returned_bill_flag_1']['type'] = 'list_key';
  /* Field: Content: terminated bill flag */
  $handler->display->display_options['fields']['field_terminated_bill_flag']['id'] = 'field_terminated_bill_flag';
  $handler->display->display_options['fields']['field_terminated_bill_flag']['table'] = 'field_data_field_terminated_bill_flag';
  $handler->display->display_options['fields']['field_terminated_bill_flag']['field'] = 'field_terminated_bill_flag';
  $handler->display->display_options['fields']['field_terminated_bill_flag']['label'] = 'Terminated';
  $handler->display->display_options['fields']['field_terminated_bill_flag']['type'] = 'list_key';
  $handler->display->display_options['fields']['field_terminated_bill_flag']['settings'] = array(
    'data_element_key' => '',
    'skip_safe' => 1,
    'skip_empty_values' => 1,
  );
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
    'payment_collection' => 'payment_collection',
  );
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Bill No.';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
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
    14 => 0,
    13 => 0,
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'bills-collection';

  /* Display: Data export */
  $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '0';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_data_export_xls';
  $handler->display->display_options['style_options']['provide_file'] = 1;
  $handler->display->display_options['style_options']['parent_sort'] = 0;
  $handler->display->display_options['path'] = 'bills-collection-export';
  $handler->display->display_options['displays'] = array(
    'page' => 'page',
    'default' => 0,
  );
  $export['bills_collection'] = $view;

  $view = new view();
  $view->name = 'collection_history';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Collection History';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Collection History';
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
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Bill No.';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Content: Assigned To */
  $handler->display->display_options['fields']['field_assign_to']['id'] = 'field_assign_to';
  $handler->display->display_options['fields']['field_assign_to']['table'] = 'field_data_field_assign_to';
  $handler->display->display_options['fields']['field_assign_to']['field'] = 'field_assign_to';
  /* Field: Content: Collection Cash Amt */
  $handler->display->display_options['fields']['field_collection_cash_amount']['id'] = 'field_collection_cash_amount';
  $handler->display->display_options['fields']['field_collection_cash_amount']['table'] = 'field_data_field_collection_cash_amount';
  $handler->display->display_options['fields']['field_collection_cash_amount']['field'] = 'field_collection_cash_amount';
  $handler->display->display_options['fields']['field_collection_cash_amount']['settings'] = array(
    'thousand_separator' => '',
    'decimal_separator' => '.',
    'scale' => '2',
    'prefix_suffix' => 1,
  );
  /* Field: Content: Collection Cheque Amt */
  $handler->display->display_options['fields']['field_collection_cheque_amount']['id'] = 'field_collection_cheque_amount';
  $handler->display->display_options['fields']['field_collection_cheque_amount']['table'] = 'field_data_field_collection_cheque_amount';
  $handler->display->display_options['fields']['field_collection_cheque_amount']['field'] = 'field_collection_cheque_amount';
  $handler->display->display_options['fields']['field_collection_cheque_amount']['settings'] = array(
    'thousand_separator' => '',
    'decimal_separator' => '.',
    'scale' => '2',
    'prefix_suffix' => 1,
  );
  /* Field: Content: Collection Cheque Amt */
  $handler->display->display_options['fields']['field_collection_cheque_amount_1']['id'] = 'field_collection_cheque_amount_1';
  $handler->display->display_options['fields']['field_collection_cheque_amount_1']['table'] = 'field_data_field_collection_cheque_amount';
  $handler->display->display_options['fields']['field_collection_cheque_amount_1']['field'] = 'field_collection_cheque_amount';
  $handler->display->display_options['fields']['field_collection_cheque_amount_1']['settings'] = array(
    'thousand_separator' => '',
    'decimal_separator' => '.',
    'scale' => '2',
    'prefix_suffix' => 1,
  );
  /* Field: Content: Collection Cheque Date */
  $handler->display->display_options['fields']['field_collection_chq_date']['id'] = 'field_collection_chq_date';
  $handler->display->display_options['fields']['field_collection_chq_date']['table'] = 'field_data_field_collection_chq_date';
  $handler->display->display_options['fields']['field_collection_chq_date']['field'] = 'field_collection_chq_date';
  $handler->display->display_options['fields']['field_collection_chq_date']['settings'] = array(
    'format_type' => 'sales_day',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_remaining_days' => 0,
  );
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
    'collection_history' => 'collection_history',
  );
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Bill No.';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
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
    14 => 0,
    13 => 0,
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'collection-history';

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
  $handler->display->display_options['path'] = 'collection-history-export';
  $handler->display->display_options['displays'] = array(
    'default' => 'default',
    'page' => 'page',
  );
  $export['collection_history'] = $view;

  $view = new view();
  $view->name = 'day_wise_collection';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Day Wise Collection';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Day Wise Collection';
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
  /* Relationship: Content: Payment Collection Reference (field_payment_collection_referen) */
  $handler->display->display_options['relationships']['field_payment_collection_referen_nid']['id'] = 'field_payment_collection_referen_nid';
  $handler->display->display_options['relationships']['field_payment_collection_referen_nid']['table'] = 'field_data_field_payment_collection_referen';
  $handler->display->display_options['relationships']['field_payment_collection_referen_nid']['field'] = 'field_payment_collection_referen_nid';
  $handler->display->display_options['relationships']['field_payment_collection_referen_nid']['delta'] = '-1';
  /* Relationship: Content: sales bill no ref (field_sales_bill_no_ref) */
  $handler->display->display_options['relationships']['field_sales_bill_no_ref_nid']['id'] = 'field_sales_bill_no_ref_nid';
  $handler->display->display_options['relationships']['field_sales_bill_no_ref_nid']['table'] = 'field_data_field_sales_bill_no_ref';
  $handler->display->display_options['relationships']['field_sales_bill_no_ref_nid']['field'] = 'field_sales_bill_no_ref_nid';
  $handler->display->display_options['relationships']['field_sales_bill_no_ref_nid']['relationship'] = 'field_payment_collection_referen_nid';
  $handler->display->display_options['relationships']['field_sales_bill_no_ref_nid']['delta'] = '-1';
  /* Relationship: Content: party_code_reference (field_party_code_reference) */
  $handler->display->display_options['relationships']['field_party_code_reference_nid']['id'] = 'field_party_code_reference_nid';
  $handler->display->display_options['relationships']['field_party_code_reference_nid']['table'] = 'field_data_field_party_code_reference';
  $handler->display->display_options['relationships']['field_party_code_reference_nid']['field'] = 'field_party_code_reference_nid';
  $handler->display->display_options['relationships']['field_party_code_reference_nid']['relationship'] = 'field_sales_bill_no_ref_nid';
  $handler->display->display_options['relationships']['field_party_code_reference_nid']['delta'] = '-1';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Bill No.';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Content: Bill Date */
  $handler->display->display_options['fields']['field_sr_bill_date']['id'] = 'field_sr_bill_date';
  $handler->display->display_options['fields']['field_sr_bill_date']['table'] = 'field_data_field_sr_bill_date';
  $handler->display->display_options['fields']['field_sr_bill_date']['field'] = 'field_sr_bill_date';
  $handler->display->display_options['fields']['field_sr_bill_date']['relationship'] = 'field_sales_bill_no_ref_nid';
  $handler->display->display_options['fields']['field_sr_bill_date']['settings'] = array(
    'format_type' => 'sales_day',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_remaining_days' => 0,
  );
  /* Field: Content: Flag */
  $handler->display->display_options['fields']['field_flag']['id'] = 'field_flag';
  $handler->display->display_options['fields']['field_flag']['table'] = 'field_data_field_flag';
  $handler->display->display_options['fields']['field_flag']['field'] = 'field_flag';
  $handler->display->display_options['fields']['field_flag']['relationship'] = 'field_payment_collection_referen_nid';
  /* Field: Content: party_code_reference */
  $handler->display->display_options['fields']['field_party_code_reference']['id'] = 'field_party_code_reference';
  $handler->display->display_options['fields']['field_party_code_reference']['table'] = 'field_data_field_party_code_reference';
  $handler->display->display_options['fields']['field_party_code_reference']['field'] = 'field_party_code_reference';
  $handler->display->display_options['fields']['field_party_code_reference']['relationship'] = 'field_sales_bill_no_ref_nid';
  $handler->display->display_options['fields']['field_party_code_reference']['label'] = 'Party Code';
  $handler->display->display_options['fields']['field_party_code_reference']['type'] = 'node_reference_plain';
  /* Field: Content: Beat Name */
  $handler->display->display_options['fields']['field_beat_name']['id'] = 'field_beat_name';
  $handler->display->display_options['fields']['field_beat_name']['table'] = 'field_data_field_beat_name';
  $handler->display->display_options['fields']['field_beat_name']['field'] = 'field_beat_name';
  $handler->display->display_options['fields']['field_beat_name']['relationship'] = 'field_party_code_reference_nid';
  /* Field: Content: Salesman */
  $handler->display->display_options['fields']['field_salesman']['id'] = 'field_salesman';
  $handler->display->display_options['fields']['field_salesman']['table'] = 'field_data_field_salesman';
  $handler->display->display_options['fields']['field_salesman']['field'] = 'field_salesman';
  $handler->display->display_options['fields']['field_salesman']['relationship'] = 'field_party_code_reference_nid';
  /* Field: Content: Salesman Code */
  $handler->display->display_options['fields']['field_salesman_code']['id'] = 'field_salesman_code';
  $handler->display->display_options['fields']['field_salesman_code']['table'] = 'field_data_field_salesman_code';
  $handler->display->display_options['fields']['field_salesman_code']['field'] = 'field_salesman_code';
  $handler->display->display_options['fields']['field_salesman_code']['relationship'] = 'field_party_code_reference_nid';
  /* Field: Content: Bill Value */
  $handler->display->display_options['fields']['field_bill_value']['id'] = 'field_bill_value';
  $handler->display->display_options['fields']['field_bill_value']['table'] = 'field_data_field_bill_value';
  $handler->display->display_options['fields']['field_bill_value']['field'] = 'field_bill_value';
  $handler->display->display_options['fields']['field_bill_value']['relationship'] = 'field_sales_bill_no_ref_nid';
  /* Field: Content: remaining amt collection */
  $handler->display->display_options['fields']['field_remaining_amt_collection']['id'] = 'field_remaining_amt_collection';
  $handler->display->display_options['fields']['field_remaining_amt_collection']['table'] = 'field_data_field_remaining_amt_collection';
  $handler->display->display_options['fields']['field_remaining_amt_collection']['field'] = 'field_remaining_amt_collection';
  $handler->display->display_options['fields']['field_remaining_amt_collection']['relationship'] = 'field_payment_collection_referen_nid';
  $handler->display->display_options['fields']['field_remaining_amt_collection']['label'] = 'To Be Collected';
  $handler->display->display_options['fields']['field_remaining_amt_collection']['settings'] = array(
    'thousand_separator' => '',
    'decimal_separator' => '.',
    'scale' => '2',
    'prefix_suffix' => 1,
  );
  /* Field: Content: Collection Cash Amt */
  $handler->display->display_options['fields']['field_collection_cash_amount']['id'] = 'field_collection_cash_amount';
  $handler->display->display_options['fields']['field_collection_cash_amount']['table'] = 'field_data_field_collection_cash_amount';
  $handler->display->display_options['fields']['field_collection_cash_amount']['field'] = 'field_collection_cash_amount';
  $handler->display->display_options['fields']['field_collection_cash_amount']['label'] = 'Cash Collection Amt';
  $handler->display->display_options['fields']['field_collection_cash_amount']['settings'] = array(
    'thousand_separator' => '',
    'decimal_separator' => '.',
    'scale' => '2',
    'prefix_suffix' => 1,
  );
  /* Field: Content: Collection Cheque Amt */
  $handler->display->display_options['fields']['field_collection_cheque_amount']['id'] = 'field_collection_cheque_amount';
  $handler->display->display_options['fields']['field_collection_cheque_amount']['table'] = 'field_data_field_collection_cheque_amount';
  $handler->display->display_options['fields']['field_collection_cheque_amount']['field'] = 'field_collection_cheque_amount';
  $handler->display->display_options['fields']['field_collection_cheque_amount']['label'] = 'Cheque Collection Amt';
  $handler->display->display_options['fields']['field_collection_cheque_amount']['settings'] = array(
    'thousand_separator' => '',
    'decimal_separator' => '.',
    'scale' => '2',
    'prefix_suffix' => 1,
  );
  /* Field: Content: Collection Cheque no */
  $handler->display->display_options['fields']['field_collection_cheque_num']['id'] = 'field_collection_cheque_num';
  $handler->display->display_options['fields']['field_collection_cheque_num']['table'] = 'field_data_field_collection_cheque_num';
  $handler->display->display_options['fields']['field_collection_cheque_num']['field'] = 'field_collection_cheque_num';
  $handler->display->display_options['fields']['field_collection_cheque_num']['label'] = 'Cheque No.';
  /* Field: Content: Collection Cheque Date */
  $handler->display->display_options['fields']['field_collection_chq_date']['id'] = 'field_collection_chq_date';
  $handler->display->display_options['fields']['field_collection_chq_date']['table'] = 'field_data_field_collection_chq_date';
  $handler->display->display_options['fields']['field_collection_chq_date']['field'] = 'field_collection_chq_date';
  $handler->display->display_options['fields']['field_collection_chq_date']['label'] = 'Cheque Date';
  $handler->display->display_options['fields']['field_collection_chq_date']['settings'] = array(
    'format_type' => 'sales_day',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_remaining_days' => 0,
  );
  /* Field: Content: Assigned To */
  $handler->display->display_options['fields']['field_assign_to']['id'] = 'field_assign_to';
  $handler->display->display_options['fields']['field_assign_to']['table'] = 'field_data_field_assign_to';
  $handler->display->display_options['fields']['field_assign_to']['field'] = 'field_assign_to';
  /* Field: Content: Collection history date */
  $handler->display->display_options['fields']['field_collection_history_date']['id'] = 'field_collection_history_date';
  $handler->display->display_options['fields']['field_collection_history_date']['table'] = 'field_data_field_collection_history_date';
  $handler->display->display_options['fields']['field_collection_history_date']['field'] = 'field_collection_history_date';
  $handler->display->display_options['fields']['field_collection_history_date']['label'] = 'Collection Date';
  $handler->display->display_options['fields']['field_collection_history_date']['settings'] = array(
    'format_type' => 'sales_day',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_remaining_days' => 0,
  );
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
    'collection_history' => 'collection_history',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['group'] = 1;
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Bill No.';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
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
    14 => 0,
    13 => 0,
  );
  /* Filter criterion: Content: Collection history date (field_collection_history_date) */
  $handler->display->display_options['filters']['field_collection_history_date_value']['id'] = 'field_collection_history_date_value';
  $handler->display->display_options['filters']['field_collection_history_date_value']['table'] = 'field_data_field_collection_history_date';
  $handler->display->display_options['filters']['field_collection_history_date_value']['field'] = 'field_collection_history_date_value';
  $handler->display->display_options['filters']['field_collection_history_date_value']['operator'] = 'between';
  $handler->display->display_options['filters']['field_collection_history_date_value']['group'] = 1;
  $handler->display->display_options['filters']['field_collection_history_date_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_collection_history_date_value']['expose']['operator_id'] = 'field_collection_history_date_value_op';
  $handler->display->display_options['filters']['field_collection_history_date_value']['expose']['label'] = 'Collection Date';
  $handler->display->display_options['filters']['field_collection_history_date_value']['expose']['operator'] = 'field_collection_history_date_value_op';
  $handler->display->display_options['filters']['field_collection_history_date_value']['expose']['identifier'] = 'field_collection_history_date_value';
  $handler->display->display_options['filters']['field_collection_history_date_value']['expose']['remember_roles'] = array(
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
    14 => 0,
    13 => 0,
  );
  $handler->display->display_options['filters']['field_collection_history_date_value']['form_type'] = 'date_popup';
  $handler->display->display_options['filters']['field_collection_history_date_value']['default_date'] = 'now -1 day';
  $handler->display->display_options['filters']['field_collection_history_date_value']['default_to_date'] = 'now';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'day-wise-collection';

  /* Display: Data export */
  $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '0';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_data_export_xls';
  $handler->display->display_options['style_options']['provide_file'] = 1;
  $handler->display->display_options['style_options']['parent_sort'] = 0;
  $handler->display->display_options['path'] = 'day-wise-collection-export';
  $handler->display->display_options['displays'] = array(
    'page' => 'page',
    'default' => 0,
  );
  $export['day_wise_collection'] = $view;

  $view = new view();
  $view->name = 'payment_collection';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Payment Collection';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Payment Collection';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'editableviews_table';
  $handler->display->display_options['style_options']['columns'] = array(
    'field_returned_bill_flag_editable' => 'field_returned_bill_flag_editable',
    'field_flag' => 'field_flag',
    'title' => 'title',
    'field_sr_bill_date' => 'field_sr_bill_date',
    'field_sales_party_code' => 'field_sales_party_code',
    'field_party_name' => 'field_party_name',
    'field_beat_name' => 'field_beat_name',
    'field_bill_value' => 'field_bill_value',
    'field_cash_amt_editable' => 'field_cash_amt_editable',
    'field_collection_cheque_amt_editable' => 'field_collection_cheque_amt_editable',
    'field_field_amnt_difference_editable' => 'field_field_amnt_difference_editable',
    'field_collection_cheque_number_editable' => 'field_collection_cheque_number_editable',
    'field_collection_cheque_date_editable' => 'field_collection_cheque_date_editable',
    'field_assigned_person' => 'field_assigned_person',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'field_returned_bill_flag_editable' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_flag' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_sr_bill_date' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_sales_party_code' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_party_name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_beat_name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_bill_value' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_cash_amt_editable' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_collection_cheque_amt_editable' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_field_amnt_difference_editable' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_collection_cheque_number_editable' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_collection_cheque_date_editable' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_assigned_person' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['style_options']['save_messages'] = 'summary';
  $handler->display->display_options['style_options']['batch_size'] = '10';
  /* Relationship: Content: sales bill no ref (field_sales_bill_no_ref) */
  $handler->display->display_options['relationships']['field_sales_bill_no_ref_nid']['id'] = 'field_sales_bill_no_ref_nid';
  $handler->display->display_options['relationships']['field_sales_bill_no_ref_nid']['table'] = 'field_data_field_sales_bill_no_ref';
  $handler->display->display_options['relationships']['field_sales_bill_no_ref_nid']['field'] = 'field_sales_bill_no_ref_nid';
  $handler->display->display_options['relationships']['field_sales_bill_no_ref_nid']['delta'] = '-1';
  /* Relationship: Content: party_code_reference (field_party_code_reference) */
  $handler->display->display_options['relationships']['field_party_code_reference_nid']['id'] = 'field_party_code_reference_nid';
  $handler->display->display_options['relationships']['field_party_code_reference_nid']['table'] = 'field_data_field_party_code_reference';
  $handler->display->display_options['relationships']['field_party_code_reference_nid']['field'] = 'field_party_code_reference_nid';
  $handler->display->display_options['relationships']['field_party_code_reference_nid']['relationship'] = 'field_sales_bill_no_ref_nid';
  $handler->display->display_options['relationships']['field_party_code_reference_nid']['delta'] = '-1';
  /* Field: Global: View result counter */
  $handler->display->display_options['fields']['counter']['id'] = 'counter';
  $handler->display->display_options['fields']['counter']['table'] = 'views';
  $handler->display->display_options['fields']['counter']['field'] = 'counter';
  $handler->display->display_options['fields']['counter']['label'] = 'Sr. No.';
  $handler->display->display_options['fields']['counter']['counter_start'] = '1';
  $handler->display->display_options['fields']['counter']['reverse'] = 0;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  /* Field: Content: Returned Bill Flag (editable) */
  $handler->display->display_options['fields']['field_returned_bill_flag_editable']['id'] = 'field_returned_bill_flag_editable';
  $handler->display->display_options['fields']['field_returned_bill_flag_editable']['table'] = 'field_data_field_returned_bill_flag';
  $handler->display->display_options['fields']['field_returned_bill_flag_editable']['field'] = 'field_returned_bill_flag_editable';
  $handler->display->display_options['fields']['field_returned_bill_flag_editable']['label'] = 'Bill Returned';
  $handler->display->display_options['fields']['field_returned_bill_flag_editable']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_returned_bill_flag_editable']['type'] = 'services';
  $handler->display->display_options['fields']['field_returned_bill_flag_editable']['settings'] = array(
    'data_element_key' => '',
    'skip_safe' => 1,
    'skip_empty_values' => 1,
  );
  $handler->display->display_options['fields']['field_returned_bill_flag_editable']['widget_type'] = 'options_onoff';
  $handler->display->display_options['fields']['field_returned_bill_flag_editable']['suppress_label'] = 1;
  $handler->display->display_options['fields']['field_returned_bill_flag_editable']['suppress_description'] = 1;
  /* Field: Content: Flag */
  $handler->display->display_options['fields']['field_flag']['id'] = 'field_flag';
  $handler->display->display_options['fields']['field_flag']['table'] = 'field_data_field_flag';
  $handler->display->display_options['fields']['field_flag']['field'] = 'field_flag';
  $handler->display->display_options['fields']['field_flag']['label'] = 'Type';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Bill No.';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Content: Bill Date */
  $handler->display->display_options['fields']['field_sr_bill_date']['id'] = 'field_sr_bill_date';
  $handler->display->display_options['fields']['field_sr_bill_date']['table'] = 'field_data_field_sr_bill_date';
  $handler->display->display_options['fields']['field_sr_bill_date']['field'] = 'field_sr_bill_date';
  $handler->display->display_options['fields']['field_sr_bill_date']['relationship'] = 'field_sales_bill_no_ref_nid';
  $handler->display->display_options['fields']['field_sr_bill_date']['settings'] = array(
    'format_type' => 'sales_day',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_remaining_days' => 0,
  );
  /* Field: Content: Sales party code */
  $handler->display->display_options['fields']['field_sales_party_code']['id'] = 'field_sales_party_code';
  $handler->display->display_options['fields']['field_sales_party_code']['table'] = 'field_data_field_sales_party_code';
  $handler->display->display_options['fields']['field_sales_party_code']['field'] = 'field_sales_party_code';
  $handler->display->display_options['fields']['field_sales_party_code']['relationship'] = 'field_sales_bill_no_ref_nid';
  $handler->display->display_options['fields']['field_sales_party_code']['label'] = 'Sales Party Code';
  /* Field: Content: remaining amt collection */
  $handler->display->display_options['fields']['field_remaining_amt_collection']['id'] = 'field_remaining_amt_collection';
  $handler->display->display_options['fields']['field_remaining_amt_collection']['table'] = 'field_data_field_remaining_amt_collection';
  $handler->display->display_options['fields']['field_remaining_amt_collection']['field'] = 'field_remaining_amt_collection';
  $handler->display->display_options['fields']['field_remaining_amt_collection']['label'] = 'To Be Collected';
  $handler->display->display_options['fields']['field_remaining_amt_collection']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_remaining_amt_collection']['alter']['text'] = '<div class="bill_value-[nid]">[field_remaining_amt_collection]</div>';
  $handler->display->display_options['fields']['field_remaining_amt_collection']['settings'] = array(
    'thousand_separator' => '',
    'decimal_separator' => '.',
    'scale' => '2',
    'prefix_suffix' => 1,
  );
  /* Field: Content: Previous Cash Amt */
  $handler->display->display_options['fields']['field_previous_cash_amt']['id'] = 'field_previous_cash_amt';
  $handler->display->display_options['fields']['field_previous_cash_amt']['table'] = 'field_data_field_previous_cash_amt';
  $handler->display->display_options['fields']['field_previous_cash_amt']['field'] = 'field_previous_cash_amt';
  $handler->display->display_options['fields']['field_previous_cash_amt']['label'] = 'Prev Cash Amt';
  $handler->display->display_options['fields']['field_previous_cash_amt']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_previous_cash_amt']['alter']['text'] = '<div class="prev_cash_amt-[nid]">[field_previous_cash_amt]</div>';
  $handler->display->display_options['fields']['field_previous_cash_amt']['settings'] = array(
    'thousand_separator' => '',
    'decimal_separator' => '.',
    'scale' => '2',
    'prefix_suffix' => 1,
  );
  /* Field: Content: Previous Cheque Amt */
  $handler->display->display_options['fields']['field_previous_cheque_amt']['id'] = 'field_previous_cheque_amt';
  $handler->display->display_options['fields']['field_previous_cheque_amt']['table'] = 'field_data_field_previous_cheque_amt';
  $handler->display->display_options['fields']['field_previous_cheque_amt']['field'] = 'field_previous_cheque_amt';
  $handler->display->display_options['fields']['field_previous_cheque_amt']['label'] = 'Prev Cheque Amt';
  $handler->display->display_options['fields']['field_previous_cheque_amt']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_previous_cheque_amt']['alter']['text'] = '<div class="prev_ch_amt-[nid]">[field_previous_cheque_amt]</div>';
  $handler->display->display_options['fields']['field_previous_cheque_amt']['settings'] = array(
    'thousand_separator' => '',
    'decimal_separator' => '.',
    'scale' => '2',
    'prefix_suffix' => 1,
  );
  /* Field: Content: Collection Cash Amt (editable) */
  $handler->display->display_options['fields']['field_cash_amt_editable']['id'] = 'field_cash_amt_editable';
  $handler->display->display_options['fields']['field_cash_amt_editable']['table'] = 'field_data_field_cash_amt';
  $handler->display->display_options['fields']['field_cash_amt_editable']['field'] = 'field_cash_amt_editable';
  $handler->display->display_options['fields']['field_cash_amt_editable']['label'] = 'Cash Amt';
  $handler->display->display_options['fields']['field_cash_amt_editable']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_cash_amt_editable']['alter']['text'] = '<div class="field_cash_amt_editable-[nid]">[field_cash_amt_editable]</div>';
  $handler->display->display_options['fields']['field_cash_amt_editable']['type'] = 'services';
  $handler->display->display_options['fields']['field_cash_amt_editable']['settings'] = array(
    'data_element_key' => '',
    'skip_safe' => 1,
    'skip_empty_values' => 1,
  );
  $handler->display->display_options['fields']['field_cash_amt_editable']['widget_type'] = '0';
  $handler->display->display_options['fields']['field_cash_amt_editable']['suppress_label'] = 1;
  $handler->display->display_options['fields']['field_cash_amt_editable']['suppress_description'] = 0;
  /* Field: Content: Collection Cheque Amt (editable) */
  $handler->display->display_options['fields']['field_collection_cheque_amt_editable']['id'] = 'field_collection_cheque_amt_editable';
  $handler->display->display_options['fields']['field_collection_cheque_amt_editable']['table'] = 'field_data_field_collection_cheque_amt';
  $handler->display->display_options['fields']['field_collection_cheque_amt_editable']['field'] = 'field_collection_cheque_amt_editable';
  $handler->display->display_options['fields']['field_collection_cheque_amt_editable']['label'] = 'Cheque Amt';
  $handler->display->display_options['fields']['field_collection_cheque_amt_editable']['type'] = 'services';
  $handler->display->display_options['fields']['field_collection_cheque_amt_editable']['settings'] = array(
    'data_element_key' => '',
    'skip_safe' => 1,
    'skip_empty_values' => 1,
  );
  $handler->display->display_options['fields']['field_collection_cheque_amt_editable']['widget_type'] = '0';
  $handler->display->display_options['fields']['field_collection_cheque_amt_editable']['suppress_label'] = 1;
  $handler->display->display_options['fields']['field_collection_cheque_amt_editable']['suppress_description'] = 0;
  /* Field: Content: field_amnt_difference (editable) */
  $handler->display->display_options['fields']['field_field_amnt_difference_editable']['id'] = 'field_field_amnt_difference_editable';
  $handler->display->display_options['fields']['field_field_amnt_difference_editable']['table'] = 'field_data_field_field_amnt_difference';
  $handler->display->display_options['fields']['field_field_amnt_difference_editable']['field'] = 'field_field_amnt_difference_editable';
  $handler->display->display_options['fields']['field_field_amnt_difference_editable']['label'] = 'Difference';
  $handler->display->display_options['fields']['field_field_amnt_difference_editable']['type'] = 'services';
  $handler->display->display_options['fields']['field_field_amnt_difference_editable']['settings'] = array(
    'data_element_key' => '',
    'skip_safe' => 1,
    'skip_empty_values' => 1,
  );
  $handler->display->display_options['fields']['field_field_amnt_difference_editable']['widget_type'] = '0';
  $handler->display->display_options['fields']['field_field_amnt_difference_editable']['suppress_label'] = 1;
  $handler->display->display_options['fields']['field_field_amnt_difference_editable']['suppress_description'] = 0;
  /* Field: Content: Collection Cheque Number (editable) */
  $handler->display->display_options['fields']['field_collection_cheque_number_editable']['id'] = 'field_collection_cheque_number_editable';
  $handler->display->display_options['fields']['field_collection_cheque_number_editable']['table'] = 'field_data_field_collection_cheque_number';
  $handler->display->display_options['fields']['field_collection_cheque_number_editable']['field'] = 'field_collection_cheque_number_editable';
  $handler->display->display_options['fields']['field_collection_cheque_number_editable']['label'] = 'Cheque No.';
  $handler->display->display_options['fields']['field_collection_cheque_number_editable']['type'] = 'services';
  $handler->display->display_options['fields']['field_collection_cheque_number_editable']['settings'] = array(
    'data_element_key' => '',
    'skip_safe' => 1,
    'skip_empty_values' => 1,
    'skip_text_format' => 1,
  );
  $handler->display->display_options['fields']['field_collection_cheque_number_editable']['widget_type'] = '0';
  $handler->display->display_options['fields']['field_collection_cheque_number_editable']['suppress_label'] = 1;
  $handler->display->display_options['fields']['field_collection_cheque_number_editable']['suppress_description'] = 0;
  /* Field: Content: Collection Cheque date (editable) */
  $handler->display->display_options['fields']['field_collection_cheque_date_editable']['id'] = 'field_collection_cheque_date_editable';
  $handler->display->display_options['fields']['field_collection_cheque_date_editable']['table'] = 'field_data_field_collection_cheque_date';
  $handler->display->display_options['fields']['field_collection_cheque_date_editable']['field'] = 'field_collection_cheque_date_editable';
  $handler->display->display_options['fields']['field_collection_cheque_date_editable']['label'] = 'Cheque Date';
  $handler->display->display_options['fields']['field_collection_cheque_date_editable']['type'] = 'services';
  $handler->display->display_options['fields']['field_collection_cheque_date_editable']['settings'] = array(
    'data_element_key' => '',
    'skip_safe' => 1,
    'skip_empty_values' => 1,
  );
  $handler->display->display_options['fields']['field_collection_cheque_date_editable']['widget_type'] = '0';
  $handler->display->display_options['fields']['field_collection_cheque_date_editable']['suppress_label'] = 1;
  $handler->display->display_options['fields']['field_collection_cheque_date_editable']['suppress_description'] = 0;
  /* Field: Content: Collection Status */
  $handler->display->display_options['fields']['field_collection_status']['id'] = 'field_collection_status';
  $handler->display->display_options['fields']['field_collection_status']['table'] = 'field_data_field_collection_status';
  $handler->display->display_options['fields']['field_collection_status']['field'] = 'field_collection_status';
  /* Field: Content: Assigned Person */
  $handler->display->display_options['fields']['field_assigned_person']['id'] = 'field_assigned_person';
  $handler->display->display_options['fields']['field_assigned_person']['table'] = 'field_data_field_assigned_person';
  $handler->display->display_options['fields']['field_assigned_person']['field'] = 'field_assigned_person';
  $handler->display->display_options['fields']['field_assigned_person']['label'] = 'Assigned To';
  /* Field: Content: Bill Value */
  $handler->display->display_options['fields']['field_bill_value']['id'] = 'field_bill_value';
  $handler->display->display_options['fields']['field_bill_value']['table'] = 'field_data_field_bill_value';
  $handler->display->display_options['fields']['field_bill_value']['field'] = 'field_bill_value';
  $handler->display->display_options['fields']['field_bill_value']['relationship'] = 'field_sales_bill_no_ref_nid';
  $handler->display->display_options['fields']['field_bill_value']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_bill_value']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_bill_value']['alter']['text'] = '<div class="bill_value-[nid]">[field_bill_value]</div>';
  /* Field: Content: PARTY_NAME */
  $handler->display->display_options['fields']['field_party_name']['id'] = 'field_party_name';
  $handler->display->display_options['fields']['field_party_name']['table'] = 'field_data_field_party_name';
  $handler->display->display_options['fields']['field_party_name']['field'] = 'field_party_name';
  $handler->display->display_options['fields']['field_party_name']['relationship'] = 'field_party_code_reference_nid';
  $handler->display->display_options['fields']['field_party_name']['label'] = 'Party Name';
  /* Field: Content: Beat Name */
  $handler->display->display_options['fields']['field_beat_name']['id'] = 'field_beat_name';
  $handler->display->display_options['fields']['field_beat_name']['table'] = 'field_data_field_beat_name';
  $handler->display->display_options['fields']['field_beat_name']['field'] = 'field_beat_name';
  $handler->display->display_options['fields']['field_beat_name']['relationship'] = 'field_party_code_reference_nid';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['group'] = 1;
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Bill No';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
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
    14 => 0,
    13 => 0,
  );
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
    'payment_collection' => 'payment_collection',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Content: Bill Date (field_sr_bill_date) */
  $handler->display->display_options['filters']['field_sr_bill_date_value']['id'] = 'field_sr_bill_date_value';
  $handler->display->display_options['filters']['field_sr_bill_date_value']['table'] = 'field_data_field_sr_bill_date';
  $handler->display->display_options['filters']['field_sr_bill_date_value']['field'] = 'field_sr_bill_date_value';
  $handler->display->display_options['filters']['field_sr_bill_date_value']['relationship'] = 'field_sales_bill_no_ref_nid';
  $handler->display->display_options['filters']['field_sr_bill_date_value']['operator'] = 'between';
  $handler->display->display_options['filters']['field_sr_bill_date_value']['group'] = 1;
  $handler->display->display_options['filters']['field_sr_bill_date_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_sr_bill_date_value']['expose']['operator_id'] = 'field_sr_bill_date_value_op';
  $handler->display->display_options['filters']['field_sr_bill_date_value']['expose']['operator'] = 'field_sr_bill_date_value_op';
  $handler->display->display_options['filters']['field_sr_bill_date_value']['expose']['identifier'] = 'field_sr_bill_date_value';
  $handler->display->display_options['filters']['field_sr_bill_date_value']['expose']['remember_roles'] = array(
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
  );
  $handler->display->display_options['filters']['field_sr_bill_date_value']['form_type'] = 'date_popup';
  /* Filter criterion: Content: Beat Name (field_beat_name) */
  $handler->display->display_options['filters']['field_beat_name_value']['id'] = 'field_beat_name_value';
  $handler->display->display_options['filters']['field_beat_name_value']['table'] = 'field_data_field_beat_name';
  $handler->display->display_options['filters']['field_beat_name_value']['field'] = 'field_beat_name_value';
  $handler->display->display_options['filters']['field_beat_name_value']['relationship'] = 'field_party_code_reference_nid';
  $handler->display->display_options['filters']['field_beat_name_value']['group'] = 1;
  $handler->display->display_options['filters']['field_beat_name_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_beat_name_value']['expose']['operator_id'] = 'field_beat_name_value_op';
  $handler->display->display_options['filters']['field_beat_name_value']['expose']['label'] = 'Beat Name';
  $handler->display->display_options['filters']['field_beat_name_value']['expose']['operator'] = 'field_beat_name_value_op';
  $handler->display->display_options['filters']['field_beat_name_value']['expose']['identifier'] = 'field_beat_name_value';
  $handler->display->display_options['filters']['field_beat_name_value']['expose']['remember_roles'] = array(
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
  );
  /* Filter criterion: Content: Assigned Person (field_assigned_person) */
  $handler->display->display_options['filters']['field_assigned_person_value']['id'] = 'field_assigned_person_value';
  $handler->display->display_options['filters']['field_assigned_person_value']['table'] = 'field_data_field_assigned_person';
  $handler->display->display_options['filters']['field_assigned_person_value']['field'] = 'field_assigned_person_value';
  $handler->display->display_options['filters']['field_assigned_person_value']['group'] = 1;
  $handler->display->display_options['filters']['field_assigned_person_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_assigned_person_value']['expose']['operator_id'] = 'field_assigned_person_value_op';
  $handler->display->display_options['filters']['field_assigned_person_value']['expose']['label'] = 'Assigned To';
  $handler->display->display_options['filters']['field_assigned_person_value']['expose']['operator'] = 'field_assigned_person_value_op';
  $handler->display->display_options['filters']['field_assigned_person_value']['expose']['identifier'] = 'field_assigned_person_value';
  $handler->display->display_options['filters']['field_assigned_person_value']['expose']['remember_roles'] = array(
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
  );
  /* Filter criterion: Content: Assignment date (field_assignment_date) */
  $handler->display->display_options['filters']['field_assignment_date_value']['id'] = 'field_assignment_date_value';
  $handler->display->display_options['filters']['field_assignment_date_value']['table'] = 'field_data_field_assignment_date';
  $handler->display->display_options['filters']['field_assignment_date_value']['field'] = 'field_assignment_date_value';
  $handler->display->display_options['filters']['field_assignment_date_value']['operator'] = 'between';
  $handler->display->display_options['filters']['field_assignment_date_value']['group'] = 1;
  $handler->display->display_options['filters']['field_assignment_date_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_assignment_date_value']['expose']['operator_id'] = 'field_assignment_date_value_op';
  $handler->display->display_options['filters']['field_assignment_date_value']['expose']['operator'] = 'field_assignment_date_value_op';
  $handler->display->display_options['filters']['field_assignment_date_value']['expose']['identifier'] = 'field_assignment_date_value';
  $handler->display->display_options['filters']['field_assignment_date_value']['expose']['remember_roles'] = array(
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
  );
  $handler->display->display_options['filters']['field_assignment_date_value']['form_type'] = 'date_popup';
  /* Filter criterion: Content: Assigned Person (field_assigned_person) */
  $handler->display->display_options['filters']['field_assigned_person_value_1']['id'] = 'field_assigned_person_value_1';
  $handler->display->display_options['filters']['field_assigned_person_value_1']['table'] = 'field_data_field_assigned_person';
  $handler->display->display_options['filters']['field_assigned_person_value_1']['field'] = 'field_assigned_person_value';
  $handler->display->display_options['filters']['field_assigned_person_value_1']['operator'] = 'not empty';
  $handler->display->display_options['filters']['field_assigned_person_value_1']['group'] = 1;
  /* Filter criterion: Content: Collection Status (field_collection_status) */
  $handler->display->display_options['filters']['field_collection_status_value']['id'] = 'field_collection_status_value';
  $handler->display->display_options['filters']['field_collection_status_value']['table'] = 'field_data_field_collection_status';
  $handler->display->display_options['filters']['field_collection_status_value']['field'] = 'field_collection_status_value';
  $handler->display->display_options['filters']['field_collection_status_value']['group'] = 1;
  $handler->display->display_options['filters']['field_collection_status_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_collection_status_value']['expose']['operator_id'] = 'field_collection_status_value_op';
  $handler->display->display_options['filters']['field_collection_status_value']['expose']['label'] = 'Collection Status';
  $handler->display->display_options['filters']['field_collection_status_value']['expose']['operator'] = 'field_collection_status_value_op';
  $handler->display->display_options['filters']['field_collection_status_value']['expose']['identifier'] = 'field_collection_status_value';
  $handler->display->display_options['filters']['field_collection_status_value']['expose']['remember_roles'] = array(
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
    14 => 0,
    13 => 0,
  );
  /* Filter criterion: Content: terminated bill flag (field_terminated_bill_flag) */
  $handler->display->display_options['filters']['field_terminated_bill_flag_value']['id'] = 'field_terminated_bill_flag_value';
  $handler->display->display_options['filters']['field_terminated_bill_flag_value']['table'] = 'field_data_field_terminated_bill_flag';
  $handler->display->display_options['filters']['field_terminated_bill_flag_value']['field'] = 'field_terminated_bill_flag_value';
  $handler->display->display_options['filters']['field_terminated_bill_flag_value']['operator'] = 'not';
  $handler->display->display_options['filters']['field_terminated_bill_flag_value']['value'] = array(
    1 => '1',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'payment-collection';
  $export['payment_collection'] = $view;

  $view = new view();
  $view->name = 'signed_bills_upload';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Signed Bills Upload';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Signed Bills Assignment';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
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
  /* Relationship: Content: sales bill no ref (field_sales_bill_no_ref) */
  $handler->display->display_options['relationships']['field_sales_bill_no_ref_nid']['id'] = 'field_sales_bill_no_ref_nid';
  $handler->display->display_options['relationships']['field_sales_bill_no_ref_nid']['table'] = 'field_data_field_sales_bill_no_ref';
  $handler->display->display_options['relationships']['field_sales_bill_no_ref_nid']['field'] = 'field_sales_bill_no_ref_nid';
  $handler->display->display_options['relationships']['field_sales_bill_no_ref_nid']['delta'] = '-1';
  /* Relationship: Content: party_code_reference (field_party_code_reference) */
  $handler->display->display_options['relationships']['field_party_code_reference_nid']['id'] = 'field_party_code_reference_nid';
  $handler->display->display_options['relationships']['field_party_code_reference_nid']['table'] = 'field_data_field_party_code_reference';
  $handler->display->display_options['relationships']['field_party_code_reference_nid']['field'] = 'field_party_code_reference_nid';
  $handler->display->display_options['relationships']['field_party_code_reference_nid']['relationship'] = 'field_sales_bill_no_ref_nid';
  $handler->display->display_options['relationships']['field_party_code_reference_nid']['delta'] = '-1';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Bill Date */
  $handler->display->display_options['fields']['field_sr_bill_date']['id'] = 'field_sr_bill_date';
  $handler->display->display_options['fields']['field_sr_bill_date']['table'] = 'field_data_field_sr_bill_date';
  $handler->display->display_options['fields']['field_sr_bill_date']['field'] = 'field_sr_bill_date';
  $handler->display->display_options['fields']['field_sr_bill_date']['relationship'] = 'field_sales_bill_no_ref_nid';
  $handler->display->display_options['fields']['field_sr_bill_date']['settings'] = array(
    'format_type' => 'sales_day',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_remaining_days' => 0,
  );
  /* Field: Content: party_code_reference */
  $handler->display->display_options['fields']['field_party_code_reference']['id'] = 'field_party_code_reference';
  $handler->display->display_options['fields']['field_party_code_reference']['table'] = 'field_data_field_party_code_reference';
  $handler->display->display_options['fields']['field_party_code_reference']['field'] = 'field_party_code_reference';
  $handler->display->display_options['fields']['field_party_code_reference']['relationship'] = 'field_sales_bill_no_ref_nid';
  $handler->display->display_options['fields']['field_party_code_reference']['type'] = 'node_reference_plain';
  /* Field: Content: Beat Name */
  $handler->display->display_options['fields']['field_beat_name']['id'] = 'field_beat_name';
  $handler->display->display_options['fields']['field_beat_name']['table'] = 'field_data_field_beat_name';
  $handler->display->display_options['fields']['field_beat_name']['field'] = 'field_beat_name';
  $handler->display->display_options['fields']['field_beat_name']['relationship'] = 'field_party_code_reference_nid';
  /* Field: Content: Salesman */
  $handler->display->display_options['fields']['field_salesman']['id'] = 'field_salesman';
  $handler->display->display_options['fields']['field_salesman']['table'] = 'field_data_field_salesman';
  $handler->display->display_options['fields']['field_salesman']['field'] = 'field_salesman';
  $handler->display->display_options['fields']['field_salesman']['relationship'] = 'field_party_code_reference_nid';
  /* Field: Content: Salesman Code */
  $handler->display->display_options['fields']['field_salesman_code']['id'] = 'field_salesman_code';
  $handler->display->display_options['fields']['field_salesman_code']['table'] = 'field_data_field_salesman_code';
  $handler->display->display_options['fields']['field_salesman_code']['field'] = 'field_salesman_code';
  $handler->display->display_options['fields']['field_salesman_code']['relationship'] = 'field_party_code_reference_nid';
  /* Field: Content: Bill Value */
  $handler->display->display_options['fields']['field_bill_value']['id'] = 'field_bill_value';
  $handler->display->display_options['fields']['field_bill_value']['table'] = 'field_data_field_bill_value';
  $handler->display->display_options['fields']['field_bill_value']['field'] = 'field_bill_value';
  $handler->display->display_options['fields']['field_bill_value']['relationship'] = 'field_sales_bill_no_ref_nid';
  /* Field: Content: File (editable) */
  $handler->display->display_options['fields']['field_file_editable']['id'] = 'field_file_editable';
  $handler->display->display_options['fields']['field_file_editable']['table'] = 'field_data_field_file';
  $handler->display->display_options['fields']['field_file_editable']['field'] = 'field_file_editable';
  $handler->display->display_options['fields']['field_file_editable']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_file_editable']['type'] = 'services';
  $handler->display->display_options['fields']['field_file_editable']['settings'] = array(
    'data_element_key' => '',
    'skip_safe' => 0,
    'skip_empty_values' => 0,
  );
  $handler->display->display_options['fields']['field_file_editable']['widget_type'] = 'file_generic';
  $handler->display->display_options['fields']['field_file_editable']['suppress_label'] = 0;
  $handler->display->display_options['fields']['field_file_editable']['suppress_description'] = 0;
  /* Sort criterion: Content: Assignment date (field_assignment_date) */
  $handler->display->display_options['sorts']['field_assignment_date_value']['id'] = 'field_assignment_date_value';
  $handler->display->display_options['sorts']['field_assignment_date_value']['table'] = 'field_data_field_assignment_date';
  $handler->display->display_options['sorts']['field_assignment_date_value']['field'] = 'field_assignment_date_value';
  $handler->display->display_options['sorts']['field_assignment_date_value']['order'] = 'DESC';
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
    'payment_collection' => 'payment_collection',
  );
  /* Filter criterion: Content: Flag (field_flag) */
  $handler->display->display_options['filters']['field_flag_value']['id'] = 'field_flag_value';
  $handler->display->display_options['filters']['field_flag_value']['table'] = 'field_data_field_flag';
  $handler->display->display_options['filters']['field_flag_value']['field'] = 'field_flag_value';
  $handler->display->display_options['filters']['field_flag_value']['value'] = array(
    'signed' => 'signed',
  );
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Bill No';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
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
  /* Filter criterion: Content: Bill Date (field_sr_bill_date) */
  $handler->display->display_options['filters']['field_sr_bill_date_value']['id'] = 'field_sr_bill_date_value';
  $handler->display->display_options['filters']['field_sr_bill_date_value']['table'] = 'field_data_field_sr_bill_date';
  $handler->display->display_options['filters']['field_sr_bill_date_value']['field'] = 'field_sr_bill_date_value';
  $handler->display->display_options['filters']['field_sr_bill_date_value']['relationship'] = 'field_sales_bill_no_ref_nid';
  $handler->display->display_options['filters']['field_sr_bill_date_value']['operator'] = 'between';
  $handler->display->display_options['filters']['field_sr_bill_date_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_sr_bill_date_value']['expose']['operator_id'] = 'field_sr_bill_date_value_op';
  $handler->display->display_options['filters']['field_sr_bill_date_value']['expose']['label'] = 'Bill Date';
  $handler->display->display_options['filters']['field_sr_bill_date_value']['expose']['operator'] = 'field_sr_bill_date_value_op';
  $handler->display->display_options['filters']['field_sr_bill_date_value']['expose']['identifier'] = 'field_sr_bill_date_value';
  $handler->display->display_options['filters']['field_sr_bill_date_value']['expose']['remember_roles'] = array(
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
    14 => 0,
    13 => 0,
  );
  $handler->display->display_options['filters']['field_sr_bill_date_value']['form_type'] = 'date_popup';
  /* Filter criterion: Content: Declaration date (field_declaration_date) */
  $handler->display->display_options['filters']['field_declaration_date_value']['id'] = 'field_declaration_date_value';
  $handler->display->display_options['filters']['field_declaration_date_value']['table'] = 'field_data_field_declaration_date';
  $handler->display->display_options['filters']['field_declaration_date_value']['field'] = 'field_declaration_date_value';
  $handler->display->display_options['filters']['field_declaration_date_value']['operator'] = 'between';
  $handler->display->display_options['filters']['field_declaration_date_value']['form_type'] = 'date_popup';

  /* Display: Signed Bills Upload */
  $handler = $view->new_display('page', 'Signed Bills Upload', 'page');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Signed Bills Upload';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Bill Number';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Content: Bill Date */
  $handler->display->display_options['fields']['field_sr_bill_date']['id'] = 'field_sr_bill_date';
  $handler->display->display_options['fields']['field_sr_bill_date']['table'] = 'field_data_field_sr_bill_date';
  $handler->display->display_options['fields']['field_sr_bill_date']['field'] = 'field_sr_bill_date';
  $handler->display->display_options['fields']['field_sr_bill_date']['relationship'] = 'field_sales_bill_no_ref_nid';
  $handler->display->display_options['fields']['field_sr_bill_date']['settings'] = array(
    'format_type' => 'sales_day',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_remaining_days' => 0,
  );
  /* Field: Content: Declaration date */
  $handler->display->display_options['fields']['field_declaration_date']['id'] = 'field_declaration_date';
  $handler->display->display_options['fields']['field_declaration_date']['table'] = 'field_data_field_declaration_date';
  $handler->display->display_options['fields']['field_declaration_date']['field'] = 'field_declaration_date';
  $handler->display->display_options['fields']['field_declaration_date']['settings'] = array(
    'format_type' => 'sales_day',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_remaining_days' => 0,
  );
  /* Field: Content: party_code_reference */
  $handler->display->display_options['fields']['field_party_code_reference']['id'] = 'field_party_code_reference';
  $handler->display->display_options['fields']['field_party_code_reference']['table'] = 'field_data_field_party_code_reference';
  $handler->display->display_options['fields']['field_party_code_reference']['field'] = 'field_party_code_reference';
  $handler->display->display_options['fields']['field_party_code_reference']['relationship'] = 'field_sales_bill_no_ref_nid';
  $handler->display->display_options['fields']['field_party_code_reference']['label'] = 'Party Code';
  $handler->display->display_options['fields']['field_party_code_reference']['type'] = 'node_reference_plain';
  /* Field: Content: Beat Name */
  $handler->display->display_options['fields']['field_beat_name']['id'] = 'field_beat_name';
  $handler->display->display_options['fields']['field_beat_name']['table'] = 'field_data_field_beat_name';
  $handler->display->display_options['fields']['field_beat_name']['field'] = 'field_beat_name';
  $handler->display->display_options['fields']['field_beat_name']['relationship'] = 'field_party_code_reference_nid';
  /* Field: Content: Salesman */
  $handler->display->display_options['fields']['field_salesman']['id'] = 'field_salesman';
  $handler->display->display_options['fields']['field_salesman']['table'] = 'field_data_field_salesman';
  $handler->display->display_options['fields']['field_salesman']['field'] = 'field_salesman';
  $handler->display->display_options['fields']['field_salesman']['relationship'] = 'field_party_code_reference_nid';
  /* Field: Content: Salesman Code */
  $handler->display->display_options['fields']['field_salesman_code']['id'] = 'field_salesman_code';
  $handler->display->display_options['fields']['field_salesman_code']['table'] = 'field_data_field_salesman_code';
  $handler->display->display_options['fields']['field_salesman_code']['field'] = 'field_salesman_code';
  $handler->display->display_options['fields']['field_salesman_code']['relationship'] = 'field_party_code_reference_nid';
  /* Field: Content: Bill Value */
  $handler->display->display_options['fields']['field_bill_value']['id'] = 'field_bill_value';
  $handler->display->display_options['fields']['field_bill_value']['table'] = 'field_data_field_bill_value';
  $handler->display->display_options['fields']['field_bill_value']['field'] = 'field_bill_value';
  $handler->display->display_options['fields']['field_bill_value']['relationship'] = 'field_sales_bill_no_ref_nid';
  /* Field: Content: remaining amt collection */
  $handler->display->display_options['fields']['field_remaining_amt_collection']['id'] = 'field_remaining_amt_collection';
  $handler->display->display_options['fields']['field_remaining_amt_collection']['table'] = 'field_data_field_remaining_amt_collection';
  $handler->display->display_options['fields']['field_remaining_amt_collection']['field'] = 'field_remaining_amt_collection';
  $handler->display->display_options['fields']['field_remaining_amt_collection']['label'] = 'To Be Collected Value';
  $handler->display->display_options['fields']['field_remaining_amt_collection']['settings'] = array(
    'thousand_separator' => '',
    'decimal_separator' => '.',
    'scale' => '2',
    'prefix_suffix' => 1,
  );
  /* Field: Content: File (editable) */
  $handler->display->display_options['fields']['field_file_editable']['id'] = 'field_file_editable';
  $handler->display->display_options['fields']['field_file_editable']['table'] = 'field_data_field_file';
  $handler->display->display_options['fields']['field_file_editable']['field'] = 'field_file_editable';
  $handler->display->display_options['fields']['field_file_editable']['label'] = 'File Upload';
  $handler->display->display_options['fields']['field_file_editable']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_file_editable']['type'] = 'services';
  $handler->display->display_options['fields']['field_file_editable']['settings'] = array(
    'data_element_key' => '',
    'skip_safe' => 0,
    'skip_empty_values' => 0,
  );
  $handler->display->display_options['fields']['field_file_editable']['widget_type'] = 'file_generic';
  $handler->display->display_options['fields']['field_file_editable']['suppress_label'] = 1;
  $handler->display->display_options['fields']['field_file_editable']['suppress_description'] = 1;
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
    'payment_collection' => 'payment_collection',
  );
  /* Filter criterion: Content: Flag (field_flag) */
  $handler->display->display_options['filters']['field_flag_value']['id'] = 'field_flag_value';
  $handler->display->display_options['filters']['field_flag_value']['table'] = 'field_data_field_flag';
  $handler->display->display_options['filters']['field_flag_value']['field'] = 'field_flag_value';
  $handler->display->display_options['filters']['field_flag_value']['value'] = array(
    'signed' => 'signed',
  );
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Bill No';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
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
  /* Filter criterion: Content: Bill Date (field_sr_bill_date) */
  $handler->display->display_options['filters']['field_sr_bill_date_value']['id'] = 'field_sr_bill_date_value';
  $handler->display->display_options['filters']['field_sr_bill_date_value']['table'] = 'field_data_field_sr_bill_date';
  $handler->display->display_options['filters']['field_sr_bill_date_value']['field'] = 'field_sr_bill_date_value';
  $handler->display->display_options['filters']['field_sr_bill_date_value']['relationship'] = 'field_sales_bill_no_ref_nid';
  $handler->display->display_options['filters']['field_sr_bill_date_value']['operator'] = 'between';
  $handler->display->display_options['filters']['field_sr_bill_date_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_sr_bill_date_value']['expose']['operator_id'] = 'field_sr_bill_date_value_op';
  $handler->display->display_options['filters']['field_sr_bill_date_value']['expose']['operator'] = 'field_sr_bill_date_value_op';
  $handler->display->display_options['filters']['field_sr_bill_date_value']['expose']['identifier'] = 'field_sr_bill_date_value';
  $handler->display->display_options['filters']['field_sr_bill_date_value']['expose']['remember_roles'] = array(
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
    14 => 0,
    13 => 0,
  );
  $handler->display->display_options['filters']['field_sr_bill_date_value']['form_type'] = 'date_popup';
  /* Filter criterion: Content: Declaration date (field_declaration_date) */
  $handler->display->display_options['filters']['field_declaration_date_value']['id'] = 'field_declaration_date_value';
  $handler->display->display_options['filters']['field_declaration_date_value']['table'] = 'field_data_field_declaration_date';
  $handler->display->display_options['filters']['field_declaration_date_value']['field'] = 'field_declaration_date_value';
  $handler->display->display_options['filters']['field_declaration_date_value']['operator'] = 'between';
  $handler->display->display_options['filters']['field_declaration_date_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_declaration_date_value']['expose']['operator_id'] = 'field_declaration_date_value_op';
  $handler->display->display_options['filters']['field_declaration_date_value']['expose']['operator'] = 'field_declaration_date_value_op';
  $handler->display->display_options['filters']['field_declaration_date_value']['expose']['identifier'] = 'field_declaration_date_value';
  $handler->display->display_options['filters']['field_declaration_date_value']['expose']['remember_roles'] = array(
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
    14 => 0,
    13 => 0,
  );
  $handler->display->display_options['filters']['field_declaration_date_value']['form_type'] = 'date_popup';
  $handler->display->display_options['filters']['field_declaration_date_value']['default_date'] = '-1 now';
  $handler->display->display_options['filters']['field_declaration_date_value']['default_to_date'] = 'now';
  /* Filter criterion: Content: File (field_file:fid) */
  $handler->display->display_options['filters']['field_file_fid']['id'] = 'field_file_fid';
  $handler->display->display_options['filters']['field_file_fid']['table'] = 'field_data_field_file';
  $handler->display->display_options['filters']['field_file_fid']['field'] = 'field_file_fid';
  $handler->display->display_options['filters']['field_file_fid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_file_fid']['expose']['operator_id'] = 'field_file_fid_op';
  $handler->display->display_options['filters']['field_file_fid']['expose']['label'] = 'Fill Uploaded';
  $handler->display->display_options['filters']['field_file_fid']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['field_file_fid']['expose']['operator'] = 'field_file_fid_op';
  $handler->display->display_options['filters']['field_file_fid']['expose']['identifier'] = 'field_file_fid';
  $handler->display->display_options['filters']['field_file_fid']['expose']['remember_roles'] = array(
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
    14 => 0,
    13 => 0,
  );
  /* Filter criterion: Content: terminated bill flag (field_terminated_bill_flag) */
  $handler->display->display_options['filters']['field_terminated_bill_flag_value']['id'] = 'field_terminated_bill_flag_value';
  $handler->display->display_options['filters']['field_terminated_bill_flag_value']['table'] = 'field_data_field_terminated_bill_flag';
  $handler->display->display_options['filters']['field_terminated_bill_flag_value']['field'] = 'field_terminated_bill_flag_value';
  $handler->display->display_options['filters']['field_terminated_bill_flag_value']['operator'] = 'not';
  $handler->display->display_options['filters']['field_terminated_bill_flag_value']['value'] = array(
    1 => '1',
  );
  $handler->display->display_options['path'] = 'signed-bills-upload';

  /* Display: Bounced Cheques Upload */
  $handler = $view->new_display('page', 'Bounced Cheques Upload', 'page_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Bounced Cheques Upload';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Bill Number';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Content: Bill Date */
  $handler->display->display_options['fields']['field_sr_bill_date']['id'] = 'field_sr_bill_date';
  $handler->display->display_options['fields']['field_sr_bill_date']['table'] = 'field_data_field_sr_bill_date';
  $handler->display->display_options['fields']['field_sr_bill_date']['field'] = 'field_sr_bill_date';
  $handler->display->display_options['fields']['field_sr_bill_date']['relationship'] = 'field_sales_bill_no_ref_nid';
  $handler->display->display_options['fields']['field_sr_bill_date']['settings'] = array(
    'format_type' => 'sales_day',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_remaining_days' => 0,
  );
  /* Field: Content: Declaration date */
  $handler->display->display_options['fields']['field_declaration_date']['id'] = 'field_declaration_date';
  $handler->display->display_options['fields']['field_declaration_date']['table'] = 'field_data_field_declaration_date';
  $handler->display->display_options['fields']['field_declaration_date']['field'] = 'field_declaration_date';
  $handler->display->display_options['fields']['field_declaration_date']['settings'] = array(
    'format_type' => 'sales_day',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_remaining_days' => 0,
  );
  /* Field: Content: party_code_reference */
  $handler->display->display_options['fields']['field_party_code_reference']['id'] = 'field_party_code_reference';
  $handler->display->display_options['fields']['field_party_code_reference']['table'] = 'field_data_field_party_code_reference';
  $handler->display->display_options['fields']['field_party_code_reference']['field'] = 'field_party_code_reference';
  $handler->display->display_options['fields']['field_party_code_reference']['relationship'] = 'field_sales_bill_no_ref_nid';
  $handler->display->display_options['fields']['field_party_code_reference']['label'] = 'Party Code';
  $handler->display->display_options['fields']['field_party_code_reference']['type'] = 'node_reference_plain';
  /* Field: Content: Beat Name */
  $handler->display->display_options['fields']['field_beat_name']['id'] = 'field_beat_name';
  $handler->display->display_options['fields']['field_beat_name']['table'] = 'field_data_field_beat_name';
  $handler->display->display_options['fields']['field_beat_name']['field'] = 'field_beat_name';
  $handler->display->display_options['fields']['field_beat_name']['relationship'] = 'field_party_code_reference_nid';
  /* Field: Content: Salesman */
  $handler->display->display_options['fields']['field_salesman']['id'] = 'field_salesman';
  $handler->display->display_options['fields']['field_salesman']['table'] = 'field_data_field_salesman';
  $handler->display->display_options['fields']['field_salesman']['field'] = 'field_salesman';
  $handler->display->display_options['fields']['field_salesman']['relationship'] = 'field_party_code_reference_nid';
  /* Field: Content: Salesman Code */
  $handler->display->display_options['fields']['field_salesman_code']['id'] = 'field_salesman_code';
  $handler->display->display_options['fields']['field_salesman_code']['table'] = 'field_data_field_salesman_code';
  $handler->display->display_options['fields']['field_salesman_code']['field'] = 'field_salesman_code';
  $handler->display->display_options['fields']['field_salesman_code']['relationship'] = 'field_party_code_reference_nid';
  /* Field: Content: Bill Value */
  $handler->display->display_options['fields']['field_bill_value']['id'] = 'field_bill_value';
  $handler->display->display_options['fields']['field_bill_value']['table'] = 'field_data_field_bill_value';
  $handler->display->display_options['fields']['field_bill_value']['field'] = 'field_bill_value';
  $handler->display->display_options['fields']['field_bill_value']['relationship'] = 'field_sales_bill_no_ref_nid';
  /* Field: Content: remaining amt collection */
  $handler->display->display_options['fields']['field_remaining_amt_collection']['id'] = 'field_remaining_amt_collection';
  $handler->display->display_options['fields']['field_remaining_amt_collection']['table'] = 'field_data_field_remaining_amt_collection';
  $handler->display->display_options['fields']['field_remaining_amt_collection']['field'] = 'field_remaining_amt_collection';
  $handler->display->display_options['fields']['field_remaining_amt_collection']['label'] = 'To Be Collected Value';
  $handler->display->display_options['fields']['field_remaining_amt_collection']['settings'] = array(
    'thousand_separator' => '',
    'decimal_separator' => '.',
    'scale' => '2',
    'prefix_suffix' => 1,
  );
  /* Field: Content: File (editable) */
  $handler->display->display_options['fields']['field_file_editable']['id'] = 'field_file_editable';
  $handler->display->display_options['fields']['field_file_editable']['table'] = 'field_data_field_file';
  $handler->display->display_options['fields']['field_file_editable']['field'] = 'field_file_editable';
  $handler->display->display_options['fields']['field_file_editable']['label'] = 'File Upload';
  $handler->display->display_options['fields']['field_file_editable']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_file_editable']['type'] = 'services';
  $handler->display->display_options['fields']['field_file_editable']['settings'] = array(
    'data_element_key' => '',
    'skip_safe' => 0,
    'skip_empty_values' => 0,
  );
  $handler->display->display_options['fields']['field_file_editable']['widget_type'] = 'file_generic';
  $handler->display->display_options['fields']['field_file_editable']['suppress_label'] = 1;
  $handler->display->display_options['fields']['field_file_editable']['suppress_description'] = 0;
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
    'payment_collection' => 'payment_collection',
  );
  /* Filter criterion: Content: Flag (field_flag) */
  $handler->display->display_options['filters']['field_flag_value']['id'] = 'field_flag_value';
  $handler->display->display_options['filters']['field_flag_value']['table'] = 'field_data_field_flag';
  $handler->display->display_options['filters']['field_flag_value']['field'] = 'field_flag_value';
  $handler->display->display_options['filters']['field_flag_value']['value'] = array(
    'bounced' => 'bounced',
  );
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Bill No';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
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
  /* Filter criterion: Content: Bill Date (field_sr_bill_date) */
  $handler->display->display_options['filters']['field_sr_bill_date_value']['id'] = 'field_sr_bill_date_value';
  $handler->display->display_options['filters']['field_sr_bill_date_value']['table'] = 'field_data_field_sr_bill_date';
  $handler->display->display_options['filters']['field_sr_bill_date_value']['field'] = 'field_sr_bill_date_value';
  $handler->display->display_options['filters']['field_sr_bill_date_value']['relationship'] = 'field_sales_bill_no_ref_nid';
  $handler->display->display_options['filters']['field_sr_bill_date_value']['operator'] = 'between';
  $handler->display->display_options['filters']['field_sr_bill_date_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_sr_bill_date_value']['expose']['operator_id'] = 'field_sr_bill_date_value_op';
  $handler->display->display_options['filters']['field_sr_bill_date_value']['expose']['operator'] = 'field_sr_bill_date_value_op';
  $handler->display->display_options['filters']['field_sr_bill_date_value']['expose']['identifier'] = 'field_sr_bill_date_value';
  $handler->display->display_options['filters']['field_sr_bill_date_value']['expose']['remember_roles'] = array(
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
    14 => 0,
    13 => 0,
  );
  $handler->display->display_options['filters']['field_sr_bill_date_value']['form_type'] = 'date_popup';
  /* Filter criterion: Content: Declaration date (field_declaration_date) */
  $handler->display->display_options['filters']['field_declaration_date_value']['id'] = 'field_declaration_date_value';
  $handler->display->display_options['filters']['field_declaration_date_value']['table'] = 'field_data_field_declaration_date';
  $handler->display->display_options['filters']['field_declaration_date_value']['field'] = 'field_declaration_date_value';
  $handler->display->display_options['filters']['field_declaration_date_value']['operator'] = 'between';
  $handler->display->display_options['filters']['field_declaration_date_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_declaration_date_value']['expose']['operator_id'] = 'field_declaration_date_value_op';
  $handler->display->display_options['filters']['field_declaration_date_value']['expose']['operator'] = 'field_declaration_date_value_op';
  $handler->display->display_options['filters']['field_declaration_date_value']['expose']['identifier'] = 'field_declaration_date_value';
  $handler->display->display_options['filters']['field_declaration_date_value']['expose']['remember_roles'] = array(
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
    14 => 0,
    13 => 0,
  );
  $handler->display->display_options['filters']['field_declaration_date_value']['form_type'] = 'date_popup';
  $handler->display->display_options['filters']['field_declaration_date_value']['default_date'] = '-1 day';
  $handler->display->display_options['filters']['field_declaration_date_value']['default_to_date'] = 'now';
  /* Filter criterion: Content: File (field_file:fid) */
  $handler->display->display_options['filters']['field_file_fid']['id'] = 'field_file_fid';
  $handler->display->display_options['filters']['field_file_fid']['table'] = 'field_data_field_file';
  $handler->display->display_options['filters']['field_file_fid']['field'] = 'field_file_fid';
  $handler->display->display_options['filters']['field_file_fid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_file_fid']['expose']['operator_id'] = 'field_file_fid_op';
  $handler->display->display_options['filters']['field_file_fid']['expose']['label'] = 'Fill Uploaded';
  $handler->display->display_options['filters']['field_file_fid']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['field_file_fid']['expose']['operator'] = 'field_file_fid_op';
  $handler->display->display_options['filters']['field_file_fid']['expose']['identifier'] = 'field_file_fid';
  $handler->display->display_options['filters']['field_file_fid']['expose']['remember_roles'] = array(
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
    14 => 0,
    13 => 0,
  );
  /* Filter criterion: Content: terminated bill flag (field_terminated_bill_flag) */
  $handler->display->display_options['filters']['field_terminated_bill_flag_value']['id'] = 'field_terminated_bill_flag_value';
  $handler->display->display_options['filters']['field_terminated_bill_flag_value']['table'] = 'field_data_field_terminated_bill_flag';
  $handler->display->display_options['filters']['field_terminated_bill_flag_value']['field'] = 'field_terminated_bill_flag_value';
  $handler->display->display_options['filters']['field_terminated_bill_flag_value']['operator'] = 'not';
  $handler->display->display_options['filters']['field_terminated_bill_flag_value']['value'] = array(
    1 => '1',
  );
  $handler->display->display_options['path'] = 'bounced-cheques-upload';

  /* Display: Signed Bills Assignment */
  $handler = $view->new_display('page', 'Signed Bills Assignment', 'page_2');
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Bill Number';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Content: Bill Date */
  $handler->display->display_options['fields']['field_sr_bill_date']['id'] = 'field_sr_bill_date';
  $handler->display->display_options['fields']['field_sr_bill_date']['table'] = 'field_data_field_sr_bill_date';
  $handler->display->display_options['fields']['field_sr_bill_date']['field'] = 'field_sr_bill_date';
  $handler->display->display_options['fields']['field_sr_bill_date']['relationship'] = 'field_sales_bill_no_ref_nid';
  $handler->display->display_options['fields']['field_sr_bill_date']['settings'] = array(
    'format_type' => 'sales_day',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_remaining_days' => 0,
  );
  /* Field: Content: party_code_reference */
  $handler->display->display_options['fields']['field_party_code_reference']['id'] = 'field_party_code_reference';
  $handler->display->display_options['fields']['field_party_code_reference']['table'] = 'field_data_field_party_code_reference';
  $handler->display->display_options['fields']['field_party_code_reference']['field'] = 'field_party_code_reference';
  $handler->display->display_options['fields']['field_party_code_reference']['relationship'] = 'field_sales_bill_no_ref_nid';
  $handler->display->display_options['fields']['field_party_code_reference']['label'] = 'Party Code';
  $handler->display->display_options['fields']['field_party_code_reference']['type'] = 'node_reference_plain';
  /* Field: Content: Beat Name */
  $handler->display->display_options['fields']['field_beat_name']['id'] = 'field_beat_name';
  $handler->display->display_options['fields']['field_beat_name']['table'] = 'field_data_field_beat_name';
  $handler->display->display_options['fields']['field_beat_name']['field'] = 'field_beat_name';
  $handler->display->display_options['fields']['field_beat_name']['relationship'] = 'field_party_code_reference_nid';
  /* Field: Content: Salesman */
  $handler->display->display_options['fields']['field_salesman']['id'] = 'field_salesman';
  $handler->display->display_options['fields']['field_salesman']['table'] = 'field_data_field_salesman';
  $handler->display->display_options['fields']['field_salesman']['field'] = 'field_salesman';
  $handler->display->display_options['fields']['field_salesman']['relationship'] = 'field_party_code_reference_nid';
  /* Field: Content: Salesman Code */
  $handler->display->display_options['fields']['field_salesman_code']['id'] = 'field_salesman_code';
  $handler->display->display_options['fields']['field_salesman_code']['table'] = 'field_data_field_salesman_code';
  $handler->display->display_options['fields']['field_salesman_code']['field'] = 'field_salesman_code';
  $handler->display->display_options['fields']['field_salesman_code']['relationship'] = 'field_party_code_reference_nid';
  /* Field: Content: Bill Value */
  $handler->display->display_options['fields']['field_bill_value']['id'] = 'field_bill_value';
  $handler->display->display_options['fields']['field_bill_value']['table'] = 'field_data_field_bill_value';
  $handler->display->display_options['fields']['field_bill_value']['field'] = 'field_bill_value';
  $handler->display->display_options['fields']['field_bill_value']['relationship'] = 'field_sales_bill_no_ref_nid';
  /* Field: Content: remaining amt collection */
  $handler->display->display_options['fields']['field_remaining_amt_collection']['id'] = 'field_remaining_amt_collection';
  $handler->display->display_options['fields']['field_remaining_amt_collection']['table'] = 'field_data_field_remaining_amt_collection';
  $handler->display->display_options['fields']['field_remaining_amt_collection']['field'] = 'field_remaining_amt_collection';
  $handler->display->display_options['fields']['field_remaining_amt_collection']['label'] = 'To Be Collected Value';
  $handler->display->display_options['fields']['field_remaining_amt_collection']['settings'] = array(
    'thousand_separator' => '',
    'decimal_separator' => '.',
    'scale' => '2',
    'prefix_suffix' => 1,
  );
  /* Field: Content: Assigned Person (editable) */
  $handler->display->display_options['fields']['field_assigned_person_editable']['id'] = 'field_assigned_person_editable';
  $handler->display->display_options['fields']['field_assigned_person_editable']['table'] = 'field_data_field_assigned_person';
  $handler->display->display_options['fields']['field_assigned_person_editable']['field'] = 'field_assigned_person_editable';
  $handler->display->display_options['fields']['field_assigned_person_editable']['label'] = 'Assigned Person';
  $handler->display->display_options['fields']['field_assigned_person_editable']['type'] = 'services';
  $handler->display->display_options['fields']['field_assigned_person_editable']['settings'] = array(
    'data_element_key' => '',
    'skip_safe' => 0,
    'skip_empty_values' => 0,
    'skip_text_format' => 1,
  );
  $handler->display->display_options['fields']['field_assigned_person_editable']['widget_type'] = '0';
  $handler->display->display_options['fields']['field_assigned_person_editable']['suppress_label'] = 1;
  $handler->display->display_options['fields']['field_assigned_person_editable']['suppress_description'] = 0;
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
    'payment_collection' => 'payment_collection',
  );
  /* Filter criterion: Content: Flag (field_flag) */
  $handler->display->display_options['filters']['field_flag_value']['id'] = 'field_flag_value';
  $handler->display->display_options['filters']['field_flag_value']['table'] = 'field_data_field_flag';
  $handler->display->display_options['filters']['field_flag_value']['field'] = 'field_flag_value';
  $handler->display->display_options['filters']['field_flag_value']['value'] = array(
    'signed' => 'signed',
  );
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Bill No';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
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
  /* Filter criterion: Content: Bill Date (field_sr_bill_date) */
  $handler->display->display_options['filters']['field_sr_bill_date_value']['id'] = 'field_sr_bill_date_value';
  $handler->display->display_options['filters']['field_sr_bill_date_value']['table'] = 'field_data_field_sr_bill_date';
  $handler->display->display_options['filters']['field_sr_bill_date_value']['field'] = 'field_sr_bill_date_value';
  $handler->display->display_options['filters']['field_sr_bill_date_value']['relationship'] = 'field_sales_bill_no_ref_nid';
  $handler->display->display_options['filters']['field_sr_bill_date_value']['operator'] = 'between';
  $handler->display->display_options['filters']['field_sr_bill_date_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_sr_bill_date_value']['expose']['operator_id'] = 'field_sr_bill_date_value_op';
  $handler->display->display_options['filters']['field_sr_bill_date_value']['expose']['operator'] = 'field_sr_bill_date_value_op';
  $handler->display->display_options['filters']['field_sr_bill_date_value']['expose']['identifier'] = 'field_sr_bill_date_value';
  $handler->display->display_options['filters']['field_sr_bill_date_value']['expose']['remember_roles'] = array(
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
    14 => 0,
    13 => 0,
  );
  $handler->display->display_options['filters']['field_sr_bill_date_value']['form_type'] = 'date_popup';
  $handler->display->display_options['filters']['field_sr_bill_date_value']['default_date'] = '-7 day';
  $handler->display->display_options['filters']['field_sr_bill_date_value']['default_to_date'] = 'now';
  /* Filter criterion: Content: Beat Name (field_beat_name) */
  $handler->display->display_options['filters']['field_beat_name_value']['id'] = 'field_beat_name_value';
  $handler->display->display_options['filters']['field_beat_name_value']['table'] = 'field_data_field_beat_name';
  $handler->display->display_options['filters']['field_beat_name_value']['field'] = 'field_beat_name_value';
  $handler->display->display_options['filters']['field_beat_name_value']['relationship'] = 'field_party_code_reference_nid';
  $handler->display->display_options['filters']['field_beat_name_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_beat_name_value']['expose']['operator_id'] = 'field_beat_name_value_op';
  $handler->display->display_options['filters']['field_beat_name_value']['expose']['label'] = 'Beat Name';
  $handler->display->display_options['filters']['field_beat_name_value']['expose']['operator'] = 'field_beat_name_value_op';
  $handler->display->display_options['filters']['field_beat_name_value']['expose']['identifier'] = 'field_beat_name_value';
  $handler->display->display_options['filters']['field_beat_name_value']['expose']['remember_roles'] = array(
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
    14 => 0,
    13 => 0,
  );
  /* Filter criterion: Content: Salesman (field_salesman) */
  $handler->display->display_options['filters']['field_salesman_value']['id'] = 'field_salesman_value';
  $handler->display->display_options['filters']['field_salesman_value']['table'] = 'field_data_field_salesman';
  $handler->display->display_options['filters']['field_salesman_value']['field'] = 'field_salesman_value';
  $handler->display->display_options['filters']['field_salesman_value']['relationship'] = 'field_party_code_reference_nid';
  $handler->display->display_options['filters']['field_salesman_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_salesman_value']['expose']['operator_id'] = 'field_salesman_value_op';
  $handler->display->display_options['filters']['field_salesman_value']['expose']['label'] = 'Salesman';
  $handler->display->display_options['filters']['field_salesman_value']['expose']['operator'] = 'field_salesman_value_op';
  $handler->display->display_options['filters']['field_salesman_value']['expose']['identifier'] = 'field_salesman_value';
  $handler->display->display_options['filters']['field_salesman_value']['expose']['remember_roles'] = array(
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
    14 => 0,
    13 => 0,
  );
  /* Filter criterion: Content: Assigned Person (field_assigned_person) */
  $handler->display->display_options['filters']['field_assigned_person_value']['id'] = 'field_assigned_person_value';
  $handler->display->display_options['filters']['field_assigned_person_value']['table'] = 'field_data_field_assigned_person';
  $handler->display->display_options['filters']['field_assigned_person_value']['field'] = 'field_assigned_person_value';
  $handler->display->display_options['filters']['field_assigned_person_value']['operator'] = 'empty';
  /* Filter criterion: Content: terminated bill flag (field_terminated_bill_flag) */
  $handler->display->display_options['filters']['field_terminated_bill_flag_value']['id'] = 'field_terminated_bill_flag_value';
  $handler->display->display_options['filters']['field_terminated_bill_flag_value']['table'] = 'field_data_field_terminated_bill_flag';
  $handler->display->display_options['filters']['field_terminated_bill_flag_value']['field'] = 'field_terminated_bill_flag_value';
  $handler->display->display_options['filters']['field_terminated_bill_flag_value']['operator'] = 'not';
  $handler->display->display_options['filters']['field_terminated_bill_flag_value']['value'] = array(
    1 => '1',
  );
  $handler->display->display_options['path'] = 'signed-bills-assignment';

  /* Display: Bounced Cheque Assignment */
  $handler = $view->new_display('page', 'Bounced Cheque Assignment', 'page_3');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Bounced Cheque Assignment';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Bill Number';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Content: Bill Date */
  $handler->display->display_options['fields']['field_sr_bill_date']['id'] = 'field_sr_bill_date';
  $handler->display->display_options['fields']['field_sr_bill_date']['table'] = 'field_data_field_sr_bill_date';
  $handler->display->display_options['fields']['field_sr_bill_date']['field'] = 'field_sr_bill_date';
  $handler->display->display_options['fields']['field_sr_bill_date']['relationship'] = 'field_sales_bill_no_ref_nid';
  $handler->display->display_options['fields']['field_sr_bill_date']['settings'] = array(
    'format_type' => 'sales_day',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_remaining_days' => 0,
  );
  /* Field: Content: party_code_reference */
  $handler->display->display_options['fields']['field_party_code_reference']['id'] = 'field_party_code_reference';
  $handler->display->display_options['fields']['field_party_code_reference']['table'] = 'field_data_field_party_code_reference';
  $handler->display->display_options['fields']['field_party_code_reference']['field'] = 'field_party_code_reference';
  $handler->display->display_options['fields']['field_party_code_reference']['relationship'] = 'field_sales_bill_no_ref_nid';
  $handler->display->display_options['fields']['field_party_code_reference']['label'] = 'Party Code';
  $handler->display->display_options['fields']['field_party_code_reference']['type'] = 'node_reference_plain';
  /* Field: Content: Beat Name */
  $handler->display->display_options['fields']['field_beat_name']['id'] = 'field_beat_name';
  $handler->display->display_options['fields']['field_beat_name']['table'] = 'field_data_field_beat_name';
  $handler->display->display_options['fields']['field_beat_name']['field'] = 'field_beat_name';
  $handler->display->display_options['fields']['field_beat_name']['relationship'] = 'field_party_code_reference_nid';
  /* Field: Content: Salesman */
  $handler->display->display_options['fields']['field_salesman']['id'] = 'field_salesman';
  $handler->display->display_options['fields']['field_salesman']['table'] = 'field_data_field_salesman';
  $handler->display->display_options['fields']['field_salesman']['field'] = 'field_salesman';
  $handler->display->display_options['fields']['field_salesman']['relationship'] = 'field_party_code_reference_nid';
  /* Field: Content: Salesman Code */
  $handler->display->display_options['fields']['field_salesman_code']['id'] = 'field_salesman_code';
  $handler->display->display_options['fields']['field_salesman_code']['table'] = 'field_data_field_salesman_code';
  $handler->display->display_options['fields']['field_salesman_code']['field'] = 'field_salesman_code';
  $handler->display->display_options['fields']['field_salesman_code']['relationship'] = 'field_party_code_reference_nid';
  /* Field: Content: Bill Value */
  $handler->display->display_options['fields']['field_bill_value']['id'] = 'field_bill_value';
  $handler->display->display_options['fields']['field_bill_value']['table'] = 'field_data_field_bill_value';
  $handler->display->display_options['fields']['field_bill_value']['field'] = 'field_bill_value';
  $handler->display->display_options['fields']['field_bill_value']['relationship'] = 'field_sales_bill_no_ref_nid';
  /* Field: Content: remaining amt collection */
  $handler->display->display_options['fields']['field_remaining_amt_collection']['id'] = 'field_remaining_amt_collection';
  $handler->display->display_options['fields']['field_remaining_amt_collection']['table'] = 'field_data_field_remaining_amt_collection';
  $handler->display->display_options['fields']['field_remaining_amt_collection']['field'] = 'field_remaining_amt_collection';
  $handler->display->display_options['fields']['field_remaining_amt_collection']['label'] = 'To Be Collected Value';
  $handler->display->display_options['fields']['field_remaining_amt_collection']['settings'] = array(
    'thousand_separator' => '',
    'decimal_separator' => '.',
    'scale' => '2',
    'prefix_suffix' => 1,
  );
  /* Field: Content: Assigned Person (editable) */
  $handler->display->display_options['fields']['field_assigned_person_editable']['id'] = 'field_assigned_person_editable';
  $handler->display->display_options['fields']['field_assigned_person_editable']['table'] = 'field_data_field_assigned_person';
  $handler->display->display_options['fields']['field_assigned_person_editable']['field'] = 'field_assigned_person_editable';
  $handler->display->display_options['fields']['field_assigned_person_editable']['label'] = 'Assigned Person';
  $handler->display->display_options['fields']['field_assigned_person_editable']['type'] = 'services';
  $handler->display->display_options['fields']['field_assigned_person_editable']['settings'] = array(
    'data_element_key' => '',
    'skip_safe' => 0,
    'skip_empty_values' => 0,
    'skip_text_format' => 1,
  );
  $handler->display->display_options['fields']['field_assigned_person_editable']['widget_type'] = 'text_textfield';
  $handler->display->display_options['fields']['field_assigned_person_editable']['suppress_label'] = 1;
  $handler->display->display_options['fields']['field_assigned_person_editable']['suppress_description'] = 0;
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
    'payment_collection' => 'payment_collection',
  );
  /* Filter criterion: Content: Flag (field_flag) */
  $handler->display->display_options['filters']['field_flag_value']['id'] = 'field_flag_value';
  $handler->display->display_options['filters']['field_flag_value']['table'] = 'field_data_field_flag';
  $handler->display->display_options['filters']['field_flag_value']['field'] = 'field_flag_value';
  $handler->display->display_options['filters']['field_flag_value']['value'] = array(
    'bounced' => 'bounced',
  );
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Bill No';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
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
  /* Filter criterion: Content: Bill Date (field_sr_bill_date) */
  $handler->display->display_options['filters']['field_sr_bill_date_value']['id'] = 'field_sr_bill_date_value';
  $handler->display->display_options['filters']['field_sr_bill_date_value']['table'] = 'field_data_field_sr_bill_date';
  $handler->display->display_options['filters']['field_sr_bill_date_value']['field'] = 'field_sr_bill_date_value';
  $handler->display->display_options['filters']['field_sr_bill_date_value']['relationship'] = 'field_sales_bill_no_ref_nid';
  $handler->display->display_options['filters']['field_sr_bill_date_value']['operator'] = 'between';
  $handler->display->display_options['filters']['field_sr_bill_date_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_sr_bill_date_value']['expose']['operator_id'] = 'field_sr_bill_date_value_op';
  $handler->display->display_options['filters']['field_sr_bill_date_value']['expose']['operator'] = 'field_sr_bill_date_value_op';
  $handler->display->display_options['filters']['field_sr_bill_date_value']['expose']['identifier'] = 'field_sr_bill_date_value';
  $handler->display->display_options['filters']['field_sr_bill_date_value']['expose']['remember_roles'] = array(
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
    14 => 0,
    13 => 0,
  );
  $handler->display->display_options['filters']['field_sr_bill_date_value']['form_type'] = 'date_popup';
  $handler->display->display_options['filters']['field_sr_bill_date_value']['default_date'] = '-7 now';
  $handler->display->display_options['filters']['field_sr_bill_date_value']['default_to_date'] = 'now';
  /* Filter criterion: Content: Beat Name (field_beat_name) */
  $handler->display->display_options['filters']['field_beat_name_value']['id'] = 'field_beat_name_value';
  $handler->display->display_options['filters']['field_beat_name_value']['table'] = 'field_data_field_beat_name';
  $handler->display->display_options['filters']['field_beat_name_value']['field'] = 'field_beat_name_value';
  $handler->display->display_options['filters']['field_beat_name_value']['relationship'] = 'field_party_code_reference_nid';
  $handler->display->display_options['filters']['field_beat_name_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_beat_name_value']['expose']['operator_id'] = 'field_beat_name_value_op';
  $handler->display->display_options['filters']['field_beat_name_value']['expose']['label'] = 'Beat Name';
  $handler->display->display_options['filters']['field_beat_name_value']['expose']['operator'] = 'field_beat_name_value_op';
  $handler->display->display_options['filters']['field_beat_name_value']['expose']['identifier'] = 'field_beat_name_value';
  $handler->display->display_options['filters']['field_beat_name_value']['expose']['remember_roles'] = array(
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
    14 => 0,
    13 => 0,
  );
  /* Filter criterion: Content: Salesman (field_salesman) */
  $handler->display->display_options['filters']['field_salesman_value']['id'] = 'field_salesman_value';
  $handler->display->display_options['filters']['field_salesman_value']['table'] = 'field_data_field_salesman';
  $handler->display->display_options['filters']['field_salesman_value']['field'] = 'field_salesman_value';
  $handler->display->display_options['filters']['field_salesman_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_salesman_value']['expose']['operator_id'] = 'field_salesman_value_op';
  $handler->display->display_options['filters']['field_salesman_value']['expose']['label'] = 'Salesman';
  $handler->display->display_options['filters']['field_salesman_value']['expose']['operator'] = 'field_salesman_value_op';
  $handler->display->display_options['filters']['field_salesman_value']['expose']['identifier'] = 'field_salesman_value';
  $handler->display->display_options['filters']['field_salesman_value']['expose']['remember_roles'] = array(
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
    14 => 0,
    13 => 0,
  );
  /* Filter criterion: Content: Cheque date (field_cheque_date) */
  $handler->display->display_options['filters']['field_cheque_date_value']['id'] = 'field_cheque_date_value';
  $handler->display->display_options['filters']['field_cheque_date_value']['table'] = 'field_data_field_cheque_date';
  $handler->display->display_options['filters']['field_cheque_date_value']['field'] = 'field_cheque_date_value';
  $handler->display->display_options['filters']['field_cheque_date_value']['operator'] = 'between';
  $handler->display->display_options['filters']['field_cheque_date_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_cheque_date_value']['expose']['operator_id'] = 'field_cheque_date_value_op';
  $handler->display->display_options['filters']['field_cheque_date_value']['expose']['operator'] = 'field_cheque_date_value_op';
  $handler->display->display_options['filters']['field_cheque_date_value']['expose']['identifier'] = 'field_cheque_date_value';
  $handler->display->display_options['filters']['field_cheque_date_value']['expose']['remember_roles'] = array(
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
    14 => 0,
    13 => 0,
  );
  $handler->display->display_options['filters']['field_cheque_date_value']['form_type'] = 'date_popup';
  /* Filter criterion: Content: Cheque Number (field_cheque_number) */
  $handler->display->display_options['filters']['field_cheque_number_value']['id'] = 'field_cheque_number_value';
  $handler->display->display_options['filters']['field_cheque_number_value']['table'] = 'field_data_field_cheque_number';
  $handler->display->display_options['filters']['field_cheque_number_value']['field'] = 'field_cheque_number_value';
  $handler->display->display_options['filters']['field_cheque_number_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_cheque_number_value']['expose']['operator_id'] = 'field_cheque_number_value_op';
  $handler->display->display_options['filters']['field_cheque_number_value']['expose']['label'] = 'Cheque No';
  $handler->display->display_options['filters']['field_cheque_number_value']['expose']['operator'] = 'field_cheque_number_value_op';
  $handler->display->display_options['filters']['field_cheque_number_value']['expose']['identifier'] = 'field_cheque_number_value';
  $handler->display->display_options['filters']['field_cheque_number_value']['expose']['remember_roles'] = array(
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
    14 => 0,
    13 => 0,
  );
  /* Filter criterion: Content: Assigned Person (field_assigned_person) */
  $handler->display->display_options['filters']['field_assigned_person_value']['id'] = 'field_assigned_person_value';
  $handler->display->display_options['filters']['field_assigned_person_value']['table'] = 'field_data_field_assigned_person';
  $handler->display->display_options['filters']['field_assigned_person_value']['field'] = 'field_assigned_person_value';
  $handler->display->display_options['filters']['field_assigned_person_value']['operator'] = 'empty';
  /* Filter criterion: Content: terminated bill flag (field_terminated_bill_flag) */
  $handler->display->display_options['filters']['field_terminated_bill_flag_value']['id'] = 'field_terminated_bill_flag_value';
  $handler->display->display_options['filters']['field_terminated_bill_flag_value']['table'] = 'field_data_field_terminated_bill_flag';
  $handler->display->display_options['filters']['field_terminated_bill_flag_value']['field'] = 'field_terminated_bill_flag_value';
  $handler->display->display_options['filters']['field_terminated_bill_flag_value']['operator'] = 'not';
  $handler->display->display_options['filters']['field_terminated_bill_flag_value']['value'] = array(
    1 => '1',
  );
  $handler->display->display_options['path'] = 'bounced-cheques-assignment';
  $export['signed_bills_upload'] = $view;

  return $export;
}
