<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function retun_vrs_form($form, &$form_state) {

  if (isset($_GET['trip_id'])) {
    $trip_id = $_GET['trip_id'];
  }
  
  $dispatch_date = date('Y-m-d');
  if (isset($_GET['dispatch_date'])) {
    $dispatch_date = $_GET['dispatch_date'];
  }

  $form['trip_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Trip ID*'),
    '#size' => 20,
    '#default_value' => $trip_id,
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
    '#title' => t('Dispatch Date'),
    '#date_format' => 'Y-m-d',
    '#attributes' => array('class' => array('dispatch_date')),
    '#date_label_position' => '',
    '#date_year_range' => '-20:+2',
    '#size' => 30,
    '#default_value' => $dispatch_date,
    '#prefix' => '<div class="col_3">',
    '#suffix' => '</div>'
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Apply'),
    '#submit' => array('retun_vrs_form_submit'),
  );

  return $form;
}

function retun_vrs_form_submit($form, &$form_state) {
  $dispatch_date = $form_state['values']['dispatch_date'];
  $trip_id = $form_state['values']['trip_id'];

  if (($form_state['values']["op"] == "Apply") && (arg(0) == 'return-vrs-time-report')) {
    drupal_goto('return-vrs-time-report', array('query' => array('dispatch_date' => $dispatch_date, 'trip_id' => $trip_id)));
  }
}

