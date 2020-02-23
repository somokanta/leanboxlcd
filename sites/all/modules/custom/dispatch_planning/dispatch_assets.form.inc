<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function _dispatch_assets_form($form, &$form_state) {
  $form = array();
  $form['#prefix'] = '<div id="itemlist-div">';
  $form['#suffix'] = '</div>';
  $trip_id = arg(1);
  //$form_state['error'] = 0;
  global $user;
  if (!empty($trip_id)) {
      $default_trip_id = $trip_id;
  }
  
  // initialize editable elements here
  $form['dispatch_count'] = array('#type' => 'value');

  $trip_number = $form_state['input']['trip_number'];
  if (empty($trip_number) && !empty($default_trip_id)) {
    $trip_number =   $default_trip_id;
  }
  $form['trip_number'] = array(
    '#prefix' => '<div class="textfield-wrapper">',
    '#type' => 'textfield',
    '#title' => t('Trip ID'),
    '#required' => TRUE,
    '#default_value'=>$default_trip_id,  
    '#autocomplete_path' => 'trip/autocomplete',
    '#ajax' => array(
      'wrapper' => 'itemlist-div',
      'callback' => 'dispatch_count_callback',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );
  if (in_array('central_distributor', $user->roles)) {
    $central_distributor_id = change_feeds_import_get_distributor_id($user->uid);
  }
  else {
    $central_distributor_id = get_central_distributor_id($user->uid);
  }
  $form['central_distributor_id'] = array(
    '#type' => 'hidden',
    '#value' => $central_distributor_id,
  );

  if (!empty($trip_number)) {
    //$distributor_id = change_feeds_import_get_distributor_id($user_id);
    $distributor_id = get_distributorid_of_trip($trip_number);
    
    // check trip if it is Returns Confirmed
    // check trip if it is VRS Confirmed
    // check trip if it is Handover Confirmed
    $trip_detail = check_trip_id_exists_or_not($distributor_id, $trip_number);
    $vrs_status = $trip_detail->field_vrs_status_value;
    
    $current_vrs_status = $vrs_status;
    $field_vrs_status = field_info_field('field_vrs_status');
    $vrs_status_values = list_allowed_values($field_vrs_status);
    unset($vrs_status_values[-1]);
    if(empty($trip_detail)) {
      form_set_error('trip_number', t("Invalid Trip ID"));
    }
    else if (isset($vrs_status_values[$current_vrs_status])) {
      form_set_error('form', t("Trip is already in $vrs_status_values[$current_vrs_status] status."));
      //drupal_set_message(t("Trip is already in $vrs_status_values[$current_vrs_status] status."), 'error');
    }
    
//    if ($vrs_status == 0) {
//      //drupal_set_message(t('Invalid Trip ID.'), 'error');
//      $form_state['error'] = 1;
//      
//    }
//    if ($vrs_status == 1) {
//      drupal_set_message(t('Trip is already in returns confirmed status.'), 'error');
//    }
//    else if ($vrs_status == 2) {
//      drupal_set_message(t('Trip is already in VRS confirmed status.'), 'error');
//    }
//    else if ($vrs_status == 3) {
//      drupal_set_message(t('Trip is already in VRS pending status.'), 'error');
//    }
//    else if ($vrs_status == 4) {
//      drupal_set_message(t('Trip is already in Handover pending status.'), 'error');
//    }
//    else if ($vrs_status == 5) {
//      drupal_set_message(t('Trip is already in Handover confirmed status.'), 'error');
//    }
    else {
      $item_details = _get_all_assets_type($trip_number,$distributor_id);

      $item_details_header = array(
        'ASSET_TYPE' => 'ASSET NAME',
        'planned_count' => 'PLANNED COUNT',
        'dispatch_count' => 'DISPATCH COUNT'
      );

      foreach ($item_details as $key => &$value) {
        $item_details[$key]['dispatch_count']['data'] = array(
          '#type' => 'textfield',
          '#title' => t('Dispatch Count'),
          '#title_display' => 'invisible',
          '#size' => 20,
          '#value' => (!empty($value['field_dispatch_count_value']) || (empty($value['field_dispatch_count_value']) && $value['field_dispatch_count_value'] === "0")) ? $value['field_dispatch_count_value'] : $value['field_planned_count_value'],
          '#name' => "dispatch_count[$key]",
          '#attributes' => array('class' => array('asset-dispatch-count')),
        );

        unset($value['field_dispatch_count_value']);
        $trip_data[$key]['asset_type'] = $value['title'];
        $trip_data[$key]['planned_count'] = $value['field_planned_count_value'];
      }

      $form['trip_data'] = array(
        '#type' => 'value',
        '#value' => $trip_data,
      );
      $form['trip_distributor_id'] = array(
        '#type' => 'hidden',
        '#value' => $distributor_id,
      );
     
      $form['trip_details'] = array(
        '#theme' => 'table',
        '#header' => $item_details_header,
        '#rows' => $item_details,
      );
      $form['submit'] = array(
        '#type' => 'submit',
        '#value' => 'submit',
      );
    }
  }
  return $form;
}

/**
 * 
 * @param type $form
 * @param type $form_state
 * @return array
 */
function dispatch_count_callback($form, &$form_state) {
  $commands[] = ajax_command_replace('#itemlist-div', render($form));
  $commands[] = ajax_command_replace('.alert-danger', '');
  $commands[] = ajax_command_replace('.alert-success', '');
  $commands[] = ajax_command_prepend('#itemlist-div', theme('status_messages'));
  return array('#type' => 'ajax', '#commands' => $commands);
  //return $form;
}

/**
 * 
 * @param array $form
 * @param type $form_state
 * @return array
 */

function _dispatch_assets_form_submit($form, &$form_state) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);

  $trip_id = $form_state['values']['trip_number'];
  $trip_data = $form_state['values']['trip_data'];
  $dispatch_count = $form_state['values']['dispatch_count'];
  $trip_distributor_id = $form_state['values']['trip_distributor_id'];
  $central_distributor_id = $form_state['values']['central_distributor_id'];
  //print_r($dispatch_count);die;
  foreach ($dispatch_count as $key => $asset_dispatch_count) {
    if (!empty($asset_dispatch_count) || $asset_dispatch_count === "0") {
      $asset_nid = get_asset_nid($trip_data[$key]['asset_type'],$central_distributor_id ); 
      $trip_nid = get_trip_nid($trip_id,$trip_distributor_id);
      if (!empty($asset_nid) && !empty($trip_nid)) {
        $query = db_select('node', 'n');
        $query->condition('n.type', 'assets_count');
        $query->condition('n.uid', $trip_distributor_id);
        $query->condition('n.title', $trip_id);
        $query->join('field_data_field_asset_type', 'at', 'at.entity_id = n.nid');
        $query->condition('at.field_asset_type_nid', $asset_nid);
        $query->leftjoin('field_data_field_dispatch_count', 'dc', 'dc.entity_id = n.nid');
        $query->fields('dc', array('field_dispatch_count_value'));
        $query->fields('n', array('nid'));
        $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

        $nid = $res[0]['nid'];
        if ($nid) {
          $saved = 1;
          $node = node_load($nid);
          $node->field_dispatch_count['und'][0]['value'] = trim($asset_dispatch_count);
          node_save($node);
        }
        else {
          $saved = 1;
          $assets_count = new StdClass();
          $assets_count->type = 'assets_count';
          node_object_prepare($assets_count); // Sets some defaults. Invokes hook_prepare() and hook_node_prepare().
          $assets_count->language = LANGUAGE_NONE; // Or e.g. 'en' if locale is enabled
          //$assets_count->uid = $dis_id;
          $assets_count->distributorid = $trip_distributor_id;
          $assets_count->status = 1; //(1 or 0): published or not
          $assets_count->comment = 0; // 0 = comments disabled, 1 = read only, 2 = read/write
          $assets_count->title = $trip_id;
          $assets_count->field_asset_trip_id['und'][0]['nid'] = $trip_nid;
          $assets_count->field_asset_type['und'][0]['nid'] = $asset_nid;
          $assets_count->field_dispatch_count['und'][0]['value'] = trim($asset_dispatch_count);

          node_save($assets_count);
        }
      }
    }
  }
  if ($saved == 1) {
   drupal_set_message(t('Asset dispatched count saved successfully'));
  }
}

