<?php

/**
 * @file
 * Multithreading scheduler
 */
error_reporting(E_ALL);
ini_set('display_errors', TRUE);
ini_set('display_startup_errors', TRUE);
define('DRUPAL_ROOT', getcwd());
require_once DRUPAL_ROOT . '/includes/bootstrap.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

/* * ** Multithread SCcript URL   * */
$script_name = '/leanbox_multithread_sdr_api.php';
$php_script = getcwd() . $script_name;

/* * **** Script for push order to SDR APP */
$thread = variable_get('no-thread', '2'); // number of thread 
$length = variable_get('no-trip-per-thread', '3');

$total = $thread * $length;
$trips = leanbox_get_available_trip_to_push($total);

if ($trips) {
    $trips_chunk = array_chunk($trips, $length);
    foreach ($trips_chunk as $value) {
        $trip_param = implode(',', $value);
        $str = "nohup nice php -f '$php_script' '$trip_param' > /dev/null &";
        shell_exec($str);
    }
}

/**
 * 
 * @param type $total
 * @return type
 * Function to get trip to be pushed to SDR APP
 */
function leanbox_get_available_trip_to_push($total) {
//apply range to find the $total no of trip pending not more than that
    $trips = array(1, 2, 3, 4, 5, 6);
    return $trips;
}

?>