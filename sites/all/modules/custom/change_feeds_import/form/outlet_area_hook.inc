<?php

// upload form for billing data

function change_feeds_import_outlet_area_hook_form($form, $form_state) {
  global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);

  // check distributor incompleted upload
  $query = db_select('custom_outlet_area_hook', 'sd');
  $query->condition('sd.distributor_id', $distributor_id, '=');
  $query->fields('sd', array('fid'));
  $fid = $query->execute()->fetchField();

  if (!empty($fid)) {
    drupal_goto("outlet-area-hook-confirm/$fid");
  }

  $form['file'] = array(
    '#type' => 'file',
    '#title' => t('Choose a CSV file for Sales Delivery data upload'),
  );
  $form['template_download'] = array(
    '#type' => 'markup',
    '#prefix' => '<div id="template_wrapper">',
    '#suffix' => '</div>',
    '#markup' => l("Download Template", drupal_get_path('module', 'change_feeds_import') . '/outlet_area_hook_template.csv'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Upload'),
  );



  return $form;
}

function change_feeds_import_outlet_area_hook_form_validate($form, &$form_state) {
  $file = file_save_upload('file', array(
    'file_validate_extensions' => array('csv'),
  ));

  $header_array_from_xsd = array(
    'PARTY CODE', 'AREA HOOK'
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

function change_feeds_import_outlet_area_hook_form_submit($form, &$form_state) {
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
    $operations[] = array('change_feeds_import_outlet_area_hook_batch_fix_process', array($data, $distributor_id, $file_name, $fid));
  }
  $batch = array(
    'title' => t('Reading Outlet Area Hook Csv'),
    'operations' => $operations,
    'init_message' => t("Please wait..."),
    'progress_message' => t('Processed @current out of @total.'),
    'error_message' => t('Upload data processing has encountered an error.'),
    'finished' => 'outlet_area_hook_csv_finished',
    'file' => drupal_get_path('module', 'change_feeds_import') . '/form/outlet_area_hook.inc',
  );
  batch_set($batch);
}

function change_feeds_import_outlet_area_hook_batch_fix_process($data, $distributor_id, $file_name, $fid, &$context) {
  if (empty($distributor_id)) {
    drupal_set_message(t('Please check your Distributor code'), 'error');
  }
  require_once drupal_get_path('module', 'change_feeds_import') . '/form/upload_billing_data.inc';

  $value['error_message'] = '';
  foreach ($data as $key => $value) {

    foreach ($value as $k => $v) {
      $error_text = '';
      $v = trim($v);
      if (empty($v)) {
        $value['error_message'] .= $k . " can not be empty.";
      }
    }


    $nid = '';
    if (isset($value['PARTY CODE'])) {
      $value['PARTY CODE'] = trim($value['PARTY CODE']);
      $nid = check_node_title_exists($value['PARTY CODE'], 'outlet_master');
      if (empty($nid)) {
        $value['error_message'] .= $value['PARTY CODE'] . ' Party code does not exist in Outlet master.';
      }
      else {
        global $user;
        $user_id = $user->uid;
        $distributor_id = change_feeds_import_get_distributor_id($user_id);

        $query = db_select('node', 'n');
        $query->join('field_data_field_hul_code', 'fh', 'fh.entity_id = n.nid');
        $query->condition('n.title', $value['PARTY CODE']);
        $query->condition('n.type', 'outlet_master');
        $query->condition('n.uid', $distributor_id);
        $query->condition('n2.uid', $distributor_id);
        $query->join('node', 'n2', 'fh.field_hul_code_value = n2.title');
        $query->join('field_data_field_hul_status', 'sta', 'sta.entity_id = n2.nid');
        $query->fields('sta', array('field_hul_status_value'));

        $status = $query->execute()->fetchField();

        if ($status) {
          $value['PARTY CODE'] = $nid;
        }
        else {
          $value['error_message'] .= $value['PARTY CODE'] . ' Address is not confirmed.';
        }
      }
    }
    if (isset($value['AREA HOOK']) && !empty($nid)) {
      $value['AREA HOOK'] = trim($value['AREA HOOK']);
      $value['uploaded_area_hook'] = $value['AREA HOOK'];
      $node = node_load($nid);
      $tname = $node->field_sales_day['und'][0]['value'];
      $tname = strtolower($tname); // lower the case as we have lowercase value stored in db

      if (empty($tname)) {
        $value['error_message'] .= " Sales day is empty for party code  " . $value['PARTY CODE'] . ' in outlet Master.';
      }
      else {
        $sales_day_tid = leanbox_check_taxonomy_exists_by_name($tname, 'Outlet days');
        
        $query = db_select('node', 'n');
        $query->fields('n', array('nid'));
        //$query->join('field_data_field_area_hook_day', 's', 's.entity_id = n.nid');
        $query->condition('n.title', $value['AREA HOOK']);
        $query->addTag('node_uid'); // no need of this condition as nid condition is given
        $query->condition('n.type', 'area_hook_master');
        // $query->condition('s.field_area_hook_day_tid', $sales_day_tid);
        $res = $query->execute()->fetchField();
        if (empty($res)) {
          $value['error_message'] .= " Area Hook " . $value['AREA HOOK'] . ' Does not exist in our system.';
        }
        else {
         // $value['AREA HOOK'] = $value['AREA HOOK'];
        }
      }
    }
    // insert into outstanding sales table 
    change_feeds_import_insert_data_in_custom_outlet_area_hook_table($value, $fid, $distributor_id);

    $context['results']['fid'] = $fid;
  }
}

function outlet_area_hook_csv_finished($success, $results, $operations) {
  if ($success) {
    $fid = $results['fid'];
    //drupal_goto("outlet-area-hook-confirm/" . $fid);
    $_GET['destination'] = "outlet-area-hook-confirm/" . $fid;
  }
  else {
    drupal_set_message('There is Error Occured in batch process');
  }
}

function change_feeds_import_insert_data_in_custom_outlet_area_hook_table($value, $fid, $distributor_id) {
  $sales_data = db_insert('custom_outlet_area_hook') // Table name no longer needs {}
      ->fields(array(
        'party_code' => $value['PARTY CODE'],
        'area_hook' => $value['AREA HOOK'],
        'created' => time(),
        'error_message' => $value['error_message'],
        'fid' => $fid,
        'distributor_id' => $distributor_id,
        'uploded_area_hook' => $value['uploaded_area_hook'],
      ))
      ->execute();
}

// show table after file is uploaded

function change_feeds_import_outlet_area_hook_confirm_content($fid) {
  global $base_url, $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);

  $query = db_select('custom_outlet_area_hook', 'sd');
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

  $header = array('Upload Date & Time', 'Total Lines Uploaded', 'Action');

  $rows['uploaded_date'] = date("Y-m-d h:i:s", $result[0]['created']);

  $rows['total_line_uploaded'] = $count;
  $discard_link = l(t('Discard'), 'outlet-area-hook-discard/' . $fid);

  $action_link = l(t('Confirm'), 'outlet-area-hook-confirm/create_content/' . $fid);

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
function change_feeds_import_outlet_area_hook_create_confirm_content($fid) {
  global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);
  // select data from stage table
  $sales_data_result = db_select('custom_outlet_area_hook', 'sd')
          ->condition('sd.fid', $fid, '=')
          ->condition('sd.distributor_id', $distributor_id, '=')
          ->fields('sd')
          ->execute()->fetchAll(PDO::FETCH_ASSOC);

  $operations = array();
  $data_chunk = array_chunk($sales_data_result, 1000);
  foreach ($data_chunk as $data) {
    $operations[] = array('change_feeds_import_outlet_area_hook_node', array($data, $distributor_id));
  }

  $batch = array(
    'operations' => $operations,
    'finished' => 'change_feeds_import_outlet_area_hook_finished',
    'title' => t('Updating outlet area hook node'),
    'init_message' => t('outlet area hook Batch Creation is starting.'),
    'progress_message' => t('Processed @current out of @total.'),
    'error_message' => t('outlet area hook Batch has encountered an error.'),
    'file' => drupal_get_path('module', 'change_feeds_import') . '/form/outlet_area_hook.inc'
  );
  batch_set($batch);
  batch_process('outlet_area_hook/import');
}

