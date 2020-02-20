<?php

/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function get_data_of_asset_summary_return_form($form, &$form_state){
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
      'callback' => 'asset_return_form_submit',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    '#submit' => array('get_data_of_asset_summary_return_form_submit'),
    '#validate' => array('get_data_of_asset_summary_return_form_validate'),
  );
  if (!empty($form_state['views_data'])) {
    $form['asset_return'] = array(
      '#type' => 'markup',
      '#markup' => $form_state['views_data'],
    );
  }
  return $form;
}

function asset_return_form_submit($form, &$form_state) {
  return $form;
}

function get_data_of_asset_summary_return_form_validate($form, &$form_state){
  global $user;
  $rs_code = dispatch_planning_get_rscode_using_uid($user->uid);
  $trip_id = $form_state['values']['trip_id'];
  if (!is_numeric($trip_id)) {
    unset($form_state['complete form']['asset_return']);
    unset($form_state['complete form']['dowmload_mark']);
    form_set_error('trip_id', "Enter Correct Integer Value");
  }

  $query = db_select('dispatch_planning_api_data', 'ad');
  $query->condition('ad.runsheet_number', $trip_id);
  $query->condition('ad.hub', $rs_code);
  $query->fields('ad', array('runsheet_number'));
  $result = $query->execute()->fetchField();
  if (empty($result)) {
    unset($form_state['complete form']['asset_return']);
    unset($form_state['complete form']['dowmload_mark']);
    form_set_error('trip_id', "Enter correct Trip Id");
  }
}

function get_data_of_asset_summary_return_form_submit($form, &$form_state) {
  $trip_id = $form_state['values']['trip_id'];
  $view_name = "asset_summary";
  $view = views_get_view($view_name, '');
  $view->set_arguments(array($trip_id));
  $rendered_html = $view->render();
  $form_state['views_data'] = $rendered_html;
  $form_state['rebuild'] = TRUE;
}