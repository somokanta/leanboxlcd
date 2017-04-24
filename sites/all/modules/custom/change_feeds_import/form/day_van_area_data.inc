<?php

// upload form for billing data

function change_feeds_import_day_van_area_form($form, $form_state) {
  global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);

  // check distributor incompleted upload
  $query = db_select('day_van_area_data', 'dva');
  $query->condition('dva.distributor_id', $distributor_id, '=');
  $query->fields('dva', array('fid'));
  $fid = $query->execute()->fetchField();

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
    '#markup' => l("Download Template", drupal_get_path('module', 'change_feeds_import') . '/day_van_area_template.csv'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Upload'),
  );

  return $form;
}

function change_feeds_import_day_van_area_form_validate($form, &$form_state) {
  $file = file_save_upload('file', array(
    'file_validate_extensions' => array('csv'),
  ));

  $header_array_from_xsd = array(
    'Sales Day',
    'Van_PL_Group',
    'Area Hook',
    'Area Hook Group Name',
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

function change_feeds_import_day_van_area_form_submit($form, &$form_state) {
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
    $operations[] = array('change_feeds_import_day_van_area_batch_fix_process', array($data, $distributor_id, $file_name, $fid));
  }
  $batch = array(
    'title' => t('Reading Data'),
    'operations' => $operations,
    'init_message' => t("Please wait..."),
    'progress_message' => t('Processed @current out of @total.'),
    'error_message' => t('Upload data processing has encountered an error.'),
    'finished' => 'day_van_area_data_csv_finished',
    'file' => drupal_get_path('module', 'change_feeds_import') . '/form/day_van_area_data.inc',
  );
  batch_set($batch);
}

function change_feeds_import_day_van_area_batch_fix_process($data, $distributor_id, $file_name, $fid, &$context) {
  if (!isset($context['results']['last_id'])) {
    $context['results']['last_id'] = "set";
    $context['results']['area_hook_result'] = check_area_exist_or_not(); // check sku7 code is present or not.
  }
  require_once drupal_get_path('module', 'change_feeds_import') . '/form/upload_billing_data.inc';

  $value['error_message'] = '';
  foreach ($data as $key => $value) {
    $sales_day = $value['Sales Day'];
    $van_pl_group = $value['Van_PL_Group'];
    $area_hook = $value['Area Hook'];
    $area_grp_name = $value['Area Hook Group Name'];

    foreach ($value as $k => $v) {
      $v = trim($v);
      if (empty($v) && $v !== '0') {
        $flag = 1;
        $value['error_message'] .= "$k can not be null.  ";
      }
      else {
        switch ($k) {
          case 'Sales Day':
            $day = strtolower($v);
            $sales_day_tid = leanbox_check_taxonomy_exists_by_name($day, 'Outlet days');
            if (empty($sales_day_tid)) {
              $value['error_message'] .= $v . " Is Not Valid.  ";
            }
            break;

          case 'Van_PL_Group':
            if (!is_numeric($van_pl_group)) {
              $value['error_message'] .= " Van PlGroup " . $value['BILL_NUMBER'] . ' should have numeric bill value.  ';
            }
            break;
          case 'Area Hook':
            if (!array_key_exists($v, $context['results']['area_hook_result'])) {
              $value['error_message'] .= $v . " Area Hook is not present in Area Hook Group Master.  ";
            }
            break;
          default:
            break;
        }
      }
    }

    // insert into custom sales table
    change_feeds_import_insert_data_in_day_van_area_data_table($value, $fid, $distributor_id);
    $context['results']['fid'] = $fid;
  }
}

function day_van_area_data_csv_finished($success, $results, $operations) {
  if ($success) {
    $fid = $results['fid'];
    drupal_goto("day-van-area-confirm/" . $fid);
  }
  else {
    drupal_set_message('There is Error Occured in batch process');
  }
}

function change_feeds_import_insert_data_in_day_van_area_data_table($value, $fid, $distributor_id) {

  $sales_data = db_insert('day_van_area_data') // Table name no longer needs {}
      ->fields(array(
        'created' => time(),
        'error_message' => $value['error_message'],
        'fid' => $fid,
        'distributor_id' => $distributor_id,
        'sales_day' => $value['Sales Day'],
        'van_pl_grp' => $value['Van_PL_Group'],
        'area_hook' => $value['Area Hook'],
        'area_hook_grp' => $value['Area Hook Group Name'],
      ))
      ->execute();
}

function check_area_exist_or_not() {
  $query = db_select('node', 'n');
  $query->fields('n', array('title'));
  $query->condition('n.type', 'area_hook_master');
  $query->addTag('node_uid');
  $result = $query->execute()->fetchAllKeyed(0, 0);
  return $result;
}

// show table after file is uploaded

function change_feeds_import_day_van_area_confirm_content($fid) {
  global $base_url, $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);
  $query = db_select('day_van_area_data', 'sd');
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
  $discard_link = l(t('Discard'), 'day-van-area-discard/' . $fid);
  $action_link = l(t('Confirm'), 'day-van-area-confirm/create_content/' . $fid);
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
function change_feeds_import_day_van_area_discard_data($fid) {
  if (!empty($fid)) {
    global $user;
    $user_id = $user->uid;
    $distributor_id = change_feeds_import_get_distributor_id($user_id);
    if (!empty($distributor_id)) {
      db_delete('day_van_area_data')
          ->condition('fid', $fid, '=')
          ->condition('distributor_id', $distributor_id, '=')
          ->execute();

      // Redirect to import page
      drupal_goto("day-van-area/import");
    }
  }
  drupal_set_message(t('Something went wrong'), 'error');
}

/**
 * Menu callback function for sales-confirm/create_content
 * Called when confirm is clicked
 * @param int $id
 */
function change_feeds_import_day_van_area_create_confirm_content($fid) {
  global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);
  // select data from stage table
  $day_van_area_data_result = db_select('day_van_area_data', 'sd')
          ->condition('sd.fid', $fid, '=')
          ->condition('sd.distributor_id', $distributor_id, '=')
          ->fields('sd')
          ->execute()->fetchAll(PDO::FETCH_ASSOC);

  $operations = array();
  $data_chunk = array_chunk($day_van_area_data_result, 100);
  foreach ($data_chunk as $data) {
    $operations[] = array('change_feeds_import_create_day_van_area_register_node', array($data, $distributor_id));
  }
  $batch = array(
    'operations' => $operations,
    'finished' => 'change_feeds_import_day_van_area_finished',
    'title' => t('Creating Day Van Area Data'),
    'init_message' => t('Batch Creation is starting.'),
    'progress_message' => t('Processed @current out of @total.'),
    'error_message' => t('Batch has encountered an error.'),
    'file' => drupal_get_path('module', 'change_feeds_import') . '/form/day_van_area_data.inc',
  );
  batch_set($batch);
  batch_process('day-van-area/import');
}

