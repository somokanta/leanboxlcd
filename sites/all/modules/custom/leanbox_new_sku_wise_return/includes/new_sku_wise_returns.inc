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
function new_sku_wise_returns_form($form, &$form_state) {
  // show bill wise entry
  global $base_url;
  $image_path = drupal_get_path('module', 'mrc_inventory_returns');
  $bill_wise = arg(1);
  if ($bill_wise == 'bill-wise') {
    $form_state['bill-wise'] = 1;
  }
  $form['#attached']['js'][] = drupal_get_path('module', 'leanbox_new_sku_wise_return') . '/js/leanbox_new_sku_wise_return.js';
  global $user;
  $distributor_id = change_feeds_import_get_distributor_id($user->uid);

  $form['#prefix'] = '<div id="itemlist-div" class="custom_form_wrap">';
  $form['#suffix'] = '</div>';

  $form['trip_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Trip ID'),
    '#required' => TRUE,
    '#autocomplete_path' => 'sku-wise-trip/autocomplete',
    '#ajax' => array(
      'wrapper' => 'itemlist-div',
      'callback' => 'trip_details_return_callback',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    '#prefix' => '<div class="form_row">',
    '#validate' => array('trip_number_validation'),
  );
  $trip_number = $form_state['input']['trip_number'];

  if (!empty($trip_number) && empty($form_state['trip_no_error'])) {
    $putaway_initial_option = array('' => 'Select');
    $putaway_group_options = get_all_putaway_groups($distributor_id);
    if (!empty($putaway_group_options)) {
      $putaway_initial_option = $putaway_initial_option + $putaway_group_options;
    }
    $form['putaway_group'] = array(
      '#type' => 'select',
      '#title' => t('Putaway Group'),
      '#options' => $putaway_initial_option,
      '#ajax' => array(
        'wrapper' => 'itemlist-div',
        'callback' => 'trip_details_return_callback',
        'method' => 'replace',
        'effect' => 'fade',
      ),
      '#attributes' => array('class' => array('putaway_list')),
    );

    $sub_putaway_options = array('' => 'Select');
    $sub_putaway_group_options = get_all_sub_putaway_groups($form_state['input']['putaway_group'], $distributor_id);
    if (!empty($sub_putaway_group_options)) {
      $sub_putaway_options = $sub_putaway_options + $sub_putaway_group_options;
    }
    $form['sub_putaway_group'] = array(
      '#type' => 'select',
      '#title' => t('Sub-Putaway Group'),
      '#options' => $sub_putaway_options,
      '#ajax' => array(
        'wrapper' => 'itemlist-div',
        'callback' => 'trip_details_return_callback',
        'method' => 'replace',
        'effect' => 'fade',
      ),
      '#attributes' => array('class' => array('sub-putaway_list')),
    );

    $radio_options = array('partial_deliver' => 'Partial Delivered Bills', 'undeliver' => 'Full Returned Bills', 'both' => 'Both');

    $form['bill_status'] = array(
      '#title' => t('Bill Status'),
      '#type' => 'radios',
      '#size' => 20,
      '#options' => $radio_options,
      '#default_value' => 'both',
      '#required' => TRUE,
    );
// this checkbox will be visible when url is new-skuwise-return/bill-wise
    if ($form_state['bill-wise'] == 1) {
      $form['show_bills'] = array(
        '#type' => 'checkbox',
        '#title' => t('Show Bills as well'),
        '#default_value' => 1,
        '#prefix' => '<div class="custom_check_2col">',
      );
    }
    else {
      $form['show_bills'] = array(
        '#type' => 'markup',
        '#prefix' => '<div class="custom_check_2col">',
      );
    }

    $form['show_skus'] = array(
      '#type' => 'checkbox',
      '#default_value' => 1,
      '#title' => t('Show only SKUs with discrepancy'),
      '#suffix' => '</div>',
    );

    $form['apply'] = array(
      '#type' => 'submit',
      '#value' => 'Apply',
      '#ajax' => array(
        'callback' => 'trip_details_return_callback',
        'wrapper' => 'itemlist-div',
        'method' => 'replace',
        'effect' => 'fade',
      ),
      '#suffix' => '</div>',
      '#submit' => array('new_sku_wise_return_submit'),
    );

    $header = array('SKU Code', 'Product Name', 'MRP', 'To Be Returned Qty', 'Actual Returned Qty');

    if (!isset($form_state['sku_wise_return_confirm']['SKUs'])) {
      $form_state['sku_wise_return_confirm']['SKUs'] = array();
    }
    $options = $form_state['sku_wise_return_confirm']['SKUs'];

    if (!empty($options)) {
      $form['bill_details'] = array(
        '#theme' => 'table',
        '#tree' => TRUE,
        '#header' => $header,
        '#rows' => array(),
        '#prefix' => '<div class="custom_middle_table">',
        '#suffix' => '</div>',
      );
    }

    $header2 = array('Bill Number', 'Bill Status', 'Party Name', 'Total Sales Qty', 'Godown Short', 'Dispatch Qty', 'To Be Returned', 'Actual Returned Qty', 'Reason Code', 'Cashier Short', 'Claimed Godown Short', 'Reason for Claim');

    foreach ($options as $key => $val) {
      $get_default_actual_return_qty = get_default_actual_return_qty($trip_number, $key, $distributor_id);

      // If there is no discrepancy, rows will be highlighted in green
      if ($val['to_be_return_qty'] == $get_default_actual_return_qty) {
        $form['bill_details']['#rows'][$key]['class'][] = 'green-row';
        $form_state['no_discrepancy_sku'][$key] = $key;
      }
      // If there is discrepancy, rows will be highlighted in red
      else {
        $form['bill_details']['#rows'][$key]['class'][] = 'red-row';
      }
      $form['bill_details']['#rows'][$key]['data'][] = $key;
      $form['bill_details']['#rows'][$key]['data'][] = $val['product_name'];
      $form['bill_details']['#rows'][$key]['data'][] = $val['mrp'];

      $form['bill_details']['#rows'][$key]['data'][] = array('data' => $val['to_be_return_qty'], 'class' => 'to_be_return_qty');
      $actual_return_qty_sku_class = 'actual_return_qty_' . $key;
      if ($form_state['bill-wise'] == 1) {
        //'readonly' => 'readonly'
        $actual_returned_qty = array(
          '#type' => 'textfield',
          '#attributes' => array('class' => array('allownumericwithoutdecimal', 'actual-return-qty', $actual_return_qty_sku_class), 'sku_code' => $key, 'readonly' => 'readonly'),
          '#size' => 10,
          '#default_value' => $get_default_actual_return_qty,
        );
      }
      else {
        $actual_returned_qty = array(
          '#type' => 'textfield',
          '#attributes' => array('class' => array('allownumericwithoutdecimal', 'actual-return-qty'), 'sku_code' => $key),
          '#size' => 10,
          '#default_value' => $get_default_actual_return_qty,
        );
      }

      $form['bill_details'][$key]["actual_returned_qty"] = &$actual_returned_qty;

      $form['bill_details']['#rows'][$key]['data'][]['data']["actual_returned_qty"] = &$actual_returned_qty;

      unset($actual_returned_qty);

      /*       * ***********************START POP UP BILL TABLE************************ */

      if (($form_state['values']['show_bills'] == 1)) {

        $form["sku_bill_table_$key"] = array(
          '#theme' => 'table',
          '#tree' => TRUE,
          '#header' => $header2,
          '#rows' => array(),
          '#sticky' => FALSE,
          '#caption' => "<b>Product Name: </b>" . $val['product_name'],
          '#prefix' => '<div id ="pop_up_bill_' . $key . '" class="pop-up_table" style="display:none;"><div class="popup_overlay"></div><div class="popup_wrap"><div class="pop_content"><div class="popup_border">'
          . '<img sku_key="' . $key . '" class="close-pop-up-button" id="close_pop_up_bill_' . $key . '" src="' . $base_url . '/' . $image_path . '/images/close_btn.png">',
          '#suffix' => '</div></div></div></div>',
        );

        $all_bills = sku_wise_return_pop_up_table($trip_number, $key, $distributor_id);

        foreach ($all_bills as $key1 => $val1) {
          $form["sku_bill_table_$key"]['#rows'][$val1['ad_bill_number']][] = $val1['ad_bill_number'];
          $form["sku_bill_table_$key"]['#rows'][$val1['ad_bill_number']][] = $val1['res_status'];
          $form["sku_bill_table_$key"]['#rows'][$val1['ad_bill_number']][] = $val1['party_name'];
          $form["sku_bill_table_$key"]['#rows'][$val1['ad_bill_number']][] = $val1['sum_net_sales_quantity'];

          $res_godown_short = array(
            '#type' => 'textfield',
            '#size' => 10,
            '#attributes' => array('class' => array('allownumericwithoutdecimal', 'res-godown-short'), 'readonly' => 'readonly'),
            '#default_value' => $val1['res_godown_short'],
          );
          $form["sku_bill_table_$key"][$val1['ad_bill_number']]["res_godown_short"] = &$res_godown_short;
          $form["sku_bill_table_$key"]['#rows'][$val1['ad_bill_number']][]['data']["res_godown_short"] = &$res_godown_short;

          $dispatch_qty = array(
            '#type' => 'textfield',
            '#size' => 10,
            '#attributes' => array('class' => array('allownumericwithoutdecimal', 'dispatch-qty'), 'readonly' => 'readonly'),
            '#default_value' => $val1['dispatch_qty'],
          );
          $form["sku_bill_table_$key"][$val1['ad_bill_number']]["dispatch_qty"] = &$dispatch_qty;
          $form["sku_bill_table_$key"]['#rows'][$val1['ad_bill_number']][]['data']["dispatch_qty"] = &$dispatch_qty;

          $form["sku_bill_table_$key"]['#rows'][$val1['ad_bill_number']][] = $val1['to_be_returned_qty'];

          $actual_returned_qty1 = array(
            '#type' => 'textfield',
            '#size' => 10,
            '#attributes' => array('class' => array('allownumericwithoutdecimal', "actual-return-qty1 pop-up-return-qty_$key")),
            '#default_value' => $val1['return_qty'],
          );

          $reason_type = $val1['res_status'];
          $reason_list = _get_bill_status_wise_reason_master_list($reason_type);
          /*if ($reason_type == 'undeliver') {
           $search = trim($val1['res_reason_for_failure']);
          }
          else {
             $search = trim($val1['res_sku_reason']);
          }
          $check_key_exist = array_search(strtolower($search), array_map('strtolower', $reason_list));
          // if farey reson_for_failure not equal to dispatch_planning _reason_list
          if (!empty($check_key_exist)) {
            unset($reason_list[$check_key_exist]);
            $reason_list["$search"] = $search;
          }*/
          //$reason_list = array("Damaged Product"=>"Damaged Product","No order/Wrong Order"=>"No order/Wrong Order","Outlet Closed"=>"Outlet Closed");
          if ($reason_type == 'undeliver') {
            $failure_reason = trim($val1['res_reason_for_failure']);
            $reason_code = array(
              '#type' => 'select',
              '#default_value' =>"$failure_reason",
              '#options' =>$reason_list,
              '#attributes' =>array('class' => array("pop-up-reason-code")),
            );
          }
          else {
            $reason_code = array(
              '#type' => 'select',
              '#default_value' =>"$val1[res_sku_reason]",
              '#options' => $reason_list,
              '#attributes' => array('class' => array("pop-up-reason-code")),
            );
          }

          $cashier_short = array(
            '#type' => 'textfield',
            '#size' => 10,
            '#default_value' => $val1['res_cashier_short'],
            '#attributes' => array('class' => array('allownumericwithoutdecimal', 'cashier-qty')),
          );

          $claimed_godown_short = array(
            '#type' => 'textfield',
            '#size' => 10,
            '#attributes' => array('class' => array('allownumericwithoutdecimal', 'claim-godown-short')),
            '#default_value' => $val1['claimed_godown_short'],
          );

          $claim_reason_intial_option = array('' => 'Select');
          $claim_reason_options = get_gs_claim_reason_list($distributor_id);
          if (!empty($claim_reason_options)) {
            $claim_reason_intial_option = $claim_reason_intial_option + $claim_reason_options;
          }
          $reason_for_claim = array(
            '#type' => 'select',
            '#options' => $claim_reason_intial_option,
            '#attributes' => array('class' => array('claim-godown-short-reason')),
            '#default_value' => $val1['claimed_godown_short_reason'],
          );

          $form["sku_bill_table_$key"][$val1['ad_bill_number']]["actual_returned_qty1"] = &$actual_returned_qty1;
          $form["sku_bill_table_$key"][$val1['ad_bill_number']]["reason_code"] = &$reason_code;
          $form["sku_bill_table_$key"][$val1['ad_bill_number']]["cashier_short"] = &$cashier_short;
          $form["sku_bill_table_$key"][$val1['ad_bill_number']]["claimed_godown_short"] = &$claimed_godown_short;
          $form["sku_bill_table_$key"][$val1['ad_bill_number']]["reason_for_claim"] = &$reason_for_claim;

          $form["sku_bill_table_$key"]['#rows'][$val1['ad_bill_number']][]['data']["actual_returned_qty1"] = &$actual_returned_qty1;
          $form["sku_bill_table_$key"]['#rows'][$val1['ad_bill_number']][]['data']["reason_code"] = &$reason_code;
          $form["sku_bill_table_$key"]['#rows'][$val1['ad_bill_number']][]['data']["cashier_short"] = &$cashier_short;
          $form["sku_bill_table_$key"]['#rows'][$val1['ad_bill_number']][]['data']["claimed_godown_short"] = &$claimed_godown_short;
          $form["sku_bill_table_$key"]['#rows'][$val1['ad_bill_number']][]['data']["reason_for_claim"] = &$reason_for_claim;

          unset($res_godown_short);
          unset($dispatch_qty);
          unset($actual_returned_qty1);
          unset($reason_code);
          unset($cashier_short);
          unset($claimed_godown_short);
          unset($reason_for_claim);
        }
      }

      /*       * ***********************END POP UP BILL TABLE************************ */
    }

    if ($form_state['triggering_element']['#value'] == 'Apply') {
      $form_state['apply_clicked'] = 1;
      unset($form_state['excess_inventory']);
      unset($form_state['input']['excess_sku']);
    }
    if (!empty($form_state['apply_clicked'])) {
      $form['excess_inventory_table'] = array(
        '#type' => 'fieldset',
        '#title' => t('Excess Inventory Declaration'),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
      );

      $form['excess_inventory_table']['excess_sku']['#prefix'] = '<div id="excess-declaration-screen">';
      $form['excess_inventory_table']['excess_sku']['#suffix'] = '</div>';

      $header1 = array('Product Name', 'MRP', 'Excess Qty', 'Credit To Cashier', 'Excess Reason', 'Remarks');

      $form['excess_inventory_table']['excess_sku'] = array(
        '#theme' => 'table',
        '#tree' => TRUE,
        '#header' => $header1,
        '#rows' => array(),
        '#prefix' => '<div id="bottom_middle_table">',
        '#suffix' => '</div>',
      );

      $excess_declaration_data = get_excess_declaration_data($trip_number, $distributor_id);

      if (!isset($form_state['excess_inventory'])) {
        $form_state['excess_inventory'] = count($excess_declaration_data);
      }

      $counter = $form_state['excess_inventory'];

      for ($i = 0; $i <= $counter; $i++) {
        $product_name = array(
          '#type' => 'textfield',
          '#autocomplete_path' => "all_products/autocomplete",
          '#attributes' => array('class' => array('product-name')),
          '#default_value' => $excess_declaration_data[$i]['product_name'],
          '#ajax' => array(
            'callback' => 'trip_details_return_callback',
            'wrapper' => 'itemlist-div',
            'method' => 'replace',
            'effect' => 'fade',
          ),
        );

        if (!empty($excess_declaration_data[$i]['product_name'])) {
          $input_product_name = $excess_declaration_data[$i]['product_name'];
        }
        else {
          $input_product_name = $form_state['input']['excess_sku'][$i]['product_name'];
        }
        $initial_option = array('' => 'Select');
        $option = get_mrp_from_product_name($input_product_name, $distributor_id);
        $default_mrp = '';
        if (count($option) == 1) {
          $default_mrp = reset($option);
        }
        elseif (count($option) > 1 && isset($form_state['input']['excess_sku'][$i]['mrp'])) {
          $default_mrp = $form_state['input']['excess_sku'][$i]['mrp'];
        }
        if (!empty($option)) {
          $initial_option = $initial_option + $option;
        }
        if (isset($form_state['input']['excess_sku'][$i]['mrp'])) {
          unset($form_state['input']['excess_sku'][$i]['mrp']);
        }

        $mrp = array(
          '#type' => 'select',
          '#default_value' => $default_mrp,
          '#options' => $initial_option,
          '#ajax' => array(
            'callback' => 'trip_details_return_callback',
            'wrapper' => 'itemlist-div',
            'method' => 'replace',
            'effect' => 'fade',
          ),
          '#attributes' => array('class' => array('mrp-list')),
        );

        $excess_qty = array(
          '#type' => 'textfield',
          '#attributes' => array('class' => array('allownumericwithoutdecimal', 'excess-qty')),
          '#default_value' => $excess_declaration_data[$i]['excess_qty'],
          '#size' => 10,
        );

        $credit_to_cashier = array(
          '#type' => 'checkbox',
          '#attributes' => array('class' => array('credit-to-cashier')),
          '#default_value' => $excess_declaration_data[$i]['credit_to_cashier'],
        );

        $reason_options = array('' => 'Select');
        $excess_reason_options = get_excess_reason_list($distributor_id);
        if (!empty($excess_reason_options)) {
          $reason_options = $reason_options + $excess_reason_options;
        }
        $excess_reason = array(
          '#type' => 'select',
          '#options' => $reason_options,
          '#attributes' => array('class' => array('excess-reason')),
          '#default_value' => $excess_declaration_data[$i]['excess_reason'],
        );

        $remarks = array(
          '#type' => 'textfield',
          '#attributes' => array('class' => array('remarks')),
          '#default_value' => $excess_declaration_data[$i]['remarks'],
          '#size' => 10,
        );

        $form['excess_inventory_table']['excess_sku'][$i]["product_name"] = &$product_name;
        $form['excess_inventory_table']['excess_sku'][$i]["mrp"] = &$mrp;
        $form['excess_inventory_table']['excess_sku'][$i]["excess_qty"] = &$excess_qty;
        $form['excess_inventory_table']['excess_sku'][$i]["credit_to_cashier"] = &$credit_to_cashier;
        $form['excess_inventory_table']['excess_sku'][$i]["excess_reason"] = &$excess_reason;
        $form['excess_inventory_table']['excess_sku'][$i]["remarks"] = &$remarks;

        $form['excess_inventory_table']['excess_sku']['#rows'][$i][]['data']["product_name"] = &$product_name;
        $form['excess_inventory_table']['excess_sku']['#rows'][$i][]['data']["mrp"] = &$mrp;
        $form['excess_inventory_table']['excess_sku']['#rows'][$i][]['data']["excess_qty"] = &$excess_qty;
        $form['excess_inventory_table']['excess_sku']['#rows'][$i][]['data']["credit_to_cashier"] = &$credit_to_cashier;
        $form['excess_inventory_table']['excess_sku']['#rows'][$i][]['data']["excess_reason"] = &$excess_reason;
        $form['excess_inventory_table']['excess_sku']['#rows'][$i][]['data']["remarks"] = &$remarks;

        unset($product_name);
        unset($mrp);
        unset($excess_qty);
        unset($credit_to_cashier);
        unset($excess_reason);
        unset($remarks);
      }
      $form['excess_inventory_table']['add_more_button'] = array(
        '#type' => 'submit',
        '#value' => t('Add item'),
        '#submit' => array('inventory_table_add_more'),
        '#ajax' => array(
          'callback' => 'trip_details_return_callback',
          'wrapper' => 'itemlist-div',
          'method' => 'replace',
          'effect' => 'fade',
        ),
      );
      if ($form_state['bill-wise'] == 1) {
        // Dispatch Supervisor
        $incharge_name_option = array('' => 'Select');
        $dispatch_supervisor_options = get_dispatch_supervisor_list($distributor_id);
        if (!empty($dispatch_supervisor_options)) {
          $dispatch_supervisor_name_option = $incharge_name_option + $dispatch_supervisor_options;
        }
        $form['return_incharge_name'] = array(
          '#title' => t('Dispatch Supervisor:'),
          '#type' => 'select',
          '#options' => $dispatch_supervisor_name_option,
          '#attributes' => array('class' => array('return-incharge-name')),
        );
      }
      else {
        $incharge_name_option = array('' => 'Select');
        $return_incharge_options = get_returns_incharge_list($distributor_id);
        if (!empty($return_incharge_options)) {
          $incharge_name_option = $incharge_name_option + $return_incharge_options;
        }
        $form['return_incharge_name'] = array(
          '#title' => t('Returns Incharge Name:'),
          '#type' => 'select',
          '#options' => $incharge_name_option,
          '#attributes' => array('class' => array('return-incharge-name')),
        );
      }
      $form['save'] = array(
        '#type' => 'submit',
        '#value' => 'Save',
//        '#ajax' => array(
//          'callback' => 'trip_details_return_callback',
//          'wrapper' => 'itemlist-div',
//          'method' => 'replace',
//          'effect' => 'fade',
//        ),
        '#validate' => array('save_sku_wise_return_validate'),
        '#submit' => array('save_sku_wise_return_submit'),
      );
// Submit button will be shown only in sku-wise-return-new/bill-wise screen
      if ($form_state['bill-wise'] != 1) {
        $form['sku_wise_return_submit_btn'] = array(
          '#type' => 'submit',
          '#value' => 'Submit',
          '#name' => 'sku_wise_return_submit_btn',
//        '#ajax' => array(
//          'callback' => 'trip_details_return_callback',
//          'wrapper' => 'itemlist-div',
//          'method' => 'replace',
//          'effect' => 'fade',
//        ),
          '#validate' => array('save_sku_wise_return_final_validate_version'),
          '#submit' => array('save_sku_wise_return_final_submit_version'),
        );
      }


      if ($form_state['bill-wise'] == 1) {
        $form['submit'] = array(
          '#type' => 'submit',
          '#value' => 'Submit',
//        '#ajax' => array(
//          'callback' => 'trip_details_return_callback',
//          'wrapper' => 'itemlist-div',
//          'method' => 'replace',
//          'effect' => 'fade',
//        ),
          '#validate' => array('sku_wise_return_validate'),
          '#submit' => array('sku_wise_return_submit'),
        );
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
  //return $form;  
  $commands[] = ajax_command_replace('#itemlist-div', render($form));
  $commands[] = ajax_command_replace('.alert-danger', '');
  $commands[] = ajax_command_replace('.alert-success', '');
  $commands[] = ajax_command_prepend('#itemlist-div', theme('status_messages'));
  return array('#type' => 'ajax', '#commands' => $commands);
}

function inventory_table_add_more($form, &$form_state) {
  $form_state['excess_inventory'] ++;
  $form_state['rebuild'] = TRUE;
  //return $form;
}

//function add_more_excess_inventory_table_callback($form, &$form_state) {
//  return $form['excess_inventory_table']['excess_sku'];
//  //return $form;
//}

function trip_number_validation($form, &$form_state) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);
  $form_state['trip_no_error'] = '';
  $trip_number = $form_state['input']['trip_number'];

  $query = db_select('dispatch_planning_api_data', 'ad');
  $query->condition('ad.runsheet_number', $trip_number, '=');
  $query->condition('ad.distributor_id', $dis_id);
  $query->fields('ad', array('id'));
  $result = $query->execute()->fetchField();
  if (empty($result) && !empty($trip_number)) {
    drupal_set_message(t("Trip ID:$trip_number has not been dispatched yet."), 'error');
    $form_state['trip_no_error'] = 1;
  }

  $query = db_select('dispatch_planning_api_data', 'ad');
  $query->condition('ad.runsheet_number', $trip_number, '=');
  $query->condition('ad.delivery_pending_flag', 1, '!=');
  $query->condition('ad.distributor_id', $dis_id);
  $query->isNull('ad.res_status');
  $query->addExpression('GROUP_CONCAT(ad.bill_number)', 'bill_numbers');
  $res = $query->execute()->fetchField();
  if ($res) {
    drupal_set_message(t("Please declare delivery outcome for all the bills in this Trip ID:$trip_number"), 'error');
    $form_state['trip_no_error'] = 1;
  }
  $trip_detail = check_trip_id_exists_or_not($dis_id, $trip_number);
  $current_vrs_status = $trip_detail->field_vrs_status_value;
  $field_vrs_status = field_info_field('field_vrs_status');
  $vrs_status = list_allowed_values($field_vrs_status);
  if ($form_state['bill-wise'] == 1) {
    unset($vrs_status[6]); // Return Bill Wise Confirmation Pending
  }
  else {
    unset($vrs_status[0]); // Returns SKU wise Confirmation Pending
  }

  if (isset($vrs_status[$current_vrs_status])) {
    drupal_set_message("You cannot edit Trip ID :$trip_number is in $vrs_status[$current_vrs_status] state.", 'error');
    $form_state['trip_no_error'] = 1;
  }
  /* if ($vrs_status == 1) {
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
    } */
}
// exclude dp bills in return
function new_sku_wise_return_submit($form, &$form_state) {
  if ($form_state['trip_no_error'] != 1) {
    global $user;
    $dis_id = change_feeds_import_get_distributor_id($user->uid);
    $trip_id = $form_state['values']['trip_number'];
    $selected_bill_status = $form_state['values']['bill_status'];
    $sub_putaway_group = $form_state['values']['sub_putaway_group'];
    $putaway_group = $form_state['values']['putaway_group'];
    $query = db_select('dispatch_planning_item_details', 'dpi');
    $query->join('dispatch_planning_api_data', 'ad', 'dpi.trip_bill_reference = ad.id');
    $query->condition('ad.delivery_pending_flag', 1, '!=');
    $query->condition('ad.runsheet_number', $trip_id);
    $query->condition('ad.distributor_id', $dis_id);
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
    if (!empty($sub_putaway_group)) {
      $query->join('node', 'n', 'n.title = dpi.sku_code and n.uid = ad.distributor_id');
      $query->condition('n.type', 'sku7_code_master', '=');
      $query->join('field_data_field_sub_putaway_group', 'spg', 'spg.entity_id = n.nid');
      $query->condition('spg.field_sub_putaway_group_nid', $sub_putaway_group, '=');
    }
    if (!empty($putaway_group) && empty($sub_putaway_group)) {
      $sub_putaway_nids = get_nids_of_selected_putaway($putaway_group, $dis_id);

      $query->join('node', 'n', 'n.title = dpi.sku_code and n.uid = ad.distributor_id');
      $query->condition('n.type', 'sku7_code_master', '=');
      $query->join('field_data_field_sub_putaway_group', 'spg', 'spg.entity_id = n.nid');
      $query->condition('spg.field_sub_putaway_group_nid', $sub_putaway_nids, 'IN');
    }

    $query->fields('dpi', array('sku_code'));
    $query->fields('dpi', array('product_name'));
    $query->fields('dpi', array('mrp'));
    $query->fields('ad', array('updated_flag'));
    // net sales qty
    //$query->fields('dpi', array('sum_net_sales_quantity'));
    // farey delivered qty
    //$query->fields('dpi', array('farey_delivered_qty'));
    // Actual delivered qty both from farey or from manual edit screen.
    //$query->fields('dpi', array('res_sku_actual_quantity1'));

    $query->addExpression('SUM(dpi.res_sku_actual_quantity1)', 'res_sku_actual_quantity1');
    $query->addExpression('SUM(dpi.farey_delivered_qty)', 'farey_delivered_qty');
    $query->addExpression('SUM(dpi.sum_net_sales_quantity)', 'sum_net_sales_quantity');

    //changed it to actual deliverd qty 
    //$query->addExpression('SUM(dpi.sum_net_sales_quantity - dpi.farey_delivered_qty)', 'to_be_return_qty');
    //$query->addExpression('SUM(dpi.sum_net_sales_quantity - dpi.res_sku_actual_quantity1)', 'to_be_return_qty');
    $query->groupBy('dpi.sku_code');
    $query->orderBy('dpi.product_name');
    $result = $query->execute()->fetchAll(PDO::FETCH_GROUP | PDO::FETCH_UNIQUE | PDO::FETCH_ASSOC);

    /*  In both SKU-wise screen and SKU-Bill wise screen, 
     *  To Be returned qty should be = Net Sales Qty -  DP lines farey_delivered_qty
     *  if Updated flag = Api else To Be returned qty should be = Net Sales Qty - DP lines
     *  res_sku_actual_quantity1
     */
    foreach ($result as $key => &$val) {
      // Manual update from manual edit screen
      if ($val['updated_flag'] == 1) {
        $val['to_be_return_qty'] = $val['sum_net_sales_quantity'] - $val['res_sku_actual_quantity1'];
      }
      // Api  update from Farey screen
      else if ($val['updated_flag'] == 2) {
        $val['to_be_return_qty'] = $val['sum_net_sales_quantity'] - $val['farey_delivered_qty'];
      }

      // Not required records if to be return qty is zero.
      if ($val['to_be_return_qty'] == 0) {
        unset($result[$key]);
      }
    }
    if (!empty($form_state['values']['show_skus'])) {
      $subquery = db_select('temp_sku_wise_return', 'tr');
      $subquery->condition('tr.trip_id', $trip_id);
      $subquery->condition('tr.distributor_id', $dis_id);
      $subquery->fields('tr', array('sku_code'));
      $subquery->fields('tr', array('actual_return_qty'));
      $res = $subquery->execute()->fetchAllKeyed(0, 1);
      foreach ($result as $key => &$val) {
        if ($val['to_be_return_qty'] == $res[$key]) {
          unset($result[$key]);
        }
      }
    }
    // Capture Return Start Time
    $form_state['returns_start_time'] = time();

    $form_state['sku_wise_return_confirm']['SKUs'] = $result;
  }

  $form_state['rebuild'] = TRUE;
}

function save_sku_wise_return_validate($form, &$form_state) {
  // watchdog('form_validate', '<pre>' . print_r($form_state, TRUE) . '</pre>');
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);

  $form_state['sku_excess_error'] = 0;
  foreach ($form_state['input']['bill_details'] as $sku => $value) {
    $dispatched_qty = 0;
    if (!empty($value['actual_returned_qty'])) {
      if (!is_numeric($value['actual_returned_qty'])) {
        $form_state['sku_excess_error'] = 1;
        drupal_set_message(t("Please enter valid actual return quantity for sku: " . $sku), 'error');
      }
    }
    if (is_numeric($value['actual_returned_qty']) && (floor($value['actual_returned_qty']) != $value['actual_returned_qty'])) {
      $form_state['sku_excess_error'] = 1;
      drupal_set_message(t("Actual return quantity should not be decimal for sku: " . $sku), 'error');
    }
    $trip_number = $form_state['values']['trip_number'];
    $main_page_actual_backend_return_qty = sku_wise_return_pop_up_table($trip_number, $sku, $dis_id);
    if (!empty($main_page_actual_backend_return_qty)) {
      foreach ($main_page_actual_backend_return_qty as $aqty => $aqty_value) {
        $dispatched_qty += $aqty_value['dispatch_qty'];
      }
      // total dispatched qty of that sku in all bills  should not greater than actual_returned_qty
      if ($dispatched_qty < $value['actual_returned_qty'] && !empty($value['actual_returned_qty'])) {
        drupal_set_message(t("Actual Returned Qty should not be greater than Dispatched quantity  for sku: " . $sku), 'error');
        $form_state['sku_excess_error'] = 1;
      }
    }
    $check_to_be_return = 0;
    foreach ($form_state['input']['bill_details'] as $sku => $value) {
      if ($value['actual_returned_qty'] != $form_state['sku_wise_return_confirm']['SKUs'][$sku]['to_be_return_qty']) {
        $check_to_be_return = 1;
        break;
      }
    }
  
  // if to_be_return_qty is equal to actual return qty of all sku then trip status will be return confirmed and version create and return incharge name is mandatory
    if ($check_to_be_return == 0 && empty($form_state['input']['return_incharge_name'])) {
        $form_state['rebuild'] = TRUE;
        form_set_error('return_incharge_name', "Please select Return Incharge Name.");
    }
  }

  // Excess Inventory Table Validation
  foreach ($form_state['input']['excess_sku'] as $k => $value) {
    if (!empty($value['product_name'])) {
      $check_product_name_exists = check_product_name($dis_id, $value['product_name']);
      if (empty($check_product_name_exists)) {
        $form_state['sku_excess_error'] = 1;
        drupal_set_message(t("This product name " . $value['product_name'] . " does not exists."), 'error');
      }
      if (empty($value['excess_qty'])) {
        $form_state['sku_excess_error'] = 1;
        drupal_set_message(t("Please enter excess quantity for product: " . $value['product_name']), 'error');
      }
      if (!empty($value['excess_qty']) && !is_numeric($value['excess_qty'])) {
        $form_state['sku_excess_error'] = 1;
        drupal_set_message(t("Excess qty should be numeric for product: " . $value['product_name']), 'error');
      }
      if (is_numeric($value['excess_qty']) && (floor($value['excess_qty']) != $value['excess_qty'])) {
        $form_state['sku_excess_error'] = 1;
        drupal_set_message(t("Actual return quantity should not be decimal for product: " . $value['product_name']), 'error');
      }
      if (!empty($value['credit_to_cashier']) && empty($value['excess_reason'])) {
        $form_state['sku_excess_error'] = 1;
        drupal_set_message(t("Please enter excess reason for product: " . $value['product_name']), 'error');
      }
    }
  }

  if ($form_state['sku_excess_error'] == 1) {
    $form_state['rebuild'] = TRUE;
  }
}

