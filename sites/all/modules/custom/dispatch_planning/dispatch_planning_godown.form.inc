<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function _godown_callback_form($form, &$form_state) {
  $form = array();
  $form['#prefix'] = '<div id="itemlist-div">';
  $form['#suffix'] = '</div>';
  $form['#attached']['js'][] = drupal_get_path('module', 'dispatch_planning') . '/js/dispatch_planning_farey.js';

  // get logged in users rs id
  $user_id = $GLOBALS['user']->uid;
  $rs_code = dispatch_planning_get_rscode_using_uid($user_id);
  $distributor_id = change_feeds_import_get_distributor_id($user_id);

  $form['trip_number'] = array(
    '#prefix' => '<div class="textfield-wrapper">',
    '#type' => 'textfield',
    '#title' => t('Trip'),
    '#required' => TRUE,
    '#autocomplete_path' => 'trip/autocomplete',
    '#ajax' => array(
      'wrapper' => 'itemlist-div',
      'callback' => 'trip_details_remove_godown_callback_trip',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );


  $trip_number = $form_state['input']['trip_number'];
  if (isset($form_state['triggering_element']['#title']) && ($form_state['triggering_element']['#title'] == 'Bill Number') && !empty($form_state['input']['bill_number'])) {
    unset($form_state['storage']['trip_id']);
  }
  if (isset($form_state['storage']['trip_id']) && !empty($form_state['storage']['trip_id'])) {
    $trip_no = $form_state['storage']['trip_id'];
    //$options = _get_all_tripsbill_godown($trip_number);
    //if (!empty($options)) {
    unset($form_state['input']['bill_number']);
    $form['bill_number'] = array(
      '#prefix' => '<div class="textfield-wrapper">',
      '#type' => 'textfield',
      '#title' => t('Bill Number'),
      // '#options' => $options,
      '#required' => TRUE,
      '#autocomplete_path' => "sales-register-bills/autocomplete/$trip_no",
      '#ajax' => array(
        'wrapper' => 'itemlist-div',
        'callback' => 'trip_details_remove_godown_callback',
        'method' => 'replace',
        'effect' => 'fade',
      ),
    );
  }
  else {
    if (!empty($trip_number) && !empty($rs_code)) {
      //$options = _get_all_tripsbill($trip_number);
      //if (!empty($options)) {
    
    // check trip if it is Returns Confirmed
    // check trip if it is VRS Confirmed
    $trip_detail = check_trip_id_exists_or_not($distributor_id, $trip_number);
    $vrs_status = $trip_detail->field_vrs_status_value;

    if($vrs_status == 1) {
       drupal_set_message(t('You cannot edit because this trip is returns confirmed.'), 'error');
    }
    else if($vrs_status == 2) {
      drupal_set_message(t('You cannot edit because this trip is VRS confirmed.'), 'error');
    }
    else if($vrs_status == 3) {
      drupal_set_message(t('You cannot edit because this trip is VRS pending.'), 'error');
    }
    else if($vrs_status == 4) {
      drupal_set_message(t('You cannot edit because this trip is Handover pending.'), 'error');
    }
    else if($vrs_status == 5) {
      drupal_set_message(t('You cannot edit because this trip is Handover confirmed.'), 'error');
    }
    else {

      if (isset($form_state['triggering_element']['#title']) && ($form_state['triggering_element']['#title'] == 'Trip') && !empty($form_state['input']['trip_number'])) {
        unset($form_state['input']['bill_number']);
        unset($form_state['trip_change']);
      }
      $form['bill_number'] = array(
        '#prefix' => '<div class="textfield-wrapper">',
        '#type' => 'textfield',
        '#title' => t('Bill Number'),
        // '#options' => $options,
        '#required' => TRUE,
        '#autocomplete_path' => "sales-register-bills/autocomplete/$trip_number",
        '#ajax' => array(
          'wrapper' => 'itemlist-div',
          'callback' => 'trip_details_remove_godown_callback',
          'method' => 'replace',
          'effect' => 'fade',
        ),
      );
      $bill_number = $form_state['input']['bill_number'];
      if (!empty($trip_number) && !empty($rs_code) && !empty($bill_number)) {

        // $bill_details = _get_noneditable_godown_delivered_bill_details($trip_number, $rs_code, $bill_number);
//         if (!empty($bill_details)) {
        //$bill_details_header = array('Bill value', 'Party name');
//            $form['bill_details'] = array(
//              '#theme' => 'table',
//              '#header' => $bill_details_header,
//              '#rows' => $bill_details,
//            );
        $exit = check_bill_and_trip_combination($trip_number, $bill_number);
        if(!empty($exit)){
        $item_details = _get_editable_godown_delivered_item_details($trip_number, $bill_number);
        $item_details_header = array(
          'SKU7_Code' => 'SKU7 Code',
          'PRODUCT_NAME' => 'SKU Description',
          //'BASEPACK_CODE' => 'Batch Code',
          'MRP' => 'MRP',
          'Sum_of_TOTAL_SALES_QTY' => 'Total Sales Quantity',
          'godown_short' => 'Godown Short'
        );

        // initialize editable elements here
        $form['godown_short'] = array('#type' => 'value');
        foreach ($item_details as $key => &$value) {
          $item_details[$key]['godown_short']['data'] = array(
            '#type' => 'textfield',
            '#title' => t('Godown Short'),
            '#title_display' => 'invisible',
            '#size' => 20,
            '#value' => $value['godown_shorts'],
            '#name' => "godown_short[$key]",
            '#attributes' => array('class' => array('godown_short_list')),
          );
          unset($value['godown_shorts']);
          $sku_data[$key]['mrp'] = $value['mrp'];
          //$sku_data[$key]['BASEPACK_CODE'] = $value['BASEPACK_CODE'];
          $sku_data[$key]['product_name'] = $value['product_name'];
          $sku_data[$key]['Sum_of_TOTAL_SALES_QTY'] = $value['Sum_of_TOTAL_SALES_QTY'];
        }
        $form['sku_data'] = array(
          '#type' => 'value',
          '#value' => $sku_data,
        );

//             foreach ($item_details as $key => $val) {
//               foreach ($val as $key1 => $val1) {
//                  if ($key1 == 'godown_short') {
//                     $item_details_new[$key][$key1] = array('data' => $val1['data'], 'data-value' => $val1, 'class' => array("$key1-val"));
//                  }
//                  else {
//                     $item_details_new[$key][$key1] = array('data' => $val1, 'data-value' => $val1, 'class' => array("$key1-val"));
//                  }
//               }
//            }


        $form['trip_details'] = array(
          '#theme' => 'table',
          '#header' => $item_details_header,
          '#rows' => $item_details,
        );
        $form['submit'] = array(
          '#type' => 'submit',
          '#value' => 'submit',
        );
        }
         else {
            drupal_set_message(t("Bill Number: $bill_number not found in the Trip: $trip_number"), 'error');
         }
        }
      }
    }
  }
  return $form;
}

function _godown_callback_form_submit($form, &$form_state) {

  global $user;
  $rs_code = dispatch_planning_get_rscode_using_uid($user->uid);
  $dis_id = change_feeds_import_get_distributor_id($user->uid);
  $trip_id = $form_state['values']['trip_number'];
  $bill_number = $form_state['values']['bill_number'];
  $sku_data = $form_state['values']['sku_data'];
  $godown = $form_state['values']['godown_short'];

  if (!empty($trip_id) && !empty($bill_number) && !empty($dis_id)) {
    $farey_status_exist = 0;
    $farey_status_msg = 0;
    foreach ($godown as $key => $godown_short_qty) {
      $unique_combi = explode('#', $key);
      $sku_code = $unique_combi[0];
      //$batch_code = $sku_data[$key]['BASEPACK_CODE'];
      db_merge('godown_data')
        ->key(array('distributor_id' => $dis_id, 'trip_id' => $trip_id, 'bill_number' => $bill_number, 'sku_code' => $sku_code))
        ->fields(array(
          'trip_id' => $trip_id,
          'bill_number' => $bill_number,
          'sku_code' => $sku_code,
          //'batch_code' => $batch_code,
          'mrp' => $sku_data[$key]['mrp'],
          'sku_desc' => $sku_data[$key]['product_name'],
          'sum_sales_qty' => $sku_data[$key]['Sum_of_TOTAL_SALES_QTY'],
          'godown_short' => $godown_short_qty,
          'distributor_id' => $dis_id,
          'created_date' => time(),
        ))
        ->execute();

      //newly added for sync in farey table
      $reference_no = $trip_id . '_' . $rs_code . '_' . $bill_number;
      $bill_row_id = _get_bill_row_id_using_reference_no($reference_no);
      if ($bill_row_id) {
        $godown_short_qty = $godown_short_qty ? $godown_short_qty : 0;

        $farey_status_exist = db_query("select res_status from dispatch_planning_api_data where id = $bill_row_id ")->fetchField();
        
        if (!empty($farey_status_exist)) {
          $farey_status_msg = 1;
          $query = db_select('dispatch_planning_item_details', 'ad');
          $query->fields('ad', array('sum_net_sales_quantity', 'res_cashier_short', 'return_qty'));
          $query->condition('ad.trip_bill_reference', $bill_row_id);
          $query->condition('ad.sku_code', $sku_code);
          //$query->condition('ad.batch_code', $batch_code, '=');
          $res_update = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

          $sum_net_sales = ($res_update[0]['sum_net_sales_quantity']) ? $res_update[0]['sum_net_sales_quantity'] : 0;
          $cashier_short = ($res_update[0]['res_cashier_short']) ? $res_update[0]['res_cashier_short'] : 0;
          $returned_qty = ($res_update[0]['return_qty']) ? $res_update[0]['return_qty'] : 0;
          
          $dispatch_qty = $sum_net_sales - $godown_short_qty;

          $deliver_qty = $dispatch_qty - $returned_qty - $cashier_short;

          db_update('dispatch_planning_item_details') // Table name no longer needs {}
            ->fields(array(
              'res_godown_short' => $godown_short_qty,
              'dispatch_qty' => $dispatch_qty,
              'res_sku_actual_quantity1' => $deliver_qty,
            ))
            ->condition('trip_bill_reference', $bill_row_id, '=')
            ->condition('sku_code', $sku_code, '=')
            //->condition('batch_code', $batch_code, '=')
            ->execute();
        }
        else {
          db_update('dispatch_planning_item_details') // Table name no longer needs {}
            ->fields(array(
              'res_godown_short' => $godown_short_qty
            ))
            ->expression('dispatch_qty', "sum_net_sales_quantity - $godown_short_qty")
            ->condition('trip_bill_reference', $bill_row_id, '=')
            ->condition('sku_code', $sku_code, '=')
            //->condition('batch_code', $batch_code, '=')
            ->execute();
        }
      }
    }
    drupal_set_message(t('Value inserted successfully'));
    if (!empty($farey_status_msg)) {
      drupal_set_message("Please update Return Manual Edit Screen for Trip: $trip_id and Bill: $bill_number", 'warning');
    }
    $form_state['storage']['trip_id'] = $trip_id;
    $form_state['rebuild'] = TRUE;
  }
}

/**
 * 
 * @global type $user
 * @param type $trip_number
 * @param type $rs_code
 * @param type $bill_no
 * @return type
 */
function _get_noneditable_godown_delivered_bill_details($trip_number, $rs_code, $bill_no) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);

  $query = db_select('dispatch_planning_api_data', 'ad');
  $query->condition('ad.runsheet_number', $trip_number);
  $query->condition('ad.hub', $rs_code);
  $query->condition('ad.bill_number', $bill_no);
  $query->fields('ad', array('bill_value', 'party_name'));
  $rows = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
  return $rows;
}

