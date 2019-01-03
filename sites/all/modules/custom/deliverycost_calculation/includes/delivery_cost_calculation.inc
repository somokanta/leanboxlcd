<?php

function resource_extra_working_form($form, &$form_state) {
  $page = arg(0);
  if ($page == 'resource') {
    $header = array(
      'Resource Type',
      'Resource Name',
    );
    $options = get_all_resources();
  }
  elseif ($page == 'vehicle') {
    $header = array(
      'Vehicle Number',
    );
    $options = get_all_running_vehicles();
  }
  $form['#attached']['js'][] = drupal_get_path('module', 'deliverycost_calculation') . '/js/delivery_cost.js';
  $form['#attached']['css'][] = drupal_get_path('module', 'deliverycost_calculation') . '/css/delivery_cost.css';
  $form['#prefix'] = '<div id="itemlist-div" class="custom_form_wrapper">';
  $form['#suffix'] = '</div>';

  $form['extra_working_date'] = array(
    '#type' => 'date_popup',
    '#title' => t('Enter Extra Working Day'),
    '#required' => TRUE,
    '#date_format' => 'Y-m-d',
    '#attributes' => array('class' => array('dispatch_date')),
    '#date_label_position' => '',
    '#date_year_range' => '-20:+2',
    '#size' => 30,
    '#prefix' => '<div class="col_33">',
    '#suffix' => '</div>'
  );


  $form['list_form_items'] = array(
    '#type' => 'tableselect',
    '#required' => TRUE,
    '#header' => $header,
    '#options' => $options,
    '#prefix' => '<div id="table-div_dc">',
    '#suffix' => '</div>',
    '#empty' => t('No forms available.'),
    '#multiple' => TRUE,
  );
  $form['resource_vehicle'] = array('#type' => 'hidden', '#value' => $page);
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Save Extra Working Day',
    '#name' => 'Save Extra Working Day',
    '#attributes' => array('class' => array('vrs-trip-submit')),
    '#submit' => array('submit_extra_working_days'),
    '#validate' => array('validate_extra_working_days'),
    '#prefix' => '',
    '#suffix' => '</div></div>'
  );

  return $form;
}

function check_resource_duplicate_extra_working_days($resource_nid, $date) {
  if (!empty($resource_nid) && !empty($date)) {
    $result = '';
    global $user;
    $distributor_id = change_feeds_import_get_distributor_id($user->uid);
    $query = db_select('node', 'n');
    $query->join('field_data_field_resource', 'resource', 'resource.entity_id = n.nid');
    $query->join('field_data_field_start_period', 'start_period', 'start_period.entity_id = n.nid');
    $query->condition('resource.field_resource_nid', $resource_nid);
    $query->condition('start_period.field_start_period_value', $date, '=');
    $query->condition('n.type', 'resource_extra_working');
    $query->condition('n.uid', $distributor_id);
    $query->fields('n', array('nid'));
    $result = $query->execute()->fetchField();
  }
  return $result;
}

function check_vehicle_duplicate_extra_working_days($vehicle_nid, $date) {
  if (!empty($vehicle_nid) && !empty($date)) {
    $result = '';
    global $user;
    $distributor_id = change_feeds_import_get_distributor_id($user->uid);
    $query = db_select('node', 'n');
    $query->join('field_data_field_vehicle_number_ref', 'vehicle', 'vehicle.entity_id = n.nid');
    $query->join('field_data_field_start_period', 'start_period', 'start_period.entity_id = n.nid');
    $query->condition('vehicle.field_vehicle_number_ref_nid', $vehicle_nid);
    $query->condition('start_period.field_start_period_value', $date, '=');
    $query->condition('n.type', 'vehicle_extra_working');
    $query->condition('n.uid', $distributor_id);
    $query->fields('n', array('nid'));
    $result = $query->execute()->fetchField();
  }
  return $result;
}

function validate_extra_working_days($form, &$form_state) {
  $resource_vehicle = $form_state['values']['resource_vehicle'];
  $resource_list = $form_state['values']['list_form_items'];
  //print_r(array_values($form_state['values']['list_form_items']));
  $extra_working_date = strtotime($form_state['values']['extra_working_date']);
  $day_name = strtolower(date('l', $extra_working_date));

  $sum = 0;
  if ($resource_vehicle == 'vehicle') {
    $mess = t('Please select at least one vehicle');
  }
  elseif ($resource_vehicle == 'resource') {
    $mess = t('Please select at least one resource');
  }
  $mess1 = '';
  $public_holidays = get_public_holiday();
  $vehicle_weekly_off = strtolower(get_weekly_off_day());
  foreach ($resource_list as $key => $value) {
    if (!empty($value)) {
      $sum += $value;
      if ($resource_vehicle == 'resource') {
        $resource_obj = node_load($key);
        $nid = check_resource_duplicate_extra_working_days($key, $extra_working_date);
        if ($nid) {
          $mess1 .= t('Duplicate extra working days of resource:' . $resource_obj->title) . '<br>';
        }
        $resource_weekly_off = $resource_obj->field_weekly_off[LANGUAGE_NONE][0]['value'];
        if (!empty($resource_weekly_off)) {
          $resource_weekly_off = strtolower($resource_weekly_off);
        }
        else {
          $resource_weekly_off = strtolower(get_resource_weekly_off_day());
        }

        if (!in_array($form_state['values']['extra_working_date'], $public_holidays) && $resource_weekly_off !== $day_name) {
          $mess1 .= t('Please select either public holiday or weekly off for resource:' . $resource_obj->title) . '<br>';
        }
      }
      elseif ($resource_vehicle == 'vehicle') {
        $nid = check_vehicle_duplicate_extra_working_days($key, $extra_working_date);
        $vehicle_obj = node_load($key);
        if ($nid) {
          $mess1 .= t('Duplicate extra working days of vehicle:' . $vehicle_obj->title) . '<br>';
        }
        if (!in_array($form_state['values']['extra_working_date'], $public_holidays) && $vehicle_weekly_off !== $day_name) {
          $mess1 .= t('Please select either public holiday or weekly off day for vehicle:' . $vehicle_obj->title) . '<br>';
        }
      }
    }
  }
  if ($sum == 0) {
    form_set_error('form', $mess);
    $form_state['#rebuild'] = TRUE;
  }
  if (!empty($mess1)) {
    form_set_error('form', $mess1);
  }
}

function submit_extra_working_days($form, &$form_state) {
  global $user;
  $distributor_id = change_feeds_import_get_distributor_id($user->uid);
  $resource_list = $form_state['values']['list_form_items'];
  $extra_working_date = strtotime($form_state['values']['extra_working_date']);
  $resource_vehicle = $form_state['values']['resource_vehicle'];
  if ($resource_vehicle == 'vehicle') {
    $type = 'vehicle_extra_working';
  }
  elseif ($resource_vehicle == 'resource') {
    $type = 'resource_extra_working';
  }
  foreach ($resource_list as $key => $value) {
    if (!empty($value)) {
      $resource_node = node_load($key);
      $node = (object) array(
          'uid' => $distributor_id,
          'name' => (isset($user->name) ? $user->name : ''),
          'type' => $type,
          'language' => LANGUAGE_NONE,
      );
      $node->title = 'extra_working_' . $resource_node->title;
      $node->field_resource[LANGUAGE_NONE][0]['nid'] = $key;
      $node->field_start_period[LANGUAGE_NONE][0]['value'] = $extra_working_date;
      $node->field_vehicle_number_ref[LANGUAGE_NONE][0]['nid'] = $key;
      node_save($node);
    }
  }
  drupal_set_message(t('Successfully Saved.'));
}

function get_weekly_off_day($resource_nid) {
  global $user;
  $distributor_id = change_feeds_import_get_distributor_id($user->uid);
  $query = db_select('node', 'n');
  $query->condition('n.uid', $distributor_id);
  $query->condition('n.type', 'pdd_master');
  $query->fields('n', array('nid'));
  $pdd_value_result = $query->execute()->fetchField();
  $pdd_data = node_load($pdd_value_result);
  $weekly = $pdd_data->field_day[LANGUAGE_NONE];
  foreach ($weekly as $key => $val) {
    $weeklyoff = $val['value'];
  }

  return $weeklyoff;
}

function delivery_cost_calculation_mtd_form($form, &$form_state) {
  $form['#attached']['css'][] = drupal_get_path('module', 'deliverycost_calculation') . '/css/delivery_cost.css';
  $form['#attached']['js'][] = drupal_get_path('module', 'deliverycost_calculation') . '/js/delivery_cost.js';
  if (!isset($form_state['delivery_cost_details'])) {
    $form_state['delivery_cost_details'] = array();
  }
  $options = $form_state['delivery_cost_details'];
  $form['#prefix'] = '<div id="itemlist-div" class="custom_form_wrapper">';
  $form['#suffix'] = '</div>';
  $form['dispatch_from_date'] = array(
    '#type' => 'date_popup',
    '#title' => t('From Date*'),
    '#date_format' => 'Y-m-d',
    '#attributes' => array('class' => array('dispatch_date')),
    '#date_label_position' => '',
    '#date_year_range' => '-20:+2',
    '#size' => 30,
    '#prefix' => '<div class="form_row form_row2"><div class="col_3">',
    '#suffix' => '</div>'
  );
  $form['dispatch_to_date'] = array(
    '#type' => 'date_popup',
    '#title' => t('To Date*'),
    '#date_format' => 'Y-m-d',
    '#attributes' => array('class' => array('dispatch_date')),
    '#date_label_position' => '',
    '#date_year_range' => '-20:+2',
    '#size' => 30,
    '#prefix' => '<div class="form_row form_row2"><div class="col_3">',
    '#suffix' => '</div></div>'
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Show',
    '#name' => 'show',
    /* '#ajax' => array(
      'callback' => 'delivery_cost_callback',
      'wrapper' => 'itemlist-div',
      'method' => 'replace',
      'effect' => 'fade',
      ), */
    '#attributes' => array('class' => array('vrs-trip-submit')),
    '#submit' => array('get_delivery_cost_details_mtd'),
    '#validate' => array('validate_delivery_cost_details_mtd'),
    '#prefix' => '<div class="col_3">',
    '#suffix' => '</div></div>'
  );
  if (!empty($options)) {
    $header = array(
      'Vehicle Number',
      'Owner',
      'Cashier',
      'Trips',
      'Dispatched Value(Rs Lakhs)',
      'Delivered Value(Rs Lakhs)',
      'Returns Value(Rs Lakhs)',
      'Average Delivered Value Per Trip',
      'Total Cost (Rs Lakhs)',
      'Total Cost (for Delivered)',
      'Average Cost Per Day (Rs Lakhs)',
      'Return % Sale(vs Delivered)',
      'Return % Sale(vs Dispatched)',
      'DP Value',
      'DP % Value',
      //'Cost % Sale',
      'Cost % Sale vs Dispatched',
      'Cost % Sale vs Delivered',
    );
    $form['delivery_cost_table'] = array(
      '#theme' => 'table',
      '#prefix' => '<div class="delivery_cost_table custom_middle_table">',
      '#tree' => TRUE,
      '#header' => $header,
      '#rows' => $options,
      '#attributes' => array(
        'class' => array('delivery_cost'),
        'id' => 'delivery_cost'
      ),
      '#suffix' => '</div>',
      '#sticky' => FALSE,
    );
    cache_set('delivery_cost_download_mtd' . session_id(), $form['delivery_cost_table'], 'cache', time() + 60 * 10);
    $rr = l('Download', "delivery_cost/export/mtd", array('attributes' => array("target" => "_blank"), 'query' => array('dispatch_from_date' => $form_state['values']['dispatch_from_date'], 'dispatch_to_date' => $form_state['values']['dispatch_to_date'])));
    $form['download_mark'] = array(
      '#type' => 'markup',
      '#markup' => $rr,
    );
  }
  return $form;
}

function delivery_cost_calculation_form($form, &$form_state) {
  $form['#attached']['css'][] = drupal_get_path('module', 'deliverycost_calculation') . '/css/delivery_cost.css';
  $form['#attached']['js'][] = drupal_get_path('module', 'deliverycost_calculation') . '/js/delivery_cost.js';
  if (!isset($form_state['delivery_cost_details'])) {
    $form_state['delivery_cost_details'] = array();
  }
  $options = $form_state['delivery_cost_details'];
  $form['#prefix'] = '<div id="itemlist-div" class="custom_form_wrapper">';
  $form['#suffix'] = '</div>';
  $form['trip_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Trip ID*'),
    '#size' => 20,
    '#maxlength' => 50,
    '#attributes' => array('class' => array('trip-id'), 'id' => 'trip-id'),
    '#prefix' => '<div class="form_row form_row2"><div class="col_3">',
  );
  $form['trip_id_wrapper']['trip_id'] = array(
    '#type' => 'markup',
    '#title' => t('OR'),
    '#prefix' => '<div id="after_trip_id" class="text-center font-weight-bold or_txt or_txt1"> OR',
    '#suffix' => '</div></div>'
  );
  $form['dispatch_date'] = array(
    '#type' => 'date_popup',
    '#title' => t('Dispatch Date*'),
    '#date_format' => 'Y-m-d',
    '#attributes' => array('class' => array('dispatch_date')),
    '#date_label_position' => '',
    '#date_year_range' => '-20:+2',
    '#size' => 30,
    '#prefix' => '<div class="col_3">',
    '#suffix' => '</div>'
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Show',
    '#name' => 'show',
    /* '#ajax' => array(
      'callback' => 'delivery_cost_callback',
      'wrapper' => 'itemlist-div',
      'method' => 'replace',
      'effect' => 'fade',
      ), */
    '#attributes' => array('class' => array('vrs-trip-submit')),
    '#submit' => array('get_delivery_cost_details'),
    //'#validate' => array('validate_delivery_cost_details'),
    '#prefix' => '',
    '#suffix' => '</div></div>'
  );
  if (!empty($options)) {
    $header = array(
      '#',
      'Vehicle Number',
      'Trip ID',
      'Vehicle Type',
      'Owner',
      'Cashier',
      'Dispatched Value(Rs Lakhs)',
      'Delivered Value(Rs Lakhs)',
      'Returns Value(Rs Lakhs)',
      'Returns %(vs Delivered)',
      'Returns %(vs Dispatched)',
      'DP Value',
      'DP %',
      'Total Cost for the day',
      'Total Cost (for Delivered)',
      'Cost as a % of Dispatched Value',
      'Cost as a % of Delivered Value',
    );
    $form['delivery_cost_table'] = array(
      '#theme' => 'table',
      '#prefix' => '<div class="delivery_cost_table">',
      '#tree' => TRUE,
      '#header' => $header,
      '#rows' => $options,
      '#attributes' => array(
        'class' => array('delivery_cost'),
        'id' => 'delivery_cost'
      ),
      '#suffix' => '</div>',
      '#sticky' => FALSE,
    );
    cache_set('delivery_cost_download' . session_id(), $form['delivery_cost_table'], 'cache', time() + 60 * 10);
    $output .= l(t('XLS'), 'return-vrs-time-report/export', array('query' => array('dispatch_date' => $form_state['values']['dispatch_date'])));
    $rr = l('Download', "delivery_cost/export/daywise", array('attributes' => array("target" => "_blank"), 'query' => array('dispatch_date' => $form_state['values']['dispatch_date'], 'trip_id' => $form_state['values']['trip_id'])));
    $form['download_mark'] = array(
      '#type' => 'markup',
      '#markup' => $rr,
    );
  }
  return $form;
}

function delivery_cost_trip_id_callback($form, &$form_state) {
  return $form['trips'];
}

function delivery_cost_callback($form, &$form_state) {
  $commands[] = ajax_command_replace('#itemlist-div', render($form));
  $commands[] = array
    (
    // The command will be used in our JavaScript file (see next section)
    'command' => 'bill_status_undeliver_filters',
    // We pass the value that the user selected in the select element to our
    // JavaScript function:
    //'selectedValue' => 123,
  );

  return array('#type' => 'ajax', '#commands' => $commands);
}

function get_dp_value($trip_id) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);
  $dp_value = 0;
  if (!empty($trip_id)) {
    $query = db_select('dispatch_planning_api_data', 'ad');
    $query->join('dispatch_planning_item_details', 'dpi', 'dpi.trip_bill_reference = ad.id');
    $query->addExpression("(SUM(dpi.sum_net_sales_quantity * dpi.sum_net_sales_value))", 'dp_value');
    $query->condition('ad.distributor_id', $dis_id, '=');
    $query->condition('ad.delivery_pending_flag', 1, '=');
    $query->condition('ad.runsheet_number', $trip_id, 'IN');
    $dp_value = $query->execute()->fetchField();
  }
  return $dp_value;
}

function get_dp_value_vehicle_number($trip_id, $vehicle_number) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);
  $dp_value = 0;
  if (!empty($vehicle_number) && !empty($trip_id)) {
    $query = db_select('dispatch_planning_api_data', 'ad');
    $query->join('dispatch_planning_item_details', 'dpi', 'dpi.trip_bill_reference = ad.id');
    $query->addExpression("(SUM(dpi.sum_net_sales_quantity * dpi.sum_net_sales_value))", 'dp_value');
    $query->condition('ad.distributor_id', $dis_id, '=');
    $query->condition('ad.delivery_pending_flag', 1, '=');
    $query->condition('ad.runsheet_number', $trip_id, 'IN');
    $query->condition('ad.vehicle', $vehicle_number);
    $query->groupBy('ad.vehicle');
    $dp_value = $query->execute()->fetchField();
  }
  return $dp_value;
}

// return value
function get_return_value($trip_id) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);
  $return_value = 0;
  if (!empty($trip_id)) {
    $query = db_select('dispatch_planning_api_data', 'ad');
    $query->join('dispatch_planning_item_details', 'dpi', 'dpi.trip_bill_reference = ad.id');
    $query->addExpression("SUM(dpi.return_qty * dpi.sum_net_sales_value)", 'return_value');
    $query->condition('ad.distributor_id', $dis_id, '=');
    $query->condition('ad.delivery_pending_flag', 1, '!=');
    $query->condition('ad.runsheet_number', $trip_id, 'IN');
    $return_value = $query->execute()->fetchField();
  }
  return $return_value;
}

