<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * 
 * @param array $form
 * @param type $form_state
 * @return array
 */
function ptl_loose_confirmation_form($form, &$form_state) {
  $form['#attached']['js'][] = drupal_get_path('module', 'dispatched_quantity_confirmation') . '/js/ptl_loose_confirmation.js';

  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);

  $form['#prefix'] = '<div id="itemlist-div">';
  $form['#suffix'] = '</div>';

  $form['trip_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Trip ID'),
    '#required' => TRUE,
    '#autocomplete_path' => 'get-all-trips/autocomplete',
    '#ajax' => array(
      'callback' => 'ptl_loose_confirmation_callback',
      'wrapper' => 'itemlist-div',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );
  $trip_id = $form_state['input']['trip_number'];
  if (isset($form_state['triggering_element']['#title']) || !empty($form_state['input']['trip_number'])) {

    if ($form_state['triggering_element']['#title'] === 'Trip ID' && $form_state['entered_trip_id'] != $form_state['input']['trip_number']) {
      $form_state['ptl_loose_confirmation']['bills'] = array();
    }
    $form_state['entered_trip_id'] = $form_state['input']['trip_number'];

    $form['trip_type'] = array(
      '#type' => 'radios',
      '#title' => t('Trip Type'),
      '#required' => TRUE,
      '#default_value' => 'PTL',
      '#options' => array('PTL' => 'PTL', 'Non PTL' => 'Non PTL'),
      '#ajax' => array(
        'callback' => 'ptl_loose_confirmation_callback',
        'wrapper' => 'itemlist-div',
        'method' => 'replace',
        'effect' => 'fade',
      ),
    );

    $form['bill_number'] = array(
      '#type' => 'textfield',
      '#title' => t('Bill Number'),
      '#autocomplete_path' => "get-all-bills/autocomplete/$trip_id",
      '#ajax' => array(
        'callback' => 'ptl_loose_confirmation_callback',
        'wrapper' => 'itemlist-div',
        'method' => 'replace',
        'effect' => 'fade',
      ),
    );

    $bill_number = $form_state['input']['bill_number'];

    $form['sku'] = array(
      '#type' => 'textfield',
      '#title' => t('SKU Code'),
      '#autocomplete_path' => "get-all-skus/autocomplete/$bill_number",
    );

    if (!isset($form_state['input']['trip_type']) || $form_state['input']['trip_type'] == 'PTL') {
      $form['non-zero-short-sku'] = array(
        '#type' => 'checkbox',
        '#title' => t('Show only SKUs with PTL Short Qty'),
        '#default_value' => 1,
      );
    }
  }
  $form['apply'] = array(
    '#type' => 'submit',
    '#value' => 'Apply',
    '#ajax' => array(
      'callback' => 'ptl_loose_confirmation_callback',
      'wrapper' => 'itemlist-div',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    '#validate' => array('trip_id_validate'),
    '#submit' => array('ptl_loose_confirmation_submit'),
  );
  if ($form_state['values']['trip_type'] == 'Non PTL') {
    $header = array('Bill Number',
      'SKU Code',
      'Product Name',
      'Original Loose Qty',
      'PPLC Confirmed Qty',
      'PPLC Reason',
      'To Be Dispatched Qty - Loose Qty');
  }
  else {
    $header = array('Bill Number',
      'SKU Code',
      'Product Name',
      'Original Loose Qty',
      'PTL Put Qty',
      'PPLC Confirmed Qty',
      'PPLC Reason',
      'To Be Dispatched Qty - Loose Qty');
  }

  if (!isset($form_state['ptl_loose_confirmation']['bills'])) {
    $form_state['ptl_loose_confirmation']['bills'] = array();
  }
  $options = $form_state['ptl_loose_confirmation']['bills'];

  if (empty($form_state['trip_no_error'])) {
    if (!empty($options)) {
      $form['bill_details'] = array(
        '#theme' => 'table',
        '#tree' => TRUE,
        '#header' => $header,
        '#rows' => array(),
        '#prefix' => '<div class="custom_middle_table">',
        '#suffix' => '</div>',
      );

      $form['save'] = array(
        '#type' => 'submit',
        '#value' => 'Save',
        '#ajax' => array(
          'callback' => 'ptl_loose_confirmation_callback',
          'wrapper' => 'itemlist-div',
          'method' => 'replace',
          'effect' => 'fade',
        ),
        '#validate' => array('dispatched_qty_confirmation_save_validate'),
        '#submit' => array('save_dispatched_qty_confirmation_submit'),
      );

      $form['submit'] = array(
        '#type' => 'submit',
        '#value' => 'Close Trip',
        '#validate' => array('dispatched_qty_confirmation_save_validate'),
        '#submit' => array('dispatched_qty_confirmation_submit'),
      );
    }

    foreach ($options as $key => $val) {
      $key = str_replace(" ", "", $key);
      $form['bill_details']['#rows'][$key][] = array('data' => $val['title'], 'class' => 'bill_num');
      $form['bill_details']['#rows'][$key][] = $val['sku7_code'];
      $form['bill_details']['#rows'][$key][] = $val['product_name'];

      $child_sku = get_child_detials_by_parent_sku($val['sku7_code'], $dis_id);
      if ($form_state['values']['trip_type'] == 'Non PTL') {
        $form['bill_details']['#rows'][$key][] = array('data' => $val['looses'], 'class' => 'looses-qty_' . $key);

        //adding classes to pplc confirm short and dispatch for ptl
        $pplc_class = array("pplc-confirmed-qty");
        $short_reason_class = array('pplc-short-reason');
        $dis_class = array('to-be-dispatched-qty_' . $key);
      }
      else {
        $form['bill_details']['#rows'][$key][] = array('data' => $val['looses'], 'class' => 'looses-qty_' . $key);

        //$form['bill_details']['#rows'][$key][] = array('data' => $val['put_qty'], 'class' =>array('put-qty_' . $val['sku7_code']));
        //put qty input field for calculation based on child sku 
        $put_qty = array(
          '#type' => 'textfield',
          '#attributes' => array('child-sku' => $child_sku['child_sku'], 'child-qty' => $child_sku['child_qty'], 'readonly' => 'readonly', 'class' => array('put_qty', 'put-qty_' . $val['title'] . '-' . $val['sku7_code'])),
          '#size' => 10,
          '#default_value' => $val['put_qty'],
        );
        $form['bill_details'][$key]["put_qty"] = &$put_qty;
        $form['bill_details']['#rows'][$key][]['data']["put_qty"] = &$put_qty;
        unset($put_qty);

        //adding classes to pplc confirm short and dispatch for ptl
        $pplc_class = array("pplc-confirmed-qty-ptl pcc-" . $val['title'] . '-' . $val['sku7_code']);
        $short_reason_class = array('pplc-short-reason-ptl sh-res-' . $val['title'] . '-' . $val['sku7_code']);
        $dis_class = array('to-be-dispatched-qty-ptl_' . $key, "dis-" . $val['title'] . '-' . $val['sku7_code']);
      }
      // Fetch default value from "dispatch_qty_confirmation" table
      $get_qty = get_qty_default_values_pplc($trip_id, $val['title'], $val['sku7_code'], $dis_id);
      $pplc_confirmed_qty = array(
        '#type' => 'textfield',
        '#attributes' => array('bill_sku' => str_replace(" ", "", $val['title']) . '-' . $val['sku7_code'], 'child-sku' => $child_sku['child_sku'], 'child-qty' => $child_sku['child_qty'], 'class' => $pplc_class),
        '#size' => 10,
        '#default_value' => $get_qty['pplc_confirmed_qty'] ? $get_qty['pplc_confirmed_qty'] : ($get_qty['short_reason'] ? 0 : ''),
      );

      $reason_options = array('' => 'Select');
      $dispatch_short_reason_options = get_short_reason_list($dis_id);
      if (!empty($dispatch_short_reason_options)) {
        $reason_options = $reason_options + $dispatch_short_reason_options;
      }
      $short_reason = array(
        '#type' => 'select',
        '#options' => $reason_options,
        '#attributes' => array('child-sku' => $child_sku['child_sku'], 'class' => $short_reason_class),
        '#default_value' => $get_qty['short_reason'],
      );

      $to_be_dispatched_qty = array(
        '#type' => 'textfield',
        '#attributes' => array('class' => $dis_class, 'readonly' => 'readonly'),
        '#size' => 10,
        '#default_value' => $get_qty['dispatch_loose_qty'] ? $get_qty['dispatch_loose_qty'] : ($get_qty['short_reason'] ? 0 : $val['put_qty']),
      );

      $form['bill_details'][$key]["pplc_confirmed_qty"] = &$pplc_confirmed_qty;
      $form['bill_details'][$key]["short_reason"] = &$short_reason;
      $form['bill_details'][$key]["to_be_dispatched_qty"] = &$to_be_dispatched_qty;

      $form['bill_details']['#rows'][$key][]['data']["pplc_confirmed_qty"] = &$pplc_confirmed_qty;
      $form['bill_details']['#rows'][$key][]['data']["short_reason"] = &$short_reason;
      $form['bill_details']['#rows'][$key][]['data']["to_be_dispatched_qty"] = &$to_be_dispatched_qty;

      unset($pplc_confirmed_qty);
      unset($short_reason);
      unset($to_be_dispatched_qty);
    }
  }
  return $form;
}

