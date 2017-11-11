<?php

/**
 * Files containing form for file upload in petty cash
 */

/**
 * 
 * @param array $form
 * @param type $form_state
 * @return array
 */
function file_upload_petty_cash_callback($form, &$form_state) {
   $form = array();
   $form['#prefix'] = '<div id="petty-cash-div">';
   $form['#suffix'] = '</div>';

   // get logged in users distributor id
   $user_id = $GLOBALS['user']->uid;
   $distributor_id = change_feeds_import_get_distributor_id($user_id);

   $form['voucher'] = array(
     '#title' => t('Voucher No'),
     '#type' => 'textfield',
     '#size' => 20,
     '#maxlength' => 20,
     '#autocomplete_path' => 'voucher-no/list',
     '#ajax' => array(
       'callback' => '_petty_cash_file_ajax',
       'wrapper' => 'petty-cash-div',
       'method' => 'replace',
       'effect' => 'fade',
     ),
   );
   $form['distributor_id'] = array(
     '#type' => 'value',
     '#value' => $distributor_id,
   );
   if (isset($form_state['values']['voucher']) && !empty($form_state['values']['voucher'])) {
      $voucher = $form_state['values']['voucher'];
      $output = _get_voucher_details_distributor_wise($distributor_id, $form_state['values']['voucher']);
      $file_id = variable_get("$voucher_$distributor_id");

      if (!empty($output['rows'])) {
         $html = theme('table', array(
           'header' => $output['header'],
           'rows' => $output['rows'],
           'attributes' => array('width' => '100%')
         ));
         $form['voucher_details'] = array(
           '#markup' => $html,
         );
         $form['voucher_rows'] = array(
           '#type' => 'value',
           '#value' => $rows,
         );
         $form['file_detail'] = array(
           '#title' => t('Upload file'),
           '#type' => 'managed_file',
           '#required' => TRUE,
           '#default_value' => !empty($file_id) ? $file_id : NULL,
           '#progress_indicator' => "bar",
           '#upload_validators' => array(
             'file_validate_extensions' => array('pdf png jpg jpeg'),
             // Pass the maximum file size in bytes
             'file_validate_size' => array(MAX_FILE_SIZE * 1024 * 1024),
           ),
           '#description' => t('Allowed extensions: pdf png jpg jpeg'),
         );
         $form['submit'] = array(
           '#value' => 'delete',
           '#type' => 'submit',
           '#size' => 20,
         );
      }
   }
   return $form;
}

/**
 * Menu callback for AJAX additions. Render voucher details.
 */
function _petty_cash_file_ajax($form, &$form_state) {
   return $form;
}

/**
 * Submit handler for form file_upload_petty_cash_callback
 * @param type $form
 * @param type $form_state
 */
function file_upload_petty_cash_callback_submit($form, &$form_state) {
   dpm($form_state, 'state');
   if (!empty($form_state['values']['voucher']) && !empty($form_state['values']['distributor_id'])) {

      $old_voucher = $form_state['values']['voucher'];
      $distributor_id = $form_state['values']['distributor_id'];

      //remove existing file while clicking remove button
      //it sets file element value to zero when clicked on remove button
      if ($form_state['values']['file_details'] == 0) {

         $exist_file_id = variable_get("$old_voucher_$distributor_id");
         //get existing file id to delete
         $exist_file = file_load($exist_file_id);
         if (!empty($exist_file)) {
            //delete file usage
            file_usage_delete($exist_file, 'leanbox_petty_cash', 'user', $distributor_id);

            // delete file forcefully
            file_delete($exist_file, TRUE);
            drupal_set_message(t('File removed.'));
         }
      }
      else {

         //adding file and updating fid in petty_cash
         $file = file_load($form_state['values']['file_details']);
         if (!empty($file->fid)) {
            //get currently uploaded file id
            $file_id = $file->fid;
            //setting file id to its variable
            variable_set("$old_voucher_$distributor_id", $file_id);
            file_usage_add($file, 'leanbox_petty_cash', 'user', $distributor_id);
            //make file status as permanent
            $file->status = FILE_STATUS_PERMANENT;
            file_save($file);

            //  update file id in petty cash table
            db_update('petty_cash')
                ->fields(array('distributor_id' => 60))
                ->condition('distributor_id', $distributor_id)
                ->condition('voucher', $old_voucher)
                ->execute();
            drupal_set_message("Documents uploaded successfully.");
         }
      }
   }
}