function get_delivery_cost_details_mtd($form, &$form_state, $export = 0) {
  global $user;
  $rows = array();
  $trip_ids = array();
  $dis_id = change_feeds_import_get_distributor_id($user->uid);
  $dispatch_from_date = $form_state['values']['dispatch_from_date'];
  $dispatch_to_date = $form_state['values']['dispatch_to_date'];
  $round_off_no_digits = 3;
  $round_off_no_digits_2 = 2;
  $lak = 100000;
  if (!empty($dispatch_from_date) && !empty($dispatch_to_date)) {
    $all_vehicles = get_all_active_vehicles_on_date_range(strtotime($dispatch_from_date), strtotime($dispatch_to_date));
    $absent_vehilce_details = get_maintenance_dates_of_vehicle(strtotime($dispatch_from_date), strtotime($dispatch_to_date));
    $all_extra_working = get_extra_working_days(strtotime($dispatch_from_date), strtotime($dispatch_to_date));
    $weekoff = get_resource_weekly_off_day();
    $dis_date = strtotime($dispatch_from_date);
    $year = date('Y', $dis_date);
    $month = date('m', $dis_date);
    $no_of_working_days = calculateWorkingDaysInMonth($year, $month, $weekoff);
    //print_r($all_vehicles);die;
  }
  $mtd_data = array();
  $rows = array();
  $trip_created_days = array();
  if (!empty($all_vehicles)) {
    // Vehicle related total fields
    $vh_total_dispatched_value_sum = $vh_total_return_value_sum = $vh_total_dp_value_sum = $vh_total_total_cost = $vh_total_delivered_value_sum = $vh_total_cost_per_day = $vh_total_trip_count = 0;
    $vh_return_percent_sale_vs_delivered = 0;
    $vh_return_percent_sale_vs_dispatched = 0;
    $vh_total_dp_percent = 0;
    $vh_total_cost_percent_dispatched = 0;
    $vh_total_cost_percent_delivered = 0;
    $vh_total_avg_trip_count = 0;
    $vh_total_delivred_cost = 0;
    foreach ($all_vehicles as $vehicle_number_value => $vehicle_values) {
      $vehicle_total_delivered_value = 0;
      $vehicle_total_dispatched_value = 0;
      // Vehicle,cashier specific total variables
      $total_dispatched_value_sum = $total_return_value_sum = $total_dp_value_sum = $total_total_cost = $total_delivered_value_sum = $total_cost_per_day = $total_trip_count = 0;
      $total_avg_trip_count = 0;
      $total_return_percent_delivered = 0;
      $total_average_cost_per_day = 0;
      $total_return_percent_dispatched = 0;
      $total_average_delivered_value_per_trip = 0;
      $total_cost_percent_dispatched = 0;
      $total_cost_percent_delivered = 0;
      $total_avg_cost_trip = 0;
      $total_dp_percent = 0;
      $total_delivred_cost = 0;
      $mtd_result = array();
      $vehicle_cost_calc = get_vehilce_per_day_cost($vehicle_number_value);
      $query = db_select('dispatch_planning_api_data', 'ad');
      $query->condition('ad.distributor_id', $dis_id, '=');
      // $query->condition('ad.delivery_pending_flag', 1, '!=');
      $query->condition('ad.date', array($dispatch_from_date, $dispatch_to_date), 'BETWEEN');
      $query->join('node', 'n', 'n.title = ad.bill_number AND n.uid = ad.distributor_id');
      $query->condition('n.type', 'sales_register_data', '=');
      $query->condition('n.uid', $dis_id);
      $query->join('dispatch_planning_item_details', 'item', 'item.trip_bill_reference = ad.id');
      //$query->addExpression("(SUM(item.return_qty * item.sum_net_sales_value))", 'return_value');
      $query->addExpression("SUM(item.return_qty * item.sum_net_sales_value)", 'return_value');
      $query->addExpression("(SUM(item.res_sku_actual_quantity1 * item.sum_net_sales_value))", 'delivered_value');
      $query->addExpression("(SUM(item.sum_net_sales_quantity * item.sum_net_sales_value))", 'dispatched_value');
      // Join with dispatch_data to check vrs status and return flage 
      $query->join('node', 'disp_data', 'disp_data.title = ad.runsheet_number');
      $query->condition('disp_data.type', 'dispatch_data');
      // $query->leftjoin('field_data_field_delivery_boy', 'delivery_boys', 'delivery_boys.entity_id = disp_data.nid');
      //$query->leftjoin('field_data_field_resource_name', 'resource_names', 'resource_names.entity_id = delivery_boys.field_delivery_boy_nid');
      $query->condition('disp_data .uid', $dis_id);
      //$query->leftjoin('field_data_field_vrs_status', 'vs', 'vs.entity_id = disp_data.nid');
      $query->leftjoin('field_data_field_vehicle_no', 'vehicle_no', 'vehicle_no.entity_id = disp_data.nid');
      // VRS Screen Return flag = 1 and VRS status = 1, 3 Means VRS pending
      $query->leftjoin('field_data_field_cashier_name', 'fld_cashier', 'fld_cashier.entity_id = disp_data.nid');
      // $vrs_status = array('1', '2', '3', '4', '5','-1','0');
      //$query->condition('vs.field_vrs_status_value', $vrs_status, 'IN');
      // Join with vehicle master
      $query->leftJoin('node', 'vehicle', 'vehicle.title = ad.vehicle');
      $query->condition('vehicle.type', 'vehicle_master');
      $query->leftjoin('field_data_field_vehicle_owner', 'vehicle_owner', 'vehicle_owner.entity_id = vehicle.nid');
      $query->leftjoin('field_data_field_vehicle_type', 'vehicle_type', 'vehicle_type.entity_id = vehicle.nid');
      $query->leftjoin('field_data_field_net_sales_bill_value', 'dispatched_value', 'dispatched_value.entity_id = disp_data.nid');
      $query->condition('vehicle.uid', $dis_id);
      // $query->groupBy('ad.runsheet_number');
      $query->groupBy('vehicle_no.field_vehicle_no_value,fld_cashier.field_cashier_name_value,ad.date');
      $query->addExpression('ad.date', 'dispacth_date');
      $query->fields('ad', array('runsheet_number'));
      $query->fields('fld_cashier', array('field_cashier_name_value'));
      $query->fields('vehicle_owner', array('field_vehicle_owner_value'));
      $query->fields('vehicle_type', array('field_vehicle_type_value'));
      $query->fields('dispatched_value', array('field_net_sales_bill_value_value'));
      $query->addExpression('vehicle_no.field_vehicle_no_value', 'vehicle_number');
      $query->condition('vehicle_no.field_vehicle_no_value', $vehicle_number_value);
      $query->addExpression("GROUP_CONCAT(DISTINCT ad.runsheet_number  separator ',' )", 'trip_ids');
      //$query->condition('vehicle_no.field_vehicle_no_value', 'MHO4EB0732');
      $res = $query->execute()->fetchAll(PDO::FETCH_UNIQUE | PDO::FETCH_ASSOC);
      //echo $vehiclenumber;
      $counter = 1;
      if (!empty($res)) {
        $trip_titles = array_keys($res);
        $delivery_boy_trip = get_deliveryboys($trip_titles);
        $trip_ids = array_keys($res);
        $total_dispatched_value = 0;
        $total_return_value = 0;
        $total_dp_value = 0;
        $total_return_percent = 0;
        $total_dp_percent = 0;
        $vh_not_utilized_non_working_days = array();
        $vh_not_ampted = array();
        $sl_index = 1;
        foreach ($res as $key => &$value) { //print_r($value);
          //$total_trips_array['trips'][$value['trip_ids']] = $value['dispacth_date'];
          //$total_trips_array['dispatch_date'][] = $value['dispacth_date'];
          $total_trips = explode(',', $value['trip_ids']);
          if (!empty($value['dispacth_date'])) {
            $dis_date = strtotime($value['dispacth_date']);
            $year = date('Y', $dis_date);
            $month = date('m', $dis_date);
            $no_of_working_days = calculateWorkingDaysInMonth($year, $month, $weekoff);
            $public_holidays = get_public_holiday($month, $year);
            $trip_created_days[$value['vehicle_number']][$value['dispacth_date']] = $value['dispacth_date'];
          }
          $vehicles_not_attempted_trips = array();
          // Not considering weekly off only calendar days for resources
          $resource_no_of_working_days = calculateWorkingDaysInMonth($year, $month);
          $vehicle_per_day_cost = 0;
          $delivery_boy_cost = 0; // Multiple delivery boy will be in one trip
          $cashier_cost = 0;
          $total_trip_delivery_cost = 0;
          $total_trip_vrs_delivered_cost = 0;
          //$dispatched_value_in_lak = round($value['field_net_sales_bill_value_value'] / $lak, 2);
          $dispatched_value_in_lak = round($value['dispatched_value'] / $lak, $round_off_no_digits);
          // ignore dp bills
          $return_value_trip_id = get_return_value($total_trips);
          if (!empty($return_value_trip_id)) {
            $return_value_in_lak = round($return_value_trip_id / $lak, $round_off_no_digits);
          }
          //$return_value_in_lak = round($value['return_value'] / $lak, $round_off_no_digits);
          $delivered_value_in_lak = round($value['delivered_value'] / $lak, $round_off_no_digits);
          $return_percent = ($dispatched_value_in_lak / $return_value_in_lak) * 100;
          $dp_value = get_dp_value($total_trips);
          $dp_value_in_lak = 0;
          $dp_percent = 0;
          if (!empty($dp_value)) {
            $dp_value_in_lak = round($dp_value / $lak, $round_off_no_digits);
            $dp_value_percent = ($dp_value_in_lak / $dispatched_value_in_lak) * 100;
          }
          // If the vehicle is contracted then no of working days will change based o pdd & weekoff
          //echo $value['vehicle_number'];die;
          $vehicle_cost = get_vehilce_per_day_cost($value['vehicle_number']);
          if (!empty($vehicle_cost[$value['vehicle_number']]['cost_per_month'])) {
            $vehilce_monthly_cost = $vehicle_cost[$value['vehicle_number']]['cost_per_month'];
            $vehicle_per_day_cost = $vehilce_monthly_cost / $no_of_working_days;
          }
          else if (!empty($vehicle_cost[$value['vehicle_number']]['cost_per_day'])) {
            $vehicle_per_day_cost = $vehicle_cost[$value['vehicle_number']]['cost_per_day'];
          }
          $input_resource_type = 'Cashier';
          $cashier_name = $value['field_cashier_name_value'];
          //$cashier_cost = get_resource_cost_per_day($cashier_name, $input_resource_type);
          $cashier_data = get_resource_cost_per_day($cashier_name, $input_resource_type);
          $trip_created_days_resources[$cashier_name][$value['dispacth_date']] = $value['dispacth_date'];
          if (!empty($cashier_data[$cashier_name]['cost_per_month'])) {
            $cashier_cost = $cashier_data[$cashier_name]['cost_per_month'] / $resource_no_of_working_days;
          }

          $delivery_boy_trip = get_deliveryboys(array($key));
          if (!empty($delivery_boy_trip[$key]['delivery_boys'])) {
            $delivery_boys = array_unique(explode(',', $delivery_boy_trip[$key]['delivery_boys']));
            if (!empty($delivery_boys)) {
              foreach ($delivery_boys as $delivery_boy_value) {
                $input_resource_type = 'Delivery Boy';
                $delivery_boy_name = $delivery_boy_value;
                $trip_created_days_resources[$delivery_boy_name][$value['dispacth_date']] = $value['dispacth_date'];
                //$delivery_boy_cost += get_resource_cost_per_day($delivery_boy_name, $input_resource_type);
                $deliveryboy_data = get_resource_cost_per_day($delivery_boy_name, $input_resource_type);
                if (!empty($deliveryboy_data[$delivery_boy_name]['cost_per_month'])) {

                  $delivery_boy_cost += $deliveryboy_data[$delivery_boy_name]['cost_per_month'] / $resource_no_of_working_days;
                }
              }
            }
          }
          $total_trip_delivery_cost = ($delivery_boy_cost + $cashier_cost + $vehicle_per_day_cost) / $lak;
          $total_trip_vrs_delivered_cost = $total_trip_delivery_cost;
          // if vrs status is delivery confirmation pending then delivered value = return value,dp value,dp%,return %  = 0
          foreach ($total_trips as $tripid_value) {
            $check_vrs_status = check_trip_id_exists_or_not($dis_id, $tripid_value);
            if ($check_vrs_status->field_vrs_status_value == -1) {
              $flag = 1;
            }
            else {
              $flag = 0;
              break;
            }
          }
          // flag to check vrs status  = delivery confirmation pending
          // flag to check vrs status  = delivery confirmation pending
          if ($flag == 1) {
            $delivered_value_in_lak = 0;
            $return_value_in_lak = 0;
            $return_vs_delivered_percent = 0;
            $dp_value_in_lak = 0;
            $dp_percent = 0;
            $dp_value_percent = 0;
            $total_trip_vrs_delivered_cost = 0;
          }
          //echo $vehicle_per_day_cost;die;

          $mtd_result[$value['vehicle_number']]['vehicle_type'] = $value['field_vehicle_type_value'];
          $mtd_result[$value['vehicle_number']]['vehicle_owner'] = $value['field_vehicle_owner_value'];
          $mtd_result[$value['vehicle_number']]['cashiers'][$value['field_cashier_name_value']][$key] = array('dispatched_value' => $dispatched_value_in_lak, 'delivered_value' => $delivered_value_in_lak, 'return_value' => $return_value_in_lak, 'dp_value' => $dp_value_in_lak, 'dp_percent' => $dp_value_percent, 'total_cost' => $total_trip_delivery_cost, 'total_trip_created' => $total_trips, 'total_cost_delivered' => $total_trip_vrs_delivered_cost);
        }
        //Cashier specific data  
        //print_r($mtd_result);die;
        if (!empty($mtd_result)) {
          $sl_index = 0;
          foreach ($mtd_result as $vehicle_number => $vehicle) {
            $vh_no = $vehicle_number;
            $vehicle_total_cost = 0;
            $not_attempted_print = 0;
            $vehicle_total_delivered_value = 0;
            $vehicle_total_dispatched_value = 0;
            foreach ($vehicle['cashiers'] as $cashier => $trips) {
              // cashier specific trips details
              //total_trip_created
              $sl_index = $sl_index + 1;
              $dispatched_value_sum = $delivered_value = $return_value_sum = $dp_value_sum = $total_cost = $delivered_value_sum = 0;
              $return_percent_sale_vs_delivered = 0;
              $return_percent_sale_vs_dispatched = 0;
              $dp_value_percent = 0;
              $total_cost = 0;
              $average_cost_per_day = 0;
              $total_not_attempted_cost = 0;
              $not_attempted_vehicle_per_day_cost = 0;
              $not_attempted_count = 0;
              $cost_per_sale = 0;
              $avg_trip_count = 0;
              $trip_count = 0;
              $total_trip_status_deliverd = 0;
              foreach ($trips as $trip_key => &$trip_value) {
                $dispatched_value_sum += $trip_value['dispatched_value'];
                $delivered_value_sum += $trip_value['delivered_value'];
                $return_value_sum += $trip_value['return_value'];
                $dp_value_sum += $trip_value['dp_value'];
                $total_cost += $trip_value['total_cost'];
                $total_trip_status_deliverd += $trip_value['total_cost_delivered'];
                $trip_count += count($trip_value['total_trip_created']);
                $avg_trip_count++;
                $total_avg_trip_count++;
                $vh_total_avg_trip_count++;
                $vehicle_total_cost += $trip_value['total_cost'];
                $vehicle_total_delivered_value += $trip_value['delivered_value'];
                $vehicle_total_dispatched_value += $trip_value['dispatched_value'];
              }

              $average_delivered_value_per_trip = $delivered_value_sum / $trip_count;
              //$average_cost_per_trip = $total_cost / $trip_count;
              //$average_cost_per_trip = $total_cost ;
              $average_cost_per_trip = $total_cost / $avg_trip_count;
              $return_percent_sale_vs_delivered = $return_value_sum / $delivered_value_sum;
              $return_percent_sale_vs_dispatched = $return_value_sum / $dispatched_value_sum;
              //$average_cost_per_day = $total_cost / $trip_count;
              //$average_cost_per_day = $total_cost;
              $average_cost_per_day = $total_cost / $trip_count;
              $dispatched_value_sum = round($dispatched_value_sum, $round_off_no_digits);
              $delivered_value_sum = round($delivered_value_sum, $round_off_no_digits);
              //$cost_per_sale = ($total_cost / $delivered_value_sum) * 100;
              $cost_per_sale_dispatched = ($total_cost / $dispatched_value_sum) * 100;
              $cost_per_sale_delivered = ($total_cost / $delivered_value_sum) * 100;
              $sum_cost_percent += $cost_per_sale;
              $dp_value_percent = ($dp_value_sum / $dispatched_value_sum) * 100;
              $dp_value_percent = round($dp_value_percent);
              $return_value_sum = round($return_value_sum, $round_off_no_digits);
              $average_delivered_value_per_trip = round($average_delivered_value_per_trip, $round_off_no_digits);
              $average_cost_per_trip = round($average_cost_per_trip, $round_off_no_digits);
              $return_percent_sale_vs_delivered = round($return_percent_sale_vs_delivered, $round_off_no_digits) * 100;
              $return_percent_sale_vs_dispatched = round($return_percent_sale_vs_dispatched, $round_off_no_digits) * 100;
              $dp_value_sum = round($dp_value_sum, $round_off_no_digits);
              //$cost_per_sale = round($cost_per_sale, $round_off_no_digits_2);
              $cost_per_sale_delivered = round($cost_per_sale_delivered, $round_off_no_digits_2);
              $cost_per_sale_dispatched = round($cost_per_sale_dispatched, $round_off_no_digits_2);
              $total_cost_cashier = $total_cost;
              $total_cost_cashier = round($total_cost_cashier, $round_off_no_digits);
              $cost_per_sale = ($total_trip_status_deliverd / $delivered_value_sum) * 100;
              $cost_per_sale = round($cost_per_sale, $round_off_no_digits_2);
              $total_cost_delivery_confirmed_cost = round($total_trip_status_deliverd, $round_off_no_digits);
              $rows[] = array($vehicle_number_value, $vehicle_values['field_vehicle_owner_value'], $cashier, $trip_count, $dispatched_value_sum, $delivered_value_sum, $return_value_sum, $average_delivered_value_per_trip, $total_cost_cashier, $total_cost_delivery_confirmed_cost, $average_cost_per_trip, $return_percent_sale_vs_delivered, $return_percent_sale_vs_dispatched, $dp_value_sum, $dp_value_percent, $cost_per_sale_dispatched, $cost_per_sale);
              $sl_index = $sl_index + 1;
              // Total fields sum
              $total_dispatched_value_sum += $dispatched_value_sum;
              $total_delivered_value_sum += $delivered_value_sum;
              $total_return_value_sum += $return_value_sum;
              $total_dp_value_sum += $dp_value_sum;
              $total_total_cost += $total_cost;
              $total_trip_count += $trip_count;
              $total_delivred_cost += $total_cost_delivery_confirmed_cost;
              //$total_avg_trip_count += $avg_trip_count;
              // $total_cost_per_day += 
              // Vehicle specific total values
              $vh_total_dispatched_value_sum += $dispatched_value_sum;
              $vh_total_delivered_value_sum += $delivered_value_sum;
              $vh_total_return_value_sum += $return_value_sum;
              $vh_total_dp_value_sum += $dp_value_sum;
              $vh_total_total_cost += $total_cost;
              $vh_total_trip_count += $trip_count;
              $vh_total_delivred_cost += $total_cost_delivery_confirmed_cost;
            }
            $total_average_delivered_value_per_trip = $total_delivered_value_sum / $total_trip_count;
            //$total_average_cost_per_day = $total_total_cost/$total_avg_trip_count;
            $total_return_percent_delivered = round($total_return_value_sum / $total_delivered_value_sum * 100, $round_off_no_digits_2);
            $total_return_percent_dispatched = round($total_return_value_sum / $total_dispatched_value_sum * 100, $round_off_no_digits_2);
            // formula total_dp/total dispatched value
            $total_dp_percent = round($total_dp_value_sum / $total_dispatched_value_sum * 100, $round_off_no_digits_2);
          }
        }
      }
      $not_attempted_cost = 0;
      $not_attempted_count = 0;
      $vehilce_monthly_cost = 0;
      $vehicle_per_day_cost = 0;
      $vehilce_monthly_cost = 0;
      if (!empty($all_extra_working[$vehicle_number_value])) {
        $trip_not_created_on_non_working_days = array_diff($all_extra_working[$vehicle_number_value], $trip_created_days[$vehicle_number_value]);
        foreach ($trip_not_created_on_non_working_days as $value) {
          if (!empty($value)) {
            $date_array = explode('-', $value);
            $year = $date_array[0];
            $month = $date_array[1];
            $no_of_working_days = calculateWorkingDaysInMonth($year, $month, $weekoff);
            if (!empty($vehicle_cost_calc[$vehicle_number_value]['cost_per_month'])) {
              $vehilce_monthly_cost = $vehicle_cost_calc[$vehicle_number_value]['cost_per_month'];
              $vehicle_per_day_cost = $vehilce_monthly_cost / $no_of_working_days;
            }
            else if (!empty($vehicle_cost_calc[$vehicle_number_value]['cost_per_day'])) {
              $vehicle_per_day_cost = $vehicle_cost_calc[$vehicle_number_value]['cost_per_day'];
            }
            $not_attempted_count++;
            $not_attempted_cost += $not_attempted_cost;
          }
        }
      }
      //Checking working days trip not created
      $ts1 = strtotime($dispatch_from_date);
      $ts2 = strtotime($dispatch_to_date);
      $contract_end_period = strtotime($vehicle_values['end_period']);
      $contract_start_period = strtotime($vehicle_values['start_period']);
      $currentdate = $ts1;
      $year1 = date('Y', $ts1);
      $year2 = date('Y', $ts2);
      $month1 = date('m', $ts1);
      $month2 = date('m', $ts2);
      $diff = (($year2 - $year1) * 12) + ($month2 - $month1);
      while ($currentdate <= $ts2 && $currentdate <= $contract_end_period) {
        $day_of_month = date('Y-m-d', $currentdate);
        if (strtolower(date('l', $currentdate)) != strtolower($weekoff) && $contract_start_period <= $currentdate) {
          if (empty($absent_vehilce_details[$vehicle_number_value][$day_of_month]) && empty($trip_created_days[$vehicle_number_value][$day_of_month])) {
            $days[$vehicle_number_value][$day_of_month] = $day_of_month;
          }
        }
        $currentdate = strtotime('+1 day', $currentdate);
      }

      // print_r($trip_created_days);die;

      foreach ($days[$vehicle_number_value] as $datekey => $date_value) {
        if (!empty($date_value)) {
          $date_array = explode('-', $date_value);
          $year = $date_array[0];
          $month = $date_array[1];
          $no_of_working_days = calculateWorkingDaysInMonth($year, $month, $weekoff);
          if (!empty($vehicle_cost_calc[$vehicle_number_value]['cost_per_month'])) {
            $vehilce_monthly_cost = $vehicle_cost_calc[$vehicle_number_value]['cost_per_month'];
            $vehicle_per_day_cost = $vehilce_monthly_cost / $no_of_working_days;
          }
          else if (!empty($vehicle_cost_calc[$vehicle_number_value]['cost_per_day'])) {
            $vehicle_per_day_cost = $vehicle_cost_calc[$vehicle_number_value]['cost_per_day'];
          }
          //if (!empty($days[$vehicle_number_value])) {
          $not_attempted_count = $not_attempted_count + 1;
          //}
          $not_attempted_cost += $vehicle_per_day_cost;
        }
      }
      $not_attempted_cost = $not_attempted_cost / $lak;
      $average_not_attempted = $not_attempted_cost / $not_attempted_count;
      if (empty($total_avg_trip_count)) {
        $total_avg_trip_count = 1;
      }
      // Trip created Cost + Trip Not created Cost
      $total_average_cost_per_day = ($total_total_cost + $not_attempted_cost) / $total_avg_trip_count;
      // Adding Trip Not created cost also
      $vh_total_total_cost += $not_attempted_cost;
      $total_total_cost += $not_attempted_cost;
      $total_cost_percent_dispatched = round($total_total_cost / $total_dispatched_value_sum * 100, $round_off_no_digits_2);
      //$total_cost_percent_delivered = round($total_total_cost / $total_delivered_value_sum * 100, $round_off_no_digits_2);
      $total_cost_percent_delivered = round($total_delivred_cost / $total_delivered_value_sum * 100, $round_off_no_digits_2);
      $total_avg_cost_trip = round($total_total_cost / $total_avg_trip_count, $round_off_no_digits_2);
      $total_delivred_cost = round($total_delivred_cost, $round_off_no_digits);
      $rows[] = array($vehicle_number_value, $vehicle_values['field_vehicle_owner_value'], 'Trip Not Created', $not_attempted_count, '', '', '', '', round($not_attempted_cost, $round_off_no_digits), '', round($average_not_attempted, $round_off_no_digits), '', '', '', '', '', '');
      $cost_per_sale = (($vehicle_total_cost + $not_attempted_cost) / $vehicle_total_delivered_value) * 100;
      $cost_per_sale_dispatched = (($vehicle_total_cost + $not_attempted_cost) / $vehicle_total_dispatched_value) * 100;
      $rows[] = array($vehicle_number_value, '', 'Sub Total', $total_trip_count, $total_dispatched_value_sum, $total_delivered_value_sum, $total_return_value_sum, round($total_average_delivered_value_per_trip, $round_off_no_digits), round($total_total_cost, $round_off_no_digits), $total_delivred_cost, round($total_average_cost_per_day, $round_off_no_digits), $total_return_percent_delivered, $total_return_percent_dispatched, $total_dp_value_sum, $total_dp_percent, $total_cost_percent_dispatched, $total_cost_percent_delivered);
    }
    $resources_not_utilized_cost = get_not_utilized_resources_cost($dispatch_from_date, $dispatch_to_date, $lak, $trip_created_days_resources);
    $total_total_cost += $resources_not_utilized_cost;
    $vh_total_total_cost += $resources_not_utilized_cost;
    $resources_not_utilized_cost = round($resources_not_utilized_cost, $round_off_no_digits);
    $rows[] = array('', '', 'Resources not utilized cost', '', '', '', '', '', $resources_not_utilized_cost, '', '', '', '', '', '', '');
    //formula total_return_value/total_delivered_value
    $vh_return_percent_sale_vs_delivered = round($vh_total_return_value_sum / $vh_total_delivered_value_sum * 100, $round_off_no_digits_2);
    //formula total_return_value/total_dispatched_value
    $vh_return_percent_sale_vs_dispatched = round($vh_total_return_value_sum / $vh_total_dispatched_value_sum * 100, $round_off_no_digits_2);
    // formula all vehicle stotal_dp_value/vh_total_dispatched_value
    $vh_total_dp_percent = round($vh_total_dp_value_sum / $vh_total_dispatched_value_sum * 100, $round_off_no_digits_2);
    // formula all vehicles totalcost/all vehicles dispatched value *100
    $vh_total_cost_percent_dispatched = round($vh_total_total_cost / $vh_total_dispatched_value_sum * 100, $round_off_no_digits_2);
    // formula all vehicles totalcost/all vehicles delivered value *100
    //$vh_total_cost_percent_delivered = round($vh_total_total_cost / $vh_total_delivered_value_sum * 100, $round_off_no_digits_2);
    $vh_total_cost_percent_delivered = round($vh_total_delivred_cost / $vh_total_delivered_value_sum * 100, $round_off_no_digits_2);
    $vh_total_dispatched_value_sum = round($vh_total_dispatched_value_sum, $round_off_no_digits_2);
    $vh_total_delivered_value_sum = round($vh_total_delivered_value_sum, $round_off_no_digits_2);
    $vh_total_return_value_sum = round($vh_total_return_value_sum, $round_off_no_digits_2);
    $vh_total_dp_value_sum = round($vh_total_dp_value_sum, $round_off_no_digits_2);
    $vh_total_total_cost = round($vh_total_total_cost, $round_off_no_digits_2);
    $vh_total_delivred_cost = round($vh_total_delivred_cost, $round_off_no_digits_2);
    $vh_average_cost_per_trip = round($vh_total_total_cost / $vh_total_avg_trip_count, $round_off_no_digits_2);
    $rows[] = array('', '', 'Total', $vh_total_trip_count, $vh_total_dispatched_value_sum, $vh_total_delivered_value_sum, $vh_total_return_value_sum, $vh_average_cost_per_trip, $vh_total_total_cost, $vh_total_delivred_cost, $vh_average_cost_per_trip, $vh_return_percent_sale_vs_delivered, $vh_return_percent_sale_vs_dispatched, $vh_total_dp_value_sum, $vh_total_dp_percent, $vh_total_cost_percent_dispatched, $vh_total_cost_percent_delivered);
  }
  if ($export == 1) {
    return $rows;
  }
  $form_state['delivery_cost_details'] = $rows;
  $form_state['rebuild'] = TRUE;
}

