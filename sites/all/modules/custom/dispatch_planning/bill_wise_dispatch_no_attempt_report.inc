<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function bill_wise_dispatch_no_attempt_report_form($form, &$form_state) {

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
    if (isset($_GET['bill_status'])) {
        $bill_status_default = explode(",", $_GET['bill_status']);
    }
    if (isset($_GET['facility_code'])) {
        $default_facility_code = $_GET['facility_code'];
    }
    if (isset($_GET['distributor_id'])) {
        $default_distributor_id = $_GET['distributor_id'];
    }
    if (isset($_GET['central_distributor_id'])) {
        $default_central_distributor_id = $_GET['central_distributor_id'];
    }

    global $user;
    $user_id = $user->uid;
    $distributor_id = change_feeds_import_get_distributor_id($user_id);
//    $central_distributor_id = $distributor_id;
    $logged_in_distributor_id = $distributor_id;

    if (in_array("super_central_distributor", $user->roles)) {

        $form['central_outer'] = array(
            '#type' => 'markup',
            '#prefix' => '<div style="color:red"><h3>Note: Select any one  Facility Code</h3></div>',
        );
        $vocabulary = taxonomy_vocabulary_machine_name_load('facility_code');
        $terms_obj = entity_load('taxonomy_term', FALSE, array('vid' => $vocabulary->vid));
        if (!empty($terms_obj)) {
            foreach ($terms_obj as $key => $value) {
                $facility[$value->tid] = $value->name;
            }
            $facility = array('' => 'Select') + $facility;
        }

        $form['facility_code'] = array(
            '#title' => t('Select Facility Code'),
            '#type' => 'select',
            '#options' => $facility,
            '#required' => TRUE,
            '#ajax' => array(
                'event' => 'change',
                'callback' => 'facility_code_ajax_callback',
                'wrapper' => 'dropdown-distributor-id',
            ),
            '#default_value' => $default_facility_code,
            '#prefix' => '<div class="facility-filter" style="display:block; margin: 0 87% 1% 0;">',
            '#suffix' => '</div>'
        );


        $facility_code = !empty($form_state['input']['facility_code']) ? $form_state['input']['facility_code'] : $_GET['facility_code'];
        // central Distributor ID of facility Code
        $central_distributor_id = get_central_distributor_id_of_facility_code($facility_code);
        if (!empty($central_distributor_id)) {
            $child_distributors = get_child_distributor_rsname($central_distributor_id);
            $distributor_ids = array('' => 'Select') + $child_distributors;
        } else {
            $distributor_ids = array('' => 'Select');
        }

        $default_value = !empty($form_state['input']['distributor_id']) ? $form_state['input']['distributor_id'] : $_GET['distributor_id'];
        //print_r($default_value);

        $form['distributor_id'] = array(
            '#title' => t('Select Distributor Name'),
            '#type' => 'select',
            '#description' => 'Select Distributor Name.',
            '#default_value' => $default_value,
            '#options' => $distributor_ids,
            '#prefix' => '<div id="dropdown-distributor-id" class="distributor-filter" style="display:inline-block; margin-bottom:20px;">',
            '#suffix' => '</div>'
        );

        $_SESSION['distributor_id'] = $child_distributors;
    } elseif (in_array("central_distributor", $user->roles)) {
        $central_distributor_id = $logged_in_distributor_id;
        $distributor_ids = get_distributor_ids();
        unset($distributor_ids[$distributor_id]);
        $child_distributors = get_child_distributor_rsname($logged_in_distributor_id);
        $distributor_ids = array('' => 'Select') + $child_distributors;
        $form['central_outer'] = array(
            '#type' => 'markup',
            '#prefix' => '<div style="color:red"><h3>Note: Select any one  Distributor Name</h3></div>',
        );
        $default_value = !empty($form_state['input']['distributor_id']) ? $form_state['input']['distributor_id'] : $_GET['distributor_id'];
        $form['distributor_id'] = array(
            '#title' => t('Select Distributor Name'),
            '#type' => 'select',
            '#options' => $distributor_ids,
            '#default_value' => $default_value,
            '#required' => TRUE,
            '#prefix' => '<div class="distributor-filter" style="display:inline-block; margin-bottom:20px;">',
            '#suffix' => '</div>'
        );
        $distributor_id = $form_state['input']['distributor_id'];
    }
    // Child distributor logged in
    else {
        $central_distributor_id = get_central_distributor_id($distributor_id);
        $form['distributor_id'] = array(
            '#type' => 'hidden',
            '#value' => $distributor_id,
        );
    }

    $form['central_distributor_id'] = array(
        '#type' => 'hidden',
        '#value' => $central_distributor_id,
    );

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

    $dp_status = variable_get('current_delivery_status');
    $bill_status = array('' => '-Select-') + get_bill_status_from_api_table() + array('delivery_pending' => $dp_status);
    $form['bill_status'] = array(
        '#title' => t('Bill Status'),
        '#type' => 'select',
        '#multiple' => true,
        '#options' => $bill_status,
        '#default_value' => $bill_status_default,
        '#prefix' => '<div class="bill-status" style="display:inline-block;margin: 20px 101px 0 50px;">',
        '#suffix' => '</div>'
    );

    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Apply'),
        '#submit' => array('bill_wise_dispatch_no_attempt_report_submit'),
        '#prefix' => '<div>',
        '#suffix' => '</div>'
    );

    return $form;
}

