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
function sku_wise_returns_form($form, &$form_state) {
  $form['#prefix'] = '<div id="itemlist-div">';
  $form['#suffix'] = '</div>';

  $form['trip_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Trip'),
    '#required' => TRUE,
    '#autocomplete_path' => 'sku-wise-trip/autocomplete',
    '#ajax' => array(
      'wrapper' => 'itemlist-div',
      'callback' => 'trip_details_return_callback',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    '#validate' => array('trip_number_validate'),
  );

  if (isset($form_state['storage']['trip_id']) && !empty($form_state['storage']['trip_id'])) {
    $trip_number = $form_state['storage']['trip_id'];
    unset($form_state['storage']['trip_id']);
    $form['sku_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Product Name'),
      '#required' => TRUE,
      '#autocomplete_path' => "sku/autocomplete/$trip_number",
      '#value' => '',
      '#ajax' => array(
        'wrapper' => 'itemlist-div',
        'callback' => 'trip_details_return_callback',
        'method' => 'replace',
        'effect' => 'fade',
      ),
    );
  }
  else {
    $trip_number = $form_state['input']['trip_number'];
    if (!empty($trip_number) && empty($form_state['trip_no_error'])) {
      $form['sku_name'] = array(
        '#type' => 'textfield',
        '#title' => t('Product Name'),
        '#required' => TRUE,
        '#autocomplete_path' => "sku/autocomplete/$trip_number",
        '#ajax' => array(
          'wrapper' => 'itemlist-div',
          'callback' => 'trip_details_return_callback',
          'method' => 'replace',
          'effect' => 'fade',
        ),
      );

      $sku_name = $form_state['input']['sku_name'];
      if (!empty($trip_number) && !empty($sku_name)) {
        $initial_option = array('' => 'Select');
        $option = get_mrp_from_sku($trip_number, $sku_name);
        $default_mrp = '';
        if (count($option) == 1) {
          $default_mrp = reset($option);
        }
        elseif (count($option) > 1 && isset($form_state['input']['mrp'])) {
          $default_mrp = $form_state['input']['mrp'];
        }
        if (!empty($option)) {
          $initial_option = $initial_option + $option;
        }

        if (isset($form_state['input']['mrp'])) {
          unset($form_state['input']['mrp']);
        }
        $form['mrp'] = array(
          '#type' => 'select',
          '#title' => t('MRP'),
          '#default_value' => $default_mrp,
          '#options' => $initial_option,
          '#required' => TRUE,
          '#ajax' => array(
            'wrapper' => 'itemlist-div',
            'callback' => 'trip_details_return_callback',
            'method' => 'replace',
            'effect' => 'fade',
          ),
          '#attributes' => array('class' => array('mrp_list')),
        );
      }

      $mrp = $default_mrp ? $default_mrp : $form_state['input']['mrp'];

      if (!empty($mrp)) {
        $get_to_be_returned_qty = get_to_be_returned_qty($trip_number, $sku_name, $mrp);

        if (isset($form_state['input']['to_be_returned_qty'])) {
          unset($form_state['input']['to_be_returned_qty']);
        }
        $form['to_be_returned_qty'] = array(
          '#type' => 'textfield',
          '#title' => t('To Be Returned Qty'),
          '#default_value' => $get_to_be_returned_qty[0]['return_qty'],
          '#attributes' => array('readonly' => 'readonly'),
        );

        $form['trip_ref_ids'] = array(
          '#type' => 'value',
          '#value' => $get_to_be_returned_qty[0]['ids']
        );

        $form['actual_returned_qty'] = array(
          '#type' => 'textfield',
          '#title' => t('Actual Returned Qty'),
          '#required' => TRUE,
          '#ajax' => array(
            'wrapper' => 'itemlist-div',
            'callback' => 'trip_details_return_callback',
            'method' => 'replace',
            'effect' => 'fade',
          ),
        );

        if (!empty($form_state['input']['actual_returned_qty']) || $form_state['input']['actual_returned_qty'] === "0") {
          $to_be_returned_qty = $form_state['values']['to_be_returned_qty'];
          $actual_returned_qty = $form_state['input']['actual_returned_qty'];

          if ($to_be_returned_qty != $actual_returned_qty) {
            $radio_options = array('partial_deliver' => 'Partial Delivered Bills', 'undeliver' => 'Full Returned Bills', 'both' => 'Both');

            $form['bill_status'] = array(
              '#title' => t('Bill Status'),
              '#type' => 'radios',
              '#size' => 20,
              '#options' => $radio_options,
              '#default_value' => 'both',
              '#required' => TRUE,
              '#ajax' => array(
                'wrapper' => 'itemlist-div',
                'callback' => 'trip_details_return_callback',
                'method' => 'replace',
                'effect' => 'fade',
              ),
            );

            $bill_status = $form_state['input']['bill_status'];
            if (empty($bill_status)) {
              $selected_bill_status = 'both';
            }
            else {
              $selected_bill_status = $bill_status;
            }

            $create_sku_wise_return_table = sku_wise_return_table($trip_number, $sku_name, $mrp, $selected_bill_status);

            if (!empty($create_sku_wise_return_table)) {

              $header = array('Bill Number', 'Party Name', 'Total Sales Qty', 'Godown Short', 'Dispatch Qty', 'Declared Qty', 'Actual Returned Qty', 'Reason Code', 'Cashier Short');

              $form['bill_details'] = array(
                '#theme' => 'table',
                '#tree' => TRUE,
                '#header' => $header,
                '#rows' => array(),
              );

              foreach ($create_sku_wise_return_table as $val) {
                $key = $val['id'];

                $form['bill_details']['#rows'][$key][] = $val['bill_number'];
                // $form['bill_details']['#rows'][$key][] = $val['res_status'];
                $form['bill_details']['#rows'][$key][] = $val['party_name'];
                $form['bill_details']['#rows'][$key][] = $val['sum_net_sales_quantity'];
                $form['bill_details']['#rows'][$key][] = $val['res_godown_short'];
                $form['bill_details']['#rows'][$key][] = $val['dispatch_qty'];
                $form['bill_details']['#rows'][$key][] = $val['return_qty'];

                $actual_returned_qty = array(
                  '#id' => 'bill_details-' . $key . '-actual_returned_qty',
                  '#type' => 'textfield',
                  '#size' => 10,
                  '#default_value' => $val['return_qty'],
                  '#attributes' => array('class' => array('actual-returned-qty')),
                );

                $reason_type = $val['res_status'];
                $reason_list = _get_bill_status_wise_reason_master_list($reason_type);

                if ($reason_type == 'undeliver') {
                  $reason_code = array(
                    '#id' => 'bill_details-' . $key . '-reason_code',
                    '#type' => 'select',
                    '#default_value' => $val['res_reason_for_failure'],
                    '#options' => $reason_list,
                  );
                }
                else {
                  $reason_code = array(
                    '#id' => 'bill_details-' . $key . '-reason_code',
                    '#type' => 'select',
                    '#default_value' => $val['res_sku_reason'],
                    '#options' => $reason_list,
                  );
                }

                $cashier_short = array(
                  '#id' => 'bill_details-' . $key . '-cashier_short',
                  '#type' => 'textfield',
                  '#size' => 10,
                  '#default_value' => $val['res_cashier_short'],
                  '#attributes' => array('class' => array('res-cashier-qty')),
                );

                $form['bill_details'][$key]["actual_returned_qty"] = &$actual_returned_qty;
                $form['bill_details'][$key]["reason_code"] = &$reason_code;
                $form['bill_details'][$key]["cashier_short"] = &$cashier_short;

                $form['bill_details']['#rows'][$key][]['data']["actual_returned_qty"] = &$actual_returned_qty;
                $form['bill_details']['#rows'][$key][]['data']["reason_code"] = &$reason_code;
                $form['bill_details']['#rows'][$key][]['data']["cashier_short"] = &$cashier_short;

                unset($actual_returned_qty);
                unset($reason_code);
                unset($cashier_short);
              }
            }
          }

          $form['confirm_submit'] = array(
            '#type' => 'submit',
            '#value' => 'Confirm',
            '#ajax' => array(
              'callback' => 'trip_details_return_callback',
              'wrapper' => 'itemlist-div',
              'method' => 'replace',
              'effect' => 'fade',
            ),
            '#validate' => array('sku_wise_return_confirm_validate'),
            '#submit' => array('sku_wise_return_confirm_submit'),
          );
        }
      }
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
function trip_details_return_callback($form, &$form_state) {
  return $form;
}

/**
 * 
 * @param type $form
 * @param type $form_state
 * @return array
 */
function sku_wise_return_table($trip_number, $sku_name, $mrp, $selected_bill_status) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);

  $query = db_select('dispatch_planning_item_details', 'dpi');
  $query->join('dispatch_planning_api_data', 'ad', 'dpi.trip_bill_reference = ad.id');
      //not required delivery pending bills of that trip
  $query->condition('ad.delivery_pending_flag' ,1,'!=');
  $query->condition('ad.runsheet_number', $trip_number, '=');
  $query->condition('ad.distributor_id', $dis_id);
  $query->condition('dpi.product_name', $sku_name, '=');
  $query->condition('dpi.mrp', $mrp, '=');
  $query->condition('dpi.line_level_status', 'line_deliver', '!=');
  if ($selected_bill_status == 'both') {
    $query->condition('ad.res_status', 'deliver', '!=');
  }
  elseif ($selected_bill_status == 'partial_deliver') {
    $query->condition('ad.res_status', 'partial_deliver', '=');
  }
  elseif ($selected_bill_status == 'undeliver') {
    $query->condition('ad.res_status', 'undeliver', '=');
  }
  $query->fields('ad', array('bill_number'));
  $query->fields('ad', array('party_name'));
  $query->fields('ad', array('res_status'));
  $query->fields('ad', array('res_reason_for_failure'));
  $query->fields('dpi', array('sum_net_sales_quantity'));
  $query->fields('dpi', array('res_godown_short'));
  $query->fields('dpi', array('dispatch_qty'));
  $query->fields('dpi', array('return_qty'));
  $query->fields('dpi', array('res_sku_reason'));
  $query->fields('dpi', array('res_cashier_short'));
  $query->fields('dpi', array('trip_bill_reference'));
  $query->fields('dpi', array('id'));
  $result = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

  return $result;
}

