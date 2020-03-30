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
  $trips = explode(',', $param);
  watchdog('test_abhi_multithread param', '<pre>' . print_r($trips, TRUE) . '</pre>');
  leanbox_test_multithread($trips);
}

/**
 * 
 * @param type $trips
 * @return string
 *  Functions for pushing order to API Asynchronously
 */
function leanbox_test_multithread($trips) {  
  $users = $type = $sap_return = $result = array();
  if (!empty($trips)) {
    foreach ($trips as $trip) {
      //actual function to process
        watchdog('test_abhi_multithread', '<pre>' . print_r($trip.' '.time(), TRUE) . '</pre>');
        sleep(10);
    }
    return 'success';
  }
}
exit;
?>