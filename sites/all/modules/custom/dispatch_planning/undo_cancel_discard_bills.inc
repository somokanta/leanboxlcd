<?php

/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function undo_cancel_discard_bills($form, &$form_state){
  $form = array();

  $form['#prefix'] = '<div id="itemlist-div" class="custom_form_wrapper trip_autocomplete_textbox">';
  $form['#suffix'] = '</div>';

  $form['bill_number'] = array(
    //'#prefix' => '<div class="textfield-wrapper">',
    '#type' => 'textfield',
    '#title' => t('Bill number'),
    //'#required' => TRUE,
    '#autocomplete_path' => 'cancel-discard-bills/autocomplete',
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
    '#submit' => array('bills_filter_form_submit'),
    '#validate' => array('bills_filter_form_validate'),
    '#limit_validation_errors' => array(array('bill_number'), array('bill_date_from'), array('bill_date_to'))
   // '#suffix' => '</div>',
  );
  
  if (!isset($form_state['bills_data']['bills'])) {
    $form_state['bills_data']['bills'] = array();
  }
  $bill_rows = $form_state['bills_data']['bills'];
  
  $header = array(
    'bill_number' => 'Bill Number',
    'reason' => 'Reason',
    'resource_master_name' => 'Updated By',
    'cancel_date' => 'Cancel/Discard Date',
    'remark' => 'Remark',
    'status' => 'Bill Status'
  );
  
  if($bill_rows){
    //dpm($bill_rows, 'bill_rows: ');
    $form['table_items'] = array(
      '#type' => 'tableselect',
      '#header' => $header,
      '#options' => $bill_rows,
      '#prefix' => '<div id="table-div">',
      '#suffix' => '</div>',
      '#empty' => t('No bills found.'),
      '#multiple' => TRUE,
    );

    $form['remove_selected_bills'] = array(
      '#type' => 'submit',
      '#value' => t('Undo Bill Status'),
      '#ajax' => array(
        'wrapper' => 'itemlist-div',
        'callback' => 'custom_ajax_callback',
        'method' => 'replace',
        'effect' => 'fade',
      ),
      '#submit' => array('undo_bill_status_submit'),
      '#limit_validation_errors' => array(array('table_items')),
    );
  }
  
  return $form;
}

function bills_filter_form_submit($form, &$form_state){
  $bill_number = $form_state['values']['bill_number'];
  $bill_date_from = $form_state['values']['bill_date_from'];
  $bill_date_to = $form_state['values']['bill_date_to'];
  $bill_rows = array();

  //unset($form_state['input']['bill_number']);
  if(!empty($bill_number)){
    $form_state['bills_data']['bills'] += get_cancel_discard_bills_data($bill_number, NULL, NULL);
  }
  else if(!empty($bill_date_from) && !empty($bill_date_to)){
    $form_state['bills_data']['bills'] += get_cancel_discard_bills_data(NULL, $bill_date_from, $bill_date_to);
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

function bills_filter_form_validate($form, &$form_state){
  $bill_number = $form_state['values']['bill_number'];
  $bill_date_from = $form_state['values']['bill_date_from'];
  $bill_date_to = $form_state['values']['bill_date_to'];
  
  if(empty($bill_number) && (empty($bill_date_from) || empty($bill_date_to))){
    form_set_error('bill_number', t('Please select Bill Number or Date Range.'));
  }
  else if(!empty($bill_number) && (!empty($bill_date_from) || !empty($bill_date_to))){
    form_set_error('bill_number', t('Please select either Bill Number or Date Range.'));
    //ajax_redirect('undo-cancel-discard-bills');
  }
  else if(!empty($bill_number)){
    $bill_flag = bill_number_exist($bill_number);
    
    if(!empty($bill_flag)) {
      $bill_status_flag = check_bill_cancel_status($bill_number);
      //dpm($bill_status_flag, 'bill_status_flag: ');

      if(empty($bill_status_flag)){
        //form_set_error('bill_number', t('Bill Status of Bill number - '.$bill_number.' is not cancel/discard. Please select only cancel/discard bills'));
        form_set_error('bill_number', t('Please select only Cancelled/Discarded bills'));
        ajax_redirect('undo-cancel-discard-bills');
      }
       
    }else{
      form_set_error('bill_number', 'Invalid Bill Number.');
      //ajax_redirect('undo-cancel-discard-bills');
    }
  }
}

function undo_bill_status_submit($form, &$form_state){
  global $user;
  $bill_number = $form_state['values']['bill_number'];
  $bill_date_from = $form_state['values']['bill_date_from'];
  $bill_date_to = $form_state['values']['bill_date_to'];
  $dis_id = change_feeds_import_get_distributor_id($user->uid);
  $bill_number_arr = array();
  $bill_number_str = '';

  $checked = array_filter($form_state['values']['table_items']);
  if(!empty($checked)){
    foreach ($checked as $key => $value) {
      $bill_number = $form_state['bills_data']['bills'][$value]['bill_number'];
      array_push($bill_number_arr, $bill_number);
      $sales_reg_node_nid = node_load($value)->field_sales_register_bill_ref['und'][0]['nid'];
      
      try{
        node_delete($value);
        unset($form_state['bills_data']['bills'][$value]);
        $form_state['rebuild'] = TRUE;
        $success = TRUE;
      }
      catch (Exception $ex) {
        drupal_set_message(t('Something went wrong.'), 'error');
      }
      
      if($success){
        $sales_reg_node = node_load($sales_reg_node_nid);
        unset($sales_reg_node->field_bill_status['und'][0]['value']);
        node_save($sales_reg_node);
      }
    }  
  }else{
    form_set_error('bill_number', t('Please select Bills to undo.'));
  }
  
  if($success){
    $bill_number_str = implode(', ', $bill_number_arr);
    drupal_set_message(t('Undo Succesfully for Bill No.- '.$bill_number_str));
    ajax_redirect('undo-cancel-discard-bills');
  }
  
}