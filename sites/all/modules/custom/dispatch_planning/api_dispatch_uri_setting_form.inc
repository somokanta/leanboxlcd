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
  return system_settings_form($form);
}
