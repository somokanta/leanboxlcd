<?php

module_load_include('inc', 'reverse_geocode', 'address_confirm');

function outlet_addition_form($form, &$form_state) {
  $form['#prefix'] = '<div id="incremental-outlet">';
  $form['#suffix'] = '</div>';
  drupal_add_js(drupal_get_path('module', 'reverse_geocode') . '/js/selectable-row-in-outlet.js', 'file');

  global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);


  $days = change_feeds_import_get_terms_using_vocubulary('Outlet Days');
  $sales_day_data = array_combine($days, $days);

  $form['sales_day'] = array(
    '#type' => 'select',
    '#title' => t('Sales Day'),
    '#options' => $sales_day_data,
    '#ajax' => array(
      'event' => 'change',
      'callback' => 'increamental_outlet_sales_day_ajax_callback',
      'wrapper' => 'van-pl-wrapper',
    ),
    '#required' => TRUE,
  );


  $form['van_pl_group'] = array(
    '#type' => 'select',
    '#title' => t('Van PL Group'),
    '#options' => array(),
    '#ajax' => array(
      'event' => 'change',
      'callback' => 'increamental_outlet_van_pl_group_ajax_callback',
      'wrapper' => 'area-hook-group-wrapper',
    ),
    '#prefix' => '<div id="van-pl-wrapper">',
    '#suffix' => '</div>',
    '#required' => TRUE,
    "#empty_option" => t('- Select -'),
  );


  $form['area_hook_group'] = array(
    '#type' => 'select',
    '#title' => t('Area Hook Group'),
    '#options' => array(),
    '#prefix' => '<div id="area-hook-group-wrapper">',
    '#suffix' => '</div>',
    '#required' => TRUE,
    "#empty_option" => t('- Select -'),
  );

  if (!empty($form_state['values']['sales_day'])) {
    $sales_day = $form_state['values']['sales_day'];
    $form['van_pl_group']['#options'] = reverse_geocode_get_data_from_sales_day($sales_day, $distributor_id);
  }

  if (!empty($form_state['values']['sales_day']) && !empty($form_state['values']['van_pl_group'])) {
    $sales_day = $form_state['values']['sales_day'];
    $van = $form_state['values']['van_pl_group'];
    $form['area_hook_group']['#options'] = reverse_geocode_get_data_for_area_group($sales_day, $van, $distributor_id);
  }

  $form['filter_submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
    '#ajax' => array(
      'wrapper' => 'increamental-outlet-form-wrapper',
      'callback' => 'increamental_outlet_filters_ajax',
    ),
    '#submit' => array('increamental_outlet_filters_submit'),
  );


  $form['results_container'] = array(
    '#prefix' => '<div id="increamental-outlet-form-wrapper">',
    '#suffix' => '</div>',
  );


  // Get newly added outlet which dont have Area Beat Code and Area Hook Cnfrm is done
  //$options = get_new_outlet_data($distributor_id);
  $options = $form_state['increamental_outlet']['results'];
  // This is just adding selected outlet to $option array to show nearest outlet all values.
  if (isset($form_state['close_array'])) {
    $show_array = $form_state['close_array'];
    $options = $show_array + $options;
  }
  $form_state['incremental_outlet']['options'] = $options;
  $class = array('selectable-row', 'table-radio');

  if (!empty($options)) {

    $header_array = array('title' => 'New Outlet',
      'field_party_name_value' => 'New Outlet Name',
      'field_dva_sales_day_value' => 'New O/L Sales Day',
      'field_dva_van_pl_group_value' => 'New O/L VAN_PL_GROUP',
      'field_dva_area_hook_group_name_value' => 'New O/L Area Hook Group',
      'Nearest Outlet Code' => 'Nearest Outlet Code',
      'Nearest Outlet Name' => 'Nearest Outlet Name',
      'Nearest Outlet Sequence' => 'Nearest Outlet Sequence',
      'Static Beat Code' => 'Static Beat Code',
      'Outlet_Sequence' => 'Outlet Sequence',
    );

    $form['results_container']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Confirm Incremental Outlet'),
      '#submit' => array('update_incremental_outlet_submit'),
      '#ajax' => array(
        'wrapper' => 'incremental-outlet',
        'callback' => 'incremental_outlet_ajax_callback',
        'method' => 'replace',
        'effect' => 'fade',
      ),
      '#states' => array(
        'visible' => array(
          ':input[name^="list_form_items"]' => array('checked' => TRUE),
        ),
      ),
      '#id' => 'incremental-outlet-confirm',
    );

    $form['results_container']['list_form_items'] = array(
      '#type' => 'tableselect',
      '#header' => $header_array,
      '#options' => $options,
      '#prefix' => '<div id="table-div">',
      '#suffix' => '</div>',
      '#empty' => t('No forms available.'),
      '#multiple' => FALSE,
      '#attributes' => array('class' => $class),
      '#default_value' => NULL,
    );
  }

  return $form;
}