function get_to_be_returned_qty_undeliver_bill($trip_number, $bill_number, $sku_code, $dis_id) {
  $to_be_returned_qty = 0;
  $bill_status = array('undeliver','partial_deliver');
  if (!empty($trip_number) && !empty($bill_number) && !empty($sku_code) && !empty($dis_id)) {
    $query = db_select('dispatch_planning_item_details', 'dpi');
    $query->join('dispatch_planning_api_data', 'ad', 'dpi.trip_bill_reference = ad.id');
    $query->condition('ad.delivery_pending_flag', 1, '!=');
    $query->condition('ad.runsheet_number', $trip_number, '=');
    $query->condition('ad.distributor_id', $dis_id);
    $query->condition('dpi.sku_code', $sku_code, '=');
    $query->condition('ad.res_status', $bill_status, 'IN');
    $query->condition('ad.bill_number', $bill_number);
    $query->fields('ad', array('bill_number'));
    $query->fields('dpi', array('sum_net_sales_quantity'));
    $query->fields('ad', array('updated_flag'));
    // net sales qty
    $query->fields('dpi', array('sum_net_sales_quantity'));
    // farey delivered qty
    $query->fields('dpi', array('farey_delivered_qty'));
    // Actual delivered qty both from farey or from manual edit screen.
    $query->fields('dpi', array('res_sku_actual_quantity1'));
    $result = $query->execute()->fetchAll(PDO::FETCH_GROUP | PDO::FETCH_UNIQUE | PDO::FETCH_ASSOC);
    if (!empty($result)) {
      foreach ($result as $key => &$val) {
        // udate from farey table
        /*  In both SKU-wise screen and SKU-Bill wise screen, 
         *  To Be returned qty should be = Net Sales Qty -  DP lines farey_delivered_qty
         *  if Updated flag = Api else To Be returned qty should be = Net Sales Qty - DP lines
         *  res_sku_actual_quantity1
         */

        // Manual update from manual edit screen
        if ($val['updated_flag'] == 1) {
          $to_be_returned_qty = $val['sum_net_sales_quantity'] - $val['res_sku_actual_quantity1'];
        }
        else if ($val['updated_flag'] == 2) {
          $to_be_returned_qty = $val['sum_net_sales_quantity'] - $val['farey_delivered_qty'];
        }
      }
    }
  }
  return $to_be_returned_qty;
}

