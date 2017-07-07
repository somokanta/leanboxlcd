<?php

// upload form for billing data

function change_feeds_import_sales_delivery_form($form, $form_state) {
  global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);

  // check distributor incompleted upload
  $query = db_select('custom_sales_delivery', 'sd');
  $query->condition('sd.distributor_id', $distributor_id, '=');
  $query->fields('sd', array('fid'));
  $fid = $query->execute()->fetchField();

  if (!empty($fid)) {
    drupal_goto("sales-delivery-data-confirm/$fid");
  }
  $data = leanbox_dashboard_all_value_using_voc_name('Delivery Reason');
  foreach($data as $val) {
    $html .= '<li>' . $val . '</li>';
  }

  $form['file'] = array(
    '#type' => 'file',
    '#title' => t('Choose a CSV file for Sales Delivery data upload'),
  );
  $form['template_download'] = array(
    '#type' => 'markup',
    '#prefix' => '<div id="template_wrapper">',
    '#suffix' => '</div>',
    '#markup' => l("Download Template", drupal_get_path('module', 'change_feeds_import') . '/sales_delivery_upload_template.csv'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Upload'),
  );

  $form['delivery_pending'] = array(
    '#type' => 'item',
    '#title' => t('Take "SALES DELIVERED AMT" Value from "Sum of NET_SALES_VALUE" from Sales Pivot Report. Please mention "Yes" to declare bills for Delivery Pending in "Delivery Pending" Column while upload.'),
  );
  $form['reason'] = array(
    '#type' => 'item',
    '#markup' => $html,
    '#title' => t('Valid Delivery Reason'),
  );
   

  return $form;
}

function change_feeds_import_sales_delivery_form_validate($form, &$form_state) {
  $file = file_save_upload('file', array(
    'file_validate_extensions' => array('csv'),
  ));

  $header_array_from_xsd = array(
    'BILL NUMBER', 'SALES DELIVERED AMT', 'DELIVERY PENDING'
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
    }
    else {
      form_set_error('file_upload', t('Failed to write the uploaded file the site\'s file folder.'));
    }
  }
  else {
    form_set_error('file', t('No file was uploaded.'));
  }
}

function change_feeds_import_sales_delivery_form_submit($form, &$form_state) {
  global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);
  $file = $form_state['values']['file'];
  $file_name = $file->filename;
  $fid = $file->fid;
  $filepath = $file->uri;

  $datas = array();
  // Making data as associative array with header.
  if (($handle = fopen($filepath, "r")) !== FALSE) {
    $header = fgetcsv($handle, 0, ',', '"');

    while (($row = fgetcsv($handle) ) !== FALSE) {

      foreach ($row as $key => &$value) {
        $value = mb_convert_encoding($value, 'UTF-8', 'UTF-8');
      }

      $datas[] = array_combine($header, $row);
    }
    fclose($handle);
  }

  $operations = array();
  $data_chunk = array_chunk($datas, 1000);
  foreach ($data_chunk as $data) {
    $operations[] = array('change_feeds_import_sales_delivery_batch_fix_process', array($data, $distributor_id, $file_name, $fid));
  }
  $batch = array(
    'title' => t('Reading Delivery data Csv'),
    'operations' => $operations,
    'init_message' => t("Please wait..."),
    'progress_message' => t('Processed @current out of @total.'),
    'error_message' => t('Upload data processing has encountered an error.'),
    'finished' => 'sales_delivery_data_csv_finished',
    'file' => drupal_get_path('module', 'change_feeds_import') . '/form/sales_delivery_data.inc',
  );
  batch_set($batch);
}

