<?php
module_load_include('inc', 'mrc_inventory_returns', 'includes/vrs_common');

function get_data_of_trip_vrs_form($form, &$form_state) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);
  $form['#attached']['js'][] = drupal_get_path('module', 'mrc_inventory_returns') . '/js/vrs_printout.js';
  $form['#prefix'] = '<div id="itemlist-div" class="custom_form_wrapper">';
  $form['#suffix'] = '</div>';

  $form['trip_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Trip Id'),
    '#size' => 10,
    '#attributes' => array('class' => array('trip-id')),
    '#prefix' => '<div class="form_row form_row2"><div class="col_3">',
    '#suffix' => '</div></div><div style="padding: 10px 0 10px 0;">OR</div>'
  );

  $form['dispatch_date_from'] = array(
    '#type' => 'date_popup',
    '#title' => t('Dispatch Date From*'),
    '#date_format' => 'Y-m-d',
    '#date_label_position' => '',
    '#date_year_range' => '-20:+2',
    '#attributes' => array('class' => array('dispatch-date-from')),
    '#size' => 30,
    '#validate'=> 'dispatch_date_from_validation',
    '#ajax' => array(
      'wrapper' => 'cashier_name_id',
      'callback' => 'ajax_refresh_cashier_name',
      'event' => 'change',
      'effect' => 'fade',
    ),
    '#prefix' => '<div class="form_row"><div class="col_2 date_range">',
    '#suffix' => '</div>'
  );

  $form['dispatch_date_to'] = array(
    '#type' => 'date_popup',
    '#title' => t('Dispatch Date To*'),
    '#date_format' => 'Y-m-d',
    '#date_label_position' => '',
    '#date_year_range' => '-20:+2',
    '#attributes' => array('class' => array('dispatch-date-to')),
    '#size' => 30,
    '#validate'=> 'dispatch_date_to_validation',
    '#ajax' => array(
      'wrapper' => 'cashier_name_id',
      'callback' => 'ajax_refresh_cashier_name',
      'event' => 'change',
      'effect' => 'fade',
    ),
    '#prefix' => '<div class="col_2 date_range">',
    '#suffix' => '</div></div>'
  );

  $dispatch_date_from = $form_state['values']['dispatch_date_from'];
  $dispatch_date_to = $form_state['values']['dispatch_date_to'];
  $form['cashier_name'] = array(
    '#type' => 'select',
    '#title' => 'Cashier Name',
    '#attributes' => array('class' => array('cashier-name'), 'style' => 'width:200px;'),
    '#options' => array('' => '--Select--') + get_cashier_names_as_per_dispatch_date_range($dispatch_date_from, $dispatch_date_to),
    '#prefix' => '<div class="col_2" id="cashier_name_id"> <div >',
    '#suffix' => '</div></div>'
  );


  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#attributes' => array('class' => array('full_submit')),
    '#ajax' => array(
      'wrapper' => 'itemlist-div',
      'callback' => 'trip_vrs_print_form_submit',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    '#submit' => array('get_data_of_trip_vrs_form_submit'),
    '#validate' => array('get_data_of_trip_vrs_form_validate')
  );


  if (!empty($form_state['views_data'])) {
    $form['hhh'] = array(
      '#type' => 'markup',
      '#markup' => $form_state['views_data'],
    );

    $trip_id = $form_state['values']['trip_id'];
  //  $check_returns_flag = check_trip_id_exists_or_not($dis_id, $trip_id);
  //  if ($check_returns_flag->field_vrs_status_value>=2) {
      $rr = l('Trip VRS Printout', "/download-trip-vrs-report", array('attributes' => array("target" => "_blank"), 'query' => array('trip_id' => $trip_id, 'dispatch_date_from' => $form_state['values']['dispatch_date_from'], 'dispatch_date_to' => $form_state['values']['dispatch_date_to'], 'cashier_name' => $form_state['values']['cashier_name'])));
      $form['download_mark'] = array(
        '#type' => 'markup',
        '#markup' => $rr,
      );
//      $export_all_versions = l('Export All Versions to XLS', "/get-all-trip-vrs-versions", array('attributes' => array("target" => "_blank"), 'query' => array('trip_id' => $trip_id, 'dispatch_date_from' => $form_state['values']['dispatch_date_from'], 'dispatch_date_to' => $form_state['values']['dispatch_date_to'], 'cashier_name' => $form_state['values']['cashier_name'])));
//      $form['export_all_versions'] = array(
//        '#type' => 'markup',
//        '#prefix' => '<div>',
//        '#markup' => $export_all_versions,
//        '#sufix' => '</div>',
//      );
    //}
  }

  return $form;
}

function ajax_refresh_cashier_name($form, &$form_state) {
  return $form['cashier_name'];
}

function trip_vrs_print_form_submit($form, &$form_state) {
  $commands[] = ajax_command_replace('#itemlist-div', render($form));
  $commands[] = ajax_command_replace('.alert-danger', '');
  $commands[] = ajax_command_replace('.alert-success', '');
  $commands[] = ajax_command_prepend('#itemlist-div', theme('status_messages'));
  return array('#type' => 'ajax', '#commands' => $commands);
  //return $form;
}

function get_data_of_trip_vrs_form_validate($form, &$form_state) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);
  $trip_id = $form_state['values']['trip_id'];
  $dispatch_date_from = $form_state['values']['dispatch_date_from'];
  $dispatch_date_to = $form_state['values']['dispatch_date_to'];
  $cashier_name = $form_state['values']['cashier_name'];

  $trip_ids = array();
  if (!empty($trip_id)) {
    $trip_ids = array($trip_id);
    $check_returns_flag = check_trip_id_exists_or_not($dis_id, $trip_id);
    if (empty($check_returns_flag)) {
      form_set_error('trip_id', "Trip ID :$trip_id does not exists.");
      ajax_redirect('trip-vrs-printout');
    }
    else if ($check_returns_flag->field_vrs_status_value == 1) {
      if(!empty($form_state['views_data']['bills'])){
        unset($form_state['views_data']['bills']);
      }
      form_set_error('trip_id', "Trip ID :$trip_id - Returns Confirmed.");
      ajax_redirect('trip-vrs-printout');
    }
    else if (empty($check_returns_flag->field_vrs_status_value)) {
      if(!empty($form_state['views_data']['bills'])){
        unset($form_state['views_data']['bills']);
      }
      form_set_error('trip_id', "Trip ID:$trip_id - Please confirm Return Inventory first.");
      ajax_redirect('trip-vrs-printout');
    }
  }
  else if ((!empty($dispatch_date_from) && !empty($dispatch_date_to)) || !empty($cashier_name)) {
    if (strtotime($dispatch_date_to) < strtotime($dispatch_date_from)) {
      drupal_set_message(t("<b>Dispatch Date To</b> should not be less than <b>Dispatch Date From</b>."), 'error');
      ajax_redirect('trip-vrs-printout');
    }else{
      $trip_ids = get_trip_ids_by_date_range($dispatch_date_from, $dispatch_date_to, $cashier_name);
      if (!empty($trip_ids)) {
        $trip_ids = array($trip_ids);
      }
    }
    
  }
  else {
    drupal_set_message(t("Please select either Trip Id or Dispatch Date Range."), "error");
    ajax_redirect('trip-vrs-printout');
  }
  
  if (empty($trip_ids)) {
    //form_set_error('trip_id', "No Trip Found.");
    drupal_set_message(t("No Trip Found."), 'error');
    ajax_redirect('trip-vrs-printout');
  }
  if (!empty($trip_id) && (!empty($dispatch_date_from) || !empty($dispatch_date_to))) {
    //form_set_error('trip_id', "Please select either Trip Id or Dispatch Date");
    drupal_set_message(t("Please select either Trip Id or Dispatch Date"), 'error');
    ajax_redirect('trip-vrs-printout');
  }
}