//For finding resources not utilized cost
function get_not_utilized_resources_cost($dispatch_from_date, $dispatch_to_date, $lak, $trip_created_days) {
  $all_resources = get_all_active_resources_on_date_range(strtotime($dispatch_from_date), strtotime($dispatch_to_date));
  $absent_resource_details = get_leave_dates_of_resources(strtotime($dispatch_from_date), strtotime($dispatch_to_date));
  $resource_all_extra_working = get_resource_extra_working_days(strtotime($dispatch_from_date), strtotime($dispatch_to_date));
  $weekoff = get_resource_weekly_off_day();
  $resources_cost = $resources_total_cost = 0;
  $all_resources_trip_pending = array();
  if (!empty($all_resources)) {
    foreach ($all_resources as $resource_name => $resource_values) {
      if (!empty($resource_values['field_weekly_off_value'])) {
        $weekoff = $resource_values['field_weekly_off_value'];
      }
      if (!empty($resource_all_extra_working[$resource_name])) {
        $trip_not_created_on_non_working_days = array_diff($resource_all_extra_working[$resource_name], $trip_created_days[$resource_name]);
        foreach ($trip_not_created_on_non_working_days as $value) {
          if (!empty($value)) {
            $date_array = explode('-', $value);
            $all_resources_trip_pending[$resource_name][$value] = $value;
            $year = $date_array[0];
            $month = $date_array[1];
            $resource_cost = 0;
            $resource_no_of_working_days = calculateWorkingDaysInMonth($year, $month);
            if (!empty($resource_values[$resource_name]['cost_per_month'])) {
              $resource_cost = $resource_values[$resource_name]['cost_per_month'] / $resource_no_of_working_days;
            }
            $resources_total_cost += $resource_cost;
          }
        }
      }
      //Checking working days trip not created
      $ts1 = strtotime($dispatch_from_date);
      $ts2 = strtotime($dispatch_to_date);
      $contract_end_period = strtotime($resource_values['end_period']);
      $contract_start_period = strtotime($resource_values['start_period']);
      $currentdate = $ts1;
      $year1 = date('Y', $ts1);
      $year2 = date('Y', $ts2);
      $month1 = date('m', $ts1);
      $month2 = date('m', $ts2);
      while ($currentdate <= $ts2 && $currentdate <= $contract_end_period) {
        $day_of_month = date('Y-m-d', $currentdate);
        if (strtolower(date('l', $currentdate)) != strtolower($weekoff) && $contract_start_period <= $currentdate) {
          if (empty($absent_resource_details[$resource_name][$day_of_month]) && empty($trip_created_days[$resource_name][$day_of_month])) {
            $days[$resource_name][$day_of_month] = $day_of_month;
          }
        }
        $currentdate = strtotime('+1 day', $currentdate);
      }
      foreach ($days[$resource_name] as $datekey => $date_value) {
        if (!empty($date_value)) {
          $date_array = explode('-', $date_value);
          $year = $date_array[0];
          $month = $date_array[1];
          $resource_no_of_working_days = calculateWorkingDaysInMonth($year, $month);
          $resource_cost = 0;
          $all_resources_trip_pending[$resource_name][$value] = $value;
          if (!empty($resource_values['cost_per_month'])) {
            $resource_cost = $resource_values['cost_per_month'] / $resource_no_of_working_days;
          }
          $resources_total_cost += $resource_cost;
        }
      }
    }
    if (!empty($resources_total_cost)) {
      $resources_total_cost = $resources_total_cost / $lak;
    }
  }

  return $resources_total_cost;
}

//get deliveryboy names using trip_id
function get_deliveryboys($trip_ids) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);
  if (!empty($trip_ids)) {
    $query = db_select('node', 'disp_data');
    $query->condition('disp_data.type', 'dispatch_data');
    $query->fields('disp_data', array('title'));
    $query->condition('disp_data.uid', $dis_id);
    $query->condition('disp_data.title', $trip_ids, 'IN');
    $query->leftjoin('field_data_field_delivery_boy', 'delivery_boys', 'delivery_boys.entity_id = disp_data.nid');
    $query->leftjoin('field_data_field_resource_name', 'resource_names', 'resource_names.entity_id = delivery_boys.field_delivery_boy_nid');
    $query->addExpression('GROUP_CONCAT(DISTINCT(resource_names.field_resource_name_value))', 'delivery_boys');
    $res = $query->execute()->fetchAll(PDO::FETCH_UNIQUE | PDO::FETCH_ASSOC);
  }
  return $res;
}

function get_extra_working_vehicles($dispatch_date) {
  $result = array();
  if (!empty($dispatch_date)) {
    global $user;
    $type = 'vehicle_extra_working';
    $distributor_id = change_feeds_import_get_distributor_id($user->uid);
    $query = db_select('node', 'n');
    $query->condition('n.type', $type);
    $query->condition('n.uid', $distributor_id);
    $query->join('field_data_field_vehicle_number_ref', 'vehicle_ref', 'vehicle_ref.entity_id = n.nid');
    $query->join('field_data_field_start_period', 'start_period', 'start_period.entity_id = n.nid');
    $query->condition('start_period.field_start_period_value', $dispatch_date, '=');
    $query->join('node', 'vehicle_master', 'vehicle_master.nid = vehicle_ref.field_vehicle_number_ref_nid');
    $query->condition('vehicle_master.type', 'vehicle_master');
    $query->fields('vehicle_master', array('title'));
    $result = $query->execute()->fetchAllkeyed(0, 0);
  }
  return $result;
}

