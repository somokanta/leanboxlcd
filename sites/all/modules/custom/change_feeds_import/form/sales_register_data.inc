<?php

// upload form for billing data

function change_feeds_import_sales_register_form($form, $form_state) {
  global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);

  // check distributor incompleted upload
  $query = db_select('custom_sales_data', 'sd');
  $query->condition('sd.distributor_id', $distributor_id, '=');
  $query->fields('sd', array('fid'));
  $fid = $query->execute()->fetchField();

  if (!empty($fid)) {
    drupal_goto("sales-confirm/$fid");
  }

  $form['file'] = array(
    '#type' => 'file',
    '#title' => t('Choose a CSV file for sales data upload'),
  );
  $form['template_download'] = array(
    '#type' => 'markup',
    '#prefix' => '<div id="template_wrapper">',
    '#suffix' => '</div>',
    '#markup' => l("Download Template", drupal_get_path('module', 'change_feeds_import') . '/sales_register_template.csv'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Upload'),
  );



  return $form;
}

function change_feeds_import_sales_register_form_validate($form, &$form_state) {
  $file = file_save_upload('file', array(
    'file_validate_extensions' => array('csv'),
  ));

  $header_array_from_xsd = array(
    'BILL_NUMBER',
    'BILL_DATE',
    'PARTY_CODE',
    'BILL_VALUE',
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

function change_feeds_import_sales_register_form_submit($form, &$form_state) {
  global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);


  $query = db_select('node', 'n');
  $query->condition('n.type', 'van_stop_master');
  $query->condition('n.title', $distributor_id);
  $query->join('field_data_field_no_of_days_for_sales_reg', 'no_da', 'no_da.entity_id = n.nid');
  $query->fields('no_da', array('field_no_of_days_for_sales_reg_value'));
  $query->addTag('node_uid');
  $days = $query->execute()->fetchAll();
  $no_of_days = $days[0]->field_no_of_days_for_sales_reg_value;

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
    $operations[] = array('change_feeds_import_sales_register_batch_fix_process', array($data, $distributor_id, $file_name, $fid, $no_of_days));
  }
  $batch = array(
    'title' => t('Reading sales Data Csv'),
    'operations' => $operations,
    'init_message' => t("Please wait..."),
    'progress_message' => t('Processed @current out of @total.'),
    'error_message' => t('Upload data processing has encountered an error.'),
    'finished' => 'sales_register_data_csv_finished',
    'file' => drupal_get_path('module', 'change_feeds_import') . '/form/sales_register_data.inc',
  );
  batch_set($batch);
}

