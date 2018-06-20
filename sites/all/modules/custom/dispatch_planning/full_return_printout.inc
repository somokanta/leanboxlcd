<?php

/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function get_data_of_full_return_form($form, &$form_state){
  $form['#prefix'] = '<div id="itemlist-div">';
  $form['#suffix'] = '</div>';

  $form['trip_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Trip Id'),
    '#size' => 10,
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('submit'),
    '#ajax' => array(
      'wrapper' => 'itemlist-div',
      'callback' => 'full_return_form_submit',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    '#submit' => array('get_data_of_full_return_form_submit'),
    '#validate' => array('get_data_of_full_return_form_validate'),
  );
  if (!empty($form_state['views_data'])) {
    $form['hhh'] = array(
      '#type' => 'markup',
      '#markup' => $form_state['views_data'],
    );
    $trip_id = $form_state['values']['trip_id'];
    $rr = l('Open PDF',"/download-return/$trip_id", array('attributes'=>array("target" => "_blank")));
    $form['dowmload_mark'] = array(
      '#type' => 'markup',
      '#markup' => $rr,
    );
  }
  return $form;
}

function full_return_form_submit($form, &$form_state) {
  return $form;
}

function get_data_of_full_return_form_validate($form, &$form_state){
  $trip_id = $form_state['values']['trip_id'];
  if (!is_numeric($trip_id)) {
    unset($form_state['complete form']['hhh']);
    unset($form_state['complete form']['dowmload_mark']);
    form_set_error('trip_id', "Enter Correct Integer Value");
  }  
  $status = check_bill_status($trip_id);
  if (!empty($status)) {
    drupal_set_message(t("Status must be present for bills : $status"), "error");
  } 
}

function get_data_of_full_return_form_submit($form, &$form_state) {
  $trip_id = $form_state['values']['trip_id'];
  $view_name = "full_return_printout";
  $view = views_get_view($view_name, '');
  $view->set_arguments(array($trip_id));
  $rendered_html = $view->render();
  $form_state['views_data'] = $rendered_html;
  $form_state['rebuild'] = TRUE;
}

function download_return($args){
  $view_name = "full_return_printout";
  $view = views_get_view($view_name, '');
  $view->set_arguments(array($args));
  $rendered_html = $view->render();
  pdf_using_mpdf_api($rendered_html);
}