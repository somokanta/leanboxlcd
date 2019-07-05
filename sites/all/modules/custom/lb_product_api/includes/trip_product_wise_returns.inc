<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function trip_product_wise_returns_form($form,&$form_state) {
    $form['#attached']['css'][] = drupal_get_path('module', 'lb_product_api') . '/css/vss.css';

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
  $form['trip_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Trip ID'),
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
  
    $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Apply'),
    '#validate' => array('trip_product_wise_returns_form_validate'),
    '#submit' => array('trip_product_wise_returns_form_submit'),
  );
 return $form;
 
}

function trip_product_wise_returns_form_validate($form, &$form_state) {
  $from_dispatch_date = $form_state['values']['from_dispatch_date'];
  $to_dispatch_date = $form_state['values']['to_dispatch_date'];
  $trip_id = $form_state['values']['trip_id'];
  if (empty($from_dispatch_date && $to_dispatch_date) && empty($trip_id) && (empty($trip_status) && empty($confirm_date))) {
    drupal_set_message(t('Please select any one filter'), 'error');
  }
}

function trip_product_wise_returns_form_submit($form, &$form_state){
  $from_dispatch_date = $form_state['values']['from_dispatch_date'];
  $to_dispatch_date = $form_state['values']['to_dispatch_date'];
  $trip_id = $form_state['values']['trip_id'];

  if (($form_state['values']["op"] == "Apply") && (arg(0) == 'trip-product-wise-returns')) {
    drupal_goto('trip-product-wise-returns', array('query' => array('from_dispatch_date' => $from_dispatch_date, 'to_dispatch_date' => $to_dispatch_date, 'trip_id' => $trip_id)));
  }
}

function trip_product_wise_returns_form_callback(){
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
   if (arg(1) != 'export') {
    $form = drupal_render(drupal_get_form('trip_product_wise_returns_form'));
  }
  if (!empty($trip_id) || !empty($from_dispatch_date && $to_dispatch_date)) {
		 
    $query = db_select('dispatch_planning_api_data', 'ad');
    $query->condition('ad.distributor_id', $distributor_id, '=');
    if (!empty($trip_id)) {
      $query->condition('ad.runsheet_number', $trip_id, '=');
    }
    if (!empty($from_dispatch_date) && !empty($to_dispatch_date)) {
      $query->condition('ad.date', array($from_dispatch_date, $to_dispatch_date), 'BETWEEN');
    }
    $query->join('dispatch_planning_item_details', 'dit', 'ad.id = dit.trip_bill_reference');
    $query->leftjoin('excess_inventory_table', 'eit', 'ad.runsheet_number = eit.trip_id AND ad.distributor_id = eit.distributor_id');
    $query->fields('ad', array('runsheet_number')); 
    $query->fields('ad', array('dispatch_date'));
    $query->fields('dit', array('mrp'));
    $query->fields('dit', array('sku_code'));
    $query->fields('dit', array('product_name'));
    $query->addExpression("SUM(distinct(CASE WHEN (dit.return_qty != '') AND (dit.return_qty != 0) AND (dit.return_qty > 0) THEN dit.return_qty  END))", 'total_return_qty');
    $query->addExpression("SUM(distinct(CASE WHEN dit.res_cashier_short != '' AND dit.res_cashier_short != 0 AND dit.res_cashier_short > 0 THEN dit.res_cashier_short END))", 'total_cashier_cash_short');
    $query->addExpression("SUM(distinct(CASE WHEN dit.res_godown_short != '' AND dit.res_godown_short != 0 AND dit.res_godown_short > 0  THEN dit.res_godown_short END))", 'total_godown_short');
    $query->addExpression("SUM(distinct(CASE WHEN dit.claimed_godown_short != '' AND dit.claimed_godown_short > 0 THEN(dit.claimed_godown_short) END))", 'pending_claimed_godown_short'); //formula changed (EARLIER: dit.claimed_godown_short * dit.sum_net_sales_value)
    $query->groupBy('dit.sku_code');
    $query->groupBy('dit.mrp');
    $res = $query->execute()->fetchAll();
		if (!empty($res)) {
			foreach($res as $key=>$val) {
				$skus[$val->sku_code] = $val->sku_code;
			}
		}
			// get excess value from excess inventory
		$excess_data= get_sku_wise_excess_quantity($skus,$distributor_id);		
    foreach ($res as $val) {
			$row['trip_id'] = $val->runsheet_number;
                        $row['dispatch_date'] = date('d-M-y ', $val->dispatch_date);
                        $row['product_name'] = $val->product_name;
                        $row['sku_code'] = $val->sku_code;
                        $row['mrp'] = round($val->mrp, 2);
			$row['total_return_qty'] = round($val->total_return_qty, 2);
                        $row['total_excess_quantity'] = round($excess_data['total_excess_quantity'],2);
                        $row['total_cashier_cash_short'] = round($val->total_cashier_cash_short, 2);
                        $row['total_godown_short'] = round($val->total_godown_short, 2);
                        $row['pending_claimed_godown_short'] = round($val->pending_claimed_godown_short, 2);
                        
		if(($row['total_return_qty'] != 0) || ($row['total_excess_quantity'] != 0) || ($row['total_cashier_cash_short'] != 0) || ($row['total_godown_short'] != 0) || ($row['pending_claimed_godown_short'] != 0)){
                    $rows[] = $row;
                }		
    }
		$header = array('Trip Id', 'Dispatch Date', 'Product Name','Sku7 Code', 'MRP', 'Returned Quantity', 'Excess Quantity', 'Cashier Short','Godown Short','Pending Claimed Godown Short');
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
    cache_set('trip-product-wise-returns' . session_id(), $html, 'cache', time() + 60 * 10);
    $output .= l(t('XLS'), 'trip-product-wise-returns/export', array('query' => array($data)));
  }
  return $form . $output;
  $form_state['rebuild'] = TRUE;
}

function trip_product_wise_report_export() {
  $file = "trip_product_wise_report.xls";
  $data = cache_get('trip-product-wise-returns' . session_id());
  if (empty($data)) {
    $table = trip_product_wise_returns_form_callback();
  }
  else {
    $table = $data->data;
  }
  header("Content-type: application/vnd.ms-excel");
  header("Content-Disposition: attachment; filename=$file");
  echo $table;
}
   // get skuwise or mrpwise
function get_sku_wise_excess_quantity($all_skus, $distributor_id) {
	if (!empty($all_skus) && !empty($distributor_id)) {
		$query = db_select('excess_inventory_table', 'eit');
		$query->condition('eit.distributor_id', $distributor_id, '=');
		$query->condition('eit.sku_code', $all_skus, 'IN');
		$query->groupBy('eit.sku_code');
                $query->groupBy('eit.mrp');
		//$query->fields('eit', array('excess_qty'));
		$query->addExpression("SUM(CASE WHEN eit.excess_qty != '' THEN (eit.excess_qty) END)", 'total_excess_quantity');
		
		//$query->addExpression("SUM(CASE WHEN eit.credit_to_cashier = 1 AND  eit.tur != '' THEN (eit.excess_qty * eit.tur) ELSE (CASE WHEN eit.credit_to_cashier = 1 AND  eit.tur IS Null THEN (eit.excess_qty * eit.mrp * 0.9) END) END)", 'excess_value_credited_to_cashier');
		
		
		$res = $query->execute()->fetchAll(PDO::FETCH_UNIQUE | PDO::FETCH_ASSOC);
	}
	return $res;
}


