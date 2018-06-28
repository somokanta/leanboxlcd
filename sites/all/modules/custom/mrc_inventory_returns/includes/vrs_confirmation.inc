<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * 
 * @param type $form
 * @param type $form_state
 * @return array
 */
module_load_include('inc', 'mrc_inventory_returns', 'includes/vrs_common');
function vrs_confirmation_form($form, &$form_state) {
    if (!empty($form_state['values']['trip_id']) || !empty($form_state['values']['trips'])) {
        $trip_id = $form_state['values']['trip_id'];
        if (empty($trip_id))
            $trip_id = $form_state['values']['trips'];
    }
    if (!isset($form_state['vrs_confirmation']['bills'])) {
        $form_state['vrs_confirmation']['bills'] = array();
    }
    $options = $form_state['vrs_confirmation']['bills'];
    $sum_s_register_bill_value = 0;
    global $user;
    $dis_id = change_feeds_import_get_distributor_id($user->uid);
    $form['#attached']['js'][] = drupal_get_path('module', 'mrc_inventory_returns') . '/js/vrs_confirmation.js';
    $form['#attributes'] = array('enctype' => 'multipart/form-data');
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
        '#ajax' => array(
            'callback' => 'vrs_confirmation_trip_id_callback',
            'event' => 'change',
            'wrapper' => 'get_tripid_dropdown',
            'method' => 'replace',
        ),
        '#prefix' => '<div class="col_3">',
        '#suffix' => '</div>'
    );
    $form['trips'] = array(
        '#type' => 'select',
        '#title' => 'Select Trip ID',
        '#prefix' => '<div id="get_tripid_dropdown" class="col_3">',
        '#suffix' => '</div>',
        '#attributes' => array('class' => array('trip-dropdown', 'col-lg-6')),
        '#ajax' => array(
            'callback' => 'vrs_confirmation_select_trip_id_callback',
            'wrapper' => 'cashier_names',
        ),
        
    );
   
    $form['cashier_name'] = array(
        '#type' => 'select',
        '#title' => 'Cashier Name',
        '#attributes' => array('class' => array('cashier_names')),
        '#prefix' => '<div class="col_2 col_2_2 cashier_name" id="cashier_names"><div>',
        '#suffix' => '</div></div>'
    );
    
    //if (isset($form_state['triggering_element']) && ($form_state['triggering_element']['#name'] == 'dispatch_date')) {
    if (!empty($form_state['values']['dispatch_date']) && is_string($form_state['values']['dispatch_date'])) {
        $form['trips']['#options'] = array('' => 'Select') + _get_trip_ids_using_dispatch_date($form_state['values']['dispatch_date']);
    }
    // }
    if (!empty($form_state['values']['trips'])) {
        $form['cashier_name']['#options'] =_get_cashier_name_using_trip_id($form_state['values']['trips']);
    }
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => 'Show',
        '#name' => 'show',
        '#ajax' => array(
            'callback' => 'vrs_confirmation_callback',
            'wrapper' => 'itemlist-div',
            'method' => 'replace',
            'effect' => 'fade',
        ),
        '#attributes' => array('class' => array('vrs-trip-submit')),
        '#submit' => array('display_vrs_trip_details'),
        '#validate' => array('validate_vrs_trip_details'),
        '#prefix' => '',
        '#suffix' => '</div>'
    );


    $bill_status_options = array('deliver' => 'Delivered'
        , 'partial_deliver' => 'Partial Delivered'
       // , 'delivery_pending' => 'Delivery Pending'
        );

    $form['bill_status'] = array(
        '#type' => 'select',
        '#id' => 'bill_status',
        '#title' => 'Delivery Outcome',
        '#options' => array('' => 'All') + $bill_status_options,
        '#prefix' => '<div class="form_row form_row2"><div class="col_2 col_2_2"> ',
    );
    
    $form['bill_status_undeliver'] = array(
        '#type' => 'checkbox',
        '#id' => 'bill_status_undeliver',
        '#title' => 'Show Full Returned as well',
        '#prefix' => '<div class="bill_status_delivered bill_status">',
        '#suffix' => '</div></div>'
    );
    $form['bill_number'] = array(
        '#type' => 'textfield',
        '#attributes' => array('class'=>array('bill_number')),
        '#id'=>'bill_number',
        '#title' => 'Bill Number',
        '#prefix' => '<div class="col_2 col_2_2"> ',
        '#suffix' => '</div>'
    );
    $payment_out_come = array('Cash' => 'Cash', 'Cheque' => 'Cheque', 'Signed Bill' => 'Signed Bill');
    $form['payment_outcome'] = array(
        '#type' => 'select',
        '#attributes' => array('class'=>array('payment_outcome','auto_complete'),'style'=>"height:32px"),
        '#id'=>'payment_outcome',
        '#title' => 'Payment Outcome',
        //'#autocomplete_path' => 'payment_outcome/autocomplete/'.$trip_id,
        '#options' => array('' => 'Select') + $payment_out_come,
        '#prefix' => '<div class="col_2 col_2_2"> ',
        '#suffix' => '</div>'
    );
    if (!empty($options)) {
        // VRS edit screen load existing data
        $vrs_version = get_current_vrs_version($trip_id, $dis_id);
        $bills = array();
        if (!empty($vrs_version)) {
            $bills = get_vrs_bill_items($trip_id, $dis_id, $vrs_version);
            $vrs_total = get_vrs_total_items($trip_id, $dis_id, $vrs_version);
            $total_key = key($vrs_total);
            $vrs_total_id = $vrs_total[$total_key]['id'];
            $vrs_created_date = $vrs_total[$total_key]['vrs_created_date'];
            $denominations = get_vrs_total_denominations($trip_id, $vrs_total_id, $dis_id);
            
        }
        foreach ($options as $key => $val) {
            $sum_s_register_bill_value += (float) $val['field_bill_value_value'];
            $trip_execution_date = $val['trip_execution_date'];
            $field_cashier_name = $val['field_cashier_name_value'];
        }
    }
    $form['bank_name'] = array(
        '#type' => 'textfield',
        '#title' => 'Bank Name',
        '#id'=>'bank_name',
        '#autocomplete_path' => 'bank_names/autocomplete/'.$trip_id,
        '#attributes'=>array('class'=>array('bank_name_filter'),'style'=>"height:32px"),
        '#prefix' => '<div class="col_2 col_2_2"> ',
        '#suffix' => '</div></div>'
    );
 
    
    $header = array(
        'Bill Number',
        'Delivery Outcome',
        'Payment Terms',
        'Payment Outcome',
        'Delivery App Bill Value',
        'Delivery App To Be Collected',
        'Bill Value',
        'To Be Collected',
        'Actual Cash Collected',
        'Actual Signed Bill Amount',
        'Signed Bill Image & Upload',
        'Actual Cheque Amount Collected',
        'Cheque Number',
        'Cheque Date',
        'Cheque Image & Upload',
        'Bank Name',
        'Short',
        'Short Reason Code',
        'Remarks',
        'Pending',
        'Pending Reason'
    );
   
    if (!empty($options)) {
        $trip_header_values[] = array($trip_id,$field_cashier_name,$sum_s_register_bill_value,$trip_execution_date);
        $short_header = array('Short Name', 'Quantity', 'Value');
        $trip_header = array('Trip ID','Cashier Name', 'Total Bill Value', 'Trip Execution Date');
        $footer_header = array('Count of Bills','Total Delivery App (To be collected)', 'Total To be collected','Total Cash Collected', 'Total Signed Bill Collected','Total Cheque Collected',  'Cashier Cash Short', 'Total Short', 'Total Cashier Debit Value');
        if (!empty($vrs_version)) {
              if (!empty($bills)) {
                $form['version_details'] = array(
                    '#type' => 'markup',
                    '#prefix' => '<div class="vrs_version" align="center"><h2>' . t('You are editing  VRS Version:' . $vrs_version),
                    '#suffix' => '</h2></div></br>',
                );
                $form['vrs_total_id'] = array(
                    '#type' => 'hidden',
                    '#value' => $vrs_total_id);

                $form['vrs_created_date'] = array(
                    '#type' => 'hidden',
                    '#value' => $vrs_created_date);
            }
        }
        $get_shorts = get_return_short($trip_id, $dis_id);
        $asset  = received_assets_count($trip_id, $dis_id) ;
        $sum_godown_short_qty = $get_shorts['sum_godown_short_qty'] ? $get_shorts['sum_godown_short_qty'] : 0;
        $sum_cashier_short_qty = $get_shorts['sum_cashier_short_qty'] ? $get_shorts['sum_cashier_short_qty'] : 0;
        $sum_godown_short_value = $get_shorts['sum_godown_short_value'] ? number_format((float) $get_shorts['sum_godown_short_value'], 2, '.', '') : 0;
        $sum_cashier_short_value = $get_shorts['sum_cashier_short_value'] ? number_format((float) $get_shorts['sum_cashier_short_value'], 2, '.', '') : 0;
        $asset_short_qty = $asset['received_qty'] ? $asset['received_qty'] : 0;
        $asset_short_value = $asset['value'] ? $asset['value'] : 0;
        $short_values[] = array('Godown Short', $sum_godown_short_qty, $sum_godown_short_value);
        $short_values[] = array('Cashier Short', $sum_cashier_short_qty, $sum_cashier_short_value);
        $short_values[] = array('Assets Short', $asset_short_qty, $asset_short_value);
        $asset_total = 0;
        if (!empty($asset_short_value)) {
            $asset_total = $asset_short_value;
            
            $form['asset_qty'] = array(
                '#type' => 'hidden',
                '#value' => $asset['received_qty'],
            );
            $form['asset_value'] = array(
                '#type' => 'hidden',
                '#value' => $asset['value'],
            );
            $form['asset_total_value'] = array(
                '#type' => 'hidden',
                '#value' => $asset_total,
            );
        }
        $form['godown_short_qty'] = array(
            '#type' => 'hidden',
            '#value' => $sum_godown_short_qty,
        );
        $form['godown_short_value'] = array(
            '#type' => 'hidden',
            '#value' => $sum_godown_short_value,
        );
        $form['cashier_short_qty'] = array(
            '#type' => 'hidden',
            '#value' => $sum_cashier_short_qty,
        );
        $form['cashier_short_value'] = array(
            '#type' => 'hidden',
            '#value' => $sum_cashier_short_value,
        );
        
        $form['header_details1'] = array(
            '#theme' => 'table',
            '#tree' => TRUE,
            '#header' => $trip_header,
            '#rows' => $trip_header_values,
            '#attributes' => array(
                'class' => array('header_details'),
                'id' => 'trip_header'
            ),
            '#prefix' => '<div class="custom_header_table">',
            '#suffix' => '</div>',
        );
        
        $form['header_details'] = array(
            '#theme' => 'table',
            '#tree' => TRUE,
            '#header' => $short_header,
            '#rows' => $short_values,
            '#attributes' => array(
                'class' => array('header_details'),
                'id' => 'header_details'
            ),
            '#prefix' => '<div class="custom_header_table">',
            '#suffix' => '</div>',
        );
        $form['bill_details'] = array(
            '#theme' => 'table',
            '#prefix' => '<div class="custom_middle_table vrs_middle_table">',
            '#tree' => TRUE,
            '#header' => $header,
            '#rows' => array(),
            '#attributes' => array(
                'class' => array('bill_details'),
                'id' => 'bill_details'
            ),
            '#suffix' => '</div>',
           // '#sticky' => FALSE,
        );
        $form['footer_details'] = array(
            '#theme' => 'table',
            '#prefix' => '</br><div class="footer_table">',
            '#tree' => TRUE,
            '#header' => $footer_header,
            '#rows' => array(),
            '#attributes' => array(
                'class' => array('footer_details'),
                'id' => 'footer_details'
            ),
            '#suffix' => '</div>',
        );

        $text_box_size = 8;
        $total_sales_register = 0;
        $total_actual_cash_collected_value = 0;
        $total_cheque_collected_value = 0;
        $total_signed_bill_value = 0;
        $short_total = 0;
        $non_bankable_reason_options = array();
        $short_reason_options = array();
        $short_reasons = get_short_reasons();
        $non_bankable_reasons = get_non_bankable_reasons();
        if (!empty($short_reasons)) {
            $short_reason_options = $short_reasons;
        }
        if (!empty($non_bankable_reasons)) {
            $non_bankable_reason_options = $non_bankable_reasons;
        }
        // Bill level table details

        foreach ($options as $key => &$val) {
            $key = str_replace(' ', '', $key);
            // Checking Farery values present in  edit field also 
            if (empty($bills)) {
                         
                $val['res_cheque_date'] = !empty($val['res_cheque_date_edit']) ? $val['res_cheque_date_edit'] : $val['res_cheque_date'];
                $val['res_cash_amount'] = !empty($val['res_cash_amount_edit']) ? round($val['res_cash_amount_edit']) : round($val['res_cash_amount']);
                $val['res_photo_of_cheque'] = !empty($val['res_photo_of_cheque_edit']) ? $val['res_photo_of_cheque_edit'] : $val['res_photo_of_cheque'];
                $val['res_cheque_amount'] = !empty($val['res_cheque_amount_edit']) ? round($val['res_cheque_amount_edit']) : round($val['res_cheque_amount']);
                $val['res_money_collect2'] = !empty($val['res_money_collect2_edit']) ? $val['res_money_collect2_edit'] : $val['res_money_collect2'];
                $val['photo_of_signed_bill_partial'] = !empty($val['photo_of_signed_bill_partial_edit']) ? $val['photo_of_signed_bill_partial_edit'] : $val['photo_of_signed_bill_partial'];
                $val['res_confirm_signed_bill_amount'] = !empty($val['res_confirm_signed_bill_amount_edit']) ? $val['res_confirm_signed_bill_amount_edit'] : $val['res_confirm_signed_bill_amount'];
                $val['res_bank_name'] = !empty($val['res_bank_name_edit']) ? $val['res_bank_name_edit'] : $val['res_bank_name'];
                $val['res_photo_of_signed_bill_delivery'] = !empty($val['res_photo_of_signed_bill_delivery_edit']) ? $val['res_photo_of_signed_bill_delivery_edit'] : $val['res_photo_of_signed_bill_delivery'];
                // Yes or No column
                $val['res_signed_bill_delivery'] = !empty($val['res_signed_bill_delivery_edit']) ? $val['res_signed_bill_delivery_edit'] : $val['res_signed_bill_delivery'];
                $val['res_signed_bill_partial'] = !empty($val['res_signed_bill_partial_edit']) ? $val['res_signed_bill_partial_edit'] : $val['res_signed_bill_partial'];
                $val['res_cheque_no'] = !empty($val['res_cheque_no_edit']) ? $val['res_cheque_no_edit'] : $val['res_cheque_no'];
                $val['res_confirm_signed_bill_amount'] = round($val['res_confirm_signed_bill_amount']);
            }
            
            //$val['sales_register_to_be_collected_amt'] = number_format((float) $val['sales_register_to_be_collected_amt'], 2, '.', '');
            
            // to be collected should be zero in the case of undeliver
            if($val['res_status'] == 'undeliver') {
                $val['sales_register_to_be_collected_amt'] = 0;
            }
            else {
                $val['sales_register_to_be_collected_amt'] = round($val['sales_register_to_be_collected_amt']);   
            }
                
           $val['to_be_collected_amt'] = round($val['to_be_collected_amt']);
            
            //'res_bank_name','res_status','res_cheque_date','res_bank_name'
            //$form['bill_details']['#rows'][$key][] = $val['ad_bill_number'];
            $form['bill_details']['#rows'][$key][] = $val['bill_number'];
             $undeliver = 0; 
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
            // Payment Outcome string
            if (($bills[$key]['actual_signed_bill_amount']>0) || ($val['res_confirm_signed_bill_amount']>0)) {
                $payment_term[] = t('Signed Bill');
            }
            if (($bills[$key]['actual_cheque_amount']>0) || ($val['res_cheque_amount']>0)) {
                $payment_term[] = t('Cheque');
            }
            if (($bills[$key]['actual_cash_amount']>0) || ($val['res_cash_amount']>0)) {
                $payment_term[] = t('Cash');
            }
            if (!empty($payment_term)) {
                 $payments_terms = implode(',',$payment_term);
            }
            $form['bill_details']['#rows'][$key][] = $bill_status;
            $form['bill_details']['#rows'][$key][] = $val['field_payment_terms_value'];
            $form['bill_details']['#rows'][$key][] = $payments_terms;
            $form['bill_details']['#rows'][$key][] = $val['bill_value'];
            $form['bill_details']['#rows'][$key][] = $val['to_be_collected_amt']; 
            $total_sales_register +=  $val['sales_register_to_be_collected_amt'];
            // Farery bill value
            $total_to_be_collected +=  $val['to_be_collected_amt'];
            $total_signed_bill_value +=  $val['res_confirm_signed_bill_amount'];
            $total_actual_cash_collected_value +=  $val['res_cash_amount'];
            $total_cheque_collected_value +=  $val['res_cheque_amount'];
            
            $bank_name = array(
                //'#id' => 'bill_details-' . $key . '-bank_name',
                '#attributes' => array('class' => array('bank_name'),'title'=>t('Bank Name')),
                '#type' => 'textfield',
                '#autocomplete_path' => 'get_bank_names/autocomplete/'.$trip_id,
                '#size' => $text_box_size,
                '#default_value' => $vrs_total_id ? $bills[$key]['bank_name'] :  $val['res_bank_name']
            );
            $cheque_number = array(
                //'#id' => 'bill_details-' . $key . '-cheque_number',
                '#attributes' => array('class' => array('cheque_number'),'title'=>t('Cheque Number')),
                '#type' => 'textfield',
                '#size' => $text_box_size,
                '#default_value' => $vrs_total_id ? $bills[$key]['cheque_number'] : $val['res_cheque_no']
            );
            if (!empty($bills[$key]['cheque_date'])) {
                $bills[$key]['cheque_date'] =  date('Y-m-d', $bills[$key]['cheque_date']); 
            }
            else if (!empty($val['res_cheque_date'])) {
                if (is_numeric($val['res_cheque_date'])) {
                    $val['res_cheque_date'] = date('Y-m-d', $val['res_cheque_date']);
                }
            }
            $cheque_date = array(
                //'#id' => 'bill_details-' . $key . '-cheque_date',
                '#attributes' => array('class' => array('cheque_date'),'title'=>t('Cheque Date')),
                '#type' => 'date_popup',
                '#date_format' => 'Y-m-d',
                '#date_label_position' => '',
                '#date_year_range' => '-20:+2',
                '#size' => $text_box_size,
                '#default_value' => $vrs_total_id ?  $bills[$key]['cheque_date'] : $val['res_cheque_date']
            );
            $sales_register_tobe_collected = array(
                //'#id' => 'bill_details-' . $key . '-sales_register_tobe_collected',
                '#attributes' => array('class' => array('sales_register_tobe_collected'), 'readonly' => 'readonly','title'=>t('To Be collected')),
                '#type' => 'textfield',
                '#size' => $text_box_size,
                '#default_value' => $val['sales_register_to_be_collected_amt'],
            );
            $s_register_bill_value = array(
                //'#id' => 'bill_details-' . $key . '-s_register_bill_value',
                '#attributes' => array('class' => array('s_register_bill_value'), 'readonly' => 'readonly','title'=>t('Bill Value')),
                '#type' => 'textfield',
                '#size' => $text_box_size,
                '#default_value' => round($val['field_bill_value_value']),
            );
            

            $actual_cash_collected = array(
                //'#id' => 'bill_details-' . $key . '-actual_cash_collected',
                '#attributes' => array('class' => array('short_cal', 'actual_cash_collected','allownumericwithoutdecimal'),'title'=>t('Actual Cash Collected')),
                '#type' => 'textfield',
                '#size' => $text_box_size,
                '#default_value' => $vrs_total_id ? round($bills[$key]['actual_cash_amount']) : round($val['res_cash_amount']),
            );
            $actual_cheque_collected = array(
               // '#id' => 'bill_details-' . $key . '-actual_cheque_collected',
                '#type' => 'textfield',
                '#attributes' => array('class' => array('short_cal', 'actual_cheque_amt_collected','allownumericwithoutdecimal'),'title'=>t('Actual Cheque Amount')),
                '#size' => $text_box_size,
                '#default_value' => $vrs_total_id ? round($bills[$key]['actual_cheque_amount']) : round($val['res_cheque_amount']),
            );
            $signed_bill_collected = array(
               // '#id' => 'bill_details-' . $key . '-signed_bill_collected',
                '#type' => 'textfield',
                '#attributes' => array('class' => array('signed_bill_collected', 'short_cal','allownumericwithoutdecimal'),'title'=>t('Signed Bill Amount')),
                '#size' => $text_box_size,
                '#default_value' => $vrs_total_id ? round($bills[$key]['actual_signed_bill_amount']) : round($val['res_confirm_signed_bill_amount']),
            );
            $short =  $val['sales_register_to_be_collected_amt'] -  $val['res_cash_amount'] -  $val['res_cheque_amount'] -  $val['res_confirm_signed_bill_amount'];
            $short_display = $short;
            $short_total += $short;
            $vrs_short = round($val['sales_register_to_be_collected_amt']) - round($bills[$key]['actual_cash_amount']) - round($bills[$key]['actual_signed_bill_amount']) - round($bills[$key]['actual_cheque_amount']) ;
            if ($vrs_short <=1 && $vrs_short>=0) {
                $vrs_short = 0;
            }
            
            $short_collected = array(
               // '#id' => 'bill_details-' . $key . '-short_collected',
                '#type' => 'textfield',
                '#attributes' => array('class' => array('short'), 'data' => $val['sales_register_to_be_collected_amt'],'title'=>t('Short Amount')),
                '#size' => $text_box_size,
                '#default_value' => $vrs_total_id ? $vrs_short : $short_display,
            );
            // Fetch Reason from Short Reason Master
            //if ($short_display > 0) {
            
            //}
            $short_reason_code = array(
              //  '#id' => 'bill_details-' . $key . '-short_reason_code',
                '#type' => 'select',
                '#attributes' => array('class' => array('short_reason_code'),'title'=>t('Short Reason Code')),
                '#default_value' => $bills[$key]['reason_code'] ? $bills[$key]['reason_code'] : '',
                '#options' => array('0' => 'Select') + $short_reason_options,
            );
            
             $non_bankable_reason_code = array(
              //  '#id' => 'bill_details-' . $key . '-non_bankable_reason_code',
                '#type' => 'select',
                '#attributes' => array('class' => array('non_bankable_reason_code'),'title'=>t('Pending Reson Code')),
                '#default_value' => $bills[$key]['non_bank_reason_code'] ? $bills[$key]['non_bank_reason_code'] : '',
                '#options' => array('0' => 'Select') + $non_bankable_reason_options,
            );
            $remarks = array(
              //  '#id' => 'bill_details-' . $key . '-remarks',
                '#type' => 'textfield',
                '#attributes' => array('class' => array('remarks'),'title'=>t('Remarks')),
                '#size' => $text_box_size,
                '#default_value' => $bills[$key]['remarks'] ? $bills[$key]['remarks'] : '',
            );
            $non_billable = array(
              //  '#id' => 'bill_details-' . $key . '-non_billable',
                '#type' => 'checkbox',
                '#attributes' => array('class' => array('non_billable'),'title'=>t('Pending')),
                '#size' => $text_box_size,
                '#default_value' => $bills[$key]['non_billable'] ? $bills[$key]['non_billable'] : '',
            );
            // 'res_photo_of_cheque', 'res_photo_of_signed_bill_delivery', 'photo_of_signed_bill_partial'
            global $base_url;
            // Fetching it from farey table
            $ph_cheque = '<div class ="vrs_signed_bill_image">';
            $ph_flag = 0;
            $span_class = '<span style = "margin-left:18px;"></span>';
            if (!empty($bills[$key]['photo_cheque'])) {
                $url = file_create_url($bills[$key]['photo_cheque']);
                $url = parse_url($url);
                $path = $base_url.'/'.$url['path'];
                $ph_cheque.= '<a target="_blank" href="'.$path.'">';
                $ph_cheque.= t('File');
                $ph_cheque.= '</a>';
                $ph_flag = 1;
            }
            else if (!empty($val['res_photo_of_cheque'])) {
                $url = $val['res_photo_of_cheque'];
                $na_position =  strpos($url,'/NA');
                $path = $url;
                if (empty($na_position)) {
                    $wrapper = file_stream_wrapper_get_instance_by_uri($url) ;
                    if (!empty($wrapper)) {
                        $path = $wrapper->getExternalUrl();
                    }
                    $ph_cheque .= '<a target="_blank" href="' . $path . '">';
                    $ph_cheque .= t('File');
                    $ph_cheque .= '</a>';
                    $ph_flag = 1;
                } else {
                    $ph_flag = 0;
                }
                
            }
             if ($ph_flag == 0){
                 $ph_cheque.= $span_class;
             }
              if ($ph_flag == 1){
                 $form['photo_cheque_'.$key] = array(
                    '#type' => 'hidden',
                    '#value' => $path,
                );
            }
             
             $ph_cheque.= '</div>';    
            
            $photo_cheque = array(
                '#type' => 'file',
                '#attributes'=>array('title'=>t('Cheque Image')),
                '#name' => 'files[cheque_' . $key . ']',
               // '#id' => 'bill_details_' . $key . '_photo_cheque',
                '#prefix' => '<div>' . $ph_cheque,
                '#suffix' => '</div>',
            );
            unset($path);
            unset($url);
            unset($wrapper);
            $ph_sign_flag = 0;
            $ph_signed_bill= '<div class ="vrs_signed_bill_image">';
            if (!empty($bills[$key]['photo_signed_bill'])) {
                $url = file_create_url($bills[$key]['photo_signed_bill']);
                $url = parse_url($url);
                $path = $base_url.'/'.$url['path'];
                $ph_signed_bill.= '<a target="_blank" href="'.$path.'">';
                $ph_signed_bill.= t('File');
                $ph_signed_bill.= '</a>';
                $ph_sign_flag = 1;
            } 
            elseif (!empty($val['res_photo_of_signed_bill_delivery']) || !empty($val['photo_of_signed_bill_partial'])) {
                $url = $val['photo_of_signed_bill_partial'];
                if (empty($url)) {
                    $url = $val['res_photo_of_signed_bill_delivery'];
                }
                $na_position =  strpos($url,'/NA');
                $path = $url;
                if (empty($na_position)) {
                    $wrapper = file_stream_wrapper_get_instance_by_uri($url) ;
                    if (!empty($wrapper)) {
                        $path = $wrapper->getExternalUrl();
                    }
                    $ph_signed_bill.= '<a target="_blank" href="'.$path.'">';
                    $ph_signed_bill.= t('File');
                    $ph_signed_bill.= '</a>';
                    $ph_sign_flag = 1;
                }
                else {
                    $ph_sign_flag = 0;
                }
                
            }
             if ($ph_sign_flag == 0){
                 $ph_signed_bill.= $span_class;
             }
             if ($ph_sign_flag == 1){
                 $form['photo_signed_bill_'.$key] = array(
                    '#type' => 'hidden',
                    '#value' => $path,
                );
            }
            $ph_signed_bill.= '</div>';   
            $photo_signed_bill = array(
                //'#id' => 'bill_details-' . $key . '-photo_signed_bill',
                '#name' => 'files[signedbill_' . $key . ']',
                '#type' => 'file',
                '#prefix' => '<div>' . $ph_signed_bill,
                '#suffix' => '</div>',
                '#attributes' => array('class' => array('photo_signed_bill'),'title'=>t('Signed Bill Image')),
            );
            $form['bill_details'][$key]["short_reason_code"] = &$short_reason_code;
            $form['bill_details'][$key]["sales_register_tobe_collected"] = &$sales_register_tobe_collected;
            $form['bill_details'][$key]["s_register_bill_value"] = &$s_register_bill_value;
            $form['bill_details'][$key]["bank_name"] = &$bank_name;
            $form['bill_details'][$key]["cheque_number"] = &$cheque_number;
            $form['bill_details'][$key]["cheque_date"] = &$cheque_date;
            $form['bill_details'][$key]["actual_cash_collected"] = &$actual_cash_collected;
            $form['bill_details'][$key]["actual_cheque_collected"] = &$actual_cheque_collected;
            $form['bill_details'][$key]["photo_cheque"] = &$photo_cheque;
            $form['bill_details'][$key]["photo_signed_bill"] = &$photo_signed_bill;
            $form['bill_details'][$key]["signed_bill_collected"] = &$signed_bill_collected;
            $form['bill_details'][$key]["remarks"] = &$remarks;
            $form['bill_details'][$key]["non_billable"] = &$non_billable;
            $form['bill_details'][$key]["non_bankable_reason_code"] = &$non_bankable_reason_code;
            
            $form['bill_details'][$key]["short_collected"] = &$short_collected;
       
            if ($undeliver == 1) {
                $na = 'NA';
                 $form['bill_details']['#rows'][$key][] = round($val['field_bill_value_value']);
                 $form['bill_details']['#rows'][$key][] = round($val['sales_register_to_be_collected_amt']);
                 $form['bill_details']['#rows'][$key][] = $val['res_cash_amount'];
                 $form['bill_details']['#rows'][$key][] = $val['res_confirm_signed_bill_amount'];
                 $form['bill_details']['#rows'][$key][] = $ph_signed_bill;
                 $form['bill_details']['#rows'][$key][] = $val['res_cheque_amount'];
                 $form['bill_details']['#rows'][$key][] = $val['cheque_number'];
                 $form['bill_details']['#rows'][$key][] = $val['res_cheque_date'];
                 $form['bill_details']['#rows'][$key][] = $ph_cheque;
                 $form['bill_details']['#rows'][$key][] = $val['res_bank_name'];
                 $form['bill_details']['#rows'][$key][] = 0;
                 $form['bill_details']['#rows'][$key][] = $na;
                 $form['bill_details']['#rows'][$key][] = $na;
                 $form['bill_details']['#rows'][$key][] = $na;
            }
            else {
                $form['bill_details']['#rows'][$key][]['data']["s_register_bill_value"] = &$s_register_bill_value;
                $form['bill_details']['#rows'][$key][]['data']["sales_register_tobe_collected"] = &$sales_register_tobe_collected;
                $form['bill_details']['#rows'][$key][]['data']["actual_cash_collected"] = &$actual_cash_collected;
                $form['bill_details']['#rows'][$key][]['data']["signed_bill_collected"] = &$signed_bill_collected;
                $form['bill_details']['#rows'][$key][]['data']["photo_signed_bill"] = &$photo_signed_bill;
                $form['bill_details']['#rows'][$key][]['data']["actual_cheque_collected"] = &$actual_cheque_collected;
                $form['bill_details']['#rows'][$key][]['data']["cheque_number"] = &$cheque_number;
                $form['bill_details']['#rows'][$key][]['data']["cheque_date"] = &$cheque_date;
                $form['bill_details']['#rows'][$key][]['data']["photo_cheque"] = &$photo_cheque;

                $form['bill_details']['#rows'][$key][]['data']["bank_name"] = &$bank_name;
                $form['bill_details']['#rows'][$key][]['data']["short_collected"] = &$short_collected;
                $form['bill_details']['#rows'][$key][]['data']["short_reason_code"] = &$short_reason_code;
                $form['bill_details']['#rows'][$key][]['data']["remarks"] = &$remarks;
                $form['bill_details']['#rows'][$key][]['data']["non_billable"] = &$non_billable;
                $form['bill_details']['#rows'][$key][]['data']["non_bankable_reason_code"] = &$non_bankable_reason_code;
                
            }
            $form['bill_details']['#rows'][$key][] = $vrs_total_id ? $bills[$key]['bank_name'] :  $val['res_bank_name'];
            unset($actual_cash_collected);
            unset($actual_cheque_collected);
            unset($signed_bill_collected);
            unset($short_collected);
            unset($sales_register_tobe_collected);
            unset($short_reason_code);
            unset($bank_name);
            unset($cheque_date);
            unset($cheque_number);
            unset($s_register_bill_value);
            unset($remarks);
            unset($photo_cheque);
            unset($photo_signed_bill);
            unset($non_billable);
            unset($path);
            unset($ph_signed_bill);
            unset($ph_cheque);
            unset($url);
            unset($non_bankable_reason_code);
            unset($payment_term);
            unset($payments_terms);
        }
        
        if (!empty($bank_names)) {
            $form['bank_name']['#options'] = array('' => 'All') + $bank_names;
            $b_names = $bank_names;
            
        }

        $bill_count = count($options);
        $key = rand(1,20);
        //$total_sales_register = number_format($total_sales_register, 2, '.', '');
        $total_sales_register = round($total_sales_register);
        $total_to_be_collected_display = round($total_to_be_collected);
        //$total_actual_cash_collected_value_display = number_format($total_actual_cash_collected_value, 2, '.', '');
        $total_actual_cash_collected_value_display = round($total_actual_cash_collected_value);
        
        //$total_cheque_collected_value_display = number_format($total_cheque_collected_value, 2, '.', '');
        $total_cheque_collected_value_display = round($total_cheque_collected_value);
        $total_signed_bill_value_display = number_format($total_signed_bill_value, 2, '.', '');
        $total_actual_cash_collected = array(
            '#id' => 'footer_details-' . $key . '-total_actual_cash_collected',
            '#attributes' => array('class' => array('total_actual_cash_collected', 'cashier_total','allownumericwithoutdecimal'),'readonly'=>'readonly'),
            '#type' => 'textfield',
            '#size' => $text_box_size,
            '#default_value' => !empty($vrs_total[$total_key]) ? round($vrs_total[$total_key]['total_actual_cash_amount']) : $total_actual_cash_collected_value_display,
        );

        $total_cheque_collected = array(
            '#id' => 'footer_details-' . $key . '-total_cheque_collected',
            '#type' => 'textfield',
            '#attributes' => array('class' => array('total_cheque_collected', 'cashier_total','allownumericwithoutdecimal'),'readonly'=>'readonly'),
            '#size' => $text_box_size,
            '#default_value' => !empty($vrs_total[$total_key]) ? round($vrs_total[$total_key]['total_actual_cheque_amount']) : $total_cheque_collected_value_display,
        );
        $total_signed_bill_collected = array(
             '#id' => 'footer_details-' . $key . '-total_signed_bill_collected',
            '#type' => 'textfield',
            '#attributes' => array('class' => array('total_signed_bill_collected', 'cashier_total'),'readonly'=>'readonly'),
            '#size' => $text_box_size,
            '#default_value' => !empty($vrs_total[$total_key]) ? round($vrs_total[$total_key]['total_actual_signed_bill_amount']) : $total_signed_bill_value_display,
        );
        //.Cashier cash short = Footer Level (To be collected - actual cash collected - actual cheque collected - actual Signed bill collected - short). Keep this field editable for now
        $total_cashier_cash_short_value = $total_sales_register - $total_actual_cash_collected_value - $total_cheque_collected_value - $total_signed_bill_value - $short_total;
        $short_display = number_format($short_total, 2, '.', '');
        $total_cashier_cash_short_display = number_format($total_cashier_cash_short_value, 2, '.', '');
        //Total Cashier Debit Value = Cashier cash short at footer level + cashier short value from header
        $total_cashier_debit_value = $total_cashier_cash_short_value + $sum_cashier_short_value + $asset_total;
        $total_cashier_debit_value = number_format($total_cashier_debit_value, 2, '.', '');
        $total_cashier_cash_short = array(
            '#id' => 'footer_details-' . $key . '-total_cashier_cash_short',
            '#type' => 'textfield',
            '#attributes' => array('class' => array('total_cashier_cash_short'),'readonly'=>'readonly'),
            '#size' => $text_box_size,
            '#default_value' => !empty($vrs_total[$total_key]) ? $vrs_total[$total_key]['total_cashier_cash_short'] : $total_cashier_cash_short_display,
        );
        $short_collected = array(
             '#id' => 'footer_details-' . $key . '-short_collected',
            '#type' => 'textfield',
            '#attributes' => array('class' => array('short_collected', 'cashier_total'), 'readonly' => 'readonly'),
            '#size' => $text_box_size,
            '#default_value' => (!empty($vrs_total[$total_key])) ? $vrs_total[$total_key]['total_short_amt'] : $short_display,
        );
        $total_cashier_debit_value_field = array(
            '#id' => 'footer_details-' . $key . '-total_cashier_debit_value',
            '#type' => 'textfield',
            '#attributes' => array('class' => array('total_cashier_debit_value'), 'readonly' => 'readonly', 'data' => $sum_cashier_short_value, 'total_sregister' =>$total_sales_register,'asset_total'=>$asset_total),
            '#size' => $text_box_size,
            '#default_value' => !empty($vrs_total[$total_key]) ? $vrs_total[$total_key]['total_cashier_debit_value'] : $total_cashier_debit_value,
        );
        $total_s_register = array(
            '#id' => 'footer_details-' . $key . '-total_s_register',
            '#type' => 'textfield',
            '#attributes' => array('class' => array('total_s_register'), 'readonly' => 'readonly'),
            '#size' => $text_box_size,
            '#default_value' => $total_sales_register,
        );
        $total_farey = array(
              '#id' => 'footer_details-' . $key . '-total_farey',
            '#type' => 'textfield',
            '#attributes' => array('class' => array('total_farey'), 'readonly' => 'readonly'),
            '#size' => $text_box_size,
            '#default_value' => $total_to_be_collected_display,
        );

        $form['footer_details'][$key]["total_actual_cash_collected"] = &$total_actual_cash_collected;
        $form['footer_details'][$key]["total_cheque_collected"] = &$total_cheque_collected;
        $form['footer_details'][$key]["total_signed_bill_collected"] = &$total_signed_bill_collected;
        $form['footer_details'][$key]["total_cashier_cash_short"] = &$total_cashier_cash_short;
        $form['footer_details'][$key]["short_collected"] = &$short_collected;
        $form['footer_details'][$key]["total_s_register"] = &$total_s_register;
        $form['footer_details'][$key]["total_cashier_debit_value"] = &$total_cashier_debit_value_field;
        $form['footer_details'][$key]["total_farey"] = &$total_farey;
        $form['footer_details']['#rows'][$key][] = $bill_count;
        //$form['footer_details']['#rows'][$key][] = $total_sales_register; 
        $form['footer_details']['#rows'][$key][]['data']["total_farey"] = &$total_farey;
        $form['footer_details']['#rows'][$key][]['data']["total_s_register"] = &$total_s_register;
        $form['footer_details']['#rows'][$key][]['data']["total_actual_cash_collected"] = &$total_actual_cash_collected;
        $form['footer_details']['#rows'][$key][]['data']["total_signed_bill_collected"] = &$total_signed_bill_collected;
        $form['footer_details']['#rows'][$key][]['data']["total_cheque_collected"] = &$total_cheque_collected;
        $form['footer_details']['#rows'][$key][]['data']["total_cashier_cash_short"] = &$total_cashier_cash_short;
        $form['footer_details']['#rows'][$key][]['data']["short_collected"] = &$short_collected;
        $form['footer_details']['#rows'][$key][]['data']["total_cashier_debit_value"] = &$total_cashier_debit_value_field;
        $form['to_be_collected'] = array(
            '#type' => 'hidden',
            '#attributes' => array('id' => 'to_be_collected'),
            '#value' => $total_sales_register,
        );
        $form['total_short'] = array(
            '#type' => 'hidden',
            '#attributes' => array('id' => 'total_short'),
            '#value' => $short_display,
        );
        if (empty($vrs_version)) {
            $version_number = 'V1.0';
        }
        else {
            $version_number = $vrs_version;
        }
        $form['confirm_vrs'] = array(
            '#type' => 'submit',
            '#value' => 'Confirm VRS',
            '#pefix' => '<div class ="confirm_vrs">',
            '#suffix' => '</div">',
            '#name' => 'confirm_vrs',
            '#attributes' => array('class' => array('vrs-trip-submit'),
            'onclick' => 'if(!confirm("Do you want to Confirm VRS version :'.$version_number.' of Trip ID: ' . $trip_id . '?")){return false;}',
            'style' => array('float: left;')),
                '#submit' => array('vrs_confirmation_form_submit'),
                '#validate' => array('vrs_confirmation_validate'),
        );
        $note_denominations = get_notes_denominations();
        if (!empty($note_denominations)) {
            $form['note_denominations'] = array(
                '#attributes' => array('class' => array('note_denominations')),
                '#prefix' => '<div class ="note_denomination_popup" id="note_denomination_popup">'
                .'<div class="img_close_btn"><img class="img_close" src="'.drupal_get_path('module', 'mrc_inventory_returns').'/images/close_btn.png"></img></div>'
                . '<div class="note_txt"> ' . t('Note denominations.Please enter note denominations.') . '</div><br>',
                '#type' => 'markup',
            );

            $coins_value = 0;
            foreach ($note_denominations as $key => $value) {
                $default_value = 0;
                if (!empty($denominations[$value['nid']]['coins'])) {
                    $coins_value = $denominations[$value['nid']]['coins'];
                }
                if (!empty($denominations[$value['nid']]['no_of_notes'])) {
                    $default_value = $denominations[$value['nid']]['no_of_notes'];
                }
                $form['note_denominations']['note_denominations_' . $value['nid']] = array(
                    '#attributes' => array('class' => array('note_denominations_text','allownumericwithoutdecimal'), 'data' => t($value['title'])),
                    '#prefix' => '<div class="note_2col">',
                    '#title' => t($value['title']),
                    '#type' => 'textfield',
                    '#suffix' => '</div>',
                    '#size' => $text_box_size,
                    '#default_value' => $default_value,
                );
            }
            $form['note_denominations']['coins'] = array(
                '#attributes' => array('class' => array('coins','allownumericwithoutdecimal')),
                '#prefix' => '<div class="note_2col">',
                '#title' => t('Coins Value'),
                '#type' => 'textfield',
                '#suffix' => '</div>',
                '#size' => $text_box_size,
                '#default_value' => $coins_value,
            );
            $form['note_denominations']['close_button'] = array(
                '#type' => 'markup',
                '#value' => 'Close',
                '#suffix' => ' <div class="clear"></div><div class="note_btn_wrap"><input id="close_button" class ="btn" type ="button" value ="Calculate">
                               <input id="close_popup_btn" class ="btn" type ="button" value ="Close"></div>
                              </div>',
                '#name' => 'close_btn',
            );
        }
    }
    return $form;
}