function bill_wise_dispatch_no_attempt_report_submit($form, &$form_state) {
    $from_bill_date = $form_state['values']['from_bill_date'];
    $to_bill_date = $form_state['values']['to_bill_date'];
    $from_dispatch_date = $form_state['values']['from_dispatch_date'];
    $to_dispatch_date = $form_state['values']['to_dispatch_date'];
    $bill_number = $form_state['values']['bill_number'];
    $party_code = $form_state['values']['party_code'];
    $bill_status = $form_state['values']['bill_status'];
    $bill_status = implode(",", $bill_status);
    $distributor_id = $form_state['values']['distributor_id'];
    $central_distributor_id = $form_state['values']['central_distributor_id'];
    $facility_code = $form_state['values']['facility_code'];

    if (($form_state['values']["op"] == "Apply") && (arg(0) == 'bill-wise-dispatch-no-attempt-report')) {
        drupal_goto('bill-wise-dispatch-no-attempt-report', array('query' => array('from_bill_date' => $from_bill_date, 'to_bill_date' => $to_bill_date, 'from_dispatch_date' => $from_dispatch_date, 'to_dispatch_date' => $to_dispatch_date, 'bill_number' => $bill_number, 'party_code' => $party_code, 'distributor_id' => $distributor_id, 'central_distributor_id' => $central_distributor_id, 'facility_code' => $facility_code, 'bill_status' => $bill_status)));
    }
}

