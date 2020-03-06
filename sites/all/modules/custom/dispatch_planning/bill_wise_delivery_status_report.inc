<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function bill_wise_delivery_status_report_form($form, &$form_state) {
    global $user;
    $distributor_id = change_feeds_import_get_distributor_id($user->uid);

    if (isset($_GET['from_bill_date'])) {
        $from_bill_date = $_GET['from_bill_date'];
    }
    if (isset($_GET['to_bill_date'])) {
        $to_bill_date = $_GET['to_bill_date'];
    }
    if (isset($_GET['from_dispatch_date'])) {
        $from_dispatch_date = $_GET['from_dispatch_date'];
    }
    if (isset($_GET['to_dispatch_date'])) {
        $to_dispatch_date = $_GET['to_dispatch_date'];
    }

    if (isset($_GET['bill_number'])) {
        $bill_number = $_GET['bill_number'];
    }
    if (isset($_GET['party_code'])) {
        $party_code = $_GET['party_code'];
    }
    // Central Distributor Logged IN
    if (in_array("central_distributor", $user->roles)) {
        $distributor_ids = get_distributor_ids();
        unset($distributor_ids[$distributor_id]);
        $distributor_ids = array('' => 'Select') + $distributor_ids;
        if (isset($_GET['distributor_id'])) {
            $distributor_id = $_GET['distributor_id'];
        }
        $form['distributor_id'] = array(
            '#title' => t('Select DistributorID'),
            '#type' => 'select',
            '#description' => 'Select DistributorID.',
            '#options' => $distributor_ids,
            '#default_value' => $distributor_id,
        );
    }
    // Child distributor logged in
    else {
        $form['distributor_id'] = array(
            '#type' => 'hidden',
            '#value' => $distributor_id,
        );
    }

    $form['from_bill_date'] = array(
        '#type' => 'date_popup',
        '#title' => 'Bill From date',
        '#date_label_position' => 'within',
        '#date_format' => 'Y-m-d',
        '#default_value' => $from_bill_date,
        '#prefix' => '<div class="date-filter"><div class="from-bill-date" style="display:inline-block;">'
    );

    $form['to_bill_date'] = array(
        '#type' => 'date_popup',
        '#title' => 'Bill To date',
        '#date_label_position' => 'within',
        '#date_format' => 'Y-m-d',
        '#default_value' => $to_bill_date,
        '#suffix' => '</div>'
    );

    $form['from_dispatch_date'] = array(
        '#type' => 'date_popup',
        '#title' => 'Dispatch From date',
        '#date_label_position' => 'within',
        '#date_format' => 'Y-m-d',
        '#default_value' => $from_dispatch_date,
        '#prefix' => '<div class="from-dispatch-date" style="display:inline-block;">'
    );

    $form['to_dispatch_date'] = array(
        '#type' => 'date_popup',
        '#title' => 'Dispatch To date',
        '#date_label_position' => 'within',
        '#date_format' => 'Y-m-d',
        '#default_value' => $to_dispatch_date,
        '#suffix' => '</div></div>'
    );

    $form['bill_number'] = array(
        '#type' => 'textfield',
        '#title' => t('Bill Number'),
        '#size' => 20,
        '#default_value' => $bill_number,
        '#maxlength' => 50,
        '#prefix' => '<div class="bill-number" style="display:inline-block;margin: 20px 101px 0 0;">',
        '#suffix' => '</div>'
    );

    $form['party_code'] = array(
        '#type' => 'textfield',
        '#title' => t('Party Code'),
        '#size' => 20,
        '#default_value' => $party_code,
        '#maxlength' => 50,
        '#prefix' => '<div class="party-code" style="display:inline-block;">',
        '#suffix' => '</div>'
    );

    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Apply'),
        '#submit' => array('bill_wise_delivery_status_report_form_submit'),
        '#prefix' => '<div>',
        '#suffix' => '</div>'
    );

    return $form;
}

