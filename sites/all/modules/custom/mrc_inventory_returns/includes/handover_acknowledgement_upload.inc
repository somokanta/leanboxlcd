<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * 
 * @param type $form
 * @param type $form_state
 * @return array
 */
function handover_acknowledgement_form($form, &$form_state) {
  $form = array();
  $form['#prefix'] = '<div id="handover-div">';
  $form['#suffix'] = '</div>';

// get logged in users distributor id
  $user_id = $GLOBALS['user']->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);

  $form['handover_id'] = array(
    '#title' => t('Handover Document ID'),
    '#type' => 'textfield',
    '#size' => 20,
    '#maxlength' => 20,
    '#autocomplete_path' => 'handover-id/list',
    '#ajax' => array(
      'callback' => '_handover_file_ajax',
      'wrapper' => 'handover-div',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );

  $form['distributor_id'] = array(
    '#type' => 'value',
    '#value' => $distributor_id,
  );
  
  if (isset($form_state['values']['handover_id']) && !empty($form_state['values']['handover_id'])) {
    $handover_id = $form_state['values']['handover_id'];
    $output = _get_handover_details($distributor_id, $handover_id);
    if (!empty($output['rows']) && empty($output['file_check'])) {
      unset($output['rows'][0]['file']);
      
      $html = theme('table', array(
        'header' => $output['header'],
        'rows' => $output['rows'],
        'attributes' => array('width' => '100%')
      ));

      $form['handover_details'] = array(
        '#markup' => $html,
      );

      $form['acknowlege_file'] = array(
        '#title' => t('Acknowledgement Upload *'),
        '#type' => 'file',
        '#description' => t('Allowed extensions: pdf png jpg jpeg txt'),
      );
      $form['submit'] = array(
        '#value' => 'Upload',
        '#type' => 'submit',
        '#size' => 20,
      );
    }
    else {
      drupal_set_message(t('Acknowledgement slip has been already uploaded for this handover id.'), 'error');
    }
  }

  return $form;
}

/**
 * Menu callback for AJAX additions. Render $handover details.
 */
function _handover_file_ajax($form, &$form_state) {
  return $form;
}

/**
 * 
 * @param type $distributor_id
 * @param type $handover_id
 */
function _get_handover_details($distributor_id, $handover_id) {
  $output = array();
  if (!empty($distributor_id) && !empty($handover_id)) {
    $substring = strtolower(substr($handover_id,0,2));
    
    $query = db_select('vrs_handover', 'vh');
    $query->condition('vh.distributor_id', $distributor_id);
    if($substring == 'cs') {
      $query->condition('vh.handover_id', $handover_id);
      $query->fields('vh', array('handover_id'));
    }
    elseif($substring == 'cq') {
      $query->condition('vh.handover_cheque_id', $handover_id);
      $query->fields('vh', array('handover_cheque_id'));
    }
    else {
      $query->condition('vh.handover_signedbill_id', $handover_id);
      $query->fields('vh', array('handover_signedbill_id'));
    }
    $query->fields('vh', array('collection_type','handover_to','handover_date','handover_amt','petty_cash_amt','remarks','file'));
    $rows = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

    $header = array(
      'Handover Document ID',
      'Handover Document Type',
      'Handover To',
      'Handover Date',
      'Handover Amount',
      'Petty Cash Amount',
      'Remarks',
    );
    if (isset($rows[0]['handover_date'])) {
      $rows[0]['handover_date'] = date('d-M-y', $rows[0]['handover_date']);
    }

    if (empty($rows[0]['file'])) {
      $output['header'] = $header;
      $output['rows'] = $rows;
      $output['file_check'] = 0;
    }
    else {
      $output['header'] = $header;
      $output['rows'] = $rows;
      $output['file_check'] = 1;
    }
  }

  return $output;
}

function handover_acknowledgement_form_validate($form, &$form_state) {
  $file = file_save_upload('acknowlege_file', array(
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
      $form_state['storage']['acknowlege_file'] = $file;
    }
    else {
      form_set_error('acknowlege_file', t('Failed to write the uploaded file the site\'s file folder.'));
    }
  }
  else {
    form_set_error('acknowlege_file', t('No file is uploaded.'));
  }
}

/**
 * Submit handler for form handover_acknowledgement_form
 * @param type $form
 * @param type $form_state
 */
function handover_acknowledgement_form_submit($form, &$form_state) {
  if (!empty($form_state['values']['handover_id']) && !empty($form_state['values']['distributor_id'])) {
    $distributor_id = $form_state['values']['distributor_id'];
    $handover_id = $form_state['values']['handover_id'];
   
//adding file and updating fid in vrs_handover
    $file = $form_state['storage']['acknowlege_file'];
    $file_id = $file->fid;

    if (!empty($file_id)) {
    // update file id in vrs_handover table
      db_update('vrs_handover')
          ->fields(array('file' => $file_id, 'file_date' => time()))
          ->condition(db_or()
            ->condition('handover_id', $handover_id)
            ->condition('handover_cheque_id', $handover_id)
            ->condition('handover_signedbill_id', $handover_id)
          )
          ->execute();
      
      // update file id in vrs_handover_history table
      db_update('vrs_handover_history')
          ->fields(array('file' => $file_id, 'file_date' => time()))
          ->condition(db_or()
            ->condition('handover_id', $handover_id)
            ->condition('handover_cheque_id', $handover_id)
            ->condition('handover_signedbill_id', $handover_id)
          )
          ->execute();
      
      drupal_set_message("Acknowledgement uploaded successfully.");
    }
  }
}