function vrs_confirmation_callback($form, &$form_state) {
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
    
    //$form_state['rebuild'] = TRUE;
    //return $form;
}

function vrs_confirmation_trip_id_callback($form, &$form_state) {
    return $form['trips'];
}

function vrs_confirmation_select_trip_id_callback($form, &$form_state) {
    return $form['cashier_name'];
}

function validate_vrs_trip_details($form, &$form_state) {
    global $user;
    $dis_id = change_feeds_import_get_distributor_id($user->uid);
    $trip_id = $form_state['values']['trip_id'];
    if (empty($trip_id)) {
        $trip_id = $form_state['values']['trips'];
    }
    if (!empty($trip_id)) {
        //1- Return confirmed, 3- Vrs pending
        // Checking VRS Status 
        /*0|Manual Edit
        1|Returns Confirmed
        2|VRS Confirmed
        3|VRS Pending
        4|Handover InProgress
        5|Handover Complete*/

        $check_returns_flag = check_trip_id_exists_or_not($dis_id, $trip_id);
        if (empty($check_returns_flag)) {
            form_set_error('trip_id', "Trip ID :$trip_id does not exists.");
            ajax_redirect('vrs-confirmation');
        } 
        else if ($check_returns_flag->field_vrs_status_value == 2) {
            unset($form_state['vrs_confirmation']['bills']);
            form_set_error('trip_id', "Trip ID :$trip_id – VRS Status already Confirmed.");
            ajax_redirect('vrs-confirmation');
        }
        else if ($check_returns_flag->field_vrs_status_value == 4) {
            unset($form_state['vrs_confirmation']['bills']);
            form_set_error('trip_id', "Trip ID :$trip_id – is in Handover InProgress state.");
            ajax_redirect('vrs-confirmation');
        }
        else if ($check_returns_flag->field_vrs_status_value == 5) {
            unset($form_state['vrs_confirmation']['bills']);
            form_set_error('trip_id', "Trip ID :$trip_id – is in Handover Complete state.");
            ajax_redirect('vrs-confirmation');
        }
        else if (empty($check_returns_flag->field_vrs_status_value)) {
            unset($form_state['vrs_confirmation']['bills']);
            form_set_error('trip_id', "Trip ID:$trip_id-Please confirm Return Inventory first.");
            ajax_redirect('vrs-confirmation');
        } 
       
    } else {
        form_set_error('trip_id', '');
        form_set_error('trips', 'Please select or enter Trip ID.');
        ajax_redirect('vrs-confirmation');
    }
}

