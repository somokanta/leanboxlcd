<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function trip_wise_parkstop_form($form, $form_state) {
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
      '3DeliveryBoysTotalTime' => array('data' => t('2DeliveryBoysTotalTime')),
      '3DeliveryBoysInAreaTime' => array('data' => t('2DeliveryBoysInAreaTime')),
      '3DeliveryBoysTotalWaitTime' => array('data' => t('2DeliveryBoysTotalWaitTime')),
      '4DeliveryBoysTotalTime' => array('data' => t('2DeliveryBoysTotalTime')),
      '4DeliveryBoysInAreaTime' => array('data' => t('2DeliveryBoysInAreaTime')),
      '4DeliveryBoysTotalWaitTime' => array('data' => t('2DeliveryBoysTotalWaitTime'))
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
  $form_state['main_data'] = $main_data;

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
          $below_data[$nid]['Child Service Time'] = $clusterStops[0]->{'ChildServiceTime'};
          $below_data[$nid]['Child Walking Time'] = $clusterStops[0]->{'ChildWalkingTime'};
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

  $form_state['below_data'] = $below_data;
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
