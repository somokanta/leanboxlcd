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

function upload_signed_bill_cheque_form($form, &$form_state) {
  if (!empty($form_state['values']['trip_id']) || !empty($form_state['values']['trips'])) {
    $trip_id = $form_state['values']['trip_id'];
    if (empty($trip_id))
      $trip_id = $form_state['values']['trips'];
  }
  if (!isset($form_state['vrs_confirmation']['bills'])) {
    $form_state['vrs_confirmation']['bills'] = array();
  }
  $options = $form_state['vrs_confirmation']['bills'];
  // variable need  
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);
  //$dis_id = get_distributorid_of_trip($trip_id);
  //$form['#attached']['js'][] = drupal_get_path('module', 'mrc_inventory_returns') . '/js/vrs_confirmation.js';
  $form['#attached']['js'][] = drupal_get_path('module', 'mrc_inventory_returns') . '/js/upload_signed_bill_cheque.js';
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
      'callback' => 'upload_signed_bill_cheque_trip_id_callback',//vrs_confirmation_trip_id_callback
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
      'callback' => 'upload_signed_bill_cheque_select_trip_id_callback',//vrs_confirmation_select_trip_id_callback
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
  // get tripids using dispatch date
  if (!empty($form_state['values']['dispatch_date']) && is_string($form_state['values']['dispatch_date'])) {
    $form['trips']['#options'] = array('' => 'Select') + _get_trip_ids_using_dispatch_date($form_state['values']['dispatch_date']);
  }
  //get cashier name using tripid
  if (!empty($form_state['values']['trips'])) {
    $form['cashier_name']['#options'] = _get_cashier_name_using_trip_id($form_state['values']['trips']);
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Show',
    '#name' => 'show',
    '#ajax' => array(
      'callback' => 'upload_signed_bill_cheque_callback',//vrs_confirmation_callback
      'wrapper' => 'itemlist-div',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    '#attributes' => array('class' => array('vrs-trip-submit'),'id' => 'show-button'),
    '#submit' => array('display_upload_signed_bill_cheque_trip_details'),//display_vrs_trip_details
    '#validate' => array('validate_upload_signed_bill_cheque_trip_details'),//validate_vrs_trip_details
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
    '#prefix' => '<div class="bill_status_delivered bill_status" id="bill_status_undeliver">',
    '#suffix' => '</div></div>'
  );
  $form['bill_number'] = array(
    '#type' => 'textfield',
    '#attributes' => array('class' => array('bill_number')),
    '#id' => 'bill_number',
    '#title' => 'Bill Number',
    '#prefix' => '<div class="col_2 col_2_2"> ',
    '#suffix' => '</div>'
  );
  $payment_out_come = array('Cash' => 'Cash', 'Cheque' => 'Cheque', 'Signed' => 'Signed','Cheque,Cash' => 'Cheque,Cash');
  $form['payment_outcome'] = array(
    '#type' => 'select',
    '#attributes' => array('class' => array('payment_outcome', 'auto_complete'), 'style' => "height:32px"),
    '#id' => 'payment_outcome',
    '#title' => 'Payment Outcome',
    '#default_value' => $payment_out_come['Signed'],
    '#options' => array('' => 'Select') + $payment_out_come,
    '#prefix' => '<div class="col_2 col_2_2"> ',
    '#suffix' => '</div>'
  );
  
  if (!empty($options)) {
      $bills = get_signed_bill_cheque_images($trip_id, $dis_id);
  }

  $form['bank_name'] = array(
    '#type' => 'textfield',
    '#title' => 'Bank Name',
    '#id' => 'bank_name',
    '#autocomplete_path' => 'bank_names/autocomplete/' . $trip_id,
    '#attributes' => array('class' => array('bank_name_filter'), 'style' => "height:32px"),
    '#prefix' => '<div class="col_2 col_2_2"> ',
    '#suffix' => '</div></div>'
  );
  $header = array(
    'Bill Number',
    'Delivery Outcome',
    'Payment Terms',
    'Payment Outcome',
    'Signed Bill Image & Upload',
    'Cheque Image & Upload',
  );
if(!empty($options)){
  $form['bill_details'] = array(
    '#theme' => 'table',
    '#prefix' => '<div class="custom_middle_table vrs_middle_table">',
    '#tree' => TRUE,
    '#header' => $header,
    '#rows' => array(),
    '#attributes' => array(
      'class' => array('bill_details'),
      'id' => 'bill_details_one'
    ),
    '#suffix' => '</div>',
    // '#sticky' => FALSE,
  );
foreach($options as $key => &$val){
  $key = str_replace(' ', '', $key);
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
  if (empty($bills[$key]['payment_term'])) {
    if (($bills[$key]['actual_signed_bill_amount'] > 0) || ($val['res_confirm_signed_bill_amount'] > 0)) {
      $payment_term[] = t('Signed');
    }
    if (($bills[$key]['actual_cheque_amount'] > 0) || ($val['res_cheque_amount'] > 0)) {
      $payment_term[] = t('Cheque');
    }
    if (($bills[$key]['actual_cash_amount'] > 0) || ($val['res_cash_amount'] > 0)) {
      $payment_term[] = t('Cash');
    }
    if (!empty($payment_term)) {
      $payments_terms = implode(',', $payment_term);
    }
  }
  else {
      $payment_term = $bills[$key]['payment_term'];
      $payments_terms = implode(',', $payment_term);
  }
  if ($undeliver == 1) {
    $payments_terms = 'NA';
  }
  $form['bill_details']['#rows'][$key][] = $bill_status;
  $form['bill_details']['#rows'][$key][] = $val['field_payment_terms_value'];
  $form['bill_details']['#rows'][$key][] = $payments_terms;
 $ph_cheque = '<div class ="vrs_signed_bill_image">';
      $ph_flag = 0;
      $span_class = '<span style = "margin-left:18px;"></span>';
      if (!empty($bills[$key]['photo_cheque'])) {
        $files = file_load_multiple(array(), array('uri' => $bills[$key]['photo_cheque']));
        $file = reset($files);   
        $photo_cheque_fid = $file->fid;
       
        $url = $bills[$key]['photo_cheque'];
        $path = $url;
        $na_position = strpos($url, '/NA');
        if (empty($na_position)) {
          $wrapper = file_stream_wrapper_get_instance_by_uri($url);
          if (!empty($wrapper)) {
            $path = $wrapper->getExternalUrl();
          }
          $ph_cheque .= '<a target="_blank" href="' . $path . '">';
          $ph_cheque .= t('File');
          $ph_cheque .= '</a>';
          $ph_flag = 1;
        }
        else {
          $ph_flag = 0;
        }
      }
      else if (!empty($val['res_photo_of_cheque'])) {
        $url = $val['res_photo_of_cheque'];
        $na_position = strpos($url, '/NA');
        $path = $url;
        if (empty($na_position)) {
          $wrapper = file_stream_wrapper_get_instance_by_uri($url);
          if (!empty($wrapper)) {
            $path = $wrapper->getExternalUrl();
          }
          $ph_cheque .= '<a target="_blank" href="' . $path . '">';
          $ph_cheque .= t('File');
          $ph_cheque .= '</a>';
          $ph_flag = 1;
        }
        else {
          $ph_flag = 0;
        }
      }
      if ($ph_flag == 0) {
        $ph_cheque .= $span_class;
      }
      if ($ph_flag == 1) {
        $form['photo_cheque_' . $key] = array(
          '#type' => 'hidden',
          '#value' => $path,
        );
      }

      $ph_cheque .= '</div>';
      $photo_cheque = array(
        '#type' => 'managed_file',
        '#attributes' => array('title' => t('Cheque Image')),
        '#default_value'=>$photo_cheque_fid,  
        '#name' => 'files[cheque_' . $key . ']',
        // '#id' => 'bill_details_' . $key . '_photo_cheque',
        '#attributes' => array('id' => 'ph_cheque'),
        '#upload_location' => 'public://bills/',    
        '#prefix' => '<div class="texdt">' . $ph_cheque,
        '#suffix' => '</div>',
      );
      unset($path);
      unset($url);
      unset($wrapper);
 unset($file);

      $ph_sign_flag = 0;
      $ph_signed_bill = '<div class ="vrs_signed_bill_image">';
      if (!empty($bills[$key]['photo_signed_bill'])) {
        $files = file_load_multiple(array(), array('uri' => $bills[$key]['photo_signed_bill']));
        $file = reset($files);   
        $photo_signed_fid = $file->fid;  
        $url = $bills[$key]['photo_signed_bill'];
        $path = $url;
        $na_position = strpos($url, '/NA');
        $wrapper = file_stream_wrapper_get_instance_by_uri($url);
        if (empty($na_position)) {
          if (!empty($wrapper)) {
            $path = $wrapper->getExternalUrl();
          }
          $ph_signed_bill .= '<a target="_blank" href="' . $path . '">';
          $ph_signed_bill .= t('File');
          $ph_signed_bill .= '</a>';
          $ph_sign_flag = 1;
        }
        else {
          $ph_sign_flag = 0;
        }
      }
      elseif (!empty($val['res_photo_of_signed_bill_delivery']) || !empty($val['photo_of_signed_bill_partial'])) {
        $url = $val['photo_of_signed_bill_partial'];
        if (empty($url)) {
          $url = $val['res_photo_of_signed_bill_delivery'];
        }
        $na_position = strpos($url, '/NA');
        $path = $url;
        if (empty($na_position)) {
          $wrapper = file_stream_wrapper_get_instance_by_uri($url);
          if (!empty($wrapper)) {
            $path = $wrapper->getExternalUrl();
          }
          $ph_signed_bill .= '<a target="_blank" href="' . $path . '">';
          $ph_signed_bill .= t('File');
          $ph_signed_bill .= '</a>';
          $ph_sign_flag = 1;
        }
        else {
          $ph_sign_flag = 0;
        }
      }
      if ($ph_sign_flag == 0) {
        $ph_signed_bill .= $span_class;
      }
      if ($ph_sign_flag == 1) {
        $form['photo_signed_bill_' . $key] = array(
          '#type' => 'hidden',
          '#value' => $path,
        );
      }
      $ph_signed_bill .= '</div>';
      $photo_signed_bill = array(
        //'#id' => 'bill_details-' . $key . '-photo_signed_bill',
        '#name' => 'files[signedbill_' . $key . ']',
        '#type' => 'managed_file',
        '#upload_location' => 'public://bills/',  
        '#prefix' => '<div>' . $ph_signed_bill,
          '#default_value'=>$photo_signed_fid,    
        '#suffix' => '</div>',
        '#attributes' => array('class' => array('photo_signed_bill'), 'id' => 'ph_sign', 'title' => t('Signed Bill Image')),
      );
          
     // if else removed 
      $form['bill_details'][$key]["photo_cheque"] = &$photo_cheque;
      $form['bill_details'][$key]["photo_signed_bill"] = &$photo_signed_bill;
      $form['bill_details']['#rows'][$key][]['data']["photo_cheque"] = &$photo_cheque;
      $form['bill_details']['#rows'][$key][]['data']["photo_signed_bill"] = &$photo_signed_bill;
      
           unset($photo_cheque_fid);
      unset($photo_signed_fid);
      unset($photo_cheque);
      unset($photo_signed_bill);

      unset($path);
      unset($ph_cheque);
      unset($url);
      unset($payment_term);
      unset($payments_terms);
      
    }
    
    if (!empty($bank_names)) {
      $form['bank_name']['#options'] = array('' => 'All') + $bank_names;
      $b_names = $bank_names;
    }
    $form['confirm_vrs'] = array(
      '#type' => 'submit',
      '#value' => 'Submit',
      '#pefix' => '<div class ="confirm_vrs">',
      '#suffix' => '</div">',
      '#name' => 'confirm_vrs',
      '#attributes' => array('class' => array('vrs-trip-submit'),
        //'onclick' => 'if(!confirm("Do you want to Confirm VRS version :' . 2 . ' of Trip ID: ' . 5 . '?")){return false;}',
        'style' => array('float: left;')),
      '#submit' => array('upload_signed_bill_cheque_form_submit'),//vrs_confirmation_form_submit
      '#validate' => array('upload_signed_bill_cheque_validate'),//vrs_confirmation_validate
    );
  }  
  return $form;
}

