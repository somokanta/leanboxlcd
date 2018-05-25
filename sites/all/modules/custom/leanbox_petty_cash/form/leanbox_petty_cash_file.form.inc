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
  if (isset($form_state['input']['voucher']) && !empty($form_state['input']['voucher'])) {
    $user_id = $GLOBALS['user']->uid;
    $distributor_id = change_feeds_import_get_distributor_id($user_id);
    $voucher = $form_state['values']['voucher'];
    $query = db_select('petty_cash', 'pc');
    $query->fields('pc', array('file', 'bill_file'));
    $query->condition('pc.distributor_id', $distributor_id);
    $query->condition('pc.voucher_no', $voucher);
    $rows = $query->execute()->fetch();
    if (!empty($rows->file) || !empty($rows->bill_file)) {
      drupal_set_message(t('Voucher or Bill copy has already been uploaded for this Voucher'), 'error');
    }
    $output = _get_voucher_details_distributor_wise($distributor_id, $voucher);
    if (!empty($output['rows'])) {

      $form['old_voucher_table_id'] = array(
        '#type' => 'value',
        '#value' => $output['rows'][0]['id'],
      );

      unset($output['rows'][0]['id']);
      $html = theme('table', array(
        'header' => $output['header'],
        'rows' => $output['rows'],
        'attributes' => array('width' => '100%')
      ));
      $form['voucher_details'] = array(
        '#markup' => $html,
      );

      $form['file'] = array(
        '#title' => t('Voucher Copy Upload'),
        '#type' => 'file',
        '#description' => t('Allowed extensions: pdf png jpg jpeg txt'),
      );
      $form['bill_file'] = array(
        '#title' => t('Bills Copy Upload'),
        '#type' => 'file',
        '#description' => t('Allowed extensions: pdf png jpg jpeg txt'),
      );
      $form['submit'] = array(
        '#value' => 'Upload',
        '#type' => 'submit',
        '#size' => 20,
        '#validate' => array('file_upload_petty_cash_callback_validate1'),
      );
    }
  }
  return $form;
}

function file_upload_petty_cash_callback_validate1($form, &$form_state) {
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

  $file = file_save_upload('bill_file', array(
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
      $form_state['storage']['bill_file'] = $file;
    }
    else {
      form_set_error('bill_file', t('Failed to write the uploaded bill file the site\'s file folder.'));
    }
  }
  if (empty($form_state['storage']['file']->fid) && empty($form_state['storage']['bill_file']->fid)) {
    form_set_error('form', t('Upload either Voucher Copy or Bill copy'));
  }
}

/**
 * Menu callback for AJAX additions. Render voucher details.
 */
function _petty_cash_file_ajax($form, &$form_state) {
  // Used for removing drupal set message on submit.
  $commands[] = ajax_command_replace('#petty-cash-div', render($form));
  $commands[] = array
    (
    'command' => 'filter_clicked',
  );
  $commands[] = ajax_command_replace('.alert-success', '');
  $commands[] = ajax_command_prepend('#petty-cash-div', theme('status_messages'));
  return array('#type' => 'ajax', '#commands' => $commands);
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
    $voucher = $form_state['values']['voucher'];
    //adding file and updating fid in petty_cash
    $file = ($form_state['storage']['file']);
    $bill_file = ($form_state['storage']['bill_file']);
    $file_id = $file->fid;
    $bill_file_id = $bill_file->fid;
    if (empty($file_id) || empty($bill_file_id)) {
      $query = db_select('petty_cash', 'pc');
      $query->fields('pc', array('file', 'bill_file'));
      $query->condition('pc.distributor_id', $distributor_id);
      $query->condition('pc.voucher_no', $voucher);
      $rows = $query->execute()->fetch();
      $file_db_id = $rows->file;
      $bill_file_db_id = $rows->bill_file;
    }
      if (!empty($file_id) || !empty($bill_file_id)) {
        $old_voucher_serial_id = $form_state['values']['old_voucher_table_id'];
        if (!empty($old_voucher_serial_id)) {
          //  update file id in petty cash table
          db_update('petty_cash')
            ->fields(array('file' => empty($file_id) ? $file_db_id : $file_id, 'bill_file' => empty($bill_file_id) ? $bill_file_db_id : $bill_file_id, 'file_date' => time()))
            ->condition('id', $old_voucher_serial_id)
            ->execute();
          drupal_set_message("Documents uploaded successfully.");
        }
      }
    }
}