function get_delivery_cost_details($form, &$form_state, $export = 0) {
  global $user;
  $rows = array();
  $trip_ids = array();
  $dis_id = change_feeds_import_get_distributor_id($user->uid);
  $trip_id = $form_state['values']['trip_id'];
  $dispatch_date = $form_state['values']['dispatch_date'];
  $all_resources = get_all_active_resources_on_date_range(strtotime($dispatch_date), strtotime($dispatch_date));
  //print_r($all_resources);die;
  $dipacth_date_filter = 0;
  $round_off_no_digits = 3;
  $round_off_no_digits_2 = 2;
  //get_trip_not_attempted_vehicles
  if (!empty($dispatch_date)) {
    $trip_ids = _get_trip_ids_using_dispatch_date($dispatch_date);
    $dis_date = $dispatch_date;
    $dipacth_date_filter = 1;
  }
  else if (!empty($trip_id)) {
    $trip_ids[$trip_id] = $trip_id;
    $dis_date = _get_dispatch_date_using_trip_id($trip_id);
  }
  $dis_date = strtotime($dis_date);
  $weekoff = get_resource_weekly_off_day();
  $year = date('Y', $dis_date);
  $month = date('m', $dis_date);
  $all_vehicles = get_all_active_vehicles_on_date_range($dis_date, $dis_date);
  $absent_vehilce_details = get_maintenance_dates_of_vehicle($dis_date, $dis_date);
  $absent_resource_details = get_leave_dates_of_resources($dis_date, $dis_date);
  $resource_all_extra_working = get_resource_extra_working_days($dis_date, $dis_date);
  // delete absent resources from that list
  if (!empty($absent_resource_details)) {
    foreach ($absent_resource_details as $key => $absent_resource_data) {
      unset($all_resources[$key]);
    }
  }
  // remove resource weekly off day from resource list. If extra working defined on week off then no need to exclude week off
  if (!empty($all_resources)) {
    foreach ($all_resources as $resource_key => &$resource_value) {
      if (empty($resource_all_extra_working[$resource_value])) {
        if (!empty($resource_value['field_weekly_off_value'])) {
          $res_selected_week_off = $resource_value['field_weekly_off_value'];
        }
        else {
          $res_selected_week_off = $weekoff;
        }
        // - Resource weekly off day and checking no extra working day defined
        if (strtolower(date('l', $dis_date)) == strtolower($res_selected_week_off) && empty($resource_all_extra_working[$resource_key][date('Y-m-d', $dis_date)])) {
          unset($all_resources[$resource_key]);
        }
        // - Resource  leave on particaular day
        if (!empty($absent_resource_details[$resource_key][date('Y-m-d', $dis_date)])) {
          unset($all_resources[$resource_key]);
        }
      }
    }
  }
  $all_extra_working = get_extra_working_days($dis_date, $dis_date);
  //print_r($resource_all_extra_working);die;
  $get_active_vehicles_on_dispatch_date = get_all_active_vehicles($dis_date);
  $resource_no_of_working_days = calculateWorkingDaysInMonth($year, $month);
  $lak = 100000;
  $active_vehicle_numbers = array_keys($all_vehicles);
  $trip_cashiers = array();
  $trip_delivery_boys = array();
  if (!empty($trip_ids)) {
    $query = db_select('dispatch_planning_api_data', 'ad');
    $query->condition('ad.distributor_id', $dis_id, '=');
    $query->condition('ad.runsheet_number', $trip_ids, 'IN');
    $query->join('node', 'n', 'n.title = ad.bill_number AND n.uid = ad.distributor_id');
    $query->condition('n.type', 'sales_register_data', '=');
    $query->condition('n.uid', $dis_id);
    if (!empty($active_vehicle_numbers)) {
      $query->condition('vehicle_no.field_vehicle_no_value', $active_vehicle_numbers, 'IN');
    }
    $query->join('dispatch_planning_item_details', 'item', 'item.trip_bill_reference = ad.id');
    $query->addExpression("SUM(item.return_qty * item.sum_net_sales_value)", 'return_value');
    $query->addExpression("SUM(item.res_sku_actual_quantity1 * item.sum_net_sales_value)", 'delivered_value');
    $query->addExpression("SUM(item.sum_net_sales_quantity * item.sum_net_sales_value)", 'dispatched_value');
    // Join with dispatch_data to check vrs status and return flage 
    $query->join('node', 'disp_data', 'disp_data.title = ad.runsheet_number');
    $query->condition('disp_data.type', 'dispatch_data');
    //$query->leftjoin('field_data_field_delivery_boy', 'delivery_boys', 'delivery_boys.entity_id = disp_data.nid');
    //$query->leftjoin('field_data_field_resource_name', 'resource_names', 'resource_names.entity_id = delivery_boys.field_delivery_boy_nid');
    $query->condition('disp_data .uid', $dis_id);
    //$query->leftjoin('field_data_field_vrs_status', 'vs', 'vs.entity_id = disp_data.nid');
    $query->leftjoin('field_data_field_vehicle_no', 'vehicle_no', 'vehicle_no.entity_id = disp_data.nid');
    // VRS Screen Return flag = 1 and VRS status = 1, 3 Means VRS pending
    $query->leftjoin('field_data_field_cashier_name', 'fld_cashier', 'fld_cashier.entity_id = disp_data.nid');
    // $vrs_status = array('1', '2', '3', '4', '5','-1','0');
    //$query->condition('vs.field_vrs_status_value', $vrs_status, 'IN');
    // Join with vehicle master
    $query->leftJoin('node', 'vehicle', 'vehicle.title = ad.vehicle');
    $query->condition('vehicle.type', 'vehicle_master');
    $query->leftjoin('field_data_field_vehicle_owner', 'vehicle_owner', 'vehicle_owner.entity_id = vehicle.nid');
    $query->leftjoin('field_data_field_vehicle_type', 'vehicle_type', 'vehicle_type.entity_id = vehicle.nid');
    //$query->leftjoin('field_data_field_net_sales_bill_value', 'dispatched_value', 'dispatched_value.entity_id = disp_data.nid');
    $query->condition('vehicle.uid', $dis_id);
    //$query->groupBy('ad.runsheet_number');
    if ($dipacth_date_filter == 1) {
      $query->groupBy('vehicle_no.field_vehicle_no_value');
    }
    else {
      $query->groupBy('ad.runsheet_number');
    }
    $query->addExpression("GROUP_CONCAT(DISTINCT ad.runsheet_number  separator ', ' )", 'trip_ids');
    $query->addExpression("GROUP_CONCAT(DISTINCT ad.runsheet_number)", 'tripids_cal');
    $query->addExpression("GROUP_CONCAT(DISTINCT fld_cashier.field_cashier_name_value  separator ', ' )", 'cashiernames');
    $query->fields('ad', array('runsheet_number'));
    $query->fields('fld_cashier', array('field_cashier_name_value'));
    $query->fields('vehicle_owner', array('field_vehicle_owner_value'));
    $query->fields('vehicle_type', array('field_vehicle_type_value'));
    $query->addExpression('vehicle_no.field_vehicle_no_value', 'vehicle_number');
    $query->addExpression('ad.date', 'dispacth_date');
    $res = $query->execute()->fetchAll(PDO::FETCH_UNIQUE | PDO::FETCH_ASSOC);
    $counter = 1;
    $active_vehicles = array_keys($get_active_vehicles_on_dispatch_date);
    $trip_ids = array_keys($res);
    $public_holidays = get_public_holiday($month, $year);
    $total_dispatched_value = 0;
    $total_return_value = 0;
    $total_return_value = 0;
    $total_dp_value = 0;
    $total_return_percent = 0;
    $total_dp_percent = 0;
    $total_delivered_value = 0;
    $trip_titles = array_keys($res);
    $no_of_working_days = calculateWorkingDaysInMonth($year, $month, $weekoff);
    // Not considering weekly off only calendar days for resources
    $total_cost_for_theday = 0;
    // variable that save only Trip status delivery confirmed cost
    $total_cost_of_trip_delivred = 0;
    foreach ($res as $key => $value) {
      //$all_vehicles
      $trip_attempted_vehicles[$value['vehicle_number']] = $value['vehicle_number'];
    }

    $vehicles_not_attempted_trips = array_diff($active_vehicles, $trip_attempted_vehicles);
    if (!empty($res)) {
      // Fetch extra working day vehicles get vehicle not utilized. excluding weekly off day
      if (strtolower(date('l', $dis_date)) == strtolower($weekoff) || in_array($value['dispacth_date'], $public_holidays)) {
        $extra_working_vehicles = get_extra_working_vehicles($dis_date);
        $attempted_vehicles = get_trip_attempted_vehicle(date('Y-m-d', $dis_date));
        $vehicles_not_utilized_on_working_days = array_diff($extra_working_vehicles, $attempted_vehicles);
        $vh_not_utilized_non_working_days = array();
        if (!empty($vehicles_not_utilized_on_working_days)) {
          foreach ($vehicles_not_utilized_on_working_days as $vehicleno => $value) {
            $vehicle_per_day_cost = 0;
            $vehilce_monthly_cost = 0;
            $vehicle_cost = get_vehilce_per_day_cost($value);
            if (!empty($vehicle_cost[$value]['cost_per_month'])) {
              $vehilce_monthly_cost = $vehicle_cost[$value]['cost_per_month'];
              $vehicle_per_day_cost = $vehilce_monthly_cost / $no_of_working_days;
            }
            else if (!empty($vehicle_cost[$value]['cost_per_day'])) {
              $vehicle_per_day_cost = $vehicle_cost[$value]['cost_per_day'];
            }
            $vh_not_utilized_non_working_days[$vehicleno] = $vehicle_per_day_cost;
          }
        }
      }
      // print_r($vh_not_utilized_non_working_days);die;
      foreach ($res as $key => $value) { //print_r($value);
        $delivery_boy_cost = 0; // Multiple delivery boy will be in one trip
        $total_trip_delivery_cost = 0;
        $dispatched_value_in_lak = round($value['dispatched_value'] / $lak, $round_off_no_digits);
        $dp_trip_ids = explode(',', $value['tripids_cal']);
        // ignore dp bills
        $return_value_trip_id = get_return_value($dp_trip_ids);
        if (!empty($return_value_trip_id)) {
          $return_value_in_lak = round($return_value_trip_id / $lak, $round_off_no_digits);
        }
        
        $vehicle_per_day_cost = 0;
        $vehilce_monthly_cost = 0;
        $cashier_cost = 0;
        $delivered_value_in_lak = round($value['delivered_value'] / $lak, $round_off_no_digits);
        $return_percent = ($dispatched_value_in_lak / $return_value_in_lak) * 100;
        $return_vs_delivered_percent = ($return_value_in_lak / $delivered_value_in_lak) * 100;
        $return_vs_dispatched_percent = ($return_value_in_lak / $dispatched_value_in_lak) * 100;
        if ($dipacth_date_filter == 1) {
          $dp_value = get_dp_value_vehicle_number($dp_trip_ids, $value['vehicle_number']);
        }
        else {
          $dp_value = get_dp_value($dp_trip_ids);
        }
        $dp_value_in_lak = 0;
        $dp_percent = 0;
        if (!empty($dp_value)) {
          $dp_value_in_lak = round($dp_value / $lak, $round_off_no_digits);
          $dp_percent = ($dp_value_in_lak / $dispatched_value_in_lak) * 100;
        }

        $vehicle_cost = get_vehilce_per_day_cost($value['vehicle_number']);
        // If the vehicle is contracted then no of working days will change based o pdd & weekoff
        if (!empty($vehicle_cost[$value['vehicle_number']]['cost_per_month'])) {
          $vehilce_monthly_cost = $vehicle_cost[$value['vehicle_number']]['cost_per_month'];
          $vehicle_per_day_cost = $vehilce_monthly_cost / $no_of_working_days;
        }
        else if (!empty($vehicle_cost[$value['vehicle_number']]['cost_per_day'])) {
          $vehicle_per_day_cost = $vehicle_cost[$value['vehicle_number']]['cost_per_day'];
        }
        $input_resource_type = 'Cashier';
        $cashier_name = $value['field_cashier_name_value'];
        if ($all_resources[$cashier_name]) {
          $trip_cashiers[$cashier_name] = $cashier_name;
          unset($all_resources[$cashier_name]);
        }

        //$cashier_monthly_cost = get_resource_cost_per_day($cashier_name, $input_resource_type);

        $cashier_data = get_resource_cost_per_day($cashier_name, $input_resource_type);
        if (!empty($cashier_data[$cashier_name]['cost_per_month'])) {
          $cashier_cost = $cashier_data[$cashier_name]['cost_per_month'] / $resource_no_of_working_days;
        }
        if ($resource_all_extra_working[$cashier_name]) {
          unset($all_resources[$cashier_name]);
        }
        // get delivery boy associated to trip
        $delivery_boy_trip = get_deliveryboys(array($key));
        if (!empty($delivery_boy_trip[$key]['delivery_boys'])) {
          $delivery_boys = array_unique(explode(',', $delivery_boy_trip[$key]['delivery_boys']));
          if (!empty($delivery_boys)) {
            foreach ($delivery_boys as $delivery_boy_value) {
              if ($all_resources[$delivery_boy_value]) {
                $trip_delivery_boys[$delivery_boy_value] = $delivery_boy_value;
                unset($all_resources[$delivery_boy_value]);
              }
              if ($resource_all_extra_working[$delivery_boy_value]) {
                unset($all_resources[$delivery_boy_value]);
              }
              $input_resource_type = 'Delivery Boy';
              $delivery_boy_name = $delivery_boy_value;
              //$delivery_boy_cost += get_resource_cost_per_day($delivery_boy_name, $input_resource_type);
              $deliveryboy_data = get_resource_cost_per_day($delivery_boy_name, $input_resource_type);
              if (!empty($deliveryboy_data[$delivery_boy_name]['cost_per_month'])) {

                $delivery_boy_cost += $deliveryboy_data[$delivery_boy_name]['cost_per_month'] / $resource_no_of_working_days;
              }
            }
          }
        }
        $total_trip_delivery_cost = $delivery_boy_cost + $cashier_cost + $vehicle_per_day_cost;
        $trip_delivery_confirmed_delivery_cost = $total_trip_delivery_cost;
        // Values will be calculated when trip status greater than -1 delivery confirmation pending
        if (!empty($value['tripids_cal'])) {
          $trip_ids_values = explode(',', $value['tripids_cal']);
          foreach ($trip_ids_values as $tripid_value) {
            $check_vrs_status = check_trip_id_exists_or_not($dis_id, $tripid_value);
            if ($check_vrs_status->field_vrs_status_value == -1) {
              $flag = 1;
            }
            else {
              $flag = 0;
              break;
            }
          }
          // flag to check vrs status  = delivery confirmation pending
          if ($flag == 1) {
            $delivered_value_in_lak = 0;
            $return_value_in_lak = 0;
            $return_vs_delivered_percent = 0;
            $return_vs_dispatched_percent = 0;
            $dp_value_in_lak = 0;
            $dp_percent = 0;
            $trip_delivery_confirmed_delivery_cost = 0;
          }
        }

        $total_dispatched_value += $dispatched_value_in_lak;
        $total_delivered_value += $delivered_value_in_lak;
        $total_return_value += $return_value_in_lak;
        $total_dp_value += $dp_value_in_lak;
        $total_return_percent += $return_percent;
        $total_return_vs_delivered_percent += $return_vs_delivered_percent;
        $total_return_vs_dispatched_percent += $return_vs_dispatched_percent;
        $total_dp_percent += $dp_percent;
        $total_cost_for_theday += $total_trip_delivery_cost / $lak;
        $total_cost_of_trip_delivred += $trip_delivery_confirmed_delivery_cost / $lak;
        $return_value_in_lak = round($return_value_in_lak, $round_off_no_digits);
        $return_vs_delivered_percent = round($return_vs_delivered_percent);
        $return_vs_dispatched_percent = round($return_vs_dispatched_percent);
        $dp_value_in_lak = round($dp_value_in_lak, $round_off_no_digits);
        $dp_percent = round($dp_percent);
        $dispatched_value_in_lak = round($dispatched_value_in_lak, $round_off_no_digits);
        $delivered_value_in_lak = round($delivered_value_in_lak, $round_off_no_digits);
        $total_trip_delivered_cost_in_lak = round($total_trip_delivery_cost / $lak, $round_off_no_digits);
        $cost_vs_delivered_cost = ($total_trip_delivered_cost_in_lak) / $delivered_value_in_lak * 100;
        $cost_vs_dispatched = ($total_trip_delivered_cost_in_lak) / $dispatched_value_in_lak * 100;
        $total_cost_vs_delivered += $cost_vs_delivered_cost;
        $total_cost_vs_dispatched += $cost_vs_dispatched;
        $total_trip_delivery_cost = round($total_trip_delivered_cost_in_lak, $round_off_no_digits);
        $rows[] = array($counter, $value['vehicle_number'], $value['trip_ids'], $value['field_vehicle_type_value'], $value['field_vehicle_owner_value'], $value['cashiernames'], $dispatched_value_in_lak, $delivered_value_in_lak, $return_value_in_lak, $return_vs_delivered_percent, $return_vs_dispatched_percent, $dp_value_in_lak, $dp_percent, $total_trip_delivery_cost, round($trip_delivery_confirmed_delivery_cost / $lak, $round_off_no_digits), round($cost_vs_dispatched, $round_off_no_digits_2), round($cost_vs_delivered_cost, $round_off_no_digits_2));
        $counter++;
      }
      if (strtolower(date('l', $dis_date)) != strtolower($weekoff)) {
        if (!empty($vehicles_not_attempted_trips) && $dipacth_date_filter == 1) {
          foreach ($vehicles_not_attempted_trips as $key => $value) {
            $vehilce_monthly_cost = 0;
            $vehicle_per_day_cost = 0;
            $vehicle_cost = get_vehilce_per_day_cost($value);
            if (!empty($vehicle_cost[$value]['cost_per_month'])) {
              $vehilce_monthly_cost = $vehicle_cost[$value]['cost_per_month'];
              $vehicle_per_day_cost = $vehilce_monthly_cost / $no_of_working_days;
            }
            else if (!empty($vehicle_cost[$value]['cost_per_day'])) {
              $vehicle_per_day_cost = $vehicle_cost[$value]['cost_per_day'];
            }
            $total_trip_delivery_cost += $vehicle_per_day_cost;
            $total_cost_for_theday += $vehicle_per_day_cost / $lak;
            $trip_not_created_reason = get_trip_not_created_reason(date('Y-m-d', $dis_date), $vehicle_number = $value, $resource_name = null, $resource_nid = null);
            if (empty($trip_not_created_reason)) {
              $trip_not_created_reason = t('Trip Not Created');
            }
            $rows[] = array($counter, $value, $trip_not_created_reason, $get_active_vehicles_on_dispatch_date[$value]['field_vehicle_type_value'], $get_active_vehicles_on_dispatch_date[$value]['field_vehicle_owner_value'], '', '', '', '', '', '', '', '', round($vehicle_per_day_cost / $lak, $round_off_no_digits_2), '', '', '');
            $counter++;
          }
        }
      }

      if ($dipacth_date_filter == 1 && !empty($vh_not_utilized_non_working_days)) {

        foreach ($vh_not_utilized_non_working_days as $key => $value) {
          $trip_not_created_reason = get_trip_not_created_reason(date('Y-m-d', $dis_date), $vehicle_number = $key, $resource_name = null, $resource_nid = null);
          if (empty($trip_not_created_reason)) {
            $trip_not_created_reason = t('Trip Not Created');
          }
          $total_trip_delivery_cost += (float) $value;
          $total_cost_for_theday += (float) $value / $lak;
          $rows[] = array($counter, $key, $trip_not_created_reason, $get_active_vehicles_on_dispatch_date[$key]['field_vehicle_type_value'], $get_active_vehicles_on_dispatch_date[$key]['field_vehicle_owner_value'], '', '', '', '', '', '', '', '', round($value / $lak, $round_off_no_digits_2), '', '', '');
          $counter++;
        }
      }
      // For absent vehicles
      if ($dipacth_date_filter == 1) {
        $absent_vehilce_details = get_maintenance_dates_of_vehicle($dis_date, $dis_date);
        foreach ($absent_vehilce_details as $ab_vehicle_number => $ab_vehiclevalue) {
          $rows[] = array($counter, $ab_vehicle_number, 'On Leave', $get_active_vehicles_on_dispatch_date[$ab_vehicle_number]['field_vehicle_type_value'], $get_active_vehicles_on_dispatch_date[$ab_vehicle_number]['field_vehicle_owner_value'], '', '', '', '', '', '', '', '', 0, '', '', '');
          $counter++;
        }
      }
      $resourc_not_utilized_cost = 0;
      if (!empty($rows)) {

        if (!empty($all_resources) && $dipacth_date_filter == 1) {
          foreach ($all_resources as $trip_cashier_name => $value) {
            $input_resource_type = Ucfirst(strtolower($value['resource_type']));
            $cashier_data = get_resource_cost_per_day($trip_cashier_name, $input_resource_type);
            if (!empty($cashier_data[$trip_cashier_name]['cost_per_month'])) {
              $resource_not_utilized_cost += ($cashier_data[$trip_cashier_name]['cost_per_month'] / $resource_no_of_working_days);
            }
          }

          $resource_not_utilized_cost = $resource_not_utilized_cost / $lak;
          $resource_not_utilized_cost = round($resource_not_utilized_cost, $round_off_no_digits);
          $total_cost_for_theday += $resource_not_utilized_cost;
        }

        $total_dispatched_value = round($total_dispatched_value, $round_off_no_digits);
        $total_return_value = round($total_return_value, $round_off_no_digits);
        $total_return_vs_delivered_percent = round($total_return_vs_delivered_percent, $round_off_no_digits);
        $total_return_vs_dispatched_percent = round($total_return_vs_dispatched_percent, $round_off_no_digits);
        $total_dp_value = round($total_dp_value, $round_off_no_digits);
        $total_dp_percent = round($total_dp_percent, $round_off_no_digits);
        $total_delivered_value = round($total_delivered_value, $round_off_no_digits);
        $total_cost_for_theday = round($total_cost_for_theday, $round_off_no_digits_2);

        //$total_cost_vs_delivered = round(($total_cost_for_theday / $total_delivered_value) * 100, $round_off_no_digits_2);
        $total_cost_vs_dispatched = round(($total_cost_for_theday / $total_dispatched_value) * 100, $round_off_no_digits_2);
        $total_return_vs_delivered_percent = round(($total_return_value / $total_delivered_value) * 100);
        $total_return_vs_dispatched_percent = round(($total_return_value / $total_dispatched_value) * 100);
        $total_dp_percent = round(($total_dp_value / $total_dispatched_value) * 100);
        // replaced with only trip status is delivery confirmed
        $total_cost_vs_delivered = round(($total_cost_of_trip_delivred / $total_delivered_value) * 100, $round_off_no_digits_2);
        if ($dipacth_date_filter == 1) {
          $rows[] = array($counter, t('Resources not utilized'), '', '', '', '', '', '', '', '', '', '', '', $resource_not_utilized_cost, '', '', '');
        }
        $rows[] = array('', t('Total'), '', '', '', '', $total_dispatched_value, $total_delivered_value, $total_return_value, $total_return_vs_delivered_percent, $total_return_vs_dispatched_percent, $total_dp_value, $total_dp_percent, $total_cost_for_theday, round($total_cost_of_trip_delivred, $round_off_no_digits_2), $total_cost_vs_dispatched, $total_cost_vs_delivered);
      }
    }
  }
  // No trip created for that date then display all active vehicle with dispatch date.
  if ($dipacth_date_filter == 1 && empty($res)) {
    $counter = 1;
    $not_attempted_cost = 0;
    if (!empty($all_vehicles)) {
      foreach ($all_vehicles as $vehicle_number_value => $vehicle_values) {

        $date_array = explode('-', $dispatch_date);
        $currentdate = strtotime($dispatch_date);
        $year = $date_array[0];
        $month = $date_array[1];
        $no_of_working_days = calculateWorkingDaysInMonth($year, $month, $weekoff);
        $vehicle_cost_calc = get_vehilce_per_day_cost($vehicle_number_value);
        if (!empty($vehicle_cost_calc[$vehicle_number_value]['cost_per_month'])) {
          $vehilce_monthly_cost = $vehicle_cost_calc[$vehicle_number_value]['cost_per_month'];
          $vehicle_per_day_cost = $vehilce_monthly_cost / $no_of_working_days;
        }
        else if (!empty($vehicle_cost_calc[$vehicle_number_value]['cost_per_day'])) {
          $vehicle_per_day_cost = $vehicle_cost_calc[$vehicle_number_value]['cost_per_day'];
        }
        if (!empty($all_extra_working[$vehicle_number_value])) {
          $not_attempted_count++;
          $not_attempted_cost += $vehicle_per_day_cost;
          $trip_not_created_reason = get_trip_not_created_reason(date('Y-m-d', $dis_date), $vehicle_number = $vehicle_number_value, $resource_name = null, $resource_nid = null);
          if (empty($trip_not_created_reason)) {
            $trip_not_created_reason = t('Trip Not Created');
          }
          $rows[] = array($counter, $vehicle_number_value, $trip_not_created_reason, $vehicle_values['field_vehicle_type_value'], $vehicle_values['field_vehicle_owner_value'], '', '', '', '', '', '', '', '', round($vehicle_per_day_cost / $lak, $round_off_no_digits_2), '', '', '');
          $counter++;
        }

        if (strtolower(date('l', $currentdate)) != strtolower($weekoff)) {
          if (empty($absent_vehilce_details[$vehicle_number_value][$dispatch_date])) {
            $not_attempted_cost += $vehicle_per_day_cost;
            $trip_not_created_reason = get_trip_not_created_reason(date('Y-m-d', $dis_date), $vehicle_number = $vehicle_number_value, $resource_name = null, $resource_nid = null);
            if (empty($trip_not_created_reason)) {
              $trip_not_created_reason = t('Trip Not Created');
            }
            $rows[] = array($counter, $vehicle_number_value, $trip_not_created_reason, $vehicle_values['field_vehicle_type_value'], $vehicle_values['field_vehicle_owner_value'], '', '', '', '', '', '', '', '', round($vehicle_per_day_cost / $lak, $round_off_no_digits_2), '', '', '');
          }
          else {
            $rows[] = array($counter, $vehicle_number_value, 'On Leave', $vehicle_values['field_vehicle_type_value'], $vehicle_values['field_vehicle_owner_value'], '', '', '', '', '', '', '', '', 0, '', '', '');
          }
          $counter++;
        }
      }
      $resource_not_utilized_cost = 0;
      if (!empty($all_resources)) {
        foreach ($all_resources as $trip_cashier_name => $value) {
          $input_resource_type = Ucfirst(strtolower($value['resource_type']));
          $cashier_data = get_resource_cost_per_day($trip_cashier_name, $input_resource_type);
          if (!empty($cashier_data[$trip_cashier_name]['cost_per_month'])) {
            $resource_not_utilized_cost += ($cashier_data[$trip_cashier_name]['cost_per_month'] / $resource_no_of_working_days);
          }
        }

        $resource_not_utilized_cost_print = round($resource_not_utilized_cost / $lak, $round_off_no_digits_2);
        $not_attempted_cost += $resource_not_utilized_cost;
      }
      $not_attempted_cost = round($not_attempted_cost / $lak, $round_off_no_digits_2);
      $rows[] = array($counter, t('Resources not utilized'), '', '', '', '', '', '', '', '', '', '', '', $resource_not_utilized_cost_print, '', '', '');
      $rows[] = array('', t('Total'), '', '', '', '', '', '', '', '', '', '', '', $not_attempted_cost, '', '', '');
    }
  }
  //print_r($all_resources);die;
  if ($export == 1) {
    return $rows;
  }
  $form_state['delivery_cost_details'] = $rows;
  $form_state['rebuild'] = TRUE;
}