function increamental_outlet_sales_day_ajax_callback(&$form, &$form_state) {
  return $form['van_pl_group'];
}

function increamental_outlet_van_pl_group_ajax_callback(&$form, &$form_state) {
  return $form['area_hook_group'];
}

function increamental_outlet_filters_ajax(&$form, &$form_state) {
  return $form['results_container'];
}

function increamental_outlet_filters_submit(&$form, &$form_state) {

  $sales_day = $form_state['values']['sales_day'];
  $van_pl_group = $form_state['values']['van_pl_group'] ? $form_state['values']['van_pl_group'] : NULL;
  $area_hook_group = $form_state['values']['area_hook_group'] ? $form_state['values']['area_hook_group'] : NULL;

  $results = increamental_outlet_get_results($sales_day, $van_pl_group, $area_hook_group);

  if (empty($results)) {
    $form_state['increamental_outlet']['results'] = array();
    drupal_set_message('No results found', 'error');
  }
  else {
    $form_state['increamental_outlet']['results'] = $results;
  }

  $form_state['rebuild'] = TRUE;
}

// When SUbmit is clicked
function update_incremental_outlet_submit($form, &$form_state) {
  global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);
  $party_code = $form_state['values']['list_form_items']; // selected party code
  $new_outlet_array = $form_state['incremental_outlet']['options']; // already calculated outlet value.  
  foreach ($new_outlet_array as $key => $value) {
    if ($key == $party_code) {
      $van_pl = $value['field_dva_van_pl_group_value'];
      $area_hook = $value['field_dva_area_hook_group_name_value'];
      // get all node's of outlet master and lat long from hul master which has same pl group and area hook group of selected outlet node.
      $selected_results = get_selected_party_code_similar_outlet_data_with_lat_lng($van_pl, $area_hook, $distributor_id);
      foreach ($selected_results as $k => $v) {
        $lat = $v->field_hul_updated_lat_value;
        $lng = $v->field_hul_updated_long_value;
        $hul_code = $v->title;
        $nid = $v->nid;

        $distsnce_in_km = distance($selected_lat, $selected_lng, $lat, $lng);
        $distsnce_in_meter[] = $distsnce_in_km * 1000;
        $distsnce_hul_code_array[] = $hul_code;
        $distsnce_nid_array[] = $nid;
      }
      $index = array_keys($distsnce_in_meter, min($distsnce_in_meter));
      $index_id = $index[0];
      $closet_hul_code = $distsnce_hul_code_array[$index_id];
      $closet_outlet_nid = $distsnce_nid_array[$index_id];
      // Get result of closest outlet node.
      $closet_result = get_closet_result_from_closet_hul_code($closet_outlet_nid, $distributor_id);
      $nearest_outlet_code = $closet_result[0]->title;
      $nearest_outlet_name = $closet_result[0]->field_party_name_value;
      $closest_sales_delivery_beat = $closet_result[0]->field_static_delivery_beat_code_value;
      $closest_sequence = $closet_result[0]->field_sequence_value;
      $nearest_outlet_sequence = $closest_sequence;


      // logic for finding closest sequence.
      $get_all_closest_sequence = get_all_closest_sequence($van_pl, $area_hook, $distributor_id);
      $flag = 1;
      while ($flag) {
        $closest_sequence = $closest_sequence + 0.001;
        if (!isset($get_all_closest_sequence["$closest_sequence"])) {
          $flag = 0;
        }
      }

      // saving sequence and static_delivery_beat_code in selected outlet node.
      $selected_nid = $value['nid'];
      if (!empty($closet_result)) {
        $node = node_load($selected_nid);
        $node->field_sequence['und'][0]['value'] = $closest_sequence;
        $node->field_static_delivery_beat_code['und'][0]['value'] = $closest_sales_delivery_beat;
        node_save($node);

        $closest_array = array('Nearest Outlet Code' => $nearest_outlet_code, 'Nearest Outlet Name' => $nearest_outlet_name,
          'Nearest Outlet Sequence' => $nearest_outlet_sequence, 'Static Beat Code' => $closest_sales_delivery_beat,
          'Outlet_Sequence' => $closest_sequence);

        $show_array[$party_code] = $new_outlet_array[$key] + $closest_array;
        $form_state['close_array'] = $show_array;
      }

      if (empty($closet_result) && empty($selected_results)) {
        drupal_set_message('No Closest Outlet Found');
      }
      $form_state['rebuild'] = TRUE;
    }
  }
}