function change_feeds_import_sales_register_batch_fix_process($data, $distributor_id, $file_name, $fid, $no_of_days, &$context) {
  if (empty($distributor_id)) {
    drupal_set_message(t('Please check your Distributor code'), 'error');
  }
  require_once drupal_get_path('module', 'change_feeds_import') . '/form/upload_billing_data.inc';

  $value['error_message'] = '';
  if (!isset($context['results']['unique_combination'])) {
    $context['results']['unique_combination'] = array();
  }

  foreach ($data as $key => $value) {

    foreach ($value as $k => $v) {
      $error_text = '';
      $v = trim($v);
      if ($k == 'BILL_VALUE') {
        if (  (empty($v)  && ($v != '0') ) || $v < '0') {
          $value['error_message'] .= $k . " can not be empty or  less than 0(Zero) for bill no " . $value['BILL_NUMBER'] . ".";
        }
      }
      else {
        if (empty($v)) {
          $value['error_message'] .= $k . " can not be empty for bill no " . $value['BILL_NUMBER'] . ".";
        }
      }
    }

    if (isset($value['BILL_DATE'])) {
     // $value['BILL_DATE'] = str_replace('/', '-', $value['BILL_DATE']);
     // $date_arr = explode('-', $value['BILL_DATE']);
     // $converted_date = $date_arr[2] . '-' . $date_arr[1] . '-' . $date_arr[0];
      $value['BILL_DATE'] = strtotime($value['BILL_DATE']);
      if (empty($value['BILL_DATE'])) {
        $value['error_message'] .= " Enter correct Bill Date for Bill No " . $value['BILL_NUMBER'] . ".";
      }
      else {

        $check_date = $value['BILL_DATE'];
        $from = strtotime(date('Y-m-d', strtotime("-$no_of_days days")));
        $to = strtotime(date('Y-m-d'));
        if (($check_date >= $from) && ($check_date <= $to)) {
          // do nothing
        }
        else {
          $value['error_message'] .= "Enter correct Bill Date for Bill No " . $value['BILL_NUMBER'] . ".";
        }
      }
    }
    if (isset($value['BILL_NUMBER'])) {
      $unique_comb = $value['BILL_NUMBER'];
      if (in_array($unique_comb, $context['results']['unique_combination'])) {
        $value['error_message'] .= "BILL_NUMBER " . $value['BILL_NUMBER'] . " is Repeated.  ";
      }
      else {
        $context['results']['unique_combination'][] = $unique_comb;
      }
    }
//      $value['BILL_NUMBER'] = trim($value['BILL_NUMBER']);
//      // Check if bill no already created as a node in content type sales_register_data
//      $title1 = check_node_title_exists($value['BILL_NUMBER'], 'sales_register_data');
//      if (!empty($title1)) {
//       
//        $title2 = check_node_title_exists($value['BILL_NUMBER'], 'sales_register_data', TRUE);
//        if (!empty($title2)) {
//          // will have to ovveride the node in this case
//        }
//        else {
//          $value['error_message'] .= " Bill no " . $value['BILL_NUMBER'] . ' already exist in our system.';
//        }
//      }
//    }
    if (isset($value['BILL_VALUE'])) {

      // Check if bill no already created as a node in content type sales_register_data
      if (!is_numeric($value['BILL_VALUE'])) {
        $value['error_message'] .= " Bill no " . $value['BILL_NUMBER'] . ' should have numeric bill value.';
      }
    }
    if (isset($value['PARTY_CODE'])) {
      $value['PARTY_CODE'] = trim($value['PARTY_CODE']);
      // Check if PARTY_CODE doesnt exist in outlet master
      $party_code = check_node_title_exists($value['PARTY_CODE'], 'outlet_master');
      if (empty($party_code)) {
        $value['error_message'] .= " Party code " . $value['PARTY_CODE'] . ' does not exist in our system.';
      }
    }
    if (empty($distributor_id)) {
      $value['error_message'] .= " Please check your Distributor code";
    }

    // insert into custom sales table
    change_feeds_import_insert_data_in_custom_sales_table($value, $fid, $distributor_id);

    $context['results']['fid'] = $fid;
  }
}

function sales_register_data_csv_finished($success, $results, $operations) {
  if ($success) {
    $fid = $results['fid'];
    //drupal_goto("sales-confirm/" . $fid);
    $_GET['destination'] = "sales-confirm/" . $fid;
  }
  else {
    drupal_set_message('There is Error Occured in batch process');
  }
}

function change_feeds_import_insert_data_in_custom_sales_table($value, $fid, $distributor_id) {

  $sales_data = db_insert('custom_sales_data') // Table name no longer needs {}
      ->fields(array(
        'bill_no' => $value['BILL_NUMBER'],
        'date' => $value['BILL_DATE'],
        'bill_value' => $value['BILL_VALUE'],
        'created' => time(),
        'error_message' => $value['error_message'],
        'fid' => $fid,
        'party_code' => $value['PARTY_CODE'],
        'distributor_id' => $distributor_id,
      ))
      ->execute();
}

// show table after file is uploaded

