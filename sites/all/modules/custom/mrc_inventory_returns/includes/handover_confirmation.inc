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
function handover_confirmation_form($form, &$form_state) {
  $form['#attached']['js'][] = drupal_get_path('module', 'mrc_inventory_returns') . '/js/fSelect.js';
  $form['#attached']['css'][] = drupal_get_path('module', 'mrc_inventory_returns') . '/css/fSelect.css';
  $form['#attached']['js'][] = drupal_get_path('module', 'mrc_inventory_returns') . '/js/handover_confirmation.js';

  $form['#prefix'] = '<div id="itemlist-div" class="custom_form_wrapper">';
  $form['#suffix'] = '</div>';

  $form['vrs_ids'] = array(
    '#type' => 'select',
    '#multiple' => TRUE,
    '#title' => 'Select VRS ID:',
    '#prefix' => '<div class="form_row"><div class="col_2">',
    '#suffix' => '</div></div>',
    '#attributes' => array('class' => array('vrs-ids-select-list'), 'multiple' => 'multiple'),
    '#size' => 2,
    '#options' => get_all_vrs_ids(),
  );

  $form['vrs_ids_wrapper']['vrs_ids'] = array(
    '#type' => 'markup',
    '#title' => t('OR'),
    '#prefix' => '<div class="form_row"><div class="col_2"><div id="after_vrs_ids" class="text-center font-weight-bold or_txt"> OR',
    '#suffix' => '</div></div></div>'
  );

  $form['trips'] = array(
    '#type' => 'select',
    '#multiple' => TRUE,
    '#title' => 'Select Trip ID:',
    '#prefix' => '<div class="form_row"><div class="col_2">',
    '#suffix' => '</div></div>',
    '#attributes' => array('class' => array('trip-ids-select-list'), 'multiple' => 'multiple'),
    '#size' => 2,
    '#options' => get_all_trip_ids(),
  );

  $form['trips_wrapper']['trips'] = array(
    '#type' => 'markup',
    '#title' => t('OR'),
    '#prefix' => '<div class="form_row"><div class="col_2"><div id="after_trips" class="text-center font-weight-bold or_txt"> OR',
    '#suffix' => '</div></div></div>'
  );

  $form['vrs_date'] = array(
    '#type' => 'date_popup',
    '#title' => t('VRS Date'),
    '#date_format' => 'Y-m-d',
    '#date_label_position' => '',
    '#date_year_range' => '-20:+2',
    '#prefix' => '<div class="form_row"><div class="col_2">',
    '#suffix' => '</div>',
      //'#attributes' => array('class' => array('vrs-date', 'col-lg-3'), 'id' => 'select_vrs_date'),
  );

  $form['collection_type'] = array(
    '#type' => 'select',
    '#title' => 'Collection Type:',
    '#required' => TRUE,
    '#prefix' => '<div class="col_2">',
    '#suffix' => '</div></div>',
    '#attributes' => array('class' => array('col-type-dropdown', 'col-lg-6'), 'id' => 'select_col_type'),
    '#options' => array('' => 'Select', 'cash' => 'Cash', 'cheque' => 'Cheque', 'signed' => 'Signed'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Show',
    '#name' => 'show',
    '#ajax' => array(
      'callback' => 'handover_confirmation_callback',
      'wrapper' => 'itemlist-div',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    '#submit' => array('handover_confirmation_form_submit'),
  );
  if (!isset($form_state['handover_confirmation']['bills'])) {
    $form_state['handover_confirmation']['bills'] = array();
  }
  $options = $form_state['handover_confirmation']['bills'];

  if (!empty($options[0]['trip_id'])) {
  $header = array(
    'Total Cash Amount Collected' => 'Total Cash Amount Collected',
    'Total Cheques Count' => 'Total Cheques Count',
    'Total Cheque Amount Collected' => 'Total Cheque Amount Collected',
    'Total SB Count' => 'Total SB Count',
    'Total Signed Bill Amount Collected' => 'Total Signed Bill Amount Collected',
    'Handover To' => 'Handover To',
    'Handover Date' => 'Handover Date',
    'Handover Amount' => 'Handover Amount',
    'Petty Cash Amount' => 'Petty Cash Amount',
    'Remarks' => 'Remarks'
  );

  $collection_type = $form_state['values']['collection_type'];

  switch ($collection_type) {
    case 'cash':
      unset($header['Total Cheques Count']);
      unset($header['Total Cheque Amount Collected']);
      unset($header['Total SB Count']);
      unset($header['Total Signed Bill Amount Collected']);
      break;
    case 'cheque':
      unset($header['Total Cash Amount Collected']);
      unset($header['Total SB Count']);
      unset($header['Total Signed Bill Amount Collected']);
      unset($header['Petty Cash Amount']);
      break;
    case 'signed':
      unset($header['Total Cash Amount Collected']);
      unset($header['Total Cheques Count']);
      unset($header['Total Cheque Amount Collected']);
      unset($header['Petty Cash Amount']);
  }

  $form['bill_details'] = array(
    '#theme' => 'table',
    '#tree' => TRUE,
    '#header' => $header,
    '#rows' => array(),
    '#prefix' => '<div class="custom_middle_table">',
    '#suffix' => '</div>',
  );

  foreach ($options as $key => $val) {
    switch ($collection_type) {
      case 'cash':
        $form['bill_details']['#rows'][$key][] = $val['total_actual_cash_amount'];
        break;
      case 'cheque':
        $form['bill_details']['#rows'][$key][] = $val['cheque_count'];
        $form['bill_details']['#rows'][$key][] = $val['total_actual_cheque_amount'];
        break;
      case 'signed':
        $form['bill_details']['#rows'][$key][] = $val['sb_count'];
        $form['bill_details']['#rows'][$key][] = $val['total_actual_signed_bill_amount'];
    }

    $handover_to = array(
      '#id' => 'bill_details-' . $key . '-handover_to',
      '#type' => 'select',
      '#options' => array('' => 'Select') + get_all_handover_names(),
    );

    $handover_date = array(
      '#id' => 'bill_details-' . $key . '-handover_date',
      '#type' => 'date_popup',
      '#date_format' => 'Y-m-d',
      '#date_label_position' => '',
      '#date_year_range' => '-20',
      '#datepicker_options' => array('maxDate' => 0),
    );

    $handover_amount = array(
      '#id' => 'bill_details-' . $key . '-handover_amount',
      '#type' => 'textfield',
      '#size' => 10,
      '#attributes' => array('class' => array('handover-amt'), 'data' => $val['total_actual_cash_amount']),
    );

    if ($collection_type == 'cash') {
      $petty_cash_amount = array(
        '#id' => 'bill_details-' . $key . '-petty_cash_amount',
        '#type' => 'textfield',
        '#size' => 10,
        '#attributes' => array('class' => array('petty-cash-amt')),
      );
    }

    $remarks = array(
      '#id' => 'bill_details-' . $key . '-remarks',
      '#type' => 'textfield',
      '#size' => 10,
    );


    $form['bill_details'][$key]["handover_to"] = &$handover_to;
    $form['bill_details'][$key]["handover_date"] = &$handover_date;
    $form['bill_details'][$key]["handover_amount"] = &$handover_amount;
    if ($collection_type == 'cash') {
      $form['bill_details'][$key]["petty_cash_amount"] = &$petty_cash_amount;
    }
    $form['bill_details'][$key]["remarks"] = &$remarks;


    $form['bill_details']['#rows'][$key][]['data']["handover_to"] = &$handover_to;
    $form['bill_details']['#rows'][$key][]['data']["handover_date"] = &$handover_date;
    $form['bill_details']['#rows'][$key][]['data']["handover_amount"] = &$handover_amount;
    if ($collection_type == 'cash') {
      $form['bill_details']['#rows'][$key][]['data']["petty_cash_amount"] = &$petty_cash_amount;
    }
    $form['bill_details']['#rows'][$key][]['data']["remarks"] = &$remarks;


    unset($handover_to);
    unset($handover_date);
    unset($handover_amount);
    unset($petty_cash_amount);
    unset($remarks);
  }

  $form['confirm_submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
//      '#ajax' => array(
//        'callback' => 'handover_confirmation_callback',
//        'wrapper' => 'itemlist-div',
//        'method' => 'replace',
//        'effect' => 'fade',
//      ),
    '#validate' => array('handover_confirm_validate'),
    '#submit' => array('handover_confirm_submit'),
  );
  }
  else {
    if ($form_state['submit_clicked'] == 1) {
      drupal_set_message(t('No Result Found.'));
    }
  }

  return $form;
}

function handover_confirmation_callback($form, &$form_state) {
  return $form;
}

/**
 * 
 * @param array $form
 * @param array $form_state
 * @return array
 */
function handover_confirmation_form_validate($form, &$form_state) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);

  $vrs_ids = array_filter($form_state['values']['vrs_ids']);
  $trips = array_filter($form_state['values']['trips']);
  $vrs_date = strtotime($form_state['values']['vrs_date']);
  $collection_type = $form_state['values']['collection_type'];

  if (empty($vrs_ids) && empty($trips) && empty($vrs_date)) {
    form_set_error('form', 'Please select either VRS ID or Trip ID or VRS Date.');
  }

  if (!empty($vrs_ids) && !empty($trips) && !empty($vrs_date)) {
    form_set_error('form', 'Please select Only one filter.');
  }
  if (!empty($vrs_ids) && (!empty($trips) || !empty($vrs_date))) {
    $msg = !empty($trips) ? 'Trip ID' : 'VRS Date';
    form_set_error('form', "Please select either VRS ID or $msg.");
  }
  elseif (!empty($trips) && (!empty($vrs_ids) || !empty($vrs_date))) {
    $msg = !empty($vrs_ids) ? 'VRS ID' : 'VRS Date';
    form_set_error('form', "Please select either Trip ID or $msg.");
  }
  elseif (!empty($vrs_date) && (!empty($trips) || !empty($vrs_ids))) {
    $msg = !empty($trips) ? 'Trip ID' : 'VRS ID';
    form_set_error('form', "Please select either VRS Date or $msg.");
  }

  if (!empty($vrs_ids)) {
    $cash_trips_with_handover = array();
    $ch_trips_with_handover = array();
    $sb_trips_with_handover = array();
    foreach ($vrs_ids as $id) {
      $explode_vrs = explode('VRS', $id);
      $trips_ids = $explode_vrs[1];
      $get_handover_id_from_trip_id = get_handover_id_from_trip_id($dis_id, $trips_ids);
      $trip_id_having_handover_id = $get_handover_id_from_trip_id->field_handover_id_value;
      $trip_id_having_cheque_handover_id = $get_handover_id_from_trip_id->field_handover_cheque_id_value;
      $trip_id_having_signed_handover_id = $get_handover_id_from_trip_id->field_handover_signedbill_id_value;
      if ($collection_type == 'cash') {
        if (!empty($trip_id_having_handover_id)) {
          $cash_trips_with_handover[] = $trips_ids;
        }
      }
      elseif ($collection_type == 'cheque') {
        if (!empty($trip_id_having_cheque_handover_id)) {
          $ch_trips_with_handover[] = $trips_ids;
        }
      }
      elseif ($collection_type == 'signed') {
        if (!empty($trip_id_having_signed_handover_id)) {
          $sb_trips_with_handover[] = $trips_ids;
        }
      }
    }

    if (!empty($cash_trips_with_handover)) {
      $ids = implode(',', $cash_trips_with_handover);
      drupal_set_message(t("Cash handover is already done for Trip id: $ids"), 'error');
      $error_flag = 1;
    }
    elseif (!empty($ch_trips_with_handover)) {
      $ids = implode(',', $ch_trips_with_handover);
      drupal_set_message(t("Cheque handover is already done for Trip id: $ids"), 'error');
      $error_flag = 1;
    }
    elseif (!empty($sb_trips_with_handover)) {
      $ids = implode(',', $sb_trips_with_handover);
      drupal_set_message(t("Signed Bill handover is already done for Trip id: $ids"), 'error');
      $error_flag = 1;
    }
  }
  elseif (!empty($trips)) {
    $cash_trips_with_handover = array();
    $ch_trips_with_handover = array();
    $sb_trips_with_handover = array();
    foreach ($trips as $val) {
      $get_handover_id_from_trip_id = get_handover_id_from_trip_id($dis_id, $val);
      $trip_id_having_handover_id = $get_handover_id_from_trip_id->field_handover_id_value;
      $trip_id_having_cheque_handover_id = $get_handover_id_from_trip_id->field_handover_cheque_id_value;
      $trip_id_having_signed_handover_id = $get_handover_id_from_trip_id->field_handover_signedbill_id_value;
      if ($collection_type == 'cash') {
        if (!empty($trip_id_having_handover_id)) {
          $cash_trips_with_handover[] = $val;
        }
      }
      elseif ($collection_type == 'cheque') {
        if (!empty($trip_id_having_cheque_handover_id)) {
          $ch_trips_with_handover[] = $val;
        }
      }
      elseif ($collection_type == 'signed') {
        if (!empty($trip_id_having_signed_handover_id)) {
          $sb_trips_with_handover[] = $val;
        }
      }
    }

    if (!empty($cash_trips_with_handover)) {
      $ids = implode(',', $cash_trips_with_handover);
      drupal_set_message(t("Cash handover is already done for Trip id: $ids"), 'error');
      $error_flag = 1;
    }
    elseif (!empty($ch_trips_with_handover)) {
      $ids = implode(',', $ch_trips_with_handover);
      drupal_set_message(t("Cheque handover is already done for Trip id: $ids"), 'error');
      $error_flag = 1;
    }
    elseif (!empty($sb_trips_with_handover)) {
      $ids = implode(',', $sb_trips_with_handover);
      drupal_set_message(t("Signed Bill handover is already done for Trip id: $ids"), 'error');
      $error_flag = 1;
    }
  }
//  elseif (!empty($vrs_date)) {
//    $get_trip_id_from_vrs_date = get_trip_id_from_vrs_date($dis_id, $vrs_date);
//    foreach ($get_trip_id_from_vrs_date as $val) {
//      $get_trip_id = $val['trip_id'];
//      $get_handover_id_from_trip_id = get_handover_id_from_trip_id($dis_id, $get_trip_id);
//      $trip_id_having_handover_id = $get_handover_id_from_trip_id->field_handover_id_value;
//      $trip_id_having_cheque_handover_id = $get_handover_id_from_trip_id->field_handover_cheque_id_value;
//      $trip_id_having_signed_handover_id = $get_handover_id_from_trip_id->field_handover_signedbill_id_value;
//      if ($collection_type == 'cash') {
//        if (!empty($trip_id_having_handover_id)) {
//          form_set_error('form', "Cash handover is already done for Trip id: $get_trip_id");
//        }
//      }
//      elseif ($collection_type == 'cheque') {
//        if (!empty($trip_id_having_cheque_handover_id)) {
//          form_set_error('form', "Cheque handover is already done for Trip id: $get_trip_id");
//        }
//      }
//      elseif ($collection_type == 'signed') {
//        if (!empty($trip_id_having_signed_handover_id)) {
//          form_set_error('form', "Signed Bill handover is already done for Trip id: $get_trip_id");
//        }
//      }
//    }
//  }

  $form_state['error_flag'] = $error_flag;
}