/**
 * 
 * @param type $form
 * @param type $form_state
 * @return array
 */
function ptl_loose_confirmation_callback($form, &$form_state) {
  //return $form;
  $commands[] = ajax_command_replace('#itemlist-div', render($form));
  $commands[] = ajax_command_replace('.alert-danger', '');
  $commands[] = ajax_command_replace('.alert-success', '');
  $commands[] = ajax_command_prepend('#itemlist-div', theme('status_messages'));
  return array('#type' => 'ajax', '#commands' => $commands);
}

function trip_id_validate($form, &$form_state) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);

  $trip_number = $form_state['values']['trip_number'];
  $trip_type = $form_state['values']['trip_type'];
  $sku_code = $form_state['values']['sku'];
  $trip_content_type = get_trip_type($trip_number, $dis_id);
  $form_state['trip_no_error'] = '';
  if (!empty($trip_number)) {
    
    $bypass_flag = get_loose_cld_flag_trip_level($trip_number, $dis_id);
    if ($bypass_flag['loose'] == 1) {
     drupal_set_message(t("Trip ID $trip_number is already closed."), 'error');
      $form_state['trip_no_error'] = 1;
    }
    
    $query = db_select('node', 'n');
    $query->condition('n.title', $trip_number, '=');
    $query->condition('n.type', 'dispatch_data');
    $query->condition('n.status', 1);
    $query->leftjoin('field_data_field_dispatch_status', 'ds', 'ds.entity_id = n.nid');
    $db_or = db_or();
    $db_or->isNull('ds.field_dispatch_status_value');
    $db_or->condition('ds.field_dispatch_status_value', 0);
    $query->condition($db_or);
    $query->fields('n', array('nid'));
    $query->condition('n.uid', $dis_id);
    $result = $query->execute()->fetchField();
    if (empty($result)) {
      drupal_set_message(t('Please enter valid trip number.'), 'error');
      $form_state['trip_no_error'] = 1;
    }
    else {
      if ($trip_content_type != $trip_type) {
        drupal_set_message(t('Trip id ' . $trip_number . ' is of type ' . $trip_content_type), 'error');
        $form_state['trip_no_error'] = 1;
      }
      
      $query = db_select('ptl_data_configuration', 'ptl');
      $query->condition('ptl.trip_id', $trip_number);
      $query->condition('ptl.distributor_id', $dis_id);
      $query->condition('ptl.api_flag', 1);
      $query->condition('ptl.put_qty_time', 0);
      $query->fields('ptl', array('trip_id'));
      $result = $query->execute()->fetchField();
      if (!empty($result)) {
        drupal_set_message(t('Trip ID '.$trip_number.' is not yet closed on PTL'), 'error');
        $form_state['trip_no_error'] = 1;
      }

      if (!empty($sku_code)) {
        $query = db_select('node', 'n');
        $query->condition('n.type', 'sku7_code_master');
        $query->condition('n.uid', $dis_id);
        $query->condition('n.title', $sku_code);
        $query->join('field_data_field_child_sku', 'csk', 'n.nid = csk.field_child_sku_nid');
        $query->fields('n', array('title'));
        $row = $query->execute()->fetchField();
        if (!empty($row)) {
          drupal_set_message(t("Entered sku $sku_code is child sku"), 'error');
          $form_state['trip_no_error'] = 1;
        }
      }

      //validation for trip present in dispatch but cron is not run
      $query = db_select('dispatch_qty_confirmation', 'dqp');
      $query->fields('dqp', array('trip_id'));
      $query->condition('dqp.distributor_id', $dis_id);
      $query->condition('dqp.trip_id', $trip_number);
      $res = $query->execute()->fetchField();
      if (empty($res)) {
        drupal_set_message(t("Trip ID $trip_number is being processed, please try again in 4 mins"), 'error');
        $form_state['trip_no_error'] = 1;
      }
      //check dp bills in trip
      $dp_bills = check_dp_bills_in_trip($result);
      if (!empty($dp_bills)) {
        drupal_set_message(t("Trip ID $trip_number contains DP Bills"), 'error');
        $form_state['trip_no_error'] = 1;
      }

      $query = db_select('dispatch_qty_confirmation', 'dqc');
      $query->condition('dqc.trip_id', $trip_number, '=');
      $query->condition('dqc.distributor_id', $dis_id, '=');
      $query->fields('dqc', array('loose_close_flag'));
      $result1 = $query->execute()->fetchField();
      if (!empty($result1)) {
        //extra check for auto closed
        $query = db_select('dispatch_qty_confirmation', 'dqp');
        $query->fields('dqp', array('id'));
        $query->condition('dqp.distributor_id', $dis_id);
        $query->condition('dqp.trip_id', $trip_number);
        $query->condition('dqp.original_loose_qty', 0, '>');
        $original_loose_qty = $query->execute()->fetchField();
        if (empty($original_loose_qty)) {
          drupal_set_message(t("Trip ID $trip_number does not have any Loose quantities"), 'error');
        }
        else {
          drupal_set_message(t("Trip ID $trip_number is already closed."), 'error');
        }
        $form_state['trip_no_error'] = 1;
      }
    }
  }
}

