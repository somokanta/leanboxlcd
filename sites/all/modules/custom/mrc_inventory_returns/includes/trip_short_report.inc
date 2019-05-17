<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
function trip_short_excess_form($form, &$form_state) {
  //$form['#attached']['js'][] = drupal_get_path('module', 'mrc_inventory_returns') . '/js/tripshort.js';
  $form['#attached']['css'][] = drupal_get_path('module', 'mrc_inventory_returns') . '/css/vrs_confirmation.css';

  if (isset($_GET['trip_id'])) {
    $trip_id = $_GET['trip_id'];
  }

  //$dispatch_date = date('Y-m-d');
  if (isset($_GET['from_dispatch_date'])) {
    $from_dispatch_date = $_GET['from_dispatch_date'];
  }
  if (isset($_GET['to_dispatch_date'])) {
    $to_dispatch_date = $_GET['to_dispatch_date'];
  }
  //$confirm_date = date('Y-m-d');
  if (isset($_GET['confirm_date'])) {
    $confirm_date = $_GET['confirm_date'];
  }
  if (isset($_GET['trip_status'])) {
    $trip_status = $_GET['trip_status'];
  }
  $form['trip_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Trip ID*'),
    '#size' => 20,
    '#default_value' => $trip_id,
    '#maxlength' => 50,
    '#attributes' => array('class' => array('trip-id'), 'id' => 'trip-id'),
    '#prefix' => '<div class="form_row form_row2" id="trip_id"><div class="col_3">',
    '#suffix' => '</div></div>',
  );


  $form['from_dispatch_date'] = array(
    '#type' => 'date_popup',
    '#title' => t('From Dispatch Date'),
    '#date_format' => 'Y-m-d',
    '#attributes' => array('class' => array('dispatch_date')),
    '#date_label_position' => '',
    '#date_year_range' => '-20:+2',
    '#default_value' => $from_dispatch_date,
    '#size' => 30,
    '#prefix' => '<div class="stat_3" id="dis_date">',
    '#suffix' => '</div>',
  );
  
  $form['to_dispatch_date'] = array(
    '#type' => 'date_popup',
    '#title' => t('To Dispatch Date'),
    '#date_format' => 'Y-m-d',
    '#attributes' => array('class' => array('dispatch_date')),
    '#date_label_position' => '',
    '#date_year_range' => '-20:+2',
    '#default_value' => $to_dispatch_date,
    '#size' => 30,
    '#prefix' => '<div class="stat_3" id="dis_date">',
    '#suffix' => '</div>',
  );

  $trip_type_field = field_info_field('field_vrs_status');
  $options = list_allowed_values($trip_type_field);
  $form['trip_status'] = array(
    '#type' => 'select',
    '#title' => t('Trip Status'),
    '#options' => array('' => 'Select') + $options,
    '#attributes' => array('class' => array('dispatch_date')),
    '#default_value' => $trip_status,
    '#prefix' => '<div class="stat_3">',
    '#suffix' => '</div>'
  );

  $form['confirm_date'] = array(
    '#type' => 'date_popup',
    '#title' => t('Delivery Confirm Date'),
    '#date_format' => 'Y-m-d',
    '#date_label_position' => '',
    '#date_year_range' => '-20:+2',
    '#size' => 30,
    '#default_value' => $confirm_date,
    '#prefix' => '<div class="stat_3">',
    '#suffix' => '</div>'
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Apply'),
    '#validate' => array('trip_short_excess_form_validate'),
    '#submit' => array('trip_short_excess_form_submit'),
  );
  return $form;
}

function trip_short_excess_form_validate($form, &$form_state) {
  $from_dispatch_date = $form_state['values']['from_dispatch_date'];
  $to_dispatch_date = $form_state['values']['to_dispatch_date'];
  $trip_id = $form_state['values']['trip_id'];
  $trip_status = $form_state['values']['trip_status'];
  $confirm_date = $form_state['values']['confirm_date'];
  if (empty($from_dispatch_date && $to_dispatch_date) && empty($trip_id) && (empty($trip_status) && empty($confirm_date))) {
    drupal_set_message(t('Please select any one filter'), 'error');
  }
}

