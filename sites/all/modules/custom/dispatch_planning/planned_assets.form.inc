<?php

/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function _planned_assets_form($form, &$form_state) {
  $form = array();
  $form['#prefix'] = '<div id="itemlist-div">';
  $form['#suffix'] = '</div>';

  $form['trip_number'] = array(
    '#prefix' => '<div class="textfield-wrapper">',
    '#type' => 'textfield',
    '#title' => t('Trip ID'),
    '#required' => TRUE,
    '#autocomplete_path' => 'trip/autocomplete',
    '#ajax' => array(
      'wrapper' => 'itemlist-div',
      'callback' => 'planned_count_callback',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );
  // initialize editable elements here
  $form['planned_count'] = array('#type' => 'value');

  $trip_number = $form_state['input']['trip_number'];

  if (!empty($trip_number)) {
    $user_id = $GLOBALS['user']->uid;
    $distributor_id = change_feeds_import_get_distributor_id($user_id);
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
    /*
      if ($vrs_status == 0) {
        form_set_error('form', t('Trip is already in returns confirmation pending status.'));
        //drupal_set_message(t('Trip is already in returns confirmation pending status.'), 'error');
      }
      if ($vrs_status == 1) {
        form_set_error('form', t('Trip is already in returns confirmed status.'));
        //drupal_set_message(t('Trip is already in returns confirmed status.'), 'error');
      }
      else if ($vrs_status == 2) {
        form_set_error('form', t('Trip is already in VRS confirmed status.'));
        //drupal_set_message(t('Trip is already in VRS confirmed status.'), 'error');
      }
      else if ($vrs_status == 3) {
        form_set_error('form', t('Trip is already in VRS pending status.'));
        //drupal_set_message(t('Trip is already in VRS pending status.'), 'error');
      }
      else if ($vrs_status == 4) {
        form_set_error('form', t('Trip is already in Handover pending status.'));
        //drupal_set_message(t('Trip is already in Handover pending status.'), 'error');
      }
      else if ($vrs_status == 5) {
        form_set_error('form', t('Trip is already in Handover confirmed status.'));
        //drupal_set_message(t('Trip is already in Handover confirmed status.'), 'error');
      }
    */
    else {
      $item_details = _get_all_assets_type($trip_number);

      $item_details_header = array(
        'ASSET_TYPE' => 'ASSET NAME',
        'planned_count' => 'PLANNED COUNT'
      );

      foreach ($item_details as $key => &$value) {
        $item_details[$key]['planned_count']['data'] = array(
          '#type' => 'textfield',
          '#title' => t('Planned Count'),
          '#title_display' => 'invisible',
          '#size' => 20,
          '#value' => $value['count'],
          '#name' => "planned_count[$key]",
          '#attributes' => array('class' => array('asset-planned-count')),
        );
        unset($value['count']);
        $trip_data[$key]['asset_type'] = $value['type'];
      }

      $form['trip_data'] = array(
        '#type' => 'value',
        '#value' => $trip_data,
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
function planned_count_callback($form, &$form_state) {
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
function _planned_assets_form_submit($form, &$form_state) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);

  $trip_id = $form_state['values']['trip_number'];
  $trip_data = $form_state['values']['trip_data'];
  $planned_count = $form_state['values']['planned_count'];

  foreach ($planned_count as $key => $asset_planned_count) {
    if (!empty($asset_planned_count)) {
      $asset_nid = get_asset_nid($trip_data[$key]['asset_type']);
      $trip_nid = get_trip_nid($trip_id);

      $query = db_select('node', 'n');
      $query->condition('n.type', 'assets_count');
      $query->condition('n.uid', $dis_id);
      $query->condition('n.title', $trip_id);
      $query->join('field_data_field_asset_type', 'at', 'at.entity_id = n.nid');
      $query->condition('at.field_asset_type_nid', $asset_nid);
      $query->leftjoin('field_data_field_planned_count', 'pc', 'pc.entity_id = n.nid');
      $query->fields('pc', array('field_planned_count_value'));
      $query->fields('n', array('nid'));
      $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

      $nid = $res[0]['nid'];
      if ($nid) {
        $node = node_load($nid);
        $node->field_planned_count['und'][0]['value'] = trim($asset_planned_count);
        node_save($node);
      }
      else {
        $assets_count = new StdClass();
        $assets_count->type = 'assets_count';
        node_object_prepare($assets_count); // Sets some defaults. Invokes hook_prepare() and hook_node_prepare().
        $assets_count->language = LANGUAGE_NONE; // Or e.g. 'en' if locale is enabled
        $assets_count->uid = $dis_id;
        $assets_count->status = 1; //(1 or 0): published or not
        $assets_count->comment = 0; // 0 = comments disabled, 1 = read only, 2 = read/write
        $assets_count->title = $trip_id;
        $assets_count->field_asset_trip_id['und'][0]['nid'] = $trip_nid;
        $assets_count->field_asset_type['und'][0]['nid'] = $asset_nid;
        $assets_count->field_planned_count['und'][0]['value'] = trim($asset_planned_count);

        node_save($assets_count);
      }
    }
  }
  drupal_set_message(t('Asset planned count saved successfully'));
}

/**
 * 
 * @param array $form
 * @param type $form_state
 * @return array
 */
function _planned_assets_form_validate($form, &$form_state) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);

  foreach ($form_state['values']['planned_count'] as $key => $val) {
    if (!empty($val) && !is_numeric($val)) {
      form_set_error('planned_count', 'Planned count should be numeric.');
    }
  }
}

function _get_all_assets_type($trip_number) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);

  $query = db_select('node', 'n');
  $query->condition('n.type', 'asset_type_master');
  $query->condition('n.uid', $dis_id);
  $query->fields('n', array('nid', 'title'));
  $results = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
  if (!empty($results)) {
    $row = array();
    foreach ($results as $val) {
      $rows = array();

      $rows['type'] = $val['title'];
      $query = db_select('node', 'n');
      $query->condition('n.type', 'assets_count');
      $query->condition('n.uid', $dis_id);
      $query->condition('n.title', $trip_number);
      $query->join('field_data_field_asset_type', 'at', 'at.entity_id = n.nid');
      $query->condition('at.field_asset_type_nid', $val['nid']);
      $query->leftjoin('field_data_field_planned_count', 'pc', 'pc.entity_id = n.nid');
      $query->fields('pc', array('field_planned_count_value'));
      $planned_count = $query->execute()->fetchField();

      $rows['count'] = $planned_count;

      $row[] = $rows;
    }
  }
  return $row;
}

function get_asset_nid($asset_name) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);

  $query = db_select('node', 'n');
  $query->condition('n.type', 'asset_type_master');
  $query->condition('n.uid', $dis_id);
  $query->condition('n.title', $asset_name);
  $query->fields('n', array('nid'));
  $results = $query->execute()->fetchField();

  return $results;
}

function get_trip_nid($trip_id) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);

  $query = db_select('node', 'n');
  $query->condition('n.type', 'dispatch_data');
  $query->condition('n.uid', $dis_id);
  $query->condition('n.title', $trip_id);
  $query->fields('n', array('nid'));
  $results = $query->execute()->fetchField();

  return $results;
}