function incremental_outlet_ajax_callback($form, &$form_state) {
  return $form;
}

function get_all_closest_sequence($van_pl, $area_hook_group, $distributor_id) {
  $query = db_select('node', 'n');
  $query->condition('n.uid', $distributor_id, '=');
//$query->condition('n.title', $tt ,'=');
//Outlet Master
  $query->join('field_data_field_hul_code', 'fh', 'fh.entity_id = n.nid');
  $query->join('field_data_field_area_hook', 'fa', 'fa.entity_id = n.nid');
  $query->join('field_data_field_static_delivery_beat_code', 'dbc2', 'dbc2.entity_id = n.nid');
  $query->isNotNull('dbc2.field_static_delivery_beat_code_value');

  $query->join('field_data_field_area_hook_group_hul', 'faconf', 'faconf.entity_id = n.nid');
  $query->condition('faconf.field_area_hook_group_hul_value', 1);
  $query->join('field_data_field_sequence', 'fsq', 'fsq.entity_id = n.nid');
  $query->condition('n.type', 'outlet_master');

  //Area hook Group Master
  $query->join('node', 'na', 'na.nid = fa.field_area_hook_nid');
  $query->join('field_data_field_dva_area_hook_group_name', 'agr', 'agr.entity_id = na.nid');
  $query->condition('agr.field_dva_area_hook_group_name_value', $area_hook_group, '=');

  $query->join('field_data_field_dva_van_pl_group', 'ppplg', 'na.nid = ppplg.entity_id');
  $query->condition('ppplg.field_dva_van_pl_group_value', $van_pl, '=');

  $query->fields('fsq', array('field_sequence_value'));
  $res = $query->execute()->fetchAllKeyed(0, 0);
  return $res;
}

function get_closet_result_from_closet_hul_code($closet_outlet_nid, $distributor_id) {
  $query = db_select('node', 'n');
  $query->condition('n.uid', $distributor_id, '=');
  $query->condition('n.nid', $closet_outlet_nid, '=');
//Outlet Master
  $query->join('field_data_field_static_delivery_beat_code', 'dbc', 'dbc.entity_id = n.nid');
  $query->join('field_data_field_party_name', 'fp', 'fp.entity_id = n.nid');
  $query->join('field_data_field_sequence', 'fsq', 'fsq.entity_id = n.nid');
  $query->fields('fp', array('field_party_name_value'));
  $query->fields('dbc', array('field_static_delivery_beat_code_value'));
  $query->fields('fsq', array('field_sequence_value'));
  $query->fields('n', array('title'));
  $res = $query->execute()->fetchAll();
  return $res;
}

function distance($lat1, $lon1, $lat2, $lon2) {

  $pi80 = M_PI / 180;
  $lat1 *= $pi80;
  $lon1 *= $pi80;
  $lat2 *= $pi80;
  $lon2 *= $pi80;

  $r = 6372.797; // mean radius of Earth in km
  $dlat = $lat2 - $lat1;
  $dlon = $lon2 - $lon1;
  $a = sin($dlat / 2) * sin($dlat / 2) + cos($lat1) * cos($lat2) * sin($dlon / 2) * sin($dlon / 2);
  $c = 2 * atan2(sqrt($a), sqrt(1 - $a));
  $km = $r * $c;

  //echo '<br/>'.$km;
  return $km;
}

