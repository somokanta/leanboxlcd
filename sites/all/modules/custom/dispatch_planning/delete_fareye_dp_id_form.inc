<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function delete_fareye_dp_id_form($form, $form_state) {

  $form['#prefix'] = '<div id="delete-data">';
  $form['#suffix'] = '</div>';

  $form['from_dp_id'] = array(
    '#type' => 'textfield',
    '#title' => t('From DP ID'),
    '#size' => 10,
  );

  $form['to_dp_id'] = array(
    '#type' => 'textfield',
    '#title' => t('To DP ID'),
    '#size' => 10,
  );

  $form['runsheet_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Runsheet Number'),
    '#size' => 10,
    '#required' => TRUE,
    '#autocomplete_path' => 'runsheet/autocomplete',
     '#ajax' => array(
      'wrapper' => 'delete-data',
      'callback' => 'delete_dp_id_form_callback',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );

  $form['fareye_submit'] = array(
    '#type' => 'button',
    '#value' => t('Check'),
  );
  if (isset($form_state['input']['runsheet_number']) && !empty($form_state['input']['runsheet_number'])) {
    $from_dp_id = $form_state['values']['from_dp_id'];
    $to_dp_id = $form_state['values']['to_dp_id'];
    $runsheet_number = $form_state['values']['runsheet_number'];
    $data = fareye_form_submit($from_dp_id, $to_dp_id, $runsheet_number);
   
        $header_array = array(
            'id' => 'DP Id',
            'referenceNo' => ' DP referenceNo',
            'bill_number' => 'DP bill_number',
            'runsheet_number' => 'DP runsheet_number',           
            'created_time' => 'Created Time',           
        );
    if (!empty($data['rows'])) {
      $form['list_form_items'] = array(
        '#theme' => 'table',
        '#header' => $header_array,
        '#rows' => $data['rows'],
      );

      $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Confirm Delete'),
         '#attributes' => array( 
          'onclick' => 'if(!confirm("Delete data of runsheet number '.$runsheet_number.'?")){return false;}', 
          'style' => array('float: right;')
          ));
    }
        else {
            drupal_set_message(t("Data not present"), 'error');
        }
  }
  return $form;
}

function delete_fareye_dp_id_form_validate($form, $form_state) {
  $from_dp_id = $form_state['values']['from_dp_id'];
  $to_dp_id = $form_state['values']['to_dp_id'];
  $runsheet_number = $form_state['values']['runsheet_number'];

  if (!empty($from_dp_id) && empty($to_dp_id)) {
    form_set_error('form', 'Please enter To DP ID');
  }
  if (!empty($to_dp_id) && empty($from_dp_id)) {
    form_set_error('form', 'Please enter From DP ID');
  }
}

/**
 * 
 * @param type $form
 * @param type $form_state
 * @return type
 */
function delete_dp_id_form_callback($form, &$form_state) {
  return $form;
}

/**
 * 
 * @global type $user
 * @param type $invoice_number
 */
function fareye_form_submit($from_dp_id, $to_dp_id, $runsheet_number) {
  if (!empty($runsheet_number)) {
    //global $user;
    //$dis_id = change_feeds_import_get_distributor_id($user->uid);
    $distributor_id = _DISTRIBUTOR_ID;
    
    $query = db_select('dispatch_planning_api_data', 'di');
    $query->fields('di',array('id','referenceNo','bill_number','runsheet_number','created_time'));
    $query->condition('di.distributor_id', $distributor_id);
    $query->condition('di.runsheet_number', $runsheet_number);
    if (!empty($from_dp_id) && !empty($to_dp_id)) {
      $query->condition('di.id', array($from_dp_id, $to_dp_id), 'BETWEEN');
    }
    $rows['rows'] = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
    return $rows;
  }
}

function delete_fareye_dp_id_form_submit($form, $form_state) {
  //global $user;
  //$distributor_id = change_feeds_import_get_distributor_id($user->uid);
  $distributor_id = _DISTRIBUTOR_ID;
  
  $from_dp_id = $form_state['values']['from_dp_id'];
  $to_dp_id = $form_state['values']['to_dp_id'];
  $runsheet_number = $form_state['values']['runsheet_number'];

 if (!empty($from_dp_id) && !empty($to_dp_id)&& !empty($runsheet_number)) {
    for ($i = $from_dp_id; $i <= $to_dp_id; $i++) {
      $api_data_deleted = db_delete('dispatch_planning_api_data');
      $api_data_deleted->condition('id', $i);
      $api_data_deleted->condition('distributor_id', $distributor_id);
      $api_data_deleted->condition('runsheet_number', $runsheet_number);
      $api_data_deleted->execute();

      $item_details_deleted = db_delete('dispatch_planning_item_details')
          ->condition('trip_bill_reference', $i)
          ->execute();
      
      $flag_updated = db_update('sms_preprocess')
          ->fields(array(
            'flag' => 0,
          ))
          ->condition('trip_id', $runsheet_number)
          ->condition('distributor_id', $distributor_id)
          ->execute();
    }
  }else if (!empty($runsheet_number) && empty($from_dp_id) && empty($tp_dp_id)) {
    $query = db_select('dispatch_planning_api_data', 'ad');
    $query->fields('ad', array('id'));
    $query->condition('ad.runsheet_number', $runsheet_number);
    $query->condition('ad.distributor_id', $distributor_id);
    $res = $query->execute()->fetchAllKeyed(0, 0);
    if (!empty($res)) {
      $num_deleted = db_delete('dispatch_planning_item_details')
          ->condition('trip_bill_reference', $res, 'IN')
          ->execute();
      
      $flag_updated = db_update('sms_preprocess')
          ->fields(array(
            'flag' => 0,
          ))
          ->condition('trip_id', $runsheet_number)
          ->condition('distributor_id', $distributor_id)
          ->execute();
    }
    $num_deleted = db_delete('dispatch_planning_api_data')
        ->condition('runsheet_number', $runsheet_number)
        ->condition('id', $res)
        ->execute();
  }
  
  drupal_set_message(t("Data deleted successfully"));
}

function runsheet_autocomplete($string = ''){
  $distributor_id = _DISTRIBUTOR_ID;
  $result = array();

  if ($string) {
    $query = db_select('dispatch_planning_api_data', 'ad');
    $query->condition('ad.runsheet_number', db_like($string) . '%', 'LIKE');
    $query->condition('ad.distributor_id', $distributor_id);
    $query->fields('ad', array('runsheet_number'));
    $query->range(0, 15);
    $result = $query->execute()->fetchAllKeyed(0, 0);
  }

  drupal_json_output($result);
}