function trip_short_excess_form_submit($form, &$form_state) {
  $from_dispatch_date = $form_state['values']['from_dispatch_date'];
  $to_dispatch_date = $form_state['values']['to_dispatch_date'];
  $trip_id = $form_state['values']['trip_id'];
  $trip_status = $form_state['values']['trip_status'];
  $confirm_date = $form_state['values']['confirm_date'];

  if (($form_state['values']["op"] == "Apply") && (arg(0) == 'reco-mismatch-report')) {
    drupal_goto('reco-mismatch-report', array('query' => array('from_dispatch_date' => $from_dispatch_date, 'to_dispatch_date' => $to_dispatch_date, 'trip_id' => $trip_id, 'trip_status' => $trip_status, 'confirm_date' => $confirm_date)));
  }
}

function trip_short_excess_form_callback() {
  $output = '';
  global $user;
  $distributor_id = change_feeds_import_get_distributor_id($user->uid);

  if (isset($_GET['trip_id'])) {
    $trip_id = $_GET['trip_id'];
  }
  if (isset($_GET['from_dispatch_date'])) {
    $from_dispatch_date = $_GET['from_dispatch_date'];
  }
  if (isset($_GET['to_dispatch_date'])) {
    $to_dispatch_date = $_GET['to_dispatch_date'];
  }
  if (isset($_GET['confirm_date'])) {
    $confirm_date = $_GET['confirm_date'];
  }

  if (isset($_GET['trip_status'])) {
    $trip_status = $_GET['trip_status'];
  }

  if (arg(1) != 'export') {
    $form = drupal_render(drupal_get_form('trip_short_excess_form'));
  }
  if (!empty($trip_id) || !empty($from_dispatch_date && $to_dispatch_date)  || !empty($confirm_date) || !empty($trip_status)) {
		 
    $query = db_select('dispatch_planning_api_data', 'ad');
    $query->condition('ad.distributor_id', $distributor_id, '=');
    if (!empty($trip_id)) {
      $query->condition('ad.runsheet_number', $trip_id, '=');
    }
    if (!empty($from_dispatch_date) && !empty($to_dispatch_date)) {
      $query->condition('ad.date', array($from_dispatch_date, $to_dispatch_date), 'BETWEEN');
    }
    $query->leftjoin('vrs_trip_total', 'vtt', 'ad.runsheet_number = vtt.trip_id AND ad.distributor_id = vtt.distributor_id');
    $query->join('dispatch_planning_item_details', 'dit', 'ad.id = dit.trip_bill_reference');
    //$query->leftjoin('excess_inventory_table', 'eit', 'ad.runsheet_number = eit.trip_id AND ad.distributor_id = eit.distributor_id');
    $query->join('node', 'n', "n.title = ad.runsheet_number AND n.uid = ad.distributor_id");
    $query->condition('n.type', 'dispatch_data');
    $query->leftjoin('field_data_field_vrs_status', 'vrs', 'n.nid = vrs.entity_id');
    if (!empty($trip_status)) {
      $query->condition('vrs.field_vrs_status_value', $trip_status, '=');
    }
    if (!empty($confirm_date)) {
			$c_date = strtotime($confirm_date);
			$c_end_date = strtotime($confirm_date) + 86399;
			$query->join('inventory_versioning_api_data', 'vad', 'ad.runsheet_number = vad.runsheet_number');
			$query->condition('vad.version_created_time', array($c_date, $c_end_date), 'BETWEEN');
    }
    $query->fields('ad', array('runsheet_number'));
    $query->fields('vtt', array('total_cashier_cash_short'));
    $query->addExpression("SUM(distinct(CASE WHEN dit.res_cashier_short > 0 AND dit.res_cashier_short != '' THEN dit.res_cashier_short*dit.tur END))", 'inventory_short_value'); //formula changed (EARLIER: dit.res_cashier_short*dit.sum_net_sales_value)
   // $query->addExpression("SUM(distinct(eit.excess_qty * eit.tur))", 'total_excess_value'); //formula changed (EARLIER: eit.excess_qty * eit.mrp * 0.9)
		//$query->addExpression("ROUND(SUM(distinct(CASE WHEN eit.tur != '' THEN (eit.excess_qty * eit.tur) ELSE (eit.excess_qty * eit.mrp * 0.9) END)),2)", 'total_excess_value');
		
   // $query->addExpression("SUM(distinct(CASE WHEN eit.credit_to_cashier = 1 THEN (eit.excess_qty * eit.tur) END))", 'excess_value_credited_to_cashier'); //formula changed (EARLIER: eit.excess_qty * eit.mrp * 0.9)
		//$query->addExpression("SUM(distinct(CASE WHEN eit.credit_to_cashier = 1 AND  eit.tur != '' THEN (eit.excess_qty * eit.tur) ELSE (eit.excess_qty * eit.mrp * 0.9) END))", 'excess_value_credited_to_cashier');
		
		$query->addExpression("SUM(distinct(CASE WHEN dit.claimed_godown_short > 0 THEN(dit.claimed_godown_short * dit.tur) END))", 'pending_claimed_godown_short_value'); //formula changed (EARLIER: dit.claimed_godown_short * dit.sum_net_sales_value)

    $query->groupBy('ad.runsheet_number');
    $res = $query->execute()->fetchAll();
		if (!empty($res)) {
			foreach($res as $key=>$val) {
				$trips[$val->runsheet_number] = $val->runsheet_number;
			}
		}
			// get excess value from excess inventory
		$excess_data= get_trip_wise_excess_value($trips,$distributor_id);		
    foreach ($res as $val) {
			$row['trip_id'] = $val->runsheet_number;
			$row['inventory_short_value'] = round($val->inventory_short_value, 2);
			$row['pending_claimed_godown_short_value'] = round($val->pending_claimed_godown_short_value, 2);
			$row['total_cashier_cash_short'] = round($val->total_cashier_cash_short, 2);
			$row['total_excess_value'] = round($excess_data[$val->runsheet_number]['total_excess_value'],2); //round($val->total_excess_value, 2);
			$row['excess_value_credited_to_cashier'] = round($excess_data[$val->runsheet_number]['excess_value_credited_to_cashier'],2); //round($val->excess_value_credited_to_cashier, 2);
			$rows[] = $row;
		}
		$header = array('Trip Id', 'Inventory Short Value', 'Pending Claimed GS Value','Cashier Cash Short', 'Total Excess Value', 'Excess Value Credited to Cashier');
    $per_page = 10;
    // Initialize the pager
    $current_page = pager_default_initialize(count($rows), $per_page);
    // Split your list into page sized chunks
    $chunks = array_chunk($rows, $per_page, TRUE);
    // Show the appropriate items from the list
    $output = theme('table', array('header' => $header, 'rows' => $chunks[$current_page]));
    // Show the pager
    $output .= theme('pager', array('quantity', count($rows)));

    $html = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('width' => '100%')));
    cache_set('reco-mismatch-report' . session_id(), $html, 'cache', time() + 60 * 10);
    $output .= l(t('XLS'), 'reco-mismatch-report/export', array('query' => array($data)));
  }
  return $form . $output;
  $form_state['rebuild'] = TRUE;
}

