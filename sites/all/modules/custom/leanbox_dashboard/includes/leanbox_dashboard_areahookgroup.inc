<?php

// upload form for billing data

function leanbox_dashboard_area_hook_group_form($form, $form_state) {
  global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);

  // check distributor incompleted upload
  $query = db_select('custom_area_hook_group', 'sd');
  $query->condition('sd.distributor_id', $distributor_id, '=');
  $query->fields('sd', array('fid'));
  $fid = $query->execute()->fetchField();

  if (!empty($fid)) {
    drupal_goto("area-hook-group-confirm/$fid");
  }

  $form['file'] = array(
    '#type' => 'file',
    '#title' => t('Choose a CSV file for Area Hook Group upload'),
  );
  $form['template_download'] = array(
    '#type' => 'markup',
    '#prefix' => '<div id="template_wrapper">',
    '#suffix' => '</div>',
    '#markup' => l(t('Download Report'), 'area-hook-grouping-download'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Upload'),
  );



  return $form;
}

function leanbox_dashboard_area_hook_group_form_validate($form, &$form_state) {
  $file = file_save_upload('file', array(
    'file_validate_extensions' => array('csv'),
  ));

  $header_array_from_xsd = array(
    'AREA', 'monday', 'tuesday', 'wednesday', 'thursday', 'friday', 'saturday', 'sunday',
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
        // form_set_error('file_upload', t($message));
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

function leanbox_dashboard_area_hook_group_form_submit($form, &$form_state) {
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
    $operations[] = array('leanbox_dashboard_area_hook_group_batch_fix_process', array($data, $distributor_id, $file_name, $fid));
  }
  $batch = array(
    'title' => t('Reading Area Hook Group Csv'),
    'operations' => $operations,
    'init_message' => t("Please wait..."),
    'progress_message' => t('Processed @current out of @total.'),
    'error_message' => t('Upload data processing has encountered an error.'),
    'finished' => 'leanbox_dashboard_area_hook_group_csv_finished',
    'file' => drupal_get_path('module', 'leanbox_dashboard') . '/includes/leanbox_dashboard_areahookgroup.inc',
  );
  batch_set($batch);
}

function leanbox_dashboard_area_hook_group_batch_fix_process($data, $distributor_id, $file_name, $fid, &$context) {
  if (empty($distributor_id)) {
    drupal_set_message(t('Please check your Distributor code'), 'error');
  }
  require_once drupal_get_path('module', 'change_feeds_import') . '/form/upload_billing_data.inc';

  foreach ($data as $key => $value) {
    $data1 = array();


    foreach ($value as $k => $v) {

      $area = $value['AREA'];
      $data1['nid'] = 0;
      $data1['group'] = 0;
      $data1['error_message'] = '';

      $v = trim($v);
      if (empty($v)) {
        $data1['error_message'] .= "Group can not be empty for combination of $k and $area.";
      }
      else {
        if ($k == 'AREA') {
          continue;
        }
        // Logic for checking same grp not repeated for other day
//        if (!isset($context[$area][$v])) {
//          $context[$area][$v] = $v;
//        }
//        else {
//          $data1['error_message'] .= "Group $v is Repeated for combination of $k and $area.";
//        }

        $sales_day_tid = leanbox_check_taxonomy_exists_by_name($k, 'Outlet days');

        $result = leanbox_dashboard_get_areahookgroup_entity($area, $sales_day_tid);
        if (!empty($result)) {
          $nid = $result['nid'];
          $group = $result['field_area_hook_group_value'];
          if ($group == $v) {
            continue;
          }
          else {
            $data1['nid'] = $nid;
            $data1['group'] = $v;
          }
        }
        else {
          $data1['error_message'] .= "No Area Hook Master available for combination of $k and $area.";
        }
      }
// insert into custom table 
      change_feeds_import_insert_data_in_custom_area_hook_group_master_table($data1, $fid, $distributor_id);
    }

    $context['results']['fid'] = $fid;
  }
}

function leanbox_dashboard_area_hook_group_csv_finished($success, $results, $operations) {
  if ($success) {
    $fid = $results['fid'];
    //drupal_goto("area-hook-group-confirm/" . $fid);
    $_GET['destination'] = "area-hook-group-confirm/" . $fid;
  }
  else {
    drupal_set_message('There is Error Occured in batch process');
  }
}

function change_feeds_import_insert_data_in_custom_area_hook_group_master_table($value, $fid, $distributor_id) {
  $sales_data = db_insert('custom_area_hook_group') // Table name no longer needs {}
      ->fields(array(
        'grp' => $value['group'],
        'area_hook_nid' => $value['nid'],
        'created' => time(),
        'error_message' => $value['error_message'],
        'fid' => $fid,
        'distributor_id' => $distributor_id,
      ))
      ->execute();
}

// show table after file is uploaded

function leanbox_dashboard_area_hook_group_confirm_content($fid) {
  global $base_url, $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);

  $query = db_select('custom_area_hook_group', 'sd');
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
  $discard_link = l(t('Discard'), 'area-hook-group-discard/' . $fid);

  $action_link = l(t('Confirm'), 'area-hook-group-confirm/create_content/' . $fid);

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
function leanbox_dashboard_area_hook_group_create_confirm_content($fid) {
  global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);
  // select data from stage table
  $sales_data_result = db_select('custom_area_hook_group', 'sd')
          ->condition('sd.fid', $fid, '=')
          ->condition('sd.distributor_id', $distributor_id, '=')
          ->fields('sd')
          ->execute()->fetchAll(PDO::FETCH_ASSOC);

  $operations = array();
  $data_chunk = array_chunk($sales_data_result, 100);
  foreach ($data_chunk as $data) {
    $operations[] = array('leanbox_dashboard_area_hook_group_node', array($data, $distributor_id));
  }

  $batch = array(
    'operations' => $operations,
    'finished' => 'leanbox_dashboard_area_hook_group_finished',
    'title' => t('Updating outlet area hook node'),
    'init_message' => t('outlet area hook Batch Creation is starting.'),
    'progress_message' => t('Processed @current out of @total.'),
    'error_message' => t('outlet area hook Batch has encountered an error.'),
    'file' => drupal_get_path('module', 'leanbox_dashboard') . '/includes/leanbox_dashboard_areahookgroup.inc'
  );
  batch_set($batch);
  batch_process('area-hook-group/import');
}

