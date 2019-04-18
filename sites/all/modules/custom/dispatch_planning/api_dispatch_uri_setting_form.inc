<?php

/**
 * @file
 * Administration page for API setting form
 *
 */
function api_dispatch_uri_setting_form($form, &$form_state) {
  $form = array();
  $form['dispatch_uri_settings'] = array(
    '#type' => 'textarea',
    '#title' => t('Dispatch planning uri API settings'),
    '#description' => t('Eg.https://uri.com/key=?=182'),
    '#default_value' => variable_get('dispatch_uri_settings'),
    //'#size' => 10,
  );
  $form['dispatch_job_type'] = array(
    '#type' => 'textfield',
    '#title' => t('Dispatch planning Job Type settings'),
    '#default_value' => variable_get('dispatch_job_type'),
    //'#size' => 10,
  );
  
  $form['del_app'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  
  $form['del_app']['new_del_app'] = array(
     '#type' => 'markup',
     '#markup' => '<b>DELIVERY APP SETTINGS:</b>',
  );
  
  $form['del_app']['new_trip_ids'] = array(
    '#type' => 'textfield',
    '#title' => t('Delivery App Trip IDs'),
    '#description' => t('Enter Trip IDs comma separated'),
    '#default_value' => variable_get('new_trip_ids'),
    //'#size' => 10,
  );
  
  $form['del_app']['new_dispatch_uri_settings'] = array(
    '#type' => 'textarea',
    '#title' => t('API Url'),
    '#description' => t('Eg.https://uri.com/key=?=182'),
    '#default_value' => variable_get('new_dispatch_uri_settings'),
    //'#size' => 10,
  );
  
  $form['del_app']['new_dispatch_job_type'] = array(
    '#type' => 'textfield',
    '#title' => t('Delivery App Job Type'),
    '#default_value' => variable_get('new_dispatch_job_type'),
    //'#size' => 10,
  );
  return system_settings_form($form);
}