function display_vrs_trip_details($form, &$form_state) {
    global $user;
    $dis_id = change_feeds_import_get_distributor_id($user->uid);
    $trip_id = $form_state['values']['trip_id'];
    $dispatch_date = $form_state['values']['dispatch_date'];
    $cashier_name = $form_state['values']['cashier_name'];
    $bill_status = $form_state['values']['bill_status'];
    $bill_number = $form_state['values']['bill_number'];
    $bill_status_undeliver = $form_state['values']['bill_status_undeliver'];
    if (empty($trip_id)) {
        $trip_id = $form_state['values']['trips'];
    }
    if (!empty($trip_id)) {
        $sub_query1 = db_select('dispatch_planning_item_details', 'dpi');
        $sub_query1->fields('dpi', array('trip_bill_reference'));
        $sub_query1->addExpression("SUM(dpi.return_qty * dpi.sum_net_sales_value)", 'return_value');
        $sub_query1->addExpression("SUM(dpi.res_cashier_short * dpi.sum_net_sales_value)", 'cashier_short_value');
        $sub_query1->addExpression("SUM(dpi.res_godown_short * dpi.sum_net_sales_value)", 'godown_short_value');
        $sub_query1->groupBy('dpi.trip_bill_reference');
        $query = db_select('dispatch_planning_api_data', 'ad');
        $query->condition('ad.distributor_id', $dis_id, '=');
        $query->condition('ad.runsheet_number', $trip_id, '=');
        $default_bill_status = array('partial_deliver','deliver','undeliver');
        
        /*if (!empty($bill_status_undeliver)) {
           array_push($default_bill_status,"undeliver");
        }*/
        
        if (!empty($dispatch_date)) {
           $query->condition('ad.date', $dispatch_date, '=');
        }
        if (!empty($cashier_name)) {
            $query->condition('ad.fieldExecutiveName', $cashier_name);
        }
       /* if (!empty($bill_status)) {
            $query->condition('ad.res_status', $bill_status, '=');
        }*/
        //else {
         $query->condition('ad.res_status', $default_bill_status, 'IN');
       // }
        if (!empty($bill_number)) {
            //$query->condition('ad.bill_number', $bill_number, '=');
        }
        
        $query->join('node', 'n', 'n.title = ad.bill_number AND n.uid = ad.distributor_id');
        $query->condition('n.type', 'sales_register_data', '=');
       // Join with outlet master to fetch payment type
        $query->join('field_data_field_party_code_reference', 'pc', 'pc.entity_id = n.nid');
        $query->join('node', 'oun', 'oun.nid = pc.field_party_code_reference_nid');
        $query->leftJoin('field_data_field_payment_terms', 'pt', 'pt.entity_id = oun.nid');
        $query->condition('oun.uid', $dis_id);
        //---------------------------
        $query->condition('n.uid', $dis_id);
        $query->join('field_data_field_bill_value', 'bv', 'bv.entity_id = n.nid');
        $query->join($sub_query1, 'item_details', 'item_details.trip_bill_reference = ad.id');
        // Join with dispatch_data to check vrs status and return flage 
        $query->join('node', 'disp_data', 'disp_data.title = ad.runsheet_number');
        $query->condition('disp_data.type', 'dispatch_data');
        $query->condition('disp_data .uid', $dis_id);
        $query->leftjoin('field_data_field_vrs_status', 'vs', 'vs.entity_id = disp_data.nid');
        // VRS Screen Return flag = 1 and VRS status = 1, 3 Means VRS pending
        
        $query->join('field_data_field_trip_executon_date', 'exe_date', 'exe_date.entity_id = disp_data.nid');
        $query->leftjoin('field_data_field_cashier_name', 'fld_cashier', 'fld_cashier.entity_id = disp_data.nid');
        $vrs_status = array('1','3');
        $query->condition('vs.field_vrs_status_value', $vrs_status,'IN');

        $query->groupBy('ad.bill_number');
      
        $query->fields('ad', array('bill_number', 'res_bank_name', 'res_status', 'res_cheque_date', 'res_bank_name', 'res_photo_of_cheque', 'res_photo_of_signed_bill_delivery', 'photo_of_signed_bill_partial', 'res_moneyTransactionType'));
        $query->fields('item_details', array('trip_bill_reference'));
        $query->fields('bv',array('field_bill_value_value'));
        $query->fields('fld_cashier',array('field_cashier_name_value'));
        
        // payment type
        $query->fields('pt', array('field_payment_terms_value'));
        // sales register to be collected amt
        $query->addExpression("SUM(bv.field_bill_value_value - item_details.return_value - item_details.cashier_short_value - item_details.godown_short_value)", 'sales_register_to_be_collected_amt');
        // Farery to be collected amt
        $query->addExpression("SUM(ad.bill_value - item_details.return_value - item_details.cashier_short_value - item_details.godown_short_value)", 'to_be_collected_amt');
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
        $query->addExpression('bv.field_bill_value_value', 'field_bill_value_value');
        $query->addExpression("DATE_FORMAT(CONVERT_TZ(FROM_UNIXTIME(exe_date.field_trip_executon_date_value),'+00:00','+05:30'), '%d-%b-%Y')", 'trip_execution_date');
        
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
        $query->orderBy('ad.res_status','asc');
        
        //$query->addExpression('ad.res_cheque_amount', 'res_cheque_amount');
        //$query->addExpression("SUM(bv.field_bill_value_value)", 'total_bill_value');
        //$query->groupBy('bv.entity_id');
        //$query->range(0, 25);
        $res = $query->execute()->fetchAll(PDO::FETCH_UNIQUE | PDO::FETCH_ASSOC);

        $form_state['vrs_confirmation']['bills'] = $res;
        $form_state['rebuild'] = TRUE;
    }
}

