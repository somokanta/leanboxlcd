<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function trip_wise_parkstop_form($form, $form_state) {
  global $user;
  $distributor_id = change_feeds_import_get_distributor_id($user->uid);
  
  $form['#prefix'] = '<div id="itemlist-div">';
  $form['#suffix'] = '</div>';
  $form['trip_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Trip ID'),
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Show Trip Report'),
    '#ajax' => array(
      'wrapper' => 'itemlist-div',
      'callback' => 'get_data_of_parkstop_form',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    '#validate' => array('parkstop_form_validate'),
    '#submit' => array('parkstop_form_submit'),
  );
  if (!empty($form_state['below_data'])) {
    $below_header_array = array(
      'Trip ID' => array('data' => t('Trip ID')),
      'Sequence' => array('data' => t('Sequence')),
      '2DeliveryBoys Sequence' => array('data' => t('2DeliveryBoys Sequence')),
      '3DeliveryBoys Sequence' => array('data' => t('3DeliveryBoys Sequence')),
      '4DeliveryBoys Sequence' => array('data' => t('4DeliveryBoys Sequence')),
      'Party Code' => array('data' => t('Party Code')),
      'Bill Number' => array('data' => t('Bill Number')),
      'nid' => array('data' => t('nid')),
      'Parkstop Travel Time (Mins)' => array('data' => t('Parkstop Travel Time (Mins)')),
      'Parkstop Service Time (Mins)' => array('data' => t('Parkstop Service Time (Mins)')),
      '2DeliveryBoysTime' => array('data' => t('2DeliveryBoysTime')),
      '2DeliveryBoysWaitTime' => array('data' => t('2DeliveryBoysWaitTime')),
      '3DeliveryBoysTime' => array('data' => t('3DeliveryBoysTime')),
      '3DeliveryBoysWaitTime' => array('data' => t('3DeliveryBoysWaitTime')),
      '4DeliveryBoysTime' => array('data' => t('4DeliveryBoysTime')),
      '4DeliveryBoysWaitTime' => array('data' => t('4DeliveryBoysWaitTime')),
      'Child Service Time' => array('data' => t('Child Service Time')),
      'Child Walking Time' => array('data' => t('Child Walking Time')),
    );
    $form['submit_vp']['below_table'] = array(
      '#empty' => t('No data available.'),
      '#prefix' => '<div class = "below-table-date-details">',
      '#suffix' => '</div>',
      '#header' => $below_header_array,
      '#theme' => 'table',
      '#tree' => TRUE,
      '#rows' => $form_state['below_data'],
    );

    //$rr = l('Open PDF', "/download-loose-picking/$trip_id", array('attributes' => array('target'=> '_blank')));
//    $form['dowmload_mark'] = array(
//      '#type' => 'markup',
//      '#markup' => $rr,
//    );
  }


  if (!empty($form_state['main_data'])) {
    $main_header_array = array(
      'Trip ID' => array('data' => t('Trip ID')),
      'Van Total KMs' => array('data' => t('Van Total KMs')),
      'Van In Area KMs' => array('data' => t('Van In Area KMs')),
      '2DeliveryBoysTotalTime' => array('data' => t('2DeliveryBoysTotalTime')),
      '2DeliveryBoysInAreaTime' => array('data' => t('2DeliveryBoysInAreaTime')),
      '2DeliveryBoysTotalWaitTime' => array('data' => t('2DeliveryBoysTotalWaitTime')),
      '3DeliveryBoysTotalTime' => array('data' => t('3DeliveryBoysTotalTime')),
      '3DeliveryBoysInAreaTime' => array('data' => t('3DeliveryBoysInAreaTime')),
      '3DeliveryBoysTotalWaitTime' => array('data' => t('3DeliveryBoysTotalWaitTime')),
      '4DeliveryBoysTotalTime' => array('data' => t('4DeliveryBoysTotalTime')),
      '4DeliveryBoysInAreaTime' => array('data' => t('4DeliveryBoysInAreaTime')),
      '4DeliveryBoysTotalWaitTime' => array('data' => t('4DeliveryBoysTotalWaitTime'))
    );
    $form['submit_vp']['main_table'] = array(
      '#empty' => t('No data available.'),
      '#prefix' => '<div class = "main-table-date-details">',
      '#suffix' => '</div>',
      '#header' => $main_header_array,
      '#theme' => 'table',
      '#tree' => TRUE,
      '#rows' => $form_state['main_data'],
    );

    //$rr = l('Open PDF', "/download-loose-picking/$trip_id", array('attributes' => array('target'=> '_blank')));
//    $form['dowmload_mark'] = array(
//      '#type' => 'markup',
//      '#markup' => $rr,
//    );

    // Download trip details
    $trip_id = $form_state['input']['trip_number'];
    $url = "/parkstop-trip-report/export/$trip_id/$distributor_id";
    if (!empty($form_state['enable_download'])) {
      $form['download_link'] = array(
        '#prefix' => '<div class ="download_trip">',
        '#markup' => l('XLS', $url, array('attributes' => array('target' => '_blank'))),
        '#suffix' => '</div>',
      );
      unset($form_state['enable_download']);
    }
  }
  return $form;
}

