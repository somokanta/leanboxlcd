<?php
/**
 * 
 * @param type $form
 * @param type $form_state
 * @return string
 * Create form
 */
function delivery_details_form($form, $form_state) {
  $form['file'] = array(
    '#type' => 'file',
    '#title' => t('Choose a CSV file for Delivery details upload'),
  );
  $form['template_download'] = array(
    '#type' => 'markup',
    '#prefix' => '<div id="template_wrapper">',
    '#suffix' => '</div>',
    '#markup' => l("Download Template", drupal_get_path('module', 'delivery_details_tracking') . '/delivery_details_upload_template.csv'),
  );

  $text = "<br>Please enter the below values in Status field:<ul><li>Delivered</li><li>Full Return</li><li>Pending</li>";
  $form['text_new'] = array(
    '#markup' => $text,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Upload'),
  );
  return $form;
}

/**
 * 
 * @global type $user
 * @param type $form
 * @param type $form_state
 * Implements form_validate
 */
function delivery_details_form_validate($form, &$form_state) {
  $file = file_save_upload('file', array(
    'file_validate_extensions' => array('csv'),
  ));

  $header_array_from_xsd = array(
    'Dispatch Date', 'Bill No.', 'Van No.', 'Trip ID', 'Cashier Name', 'Delivery Boy Name', 'Status', 'Delivery Date (dd-mmm-yyyy)', 'Comments'
  );
  if ($file) {
    if ($file = file_move($file, 'public://')) {
      $form_state['values']['file'] = $file;
      $filepath = $form_state['values']['file']->uri;
      $data = array_map("str_getcsv", file($filepath, FILE_SKIP_EMPTY_LINES));
      $header_from_csv = array_shift($data);
      if ($header_array_from_xsd != $header_from_csv) {
        $type = 'error';
        $message = 'Header mismatch.';
        form_set_error('file_upload', t($message));
      }
      $datas = array();
      if (($handle = fopen($filepath, "r")) !== FALSE) {
        $header = fgetcsv($handle, 0, ',', '"');

        while (($row = fgetcsv($handle) ) !== FALSE) {

          foreach ($row as $key => &$value) {
            $value = mb_convert_encoding(trim($value), 'UTF-8', 'UTF-8');
          }

          $datas[] = array_combine($header, $row);
        }
        fclose($handle);
      }

      global $user;
      $dis_id = change_feeds_import_get_distributor_id($user->uid);
      $error_flag = 0;
      foreach ($datas as $key => $val) {
        $bill_no = $val['Bill No.'];
        $status = ucwords(strtolower($val['Status']));
        $dispatch_date = $val['Dispatch Date'];
        $delivery_date = $val['Delivery Date (dd-mmm-yyyy)'];
        if (empty($status)) {
          drupal_set_message(t("Status cannot be empty for bill number $bill_no"), 'error');
          $error_flag = 1;
        }
        if ($status != 'Delivered' && $status != 'Full Return' && $status != 'Pending') {
          drupal_set_message(t("Status $status is not valid for bill number $bill_no"), 'error');
          $error_flag = 1;
        }

        $query1 = db_select('node', 'n');
        $query1->condition('n.type', 'sales_register_data');
        $query1->condition('n.uid', $dis_id, "=");
        $query1->condition('n.title', $bill_no, "=");
        $query1->join('field_data_field_trip_id_reference', 'ref', 'ref.entity_id = n.nid');
        $query1->fields('ref', array('field_trip_id_reference_nid'));
        $nid = $query1->execute()->fetchField();
        if (empty($nid)) {
          drupal_set_message(t('Bill Number ' . $bill_no . ' should be attached to a trip ID'), 'error');
          $error_flag = 1;
        }

        if (!isset($res['unique_combination'])) {
          $res['unique_combination'] = array();
        }
        $unique_comb = $bill_no . $dispatch_date;
        if (array_key_exists($unique_comb, $res['unique_combination'])) {
          drupal_set_message(t("Duplicate Data Found Of Bill Number: $bill_no and Dispatch Date: $dispatch_date. Please Remove Duplicate Data and Upload Again."), 'error');
          $error_flag = 1;
        }
        $res['unique_combination'][$unique_comb] = $unique_comb;

        $todays_date = strtotime(date('d-M-Y'));
        $query = db_select('delivery_details', 'dt');
        $query->condition('dt.bill_number', $bill_no, "=");
        $query->condition('dt.distributor_id', $dis_id, "=");
        $query->fields('dt', array('status', 'dispatch_date'));
        $result = $query->execute()->fetchAll();
        foreach ($result as $key => $value) {
          if ($value->status == 'Full Return' || $value->status == 'Delivered') {
            drupal_set_message(t("The Bill Number $bill_no is already marked as Full Return or Delivered"), 'error');
            $error_flag = 1;
            break;
          }
          else if ($value->status == 'Pending' && $value->dispatch_date == strtotime($dispatch_date)) {
            drupal_set_message(t("Bill number $bill_no and dispatch date $dispatch_date combination is already present"), 'error');
            $error_flag = 1;
            break;
          }
        }
        
        if (!empty($delivery_date) && ($status == 'Pending' || $status == 'Full Return')) {
          drupal_set_message(t("Delivery date has to be blank if status is Pending or Full Return for bill number $bill_no and dispatch date $dispatch_date"), 'error');
          $error_flag = 1;
        }
        if (empty($delivery_date) && $status == 'Delivered') {
          drupal_set_message(t("Delivery date is mandatory for Delivered status for bill number $bill_no"), 'error');
          $error_flag = 1;
        }
      }
      if ($error_flag == 1) {
        form_set_error('form', t('Above Error occured'));
        file_delete($file,TRUE);
      }
    }
    else {
      form_set_error('file_upload', t('Failed to write the uploaded file the site\'s file folder.'));
    }
  }
  else {
    form_set_error('file', t('No file was uploaded.'));
  }
}

