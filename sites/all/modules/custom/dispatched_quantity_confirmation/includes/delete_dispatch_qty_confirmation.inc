<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function delete_dispatch_qty_confirmation_form($form, &$form_state) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);

  $form['#prefix'] = '<div id="itemlist-div">';
  $form['#suffix'] = '</div>';

  $form['trip_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Trip ID'),
    '#required' => TRUE,
  );
  $trip_number = $form_state['input']['trip_number'];

  $form['all'] = array(
    '#type' => 'checkbox',
    '#title' => t('All'),
  );
  $form['loose'] = array(
    '#type' => 'checkbox',
    '#title' => t('Loose'),
  );
  $form['cld'] = array(
    '#type' => 'checkbox',
    '#title' => t('CLD'),
  );

  $form['apply'] = array(
    '#type' => 'submit',
    '#value' => 'Apply Changes',
    '#attributes' => array(
      'onclick' => 'if(!confirm("Are you sure you want to confirm ?")){return false;}',
    ),
    '#submit' => array('delete_dispatch_qty_confirmation_submit'),
  );

  return $form;
}

function delete_dispatch_qty_confirmation_submit($form, &$form_state) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);
  $trip_number = $form_state['values']['trip_number'];
  $all = $form_state['values']['all'];
  $loose = $form_state['values']['loose'];
  $cld = $form_state['values']['cld'];
  
  if ($all == 1) {
    //reset updated flag and other in custom billing data.
    reset_updated_flag_custom_billing($trip_number, $dis_id);
    
    $version_type = 'All';
    create_version_dispatch_qty_conf($trip_number, $dis_id,$version_type);    
    db_update('dispatch_qty_preprocess')
      ->fields(array(
        'flag' => 0,
      ))
      ->condition('trip_id', $trip_number, '=')
      ->condition('distributor_id', $dis_id, '=')
      ->execute();

    db_delete('dispatch_qty_confirmation')
      ->condition('trip_id', $trip_number, '=')
      ->condition('distributor_id', $dis_id, '=')
      ->execute();

    $nid = get_nid_from_title($trip_number, 'dispatch_data');
    $node = node_load($nid);
    $node->field_loose_confirmation['und'][0]['value'] = 0;
    $node->field_cld_confirmation['und'][0]['value'] = 0;
    node_save($node);

    drupal_set_message(t('Trip id ' . $trip_number . ' is successfully deleted.'));
  }
  if ($loose == 1) {
    //reset updated flag and other in custom billing data.
    reset_updated_flag_custom_billing($trip_number, $dis_id);
    
    $version_type = 'Loose';
    create_version_dispatch_qty_conf($trip_number, $dis_id, $version_type);
    
    db_update('dispatch_qty_confirmation')
      ->fields(array(
        'loose_close_flag' => 0,
        //'type' => '',
        'loose_created_date' => 0,
        'ptl_put_qty' => 0,
        'short_reason' => '',
        'dispatch_loose_qty' => 0,
        'pplc_confirmed_qty' => 0,
        'masterptl_short_qty' => 0,
        'to_be_dispatch_qty' => 0,
      ))
      ->condition('trip_id', $trip_number, '=')
      //->condition('original_loose_qty', 0, '>')
      ->condition('distributor_id', $dis_id, '=')
      ->execute();
    
    //Sync To be Dispatch Qty
    db_update('dispatch_qty_confirmation') // Table name no longer needs {}
      ->expression('to_be_dispatch_qty', "dispatch_loose_qty + (dispatch_cld_qty * upc) + cld_loose_child_qty")
      ->condition('trip_id', $trip_number, '=')
      ->execute();
    //End
    $nid = get_nid_from_title($trip_number, 'dispatch_data');
    $node = node_load($nid);
    $node->field_loose_confirmation['und'][0]['value'] = 0;
    node_save($node);

    drupal_set_message(t('Trip id ' . $trip_number . ' loose changes are reset'));
  }
  if ($cld == 1) {
    //reset updated flag and other in custom billing data.
    reset_updated_flag_custom_billing($trip_number, $dis_id);
    
    $version_type = 'CLD'; 
    create_version_dispatch_qty_conf($trip_number, $dis_id, $version_type);   
    
    db_update('dispatch_qty_confirmation')
      ->fields(array(
        'cld_close_flag' => 0,
        //'type' => '',
        'cld_short_reason' => '',
        'dispatch_cld_qty' => 0,
        'cld_confirmed_qty' => 0,
        'cld_retail_confirmed_flag' => 0,
        'cld_loose_child_qty' => 0,
      ))
      ->condition('trip_id', $trip_number, '=')
      //->condition('original_cld_qty', 0, '>')
      ->condition('distributor_id', $dis_id, '=')
      ->execute();
    
    //Sync To be Dispatch Qty
    db_update('dispatch_qty_confirmation') // Table name no longer needs {}
      ->expression('to_be_dispatch_qty', "dispatch_loose_qty + (dispatch_cld_qty * upc) + cld_loose_child_qty")
      ->condition('trip_id', $trip_number, '=')
      ->execute();
    //End

    $nid = get_nid_from_title($trip_number, 'dispatch_data');
    $node = node_load($nid);
    $node->field_cld_confirmation['und'][0]['value'] = 0;
    node_save($node);

    drupal_set_message(t('Trip id ' . $trip_number . ' cld changes are reset'));
  }
}

function reset_updated_flag_custom_billing($trip_number, $dis_id){
    $query = db_select('dispatch_qty_confirmation', 'dqc');
    $query->condition('dqc.trip_id', $trip_number);
    $query->condition('dqc.distributor_id', $dis_id);
    $query->fields('dqc', array('bill_number'));
    $bills = $query->distinct()->execute()->fetchCol();
    
    if (!empty($bills)) {
      db_update('custom_billing_data') // Table name no longer needs {}
      ->expression('Sum_of_TOTAL_SALES_QTY', 'original_Sum_of_TOTAL_SALES_QTY')
      ->fields(array(
        'updated_flag' => NULL,
        'updated_date' => NULL,
      ))  
      ->condition('bill_number', $bills, 'IN')
      ->condition('distributor_id', $dis_id, '=')
      ->execute();
          
      //original bill value to bill value in sales register
      foreach ($bills as $val) {
        $nid = get_nid_from_title($val, 'sales_register_data');
        $node = node_load($nid);
        $node->field_bill_value['und'][0]['value'] = $node->field_original_bill_value['und'][0]['value'];
        node_save($node);       
      }
    }    
}