/**
 * 
 * @param array $form
 * @param type $form_state
 * @return array
 */
function _dispatch_assets_form_validate($form, &$form_state) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);
  $trip_id = $form_state['values']['trip_number'];
//  if($form_state['error'] == 1) {
//    form_set_error('trip_number', 'Invalid Trip ID.');
//  }
  foreach ($form_state['values']['dispatch_count'] as $key => $val) {
    if (!empty($val) && !is_numeric($val)) {
      form_set_error('dispatch_count', 'Dispatched count should be numeric.');
    }
    
    if(strlen($val) == 0){
      $flag = 0;
    }
    else {
      $flag = 1;
      break;
    }
  }
//  if ($flag == 0 && $form_state['error']!==1) {
  if ($flag == 0) {
    form_set_error('dispatch_count', 'Please enter Dispatched Assets for Trip ID ' . $trip_id);
  }
}

function _get_all_assets_type($trip_number,$distributor_id) {
  global $user;
  //$dis_id = change_feeds_import_get_distributor_id($user->uid);
  if (in_array('central_distributor', $user->roles)) {
    $central_distributor_id = change_feeds_import_get_distributor_id($user->uid);
  }
  else {
    // have to fetch parent distributor id
    $central_distributor_id = get_central_distributor_id($user->uid);
  }
  // Asset Type master belongs to central distributor
  $query = db_select('node', 'n');
  $query->condition('n.type', 'asset_type_master');
  $query->condition('n.uid', $central_distributor_id);
  $query->fields('n', array('nid', 'title'));
  $results = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
  if (!empty($results)) {
    $row = array();
    foreach ($results as $val) {
      $rows = array();

      $query = db_select('node', 'n');
      $query->condition('n.type', 'assets_count');
      $query->condition('n.uid', $distributor_id);
      $query->condition('n.title', $trip_number);
      $query->join('field_data_field_asset_type', 'at', 'at.entity_id = n.nid');
      $query->condition('at.field_asset_type_nid', $val['nid']);
      $query->leftjoin('field_data_field_dispatch_count', 'dc', 'dc.entity_id = n.nid');
      $query->leftjoin('field_data_field_planned_count', 'pc', 'pc.entity_id = n.nid');
      $query->fields('dc', array('field_dispatch_count_value'));
      $query->fields('pc', array('field_planned_count_value'));
      $count = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
      $rows['title'] = $val['title'];
      $rows['field_dispatch_count_value'] = $count[0]['field_dispatch_count_value'];
      $rows['field_planned_count_value'] = $count[0]['field_planned_count_value'];

      $row[] = $rows;
    }
  }
  return $row;
}

function get_asset_nid($asset_name,$central_distributor_id) {
  //global $user;
  //$dis_id = change_feeds_import_get_distributor_id($user->uid);
  // Asset belongs to central distributor
  $results = '';
  if(!empty($central_distributor_id)) {
    $query = db_select('node', 'n');
    $query->condition('n.type', 'asset_type_master');
    //$query->condition('n.uid', $dis_id);
    $query->condition('n.uid', $central_distributor_id);
    $query->condition('n.title', $asset_name);
    $query->fields('n', array('nid'));
    $results = $query->execute()->fetchField();
  }
  return $results;
}

function get_trip_nid($trip_id,$distributor_id) {
  //global $user;
 // $dis_id = change_feeds_import_get_distributor_id($user->uid);
  $results = '';
  if (!empty($distributor_id)) {
    $query = db_select('node', 'n');
    $query->condition('n.type', 'dispatch_data');
    //$query->condition('n.uid', $dis_id);
    $query->condition('n.uid', $distributor_id);
    $query->condition('n.title', $trip_id);
    $query->fields('n', array('nid'));
    $results = $query->execute()->fetchField();
  }

  return $results;
}
