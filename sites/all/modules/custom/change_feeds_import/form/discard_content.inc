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

  $message = "Data is Discarded Successfully";
  drupal_set_message($message);
  drupal_goto("/billing_data/import");
}
