<?php

/**
 * @file
 * SAP PI Push to SAP Multithreading
 */

error_reporting(E_ALL);
ini_set('display_errors', TRUE);
ini_set('display_startup_errors', TRUE);
define('DRUPAL_ROOT', getcwd());
require_once DRUPAL_ROOT . '/includes/bootstrap.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);


dsm($_SERVER['argv'][1]);
dsm("hello");
watchdog('mine', "helloooooo".$_SERVER['argv'][1]);
?>