function vrs_confirmation_validate($form, &$form_state) {
    global $user;
    $messages = array();
    $dis_id = change_feeds_import_get_distributor_id($user->uid);
    $trip_id = $form_state['values']['trip_id'];
    if (empty($trip_id))
        $trip_id = $form_state['values']['select_trip'];
    $key_footer = key($form_state['values']['footer_details']);
    $to_be_collected =  (float) $form_state['values']['footer_details'][$key_footer]['total_s_register'];
    $total_short = (float) $form_state['values']['footer_details'][$key_footer]['short_collected'];
    $total_actual_cash = (float) $form_state['values']['footer_details'][$key_footer]['total_actual_cash_collected'];
    $total_cheque_collected = (float) $form_state['values']['footer_details'][$key_footer]['total_cheque_collected'];
    $total_signed_bill_collected = (float) $form_state['values']['footer_details'][$key_footer]['total_signed_bill_collected'];
    $total_cashier_cash_short = (float) $form_state['values']['footer_details'][$key_footer]['total_cashier_cash_short'];
    $total_cash = $total_actual_cash + $total_cheque_collected + $total_signed_bill_collected + $total_cashier_cash_short + $total_short;
    if (!empty($form_state['values']['bill_details'])) {
        //2. At Table Level, for each bill to be collected should be = cash amount + cheque + signed bill + short
        foreach ($form_state['values']['bill_details'] as $key => $value) {
            $bill_level_to_be_collected = (float) $value['sales_register_tobe_collected'];
            $bill_level_actual_cash = (float) $value['actual_cash_collected'] + (float) $value['actual_cheque_collected'] + (float) $value['signed_bill_collected'] + (float) $value['short_collected'];
            // Difference in RS 1  Short amt will be greater than RS 1
            $diff_amt = $bill_level_to_be_collected - $bill_level_actual_cash;
            if ($diff_amt<=1 && $diff_amt>=0) {
            }
            else {
                $messages[] = t('Error:Bill Number:' . $key . '-To be collected amount should be equal to (Actual Cash Collected + Actual Cheque Amount + Actual Signed Bill + Short).');
            }
            $short_collected = (float) $value['short_collected'];
            $non_bankable = $value['non_billable'];
            if (!empty($short_collected) && empty($value['short_reason_code'])) {
                $messages[] = t('Error:Bill Number:' . $key . '- Please select the Short Reason');
            }
            if (empty($short_collected) && !empty($value['short_reason_code'])) {
                $messages[] = t('Error:Bill Number:' . $key . '- There is no short. Please remove the selection of short reason.');
            }
            if (!empty($non_bankable) && empty($value['non_bankable_reason_code'])) {
                $messages[] = t('Error:Bill Number:' . $key . '- Please select the Pending Reason Code');
            }
            if (empty($non_bankable) && !empty($value['non_bankable_reason_code'])) {
                $messages[] = t('Error:Bill Number:' . $key . '- Please remove the Pending Reason.');
            }
            // Signed bill validation
            
            $photo_signed_bill = "signedbill_$key";
            $photo_cheque = "cheque_$key";
            //photo_cheque_
            If (!empty($value['signed_bill_collected'])) {

                if (empty($form["photo_signed_bill_$key"]) && empty($_FILES['files']['name'][$photo_signed_bill])) {
                    $messages[] = t('Error:Bill Number:' . $key . '- Please upload signed bill image.');
                    
                }
            }
            // Cheque Validation
            If (!empty($value['actual_cheque_collected'])) {

                if (empty($form["photo_cheque_$key"]) && empty($_FILES['files']['name'][$photo_cheque])) {
                    $messages[] = t('Error:Bill Number:' . $key . '- Please upload cheque image.');
                    
                }
                if (empty($value["cheque_number"])) {
                    $messages[] = t('Error:Bill Number:' . $key . '- Please enter cheque number.');
                    
                }
                if (empty($value["cheque_date"])) {
                    $messages[] = t('Error:Bill Number:' . $key . '- Please enter cheque date.');
                    
                }
                if (empty($value["bank_name"])) {
                    $messages[] = t('Error:Bill Number:' . $key . '- Please enter bank name.');
                }
            }
            $bank_validate = validate_bank_names($value["bank_name"],$trip_id);
            if ($bank_validate === 1) {
                     $messages[] = t('Error:Bill Number:' . $key . '- Bank Name does not exist in Bank Master.');
                }
            
        }
    }
    // Note denomination validation
     $note_denominations = get_notes_denominations();
        $denominations = array();
        if (!empty($note_denominations)) {
            foreach ($note_denominations as $key => $value) {
                $no_notes = $form_state['values']['note_denominations_' . $value['nid']];
                $coins = (float) $form_state['values']['coins'];
                if (!empty($no_notes) && is_numeric($no_notes)) {
                    $denominations[$value['nid']] = array('trip_id' => $trip_id, 'vrs_total_id' => $vrs_total_id,
                        'denominations_nid' => $value['nid'], 'no_of_notes' => $no_notes
                        , 'distributor_id' => $dis_id, 'coins' => $coins);
                }
            }
            if (empty($denominations)) {
              $messages[] = t('Error:Please enter the cash denomination');
            }
        }
    
   //1. At Footer Level, Total To be collected should be = Footer level total cash + total cheque + total signed bill + cashier short + short
    if ($total_cash !== $to_be_collected) {
        $messages[] = t('Error:To be collected amount should be equal to total cash + total cheque + total signed bill + cashier short + short.');
    }
    $bill_numbers = array_keys($form_state['values']['bill_details']);
    $bill_status = get_bill_details($bill_numbers, $trip_id);
    if (!empty($messages)) {
        foreach ($messages as $key => $error) {
            $mess .= $error . '<br><br>';
        }
        form_set_error('file', $mess);
        $form_state['rebuild'] = TRUE;
    }
}