function get_selected_party_code_similar_outlet_data_with_lat_lng($van_pl, $area_hook_group, $distributor_id) {

  ######### NEW LOGIC###############
  $query = db_select('node', 'n');
  $query->condition('n.uid', $distributor_id, '=');

//Outlet Master
  $query->join('field_data_field_area_hook', 'fa', 'fa.entity_id = n.nid');
  $query->join('field_data_field_hul_code', 'fh', 'fh.entity_id = n.nid');
  $query->join('field_data_field_static_delivery_beat_code', 'dbc2', 'dbc2.entity_id = n.nid');
  $query->isNotNull('dbc2.field_static_delivery_beat_code_value');

  $query->join('field_data_field_area_hook_group_hul', 'faconf', 'faconf.entity_id = n.nid');
  $query->condition('faconf.field_area_hook_group_hul_value', 1);
  $query->condition('n.type', 'outlet_master');

//Area hook Master
  $query->join('node', 'na', 'na.nid = fa.field_area_hook_nid');

  $query->join('field_data_field_dva_area_hook_group_name', 'agr', 'agr.entity_id = na.nid');
  $query->condition('agr.field_dva_area_hook_group_name_value', $area_hook_group, '=');
  //$query->groupBy('agr.field_dva_area_hook_group_name_value');

  $query->join('field_data_field_dva_van_pl_group', 'ppplg', 'na.nid = ppplg.entity_id');
  $query->condition('ppplg.field_dva_van_pl_group_value', $van_pl, '=');



//HUL code Master
  $query->join('node', 'nh', 'nh.title = fh.field_hul_code_value');
  $query->join('field_data_field_hul_status', 'fhs', 'fhs.entity_id = nh.nid');
  $query->condition('fhs.field_hul_status_value', 1);
  $query->Join('field_data_field_hul_updated_lat', 'hul_lat_u', 'hul_lat_u.entity_id = nh.nid');
  $query->fields('hul_lat_u', array('field_hul_updated_lat_value'));
  $query->Join('field_data_field_hul_updated_long', 'hul_long_u', 'hul_long_u.entity_id = nh.nid');
  $query->fields('hul_long_u', array('field_hul_updated_long_value'));
  $query->condition('nh.uid', $distributor_id, '=');

  $query->fields('n', array('nid'));
  $query->fields('nh', array('title'));
  $res = $query->execute()->fetchAll();
  return $res;
}