/**
 * 
 * @param type $form
 * @param type $form_state
 * @param type $form_state
 * Implements form submit
 */
function delivery_details_form_submit($form, &$form_state) {

  $file = $form_state['values']['file'];
  $filepath = $file->uri;

  $datas = array();

  if (($handle = fopen($filepath, "r")) !== FALSE) {
    $header = fgetcsv($handle, 0, ',', '"');

    while (($row = fgetcsv($handle) ) !== FALSE) {

      foreach ($row as $key => &$value) {
        $value = mb_convert_encoding(trim($value), 'UTF-8', 'UTF-8');
      }

      $datas[] = array_combine($header, $row);
    }
    fclose($handle);
  }

  $operations = array();
  $data_chunk = array_chunk($datas, 200);
  foreach ($data_chunk as $data) {
    $operations[] = array('delivery_details_form_process', array($data));
  }
  $batch = array(
    'title' => t('Uploading Delivery Details Data Csv'),
    'operations' => $operations,
    'init_message' => t("Please wait..."),
    'progress_message' => t('Processed @current out of @total.'),
    'error_message' => t('Upload data processing has encountered an error.'),
    'finished' => 'delivery_details_upload_form_finished',
    'file' => drupal_get_path('module', 'delivery_details_tracking') . '/delivery_details_upload.inc',
  );
  batch_set($batch);
}

function delivery_details_form_process($data, &$context) {
  insert_data_in_delivery_details_table($data);
}

function delivery_details_upload_form_finished($success, $results, $operations) {
  if ($success) {
    drupal_set_message('Delivery Details Data are Uploaded successfully');
    $_GET['destination'] = "delivery-details/import";
  }
  else {
    drupal_set_message('There is Error Occured in batch process');
  }
}

function insert_data_in_delivery_details_table($data) {
  global $user;
  $distributor_id = change_feeds_import_get_distributor_id($user->uid);

  foreach ($data as $value) {
    $dispatch_date = strtotime($value['Dispatch Date']);
    $bill_number = $value['Bill No.'];
    $van_number = $value['Van No.'];
    $trip_id = $value['Trip ID'];
    $cashier_name = $value['Cashier Name'];
    $del_boy_name = $value['Delivery Boy Name'];
    $status = ucwords(strtolower($value['Status']));
    $del_date = strtotime($value['Delivery Date (dd-mmm-yyyy)']);
    $comments = $value['Comments'];

    db_insert('delivery_details')
      ->fields(array(
        'dispatch_date' => !empty($dispatch_date)  ? $dispatch_date : null,
        'bill_number' => $bill_number,
        'van_number' => $van_number,
        'trip_id' => !empty($trip_id) ? $trip_id : null,
        'cashier_name' => $cashier_name,
        'delivery_boy_name' => $del_boy_name,
        'status' => $status,
        'delivery_date' => !empty($del_date) ? $del_date : null,
        'comments' => $comments,
        'distributor_id' => $distributor_id,
        'created_time' => REQUEST_TIME,
      ))
      ->execute();
  }
}