//Get vehicle monthly cost
function get_vehilce_per_day_cost($vehicle_number) {
  global $user;
  $vehilce = array();
  $distributor_id = change_feeds_import_get_distributor_id($user->uid);
  if (!empty($vehicle_number)) {
    $vehicle_obj = node_load_multiple(NULL, array('title' => $vehicle_number, 'type' => 'vehicle_master', 'uid' => $distributor_id));
    if (!empty($vehicle_obj)) {
      $key = key($vehicle_obj);
      $cost_per_month = $vehicle_obj[$key]->field_cost_per_month[LANGUAGE_NONE][0]['value'];
      $cost_per_day = $vehicle_obj[$key]->field_cost_per_day[LANGUAGE_NONE][0]['value'];
      $mode_of_contract = $vehicle_obj[$key]->field_mode_of_contract[LANGUAGE_NONE][0]['value'];
      if ($mode_of_contract == 'Contracted' && !empty($cost_per_month)) {
        $vehilce[$vehicle_number] = array('cost_per_month' => $cost_per_month);
      }
      else if ($mode_of_contract == 'Market' && !empty($cost_per_day)) {
        $vehilce[$vehicle_number] = array('cost_per_day' => $cost_per_day);
      }
    }
  }
  return $vehilce;
}

function calculateWorkingDaysInMonth($year = '', $month = '', $weekoff = null) {
  //in case no values are passed to the function, use the current month and year
  if ($year == '') {
    $year = date('Y');
  }
  if ($month == '') {
    $month = date('m');
  }
  //create a start and an end datetime value based on the input year 
  $startdate = strtotime($year . '-' . $month . '-01');
  $enddate = strtotime('+' . (date('t', $startdate) - 1) . ' days', $startdate);
  $currentdate = $startdate;
  //get the total number of days in the month	
  $return = intval((date('t', $startdate)), 10);
  //loop through the dates, from the start date to the end date
  while ($currentdate <= $enddate) {
    // checking public holidays
    /* if (!empty($public_holidays)) {
      foreach ($public_holidays as $key => $value) {
      $public_hd = strtotime($value);
      if ($currentdate == $public_hd && strtolower(date('l',$public_hd))!=strtolower($weekoff)) {
      $return = $return - 1;
      }
      }
      } */
    //if you encounter a weekly off remove from the total days count
    if ((strtolower(date('l', $currentdate)) == strtolower($weekoff)) && !empty($weekoff)) {
      $return = $return - 1;
    }
    $currentdate = strtotime('+1 day', $currentdate);
  } //end date walk loop
  //return the number of working days
  return $return;
}

function get_resource_cost_per_day($resource_name, $input_resource_type) {
  global $user;
  $resource_details = array();
  $distributor_id = change_feeds_import_get_distributor_id($user->uid);
  if (!empty($resource_name) && !empty($input_resource_type)) {
    $input_resource_type = strtolower($input_resource_type);
    //$resource_obj = node_load_multiple(NULL, array('title' => $input_resource_type . $resource_name, 'type' => 'resource_master', 'uid' => $distributor_id));
    $query = db_select('node', 'n');
    $query->condition('n.type', 'resource_master');
    $query->condition('n.uid', $distributor_id);
    $query->join('field_data_field_resource_type', 'rt', 'rt.entity_id = n.nid');
    $query->join('field_data_field_resource_name', 'rn', 'rn.entity_id = n.nid');
    $query->condition('rt.field_resource_type_value', $input_resource_type);
    $query->condition('rn.field_resource_name_value', $resource_name);
    $query->leftjoin('field_data_field_cost_per_month', 'cost_per_month', 'cost_per_month.entity_id = n.nid');
    $query->leftjoin('field_data_field_weekly_off', 'weeklyoff', 'weeklyoff.entity_id = n.nid');
    $query->fields('rn', array('field_resource_name_value'));
    //$query->fields('cost_per_month', array('field_cost_per_month_value'));
    // $query->fields('weeklyoff', array('field_weekly_off_value'));
    $query->addExpression('cost_per_month.field_cost_per_month_value', 'cost_per_month');
    $query->addExpression('weeklyoff.field_weekly_off_value', 'weeklyoff');
    $resource_details = $query->execute()->fetchAll(PDO::FETCH_UNIQUE | PDO::FETCH_ASSOC);
    /* if (!empty($resource_obj)) {
      $key = key($resource_obj);
      $cost_per_month = $resource_obj[$key]->field_cost_per_month[LANGUAGE_NONE][0]['value'];
      $cost_per_day = $resource_obj[$key]->field_cost_per_day[LANGUAGE_NONE][0]['value'];
      $resource_weekly_off = $resource_obj[$key]->field_weekly_off[LANGUAGE_NONE][0]['value'];
      $resource_details[$resource_name] = array('cost_per_month'=>$cost_per_month,'weeklyoff'=>$resource_weekly_off);
      } */
  }
  return $resource_details;
}

function get_trip_attempted_vehicle($dispatch_date) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);
  if (!empty($dispatch_date)) {
    $query = db_select('dispatch_planning_api_data', 'ad');
    $query->condition('ad.distributor_id', $dis_id, '=');
    $query->condition('ad.date', $dispatch_date, '=');
    $query->join('node', 'n', 'n.title = ad.bill_number AND n.uid = ad.distributor_id');
    $query->condition('n.type', 'sales_register_data', '=');
    $query->condition('n.uid', $dis_id);
    $query->join('dispatch_planning_item_details', 'item', 'item.trip_bill_reference = ad.id');
    $query->join('node', 'disp_data', 'disp_data.title = ad.runsheet_number');
    $query->condition('disp_data.type', 'dispatch_data');
    $query->condition('disp_data .uid', $dis_id);
    $query->leftJoin('node', 'vehicle', 'vehicle.title = ad.vehicle');
    $query->condition('vehicle.type', 'vehicle_master');
    $query->condition('vehicle.uid', $dis_id);
    $query->groupBy('ad.runsheet_number');
    $query->addExpression('vehicle.title', 'vehicle_number');
    $res = $query->execute()->fetchAllKeyed(0, 0);
  }
  return $res;
}

function validate_delivery_cost_details_mtd(&$form, &$form_state) {
  global $user;
  $rows = array();
  $trip_ids = array();
  $dis_id = change_feeds_import_get_distributor_id($user->uid);
  $dispatch_from_date = $form_state['values']['dispatch_from_date'];
  $dispatch_to_date = $form_state['values']['dispatch_to_date'];
  if (!empty($dispatch_from_date) && !empty($dispatch_to_date)) {
    $query = db_select('dispatch_planning_api_data', 'ad');
    $query->condition('ad.distributor_id', $dis_id, '=');
    $query->condition('ad.date', array($dispatch_from_date, $dispatch_to_date), 'BETWEEN');
    $query->join('node', 'n', 'n.title = ad.bill_number AND n.uid = ad.distributor_id');
    $query->condition('n.type', 'sales_register_data', '=');
    $query->condition('n.uid', $dis_id);
    $query->join('dispatch_planning_item_details', 'item', 'item.trip_bill_reference = ad.id');
    $query->addExpression("ROUND(SUM(item.return_qty * item.tur))", 'return_value');
    // Join with dispatch_data to check vrs status and return flage 
    $query->join('node', 'disp_data', 'disp_data.title = ad.runsheet_number');
    $query->condition('disp_data.type', 'dispatch_data');
    $query->condition('disp_data .uid', $dis_id);
    //$query->leftjoin('field_data_field_vrs_status', 'vs', 'vs.entity_id = disp_data.nid');
    $query->leftjoin('field_data_field_vehicle_no', 'vehicle_no', 'vehicle_no.entity_id = disp_data.nid');
    // VRS Screen Return flag = 1 and VRS status = 1, 3 Means VRS pending
    $query->leftjoin('field_data_field_cashier_name', 'fld_cashier', 'fld_cashier.entity_id = disp_data.nid');
    // $vrs_status = array('1', '2', '3', '4', '5','-1','0');
    //$query->condition('vs.field_vrs_status_value', $vrs_status, 'IN');
    // Join with vehicle master
    $query->leftJoin('node', 'vehicle', 'vehicle.title = ad.vehicle');
    $query->condition('vehicle.type', 'vehicle_master');
    $query->leftjoin('field_data_field_vehicle_owner', 'vehicle_owner', 'vehicle_owner.entity_id = vehicle.nid');
    $query->leftjoin('field_data_field_vehicle_type', 'vehicle_type', 'vehicle_type.entity_id = vehicle.nid');
    $query->leftjoin('field_data_field_net_sales_bill_value', 'dispatched_value', 'dispatched_value.entity_id = disp_data.nid');
    $query->condition('vehicle.uid', $dis_id);
    $query->groupBy('ad.runsheet_number');
    $query->fields('ad', array('runsheet_number'));
    $query->fields('fld_cashier', array('field_cashier_name_value'));
    $query->fields('vehicle_owner', array('field_vehicle_owner_value'));
    $query->fields('vehicle_type', array('field_vehicle_type_value'));
    $query->fields('dispatched_value', array('field_net_sales_bill_value_value'));
    $query->addExpression('ad.updated_time', 'farey_updated_time');
    $res = $query->execute()->fetchAll(PDO::FETCH_UNIQUE | PDO::FETCH_ASSOC);
  }
  if (empty($res)) {
    //form_set_error('form',t('No data found'));
    //$form_state['#rebuild'] = TRUE;
    //drupal_goto('delivery-cost/mtd');
  }
}