function ptl_loose_confirmation_submit($form, &$form_state) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);
  $bill_number = $form_state['values']['bill_number'];
  $sku_code = $form_state['values']['sku'];
  $non_zero_short_sku = $form_state['values']['non-zero-short-sku'];

  if (empty($form_state['trip_no_error'])) {
    $trip_number = $form_state['values']['trip_number'];
    $trip_type = $form_state['values']['trip_type'];
//    $child_sku = get_child_detials_by_parent_sku($sku_code, $dis_id);
//    $final_sku[] = $sku_code;
//    $final_sku[] =  $child_sku['child_sku'];


    if ($trip_type == 'Non PTL') {
      $child_sku = get_child_sku_query_wise($trip_number, $dis_id, $bill_number, $sku_code);

      $query = db_select('dispatch_qty_confirmation', 'dqc');
      $query->condition('dqc.trip_id', $trip_number);
      $query->condition('dqc.distributor_id', $dis_id);
      if (!empty($bill_number)) {
        $query->condition('dqc.bill_number', $bill_number);
      }
      if (!empty($sku_code)) {
        $query->condition('dqc.sku_code', $sku_code);
      }
      if (!empty($child_sku['child_sku'])) {
        $query->condition('dqc.sku_code', $child_sku['child_sku'], 'NOT IN');
      }
      $query->condition('dqc.original_loose_qty', 0, '>');
      $query->addExpression("CONCAT(dqc.bill_number, '-', dqc.sku_code)", "grp_concat");
      $query->addExpression("dqc.bill_number", "title");
      $query->addExpression("dqc.sku_code", "sku7_code");
      $query->addExpression("dqc.product_name", "product_name");
      $query->addExpression("dqc.original_loose_qty", "looses");
      $result = $query->execute()->fetchAll(PDO::FETCH_GROUP | PDO::FETCH_UNIQUE | PDO::FETCH_ASSOC);
    }
    elseif ($trip_type == 'PTL') {
      //show child sku by default without selecting any filter
      $child_sku = get_child_sku_query_wise($trip_number, $dis_id, $bill_number, $sku_code, $non_zero_short_sku);
//      $final_sku[] = $sku_code;
//      $final_sku[] =  $child_sku['child_sku'];
      $query = db_select('dispatch_qty_confirmation', 'dqc');
      $query->leftjoin('ptl_data_configuration', 'pdc', ' dqc.trip_id = pdc.trip_id AND dqc.bill_number = pdc.bill_number AND dqc.sku_code = pdc.sku_code AND pdc.distributor_id = dqc.distributor_id');
      $query->condition('dqc.trip_id', $trip_number);
      $query->condition('dqc.distributor_id', $dis_id);
      if (!empty($bill_number)) {
        $query->condition('dqc.bill_number', $bill_number);
      }
      if (!empty($sku_code)) {
        $query->condition('dqc.sku_code', $sku_code, '=');
      }
      if (!empty($child_sku['child_sku'])) {
        $query->condition('dqc.sku_code', $child_sku['child_sku'], 'NOT IN');
      }
      $query->addExpression("CONCAT(dqc.bill_number, '-', dqc.sku_code)", "grp_concat");
      if (!empty($non_zero_short_sku)) {
        $query->condition('short_put_qty', 0, '>');
//        $db_and1 = db_and();
//        $db_and1->condition('short_put_qty', 0, '>');
//
//        $db_and2 = db_and();
//        if (!empty($bill_number)) {
//          $db_and2->condition('dqc.bill_number',$bill_number);
//        }
//        //append child sku to parent even if child is short.
//        if (!empty($child_sku['child_sku'])) {
//          $db_and2->condition('dqc.sku_code', $child_sku['child_sku']);
//        }
//        
//        $db_or = db_or();
//        $db_or->condition($db_and1);
//        if (!empty($bill_number) || !empty($child_sku['child_sku'])) {
//          $db_or->condition($db_and2);
//        }
//        
//        $query->condition($db_or); 
      }
      $query->condition('dqc.original_loose_qty', 0, '>');
      $query->addExpression('dqc.bill_number', 'title');
      $query->addExpression('dqc.sku_code', 'sku7_code');
      $query->addExpression('dqc.product_name', 'product_name');
      $query->addExpression('pdc.put_qty', 'put_qty');
      $query->addExpression('dqc.original_loose_qty', 'looses');
      $query->orderBy('pdc.sequence_no');
      $result = $query->execute()->fetchAll(PDO::FETCH_GROUP | PDO::FETCH_UNIQUE | PDO::FETCH_ASSOC);
    }

    $form_state['ptl_loose_confirmation']['bills'] = $result;
  }
  $form_state['rebuild'] = TRUE;
}