function trip_short_excess_report_export() {
  $file = "reco-mismatch-report.xls";
  $data = cache_get('reco-mismatch-report' . session_id());
  if (empty($data)) {
    $table = trip_short_excess_form_callback();
  }
  else {
    $table = $data->data;
  }
  header("Content-type: application/vnd.ms-excel");
  header("Content-Disposition: attachment; filename=$file");
  echo $table;
}


function get_trip_wise_excess_value($all_trips, $distributor_id) {
	if (!empty($all_trips) && !empty($distributor_id)) {
		$query = db_select('excess_inventory_table', 'eit');
		$query->condition('eit.distributor_id', $distributor_id, '=');
		$query->condition('eit.trip_id', $all_trips, 'IN');
		$query->groupBy('eit.trip_id');
		$query->fields('eit', array('trip_id'));
		//$query->fields('eit', array('credit_to_cashier'));
		$query->addExpression("SUM(CASE WHEN eit.tur != '' THEN (eit.excess_qty * eit.tur) ELSE (eit.excess_qty * eit.mrp * 0.9) END)", 'total_excess_value');
		
		$query->addExpression("SUM(CASE WHEN eit.credit_to_cashier = 1 AND  eit.tur != '' THEN (eit.excess_qty * eit.tur) ELSE (CASE WHEN eit.credit_to_cashier = 1 AND  eit.tur IS Null THEN (eit.excess_qty * eit.mrp * 0.9) END) END)", 'excess_value_credited_to_cashier');
		
		
		$res = $query->execute()->fetchAll(PDO::FETCH_UNIQUE | PDO::FETCH_ASSOC);
	}
	return $res;
}