function validate_delivery_cost_details(&$form, &$form_state) {
  global $user;
  $rows = array();
  $trip_ids = array();
  $dis_id = change_feeds_import_get_distributor_id($user->uid);
  $trip_id = $form_state['values']['trip_id'];
  $dispatch_date = $form_state['values']['dispatch_date'];
  //get_trip_not_attempted_vehicles
  if (!empty($dispatch_date)) {
    $trip_ids = _get_trip_ids_using_dispatch_date($dispatch_date);
    $dis_date = $dispatch_date;
  }
  else if (!empty($trip_id)) {
    $trip_ids[$trip_id] = $trip_id;
    $dis_date = _get_dispatch_date_using_trip_id($trip_id);
  }
  $dis_date = strtotime($dis_date);
  if (!empty($trip_ids)) {
    $query = db_select('dispatch_planning_api_data', 'ad');
    $query->condition('ad.distributor_id', $dis_id, '=');
    $query->condition('ad.runsheet_number', $trip_ids, 'IN');
    $query->join('node', 'n', 'n.title = ad.bill_number AND n.uid = ad.distributor_id');
    $query->condition('n.type', 'sales_register_data', '=');
    $query->condition('n.uid', $dis_id);
    $query->join('dispatch_planning_item_details', 'item', 'item.trip_bill_reference = ad.id');
    $query->addExpression("ROUND(SUM(item.return_qty * item.tur))", 'return_value');
    // Join with dispatch_data to check vrs status and return flage 
    $query->join('node', 'disp_data', 'disp_data.title = ad.runsheet_number');
    $query->condition('disp_data.type', 'dispatch_data');
    $query->condition('disp_data .uid', $dis_id);
    //$query->leftjoin('field_data_field_vrs_status', 'vs', 'vs.entity_id = disp_data.nid');
    $query->leftjoin('field_data_field_vehicle_no', 'vehicle_no', 'vehicle_no.entity_id = disp_data.nid');
    // VRS Screen Return flag = 1 and VRS status = 1, 3 Means VRS pending
    $query->leftjoin('field_data_field_cashier_name', 'fld_cashier', 'fld_cashier.entity_id = disp_data.nid');
    // $vrs_status = array('1', '2', '3', '4', '5','-1','0');
    //$query->condition('vs.field_vrs_status_value', $vrs_status, 'IN');
    // Join with vehicle master
    $query->leftJoin('node', 'vehicle', 'vehicle.title = ad.vehicle');
    $query->condition('vehicle.type', 'vehicle_master');
    $query->leftjoin('field_data_field_vehicle_owner', 'vehicle_owner', 'vehicle_owner.entity_id = vehicle.nid');
    $query->leftjoin('field_data_field_vehicle_type', 'vehicle_type', 'vehicle_type.entity_id = vehicle.nid');
    $query->leftjoin('field_data_field_net_sales_bill_value', 'dispatched_value', 'dispatched_value.entity_id = disp_data.nid');
    $query->condition('vehicle.uid', $dis_id);
    $query->groupBy('ad.runsheet_number');
    $query->fields('ad', array('runsheet_number'));
    $query->fields('fld_cashier', array('field_cashier_name_value'));
    $query->fields('vehicle_owner', array('field_vehicle_owner_value'));
    $query->fields('vehicle_type', array('field_vehicle_type_value'));
    $query->fields('dispatched_value', array('field_net_sales_bill_value_value'));
    $query->addExpression('ad.updated_time', 'farey_updated_time');
    $query->addExpression('ad.delivery_pending_flag', 'delivery_pending_flag');
    $query->addExpression('vehicle_no.field_vehicle_no_value', 'vehicle_number');
    $res = $query->execute()->fetchAll(PDO::FETCH_UNIQUE | PDO::FETCH_ASSOC);
  }

  if (empty($res)) {
    form_set_error('form', t('No data found'));
    $form_state['#rebuild'] = TRUE;
    drupal_goto('delivery-cost');
  }
}

function delivery_cost_export() {
  // this is for cache expired
  $paramenters = drupal_get_query_parameters();
  if (!empty($paramenters)) {
    if (!empty($paramenters['dispatch_date'])) {
      $form_state['values']['dispatch_date'] = $paramenters['dispatch_date'];
    }
    elseif (!empty($paramenters['trip_id'])) {
      $form_state['values']['trip_id'] = $paramenters['trip_id'];
    }
    if (!empty($paramenters['dispatch_from_date']) && !empty($paramenters['dispatch_to_date'])) {
      $form_state['values']['dispatch_from_date'] = $paramenters['dispatch_from_date'];
      $form_state['values']['dispatch_to_date'] = $paramenters['dispatch_to_date'];
    }
  }
  $arg3 = arg(2);
  $label = $arg3;
  $cache_name = '';
  $file = "hul_delivery_cost_$label" . '_' . date('d-M-Y') . '_' . time() . ".xls";
  // for MTD export
  if ($label == 'mtd') {
    $data = cache_get('delivery_cost_download_mtd' . session_id());
    if (empty($data)) {
      $options = get_delivery_cost_details_mtd($form, $form_state, $export = 1);
      $header = array(
        'Vehicle Number',
        'Owner',
        'Cashier',
        'Trips',
        'Dispatched Value(Rs Lakhs)',
        'Delivered Value(Rs Lakhs)',
        'Returns Value(Rs Lakhs)',
        'Average Delivered Value Per Trip',
        'Total Cost (Rs Lakhs)',
        'Total Cost (for Delivered)',
        'Average Cost Per Day (Rs Lakhs)',
        'Return % Sale(vs Delivered)',
        'Return % Sale(vs Dispatched)',
        'DP Value',
        'DP % Value',
        //'Cost % Sale',
        'Cost % Sale vs Dispatched',
        'Cost % Sale vs Delivered',
      );
      $delivery_cost_table = array(
        '#theme' => 'table',
        '#prefix' => '<div class="delivery_cost_table custom_middle_table">',
        '#tree' => TRUE,
        '#header' => $header,
        '#rows' => $options,
        '#attributes' => array(
          'class' => array('delivery_cost'),
          'id' => 'delivery_cost'
        ),
        '#suffix' => '</div>',
        '#sticky' => FALSE,
      );
      $table = drupal_render($delivery_cost_table);
    }
    else {
      $table = drupal_render($data->data);
    }
  }
  // Delivery cost export
  else {
    $data = cache_get('delivery_cost_download' . session_id());
    // if session data is empty then calculate to get delivery cost export
    if (empty($data->data)) {
      $options = get_delivery_cost_details($form, $form_state, $export = 1);
      $header = array(
        '#',
        'Vehicle Number',
        'Trip ID',
        'Vehicle Type',
        'Owner',
        'Cashier',
        'Dispatched Value(Rs Lakhs)',
        'Delivered Value(Rs Lakhs)',
        'Returns Value(Rs Lakhs)',
        'Returns %(vs Delivered)',
        'Returns %(vs Dispatched)',
        'DP Value',
        'DP %',
        'Total Cost for the day',
        'Total Cost (for Delivered)',
        'Cost as a % of Dispatched Value',
        'Cost as a % of Delivered Value',
      );
      $delivery_cost_table = array(
        '#theme' => 'table',
        '#prefix' => '<div class="delivery_cost_table">',
        '#tree' => TRUE,
        '#header' => $header,
        '#rows' => $options,
        '#attributes' => array(
          'class' => array('delivery_cost'),
          'id' => 'delivery_cost'
        ),
        '#suffix' => '</div>',
        '#sticky' => FALSE,
      );
      $table = drupal_render($delivery_cost_table);
    }
    else {
      $table = drupal_render($data->data);
    }
  }
  header("Content-type: application/vnd.ms-excel");
  header("Content-Disposition: attachment; filename=$file");
  echo $table;
}

function vehicle_day_wise_performance_form($form, &$form_state) {
  $form['#attached']['css'][] = drupal_get_path('module', 'deliverycost_calculation') . '/css/delivery_cost.css';
  $form['#attached']['js'][] = drupal_get_path('module', 'deliverycost_calculation') . '/js/delivery_cost.js';
  if (!isset($form_state['delivery_cost_details'])) {
    $form_state['delivery_cost_details'] = array();
  }
  $options = $form_state['delivery_cost_details'];
  $date = date('Y-m-d');
  $end_date = $date;
  $start_date = date('Y-m-d', strtotime($date . ' - 5 days'));
  $form['#prefix'] = '<div id="itemlist-div" class="custom_form_wrapper">';
  $form['#suffix'] = '</div>';
  $form['dispatch_from_date'] = array(
    '#type' => 'date_popup',
    '#title' => t('Dispatch From Date*'),
    '#date_format' => 'Y-m-d',
    '#attributes' => array('class' => array('dispatch_date')),
    '#date_label_position' => '',
    '#date_year_range' => '-20:+2',
    '#size' => 30,
    '#default_value' => $start_date,
    '#prefix' => '<div class="form_row form_row2"><div class="col_3 vehicle_performance">',
    '#suffix' => '</div>'
  );
  $form['dispatch_to_date'] = array(
    '#type' => 'date_popup',
    '#title' => t('Dispatch To Date*'),
    '#date_format' => 'Y-m-d',
    '#attributes' => array('class' => array('dispatch_date')),
    '#date_label_position' => '',
    '#date_year_range' => '-20:+2',
    '#default_value' => $end_date,
    '#size' => 30,
    '#prefix' => '<div class="form_row form_row2"><div class="col_3 vehicle_performance">',
    '#suffix' => '</div>'
  );
  $form['vehicle_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Vehicle Number'),
    '#autocomplete_path' => 'vehicle_maintenance/autocomplete',
    '#size' => 20,
    '#maxlength' => 50,
    //'#attributes' => array('class' => array('trip-id'), 'id' => 'trip-id'),
    '#prefix' => '<div class="form_row form_row2"><div class="col_3 vehicle_performance">',
    '#suffix' => '</div></div></div>'
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Show',
    '#name' => 'show',
    '#attributes' => array('class' => array('vrs-trip-submit')),
    '#submit' => array('vehicle_day_wise_performance_form_submit'),
    // '#validate' => array('validate_delivery_cost_details_mtd'),
    '#prefix' => '<div class="col_3">',
    '#suffix' => '</div></div>'
  );
  if (!empty($options)) {
    $header = array(
      'Vehicle Number',
      'Date',
      'Active/On Leave',
      'Trip ID',
      'Previous Trip Status',
      'Cashier',
      'Dispatched Value(Rs Lakhs)',
      'Delivered Value(Rs Lakhs)',
      'Cost(Rs Lakhs)',
      'Cost % of Dispatched Value',
      'Cost % of Delivered Value',
      'Reason if on Leave/Maintenance',
    );
    $form['delivery_cost_table'] = array(
      '#theme' => 'table',
      '#prefix' => '<div class="delivery_cost_table custom_middle_table">',
      '#tree' => TRUE,
      '#header' => $header,
      '#rows' => $options,
      '#attributes' => array(
        'class' => array('delivery_cost'),
        'id' => 'delivery_cost'
      ),
      '#suffix' => '</div>',
      '#sticky' => FALSE,
    );
    cache_set('delivery_cost_download_vehicle' . session_id(), $form['delivery_cost_table'], 'cache', time() + 60 * 10);
    $rr = l('Download', "vehicle_daily_wise_report/export", array('attributes' => array("target" => "_blank"), 'query' => array('dispatch_from_date' => $form_state['values']['dispatch_from_date'], 'dispatch_to_date' => $form_state['values']['dispatch_to_date'], 'vehicle_number' => $form_state['values']['vehicle_number'])));
    $form['download_mark'] = array(
      '#type' => 'markup',
      '#markup' => $rr,
    );
  }
  return $form;
}