function change_feeds_import_create_day_van_area_register_node($day_van_area_data_result, $distributor_id, &$context) {
  if (!empty($day_van_area_data_result)) {
    $created_count = 0;
    $updated_count = 0;
    foreach ($day_van_area_data_result as $key => $val) {
      if (!empty($val['sales_day']) && !empty($val['van_pl_grp']) && !empty($val['area_hook']) && empty($val['error_message'])) {
        require_once drupal_get_path('module', 'change_feeds_import') . '/form/upload_billing_data.inc';
        // check to update node
        $sales_day = $val['sales_day'];
        $van_pl_grp = $val['van_pl_grp'];
        $area_hook = $val['area_hook'];
        $area_hook_grp = $val['area_hook_grp'];
        $title = $sales_day . $van_pl_grp . $area_hook;
        $nid = check_node_title_exists($title, 'day_van_pl_gp_area_hook_grouping');
        if (empty($nid)) {
          $billing_error_validate = TRUE;
          $node = new stdClass();
          $node->title = $title;
          $node->type = "day_van_pl_gp_area_hook_grouping";
          node_object_prepare($node); // Sets some defaults. Invokes hook_prepare() and hook_node_prepare().
          $node->language = LANGUAGE_NONE; // Or e.g. 'en' if locale is enabled
          $node->uid = $val['distributor_id'];
          $node->status = 1; //(1 or 0): published or not
          $node->comment = 0; // 0 = comments disabled, 1 = read only, 2 = read/write

          $node->field_dva_sales_day[LANGUAGE_NONE][0]['value'] = $sales_day;
          $node->field_dva_van_pl_group[LANGUAGE_NONE][0]['value'] = $van_pl_grp;
          $node->field_dva_area_hook[LANGUAGE_NONE][0]['value'] = $area_hook;
          $node->field_dva_area_hook_group_name[LANGUAGE_NONE][0]['value'] = $area_hook_grp;
          node_save($node);
          $created_count++;
        }
        else {
          $node = node_load($nid);
          $node->field_dva_sales_day[LANGUAGE_NONE][0]['value'] = $sales_day;
          $node->field_dva_van_pl_group[LANGUAGE_NONE][0]['value'] = $van_pl_grp;
          $node->field_dva_area_hook[LANGUAGE_NONE][0]['value'] = $area_hook;
          $node->field_dva_area_hook_group_name[LANGUAGE_NONE][0]['value'] = $area_hook_grp;
          node_save($node);
          $updated_count++;
        }
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
function change_feeds_import_day_van_area_finished($success, $results, $operations) {
  if ($success) {
    global $user;
    $user_id = $user->uid;
    $distributor_id = change_feeds_import_get_distributor_id($user_id);

    // delete data from stage_billing_data.
    $fid = $results['fid'];
    $created_count = $results['created_count'];
    $updated_count = $results['updated_count'];

    if (!empty($fid)) {
      db_delete('day_van_area_data')
          ->condition('fid', $fid, '=')
          ->condition('distributor_id', $distributor_id, '=')
          ->execute();
    }

    if ($created_count != 0) {
      drupal_set_message(t("$created_count node was updated"));
    }
    if ($updated_count != 0) {
      drupal_set_message(t("$updated_count node was updated"));
    }
  }
  else {
    drupal_set_message('There is Error Occured in batch process');
  }
}
