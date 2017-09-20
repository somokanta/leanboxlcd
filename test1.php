<?php

/**
 * @file
 * Cron bill creation
 */

error_reporting(E_ALL);
ini_set('display_errors', TRUE);
ini_set('display_startup_errors', TRUE);
define('DRUPAL_ROOT', getcwd());
require_once DRUPAL_ROOT . '/includes/bootstrap.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

	dsm($_SERVER['argv'][1]);
 	global $user;
 	$user_load = user_load($_SERVER['argv'][1]);
 	$user = $user_load;
 	user_login_finalize();
	
	dsm("hello");
	dsm($user);

	watchdog('cron_test', "helloooooo".$_SERVER['argv'][1]);
	watchdog('cron_test', '<pre>' . print_r($user, TRUE) . '</pre>');

	if (user_is_logged_in()) {
		module_load_include('pages.inc', 'user', 'user');
		user_logout_current_user();
	}

?>