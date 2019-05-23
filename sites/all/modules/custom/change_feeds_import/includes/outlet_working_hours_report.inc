<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function working_hours_form($form, &$form_state) {
$form['#attached']['css'][] = drupal_get_path('module', 'change_feeds_import') . '/css/outlet_working_hours.css';
  if (isset($_GET['hul_code'])) {
    $hul_code = $_GET['hul_code'];
  }
  if (isset($_GET['party_name'])) {
    $party_name = $_GET['party_name'];
  }

  $form['#prefix'] = '<div id="itemlist-div">';
  $form['#suffix'] = '</div>';
  $form['hul_code'] = array(
    '#type' => 'textfield',
    '#title' => t('HUL Code'),
    '#default_value' => $hul_code,
    '#size' => 20,
    '#prefix' => '<div class="hul-code" id="outlet-hul-code">',
    '#suffix' => '</div>'
  );
  $form['party_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Party Name'),
    '#default_value' => $party_name,
    '#size' => 30,
    '#prefix' => '<div class="party-name" id="outlet-party-name">',
    '#suffix' => '</div>'
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Apply'),
    '#submit' => array('working_hours_form_submit'),
    '#prefix' => '<div class="outlet-submit" id="outlet-report-submit">',
    '#suffix' => '</div>'
  );

  return $form;
}

function working_hours_form_submit($form, &$form_state) {
  $hul_code = $form_state['values']['hul_code'];
  $party_name = $form_state['values']['party_name'];

  if (($form_state['values']["op"] == "Apply") && (arg(0) == 'working-hours-report')) {
    drupal_goto('working-hours-report', array('query' => array('hul_code' => $hul_code, 'party_name' => $party_name)));
  }
}

function working_hours_form_callback() {
  $output = '';
  if (isset($_GET['hul_code'])) {
    $hul_code = $_GET['hul_code'];
  }
  if (isset($_GET['party_name'])) {
    $party_name = $_GET['party_name'];
  }

  if (arg(1) != 'export') {
    $form = drupal_render(drupal_get_form('working_hours_form'));
  }

  // Days Data.
  $query1 = db_select('outlet_working_hours', 'wh');
  if (!empty($hul_code)) {
    $query1->condition('wh.hul_code', $hul_code, '=');
  }
  $query1->join('node', 'n', 'n.title = wh.hul_code');
  $query1->leftjoin('field_data_field_hul_party_name', 'pn', 'pn.entity_id = n.nid');
  if (!empty($party_name)) {
    $query1->condition('pn.field_hul_party_name_value', $party_name, '=');
  }
  $query1->fields('wh', array('hul_code', 'days', 'from_time', 'to_time', 'updated_time'));
  $query1->fields('pn', array('field_hul_party_name_value'));
  $query1->orderBy('wh.id');
  $result_days = $query1->execute()->fetchAll(PDO::FETCH_ASSOC);

  foreach ($result_days as $key_days => $val_days) {
    $days_time_value = $val_days['from_time'] . '-' . $val_days['to_time'];
    if (!empty($val_days['days'])) {
      $row1[$val_days['hul_code']]['hul_code'] = $val_days['hul_code'];
      $row1[$val_days['hul_code']]['party_name'] = $val_days['field_hul_party_name_value'];
      if ($days_time_value == 'Closed-Closed') {
        $row1[$val_days['hul_code']][$val_days['days']] = 'Closed';
      }
      else {
        $row1[$val_days['hul_code']][$val_days['days']] = ltrim($row1[$val_days['hul_code']][$val_days['days']] . ',' . $days_time_value, ',');
      }
      $row1[$val_days['hul_code']]['updated_time'] = date('Y-M-d h:i:s', $val_days['updated_time']);
      unset($row1[$val_days['hul_code']]['updated_time']);
      $row1[$val_days['hul_code']]['updated_time'] = !empty($val_days['updated_time']) ? date('Y-M-d h:i:s', $val_days['updated_time']) : '';
    }
  }

  $below_header_array = array(
    'HUL Code' => array('data' => t('HUL Code')),
    'Party Name' => array('data' => t('Party Name')),
    'Mon' => array('data' => t('Mon')),
    'Tue' => array('data' => t('Tue')),
    'Wed' => array('data' => t('Wed')),
    'Thu' => array('data' => t('Thu')),
    'Fri' => array('data' => t('Fri')),
    'Sat' => array('data' => t('Sat')),
    'Sun' => array('data' => t('Sun')),
    'Updated Time' => array('data' => t('Updated Time'))
  );
  $per_page = 10;
  // Initialize the pager
  $current_page = pager_default_initialize(count($row1), $per_page);
  // Split your list into page sized chunks
  $chunks = array_chunk($row1, $per_page, TRUE);
  // Show the appropriate items from the list
  $below_output = theme('table', array('header' => $below_header_array, 'rows' => $chunks[$current_page]));
  // Show the pager
  $below_output .= theme('pager', array('quantity', count($row1)));

  // Special Days Data.
  $query = db_select('outlet_working_hours', 'wh');
  if (!empty($hul_code)) {
    $query->condition('wh.hul_code', $hul_code, '=');
  }
  $query->join('node', 'n', 'n.title = wh.hul_code');
  $query->leftjoin('field_data_field_hul_party_name', 'pn', 'pn.entity_id = n.nid');
  if (!empty($party_name)) {
    $query->condition('pn.field_hul_party_name_value', $party_name, '=');
  }
  $query->fields('wh', array('hul_code', 'from_time', 'to_time', 'special_days', 'updated_time'));
  $query->fields('pn', array('field_hul_party_name_value'));
  $query->orderBy('wh.id');
  $result = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

  foreach ($result as $key => $val) {
    $time_value = $val['from_time'] . '-' . $val['to_time'];
    if (!empty($val['special_days'])) {

      $row[$val['hul_code'] . $val['special_days']]['hul_code'] = $val['hul_code'];
      $row[$val['hul_code'] . $val['special_days']]['party_name'] = $val['field_hul_party_name_value'];
      $row[$val['hul_code'] . $val['special_days']]['special_days'] = date('Y-M-d', $val['special_days']);
      if ($time_value == 'Closed-Closed') {
        $row[$val['hul_code'] . $val['special_days']][$val['special_days']] = 'Closed';
      }
      else {
        $row[$val['hul_code'] . $val['special_days']][$val['special_days']] = ltrim($row[$val['hul_code']][$val['special_days']] . ',' . $time_value, ',');
      }
      if (!empty($val['updated_time'])) {
        $row[$val['hul_code'] . $val['special_days']]['updated_time'] = date('Y-M-d h:i:s', $val['updated_time']);
      }
      else {
        $row[$val['hul_code'] . $val['special_days']]['updated_time'] = '';
      }
    }
  }

  $hours_main_header = array(
    'HUL Code',
    'Party Name',
    'Special Days',
    'Time',
    'Updated Time'
  );
  $per_page = 10;
  // Initialize the pager
  $current_page = pager_default_initialize(count($row), $per_page);
  // Split your list into page sized chunks
  $chunks = array_chunk($row, $per_page, TRUE);
  // Show the appropriate items from the list
  $main_output = theme('table', array('header' => $hours_main_header, 'rows' => $chunks[$current_page]));
  // Show the pager
  $main_output .= theme('pager', array('quantity', count($row)));

  $html = theme('table', array('header' => $hours_main_header, 'rows' => $row, 'attributes' => array('width' => '100%')));
  cache_set('working-hours-report' . session_id(), $html, 'cache', time() + 60 * 10);
  $main_output .= l(t('XLS'), 'working-hours-report/export', array('query' => array('hul_code' => $hul_code, 'party_name' => $party_name)));

  return $form . $below_output . $main_output;
  $form_state['rebuild'] = TRUE;
}