function bill_wise_delivery_status_report_form_submit($form, &$form_state) {
    $from_bill_date = $form_state['values']['from_bill_date'];
    $to_bill_date = $form_state['values']['to_bill_date'];
    $from_dispatch_date = $form_state['values']['from_dispatch_date'];
    $to_dispatch_date = $form_state['values']['to_dispatch_date'];
    $bill_number = $form_state['values']['bill_number'];
    $party_code = $form_state['values']['party_code'];
    $distributor_id = $form_state['values']['distributor_id'];
    if (($form_state['values']["op"] == "Apply") && (arg(0) == 'bill-wise-delivery-status-report')) {
        drupal_goto('bill-wise-delivery-status-report', array('query' => array('from_bill_date' => $from_bill_date, 'to_bill_date' => $to_bill_date, 'from_dispatch_date' => $from_dispatch_date, 'to_dispatch_date' => $to_dispatch_date, 'bill_number' => $bill_number, 'party_code' => $party_code, 'distributor_id' => $distributor_id)));
    }
}

function bill_wise_delivery_status_report_callback() {
    if (isset($_GET['from_bill_date'])) {
        $from_bill_date = strtotime($_GET['from_bill_date']);
    }
    if (isset($_GET['to_bill_date'])) {
        $to_bill_date = strtotime($_GET['to_bill_date']);
    }

    if (isset($_GET['from_dispatch_date'])) {
        $from_dispatch_date = strtotime($_GET['from_dispatch_date']);
    }
    if (isset($_GET['to_dispatch_date'])) {
        $to_dispatch_date = strtotime($_GET['to_dispatch_date']);
    }

    if (isset($_GET['bill_number'])) {
        $bill_number = $_GET['bill_number'];
    }
    if (isset($_GET['party_code'])) {
        $party_code = $_GET['party_code'];
    }
    if (arg(1) != 'export') {
        $form = drupal_render(drupal_get_form('bill_wise_delivery_status_report_form'));
    }
    if (isset($_GET['distributor_id'])) {
        $distributor_id = $_GET['distributor_id'];
    }

    $query = db_select('dispatch_planning_api_data', 'ad');
    $query->condition('ad.distributor_id', $distributor_id, '=');
    if (!empty($bill_number)) {
        $query->condition('ad.bill_number', $bill_number, '=');
    }
    if (!empty($party_code)) {
        $query->condition('ad.party_code', $party_code, '=');
    }
    if (!empty($from_dispatch_date) && !empty($to_dispatch_date)) {
        $query->condition('ad.dispatch_date', array($from_dispatch_date, $to_dispatch_date), 'BETWEEN');
    }
    if (!empty($from_bill_date) && !empty($to_bill_date)) {
        $query->condition('ad.bill_date', array($from_bill_date, $to_bill_date), 'BETWEEN');
    }
    $query->fields('ad', array('bill_number', 'bill_date', 'beat_name', 'party_code', 'party_name', 'bill_value'));
    $query->addexpression("MAX(ad.runsheet_number)", 'trip_number');
    $query->addExpression('MIN(ad.dispatch_date)', 'first_dispatch_date');
    $query->addExpression('MAX(ad.dispatch_date)', 'last_dispatch_date');
    $query->addexpression("COUNT(DISTINCT(ad.dispatch_date))", 'count_dispatch_date');
    $query->addexpression("ad.fieldExecutiveName", 'fieldExecutiveName');
    $query->addexpression("(CASE WHEN (ad.id = MAX(ad.id)) THEN ad.res_status END)", 'current_bill_status');
    $query->addexpression("(CASE WHEN (ad.id = MAX(ad.id)) THEN ad.delivery_pending_flag END)", 'delivery_pending_flag');
    $query->addexpression("COUNT(DISTINCT(CASE WHEN ad.res_reason_for_failure != 'Outlet not attempted' THEN ad.runsheet_number END))", 'counts_of_attempts');
    $query->addexpression("MIN(CASE WHEN ad.res_reason_for_failure != 'Outlet not attempted' THEN ad.dispatch_date END)", 'first_attempt_date');
    $query->addexpression("CASE WHEN (ad.delivery_pending_flag = 0 and (ad.res_status IS NOT NULL or ad.res_status !='')) THEN ad.dispatch_date END", 'delivery_full_returned_date');
    $query->addexpression("CASE WHEN (ad.delivery_pending_flag = 0 and (ad.res_status IS NOT NULL or ad.res_status !='')) THEN ad.runsheet_number END", 'delivered_full_returned_trip_id');
    $query->addexpression("CASE WHEN (ad.delivery_pending_flag = 0 and (ad.res_status IS NOT NULL or ad.res_status !='')) THEN ad.fieldExecutiveName END", 'delivery_full_returned_cashier');
    $query->addexpression("(MAX(CASE WHEN ad.res_reason_for_failure != 'Outlet not attempted' THEN ad.dispatch_date END))", 'last_attempt_date');

    $query->join('node', 'n', 'n.title = ad.runsheet_number');
    $query->condition('n.type', 'dispatch_data', '=');
    $query->condition('n.uid', $distributor_id);

    $query->join('node', 'n1', 'n1.title = ad.bill_number');
    $query->condition('n1.type', 'sales_register_data', '=');
    $query->condition('n1.uid', $distributor_id);
    $query->leftjoin('field_data_field_sr_new_bill_date', 'bill_date', 'bill_date.entity_id = n1.nid');
    $query->leftjoin('field_data_field_outlet_order_no', 'order_no', 'order_no.entity_id = n1.nid');
    $query->leftjoin('field_data_field_bill_area_codes', 'bill_area', 'bill_area.entity_id = n.nid');
    $query->join('dispatch_planning_item_details', 'item_details', 'item_details.trip_bill_reference = ad.id');
    $query->addexpression("COUNT(item_details.sku_code)", 'count_sku_code');
    $query->fields('bill_date', array('field_sr_new_bill_date_value'));
    $query->fields('order_no', array('field_outlet_order_no_value'));
    $query->fields('bill_area', array('field_bill_area_codes_value'));
    $query->groupBy('ad.bill_number');
    $query->orderBy('ad.runsheet_number');
    $result = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

    $query = db_select('node', 'n');
    $query->condition('n.title', $distributor_id);
    $query->condition('n.type', 'pdd_master');
    $query->leftjoin('field_data_field_day', 'fd', 'fd.entity_id = n.nid');
    $query->fields('fd', array('field_day_value'));
    $weeklyoff = $query->execute()->fetchCol();

    $query = db_select('node', 'n');
    $query->condition('n.title', $distributor_id);
    $query->condition('n.type', 'holiday_master');
    $query->leftjoin('field_data_field_holiday', 'hday', 'hday.entity_id = n.nid');
    $query->addExpression("DATE_FORMAT(CONVERT_TZ(FROM_UNIXTIME(hday.field_holiday_value),'+00:00','+05:30'), '%Y-%m-%d')", 'hdate');
    $holiday_result = $query->execute()->fetchCol();

    $row = array();

    foreach ($result as $val) {
        $start = new DateTime(date('d-M-y', $val['bill_date']));
        $end = new DateTime(date('d-M-y', $val['last_attempt_date']));
        $end->modify('+1 day');
        $interval = $end->diff($start);
        
        if ($val['delivery_pending_flag'] == 0) {
            // total days
            $days = $interval->days;
            // create an iterateable period of date (P1D equates to 1 day)
            $period = new DatePeriod($start, new DateInterval('P1D'), $end);

            foreach ($period as $dt) {
                $curr = $dt->format('l');
                if (in_array($curr, $weeklyoff)) {
                    $days--;
                } elseif (in_array($dt->format('Y-m-d'), $holiday_result)) {
                    $days--;
                }
            }
        }

        if ($val['current_bill_status'] == 'deliver') {
            $bill_status = 'Delivered';
        } elseif ($val['current_bill_status'] == 'partial_deliver') {
            $bill_status = 'Partial Delivered';
        } elseif ($val['current_bill_status'] == 'undeliver') {
            $bill_status = 'Full Returned';
        }

        $row['order_date'] = date('d-M-y', $val['field_sr_new_bill_date_value']);
        $row['order_no'] = $val['field_outlet_order_no_value'];
        $row['bill_date'] = date('d-M-y', $val['bill_date']);
        $row['bill_number'] = $val['bill_number'];
        $row['beat_name'] = $val['beat_name'];
        $row['area_code'] = $val['field_bill_area_codes_value'];
        $row['party_code'] = $val['party_code'];
        $row['party_name'] = $val['party_name'];
        $row['count_sku_code'] = $val['count_sku_code'];
        $row['bill_value'] = $val['bill_value'];
        $row['first_dispatch_date'] = $val['first_dispatch_date'] ? date('d-M-y', $val['first_dispatch_date']) : '';
        $row['count_of_dispatches'] = $val['count_dispatch_date'];
        $row['last_dispatch_date'] = $val['last_dispatch_date'] ? date('d-M-y', $val['last_dispatch_date']) : '';
        $row['counts_of_attempts'] = $val['counts_of_attempts'];
        $row['first_attempt_date'] = $val['first_attempt_date'] ? date('d-M-y', $val['first_attempt_date']) : '';
        $row['exception_status'] = $val['exception_status'];
        $row['exception_reason'] = $val['exception_reason'];
        $row['delivery_days'] = $days; //round($datediff / (60 * 60 * 24));
        $row['current_delivery_status'] = $bill_status;
        $row['delivery_full_returned_date'] = $val['delivery_full_returned_date'] ? date('d-M-y', $val['delivery_full_returned_date']) : '';
        $row['delivered_full_returned_trip_id'] = $val['delivered_full_returned_trip_id'];
        $row['delivery_full_returned_cashier'] = $val['delivery_full_returned_cashier'];
        $rows[] = $row;
    }
    $header = array('Order date', 'Order Number', 'Bill Date', 'Bill No.', 'Beat name', 'Area code', 'Party Code', 'Party name', 'Count of bill lines', 'Sales Value', '1st Dispatch Date', 'Count of dispatches', 'Last Dispatch Date', 'Counts of Attempts', '1st Attempt Date', 'Exception status', 'Exception Reason', 'Delivery Days', 'Current Delivery Status', 'Delivery/Full Returned date', 'Delivered/Full Returned Trip id', 'Delivery/Full Returned Cashier');

    $per_page = 10;
    // Initialize the pager
    $current_page = pager_default_initialize(count($rows), $per_page);
    // Split your list into page sized chunks
    $chunks = array_chunk($rows, $per_page, TRUE);
    // Show the appropriate items from the list
    $output = theme('table', array('header' => $header, 'rows' => $chunks[$current_page]));
    // Show the pager
    $output .= theme('pager', array('quantity', count($rows)));

    $html = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('cellpadding' => '5', 'cellspacing' => '0'), 'empty' => t('No Records has been found')));

    cache_set('bill-wise-delivery-status-report' . session_id(), $html, 'cache', time() + 60 * 10);
    $output .= l(t('XLS'), 'bill-wise-delivery-status-report/export', array('query' => array($data)));
    return $form . $output;
}

function bill_wise_delivery_status_report_export() {
    $file = "Bill_Wise_Delivery_Status_Report.xls";
    $data = cache_get('bill-wise-delivery-status-report' . session_id());
    if (empty($data)) {
        $table = delivery_pending_report_callback();
    } else {
        $table = $data->data;
    }
    header("Content-type: application/vnd.ms-excel");
    header("Content-Disposition: attachment; filename=$file");
    echo $table;
}
