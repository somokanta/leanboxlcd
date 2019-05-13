<?php 

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function get_cancel_discard_bills($form, &$form_state){
  $form = array();

  $form['#prefix'] = '<div id="itemlist-div" class="custom_form_wrapper trip_autocomplete_textbox">';
  $form['#suffix'] = '</div>';
  
  $form['#attached']['js'][] = drupal_get_path('module', 'dispatch_planning') . '/js/dispatch_planning_form.js';
  $form['bill_number'] = array(
    //'#prefix' => '<div class="textfield-wrapper">',
    '#type' => 'textfield',
    '#title' => t('Bill number'),
    //'#required' => TRUE,
    '#autocomplete_path' => 'bill-not-attached-to-trip/autocomplete',
    '#prefix' => '<div class="bill_number_autocomplete_textbox">',
    '#suffix' => '<div style="padding: 0 0 12px 0;">OR</div></div>'
  );

  $form['bill_date_from'] = array(
    '#type' => 'date_popup',
    '#title' => t('Bill Date From'),
    '#date_format' => 'Y-m-d',
    //'#default_value' => '2019-03-27',
    '#date_label_position' => '',
    '#date_year_range' => '-20:+2',
    '#attributes' => array('class' => array('dispatch-date-from')),
    '#size' => 30,
    '#validate'=> 'bill_date_from_validation',
    '#prefix' => '<div class="form_row"><div class="col_2 date_range">',
    '#suffix' => '</div>'
  );

  $form['bill_date_to'] = array(
    '#type' => 'date_popup',
    '#title' => t('Bill Date To'),
    '#date_format' => 'Y-m-d',
    //'#default_value' => '2019-03-29',
    '#date_label_position' => '',
    '#date_year_range' => '-20:+2',
    '#attributes' => array('class' => array('dispatch-date-to')),
    '#size' => 30,
    '#validate'=> 'bill_date_to_validation',
    '#prefix' => '<div class="col_2 date_range">',
    '#suffix' => '</div></div>'
  );

  $form['apply'] = array(
    '#type' => 'submit',
    '#value' => t('Add Bills'),
    '#ajax' => array(
      'wrapper' => 'itemlist-div',
      'callback' => 'bills_filter_ajax_form_apply',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    '#submit' => array('bills_filter_form_apply'),
    '#validate' => array('bills_filter_form_validate'),
    '#limit_validation_errors' => array(array('bill_number'), array('bill_date_from'), array('bill_date_to'))
   // '#suffix' => '</div>',
  );

  if (!isset($form_state['bills_data']['bills'])) {
    $form_state['bills_data']['bills'] = array();
  }
  $bill_rows = $form_state['bills_data']['bills'];

  $headers = array(
    'bill_number' => 'Bill Number',
    'bill_date' => 'Bill Date',
    'party_code' => 'Party Code',
    'pl_group'=> 'PL Group',
    'party_name' => 'Party Name',
    'bill_value' => 'Bill Value'
  );
    
  foreach ($headers as $key => $value) {
    $sort_options[$key . ':asc'] = $value . ' - Asc';
    $sort_options[$key . ':desc'] = $value . ' - Desc';
  }

  $form['sort'] = array(
    '#type' => 'select',
    '#options' => $sort_options,
    '#ajax' => array(
      'wrapper' => 'itemlist-div',
      'callback' => 'custom_ajax_sort_callback',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    '#attributes' => array(
      'class' => array('sort-select', 'element-invisible'),
    ),
  );
    
  if (isset($form_state['triggering_element']) && ($form_state['triggering_element']['#name'] == 'sort')) {
    $active = explode(':', $form_state['values']['sort']);
    $form_state['bills_data']['sort_field'] = $active[0];
    $form_state['bills_data']['sort_order'] = $active[1];
  }

  if (isset($form_state['bills_data']['sort_field'])) {
    $active_sort = $form_state['bills_data']['sort_field'];
  }
  if (isset($form_state['bills_data']['sort_order'])) {
    $active_order = $form_state['bills_data']['sort_order'];
  }
  if ($active_order == 'asc') {
    $next_order = 'desc';
  }
  elseif ($active_order == 'desc') {
    $next_order = 'asc';
  }
  
  foreach ($headers as $k => $header) {
    if($k == 'bill_value'){
      $header_array[$k] = array(
        'data' => array(
          '#type' => 'link',
          '#title' => $header . (($active_sort == $k && $next_order) ? theme('tablesort_indicator', array('style' => $next_order)) : ''),
          '#href' => '',
          '#attributes' => array(
            'class' => ($active_sort == $k) ? array('active', 'header-link') : array('header-link'),
            'id' => $k,
            'data-sort' => ($active_sort == $k && $next_order) ? $next_order : '',
          ),
          '#options' => array(
            'html' => true,
          )
        ),
      );
    }else{
      $header_array[$k] = array('data' => $header);
    }
  }
  
  if($bill_rows){
    if (isset($active_sort)) {
      $bill_rows = _get_header_sort($bill_rows, $active_sort, $active_order);
    }
    //dpm($bill_rows, 'bill_rows: ');
    $form['table_items'] = array(
      '#type' => 'tableselect',
      '#header' => $header_array,
      '#options' => $bill_rows,
      '#prefix' => '<div id="table-div">',
      '#suffix' => '</div>',
      '#empty' => t('No bills found.'),
      '#multiple' => TRUE,
    );

    $form['remove_selected_bills'] = array(
      '#type' => 'submit',
      '#value' => t('Remove Checked items'),
      '#ajax' => array(
        'wrapper' => 'itemlist-div',
        'callback' => 'custom_ajax_callback',
        'method' => 'replace',
        'effect' => 'fade',
      ),
      '#submit' => array('remove_selected_bills_ajax_submit'),
      '#limit_validation_errors' => array(array('table_items')),
    );

    $form['action'] = array(
      '#type' => 'select',
      '#title' => t('Action'),
      '#ajax' => array(
        'wrapper' => 'reason_id',
        'callback' => 'action_ajax_callback',
        'event' => 'change',
        'effect' => 'fade'  
      ),
      '#options' => array('cancel' => 'Cancel', 'discard' => 'Discard'),
      '#required' => TRUE
    );
    
    //$action_selected = $form_state['values']['action'];
    $action_selected = !empty($action_selected) ? $form_state['values']['action'] : $form_state['input']['action'];
    $res = get_bill_reasons_as_per_action($action_selected);
    $form['reason'] = array(
      '#type' => 'select',
      '#title' => t('Reason'),
      '#options' => array('' => '--Select--') + $res,
      '#required' => TRUE,
      '#prefix' => '<div id="reason_id">',
      '#suffix' => '</div>'
    );

    $form['updated_by'] = array(
      '#type' => 'select',
      '#title' => t('Updated By'),
      '#options' => array('' => '--Select--') + get_all_resources_by_type('Bill Cancellation Incharge'),
      '#required' => TRUE
    );

    $form['remark'] = array(
      '#type' => 'textarea',//text_format',
      '#title' => t('Remark')
    );

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Submit'),
//      '#ajax' => array(
//        'wrapper' => 'itemlist-div',
//        'callback' => 'bills_filter_ajax_form_submit',
//        'method' => 'replace',
//        'effect' => 'fade',
//      ),
      '#submit' => array('bills_form_submit'),
      '#validate' => array('bills_form_validate'),
      '#limit_validation_errors' => array(array('action'), array('reason'), array('updated_by'))
     // '#suffix' => '</div>',
    );
  }
  return $form;
} 

function custom_ajax_sort_callback($form, &$form_state) {
  return $form;
}

function action_ajax_callback($form, &$form_state){
  return $form['reason'];
}

function remove_selected_bills_ajax_submit($form, &$form_state){
  $rows = $form_state['input']['table_items'];
  $checked = array_filter($form_state['values']['table_items']);
  if (!empty($checked)) {
    $form_state['bills_data']['bills'] = array_diff_key($form_state['bills_data']['bills'], array_flip($checked));
  }
  $form_state['rebuild'] = TRUE;
}


function bills_filter_form_validate($form, &$form_state){
  $bill_number = $form_state['values']['bill_number'];
  $bill_date_from = $form_state['values']['bill_date_from'];
  $bill_date_to = $form_state['values']['bill_date_to'];
  
  if(empty($bill_number) && (empty($bill_date_from) || empty($bill_date_to))){
    form_set_error('bill_number', t('Please select Bill Number or Date Range.'));
  }
  else if(!empty($bill_number) && (!empty($bill_date_from) || !empty($bill_date_to))){
    form_set_error('bill_number', t('Please select either Bill Number or Date Range.'));
    //ajax_redirect('cancel-discard-bills');
  }
  else if(!empty($bill_number)){
    $bill_flag = bill_number_exist($bill_number);
    
    if(!empty($bill_flag)) {
      $bill_attached_flag = check_bill_not_attached_to_trip($bill_number);
      if(!empty($bill_attached_flag)){
        $trip_id = node_load($bill_attached_flag)->title;
        form_set_error('bill_number', t('Bill number - '.$bill_number.' is attached to Trip ID - '.$trip_id));
        ajax_redirect('cancel-discard-bills');
      }
    }else{
      form_set_error('bill_number', 'Invalid Bill Number.');
      //ajax_redirect('cancel-discard-bills');
    }
  }
}

function bills_filter_form_apply($form, &$form_state){
  $bill_number = $form_state['values']['bill_number'];
  $bill_date_from = $form_state['values']['bill_date_from'];
  $bill_date_to = $form_state['values']['bill_date_to'];
  $bill_rows = array();

  //unset($form_state['input']['bill_number']);
  if(!empty($bill_number)){
    $form_state['bills_data']['bills'] += get_bills_data($bill_number, NULL, NULL);
  }
  else if(!empty($bill_date_from) && !empty($bill_date_to)){
    $form_state['bills_data']['bills'] += get_bills_data(NULL, $bill_date_from, $bill_date_to);
  }
  else{
   form_set_error('bill_number', t('Please select either Bill Number or Date Range.'));
  }
  $form_state['rebuild'] = TRUE;
}

function bills_filter_ajax_form_apply($form, &$form_state){
    $commands[] = ajax_command_replace('#itemlist-div', render($form));
    $commands[] = ajax_command_replace('.alert-danger', '');
    $commands[] = ajax_command_replace('.alert-success', '');
    $commands[] = ajax_command_prepend('#itemlist-div', theme('status_messages'));
    return array('#type' => 'ajax', '#commands' => $commands);
}


function bills_form_submit($form, &$form_state){
  global $user;
  $bill_number = $form_state['values']['bill_number'];
  $bill_date_from = $form_state['values']['bill_date_from'];
  $bill_date_to = $form_state['values']['bill_date_to'];
  $dis_id = change_feeds_import_get_distributor_id($user->uid);
  $bill_number_arr = array();
  $bill_number_str = $action = '';
  foreach($form_state['bills_data']['bills'] as $key => $value) {
    $sales_register_data_nid = $key;
    $bill_number = $value['bill_number']; //$form_state['values']['bill_number'];
    $reason_nid = $form_state['values']['reason'];
    $updated_by_nid = $form_state['values']['updated_by'];
    $remark = $form_state['values']['remark'];
    $action = $form_state['values']['action'];
    array_push($bill_number_arr, $value['bill_number']);
    
    $node_obj = new stdClass();
    $node_obj->type = "cancel_discard_bills";
//    node_object_prepare($node_obj); // Sets some defaults. Invokes hook_prepare() and hook_node_prepare().
//    $node_obj->language = LANGUAGE_NONE; // Or e.g. 'en' if locale is enabled
    $node_obj->uid = $dis_id;
//    $node_obj->status = 1; //(1 or 0): published or not
//    $node_obj->comment = 0;
    $node_obj->title = $bill_number;

    $node_obj->field_sales_register_bill_ref['und'][0]['nid'] = $sales_register_data_nid;
    $node_obj->field_updated_by_resource_master['und'][0]['nid'] = $updated_by_nid;
    $node_obj->field_reasons_ref['und'][0]['nid'] = $reason_nid;   
    $node_obj->field_remark['und'][0]['value'] = $remark;
    $node_obj->field_action['und'][0]['value'] = $action;
    $node_obj->field_cancel_date['und'][0]['value'] = date('Y-m-d H:i:s');

    try {
      node_save($node_obj);
      $success = TRUE;
    }
    catch (Exception $e) {
      drupal_set_message(t('Something went wrong.'), 'error');
    }
    if($success){
      $sales_reg_node = node_load($sales_register_data_nid);
      $sales_reg_node->field_bill_status['und'][0]['value'] = $action;
      node_save($sales_reg_node);
    }
  } //end of foreach()
  if($success){
    $bill_number_str = implode(', ', $bill_number_arr);
    if($action == 'cancel'){
      $action_text = 'Cancelled';
    }else if($action == 'discard'){
      $action_text = 'Discarded';
    }
    drupal_set_message(t($action_text.' successfully for Bill No.- '.$bill_number_str));
  }
  //return $form;
}

function bills_form_validate($form, &$form_state){
  $bill_number = $form_state['values']['bill_number'];
  $bill_date_from = $form_state['values']['bill_date_from'];
  $bill_date_to = $form_state['values']['bill_date_to'];
  
  
  if(empty($bill_number) && (empty($bill_date_from) || empty($bill_date_to))){
    form_set_error('bill_number', t('Please select Bill Number or Date Range.'));
  }
  else if(!empty($bill_number) && (!empty($bill_date_from) || !empty($bill_date_to))){
    form_set_error('bill_number', t('Please select either Bill Number or Date Range.'));
    ajax_redirect('cancel-discard-bills');
  }
  else if(!empty($bill_number)){
    $bill_flag = bill_number_exist($bill_number);
    if(!empty($bill_flag)) {
      $bill_attached_flag = check_bill_not_attached_to_trip($bill_number);
      if(!empty($bill_attached_flag)){
        $trip_id = node_load($bill_attached_flag)->title;
        //drupal_set_message(t('Bill Number - '.$bill_number.' is attached to Trip ID - '.$trip_id), 'error');
        form_set_error('bill_number', t('Bill number - '.$bill_number.' is attached to Trip ID - '.$trip_id));
        //ajax_redirect('cancel-discard-bills');
      }
    }else{
      form_set_error('bill_number', 'Invalid Bill Number.');
      //ajax_redirect('cancel-discard-bills');
    }
  }
  
  //return $form;
}

function bill_date_from_validation($form, &$form_state){
}

function bill_date_to_validation($form, &$form_state){
}
