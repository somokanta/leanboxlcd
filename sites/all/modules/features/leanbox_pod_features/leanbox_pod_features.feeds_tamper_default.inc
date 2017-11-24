<?php
/**
 * @file
 * leanbox_pod_features.feeds_tamper_default.inc
 */

/**
 * Implements hook_feeds_tamper_default().
 */
function leanbox_pod_features_feeds_tamper_default() {
  $export = array();

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'pod_call_status-call_status-required';
  $feeds_tamper->importer = 'pod_call_status';
  $feeds_tamper->source = 'Call Status';
  $feeds_tamper->plugin_id = 'required';
  $feeds_tamper->settings = array(
    'invert' => 0,
    'log' => 0,
  );
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Required field';
  $export['pod_call_status-call_status-required'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'pod_delivery_status-delivery_status-required';
  $feeds_tamper->importer = 'pod_delivery_status';
  $feeds_tamper->source = 'Delivery Status';
  $feeds_tamper->plugin_id = 'required';
  $feeds_tamper->settings = array(
    'invert' => 0,
    'log' => 0,
  );
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Required field';
  $export['pod_delivery_status-delivery_status-required'] = $feeds_tamper;

  return $export;
}