function get_data_of_parkstop_form($form, &$form_state) {
  return $form;
}

function parkstop_form_validate($form, &$form_state) {
  $trip_id = $form_state['values']['trip_number'];
  global $user;
  $distributor_id = change_feeds_import_get_distributor_id($user->uid);
  $query = db_select('node', 'n');
  $query->condition('n.type', 'dispatch_data');
  $query->condition('n.title', $trip_id, '=');
  $query->condition('n.uid', $distributor_id);
  $query->join('field_data_field_delivery_boy_count', 'db', 'db.entity_id = n.nid');
  $query->addfield('db', 'field_delivery_boy_count_value', 'db_count');
  $db_count = $query->execute()->fetchField();
  if (empty($db_count)) {
    form_set_error('form', 'No data exist for trip ID, Please check');
  }
}

function parkstop_form_submit($form, &$form_state) {
  $trip_id = $form_state['values']['trip_number'];
  global $user;
  $distributor_id = change_feeds_import_get_distributor_id($user->uid);

  $parkstop_data_report = parkstop_data_report($trip_id, $distributor_id);
  $form_state['below_data'] = $parkstop_data_report['below_data'];
  $form_state['main_data'] = $parkstop_data_report['main_data'];
  $form_state['enable_download'] = 1;
  $form_state['rebuild'] = TRUE;
}

function loop_again_to_write_row($value, $nid, $trip_id, $p, $child_seq, $del_boy, $distributor_id, $old_data, $n) {
  $below_data[$nid]['Trip ID'] = $trip_id;

  if ($del_boy == 2) {
    $below_data[$nid]['Sequence'] = ++$n; // increment only once
    $below_data[$nid]['2DeliveryBoys Sequence'] = $p . '.' . str_pad($child_seq, 2, 0, STR_PAD_LEFT);
    $below_data[$nid]['3DeliveryBoys Sequence'] = $old_data['3DeliveryBoys Sequence'];
    $below_data[$nid]['4DeliveryBoys Sequence'] = $old_data['4DeliveryBoys Sequence'];
  }
  else if ($del_boy == 3) {
    $below_data[$nid]['Sequence'] = $old_data['Sequence'];
    $below_data[$nid]['2DeliveryBoys Sequence'] = $old_data['2DeliveryBoys Sequence'];
    $below_data[$nid]['3DeliveryBoys Sequence'] = $p . '.' . str_pad($child_seq, 2, 0, STR_PAD_LEFT);
    $below_data[$nid]['4DeliveryBoys Sequence'] = $old_data['4DeliveryBoys Sequence'];
  }
  else {
    $below_data[$nid]['Sequence'] = $old_data['Sequence'];
    $below_data[$nid]['2DeliveryBoys Sequence'] = $old_data['2DeliveryBoys Sequence'];
    $below_data[$nid]['3DeliveryBoys Sequence'] = $old_data['3DeliveryBoys Sequence'];
    $below_data[$nid]['4DeliveryBoys Sequence'] = $p . '.' . str_pad($child_seq, 2, 0, STR_PAD_LEFT);
  }
  $latlong = get_latlong_from_bills($nid, $distributor_id);
  $below_data[$nid]['Party Code'] = $latlong['party_code'];
  $below_data[$nid]['Bill Number'] = $latlong['bill_num'];
  $below_data[$nid]['nid'] = $nid;

  $below_data[$nid]['Parkstop Travel Time (Mins)'] = $value->ParkstopTravelTime;
  $below_data[$nid]['Parkstop Service Time (Mins)'] = $value->ParentServiceTime;

  $completion_time = $value->ApproxCompletionTime;

  $below_data[$nid]['2DeliveryBoysTime'] = $completion_time[0]->{'2DeliveryBoysTime'};
  $below_data[$nid]['2DeliveryBoysWaitTime'] = $completion_time[0]->{'2DeliveryBoysWaitTime'};
  $below_data[$nid]['3DeliveryBoysTime'] = $completion_time[1]->{'3DeliveryBoysTime'};
  $below_data[$nid]['3DeliveryBoysWaitTime'] = $completion_time[1]->{'3DeliveryBoysWaitTime'};
  $below_data[$nid]['4DeliveryBoysTime'] = $completion_time[2]->{'4DeliveryBoysTime'};
  $below_data[$nid]['4DeliveryBoysWaitTime'] = $completion_time[2]->{'4DeliveryBoysWaitTime'};

  return $below_data[$nid];
}