function get_data_of_trip_vrs_form_submit($form, &$form_state) {
  $trip_id = $form_state['values']['trip_id'];
  $dispatch_date_from = $form_state['values']['dispatch_date_from'];
  $dispatch_date_to = $form_state['values']['dispatch_date_to'];
  $cashier_name = $form_state['values']['cashier_name'];
  $rendered_html = '';

  if (!empty($trip_id) || (!empty($dispatch_date_from) && !empty($dispatch_date_to))) {
    if (!empty($trip_id)) {
      $trip_ids = array($trip_id);
    }
    else if (!empty($dispatch_date_from) && !empty($dispatch_date_to)) {
      $trip_ids = get_trip_ids_by_date_range($dispatch_date_from, $dispatch_date_to, $cashier_name);
      if (!empty($trip_ids)) {
        $trip_ids = array($trip_ids);
      }
    }

    if (!empty($trip_ids)) {
      $view_name = "multiple_trip_vrs_report";
      $view = views_get_view($view_name, '');
      $view->set_arguments($trip_ids);
      $rendered_html = $view->render();
      $form_state['views_data'] = array();

      if (!empty($rendered_html)) {
        $form_state['views_data'] = $rendered_html;
        $form_state['rebuild'] = TRUE;
      }
    }
  }
}

function download_trip_vrs_report() {
  $params = drupal_get_query_parameters();
  //watchdog('params', '<pre>params: ' . print_r($params) . '</pre>');
  if (!empty($params)) {
    $trip_id = $params['trip_id'];
    $dispatch_date_from = $params['dispatch_date_from'];
    $dispatch_date_to = $params['dispatch_date_to'];
    $cashier_name = $params['cashier_name'];
    if (!empty($trip_id)) {
      $trip_ids = array($trip_id);
    }
    else if ((!empty($dispatch_date_from) && !empty($dispatch_date_to)) || !empty($cashier_name)) {
      $trip_ids = get_trip_ids_by_date_range($dispatch_date_from, $dispatch_date_to, $cashier_name);
      if (!empty($trip_ids)) {
        $trip_ids = array($trip_ids);
      }
    }
    
    $output = "";
    $trip_ids_arr = array();
    $trip_ids_arr = explode(',', $trip_ids[0]);
    //watchdog('trip_ids_arr', '<pre>trip_ids_arr: ' . print_r($trip_ids_arr) . '</pre>');
    foreach ($trip_ids_arr as $key => $value) {
      $table_html = get_vrs_print($value);
      if(!empty($table_html)){
        $output.=  get_vrs_print($value);
        if($value != end($trip_ids_arr))
          $output.= '<div style="page-break-before: always;"> </div>';
      }
    }
    //watchdog('output', '<pre>output: ' . print_r($output) . '</pre>');
    if(!empty($output)) {
      pdf_using_mpdf_api($output);
    }
  }
}