function vehicle_day_wise_performance_form_submit($form, &$form_state, $export = 0) {
  global $user;
  $rows = array();
  $trip_ids = array();
  $dis_id = change_feeds_import_get_distributor_id($user->uid);
  $dispatch_from_date = $form_state['values']['dispatch_from_date'];
  $dispatch_to_date = $form_state['values']['dispatch_to_date'];
  $round_off_no_digits = 3;
  $round_off_no_digits_2 = 4;
  $vehicle_number = $form_state['values']['vehicle_number'];
  $lak = 100000;
  if (!empty($dispatch_from_date) && !empty($dispatch_to_date)) {
    $all_vehicles = get_all_active_vehicles_on_date_range(strtotime($dispatch_from_date), strtotime($dispatch_to_date));
    $absent_vehilce_details = get_maintenance_dates_of_vehicle(strtotime($dispatch_from_date), strtotime($dispatch_to_date), 1);
    $all_extra_working = get_extra_working_days(strtotime($dispatch_from_date), strtotime($dispatch_to_date));
    $weekoff = get_resource_weekly_off_day();
    $dis_date = strtotime($dispatch_from_date);
    $year = date('Y', $dis_date);
    $month = date('m', $dis_date);
    $no_of_working_days = calculateWorkingDaysInMonth($year, $month, $weekoff);
    if (!empty($all_vehicles[$vehicle_number])) {
      $vehicle_list[$vehicle_number] = $all_vehicles[$vehicle_number];
    }
    else {
      $vehicle_list = $all_vehicles;
    }
    $all_vehicles = $vehicle_list;
  }
  $mtd_data = array();
  $rows = array();
  $trip_created_days = array();
  if (!empty($all_vehicles)) {
    foreach ($all_vehicles as $vehicle_number_value => $vehicle_values) {
      $vehicle_total_delivered_value = 0;
      $vehicle_total_dispatched_value = 0;
      $mtd_result = array();
      $vehicle_cost_calc = get_vehilce_per_day_cost($vehicle_number_value);
      $query = db_select('dispatch_planning_api_data', 'ad');
      $query->condition('ad.distributor_id', $dis_id, '=');
      // $query->condition('ad.delivery_pending_flag', 1, '!=');
      $query->condition('ad.date', array($dispatch_from_date, $dispatch_to_date), 'BETWEEN');
      $query->join('node', 'n', 'n.title = ad.bill_number AND n.uid = ad.distributor_id');
      $query->condition('n.type', 'sales_register_data', '=');
      $query->condition('n.uid', $dis_id);
      $query->join('dispatch_planning_item_details', 'item', 'item.trip_bill_reference = ad.id');
      //$query->addExpression("(SUM(item.return_qty * item.sum_net_sales_value))", 'return_value');
      $query->addExpression("SUM(item.return_qty * item.sum_net_sales_value)", 'return_value');
      $query->addExpression("(SUM(item.res_sku_actual_quantity1 * item.sum_net_sales_value))", 'delivered_value');
      $query->addExpression("(SUM(item.sum_net_sales_quantity * item.sum_net_sales_value))", 'dispatched_value');
      // Join with dispatch_data to check vrs status and return flage 
      $query->join('node', 'disp_data', 'disp_data.title = ad.runsheet_number');
      $query->condition('disp_data.type', 'dispatch_data');
      // $query->leftjoin('field_data_field_delivery_boy', 'delivery_boys', 'delivery_boys.entity_id = disp_data.nid');
      //$query->leftjoin('field_data_field_resource_name', 'resource_names', 'resource_names.entity_id = delivery_boys.field_delivery_boy_nid');
      $query->condition('disp_data .uid', $dis_id);
      //$query->leftjoin('field_data_field_vrs_status', 'vs', 'vs.entity_id = disp_data.nid');
      $query->leftjoin('field_data_field_vehicle_no', 'vehicle_no', 'vehicle_no.entity_id = disp_data.nid');
      // VRS Screen Return flag = 1 and VRS status = 1, 3 Means VRS pending
      $query->leftjoin('field_data_field_cashier_name', 'fld_cashier', 'fld_cashier.entity_id = disp_data.nid');
      // $vrs_status = array('1', '2', '3', '4', '5','-1','0');
      //$query->condition('vs.field_vrs_status_value', $vrs_status, 'IN');
      // Join with vehicle master
      $query->leftJoin('node', 'vehicle', 'vehicle.title = ad.vehicle');
      $query->condition('vehicle.type', 'vehicle_master');
      $query->condition('vehicle.uid', $dis_id);
      $query->groupBy('vehicle_no.field_vehicle_no_value,ad.date');
      $query->addExpression("GROUP_CONCAT(DISTINCT ad.runsheet_number  separator ', ' )", 'trip_ids');
      $query->addExpression("GROUP_CONCAT(DISTINCT fld_cashier.field_cashier_name_value  separator ', ' )", 'cashier_names');
      $query->addExpression('ad.date', 'dispacth_date');
      $query->fields('ad', array('runsheet_number'));
      $query->fields('fld_cashier', array('field_cashier_name_value'));
      $query->addExpression('vehicle_no.field_vehicle_no_value', 'vehicle_number');
      $query->condition('vehicle_no.field_vehicle_no_value', $vehicle_number_value);
      //$query->condition('vehicle_no.field_vehicle_no_value', 'testvehicle123');
      $res = $query->execute()->fetchAll(PDO::FETCH_UNIQUE | PDO::FETCH_ASSOC);
      //echo $vehiclenumber;
      //print_r($res);die;
      $counter = 1;
      $vehicle_name_printed = 0;
      if (!empty($res)) {
        $trip_titles = array_keys($res);
        $delivery_boy_trip = get_deliveryboys($trip_titles);
        $trip_ids = array_keys($res);
        $total_dispatched_value = 0;
        $total_return_value = 0;
        $total_dp_value = 0;
        $total_return_percent = 0;
        $total_dp_percent = 0;
        $vh_not_utilized_non_working_days = array();
        $vh_not_ampted = array();

        foreach ($res as $key => $value) { //print_r($value);
          if (!empty($value['dispacth_date'])) {
            $dis_date = strtotime($value['dispacth_date']);
            $year = date('Y', $dis_date);
            $month = date('m', $dis_date);
            $no_of_working_days = calculateWorkingDaysInMonth($year, $month, $weekoff);
            $public_holidays = get_public_holiday($month, $year);
            $trip_created_days[$value['vehicle_number']][$value['dispacth_date']] = $value['dispacth_date'];
          }

          $vehicles_not_attempted_trips = array();
          // Not considering weekly off only calendar days for resources
          $resource_no_of_working_days = calculateWorkingDaysInMonth($year, $month);
          $vehicle_per_day_cost = 0;
          $delivery_boy_cost = 0; // Multiple delivery boy will be in one trip
          $cashier_cost = 0;
          $total_trip_delivery_cost = 0;
          //$dispatched_value_in_lak = round($value['field_net_sales_bill_value_value'] / $lak, 2);
          $dispatched_value_in_lak = round($value['dispatched_value'] / $lak, $round_off_no_digits);
          $return_value_in_lak = round($value['return_value'] / $lak, $round_off_no_digits);
          ;
          $delivered_value_in_lak = round($value['delivered_value'] / $lak, $round_off_no_digits);
          $return_percent = ($dispatched_value_in_lak / $return_value_in_lak) * 100;
          $dp_value = get_dp_value(array($key));
          $dp_value_in_lak = 0;
          $dp_percent = 0;
          if (!empty($dp_value)) {
            $dp_value_in_lak = round($dp_value / $lak, $round_off_no_digits);
            $dp_value_percent = ($dp_value_in_lak / $dispatched_value_in_lak) * 100;
          }
          // If the vehicle is contracted then no of working days will change based o pdd & weekoff
          //echo $value['vehicle_number'];die;
          $vehicle_cost = get_vehilce_per_day_cost($value['vehicle_number']);
          if (!empty($vehicle_cost[$value['vehicle_number']]['cost_per_month'])) {
            $vehilce_monthly_cost = $vehicle_cost[$value['vehicle_number']]['cost_per_month'];
            $vehicle_per_day_cost = $vehilce_monthly_cost / $no_of_working_days;
          }
          else if (!empty($vehicle_cost[$value['vehicle_number']]['cost_per_day'])) {
            $vehicle_per_day_cost = $vehicle_cost[$value['vehicle_number']]['cost_per_day'];
          }
          $input_resource_type = 'Cashier';
          $cashier_name = $value['field_cashier_name_value'];
          //$cashier_cost = get_resource_cost_per_day($cashier_name, $input_resource_type);
          $cashier_data = get_resource_cost_per_day($cashier_name, $input_resource_type);
          if (!empty($cashier_data[$cashier_name]['cost_per_month'])) {
            $cashier_cost = $cashier_data[$cashier_name]['cost_per_month'] / $resource_no_of_working_days;
          }

          $delivery_boy_trip = get_deliveryboys(array($key));
          if (!empty($delivery_boy_trip[$key]['delivery_boys'])) {
            $delivery_boys = array_unique(explode(',', $delivery_boy_trip[$key]['delivery_boys']));
            if (!empty($delivery_boys)) {
              foreach ($delivery_boys as $delivery_boy_value) {
                $input_resource_type = 'Delivery Boy';
                $delivery_boy_name = $delivery_boy_value;
                //$delivery_boy_cost += get_resource_cost_per_day($delivery_boy_name, $input_resource_type);
                $deliveryboy_data = get_resource_cost_per_day($delivery_boy_name, $input_resource_type);
                if (!empty($deliveryboy_data[$delivery_boy_name]['cost_per_month'])) {

                  $delivery_boy_cost += $deliveryboy_data[$delivery_boy_name]['cost_per_month'] / $resource_no_of_working_days;
                }
              }
            }
          }

          //echo $vehicle_per_day_cost;die;
          $total_trip_delivery_cost = ($delivery_boy_cost + $cashier_cost + $vehicle_per_day_cost) / $lak;
          $total_trip_delivery_cost = round($total_trip_delivery_cost, $round_off_no_digits_2);
          $mtd_result[$value['vehicle_number']]['cashiers'][$value['field_cashier_name_value']][$key] = array('dispatched_value' => $dispatched_value_in_lak, 'delivered_value' => $delivered_value_in_lak, 'return_value' => $return_value_in_lak, 'dp_value' => $dp_value_in_lak, 'dp_percent' => $dp_value_percent, 'total_cost' => $total_trip_delivery_cost);
          $cost_percent_dispatched = round($total_trip_delivery_cost / $dispatched_value_in_lak * 100);
          $cost_percent_delivered = round($total_trip_delivery_cost / $delivered_value_in_lak * 100);
          $previous_trip_status = get_previous_trip_status($dis_date, $vehicle_number_value);
          $rows[$vehicle_number_value][$value['dispacth_date']] = array('vehicle_no' => '', 'date' => date('d-M-Y', $dis_date), 'active' => 'Active', 'trip_ids' => $value['trip_ids'], $previous_trip_status, 'cashier' => $value['cashier_names'], 'dispatched_value' => $dispatched_value_in_lak, 'delivered_value' => $delivered_value_in_lak, 'cost' => $total_trip_delivery_cost, 'cost_vs_dispatched' => $cost_percent_dispatched, 'cost_vs_delivered' => $cost_percent_delivered, 'leave_reason' => '');
        }
      }

      $not_attempted_cost = 0;
      $not_attempted_count = 0;
      $vehilce_monthly_cost = 0;
      $vehicle_per_day_cost = 0;
      $vehilce_monthly_cost = 0;
      if (!empty($all_extra_working[$vehicle_number_value])) {
        $trip_not_created_on_non_working_days = array_diff($all_extra_working[$vehicle_number_value], $trip_created_days[$vehicle_number_value]);
        foreach ($trip_not_created_on_non_working_days as $value) {
          if (!empty($value)) {
            $date_array = explode('-', $value);
            $year = $date_array[0];
            $month = $date_array[1];
            $no_of_working_days = calculateWorkingDaysInMonth($year, $month, $weekoff);
            if (!empty($vehicle_cost_calc[$vehicle_number_value]['cost_per_month'])) {
              $vehilce_monthly_cost = $vehicle_cost_calc[$vehicle_number_value]['cost_per_month'];
              $vehicle_per_day_cost = $vehilce_monthly_cost / $no_of_working_days;
            }
            else if (!empty($vehicle_cost_calc[$vehicle_number_value]['cost_per_day'])) {
              $vehicle_per_day_cost = $vehicle_cost_calc[$vehicle_number_value]['cost_per_day'];
            }
            $not_attempted_count++;
            $not_attempted_cost += $not_attempted_cost;
            $previous_trip_status = get_previous_trip_status(strtotime($value), $vehicle_number_value);
            $rows[$vehicle_number_value][$value] = array('vehicle_no' => '', 'date' => $value, 'active' => 'Active', 'trip_ids' => 'Trip Not Created', $previous_trip_status, 'cashier_names' => '', 'dispatched_value' => '', 'delivered_value' => '', 'cost' => round($vehicle_per_day_cost / $lak, $round_off_no_digits_2), 'cost_vs_dispatched' => '', 'cost_vs_delivered' => '', 'leave_reason' => '');
          }
        }
      }
      //Checking working days trip not created
      $ts1 = strtotime($dispatch_from_date);
      $ts2 = strtotime($dispatch_to_date);
      $contract_end_period = strtotime($vehicle_values['end_period']);
      $contract_start_period = strtotime($vehicle_values['start_period']);
      $currentdate = $ts1;
      $year1 = date('Y', $ts1);
      $year2 = date('Y', $ts2);
      $month1 = date('m', $ts1);
      $month2 = date('m', $ts2);
      while ($currentdate <= $ts2 && $currentdate <= $contract_end_period) {
        $day_of_month = date('Y-m-d', $currentdate);
        if (strtolower(date('l', $currentdate)) != strtolower($weekoff) && $contract_start_period <= $currentdate) {
          //if (empty($absent_vehilce_details[$vehicle_number_value][$day_of_month]) && empty($trip_created_days[$vehicle_number_value][$day_of_month])) {
          if (empty($trip_created_days[$vehicle_number_value][$day_of_month])) {  
           $days[$vehicle_number_value][$day_of_month] = $day_of_month;
          }
        }
        $currentdate = strtotime('+1 day', $currentdate);
      }
      // print_r($trip_created_days);die;

      foreach ($days[$vehicle_number_value] as $datekey => $date_value) {
        if (!empty($date_value)) {
          $date_array = explode('-', $date_value);
          $year = $date_array[0];
          $month = $date_array[1];
          $no_of_working_days = calculateWorkingDaysInMonth($year, $month, $weekoff);
          if (!empty($vehicle_cost_calc[$vehicle_number_value]['cost_per_month'])) {
            $vehilce_monthly_cost = $vehicle_cost_calc[$vehicle_number_value]['cost_per_month'];
            $vehicle_per_day_cost = $vehilce_monthly_cost / $no_of_working_days;
          }
          else if (!empty($vehicle_cost_calc[$vehicle_number_value]['cost_per_day'])) {
            $vehicle_per_day_cost = $vehicle_cost_calc[$vehicle_number_value]['cost_per_day'];
          }
          //if (!empty($days[$vehicle_number_value])) {
          $not_attempted_count = $not_attempted_count + 1;
          //}
          $not_attempted_cost += $vehicle_per_day_cost;
          $previous_trip_status = get_previous_trip_status(strtotime($date_value), $vehicle_number_value);
          $trip_not_created_reason = get_trip_not_created_reason($date_value, $vehicle_number = $vehicle_number_value, $resource_name = null, $resource_nid = null);
          if (empty($trip_not_created_reason)) {
            $trip_not_created_reason = t('Trip Not Created');
          }
          $rows[$vehicle_number_value][$date_value] = array('vehicle_no' => '', 'date' => $date_value, 'active' => 'Active', 'trip_ids' => $trip_not_created_reason, $previous_trip_status, 'cashier_names' => '', 'dispatched_value' => '', 'delivered_value' => '', 'cost' => round($vehicle_per_day_cost / $lak, $round_off_no_digits_2), 'cost_vs_dispatched' => '', 'cost_vs_delivered' => '', 'leave_reason' => '');
        }
      }
      $not_attempted_cost = $not_attempted_cost / $lak;
      $average_not_attempted = $not_attempted_cost / $not_attempted_count;


      $cost_per_sale = (($vehicle_total_cost + $not_attempted_cost) / $vehicle_total_delivered_value) * 100;
      $cost_per_sale_dispatched = (($vehicle_total_cost + $not_attempted_cost) / $vehicle_total_dispatched_value) * 100;
      //$rows[] = array($vehicle_number_value, '', 'Total', '', '', '', '', '', '', '', '', '', '', round($cost_per_sale_dispatched,$round_off_no_digits_2),round($cost_per_sale,$round_off_no_digits_2));


      if (!empty($absent_vehilce_details[$vehicle_number_value])) {
        foreach ($absent_vehilce_details[$vehicle_number_value] as $date_key => $date_value) {
          $rows[$vehicle_number_value][$date_key] = array('vehicle_no' => '', 'date' => $date_key, 'active' => 'On Leave', 'trip_ids' => '', 'previous_trip' => '', 'cashier_names' => '', 'dispatched_value' => '', 'delivered_value' => '', 'cost' => '', 'cost_vs_dispatched' => '', 'cost_vs_delivered' => '', 'leave_reason' => $date_value);
        }
      }
      //print_r($absent_vehilce_details);die;
      usort($rows[$vehicle_number_value], "sortFunction");

      $rows[$vehicle_number_value][0]['vehicle_no'] = $vehicle_number_value;
      $datas[$vehicle_number_value] = $rows[$vehicle_number_value];
    }
    if (!empty($datas)) {
      $rows = array();
      foreach ($datas as $key => $value) {
        foreach ($value as &$val) {
          $val['date'] = date('d-M-Y', strtotime($val['date']));
          $rows[] = $val;
        }
      }
      $rows[] = array();
    }

    // print_r($rows);die;
  }
  if ($export == 1) {
    return $rows;
  }
  $form_state['delivery_cost_details'] = $rows;
  $form_state['rebuild'] = TRUE;
}

function sortFunction($a, $b) {
  return strtotime($a["date"]) - strtotime($b["date"]);
}

function vehicle_daily_wise_report_export() {
  $query_parameters = drupal_get_query_parameters();
  $file = "hul_vehicle_daily_wise_report_" . date('d-M-Y') . ".xls";
  $data = cache_get('delivery_cost_download_vehicle' . session_id());
  if (empty($data->data)) {
    if (!empty($query_parameters['dispatch_from_date']) && !empty($query_parameters['dispatch_to_date'])) {
      $form_state['values']['dispatch_from_date'] = $query_parameters['dispatch_from_date'];
      $form_state['values']['dispatch_to_date'] = $query_parameters['dispatch_to_date'];
      $form_state['values']['vehicle_number'] = $query_parameters['vehicle_number'];
      ;
      $options = vehicle_day_wise_performance_form_submit($form, $form_state, $export = 1);
    }
    if (!empty($options)) {
      $header = array(
        'Vehicle Number',
        'Date',
        'Active/On Leave',
        'Trip ID',
        'Previous Trip Status',
        'Cashier',
        'Dispatched Value(Rs Lakhs)',
        'Delivered Value(Rs Lakhs)',
        'Cost(Rs Lakhs)',
        'Cost % of Dispatched Value',
        'Cost % of Delivered Value',
        'Reason if on Leave/Maintenance',
      );
      $delivery_cost_table = array(
        '#theme' => 'table',
        '#prefix' => '<div class="delivery_cost_table custom_middle_table">',
        '#tree' => TRUE,
        '#header' => $header,
        '#rows' => $options,
        '#attributes' => array(
          'class' => array('delivery_cost'),
          'id' => 'delivery_cost'
        ),
        '#suffix' => '</div>',
        '#sticky' => FALSE,
      );
      $table = drupal_render($delivery_cost_table);
    }
  }
  else {
    $table = drupal_render($data->data);
  }
  header("Content-type: application/vnd.ms-excel");
  header("Content-Disposition: attachment; filename=$file");
  echo $table;
}

function get_previous_trip_status($dispatch_date, $vehicle_number) {
  $vrs_status = '';
  if (!empty($dispatch_date) && !empty($vehicle_number)) {
    global $user;
    $dis_id = change_feeds_import_get_distributor_id($user->uid);
    $previous_date = strtotime('-1 day', $dispatch_date);
    //echo date('Y-m-d',$previous_date);

    $previous_trip_ids = _get_previous_trip_id($previous_date, $vehicle_number);
    if (!empty($previous_trip_ids)) {
      foreach ($previous_trip_ids as $previous_trip_id => $value) {
        $trip_detail = check_trip_id_exists_or_not($dis_id, $previous_trip_id);
        $status = $trip_detail->field_vrs_status_value;
        $field = field_info_field('field_vrs_status');
        $allowed_values = list_allowed_values($field);
        if (isset($allowed_values[$status])) {
          $vrs_status .= $previous_trip_id . '-' . $allowed_values[$status] . '<br>';
        }
      }
    }
  }
  return $vrs_status;
}

function _get_previous_trip_id($dispatch_date, $vehicle_number) {
  $res = '';
  if (!empty($dispatch_date) && !empty($vehicle_number)) {
    global $user;
    $dispatch_date = date('Y-m-d', $dispatch_date);
    $weekoff = get_resource_weekly_off_day();
    $dis_id = change_feeds_import_get_distributor_id($user->uid);
    $query = db_select('node', 'n');
    $query->condition('n.type', 'dispatch_data');
    $query->condition('n.uid', $dis_id);
    $query->join('field_data_field_dispatch_date', 't', 't.entity_id = n.nid');
    $query->join('field_data_field_vehicle_no', 'vehicle', 'vehicle.entity_id = n.nid');
    $query->condition('vehicle.field_vehicle_no_value', $vehicle_number);
    $query->condition('t.field_dispatch_date_value', $dispatch_date);
    $query->fields('n', array('title'));
    $res = $query->execute()->fetchAll(PDO::FETCH_UNIQUE | PDO::FETCH_ASSOC);
    $dispatch_date_in_strtotime = strtotime($dispatch_date);
    if (strtolower(date('l', $dispatch_date_in_strtotime)) == strtolower($weekoff) && empty($res)) {
      $previous_date = strtotime('-1 day', $dispatch_date_in_strtotime);
      $res = _get_previous_trip_id($previous_date, $vehicle_number);
    }
  }
  return $res;
}

// resource daily wise performance report
function resource_day_wise_performance_form($form, &$form_state) {
  $form['#attached']['css'][] = drupal_get_path('module', 'deliverycost_calculation') . '/css/delivery_cost.css';
  $form['#attached']['js'][] = drupal_get_path('module', 'deliverycost_calculation') . '/js/delivery_cost.js';
  if (!isset($form_state['delivery_cost_details'])) {
    $form_state['delivery_cost_details'] = array();
  }
  $options = $form_state['delivery_cost_details'];
  $date = date('Y-m-d');
  $end_date = $date;
  $start_date = date('Y-m-d', strtotime($date . ' - 5 days'));
  $form['#prefix'] = '<div id="itemlist-div" class="custom_form_wrapper">';
  $form['#suffix'] = '</div>';
  $form['dispatch_from_date'] = array(
    '#type' => 'date_popup',
    '#title' => t('Dispatch From Date*'),
    '#date_format' => 'Y-m-d',
    '#attributes' => array('class' => array('dispatch_date')),
    '#date_label_position' => '',
    '#date_year_range' => '-20:+2',
    '#size' => 30,
    '#default_value' => $start_date,
    '#prefix' => '<div class="form_row form_row2"><div class="col_3 vehicle_performance">',
    '#suffix' => '</div>'
  );
  $form['dispatch_to_date'] = array(
    '#type' => 'date_popup',
    '#title' => t('Dispatch To Date*'),
    '#date_format' => 'Y-m-d',
    '#attributes' => array('class' => array('dispatch_date')),
    '#date_label_position' => '',
    '#date_year_range' => '-20:+2',
    '#default_value' => $end_date,
    '#size' => 30,
    '#prefix' => '<div class="form_row form_row2"><div class="col_3 vehicle_performance">',
    '#suffix' => '</div>'
  );
  //$options = array(''=>t('-Select-'),'Cashier'=>'Cashier','Delivery Boy'=>'Delivery Boy');
  /* $form['resource_type'] = array(
    '#type' => 'select',
    '#title' => t('Resource Type'),
    '#options' => $options,
    '#size' => 20,
    '#maxlength' => 50,
    //'#attributes' => array('class' => array('trip-id'), 'id' => 'trip-id'),
    '#prefix' => '<div class="form_row form_row2"><div class="col_3 vehicle_performance">',
    '#suffix' => '</div></div></div>'
    ); */

  $form['resource_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Resource Name'),
    '#autocomplete_path' => 'get_active_resource/autocomplete',
    '#size' => 20,
    '#maxlength' => 50,
    //'#attributes' => array('class' => array('trip-id'), 'id' => 'trip-id'),
    '#prefix' => '<div class="form_row form_row2"><div class="col_3 vehicle_performance">',
    '#suffix' => '</div></div></div>'
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Show',
    '#name' => 'show',
    '#attributes' => array('class' => array('vrs-trip-submit')),
    '#submit' => array('resource_day_wise_performance_form_submit'),
    //'#validate' => array('validate_delivery_cost_details_mtd'),
    '#prefix' => '<div class="col_3">',
    '#suffix' => '</div></div>'
  );
  if (!empty($options)) {
    $header = array(
      'Resource Name',
      'Resource Type',
      'Dispatch Date',
      'Active/On Leave',
      'Trip ID',
      'Previous Trip Status',
      'Cost',
      'Vehicle Number',
      'Reason if on Leave/Maintenance',
    );
    $form['delivery_cost_table'] = array(
      '#theme' => 'table',
      '#prefix' => '<div class="delivery_cost_table custom_middle_table">',
      '#tree' => TRUE,
      '#header' => $header,
      '#rows' => $options,
      '#attributes' => array(
        'class' => array('delivery_cost'),
        'id' => 'delivery_cost'
      ),
      '#suffix' => '</div>',
      '#sticky' => FALSE,
    );
    cache_set('delivery_cost_download_resource' . session_id(), $form['delivery_cost_table'], 'cache', time() + 60 * 10);
    $rr = l('Download', "resource_daily_wise_report/export", array('attributes' => array("target" => "_blank"), 'query' => array('dispatch_from_date' => $form_state['values']['dispatch_from_date'], 'dispatch_to_date' => $form_state['values']['dispatch_to_date'], 'resource_name' => $form_state['values']['resource_name'])));
    $form['download_mark'] = array(
      '#type' => 'markup',
      '#markup' => $rr,
    );
  }
  return $form;
}