/**
 * 
 * @param array $form
 * @param array $form_state
 * @return array
 */
function vrs_confirmation_form_submit($form, &$form_state) {
    global $user;
    //print_r($form_state);die;
    $vrs_total_id = $form_state['values']['vrs_total_id'];
    $vrs_created_date = $form_state['values']['vrs_created_date'];
    $dis_id = change_feeds_import_get_distributor_id($user->uid);
    $trip_id = $form_state['values']['trip_id'];
    if (empty($trip_id)) {
        $trip_id = $form_state['values']['select_trip'];
    }
    $key_footer = key($form_state['values']['footer_details']);
    $to_be_collected =  $form_state['values']['footer_details'][$key_footer]['total_s_register'];
    $total_short = (float) $form_state['values']['footer_details'][$key_footer]['short_collected'];
    $total_actual_cash =  $form_state['values']['footer_details'][$key_footer]['total_actual_cash_collected'];
    $total_cheque_collected =  $form_state['values']['footer_details'][$key_footer]['total_cheque_collected'];
    $total_signed_bill_collected = (float) $form_state['values']['footer_details'][$key_footer]['total_signed_bill_collected'];
    $total_cashier_cash_short = (float) $form_state['values']['footer_details'][$key_footer]['total_cashier_cash_short'];
    $total_cash = $total_actual_cash + $total_cheque_collected + $total_signed_bill_collected + $total_cashier_cash_short + $total_short;
    $asset_short_total = (float) $form_state['values']['asset_total_value'];
    //header level hidden field value         
    $cashier_short_value =  $form_state['values']['cashier_short_value'];
    $footer_level_cashier_debit_value = ($to_be_collected - ($total_short + $total_actual_cash + $total_cheque_collected + $total_signed_bill_collected)) + $cashier_short_value + $asset_short_total ;
    $to_be_collected_farey = (float) $form_state['values']['footer_details'][$key_footer]['total_farey'];
    $check_count = 0;
    $sb_count = 0;
    $bill_numbers = array_keys($form_state['values']['bill_details']);
    $bill_status = get_bill_details($bill_numbers, $trip_id);
    if (!empty($form_state['values']['bill_details'])) {
        //2. At Table Level, for each bill to be collected should be = cash amount + cheque + signed bill + short
        foreach ($form_state['values']['bill_details'] as $key => $value) {
            $bill_level_to_be_collected = $value['sales_register_tobe_collected'];
            $bill_level_actual_cash = (float) $value['actual_cash_collected'] + (float) $value['actual_cheque_collected'] + (float) $value['signed_bill_collected'] + (float) $value['short_collected'];
            $check_collected = (float) $value['actual_cheque_collected'];
            $signed_bill_collected = (float) $value['signed_bill_collected'];
            if (!empty($check_collected)) {
                $check_count++;
            }
            if (!empty($signed_bill_collected)) {
                $sb_count++;
            }
        }
    }
    
    $trip_items = array();
    $dir = 'public://bills';
    file_prepare_directory($dir, FILE_CREATE_DIRECTORY);
    $vrs_version = get_current_vrs_version($trip_id, $dis_id);
    if (empty($vrs_version)) {
        $version_number = 'V1.0';
    } else {
        $v_number = substr($vrs_version, 1);
        $v_number = (float) $v_number + 0.1;
        $version_number = 'V' . $v_number;
    }
    if (!empty($form_state['values']['bill_details'])) {
        $vrs_id = "VRS$trip_id";
        foreach ($form_state['values']['bill_details'] as $key => $value) {
            
            $photo_cheque = "cheque_$key";
            $photo_signed_bill = "signedbill_$key";
            if (isset($_FILES['files']['name'][$photo_cheque])) {
                $fname = explode('.', $_FILES['files']['name'][$photo_cheque]);
                $file_rename = $photo_cheque . '.' . $fname[1];
                $file = file_save_upload($photo_cheque, array('file_validate_extensions' => array('png gif jpg jpeg pdf')), "public://", FILE_EXISTS_REPLACE);
                if ($file && !empty($fname[1])) {
                    if ($file = file_move($file, $dir.'/' . $file_rename, FILE_EXISTS_REPLACE)) {
                        $file->status = FILE_STATUS_PERMANENT;
                        file_save($file);
                        $trip_items[$key]['photo_cheque'] = $file->uri;
                    } else {
                        form_set_error('file', t('Failed to write the uploaded file the site\'s file folder.'));
                    }
                }
            }
            if (isset($_FILES['files']['name'][$photo_signed_bill])) {
                $fname = explode('.', $_FILES['files']['name'][$photo_signed_bill]);
                $file_rename = $photo_signed_bill . '.' . $fname[1];
                $file = file_save_upload($photo_signed_bill, array('file_validate_extensions' => array('png gif jpg jpeg pdf')), "public://", FILE_EXISTS_REPLACE);
                if ($file && !empty($fname[1])) {
                    if ($file = file_move($file, $dir.'/' . $file_rename, FILE_EXISTS_REPLACE)) {
                        $file->status = FILE_STATUS_PERMANENT;
                        file_save($file);
                        $trip_items[$key]['photo_signed_bill'] = $file->uri;
                    } else {
                        form_set_error('file', t('Failed to write the uploaded file the site\'s file folder.'));
                    }
                }
            }
            unset($photo_signed_bill);
            unset($photo_cheque);

            $trip_items[$key]['trip_id'] = $trip_id;
            $trip_items[$key]['vrs_id'] = $vrs_id;
            $trip_items[$key]['bill_number'] = $key;
            $trip_items[$key]['bill_status'] = $bill_status[$key]['res_status'];
            $trip_items[$key]['to_be_collected_amt_sales_register'] = (float) $value['sales_register_tobe_collected'];
            $trip_items[$key]['to_be_collected_amt'] = (float) $value['farey_bill_value'];
            $trip_items[$key]['actual_cash_amount'] = (float) $value['actual_cash_collected'];
            $trip_items[$key]['actual_cheque_amount'] = (float) $value['actual_cheque_collected'];
            $trip_items[$key]['actual_signed_bill_amount'] = (float) $value['signed_bill_collected'];
            $trip_items[$key]['cheque_number'] = $value['cheque_number'];
            $trip_items[$key]['bank_name'] = $value['bank_name'];
            if (!empty($value['cheque_date'])) {
                $trip_items[$key]['cheque_date'] = (int) strtotime($value['cheque_date']);
            } else {
                $trip_items[$key]['cheque_date'] = 0;
            }
            $trip_items[$key]['short_amt'] = (float) $value['short_collected'];
            $trip_items[$key]['reason_code'] = $value['short_reason_code'];
            $trip_items[$key]['vrs_version_no'] = $version_number;
            if (!empty($vrs_total_id) && !empty($vrs_created_date)) {
                $trip_items[$key]['vrs_created_date'] = $vrs_created_date;
            } else {
                $trip_items[$key]['vrs_created_date'] = strtotime(date('Y-m-d'));
            }
            $trip_items[$key]['vrs_confirm_date'] = strtotime(date('Y-m-d'));
            
            $trip_items[$key]['distributor_id'] = $dis_id;
            $trip_items[$key]['remarks'] = $value['remarks'];
            $trip_items[$key]['non_billable'] = $value['non_billable'] ? 1 : 0;
            $trip_items[$key]['non_bank_reason_code'] = $value['non_bankable_reason_code'] ;
            if (!isset($no_billable) && $value['non_billable']==1) {
                $no_billable = 1;
            }
        } 
        if ($no_billable == 1) {
            $vrs_status = 3;
        } else {
            $vrs_status = 2;
        }
        //  // Update vrs version in trip level
        $node = update_vrs_version($trip_id, $version_number, $dis_id,$vrs_status);
        if (!empty($node))
         insert_vrs_bill_items($trip_items, $trip_id, $version_number, $dis_id, $vrs_total_id);
    }
    // Saving trip total vrs total   
    $trip_items_total = array();
    if (!empty($form_state['values']['footer_details']) && !empty($node)) {
        $bill_count = count($form_state['values']['bill_details']);
        $key = 0;
        $trip_items_total['trip_id'] = $trip_id;
        $trip_items_total['vrs_id'] = $vrs_id;
        $trip_items_total['vrs_version_no'] = $version_number;
        $trip_items_total['bill_count'] = $bill_count;
        $trip_items_total['total_to_be_collected_sales_register'] = $to_be_collected;
        $trip_items_total['total_to_be_collected_amt'] = $to_be_collected_farey;
        $trip_items_total['total_actual_cash_amount'] = $total_actual_cash;
        $trip_items_total['total_actual_cheque_amount'] = $total_cheque_collected;
        $trip_items_total['total_actual_signed_bill_amount'] = $total_signed_bill_collected;
        $trip_items_total['total_cashier_cash_short'] = $total_cashier_cash_short;
        $trip_items_total['total_short_amt'] = $total_short;
        $trip_items_total['total_cashier_debit_value'] = $footer_level_cashier_debit_value;
        $trip_items_total['cheque_count'] = $check_count;
        $trip_items_total['sb_count'] = $sb_count;
        if (!empty($vrs_total_id) && !empty($vrs_created_date)) {
            $trip_items[$key]['vrs_created_date'] = $vrs_created_date;
        } else {
            $trip_items[$key]['vrs_created_date'] = strtotime(date('Y-m-d'));
        }
        $trip_items_total['vrs_confirm_date'] = strtotime(date('Y-m-d'));
        $trip_items_total['distributor_id'] = $dis_id;
        $vrs_total_id_insert = insert_vrs_trip_total($trip_items_total, $trip_id, $version_number, $vrs_total_id);
        if (!empty($vrs_total_id))
            $vrs_total_id = $vrs_total_id;
        else
            $vrs_total_id = $vrs_total_id_insert;

       // Saving Note denominations
        $note_denominations = get_notes_denominations();
        $denominations = array();
        if (!empty($note_denominations)) {
            foreach ($note_denominations as $key => $value) {
                $no_notes = $form_state['values']['note_denominations_' . $value['nid']];
                $coins = (float) $form_state['values']['coins'];
                if (!empty($no_notes) && is_numeric($no_notes)) {
                    $denominations[$value['nid']] = array('trip_id' => $trip_id, 'vrs_total_id' => $vrs_total_id,
                        'denominations_nid' => $value['nid'], 'no_of_notes' => $no_notes
                        , 'distributor_id' => $dis_id);
                }
            }
            if (!empty($denominations)) {
                reset($denominations);
                $first_key = key($denominations);
                $denominations[$first_key]['coins'] = $coins;
                insert_denominations($denominations, $trip_id, $vrs_total_id);
            }
        }
    }

   
    if ($node)
        drupal_set_message(t('Successfully created VRS-Version:' . $version_number));
    else
        drupal_set_message(t('Failed to create VRS-Version:' . $version_number),'error');
}





