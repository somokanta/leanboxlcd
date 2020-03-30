<?php

/**
 * @file
 * Multithreading API
 */

error_reporting(E_ALL);
ini_set('display_errors', TRUE);
ini_set('display_startup_errors', TRUE);
define('DRUPAL_ROOT', getcwd());
require_once DRUPAL_ROOT . '/includes/bootstrap.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

if($param = $_SERVER['argv'][1]) {
  $orders = explode(',', $param);
  leanbox_test_multithread($orders);
}

/**
 * 
 * @param type $orders
 * @return string
 *  Functions for pushing order to API Asynchronously
 */
function leanbox_test_multithread($orders) {  
  $users = $type = $sap_return = $result = array();
  if (!empty($orders)) {
    foreach ($orders as $order_id) {
      //actual function to process
        watchdog('test_abhi_multithread', '<pre>' . print_r($order_id, TRUE) . '</pre>');
        //sleep(1);
    }
    return 'success';
  }
}
exit;
?>