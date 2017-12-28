<?php

// upload form for billing data

function sku_ean_mrp_feeds_import_form($form, $form_state) {
  $form['file'] = array(
    '#type' => 'file',
    '#title' => t('Choose a CSV file for SKU7 EAN and MRP data upload'),
  );
  $form['template_download'] = array(
    '#type' => 'markup',
    '#prefix' => '<div id="template_wrapper">',
    '#suffix' => '</div>',
    '#markup' => l("Download Template", drupal_get_path('module', 'leanbox_sku_cp') . '/sku_ean_mrp.csv'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Upload'),
  );
  return $form;
}

function sku_ean_mrp_feeds_import_form_validate($form, &$form_state) {
  $file = file_save_upload('file', array(
    'file_validate_extensions' => array('csv'),
  ));

  $header_array_from_xsd = array(
    'SKU7_Code',
    'EAN',
    'MRP',
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

function sku_ean_mrp_feeds_import_form_submit($form, &$form_state) {
  global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);
  $rs_code = dispatch_planning_get_rscode_using_uid($distributor_id);
  $file = $form_state['values']['file'];
  $filepath = $file->uri;

  $datas = array();
  // Making data as associative array with header.
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
  $data_chunk = array_chunk($datas, 1);
  foreach ($data_chunk as $data) {
    $operations[] = array('sku_ean_mrp_upload_batch_process', array($data, $rs_code));
  }
  $batch = array(
    'title' => t('Reading SKU7 EAN and MRP Csv'),
    'operations' => $operations,
    'init_message' => t("Please wait..."),
    'progress_message' => t('Processed @current out of @total.'),
    'error_message' => t('Upload data processing has encountered an error.'),
    'finished' => 'sku_ean_mrp_csv_finished',
    'file' => drupal_get_path('module', 'leanbox_sku_cp') . '/form/sku_ean_mrp.inc',
  );
  batch_set($batch);
}

function sku_ean_mrp_upload_batch_process($data, $rs_code, &$context) {
  if (!isset($context['results']['sku_code_result'])) {
    // check sku7 code is present or not.
    $context['results']['sku_code_result'] = check_sku7_code_exist_or_not();
    $context['results']['error_flag'] = 0;
  }

  foreach ($data as $value) {
    $sku7_code = $value['SKU7_Code'];
    $ean = $value['EAN'];

    if (!array_key_exists($sku7_code, $context['results']['sku_code_result'])) {
      $context['results']['error_flag'] = 1;
      $error_text = "\n" . $sku7_code . " SKU7_CODE is not present in SKU7_CODE Master.";
      drupal_set_message($error_text, 'error');
    }
    /*$exist_sku_of_ean = check_ean_combination_with_other_sku($ean, $sku7_code, $rs_code);
    if ($exist_sku_of_ean) {
      $context['results']['error_flag'] = 1;
      $error_text = "\n" . $ean . "is already present with SKU7_CODE " . $exist_sku_of_ean . " and you are uploading with SKU7_CODE $sku7_code";
      drupal_set_message($error_text, 'error');
    }*/
  }

  if ($context['results']['error_flag'] == 0) {
    // insert into custom sales table
    insert_data_in_sku_ean_mrp_table($data, $rs_code);
  }
}

function sku_ean_mrp_csv_finished($success, $results, $operations) {
  if ($success) {
    if ($results['error_flag'] == 1) {
      drupal_set_message('Please fix the above issue and upload again!!', 'error');
    }
    else {
      drupal_set_message('SKU EAN and MRP uploaded successfully');
    }

    $_GET['destination'] = "sku-ean-mrp/import";
  }
  else {
    drupal_set_message('There is Error Occured in batch process');
  }
}

//check ean combination with sku except passed sku
function check_ean_combination_with_other_sku($ean, $sku7_code, $rs_code) {
  $query = db_select('sku_ean_mrp', 'sem');
  $query->condition('sem.sku', $sku7_code, '!=');
  $query->condition('sem.ean', $ean, '=');
  $query->condition('sem.rs_code', $rs_code, '=');
  $query->fields('sem', array('sku'));
  $exist_sku_of_ean = $query->execute()->fetchField();
  return $exist_sku_of_ean;
}

function insert_data_in_sku_ean_mrp_table($data, $rs_code) {
  foreach ($data as $value) {
    $sku7_code = $value['SKU7_Code'];
    $ean = $value['EAN'];
    $mrp = $value['MRP'];

    db_merge('sku_ean_mrp')
      ->key(array('sku' => $sku7_code,'rs_code' => $rs_code))
      ->fields(array(
        'sku' => $sku7_code,
        'ean' => $ean,
        'mrp' => $mrp,
        'rs_code' => $rs_code,
        'flag' => 0, // enable for api
        'created_time' => REQUEST_TIME
      ))
      ->execute();
  }
}