function dispatched_qty_confirmation_save_validate($form, &$form_state) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);

  $trip_number = $form_state['values']['trip_number'];
  $trip_type = $form_state['values']['trip_type'];
  $trip_content_type = get_trip_type($trip_number, $dis_id);
  if ($trip_content_type != $trip_type) {
    drupal_set_message(t('Trip id ' . $trip_number . ' is of type ' . $trip_content_type), 'error');
    $form_state['rebuild'] = TRUE;
  }

  foreach ($form_state['values']['bill_details'] as $key => $val) {
    $original_loose_qty = $form_state['ptl_loose_confirmation']['bills'][$key]['looses'];
    $to_be_dispatched_qty = $val['to_be_dispatched_qty'];
    if ($to_be_dispatched_qty > $original_loose_qty) {
      drupal_set_message(t("To be dispatched - Loose Qty cannot be greater than Original Loose Qty for: $key."), 'error');
      $form_state['rebuild'] = TRUE;
    }
    if (!empty($val['pplc_confirmed_qty']) || $val['pplc_confirmed_qty'] === '0') {
      if (!is_numeric($val['pplc_confirmed_qty'])) {
        drupal_set_message(t("PPLC confirmed qty should be numeric for: $key."), 'error');
        $form_state['rebuild'] = TRUE;
      }
      $pplc_confirmed_qty = $val['pplc_confirmed_qty'];
      if ($form_state['values']['trip_type'] == 'PTL') {
        $ptl_put_qty = $val['put_qty'];
        // confirmed qty is compared on basis of original loose qty rather then put qty
        if ($pplc_confirmed_qty != $ptl_put_qty) {
          if (empty($val['short_reason'])) {
            drupal_set_message(t("Please Select PPLC Reason for Bill-SKU: $key because PPLC Confirmed Qty is not equal to PTL Put Qty"), 'error');
            $form_state['rebuild'] = TRUE;
          }
        }
        if ($pplc_confirmed_qty > $original_loose_qty) {
          drupal_set_message(t("PPLC confirmed qty cannot be greater than Original loose qty for: $key."), 'error');
          $form_state['rebuild'] = TRUE;
        }
//        if ($pplc_confirmed_qty != $original_loose_qty) {
//          if (empty($val['short_reason'])) {
//            drupal_set_message(t("PPLC Reason should be mandatory for: $key."), 'error');
//            $form_state['rebuild'] = TRUE;
//          }
//        }        
      }
      else if ($form_state['values']['trip_type'] == 'Non PTL') {
        if ($pplc_confirmed_qty != $original_loose_qty) {
          if (empty($val['short_reason'])) {
            drupal_set_message(t("Please select PPLC Reason for Bill-SKU: $key because PPLC Confirmed Qty is not equal to Original Qty."), 'error');
            $form_state['rebuild'] = TRUE;
          }
        }
        if ($pplc_confirmed_qty > $original_loose_qty) {
          drupal_set_message(t("PPLC confirmed qty cannot be greater than original loose qty for: $key."), 'error');
          $form_state['rebuild'] = TRUE;
        }
      }
    }
    else {
      if ($form_state['triggering_element']['#value'] == 'Close Trip' && $form_state['values']['trip_type'] == 'Non PTL') {
        drupal_set_message(t("PPLC confirmed qty is mandatory for: $key."), 'error');
        $form_state['rebuild'] = TRUE;
      }
    }
  }
}

function save_dispatched_qty_confirmation_submit($form, &$form_state) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);
  $trip_id = $form_state['values']['trip_number'];

  if ($form_state['values']['trip_type'] == 'PTL') {
    foreach ($form_state['values']['bill_details'] as $key => $val) {
      $pplc_confirmed_qty = $val['pplc_confirmed_qty'];
      $short_reason = $val['short_reason'];
      $to_be_dispatched_qty = $val['to_be_dispatched_qty'];
      $put_qty = $val['put_qty'];

      $bill_no = $form_state['ptl_loose_confirmation']['bills'][$key]['title'];
      $sku7_code = $form_state['ptl_loose_confirmation']['bills'][$key]['sku7_code'];
      $product_name = $form_state['ptl_loose_confirmation']['bills'][$key]['product_name'];
      //$put_qty = $form_state['ptl_loose_confirmation']['bills'][$key]['put_qty'];
      $looses = $form_state['ptl_loose_confirmation']['bills'][$key]['looses'];
      if ($to_be_dispatched_qty !== '') {
        db_update('dispatch_qty_confirmation')
          ->fields(array(
            'ptl_put_qty' => (!empty($put_qty)) ? $put_qty : 0,
            'pplc_confirmed_qty' => (!empty($pplc_confirmed_qty)) ? $pplc_confirmed_qty : 0,
            'short_reason' => (!empty($pplc_confirmed_qty) || $pplc_confirmed_qty === '0') ? $short_reason : '',
            'dispatch_loose_qty' => (!empty($pplc_confirmed_qty) || ($pplc_confirmed_qty === '0' && !empty($short_reason))) ? $pplc_confirmed_qty : $put_qty,
            'type' => 'PTL',
          ))
          ->condition('trip_id', $trip_id, '=')
          ->condition('bill_number', $bill_no, '=')
          ->condition('sku_code', $sku7_code, '=')
          ->execute();
      }
    }
  }
  else if ($form_state['values']['trip_type'] == 'Non PTL') {
    foreach ($form_state['values']['bill_details'] as $key => $val) {
      $pplc_confirmed_qty = $val['pplc_confirmed_qty'];
      $short_reason = $val['short_reason'];
      $to_be_dispatched_qty = $val['to_be_dispatched_qty'];

      $bill_no = $form_state['ptl_loose_confirmation']['bills'][$key]['title'];
      $sku7_code = $form_state['ptl_loose_confirmation']['bills'][$key]['sku7_code'];
      $product_name = $form_state['ptl_loose_confirmation']['bills'][$key]['product_name'];
      //$put_qty = $form_state['ptl_loose_confirmation']['bills'][$key]['put_qty'];
      $looses = $form_state['ptl_loose_confirmation']['bills'][$key]['looses'];

      if ($to_be_dispatched_qty !== '') {
        db_update('dispatch_qty_confirmation')
          ->fields(array(
            'ptl_put_qty' => 0,
            'pplc_confirmed_qty' => (!empty($pplc_confirmed_qty)) ? $pplc_confirmed_qty : 0,
            'short_reason' => (!empty($pplc_confirmed_qty) || $pplc_confirmed_qty === '0') ? $short_reason : '',
            'dispatch_loose_qty' => (!empty($pplc_confirmed_qty) || $pplc_confirmed_qty === '0') ? $pplc_confirmed_qty : 0,
            'type' => 'Non PTL',
          ))
          ->condition('trip_id', $trip_id, '=')
          ->condition('bill_number', $bill_no, '=')
          ->condition('sku_code', $sku7_code, '=')
          ->execute();
      }
    }
  }

  drupal_set_message(t("Data saved successfully."));
  unset($form_state['ptl_loose_confirmation']['bills']);
  $form_state['rebuild'] = TRUE;
}