// if to be return qty is equal to actual return qty then
function save_return_final_version($trip_id, $dis_id, $returns_incharge_name = '') {
  if (!empty($trip_id) && !empty($dis_id)) {
    $trip_nid = check_trip_id_exists_or_not($dis_id, $trip_id);
    if (!empty($trip_nid->nid)) {
      $node = node_load($trip_nid->nid);
      // Status updated to 'Returned Confirm'
      $node->field_vrs_status['und'][0]['value'] = 1;
      $returns_version = $node->field_returned_version['und'][0]['value'];
      if (empty($returns_version)) {
        $version_no = 'V_1.0';
        $node->field_returned_version['und'][0]['value'] = $version_no;
      }
      // version increment
      else {
        $exploded_val = explode('_', $returns_version);
        $updated_version = $exploded_val[1] + 0.1;
        $version_no = "V_$updated_version";
        $node->field_returned_version['und'][0]['value'] = $version_no;
      }
      $sku_returns_version = $node->field_sku_return_version['und'][0]['value'];
      if (empty($sku_returns_version)) {
        $sku_version_no = 'V_1.0';
        $node->field_sku_return_version['und'][0]['value'] = $sku_version_no;
      }
      // version increment
      else {
        $exploded_val = explode('_', $sku_returns_version);
        $updated_version = $exploded_val[1] + 0.1;
        $sku_version_no = "V_$updated_version";
        $node->field_sku_return_version['und'][0]['value'] = $sku_version_no;
      }
      unset($node->field_bill_wise_return_end_time[LANGUAGE_NONE][0]['value']);
      $node->field_returns_closure_id['und'][0]['value'] = "RC$trip_id";
      $node->field_returns_incharge_name['und'][0]['value'] = $returns_incharge_name;
      node_save($node);

      // Insert the Returns Incharge Name in Fareye Table
      db_update('dispatch_planning_api_data') // Table name no longer needs {}
        ->fields(array(
          'returns_incharge_name' => $returns_incharge_name,
        ))
        ->condition('runsheet_number', $trip_id, '=')
        ->condition('distributor_id', $dis_id, '=')
        ->execute();
      $query = db_select('dispatch_planning_api_data', 'ad');
      $query->condition('ad.runsheet_number', $trip_id);
      $query->condition('ad.distributor_id', $dis_id);
      $query->fields('ad');
      $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

      $bills_items = get_bill_items_details($trip_id, $dis_id);

      // add the header level data in new versioning header level table
      foreach ($res as $key => $val) {
        $query = db_insert('inventory_versioning_api_data');
        $cur_id = $val['id'];
        unset($val['id']);
        $val['version_created_time'] = REQUEST_TIME;
        $val['version_no'] = $version_no;
        $val['version_flag'] = 2;
        $query->fields($val);
        $result_id = $query->execute();

        $fields = array(
          'tur',
          'mrp',
          'product_name',
          'sum_net_sales_cases',
          'sum_net_sales_quantity',
          'sku_code',
          'sum_net_sales_kgs',
          'sum_net_sales_value',
          'sku_description',
          'trip_bill_reference',
          'res_sku_actual_quantity1',
          'og_sum_net_sales_quantity',
          'res_sku_reason',
          'photo_of_signed_bill_partial',
          'updated_flag',
          'res_godown_short',
          'res_cashier_short',
          'dispatch_qty',
          'return_qty',
          'farey_delivered_qty',
          'money_collect_type',
          'line_level_status',
          'claimed_godown_short',
          'claimed_godown_short_reason',
          'approved_quantity',
          'approval_flag',
          'godown_short_reason',
          'approved_by',
          'remarks',
        );
        // add the child level data in new versioning child level table
        if (!empty($result_id)) {
          $query1 = db_insert('inventory_versioning_item_details')->fields($fields);
          foreach ($bills_items[$cur_id] as $value1) {
            unset($value1['id']);
            $value1['trip_bill_reference'] = $result_id;
            $query1->values($value1);
          }
          $query1->execute();
        }
      }
    }
  }
  return $version_no;
}