function change_feeds_import_sales_confirm_content($fid) {
  global $base_url, $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);

  $query = db_select('custom_sales_data', 'sd');
  $query->condition('sd.fid', $fid, '=');
  $query->condition('sd.distributor_id', $distributor_id, '=');
  $query->fields('sd');
  $result = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

  // Count of result
  $count = count($result);

  $total_bill_value = 0;
  $error_flag = FALSE;
  foreach ($result as $key => $val) {
    $total_bill_value = $total_bill_value + $val['bill_value'];
    if (!empty($val['error_message']) || $val['error_message'] != NULL) {
      $error_flag = TRUE;
      drupal_set_message($val['error_message'], 'error');
    }
  }

  $header = array('Upload Date & Time', 'Total Lines Uploaded', 'Sum Of Total Bill Amt', 'Action');

  $rows['uploaded_date'] = date("Y-m-d h:i:s", $result[0]['created']);

  $rows['total_line_uploaded'] = $count;
  $rows['Sum_of_bill_value'] = $total_bill_value;
  $discard_link = l(t('Discard'), 'sales-discard/' . $fid);

  $action_link = l(t('Confirm'), 'sales-confirm/create_content/' . $fid);

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
function change_feeds_import_sales_create_confirm_content($fid) {
  global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);
  // select data from stage table
  $sales_data_result = db_select('custom_sales_data', 'sd')
          ->condition('sd.fid', $fid, '=')
          ->condition('sd.distributor_id', $distributor_id, '=')
          ->fields('sd')
          ->execute()->fetchAll(PDO::FETCH_ASSOC);

  $operations = array();
  $data_chunk = array_chunk($sales_data_result, 100);
  foreach ($data_chunk as $data) {
    $operations[] = array('change_feeds_import_create_sales_register_node', array($data, $distributor_id));
  }

  $batch = array(
    'operations' => $operations,
    'finished' => 'change_feeds_import_sales_reg_finished',
    'title' => t('Creating Sales register Data'),
    'init_message' => t('Sales Data Batch Creation is starting.'),
    'progress_message' => t('Processed @current out of @total.'),
    'error_message' => t('Sales Data Batch has encountered an error.'),
    'file' => drupal_get_path('module', 'change_feeds_import') . '/form/sales_register_data.inc'
  );
  batch_set($batch);
  batch_process('sales-register/import');
}