function get_vrs_print($trip_id) {
  $output = '';
  if (!empty($trip_id)) {
    global $user;
    $dis_id = change_feeds_import_get_distributor_id($user->uid);
    $options = get_vrs_items($trip_id);
    if (!empty($options)) {
      // VRS edit screen load existing data
      $bills_records = array();
      $cheque_details = array();
      $signed_bill_details = array();
      foreach ($options as $key => &$val) {
        $sum_s_register_bill_value += round($val['field_bill_value_value']);
        $dispatch_date = $val['dispatch_date'];
        $field_cashier_name = $val['field_cashier_name_value'];
        $billing_date[$val['billing_date']] = $val['billing_date'];
        if ($val['check_delivered_value_less'] == 1) {
          $val['sales_register_to_be_collected_amt'] = 0;
        }
        if ($val['res_status'] == 'undeliver') {
          $val['sales_register_to_be_collected_amt'] = 0;
        }
        $sum_claimed_godown_short_qty += $val['claimed_godown_short_qty'];
        $sum_claimed_godown_short_value += $val['claimed_gs_value'];
      }
      $sum_claimed_godown_short_value = round($sum_claimed_godown_short_value, 2);
      if (count($billing_date) > 1) {
        $billing_dates = implode(',', $billing_date);
      }
      else {
        $billing_dates = reset($billing_date);
      }
      $vrs_version = get_current_vrs_version($trip_id, $dis_id);
      $van_no = get_van_no($trip_id, $dis_id);
      $short_header = array('Short Name', 'Quantity', 'Value');
      $trip_header = array('Trip ID', 'Cashier Name', 'Total Bill Value(Sales Register)', 'Trip Execution Date');
      $bill_header = array(
        'Bill Number',
        'Delivery Outcome',
        'Payment Outcome',
        'Sales Register Bill Value',
        'Sales Register To Be Collected',
        'Actual Cash Collected',
        'Actual Signed Bill Amount',
        'Actual Cheque Amount Collected',
        'Cheque Number',
        'Cheque Date',
        'Bank Name',
      );
      $footer_header = array('Count of Bills', 'Total to be collected(Sales Register)', 'Total Cash Collected', 'Total Signed Bill Collected', 'Total Cheque Collected', 'Cashier Cash Short', 'Total Short', 'Total Cashier Debit Value');
      $trip_header_values[] = array($trip_id, $field_cashier_name, $sum_s_register_bill_value, $dispatch_date);
      $get_shorts = get_return_short($trip_id, $dis_id);
      $asset = received_assets_count($trip_id, $dis_id);
      $assets_details = received_assets_count_details($trip_id, $dis_id);
      $sum_godown_short_qty = $get_shorts['sum_godown_short_qty'] ? $get_shorts['sum_godown_short_qty'] : 0;
      $sum_cashier_short_qty = $get_shorts['sum_cashier_short_qty'] ? $get_shorts['sum_cashier_short_qty'] : 0;
      $sum_godown_short_value = $get_shorts['sum_godown_short_value'] ? number_format((float) $get_shorts['sum_godown_short_value'], 2, '.', '') : 0;
      $sum_cashier_short_value = $get_shorts['sum_cashier_short_value'] ? number_format((float) $get_shorts['sum_cashier_short_value'], 2, '.', '') : 0;
      $asset_short_qty = $asset['received_qty'] ? $asset['received_qty'] : 0;
      $asset_short_value = $asset['value'] ? $asset['value'] : 0;
      $short_values[] = array('Godown Short', $sum_godown_short_qty, $sum_godown_short_value);
      $short_values[] = array('Cashier Short', $sum_cashier_short_qty, $sum_cashier_short_value);
      $short_values[] = array('Assets Short', $asset_short_qty, $asset_short_value);
      $excess_values = get_excess_declaration_cashier_data($trip_id, $dis_id);
      $excessvalue = $excess_values[0]['excess_value'] ? $excess_values[0]['excess_value'] : 0;
      $asset_total = 0;
      if (!empty($vrs_version)) {
        $bills = get_vrs_bill_items($trip_id, $dis_id, $vrs_version);
        $vrs_total = get_vrs_total_items($trip_id, $dis_id, $vrs_version);
        $total_key = key($vrs_total);
        $vrs_total_id = $vrs_total[$total_key]['id'];
        $vrs_created_date = $vrs_total[$total_key]['vrs_created_date'];
        $denominations = get_vrs_total_denominations($trip_id, $dis_id);
      }
      foreach ($options as $key => &$val) {
        switch ($val['res_status']) {
          case 'partial_deliver';
            $bill_status = t('Partial Delivered');
            break;

          case 'deliver';
            $bill_status = t('Delivered');
            break;

          case 'undeliver';
            $undeliver = 1;
            $bill_status = t('Full Returned');
            break;
            defualt:
            $bill_status = t('NA');
        }

        $bills[$key]['actual_signed_bill_amount'] = round($bills[$key]['actual_signed_bill_amount']);
        $bills[$key]['actual_cash_amount'] = round($bills[$key]['actual_cash_amount']);
        $bills[$key]['actual_cheque_amount'] = round($bills[$key]['actual_cheque_amount']);
        if (($bills[$key]['actual_signed_bill_amount'] > 0)) {
          $payment_term[] = t('Signed Bill');
          $pending_amt = round($val['field_bill_value_value']) - round($bills[$key]['actual_signed_bill_amount']);
          $signed_bill_details[$key] = array('bill_value' => round($val['field_bill_value_value']), 'bill_no' => $key, 'amt' => $bills[$key]['actual_signed_bill_amount'], 'edit' => $pending_amt);
        }
        if (!empty($bills[$key]['cheque_date'])) {
          $bills[$key]['cheque_date'] = date('d-m-Y', $bills[$key]['cheque_date']);
        }
        if (($bills[$key]['actual_cheque_amount'] > 0)) {
          $payment_term[] = t('Cheque');
          $cheque_details[$key] = array('date' => $bills[$key]['cheque_date'], 'bill_no' => $key, 'amt' => $bills[$key]['actual_cheque_amount'], 'remarks' => $bills[$key]['remarks']);
        }
        if (($bills[$key]['actual_cash_amount'] > 0)) {
          $payment_term[] = t('Cash');
        }
        if (!empty($payment_term)) {
          $payments_terms = implode(',', $payment_term);
        }

        $bills[$key]['payments_terms'] = $payments_terms;
        unset($payments_terms);
        unset($payment_term);
        $bills_records[] = array($val['bill_number'],
          $bill_status,
          $bills[$key]['payments_terms'],
          round($val['bill_value']),
          round($val['sales_register_to_be_collected_amt']),
          $bills[$key]['actual_cash_amount'],
          $bills[$key]['actual_signed_bill_amount'],
          $bills[$key]['actual_cheque_amount'],
          $bills[$key]['cheque_number'],
          $bills[$key]['cheque_date'],
          $bills[$key]['bank_name']
        );
        unset($bill_status);
      }
      //border="1" cellpadding="5" cellspacing="0" width="100%" style="color:#000; font-size: 13px;" class="views-table cols-9 table table-hover table-striped"
      //$output = render($header_details1);
      //return $output;
      // bill level
      $sum_unify_return = 0;
      $sum_actual_return_inventory = 0;
      foreach ($options as $key => &$val) {
        $total_sales_register += round($val['sales_register_to_be_collected_amt']);
        $unify_return_sheet = round($val['unify_sales_return']);
        $sum_unify_return += $unify_return_sheet;
        $sum_actual_return_inventory += round($val['total_delivered_value']);
      }
      $total_sales_register = $total_sales_register;
      $bill_count = count($options);
      $footer_table_values[] = array($bill_count, $total_sales_register, $vrs_total[$total_key]['total_actual_cash_amount'], $vrs_total[$total_key]['total_actual_signed_bill_amount'], $vrs_total[$total_key]['total_actual_cheque_amount'], $vrs_total[$total_key]['total_cashier_cash_short'], $vrs_total[$total_key]['total_short_amt'], $vrs_total[$total_key]['total_cashier_debit_value']);
      $note_denominations = get_notes_denominations();
      $table1 = '<table border="1" cellpadding="5" cellspacing="0" width="100%" style="color:#000; font-size: 13px;" class="views-table cols-13 table table-hover table-striped">';
      $table1 .= '<tr><td>' . '<b>' . t('Trip ID') . '</b>' . '</td>';
      $table1 .= '<td>' . $trip_id . '</td>';
      $table1 .= '<td>' . '<h4>' . t('Van Reconciliation Sheet(VRS)') . 'Version:' . $vrs_version . '</h4>' . '</td>';
      $table1 .= '<td colspan="2">' . '<b>' . t('Done By') . '</b>' . '</td>';
      $table1 .= '</tr>';
      $table1 .= '<tr><td colspan="2">' . t('VAN No') . '</td>';
      $table1 .= '<td>' . $van_no . '</td>';
      $table1 .= '<td colspan="2">' . t('Cashier Name') . '</td>';
      $table1 .= '<td>' . $field_cashier_name . '</td></tr>';

      $table1 .= '<tr><td colspan="2">' . t('Billing Date') . '</td>';
      $table1 .= '<td>' . $billing_dates . '</td>';
      $table1 .= '<td colspan="2">' . t('Dispatch Date') . '</td>';
      $table1 .= '<td>' . $dispatch_date . '</td></tr>';
      $table1 .= '<tr><td>' . t('Start Time') . '</td>';
      $table1 .= '<td colspan="2">' . '</td>';
      $table1 .= '<td>' . t('End Time') . '</td>';
      $table1 .= '<td colspan="2">' . '</td>';
      $table1 .= '</tr>';
      $table1 .= '</table>';
      if (!empty($note_denominations)) {
        $table2 = '<table border="1" cellpadding="5" cellspacing="0" width="100%" style="color:#000; font-size: 13px;" class="views-table cols-13 table table-hover table-striped">';
        $table2 .= '<tr><td colspan="4" style="text-align:center">' . '<h4 >' . t('CASH Details') . '</h4>' . '</td></tr>';
        $table2 .= '<tr>';
        $table2 .= '<td>' . t('Deno') . '</td>';
        $table2 .= '<td>' . '</td>';
        $table2 .= '<td>' . t('Count') . '</td>';
        $table2 .= '<td>' . t('Total') . '</td>';
        $table2 .= '</tr>';
        $total_deno = 0;
        foreach ($note_denominations as $key => $value) {
          $default_value = 0;
          if (!empty($denominations[$value['nid']]['coins'])) {
            $coins_value = $denominations[$value['nid']]['coins'];
          }
          if (!empty($denominations[$value['nid']]['no_of_notes'])) {
            $default_value = $denominations[$value['nid']]['no_of_notes'];
          }
          $sum_deno = ($value['title'] * $default_value);
          $total_deno += $sum_deno;
          $table2 .= '<tr>';
          $table2 .= '<td>' . $value['title'] . '</td>';
          $table2 .= '<td>' . t('X') . '</td>';
          $table2 .= '<td>' . $default_value . '</td>';
          $table2 .= '<td style="text-align:center;" align="right">' . $sum_deno . '</td>';
          $table2 .= '</tr>';
          unset($sum_deno);
        }
        $total_deno = $total_deno + $coins_value;
        $table2 .= '<tr>';
        $table2 .= '<td>' . t('Coins') . '</td>';
        $table2 .= '<td colspan="3" align="right">' . $coins_value . '</td>';
        $table2 .= '</tr>';

        $table2 .= '<tr>';
        $table2 .= '<td colspan="3"><b>' . t('Total Cash Amt') . '</b></td>';
        $table2 .= '<td align="right">' . $vrs_total[$total_key]['total_actual_cash_amount'] . '</td>';
        $table2 .= '</tr>';
        $table2 .= '</table>';
      }

      if (!empty($cheque_details)) {
        $table3 = '<table border="1" cellpadding="5" cellspacing="0" width="100%" style="color:#000; font-size: 13px;" class="views-table cols-13 table table-hover table-striped;float:right;margin-left:20px;">';
        $table3 .= '<tr><td colspan="4" style="text-align:center">' . '<h4 >' . t('CHEQUE Details') . '</h4>' . '</td></tr>';
        $table3 .= '<tr>';
        $table3 .= '<td>' . t('Chq Dt') . '</td>';
        $table3 .= '<td>' . t('Bill No') . '</td>';
        $table3 .= '<td>' . t('Amount') . '</td>';
        $table3 .= '<td>' . t('Remarks') . '</td>';
        $table3 .= '</tr>';
        foreach ($cheque_details as $check_key => $value) {
          $table3 .= '<tr>';
          $table3 .= '<td>' . $cheque_details[$check_key]['date'] . '</td>';
          $table3 .= '<td>' . $cheque_details[$check_key]['bill_no'] . '</td>';
          $table3 .= '<td align="right">' . $cheque_details[$check_key]['amt'] . '</td>';
          $table3 .= '<td>' . $cheque_details[$check_key]['remarks'] . '</td>';
          $table3 .= '</tr>';
        }
        $table3 .= '<tr>';
        $table3 .= '<td colspan="2"><b>' . t('Total Cheque Amt') . '</b></td>';
        $table3 .= '<td align="right">' . $vrs_total[$total_key]['total_actual_cheque_amount'] . '</td>';
        $table3 .= '<td></td>';
        $table3 .= '</tr>';
        $table3 .= '</table>';
        $table3 .= '</table>';
      }
      // signed bill details
      if (!empty($signed_bill_details)) {
        $table5 = '<table border="1" cellpadding="5" cellspacing="0" width="100%" style="margin-top:10px;color:#000; font-size: 13px;" class="views-table cols-13 table table-hover table-striped;float:right;margin-left:20px;">';
        $table5 .= '<tr><td colspan="4" style="text-align:center">' . '<h4 >' . t('SIGNED BILL Details') . '</h4>' . '</td></tr>';
        $table5 .= '<tr>';
        $table5 .= '<td>' . t('Bill No') . '</td>';
        $table5 .= '<td>' . t('Bill Value') . '</td>';
        $table5 .= '<td>' . t('Edit') . '</td>';
        $table5 .= '<td>' . t('Amount') . '</td>';
        $table5 .= '</tr>';
        foreach ($signed_bill_details as $signed_key => $value) {
          $table5 .= '<tr>';
          $table5 .= '<td>' . $signed_bill_details[$signed_key]['bill_no'] . '</td>';
          $table5 .= '<td>' . $signed_bill_details[$signed_key]['bill_value'] . '</td>';
          $table5 .= '<td>' . $signed_bill_details[$signed_key]['edit'] . '</td>';
          $table5 .= '<td align="right">' . $signed_bill_details[$signed_key]['amt'] . '</td>';
          $table5 .= '</tr>';
        }
        $table5 .= '<tr>';
        $table5 .= '<td colspan="3"><b>' . t('Total Signed Bill Amt') . '</b></td>';
        $table5 .= '<td align="right">' . $vrs_total[$total_key]['total_actual_signed_bill_amount'] . '</td>';
        $table5 .= '</tr>';
        $table5 .= '</table>';
      }

      $total_sum_bill = $sum_unify_return + $vrs_total[$total_key]['total_actual_signed_bill_amount'] + $vrs_total[$total_key]['total_actual_cheque_amount'] + $vrs_total[$total_key]['total_actual_cash_amount'];
      $table6 = '<table border="1" cellpadding="5" cellspacing="0" width="100%" style="margin-top:10px;color:#000; font-size: 13px;" class="views-table cols-13 table table-hover table-striped;float:right;margin-left:20px;">';
      $table6 .= '<tr><td colspan="4" align="right">' . '<h4 >' . t('F) Total Van Value:') . $sum_s_register_bill_value;
      '</h4>' . '</td></tr>';
      $table6 .= '<tr><td colspan="4" align="center">' . '<h4 >' . t('G) Cash + Chq + Signed Bill + Unify Return: ') . $total_sum_bill . '</h4>' . '</td></tr>';
      $table6 .= '<tr>';
      $table6 .= '<td><b>' . t('A) Unify Sales Return Sheet Value') . '</b></td>';
      $table6 .= '<td>' . $sum_unify_return . '</td>';
      $table6 .= '<td><b>' . t('H) Cash Short (F-G)') . '</b></td>';
      $table6 .= '<td>' . $vrs_total[$total_key]['total_cashier_cash_short'] . '</td>';
      $table6 .= '</tr>';

      $table6 .= '<tr>';
      $table6 .= '<td><b>' . t('B) Excess Inventory Value(MRP - 10%) or Actual as per "Sales Return Sheet"') . '</b></td>';
      $table6 .= '<td>' . '</td>';
      $table6 .= '<td>';
      if (!empty($assets_details)) {

        $table6 .= '<table width="100%" border="1" cellpadding="0" cellspacing="0" style="border-color:white;border:1px solid black" >';
        $table6 .= '<tr>';
        $table6 .= '<td>' . t('Assets Name') . '</td>';
        $table6 .= '<td>' . t('Value') . '</td>';
        $table6 .= '<td>' . t('Per Assets Short qty') . '</td>';
        $table6 .= '</tr>';
        foreach ($assets_details as $as_key => $as_val) {
          $table6 .= '<tr>';
          $table6 .= '<td>' . $as_val['asset_name'] . '</td>';
          $table6 .= '<td>' . $as_val['field_asset_value_value'] . '</td>';
          $table6 .= '<td>' . $as_val['short_qty'] . '</td>';
          $table6 .= '</tr>';
        }
        $table6 .= '</table>';
      }

      $table6 .= '</td>';

      $table6 .= '<td>';

      $table6 .= '<table width="100%" border="1" cellpadding="0" cellspacing="0" style="border-color:white;border:1px solid black" >';
      $table6 .= '<tr>';
      $table6 .= '<td><b>' . t('J) Crates Short Val') . '</b></td>';
      $table6 .= '</tr>';
      $table6 .= '<tr>';
      $table6 .= '<td>' . $asset_short_value . '</td>';
      $table6 .= '</tr>';
      $table6 .= '</table>';
      $table6 .= '</td>';
      $table6 .= '</tr>';


      $table6 .= '<tr>';
      $table6 .= '<td><b>' . t('C) Actual Return Inventory Value  Recd from "Sales Return Sheet"') . '</b></td>';
      $table6 .= '<td>' . $sum_actual_return_inventory . '</td>';
      $table6 .= '<td>';
      $table6 .= '</td>';
      $table6 .= '<td>';
      $table6 .= '</td>';
      $table6 .= '</tr>';

      $table6 .= '<tr>';
      $table6 .= '<td><b>' . t('D) Godown Short(Actual as per "Sales Return Sheet"') . '</b></td>';
      $table6 .= '<td>' . $sum_godown_short_value . '</td>';
      $table6 .= '<td colspan="2">';
      $table6 .= '<table width="100%" border="1" cellpadding="0" cellspacing="0" style="border-color:white;border:1px solid black" >';
      $table6 .= '<tr>';
      $table6 .= '<td><b>' . t('Pending claimed GS value (K)') . '<b></td>';
      //$sum_claimed_godown_short_value = round($sum_claimed_godown_short_value);
      $table6 .= '<td>' . $sum_claimed_godown_short_value . '</td>';
      $table6 .= '</tr>';

      $table6 .= '<tr>';
      //$table6.= '<td><b>'.t('Total Cashier Short.(E+H+J-I)').'<b></td>';
      $table6 .= '<td><b>' . t('Total Cashier Short (E+H+J+K-I)') . '<b></td>';
      // cashier debit value calculation
      $cashier_short_calculation = $vrs_total[$total_key]['total_to_be_collected_sales_register'] - $vrs_total[$total_key]['total_actual_cash_amount'] - $vrs_total[$total_key]['total_actual_cheque_amount'] - $vrs_total[$total_key]['total_actual_signed_bill_amount'] - $vrs_total[$total_key]['rto_charges'] - $vrs_total[$total_key]['total_short_amt'];
      $cashier_debit_value = $cashier_short_calculation + $sum_cashier_short_value + $asset_short_value + $sum_claimed_godown_short_value - $excessvalue;
      //$cashier_debit_value = round($cashier_debit_value);
      //$table6.= '<td>'.$vrs_total[$total_key]['total_cashier_debit_value'].'</td>';   
      $table6 .= '<td>' . $cashier_debit_value . '</td>';
      $table6 .= '</tr>';
      $table6 .= '</table>';
      $table6 .= '</td>';
      $table6 .= '</tr>';

      $table6 .= '<tr>';
      $table6 .= '<td><b>' . t('E) Cashier Short Inventory Value (Actual as per Sales Return Sheet):(A-B-C-D)') . '</b></td>';
      $table6 .= '<td>' . $sum_cashier_short_value . '</td>';
      $table6 .= '<td colspan="2">';
      $table6 .= '<table width="100%" border="1" cellpadding="0" cellspacing="0" style="border-color:white;border:1px solid black" >';
      $table6 .= '<tr>';
      $table6 .= '<td width="50%"><b>' . t('Van Return Time') . '</b></td>';
      $table6 .= '<td width="50%">' . '</td>';
      $table6 .= '</tr>';
      $table6 .= '<tr>';
      $table6 .= '<td width="50%"><b>' . t('Reco Completion Time') . '</b></td>';
      $table6 .= '<td width="50%">' . '</td>';
      $table6 .= '</tr>';
      $table6 .= '</table>';
      $table6 .= '</td>';
      $table6 .= '</tr>';

      $table6 .= '<tr>';
      $table6 .= '<td><b>' . t('I) R.T.O') . '</b></td>';
      $table6 .= '<td>' . $vrs_total[$total_key]['rto_charges'] . '</td>';
      $table6 .= '<td><b>' . t('Cashier Signature') . '</b></td>';
      $table6 .= '<td><b>' . t('') . '</b></td>';
      $table6 .= '</tr>';


      $table6 .= '</table>';


      $table4 = '<table "width=100%" style ="margin-top:10px;">';
      $table4 .= '<tr>';
      $table4 .= '<td>' . $table2 . '</td>';
      $table4 .= '<td>' . $table3 . '</td>';
      $table4 .= '</tr>';
      $table4 .= '</table>';
      $output = render($table1);
      $output1 = render($table2);
      $output2 = render($table3);
      $output .= render($table4);
      $output .= render($table5);
      $output .= render($table6);
    }
  }
  return $output;
}

