<?php

// when discard button is clicked 
function change_feeds_import_discard_data($id) {

  // change status in error_billing_data
  $status_updated = db_update('error_billing_data') // Table name no longer needs {}
      ->fields(array(
        'status' => "Stopped",
      ))
      ->condition('id', $id, '=')
      ->execute();

  // delete data from stage_billing_data.
  db_delete('stage_billing_data')
      ->condition('error_id', $id, '=')
      ->execute();

   global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);
  $csv_file = $distributor_id . "_error.csv";
  
   $path = "public://billing_data/$csv_file";
  if (file_exists($path)) {
    drupal_unlink($path); //delete the generated error file
  }
  
  $message = "Data is Discarded Successfully";
  drupal_set_message($message);
  drupal_goto("/billing_data/import");
}