function _get_bill_status_wise_reason_master_list($reason_type) {
  $query = db_select('node', 'n');
  $query->join('field_data_field_reason_type', 'rt', 'rt.entity_id = n.nid');
  $query->join('field_data_field_dp_reason', 'dr', 'dr.entity_id = n.nid');
  $query->condition('rt.field_reason_type_value', $reason_type);
  $query->fields('dr', array('field_dp_reason_value'));
  $query->addTag('node_distributor_access');
  $res = $query->execute()->fetchAllKeyed(0, 0);
  $res1 = array('' => '- select -');
  $response = $res1 + $res;
  return $response;
}

function sku_wise_return_confirm_submit($form, &$form_state) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);

  $trip_number = $form_state['values']['trip_number'];
  $product_name = $form_state['values']['sku_name'];
  $mrp = $form_state['values']['mrp'];

  $to_be_returned_qty = $form_state['values']['to_be_returned_qty'];
  $actual_returned_qty = $form_state['values']['actual_returned_qty'];

  $trip_ref_ids = $form_state['values']['trip_ref_ids'];
  $trip_ref_ids_arr = explode(",", $trip_ref_ids);

  if ($to_be_returned_qty == $actual_returned_qty) {
    foreach ($trip_ref_ids_arr as $id) {
      db_update('dispatch_planning_api_data') // Table name no longer needs {}
        ->fields(array(
          'updated_flag' => 1,
          'updated_time' => REQUEST_TIME,
        ))
        ->condition('runsheet_number', $trip_number, '=')
        ->condition('id', $id, '=')
        ->execute();
    }
  }
  else {
    $bill_details = $form_state['values']['bill_details'];
    foreach ($bill_details as $id => $value) {

      $query = db_select('dispatch_planning_item_details', 'dpi');
      $query->join('dispatch_planning_api_data', 'ad', 'dpi.trip_bill_reference = ad.id');
      $query->addfield('ad', 'res_status', 'res_status');
      $query->fields('dpi', array('res_godown_short', 'dispatch_qty', 'trip_bill_reference'));
      $query->condition('dpi.id', $id, '=');
      $itdatas = $query->execute()->fetchAll();
      $itdata = $itdatas[0];

      if ($itdata->res_status == 'undeliver') {
        $ret_cash = $value['actual_returned_qty'] + $value['cashier_short']; //Return Qty + Cashier Short  
        $ret_cash_godown = $ret_cash + $itdata->res_godown_short; //Return Qty + Cashier Short + Godown Short
        $line_status = '';
        if ($ret_cash == $itdata->dispatch_qty) {
          $line_status = 'line_undeliver';
        }
        else if ($ret_cash_godown > 0) {
          $line_status = 'line_partial_deliver';
        }
        else if ($ret_cash_godown == 0) {
          $line_status = 'line_deliver';
        }

        db_update('dispatch_planning_item_details') // Table name no longer needs {}
          ->fields(array(
            'res_sku_actual_quantity1' => $itdata->dispatch_qty - $value['actual_returned_qty'] - $value['cashier_short'],
            'return_qty' => $value['actual_returned_qty'],
            'res_sku_reason' => $value['reason_code'],
            'res_cashier_short' => $value['cashier_short'],
            'line_level_status' => $line_status, // line level status
          ))
          ->condition('product_name', $product_name, '=')
          ->condition('mrp', $mrp, '=')
          ->condition('id', $id, '=')
          ->execute();

        if ($line_status == 'line_undeliver') {
          db_update('dispatch_planning_api_data') // Table name no longer needs {}
            ->fields(array(
              'res_reason_for_failure' => $value['reason_code'] ? $value['reason_code'] : '',
              'updated_flag' => 1,
              'updated_time' => REQUEST_TIME,
            ))
            ->condition('runsheet_number', $trip_number, '=')
            ->condition('id', $itdata->trip_bill_reference, '=')
            ->execute();
        }
        else {
          db_update('dispatch_planning_api_data') // Table name no longer needs {}
            ->fields(array(
              'res_status' => 'partial_deliver',
              'res_reason_for_failure'=>'',
              'updated_flag' => 1,
              'updated_time' => REQUEST_TIME,
            ))
            ->condition('runsheet_number', $trip_number, '=')
            ->condition('id', $itdata->trip_bill_reference, '=')
            ->execute();
        }
      }
      else if ($itdata->res_status == 'partial_deliver') {

        $ret_cash = $value['actual_returned_qty'] + $value['cashier_short']; //Return Qty + Cashier Short  
        $ret_cash_godown = $ret_cash + $itdata->res_godown_short; //Return Qty + Cashier Short + Godown Short
        $line_status = '';
        if ($ret_cash == $itdata->dispatch_qty) {
          $line_status = 'line_undeliver';
        }
        else if ($ret_cash_godown > 0) {
          $line_status = 'line_partial_deliver';
        }
        else if ($ret_cash_godown == 0) {
          $line_status = 'line_deliver';
        }
        db_update('dispatch_planning_api_data') // Table name no longer needs {}
          ->fields(array(
            'updated_flag' => 1,
            'updated_time' => REQUEST_TIME,
          ))
          ->condition('runsheet_number', $trip_number, '=')
          ->condition('id', $itdata->trip_bill_reference, '=')
          ->execute();

        db_update('dispatch_planning_item_details') // Table name no longer needs {}
          ->fields(array(
            'res_sku_actual_quantity1' => $itdata->dispatch_qty - $value['actual_returned_qty'] - $value['cashier_short'],
            'return_qty' => $value['actual_returned_qty'],
            'res_sku_reason' => $value['reason_code'],
            'res_cashier_short' => $value['cashier_short'],
            'line_level_status' => $line_status, // line level status
          ))
          ->condition('product_name', $product_name, '=')
          ->condition('mrp', $mrp, '=')
          ->condition('id', $id, '=')
          ->execute();
      }
    }
  }

  $form_state['storage']['trip_id'] = $trip_number;
  drupal_set_message(t('Sku-wise returns confirm successfully.'));
  $form_state['rebuild'] = TRUE;
}