function change_feeds_import_outlet_area_hook_node($sales_data_result, $distributor_id, &$context) {
  if (!empty($sales_data_result)) {
    $count = 0;
    foreach ($sales_data_result as $key => $val) {

      if (!empty($val['party_code']) && empty($val['error_message'])) {

        // check to update node
        $nid = $val['party_code'];
        if (empty($nid)) {

          // we dont have to create new node here
        }
        else {
          $area_hook = $val['uploded_area_hook']; // Uploaded Area Hook
          $node = node_load($nid);
//          $sales_day = $node->field_sales_day['und'][0]['value']; // Sales Day from Outlet Master
//          $ol_channel = $node->field_channel['und'][0]['value']; // Channle from Outlet Master
//          $ol_pl_grp = $node->field_pl_group['und'][0]['value']; // PL Grp from Outlet Master
//          $van_pl_grp = get_van_pl_grp_from_hannel_pl_grp($ol_channel, $ol_pl_grp,$distributor_id); // VAN PL GRP from Party Pack Master
//          $day_van_area_nid = get_nid_from_master_using_sales_day_van_grp_area_hook($sales_day,$van_pl_grp,$area_hook);

          $node->field_outlet_area_hook['und'][0]['value'] = $area_hook;
//          field_attach_update('node', $node);
//          entity_get_controller('node')->resetCache(array($nid));
          node_save($node);

          $count++;
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
function change_feeds_import_outlet_area_hook_finished($success, $results, $operations) {
  if ($success) {
    global $user;
    $user_id = $user->uid;
    $distributor_id = change_feeds_import_get_distributor_id($user_id);

    // delete data from stage_billing_data.
    $fid = $results['fid'];
    $count = $results['count'];

    if (!empty($fid)) {
      db_delete('custom_outlet_area_hook')
          ->condition('fid', $fid, '=')
          ->condition('distributor_id', $distributor_id, '=')
          ->execute();
    }

    drupal_set_message(t("$count node was updated"));
    //drupal_goto("outlet-area-hook/import");
    $_GET['destination'] = "outlet-area-hook/import";
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
function change_feeds_import_outlet_area_hook_discard_data($fid) {
  if (!empty($fid)) {
    global $user;
    $user_id = $user->uid;
    $distributor_id = change_feeds_import_get_distributor_id($user_id);
    if (!empty($distributor_id)) {
      db_delete('custom_outlet_area_hook')
          ->condition('fid', $fid, '=')
          ->condition('distributor_id', $distributor_id, '=')
          ->execute();

      // Redirect to import page
      drupal_goto("outlet-area-hook/import");
    }
  }
  drupal_set_message(t('Something went wrong'), 'error');
}

function get_van_pl_grp_from_hannel_pl_grp($ol_channel, $ol_pl_grp,$distributor_id) {
  $query = db_select('node', 'n');
  $query->condition('n.type', 'outlet_master');
  $query->addTag('node_uid');
  $query->join('field_data_field_channel', 'ch', 'ch.entity_id = n.nid');
  $query->condition('ch.field_channel_value', $ol_channel);
  $query->condition('ch.bundle', 'outlet_master');
  $query->join('field_data_field_pl_group', 'pl', 'pl.entity_id = n.nid');
  $query->condition('pl.field_pl_group_value', $ol_pl_grp);
  $query->condition('pl.bundle', 'outlet_master');
  $query->join('field_data_field_channel', 'ppmch', 'ch.field_channel_value = ppmch.field_channel_value');
  $query->condition('ppmch.bundle', 'party_pack_master');
  $query->join('node', 'ppn', 'ppn.nid = ppmch.entity_id');
  $query->condition('ppn.uid', $distributor_id, '=');
  $query->join('field_data_field_pl_group', 'ppmpl', 'pl.field_pl_group_value = ppmpl.field_pl_group_value and ppmch.entity_id = ppmpl.entity_id');
  $query->condition('ppmch.bundle', 'party_pack_master');
  $query->join('field_data_field_van_pl_group', 'ppplg', 'ppn.nid = ppplg.entity_id');
  $query->fields('ppplg', array('field_van_pl_group_value'));
  $van_pl_grp = $query->execute()->fetchField();
  return $van_pl_grp;
}

function get_nid_from_master_using_sales_day_van_grp_area_hook($sales_day, $van_pl_grp, $area_hook) {
  $query = db_select('node', 'n');
  $query->condition('n.type', 'day_van_pl_gp_area_hook_grouping');
  $query->addTag('node_uid');
  $query->join('field_data_field_dva_area_hook', 'ah', 'ah.entity_id = n.nid');
  $query->condition('ah.field_dva_area_hook_value', $area_hook, '=');
  $query->join('field_data_field_dva_van_pl_group', 'ppplg', 'ppplg.entity_id = n.nid');
  $query->condition('ppplg.field_dva_van_pl_group_value', $van_pl_grp, '=');
  $query->join('field_data_field_dva_sales_day', 'sd', 'sd.entity_id = n.nid');
  $query->condition('sd.field_dva_sales_day_value', $sales_day, '=');
  $query->fields('n', array('nid'));
  $nid = $query->execute()->fetchField();
  return $nid;
}