/**
 * 
 * @param array $form
 * @param array $form_state
 * @return array
 */
function handover_confirmation_form_submit($form, &$form_state) {
  if ($form_state['error_flag'] != 1) {
    global $user;
    $dis_id = change_feeds_import_get_distributor_id($user->uid);

    $vrs_ids = $form_state['values']['vrs_ids'];
    $trips = $form_state['values']['trips'];
    $vrs_date = strtotime($form_state['values']['vrs_date']);
    $collection_type = $form_state['values']['collection_type'];


    $query = db_select('vrs_trip_total', 'vtt');
    $query->condition('vtt.distributor_id', $dis_id);

    if (!empty($vrs_ids)) {
      $query->condition('vtt.vrs_id', $vrs_ids, 'IN');
    }

    if (!empty($trips)) {
      $query->condition('vtt.trip_id', $trips, 'IN');
    }

    if (!empty($vrs_date)) {
      $sub_query1 = db_select('vrs_handover', 'vhh');
      $sub_query1->join('vrs_trip_total', 'vt', 'vt.trip_id = vhh.trip_id');
      $sub_query1->condition('vt.vrs_confirm_date', $vrs_date, '=');
      $sub_query1->condition('vhh.collection_type', $collection_type, '=');
      $sub_query1->fields('vhh', array('trip_id'));
      $res1 = $sub_query1->execute()->fetchCol();

      $query->condition('vtt.vrs_confirm_date', $vrs_date, '=');
      if (!empty($res1)) {
        $query->condition('vtt.trip_id', $res1, 'NOT IN');
      }
    }

    $query->fields('vtt', array('trip_id'));
    $query->addExpression('SUM(vtt.total_actual_cash_amount)', 'total_actual_cash_amount');
    $query->addExpression('SUM(vtt.cheque_count)', 'cheque_count');
    $query->addExpression('SUM(vtt.total_actual_cheque_amount)', 'total_actual_cheque_amount');
    $query->addExpression('SUM(vtt.sb_count)', 'sb_count');
    $query->addExpression('SUM(vtt.total_actual_signed_bill_amount)', 'total_actual_signed_bill_amount');

    $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

    $form_state['handover_confirmation']['bills'] = $res;
    $form_state['submit_clicked'] = 1;
  }
  else {
    unset($form_state['submit_clicked']);
    unset($form_state['handover_confirmation']['bills']);
  }
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit function of handover table
 */
function handover_confirm_submit($form, &$form_state) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);

  // If Trip is selected get trips and vrs ids
  $all_trips = $form_state['values']['trips'];
  if (!empty($all_trips)) {
    // Get all selected trips
    $trips_ids = array();
    $vrs_ids = array();
    foreach ($all_trips as $k => $trips) {
      $trips_ids[] = $trips;
      $vrs_ids[] = 'VRS' . $trips;
    }
    $trip_arr = implode(',', $trips_ids);
    $vrs_arr = implode(',', $vrs_ids);
  }

  // If VRS ID is selected get trips and vrs ids
  $all_vrs_ids = $form_state['values']['vrs_ids'];
  if (!empty($all_vrs_ids)) {
    // Get all selected vrs ids
    $trips_ids = array();
    $vrs_ids = array();
    foreach ($all_vrs_ids as $k => $vrs_id) {
      $explode_vrs = explode('VRS', $vrs_id);

      $trips_ids[] = $explode_vrs[1];
      $vrs_ids[] = $vrs_id;
    }
    $trip_arr = implode(',', $trips_ids);
    $vrs_arr = implode(',', $vrs_ids);
  }

  // If VRS Date is selected get trips and vrs ids
  $vrs_date = strtotime($form_state['values']['vrs_date']);
  if (!empty($vrs_date)) {
    $get_trip_id_from_vrs_date = get_trip_id_from_vrs_date($dis_id, $vrs_date);

    $trips_ids = array();
    $vrs_ids = array();
    foreach ($get_trip_id_from_vrs_date as $val) {
      $trips_ids[] = $val['trip_id'];
      $vrs_ids[] = 'VRS' . $val['trip_id'];
    }
    $trip_arr = implode(',', $trips_ids);
    $vrs_arr = implode(',', $vrs_ids);
  }

  $collection_type = $form_state['values']['collection_type'];
  switch ($collection_type) {
    case 'cash':
      // Get Handover ID
      $get_cash_handover_id = get_handover_id('cash', $dis_id);
      if (!empty($get_cash_handover_id)) {
        $handover_id = explode("_", $get_cash_handover_id);
        $id = $handover_id[1];
        if (!empty($id)) {
          $incremented_id = ++$id;
          $final_cash_handover_id = "CS" . date('dmY') . '_' . $incremented_id;
        }
        else {
          $final_cash_handover_id = "CS" . date('dmY') . '_1';
        }
      }
      else {
        $final_cash_handover_id = "CS" . date('dmY');
      }
      break;

    case 'cheque':
      // Get Handover ID
      $get_cheque_handover_id = get_handover_id('cheque', $dis_id);
      if (!empty($get_cheque_handover_id)) {
        $handover_id = explode("_", $get_cheque_handover_id);
        $id = $handover_id[1];
        if (!empty($id)) {
          $incremented_id = ++$id;
          $final_cheque_handover_id = "CQ" . date('dmY') . '_' . $incremented_id;
        }
        else {
          $final_cheque_handover_id = "CQ" . date('dmY') . '_1';
        }
      }
      else {
        $final_cheque_handover_id = "CQ" . date('dmY');
      }
      break;

    case 'signed':
      // Get Handover ID
      $get_signed_handover_id = get_handover_id('signed', $dis_id);
      if (!empty($get_signed_handover_id)) {
        $handover_id = explode("_", $get_signed_handover_id);
        $id = $handover_id[1];
        if (!empty($id)) {
          $incremented_id = ++$id;
          $final_signedbill_handover_id = "SB" . date('dmY') . '_' . $incremented_id;
        }
        else {
          $final_signedbill_handover_id = "SB" . date('dmY') . '_1';
        }
      }
      else {
        $final_signedbill_handover_id = "SB" . date('dmY');
      }
  }

  // Insert the handover details in "handover table"
  foreach ($form_state['values']['bill_details'] as $key => $value) {
    $vrs_handover = db_insert('vrs_handover') // Table name no longer needs {}
        ->fields(array(
          'trip_id' => $trip_arr,
          'vrs_id' => $vrs_arr,
          'collection_type' => $collection_type,
          'total_cash_amt' => ($value["handover_amount"] + $value["petty_cash_amount"]),
          'total_cheque_count' => $value["cheque_count"] ? $value["cheque_count"] : 0,
          'cheque_amt' => ($value["handover_amount"] + $value["petty_cash_amount"]),
          'signed_bills_count' => $value["sb_count"] ? $value["sb_count"] : 0,
          'signed_bills_amt' => ($value["handover_amount"] + $value["petty_cash_amount"]),
          'handover_to' => $value["handover_to"] ? $value["handover_to"] : '',
          'handover_date' => strtotime($value["handover_date"]) ? strtotime($value["handover_date"]) : 0,
          'handover_amt' => $value["handover_amount"] ? $value["handover_amount"] : 0,
          'petty_cash_amt' => $value["petty_cash_amount"] ? $value["petty_cash_amount"] : 0,
          'remarks' => $value["remarks"] ? $value["remarks"] : '',
          'handover_id' => ($collection_type == 'cash') ? $final_cash_handover_id : '',
          'handover_cheque_id' => ($collection_type == 'cheque') ? $final_cheque_handover_id : '',
          'handover_signedbill_id' => ($collection_type == 'signed') ? $final_signedbill_handover_id : '',
          'distributor_id' => $dis_id,
          'created_date' => strtotime(date('d-m-Y')),
        ))
        ->execute();

    // Update handover ID at the trip level in "Dispatch Data"
    foreach ($trips_ids as $t_ids) {
      $trip_nid = check_trip_id_exists_or_not($dis_id, $t_ids);
      $node = node_load($trip_nid->nid);
      if ($collection_type == 'cash') {
        $node->field_handover_id['und'][0]['value'] = $final_cash_handover_id;
        //Update Cash Version No
        $cash_version = $node->field_handover_cash_version['und'][0]['value'];
        if (empty($cash_version)) {
          $cash_version_no = 'V_1.0';
          $node->field_handover_cash_version['und'][0]['value'] = $cash_version_no;
        }
        else {
          $exploded_val = explode('_', $cash_version);
          $updated_version = $exploded_val[1] + 0.1;
          $cash_version_no = "V_$updated_version";
          $node->field_handover_cash_version['und'][0]['value'] = $cash_version_no;
        }
      }
      if ($collection_type == 'cheque') {
        $node->field_handover_cheque_id['und'][0]['value'] = $final_cheque_handover_id;
        //Update Cheque Version No
        $cheque_version = $node->field_handover_cheque_version['und'][0]['value'];
        if (empty($cheque_version)) {
          $cheque_version_no = 'V_1.0';
          $node->field_handover_cheque_version['und'][0]['value'] = $cheque_version_no;
        }
        else {
          $exploded_val = explode('_', $cheque_version);
          $updated_version = $exploded_val[1] + 0.1;
          $cheque_version_no = "V_$updated_version";
          $node->field_handover_cheque_version['und'][0]['value'] = $cheque_version_no;
        }
      }
      if ($collection_type == 'signed') {
        $node->field_handover_signedbill_id['und'][0]['value'] = $final_signedbill_handover_id;
        //Update Signed Version No
        $signed_version = $node->field_handover_signed_version['und'][0]['value'];
        if (empty($signed_version)) {
          $signed_version_no = 'V_1.0';
          $node->field_handover_signed_version['und'][0]['value'] = $signed_version_no;
        }
        else {
          $exploded_val = explode('_', $signed_version);
          $updated_version = $exploded_val[1] + 0.1;
          $signed_version_no = "V_$updated_version";
          $node->field_handover_signed_version['und'][0]['value'] = $signed_version_no;
        }
      }
      
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

      if ($count <= 2) {
        $node->field_vrs_status['und'][0]['value'] = 4;
      }
      else {
        $node->field_vrs_status['und'][0]['value'] = 5;
      }

      node_save($node);
    }

    // Create history for VRS Handover
    $vrs_handover_history = db_insert('vrs_handover_history') // Table name no longer needs {}
        ->fields(array(
          'trip_id' => $trip_arr,
          'vrs_id' => $vrs_arr,
          'collection_type' => $collection_type,
          'total_cash_amt' => ($value["handover_amount"] + $value["petty_cash_amount"]),
          'total_cheque_count' => $value["cheque_count"] ? $value["cheque_count"] : 0,
          'cheque_amt' => ($value["handover_amount"] + $value["petty_cash_amount"]),
          'signed_bills_count' => $value["sb_count"] ? $value["sb_count"] : 0,
          'signed_bills_amt' => ($value["handover_amount"] + $value["petty_cash_amount"]),
          'handover_to' => $value["handover_to"] ? $value["handover_to"] : '',
          'handover_date' => strtotime($value["handover_date"]) ? strtotime($value["handover_date"]) : 0,
          'handover_amt' => $value["handover_amount"] ? $value["handover_amount"] : 0,
          'petty_cash_amt' => $value["petty_cash_amount"] ? $value["petty_cash_amount"] : 0,
          'remarks' => $value["remarks"] ? $value["remarks"] : '',
          'handover_id' => ($collection_type == 'cash') ? $final_cash_handover_id : '',
          'handover_cheque_id' => ($collection_type == 'cheque') ? $final_cheque_handover_id : '',
          'handover_signedbill_id' => ($collection_type == 'signed') ? $final_signedbill_handover_id : '',
          'distributor_id' => $dis_id,
          'created_date' => strtotime(date('d-m-Y')),
          'cash_version_no' => ($collection_type == 'cash') ? $cash_version_no : '',
          'cheque_version_no' => ($collection_type == 'cheque') ? $cheque_version_no : '',
          'signed_version_no' => ($collection_type == 'signed') ? $signed_version_no : '',
        ))
        ->execute();
    
  }

  drupal_set_message(t("Handover submitted successfully.Handover ID is $final_cash_handover_id$final_cheque_handover_id$final_signedbill_handover_id."));
  unset($form_state['values']['bill_details']);
}