function working_hours_report_export() {

  if (isset($_GET['hul_code'])) {
    $hul_code = $_GET['hul_code'];
  }
  if (isset($_GET['party_name'])) {
    $party_name = $_GET['party_name'];
  }

  $hul_data = working_hours_data_report($hul_code, $party_name);

  $module = module_load_include('inc', 'phpexcel');
  $wrapper = file_stream_wrapper_get_instance_by_uri('public://');
  // Generate a file name. If it's unique, it's less likely to conflict with an
  // existing file. You could also put up some more checks on this, if it's likely
  // to conflict (like, when you have many export/download requests).

  $filename = 'outlet-working_hours-report.xls';
  $filepath = $wrapper->realpath() . "/" . $filename;
  $hours_below_header = array(
    'HUL Code',
    'Party Name',
    'Mon',
    'Tue',
    'Wed',
    'Thu',
    'Fri',
    'Sat',
    'Sun',
    'Updated Time'
  );
  $hours_main_header = array(
    'HUL Code',
    'Party Name',
    'Special Days',
    'Time',
    'Updated Time'
  );

  $i = 0;
  foreach ($hul_data['main_data'] as $val) {

    foreach ($val as $v) {
      $main_data[$i][] = $v;
    }
    $i++;
  }
  $i = 0;
  foreach ($hul_data['below_data'] as $val) {

    foreach ($val as $v) {
      $below_data[$i][] = $v;
    }
    $i++;
  }

  $result = phpexcel_export(
    array('days_data' => $hours_below_header, 'special_days_data' => $hours_main_header
    ), array('days_data' => $below_data, 'special_days_data' => $main_data
    ), $filepath
  );

  if ($result == 1) {
    // Exported successfully. Let's register the file with Drupal. We simply
    // tell Drupal to copy the file over the existing one, by passing in
    // temporary://$filename.
    $file = file_save_data(
      file_get_contents($filepath), "public://$filename", FILE_EXISTS_REPLACE
    );

    // By default, the file is stored as a permanent file. Let's make it
    // temporary, so Drupal will remove it (in 6 hours, if your cron is set up
    // correctly).
    $file->status = 0;
    file_save($file);
    $file->uri;
    // Start downloading. This requires a hook_file_download() implementation!
    $headers = file_download_headers($file->uri);
    $headers['Content-disposition'] = 'attachment; filename=' . $filename;
    working_hours_file($file->uri, $headers);
  }
  else {
    echo "error";
  }
}