//Save btn function
function save_sku_wise_return_submit($form, &$form_state) {
  if (empty($form_state['sku_excess_error'])) {
    global $user;
    $dis_id = change_feeds_import_get_distributor_id($user->uid);
    $trip_id = $form_state['values']['trip_number'];
    $returns_start_time = $form_state['returns_start_time'];
    $returns_incharge_name = $form_state['input']['return_incharge_name'];
    // Merging the sku data in temp table
    merge_sku_data_in_temp_table($form_state['input']['bill_details'], $trip_id, $dis_id);
    $existing_bill_wise_return_start_time = $existing_bill_wise_return_start_time = 0;
    // Save sku wise return start time
    if ($form_state['bill-wise'] != 1) {
      $existing_return_start_time = check_skuwise_return_start_time_present($trip_id, $dis_id);
      // Returns start time empty then save return start time in db
      $type = 'sku_wise';
      $return_name = 'skuwise_return_start_time';
      if (empty($existing_return_start_time)) {
        save_returns_start_time($type, $trip_id, $returns_start_time, $dis_id);
        save_return_time($trip_id, $return_name,$returns_start_time, $version = null, $dis_id);
      }
      // insert data into return time history table
      else {
        save_return_time($trip_id, $return_name,$returns_start_time, $version = null, $dis_id);
      }
    }
    else {
      $type = 'bill_wise';
      $return_name = 'billwise_return_start_time';
      $existing_bill_wise_return_start_time = check_billwise_return_start_time_present($trip_id, $dis_id);
      if (empty($existing_bill_wise_return_start_time)) {
        save_returns_start_time($type, $trip_id, $returns_start_time, $dis_id);
        save_return_time($trip_id, $return_name,$returns_start_time, $version = null, $dis_id);
      }
       else {
        save_return_time($trip_id, $return_name,$returns_start_time, $version = null, $dis_id);
      }
    }
    //Saving the excess inventory declaration data
    store_excess_inventory_declaration_data($form_state['input']['excess_sku'], $trip_id, $dis_id);
    //print_r($form_state['sku_wise_return_confirm']['SKUs']);
    //print_r($form_state['no_discrepancy_sku']);
    //print_r($form_state['input']['bill_details']);
    // Checking To be return qty is equal to actual return qty
    $check_to_be_return = 0;
    foreach ($form_state['input']['bill_details'] as $sku => $value) {
      if ($value['actual_returned_qty'] != $form_state['sku_wise_return_confirm']['SKUs'][$sku]['to_be_return_qty']) {
        $check_to_be_return = 1;
        break;
      }
    }
    // if to_be_return_qty is equal to actual return qty of all sku then trip status will be return confirmed and version create
    if ($check_to_be_return == 0) {
      $version = save_return_final_version($trip_id, $dis_id, $returns_incharge_name);
      $end_time = time();
      $type = 'sku_wise';
      save_returns_end_time($type, $trip_id, $end_time, $dis_id);
      $return_name = 'bill_wise_return_end_time';
      save_return_time($trip_id,$return_name,$end_time,$version, $dis_id);
    }
    if ($form_state['values']['show_bills'] == 1 && empty($form_state['sku_excess_error'])) {
      foreach ($form_state['input']['bill_details'] as $sku => $value) {
        foreach ($form_state['input']["sku_bill_table_$sku"] as $bill => $val) {
          $query = db_select('dispatch_planning_item_details', 'dpi');
          $query->join('dispatch_planning_api_data', 'ad', 'dpi.trip_bill_reference = ad.id');
          $query->addfield('ad', 'res_status', 'res_status');
          $query->fields('dpi', array('trip_bill_reference'));
          $query->fields('dpi', array('id'));
          $query->condition('dpi.sku_code', $sku, '=');
          $query->condition('ad.bill_number', $bill, '=');
          $query->condition('ad.distributor_id', $dis_id, '=');
          $query->condition('ad.runsheet_number', $trip_id, '=');
          $itdatas = $query->execute()->fetchAll();
          $itdata = $itdatas[0];
          // Commented res_staus & line level status
          if ($itdata->res_status == 'undeliver') {
            $ret_cash = $val['actual_returned_qty1'] + $val['cashier_short']; //Return Qty + Cashier Short  
            $ret_cash_godown = $ret_cash + $val['claimed_godown_short']; //Return Qty + Cashier Short + claimed Godown Short
            $line_status = '';
            if ($ret_cash_godown == $val['dispatch_qty']) {
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
                ///No need to update actual delivered qty while approving return
               // 'res_sku_actual_quantity1' => (int) $val['dispatch_qty'] - $val['actual_returned_qty1'] - $val['cashier_short'] - $val['claimed_godown_short'],
                'return_qty' => (int) $val['actual_returned_qty1'],
                //'dispatch_qty' => $val['dispatch_qty'],
                'res_sku_reason' => ($line_status != 'line_undeliver') ? $val['reason_code'] : '',
                'res_cashier_short' => (int) $val['cashier_short'],
                'claimed_godown_short' => (int) $val['claimed_godown_short'],
                'claimed_godown_short_reason' => $val['reason_for_claim'],
                //'line_level_status' => $line_status, // line level status
                'declared_claimed_godown_short'=>(int) $val['claimed_godown_short']
              ))
              ->condition('sku_code', $sku, '=')
              ->condition('id', $itdata->id, '=')
              ->execute();

            if ($line_status == 'line_undeliver') {
              if (!empty($val['reason_code'])) {
                db_update('dispatch_planning_api_data') // Table name no longer needs {}
                  ->fields(array(
                    'res_reason_for_failure' => $val['reason_code'] ? $val['reason_code'] : '',
                    'updated_flag' => 1,
                    'updated_time' => REQUEST_TIME,
                  ))
                  ->condition('runsheet_number', $trip_id, '=')
                  ->condition('id', $itdata->trip_bill_reference, '=')
                  ->execute();
              }
              else {
                db_update('dispatch_planning_api_data') // Table name no longer needs {}
                  ->fields(array(
                    'updated_flag' => 1,
                    'updated_time' => REQUEST_TIME,
                  ))
                  ->condition('runsheet_number', $trip_id, '=')
                  ->condition('id', $itdata->trip_bill_reference, '=')
                  ->execute();
              }
            }
            else {
              db_update('dispatch_planning_api_data') // Table name no longer needs {}
                ->fields(array(
                  // 'res_status' => 'partial_deliver',
                  'res_reason_for_failure' => '',
                  'updated_flag' => 1,
                  'updated_time' => REQUEST_TIME,
                ))
                ->condition('runsheet_number', $trip_id, '=')
                ->condition('id', $itdata->trip_bill_reference, '=')
                ->execute();
            }
          }
          else if ($itdata->res_status == 'partial_deliver') {

            $ret_cash = $val['actual_returned_qty1'] + $val['cashier_short']; //Return Qty + Cashier Short  
            $ret_cash_godown = $ret_cash + $val['claimed_godown_short']; //Return Qty + Cashier Short + Claimed Godown Short
            $line_status = '';
            if ($ret_cash_godown == $val['dispatch_qty']) {
              $line_status = 'line_undeliver';
            }
            else if ($ret_cash_godown > 0) {
              $line_status = 'line_partial_deliver';
            }
            else if ($ret_cash_godown == 0) {
              $line_status = 'line_deliver';
            }
            // Sku wise screen


            db_update('dispatch_planning_item_details') // Table name no longer needs {}
              ->fields(array(
                ///No need to update actual delivered qty while approving return
               // 'res_sku_actual_quantity1' => (int) $val['dispatch_qty'] - $val['actual_returned_qty1'] - $val['cashier_short'] - $val['claimed_godown_short'],
                'return_qty' => (int) $val['actual_returned_qty1'],
                // 'dispatch_qty' => $val['dispatch_qty'],
                'res_sku_reason' => $val['reason_code'],
                'res_cashier_short' => (int) $val['cashier_short'],
                'claimed_godown_short' => (int) $val['claimed_godown_short'],
                'claimed_godown_short_reason' => $val['reason_for_claim'],
               // 'line_level_status' => $line_status, // line level status
                'declared_claimed_godown_short'=>(int) $val['claimed_godown_short']
              ))
              ->condition('sku_code', $sku, '=')
              ->condition('id', $itdata->id, '=')
              ->execute();
          }
        }
      }
    }
    drupal_set_message(t("Data saved successfully."));
    //ajax_redirect('sku-wise-return-new');
  }
}