/**
 * 
 * @param type $form
 * @param type $form_state
 * @return array
 */
function trip_details_remove_godown_callback($form, &$form_state) {
  return $form;
}
function trip_details_remove_godown_callback_trip($form, &$form_state) {
  $form_state['trip_change'] = 1;
  return $form;
}

/**
 * 
 * @param type $string
 * @param type $trip_id
 */
function _get_all_tripsbill_godown($trip_id) {
  $result = array();

  if (!empty($trip_id)) {
    $query = db_select('node', 'n');
    $query->Join('field_data_field_trip_id_reference', 'ft', 'ft.entity_id = n.nid');
    $query->Join('node', 'n1', 'n1.nid = ft.field_trip_id_reference_nid');
    $query->condition('n.type', 'sales_register_data');
    $query->condition('n1.title', $trip_id);
    $query->fields('n', array('title'));
    // $query->range(0, 15);
    $query->addTag('node_distributor_access');
    $result = $query->execute()->fetchAllKeyed(0, 0);
  }

  return $result;
}

function _get_editable_godown_delivered_item_details($trip_id, $bill_no) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);

//  $query = db_select('custom_billing_data', 'cbd');
//  $query->condition('cbd.distributor_id', $dis_id);
//  $query->condition('cbd.bill_number', $bill_no);
//  $query->fields('cbd', array('sku7_code', 'sku7_code', 'product_name', 'BASEPACK_CODE', 'mrp', 'Sum_of_TOTAL_SALES_QTY'));
//  $rows = $query->execute()->fetchAll(PDO::FETCH_UNIQUE | PDO::FETCH_ASSOC);
  $query = db_select('custom_billing_data', 'cbd');
  $query->condition('cbd.distributor_id', $dis_id);
  $query->condition('cbd.bill_number', $bill_no);
  //$query->addExpression("concat(sku7_code,'#',BASEPACK_CODE)", 'unique_combi');
  //$query->fields('cbd', array('sku7_code', 'product_name', 'BASEPACK_CODE', 'mrp', 'Sum_of_TOTAL_SALES_QTY'));
  $query->addExpression("sku7_code", 'unique_combi');
  $query->fields('cbd', array('sku7_code', 'product_name', 'mrp', 'Sum_of_TOTAL_SALES_QTY'));
  $results = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

  $rows = array();
  foreach ($results as $val) {
    $unique_combi = $val['unique_combi'];
    unset($val['unique_combi']);
    $rows[$unique_combi] = $val;
  }

  $query = db_select('godown_data', 'gd');
  $query->condition('gd.trip_id', $trip_id);
  $query->condition('gd.distributor_id', $dis_id);
  $query->condition('gd.bill_number', $bill_no);
  //$query->addExpression("concat(sku_code,'#',batch_code)", 'unique_combi');
  $query->addExpression("sku_code", 'unique_combi');
  $query->fields('gd', array('godown_short'));
  $rows2 = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

  if ($rows2) {
    foreach ($rows2 as $value) {
      $unique_combi = $value['unique_combi'];
      if (isset($rows[$unique_combi])) {
        $rows[$unique_combi]['godown_shorts'] = $value['godown_short'];
      }
    }
  }
  return $rows;
}
function check_bill_and_trip_combination($trip_id, $bill_no) {
    $query = db_select('node', 'n');
    $query->Join('field_data_field_trip_id_reference', 'ft', 'ft.entity_id = n.nid');
    $query->Join('node', 'n1', 'n1.nid = ft.field_trip_id_reference_nid');
    $query->condition('n.type', 'sales_register_data');
    $query->condition('n.title', $bill_no);
    $query->condition('n1.title', $trip_id);
    $query->fields('n', array('title'));
    $query->addTag('node_distributor_access');
    $bill_no = $query->execute()->fetchField();
    return $bill_no;
}