function get_vrs_items($trip_id) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);
  //$bill_status_undeliver = $form_state['values']['bill_status_undeliver'];
  $res = array();
  if (!empty($trip_id)) {
    $sub_query1 = db_select('dispatch_planning_item_details', 'dpi');
    $sub_query1->fields('dpi', array('trip_bill_reference'));
    $sub_query1->addExpression("SUM(dpi.return_qty * dpi.sum_net_sales_value)", 'return_value');
    $sub_query1->addExpression('(SUM(dpi.sum_net_sales_value * dpi.res_sku_actual_quantity1))', 'delivered_value');
    $sub_query1->addExpression("SUM(dpi.res_cashier_short * dpi.sum_net_sales_value)", 'cashier_short_value');
    $sub_query1->addExpression("SUM(dpi.res_godown_short * dpi.sum_net_sales_value)", 'godown_short_value');
    $sub_query1->addExpression('(SUM(dpi.return_qty * dpi.sum_net_sales_value))', 'total_delivered_value');
    $sub_query1->addExpression("SUM(dpi.claimed_godown_short * dpi.sum_net_sales_value)", 'claimed_godown_short_value');
    $sub_query1->addExpression("SUM(dpi.claimed_godown_short)", 'claimed_godown_short_qty');
    $sub_query1->fields('dpi', array('return_qty', 'sum_net_sales_value'));
    $sub_query1->join('dispatch_planning_api_data', 'dp_header', 'dpi.trip_bill_reference = dp_header.id AND dp_header.distributor_id =' . $dis_id);
    $sub_query1->condition('dp_header.runsheet_number', $trip_id, '=');
    $sub_query1->groupBy('dpi.trip_bill_reference');
    $query = db_select('dispatch_planning_api_data', 'ad');
    $query->condition('ad.delivery_pending_flag', 1, '!=');
    $query->condition('ad.distributor_id', $dis_id, '=');
    $query->condition('ad.runsheet_number', $trip_id, '=');
    $default_bill_status = array('partial_deliver', 'deliver', 'undeliver');
    $query->condition('ad.res_status', $default_bill_status, 'IN');
    $query->join('node', 'n', 'n.title = ad.bill_number AND n.uid = ad.distributor_id');
    $query->condition('n.type', 'sales_register_data', '=');
    $query->leftjoin('field_data_field_sr_bill_date', 'sr_bill_date', 'sr_bill_date.entity_id = n.nid');
    $query->leftJoin('field_data_field_bill_level_discount', 'bill_discount', 'bill_discount.entity_id = n.nid');

    // Join with outlet master to fetch payment type
    $query->join('field_data_field_party_code_reference', 'pc', 'pc.entity_id = n.nid');
    $query->join('node', 'oun', 'oun.nid = pc.field_party_code_reference_nid');
    $query->leftJoin('field_data_field_payment_terms', 'pt', 'pt.entity_id = oun.nid');
    $query->condition('oun.uid', $dis_id);
    //---------------------------
    $query->condition('n.uid', $dis_id);
    $query->join('field_data_field_bill_value', 'bv', 'bv.entity_id = n.nid');
    $query->join($sub_query1, 'item_details', 'item_details.trip_bill_reference = ad.id');
    $query->addExpression("(item_details.claimed_godown_short_value)", 'claimed_gs_value');
    // Join with dispatch_data to check vrs status and return flage 
    $query->join('node', 'disp_data', 'disp_data.title = ad.runsheet_number');
    $query->condition('disp_data.type', 'dispatch_data');
    $query->condition('disp_data .uid', $dis_id);
    $query->leftjoin('field_data_field_vrs_status', 'vs', 'vs.entity_id = disp_data.nid');
    // VRS Screen and VRS status = 1, 3 Means VRS pending
    //$query->join('field_data_field_trip_executon_date', 'exe_date', 'exe_date.entity_id = disp_data.nid');
    $query->join('field_data_field_dispatch_date', 'dispatch_date', 'dispatch_date.entity_id = disp_data.nid');

    $query->join('field_data_field_cashier_name', 'fld_cashier', 'fld_cashier.entity_id = disp_data.nid');
    $vrs_status = array('2', '3', '4', '5');
    $query->condition('vs.field_vrs_status_value', $vrs_status, 'IN');

    $query->groupBy('ad.bill_number');
    $query->orderBy('item_details.trip_bill_reference');
    $query->fields('ad', array('bill_number', 'res_bank_name', 'res_status', 'res_cheque_date', 'res_bank_name', 'res_photo_of_cheque', 'res_photo_of_signed_bill_delivery', 'photo_of_signed_bill_partial', 'res_moneyTransactionType'));
    $query->fields('item_details', array('trip_bill_reference'));
    //$query->fields('bv',array('field_bill_value_value'));
    $query->fields('fld_cashier', array('field_cashier_name_value'));


    // payment type
    $query->fields('pt', array('field_payment_terms_value'));
    // sales register to be collected amt
    //$query->addExpression("SUM(bv.field_bill_value_value - item_details.return_value - item_details.cashier_short_value - item_details.godown_short_value)", 'sales_register_to_be_collected_amt');
    // Farery to be collected amt
    //$query->addExpression("SUM(ad.bill_value - item_details.return_value - item_details.cashier_short_value - item_details.godown_short_value)", 'to_be_collected_amt');
    // As per new ptl_vrs change changed sales register billvalue to farey bill_value
    $query->addExpression("SUM(ad.bill_value - item_details.return_value - item_details.cashier_short_value - item_details.godown_short_value - item_details.claimed_godown_short_value)", 'sales_register_to_be_collected_amt');
    // Farery to be collected amt
    $query->addExpression("SUM(ad.bill_value - item_details.return_value - item_details.cashier_short_value - item_details.godown_short_value - item_details.claimed_godown_short_value)", 'to_be_collected_amt');
    $query->addExpression("SUM(item_details.return_value + item_details.cashier_short_value + item_details.godown_short_value + item_details.claimed_godown_short_value)", 'unify_sales_return');
    $query->addExpression('ad.res_cash_amount', 'res_cash_amount');
    $query->addExpression('ad.res_cheque_amount', 'res_cheque_amount');
    $query->addExpression('ad.bill_value', 'bill_value');
    $query->addExpression('ad.bill_number', 'bill_number');
    $query->addExpression('ad.res_bank_name', 'res_bank_name');
    $query->addExpression('ad.res_status', 'res_status');
    $query->addExpression('ad.res_cheque_date', 'res_cheque_date');
    $query->addExpression('ad.res_photo_of_cheque', 'res_photo_of_cheque');
    $query->addExpression('ad.res_photo_of_signed_bill_delivery', 'res_photo_of_signed_bill_delivery');
    $query->addExpression('ad.photo_of_signed_bill_partial', 'photo_of_signed_bill_partial');
    $query->addExpression('ad.res_confirm_signed_bill_amount', 'res_confirm_signed_bill_amount');
    //// As per new ptl_vrs change changed 
    $query->addExpression('ad.bill_value', 'field_bill_value_value');
    //$query->addExpression("DATE_FORMAT(CONVERT_TZ(FROM_UNIXTIME(exe_date.field_trip_executon_date_value),'+00:00','+05:30'), '%d-%b-%Y')", 'trip_execution_date');
    $query->addExpression('ad.res_cheque_date_edit', 'res_cheque_date_edit');
    $query->addExpression('ad.res_photo_of_cheque_edit', 'res_photo_of_cheque_edit');
    $query->addExpression('ad.res_cheque_amount_edit', 'res_cheque_amount_edit');
    $query->addExpression('ad.res_money_collect2_edit', 'res_money_collect2_edit');
    $query->addExpression('ad.photo_of_signed_bill_partial_edit', 'photo_of_signed_bill_partial_edit');
    $query->addExpression('ad.res_confirm_signed_bill_amount_edit', 'res_confirm_signed_bill_amount_edit');
    $query->addExpression('ad.res_bank_name_edit', 'res_bank_name_edit');
    $query->addExpression('ad.res_photo_of_signed_bill_delivery_edit', 'res_photo_of_signed_bill_delivery_edit');
    $query->addExpression('ad.res_cash_amount_edit', 'res_cash_amount_edit');
    $query->addExpression('ad.res_signed_bill_delivery_edit', 'res_signed_bill_delivery_edit');
    $query->addExpression('ad.res_signed_bill_partial_edit', 'res_signed_bill_partial_edit');
    $query->addExpression('ad.res_cheque_no', 'res_cheque_no');
    $query->addExpression('ad.res_cheque_no_edit', 'res_cheque_no_edit');
    $query->addExpression('item_details.return_qty', 'return_qty');
    $query->addExpression('item_details.sum_net_sales_value', 'sum_net_sales_value');
    $query->addExpression('(SUM(item_details.total_delivered_value))', 'total_delivered_value');
    $query->addExpression("DATE_FORMAT(ad.date,'%d-%b-%Y')", 'dispatch_date');
    $query->addExpression("DATE_FORMAT(CONVERT_TZ(FROM_UNIXTIME(sr_bill_date.field_sr_bill_date_value),'+00:00','+05:30'), '%d-%b-%Y')", 'billing_date');
    $query->addExpression('bill_discount.field_bill_level_discount_value', 'bill_level_discount');
    $query->addexpression("(CASE WHEN bill_discount.field_bill_level_discount_value >= item_details.delivered_value THEN 1 ELSE 0 END)", 'check_delivered_value_less');
    $res = $query->execute()->fetchAll(PDO::FETCH_UNIQUE | PDO::FETCH_ASSOC);
  }
  return $res;
}