function insert_vrs_trip_total($values, $trip_id, $version_number, $vrs_total_id = 0) {
    $vrs_trip_total_field = array('trip_id',
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
        'vrs_version_no',
        'vrs_created_date',
        'vrs_confirm_date',
        'distributor_id',
    );
    if (!empty($vrs_total_id)) {
        foreach ($values as $record) {
            $res = db_update('vrs_trip_total') // Table name no longer needs {}
                    ->fields($values)
                    ->condition('trip_id', $trip_id)
                    ->condition('id', $vrs_total_id)
                    ->execute();
        }
    } else {
        $res = db_insert('vrs_trip_total')->fields($values)->execute();
    }
    // Always insert into vrs_trip_total_history table
    $res1 = db_insert('vrs_trip_total_history')->fields($values)->execute();
    return $res;
}

function insert_vrs_bill_items($values, $trip_id, $version_number, $distributor_id, $vrs_total_id = 0) {
    $vrs_trip_field = array('trip_id',
        'vrs_id',
        'bill_number',
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
        'vrs_version_no',
        'vrs_created_date',
        'vrs_confirm_date',
        'distributor_id',
        'remarks',
        'non_billable',
        'photo_cheque',
        'photo_signed_bill',
        'non_bank_reason_code'
    );

    // Update current version        
    if (!empty($vrs_total_id)) {

        foreach ($values as $record) {
            db_merge('vrs_trip_items') // Table name no longer needs {}
                    ->fields($record)
                    ->condition('trip_id', $record['trip_id'])
                    ->condition('bill_number', $record['bill_number'])
                    ->execute();
        }
    } else {

        $query = db_insert('vrs_trip_items')->fields($vrs_trip_field);
        foreach ($values as $record) {
            $query->values($record);
        }
        $query->execute();
    }
    // Insert into history table when click on confirm vrs
    $query = db_insert('vrs_trip_items_history')->fields($vrs_trip_field);
    foreach ($values as $record) {
        $query->values($record);
    }
    $query->execute();
      
}