function resource_day_wise_performance_form_submit($form, &$form_state, $export = 0) {
  global $user;
  $rows = array();
  $trip_ids = array();
  $dis_id = change_feeds_import_get_distributor_id($user->uid);
  $dispatch_from_date = $form_state['values']['dispatch_from_date'];
  $dispatch_to_date = $form_state['values']['dispatch_to_date'];
  $resource_name = $form_state['values']['resource_name'];
  $round_off_no_digits = 3;
  $round_off_no_digits_2 = 2;
  $lak = 100000;
  if (!empty($dispatch_from_date) && !empty($dispatch_to_date)) {
    $all_resources = get_all_active_resources_on_date_range(strtotime($dispatch_from_date), strtotime($dispatch_to_date));
    $absent_resource_details = get_leave_dates_of_resources(strtotime($dispatch_from_date), strtotime($dispatch_to_date));
    $resource_all_extra_working = get_resource_extra_working_days(strtotime($dispatch_from_date), strtotime($dispatch_to_date));
    $weekoff = get_resource_weekly_off_day();
    $dis_date = strtotime($dispatch_from_date);
    $year = date('Y', $dis_date);
    $month = date('m', $dis_date);
    $no_of_working_days = calculateWorkingDaysInMonth($year, $month, $weekoff);
    if (!empty($all_resources[$resource_name])) {
      $resource_list[$resource_name] = $all_resources[$resource_name];
    }
    else {
      $resource_list = $all_resources;
    }
    $resource_list = $resource_list;
  }
  $rows = array();
  $trip_created_days = array();
  if (!empty($resource_list)) {
    foreach ($resource_list as $resource_name => $resource_values) {
      // Resource Selected weekly off day
      if (!empty($resource_values['field_weekly_off_value'])) {
        $weekoff = $resource_values['field_weekly_off_value'];
      }
      $res_type = Ucfirst(strtolower($resource_values['resource_type']));
      $query = db_select('dispatch_planning_api_data', 'ad');
      $query->condition('ad.distributor_id', $dis_id, '=');
      if (strtolower(trim($resource_values['resource_type'])) == 'cashier') {
        $query->condition('fld_cashier.field_cashier_name_value', $resource_name);
        $query->groupBy('fld_cashier.field_cashier_name_value,ad.date');
      }
      if (strtolower(trim($resource_values['resource_type'])) == 'delivery boy') {
        // Fetch delivery boy nid for join with dispatch data
        $delivery_boy_nid = check_resource_exist($resource_values['resource_type'], $resource_name);
        if (!empty($delivery_boy_nid)) {
          $query->condition('delivery_boy.field_delivery_boy_nid', $delivery_boy_nid); //
          $query->groupBy('delivery_boy.field_delivery_boy_nid,ad.date');
        }
      }
      // $query->condition('ad.delivery_pending_flag', 1, '!=');
      $query->condition('ad.date', array($dispatch_from_date, $dispatch_to_date), 'BETWEEN');
      $query->join('node', 'n', 'n.title = ad.bill_number AND n.uid = ad.distributor_id');
      $query->condition('n.type', 'sales_register_data', '=');
      $query->condition('n.uid', $dis_id);
      $query->join('dispatch_planning_item_details', 'item', 'item.trip_bill_reference = ad.id');
      // Join with dispatch_data to check vrs status and return flage 
      $query->join('node', 'disp_data', 'disp_data.title = ad.runsheet_number');
      $query->condition('disp_data.type', 'dispatch_data');
      $query->condition('disp_data .uid', $dis_id);
      //$query->leftjoin('field_data_field_vrs_status', 'vs', 'vs.entity_id = disp_data.nid');
      $query->leftjoin('field_data_field_vehicle_no', 'vehicle_no', 'vehicle_no.entity_id = disp_data.nid');
      // VRS Screen Return flag = 1 and VRS status = 1, 3 Means VRS pending
      $query->leftjoin('field_data_field_cashier_name', 'fld_cashier', 'fld_cashier.entity_id = disp_data.nid');
      $query->leftjoin('field_data_field_delivery_boy', 'delivery_boy', 'delivery_boy.entity_id = disp_data.nid');
      $query->leftJoin('node', 'vehicle', 'vehicle.title = ad.vehicle');
      $query->condition('vehicle.type', 'vehicle_master');
      $query->condition('vehicle.uid', $dis_id);
      $query->addExpression("GROUP_CONCAT(DISTINCT ad.runsheet_number  separator ', ' )", 'trip_ids');
      $query->addExpression('ad.date', 'dispacth_date');
      $query->fields('ad', array('runsheet_number'));
      $query->fields('fld_cashier', array('field_cashier_name_value'));
      $query->addExpression('vehicle_no.field_vehicle_no_value', 'vehicle_number');
      $res = $query->execute()->fetchAll(PDO::FETCH_UNIQUE | PDO::FETCH_ASSOC);
      $vehicle_name_printed = 0;
      $input_resource_type = $resource_values['resource_type'];
      $resource_data = get_resource_cost_per_day($resource_name, $input_resource_type);
      if (!empty($res)) {
        foreach ($res as $key => $value) {
          $resource_cost = 0;
          if (!empty($value['dispacth_date'])) {
            $dis_date = strtotime($value['dispacth_date']);
            $year = date('Y', $dis_date);
            $month = date('m', $dis_date);
            $resource_no_of_working_days = calculateWorkingDaysInMonth($year, $month);
            $public_holidays = get_public_holiday($month, $year);
            $trip_created_days[$resource_name][$value['dispacth_date']] = $value['dispacth_date'];
          }
          if (!empty($resource_data[$resource_name]['cost_per_month'])) {
            $resource_cost = $resource_data[$resource_name]['cost_per_month'] / $resource_no_of_working_days;
          }
          //$previous_trip_status  = get_previous_trip_status($dis_date,$vehicle_number_value);
          $previous_trip_status = get_resource_previous_trip_status($dis_date, $resource_name, $resource_values['resource_type']);
          $resource_cost = round($resource_cost);
          $rows[$resource_name][$value['dispacth_date']] = array('resource_name' => '', 'resource_type' => '', 'date' => date('d-M-Y', $dis_date), 'active' => 'Active', 'trip_ids' => $value['trip_ids'], $previous_trip_status, 'cost' => $resource_cost, 'vehicle_number' => $value['vehicle_number'], 'leave_reason' => '');
        }
      }
      if (!empty($resource_all_extra_working[$resource_name])) {
        $trip_not_created_on_non_working_days = array_diff($resource_all_extra_working[$resource_name], $trip_created_days[$resource_name]);
        foreach ($trip_not_created_on_non_working_days as $value) {
          if (!empty($value)) {
            $date_array = explode('-', $value);
            $year = $date_array[0];
            $month = $date_array[1];
            $resource_cost = 0;
            $resource_no_of_working_days = calculateWorkingDaysInMonth($year, $month);
            if (!empty($resource_data[$resource_name]['cost_per_month'])) {
              $resource_cost = $resource_data[$resource_name]['cost_per_month'] / $resource_no_of_working_days;
            }
            $resource_cost = round($resource_cost);
            $previous_trip_status = get_resource_previous_trip_status(strtotime($value), $resource_name, $resource_values['resource_type']);
            //$rows[$vehicle_number_value][$value] =  array('vehicle_no'=>'','date'=>$value,'active'=>'Active','trip_ids'=>'Trip Not Created',$previous_trip_status,'cashier_names'=>'','dispatched_value'=>'','delivered_value'=>'','cost'=>round($vehicle_per_day_cost/$lak,$round_off_no_digits_2),'cost_vs_dispatched'=>'','cost_vs_delivered'=>'','leave_reason'=>'');
            $trip_not_created_reason = get_trip_not_created_reason($value, $vehicle_number = null, $resource_name = $resource_name, $resource_nid = null, $resource_type = $resource_values['field_resource_type_value']);
            if (empty($trip_not_created_reason)) {
              $trip_not_created_reason = t('Trip Not Created');
            }
            $rows[$resource_name][$value] = array('resource_name' => '', 'resource_type' => '', 'date' => $value, 'active' => 'Active', 'trip_ids' => $trip_not_created_reason, $previous_trip_status, 'cost' => $resource_cost, 'vehicle_number' => '', 'leave_reason' => '');
          }
        }
      }
      //Checking working days trip not created
      $ts1 = strtotime($dispatch_from_date);
      $ts2 = strtotime($dispatch_to_date);
      $contract_end_period = strtotime($resource_values['end_period']);
      $contract_start_period = strtotime($resource_values['start_period']);
      $currentdate = $ts1;
      $year1 = date('Y', $ts1);
      $year2 = date('Y', $ts2);
      $month1 = date('m', $ts1);
      $month2 = date('m', $ts2);
      $diff = (($year2 - $year1) * 12) + ($month2 - $month1);
      if ($diff > 0) {
        
      }
      while ($currentdate <= $ts2 && $currentdate <= $contract_end_period) {
        $day_of_month = date('Y-m-d', $currentdate);
        if (strtolower(date('l', $currentdate)) != strtolower($weekoff) && $contract_start_period <= $currentdate) {
          if (empty($absent_resource_details[$resource_name][$day_of_month]) && empty($trip_created_days[$resource_name][$day_of_month])) {
            $days[$resource_name][$day_of_month] = $day_of_month;
          }
        }
        $currentdate = strtotime('+1 day', $currentdate);
      }


      foreach ($days[$resource_name] as $datekey => $date_value) {
        if (!empty($date_value)) {
          $date_array = explode('-', $date_value);
          $year = $date_array[0];
          $month = $date_array[1];
          $resource_no_of_working_days = calculateWorkingDaysInMonth($year, $month);
          $resource_cost = 0;
          //$previous_trip_status  = get_previous_trip_status(strtotime($date_value),$vehicle_number_value);
          //print_r(get_resource_previous_trip_status($strtotime($date_value),$resource_name,$resource_values['resource_type']));
          $previous_trip_status = get_resource_previous_trip_status(strtotime($date_value), $resource_name, $resource_values['resource_type']);
          //$rows[$vehicle_number_value][$date_value] =  array('vehicle_no'=>'','date'=>$date_value,'active'=>'Active','trip_ids'=>'Trip Not Created',$previous_trip_status,'cashier_names'=>'','dispatched_value'=>'','delivered_value'=>'','cost'=>round($vehicle_per_day_cost/$lak,$round_off_no_digits_2),'cost_vs_dispatched'=>'','cost_vs_delivered'=>'','leave_reason'=>'');
          if (!empty($resource_data[$resource_name]['cost_per_month'])) {
            $resource_cost = $resource_data[$resource_name]['cost_per_month'] / $resource_no_of_working_days;
          }
          $resource_cost = round($resource_cost);
          $trip_not_created_reason = get_trip_not_created_reason($date_value, $vehicle_number = null, $resource_name = $resource_name, $resource_nid = null, $resource_type = $resource_values['field_resource_type_value']);
          if (empty($trip_not_created_reason)) {
            $trip_not_created_reason = t('Trip Not Created');
          }
          $rows[$resource_name][$date_value] = array('resource_name' => '', 'resource_type' => '', 'date' => $date_value, 'active' => 'Active', 'trip_ids' => $trip_not_created_reason, $previous_trip_status, 'cost' => $resource_cost, 'vehicle_number' => '', 'leave_reason' => '');
        }
      }


      if (!empty($absent_resource_details[$resource_name])) {
        foreach ($absent_resource_details[$resource_name] as $date_key => $date_value) {
          //$rows[$resource_name][$date_key] =  array('vehicle_no'=>'','date'=>$date_key,'active'=>'On Leave','trip_ids'=>'','cashier_names'=>'','dispatched_value'=>'','delivered_value'=>'','cost'=>'','cost_vs_dispatched'=>'','cost_vs_delivered'=>'','leave_reason'=>$date_value);
          $rows[$resource_name][$date_key] = array('resource_name' => '', 'resource_type' => '', 'date' => $date_key, 'active' => 'On Leave', 'trip_ids' => 'Trip Not Created', $previous_trip_status, 'cost' => 0, 'vehicle_number' => '', 'leave_reason' => $date_value);
        }
      }
      usort($rows[$resource_name], "sortFunction");
      $rows[$resource_name][0]['resource_name'] = $resource_name;
      $rows[$resource_name][0]['resource_type'] = $res_type;
      $datas[$resource_name] = $rows[$resource_name];
    }

    if (!empty($datas)) {
      $rows = array();
      foreach ($datas as $key => $value) {
        foreach ($value as &$val) {
          $val['date'] = date('d-M-Y', strtotime($val['date']));
          $rows[] = $val;
        }
      }
      $rows[] = array();
    }

    // print_r($rows);die;
  }
  if ($export == 1) {
    return $rows;
  }
  $form_state['delivery_cost_details'] = $rows;
  $form_state['rebuild'] = TRUE;
}

function resource_daily_wise_report_export() {
  $query_parameters = drupal_get_query_parameters();
  $file = "hul_resource_daily_wise_report_" . date('d-M-Y') . ".xls";
  $data = cache_get('delivery_cost_download_resource' . session_id());
  if (empty($data->data)) {
    if (!empty($query_parameters['dispatch_from_date']) && !empty($query_parameters['dispatch_to_date'])) {
      $form_state['values']['dispatch_from_date'] = $query_parameters['dispatch_from_date'];
      $form_state['values']['dispatch_to_date'] = $query_parameters['dispatch_to_date'];
      $form_state['values']['resource_name'] = $query_parameters['resource_name'];
      $options = resource_day_wise_performance_form_submit($form, $form_state, $export = 1);
    }
    if (!empty($options)) {
      $header = array(
        'Resource Name',
        'Resource Type',
        'Dispatch Date',
        'Active/On Leave',
        'Trip ID',
        'Previous Trip Status',
        'Cost',
        'Vehicle Number',
        'Reason if on Leave/Maintenance',
      );
      $delivery_cost_table = array(
        '#theme' => 'table',
        '#prefix' => '<div class="delivery_cost_table custom_middle_table">',
        '#tree' => TRUE,
        '#header' => $header,
        '#rows' => $options,
        '#attributes' => array(
          'class' => array('delivery_cost'),
          'id' => 'delivery_cost'
        ),
        '#suffix' => '</div>',
        '#sticky' => FALSE,
      );
      $table = drupal_render($delivery_cost_table);
    }
  }
  else {
    $table = drupal_render($data->data);
  }
  header("Content-type: application/vnd.ms-excel");
  header("Content-Disposition: attachment; filename=$file");
  echo $table;
}

function get_resource_previous_trip_status($dispatch_date, $resource_name, $resource_type) {
  $vrs_status = '';

  if (!empty($dispatch_date) && !empty($resource_name) && !empty($resource_type)) {
    global $user;
    $dis_id = change_feeds_import_get_distributor_id($user->uid);
    $previous_date = strtotime('-1 day', $dispatch_date);
    $previous_trip_ids = _get_resource_previous_trip_id($previous_date, $resource_name, $resource_type);
    if (!empty($previous_trip_ids)) {
      foreach ($previous_trip_ids as $previous_trip_id => $value) {
        $trip_detail = check_trip_id_exists_or_not($dis_id, $previous_trip_id);
        $status = $trip_detail->field_vrs_status_value;
        $field = field_info_field('field_vrs_status');
        $allowed_values = list_allowed_values($field);
        if (isset($allowed_values[$status])) {
          $vrs_status .= $previous_trip_id . '-' . $allowed_values[$status] . '<br>';
        }
      }
    }
  }
  return $vrs_status;
}

function _get_resource_previous_trip_id($dispatch_date, $resource_name, $resource_type) {
  $res = '';
  if (!empty($dispatch_date) && !empty($resource_name) && !empty($resource_type)) {
    global $user;
    $dispatch_date = date('Y-m-d', $dispatch_date);
    $weekoff = get_resource_weekly_off_day();
    $dis_id = change_feeds_import_get_distributor_id($user->uid);
    $query = db_select('node', 'n');
    $query->condition('n.type', 'dispatch_data');
    $query->condition('n.uid', $dis_id);
    $query->join('field_data_field_dispatch_date', 't', 't.entity_id = n.nid');
    $query->leftjoin('field_data_field_cashier_name', 'fld_cashier', 'fld_cashier.entity_id = n.nid');
    $query->leftjoin('field_data_field_delivery_boy', 'delivery_boy', 'delivery_boy.entity_id = n.nid');
    $query->condition('t.field_dispatch_date_value', $dispatch_date);
    if (strtolower(trim($resource_type)) == 'cashier') {
      $query->condition('fld_cashier.field_cashier_name_value', $resource_name);
    }
    if (strtolower(trim($resource_type)) == 'delivery boy') {
      // Fetch delivery boy nid for join with dispatch data
      $delivery_boy_nid = check_resource_exist($resource_type, $resource_name);
      if (!empty($delivery_boy_nid)) {
        $query->condition('delivery_boy.field_delivery_boy_nid', $delivery_boy_nid); //
      }
    }

    $query->fields('n', array('title'));
    $res = $query->execute()->fetchAll(PDO::FETCH_UNIQUE | PDO::FETCH_ASSOC);
    $dispatch_date_in_strtotime = strtotime($dispatch_date);
    if (strtolower(date('l', $dispatch_date_in_strtotime)) == strtolower($weekoff) && empty($res)) {
      $previous_date = strtotime('-1 day', $dispatch_date_in_strtotime);
      $res = _get_resource_previous_trip_id($previous_date, $resource_name, $resource_type);
    }
  }
  return $res;
}

function get_trip_not_created_reason($declration_date, $vehicle_number = null, $resource_name = null, $resource_nid = null, $resource_type = null) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);
  $res = '';
  if (!empty($declration_date)) {
    $declaration_date = strtotime($declration_date);
    $dis_id = change_feeds_import_get_distributor_id($user->uid);
    $query = db_select('vehicle_resource_daily_status', 'vrs');
    $query->condition('vrs.distributor_id', $dis_id);
    $query->condition('vrs.declaration_date', $declaration_date);
    if (!empty($vehicle_number)) {
      $query->condition('vrs.vehicle_number', $vehicle_number);
    }
    if (!empty($resource_type)) {
      $query->condition('vrs.resource_type', $resource_type);
    }
    if (!empty($resource_name)) {
      $query->condition('vrs.resource_name', $resource_name);
    }
    $query->fields('vrs', array('reason'));
    $res = $query->execute()->fetchField();
    if (!empty($res)) {
      $term_obj = taxonomy_term_load($res);
      if (!empty($term_obj))
        return $term_obj->name;
    }
  }
  return $res;
}