function get_van_no($trip_id, $distributor_id) {
  $res = NULL;
  if (!empty($trip_id) && !empty($distributor_id)) {
    $result = array();
    $query = db_select('node', 'n');
    $query->condition('n.type', 'hht_master');
    $query->condition('n.uid', $distributor_id, '=');
    $query->fields('n', array('title'));
    $query->leftJoin('field_data_field_hht_id', 'fh', 'fh.field_hht_id_nid = n.nid');
    $query->leftJoin('node', 'nt', 'nt.nid = fh.entity_id');
    $query->condition('nt.type', 'dispatch_data');
    $query->condition('nt.uid', $distributor_id, '=');
    $query->condition('nt.title', $trip_id, '=');
    $res = $query->execute()->fetchField();
  }
  return $res;
}

function mrc_inventory_returns_file_download($uri) {
  global $user;
  $files = file_load_multiple(array(), array('uri' => $uri));
  if (!empty($files)) {
    foreach ($files as $file) {
      $filename = $file->filename;
    }
    return array(
      'Content-type' => file_get_mimetype($uri),
      'Content-disposition' => 'attachment; filename=' . $filename
    );
  }
}

/*
function vrs_export($vrs_data, $trip_id) {
//   $file = "TripDetailsDownload.xls";
//    $data = cache_get('trip_download' . session_id());
//    if (!empty($data)) {
//    $output = $data->data;
//    }
//
//    header("Content-Type: application/force-download");
//    header("Content-type: application/vnd.ms-excel");
//    header("Content-Disposition: attachment; filename=$file");
//    echo $output; 
//  $data = cache_get('trip_download' . session_id());
//  if (!empty($data)) {
//    $trip_data = $data->data;
//  }
//  // create directory if not exist
//  $dir = 'public://vrs_trip_history';
//  file_prepare_directory($dir, FILE_CREATE_DIRECTORY);
//  $module = module_load_include('inc', 'phpexcel');
//  $wrapper = file_stream_wrapper_get_instance_by_uri($dir);
//  // Generate a file name. If it's unique, it's less likely to conflict with an
//  // existing file. You could also put up some more checks on this, if it's likely
//  // to conflict (like, when you have many export/download requests).
//
//  $filename = 'VRS-Trip-' . $trip_id . '-Details-' . date('d-m-Y') . '-' . uniqid() . '.xls';
//  $filepath = $wrapper->realpath() . '/' . $filename;
//
//  $vrs_bill_items_header = array(
//    'Trip ID',
//    'VRS Version Number',
//    'Bill Number',
//    'Delivery Outcome',
//    'Sales Register To Be Collected',
//    'Actual Cash Collected',
//    'Actual Signed Bill Amount',
//    'Signed Bill Image',
//    'Actual Cheque Amount Collected',
//    'Cheque Number',
//    'Cheque Date',
//    'Cheque Image',
//    'Bank Name',
//    'Short',
//    'Short Reason',
//    'Remarks',
//    'Is Non Bankable',
//    'Non Bankable Reason'
//  );
//
//  $vrs_total_header = array('Count of Bills', 'Total Delivery App (To be collected)', 'Total to be collected(Sales Register)', 'Total Cash Collected', 'Total Cheque Collected', 'Total Signed Bill Collected', 'RTO Charges', 'Cashier Cash Short', 'Total Short', 'Total Cashier Debit Value');
//  $result = phpexcel_export(
//    array('vrs_bill' => $vrs_bill_items_header,
//    ), array('vrs_bill' => $vrs_data,
//    ), $filepath
//  );
//
//  if ($result == 1) {
//    // Exported successfully. Let's register the file with Drupal. We simply
//    // tell Drupal to copy the file over the existing one, by passing in
//    // temporary://$filename.
//    $file = file_save_data(
//      file_get_contents($filepath), "$dir/$filename", FILE_EXISTS_REPLACE
//    );
//
//    // By default, the file is stored as a permanent file. Let's make it
//    // temporary, so Drupal will remove it (in 6 hours, if your cron is set up
//    // correctly).
//    $file->status = 0;
//    file_save($file);
//    $file->uri;
//    // Start downloading. This requires a hook_file_download() implementation!
//    $headers = file_download_headers($file->uri);
//    transfer_file($file, $headers);
//  }
//  else {
//    echo "error";
//  }
}
*/

