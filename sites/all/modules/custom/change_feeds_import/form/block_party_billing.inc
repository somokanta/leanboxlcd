<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function block_party_billing_form($form, &$form_state) {
    $form['#attached']['js'][] = drupal_get_path('module', 'change_feeds_import') . '/js/block_party_billing.js';
    global $user;
    $distributor_id = change_feeds_import_get_distributor_id($user->uid);
    $logged_in_distributor_id = $distributor_id;
    $form['#prefix'] = '<div id="itemlist-div">';
    $form['#suffix'] = '</div>';
    // Central Distributor Logged IN
    if (in_array("central_distributor", $user->roles)) {
        $distributor_ids = get_distributor_ids();
        unset($distributor_ids[$distributor_id]);
        $child_distributors = get_child_distributor_rsname($logged_in_distributor_id);
        $distributor_ids = array('' => 'Select') + $child_distributors;
        /* $form['central_outer'] = array(
          '#type' => 'markup',
          '#prefix' => '<div style="color:red"><h3>Note: Select any one  Distributor Name before creating a Trip</h3></div>',
          ); */
        $form['distributor_id'] = array(
            '#title' => t('Select Distributor Name'),
            '#type' => 'select',
            '#options' => $distributor_ids,
            '#required' => TRUE,
            '#attributes' => array('class' => array('distid')),
            '#ajax' => array(
                'wrapper' => 'itemlist-div',
                'callback' => 'distributor_callback_change',
                'method' => 'replace',
                'effect' => 'fade',
            ),
        );
        $distributor_id = $form_state['input']['distributor_id'] ? $form_state['input']['distributor_id'] : $distributor_id;
    }
    // Child distributor logged in
    else {
        $form['distributor_id'] = array(
            '#type' => 'hidden',
            '#value' => $distributor_id,
        );
    }

    $form['hul_code'] = array(
        '#type' => 'textfield',
        '#title' => t('HUL Code'),
        '#autocomplete_path' => 'hul-code/autocomplete/' . $distributor_id,
        '#prefix' => '<div class="form_row">',
        '#suffix' => '</div>',
    );

    $form['party_name'] = array(
        '#type' => 'textfield',
        '#title' => t('Party Name'),
        '#autocomplete_path' => 'party-name/autocomplete/' . $distributor_id,
        '#prefix' => '<div class="form_row">',
        '#suffix' => '</div>',
    );

    $form['apply'] = array(
        '#type' => 'submit',
        '#value' => 'Apply',
        '#ajax' => array(
            'callback' => 'block_party_billing_callback',
            'wrapper' => 'itemlist-div',
            'method' => 'replace',
            'effect' => 'fade',
        ),
        '#submit' => array('block_party_billing_submit'),
//        '#validate' => array('block_party_billing_validate'),
    );

    if (!isset($form_state['block_party_billing']['block_data'])) {
        $form_state['block_party_billing']['block_data'] = array();
    }
    $options_block = $form_state['block_party_billing']['block_data'];
    $header_block = array('Date', 'Status', 'Reason', 'Remarks');

    if (!empty($options_block)) {
        $form['block_details'] = array(
            '#theme' => 'table',
            '#tree' => TRUE,
            '#caption' => "<b>Block/Unblock History: </b>",
            '#header' => $header_block,
            '#rows' => $options_block,
            '#prefix' => '<div class="custom_middle_table_block">',
            '#suffix' => '</div>',
        );
    }


    if (!isset($form_state['block_party_billing']['hul_data'])) {
        $form_state['block_party_billing']['hul_data'] = array();
    }
    $options = $form_state['block_party_billing']['hul_data'];
    $header = array('HUL Code', 'Party Name', 'Block/ Unblock');

    if (!empty($options)) {
        $form['hul_details'] = array(
            '#theme' => 'table',
            '#tree' => TRUE,
            '#caption' => "<b>HUL Party Details: </b>",
            '#header' => $header,
            '#rows' => $options,
            '#prefix' => '<div class="custom_middle_table1">',
            '#suffix' => '</div>',
        );
    }

    if (!isset($form_state['block_party_billing']['bill_data'])) {
        $form_state['block_party_billing']['bill_data'] = array();
    }
    $options1 = $form_state['block_party_billing']['bill_data'];
    $header1 = array('Date', 'Bill no.', 'Status');

    if (!empty($options1)) {
        $form['bill_details'] = array(
            '#theme' => 'table',
            '#tree' => TRUE,
            '#caption' => "<b>Billing Details: </b>",
            '#header' => $header1,
            '#rows' => $options1,
            '#prefix' => '<div class="custom_middle_table">',
            '#suffix' => '</div>',
        );

        $form['modify'] = array(
            '#type' => 'submit',
            '#value' => 'Modify',
            '#ajax' => array(
                'callback' => 'block_party_billing_callback',
                'wrapper' => 'itemlist-div',
                'method' => 'replace',
                'effect' => 'fade',
            ),
            '#submit' => array('block_party_modify_submit'),
//        '#validate' => array('block_party_billing_validate'),
        );
    }


    $modify = $form_state['modify_status'];
    if ($modify == 1) {
        $form['data'] = array(
            '#type' => 'markup',
            '#prefix' => '<div id ="pop_up_bill_" class="pop-up_table" style="display:none;"><div class="popup_overlay"></div><div class="popup_wrap"><div class="pop_content"><div class="popup_border">'
            . '<img sku_key="" class="close-pop-up-button" id="close_pop_up_bill_" src="sites/all/modules/custom/mrc_inventory_returns/images/close_btn.png">',
        );

        $field = field_info_field('field_block_status');
        $allowed_values = list_allowed_values($field);
        $form['status'] = array(
            '#type' => 'select',
            '#options' => array('' => 'Select') + $allowed_values,
            '#title' => t('Block/Unblock Status'),
        );

        $form['reason_code'] = array(
            '#type' => 'textfield',
            '#title' => t('Block/Unblock Reason'),
        );

        $form['remarks'] = array(
            '#type' => 'textfield',
            '#title' => t('Block/Unblock Remarks'),
        );

        $form['modify_submit'] = array(
            '#type' => 'submit',
            '#value' => 'Submit',
            '#validate' => array('modify_final_validate'),
            '#submit' => array('modify_final_submit'),
            '#suffix' => '</div></div></div></div></div>'
        );
    }

    return $form;
}

