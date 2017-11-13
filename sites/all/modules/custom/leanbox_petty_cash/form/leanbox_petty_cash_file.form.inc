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
//         $form['voucher_rows'] = array(
//           '#type' => 'value',
//           '#value' => $rows,
//         );
         $form['file'] = array(
           '#title' => t('Upload file'),
           '#type' => 'file',
           //  '#required' => TRUE,
           '#description' => t('Allowed extensions: pdf png jpg jpeg txt'),
         );
         $form['submit'] = array(
           '#value' => 'Upload',
           '#type' => 'submit',
           '#size' => 20,
         );
      }
   }
   return $form;
}

function file_upload_petty_cash_callback_validate($form, &$form_state) {
   $file = file_save_upload('file', array(
     'file_validate_extensions' => array('pdf png jpg jpeg txt'), // Validate extensions.
   ));
   // If the file passed validation:
   if ($file) {
      // Change the status
      $file->status = FILE_STATUS_PERMANENT;
      // Update the file status into the database
      file_save($file);
      // Move the file, into the Drupal file system
      if ($file = file_move($file, 'public://')) {
         // Save the file for use in the submit handler.
         $form_state['storage']['file'] = $file;
      }
      else {
         form_set_error('file', t('Failed to write the uploaded file the site\'s file folder.'));
      }
   }
   else {
      form_set_error('file', t('No file was uploaded. kinldy rename it and upload again.'));
   }
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
   if (!empty($form_state['values']['voucher']) && !empty($form_state['values']['distributor_id'])) {

      $old_voucher = $form_state['values']['voucher'];
      $distributor_id = $form_state['values']['distributor_id'];

      //adding file and updating fid in petty_cash
      $file = ($form_state['storage']['file']);
      if (!empty($file->fid)) {
         $file_id = $file->fid;


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
