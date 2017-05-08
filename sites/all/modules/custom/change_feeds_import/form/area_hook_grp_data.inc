<?php

// upload form for billing data

function change_feeds_import_area_hook_grp_form($form, $form_state) {
  global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);

  // check distributor incompleted upload
//  $query = db_select('area_hook_grp_master', 'dva');
//  $query->condition('dva.distributor_id', $distributor_id, '=');
//  $query->fields('dva', array('fid'));
//  $fid = $query->execute()->fetchField();

  if (!empty($fid)) {
    drupal_goto("day-van-area-confirm/$fid");
  }

  $form['file'] = array(
    '#type' => 'file',
    '#title' => t('Choose a CSV file for sales data upload'),
  );
  $form['template_download'] = array(
    '#type' => 'markup',
    '#prefix' => '<div id="template_wrapper">',
    '#suffix' => '</div>',
    '#markup' => l("Download Template", drupal_get_path('module', 'change_feeds_import') . '/area_hook_grp_master_template.csv'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Upload'),
  );

  return $form;
}

function change_feeds_import_area_hook_grp_form_validate($form, &$form_state) {
  $file = file_save_upload('file', array(
    'file_validate_extensions' => array('csv'),
  ));

  $header_array_from_xsd = array(
    'Area Hook Group Name',
    'Area Hook Group Id',
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

function change_feeds_import_area_hook_grp_form_submit($form, &$form_state) {
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
    $operations[] = array('change_feeds_import_area_hook_grp_batch_fix_process', array($data, $distributor_id, $file_name, $fid));
  }
  $batch = array(
    'title' => t('Reading Data'),
    'operations' => $operations,
    'init_message' => t("Please wait..."),
    'progress_message' => t('Processed @current out of @total.'),
    'error_message' => t('Upload data processing has encountered an error.'),
    'finished' => 'area_hook_grp_data_csv_finished',
    'file' => drupal_get_path('module', 'change_feeds_import') . '/form/area_hook_grp_data.inc',
  );
  batch_set($batch);
}

function change_feeds_import_area_hook_grp_batch_fix_process($data, $distributor_id, $file_name, $fid, &$context) {
  if (!isset($context['results']['last_id'])) {
    $context['results']['last_id'] = "set";
  }
  // require_once drupal_get_path('module', 'change_feeds_import') . '/form/upload_billing_data.inc';

  $value['error_message'] = '';
  foreach ($data as $key => $value) {
    $area_name = $value['Area Hook Group Name'];
    $area_id = $value['Area Hook Group Id'];
    
    
    foreach ($value as $k => $v) {
      $v = trim($v);
      if (empty($v) && $v !== '0') {
        $flag = 1;
        $value['error_message'] .= "$k can not be null.  ";
      }
    }

    $area_array = check_area_id_exists($area_id);
    if (!empty($area_array)) {
      if ($area_id_bool = in_array($area_id, $area_array)) {
        $value['error_message'] .= "$area_id Area Id Already exist in Area Hook Master";
      }
    }
    
     if (isset($area_id_array[$area_id])) {
        $value['error_message'] .= "$area_id Area Id Already exist in File";
      }
      $area_id_array[$area_id] = $area_id;
      
    $check = $area_name . '-' . $area_id;
    if (in_array($check, $context['results']['area_hook_grp_master_result'])) {
      $value['error_message'] .= "Duplicate Record occured for Area Hook Group Name=$area_name and Area Hook Group Id = $area_id";
    }

    $area_id_length = strlen($area_id);
    if ($area_id_length > 4) {
      $value['error_message'] .= "$area_id has to be 4 digit.";
    }
    else {
      if (!preg_match("/^[a-zA-Z0-9]+$/", $area_id)) {
        $value['error_message'] .= "$area_id has to AlphaNumeric Only.";
      }
      
    }

    // insert into custom sales table
    change_feeds_import_insert_data_in_area_hook_grp_data_table($value, $fid, $distributor_id);
    $context['results']['fid'] = $fid;
  }
}

function area_hook_grp_data_csv_finished($success, $results, $operations) {
  if ($success) {
    $fid = $results['fid'];
    drupal_goto("area-hook-grp-confirm/" . $fid);
  }
  else {
    drupal_set_message('There is Error Occured in batch process');
  }
}

function change_feeds_import_insert_data_in_area_hook_grp_data_table($value, $fid, $distributor_id) {
  $sales_data = db_insert('area_hook_grp_master') // Table name no longer needs {}
      ->fields(array(
        'created' => time(),
        'error_message' => $value['error_message'],
        'fid' => $fid,
        'distributor_id' => $distributor_id,
        'area_name' => $value['Area Hook Group Name'],
        'area_id' => $value['Area Hook Group Id'],
      ))
      ->execute();
}

function check_area_hook_grp_master_exist_or_not() {
  $query = db_select('node', 'n');
  $query->fields('n', array('title'));
  $query->condition('n.type', 'area_hook_group_master');
  $query->innerjoin('field_data_field_area_hook_group_id', 'fah', 'fah.entity_id = n.nid');
  $query->fields('fah', array('field_area_hook_group_id_value'));
  $query->addTag('node_uid');
  $result = $query->execute()->fetchAllKeyed(0, 1);
  return $result;
}

// show table after file is uploaded

function change_feeds_import_area_hook_grp_confirm_content($fid) {
  global $base_url, $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);
  $query = db_select('area_hook_grp_master', 'sd');
  $query->condition('sd.fid', $fid, '=');
  $query->condition('sd.distributor_id', $distributor_id, '=');
  $query->fields('sd');
  $result = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
  // Count of result
  $count = count($result);
  $error_flag = FALSE;
  foreach ($result as $key => $val) {
    if (!empty($val['error_message']) || $val['error_message'] != NULL) {
      $error_flag = TRUE;
      drupal_set_message($val['error_message'], 'error');
    }
  }

  $header = array('Upload Date & Time', 'Total Lines Uploaded', 'Action');
  $rows['uploaded_date'] = date("Y-m-d h:i:s", $result[0]['created']);
  $rows['total_line_uploaded'] = $count;
  $discard_link = l(t('Discard'), 'area-hook-grp-discard/' . $fid);
  $action_link = l(t('Confirm'), 'area-hook-grp-confirm/create_content/' . $fid);
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
 * Menu callback for sales-discard
 * 
 * @param int $fid
 */
function change_feeds_import_area_hook_grp_discard_data($fid) {
  if (!empty($fid)) {
    global $user;
    $user_id = $user->uid;
    $distributor_id = change_feeds_import_get_distributor_id($user_id);
    if (!empty($distributor_id)) {
      db_delete('area_hook_grp_master')
          ->condition('fid', $fid, '=')
          ->condition('distributor_id', $distributor_id, '=')
          ->execute();

      // Redirect to import page
      drupal_goto("area-hook-grp/import");
    }
  }
  drupal_set_message(t('Something went wrong'), 'error');
}

/**
 * Menu callback function for sales-confirm/create_content
 * Called when confirm is clicked
 * @param int $id
 */
function change_feeds_import_area_hook_grp_create_confirm_content($fid) {
  global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);
  // select data from stage table
  $area_hook_grp_data_result = db_select('area_hook_grp_master', 'sd')
          ->condition('sd.fid', $fid, '=')
          ->condition('sd.distributor_id', $distributor_id, '=')
          ->fields('sd')
          ->execute()->fetchAll(PDO::FETCH_ASSOC);

  $operations = array();
  $data_chunk = array_chunk($area_hook_grp_data_result, 5);
  foreach ($data_chunk as $data) {
    $operations[] = array('change_feeds_import_create_area_hook_grp_register_node', array($data, $distributor_id));
  }
  $batch = array(
    'operations' => $operations,
    'finished' => 'change_feeds_import_area_hook_grp_finished',
    'title' => t('Creating Day Van Area Data'),
    'init_message' => t('Batch Creation is starting.'),
    'progress_message' => t('Processed @current out of @total.'),
    'error_message' => t('Batch has encountered an error.'),
    'file' => drupal_get_path('module', 'change_feeds_import') . '/form/area_hook_grp_data.inc',
  );
  batch_set($batch);
  batch_process('area-hook-grp/import');
}