function insert_denominations($values, $trip_id, $vrs_total_id = 0) {
    if (!empty($values)) {

        $del = db_delete('vrs_trip_denominations');
        $del->condition('trip_id', $trip_id)->execute();
        $denomination_field = array('trip_id',
            'vrs_total_id',
            'denominations_nid',
            'no_of_notes',
            'coins',
            'distributor_id',
        );

        $query = db_insert('vrs_trip_denominations')->fields($denomination_field);
        foreach ($values as $record) {
            $query->values($record);
        }
        $query->execute();
    }
}



function update_vrs_version($trip_id, $version_number, $distributor_id,$vrs_status) {
    if (!empty($trip_id) && !empty($version_number) && !empty($distributor_id) && !empty($vrs_status)) {
        $node_obj = node_load_multiple(NULL, array('title' =>$trip_id, 'type' => 'dispatch_data', 'uid' => $distributor_id));
        $key = key($node_obj);
        $node = $node_obj[$key];
        if (!empty($node)) {
            // 2 1 menans vrs confirmed.
            $node->field_vrs_closure_id[LANGUAGE_NONE][0]['value'] = 'VRS' . $trip_id;
            $node->field_vrs_version[LANGUAGE_NONE][0]['value'] = $version_number;
            $node->field_vrs_status[LANGUAGE_NONE][0]['value'] = $vrs_status;
            node_save($node);
            return $node;
        }
    }
}