function transfer_file($file, $headers) {
  $uri = $file->uri;
  if (ob_get_level()) {
    ob_end_clean();
  }

  foreach ($headers as $name => $value) {
    drupal_add_http_header($name, $value);
  }
  drupal_send_headers();
  $scheme = file_uri_scheme($uri);
  // Transfer file in 1024 byte chunks to save memory usage.
  if ($scheme && file_stream_wrapper_valid_scheme($scheme) && $fd = fopen($uri, 'rb')) {
    while (!feof($fd)) {
      print fread($fd, 1024);
    }
    fclose($fd);
  }
  else {
    drupal_not_found();
  }
  //file_unmanaged_delete($uri);
  file_delete($file, TRUE);
  drupal_exit();
}

/*
  function get_trip_vrs_versions() {
    $params = drupal_get_query_parameters();
    if(!empty($params)){
      $trip_id = $params['trip_id'];
      $dispatch_date_from = $params['dispatch_date_from'];
      $dispatch_date_to = $params['dispatch_date_to'];
      $cashier_name = $params['cashier_name'];
      if (!empty($trip_id)) {
        $trip_ids = array($trip_id);
      }
      else if ((!empty($dispatch_date_from) && !empty($dispatch_date_to)) || !empty($cashier_name)) {
        $trip_ids = get_trip_ids_by_date_range($dispatch_date_from, $dispatch_date_to, $cashier_name);
        if (!empty($trip_ids)) {
          $trip_ids = array($trip_ids);
        }
      }

      $trip_ids_arr = array();
      $trip_ids_arr = explode(',', $trip_ids[0]);
      foreach ($trip_ids_arr as $trip_id) {
        if (!empty($trip_id)) {
          global $user, $base_url;
          $bill_items = array('vrs_version_no', 'bill_number',
            'vrs_id',
            'trip_id',
            'bill_status',
            'to_be_collected_amt_sales_register',
            'to_be_collected_amt',
            'actual_cash_amount',
            'actual_cheque_amount',
            'actual_signed_bill_amount',
            'cheque_number',
            'cheque_date',
            'bank_name',
            'short_amt',
            'reason_code',
            'vrs_created_date',
            'vrs_confirm_date',
            'distributor_id',
            'remarks',
            'non_billable',
            'photo_cheque',
            'photo_signed_bill',
            'non_bank_reason_code'
          );
          $vrs_total = array(
            'vrs_version_no',
            'trip_id',
            'vrs_id',
            'bill_count',
            'total_to_be_collected_sales_register',
            'total_to_be_collected_amt',
            'total_actual_cash_amount',
            'total_actual_cheque_amount',
            'total_actual_signed_bill_amount',
            'total_short_amt',
            'total_cashier_debit_value',
            'total_cashier_cash_short',
            'cheque_count',
            'sb_count',
            'vrs_created_date',
            'vrs_confirm_date',
            'distributor_id',
            'rto_charges'
          );
          $dis_id = change_feeds_import_get_distributor_id($user->uid);
          $query = db_select('vrs_trip_items_history', 'vr_items');
          $query->condition('vr_items.distributor_id', $dis_id, '=');
          $query->condition('vr_items.trip_id', $trip_id, '=');
          //$query->join('vrs_trip_total_history', 'vr_total', 'vr_total.vrs_version_no = vr_items.vrs_version_no');
          $query->orderBy('vr_items.id', 'desc');
          //$query->groupBy('vr_items.vrs_version_no,vr_items.bill_number,vr_total.vrs_version_no');
          $query->fields('vr_items', $bill_items);
          //$query->fields('vr_total',$vrs_total);
          $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

          $query1 = db_select('vrs_trip_total_history', 'vr_total');
          $query1->condition('vr_total.distributor_id', $dis_id, '=');
          $query1->condition('vr_total.trip_id', $trip_id, '=');
          $query1->orderBy('vr_total.id', 'desc');
          $query1->fields('vr_total', $vrs_total);
          $res1 = $query1->execute()->fetchAll(PDO::FETCH_UNIQUE | PDO::FETCH_ASSOC);
          $vrs = array();
          $vrs_data = array();
          $short_reasons = get_short_reasons();
          $non_bankable_reasons = get_non_bankable_reasons();
          if (!empty($res)) {
            if (!empty($res)) {
              foreach ($res as $key => $value) {
                $vrs[$value['vrs_version_no']][] = $value + $res1[$value['vrs_version_no']];
              }
            }
            $asset = received_assets_count($trip_id, $dis_id);
            foreach ($vrs as $key => &$val) {
              foreach ($val as $key1 => &$value) {
                switch ($value['bill_status']) {
                  case 'partial_deliver';
                    $bill_status = t('Partial Delivered');
                    break;

                  case 'deliver';
                    $bill_status = t('Delivered');
                    break;

                  case 'undeliver';
                    $undeliver = 1;
                    $bill_status = t('Full Returned');
                    break;
                  defualt:
                    $bill_status = t('NA');
                }
                $value['bill_status'] = $bill_status;
                if (!empty($value['cheque_date'])) {
                  $value['cheque_date'] = date('Y-m-d', $value['cheque_date']);
                }
                else {
                  $value['cheque_date'] = '';
                }
                if (!empty($value['non_billable'])) {
                  $value['non_billable'] = t('Yes');
                }
                else {
                  $value['non_billable'] = t('No');
                }
                if (!empty($value['short_amt'])) {
                  foreach ($short_reasons as $code => $short_value) {
                    if ($value['reason_code'] == $code) {
                      $value['reason_code'] = $short_value;
                    }
                  }
                }
                if (!empty($value['non_bank_reason_code'])) {
                  foreach ($non_bankable_reasons as $code => $reason_value) {
                    if ($value['non_bank_reason_code'] == $code) {
                      $value['non_bank_reason_code'] = $reason_value;
                    }
                  }
                }
                else {
                  $value['non_bank_reason_code'] = t('');
                }
                $vrs_data[] = array(
                  $value['trip_id'],
                  $value['vrs_version_no'],
                  $value['bill_number'],
                  $value['bill_status'],
                  $value['to_be_collected_amt_sales_register'],
                  $value['actual_cash_amount'],
                  $value['actual_signed_bill_amount'],
                  $value['photo_signed_bill'],
                  $value['actual_cheque_amount'],
                  $value['cheque_number'],
                  $value['cheque_date'],
                  $value['photo_cheque'],
                  $value['bank_name'],
                  $value['short_amt'],
                  $value['reason_code'],
                  $value['remarks'],
                  $value['non_billable'],
                  $value['non_bank_reason_code'],
                );
                $bill_count = $value['bill_count'];
                $total_tobe_collected = $value['total_to_be_collected_sales_register'];
                $total_cash = $value['total_actual_cash_amount'];
                $total_cheque = $value['total_actual_cheque_amount'];
                $total_signed_bill = $value['total_actual_signed_bill_amount'];
                $total_short = $value['total_short_amt'];
                $rto_charges = $value['rto_charges'];
                $total_cashier_short = $value['total_cashier_cash_short'];
                $total_cashier_debit_value = $value['total_cashier_debit_value'];
              }
              $vrs_data[] = array('Bill count', 'Total to be collected', 'Total Actual Cash', 'Total Cheque Amt', 'Total Signed Bill', 'RTO Charges', 'Total Short', 'Total Cashier Short', 'Total Cashier Debit Value');
              $vrs_data[] = array($bill_count, $total_tobe_collected, $total_cash, $total_cheque, $total_signed_bill, $rto_charges, $total_short, $total_cashier_short, $total_cashier_debit_value);
              unset($bill_count);
              unset($total_tobe_collected);
              unset($total_cash);
              unset($total_cheque);
              unset($total_signed_bill);
              unset($total_cashier_short);
              unset($total_cashier_debit_value);
            }
            vrs_export($vrs_data, $trip_id);
          }
        }
      }
    }
    //  $query->leftjoin('dispatch_planning_item_details', 'dpi', 'dpi.trip_bill_reference = ad.id');
    //  $query->groupBy('dpi.trip_bill_reference');  
  }
 */

function dispatch_date_from_validation(){
}

function dispatch_date_to_validation(){
}