// first screen submit validation
function save_sku_wise_return_final_validate_version($form, &$form_state) {
  // watchdog('form_validate', '<pre>' . print_r($form_state, TRUE) . '</pre>');
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);
  $trip_number = $form_state['values']['trip_number'];
  $form_state['sku_excess_error'] = 0;
  $temp_data = get_temp_data($trip_number, $dis_id);
  $actual_return_qty_not_declared = 0;
  foreach ($form_state['values']['bill_details'] as $sku => $value) {
    if (empty($value['actual_returned_qty']) && strlen($value['actual_returned_qty']) == 0) {
      $form_state['sku_excess_error'] = 1;
      drupal_set_message(t("Please declare actual return quantity for SKU:$sku"), 'error');
    }
  }
  foreach ($form_state['input']['bill_details'] as $sku => $value) {
    $dispatched_qty = 0;
    if (!empty($value['actual_returned_qty'])) {
      if (!is_numeric($value['actual_returned_qty'])) {
        $form_state['sku_excess_error'] = 1;
        drupal_set_message(t("Please enter valid actual return quantity for sku: " . $sku), 'error');
      }
    }
    if (is_numeric($value['actual_returned_qty']) && (floor($value['actual_returned_qty']) != $value['actual_returned_qty'])) {
      $form_state['sku_excess_error'] = 1;
      drupal_set_message(t("Actual return quantity should not be decimal for sku: " . $sku), 'error');
    }
    $main_page_actual_backend_return_qty = sku_wise_return_pop_up_table($trip_number, $sku, $dis_id);
    if (!empty($main_page_actual_backend_return_qty)) {
      foreach ($main_page_actual_backend_return_qty as $aqty => $aqty_value) {
        $dispatched_qty += $aqty_value['dispatch_qty'];
      }
      // total dispatched qty of that sku in all bills  should not greater than actual_returned_qty
      if ($dispatched_qty < $value['actual_returned_qty'] && !empty($value['actual_returned_qty'])) {
        drupal_set_message(t("Actual Returned Qty should not be greater than Dispatched quantity  for sku: " . $sku), 'error');
        $form_state['sku_excess_error'] = 1;
      }
    }

    if ($form_state['values']['show_bills'] == 1) {
      // Pop up table validation
      // if (empty($form_state['no_discrepancy_sku'][$sku])) {
      $actual_return_qty = $value['actual_returned_qty'];
      if (!empty($actual_return_qty) || $actual_return_qty === '0') {
        $total_qty_bill_wise = 0;

        foreach ($form_state['input']["sku_bill_table_$sku"] as $bill => $val) {
          $total_qty_bill_wise += $val['actual_returned_qty1'];
          if (!empty($actual_return_qty) || $actual_return_qty == 0) {
            if ($val['actual_returned_qty1'] > 0) {
              if (empty($val['reason_code'])) {
                $form_state['sku_excess_error'] = 1;
                drupal_set_message(t("Reason code of sku:$sku is mandatory as actual returned qty is greater than zero of bill: $bill"), 'error');
              }
            }

            if ($val['dispatch_qty'] < ($val['actual_returned_qty1'] + $val['cashier_short'] + $val['claimed_godown_short'])) {
              $form_state['sku_excess_error'] = 1;
              drupal_set_message(t("Return quantity,cashier short and claimed godown short for sku:$sku cannot be greater than Dispatch quantity of bill: $bill"), 'error');
            }

            if ($val['claimed_godown_short'] > 0) {
              if (empty($val['reason_for_claim'])) {
                $form_state['sku_excess_error'] = 1;
                drupal_set_message(t("Reason for claim of sku:$sku is mandatory as claimed godown short is greater than zero of bill: $bill"), 'error');
              }
            }
          }
        }
        if ($actual_return_qty != $total_qty_bill_wise) {
          drupal_set_message(t("Actual Returned Qty for SKU: $sku is not equal to Sum of Actual Returned Qty at Bill Level"), 'error');
          $form_state['sku_excess_error'] = 1;
        }
      }
      //}
    }
  }

  // Excess Inventory Table Validation
  foreach ($form_state['input']['excess_sku'] as $k => $value) {
    if (!empty($value['product_name'])) {
      $check_product_name_exists = check_product_name($dis_id, $value['product_name']);
      if (empty($check_product_name_exists)) {
        $form_state['sku_excess_error'] = 1;
        drupal_set_message(t("This product name " . $value['product_name'] . " does not exists."), 'error');
      }
      if (empty($value['excess_qty'])) {
        $form_state['sku_excess_error'] = 1;
        drupal_set_message(t("Please enter excess quantity for product: " . $value['product_name']), 'error');
      }
      if (!empty($value['excess_qty']) && !is_numeric($value['excess_qty'])) {
        $form_state['sku_excess_error'] = 1;
        drupal_set_message(t("Excess qty should be numeric for product: " . $value['product_name']), 'error');
      }
      if (is_numeric($value['excess_qty']) && (floor($value['excess_qty']) != $value['excess_qty'])) {
        $form_state['sku_excess_error'] = 1;
        drupal_set_message(t("Actual return quantity should not be decimal for product: " . $value['product_name']), 'error');
      }
      if (!empty($value['credit_to_cashier']) && empty($value['excess_reason'])) {
        $form_state['sku_excess_error'] = 1;
        drupal_set_message(t("Please enter excess reason for product: " . $value['product_name']), 'error');
      }
    }
  }

  if (empty($form_state['input']['return_incharge_name'])) {
    $form_state['rebuild'] = TRUE;
    form_set_error('return_incharge_name', "Please select Return Incharge Name.");
  }

  if ($form_state['sku_excess_error'] == 1) {
    $form_state['rebuild'] = TRUE;
  }
}