function upload_signed_bill_cheque_trip_id_callback($form, &$form_state) {
  return $form['trips'];
}
function upload_signed_bill_cheque_select_trip_id_callback($form, &$form_state) {
  return $form['cashier_name'];
}
function upload_signed_bill_cheque_callback($form, &$form_state) {
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

function validate_upload_signed_bill_cheque_trip_details($form, &$form_state){
  $trip_id = $form_state['values']['trip_id'];
  if (empty($trip_id)) {
    $trip_id = $form_state['values']['trips'];
  }
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);
  $error = '';
  if (!empty($trip_id)) {
    $check_returns_flag = check_trip_id_exists_or_not($dis_id, $trip_id);
    if (empty($check_returns_flag)) {
       form_set_error('trip_id', "Trip ID :$trip_id does not exists.");
      ajax_redirect('upload-signed-bill-and-cheque');
    }
    $current_vrs_status = $check_returns_flag->field_vrs_status_value;
    $field_vrs_status = field_info_field('field_vrs_status');
    $vrs_status = list_allowed_values($field_vrs_status);
    unset($vrs_status[0]); // vrs confirmation pending
    unset($vrs_status[1]); // return confirmed
    unset($vrs_status[2]); //Vrs screen entry return conrimed and vrs confirmation pending
    unset($vrs_status[3]);
    unset($vrs_status[4]);
    unset($vrs_status[5]);
    unset($vrs_status[6]);
    if (isset($vrs_status[$current_vrs_status])) {
      form_set_error('trip_id', "Trip ID :$trip_id is in $vrs_status[$current_vrs_status] state.");
      ajax_redirect('upload-signed-bill-and-cheque');
    }
    else if (($check_returns_flag->field_vrs_status_value == -1)) {
      unset($form_state['vrs_confirmation']['bills']);
      form_set_error('trip_id', "Trip ID:$trip_id-Please confirm Delivery Confirmation.");
      ajax_redirect('upload-signed-bill-and-cheque');
    }
  }
  else {
    form_set_error('trip_id', '');
    form_set_error('trips', 'Please select or enter Trip ID.');
    ajax_redirect('upload-signed-bill-and-cheque');
  }
}
function display_upload_signed_bill_cheque_trip_details($form, &$form_state) {
  global $user;
  $res = array();
  $trip_id = $form_state['values']['trip_id']; //trip id 
  $dispatch_date = $form_state['values']['dispatch_date']; //dispatch date filter
  $cashier_name = $form_state['values']['cashier_name']; //cashier name
  //$bill_status = $form_state['values']['bill_status']; //Deliverty outcome not used in query
  $bill_number = $form_state['values']['bill_number']; //bill number
 // $bill_status_undeliver = $form_state['values']['bill_status_undeliver']; //checkbox not used in query
  $payment_outcome = $form_state['values']['payment_outcome']; //missed in vrs confirmation
  if (empty($trip_id)) {
    $trip_id = $form_state['values']['trips'];
  }
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);
  if (!empty($trip_id)) {
    $query = db_select('dispatch_planning_api_data', 'ad');
    $query->condition('ad.runsheet_number', $trip_id);
    if (!empty($cashier_name)) {
      $query->condition('ad.fieldExecutiveName', $cashier_name);
    }
    if (!empty($dispatch_date)) {
      $query->condition('ad.date', $dispatch_date, '=');
    }
    $default_bill_status = array('partial_deliver', 'deliver');//delivery outcome filter based
    $query->condition('ad.res_status', $default_bill_status, 'IN');
    if(!empty($bill_number)){
      $query->condition('ad.bill_number', $bill_number);
    }
    if(!empty($payment_outcome)){
      //$query->condition('ad.res_money_collect2', $payment_outcome);
    }
    // Join with outlet master to fetch payment type
      $query->join('node', 'n', 'n.title = ad.bill_number AND n.uid = ad.distributor_id');
      $query->condition('n.type', 'sales_register_data', '=');
      $query->join('field_data_field_party_code_reference', 'pc', 'pc.entity_id = n.nid');
      $query->join('node', 'oun', 'oun.nid = pc.field_party_code_reference_nid');
      $query->leftJoin('field_data_field_payment_terms', 'pt', 'pt.entity_id = oun.nid');
      $query->condition('oun.uid', $dis_id);//outletnid
      $query->condition('n.uid', $dis_id);// salesregisternid
      $query->join('node', 'disp_data', 'disp_data.title = ad.runsheet_number');
      $query->condition('disp_data.type', 'dispatch_data');
      $query->condition('disp_data .uid', $dis_id);
      $query->leftjoin('field_data_field_vrs_status', 'vs', 'vs.entity_id = disp_data.nid');
      // VRS Screen Return flag = 1 and VRS status = 1, 3 Means VRS pending
      $vrs_status = array('1', '5','0','2','3','4','6');
      $query->condition('vs.field_vrs_status_value', '-1', '!=');
      $query->groupBy('ad.bill_number,ad.distributor_id');
      $query->fields('ad', array('bill_number','res_status', 'bill_value'));
      $query->addExpression('ad.bill_number', 'bill_number');
      $query->addExpression('ad.res_photo_of_cheque', 'res_photo_of_cheque');
      $query->addExpression('ad.res_photo_of_signed_bill_delivery', 'res_photo_of_signed_bill_delivery');
      $query->addExpression('ad.photo_of_signed_bill_partial', 'photo_of_signed_bill_partial');
      $query->addExpression('ad.res_confirm_signed_bill_amount', 'res_confirm_signed_bill_amount');
      $query->addExpression('ad.res_cheque_amount', 'res_cheque_amount');
      $query->addExpression('ad.res_cash_amount', 'res_cash_amount');
      $query->addExpression('ad.res_money_collect2', 'payment_outcome');//payment outcome value
      // payment type
      $query->fields('pt', array('field_payment_terms_value'));
      $query->leftJoin('vrs_trip_items','vrs','vrs.trip_id = ad.runsheet_number AND vrs.bill_number = ad.bill_number AND vrs.distributor_id = ad.distributor_id');
      $query->fields('vrs', array('photo_cheque','photo_signed_bill'));
      $query->orderBy('ad.res_status', 'asc');
      $res = $query->execute()->fetchAll(PDO::FETCH_UNIQUE | PDO::FETCH_ASSOC);
  }
      $form_state['vrs_confirmation']['bills'] = $res;
      $form_state['start_time'] = REQUEST_TIME;
      $form_state['rebuild'] = TRUE;
}