function dispatched_qty_confirmation_submit($form, &$form_state) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);
  $trip_id = $form_state['values']['trip_number'];
  $child_sku = get_child_sku_query_wise($trip_id, $dis_id);

  if ($form_state['values']['trip_type'] == 'PTL') {
    $query = db_select('dispatch_qty_confirmation', 'dqc');
    $query->leftjoin('ptl_data_configuration', 'pdc', ' dqc.trip_id = pdc.trip_id AND dqc.bill_number = pdc.bill_number AND dqc.sku_code = pdc.sku_code AND pdc.distributor_id = dqc.distributor_id');
    $query->condition('dqc.trip_id', $trip_id);
    $query->condition('dqc.distributor_id', $dis_id);
    if (!empty($child_sku['child_sku'])) {
      $query->condition('dqc.sku_code', $child_sku['child_sku'], 'NOT IN');
    }
    $query->addExpression("CONCAT(dqc.bill_number, '-', dqc.sku_code)", "grp_concat");
    $query->addExpression("dqc.bill_number", "title");
    $query->addExpression("dqc.sku_code", "sku7_code");
    $query->addExpression("dqc.product_name", "product_name");
    $query->addExpression('pdc.put_qty', 'put_qty');
    $query->addExpression("dqc.original_loose_qty", "looses");
    $query->addExpression("dqc.pplc_confirmed_qty", "pplc_confirmed_qty");
    $query->addExpression("dqc.dispatch_loose_qty", "dispatch_loose_qty");
    $query->addExpression("dqc.short_reason", "short_reason");
    $result = $query->execute()->fetchAll(PDO::FETCH_GROUP | PDO::FETCH_UNIQUE | PDO::FETCH_ASSOC);

    foreach ($result as $key => $val) {
      $bill_no = $val['title'];
      $sku7_code = $val['sku7_code'];
      $product_name = $val['product_name'];
      //$put_qty = $val['put_qty'];
      $looses = $val['looses'];

      $pplc_confirmed_qty = isset($form_state['values']['bill_details'][$key]['pplc_confirmed_qty']) ? $form_state['values']['bill_details'][$key]['pplc_confirmed_qty'] : $val['pplc_confirmed_qty'];
      $short_reason = isset($form_state['values']['bill_details'][$key]['short_reason']) ? $form_state['values']['bill_details'][$key]['short_reason'] : $val['short_reason'];
      $to_be_dispatched_qty = isset($form_state['values']['bill_details'][$key]['to_be_dispatched_qty']) ? $form_state['values']['bill_details'][$key]['to_be_dispatched_qty'] : $val['dispatch_loose_qty'];
      $put_qty = isset($form_state['values']['bill_details'][$key]['put_qty']) ? $form_state['values']['bill_details'][$key]['put_qty'] : $val['put_qty'];

      if ($to_be_dispatched_qty !== '' && !is_null($to_be_dispatched_qty)) {
        //this is required due to value came from query and checked in form_state****
        db_update('dispatch_qty_confirmation')
          ->fields(array(
            'ptl_put_qty' => (!empty($put_qty)) ? $put_qty : 0,
            'pplc_confirmed_qty' => (!empty($pplc_confirmed_qty)) ? $pplc_confirmed_qty : 0,
            'short_reason' => (!empty($pplc_confirmed_qty) || $pplc_confirmed_qty === '0') ? $short_reason : '',
            'dispatch_loose_qty' => (!empty($pplc_confirmed_qty) || ($pplc_confirmed_qty === '0' && !empty($short_reason))) ? $pplc_confirmed_qty : $put_qty,
            'type' => 'PTL',
            //'loose_close_flag' => 1,
          ))
          ->condition('trip_id', $trip_id, '=')
          ->condition('bill_number', $bill_no, '=')
          ->condition('sku_code', $sku7_code, '=')
          ->execute();

//        $child_sku = get_child_detials_by_parent_sku($sku7_code, $dis_id);
//        if (!empty($child_sku['child_sku'])) {
//          $child_ratio = $child_sku['child_qty'];
//
//          db_update('dispatch_qty_confirmation')
//            ->expression('ptl_put_qty', 'ptl_put_qty + :put', array(':put' => $put_qty * $child_ratio))
//            ->expression('pplc_confirmed_qty', 'pplc_confirmed_qty + :con', array(':con' => $pplc_confirmed_qty * $child_ratio))
//            ->expression('dispatch_loose_qty', 'dispatch_loose_qty + :dis', array(':dis' => $to_be_dispatched_qty * $child_ratio))
//            ->fields(array(
//              'loose_close_flag' => 1,
//              'short_reason' => !empty($short_reason) ? $short_reason : '',
//            ))
//            ->condition('trip_id', $trip_id, '=')
//            ->condition('bill_number', $bill_no, '=')
//            ->condition('sku_code', $child_sku['child_sku'], '=')
//            ->execute();
//        }
      }
      /* else{
        //update ptl put qty for non existing form elements
        db_update('dispatch_qty_confirmation')
        ->fields(array(
        'ptl_put_qty' => (!empty($put_qty)) ? $put_qty : 0,
        ))
        ->condition('trip_id', $trip_id, '=')
        ->condition('bill_number', $bill_no, '=')
        ->condition('sku_code', $sku7_code, '=')
        ->execute();

        $child_sku = get_child_detials_by_parent_sku($sku7_code, $dis_id);
        if (!empty($child_sku['child_sku'])) {
        $child_ratio = $child_sku['child_qty'];

        db_update('dispatch_qty_confirmation')
        ->expression('ptl_put_qty', 'ptl_put_qty + :put', array(':put' => $put_qty*$child_ratio))
        ->expression('pplc_confirmed_qty', 'pplc_confirmed_qty + :con', array(':con' => $pplc_confirmed_qty*$child_ratio))
        ->expression('dispatch_loose_qty', 'dispatch_loose_qty + :dis', array(':dis' => $to_be_dispatched_qty*$child_ratio))
        ->fields(array(
        'loose_close_flag' => 1,
        'short_reason' => !empty($short_reason) ? $short_reason : '',
        ))
        ->condition('trip_id', $trip_id, '=')
        ->condition('bill_number', $bill_no, '=')
        ->condition('sku_code', $child_sku['child_sku'], '=')
        ->execute();
        }
        } */
    }
    $validate = check_validation_trip_data_loose($trip_id, $dis_id, 'PTL');
    $bill_check = implode(',', $validate['bill_sku']);
    if ($validate['flag'] == 1) {
      //CHILD
      foreach ($result as $key => $val) {
        $bill_no = $val['title'];
        $sku7_code = $val['sku7_code'];
        $product_name = $val['product_name'];
        //$put_qty = $val['put_qty'];
        $looses = $val['looses'];

        $pplc_confirmed_qty = isset($form_state['values']['bill_details'][$key]['pplc_confirmed_qty']) ? $form_state['values']['bill_details'][$key]['pplc_confirmed_qty'] : $val['pplc_confirmed_qty'];
        $short_reason = isset($form_state['values']['bill_details'][$key]['short_reason']) ? $form_state['values']['bill_details'][$key]['short_reason'] : $val['short_reason'];
        $to_be_dispatched_qty = isset($form_state['values']['bill_details'][$key]['to_be_dispatched_qty']) ? $form_state['values']['bill_details'][$key]['to_be_dispatched_qty'] : $val['dispatch_loose_qty'];
        $put_qty = isset($form_state['values']['bill_details'][$key]['put_qty']) ? $form_state['values']['bill_details'][$key]['put_qty'] : $val['put_qty'];
        $child_to_be_dispatch_qty = (!empty($pplc_confirmed_qty) || ($pplc_confirmed_qty === '0' && !empty($short_reason))) ? $pplc_confirmed_qty : $put_qty;
        if ($to_be_dispatched_qty !== '' && !is_null($to_be_dispatched_qty)) {
          //this is required due to value came from query and checked in form_state****
          $child_sku_exist = get_child_detials_by_parent_sku($sku7_code, $dis_id);
          if (!empty($child_sku_exist['child_sku'])) {
            $child_ratio = $child_sku_exist['child_qty'];

            db_update('dispatch_qty_confirmation')
              ->expression('ptl_put_qty', 'ptl_put_qty + :put', array(':put' => $put_qty * $child_ratio))
              ->expression('pplc_confirmed_qty', 'pplc_confirmed_qty + :con', array(':con' => $pplc_confirmed_qty * $child_ratio))
              ->expression('dispatch_loose_qty', 'dispatch_loose_qty + :dis', array(':dis' => $child_to_be_dispatch_qty * $child_ratio))
              ->fields(array(
                'loose_close_flag' => 1,
                'short_reason' => !empty($short_reason) ? $short_reason : '',
              ))
              ->condition('trip_id', $trip_id, '=')
              ->condition('bill_number', $bill_no, '=')
              ->condition('sku_code', $child_sku_exist['child_sku'], '=')
              ->execute();
          }
        }
      }
      //END CHILD
      //update flag and type for all sku of that trip whose flag is zero and loose > 0 
      if ($child_sku['child_sku']) {
        db_update('dispatch_qty_confirmation')
          ->expression('dispatch_loose_qty', "CASE WHEN dispatch_loose_qty = 0 and short_reason = '' THEN ptl_put_qty ELSE dispatch_loose_qty END")
          ->fields(array(
            'loose_close_flag' => 1,
            'type' => 'PTL',
            'loose_created_date' => REQUEST_TIME
          ))
          ->condition('trip_id', $trip_id, '=')
          ->condition('sku_code', $child_sku['child_sku'], 'NOT IN')//dispatch_loose_qty only for parent sku
          ->execute();


        //Flag update for all
        db_update('dispatch_qty_confirmation')
          ->fields(array(
            'loose_close_flag' => 1,
            'type' => 'PTL',
            'loose_created_date' => REQUEST_TIME
          ))
          ->condition('trip_id', $trip_id, '=')
          ->execute();
      }
      else {
        db_update('dispatch_qty_confirmation')
          ->expression('dispatch_loose_qty', "CASE WHEN dispatch_loose_qty = 0 and short_reason = '' THEN ptl_put_qty ELSE dispatch_loose_qty END")
          ->fields(array(
            'loose_close_flag' => 1,
            'type' => 'PTL',
            'loose_created_date' => REQUEST_TIME
          ))
          ->condition('trip_id', $trip_id, '=')
          ->execute();
      }


      //Sync To be Dispatch Qty
      db_update('dispatch_qty_confirmation') // Table name no longer needs {}
        ->expression('to_be_dispatch_qty', "dispatch_loose_qty + (dispatch_cld_qty * upc) + cld_loose_child_qty")
        ->condition('trip_id', $trip_id, '=')
        ->execute();
      //End
      //update dispatch data
      $nid = get_nid_from_title($trip_id, 'dispatch_data');
      $node = node_load($nid);
      $node->field_loose_confirmation['und'][0]['value'] = 1;
      node_save($node);
    }
    else {
      drupal_set_message(t('Loose short reason should be mandatory for bill-sku ' . $bill_check), 'error');
      $form_state['rebuild'] = TRUE;
    }
  }
  else if ($form_state['values']['trip_type'] == 'Non PTL') {
    $query = db_select('dispatch_qty_confirmation', 'dqc');
    $query->condition('dqc.trip_id', $trip_id);
    $query->condition('dqc.distributor_id', $dis_id);
    if (!empty($child_sku['child_sku'])) {
      $query->condition('dqc.sku_code', $child_sku['child_sku'], 'NOT IN');
    }
    $query->addExpression("CONCAT(dqc.bill_number, '-', dqc.sku_code)", "grp_concat");
    $query->addExpression("dqc.bill_number", "title");
    $query->addExpression("dqc.sku_code", "sku7_code");
    $query->addExpression("dqc.product_name", "product_name");
    $query->addExpression("dqc.original_loose_qty", "looses");
    $query->addExpression("dqc.pplc_confirmed_qty", "pplc_confirmed_qty");
    $query->addExpression("dqc.dispatch_loose_qty", "dispatch_loose_qty");
    $query->addExpression("dqc.short_reason", "short_reason");
    $result = $query->execute()->fetchAll(PDO::FETCH_GROUP | PDO::FETCH_UNIQUE | PDO::FETCH_ASSOC);

    foreach ($result as $key => $val) {
      $bill_no = $val['title'];
      $sku7_code = $val['sku7_code'];
      $product_name = $val['product_name'];
      $looses = $val['looses'];

//      $pplc_confirmed_qty = $form_state['values']['bill_details'][$key]['pplc_confirmed_qty'];
//      $short_reason = $form_state['values']['bill_details'][$key]['short_reason'];
//      $to_be_dispatched_qty = $form_state['values']['bill_details'][$key]['to_be_dispatched_qty'];

      $pplc_confirmed_qty = isset($form_state['values']['bill_details'][$key]['pplc_confirmed_qty']) ? $form_state['values']['bill_details'][$key]['pplc_confirmed_qty'] : $val['pplc_confirmed_qty'];
      $short_reason = isset($form_state['values']['bill_details'][$key]['short_reason']) ? $form_state['values']['bill_details'][$key]['short_reason'] : $val['short_reason'];
      $to_be_dispatched_qty = isset($form_state['values']['bill_details'][$key]['to_be_dispatched_qty']) ? $form_state['values']['bill_details'][$key]['to_be_dispatched_qty'] : $val['dispatch_loose_qty'];

      if ($to_be_dispatched_qty !== '' && !is_null($to_be_dispatched_qty)) {
        //this is required due to value came from query and checked in form_state****
        //if(!is_null($to_be_dispatched_qty)){
        db_update('dispatch_qty_confirmation')
          ->fields(array(
            'ptl_put_qty' => 0,
            'pplc_confirmed_qty' => (!empty($pplc_confirmed_qty)) ? $pplc_confirmed_qty : 0,
            'short_reason' => (!empty($pplc_confirmed_qty) || $pplc_confirmed_qty === '0') ? $short_reason : '',
            'dispatch_loose_qty' => (!empty($pplc_confirmed_qty) || $pplc_confirmed_qty === '0') ? $pplc_confirmed_qty : 0,
            'type' => 'Non PTL',
            //'loose_close_flag' => 1,
          ))
          ->condition('trip_id', $trip_id, '=')
          ->condition('bill_number', $bill_no, '=')
          ->condition('sku_code', $sku7_code, '=')
          ->execute();
        //}
//            $child_sku = get_child_detials_by_parent_sku($sku7_code, $dis_id);
//            if (!empty($child_sku['child_sku'])) {
//                $child_ratio = $child_sku['child_qty'];
//
//                  db_update('dispatch_qty_confirmation')
//                    //->expression('ptl_put_qty', 'ptl_put_qty + :put', array(':put' => $put_qty*$child_ratio))                   
//                    ->expression('pplc_confirmed_qty', 'pplc_confirmed_qty + :con', array(':con' => $pplc_confirmed_qty*$child_ratio))
//                     ->expression('dispatch_loose_qty', 'dispatch_loose_qty + :dis', array(':dis' => $to_be_dispatched_qty*$child_ratio))
//                    ->fields(array(
//                      'loose_close_flag' => 1,
//                      'short_reason' => !empty($short_reason) ? $short_reason : '',
//                    ))
//                    ->condition('trip_id', $trip_id, '=')
//                    ->condition('bill_number', $bill_no, '=')
//                    ->condition('sku_code', $child_sku['child_sku'], '=')
//                    ->execute();
//            }           
      }
    }
    $validate = check_validation_trip_data_loose($trip_id, $dis_id, 'Non PTL');
    $bill_check = implode(',', $validate['bill_sku']);
    //check validation for all skus in a trip
    if ($validate['flag'] == 1) {
      // FOR CHILD
      foreach ($result as $key => $val) {
        $bill_no = $val['title'];
        $sku7_code = $val['sku7_code'];
        $product_name = $val['product_name'];
        $looses = $val['looses'];

        //      $pplc_confirmed_qty = $form_state['values']['bill_details'][$key]['pplc_confirmed_qty'];
        //      $short_reason = $form_state['values']['bill_details'][$key]['short_reason'];
        //      $to_be_dispatched_qty = $form_state['values']['bill_details'][$key]['to_be_dispatched_qty'];

        $pplc_confirmed_qty = isset($form_state['values']['bill_details'][$key]['pplc_confirmed_qty']) ? $form_state['values']['bill_details'][$key]['pplc_confirmed_qty'] : $val['pplc_confirmed_qty'];
        $short_reason = isset($form_state['values']['bill_details'][$key]['short_reason']) ? $form_state['values']['bill_details'][$key]['short_reason'] : $val['short_reason'];
        $to_be_dispatched_qty = isset($form_state['values']['bill_details'][$key]['to_be_dispatched_qty']) ? $form_state['values']['bill_details'][$key]['to_be_dispatched_qty'] : $val['dispatch_loose_qty'];
        $child_to_be_dispatch_qty = (!empty($pplc_confirmed_qty) || $pplc_confirmed_qty === '0') ? $pplc_confirmed_qty : 0;
        
        if ($to_be_dispatched_qty !== '' && !is_null($to_be_dispatched_qty)) {
          $child_sku_exist = get_child_detials_by_parent_sku($sku7_code, $dis_id);
          if (!empty($child_sku_exist['child_sku'])) {
            $child_ratio = $child_sku_exist['child_qty'];

            db_update('dispatch_qty_confirmation')
              //->expression('ptl_put_qty', 'ptl_put_qty + :put', array(':put' => $put_qty*$child_ratio))                   
              ->expression('pplc_confirmed_qty', 'pplc_confirmed_qty + :con', array(':con' => $pplc_confirmed_qty * $child_ratio))
              ->expression('dispatch_loose_qty', 'dispatch_loose_qty + :dis', array(':dis' => $child_to_be_dispatch_qty * $child_ratio))
              ->fields(array(
                'loose_close_flag' => 1,
                'short_reason' => !empty($short_reason) ? $short_reason : '',
              ))
              ->condition('trip_id', $trip_id, '=')
              ->condition('bill_number', $bill_no, '=')
              ->condition('sku_code', $child_sku_exist['child_sku'], '=')
              ->execute();
          }
        }
      }
      // END CHILD
      //update flag and type for all sku of that trip whose flag is zero and loose > 0 
       if ($child_sku['child_sku']) {
        db_update('dispatch_qty_confirmation')
         ->expression('dispatch_loose_qty', "CASE WHEN dispatch_loose_qty = 0 and short_reason = '' THEN original_loose_qty ELSE dispatch_loose_qty END")
         ->condition('trip_id', $trip_id, '=')
         ->condition('sku_code', $child_sku['child_sku'], 'NOT IN')//dispatch_loose_qty only for parent sku
         ->execute();

        //Flag update for all
         db_update('dispatch_qty_confirmation')
         ->fields(array(
           'loose_close_flag' => 1,
           'type' => 'Non PTL',
           'loose_created_date' => REQUEST_TIME
         ))
         ->condition('trip_id', $trip_id, '=')
         ->execute();
        
      }else{
      db_update('dispatch_qty_confirmation')
        ->expression('dispatch_loose_qty', "CASE WHEN dispatch_loose_qty = 0 and short_reason = '' THEN original_loose_qty ELSE dispatch_loose_qty END")
        ->fields(array(
          'loose_close_flag' => 1,
          'type' => 'Non PTL',
          'loose_created_date' => REQUEST_TIME
        ))
        ->condition('trip_id', $trip_id, '=')
        ->execute();
      }
      

      //Sync To be Dispatch Qty
      db_update('dispatch_qty_confirmation') // Table name no longer needs {}
        ->expression('to_be_dispatch_qty', "dispatch_loose_qty + (dispatch_cld_qty * upc) + cld_loose_child_qty")
        ->condition('trip_id', $trip_id, '=')
        ->execute();
      //End
      //update dispatch data
      $nid = get_nid_from_title($trip_id, 'dispatch_data');
      $node = node_load($nid);
      $node->field_loose_confirmation['und'][0]['value'] = 1;
      node_save($node);
    }
    else {
      drupal_set_message(t('Loose short reason should be mandatory for bill-sku ' . $bill_check), 'error');
      $form_state['rebuild'] = TRUE;
    }
  }
  if ($validate['flag'] == 1) {
    drupal_set_message(t("Trip ID $trip_id - Loose Confirmation Completed"));
  }
}