function change_feeds_import_create_area_hook_grp_register_node($area_hook_grp_data_result, $distributor_id, &$context) {
  if (!empty($area_hook_grp_data_result)) {
    $created_count = 0;
    $updated_count = 0;
    foreach ($area_hook_grp_data_result as $key => $val) {
      require_once drupal_get_path('module', 'change_feeds_import') . '/form/upload_billing_data.inc';
      // check to update node
      $area_name = $val['area_name'];
      $area_id = $val['area_id'];
      $title = $area_name;
      $nid = check_node_title_exists($title, 'area_hook_group_master');
      if (empty($nid)) {
        $billing_error_validate = TRUE;
        $node = new stdClass();
        $node->title = $title;
        $node->type = "area_hook_group_master";
        node_object_prepare($node); // Sets some defaults. Invokes hook_prepare() and hook_node_prepare().
        $node->language = LANGUAGE_NONE; // Or e.g. 'en' if locale is enabled
        $node->uid = $val['distributor_id'];
        $node->status = 1; //(1 or 0): published or not
        $node->comment = 0; // 0 = comments disabled, 1 = read only, 2 = read/write

        $node->field_area_hook_group_id['und'][0]['value'] = $area_id;
        node_save($node);
        $created_count++;
      }
      else {
        $node = node_load($nid);
        //$node->field_dva_sales_day[LANGUAGE_NONE][0]['value'] = $sales_day;
        //$node->field_dva_van_pl_group[LANGUAGE_NONE][0]['value'] = $van_pl_grp;
        //$node->field_dva_area_hook[LANGUAGE_NONE][0]['value'] = $area_hook;
        $node->field_area_hook_group_id['und'][0]['value'] = $area_id;
        node_save($node);
        $updated_count++;
      }

      $fid = $val['fid'];
    }
  }
  $context['results']['fid'] = $fid;
  $context['results']['created_count'] = $created_count;
  $context['results']['updated_count'] = $updated_count;
}

/**
 * finish function for batch process
 * 
 * @param boolean $success
 * @param array $results
 * @param array $operations
 */
function change_feeds_import_area_hook_grp_finished($success, $results, $operations) {
  if ($success) {
    global $user;
    $user_id = $user->uid;
    $distributor_id = change_feeds_import_get_distributor_id($user_id);

    // delete data from stage_billing_data.
    $fid = $results['fid'];
    $created_count = $results['created_count'];
    $updated_count = $results['updated_count'];

    if (!empty($fid)) {
      db_delete('area_hook_grp_master')
          ->condition('fid', $fid, '=')
          ->condition('distributor_id', $distributor_id, '=')
          ->execute();
    }

    if ($created_count != 0) {
      drupal_set_message(t("$created_count node was Created"));
    }
    if ($updated_count != 0) {
      drupal_set_message(t("$updated_count node was updated"));
    }
  }
  else {
    drupal_set_message('There is Error Occured in batch process');
  }
}