function change_feeds_import_create_sales_register_node($sales_data_result, $distributor_id, &$context) {
  if (!empty($sales_data_result)) {
    $billing_error_validate = FALSE;
    $count = 0;
    foreach ($sales_data_result as $key => $val) {

      if (!empty($val['party_code']) && !empty($val['bill_no']) && !empty($val['date']) && empty($val['error_message'])) {

        require_once drupal_get_path('module', 'change_feeds_import') . '/form/upload_billing_data.inc';
        // check to update node

        $nid = check_node_title_exists($val['bill_no'], 'sales_register_data');
        if (empty($nid)) {
          $billing_error_validate = TRUE;
          $node = new stdClass();
          $node->title = $val['bill_no'];
          $node->type = "sales_register_data";
          node_object_prepare($node); // Sets some defaults. Invokes hook_prepare() and hook_node_prepare().
          $node->language = LANGUAGE_NONE; // Or e.g. 'en' if locale is enabled
          $node->uid = $val['distributor_id'];
          $node->status = 1; //(1 or 0): published or not
          $node->comment = 0; // 0 = comments disabled, 1 = read only, 2 = read/write

          $node->field_sr_new_bill_date[LANGUAGE_NONE][0]['value'] = $val['date'];

          if (isset($context['results']['dates'][$val['date']])) {
            $pdd = $context['results']['dates'][$val['date']];
          }
          else {
            $pdd = get_promised_delivery_date($val['date']);
            $context['results']['dates'][$val['date']] = $pdd;
          }

          $node->field_promised_date[LANGUAGE_NONE][0]['value'] = $pdd;

          $node->field_bill_value[LANGUAGE_NONE][0]['value'] = $val['bill_value'];
          $node->field_sales_party_code[LANGUAGE_NONE][0]['value'] = $val['party_code'];
          $node->field_sales_updated_date[LANGUAGE_NONE][0]['value'] = time();
          node_save($node);
        }
        else {
          $node = node_load($nid);

          //if ($node->field_sr_bill_date[LANGUAGE_NONE][0]['value'] != $val['date']) {
          $node->field_sr_new_bill_date[LANGUAGE_NONE][0]['value'] = $val['date'];

          if (isset($context['results']['dates'][$val['date']])) {
            $pdd = $context['results']['dates'][$val['date']];
          }
          else {
            $pdd = get_promised_delivery_date($val['date']);
            $context['results']['dates'][$val['date']] = $pdd;
          }

          $node->field_promised_date[LANGUAGE_NONE][0]['value'] = $pdd;
          //}

          $node->field_bill_value[LANGUAGE_NONE][0]['value'] = $val['bill_value'];
          $node->field_sales_party_code[LANGUAGE_NONE][0]['value'] = $val['party_code'];
          $node->field_sales_updated_date[LANGUAGE_NONE][0]['value'] = time();
          node_save($node);
        }
        $count++;
      }
      $fid = $val['fid'];
    }
    if ($billing_error_validate) {
      // Update the status of error billing table to validate
      change_feeds_import_change_status_of_error_billing_table();
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
function change_feeds_import_sales_reg_finished($success, $results, $operations) {
  if ($success) {
    global $user;
    $user_id = $user->uid;
    $distributor_id = change_feeds_import_get_distributor_id($user_id);

    // delete data from stage_billing_data.
    $fid = $results['fid'];
    $count = $results['count'];

    if (!empty($fid)) {
      db_delete('custom_sales_data')
          ->condition('fid', $fid, '=')
          ->condition('distributor_id', $distributor_id, '=')
          ->execute();
    }

    drupal_set_message(t("$count node was created"));
    //drupal_goto("sales-register/import");
    $_GET['destination'] = "sales-register/import";
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
function change_feeds_import_sales_discard_data($fid) {
  if (!empty($fid)) {
    global $user;
    $user_id = $user->uid;
    $distributor_id = change_feeds_import_get_distributor_id($user_id);
    if (!empty($distributor_id)) {
      db_delete('custom_sales_data')
          ->condition('fid', $fid, '=')
          ->condition('distributor_id', $distributor_id, '=')
          ->execute();

      // Redirect to import page
      drupal_goto("sales-register/import");
    }
  }
  drupal_set_message(t('Something went wrong'), 'error');
}

function get_promised_delivery_date($date) {
  global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);
  
  $query = db_select('node', 'n');
  $query->condition('n.uid', $distributor_id);
  $query->condition('n.type', 'pdd_master');
  $query->fields('n', array('nid'));
  $pdd_value_result = $query->execute()->fetchField();
  
  $pdd_data = node_load($pdd_value_result);
  $pdd_value = $pdd_data->field_pdd[LANGUAGE_NONE][0]['value'];
  $weekly = $pdd_data->field_day[LANGUAGE_NONE];
  foreach ($weekly as $key => $val) {
    $weeklyoff[] = $val['value'];
  }

  $query = db_select('node', 'n');
  $query->condition('n.uid', $distributor_id);
  $query->condition('n.type', 'holiday_master');
  $query->leftjoin('field_data_field_holiday', 'hday', 'hday.entity_id = n.nid');
  $query->addExpression("DATE_FORMAT(CONVERT_TZ(FROM_UNIXTIME(hday.field_holiday_value),'+00:00','+05:30'), '%Y-%m-%d')", 'hdate');
  $holiday_result = $query->execute()->fetchCol();

  $billing_date_stamp = $date;
  $pdd = holiday_get_working_day($billing_date_stamp, $pdd_value, $holiday_result, $weeklyoff);
  
  return strtotime($pdd);
}