function leanbox_dashboard_area_hook_group_node($sales_data_result, $distributor_id, &$context) {
  if (!empty($sales_data_result)) {
    $count = 0;
    foreach ($sales_data_result as $key => $val) {

      $nid = $val['area_hook_nid'];
      if (!empty($nid) && empty($val['error_message'])) {

        // check to update node

        if (empty($nid)) {

          // we dont have to create new node here
        }
        else {
          $node = node_load($nid);
          $node->field_area_hook_group['und'][0]['value'] = $val['grp'];
          field_attach_update('node', $node);
          entity_get_controller('node')->resetCache(array($nid));
        }
        $count++;
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
function leanbox_dashboard_area_hook_group_finished($success, $results, $operations) {
  if ($success) {
    global $user;
    $user_id = $user->uid;
    $distributor_id = change_feeds_import_get_distributor_id($user_id);

    // delete data from stage_billing_data.
    $fid = $results['fid'];
    $count = $results['count'];

    if (!empty($fid)) {
      db_delete('custom_area_hook_group')
          ->condition('fid', $fid, '=')
          ->condition('distributor_id', $distributor_id, '=')
          ->execute();
    }

    drupal_set_message(t("$count node was updated"));
    //drupal_goto("area-hook-group/import");
    $_GET['destination'] = "area-hook-group/import";
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
function leanbox_dashboard_area_hook_group_discard_data($fid) {
  if (!empty($fid)) {
    global $user;
    $user_id = $user->uid;
    $distributor_id = change_feeds_import_get_distributor_id($user_id);
    if (!empty($distributor_id)) {
      db_delete('custom_area_hook_group')
          ->condition('fid', $fid, '=')
          ->condition('distributor_id', $distributor_id, '=')
          ->execute();

      // Redirect to import page
      drupal_goto("area-hook-group/import");
    }
  }
  drupal_set_message(t('Something went wrong'), 'error');
}