// first screen submit
function save_sku_wise_return_final_submit_version($form, &$form_state) {
  if (empty($form_state['sku_excess_error'])) {
    global $user;
    $dis_id = change_feeds_import_get_distributor_id($user->uid);
    $trip_id = $form_state['values']['trip_number'];
    $returns_start_time = $form_state['returns_start_time'];
    $existing_return_start_time = check_skuwise_return_start_time_present($trip_id, $dis_id);
    // Returns start time empty then save return start time in db
    $type = 'sku_wise';
    $return_name = 'skuwise_return_start_time';
    if (empty($existing_return_start_time)) {
      save_returns_start_time($type, $trip_id, $returns_start_time, $dis_id);
      save_return_time($trip_id, $return_name,$returns_start_time,null,$dis_id);
    }
    $end_time = time();
    save_returns_end_time($type, $trip_id, $end_time, $dis_id);
    // Merging the sku data in temp table
    merge_sku_data_in_temp_table($form_state['input']['bill_details'], $trip_id, $dis_id);
    //Saving the excess inventory declaration data
    store_excess_inventory_declaration_data($form_state['input']['excess_sku'], $trip_id, $dis_id);
    // Save Return incharge name
    $returns_incharge_name = $form_state['input']['return_incharge_name'];
    $trip_nid = check_trip_id_exists_or_not($dis_id, $trip_id);
    // if to_be_return_qty is equal to actual return qty of all sku then trip status will be return confirmed and version create
    $check_to_be_return = 0;
    foreach ($form_state['input']['bill_details'] as $sku => $value) {
      if ($value['actual_returned_qty'] != $form_state['sku_wise_return_confirm']['SKUs'][$sku]['to_be_return_qty']) {
        $check_to_be_return = 1;
        break;
      }
    }
    // if to_be_return_qty is equal to actual return qty of all sku then trip status will be return confirmed and version create
    if ($check_to_be_return == 0) {
      $version_no = save_return_final_version($trip_id, $dis_id, $returns_incharge_name);
      $end_time = time();
      $return_name = 'billwise_return_end_time';
      save_return_time($trip_id, $return_name,$end_time,$version_no,$dis_id);
    }
    if (!empty($trip_nid->nid) && $check_to_be_return == 1) {
      $node = node_load($trip_nid->nid);
      //6|Returns Bill wise Confirmation Pending
      $node->field_vrs_status['und'][0]['value'] = 6;

      $returns_version = $node->field_sku_return_version['und'][0]['value'];
      if (empty($returns_version)) {
        $version_no = 'V_1.0';
        $node->field_sku_return_version['und'][0]['value'] = $version_no;
      }
      // version increment
      else {
        $exploded_val = explode('_', $returns_version);
        $updated_version = $exploded_val[1] + 0.1;
        $version_no = "V_$updated_version";
        $node->field_sku_return_version['und'][0]['value'] = $version_no;
      }
      $node->field_returns_incharge_name['und'][0]['value'] = $returns_incharge_name;
      node_save($node);
      // Insert the Returns Incharge Name in Fareye Table
      db_update('dispatch_planning_api_data') // Table name no longer needs {}
        ->fields(array(
          'returns_incharge_name' => $returns_incharge_name,
        ))
        ->condition('runsheet_number', $trip_id, '=')
        ->condition('distributor_id', $dis_id, '=')
        ->execute();
      $return_name = 'skuwise_return_end_time';
      save_return_time($trip_id, $return_name,time(),$version_no,$dis_id);
      
    }
    If (!empty($existing_return_start_time)) {
        $return_name = 'skuwise_return_start_time';  
        save_return_time($trip_id, $return_name,$returns_start_time,$version_no,$dis_id);
     }
    save_sku_return_version($version_no, $form_state['input']['bill_details'], $trip_id, $dis_id);
    drupal_set_message(t("Data saved successfully."));
  }
}

function get_temp_data($trip_id, $distributor_id) {
  $result = array();
  if (!empty($trip_id) && !empty($distributor_id)) {
    $query = db_select('temp_sku_wise_return', 'temp_return');
    $query->condition('trip_id', $trip_id);
    $query->condition('distributor_id', $distributor_id);
    $query->fields('temp_return', array('sku_code'));
    $query->fields('temp_return', array('actual_return_qty'));
    $result = $query->execute()->fetchAllKeyed(0, 1);
  }
  return $result;
}

function sku_wise_return_validate($form, &$form_state) {
  //watchdog('validate1', '<pre>' . print_r($form_state, TRUE) . '</pre>');  
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);
  $trip_id = $form_state['values']['trip_number'];
  $temp_data = get_temp_data($trip_id, $dis_id);
  $query = db_select('dispatch_planning_item_details', 'dpi');
  $query->join('dispatch_planning_api_data', 'ad', 'dpi.trip_bill_reference = ad.id');
  $query->condition('ad.delivery_pending_flag', 1, '!=');
  $query->condition('dpi.dispatch_qty', 0, '>');
  $query->condition('ad.runsheet_number', $trip_id);
  $query->condition('ad.distributor_id', $dis_id);
  $query->condition('dpi.line_level_status', 'line_deliver', '!=');
  $query->condition('ad.res_status', 'deliver', '!=');
  $query->fields('dpi', array('sku_code'));
  $selected_sku = $query->execute()->fetchCol();
  $all_sku = array();
  $mess = '';
  $actual_return_qty_not_declared = 0;
  $result = array_diff($selected_sku, $all_sku);

  foreach ($form_state['values']['bill_details'] as $sku => $value) {
    if (empty($value['actual_returned_qty']) && strlen($value['actual_returned_qty']) == 0) {
      $form_state['rebuild'] = TRUE;
      $mess .= t("Please declare actual return quantity for SKU:$sku") . '<br>';
    }
  }
  if ($form_state['values']['show_bills'] != 1) {
    $form_state['rebuild'] = TRUE;
    //form_set_error('form', 'Show bills as well should be checked.');
    $mess .= t('Show bills as well should be checked.') . '<br>';
  }
  else {
    foreach ($form_state['input']['bill_details'] as $sku => $value) {
      if (!is_numeric($value['actual_returned_qty'])) {
        $form_state['rebuild'] = TRUE;
        //form_set_error('form', "Please enter valid actual return quantity for sku: " . $sku);
        $mess .= t("Please enter valid actual return quantity for sku:$sku") . '<br>';
      }
      if (is_numeric($value['actual_returned_qty']) && (floor($value['actual_returned_qty']) != $value['actual_returned_qty'])) {
        $form_state['rebuild'] = TRUE;
        //form_set_error('form', "Actual return quantity should not be decimal for sku: " . $sku);
        $mess .= t("Actual return quantity should not be decimal for sku:$sku") . '<br>';
      }
      //if (empty($form_state['no_discrepancy_sku'][$sku])) {
      //if (!empty($actual_return_qty) || $actual_return_qty == 0) {
      $actual_return_qty = $value['actual_returned_qty'];

      $total_qty_bill_wise = 0;
      $all_bills = sku_wise_return_pop_up_table($trip_id, $sku, $dis_id);
      foreach ($form_state['input']["sku_bill_table_$sku"] as $bill => $val) {

        // Bill level tobe_returned_qty of undelivered bill, it will set if
        //  the bill status is undeliver
        /*
         * In SKU-Bill wise screen, put a check that if an SKU is present in a bill 
         * for which bill status = undeliver, then To be returned qty 
         * of that bill should be = actual returned qty of that bill + cashier short of
         *  that bill + claimed godown short of that bill
         */
        $bill_level_tobe_returned_qty_undeliver = get_to_be_returned_qty_undeliver_bill($trip_id, $bill, $sku, $dis_id);
        //echo $bill_level_tobe_returned_qty_undeliver;die;
        if (!empty($bill_level_tobe_returned_qty_undeliver)) {
          $actual_returned_qty_in_pop = $val['actual_returned_qty1'] + $val['cashier_short'] + $val['claimed_godown_short'];
          if ($bill_level_tobe_returned_qty_undeliver != $actual_returned_qty_in_pop) {
            $form_state['sku_excess_error'] = 1;
            $mess.= t("To be returned qty of Bill Number:$bill of SKU:$sku should be equal to actual returned qty + cashier short + claimed godown short");
            drupal_set_message(t("To be returned qty of Bill Number:$bill of SKU:$sku should be equal to actual returned qty + cashier short + claimed godown short"), 'error');
          }
        }

        if (!empty($actual_return_qty) || $actual_return_qty == 0) {
          $total_qty_bill_wise += $val['actual_returned_qty1'];
          // checking bill level to-be return qty = equal to actual return qty
          if ($val['actual_returned_qty1'] > 0 && $all_bills[$bill]['to_be_returned_qty'] != $val['actual_returned_qty1']) {
            if (empty($val['reason_code'])) {
              $form_state['rebuild'] = TRUE;
              //form_set_error('form', "Reason code of sku:$sku is mandatory as actual returned qty is greater than zero of bill: $bill");
              $mess .= t("Reason code of sku:$sku is mandatory as actual returned qty is greater than zero of bill: $bill") . '<br>';
            }
          }

          if ($val['dispatch_qty'] < ($val['actual_returned_qty1'] + $val['cashier_short'] + $val['claimed_godown_short'])) {
            $form_state['rebuild'] = TRUE;
            //form_set_error('form', "Return quantity,cashier short and claimed godown short for sku:$sku cannot be greater than Dispatch quantity of bill: $bill");
            $mess .= t("Return quantity,cashier short and claimed godown short for sku:$sku cannot be greater than Dispatch quantity of bill: $bill") . '<br>';
          }

          if ($val['claimed_godown_short'] > 0) {
            if (empty($val['reason_for_claim'])) {
              $form_state['rebuild'] = TRUE;
              //form_set_error('form', "Reason for claim of sku:$sku is mandatory as claimed godown short is greater than zero of bill: $bill");
              $mess .= t("Reason for claim of sku:$sku is mandatory as claimed godown short is greater than zero of bill: $bill") . '<br>';
            }
          }
        }
      }

      if ($actual_return_qty != $total_qty_bill_wise) {
        $form_state['rebuild'] = TRUE;
        //Actual Returned Qty for SKU <> is not equal to Sum of Actual Returned Qty at Bill Level
        //form_set_error('form', "Actual Returned Qty for SKU: $sku is not equal to Sum of Actual Returned Qty at Bill Level");
        $mess .= t("Actual Returned Qty for SKU: $sku is not equal to Sum of Actual Returned Qty at Bill Level") . '<br>';
      }
      //}
      //}
    }
  }


  // Excess Inventory Table Validation
  foreach ($form_state['input']['excess_sku'] as $k => $value) {
    if (!empty($value['product_name'])) {
      $check_product_name_exists = check_product_name($dis_id, $value['product_name']);
      if (empty($check_product_name_exists)) {
        $form_state['sku_excess_error'] = 1;
        //drupal_set_message(t("This product name " . $value['product_name'] . " does not exists."), 'error');
        $mess .= t("This product name " . $value['product_name'] . " does not exists.") . '<br>';
      }
      if (empty($value['excess_qty'])) {
        $form_state['rebuild'] = TRUE;
        //form_set_error('form', "Please enter excess quantity for product: " . $value['product_name']);
        $mess .= t("Please enter excess quantity for product: " . $value['product_name']) . '<br>';
      }
      if (!empty($value['excess_qty']) && !is_numeric($value['excess_qty'])) {
        $form_state['rebuild'] = TRUE;
        //form_set_error('form', "Excess qty should be numeric for product: " . $value['product_name']);
        $mess .= t("Excess qty should be numeric for product: " . $value['product_name']) . '<br>';
      }
      if (is_numeric($value['excess_qty']) && (floor($value['excess_qty']) != $value['excess_qty'])) {
        $form_state['rebuild'] = TRUE;
        //form_set_error('form', "Actual return quantity should not be decimal for product: " . $value['product_name']);
        $mess .= t("Actual return quantity should not be decimal for product: " . $value['product_name']) . '<br>';
      }
      if (!empty($value['credit_to_cashier']) && empty($value['excess_reason'])) {
        $form_state['rebuild'] = TRUE;
        //form_set_error('form', "Please enter excess reason for product: " . $value['product_name']);
        $mess .= t("Please enter excess reason for product: " . $value['product_name']) . '<br>';
      }
    }
  }
  if (!empty($mess)) {
    $form_state['rebuild'] = TRUE;
    form_set_error('form', $mess);
  }
  if ($form_state['bill-wise'] == 1) {
    if (empty($form_state['input']['return_incharge_name'])) {
      $form_state['rebuild'] = TRUE;
      form_set_error('return_incharge_name', "Please select Dispatch Supervisor Name.");
    }
  }
}

