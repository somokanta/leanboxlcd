<?php

/**
 * Files containing form for Van planning form
 */

/**
 * 
 * @param array $form
 * @param type $form_state
 * @return array
 */
function _van_planning_callback_form($form, &$form_state) {
  $form = array();
  $form['#prefix'] = '<div id="van-itemlist-div">';
  $form['#suffix'] = '</div>';
  $form['#attached']['js'][] = drupal_get_path('module', 'van_planning') . '/js/van_planning.js';
  $form['#attached']['css'][] = drupal_get_path('module', 'van_planning') . '/css/van_planning.css';
  $user_id = $GLOBALS['user']->uid;
  $rs_code = dispatch_planning_get_rscode_using_uid($user_id);
  $dis_id = change_feeds_import_get_distributor_id($user_id);
  if(empty($form_state['triggering_element'])){
    // means url is hit. First time on this page
    unset($_SESSION['park_stop']);
  }
  $form['dis_id'] = array(
    '#type' => 'value',
    '#value' => $dis_id,
  );
  $form['user_id'] = array(
    '#type' => 'value',
    '#value' => $user_id,
  );
  $form['total_peding_bills'] = array(
    '#type' => 'markup',
    '#prefix' => '<div id="link_wrapper">',
    '#suffix' => '</div>',
    '#markup' => l("View Pending Summary", "pending-summary", array('attributes' => array("target" => "_blank"))),
  );
  $form['todays_completed_van'] = array(
    '#type' => 'markup',
    '#prefix' => '<div id="template_wrapper2">',
    '#suffix' => '</div>',
    '#markup' => l("View Completed Vans", "completed-vans", array('attributes' => array("target" => "_blank"))),
  );
  $form['editable_vp'] = array(
    '#type' => 'fieldset',
    '#title' => t('Create Van Planning'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['editable_vp']['vp']['#prefix'] = '<div id="van-planning-screen">';
  $form['editable_vp']['vp']['#suffix'] = '</div>';
  // For reset in main table
  if ($form_state['clicked_button']['#name'] == 'pb_reset') {
    unset($form_state['input']['area_code']);
    unset($form_state['input']['van_pl_group']);
    unset($form_state['input']['pl_group']);
    unset($form_state['input']['pb_date']['date']);
    unset($form_state['input']['pb_range_op']);
    unset($form_state['input']['pb_value']);
    unset($form_state['input']['pb_weight']);
    unset($form_state['input']['pb_volume']);
    unset($form_state['input']['outlet_area_hook_grp']);
  }
  // For reset in modify van table- popup
  if($form_state['clicked_button']['#name'] == 'bill_wise_reset'){
    unset($form_state['input']['bill_area_code']);
    unset($form_state['input']['bill_pl_group']);
    unset($form_state['input']['bill_wise_range_op']);
    unset($form_state['input']['bill_wise_value']);
    unset($form_state['input']['bill_wise_weight']);
    unset($form_state['input']['bill_wise_volume']);
    unset($form_state['input']['bill_wise_rs_kg']);
    unset($form_state['input']['bill_wise_rs_cft']);
  }
  //For reset in add bill table- popup
  if($form_state['clicked_button']['#name'] == 'child_bill_wise_reset'){
    unset($form_state['input']['child_bill_area_code']);
    unset($form_state['input']['child_bill_pl_group']);
    unset($form_state['input']['child_bill_date']['date']);
    unset($form_state['input']['child_bill_wise_range_op']);
    unset($form_state['input']['child_bill_wise_value']);
    unset($form_state['input']['child_bill_wise_weight']);
    unset($form_state['input']['child_bill_wise_volume']);
    unset($form_state['input']['child_bill_wise_rs_kg']);
    unset($form_state['input']['child_bill_wise_rs_cft']);
  }
  $form['editable_vp']['vp']['outlet_area_hook_grp'] = array(
    '#type' => 'textfield',
    '#title' => t('Area Hook Group'),
    '#ajax' => array(
      'wrapper' => 'van-itemlist-div',
      'callback' => 'van_planning_filter_ajax_callback',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    '#autocomplete_path' => 'outlet-areahookgrp-master/autocomplete',
    '#prefix' => '<div class="form_row">',
  );
  $form['editable_vp']['vp']['area_code'] = array(
    '#type' => 'textfield',
    '#title' => t('Area Code'),
    '#ajax' => array(
      'wrapper' => 'van-itemlist-div',
      'callback' => 'van_planning_filter_ajax_callback',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    '#autocomplete_path' => 'areacode-master/autocomplete',
    '#prefix' => '<div class="form_row">',
  );
  $form['editable_vp']['vp']['pl_group'] = array(
    '#type' => 'textfield',
    '#title' => t('PL Group'),
    '#ajax' => array(
      'wrapper' => 'van-itemlist-div',
      'callback' => 'van_planning_filter_ajax_callback',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    '#autocomplete_path' => 'plgroup-master/autocomplete',
  );
  $form['editable_vp']['vp']['van_pl_group'] = array(
    '#type' => 'textfield',
    '#title' => t('Van Pl Group'),
    '#ajax' => array(
      'wrapper' => 'van-itemlist-div',
      'callback' => 'van_planning_filter_ajax_callback',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    '#autocomplete_path' => 'vanplgroup-master/autocomplete',
  );
  $form['editable_vp']['vp']['pb_date'] = array(
    '#type' => 'date_popup',
    '#date_label_position' => '',
    '#title' => t('Bill Date'),
    '#date_format' => 'Y-m-d',
    '#ajax' => array(
      'wrapper' => 'van-itemlist-div',
      'callback' => 'van_planning_filter_ajax_callback',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );

  $form['editable_vp']['vp']['pb_range_op'] = array(
    '#type' => 'select',
    '#title' => t('Bill Range Operation'),
    '#options' => array('' => '-select-', '=' => '=', '>' => '>', '<' => '<'),
    '#ajax' => array(
      'wrapper' => 'van-itemlist-div',
      'callback' => 'van_planning_filter_ajax_callback',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );
  $form['editable_vp']['vp']['pb_value'] = array(
    '#type' => 'textfield',
    '#title' => t('Bill Value'),
    '#ajax' => array(
      'wrapper' => 'van-itemlist-div',
      'callback' => 'van_planning_filter_ajax_callback',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );
  $form['editable_vp']['vp']['pb_weight'] = array(
    '#type' => 'textfield',
    '#title' => t('Weight'),
    '#ajax' => array(
      'wrapper' => 'van-itemlist-div',
      'callback' => 'van_planning_filter_ajax_callback',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );
  $form['editable_vp']['vp']['pb_volume'] = array(
    '#type' => 'textfield',
    '#title' => t('Volume'),
    '#ajax' => array(
      'wrapper' => 'van-itemlist-div',
      'callback' => 'van_planning_filter_ajax_callback',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    //'#suffix' => '</div>',// Because of reset
  );
  $form['editable_vp']['vp']['pb_reset'] = array(
    '#type' => 'submit',
    '#value' => 'Reset',
    '#name' => 'pb_reset',
    '#prefix' => '<div class="pb_reset_div">',
    '#ajax' => array(
      'wrapper' => 'van-itemlist-div',
      'callback' => 'van_planning_filter_ajax_callback',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    '#suffix' => '</div></div>',
    '#submit' => array('van_planning_filter_reset_ajax_submit'),
  );
  
  if ($form_state['move_to_enable'] == 1) {
    $form['editable_vp']['move_to_select'] = array(
      '#title' => t('Create New Group'),
      '#type' => 'textfield',
      '#id' => 'move_to_select',
      '#prefix' => '<div id = "show_in_popup"><div class="popup_overlay"></div><div class="popup_wrap"><div class="pop_content">',
    );
    $form['editable_vp']['move_to_select_error'] = array(
      '#type' => 'markup',
      '#title' => '',
      '#prefix' => '<div id="move_to_select_error">',
      '#suffix' => '</div>'
    );
    $form['editable_vp']['move_to_button'] = array(
      '#type' => 'submit',
      '#value' => 'Create',
      '#name' => 'create_van_button',
      '#ajax' => array(
        'wrapper' => 'van-itemlist-div',
        'callback' => 'show_move_to_ajax',
        'method' => 'replace',
        'effect' => 'fade',
      ),
      '#validate' => array('create_move_to_van_validate'),
      '#submit' => array('create_move_to_van_submit'),
    );
    $form['editable_vp']['van_close'] = array(
      '#type' => 'submit',
      '#value' => 'Close',
      '#name' => 'van_close',
      '#ajax' => array(
        'wrapper' => 'van-itemlist-div',
        'callback' => 'van_planning_filter_ajax_callback',
        'method' => 'replace',
        'effect' => 'fade',
      ),
      '#suffix' => '</div></div></div>',
      '#submit' => array('van_planning_filter_close_ajax_submit'),
    );
  }
  //move feature end
  $header_array = array(
    'area_code' => array('data' => t('Area Code')),
    'pl_group' => array('data' => t('Pl Group')),
    'bill_count' => array('data' => t('Bills')),
    'bill_value' => array('data' => t('Value (Rs)')),
    'sum_kg' => array('data' => t('Weight (KGs)')),
    'sum_cft' => array('data' => t('Volume (CFT)')),
    'rs_per_sum_kg' => array('data' => t('Rs/Kg')),
    'rs_per_sum_cft' => array('data' => t('Rs/Cft')),
    'bill_no' => array('data' => t('bill_no'), 'class' => array('bill_no_val')),
    'bill_group' => array('data' => t('bill_group'), 'class' => array('bill_group_data')),
  );

  $area_code = $form_state['input']['area_code'];
  $van_pl_group = $form_state['input']['van_pl_group'];
  $pl_group = $form_state['input']['pl_group'];
  $bill_date = strtotime($form_state['input']['pb_date']['date']);
  $bill_range_op = $form_state['input']['pb_range_op'];
  $bill_value = $form_state['input']['pb_value'];
  $weight = $form_state['input']['pb_weight'];
  $volume = $form_state['input']['pb_volume'];
  $outlet_area_hook_grp = $form_state['input']['outlet_area_hook_grp'];
  if (!empty($area_code) ||
    !empty($van_pl_group) ||
    (!empty($bill_range_op) && (!empty($bill_value) || !empty($weight) || !empty($volume))) ||
    !empty($pl_group) ||
    !empty($bill_date) ||
    !empty($bill_value) ||
    !empty($outlet_area_hook_grp) ||
    !empty($weight) ||
    !empty($volume)) {
    $input_data = TRUE;
  }
  if ($input_data) {
    $area_codes = 'no_input';
    if (!empty($area_code)) {
      $area_codes = _get_nearby_area_codes($area_code, $dis_id);
    }
    $bill_array = array();
    if (isset($form_state['bill_array'])) {
      $bill_array = $form_state['bill_array'];
    }
    $options = van_planning_ajax_submit($area_codes, $van_pl_group, $pl_group, $bill_date, $bill_range_op, $bill_value, $weight, $volume, $bill_array, $outlet_area_hook_grp);
    $form['editable_vp']['vp']['list_form_items'] = array(
      '#type' => 'tableselect',
      '#header' => $header_array,
      '#options' => $options,
      '#prefix' => '<div id="van-table-div">',
      '#suffix' => '</div>',
      '#empty' => t('No data available.'),
      '#multiple' => TRUE,
      '#js_select' => TRUE, // give Select All checkbox
    );
  }
  if (!empty($options)) {
    $form['editable_vp']['vp']['move_to'] = array(
      '#type' => 'submit',
      '#value' => 'Move To',
      '#name' => 'move_button',
      '#ajax' => array(
        'wrapper' => 'van-itemlist-div',
        'callback' => 'show_move_to_ajax', // simple ajax callback
        'method' => 'replace',
        'effect' => 'fade',
      ),
      '#submit' => array('show_move_to_form_submit'),
    );
  }
  if (isset($form_state['new_dispatch_planning']) && !empty($form_state['new_dispatch_planning'])) {
    // New group created element 
    $van_header_array = array(
      'pl_group' => array('data' => t('Pl Group')),
      'bill_count' => array('data' => t('Bills')),
      'sku_count' => array('data' => t('Sku Count')),
      'bill_value' => array('data' => t('Value (Rs)')),
      'cld' => array('data' => t('CLD Lines')),
      'loose' => array('data' => t('Loose Lines')),
      'sum_kg' => array('data' => t('Weight (KGs)')),
      'sum_cft' => array('data' => t('Volume (CFT)')),
      'rs_per_sum_kg' => array('data' => t('Rs/Kg')),
      'rs_per_sum_cft' => array('data' => t('Rs/Cft')),
      'area_code' => array('data' => t('Area Code'), 'class' => array('area_code_val')),
      'bill_no' => array('data' => t('bill_no'), 'class' => array('bill_no_val')),
    );
    $form['editable_vp']['vp_group']['#prefix'] = '<div id="van-planning-group">';
    $form['editable_vp']['vp_group']['#suffix'] = '</div>';

    $val['total']['bill_count'] = $val['total']['bill_value'] = $val['total']['sum_kg'] = $val['total']['sum_cft'] = 0;
    foreach ($form_state['new_dispatch_planning'] as $key => $val) {
      $total_bill_array = $total_sales_nid_array = array();
      $val1['total'][$key]['bill_count'] = $val1['total'][$key]['bill_value'] = 0;
      $val1['total'][$key]['sum_kg'] = $val1['total'][$key]['sum_cft'];
      $for_total_combined_area_codes = array();
      foreach ($val as $group => $revised_bill) {
        $bill_value = $val[$group]['bill_value'];
        $val[$group]['bill_value'] = number_format($bill_value);
        $total_sales_nid_array1 = array();
        $combined_area_codes = array();
        $val_bill_numbers = json_decode($revised_bill['bill_no']['data-value'], 1);
        foreach ($val_bill_numbers as $nid => $bill_array) {
          $total_sales_nid_array1[$nid] = $nid;
          $combined_area_codes[$bill_array['area_code']] = $bill_array['area_code']; //for area code
          $for_total_combined_area_codes[$bill_array['area_code']] = $bill_array['area_code']; //for area code
        }
        $combined_area_codes = implode(",", $combined_area_codes);
        $val[$group]['area_code'] = $combined_area_codes;
        $cld_loose_array = _get_skus_cld_loose_count($total_sales_nid_array1, $dis_id);
        $val[$group]['cld'] = $cld_loose_array['cld'];
        $val[$group]['loose'] = $cld_loose_array['loose'];
        $val[$group]['sku_count'] = $cld_loose_array['sku_count'];

        $total_bill_array += $val_bill_numbers;
        $total_sales_nid_array += $total_sales_nid_array1;
        $val['total']['pl_group'] = '<b>TOTAL</b>';
        $val['total']['bill_count'] += $val[$group]['bill_count'];
        $val['total']['bill_value'] += $bill_value;
        $val['total']['sum_kg'] += $val[$group]['sum_kg'];
        $val['total']['sum_cft'] += $val[$group]['sum_cft'];

        $val1['total'][$key]['bill_count'] += $val[$group]['bill_count'];
        $val1['total'][$key]['bill_value'] += $bill_value;
        $val1['total'][$key]['sum_kg'] += $val[$group]['sum_kg'];
        $val1['total'][$key]['sum_cft'] += $val[$group]['sum_cft'];
      }
      // For total area_code
      $for_total_combined_area_codes = implode(",", $for_total_combined_area_codes);
      $val['total']['area_code'] = $for_total_combined_area_codes;
      // for rs/kg and rs/cf
      $val['total']['rs_per_sum_kg'] = round($val['total']['bill_value'] / $val['total']['sum_kg'], 2);
      $val['total']['rs_per_sum_cft'] = round($val['total']['bill_value'] / $val['total']['sum_cft'], 2);

      $val['total']['bill_value'] = number_format($val['total']['bill_value']);
      // for rs/kg and rs/cf in van wise single low. submmit van planning
      $val1['total'][$key]['rs_per_sum_kg'] = round($val1['total'][$key]['bill_value'] / $val1['total'][$key]['sum_kg'], 2);
      $val1['total'][$key]['rs_per_sum_cft'] = round($val1['total'][$key]['bill_value'] / $val1['total'][$key]['sum_cft'], 2);

      $val1['total'][$key]['bill_value'] = number_format($val1['total'][$key]['bill_value']);
      // pass third variable TRUE to fetch lat and long for each bill
      $cld_loose_array = _get_skus_cld_loose_count($total_sales_nid_array, $dis_id, TRUE);

      // all latitude and longitude van wise
      $vanwise_all_latlong_array = $cld_loose_array['lat_long'];
      $total_dist = $total_time = $total_del_boy = '';
      $bill_sequencing_array = array();// intialize because one van value can't go in another van

      if ($form_state['clicked_button']['#name'] == 'check_seq') {

        //abhi test
        $python_bills_nid = array_keys($vanwise_all_latlong_array);
        $response_data = parkstop_calculation_api($python_bills_nid, $dis_id, $key);
        //watchdog("$key", '<pre>' . print_r($python_response, TRUE) . '</pre>');
        //abhi test end
/*
        // call lat long sequencing api here
        $Locations['visits'] = array();
        foreach ($vanwise_all_latlong_array as $sales_nid => $lat_long_val) {

          $Locations['visits'][$sales_nid]['location']['name'] = (string) $sales_nid;
          $Locations['visits'][$sales_nid]['location']['lat'] = $lat_long_val['latitude'];
          $Locations['visits'][$sales_nid]['location']['lng'] = $lat_long_val['longitude'];
          $Locations['visits'][$sales_nid]['duration'] = 7; //hardcode 7 min stop on every outlet 
        }

        $conf_nid = check_node_title_exists($dis_id, 'van_stop_master');
        $conf_node = node_load($conf_nid);
        $origin_lat = $conf_node->field_distributor_latitude[LANGUAGE_NONE][0]['value'];
        $origin_long = $conf_node->field_distributor_longitude[LANGUAGE_NONE][0]['value'];
        $Locations['fleet']['origin']['start_location'] = array(
          "id" => 'origin',
          "name" => 'origin',
          "lat" => $origin_lat,
          "lng" => $origin_long
        );

        $speed = 0.3;
        $Locations['fleet']['origin']['speed'] = $speed; // speed of the driver


        $Locations['options']['shortest_distance'] = true;
        $Locations['options']['polylines'] = true;
        $Locations_data = json_encode($Locations);
        $authorization = 'bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJfaWQiOiI1ODgzOWY4ZjYyZmQyMzkzMGE4NjQwYjEiLCJpYXQiOjE1MDQ2Nzg4NTl9.AzJLVp6HDk6lbXCWxI1T8Y7US6R0panNjkY5B-pkdqg';
        $request_headers = array('Content-Type' => 'application/json', 'Authorization' => $authorization);
        $roitufic_api_url = variable_get('api_routific_url', 'https://api.routific.com/v1/vrp');
        $routific_options = array('headers' => $request_headers, 'method' => 'POST', 'data' => $Locations_data);
        //$response_data = drupal_http_request($roitufic_api_url, $routific_options);
        $response_data = json_decode($response_data->data);
 */
        if (!empty($response_data->error)) {
          form_set_error('form', 'API Error' . $response_data->error);
          //emailer for notification start
            global $base_url;
            $params['subject'] = "Van Planning API Error on $base_url for van: $key";
            $params['body'] = $response_data->error;
            $mail_to = variable_get('parkstop_api_error_email',"abhishek.r@iksula.com,kusha@leanbox.in");
            $from = "MIS.HELPDESK@leanbox.in";
            drupal_mail('adhoc_task', 'bill_date_missing', $mail_to, LANGUAGE_NONE, $params, $from);
            //emailer for notification end
        }
        $form_state['tim_dist_response'][$key] = $response_data;
      }
      $response_data = '';
      if (isset($form_state['tim_dist_response'][$key])) {
        $response_data = $form_state['tim_dist_response'][$key];
      }
      if ($response_data->status_message === 'OK') {
        //$response_array = $response_data->solution->origin;
        $bill_sequencing_array = get_drs_bill_sequence_by_parkstop(json_decode($response_data->data), $key);

        //Minus the first stop time and distance
        //$total_time = $response_data->total_working_time - (strtotime($response_array[1]->arrival_time) - strtotime($response_array[0]->arrival_time)) / 60;
        //$total_dist = round($response_data->total_distance - ($response_array[1]->distance / 1000), 1); // its in meter show convert in km and then minus
        
        $total_time = $_SESSION['park_stop'][$key]['time'];
        $total_dist = $_SESSION['park_stop'][$key]['dist'];//newly
        $total_del_boy = $_SESSION['park_stop'][$key]['del_boy'];//newly
        
        /*
        foreach ($response_array as $k => $v) {
          if ($v->location_name !== 'origin') {
            $bill_sequencing_array[$v->location_name]['sequence'] = $i;
            //start
            if ($k == 0) {
              $travel_time = 0;
            }
            else if ($k == 1) {
              $travel_time = strtotime($response_array[$k]->arrival_time) - strtotime($response_array[$k - 1]->arrival_time);
            }
            else {
              $travel_time = strtotime($response_array[$k]->arrival_time) - strtotime($response_array[$k - 1]->finish_time);
            }
            $travel_time = $travel_time / 60; // min
            $bill_sequencing_array[$v->location_name]['prev_travel_time'] = $travel_time;
            $bill_sequencing_array[$v->location_name]['prev_distance'] = round(($v->distance) / 1000, 2);
            //end
            $i++;
          }
        }*/
      }

      if (isset($bill_sequencing_array) && !empty($bill_sequencing_array)) {
        $count = count($bill_sequencing_array);
        foreach ($vanwise_all_latlong_array as $bill => $seq) {
          if (isset($bill_sequencing_array[$bill])) {
            $vanwise_all_latlong_array[$bill]['sequence'] = $bill_sequencing_array[$bill]['sequence'];
            $total_bill_array[$bill]['sequence'] = $bill_sequencing_array[$bill]['sequence'];
            $total_bill_array[$bill]['park_seq'] = $bill_sequencing_array[$bill]['park_seq'];
            //$total_bill_array[$bill]['prev_travel_time'] = $bill_sequencing_array[$bill]['prev_travel_time'];
            //$total_bill_array[$bill]['prev_distance'] = $bill_sequencing_array[$bill]['prev_distance'];
          }
          else {
            $vanwise_all_latlong_array[$bill]['sequence'] = ++$count;
            $total_bill_array[$bill]['sequence'] = $count;
            $total_bill_array[$bill]['park_seq'] = $count;
          }
        }
      }

      $vanwise_all_latlong_array = _get_header_sort_data($vanwise_all_latlong_array, 'sequence');
      if (isset($_SESSION['van_wise_data'][$key . '-' . $user_id])) {
        unset($_SESSION['van_wise_data'][$key . '-' . $user_id]);
      }
      $_SESSION['van_wise_data'][$key . '-' . $user_id] = json_encode($vanwise_all_latlong_array);

      $val['total']['cld'] = $cld_loose_array['cld'];
      $val['total']['loose'] = $cld_loose_array['loose'];
      $val['total']['sku_count'] = $cld_loose_array['sku_count'];
      $val['total']['bill_no']['data'] = '';
      $val['total']['bill_no']['class'] = array('bill_no_val');
      $val['total']['bill_no']['data-value'] = '';

      $val1['total'][$key]['cld'] = $cld_loose_array['cld'];
      $val1['total'][$key]['loose'] = $cld_loose_array['loose'];
      $val1['total'][$key]['bill_wise_loose'] = $cld_loose_array['bill_wise_loose'];
      $val1['total'][$key]['cld_value'] = $cld_loose_array['cld_value'];
      $val1['total'][$key]['loose_value'] = $cld_loose_array['loose_value'];
      $val1['total'][$key]['time'] = $total_time;
      $val1['total'][$key]['dist'] = $total_dist;
      $val1['total'][$key]['del_boy'] = $total_del_boy;
      $val1['total'][$key]['sku_count'] = $cld_loose_array['sku_count'];
      $val1['total'][$key]['area_code']['data'] = '';
      $val1['total'][$key]['area_code']['class'] = array('area_code_val');
      $val1['total'][$key]['area_code']['data-value'] = '';
      $val1['total'][$key]['bill_no']['data'] = '';
      $val1['total'][$key]['bill_no']['class'] = array('bill_no_val');
      $val1['total'][$key]['bill_no']['data-value'] = json_encode($total_bill_array);
      $val1['total'][$key]['van_id'] = $key;

      // prepare data for map
      $map_address_url = 'van-map-planning/' . $key;
      $val1['total'][$key]['map_data']['data'] = l(t('View Map'), $map_address_url, array('attributes' => array('target' => '_blank')));
      $val1['total'][$key]['parkstop_link']['data'] = l(t('View Parkstop'), 'park-stop-plotting/' . $key, array('attributes' => array('target' => '_blank')));

      // create a rows for each van total row to use in final_van_planning table
      $vp_final_rows[$key] = $val1['total'][$key];

      $form_state['time_dist'][$key]['time'] = $total_time;
      $form_state['time_dist'][$key]['dist'] = $total_dist;
      $form_state['time_dist'][$key]['del_boy'] = $total_del_boy;

      $form['editable_vp']['vp_group'][$key]['van_edit_' . $key] = array(
        '#type' => 'tableselect',
        '#name' => 'van_edit_' . $key,
        '#empty' => t('No data available.'),
        '#multiple' => FALSE,
        '#js_select' => FALSE, //Dont give Select All checkbox
        //'<div class = "seq-time-dist"><div class = "total-time">In Area Time (Min) - ' . $total_time . '</div><div class = "total-time">In Area KMs - ' . $total_dist . '</div></div>'
        '#prefix' => '<div class = "van-edit"><h3>' . $key . '</h3>Area Codes: ' . $for_total_combined_area_codes,
        '#header' => $van_header_array,
        '#options' => $val,
      );

      $form['editable_vp']['vp_group'][$key]['modify_van'] = array(
        '#type' => 'submit',
        '#value' => 'Modify ' . $key,
        '#name' => 'modify_van',
        '#ajax' => array(
          'wrapper' => 'van-itemlist-div',
          'callback' => 'van_planning_ajax_callback', // simple ajax callback
          'method' => 'replace',
          'effect' => 'fade',
        ),
        '#suffix' => '</div>',
        '#submit' => array('van_planning_modify_van_ajax_submit'),
      );
    }

//    $form['editable_vp']['vp_group'][$key]['check_seq'] = array(
//      '#type' => 'submit',
//      '#value' => 'Calculate Time-Distance',
//      '#name' => 'check_seq',
//      '#ajax' => array(
//        'wrapper' => 'van-itemlist-div',
//        'callback' => 'van_planning_ajax_callback', // eimple ajax callback
//        'method' => 'replace',
//        'effect' => 'fade',
//      ),
//      '#submit' => array('van_planning_filter_ajax_submit'),
//    );

    // New group created element 
    $bill_header_array = array(
      'area_code' => array('data' => t('Area Code')),
      'pl_group' => array('data' => t('Pl Group')),
      'bill_no' => array('data' => t('Bill no')),
      'bill_value' => array('data' => t('Value-RsK')),
      'sum_kg' => array('data' => t('Weight')),
      'sum_cft' => array('data' => t('Volume')),
      'rs_per_sum_kg' => array('data' => t('Rs/Kg')),
      'rs_per_sum_cft' => array('data' => t('Rs/Cft')),
      'party_code_val'=>array('data'=>t('Party Code')),//NEWFIELD
      'party_name_val'=>array('data'=>t('Party Name'))
    );

    // find selected van no in popup
    $selected_modify_bill = '';
    if (isset($form_state['selected_van_grp'])) {
      $selected_modify_bill = $form_state['selected_van_grp'];
    }

    if ($form_state['clicked_button']['#name'] == 'close_bill') {
      unset($form_state['clicked_add_other_bills']);
    }

    if (isset($form_state['selected_modify_bill_array'][$selected_modify_bill]) && !empty($form_state['selected_modify_bill_array'][$selected_modify_bill])) {

      // show result in pop up form
      if ((preg_match('/child_bill_/', $form_state['triggering_element']['#name'])) || (preg_match('/bill_/', $form_state['triggering_element']['#name'])) || $form_state['clicked_button']['#name'] == 'add_other_bills' || $form_state['clicked_button']['#name'] == 'modify_van' || $form_state['clicked_button']['#name'] == 'remove_bills' || $form_state['clicked_button']['#name'] == 'unassigned_bills_add') {
        if ($form_state['clicked_button']['#name'] == 'add_other_bills') {
          $form_state['clicked_add_other_bills'] = 1;
        }

        // filters below
        $form['editable_vp']['vp_pop']['#prefix'] = '<div class="van-planning-popup-screen"><div class="popup_overlay"></div><div class="popup_wrap"><div class="pop_content"><div class="popup_border">';
        $form['editable_vp']['vp_pop']['#suffix'] = '</div></div></div></div>';

        $form['editable_vp']['vp_pop']['close_bill'] = array(
          '#type' => 'submit',
          '#value' => 'X',
          '#name' => 'close_bill',
          '#ajax' => array(
            'wrapper' => 'van-itemlist-div',
            'callback' => 'van_planning_ajax_callback', // simple ajax callback
            'method' => 'replace',
            'effect' => 'fade',
          ),
          '#attributes' => array('style' => 'float:right'),
          '#submit' => array('van_planning_filter_ajax_submit'),
        );
        $form['editable_vp']['vp_pop'][$selected_modify_bill]['bill_area_code'] = array(
          '#type' => 'textfield',
          '#title' => t('Area Code'),
          '#ajax' => array(
            'wrapper' => 'van-itemlist-div',
            'callback' => 'van_planning_filter_ajax_callback',
            'method' => 'replace',
            'effect' => 'fade',
          ),
          '#autocomplete_path' => 'areacode-master/autocomplete',
          '#name' => 'bill_area_code',
          '#prefix' => '<div class="form_row">',
        );
        $form['editable_vp']['vp_pop'][$selected_modify_bill]['bill_pl_group'] = array(
          '#type' => 'textfield',
          '#title' => t('PL Group'),
          '#ajax' => array(
            'wrapper' => 'van-itemlist-div',
            'callback' => 'van_planning_filter_ajax_callback',
            'method' => 'replace',
            'effect' => 'fade',
          ),
          '#autocomplete_path' => 'plgroup-master/autocomplete',
          '#name' => 'bill_pl_group',
        );

        $form['editable_vp']['vp_pop'][$selected_modify_bill]['bill_wise_range_op'] = array(
          '#type' => 'select',
          '#title' => t('Bill Range Operation'),
          '#options' => array('' => '-select-', '=' => '=', '>' => '>', '<' => '<'),
          '#ajax' => array(
            'wrapper' => 'van-itemlist-div',
            'callback' => 'van_planning_filter_ajax_callback',
            'method' => 'replace',
            'effect' => 'fade',
          ),
          '#name' => 'bill_wise_range_op',
        );
        $form['editable_vp']['vp_pop'][$selected_modify_bill]['bill_wise_value'] = array(
          '#type' => 'textfield',
          '#title' => t('Bill Value'),
          '#ajax' => array(
            'wrapper' => 'van-itemlist-div',
            'callback' => 'van_planning_filter_ajax_callback',
            'method' => 'replace',
            'effect' => 'fade',
          ),
          '#name' => 'bill_wise_value',
        );
        $form['editable_vp']['vp_pop'][$selected_modify_bill]['bill_wise_weight'] = array(
          '#type' => 'textfield',
          '#title' => t('Weight'),
          '#ajax' => array(
            'wrapper' => 'van-itemlist-div',
            'callback' => 'van_planning_filter_ajax_callback',
            'method' => 'replace',
            'effect' => 'fade',
          ),
          '#name' => 'bill_wise_weight',
        );
        $form['editable_vp']['vp_pop'][$selected_modify_bill]['bill_wise_volume'] = array(
          '#type' => 'textfield',
          '#title' => t('Volume'),
          '#ajax' => array(
            'wrapper' => 'van-itemlist-div',
            'callback' => 'van_planning_filter_ajax_callback',
            'method' => 'replace',
            'effect' => 'fade',
          ),
          '#name' => 'bill_wise_volume',
        );
        $form['editable_vp']['vp_pop'][$selected_modify_bill]['bill_wise_rs_kg'] = array(
          '#type' => 'textfield',
          '#title' => t('RS/Kg'),
          '#ajax' => array(
            'wrapper' => 'van-itemlist-div',
            'callback' => 'van_planning_filter_ajax_callback',
            'method' => 'replace',
            'effect' => 'fade',
          ),
          '#name' => 'bill_wise_rs_kg',
        );
        $form['editable_vp']['vp_pop'][$selected_modify_bill]['bill_wise_rs_cft'] = array(
          '#type' => 'textfield',
          '#title' => t('RS/Cft'),
          '#ajax' => array(
            'wrapper' => 'van-itemlist-div',
            'callback' => 'van_planning_filter_ajax_callback',
            'method' => 'replace',
            'effect' => 'fade',
          ),
          '#name' => 'bill_wise_rs_cft',
          //'#suffix' => '</div>',// Because of reset 
        );
        $form['editable_vp']['vp_pop'][$selected_modify_bill]['bill_wise_reset'] = array(
          '#type' => 'submit',
          '#value' => 'Reset',
          '#name' => 'bill_wise_reset',
          '#prefix' => '<div class="pb_reset_div">',
          '#ajax' => array(
            'wrapper' => 'van-itemlist-div',
            'callback' => 'van_planning_filter_ajax_callback',
            'method' => 'replace',
            'effect' => 'fade',
          ),
          '#suffix' => '</div></div>',
          '#submit' => array('van_planning_filter_reset_ajax_submit'),
        );

        // check if any filter selected 
        $bill_area_code = $form_state['input']['bill_area_code'];
        $bill_pl_group = $form_state['input']['bill_pl_group'];
        $bill_wise_range_op = $form_state['input']['bill_wise_range_op'];
        $bill_wise_value = $form_state['input']['bill_wise_value'];
        $bill_wise_weight = $form_state['input']['bill_wise_weight'];
        $bill_wise_volume = $form_state['input']['bill_wise_volume'];
        $bill_wise_rs_kg = $form_state['input']['bill_wise_rs_kg'];
        $bill_wise_rs_cft = $form_state['input']['bill_wise_rs_cft'];
        if (!empty($bill_area_code) ||
          !empty($bill_pl_group) ||
          !empty($bill_wise_range_op) ||
          !empty($bill_wise_value) ||
          !empty($bill_wise_weight) ||
          !empty($bill_wise_volume) ||
          !empty($bill_wise_rs_kg) ||
          !empty($bill_wise_rs_cft)) {
          $bill_wise_input_data = TRUE;
        }

        $all_bill_options = $form_state['selected_modify_bill_array'][$selected_modify_bill];

        foreach ($all_bill_options as $key => $val) {
          $all_bill_options[$key]['bill_value'] = number_format($val['bill_value']);
          $bill_wise_array[] = $val['bill_no'];
        }
        if ($bill_wise_input_data) {

          $all_bill_options = _get_van_planning_bill_wise_data($bill_area_code, $bill_pl_group, $bill_wise_range_op, $bill_wise_value, $bill_wise_weight, $bill_wise_volume, $bill_wise_rs_kg, $bill_wise_rs_cft, $bill_wise_array);
        }

        // show result in pop up form
        $form['editable_vp']['vp_pop'][$selected_modify_bill]['modify_van_bills'] = array(
          '#type' => 'tableselect',
          '#empty' => t('No data available.'),
          '#multiple' => TRUE,
          '#js_select' => TRUE, //Given on 24July
          '#prefix' => '<div class = "bill-edit"><h3>Bill Wise data</h3>',
          '#suffix' => '</div>',
          '#header' => $bill_header_array,
          '#options' => $all_bill_options,
        );

        $form['editable_vp']['vp_pop']['remove_bills'] = array(
          '#type' => 'submit',
          '#value' => 'Remove Bills',
          '#name' => 'remove_bills',
          '#ajax' => array(
            'wrapper' => 'van-itemlist-div',
            'callback' => 'van_planning_ajax_callback', // simple ajax callback
            'method' => 'replace',
            'effect' => 'fade',
          ),
          '#submit' => array('van_planning_remove_bill_ajax_submit'),
        );
        $form['editable_vp']['vp_pop']['add_other_bills'] = array(
          '#type' => 'submit',
          '#value' => 'Add Bills',
          '#name' => 'add_other_bills',
          '#ajax' => array(
            'wrapper' => 'van-itemlist-div',
            'callback' => 'van_planning_ajax_callback_scroll', // simple ajax callback
            'method' => 'replace',
            'effect' => 'fade',
          ),
          '#id' => 'child_element_visibility',
          '#submit' => array('van_planning_filter_ajax_submit'),
        );

        if (isset($form_state['clicked_add_other_bills'])) {
// others bills
          $form['editable_vp']['vp_pop']['child_bills']['#prefix'] = '<div class="child-popup-screen"><div class="child_overlay"></div><div class="child_popup_wrap"><div class="child_pop_content"><div class="popup_border">';
          $form['editable_vp']['vp_pop']['child_bills']['#suffix'] = '</div></div></div></div>';
          $form['editable_vp']['vp_pop']['child_bills'][$selected_modify_bill]['child_bill_area_code'] = array(
            '#type' => 'textfield',
            '#title' => t('Area Code'),
            '#ajax' => array(
              'wrapper' => 'van-itemlist-div',
              'callback' => 'van_planning_ajax_callback_scroll',
              'method' => 'replace',
              'effect' => 'fade',
            ),
            '#autocomplete_path' => 'areacode-master/autocomplete',
            '#name' => 'child_bill_area_code',
            '#prefix' => '<div class="form_row">',
          );
          $form['editable_vp']['vp_pop']['child_bills'][$selected_modify_bill]['child_bill_pl_group'] = array(
            '#type' => 'textfield',
            '#title' => t('PL Group'),
            '#ajax' => array(
              'wrapper' => 'van-itemlist-div',
              'callback' => 'van_planning_ajax_callback_scroll',
              'method' => 'replace',
              'effect' => 'fade',
            ),
            '#autocomplete_path' => 'plgroup-master/autocomplete',
            '#name' => 'child_bill_pl_group',
          );
          $form['editable_vp']['vp_pop']['child_bills'][$selected_modify_bill]['child_bill_date'] = array(
            '#type' => 'date_popup',
            '#date_label_position' => '',
            '#title' => t('Bill Date'),
            '#date_format' => 'Y-m-d',
            '#ajax' => array(
              'wrapper' => 'van-itemlist-div',
              'callback' => 'van_planning_ajax_callback_scroll',
              'method' => 'replace',
              'effect' => 'fade',
            ),
            '#name' => 'child_bill_date',
          );
          $form['editable_vp']['vp_pop']['child_bills'][$selected_modify_bill]['child_bill_wise_range_op'] = array(
            '#type' => 'select',
            '#title' => t('Bill Range Operation'),
            '#options' => array('' => '-select-', '=' => '=', '>' => '>', '<' => '<'),
            '#ajax' => array(
              'wrapper' => 'van-itemlist-div',
              'callback' => 'van_planning_ajax_callback_scroll',
              'method' => 'replace',
              'effect' => 'fade',
            ),
            '#name' => 'child_bill_wise_range_op',
          );
          $form['editable_vp']['vp_pop']['child_bills'][$selected_modify_bill]['child_bill_wise_value'] = array(
            '#type' => 'textfield',
            '#title' => t('Bill Value'),
            '#ajax' => array(
              'wrapper' => 'van-itemlist-div',
              'callback' => 'van_planning_ajax_callback_scroll',
              'method' => 'replace',
              'effect' => 'fade',
            ),
            '#name' => 'child_bill_wise_value',
          );
          $form['editable_vp']['vp_pop']['child_bills'][$selected_modify_bill]['child_bill_wise_weight'] = array(
            '#type' => 'textfield',
            '#title' => t('Weight'),
            '#ajax' => array(
              'wrapper' => 'van-itemlist-div',
              'callback' => 'van_planning_ajax_callback_scroll',
              'method' => 'replace',
              'effect' => 'fade',
            ),
            '#name' => 'child_bill_wise_weight',
          );
          $form['editable_vp']['vp_pop']['child_bills'][$selected_modify_bill]['child_bill_wise_volume'] = array(
            '#type' => 'textfield',
            '#title' => t('Volume'),
            '#ajax' => array(
              'wrapper' => 'van-itemlist-div',
              'callback' => 'van_planning_ajax_callback_scroll',
              'method' => 'replace',
              'effect' => 'fade',
            ),
            '#name' => 'child_bill_wise_volume',
          );
          $form['editable_vp']['vp_pop']['child_bills'][$selected_modify_bill]['child_bill_wise_rs_kg'] = array(
            '#type' => 'textfield',
            '#title' => t('RS/Kg'),
            '#ajax' => array(
              'wrapper' => 'van-itemlist-div',
              'callback' => 'van_planning_ajax_callback_scroll',
              'method' => 'replace',
              'effect' => 'fade',
            ),
            '#name' => 'child_bill_wise_rs_kg',
          );
          $form['editable_vp']['vp_pop']['child_bills'][$selected_modify_bill]['child_bill_wise_rs_cft'] = array(
            '#type' => 'textfield',
            '#title' => t('RS/Cft'),
            '#ajax' => array(
              'wrapper' => 'van-itemlist-div',
              'callback' => 'van_planning_ajax_callback_scroll',
              'method' => 'replace',
              'effect' => 'fade',
            ),
            '#name' => 'child_bill_wise_rs_cft',
            //'#suffix' => '</div>',// Because of reset
          );
          $form['editable_vp']['vp_pop']['child_bills'][$selected_modify_bill]['child_bill_wise_reset'] = array(
            '#type' => 'submit',
            '#value' => 'Reset',
            '#name' => 'child_bill_wise_reset',
            '#prefix' => '<div class="pb_reset_div">',
            '#ajax' => array(
              'wrapper' => 'van-itemlist-div',
              'callback' => 'van_planning_ajax_callback_scroll',
              'method' => 'replace',
              'effect' => 'fade',
            ),
            '#suffix' => '</div></div>',
            '#submit' => array('van_planning_filter_reset_ajax_submit'),
          );
          $bill_area_code = $form_state['input']['child_bill_area_code'];
          $bill_pl_group = $form_state['input']['child_bill_pl_group'];

          $child_bill_wise_date = $form_state['input']['child_bill_date']['date'];
          $bill_wise_range_op = $form_state['input']['child_bill_wise_range_op'];

          $bill_wise_value = $form_state['input']['child_bill_wise_value'];
          $bill_wise_weight = $form_state['input']['child_bill_wise_weight'];
          $bill_wise_volume = $form_state['input']['child_bill_wise_volume'];
          $bill_wise_rs_kg = $form_state['input']['child_bill_wise_rs_kg'];
          $bill_wise_rs_cft = $form_state['input']['child_bill_wise_rs_cft'];
          $bill_wise_input_data = FALSE;
          if (!empty($bill_area_code) ||
            !empty($bill_pl_group) ||
            !empty($child_bill_wise_date) ||
            !empty($bill_wise_range_op) ||
            !empty($bill_wise_value) ||
            !empty($bill_wise_weight) ||
            !empty($bill_wise_volume) ||
            !empty($bill_wise_rs_kg) ||
            !empty($bill_wise_rs_cft)) {
            $bill_wise_input_data = TRUE;
          }
          
          // Without filter don't show the value. Show commented below codes
          //$constant_bill_array1 = _get_van_planning_bill_wise_data1($bill_area_code, $bill_pl_group, $bill_wise_range_op, $bill_wise_value, $bill_wise_weight, $bill_wise_volume, $bill_wise_rs_kg, $bill_wise_rs_cft, $bill_wise_array);
          if ($bill_wise_input_data) {
            $constant_bill_array1 = _get_van_planning_bill_wise_data1($bill_area_code, $bill_pl_group, $bill_wise_range_op, $bill_wise_value, $bill_wise_weight, $bill_wise_volume, $bill_wise_rs_kg, $bill_wise_rs_cft, $bill_wise_array, $child_bill_wise_date);
          }
          // New group created element 
          $child_bill_header_array = array(
            'area_code' => array('data' => t('Area Code')),
            'pl_group' => array('data' => t('Pl Group')),
            'bill_no' => array('data' => t('Bill no')),
            'bill_value' => array('data' => t('Value-RsK')),
            'sum_kg' => array('data' => t('Weight')),
            'sum_cft' => array('data' => t('Volume')),
            'rs_per_sum_kg' => array('data' => t('Rs/Kg')),
            'rs_per_sum_cft' => array('data' => t('Rs/Cft')),
            'party_code_val'=>array('data'=>t('Party Code')),//NEWFIELD
            'party_name_val'=>array('data'=>t('Party Name')),
            'bill_group' => array('data' => t('bill_group'), 'class' => array('bill_no_val')),
          );
          // show result in pop up's pop up form
          if (!empty($constant_bill_array1)) {
            $form['editable_vp']['vp_pop']['child_bills'][$selected_modify_bill]['unassigned_bills_table'] = array(
              '#type' => 'tableselect',
              '#empty' => t('No data available.'),
              '#multiple' => TRUE,
              '#js_select' => TRUE, //Dont give Select All checkbox
              '#prefix' => '<div class = "bill-edit"><h3></h3>',
              '#suffix' => '</div>',
              '#header' => $child_bill_header_array,
              '#options' => $constant_bill_array1,
            );
            $form['editable_vp']['vp_pop']['child_bills'][$selected_modify_bill]['unassigned_bills_add'] = array(
              '#type' => 'submit',
              '#value' => "Move to $selected_modify_bill",
              '#name' => 'unassigned_bills_add',
              '#ajax' => array(
                'wrapper' => 'van-itemlist-div',
                'callback' => 'van_planning_ajax_callback', // simple ajax callback
                'method' => 'replace',
                'effect' => 'fade',
              ),
              '#submit' => array('van_planning_unassigned_bills_ajax_submit'),
            );
          }
        }
      }
    }

    //set of submit fieldset
    $form['submit_vp'] = array(
      '#type' => 'fieldset',
      '#title' => t('Submit Van Planning'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    $form['submit_vp']['trip_execution_date'] = array(
      '#type' => 'date_popup',
      '#date_format' => 'm/d/y',
      '#title' => t('Trip Execution Date*'),
      '#date_label_position' => '',
      '#date_year_range' => '-20:+1',
      '#size' => 30,
    );
    $vp_final_header_array = array(
      'van_id' => array('data' => t('Van')),
      'area_codes' => array('data' => t('Area Code')),
      'map_data' => array('data' => t('Map')),
      'parkstop_link' => array('data' => t('Parkstop link')),
      'del_boy' => array('data' => t('Selected DB Count')),
      'dist' => array('data' => t('In-Area KMs')),
      'time' => array('data' => t('In-Area Time (Hrs)')),      
      'bill_count' => array('data' => t('Bills')),
      'sku_count' => array('data' => t('Sku Count')),
      'bill_value' => array('data' => t('Value-RsK')),
      'bill_less_Five' => array('data' => t('<500 Rs. Bill Count & Value')),
      'bill_less_thou' => array('data' => t('500-1000 Rs. Bill Count & Value')),
      'cld' => array('data' => t('CLD Lines')),
      'loose' => array('data' => t('Loose Lines')),
      'bill_wise_loose' => array('data' => t('Bill Loose Lines')),
      'cld_value' => array('data' => t('CLD Lines Value')),
      'loose_value' => array('data' => t('Loose Lines Value')),
      'sum_kg' => array('data' => t('Weight')),
      'sum_cft' => array('data' => t('Volume')),
      'rs_per_sum_kg' => array('data' => t('Rs/Kg')),
      'rs_per_sum_cft' => array('data' => t('Rs/Cft')),
      'bill_no' => array('data' => t('bill_no'), 'class' => array('bill_no_val')),
      'vehicle' => array('data' => t('Vehicle')),
      'cashier' => array('data' => t('Cashier')),
      'hht_id' => array('data' => t('HHT ID*')),
    );
    $form['submit_vp']['vp_final_table'] = array(
      '#empty' => t('No data available.'),
      '#prefix' => '<div class = "bill-edit trip-date-details">',
      '#suffix' => '</div>',
      '#header' => $vp_final_header_array,
      '#theme' => 'table',
      '#tree' => TRUE,
      '#rows' => array(),
    );

    foreach ($vp_final_rows as $k => $val) {
      $form['submit_vp']['vp_final_table']['#rows'][$k]['van_id'] = $val['van_id'];
      $all_data_for_area_code = json_decode($val['bill_no']['data-value'], 1);
      $combined_area_codes = $bill_below_fiveh = $bill_betwn_five_thou = array();
      foreach ($all_data_for_area_code as $data_for_final_tb) {
        $combined_area_codes[$data_for_final_tb['area_code']] = $data_for_final_tb['area_code']; //for area code
        if ($data_for_final_tb['bill_value'] < 500) {
          $bill_below_fiveh['count'] +=1;
          $bill_below_fiveh['value'] +=$data_for_final_tb['bill_value'];
        }
        if ($data_for_final_tb['bill_value'] >= 500 && $data_for_final_tb['bill_value'] <= 1000) {
          $bill_betwn_five_thou['count'] +=1;
          $bill_betwn_five_thou['value'] +=$data_for_final_tb['bill_value'];
        }
      }
      $combined_area_codes = implode(", ", $combined_area_codes);
      $form['submit_vp']['vp_final_table']['#rows'][$k]['area_codes'] = $combined_area_codes;
      $form['submit_vp']['vp_final_table']['#rows'][$k]['map_data'] = $val['map_data'];
      $form['submit_vp']['vp_final_table']['#rows'][$k]['parkstop_link'] = $val['parkstop_link'];
      $form['submit_vp']['vp_final_table']['#rows'][$k]['del_boy'] = $val['del_boy'];
      $form['submit_vp']['vp_final_table']['#rows'][$k]['dist'] = $val['dist'];
      $form['submit_vp']['vp_final_table']['#rows'][$k]['time'] = $val['time'];
      $form['submit_vp']['vp_final_table']['#rows'][$k]['bill_count'] = $val['bill_count'];
      $form['submit_vp']['vp_final_table']['#rows'][$k]['sku_count'] = $val['sku_count'];
      $form['submit_vp']['vp_final_table']['#rows'][$k]['bill_value'] = $val['bill_value'];
      $form['submit_vp']['vp_final_table']['#rows'][$k]['bill_less_five'] = "Count:" . $bill_below_fiveh['count'] . ' / Value:' . $bill_below_fiveh['value'];
      $form['submit_vp']['vp_final_table']['#rows'][$k]['bill_less_thou'] = "Count:" . $bill_betwn_five_thou['count'] . ' / Value:' . $bill_betwn_five_thou['value'];
      $form['submit_vp']['vp_final_table']['#rows'][$k]['cld'] = $val['cld'];
      $form['submit_vp']['vp_final_table']['#rows'][$k]['loose'] = $val['loose'];
      $form['submit_vp']['vp_final_table']['#rows'][$k]['bill_wise_loose'] = $val['bill_wise_loose'];
      $form['submit_vp']['vp_final_table']['#rows'][$k]['cld_value'] = $val['cld_value'];
      $form['submit_vp']['vp_final_table']['#rows'][$k]['loose_value'] = $val['loose_value'];
      $form['submit_vp']['vp_final_table']['#rows'][$k]['sum_kg'] = $val['sum_kg'];
      $form['submit_vp']['vp_final_table']['#rows'][$k]['sum_cft'] = $val['sum_cft'];
      $form['submit_vp']['vp_final_table']['#rows'][$k]['rs_per_sum_kg'] = $val['rs_per_sum_kg'];
      $form['submit_vp']['vp_final_table']['#rows'][$k]['rs_per_sum_cft'] = $val['rs_per_sum_cft'];
      $form['submit_vp']['vp_final_table']['#rows'][$k]['bill_no'] = $val['bill_no'];

      $vehicle_list = array(
        '#attributes' => array('class' => array('vp_final_editable_element'), 'title' => t('Vehicle')),
        '#type' => 'textfield',
        '#autocomplete_path' => 'vehicle/autocomplete',
        '#prefix' => '<div class = "form-autocomplete_fields">',
        '#suffix' => '<div>',
        '#size' => 10,
      );
      $cashier_list = array(
        '#attributes' => array('class' => array('vp_final_editable_element'), 'title' => t('Cashier')),
        '#type' => 'textfield',
        '#autocomplete_path' => 'cashier/autocomplete',
        '#prefix' => '<div class = "form-autocomplete_fields">',
        '#suffix' => '<div>',
        '#size' => 10,
      );
      $hht_id_list = array(
        '#attributes' => array('class' => array('vp_final_editable_element'), 'title' => t('HHT ID')),
        '#type' => 'textfield',
        '#autocomplete_path' => 'hht-id-master/autocomplete',
        '#prefix' => '<div class = "form-autocomplete_fields">',
        '#suffix' => '<div>',
        '#size' => 10,
      );

      $form['submit_vp']['vp_final_table'][$k]["vehicle_list"] = &$vehicle_list;
      $form['submit_vp']['vp_final_table'][$k]["cashier_list"] = &$cashier_list;
      $form['submit_vp']['vp_final_table'][$k]["hht_id_list"] = &$hht_id_list;

      $form['submit_vp']['vp_final_table']['#rows'][$k][]['data']["vehicle_list"] = &$vehicle_list;
      $form['submit_vp']['vp_final_table']['#rows'][$k][]['data']["cashier_list"] = &$cashier_list;
      $form['submit_vp']['vp_final_table']['#rows'][$k][]['data']["hht_id_list"] = &$hht_id_list;
      unset($vehicle_list);
      unset($cashier_list);
      unset($hht_id_list);
    }
    $form['submit_vp']['check_seq'] = array(
      '#type' => 'submit',
      '#value' => 'Calculate Time/Distance',
      '#name' => 'check_seq',
      '#ajax' => array(
        'wrapper' => 'van-itemlist-div',
        'callback' => 'van_planning_ajax_callback', // eimple ajax callback
        'method' => 'replace',
        'effect' => 'fade',
      ),
      '#submit' => array('van_planning_filter_ajax_submit'),
    );
    $form['submit_vp']['sync_time_distance'] = array(
      '#type' => 'submit',
      '#value' => 'Refresh',
      '#name' => 'sync_time_distance',
      '#ajax' => array(
        'wrapper' => 'van-itemlist-div',
        'callback' => 'van_planning_ajax_callback', // eimple ajax callback
        'method' => 'replace',
        'effect' => 'fade',
      ),
      '#submit' => array('van_planning_filter_ajax_submit'),
      //'#submit' => array('sync_parkstop_time_distance'),
    );
    $form['submit_vp']['check_duplicates'] = array(
      '#type' => 'submit',
      '#value' => 'Check Duplicates',
      '#name' => 'check_duplicates',
      '#validate' => array('_check_duplicates_validate'),
      '#submit' => array('_check_duplicates_submit'),
    );
    drupal_add_js(drupal_get_path('module', 'van_planning') . '/js/prevent_multiple_click.js', 'file');
    $form['submit_vp']['create_van_planning'] = array(
      '#type' => 'submit',
      '#value' => 'Submit and Create Van Planning',
      '#name' => 'create_van_planning',
      '#prefix' => "<div class='multiple_click'>",
      '#suffix'=> "</div><div id = 'referesh_warning' style='display:none';><p><font color='red'>Van Planning is in progress. Please do not click on Submit again.</font></p></div>",
      '#validate' => array('_create_van_validate'),
      '#submit' => array('_create_van_submit'),
    );
  }
  return $form;
}

/**
 * 
 * @param type $form
 * @param type $form_state
 */
function show_move_to_form_submit($form, &$form_state) {
  $form_state['move_to_enable'] = 1;
  $form_state['rebuild'] = TRUE;
}

/**
 * 
 * @param type $form
 * @param type $form_state
 */
function van_planning_modify_van_ajax_submit($form, &$form_state) {
  $modify_van = $form_state['values']['modify_van'];
  $array = explode(' ', $modify_van);
  $selected_modify_bill = $array[1];
  if (!empty($selected_modify_bill)) {
    $selected_modify_bill_array = $form_state['van_wise_bills'][$selected_modify_bill];
    $form_state['selected_modify_bill_array'][$selected_modify_bill] = $selected_modify_bill_array;
    $form_state['selected_van_grp'] = $selected_modify_bill;
  }
  $form_state['rebuild'] = TRUE;
}

/**
 * 
 * @param type $form
 * @param type $form_state
 */
function create_move_to_van_validate($form, &$form_state) {
  $move_to_select = $form_state['values']['move_to_select'];
  if (strpos($move_to_select, ' ') !== false) {
    // Space found.
    form_set_error('move_to_select', 'Space not allowed in Van name');
  }
}

function create_move_to_van_submit($form, &$form_state) {

  $group_name = $form_state['values']['move_to_select'];
  unset($form_state['move_to_enable']);
  unset($form_state['input']['move_to_select']);
  if (!isset($form_state['bill_array'])) {
    $form_state['bill_array'] = array();
  }

  if (!empty($group_name)) {
    foreach ($form_state['values']['list_form_items'] as $key => $value) {
      if ($key != "main_table_total"  && $key != 'main_table_subtotal') { // Don't allow total row and subtotal row also
        if (!empty($value)) {
          if (!isset($form_state['van_wise_bills'][$group_name])) {
            $form_state['van_wise_bills'][$group_name] = array();
          }
          $exploded_array = explode('#', $value);
          $exploded_value = $exploded_array[1];

          if (!isset($form_state['new_dispatch_planning'][$group_name][$exploded_value])) {
            $form_state['new_dispatch_planning'][$group_name][$exploded_value] = array();
            $new_array1 = json_decode($form_state['complete form']['editable_vp']['vp']['list_form_items']['#options'][$value]['bill_group']['data-value'], 1);
            $bill_array1 = json_decode($form_state['complete form']['editable_vp']['vp']['list_form_items']['#options'][$value]['bill_no']['data-value'], 1);
            $form_state['new_dispatch_planning'][$group_name][$exploded_value] += $new_array1[$value];
            $form_state['van_wise_bills'][$group_name] += $bill_array1;
          }
          else {
            if (!isset($form_state['new_dispatch_planning'][$group_name][$exploded_value]['sum_cft'])) {
              $form_state['new_dispatch_planning'][$group_name][$exploded_value]['sum_cft'] = 0;
            }
            if (!isset($form_state['new_dispatch_planning'][$group_name][$exploded_value]['bill_count'])) {
              $form_state['new_dispatch_planning'][$group_name][$exploded_value]['bill_count'] = 0;
            }
            if (!isset($form_state['new_dispatch_planning'][$group_name][$exploded_value]['bill_value'])) {
              $form_state['new_dispatch_planning'][$group_name][$exploded_value]['bill_value'] = 0;
            }
            if (!isset($form_state['new_dispatch_planning'][$group_name][$exploded_value]['sum_kg'])) {
              $form_state['new_dispatch_planning'][$group_name][$exploded_value]['sum_kg'] = 0;
            }
            $new_array1 = json_decode($form_state['complete form']['editable_vp']['vp']['list_form_items']['#options'][$value]['bill_group']['data-value'], 1);
            $form_state['new_dispatch_planning'][$group_name][$exploded_value]['bill_count'] += $new_array1[$value]['bill_count'];
            $form_state['new_dispatch_planning'][$group_name][$exploded_value]['bill_value'] += $new_array1[$value]['bill_value'];
            $form_state['new_dispatch_planning'][$group_name][$exploded_value]['sum_kg'] += $new_array1[$value]['sum_kg'];
            $form_state['new_dispatch_planning'][$group_name][$exploded_value]['sum_cft'] += $new_array1[$value]['sum_cft'];
            $form_state['new_dispatch_planning'][$group_name][$exploded_value]['rs_per_sum_kg'] = round($form_state['new_dispatch_planning'][$group_name][$exploded_value]['bill_value'] / $form_state['new_dispatch_planning'][$group_name][$exploded_value]['sum_kg'], 2);
            $form_state['new_dispatch_planning'][$group_name][$exploded_value]['rs_per_sum_cft'] = round($form_state['new_dispatch_planning'][$group_name][$exploded_value]['bill_value'] / $form_state['new_dispatch_planning'][$group_name][$exploded_value]['sum_cft'], 2);
            $exist_data = json_decode($form_state['new_dispatch_planning'][$group_name][$exploded_value]['bill_no']['data-value'], 1);
            $new_data = json_decode($new_array1[$value]['bill_no']['data-value'], 1);
            $total_data = $exist_data + $new_data;
            $form_state['new_dispatch_planning'][$group_name][$exploded_value]['bill_no']['data-value'] = json_encode($total_data);
            $form_state['van_wise_bills'][$group_name] += $new_data;
          }
          $bill_array = json_decode($form_state['complete form']['editable_vp']['vp']['list_form_items']['#options'][$value]['bill_no']['data-value'], 1);
          $form_state['bill_array'] += $bill_array;
        }
      }
    }
    //newly added in crete new van submit
    unset($form_state['tim_dist_response'][$group_name]);
    unset($_SESSION['park_stop'][$group_name]);
  }
  $form_state['rebuild'] = TRUE;
}

/**
 * 
 * @param type $form
 * @param array $form_state
 */
function van_planning_filter_ajax_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
}

/**
 * 
 * @param type $form
 * @param array $form_state
 */
function van_planning_filter_close_ajax_submit($form, &$form_state) {
  unset($form_state['move_to_enable']);
  $form_state['rebuild'] = TRUE;
}

function van_planning_filter_reset_ajax_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
}
/**
 * 
 * @param type $form
 * @param array $form_state
 */
function van_planning_remove_bill_ajax_submit($form, &$form_state) {

  $removed = FALSE;
  $unique_code = $form_state['selected_van_grp'];

  foreach ($form_state['values']['modify_van_bills'] as $key => $val) {
    if (!empty($val)) {
      $removed = TRUE;
      // remove element from table
      unset($form_state['selected_modify_bill_array'][$unique_code][$val]);
      unset($form_state['van_wise_bills'][$unique_code][$val]);
      // remove from $form_state['bill_array']
      unset($form_state['bill_array'][$key]);
      unset($form_state['tim_dist_response'][$unique_code]);
      unset($_SESSION['park_stop'][$unique_code]);
    }
  }

  if ($removed) {
    // Recalculate Van Wise Group logic here for changed group only
    $new_array = array();
    // $bill_array = json_decode($form_state['new_dispatch_planning'][$unique_code]['bill_no']['data-value'], 1);
    $new_bill_array = $form_state['selected_modify_bill_array'][$unique_code];

    foreach ($form_state['selected_modify_bill_array'][$unique_code] as $key => $val) {
      if (!isset($new_array[$val['pl_group']]['bill_count'])) {
        $new_array[$val['pl_group']]['bill_count'] = 0;
      }
      if (!isset($new_array[$val['pl_group']]['bill_value'])) {
        $new_array[$val['pl_group']]['bill_value'] = 0;
      }
      if (!isset($new_array[$val['pl_group']]['sum_kg'])) {
        $new_array[$val['pl_group']]['sum_kg'] = 0;
      }
      if (!isset($new_array[$val['pl_group']]['sum_cft'])) {
        $new_array[$val['pl_group']]['sum_cft'] = 0;
      }
      $new_array[$val['pl_group']]['bill_count'] += 1;
      $new_array[$val['pl_group']]['bill_value'] += $val['bill_value'];
      $new_array[$val['pl_group']]['area_code'] = $val['area_code'];
      $new_array[$val['pl_group']]['pl_group'] = $val['pl_group'];
      $new_array[$val['pl_group']]['sum_kg'] += $val['sum_kg'];
      $new_array[$val['pl_group']]['sum_cft'] += $val['sum_cft'];

      $res_1[$val['pl_group']][$key] = array(
        'bill_no' => $val['bill_no'],
        'bill_value' => round($val['bill_value']),
        'area_code' => $val['area_code'],
        'pl_group' => $val['pl_group'],
        'sum_kg' => round($val['sum_kg']),
        'sum_cft' => round($val['sum_cft']),
        'rs_per_sum_kg' => round($val['bill_value'] / $val['sum_kg'], 2),
        'rs_per_sum_cft' => round($val['bill_value'] / $val['sum_cft'], 2),
        'party_code_val'=>$val['party_code_val'],//NEWFIELD
        'party_name_val'=>$val['party_name_val']
      );
    }

    foreach ($new_array as $key => $val) {
      $new_array[$val['pl_group']]['rs_per_sum_kg'] = round($new_array[$val['pl_group']]['bill_value'] / $new_array[$val['pl_group']]['sum_kg'], 2);
      $new_array[$val['pl_group']]['rs_per_sum_cft'] = round($new_array[$val['pl_group']]['bill_value'] / $new_array[$val['pl_group']]['sum_cft'], 2);
      $new_array[$val['pl_group']]['bill_no']['data'] = '';
      $new_array[$val['pl_group']]['bill_no']['class'] = array('bill_no_val');
      $new_array[$val['pl_group']]['bill_no']['data-value'] = json_encode($res_1[$val['pl_group']]);
      unset($new_array[$val['pl_group']]['sales_nid']);
    }

    if (!empty($new_bill_array)) {
      $form_state['new_dispatch_planning'][$unique_code] = $new_array;
    }
    else {
      unset($form_state['new_dispatch_planning'][$unique_code]);
    }
  }

  $form_state['rebuild'] = TRUE;
}

/**
 * 
 * @param type $form
 * @param type $form_state
 */
function van_planning_unassigned_bills_ajax_submit($form, &$form_state) {
  $removed = FALSE;
  $group_name = $form_state['selected_van_grp'];
  // recalculate time and distance routing api
  unset($form_state['tim_dist_response'][$group_name]);
  unset($_SESSION['park_stop'][$group_name]);
  foreach ($form_state['values']['unassigned_bills_table'] as $key => $val) {
    if (!empty($val)) {
      $new_data = $exist_data = array();
      // get bill array details
      $new_array1 = json_decode($form_state['complete form']['editable_vp']['vp_pop']['child_bills'][$group_name]['unassigned_bills_table']['#options'][$val]['bill_group']['data-value'], 1);
      $exploded_value = $new_array1[$val]['pl_group'];
      $new_data[$val] = $new_array1[$val];
      foreach ($form_state['new_dispatch_planning'] as $van_grp => $pl_grp_data) {

        if ($van_grp == $group_name) {
          // if same group then do not perform this action
          continue;
        }

        foreach ($pl_grp_data as $plgrp => $plgrpdata) {
          $all_van_bill_array_data = json_decode($plgrpdata['bill_no']['data-value'], 1);

          foreach ($all_van_bill_array_data as $sales_nid => $sales_bill_data) {
            if ($sales_nid == $val) {

              $form_state['new_dispatch_planning'][$van_grp][$plgrp]['bill_count'] = $form_state['new_dispatch_planning'][$van_grp][$plgrp]['bill_count'] - 1;
              $form_state['new_dispatch_planning'][$van_grp][$plgrp]['bill_value'] = $form_state['new_dispatch_planning'][$van_grp][$plgrp]['bill_value'] - $sales_bill_data['bill_value'];
              $form_state['new_dispatch_planning'][$van_grp][$plgrp]['sum_kg'] = $form_state['new_dispatch_planning'][$van_grp][$plgrp]['sum_kg'] - $sales_bill_data['sum_kg'];
              $form_state['new_dispatch_planning'][$van_grp][$plgrp]['sum_cft'] = $form_state['new_dispatch_planning'][$van_grp][$plgrp]['sum_cft'] - $sales_bill_data['sum_cft'];
              $form_state['new_dispatch_planning'][$van_grp][$plgrp]['rs_per_sum_kg'] = $form_state['new_dispatch_planning'][$van_grp][$plgrp]['bill_value'] - $form_state['new_dispatch_planning'][$van_grp][$plgrp]['sum_kg'];
              $form_state['new_dispatch_planning'][$van_grp][$plgrp]['rs_per_sum_cft'] = $form_state['new_dispatch_planning'][$van_grp][$plgrp]['bill_value'] - $form_state['new_dispatch_planning'][$van_grp][$plgrp]['sum_cft'];

              // remove bill data array from hidden field
              unset($all_van_bill_array_data[$sales_nid]);
              unset($form_state['tim_dist_response'][$van_grp]);
              unset($_SESSION['park_stop'][$van_grp]);
              unset($form_state['van_wise_bills'][$van_grp][$sales_nid]);
              // remove from selcted van grp
              unset($form_state['selected_modify_bill_array'][$van_grp][$sales_nid]);
              if (empty($form_state['new_dispatch_planning'][$van_grp][$plgrp]['bill_count'])) {
                unset($form_state['new_dispatch_planning'][$van_grp][$plgrp]);
                break 2;
              }
            }
          }
          $form_state['new_dispatch_planning'][$van_grp][$plgrp]['bill_no']['data-value'] = json_encode($all_van_bill_array_data, 1);
        }

        if (empty($form_state['new_dispatch_planning'][$van_grp])) {
          unset($form_state['new_dispatch_planning'][$van_grp]);
        }
      }

      if (!isset($form_state['new_dispatch_planning'][$group_name][$exploded_value]['sum_cft'])) {
        $form_state['new_dispatch_planning'][$group_name][$exploded_value]['sum_cft'] = 0;
      }
      if (!isset($form_state['new_dispatch_planning'][$group_name][$exploded_value]['bill_count'])) {
        $form_state['new_dispatch_planning'][$group_name][$exploded_value]['bill_count'] = 0;
      }
      if (!isset($form_state['new_dispatch_planning'][$group_name][$exploded_value]['bill_value'])) {
        $form_state['new_dispatch_planning'][$group_name][$exploded_value]['bill_value'] = 0;
      }
      if (!isset($form_state['new_dispatch_planning'][$group_name][$exploded_value]['sum_kg'])) {
        $form_state['new_dispatch_planning'][$group_name][$exploded_value]['sum_kg'] = 0;
      }

      $form_state['new_dispatch_planning'][$group_name][$exploded_value]['area_code'] = $new_array1[$val]['area_code'];
      $form_state['new_dispatch_planning'][$group_name][$exploded_value]['pl_group'] = $new_array1[$val]['pl_group'];
      $form_state['new_dispatch_planning'][$group_name][$exploded_value]['bill_count'] += 1;
      $form_state['new_dispatch_planning'][$group_name][$exploded_value]['bill_value'] += $new_array1[$val]['bill_value'];
      $form_state['new_dispatch_planning'][$group_name][$exploded_value]['sum_kg'] += $new_array1[$val]['sum_kg'];
      $form_state['new_dispatch_planning'][$group_name][$exploded_value]['sum_cft'] += $new_array1[$val]['sum_cft'];
      $form_state['new_dispatch_planning'][$group_name][$exploded_value]['rs_per_sum_kg'] = round($form_state['new_dispatch_planning'][$group_name][$exploded_value]['bill_value'] / $form_state['new_dispatch_planning'][$group_name][$exploded_value]['sum_kg'], 2);
      $form_state['new_dispatch_planning'][$group_name][$exploded_value]['rs_per_sum_cft'] = round($form_state['new_dispatch_planning'][$group_name][$exploded_value]['bill_value'] / $form_state['new_dispatch_planning'][$group_name][$exploded_value]['sum_cft'], 2);
      $form_state['new_dispatch_planning'][$group_name][$exploded_value]['bill_no']['data'] = '';
      $form_state['new_dispatch_planning'][$group_name][$exploded_value]['bill_no']['class'] = array('bill_no_val');
      if (isset($form_state['new_dispatch_planning'][$group_name][$exploded_value]['bill_no']['data-value'])) {
        $exist_data = json_decode($form_state['new_dispatch_planning'][$group_name][$exploded_value]['bill_no']['data-value'], 1);
      }
      $total_data = $exist_data + $new_array1;
      $form_state['new_dispatch_planning'][$group_name][$exploded_value]['bill_no']['data-value'] = json_encode($total_data);
      $form_state['van_wise_bills'][$group_name] += $new_data;
      $form_state['selected_modify_bill_array'][$group_name] += $new_data;

      $form_state['bill_array'][$val] = $new_data[$val];
    }
  }

  $form_state['rebuild'] = TRUE;
}

/**
 * 
 * @param type $form
 * @param type $form_state
 * @return array
 */
function van_planning_filter_ajax_callback($form, &$form_state) {
  return $form;
}

/**
 * 
 * @param type $form
 * @param type $form_state
 * @return array
 */
function van_planning_ajax_callback($form, &$form_state) {
  return $form;
}

/**
 * 
 * @param type $form
 * @param type $form_state
 * @return type
 * perform scolling after that
 */
function van_planning_ajax_callback_scroll($form, &$form_state) {
  $commands[] = ajax_command_replace('#van-itemlist-div', render($form));
  $commands[] = array
    (
    // The command will be used in our JavaScript file (see next section)
    'command' => 'scrolling_field',
    // We pass the value that the user selected in the select element to our
    // JavaScript function:
    //'selectedValue' => 123,
  );

  return array('#type' => 'ajax', '#commands' => $commands);
  //return $form; // Previous Code
}

/**
 * 
 * @param type $form
 * @param type $form_state
 * @return array
 */
function show_move_to_ajax($form, &$form_state) {
//   $commands[] = ajax_command_replace('#van-itemlist-div', render($form));
//   $group_name = $form_state['values']['move_to_select'];
//   if (!empty($group_name)) {
//      if ($form_state['clicked_button']['#name'] == 'create_van_button') {
//         foreach ($form_state['values']['list_form_items'] as $key => $value) {
//            if (!empty($value)) {
//               $commands[] = ajax_command_remove(".$value");
//            }
//         }
//      }
//   }
//
//   return array('#type' => 'ajax', '#commands' => $commands);
  return $form;
}

/**
 * 
 * @param type $form
 * @param type $form_state
 */
function van_planning_ajax_submit($area_codes, $van_pl_group, $pl_group, $bill_date, $bill_range_op, $bill_value, $weight, $volume, $bill_array, $outlet_area_hook_grp) {
  $user_id = $GLOBALS['user']->uid;
  $dis_id = change_feeds_import_get_distributor_id($user_id);
  $no_of_days = variable_get('van_planning_dates', 15);
  $billing_start_date = strtotime(date('Y-m-d') . "- $no_of_days days"); // @todo replace 15 this line
  $billing_end_date = strtotime(date("Y-m-d"));

  $query = db_select('node', 'n');
  $query->condition('n.type', 'hul_code_master');
  $query->leftjoin('field_data_field_area_code', 'ac', 'ac.entity_id = n.nid');
  $query->condition('n.uid', $dis_id);

  // join area code master

  $query->leftjoin('node', 'n1', "n1.nid = ac.field_area_code_nid and n1.type ='area_code_master' and n1.uid = $dis_id ");
  $query->addExpression("CASE WHEN (n1.title IS NOT NULL) THEN n1.title ELSE 'NA' END", 'area_code');
  $query->groupBy('n1.title'); // group by Area code
  if (!empty($area_codes)) {
    if ($area_codes != 'no_input') {
      $query->condition('n1.title', $area_codes, 'IN'); // condition for Area code
    }
  }
  else {
    $query->condition('n1.title', ''); // condition for Area code
  }

  // join outlet master
  $query->join('field_data_field_hul_code_reference', 'hcr', 'hcr.field_hul_code_reference_nid = n.nid');
  $query->join('node', 'n2', 'n2.nid = hcr.entity_id');
  $query->condition('n2.type', 'outlet_master');
  $query->condition('n2.uid', $dis_id);
  $query->join('field_data_field_pl_group', 'fpg', 'fpg.entity_id = n2.nid');
  $query->groupBy('fpg.field_pl_group_value'); // group by Pl Group
  $query->addField('fpg', 'field_pl_group_value', 'pl_group');
  if (!empty($pl_group)) {
    $query->condition('fpg.field_pl_group_value', $pl_group); // condition for Pl Group
  }
  // Party code and party Name NEWFIELD
  $query->join('field_data_field_party_code', 'fpc', 'fpc.entity_id = n2.nid');
  $query->join('field_data_field_party_name', 'fpn', 'fpn.entity_id = n2.nid');
  
  // join sales register data
  $query->join('field_data_field_party_code_reference', 'pcr', 'pcr.field_party_code_reference_nid = n2.nid');
  if (!empty($outlet_area_hook_grp)) {
    $query->join('field_data_field_area_hook', 'ah', 'ah.entity_id = n2.nid'); // Wrong join pcr.entity_id
    $query->join('node', 'n10', 'n10.nid = ah.field_area_hook_nid');
    $query->condition('n10.type', 'day_van_pl_gp_area_hook_grouping');
    $query->condition('n10.uid', $dis_id);

    $query->join('field_data_field_dva_area_hook_group_name', 'dvah', 'dvah.entity_id = n10.nid'); // Wrong join pcr.entity_id
    $query->condition('dvah.field_dva_area_hook_group_name_value', $outlet_area_hook_grp);
  }

  $query->join('node', 'n3', 'n3.nid = pcr.entity_id');
  $query->condition('n3.type', 'sales_register_data');
  $query->condition('n3.uid', $dis_id);

  $query->leftJoin('field_data_field_trip_id_reference', 'ft', 'ft.entity_id = n3.nid');
  $query->leftJoin('field_data_field_delivery_pending', 'dp', 'n3.nid = dp.entity_id');

//  $db_or = db_or();
//  $db_or->isNull('ft.field_trip_id_reference_nid');
//  $db_or->condition('dp.field_delivery_pending_value', 1);
//  $query->condition($db_or);
  
  //Exclude delivery pending bills. commented above line and inserted below line
  $query->isNull('ft.field_trip_id_reference_nid');
  $query->isNull('dp.field_delivery_pending_value');

  $query->addField('n3', 'nid', 'sales_nid');
  $query->join('field_data_field_sr_bill_date', 'sbd', 'sbd.entity_id = n3.nid');
  $query->condition('sbd.field_sr_bill_date_value', array($billing_start_date, $billing_end_date), 'BETWEEN');

  if (!empty($bill_date)) {
    $query->condition('sbd.field_sr_bill_date_value', $bill_date);
  }
  if (!empty($bill_array)) {
    $query->condition('n3.title', $bill_array, 'NOT IN');
  }
  $query->join('field_data_field_bill_net_sales', 'bv', 'bv.entity_id = n3.nid');
  $query->addField('n3', 'title', 'bill_no');
  $query->addExpression('ROUND(bv.field_bill_net_sales_value)', 'bill_value');

  // custom billing data
  $query->join('custom_billing_data', 'cb', 'cb.bill_number = n3.title');
  $query->condition('cb.distributor_id', $dis_id);
  $query->groupBy('cb.bill_number');
  // join sku7 code master
  $query->addExpression('ROUND(SUM(cb.Derived_NET_SALES_KGS))', 'sum_kg');
  $query->addExpression('ROUND(SUM(cb.Derived_NET_SALES_VOLUME))', 'sum_cft');

  // join Party Pack Master (Van pl group)
  if (!empty($van_pl_group)) {
    $query->join('field_data_field_pp_master_reference', 'ppr', 'ppr.entity_id = n2.nid');
    $query->join('node', 'n5', 'n5.nid = ppr.field_pp_master_reference_nid');
    $query->join('field_data_field_van_pl_group', 'vpg', 'vpg.entity_id = n5.nid');
    $query->condition('n5.type', 'party_pack_master');
    $query->condition('n5.uid', $dis_id);
    $query->condition('vpg.field_van_pl_group_value', $van_pl_group);
  }
  // Party code and party Name NEWFIELD
  $query->addExpression('fpc.field_party_code_value', 'party_code_val');
  $query->addExpression('fpn.field_party_name_value', 'party_name_val');

  // condition according to operator selected
  if (!empty($bill_range_op)) {
    if (!empty($bill_value)) {
      $query->addExpression('ROUND(CONVERT(field_bill_net_sales_value, SIGNED))', 'bill_value1');
      $query->havingCondition('bill_value1', $bill_value, $bill_range_op);
    }
    if (!empty($weight)) {
      $query->havingCondition('sum_kg', $weight, $bill_range_op);
    }
    if (!empty($volume)) {
      $query->havingCondition('sum_cft', $volume, $bill_range_op);
    }
  }
  $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

  foreach ($res as $key => $val) {
    foreach ($val as $key1 => $val1) {
      if ($key1 == 'bill_no') {
        if (!isset($res1[$val['area_code'] . '#' . $val['pl_group']]['bill_count'])) {
          $res1[$val['area_code'] . '#' . $val['pl_group']]['bill_count'] = 0;
        }
        $res1[$val['area_code'] . '#' . $val['pl_group']]['bill_count'] += 1;
      }
      else if (($key1 == 'bill_value') || ($key1 == 'sum_kg') || ($key1 == 'sum_cft')) {
        if (!isset($res1[$val['area_code'] . '#' . $val['pl_group']][$key1])) {
          $res1[$val['area_code'] . '#' . $val['pl_group']][$key1] = 0;
        }
        $res1[$val['area_code'] . '#' . $val['pl_group']][$key1] += $val1;
      }
      else {
        $res1[$val['area_code'] . '#' . $val['pl_group']][$key1] = $val1;
      }
    }

    // bill_no , bill_value, area_code, pl_grp, weight, volume
    $res_1[$val['area_code'] . '#' . $val['pl_group']][$val['sales_nid']] = array(
      'bill_no' => $val['bill_no'],
      'bill_value' => round($val['bill_value']),
      'area_code' => $val['area_code'],
      'pl_group' => $val['pl_group'],
      'sum_kg' => round($val['sum_kg']),
      'sum_cft' => round($val['sum_cft']),
      'rs_per_sum_kg' => round($val['bill_value'] / $val['sum_kg'], 2),
      'rs_per_sum_cft' => round($val['bill_value'] / $val['sum_cft'], 2),
      'party_code_val'=>$val['party_code_val'],//NEWFIELD
      'party_name_val'=>$val['party_name_val']
    );

    $res1[$val['area_code'] . '#' . $val['pl_group']]['bill_no'] = array('data' => '', 'class' => array('bill_no_val'), 'data-value' => json_encode($res_1[$val['area_code'] . '#' . $val['pl_group']]));
  }
  $subtotal = array();
  $total = array();
  foreach ($res1 as $array_key => $array_value) {
    $res2 = array();
    $res1[$array_key]['bill_value'] = number_format($array_value['bill_value']);
    $res1[$array_key]['rs_per_sum_kg'] = round($array_value['bill_value'] / $array_value['sum_kg'], 2);
    $res1[$array_key]['rs_per_sum_cft'] = round($array_value['bill_value'] / $array_value['sum_cft'], 2);
    $res1[$array_key]['#attributes'] = array('class' => array("$array_key"));
        
    // Adding class for bill count, bill value, weight and volume 
    $res1[$array_key]['bill_count'] = array('data' => $array_value['bill_count'],'class' => array("van_bill_count"));
    $res1[$array_key]['bill_value'] = array('data' => $array_value['bill_value'],'class' => array("van_bill_value"));
    $res1[$array_key]['sum_kg'] = array('data' => $array_value['sum_kg'],'class' => array("van_bill_sumkg"));
    $res1[$array_key]['sum_cft'] = array('data' => $array_value['sum_cft'],'class' => array("van_bill_sumcft"));

    // bill_no , bill_value, area_code, pl_grp, weight, volume
    $res2[$array_key] = array(
      'bill_count' => $array_value['bill_count'],
      'bill_value' => round($array_value['bill_value']),
      'area_code' => $array_value['area_code'],
      'pl_group' => $array_value['pl_group'],
      'sum_kg' => round($array_value['sum_kg']),
      'sum_cft' => round($array_value['sum_cft']),
      'rs_per_sum_kg' => round($res1[$array_key]['rs_per_sum_kg'], 2),
      'rs_per_sum_cft' => round($res1[$array_key]['rs_per_sum_cft'], 2),
      'bill_no' => $array_value['bill_no']);

    $res1[$array_key]['bill_group'] = array('data' => '', 'class' => array('bill_group_data'), 'data-value' => json_encode($res2));

    //Subtotal
    $subtotal['area_code'] = '<b>Sub Total</b>';
    $subtotal['bill_count'] = array('data' => '', 'class' => array('subtotal_bill_count'));
    $subtotal['bill_value'] = array('data' => '', 'class' => array('subtotal_bill_value'));
    $subtotal['sum_kg'] = array('data' => '', 'class' => array('subtotal_bill_sumkg'));
    $subtotal['sum_cft'] = array('data' => '', 'class' => array('subtotal_bill_sumcft'));
    $subtotal['bill_no'] = array('data' => '', 'class' => array('bill_no_val'), 'data-value' => '');
    $subtotal['#attributes'] = array('class' => array("hide_prev_checkbox"));
    $subtotal['bill_group'] = array('data' => '', 'class' => array('bill_group_data'), 'data-value' => '');
    
    $total['area_code'] = '<b>Total</b>';
    $total['bill_count'] += $array_value['bill_count'];
    $total['bill_value'] += $array_value['bill_value'];
    $total['sum_kg'] += $array_value['sum_kg'];
    $total['sum_cft'] += $array_value['sum_cft'];
    $total['bill_no'] = array('data' => '', 'class' => array('bill_no_val'), 'data-value' => '');
    $total['#attributes'] = array('class' => array("hide_prev_checkbox"));
    $total['bill_group'] = array('data' => '', 'class' => array('bill_group_data'), 'data-value' => '');
  }
  if (!empty($res)) {
    $total['bill_value'] = number_format($total['bill_value']);
    $res1['main_table_subtotal'] = $subtotal; //subtotal
    $res1['main_table_total'] = $total;
  }
  return $res1;
}

/**
 * 
 * @param type $bill_area_code
 * @param type $bill_pl_group
 * @param type $bill_range_op
 * @param type $bill_wise_value
 * @param type $bill_wise_weight
 * @param type $bill_wise_volume
 * @param type $bill_wise_rs_kg
 * @param type $bill_wise_rs_cft
 * @param type $selected_modify_bill_array
 */
function _get_van_planning_bill_wise_data($bill_area_code, $bill_pl_group, $bill_wise_range_op, $bill_wise_value, $bill_wise_weight, $bill_wise_volume, $bill_wise_rs_kg, $bill_wise_rs_cft, $bill_wise_array) {
  $user_id = $GLOBALS['user']->uid;
  $dis_id = change_feeds_import_get_distributor_id($user_id);

  $query = db_select('node', 'n');
  $query->condition('n.type', 'hul_code_master');
  $query->leftjoin('field_data_field_area_code', 'ac', 'ac.entity_id = n.nid');
  $query->condition('n.uid', $dis_id);

  // join area code master
  $query->leftjoin('node', 'n1', "n1.nid = ac.field_area_code_nid and n1.type ='area_code_master' and n1.uid = $dis_id ");
  $query->groupBy('n1.title'); // group by Area code
  if (!empty($bill_area_code)) {
    $query->condition('n1.title', $bill_area_code); // condition for Area code
  }

  // join outlet master
  $query->join('field_data_field_hul_code_reference', 'hcr', 'hcr.field_hul_code_reference_nid = n.nid');
  $query->join('node', 'n2', 'n2.nid = hcr.entity_id');
  $query->condition('n2.type', 'outlet_master');
  $query->condition('n2.uid', $dis_id);
  $query->join('field_data_field_pl_group', 'fpg', 'fpg.entity_id = n2.nid');
  $query->groupBy('fpg.field_pl_group_value'); // group by Pl Group
  if (!empty($bill_pl_group)) {
    $query->condition('fpg.field_pl_group_value', $bill_pl_group); // condition for Pl Group
  }
  // Party code and party Name NEWFIELD
  $query->join('field_data_field_party_code', 'fpc', 'fpc.entity_id = n2.nid');
  $query->join('field_data_field_party_name', 'fpn', 'fpn.entity_id = n2.nid');
  
  // join sales register data
  $query->join('field_data_field_party_code_reference', 'pcr', 'pcr.field_party_code_reference_nid = n2.nid');
  $query->join('node', 'n3', 'n3.nid = pcr.entity_id');
  $query->condition('n3.type', 'sales_register_data');
  $query->condition('n3.uid', $dis_id);

  $query->leftJoin('field_data_field_trip_id_reference', 'ft', 'ft.entity_id = n3.nid');
  $query->leftJoin('field_data_field_delivery_pending', 'dp', 'n3.nid = dp.entity_id');

//  $db_or = db_or();
//  $db_or->isNull('ft.field_trip_id_reference_nid');
//  $db_or->condition('dp.field_delivery_pending_value', 1);
//  $query->condition($db_or);

  //Exclude delivery pending bills. commented above line and inserted below line
  $query->isNull('ft.field_trip_id_reference_nid');
  $query->isNull('dp.field_delivery_pending_value');
  
  if (!empty($bill_wise_array)) {
    $query->condition('n3.title', $bill_wise_array, 'IN');
  }
  $query->join('field_data_field_bill_net_sales', 'bv', 'bv.entity_id = n3.nid');
  $query->addExpression('n3.nid', 'sales_nid');
  $query->addExpression("CASE WHEN (n1.title IS NOT NULL) THEN n1.title ELSE 'NA' END", 'area_code');
  $query->addExpression('fpg.field_pl_group_value', 'pl_group');

  $query->addExpression('n3.title', 'bill_no');
  $query->addExpression('ROUND(bv.field_bill_net_sales_value)', 'bill_value');


  // custom billing data
  $query->join('custom_billing_data', 'cb', 'cb.bill_number = n3.title');
  $query->condition('cb.distributor_id', $dis_id);
  $query->groupBy('cb.bill_number');
  // join sku7 code master
  $query->addExpression('ROUND(SUM(cb.Derived_NET_SALES_KGS))', 'sum_kg');
  $query->addExpression('ROUND(SUM(cb.Derived_NET_SALES_VOLUME))', 'sum_cft');
  $query->addExpression('ROUND(ROUND(bv.field_bill_net_sales_value) / ROUND(SUM(cb.Derived_NET_SALES_KGS)),2)', 'rs_per_sum_kg');
  $query->addExpression('ROUND(ROUND(bv.field_bill_net_sales_value) / ROUND(SUM(cb.Derived_NET_SALES_VOLUME)),2)', 'rs_per_sum_cft');
  // condition according to operator selected

  // Party code and party Name NEWFIELD
  $query->addExpression('fpc.field_party_code_value', 'party_code_val');
  $query->addExpression('fpn.field_party_name_value', 'party_name_val');

  if (!empty($bill_wise_range_op)) {
    if (!empty($bill_wise_value)) {
      $query->addExpression('ROUND(CONVERT(field_bill_net_sales_value, SIGNED))', 'bill_value1');
      $query->havingCondition('bill_value1', $bill_wise_value, $bill_wise_range_op);
    }
    if (!empty($bill_wise_weight)) {
      $query->havingCondition('sum_kg', $bill_wise_weight, $bill_wise_range_op);
    }
    if (!empty($bill_wise_volume)) {
      $query->havingCondition('sum_cft', $bill_wise_volume, $bill_wise_range_op);
    }
    if (!empty($bill_wise_rs_kg)) {
      $query->havingCondition('rs_per_sum_kg', $bill_wise_rs_kg, $bill_wise_range_op);
    }
    if (!empty($bill_wise_rs_cft)) {
      $query->havingCondition('rs_per_sum_cft', $bill_wise_rs_cft, $bill_wise_range_op);
    }
  }
  $res = $query->execute()->fetchAll(PDO::FETCH_GROUP | PDO::FETCH_UNIQUE | PDO::FETCH_ASSOC);

  foreach ($res as $key => $val) {
    if (isset($val['bill_value'])) {

      $res[$key]['bill_value'] = number_format($val['bill_value']);
    }
    if (isset($val['bill_value1'])) {
      unset($res[$key]['bill_value1']);
    }
    else {
      $res[$key]['#attributes'] = array('class' => array($val['bill_no']));
    }
  }
  return $res;
}

/**
 * 
 * @param type $bill_area_code
 * @param type $bill_pl_group
 * @param type $bill_range_op
 * @param type $bill_wise_value
 * @param type $bill_wise_weight
 * @param type $bill_wise_volume
 * @param type $bill_wise_rs_kg
 * @param type $bill_wise_rs_cft
 * @param type $selected_modify_bill_array
 */
function _get_van_planning_bill_wise_data1($bill_area_code, $bill_pl_group, $bill_wise_range_op, $bill_wise_value, $bill_wise_weight, $bill_wise_volume, $bill_wise_rs_kg, $bill_wise_rs_cft, $bill_wise_array, $child_bill_wise_date = '') {
  $user_id = $GLOBALS['user']->uid;
  $dis_id = change_feeds_import_get_distributor_id($user_id);

  $billing_start_date = '';
  $billing_end_date = '';
  $child_bill_wise_date = strtotime($child_bill_wise_date);

  if (empty($child_bill_wise_date)) {
    $no_of_days = variable_get('van_planning_dates', 15);
    $billing_start_date = strtotime(date('Y-m-d') . "- $no_of_days days"); // @todo replace 15 this line
    $billing_end_date = strtotime(date("Y-m-d"));
  }


  $query = db_select('node', 'n');
  $query->condition('n.type', 'hul_code_master');
  $query->leftjoin('field_data_field_area_code', 'ac', 'ac.entity_id = n.nid');
  $query->condition('n.uid', $dis_id);

  // join area code master
  $query->leftjoin('node', 'n1', "n1.nid = ac.field_area_code_nid and n1.type ='area_code_master' and n1.uid = $dis_id ");
  $query->groupBy('n1.title'); // group by Area code
  if (!empty($bill_area_code)) {
    $query->condition('n1.title', $bill_area_code); // condition for Area code
  }

  // join outlet master
  $query->join('field_data_field_hul_code_reference', 'hcr', 'hcr.field_hul_code_reference_nid = n.nid');
  $query->join('node', 'n2', 'n2.nid = hcr.entity_id');
  $query->condition('n2.type', 'outlet_master');
  $query->condition('n2.uid', $dis_id);
  $query->join('field_data_field_pl_group', 'fpg', 'fpg.entity_id = n2.nid');
  $query->groupBy('fpg.field_pl_group_value'); // group by Pl Group
  if (!empty($bill_pl_group)) {
    $query->condition('fpg.field_pl_group_value', $bill_pl_group); // condition for Pl Group
  }
  // Party code and party Name NEWFIELD
  $query->join('field_data_field_party_code', 'fpc', 'fpc.entity_id = n2.nid');
  $query->join('field_data_field_party_name', 'fpn', 'fpn.entity_id = n2.nid');

  // join sales register data
  $query->join('field_data_field_party_code_reference', 'pcr', 'pcr.field_party_code_reference_nid = n2.nid');
  $query->join('node', 'n3', 'n3.nid = pcr.entity_id');
  $query->condition('n3.type', 'sales_register_data');
  $query->condition('n3.uid', $dis_id);

  $query->leftJoin('field_data_field_trip_id_reference', 'ft', 'ft.entity_id = n3.nid');
  $query->leftJoin('field_data_field_delivery_pending', 'dp', 'n3.nid = dp.entity_id');

//  $db_or = db_or();
//  $db_or->isNull('ft.field_trip_id_reference_nid');
//  $db_or->condition('dp.field_delivery_pending_value', 1);
//  $query->condition($db_or);
  
  //Exclude delivery pending bills. commented above line and inserted below line
  $query->isNull('ft.field_trip_id_reference_nid');
  $query->isNull('dp.field_delivery_pending_value');

  $query->join('field_data_field_sr_bill_date', 'sbd', 'sbd.entity_id = n3.nid');
  if (!empty($child_bill_wise_date)) {
    $query->condition('sbd.field_sr_bill_date_value', $child_bill_wise_date);
  }
  else {
    $query->condition('sbd.field_sr_bill_date_value', array($billing_start_date, $billing_end_date), 'BETWEEN');
  }

  if (!empty($bill_wise_array)) {
    $query->condition('n3.title', $bill_wise_array, 'NOT IN');
  }
  $query->join('field_data_field_bill_net_sales', 'bv', 'bv.entity_id = n3.nid');
  $query->addExpression('n3.nid', 'sales_nid');
  $query->addExpression("CASE WHEN (n1.title IS NOT NULL) THEN n1.title ELSE 'NA' END", 'area_code');

  $query->addExpression('fpg.field_pl_group_value', 'pl_group');

  $query->addExpression('n3.title', 'bill_no');

  $query->addExpression('ROUND(bv.field_bill_net_sales_value)', 'bill_value');


  // custom billing data
  $query->join('custom_billing_data', 'cb', 'cb.bill_number = n3.title');
  $query->condition('cb.distributor_id', $dis_id);
  $query->groupBy('cb.bill_number');
  // join sku7 code master
  $query->addExpression('ROUND(SUM(cb.Derived_NET_SALES_KGS))', 'sum_kg'); //MRC_WEIGHT
  $query->addExpression('ROUND(SUM(cb.Derived_NET_SALES_VOLUME))', 'sum_cft');
  $query->addExpression('ROUND(ROUND(bv.field_bill_net_sales_value) / ROUND(SUM(cb.Derived_NET_SALES_KGS)),2)', 'rs_per_sum_kg');
  $query->addExpression('ROUND(ROUND(bv.field_bill_net_sales_value) / ROUND(SUM(cb.Derived_NET_SALES_VOLUME)),2)', 'rs_per_sum_cft');
  // condition according to operator selected

  // Party code and party Name NEWFIELD
  $query->addExpression('fpc.field_party_code_value', 'party_code_val');
  $query->addExpression('fpn.field_party_name_value', 'party_name_val');

  if (!empty($bill_wise_range_op)) {
    if (!empty($bill_wise_value)) {
      $query->addExpression('ROUND(CONVERT(field_bill_net_sales_value, SIGNED))', 'bill_value1');
      $query->havingCondition('bill_value1', $bill_wise_value, $bill_wise_range_op);
    }
    if (!empty($bill_wise_weight)) {
      $query->havingCondition('sum_kg', $bill_wise_weight, $bill_wise_range_op);
    }
    if (!empty($bill_wise_volume)) {
      $query->havingCondition('sum_cft', $bill_wise_volume, $bill_wise_range_op);
    }
    if (!empty($bill_wise_rs_kg)) {
      $query->havingCondition('rs_per_sum_kg', $bill_wise_rs_kg, $bill_wise_range_op);
    }
    if (!empty($bill_wise_rs_cft)) {
      $query->havingCondition('rs_per_sum_cft', $bill_wise_rs_cft, $bill_wise_range_op);
    }
  }
  $res = $query->execute()->fetchAll(PDO::FETCH_GROUP | PDO::FETCH_UNIQUE | PDO::FETCH_ASSOC);
  foreach ($res as $key => $val) {
    if (isset($val['bill_value'])) {

      $res[$key]['bill_value'] = number_format($val['bill_value']);
    }
    if (isset($val['bill_value1'])) {
      unset($res[$key]['bill_value1']);
    }
    else {
      $res[$key]['#attributes'] = array('class' => array($val['bill_no']));
    }


    // bill_no , bill_value, area_code, pl_grp, weight, volume
    $res_1[$key][$key] = array(
      'bill_no' => $val['bill_no'],
      'bill_value' => round($val['bill_value']),
      'area_code' => $val['area_code'],
      'pl_group' => $val['pl_group'],
      'sum_kg' => round($val['sum_kg']),
      'sum_cft' => round($val['sum_cft']),
      'rs_per_sum_kg' => round($val['bill_value'] / $val['sum_kg'], 2),
      'rs_per_sum_cft' => round($val['bill_value'] / $val['sum_cft'], 2),
      'party_code_val'=>$val['party_code_val'],//NEWFIELD
      'party_name_val'=>$val['party_name_val']
      );

    $res[$key]['bill_group'] = array('data' => '',
      'class' => array('bill_no_val'),
      'data-value' => json_encode($res_1[$key]));
  }
  return $res;
}

/**
 * 
 * @param type $area_code
 * @param type $dis_id
 * @return array
 */
function _get_nearby_area_codes($area_code, $dis_id) {
  $res1 = array();
  $query = db_select('node', 'n');
  $query->leftjoin('field_data_field_linked_area_code', 'lc', 'lc.entity_id = n.nid');
  $query->leftjoin('node', 'n1', 'n1.nid = lc.field_linked_area_code_nid');
  $query->condition('n.title', $area_code);
  $query->fields('n', array('title'));
  $query->fields('n1', array('title'));
  $query->condition('n.type', 'area_code_master');
  $query->condition('n.uid', $dis_id);
  $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
  if (!empty($res)) {
    $res1[] = $res[0]['title'];
    foreach ($res as $val) {
      $res1[] = $val['n1_title'];
    }
  }

  return $res1;
}

/**
 * 
 * @param type $bill_numbers
 * @param type $distributor_id
 * @return array
 */
function _get_skus_cld_loose_count($total_sales_nid_array, $distributor_id, $lat_long = FALSE) {

  $res1 = array();
  $query = db_select('custom_billing_data', 'bd');
  $query->addfield('bd', 'Sum_of_TOTAL_SALES_QTY', 'total_qty');

  $query->addfield('bd', 'sku7_code', 'sku_code');
  $query->addfield('bd', 'bill_number', 'bill_number');
  $query->addExpression('ROUND(((Sum_of_NET_SALES_VALUE - ((Sum_of_OTHER_DISCOUNT + Sum_of_SCHEME_DISCOUNT)*(1 + (Sum_of_TAX_PERCENTAGE/100))))/Sum_of_TOTAL_SALES_QTY), 2)', 'sum_net_sales_value');
  $query->groupBy('bd.sku7_code');
  $query->groupBy('bd.bill_number');

  $query->condition('bd.distributor_id', $distributor_id);
  $query->join('node', 'n', 'n.title = bd.sku7_code AND bd.distributor_id = n.uid');
  $query->condition('n.type', 'sku7_code_master');
  $query->join('field_data_field_unit_case_units_', 'fucu', 'fucu.entity_id = n.nid');
  $query->fields('fucu', array('field_unit_case_units__value'));

  $query->join('node', 'n1', 'n1.title = bd.bill_number AND bd.distributor_id = n1.uid');
  $query->condition('n1.type', 'sales_register_data');
  $query->addfield('n1', 'nid', 'sales_nid');
  $query->addfield('n1', 'title', 'bill_no');
  $query->addfield('bd', 'party_code', 'party_code');

  $query->condition('n1.nid', $total_sales_nid_array, 'IN');
  if ($lat_long) {
    $query->join('field_data_field_party_code_reference', 'pcr', 'pcr.entity_id = n1.nid');
    $query->join('node', 'n2', 'n2.nid = pcr.field_party_code_reference_nid');
    $query->condition('n2.type', 'outlet_master');
    $query->join('field_data_field_hul_code_reference', 'hcr', 'hcr.entity_id = n2.nid');
    $query->join('node', 'n3', 'hcr.field_hul_code_reference_nid = n3.nid');
    $query->leftjoin('field_data_field_hul_updated_lat', 'la', 'la.entity_id = n3.nid');
    $query->leftjoin('field_data_field_hul_updated_long', 'lg', 'lg.entity_id = n3.nid');
    $query->addfield('la', 'field_hul_updated_lat_value', 'latitude');
    $query->addfield('lg', 'field_hul_updated_long_value', 'longitude');
  }
  $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

  $unique_sku = $sku_array = $bill_wise_unique_sku = $unique_sku_co = array();
  $res1['sku_count'] = $res1['cld'] = $res1['loose'] = $res1['cld_value'] = $res1['loose_value'] = 0;
  foreach ($res as $val) {
    if (isset($val['latitude']) && isset($val['longitude'])) {
      if (!isset($res1['lat_long'][$val['sales_nid']])) {
        $res1['lat_long'][$val['sales_nid']]['latitude'] = $val['latitude'];
        $res1['lat_long'][$val['sales_nid']]['longitude'] = $val['longitude'];
        $res1['lat_long'][$val['sales_nid']]['bill_no'] = $val['bill_no'];
        $res1['lat_long'][$val['sales_nid']]['party_code'] = $val['party_code'];
      }
    }
    if (!isset($unique_sku_co[$val['sku_code']])) {
      $unique_sku_co[$val['sku_code']] = 1;
      $res1['sku_count'] += 1;
    }

    $cld = (int) ($val['total_qty'] / $val['field_unit_case_units__value']);
    $loose = $val['total_qty'] % $val['field_unit_case_units__value'];
    if ($cld >= 1) {
      if (!isset($unique_sku['cld'][$val['sku_code']])) {
        $unique_sku['cld'][$val['sku_code']] = 1;
        $res1['cld'] += 1;
      }
      $res1['cld_value'] += ($val['sum_net_sales_value'] * $cld * $val['field_unit_case_units__value']);
    }
    if ($loose >= 1) {
      if (!isset($unique_sku['loose'][$val['sku_code']])) {
        $unique_sku['loose'][$val['sku_code']] = 1;
        $res1['loose'] += 1;
      }
      if (!isset($bill_wise_unique_sku['bill_wise_loose'][$val['sales_nid']][$val['sku_code']])) {
        $bill_wise_unique_sku['bill_wise_loose'][$val['sales_nid']][$val['sku_code']] = 1;
        $res1['bill_wise_loose'] += 1;
      }
      $res1['loose_value'] += ($val['sum_net_sales_value'] * $loose);
    }
  }
  $res1['cld_value'] = round($res1['cld_value']);
  $res1['loose_value'] = round($res1['loose_value']);
  return $res1;
}

function _get_header_sort_data($array, $field_name) {
  uasort($array, function ($a, $b) use ($field_name) {
    if ($a[$field_name] == $b[$field_name]) {
      return 0;
    }
    return ($a[$field_name] < $b[$field_name]) ? -1 : 1;
  });
  return $array;
}

/**
 * Submit handler for create trip
 * @param type $form
 * @param type $form_state
 */
function _create_van_validate($form, &$form_state) {
  $dis_id = $form_state['values']['dis_id'];
  $form_state['error_occur'] = 0;
  $trip_execution_date = strtotime($form_state['values']['trip_execution_date']);

  // Trip Execution Date validation
  if (empty($trip_execution_date)) {
    $form_state['error_occur'] = 1;
    drupal_set_message(t('Trip execution date should not be empty.'), 'error');
  }
  else {
    $today = strtotime(date('Y-m-d 00:00:00'));
    if ($trip_execution_date < $today) {
      $form_state['error_occur'] = 1;
      drupal_set_message(t('Trip execution date cannot be a previous date.'), 'error');
    }
  }

  $hht_array = array();
  foreach ($form_state['values']['vp_final_table'] as $group => $val) {
    $cashier = trim($val['cashier_list']);
    $vehicle_number = trim($val['vehicle_list']);
    $hht_id = trim($val['hht_id_list']);

    if (empty($hht_id)) {
      $form_state['error_occur'] = 1;
      drupal_set_message(t('HHT id should not be empty.'), 'error');
    }

    // Validation for HHT ID
    if (!empty($hht_id) && !empty($trip_execution_date)) {
      if (empty(validate_hht_id($trip_execution_date, $hht_id))) {
        $form_state['error_occur'] = 1;
        drupal_set_message(t("HHT id: $hht_id is invalid or already exists for this date"), 'error');
      }
    }

    // Validation for Cashier
    if (!empty($cashier) && empty(validate_cashier($cashier))) {
      $form_state['error_occur'] = 1;
      drupal_set_message(t('Please enter a valid cashier'), 'error');
    }

    // Validation for Vehicle
    if (!empty($vehicle_number) && empty(validate_vehicle_number($vehicle_number))) {
      $form_state['error_occur'] = 1;
      drupal_set_message(t('Please enter a valid vehicle number'), 'error');
    }

    $hht_array[] = $hht_id;
  }

  // Check duplicacy of HHT ID in more than one van.
  if (!empty(array_filter($hht_array))) {
    $duplicate_values = array_count_values($hht_array);
    foreach ($duplicate_values as $van => $count) {
      if ($count >= 2) {
        $form_state['error_occur'] = 1;
        drupal_set_message(t("Please select HHT ID $van only in one Van."), 'error');
      }
    }
  }
}

/**
 * 
 * @param type $form
 * @param type $form_state
 */
function _check_duplicates_validate($form, &$form_state) {
  $dis_id = $form_state['values']['dis_id'];
  $form_state['error_occur'] = 0;

  // Same HUL Code should not be present in multiple van
  $data = $form_state['values']['vp_final_table'];
  $res = array();
  if (!empty($data)) {
    foreach ($data as $van => $val1) {
      $data_sales_nids = json_decode($form_state['complete form']['submit_vp']['vp_final_table']['#rows'][$van]['bill_no']['data-value'], 1);
      $sales_nids = array_keys($data_sales_nids);
      if (!empty($data_sales_nids)) {
        $query = db_select('node', 'n');
        $query->condition('n.type', 'hul_code_master');
        $query->condition('n.uid', $dis_id);

        $query->join('field_data_field_hul_code_reference', 'hcr', 'hcr.field_hul_code_reference_nid = n.nid');
        $query->join('node', 'n2', 'n2.nid = hcr.entity_id');
        $query->condition('n2.type', 'outlet_master');
        $query->condition('n2.uid', $dis_id);

        $query->join('field_data_field_party_code_reference', 'pcr', 'pcr.field_party_code_reference_nid = n2.nid');
        $query->join('node', 'n3', 'n3.nid = pcr.entity_id');
        $query->condition('n3.type', 'sales_register_data');
        $query->condition('n3.uid', $dis_id);
        $query->condition('n3.nid', $sales_nids, 'IN');
        $query->addField('n3', 'title', 'bill_number');
        $query->addField('n', 'title', 'party_code');

        $res[$van] = $query->execute()->fetchAllKeyed(0, 1);
      }
    }
  }

  $error_arr = array();
  $res_copy = $res; // Compare with each van for duplicate HUL code
  foreach ($res as $van => $val) {
    foreach ($res_copy as $van_copy => $val_copy) {
      if ($van != $van_copy) {// Don't compare in same van
        $duplicate_value = array_intersect($val, $val_copy);
        if (!empty($duplicate_value)) {// Store duplicate value if found
          if (!isset($error_arr[$van])) {
            $error_arr[$van] = array();
          }
          $error_arr[$van] += $duplicate_value;
        }
      }
    }
  }

  if (!empty($error_arr)) {
    $form_state['error_occur'] = 1;
    drupal_set_message("Same HUL code bill numbers should be in the same van", 'warning');
  }else{
    drupal_set_message("No Duplicates!!");
  }
  foreach ($error_arr as $van => $value) {
    foreach ($value as $bill => $hul_code) {
      drupal_set_message("Van: $van , Bill Number : $bill ,Hul Code : $hul_code", 'warning');
    }
  }
}

/**
 * 
 * @param type $form
 * @param array $form_state
 */
function _check_duplicates_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
}

/**
 * 
 * @param type $form
 * @param array $form_state
 */
function sync_parkstop_time_distance($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler for create trip
 * @param type $form
 * @param type $form_state
 */
function _create_van_submit($form, &$form_state) {
  // logic to create trip
  if (empty($form_state['error_occur'])) {
    $dis_id = $form_state['values']['dis_id'];
    $user_id = $form_state['values']['user_id'];
    $data = $form_state['values']['vp_final_table'];
    $trip_execution_date = $form_state['values']['trip_execution_date'];
    if (!empty($data)) {
      foreach ($data as $van => $val1) {
        $disp_planning[$van]['bill_count'] = $form_state['complete form']['submit_vp']['vp_final_table']['#rows'][$van]['bill_count'];
        $disp_planning[$van]['bill_value'] = str_replace(",", "", $form_state['complete form']['submit_vp']['vp_final_table']['#rows'][$van]['bill_value']);
        $disp_planning[$van]['bill_array'] = json_decode($form_state['complete form']['submit_vp']['vp_final_table']['#rows'][$van]['bill_no']['data-value'], 1);

        //New Field on TRIP
        $disp_planning[$van]['loose'] = $form_state['complete form']['submit_vp']['vp_final_table']['#rows'][$van]['loose'];
        $disp_planning[$van]['cld'] = $form_state['complete form']['submit_vp']['vp_final_table']['#rows'][$van]['cld'];
        $disp_planning[$van]['sum_kg'] = $form_state['complete form']['submit_vp']['vp_final_table']['#rows'][$van]['sum_kg'];
        $disp_planning[$van]['sum_cft'] = $form_state['complete form']['submit_vp']['vp_final_table']['#rows'][$van]['sum_cft'];
        $disp_planning[$van]['area_codes'] = $form_state['complete form']['submit_vp']['vp_final_table']['#rows'][$van]['area_codes'];

        $disp_planning[$van]['field_sku_count'] = $form_state['complete form']['submit_vp']['vp_final_table']['#rows'][$van]['sku_count'];
        $disp_planning[$van]['field_bill_loose_lines'] = $form_state['complete form']['submit_vp']['vp_final_table']['#rows'][$van]['bill_wise_loose'];
        $disp_planning[$van]['field_cld_lines_value'] = $form_state['complete form']['submit_vp']['vp_final_table']['#rows'][$van]['cld_value'];
        $disp_planning[$van]['field_loose_lines_value'] = $form_state['complete form']['submit_vp']['vp_final_table']['#rows'][$van]['loose_value'];

//***************************************** PREVAN DATA START****************************************//
        /*        $todays_date = strtotime(date('Y-m-d'));

          $txn = db_transaction();

          $query = db_select('prevan_sequence', 'ts');
          $query->forUpdate();
          $query->addField('ts', 'next_id');
          $query->condition('ts.distributor_id', $dis_id);
          $query->condition('ts.sequencing_date', $todays_date);
          $prevan_trip_id = $query->execute()->fetchField();

          if ($prevan_trip_id) {
          // Increment the next_value.
          db_update('prevan_sequence')
          ->fields(array('next_id' => $prevan_trip_id + 1))
          ->condition('distributor_id', $dis_id)
          ->condition('sequencing_date', $todays_date)
          ->execute();
          }
          else {
          $prevan_trip_id = 1;
          $insert = db_insert('prevan_sequence')
          ->fields(array(
          'distributor_id' => $dis_id,
          'next_id' => $prevan_trip_id + 1,
          'sequencing_date' => $todays_date
          ));
          $insert->execute();
          }

          $today_date = date('dmy');

          // prevan_data node creation
          $node = new stdClass();
          $node->title = "PV$prevan_trip_id-$today_date";
          $node->type = "prevan_data";
          node_object_prepare($node); // Sets some defaults. Invokes hook_prepare() and hook_node_prepare().
          $node->language = LANGUAGE_NONE; // Or e.g. 'en' if locale is enabled
          $node->uid = $user_id;
          $node->status = 1; //(1 or 0): published or not
          $node->comment = 0; // 0 = comments disabled, 1 = read only, 2 = read/write
          $node->field_prevan_bill_count[$node->language][0]['value'] = $disp_planning[$van]['bill_count'];
          $node->field_prevan_bill_value[$node->language][0]['value'] = (float) $disp_planning[$van]['bill_value'];
          $node->field_prevan_vehicle_van[$node->language][0]['value'] = $val1['vehicle_list'];
          //$node->field_prevan_delivery_lead[$node->language][0]['value'] = $form_state['values']['delivery_lead'][$van];
          //$node->field_prevan_team_lead[$node->language][0]['value'] = $form_state['values']['team_lead'][$van];
          // field_prevan_total_distance
          $node->field_prevan_total_distance[$node->language][0]['value'] = (float) $form_state['time_dist'][$van]['dist'];

          // field_prevan_travel_time
          $node->field_prevan_travel_time[$node->language][0]['value'] = (float) $form_state['time_dist'][$van]['time'];

          //$node->field_trip_creation_date[$node->language][0]['value'] = strtotime(date('Y/m/d 00:00:00'));
          // $node->field_trip_executon_date[$node->language][0]['value'] = $trip_execution_date;
          // $node->field_hht_id[$node->language][0]['nid'] = get_nid_from_hht_id($hht_id);

          if ($seq_flag) {
          //  $node->field_sequence_method[$node->language][0]['value'] = 1;
          //  $node->field_total_travel_time[$node->language][0]['value'] = round($total_travel_time / 60, 2);
          //  $node->field_total_distance[$node->language][0]['value'] = round($total_distance, 4);
          }
          else {
          // $node->field_sequence_method[$node->language][0]['value'] = 0;
          }

          $node = node_submit($node); // Prepare node for saving
          node_save($node);

          $prevan_nid = $node->nid; */
//***************************************** PREVAN DATA END****************************************//
//************************************ DISPATCH DATA START****************************************//
        $txn = db_transaction();

        $query = db_select('trip_id_sequence', 'ts');
        $query->forUpdate();
        $query->addField('ts', 'next_id');
        $query->condition('ts.distributor_id', $dis_id);
        $trip_id = $query->execute()->fetchField();


        if ($trip_id) {
          // Increment the next_value.
          db_update('trip_id_sequence')
            ->fields(array('next_id' => $trip_id + 1))
            ->condition('distributor_id', $dis_id)
            ->execute();
        }
        else {
          $trip_id = 1;
          $insert = db_insert('trip_id_sequence')
            ->fields(array(
            'distributor_id' => $dis_id,
            'next_id' => $trip_id + 1
          ));
          $insert->execute();
        }
        
        
        $sales_nids = array_keys($disp_planning[$van]['bill_array']);
        $query = db_select('node', 'n');
        $query->condition('n.type', 'sales_register_data');
        $query->condition('n.uid', $dis_id);
        $query->condition('n.nid', $sales_nids, 'IN');
        $query->join('field_data_field_bill_value', 'bv', 'bv.entity_id = n.nid');
        $query->addExpression('ROUND(SUM(bv.field_bill_value_value))', 'bill_value');
        $dispatch_bill_value = $query->execute()->fetchField();
        
        // dispatch_data node creation
        $node1 = new stdClass();
        $node1->title = $trip_id;
        $node1->type = "dispatch_data";
        node_object_prepare($node1); // Sets some defaults. Invokes hook_prepare() and hook_node_prepare().
        $node1->language = LANGUAGE_NONE; // Or e.g. 'en' if locale is enabled
        $node1->uid = $user->uid;
        $node1->status = 1; //(1 or 0): published or not
        $node1->comment = 0; // 0 = comments disabled, 1 = read only, 2 = read/write
        $node1->field_bill_count[LANGUAGE_NONE][0]['value'] = $disp_planning[$van]['bill_count'];
        $node1->field_dispatch_bill_value[LANGUAGE_NONE][0]['value'] = $dispatch_bill_value;
        
        //dispatch_bill_net_sales_value
        $node1->field_net_sales_bill_value[LANGUAGE_NONE][0]['value'] = $disp_planning[$van]['bill_value'];
        
        $node1->field_cashier_name[LANGUAGE_NONE][0]['value'] = $val1['cashier_list'];
        $node1->field_vehicle_no[LANGUAGE_NONE][0]['value'] = $val1['vehicle_list'];
        $node1->field_trip_id[LANGUAGE_NONE][0]['value'] = $trip_id;
        $node1->field_trip_creation_date[LANGUAGE_NONE][0]['value'] = strtotime(date('Y/m/d 00:00:00'));
        $node1->field_trip_executon_date[LANGUAGE_NONE][0]['value'] = $trip_execution_date;
        $node1->field_hht_id[LANGUAGE_NONE][0]['nid'] = get_nid_from_hht_id($val1['hht_id_list']);

        if (!empty($form_state['time_dist'][$van]['dist']) && !empty($form_state['time_dist'][$van]['time'])) {
          $node1->field_sequence_method[LANGUAGE_NONE][0]['value'] = 1; // api
          $node1->field_total_travel_time[LANGUAGE_NONE][0]['value'] = $_SESSION['park_stop'][$van]['time'] ? $_SESSION['park_stop'][$van]['time'] : $form_state['time_dist'][$van]['time'];
          $node1->field_total_distance[LANGUAGE_NONE][0]['value'] = $_SESSION['park_stop'][$van]['dist'] ? $_SESSION['park_stop'][$van]['dist'] : $form_state['time_dist'][$van]['dist'];
          $node1->field_delivery_boy_count[LANGUAGE_NONE][0]['value'] = $_SESSION['park_stop'][$van]['del_boy'] ? $_SESSION['park_stop'][$van]['del_boy'] :$form_state['time_dist'][$van]['del_boy'];
        }
        else {
          $node1->field_sequence_method[LANGUAGE_NONE][0]['value'] = 0;
        }

        //New data
        $node1->field_entered_prevan[LANGUAGE_NONE][0]['value'] = $van;
        $node1->field_cld_product_lines[LANGUAGE_NONE][0]['value'] = $disp_planning[$van]['cld'];
        $node1->field_loose_product_lines[LANGUAGE_NONE][0]['value'] = $disp_planning[$van]['loose'];
        $node1->field_bill_weight[LANGUAGE_NONE][0]['value'] = $disp_planning[$van]['sum_kg'];
        $node1->field_bill_volume[LANGUAGE_NONE][0]['value'] = $disp_planning[$van]['sum_cft'];
        $node1->field_bill_area_codes[LANGUAGE_NONE][0]['value'] = $disp_planning[$van]['area_codes'];

        $node1->field_sku_count[LANGUAGE_NONE][0]['value'] = $disp_planning[$van]['field_sku_count'];
        $node1->field_bill_loose_lines[LANGUAGE_NONE][0]['value'] = $disp_planning[$van]['field_bill_loose_lines'];
        $node1->field_cld_lines_value[LANGUAGE_NONE][0]['value'] = $disp_planning[$van]['field_cld_lines_value'];
        $node1->field_loose_lines_value[LANGUAGE_NONE][0]['value'] = $disp_planning[$van]['field_loose_lines_value'];

        $node1 = node_submit($node1); // Prepare node for saving
        node_save($node1);
        $trip_nid = $node1->nid;
        // update parkstop response log
        if (!empty($form_state['time_dist'][$van]['dist']) && !empty($form_state['time_dist'][$van]['time'])) {
          update_parkstop_plotting_log($van, $trip_id, $dis_id);
        }
        //************************************* DISPATCH DATA END ********************************************//

        $count = 1;
        foreach ($disp_planning[$van]['bill_array'] as $nid => $value2) {
          $node = node_load($nid);
          if (!empty($form_state['time_dist'][$van]['dist']) && !empty($form_state['time_dist'][$van]['time'])) {
            $node->field_bill_sequence[LANGUAGE_NONE][0]['value'] = $value2['park_seq'] ? $value2['park_seq'] : $count; // $count has no use. kept for safety
            $node->field_drs_sequence[LANGUAGE_NONE][0]['value'] = $value2['sequence'] ? $value2['sequence'] : $count++; //Increament the counter once
            // calculate time and distance
            //$node->field_prev_arrival_time[LANGUAGE_NONE][0]['value'] = $value2['prev_travel_time'];
            //$node->field_prev_distance[LANGUAGE_NONE][0]['value'] = $value2['prev_distance'];
          }else{
            $node->field_bill_sequence[LANGUAGE_NONE][0]['value'] = $count;
            $node->field_drs_sequence[LANGUAGE_NONE][0]['value'] = $count++; //Increament the counter once
            // calculate time and distance
            //$node->field_prev_arrival_time[LANGUAGE_NONE][0]['value'] = '';
            //$node->field_prev_distance[LANGUAGE_NONE][0]['value'] = '';
          }
          if (isset($node->field_delivery_pending[LANGUAGE_NONE])) {
            $node->field_delivery_pending = array();
          }
          //$node->field_prevan_reference[LANGUAGE_NONE][0]['nid'] = $prevan_nid;
          $node->field_trip_id_reference[LANGUAGE_NONE][0]['nid'] = $trip_nid;
          node_save($node);
        }
        $all_area_codes = $disp_planning[$van]['area_codes'];
        drupal_set_message(t('Successfully created Trip id: ' . $trip_id . ' for Area Codes: ' . $all_area_codes));
        unset($area_codes);
      }
    }
  }
  else {
    $form_state['rebuild'] = TRUE;
  }
}