function working_hours_file($uri, $headers) {
  if (ob_get_level()) {
    ob_end_clean();
  }
  foreach ($headers as $name => $value) {
    drupal_add_http_header($name, $value);
  }
  drupal_send_headers();
  $scheme = file_uri_scheme($uri);
  // Transfer file in 1024 byte chunks to save memory usage.
  if ($scheme && file_stream_wrapper_valid_scheme($scheme) && $fd = fopen($uri, 'rb')) {
    while (!feof($fd)) {
      print fread($fd, 1024);
    }
    fclose($fd);
  }
  else {
    drupal_not_found();
  }
  file_unmanaged_delete($uri);
  drupal_exit();
}

function working_hours_data_report($hul_code, $party_name) {
  //Days Data table
  $query1 = db_select('outlet_working_hours', 'wh');
  if (!empty($hul_code)) {
    $query1->condition('wh.hul_code', $hul_code, '=');
  }
  $query1->join('node', 'n', 'n.title = wh.hul_code');
  $query1->leftjoin('field_data_field_hul_party_name', 'pn', 'pn.entity_id = n.nid');
  if (!empty($party_name)) {
    $query1->condition('pn.field_hul_party_name_value', $party_name, '=');
  }
  $query1->fields('wh', array('hul_code', 'days', 'from_time', 'to_time', 'updated_time'));
  $query1->fields('pn', array('field_hul_party_name_value'));
  $query1->orderBy('wh.id');
  $result_days = $query1->execute()->fetchAll(PDO::FETCH_ASSOC);

  foreach ($result_days as $key_days => $val_days) {
    $days_time_value = $val_days['from_time'] . '-' . $val_days['to_time'];
    if (!empty($val_days['days'])) {
      $row1[$val_days['hul_code']]['hul_code'] = $val_days['hul_code'];
      $row1[$val_days['hul_code']]['party_name'] = $val_days['field_hul_party_name_value'];
      if ($days_time_value == 'Closed-Closed') {
        $row1[$val_days['hul_code']][$val_days['days']] = 'Closed';
      }
      else {
        $row1[$val_days['hul_code']][$val_days['days']] = ltrim($row1[$val_days['hul_code']][$val_days['days']] . ',' . $days_time_value, ',');
      }
      $row1[$val_days['hul_code']]['updated_time'] = date('Y-M-d h:i:s', $val_days['updated_time']);
      unset($row1[$val_days['hul_code']]['updated_time']);
      $row1[$val_days['hul_code']]['updated_time'] = !empty($val_days['updated_time']) ? date('Y-M-d h:i:s', $val_days['updated_time']) : '';
    }
  }
  $table['below_data'] = $row1;

  // Special Days Data.
  $query = db_select('outlet_working_hours', 'wh');
  if (!empty($hul_code)) {
    $query->condition('wh.hul_code', $hul_code, '=');
  }
  $query->join('node', 'n', 'n.title = wh.hul_code');
  $query->leftjoin('field_data_field_hul_party_name', 'pn', 'pn.entity_id = n.nid');
  if (!empty($party_name)) {
    $query->condition('pn.field_hul_party_name_value', $party_name, '=');
  }
  $query->fields('wh', array('hul_code', 'from_time', 'to_time', 'special_days', 'updated_time'));
  $query->fields('pn', array('field_hul_party_name_value'));
  $query->orderBy('wh.id');
  $result = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

  foreach ($result as $key => $val) {
    $time_value = $val['from_time'] . '-' . $val['to_time'];
    if (!empty($val['special_days'])) {

      $row[$val['hul_code'] . $val['special_days']]['hul_code'] = $val['hul_code'];
      $row[$val['hul_code'] . $val['special_days']]['party_name'] = $val['field_hul_party_name_value'];
      $row[$val['hul_code'] . $val['special_days']]['special_days'] = date('Y-M-d', $val['special_days']);
      if ($time_value == 'Closed-Closed') {
        $row[$val['hul_code'] . $val['special_days']][$val['special_days']] = 'Closed';
      }
      else {
        $row[$val['hul_code'] . $val['special_days']][$val['special_days']] = ltrim($row[$val['hul_code']][$val['special_days']] . ',' . $time_value, ',');
      }
      if (!empty($val['updated_time'])) {
        $row[$val['hul_code'] . $val['special_days']]['updated_time'] = date('Y-M-d h:i:s', $val['updated_time']);
      }
      else {
        $row[$val['hul_code'] . $val['special_days']]['updated_time'] = '';
      }
    }
  }
  $table['main_data'] = $row;

  return $table;
}