//function block_party_billing_validate($form, &$form_state) {
//    
//}

function block_party_billing_submit($form, &$form_state) {
    $hul_code = $form_state['values']['hul_code'];
    $distributor_id = $form_state['values']['distributor_id'];
    $form_state['modify_status'] = 0;

    $party_name = $form_state['values']['party_name'];
    $hul_code = get_hul_code_from_party_name($party_name, $distributor_id);
    
    $nid = get_nid_from_title($hul_code, 'hul_code_master', $distributor_id);

    $query = db_select('node', 'n');
    $query->condition('n.type', 'hul_code_master');
    $query->condition('n.title', $hul_code);
    $query->condition('n.uid', $distributor_id);
    $query->fields('n', array('title'));
    $query->join('field_data_field_hul_code_reference', 'ref', 'ref.field_hul_code_reference_nid = n.nid');
    $query->join('field_data_field_party_name', 'pn', 'pn.entity_id = ref.entity_id');
    $query->fields('pn', array('field_party_name_value'));
    $query->join('field_data_field_block_status', 'bs', 'bs.entity_id = n.nid');
    $query->addexpression("(CASE WHEN bs.field_block_status_value = 1 THEN 'Block' ELSE 'Unblock' END)", 'block_status');
    $result = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
    $form_state['block_party_billing']['hul_data'] = $result;

    $res = array();
    $query = db_select('node', 'n');
    $query->condition('n.type', 'hul_code_master');
    $query->condition('n.title', $hul_code);
    $query->condition('n.uid', $distributor_id);
    $query->join('field_data_field_hul_code_reference', 'ref', 'ref.field_hul_code_reference_nid = n.nid');
    $query->join('node', 'n1', 'ref.entity_id = n1.nid');
    $query->join('field_data_field_party_code_reference', 'pc', 'pc.field_party_code_reference_nid = n1.nid');
    $query->join('field_data_field_sr_bill_date', 'bd', 'bd.entity_id = pc.entity_id');
    $query->leftjoin('field_data_field_bill_status', 'bs', 'bs.entity_id = pc.entity_id');
    $query->join('node', 'n2', 'bd.entity_id = n2.nid');
    $query->fields('bd', array('field_sr_bill_date_value'));
    $query->fields('n2', array('title'));
    $query->fields('bs', array('field_bill_status_value'));
    $result1 = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

    foreach ($result1 as $v) {
        $v['field_sr_bill_date_value'] = date('d-M-y', $v['field_sr_bill_date_value']);
        $res[] = $v;
    }
    $form_state['block_party_billing']['bill_data'] = $res;


    $res1 = array();
    $query = db_select('block_party_history', 'bh');
    $query->condition('bh.hul_nid', $nid);
    $query->fields('bh', array('created_time'));
    $query->fields('bh', array('status'));
    $query->fields('bh', array('reason'));
    $query->fields('bh', array('remarks'));
    $result2 = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

    foreach ($result2 as $v) {
        $v['created_time'] = date('d-M-y', $v['created_time']);
        $res1[] = $v;
    }
    $form_state['block_party_billing']['block_data'] = $res1;

    $form_state['rebuild'] = TRUE;
}