//Submit btn
function sku_wise_return_submit($form, &$form_state) {
  //watchdog('submit1', '<pre>' . print_r($form_state, TRUE) . '</pre>');
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);
  $trip_id = $form_state['values']['trip_number'];
  $returns_end_time = time();
  $returns_start_time = $form_state['returns_start_time'];
  $existing_bill_wise_return_start_time = 0;
  $existing_bill_wise_return_start_time = check_billwise_return_start_time_present($trip_id, $dis_id);
  if ($form_state['values']['show_bills'] == 1) {
      if ($form_state['bill-wise'] == 1) {
      $return_name = 'billwise_return_start_time';  
      $type = 'bill_wise';
      $end_time = time();
      $existing_bill_wise_return_start_time = check_billwise_return_start_time_present($trip_id, $dis_id);
      if (empty($existing_bill_wise_return_start_time)) {
        save_returns_start_time($type, $trip_id, $returns_start_time, $dis_id);
        save_return_time($trip_id, $return_name, $returns_start_time, $version = null, $dis_id);
      } // Insert into return version table
      save_returns_end_time($type, $trip_id, $end_time, $dis_id);
    }
    foreach ($form_state['input']['bill_details'] as $sku => $value) {
      foreach ($form_state['input']["sku_bill_table_$sku"] as $bill => $val) {
        $query = db_select('dispatch_planning_item_details', 'dpi');
        $query->join('dispatch_planning_api_data', 'ad', 'dpi.trip_bill_reference = ad.id');
        $query->addfield('ad', 'res_status', 'res_status');
        $query->fields('dpi', array('trip_bill_reference'));
        $query->fields('dpi', array('id'));
        $query->condition('dpi.sku_code', $sku, '=');
        $query->condition('ad.bill_number', $bill, '=');
        $query->condition('ad.distributor_id', $dis_id, '=');
        $query->condition('ad.runsheet_number', $trip_id, '=');
        $itdatas = $query->execute()->fetchAll();
        $itdata = $itdatas[0];
        if ($itdata->res_status == 'undeliver') {
          //6. Comment the code where Bill Status/Line Status is changing when Bill Status = Undeliver
          $ret_cash = $val['actual_returned_qty1'] + $val['cashier_short']; //Return Qty + Cashier Short  
          $ret_cash_godown = $ret_cash + $val['claimed_godown_short']; //Return Qty + Cashier Short + claimed Godown Short
          $line_status = '';
          if ($ret_cash_godown == $val['dispatch_qty']) {
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
              //'res_sku_actual_quantity1' => (int) $val['dispatch_qty'] - $val['actual_returned_qty1'] - $val['cashier_short'] - $val['claimed_godown_short'],
              'return_qty' => (int) $val['actual_returned_qty1'],
              //'dispatch_qty' => $val['dispatch_qty'],
              'res_sku_reason' => ($line_status != 'line_undeliver') ? $val['reason_code'] : '',
              'res_cashier_short' => (int) $val['cashier_short'],
              'claimed_godown_short' => (int) $val['claimed_godown_short'],
              'claimed_godown_short_reason' => $val['reason_for_claim'],
              //'line_level_status' => $line_status, // line level status
              'declared_claimed_godown_short' => (int) $val['claimed_godown_short']
            ))
            ->condition('sku_code', $sku, '=')
            ->condition('id', $itdata->id, '=')
            ->execute();

          if ($line_status == 'line_undeliver') {
            if (!empty($val['reason_code'])) {
              db_update('dispatch_planning_api_data') // Table name no longer needs {}
                ->fields(array(
                  'res_reason_for_failure' => $val['reason_code'] ? $val['reason_code'] : '',
                  'updated_flag' => 1,
                  'updated_time' => REQUEST_TIME,
                ))
                ->condition('runsheet_number', $trip_id, '=')
                ->condition('id', $itdata->trip_bill_reference, '=')
                ->execute();
            }
            else {
              db_update('dispatch_planning_api_data') // Table name no longer needs {}
                ->fields(array(
                  'updated_flag' => 1,
                  'updated_time' => REQUEST_TIME,
                ))
                ->condition('runsheet_number', $trip_id, '=')
                ->condition('id', $itdata->trip_bill_reference, '=')
                ->execute();
            }
          }
          else {
            if ($form_state['bill-wise'] == 1) {
              db_update('dispatch_planning_api_data') // Table name no longer needs {}
                ->fields(array(
                  // 'res_status' => 'partial_deliver',
                  'res_reason_for_failure' => '',
                  'updated_flag' => 1,
                  'updated_time' => REQUEST_TIME,
                ))
                ->condition('runsheet_number', $trip_id, '=')
                ->condition('id', $itdata->trip_bill_reference, '=')
                ->execute();
            }
          }
        }
        else if ($itdata->res_status == 'partial_deliver') {

          $ret_cash = $val['actual_returned_qty1'] + $val['cashier_short']; //Return Qty + Cashier Short  
          $ret_cash_godown = $ret_cash + $val['claimed_godown_short']; //Return Qty + Cashier Short + Claimed Godown Short
          $line_status = '';
          if ($ret_cash_godown == $val['dispatch_qty']) {
            $line_status = 'line_undeliver';
          }
          else if ($ret_cash_godown > 0) {
            $line_status = 'line_partial_deliver';
          }
          else if ($ret_cash_godown == 0) {
            $line_status = 'line_deliver';
          }
          /*
            * 1. To be returned qty for each bill should be = actual returned qty of that bill + cashier short of that bill + claimed godown short of that bill. 
             This should be the check for both Bill status = undeliver and partial_deliver. This validation needs to be put on Submit and not on Save.
             2. Comment the code where we are updating the line level status (for both Bill Status undeliver and partial_deliver). Both at Save and Submit. This I have not tested though.
             3. Comment the code line marked in RED below. Comment this both at Save and Submit:res_sku_actual_quantity1' => (int) $val['dispatch_qty'] - $val['actual_returned_qty1'] - $val['cashier_short'] - $val['claimed_godown_short'],
           */

          db_update('dispatch_planning_item_details') // Table name no longer needs {}
            ->fields(array(
             // 'res_sku_actual_quantity1' => (int) $val['dispatch_qty'] - $val['actual_returned_qty1'] - $val['cashier_short'] - $val['claimed_godown_short'],
              'return_qty' => (int) $val['actual_returned_qty1'],
              //'dispatch_qty' => $val['dispatch_qty'],
              'res_sku_reason' => $val['reason_code'],
              'res_cashier_short' => (int) $val['cashier_short'],
              'claimed_godown_short' => (int) $val['claimed_godown_short'],
              'claimed_godown_short_reason' => $val['reason_for_claim'],
              //'line_level_status' => $line_status, // line level status
              'declared_claimed_godown_short' => (int) $val['claimed_godown_short']
            ))
            ->condition('sku_code', $sku, '=')
            ->condition('id', $itdata->id, '=')
            ->execute();
        }
      }
    }

    // Merging the sku data in temp table
    merge_sku_data_in_temp_table($form_state['input']['bill_details'], $trip_id, $dis_id);

    //Saving the excess inventory declaration data
    store_excess_inventory_declaration_data($form_state['input']['excess_sku'], $trip_id, $dis_id);

    $returns_incharge_name = $form_state['input']['return_incharge_name'];
    $trip_nid = check_trip_id_exists_or_not($dis_id, $trip_id);
    if (!empty($trip_nid->nid)) {
      $node = node_load($trip_nid->nid);
      // Status updated to 'Returned Confirm'
      $node->field_vrs_status['und'][0]['value'] = 1;

      $returns_version = $node->field_returned_version['und'][0]['value'];
      if (empty($returns_version)) {
        $version_no = 'V_1.0';
        $node->field_returned_version['und'][0]['value'] = $version_no;
      }
      // version increment
      else {
        $exploded_val = explode('_', $returns_version);
        $updated_version = $exploded_val[1] + 0.1;
        $version_no = "V_$updated_version";
        $node->field_returned_version['und'][0]['value'] = $version_no;
      }

      $node->field_returns_closure_id['und'][0]['value'] = "RC$trip_id";
      $node->field_return_dispatch_supervisor['und'][0]['nid'] = $returns_incharge_name;
      node_save($node);
      $return_name = 'billwise_return_end_time';
      save_return_time($trip_id, $return_name,time(),$version_no,$dis_id);
      if (!empty($existing_bill_wise_return_start_time)) {
          // Save sku wise return start time
         $return_name = 'billwise_return_start_time';
         save_return_time($trip_id, $return_name, $returns_start_time, $version_no, $dis_id);
      }

      // Insert the Returns Incharge Name in Fareye Table
      /* db_update('dispatch_planning_api_data') // Table name no longer needs {}
        ->fields(array(
        'returns_incharge_name' => $returns_incharge_name,
        ))
        ->condition('runsheet_number', $trip_id, '=')
        ->condition('distributor_id', $dis_id, '=')
        ->execute();
       */
      $query = db_select('dispatch_planning_api_data', 'ad');
      $query->condition('ad.runsheet_number', $trip_id);
      $query->condition('ad.distributor_id', $dis_id);
      $query->fields('ad');
      $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

      $bills_items = get_bill_items_details($trip_id, $dis_id);

      // add the header level data in new versioning header level table
      foreach ($res as $key => $val) {
        $query = db_insert('inventory_versioning_api_data');
        $cur_id = $val['id'];
        unset($val['id']);
        $val['version_created_time'] = REQUEST_TIME;
        $val['version_no'] = $version_no;
        $val['version_flag'] = 2;
        $query->fields($val);
        $result_id = $query->execute();

        $fields = array(
          'tur',
          'mrp',
          'product_name',
          'sum_net_sales_cases',
          'sum_net_sales_quantity',
          'sku_code',
          'sum_net_sales_kgs',
          'sum_net_sales_value',
          'sku_description',
          'trip_bill_reference',
          'res_sku_actual_quantity1',
          'og_sum_net_sales_quantity',
          'res_sku_reason',
          'photo_of_signed_bill_partial',
          'updated_flag',
          'res_godown_short',
          'res_cashier_short',
          'dispatch_qty',
          'return_qty',
          'farey_delivered_qty',
          'money_collect_type',
          'line_level_status',
          'claimed_godown_short',
          'claimed_godown_short_reason',
          'approved_quantity',
          'approval_flag',
          'godown_short_reason',
          'approved_by',
          'remarks',
          'declared_claimed_godown_short' 
        );
        // add the child level data in new versioning child level table
        if (!empty($result_id)) {
          $query1 = db_insert('inventory_versioning_item_details')->fields($fields);
          foreach ($bills_items[$cur_id] as $value1) {
            unset($value1['id']);
            $value1['trip_bill_reference'] = $result_id;
            $query1->values($value1);
          }
          $query1->execute();
        }
      }
    }
  }
  drupal_set_message(t("Data submit successfully."));
  //form_ajax_redirect('sku-wise-return-new');
}