function trip_number_validate($form, &$form_state) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);
  $form_state['trip_no_error'] = '';
  $trip_number = $form_state['input']['trip_number'];

  $query = db_select('dispatch_planning_api_data', 'ad');
  $query->condition('ad.runsheet_number', $trip_number, '=');
  $query->condition('ad.distributor_id', $dis_id);
  $query->fields('ad', array('id'));
  $result = $query->execute()->fetchField();
  if (empty($result)) {
    drupal_set_message(t("Trip ID:$trip_number has not been dispatched yet."), 'error');
    $form_state['trip_no_error'] = 1;
  }


  $query = db_select('dispatch_planning_api_data', 'ad');
  $query->condition('ad.runsheet_number', $trip_number, '=');
  $query->condition('ad.distributor_id', $dis_id);
  $query->isNull('ad.res_status');
  $query->addExpression('GROUP_CONCAT(ad.bill_number)', 'bill_numbers');
  $res = $query->execute()->fetchField();
  if ($res) {
    drupal_set_message(t("Please declare delivery outcome for all the bills in this Trip ID:$trip_number"), 'error');
    $form_state['trip_no_error'] = 1;
  }

  $trip_detail = check_trip_id_exists_or_not($dis_id, $trip_number);
  $vrs_status = $trip_detail->field_vrs_status_value;

  if ($vrs_status == 1) {
    drupal_set_message(t('You cannot edit because this trip is returns confirmed.'), 'error');
    $form_state['trip_no_error'] = 1;
  }
  else if ($vrs_status == 2) {
    drupal_set_message(t('You cannot edit because this trip is VRS confirmed.'), 'error');
    $form_state['trip_no_error'] = 1;
  }
  else if ($vrs_status == 3) {
    drupal_set_message(t('You cannot edit because this trip is VRS pending.'), 'error');
    $form_state['trip_no_error'] = 1;
  }
  else if ($vrs_status == 4) {
    drupal_set_message(t('You cannot edit because this trip is Handover pending.'), 'error');
    $form_state['trip_no_error'] = 1;
  }
  else if ($vrs_status == 5) {
    drupal_set_message(t('You cannot edit because this trip is Handover confirmed.'), 'error');
    $form_state['trip_no_error'] = 1;
  }
}

function sku_wise_return_confirm_validate($form, &$form_state) {
  $actual_returned_qty = $form_state['input']['actual_returned_qty'];

  $to_be_returned_qty = $form_state['values']['to_be_returned_qty'];

  if ($to_be_returned_qty != $actual_returned_qty) {
    $bill_details = $form_state['values']['bill_details'];
    foreach ($bill_details as $value) {
      $edit_actual_returned_qty += $value['actual_returned_qty'];
    }

    if ($actual_returned_qty != $edit_actual_returned_qty) {
      $form_state['rebuild'] = TRUE;
      form_set_error('form', "Actual Returned Qty is not equal to To be Returned Qty.");
      $form_state['actual_returned_qty_error'] = 1;
    }
    else {
      unset($form_state['actual_returned_qty_error']);
    }
  }
  else {
    unset($form_state['actual_returned_qty_error']);
  }
}