function bill_wise_dispatch_no_attempt_report_callback() {
    global $user;
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
    if (isset($_GET['bill_status'])) {
        $bill_status = explode(",", $_GET['bill_status']);
    }
    if (arg(1) != 'export') {
        $form = drupal_render(drupal_get_form('bill_wise_dispatch_no_attempt_report_form'));
    }
    if (isset($_GET['distributor_id'])) {
        $distributor_id = $_GET['distributor_id'];
    }
    if (isset($_GET['central_distributor_id'])) {
        $central_distributor_id = $_GET['central_distributor_id'];
    }
    if (!empty($_GET['facility_code'])) {
        $facility_code = $_GET['facility_code'];
    }
    if (empty($distributor_id)) {
        $distributor_id = change_feeds_import_get_distributor_id($user->uid);
    } else {
        $distributor_id = $distributor_id;
    }
    // child logged in  
    if (empty($central_distributor_id) && !in_array("central_distributor", $user->roles) && !in_array("super_central_distributor", $user->roles)) {
        $central_distributor_id = get_central_distributor_id($distributor_id);
    }

    if ((in_array("central_distributor", $user->roles) || in_array("super_central_distributor", $user->roles)) && empty($_GET['distributor_id'])) {
        if (!empty($facility_code)) {
            $central_distributor_id = get_central_distributor_id_of_facility_code($facility_code);
            if (!empty($central_distributor_id)) {
                $child_distributors = get_child_distributor_rsname($central_distributor_id);
                $distributor_ids = array_keys($child_distributors);
            }
        } else {
            $distributor_ids = get_distributor_ids();
            $user_id = $user->uid;
            $distributor_id = change_feeds_import_get_distributor_id($user_id);
            if ((in_array("central_distributor", $user->roles) || !in_array("super_central_distributor", $user->roles)) && empty($_GET['distributor_id'])) {
                $central_distributor_id = $distributor_id;
            } elseif ((!in_array("central_distributor", $user->roles) || in_array("super_central_distributor", $user->roles)) && empty($_GET['distributor_id'])) {
                $central_distributor_id = get_child_distributor_ids($distributor_id);
            } else {
                $central_distributor_id = get_central_distributor_id($distributor_id);
            }
        }
    } elseif (empty($distributor_ids) && !empty($distributor_id)) {
        $distributor_ids = array($distributor_id);
    }

    // Fetch data from table
    $query = db_select('dispatch_planning_api_data', 'ad');
    $query->condition('ad.distributor_id', $distributor_ids, 'IN');

    if (!empty($bill_number)) {
        $query->condition('ad.bill_number', $bill_number, '=');
    }
    if (!empty($party_code)) {
        $query->condition('ad.party_code', $party_code, '=');
    }
    if (!empty($bill_status)) {
        if (($key = array_search('delivery_pending', $bill_status)) !== false) {
            if (count($bill_status) == 1) {
                $query->condition('ad.delivery_pending_flag', 1, '=');
            } else if (count($bill_status) > 1) {
                unset($bill_status[$key]);
                $query->condition('ad.delivery_pending_flag', 1, '=');
                $query->condition('ad.res_status', $bill_status, 'IN');
            }
        } else {
            $query->condition('ad.res_status', $bill_status, 'IN');
        }
    }
    if (!empty($from_dispatch_date) && !empty($to_dispatch_date)) {
        $query->condition('ad.dispatch_date', array($from_dispatch_date, $to_dispatch_date), 'BETWEEN');
    }
    if (!empty($from_bill_date) && !empty($to_bill_date)) {
        $query->condition('ad.bill_date', array($from_bill_date, $to_bill_date), 'BETWEEN');
    }
    $query->condition('ad.attempted_status', 'No', '=');
    $query->fields('ad', array('bill_number', 'bill_date', 'beat_name', 'party_name', 'dispatch_date'));
    $query->addexpression("MAX((ad.runsheet_number))", 'runsheet_number');
    $query->addexpression("COUNT(DISTINCT(ad.runsheet_number))", 'count_dispatch_date');
    $query->addexpression("MAX(CASE WHEN ad.delivery_pending_flag = 0 THEN ad.res_status END)", 'current_bill_status');
    $query->addexpression("(CASE WHEN (ad.id = MAX(ad.id)) THEN ad.delivery_pending_flag END)", 'delivery_pending_flag');

    $query->join('node', 'n', 'n.title = ad.runsheet_number');
    $query->condition('n.type', 'dispatch_data', '=');
    $query->condition('n.uid', $distributor_ids, 'IN');
    $query->join('node', 'n1', 'n1.title = ad.bill_number');
    $query->condition('n1.type', 'sales_register_data', '=');
    $query->condition('n1.uid', $distributor_ids, 'IN');

    $query->leftjoin('field_data_field_sr_new_bill_date', 'bill_date', 'bill_date.entity_id = n1.nid');
    $query->leftjoin('field_data_field_outlet_order_no', 'order_no', 'order_no.entity_id = n1.nid');
    $query->leftjoin('field_data_field_bill_area_codes', 'bill_area', 'bill_area.entity_id = n.nid');

    $query->join('node', 'n2', 'n2.title = ad.party_hll_code');
    $query->condition('n2.type', 'hul_code_master', '=');
    $query->condition('n2.uid', $distributor_ids, 'IN');
    $query->leftjoin('field_data_field_area_code', 'ac', 'ac.entity_id = n2.nid');

    $query->leftjoin('node', 'n3', 'ac.field_area_code_nid = n3.nid');
//    $query->condition('n3.type', 'area_code_master', '=');
    $query->join('users', 'u', 'u.uid = ad.distributor_id');
    $query->join('field_data_field_distributor_name', 'dn', 'u.uid = dn.entity_id');
    $query->join('field_data_field_facility_code', 'fc', 'u.uid = fc.entity_id');

    $query->fields('bill_date', array('field_sr_new_bill_date_value'));
    $query->fields('order_no', array('field_outlet_order_no_value'));
    $query->fields('dn', array('field_distributor_name_value'));
    $query->fields('bill_area', array('field_bill_area_codes_value'));
    $query->fields('n3', array('title'));
    $query->fields('fc', array('field_facility_code_tid'));
    $query->groupBy('ad.bill_number');
    $query->groupBy('ad.distributor_id');
    $result = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

    $row = array();

    foreach ($result as $val) {

//        if ($val['delivery_pending_flag'] == 0) {
            if ($val['current_bill_status'] == 'deliver') {
                $bill_status = variable_get('deliver_status'); //'Delivered';
            } elseif ($val['current_bill_status'] == 'partial_deliver') {
                $bill_status = variable_get('partial_deliver_status'); //'Partial Delivered';
            } elseif ($val['current_bill_status'] == 'undeliver') {
                $bill_status = variable_get('undeliver_status'); //'Full Returned';
            }
            if($val['delivery_pending_flag'] == 1){
             $bill_status = variable_get('current_delivery_status');
            }
//        } else {
//            $bill_status = variable_get('current_delivery_status');
//        }

        $facility_code = get_facility_code_from_tid($val['field_facility_code_tid']);
        $row['order_date'] = date('d-M-y', $val['field_sr_new_bill_date_value']);
        $row['order_no'] = $val['field_outlet_order_no_value'];
        $row['bill_date'] = date('d-M-y', $val['bill_date']);
        $row['bill_number'] = $val['bill_number'];
        $row['beat_name'] = $val['beat_name'];
        $row['party_name'] = $val['party_name'];
        $row['runsheet_number'] = $val['runsheet_number'];
        $row['area_code'] = $val['title'];
        $row['dispatch_date'] = date('d-M-y', $val['dispatch_date']);
        $row['current_delivery_status'] = $bill_status;
        $row['count_of_dispatches'] = $val['count_dispatch_date'];
        $row['distributor_name'] = $val['field_distributor_name_value'];
        $row['facility_code'] = $facility_code;
        $rows[] = $row;
    }
    $header = array('Order date', 'Order Number', 'Bill Date', 'Bill No.', 'Beat name', 'Party name', 'Trip Id', 'Area code', 'Dispatch Date', 'Delivery Status', 'Count of dispatches', 'Distributor Name', 'Facility Code');

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

    cache_set('bill-wise-dispatch-no-attempt-report' . session_id(), $html, 'cache', time() + 60 * 10);
    $output .= l(t('XLS'), 'bill-wise-dispatch-no-attempt-report/export', array('query' => array($data)));
    return $form . $output;
}

function bill_wise_dispatch_no_attempt_report_export() {
    $file = "Bill_Wise_No_Attempt_Report.xls";
    $data = cache_get('bill-wise-dispatch-no-attempt-report' . session_id());
    if (empty($data)) {
        $table = bill_wise_dispatch_no_attempt_report_callback();
    } else {
        $table = $data->data;
    }
    header("Content-type: application/vnd.ms-excel");
    header("Content-Disposition: attachment; filename=$file");
    echo $table;
}

function facility_code_ajax_callback($form, &$form_state) {
    return $form['distributor_id'];
}