// Get handover ID from VRS_handover table for today's date
function get_handover_id($type, $dis_id) {
  $today_date = strtotime(date('d-m-Y'));

  $query = db_select('vrs_handover', 'vh');
  $query->condition('vh.distributor_id', $dis_id, '=');
  $query->condition('vh.created_date', $today_date, '=');
  if ($type == 'cash') {
    $query->fields('vh', array('handover_id'));
  }
  elseif ($type == 'cheque') {
    $query->fields('vh', array('handover_cheque_id'));
  }
  elseif ($type == 'signed') {
    $query->fields('vh', array('handover_signedbill_id'));
  }
  $query->orderBy('id', 'DESC');
  $res = $query->execute()->fetchAll();

  foreach ($res as $ids) {
    if (!empty($ids->handover_id)) {
      $handover_id = $ids->handover_id;
      break;
    }
    elseif (!empty($ids->handover_cheque_id)) {
      $handover_id = $ids->handover_cheque_id;
      break;
    }
    elseif (!empty($ids->handover_signedbill_id)) {
      $handover_id = $ids->handover_signedbill_id;
      break;
    }
    else {
      $handover_id = '';
    }
  }

  return $handover_id;
}

/**
 * 
 * @param array $form
 * @param array $form_state
 * @return array
 */