function _banks_autocomplete($trip_id,$string) {
 $bank_names = array();
 if (!empty($trip_id)) {
        global $user;
        $dis_id = change_feeds_import_get_distributor_id($user->uid);
        $query = db_select('dispatch_planning_api_data', 'ad');
        $query->condition('ad.distributor_id', $dis_id, '=');
        $query->condition('ad.runsheet_number', $trip_id, '=');
        $query->leftJoin('vrs_trip_items', 'vt', 'vt.trip_id = ad.runsheet_number');
        $db_or = db_or();
        $db_or->condition('ad.res_bank_name_edit', '%' . db_like($string).'%' ,'LIKE');
        $db_or->condition('ad.res_bank_name', '%' . db_like($string).'%' ,'LIKE');
        $db_or->condition('vt.bank_name', '%' . db_like($string).'%' ,'LIKE');
        $query->condition($db_or);
        $query->fields('ad', array('res_bank_name_edit','res_bank_name'));
        $query->fields('vt', array('bank_name'));
        $query->groupBy('vt.bank_name');
        $query->groupBy('ad.res_bank_name_edit');
        $query->groupBy('ad.res_bank_name');
        $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
        if(!empty($res)) {
               foreach($res as $key => $value) {
                   if (!empty( $value['res_bank_name_edit'])) {
                       $bank_names[$value['res_bank_name_edit']] = $value['res_bank_name_edit'];
                   }
                   if (!empty($value['res_bank_name'])) {
                       $bank_names[$value['res_bank_name']] = $value['res_bank_name'];
                   }
                   if (!empty($value['bank_name'])) {
                       $bank_names[$value['bank_name']] = $value['bank_name'];
                   }
               }
            }
        
    }
     print json_encode($bank_names);
     exit();
}
/*function _payment_outcome_autocomplete($b1,$trip_id,$string) {
    $payment_out_come = array();
    if (!empty($trip_id)) {
        //$string = ucfirst($str);
        $payment_out_come = array('Cash' => 'Cash', 'Cheque' => 'Cheque', 'Signed Bill' => 'Signed Bill');
        $input = preg_quote($string, '~');
        $result = preg_grep('~' . ucfirst($input) . '~', $payment_out_come);
    }
    print json_encode($result);
    exit();
}*/

function validate_bank_names($bank_name,$trip_id) {
    global $user;
    $dis_id = change_feeds_import_get_distributor_id($user->uid);   
    if (!empty($bank_name)) {
       $query = db_select('node', 'n');
       $query->condition('n.type', 'bank_master');
       $query->condition('n.title',$bank_name);
       $query->condition('n.status', 1);
       $query->fields('n', array('title'));
       $result = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
       // Checking it in trip level
       if(empty($result)) {
            $query = db_select('dispatch_planning_api_data', 'ad');
            $query->condition('ad.distributor_id', $dis_id, '=');
            $query->condition('ad.runsheet_number',$trip_id);
            $db_or = db_or();
            $db_or->condition('ad.res_bank_name',$bank_name);
            $db_or->condition('ad.res_bank_name_edit',$bank_name);
            $query->condition($db_or);
            $query->fields('ad', array('res_bank_name_edit','res_bank_name'));
            $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
            if(empty($res)) {
                 return 1; 
           }
    }
     
}
    return 0;
}


function _get_cashier_name_using_trip_id($trip_id) {
   $res = array();
   if (!empty($trip_id)) {
      global $user;
      $dis_id = change_feeds_import_get_distributor_id($user->uid);
      $query = db_select('node', 'n');
      $query->condition('n.type', 'dispatch_data');
      $query->condition('n.uid', $dis_id);
      $query->leftjoin('field_data_field_cashier_name', 'cn', 'cn.entity_id = n.nid');
      $query->fields('cn', array('field_cashier_name_value'));
      $query->condition('n.title', $trip_id);
      $res = $query->execute()->fetchAllKeyed(0, 0);
   }
   return $res;
 
}