function get_latlong_from_bills($nid, $distributor_id) {
  $query = db_select('node', 'n');
  $query->condition('n.type', 'sales_register_data');
  $query->condition('n.nid', $nid, '=');
  $query->condition('n.uid', $distributor_id);
  $query->join('field_data_field_bill_value', 'bv', 'bv.entity_id = n.nid');
  $query->join('field_data_field_party_code_reference', 'pc', 'pc.entity_id = n.nid');
  $query->join('node', 'n1', 'n1.nid = pc.field_party_code_reference_nid');
  $query->condition('n1.type', 'outlet_master');

  $query->addfield('n', 'title', 'bill_num');
  $query->addfield('n1', 'title', 'party_code');
  $res = $query->execute()->fetch(PDO::FETCH_ASSOC);
  return $res;
}

function _get_delivery_boy_sort($array, $field_name) {
  uasort($array, function ($a, $b) use ($field_name) {
    if ($a[$field_name] == $b[$field_name]) {
      return 0;
    }
    return ($a[$field_name] < $b[$field_name]) ? -1 : 1;
  });
  return $array;
}

function parkstop_trip_report_export($trip_id, $distributor_id) {
  $trip_data = parkstop_data_report($trip_id, $distributor_id);

  $module = module_load_include('inc', 'phpexcel');
  $wrapper = file_stream_wrapper_get_instance_by_uri('public://');
  // Generate a file name. If it's unique, it's less likely to conflict with an
  // existing file. You could also put up some more checks on this, if it's likely
  // to conflict (like, when you have many export/download requests).

  $filename = 'trip-wise-parkstop-report_'.$trip_id . '.xls';
  $filepath = $wrapper->realpath() . "/" . $filename;
  $parkstop_below_header = array(
    'Trip ID',
    'Sequence',
    '2DeliveryBoys Sequence',
    '3DeliveryBoys Sequence',
    '4DeliveryBoys Sequence',
    'Party Code',
    'Bill Number',
    'nid',
    'Parkstop Travel Time (Mins)',
    'Parkstop Service Time (Mins)',
    '2DeliveryBoysTime',
    '2DeliveryBoysWaitTime',
    '3DeliveryBoysTime',
    '3DeliveryBoysWaitTime',
    '4DeliveryBoysTime',
    '4DeliveryBoysWaitTime',
    'Child Service Time',
    'Child Walking Time',
  );
  $parkstop_main_header = array(
    'Trip ID',
    'Van Total KMs',
    'Van In Area KMs',
    '2DeliveryBoysTotalTime',
    '2DeliveryBoysInAreaTime',
    '2DeliveryBoysTotalWaitTime',
    '3DeliveryBoysTotalTime',
    '3DeliveryBoysInAreaTime',
    '3DeliveryBoysTotalWaitTime',
    '4DeliveryBoysTotalTime',
    '4DeliveryBoysInAreaTime',
    '4DeliveryBoysTotalWaitTime');

  $i = 0;
  foreach ($trip_data['main_data'] as $val) {

    foreach ($val as $v) {
      $main_data[$i][] = $v;
    }
    $i++;
  }
  $i = 0;
  foreach ($trip_data['below_data'] as $val) {

    foreach ($val as $v) {
      $below_data[$i][] = $v;
    }
    $i++;
  }

  $result = phpexcel_export(
    array('main_data' => $parkstop_main_header,
          'below_data' => $parkstop_below_header),
    array('main_data' => $main_data,
          'below_data' => $below_data
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
    trip_parkstop_transfer_file($file->uri, $headers);
  }
  else {
    echo "error";
  }
}

function trip_parkstop_transfer_file($uri, $headers) {
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

function parkstop_data_report($trip_id, $distributor_id) {
  if (!empty($trip_id)) {
    $query = db_select('parkstop_api_log', 'ps');
    $query->condition('ps.distributor_id', $distributor_id, '=');
    $query->condition('ps.trip_id', $trip_id, '=');
    $query->fields('ps', array('api_response'));
    $query->orderBy('ps.id', 'asc');
    $result = $query->execute()->fetch(PDO::FETCH_ASSOC);
  }
  $res_decode = json_decode($result['api_response']);

  //main table
  $k = 1;
  $table = array();
  $main_data[$k]['Trip ID'] = $trip_id;
  $main_data[$k]['Van Total KMs'] = $res_decode->VanTotalKMs;
  $main_data[$k]['Van In Area KMs'] = $res_decode->VanInAreaKMs;

  $vaninareatime = $res_decode->VanInAreaTime;
  $vantime = $res_decode->VanTime;
  $vantotalwaittime = $res_decode->VanTotalWaitTime;
  $main_data[$k]['2DeliveryBoysTotalTime'] = $vantime->{'2DeliveryBoysTotalTime'};
  $main_data[$k]['2DeliveryBoysInAreaTime'] = $vaninareatime->{'2DeliveryBoysInAreaTime'};
  $main_data[$k]['2DeliveryBoysTotalWaitTime'] = $vantotalwaittime->{'2DeliveryBoysTotalWaitTime'};
  $main_data[$k]['3DeliveryBoysTotalTime'] = $vantime->{'3DeliveryBoysTotalTime'};
  $main_data[$k]['3DeliveryBoysInAreaTime'] = $vaninareatime->{'3DeliveryBoysInAreaTime'};
  $main_data[$k]['3DeliveryBoysTotalWaitTime'] = $vantotalwaittime->{'3DeliveryBoysTotalWaitTime'};
  $main_data[$k]['4DeliveryBoysTotalTime'] = $vantime->{'4DeliveryBoysTotalTime'};
  $main_data[$k]['4DeliveryBoysInAreaTime'] = $vaninareatime->{'4DeliveryBoysInAreaTime'};
  $main_data[$k]['4DeliveryBoysTotalWaitTime'] = $vantotalwaittime->{'4DeliveryBoysTotalWaitTime'};
  $table['main_data'] = $main_data;

  //below table
  $p = $n = 0;
  $stops = $res_decode->Van;

  foreach ($stops as $key => $value) {

    $nid = $value->OptimalStop;

    $below_data[$nid]['Trip ID'] = $trip_id;
    $below_data[$nid]['Sequence'] = ++$n;
    $below_data[$nid]['2DeliveryBoys Sequence'] = ++$p;
    $below_data[$nid]['3DeliveryBoys Sequence'] = $p;
    $below_data[$nid]['4DeliveryBoys Sequence'] = $p;

    $latlong = get_latlong_from_bills($nid, $distributor_id);
    $below_data[$nid]['Party Code'] = $latlong['party_code'];
    $below_data[$nid]['Bill Number'] = $latlong['bill_num'];
    $below_data[$nid]['nid'] = $nid;

    $below_data[$nid]['Parkstop Travel Time (Mins)'] = $value->ParkstopTravelTime;
    $below_data[$nid]['Parkstop Service Time (Mins)'] = $value->ParentServiceTime;

    $completion_time = $value->ApproxCompletionTime;

    $below_data[$nid]['2DeliveryBoysTime'] = $completion_time[0]->{'2DeliveryBoysTime'};
    $below_data[$nid]['2DeliveryBoysWaitTime'] = $completion_time[0]->{'2DeliveryBoysWaitTime'};
    $below_data[$nid]['3DeliveryBoysTime'] = $completion_time[1]->{'3DeliveryBoysTime'};
    $below_data[$nid]['3DeliveryBoysWaitTime'] = $completion_time[1]->{'3DeliveryBoysWaitTime'};
    $below_data[$nid]['4DeliveryBoysTime'] = $completion_time[2]->{'4DeliveryBoysTime'};
    $below_data[$nid]['4DeliveryBoysWaitTime'] = $completion_time[2]->{'4DeliveryBoysWaitTime'};

    $below_data[$nid]['Child Service Time'] = '';
    $below_data[$nid]['Child Walking Time'] = '';


    $clusterStops = $completion_time[0]->ClusterStops;
    if (!empty($clusterStops)) {
      for ($i = 0; $i < 3; $i++) {
        $clusterStops = $completion_time[$i]->ClusterStops;
        foreach ($clusterStops as $k => $val) {
          $nid = $clusterStops[$k]->{'ChildStop'};
          //increment the loop
          $below_data[$nid] = loop_again_to_write_row($value, $nid, $trip_id, $p, $k + 1, $i + 2, $distributor_id, $below_data[$nid], $n);
          $n = $below_data[$nid]['Sequence'];
          $below_data[$nid]['Child Service Time'] = $clusterStops[$k]->{'ChildServiceTime'};
          $below_data[$nid]['Child Walking Time'] = $clusterStops[$k]->{'ChildWalkingTime'};
        }
      }
    }
  }

  $query = db_select('node', 'n');
  $query->condition('n.type', 'dispatch_data');
  $query->condition('n.title', $trip_id, '=');
  $query->condition('n.uid', $distributor_id);
  $query->join('field_data_field_delivery_boy_count', 'db', 'db.entity_id = n.nid');
  $query->addfield('db', 'field_delivery_boy_count_value', 'db_count');
  $db_count = $query->execute()->fetchField();
  $below_data = _get_delivery_boy_sort($below_data, $db_count . 'DeliveryBoys Sequence');

  $table['below_data'] = $below_data;

  return $table;
}