function upload_signed_bill_cheque_validate($form, &$form_state){

}
function upload_signed_bill_cheque_form_submit($form, &$form_state){
  $vrs_start_time = $form_state['start_time'];
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);
  $trip_id = $form_state['values']['trip_id'];
  if (empty($trip_id)) {
    $trip_id = $form_state['values']['trips'];
  }
  //$dis_id = get_distributorid_of_trip($trip_id);
  $trip_items = array();
  $dir = 'public://bills';
  file_prepare_directory($dir, FILE_CREATE_DIRECTORY);
  //print_r($form_state['values']['bill_details']);die;
  $bill_numbers = array_keys($form_state['values']['bill_details']);
  if (!empty($form_state['values']['bill_details'])) {
    foreach ($form_state['values']['bill_details'] as $key => $value) {
       $trip_items[$key]['photo_cheque'] = $trip_items[$key]['photo_signed_bill'] = '';
      if (!empty($value['photo_cheque']))   {
         $photo_cheque_file = file_load($value['photo_cheque']);
         $photo_cheque_file->status = FILE_STATUS_PERMANENT;
         $photo_cheque_file =file_save($photo_cheque_file);
         $file_object = file_load($value['photo_cheque']);
         $trip_items[$key]['photo_cheque'] = $file_object->uri;
         file_usage_add($file_object, 'mrc_inventory_returns', 'vrs', $value['photo_cheque']);
      }
   
      if (!empty($value['photo_signed_bill']))   {
         $photo_signed_file = file_load($value['photo_signed_bill']);
         $photo_signed_file->status = FILE_STATUS_PERMANENT;
         $photo_signed_file =file_save($photo_signed_file);
         $file_object = file_load($value['photo_signed_bill']);
         $trip_items[$key]['photo_signed_bill'] = $file_object->uri;
         file_usage_add($file_object, 'mrc_inventory_returns', 'vrs', $value['photo_signed_bill']);
       }
      unset($photo_signed_file);
      unset($photo_cheque_file);
      $trip_items[$key]['trip_id'] = $trip_id;
      $trip_items[$key]['bill_number'] = $key;
      $trip_items[$key]['distributor_id'] = $dis_id;
      if (!empty($form_state['values']["photo_signed_bill_$key"]) && empty($trip_items[$key]['photo_signed_bill'])) {
        $trip_items[$key]['photo_signed_bill'] = $form_state['values']["photo_signed_bill_$key"];
      }
      if (!empty($form_state['values']["photo_cheque_$key"]) && empty($trip_items[$key]['photo_cheque'])) {
        $trip_items[$key]['photo_cheque'] = $form_state['values']["photo_cheque_$key"];
      }
    }
      insert_signed_bill_cheque_images($trip_items, $trip_id, $dis_id);
  }
}