function return_vrs_callback() {

    global $user;
    $distributor_id = change_feeds_import_get_distributor_id($user->uid);
    $dispatch_date = date('Y-m-d');

    if (isset($_GET['trip_id'])) {
        $trip_id = $_GET['trip_id'];
    }

    if (isset($_GET['dispatch_date'])) {
        $dispatch_date = $_GET['dispatch_date'];
    }
    $logged_in_dis_id = $distributor_id;
    if (in_array("central_distributor", $user->roles)) {
        $central_distributor_id = $logged_in_dis_id;
    }
    else {
        $central_distributor_id = get_central_distributor_id($logged_in_dis_id);
    }

    if (arg(1) != 'export') {
        $form = drupal_render(drupal_get_form('retun_vrs_form'));
    }

    $get_subdivision_nids = check_product_validation($distributor_id);
    if (!empty($trip_id)) {
        $trip_ids = array($trip_id);
        $distributor_ids = array(get_distributorid_of_trip($trip_id));
    }
    else if (!empty($dispatch_date)) {
        $trip_ids = _get_trip_ids_using_dispatch_date($dispatch_date);
        $distributor_ids = get_distributor_ids();
    }

    if (!empty($trip_ids)) {
        $sub_query = db_select('node', 'n1');
        $sub_query->condition('n1.uid', $distributor_ids, 'IN');
        $sub_query->condition('n1.type', 'assets_count', '=');
        $sub_query->condition('n1.title', $trip_ids, 'IN');
        $sub_query->leftjoin('field_data_field_received_asset_time', 'at', 'at.entity_id = n1.nid');
        $sub_query->fields('at', array('field_received_asset_time_value'));
        $sub_query->fields('n1', array('title'));
        $sub_query->distinct();
        $sub_query->groupBy('n1.title');
    }

    $query = db_select('dispatch_planning_api_data', 'ad');
    $query->condition('ad.distributor_id', $distributor_ids, 'IN');
    $query->condition('ad.delivery_pending_flag', 1, '!=');
    if (!empty($trip_id)) {
        $query->condition('ad.runsheet_number', $trip_id, '=');
    }
    else if (!empty($dispatch_date)) {
        $query->condition('ad.date', $dispatch_date, '=');
    }

    $query->fields('ad', array('runsheet_number'));
    $query->fields('ad', array('date'));
    $query->fields('ad', array('fieldExecutiveName'));
//  $query->leftjoin('node', 'n1', "n1.title = ad.runsheet_number and n1.type ='assets_count' and n1.uid = $distributor_id");
//  $query->leftjoin('field_data_field_received_asset_time', 'at', 'at.entity_id = n1.nid');
//  $query->fields('at', array('field_received_asset_time_value'));
    if (!empty($trip_ids)) {
        $query->leftjoin($sub_query, 'asset_count', 'asset_count.title = ad.runsheet_number');
        $query->fields('asset_count', array('field_received_asset_time_value'));
    }

    $query->join('node', 'disp_data', 'disp_data.title = ad.runsheet_number');
    $query->condition('disp_data.type', 'dispatch_data');
    $query->condition('disp_data .uid', $distributor_ids,'IN');
    $query->join('field_data_field_vehicle_arrival_date', 'vad', 'vad.entity_id = disp_data.nid');
    $query->join('field_data_field_vehicle_arrival_time', 'vat', 'vat.entity_id = disp_data.nid');
    $query->fields('vad', array('field_vehicle_arrival_date_value'));
    $query->fields('vat', array('field_vehicle_arrival_time_value'));
    $query->join('field_data_field_vrs_status', 'vs', 'vs.entity_id = disp_data.nid');
    // Sku Wise
    $query->leftjoin('field_data_field_sku_wise_return_start_time', 'sku_start_time', 'sku_start_time.entity_id = disp_data.nid');
    $query->leftjoin('field_data_field_sku_wise_return_end_time', 'sku_end_time', 'sku_end_time.entity_id = disp_data.nid');
    // Bill wise
    $query->leftjoin('field_data_field_bill_wise_return_starttime', 'bill_start_time', 'bill_start_time.entity_id = disp_data.nid');
    $query->leftjoin('field_data_field_bill_wise_return_end_time', 'bill_end_time', 'bill_end_time.entity_id = disp_data.nid');
    // Returns InCharge name

    $query->leftjoin('field_data_field_returns_incharge_name', 'returns_incharge', 'returns_incharge.entity_id = disp_data.nid');
    $query->leftjoin('field_data_field_return_dispatch_supervisor', 'dispatch_supervisor', 'dispatch_supervisor.entity_id = disp_data.nid');
    $query->leftjoin('field_data_field_resource_name', 'resource', 'resource.entity_id = dispatch_supervisor.field_return_dispatch_supervisor_nid');

    $query->fields('vs', array('field_vrs_status_value'));
    $query->fields('sku_start_time', array('field_sku_wise_return_start_time_value'));
    $query->fields('sku_end_time', array('field_sku_wise_return_end_time_value'));
    $query->fields('bill_start_time', array('field_bill_wise_return_starttime_value'));
    $query->fields('bill_end_time', array('field_bill_wise_return_end_time_value'));

    $query->leftjoin('field_data_field_dispatch_start_time', 'dst', 'dst.entity_id = disp_data.nid');
    $query->fields('dst', array('field_dispatch_start_time_value'));

    $query->addExpression('resource.field_resource_name_value', 'return_supervisor');
    $query->addExpression('returns_incharge.field_returns_incharge_name_value', 'return_incharge');

    $query->addExpression('MIN(CASE WHEN (ad.updated_flag = 1) THEN ad.updated_time END)', 'min_updated_time');
    $query->addExpression('MAX(CASE WHEN (ad.updated_flag = 1) THEN ad.updated_time END)', 'max_updated_time');
    $query->addExpression('(MAX(CASE WHEN (ad.updated_flag = 1) THEN ad.updated_time END) - MIN(CASE WHEN (ad.updated_flag = 1) THEN ad.updated_time END))', 'total_return_time');
    $query->groupBy('ad.runsheet_number');
    $query->join('dispatch_planning_item_details', 'idt', 'ad.id = idt.trip_bill_reference');
    $query->addexpression("SUM(idt.return_qty * idt.sum_net_sales_value)", 'return_value');
    $query->addexpression("(SUM(idt.return_qty * idt.sum_net_sales_value))*5", 'return_prod');
    $query->leftjoin('vrs_trip_total_history', 'vtth', 'ad.runsheet_number = vtth.trip_id AND ad.distributor_id = vtth.distributor_id');
    $query->addExpression("CASE WHEN vtth.vrs_version_no = 'V1.0' THEN vtth.vrs_start_time END", 'vrs_start_time');
    $query->leftjoin('vrs_trip_total', 'vtt', 'ad.runsheet_number = vtt.trip_id AND ad.distributor_id = vtt.distributor_id');
    $query->fields('vtt', array('vrs_end_time'));
    $query->addexpression("vtt.vrs_end_time - (CASE WHEN vtth.vrs_version_no = 'V1.0' THEN vtth.vrs_start_time END)", 'vrs_time');
    $query->fields('vtt', array('total_actual_cash_amount', 'total_actual_cheque_amount', 'total_actual_signed_bill_amount'));

    $query->join('node', 'n2', "n2.title = idt.sku_code");
    $query->condition('n2.type', 'sku7_code_master', '=');
   // $query->condition('n2.uid', $distributor_id);
    $query->leftjoin('field_data_field_product_devision_reference', 'pdr', 'pdr.entity_id = n2.nid');
    $query->addexpression("SUM(CASE WHEN pdr.field_product_devision_reference_nid IN ($get_subdivision_nids) THEN idt.return_qty END)", 'return_sku_qty_sachets');
    $query->addexpression("SUM(CASE WHEN pdr.field_product_devision_reference_nid NOT IN ($get_subdivision_nids) THEN idt.return_qty END)", 'return_sku_qty_wo_sachets');
    $query->addExpression("((vtt.total_actual_cash_amount + vtt.total_actual_cheque_amount + vtt.total_actual_signed_bill_amount)*10)/((vtt.vrs_end_time - (CASE WHEN vtth.vrs_version_no = 'V1.0' THEN vtth.vrs_start_time END))/60)", 'vrs_prod_value');

    //$query->addExpression("SUM(ad.bill_value)", 'van_value'); //new field added
    $query->addExpression("SUM(idt.dispatch_qty * idt.sum_net_sales_value)", 'dispatched_value'); //new field added
    $query->fields('vtt', array('total_cashier_cash_short', 'total_short_amt')); //new field added ()
    //
    $query->addExpression("COUNT(distinct CASE WHEN (idt.line_level_status = 'line_undeliver') THEN idt.sku_code END)", 'return_sku_lines_count'); // new field added
    //$query->addExpression("COUNT(CASE WHEN (idt.return_qty > 0) THEN idt.return_qty END)", 'return_qty');

    $result = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
    if (!empty($result)) {
        $bill_value_result = get_total_bill_value($trip_ids);
    }
    $return_confirmed_vrs_status = array(1, 2, 3, 4, 5);
    foreach ($result as $val) {
        $row['trip_id'] = $val['runsheet_number'];
        $dispatch_date = strtotime($val['date']);
        if (isset($dispatch_date)) {
            $row['dispatch_date'] = date('d-M-y', $dispatch_date);
        }
        else {
            $row['dispatch_date'] = '';
        }
        $row['fieldExecutiveName'] = $val['fieldExecutiveName'];
        $field = field_info_field('field_vrs_status');
        $allowed_values = list_allowed_values($field);
        foreach ($allowed_values as $key => $value) {
            if ($key == $val['field_vrs_status_value']) {
                $row['field_vrs_status_value'] = $value;
            }
        }
        $row['field_dispatch_start_time_value'] = $val['field_dispatch_start_time_value'];

        // Vehicle Arrival Time.
        $row['field_data_field_vehicle_arrival_date'] = date('d-M-y', $val['field_vehicle_arrival_date_value']) . ' ' . $val['field_vehicle_arrival_time_value'];

        if (isset($val['field_received_asset_time_value'])) {
            $row['field_received_asset_time_value'] = date('d-M-y H:i:s', $val['field_received_asset_time_value']);
        }
        else {
            $row['field_received_asset_time_value'] = '';
        }

        //Delivery Confirmation time
        $query = db_select('inventory_versioning_api_data', 'iad');
        $query->condition('iad.distributor_id', $distributor_ids, 'IN');
        $query->condition('iad.version_flag', 1, '=');
        $query->condition('iad.runsheet_number', $val['runsheet_number'], '=');
        $query->fields('iad', array('version_created_time'));
        $query->orderBy('iad.id', 'DESC');
        $query->range(0, 1);
        $version_created_time = $query->execute()->fetchField();

        if (!empty($version_created_time)) {
            $row['version_created_time'] = date('d-M-y H:i:s', $version_created_time);
        }
        else {
            $row['version_created_time'] = '';
        }
        //Delivery Confirmation time

        if (!empty($val['field_sku_wise_return_start_time_value'])) {
            $row['sku_start_time'] = date('d-M-y H:i:s', $val['field_sku_wise_return_start_time_value']);
            $return_start_time = $val['field_sku_wise_return_start_time_value'];
        }
        else {
            $row['sku_start_time'] = '';
        }

        if (!empty($val['field_sku_wise_return_end_time_value'])) {
            $row['sku_end_time'] = date('d-M-y H:i:s', $val['field_sku_wise_return_end_time_value']);
            $sku_return_end_time = $val['field_sku_wise_return_end_time_value'];
        }
        else {
            $row['sku_end_time'] = '';
        }
        // returns incharge
        $row['return_incharge'] = $val['return_incharge'];

        if (!empty($val['field_bill_wise_return_starttime_value'])) {
            if (in_array($val['field_vrs_status_value'], $return_confirmed_vrs_status) && empty($val['field_bill_wise_return_end_time_value'])) {
                $row['bill_start_time'] = '';
            }
            else {
                $row['bill_start_time'] = date('d-M-y H:i:s', $val['field_bill_wise_return_starttime_value']);
            }
        }
        else {
            $row['bill_start_time'] = '';
        }
        if (!empty($val['field_bill_wise_return_end_time_value'])) {
            $row['bill_end_time'] = date('d-M-y H:i:s', $val['field_bill_wise_return_end_time_value']);
            $returns_end_time = $val['field_bill_wise_return_end_time_value'];
        }
        else {
            $row['bill_end_time'] = '';
        }
        if (empty($returns_end_time) && !empty($sku_return_end_time)) {
            $returns_end_time = $sku_return_end_time;
        }
        // return supervisor incharge
        $row['return_supervisor'] = $val['return_supervisor'];
        if (in_array($val['field_vrs_status_value'], $return_confirmed_vrs_status) && !empty($returns_end_time) && !empty($return_start_time)) {
            $total_return_time = $returns_end_time - $return_start_time;
        }
        if (!empty($total_return_time)) {
            $row['total_return_time'] = gmdate('H:i:s', $total_return_time);
        }
        else {
            $row['total_return_time'] = '';
        }
        $row['return_value'] = round($val['return_value']);
        $row['return_qty_wo_sachets'] = $val['return_sku_qty_wo_sachets'];
        $row['return_qty_sachets'] = $val['return_sku_qty_sachets'];
        $row['return_prod'] = round($val['return_prod'] / ($total_return_time) * 60);
        if (!empty($val['vrs_start_time'])) {
            $row['vrs_start_time'] = date('d-M-y H:i:s', $val['vrs_start_time']);
        }
        else {
            $row['vrs_start_time'] = '';
        }
        if (!empty($val['vrs_end_time'])) {
            $row['vrs_end_time'] = date('d-M-y H:i:s', $val['vrs_end_time']);
        }
        else {
            $row['vrs_end_time'] = '';
        }
        if (!empty($val['vrs_time'])) {
            $row['vrs_time'] = gmdate('H:i:s', $val['vrs_time']);
        }
        else {
            $row['vrs_time'] = '';
        }
        $row['total_actual_cash_amount'] = $val['total_actual_cash_amount'];
        $row['total_actual_cheque_amount'] = $val['total_actual_cheque_amount'];
        $row['total_actual_signed_bill_amount'] = $val['total_actual_signed_bill_amount'];
        $row['vrs_prod_value'] = round($val['vrs_prod_value']);

        $row['van_value'] = $bill_value_result[$val['runsheet_number']]; //new added 
        $row['dispatched_value'] = round($val['dispatched_value']); //new added
        $row['total_cashier_cash_short'] = $val['total_cashier_cash_short']; //new added
        $row['total_short_amt'] = $val['total_short_amt']; //new added

        $collected_value = $val['total_actual_cash_amount'] + $val['total_actual_cheque_amount'] + $val['total_actual_signed_bill_amount'];
        $reco_diff = $bill_value_result[$val['runsheet_number']] - ($collected_value + $val['total_cashier_cash_short'] + $val['total_short_amt'] + round($val['return_value']));
        $row['reco_diff'] = $reco_diff; //new added
        $row['return_sku_lines_count'] = $val['return_sku_lines_count']; // new added

        $rows[] = $row;
        unset($returns_end_time);
        unset($return_start_time);
        unset($total_return_time);
        unset($sku_return_end_time);
    }

    $header = array('Trip ID', 'Dispatch Date', 'Cashier Name', 'Status', 'Dispatch Time', 'Vehicle Arrival Time', 'Received Assets Time', 'Delivery Confirmation Time', 'SKU Wise Returns Start Time', 'SKU Wise Returns End Time', 'Returns Incharge', 'Bill Wise Returns Start Time', 'Bill Wise Returns End Time', 'Bill Wise Return Superviser', 'Total Returns Time taken', 'Total Returned Value', 'Returned SKU Quantity(w/o Sachets)', 'Returned Quantity of Sachets', 'Returns Productivity (Value/5 Min)', 'VRS Start Time', 'VRS End Time', 'Total VRS Time taken', 'Cash Value', 'Cheque Value', 'Signed Bill Value', 'VRS Productivity (Value / 10 Min)', 'Van Value', 'Dispatched Value', 'Cash Short By Cashier', 'Outlet Adjustment', 'Reco Difference', 'Returned SKU lines');
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

    cache_set('return-vrs-time-report' . session_id(), $html, 'cache', time() + 60 * 10);
    $output .= l(t('XLS'), 'return-vrs-time-report/export', array('query' => array($data)));
    return $form . $output;
}

function return_vrs_export() {
  $file = "Return_VRS_Report.xls";
  $data = cache_get('return-vrs-time-report' . session_id());
  if (empty($data)) {
    $table = return_vrs_callback();
  }
  else {
    $table = $data->data;
  }
  header("Content-type: application/vnd.ms-excel");
  header("Content-Disposition: attachment; filename=$file");
  echo $table;
}