function change_feeds_import_sales_delivery_batch_fix_process($data, $distributor_id, $file_name, $fid, &$context) {
  if (empty($distributor_id)) {
    drupal_set_message(t('Please check your Distributor code'), 'error');
  }
  require_once drupal_get_path('module', 'change_feeds_import') . '/form/upload_billing_data.inc';

  $value['error_message'] = '';
  foreach ($data as $key => $value) {

    foreach ($value as $k => $v) {
      $error_text = '';
      $v = trim($v);
      if ($k == 'SALES DELIVERED AMT') {
        if (empty($v) && $v !== '0') {
          $value['error_message'] .= $k . " can not be empty for bill no " . $value['BILL NUMBER'] . ".";
        }
      }
      else {
        if (empty($v) && $k != 'DELIVERY PENDING') {
          $value['error_message'] .= $k . " can not be empty for bill no " . $value['BILL NUMBER'] . ".";
        }
      }
    }

//    if (isset($value['IMPACT DELIVERY DATE'])) {
//      $value['IMPACT DELIVERY DATE'] = strtotime($value['IMPACT DELIVERY DATE']);
//      if (empty($value['IMPACT DELIVERY DATE'])) {
//        $value['error_message'] .= " Enter correct Bill Date for Bill No " . $value['BILL NUMBER'] . ".";
//      }
//    }
    if (isset($value['BILL NUMBER'])) {
      $value['BILL NUMBER'] = trim($value['BILL NUMBER']);
      // Check if bill no already created as a node in content type sales_register_data
      $title = check_node_title_exists($value['BILL NUMBER'], 'sales_register_data');

      if (empty($title)) {
        $value['error_message'] .= " Bill no " . $value['BILL NUMBER'] . ' Does not exist in our system.';
      }
    }
//    if (isset($value['PARTIAL/FULL'])) {
//      $value['PARTIAL/FULL'] = trim($value['PARTIAL/FULL']);
//      // Check if bill no already created as a node in content type sales_register_data
//      $partial_full_tid = leanbox_check_taxonomy_exists_by_name($value['PARTIAL/FULL'], 'Delivery Status');
//
//      if (empty($partial_full_tid)) {
//        $value['error_message'] .= " PARTIAL/FULL " . $value['PARTIAL/FULL'] . ' Does not exist for ' . $value['BILL NUMBER'] . '.';
//      }
//      else {
//        $value['PARTIAL/FULL'] = $partial_full_tid;
//      }
//    }
//    if (isset($value['REASON'])) {
//      $value['REASON'] = trim($value['REASON']);
//      // Check if bill no already created as a node in content type sales_register_data
//      $reason_tid = leanbox_check_taxonomy_exists_by_name($value['REASON'], 'Delivery Reason');
//
//      if (empty($reason_tid)) {
//        $value['error_message'] .= " REASON " . $value['REASON'] . ' Does not exist for bill no ' . $value['BILL NUMBER'] . '.';
//      }
//      else {
//        $value['REASON'] = $reason_tid;
//      }
//    }

    if (isset($value['SALES DELIVERED AMT'])) {

      if (!is_numeric($value['SALES DELIVERED AMT'])) {
        $value['error_message'] .= $value['SALES DELIVERED AMT'] . ' should be numeric bill value for bill no ' . $value['BILL_NUMBER'] . '.';
      }
    }

    if (isset($value['DELIVERY PENDING'])) {
      $value['DELIVERY PENDING'] = trim($value['DELIVERY PENDING']);
      if (!empty($value['DELIVERY PENDING'])) {
        $field = field_info_field('field_delivery_pending');
        $allowed_values = list_allowed_values($field);
        if ($delivery_pending = in_array($value['DELIVERY PENDING'], $allowed_values)) {
          $value['DELIVERY PENDING'] = $delivery_pending;
        }
        else {
          $value['error_message'] .= 'Please enter correct value for DELIVERY PENDING for bill no ' . $value['BILL_NUMBER'] . '.';
        }
      }
    }
    // insert into outstanding sales table 
    change_feeds_import_insert_data_in_custom_sales_delivery_table($value, $fid, $distributor_id);

    $context['results']['fid'] = $fid;
  }
}

function sales_delivery_data_csv_finished($success, $results, $operations) {
  if ($success) {
    $fid = $results['fid'];
    drupal_goto("sales-delivery-data-confirm/" . $fid);
  }
  else {
    drupal_set_message('There is Error Occured in batch process');
  }
}

function change_feeds_import_insert_data_in_custom_sales_delivery_table($value, $fid, $distributor_id) {

  $sales_data = db_insert('custom_sales_delivery') // Table name no longer needs {}
      ->fields(array(
        'bill_no' => $value['BILL NUMBER'],
       // 'impact_del_date' => $value['IMPACT DELIVERY DATE'],
        'impact_del_date' => time(),
        'sales_amt' => $value['SALES DELIVERED AMT'],
        'partial_full' => '',
       // 'reason' => $value['REASON'],
        'reason' =>'',
        'delivery_pending' => $value['DELIVERY PENDING'],
        'created' => time(),
        'error_message' => $value['error_message'],
        'fid' => $fid,
        'distributor_id' => $distributor_id,
      ))
      ->execute();
}

// show table after file is uploaded

function change_feeds_import_sales_delivery_confirm_content($fid) {
  global $base_url, $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);

  $query = db_select('custom_sales_delivery', 'sd');
  $query->condition('sd.fid', $fid, '=');
  $query->condition('sd.distributor_id', $distributor_id, '=');
  $query->fields('sd');
  $result = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

  // Count of result
  $count = count($result);

  $total_bill_value = 0;
  $error_flag = FALSE;
  foreach ($result as $key => $val) {
    $total_bill_value = $total_bill_value + $val['sales_amt'];
    if (!empty($val['error_message']) || $val['error_message'] != NULL) {
      $error_flag = TRUE;
      drupal_set_message($val['error_message'], 'error');
    }
  }

  $header = array('Upload Date & Time', 'Total Lines Uploaded', 'Sum Of Total Return Amt', 'Action');

  $rows['uploaded_date'] = date("Y-m-d h:i:s", $result[0]['created']);

  $rows['total_line_uploaded'] = $count;
  $rows['Sum_of_bill_value'] = $total_bill_value;
  $discard_link = l(t('Discard'), 'sales-delivery-data-discard/' . $fid);

  $action_link = l(t('Confirm'), 'sales-delivery-data-confirm/create_content/' . $fid);

  if ($error_flag) {
    $rows['action'] = $discard_link;
  }
  else {
    $rows['action'] = $action_link . '/' . $discard_link;
  }

  $build['pager_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => array($rows),
    '#empty' => t('There are no date formats found.'),
  );

  return $build;
}