function insert_signed_bill_cheque_images($values,$trip_id,$dis_id){
  $bill_number  = array_keys($values);
    $vrs_trip_field = array('trip_id',
    'bill_number',
    'distributor_id',
    'photo_signed_bill',  
    'photo_cheque',
  );
  if(!empty($values)){
    foreach ($values as $record) {
      db_merge('vrs_trip_items')
      ->fields($record)
      ->condition('trip_id', $trip_id)
      ->condition('distributor_id', $dis_id)
      ->condition('bill_number', $record['bill_number'])
      ->execute();
    }
    $message = t('Data Uploaded Successfully:for Trip ID:' . $trip_id);
    drupal_set_message($message);
  }
  else {
    drupal_set_message(t('Failed to Uploade data for Trip ID:' . $trip_id), 'error');
  }
}



// function to fetch vrs bill items
function get_signed_bill_cheque_images($trip_id, $distributor_id) {
    $res = array();
    if (!empty($trip_id) && !empty($distributor_id)) {
        $query = db_select('vrs_trip_items', 'bills');
        $query->condition('bills.trip_id', $trip_id);
        $query->condition('bills.distributor_id', $distributor_id);
        $query->fields('bills', array('bill_number', 'bill_status', 'to_be_collected_amt_sales_register'
            , 'to_be_collected_amt', 'actual_cash_amount', 'actual_cheque_amount',
            'actual_signed_bill_amount', 'cheque_number', 'cheque_date', 'bank_name'
            , 'short_amt', 'reason_code', 'vrs_version_no', 'trip_id', 'remarks', 'non_billable', 'photo_cheque', 'photo_signed_bill','non_bank_reason_code','vrs_confirm_date'));
        $res = $query->execute()->fetchAll(PDO::FETCH_UNIQUE | PDO::FETCH_ASSOC);
    }
    return $res;
}