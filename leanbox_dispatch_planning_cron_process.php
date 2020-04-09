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
    watchdog('test_abhi_multithread dispatch param', '<pre>' . print_r($param . ' ' . time(), TRUE) . '</pre>');
    $ids = explode(',', $param);

    if (!empty($ids)) {
        $query = db_select('dispatch_planning_api_log', 'a');
        $query->fields('a', array('request_data', 'id', 'trip_id'));
        $query->condition('a.id', $ids, 'IN');
        $query->condition('a.api_type', 'dispatch_planning');
        $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

        dispatch_planning_cron_process($res); //call actual function to process
    }
}

exit;
?>