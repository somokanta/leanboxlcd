<?php

/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function get_data_of_delivery_pending_form($form, &$form_state){
  $form['#attached']['js'][] = drupal_get_path('module', 'dispatch_planning') . '/js/delivery_pending.js';  
  $form['#prefix'] = '<div id="itemlist-div">';
  $form['#suffix'] = '</div>';

  $form['trip_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Trip ID'),
    '#size' => 10,
    '#required' => TRUE,
  );
   $bill_options = array('bill_wise'=>t('Bill wise'),'sku_wise'=>t('SKU wise'));
   $form['bill_option'] = array(
        '#type' => 'radios',
        '#required' => true,
        '#title' => t('View Bill wise or SKU wise'),
        '#options' => $bill_options,
        '#suffix' => '</div>'
    );

    $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('submit'),
    '#ajax' => array(
      'wrapper' => 'itemlist-div',
      'callback' => 'get_data_of_delivery_pending_form_callback',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    '#submit' => array('get_data_of_delivery_pending_form_submit'),
    '#validate' => array('get_data_of_delivery_pending_form_validate'),
  );
  if (!empty($form_state['views_data'])) {
    $form['delivery_pending'] = array(
      '#type' => 'markup',
      '#markup' => $form_state['views_data'],
    );
    $trip_id = $form_state['values']['trip_id'];
    $bill_wise_option = $form_state['input']['bill_option'];
    $rr = l('Open PDF',"/download-delivery-pending/$trip_id/$bill_wise_option", array('attributes'=>array("target" => "_blank")));
    $form['dowmload_mark'] = array(
      '#type' => 'markup',
      '#markup' => $rr,
    );
  }
  return $form;
}

function get_data_of_delivery_pending_form_callback($form, &$form_state) {
  return $form;

}

function get_data_of_delivery_pending_form_validate($form, &$form_state){
  $trip_id = $form_state['values']['trip_id'];
  if (empty($trip_id)) {
       form_set_error('trip_id', "Please enter Trip ID.");
  }
  if (empty($form_state['values']['bill_option'])) {
       form_set_error('bill_option', "Please selct Bill wise or SKU wise option to view.");
  }
}
function check_delivery_pending_bill_exists($trip_id) {
        global $user;
        $distributor_id = change_feeds_import_get_distributor_id( $user->uid);
        $dp_rows = array();
    if (!empty($trip_id)) {
        $query = db_select('dispatch_planning_api_data', 'ad');
        $query->fields('ad', array('id'));
        $query->condition('runsheet_number', $trip_id, '=');
        $query->condition('delivery_pending_flag', 1);
        $query->condition('distributor_id', $distributor_id, '=');
        $dp_rows = $query->execute()->fetchAllKeyed(0, 0);
    }
    return $dp_rows;
}


function get_data_of_delivery_pending_form_submit($form, &$form_state) {
  $trip_id = $form_state['values']['trip_id'];
  $bill_option = $form_state['values']['bill_option'];
  $view_name = "delivery_pending_printout_pp_van";
  global $user;
  $distributor_id = change_feeds_import_get_distributor_id($user->uid);
  $check_trip_id = check_trip_id_exists_or_not($distributor_id, $trip_id);
    if (empty($check_trip_id->nid)) {
      form_set_error('trip_id', "Trip ID :$trip_id does not exists.");
    }
  //delivery_pending_printout_non_pp
  $van_type = check_trip_van_type($trip_id);
  //$van_type == 1 Party Pack VAN 2 == NON PP VAN
  if ($bill_option == 'bill_wise') {
    $view_name = "delivery_pending_printout_pp_van";
  }// aggregation
  else if($bill_option == 'sku_wise') {
      $view_name = "delivery_pending_printout_non_pp";
  }
  $delivery_pending_bills = check_delivery_pending_bill_exists($trip_id);
  if (!empty($delivery_pending_bills)) {
    $view = views_get_view($view_name, '');
    $view->set_arguments(array($trip_id));
    $rendered_html = $view->render();
    $form_state['views_data'] = $rendered_html;
  }
  else {
       //form_set_error('trip_id', "No Bills Found.");
       ajax_redirect('delivery-pending-printout');
  }
    $form_state['rebuild'] = TRUE;
}

function download_delivery_pending($trip_id,$bill_option){
  $view_name = "delivery_pending_printout_pp_van";
  //delivery_pending_printout_non_pp
  $van_type = check_trip_van_type($trip_id);
  //$van_type == 1 Party Pack VAN 2 == NON PP VAN
  if ($bill_option == 'bill_wise') {
    $view_name = "delivery_pending_printout_pp_van";
  }// aggregation
  else if($bill_option == 'sku_wise') {
      $view_name = "delivery_pending_printout_non_pp";
  }
  $view = views_get_view($view_name, '');
  $view->set_arguments(array($trip_id));
  $rendered_html = $view->render();
  pdf_using_mpdf_api($rendered_html);
}
// This for checking Delivery pending Dp Van type
function check_trip_van_type($trip_id) {
    $selected_option = 0;
    global $user;
    if (!empty($trip_id)) {
        $user_id = $user->uid;
        $distributor_id = change_feeds_import_get_distributor_id($user_id);
        $node_obj = node_load_multiple(NULL, array('title' => $trip_id, 'type' => 'dispatch_data', 'uid' => $distributor_id));
        $trip_nid = key($node_obj);
        $node = $node_obj[$trip_nid];
        $selected_option = $node->field_is_party_pack[LANGUAGE_NONE][0]['value'];
    }
    return $selected_option;
}