function merge_sku_data_in_temp_table($bill_details, $trip_id, $dis_id) {
  //watchdog('uppar_table', '<pre>' . print_r($value, TRUE) . '</pre>');
  foreach ($bill_details as $sku => $value) {
    $actual_return_qty = $value['actual_returned_qty'];

    db_merge('temp_sku_wise_return')
      ->key(array('trip_id' => $trip_id, 'sku_code' => $sku))
      ->fields(array(
        'trip_id' => $trip_id,
        'sku_code' => $sku,
        'actual_return_qty' => $actual_return_qty,
        'distributor_id' => $dis_id,
      ))
      ->execute();
  }
}

function check_skuwise_return_start_time_present($trip_id, $dis_id) {
  $res = 0;
  if (!empty($trip_id) && !empty($dis_id)) {
    $query = db_select('node', 'n');
    $query->condition('n.title', $trip_id, '=');
    $query->condition('n.type', 'dispatch_data', '=');
    $query->condition('n.uid', $dis_id);
    $query->join('field_data_field_sku_wise_return_start_time', 'rt_start_time', 'rt_start_time.entity_id = n.nid');
    $query->fields('rt_start_time', array('field_sku_wise_return_start_time_value'));
    $res = $query->execute()->fetchField();
  }
  return $res;
}

function check_billwise_return_start_time_present($trip_id, $dis_id) {
  $res = 0;
  if (!empty($trip_id) && !empty($dis_id)) {
    $query = db_select('node', 'n');
    $query->condition('n.title', $trip_id, '=');
    $query->condition('n.type', 'dispatch_data', '=');
    $query->condition('n.uid', $dis_id);
    $query->join('field_data_field_bill_wise_return_starttime', 'rt_start_time', 'rt_start_time.entity_id = n.nid');
    $query->fields('rt_start_time', array('field_bill_wise_return_starttime_value'));
    $res = $query->execute()->fetchField();
  }
  return $res;
}

function save_sku_return_version($version_no, $bill_details, $trip_id, $dis_id) {
  if (!empty($version_no) && !empty($bill_details) && !empty($trip_id) && !empty($dis_id)) {
    $created_date = time();
    $fields = array(
      'trip_id',
      'sku_code',
      'actual_return_qty',
      'version_no',
      'distributor_id',
      'created_date',
    );
    // add the child level data in new versioning child level table
    $query = db_insert('sku_wise_return_version')->fields($fields);
    $records = array();
    foreach ($bill_details as $sku => $value) {
      $value['trip_id'] = $trip_id;
      $value['actual_return_qty'] = $value['actual_returned_qty'];
      unset($value['actual_returned_qty']);
      $value['sku_code'] = $sku;
      $value['version_no'] = $version_no;
      $value['created_date'] = $created_date;
      $value['distributor_id'] = $dis_id;
      $query->values($value);
    }
    $query->execute();
  }
}

function save_returns_start_time($type, $trip_id, $start_time, $distributor_id) {
  if (!empty($type) && !empty($trip_id) && !empty($start_time) && !empty($distributor_id)) {
    $query = db_select('node', 'n');
    $query->condition('n.title', $trip_id, '=');
    $query->condition('n.type', 'dispatch_data', '=');
    $query->condition('n.uid', $distributor_id);
    $query->fields('n', array('nid'));
    $nid = $query->execute()->fetchField();
    if (!empty($nid)) {
      $node = node_load($nid);
      if ($type == 'sku_wise') {
        $node->field_sku_wise_return_start_time[LANGUAGE_NONE][0]['value'] = $start_time;
      }
      elseif ($type == 'bill_wise') {
        $node->field_bill_wise_return_starttime[LANGUAGE_NONE][0]['value'] = $start_time;
      }
      node_save($node);
    }
  }
}

function save_returns_end_time($type, $trip_id, $end_time, $distributor_id) {
  if (!empty($type) && !empty($trip_id) && !empty($end_time) && !empty($distributor_id)) {
    $query = db_select('node', 'n');
    $query->condition('n.title', $trip_id, '=');
    $query->condition('n.type', 'dispatch_data', '=');
    $query->condition('n.uid', $distributor_id);
    $query->fields('n', array('nid'));
    $nid = $query->execute()->fetchField();
    if (!empty($nid)) {
      $node = node_load($nid);
      if ($type == 'sku_wise') {
        $node->field_sku_wise_return_end_time[LANGUAGE_NONE][0]['value'] = $end_time;
      }
      elseif ($type == 'bill_wise') {
        $node->field_bill_wise_return_end_time[LANGUAGE_NONE][0]['value'] = $end_time;
      }
      node_save($node);
    }
  }
}

function store_excess_inventory_declaration_data($all_excess_sku, $trip_id, $dis_id) {
  // Firstly delete the existing excess inventory data for the particular trip
  $del_existing_data = db_delete('excess_inventory_table')
    ->condition('trip_id', $trip_id, '=')
    ->condition('distributor_id', $dis_id, '=')
    ->execute();

  //Then save the fresh data
  foreach ($all_excess_sku as $key => $value) {
    // watchdog('lower_table', '<pre>' . print_r($value, TRUE) . '</pre>');
    $excess_product_name = $value['product_name'];
    $product_mrp = $value['mrp'];
    $excess_qty = $value['excess_qty'];
    $credit_to_cashier = $value['credit_to_cashier'];
    $excess_reason = $value['excess_reason'];
    $remarks = $value['remarks'];

    if (!empty($excess_product_name)) {
      db_merge('excess_inventory_table')
        ->key(array('trip_id' => $trip_id, 'product_name' => $excess_product_name))
        ->fields(array(
          'trip_id' => $trip_id,
          'product_name' => $excess_product_name,
          'mrp' => !empty($product_mrp) ? $product_mrp : '',
          'excess_qty' => !empty($excess_qty) ? $excess_qty : 0,
          'credit_to_cashier' => !empty($credit_to_cashier) ? $credit_to_cashier : 0,
          'excess_reason' => !empty($excess_reason) ? $excess_reason : '',
          'remarks' => !empty($remarks) ? $remarks : '',
          'distributor_id' => $dis_id,
        ))
        ->execute();
    }
  }
}
function save_return_time($trip_id,$return_name,$time_value,$version = null, $distributor_id) {
  if (!empty($trip_id) && !empty($return_name) && !empty($time_value)) {
    //sku_wise_return_time_history
    $fields = array(
      'trip_id',
      'return_version_name',
      'time_value',
      'return_version_value',
      'distributor_id',
      'created_date',
    );
    $query = db_insert('sku_wise_return_time_history')->fields($fields);
    $values['trip_id'] = $trip_id;
    $values['return_version_name'] = $return_name;
    $values['time_value'] = $time_value;
    $values['return_version_value'] = $version;
    $values['distributor_id'] = $distributor_id;
    $values['created_date'] = time();
    $query->values($values);
    $query->execute();
  }
}
// Checking return start time present
function get_returns_start_time($trip_id,$return_name,$distributor_id) {
  if (!empty($trip_id)&& !empty($return_name) && !empty($distributor_id)) {
    $query = db_select('sku_wise_return_time_history','sku_time');
             $query->condition('sku_time.trip_id',$trip_id);
             $query->condition('sku_time.distributor_id',$distributor_id);
             $query->condition('sku_time.return_version_name',$return_name);
             $query->fields('sku_time','id');
             $id = $query->execute()->fetchField();
             
  }
  return $id;
}

// Checking return start time present
function get_returns_end_time($trip_id,$distributor_id) {
  $result = array();
  if (!empty($trip_id) && !empty($distributor_id)) {
    $query = db_select('sku_wise_return_time_history','sku_time');
             $query->condition('sku_time.trip_id',$trip_id);
             $query->condition('sku_time.distributor_id',$distributor_id);
             $query->condition('sku_time.return_version_name',array('billwise_return_end_time','skuwise_return_end_time'),'IN');
             $query->isNotNull('sku_time.return_version_value');
             $query->fields('sku_time',array('return_version_name'));
             $query->fields('sku_time',array('time_value'));
             $query->orderBy('sku_time.id', 'desc');
             $result= $query->execute()->fetchAllKeyed(0,1);
             
  }

}