function check_validation_trip_data_loose($trip_id, $dis_id, $type) {
  $child_sku = get_child_sku_query_wise($trip_id, $dis_id);
  $validation_pass['flag'] = 1;
  $query = db_select('dispatch_qty_confirmation', 'dc');
  $query->condition('dc.trip_id', $trip_id, '=');
  $query->condition('dc.distributor_id', $dis_id, '=');
  $query->condition('dc.original_loose_qty', 0, '>');
  if (!empty($child_sku['child_sku'])) {
    $query->condition('dc.sku_code', $child_sku['child_sku'], 'NOT IN');
  }
  $query->fields('dc', array('bill_number', 'sku_code', 'pplc_confirmed_qty', 'short_reason', 'original_loose_qty', 'ptl_put_qty'));
  $result = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
  foreach ($result as $key => $value) {
    $original_loose_qty = $value['original_loose_qty'];
    $pplc_confirmed_qty = $value['pplc_confirmed_qty'];
    $short_reason = $value['short_reason'];
    $ptl_put_qty = $value['ptl_put_qty'];
    if ($type == 'PTL') {
      // check required only for non zero pplc confirm qty, else copy put qty in to be dispatch while submit
      if ($pplc_confirmed_qty != $ptl_put_qty && empty($short_reason) && !empty($pplc_confirmed_qty)) {
        $validation_pass['flag'] = 0;
        $validation_pass['bill_sku'][$key] = $value['bill_number'] . '-' . $value['sku_code'];
      }
    }
    else {
      // check required for all data
      if ($pplc_confirmed_qty != $original_loose_qty && empty($short_reason)) {
        $validation_pass['flag'] = 0;
        $validation_pass['bill_sku'][$key] = $value['bill_number'] . '-' . $value['sku_code'];
      }
    }
  }
  return $validation_pass;
}

