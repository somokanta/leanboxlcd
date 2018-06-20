<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * 
 * @param type $form
 * @param type $form_state
 * @return array
 */
function handover_confirmation_delete_form($form, &$form_state) {
  $form['#prefix'] = '<div id="delete-data">';
  $form['#suffix'] = '</div>';

  $form['trip_number'] = array(
    '#prefix' => '<div class="textfield-wrapper">',
    '#type' => 'textfield',
    '#title' => t('Trip'),
    '#required' => TRUE,
    '#autocomplete_path' => 'confirm_trips/autocomplete',
    '#ajax' => array(
      'wrapper' => 'delete-data',
      'callback' => 'confirm_trips_remove_callback',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );

  $form['handover_type'] = array(
    '#type' => 'select',
    '#title' => 'Handover Type:',
    '#required' => TRUE,
    '#prefix' => '<div class="col_2">',
    '#suffix' => '</div></div>',
    '#attributes' => array('class' => array('col-type-dropdown', 'col-lg-6'), 'id' => 'select_col_type'),
    '#options' => array('' => 'Select'),
  );

  if (!empty($form_state['values']['trip_number']) && is_string($form_state['values']['trip_number'])) {
    $form['handover_type']['#options'] = array('' => 'Select') + _get_handover_types_using_trip_number($form_state['values']['trip_number']);
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Show',
    '#name' => 'show',
    '#ajax' => array(
      'callback' => 'confirm_trips_remove_callback',
      'wrapper' => 'delete-data',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    '#submit' => array('handover_confirmation_delete_form_submit'),
  );

  $header = array('Trip ID', 'VRS ID', 'Collection Type', 'Handover To', 'Handover Date', 'Handover Amount', 'Petty Cash Amount', 'Remarks', 'Handover ID');

  if (!isset($form_state['handover_confirmation']['bills'])) {
    $form_state['handover_confirmation']['bills'] = array();
  }
  $options = $form_state['handover_confirmation']['bills'];

  if (!empty($options)) {
    unset($options[0]['id']);
    $trip_number = $options[0]['trip_id'];

    $form['bill_details'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $options,
    );

    $form['submit1'] = array(
      '#type' => 'submit',
      '#value' => t('Confirm Delete'),
      '#submit' => array('handover_delete_submit'),
      '#attributes' => array(
        'onclick' => 'if(!confirm("Do you want to Delete version of Trip ID: ' . $trip_number . '?")){return false;}',
        'style' => array('float: right;')
    ));
  }

  return $form;
}

function confirm_trips_remove_callback($form, &$form_state) {
  return $form;
}

/**
 * 
 * @param array $form
 * @param array $form_state
 * @return array
 */
function handover_confirmation_delete_form_submit($form, &$form_state) {
  $trip_number = $form_state['values']['trip_number'];
  $handover_type = $form_state['values']['handover_type'];

  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);

  $query = db_select('vrs_handover', 'vh');
  $query->condition('vh.distributor_id', $dis_id);
  $query->condition('vh.trip_id', $trip_number, '=');
  $query->condition('vh.collection_type', $handover_type, '=');
  $query->fields('vh', array('id'));
  $query->fields('vh', array('trip_id'));
  $query->fields('vh', array('vrs_id'));
  $query->fields('vh', array('collection_type'));
  $query->fields('vh', array('handover_to'));
  $query->fields('vh', array('handover_date'));
  $query->fields('vh', array('handover_amt'));
  $query->fields('vh', array('petty_cash_amt'));
  $query->fields('vh', array('remarks'));
  if ($handover_type == 'cash') {
    $query->fields('vh', array('handover_id'));
  }
  elseif ($handover_type == 'cheque') {
    $query->fields('vh', array('handover_cheque_id'));
  }
  elseif ($handover_type == 'signed') {
    $query->fields('vh', array('handover_signedbill_id'));
  }
  $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

  if (isset($res[0]['handover_date'])) {
    $res[0]['handover_date'] = date('d-M-y', $res[0]['handover_date']);
  }

  $form_state['handover_confirmation']['bills'] = $res;
  $form_state['rebuild'] = TRUE;
}

/**
 * 
 * @param type $form
 * @param type $form_state
 * @return type
 */
function handover_delete_submit($form, $form_state) {
  global $user;
  $distributor_id = change_feeds_import_get_distributor_id($user->uid);

  $handover_type = $form_state['values']['handover_type'];
  $trip_number = $form_state['values']['trip_number'];
  $trip_arr = explode(",", $trip_number);

  foreach ($trip_arr as $ids) {
    $trip_nid = check_trip_id_exists_or_not($distributor_id, $ids);
    if (!empty($trip_nid->nid)) {
      $node = node_load($trip_nid->nid);

      $count = 0;
      if (!empty($node->field_handover_id['und'][0]['value'])) {
        $count++;
      }
      if (!empty($node->field_handover_cheque_id['und'][0]['value'])) {
        $count++;
      }
      if (!empty($node->field_handover_signedbill_id['und'][0]['value'])) {
        $count++;
      }

      if ($count > 1) {
        // Status updated to 'Handover In Progress'
        $node->field_vrs_status['und'][0]['value'] = 4;
      }
      else {
        // Status updated to 'VRS Confirmed'
        $node->field_vrs_status['und'][0]['value'] = 2;
      }


      if ($handover_type == 'cash') {
        $node->field_handover_id['und'][0]['value'] = '';
      }
      elseif ($handover_type == 'cheque') {
        $node->field_handover_cheque_id['und'][0]['value'] = '';
      }
      elseif ($handover_type == 'signed') {
        $node->field_handover_signedbill_id['und'][0]['value'] = '';
      }

      node_save($node);
    }
  }

  // Delete row from VRS_handover table
  $row_id = $form_state['handover_confirmation']['bills'][0]['id'];
  db_delete('vrs_handover')
      ->condition('id', $row_id)
      ->execute();

  drupal_set_message(t("Handover deleted successfully for Trip ID: $trip_number."));
}

function _get_handover_types_using_trip_number($trip_number) {
  if (!empty($trip_number)) {
    global $user;
    $dis_id = change_feeds_import_get_distributor_id($user->uid);
    $query = db_select('vrs_handover', 'vh');
    $query->condition('vh.distributor_id', $dis_id);
    $query->condition('vh.trip_id', $trip_number, '=');
    $query->fields('vh', array('collection_type'));
    $res = $query->execute()->fetchAllKeyed(0, 0);
  }
  return $res;
}