function block_party_modify_submit($form, &$form_state) {
    $form_state['modify_status'] = 1;
    $form_state['rebuild'] = TRUE;
}

function modify_final_validate($form, &$form_state) {
    $hul_code = $form_state['values']['hul_code'];
    $status = $form_state['values']['status'];
    $reason_code = $form_state['values']['reason_code'];
    $remarks = $form_state['values']['remarks'];
    $distributor_id = $form_state['values']['distributor_id'];

    if ($status == "") {
        form_set_error('status', 'Status cannot be empty');
    }
    if (empty($reason_code)) {
        form_set_error('reason_code', 'Reason cannot be empty');
    }
    if (empty($remarks)) {
        form_set_error('remarks', 'Reamrks cannot be empty');
    }
}

function modify_final_submit($form, &$form_state) {
    $hul_code = $form_state['values']['hul_code'];
    $status = $form_state['values']['status'];
    $reason_code = $form_state['values']['reason_code'];
    $remarks = $form_state['values']['remarks'];
    $distributor_id = $form_state['values']['distributor_id'];
    
    $party_name = $form_state['values']['party_name'];
    $hul_code = get_hul_code_from_party_name($party_name, $distributor_id);

    $nid = get_nid_from_title($hul_code, 'hul_code_master', $distributor_id);
    $node = node_load($nid);
    $node->field_block_status[$node->language][0]['value'] = $status;
    $node->field_block_remarks[$node->language][0]['value'] = $reason_code;
    $node->field_block_reason[$node->language][0]['value'] = $remarks;
    node_save($node);

    db_insert('block_party_history')
            ->fields(array(
                'hul_nid' => $nid,
                'status' => ($status == 1) ? 'Block' : 'Unblock',
                'remarks' => $remarks,
                'reason' => $reason_code,
                'created_time' => REQUEST_TIME,
            ))
            ->execute();

    drupal_set_message(t('Data saved successfully for HUL Code : ' . $hul_code));
}

function get_hul_code_from_party_name($party_name, $distributor_id) {
    $query = db_select('field_data_field_party_name', 'pn');
    $query->condition('pn.bundle', 'outlet_master');
    $query->condition('pn.field_party_name_value', $party_name);
    $query->join('field_data_field_hul_code_reference', 'ref', 'ref.entity_id = pn.entity_id');
    $query->join('node', 'n', 'ref.field_hul_code_reference_nid = n.nid');
    $query->condition('n.uid', $distributor_id);
    $query->fields('n', array('title'));
    $result = $query->execute()->fetchField();
    return $result; 
}

function block_party_billing_callback($form, &$form_state) {
    $commands[] = ajax_command_replace('#itemlist-div', render($form));
    $commands[] = ajax_command_replace('.alert-danger', '');
    $commands[] = ajax_command_replace('.alert-success', '');
    $commands[] = ajax_command_prepend('#itemlist-div', theme('status_messages'));
    return array('#type' => 'ajax', '#commands' => $commands);
}