function increamental_outlet_get_results($sales_day, $van_pl_group, $area_hook_group) {

//  $query = db_select('node', 'n');
//  $query->condition('n.type', 'outlet_master');
//  $query->condition('n.uid', $distributor_id, '=');
//
//// Outlet Master
//  $query->join('field_data_field_hul_code', 'fh', 'fh.entity_id = n.nid');
//  $query->leftjoin('field_data_field_static_delivery_beat_code', 'dbc', 'dbc.entity_id = n.nid');
//  $query->leftjoin('field_data_field_area_hook_group_hul', 'fagh', 'fagh.entity_id = n.nid');
//  $query->join('field_data_field_party_name', 'pn', 'pn.entity_id = n.nid');
//  $query->join('field_data_field_sales_day', 'sd', 'sd.entity_id = n.nid');
//  $query->join('field_data_field_area_hook', 'ah', 'ah.entity_id = n.nid');
//  $query->join('field_data_field_area_hook_group', 'agr', 'agr.entity_id = ah.field_area_hook_nid');
//  $query->join('field_data_field_channel', 'ch', 'ch.entity_id = n.nid');
//  $query->join('field_data_field_pl_group', 'pl', 'pl.entity_id = n.nid');
//
//  $query->isNull('dbc.field_static_delivery_beat_code_value');
//  $query->condition('fagh.field_area_hook_group_hul_value', 1, '=');
//  $query->condition('ch.bundle', 'outlet_master');
//  $query->condition('pl.bundle', 'outlet_master');
//
////Party Pack Master
//  $query->join('field_data_field_channel', 'ppmch', 'ch.field_channel_value = ppmch.field_channel_value');
//  $query->join('node', 'ppn', 'ppn.nid = ppmch.entity_id');
//  $query->join('field_data_field_pl_group', 'ppmpl', 'pl.field_pl_group_value = ppmpl.field_pl_group_value and ppmch.entity_id = ppmpl.entity_id');
//  $query->join('field_data_field_van_pl_group', 'ppplg', 'ppn.nid = ppplg.entity_id');
//  $query->condition('ppmch.bundle', 'party_pack_master');
//  $query->condition('ppn.uid', $distributor_id, '=');
//  $query->condition('ppmch.bundle', 'party_pack_master');
//
////HUL code Master
//  $query->join('node', 'nh', 'nh.title = fh.field_hul_code_value');
//  $query->Join('field_data_field_hul_updated_lat', 'hul_lat_u', 'hul_lat_u.entity_id = nh.nid');
//  $query->Join('field_data_field_hul_updated_long', 'hul_long_u', 'hul_long_u.entity_id = nh.nid');
//
//  $query->fields('n', array('title'));
//  $query->fields('pn', array('field_party_name_value'));
//  $query->fields('sd', array('field_sales_day_value'));
//  $query->fields('ppplg', array('field_van_pl_group_value'));
//  $query->fields('agr', array('field_area_hook_group_value'));
//  $query->fields('hul_lat_u', array('field_hul_updated_lat_value'));
//  $query->fields('hul_long_u', array('field_hul_updated_long_value'));
//  $query->fields('n', array('nid'));
//  $selected_res = $query->execute()->fetchAllAssoc('title', PDO::FETCH_ASSOC);
//  return $selected_res;
  ##### NEW LOGIC ########
  global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);

  $query = db_select('node', 'n');

  $query->addTag('node_uid');

  //Outlet Master
  $query->join('field_data_field_hul_code', 'fh', 'fh.entity_id = n.nid');
  $query->leftjoin('field_data_field_static_delivery_beat_code', 'dbc', 'dbc.entity_id = n.nid');
  $query->isNull('dbc.field_static_delivery_beat_code_value');
  $query->leftjoin('field_data_field_area_hook_group_hul', 'fagh', 'fagh.entity_id = n.nid');
  $query->join('field_data_field_party_name', 'fp', 'fp.entity_id = n.nid');

  $query->condition('n.type', 'outlet_master');
  $query->join('field_data_field_area_hook', 'fa', 'fa.entity_id = n.nid');

  //Area hook Master
  $query->join('node', 'na', 'na.nid = fa.field_area_hook_nid');

  $query->join('field_data_field_dva_sales_day', 'sd', 'sd.entity_id = na.nid');
  $query->condition('sd.field_dva_sales_day_value', $sales_day, '=');
  $query->join('field_data_field_dva_area_hook_group_name', 'agr', 'agr.entity_id = na.nid');
  $query->condition('agr.field_dva_area_hook_group_name_value', $area_hook_group, '=');
  $query->join('field_data_field_dva_van_pl_group', 'ppplg', 'na.nid = ppplg.entity_id');
  $query->condition('ppplg.field_dva_van_pl_group_value', $van_pl_group, '=');


//HUL code Master
  $query->join('node', 'nh', 'nh.title = fh.field_hul_code_value');
  $query->Join('field_data_field_hul_updated_lat', 'hul_lat_u', 'hul_lat_u.entity_id = nh.nid');
  $query->Join('field_data_field_hul_updated_long', 'hul_long_u', 'hul_long_u.entity_id = nh.nid');
  $query->condition('nh.uid', $distributor_id, '=');


  $query->fields('n', array('title'));
  $query->fields('fp', array('field_party_name_value'));
  $query->fields('sd', array('field_dva_sales_day_value'));
  $query->fields('ppplg', array('field_dva_van_pl_group_value'));
  $query->fields('agr', array('field_dva_area_hook_group_name_value'));
  $query->fields('hul_lat_u', array('field_hul_updated_lat_value'));
  $query->fields('hul_long_u', array('field_hul_updated_long_value'));
  $query->fields('n', array('nid'));

  $selected_res = $query->execute()->fetchAllAssoc('title', PDO::FETCH_ASSOC);
  return $selected_res;
}