function get_child_sku_query_wise($trip_number, $dis_id, $bill_number, $sku_code, $non_zero_short_sku) {

  $all_parent_sku = get_all_parent_skus_query_wise($trip_number, $dis_id, $bill_number, $sku_code, $non_zero_short_sku);
  if (!empty($all_parent_sku)) {
    $result = array();
    $query = db_select('node', 'n');
    $query->condition('n.type', 'sku7_code_master');
    $query->condition('n.uid', $dis_id);
    $query->condition('n.title', $all_parent_sku, 'IN');
    $query->leftjoin('field_data_field_child_sku', 'csk', 'csk.entity_id = n.nid');
    $query->leftjoin('node', 'n1', 'n1.nid = csk.field_child_sku_nid');

    $query->join('field_data_field_sku7_code', 'sc', 'sc.entity_id = n1.nid');
    $query->fields('sc', array('field_sku7_code_value'));

    $result['child_sku'] = $query->execute()->fetchCol();
  }
  return $result;
}

function get_all_parent_skus_query_wise($trip_number, $dis_id, $bill_number, $sku_code, $non_zero_short_sku) {

  $query = db_select('dispatch_qty_confirmation', 'dqc');
  $query->leftjoin('ptl_data_configuration', 'pdc', ' dqc.trip_id = pdc.trip_id AND dqc.bill_number = pdc.bill_number AND dqc.sku_code = pdc.sku_code AND pdc.distributor_id = dqc.distributor_id');
  $query->condition('dqc.trip_id', $trip_number);
  $query->condition('dqc.distributor_id', $dis_id);
  if (!empty($bill_number)) {
    $query->condition('dqc.bill_number', $bill_number);
  }
  if (!empty($sku_code)) {
    $query->condition('dqc.sku_code', $sku_code);
  }
  if (!empty($non_zero_short_sku)) {
    $query->condition('short_put_qty', 0, '>');
  }
  $query->addExpression('dqc.sku_code', 'sku7_code');
  $result = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
  return $result;
}
