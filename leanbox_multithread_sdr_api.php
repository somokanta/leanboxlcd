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

if ($param = $_SERVER['argv'][1]) {
    watchdog('test_abhi_multithread param', '<pre>' . print_r($param . ' ' . time(), TRUE) . '</pre>');
    $ids = explode(',', $param);
    
    if (!empty($ids)) {
        $query = db_select('sdr_app_log_preprocess', 'sdr');
        $query->fields('sdr', array('id', 'trip_id', 'distributor_id', 'bills'));
        $query->orderBy('sdr.created_date');
        $query->condition('sdr.id', $ids, 'IN');
        $trips = $query->execute()->fetchAll();

        sdr_api_process($trips); //call actual function to process
    }
}

exit;
?>