/**
 * Menu callback function for sales-confirm/create_content
 * Called when confirm is clicked
 * @param int $id
 */
function change_feeds_import_sales_delivery_create_confirm_content($fid) {
  global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);
  // select data from stage table
  $sales_data_result = db_select('custom_sales_delivery', 'sd')
          ->condition('sd.fid', $fid, '=')
          ->condition('sd.distributor_id', $distributor_id, '=')
          ->fields('sd')
          ->execute()->fetchAll(PDO::FETCH_ASSOC);

  $operations = array();
  $data_chunk = array_chunk($sales_data_result, 1000);
  foreach ($data_chunk as $data) {
    $operations[] = array('change_feeds_import_sales_delivery_node', array($data, $distributor_id));
  }

  $batch = array(
    'operations' => $operations,
    'finished' => 'change_feeds_import_sales_delivery_finished',
    'title' => t('Creating Sales Delivery activity node'),
    'init_message' => t('sales_delivery Data Batch Creation is starting.'),
    'progress_message' => t('Processed @current out of @total.'),
    'error_message' => t('sales_delivery Data Batch has encountered an error.'),
    'file' => drupal_get_path('module', 'change_feeds_import') . '/form/sales_delivery_data.inc'
  );
  batch_set($batch);
  batch_process('outstanding-activity/import');
}

/**
 * 
 * @param type $type
 * @return type
 */
function get_node_titles_by_type($type) {
  $query1 = db_select('node', 'n');
  $query1->condition('n.type', $type);
  $query1->fields('n', array('title','nid'));
  $query1->addTag('node_uid');
  $nids = $query1->execute()->fetchAllKeyed(0,1);
  return $nids;
}

function change_feeds_import_sales_delivery_node($sales_data_result, $distributor_id, &$context) {
  if (!empty($sales_data_result)) {
    $count = 0;
    $node_titles = get_node_titles_by_type('sales_register_data');
    foreach ($sales_data_result as $key => $val) {

      if (!empty($val['bill_no']) && empty($val['error_message'])) {

        // check to update node
        if (!array_key_exists($val['bill_no'], $node_titles)) {

          // we dont have to create new node here
        }
        else {
          $nid = $node_titles[$val['bill_no']];
          
          $node = node_load($nid);
          if (!empty($node)) {
         // $node->field_impact_delivery_date[LANGUAGE_NONE][0]['value'] = $val['impact_del_date'];
          $node->field_sales_return_amt[LANGUAGE_NONE][0]['value'] = $val['sales_amt'];
        //  $node->field_delivery_status[LANGUAGE_NONE][0]['tid'] = $val['partial_full'];
         // $node->field_deliver_reason[LANGUAGE_NONE][0]['tid'] = $val['reason'];
          if (empty($val['delivery_pending'])) {
            $node->field_delivery_pending = array();
          }
          else {
            $node->field_delivery_pending[LANGUAGE_NONE][0]['value'] = $val['delivery_pending'];
          }
          node_save($node);
          
          $count++;
          }
        }
      }
      $fid = $val['fid'];
    }
  }
  $context['results']['fid'] = $fid;
  $context['results']['count'] = $count;
}

/**
 * finish function for batch process
 * 
 * @param boolean $success
 * @param array $results
 * @param array $operations
 */
function change_feeds_import_sales_delivery_finished($success, $results, $operations) {
  if ($success) {
    global $user;
    $user_id = $user->uid;
    $distributor_id = change_feeds_import_get_distributor_id($user_id);

    // delete data from stage_billing_data.
    $fid = $results['fid'];
    $count = $results['count'];

    if (!empty($fid)) {
      db_delete('custom_sales_delivery')
          ->condition('fid', $fid, '=')
          ->condition('distributor_id', $distributor_id, '=')
          ->execute();
    }

    drupal_set_message(t("$count node was created"));
    drupal_goto("sales-delivery-data/import");
  }
  else {
    drupal_set_message('There is Error Occured in batch process');
  }
}

/**
 * Menu callback for sales-discard
 * 
 * @param int $fid
 */
function change_feeds_import_sales_delivery_discard_data($fid) {
  if (!empty($fid)) {
    global $user;
    $user_id = $user->uid;
    $distributor_id = change_feeds_import_get_distributor_id($user_id);
    if (!empty($distributor_id)) {
      db_delete('custom_sales_delivery')
          ->condition('fid', $fid, '=')
          ->condition('distributor_id', $distributor_id, '=')
          ->execute();

      // Redirect to import page
      drupal_goto("sales-delivery-data/import");
    }
  }
  drupal_set_message(t('Something went wrong'), 'error');
}