function handover_confirm_validate($form, &$form_state) {
  $coll_type = $form_state['values']['collection_type'];
  if ($coll_type == 'cash') {
    $total_cash_amt_collected = $form_state['handover_confirmation']['bills'][0]['total_actual_cash_amount'];
    $handover_amt = $form_state['values']['bill_details'][0]['handover_amount'];
    $petty_cash_amt = $form_state['values']['bill_details'][0]['petty_cash_amount'];
    if (!empty($handover_amt)) {
      // check for decimal in digit
      if (!preg_match('/^[\d]+$/', $handover_amt)) {
        form_set_error('form', 'Handover Amount should not be in decimal.');
        $form_state['rebuild'] = TRUE;
      }
      $total_amt = $handover_amt + $petty_cash_amt;
      if ($total_cash_amt_collected != $total_amt) {
        form_set_error('form', 'Sum of handover amount and petty cash amount should be equal to Total Cash Amount Collected.');
        $form_state['rebuild'] = TRUE;
      }
    }
    else {
      form_set_error('form', 'Please enter Handover Amount.');
      $form_state['rebuild'] = TRUE;
    }

    if (!empty($petty_cash_amt)) {
      if (!preg_match('/^[\d]+$/', $petty_cash_amt)) {
        form_set_error('form', 'Petty Cash Amount should not be in decimal.');
        $form_state['rebuild'] = TRUE;
      }
    }
  }
  elseif ($coll_type == 'cheque') {
    $total_cheque_amount_collected = $form_state['handover_confirmation']['bills'][0]['total_actual_cheque_amount'];
    $handover_amt = $form_state['values']['bill_details'][0]['handover_amount'];

    if (!empty($handover_amt)) {
      if (!preg_match('/^[\d]+$/', $handover_amt)) {
        form_set_error('form', 'Handover Amount should not be in decimal.');
        $form_state['rebuild'] = TRUE;
      }
      else {
        if ($total_cheque_amount_collected != $handover_amt) {
          form_set_error('form', 'Handover amount should be equal to Total Cheque Amount Collected.');
          $form_state['rebuild'] = TRUE;
        }
      }
    }
    else {
      form_set_error('form', 'Please enter Handover Amount.');
      $form_state['rebuild'] = TRUE;
    }
  }
  elseif ($coll_type == 'signed') {
    $total_signed_amount_collected = $form_state['handover_confirmation']['bills'][0]['total_actual_signed_bill_amount'];
    $handover_amt = $form_state['values']['bill_details'][0]['handover_amount'];

    if (!empty($handover_amt)) {
      if (!preg_match('/^[\d]+$/', $handover_amt)) {
        form_set_error('form', 'Handover Amount should not be in decimal.');
        $form_state['rebuild'] = TRUE;
      }
      else {
        if ($total_signed_amount_collected != $handover_amt) {
          form_set_error('form', 'Handover amount should be equal to Total Signed Amount Collected.');
          $form_state['rebuild'] = TRUE;
        }
      }
    }
    else {
      form_set_error('form', 'Please enter Handover Amount.');
      $form_state['rebuild'] = TRUE;
    }
  }
}

function get_trip_id_from_vrs_date($dis_id, $vrs_date) {
  $query = db_select('vrs_trip_total', 'vtt');
  $query->condition('vtt.distributor_id', $dis_id);
  $query->condition('vtt.vrs_confirm_date', $vrs_date, '=');
  $query->fields('vtt', array('trip_id'));
  $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

  return $res;
}

function get_handover_id_from_trip_id($dis_id, $trip_id) {
  $query = db_select('node', 'n');
  $query->condition('n.type', 'dispatch_data');
  $query->condition('n.uid', $dis_id);
  $query->condition('n.title', $trip_id);
  $query->leftjoin('field_data_field_handover_id', 'hid', 'hid.entity_id = n.nid');
  $query->leftjoin('field_data_field_handover_cheque_id', 'hcid', 'hcid.entity_id = n.nid');
  $query->leftjoin('field_data_field_handover_signedbill_id', 'hsid', 'hsid.entity_id = n.nid');
  $query->fields('hid', array('field_handover_id_value'));
  $query->fields('hcid', array('field_handover_cheque_id_value'));
  $query->fields('hsid', array('field_handover_signedbill_id_value'));
  $results = $query->execute()->fetchAll();

  return $results[0];
}
