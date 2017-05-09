<?php

function show_area_locations() {
  $url = arg(0);
  $license_key = variable_get('mapmyindia_map_key', '98thppnrdg3vpp7xupfg9eotrl2rliqu');
  $google_key = variable_get('google_places_key', 'AIzaSyBUpfJiBx6AmZY6vPa4nyYjOmU1kSsHxSQ');
  drupal_add_js("https://apis.mapmyindia.com/advancedmaps/v1/$license_key/map_load?v=0.1", 'external');
  drupal_add_js("https://maps.googleapis.com/maps/api/js?key=$google_key&libraries=places", 'external');
  drupal_add_js(drupal_get_path('module', 'reverse_geocode') . '/js/selectable-row.js', 'file');
  drupal_add_js(drupal_get_path('module', 'reverse_geocode') . '/js/leaflet.smoothmarkerbouncing.js', 'file');
  drupal_add_js(drupal_get_path('module', 'reverse_geocode') . '/js/multi-map.js', 'file');

  if ($url == "area-confirm") {
    $type = "area-checkbox";
  }
  elseif ($url == "area-confirm-view") {
    //  drupal_add_js(drupal_get_path('module', 'reverse_geocode') . '/js/multi-map-view.js', 'file');
    $type = "area-view";
  }


  $form = drupal_render(drupal_get_form('area_locations_form', $type));
  $result['map-container'] = array(
    '#markup' => '<div id="map-container"></div>',
  );

  return $form . render($result);
}

function reverse_geocode_get_data_for_sales_day($distributor_id) {
  $query = db_select('node', 'n');
  $query->condition('n.type', 'outlet_master');
  $query->addTag('node_uid');
  $query->leftjoin('field_data_field_sales_day', 'sd', 'sd.entity_id = n.nid');
  $query->fields('sd', array('field_sales_day_value'));
  $query->groupBy('field_sales_day_value');
  $res = $query->execute()->fetchallkeyed(0, 0);
  return $res;
}

function sales_day_callback($form, &$form_state) {
  return $form;
}

function reverse_geocode_get_data_from_sales_day($sales_day, $distributor_id) {
//  $query = db_select('node', 'n');
//  $query->condition('n.type', 'outlet_master');
//  $query->condition('n.uid', $distributor_id, '=');
//  if (!is_null($sales_day)) {
//    $query->join('field_data_field_sales_day', 'sd', 'sd.entity_id = n.nid');
//    $query->condition('sd.field_sales_day_value', $sales_day, '=');
//  }
//  $query->join('field_data_field_channel', 'ch', 'ch.entity_id = n.nid');
//  $query->condition('ch.bundle', 'outlet_master');
//  $query->join('field_data_field_pl_group', 'pl', 'pl.entity_id = n.nid');
//  $query->condition('pl.bundle', 'outlet_master');
//  $query->join('field_data_field_channel', 'ppmch', 'ch.field_channel_value = ppmch.field_channel_value');
//  $query->condition('ppmch.bundle', 'party_pack_master');
//  $query->join('node', 'ppn', 'ppn.nid = ppmch.entity_id');
//  $query->condition('ppn.uid', $distributor_id, '=');
//  $query->join('field_data_field_pl_group', 'ppmpl', 'pl.field_pl_group_value = ppmpl.field_pl_group_value and ppmch.entity_id = ppmpl.entity_id');
//  $query->condition('ppmch.bundle', 'party_pack_master');
//  $query->join('field_data_field_van_pl_group', 'ppplg', 'ppmpl.entity_id = ppplg.entity_id');
//  $query->fields('ppplg', array('field_van_pl_group_value'));
//  $res = $query->execute()->fetchallkeyed(0, 0);
//
//  return $res;
  ##### NEW LOGIC ##############################
  $query = db_select('node', 'n');
  $query->addTag('node_uid');

  //Outlet Master
  $query->join('field_data_field_area_hook', 'fa', 'fa.entity_id = n.nid');

  //Day-Van-PL-Gp-Area Hook Master
  $query->join('node', 'na', 'na.nid = fa.field_area_hook_nid');
  $query->join('field_data_field_dva_van_pl_group', 'fv', 'fv.entity_id = na.nid');
  if (!is_null($sales_day)) {
    $query->join('field_data_field_dva_sales_day', 'sd', 'sd.entity_id = na.nid');
    $query->condition('sd.field_dva_sales_day_value', $sales_day, '=');
  }
  $query->fields('fv', array('field_dva_van_pl_group_value'));

  $res = $query->execute()->fetchallkeyed(0, 0);
  return $res;
}

function reverse_geocode_get_data_for_area_group($sales_day, $van_pl_group, $distributor_id) {
//  $query = db_select('node', 'n');
//  $query->condition('n.type', 'party_pack_master');
//  $query->condition('n.uid', $distributor_id, '=');
//  $query->join('field_data_field_van_pl_group', 'ppplg', 'n.nid = ppplg.entity_id');
//  $query->condition('ppplg.field_van_pl_group_value', $van_pl_group, '=');
//  $query->join('field_data_field_channel', 'ch', 'ch.entity_id = n.nid');
//  $query->condition('ch.bundle', 'party_pack_master');
//  $query->join('field_data_field_pl_group', 'ppmpl', 'ppmpl.entity_id = n.nid');
//  $query->condition('ppmpl.bundle', 'party_pack_master');
//  $query->join('field_data_field_channel', 'ppmch', 'ch.field_channel_value = ppmch.field_channel_value');
//  $query->condition('ppmch.bundle', 'outlet_master');
////$query->fields('ppmch', array('field_channel_value'));
//  $query->join('node', 'oo', 'oo.nid = ppmch.entity_id');
//  $query->condition('oo.type', 'outlet_master');
//  $query->condition('oo.uid', $distributor_id, '=');
//  $query->join('field_data_field_pl_group', 'outpl', 'ppmpl.field_pl_group_value = outpl.field_pl_group_value and ppmch.entity_id = outpl.entity_id');
//  $query->condition('ppmch.bundle', 'outlet_master');
////$query->fields('outpl', array('field_pl_group_value'));
//  if (!is_null($sales_day)) {
//    $query->join('field_data_field_sales_day', 'sd', 'sd.entity_id = oo.nid');
//    $query->condition('sd.field_sales_day_value', $sales_day, '=');
//  }
//  $query->join('field_data_field_area_hook', 'ah', 'ah.entity_id = oo.nid');
////$query->fields('ah', array('field_area_hook_nid'));
//  $query->join('node', 'ag', 'ag.nid = ah.field_area_hook_nid');
//  $query->condition('ag.uid', $distributor_id, '=');
//  $query->join('field_data_field_area_hook_group', 'agr', 'agr.entity_id = ag.nid');
//  $query->fields('agr', array('field_area_hook_group_value'));
//  $res = $query->execute()->fetchallkeyed(0, 0);
//  return $res;
  ##### NEW LOGIC ##############################
  $query = db_select('node', 'n');
  $query->addTag('node_uid');

  //Outlet Master
  $query->join('field_data_field_area_hook', 'fa', 'fa.entity_id = n.nid');

  //Day-Van-PL-Gp-Area Hook Master
  $query->join('node', 'na', 'na.nid = fa.field_area_hook_nid');
  $query->join('field_data_field_dva_van_pl_group', 'fv', 'fv.entity_id = na.nid');
  $query->join('field_data_field_dva_area_hook_group_name', 'agr', 'agr.entity_id = na.nid');
  if (!is_null($sales_day)) {
    $query->join('field_data_field_dva_sales_day', 'sd', 'sd.entity_id = na.nid');
    $query->condition('sd.field_dva_sales_day_value', $sales_day, '=');
  }
  $query->condition('fv.field_dva_van_pl_group_value', $van_pl_group, '=');
  $query->fields('agr', array('field_dva_area_hook_group_name_value'));
  $res = $query->execute()->fetchallkeyed(0, 0);
  return $res;
}

function area_locations_form($form, &$form_state, $type) {

  global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);
  $sales_day_data = reverse_geocode_get_data_for_sales_day($distributor_id);
  $form['#prefix'] = '<div id="address-selection-map">';
  $form['#suffix'] = '</div>';
  $form_state['type'] = $type;

  if ($type == "area-checkbox") {
    $header_array = array(
      'sr_no' => array('data' => 'Sr No', 'class' => 'sr-cell'),
      'party_code' => array('data' => 'Party Code', 'class' => 'party-code-cell'),
      'party_name' => array('data' => 'Party Name', 'class' => 'party-name-cell'),
      'area_group_status' => array('data' => 'Status', 'class' => 'status-cell'),
      'details' => array('data' => '', 'class' => 'details-cell'),
    );
  }
  else {
    $header_array = array(
      'sr_no' => array('data' => 'Sequence No', 'class' => 'sr-cell'),
      'party_code' => array('data' => 'Party Code', 'class' => 'party-code-cell'),
      'party_name' => array('data' => 'Party Name', 'class' => 'party-name-cell'),
      'area_group_status' => array('data' => 'Status', 'class' => 'status-cell'),
      'details' => array('data' => '', 'class' => 'details-cell'),
    );
  }

  $form['sales_day'] = array(
    '#type' => 'select',
    '#title' => t('Sales Day'),
//    '#prefix' => '<div id="address-selection-map">',
//    '#suffix' => '</div>',
    // '#default_value' => isset($form_state['values']['sales_day']) ? $form_state['values']['sales_day'] : NULL,
    '#options' => $sales_day_data,
    '#ajax' => array(
      'event' => 'change',
      'callback' => 'sales_day_callback',
      'wrapper' => 'address-selection-map',
    ),
    '#required' => TRUE,
  );


  $form['van_pl_group'] = array(
    '#type' => 'select',
    '#title' => t('Van PL Group'),
//    '#prefix' => '<div id="address-selection-map">',
//    '#suffix' => '</div>',
    // '#default_value' => isset($form_state['values']['sales_day']) ? $form_state['values']['sales_day'] : NULL,
    '#options' => array(),
    '#ajax' => array(
      'event' => 'change',
      'callback' => 'sales_day_callback',
      'wrapper' => 'address-selection-map',
    ),
    '#required' => TRUE,
  );


  $form['area_hook_group'] = array(
    '#type' => 'select',
    '#title' => t('Area Hook Group'),
//    '#prefix' => '<div id="address-selection-map">',
//    '#suffix' => '</div>',
    // '#default_value' => isset($form_state['values']['sales_day']) ? $form_state['values']['sales_day'] : NULL,
    '#options' => array(),
    '#required' => TRUE,
  );
  if (!empty($form_state['values']['sales_day'])) {
    $sales_day = $form_state['values']['sales_day'];
    $rrr = reverse_geocode_get_data_from_sales_day($sales_day, $distributor_id);
    $form['van_pl_group']['#options'] = reverse_geocode_get_data_from_sales_day($sales_day, $distributor_id);
  }

  if (!empty($form_state['values']['sales_day']) && !empty($form_state['values']['van_pl_group'])) {
    $sales_day = $form_state['values']['sales_day'];
    $van = $form_state['values']['van_pl_group'];
    $form['area_hook_group']['#options'] = reverse_geocode_get_data_for_area_group($sales_day, $van, $distributor_id);
  }

//  if ($type == "area-view") {
//    $form['dummy'] = array(
//      "#markup" => '<div class="clearance"></div>',
//        //'#required' => TRUE,
//    );
//    
//  }
  $area_status = array(0 => "Non-Confirmed", 1 => "Confirmed");
  $form['area_status'] = array(
    '#type' => 'select',
    '#title' => t('status'),
    '#options' => $area_status,
    "#empty_option" => t('- Select -'),
  );

  $form['filter'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
    '#ajax' => array(
      'wrapper' => 'address-selection-map',
      'callback' => 'filter_area_wise',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    '#validate' => array('area_wise_filter_validate'),
    '#submit' => array('area_wise_filters_submit'),
    '#id' => 'area-filter',
  );


  if (!isset($form_state['reverse_geocode']['rows'])) {
    $form_state['reverse_geocode']['rows'] = array();
  }
  $options = $form_state['reverse_geocode']['rows'];
  if ($options) {
    if ($type == "area-checkbox") {
      $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Confirm Area Hook Gp'),
        '#submit' => array('update_status_submit'),
        '#ajax' => array(
          'wrapper' => 'address-selection-map',
          'callback' => 'area_hook_ajax_callback',
          'method' => 'replace',
          'effect' => 'fade',
        ),
        '#states' => array(
          'visible' => array(
            ':input[name^="list_form_items"]' => array('checked' => TRUE),
          ),
        ),
        '#id' => 'area-hook-confirm',
      );
    }

    $multiple = TRUE;
    $class = array('selectable-row', 'table-checkbox');
    $form['list_form_items'] = array(
      '#type' => 'tableselect',
      '#header' => $header_array,
      '#options' => $options,
      '#prefix' => '<div id="table-div">',
      '#suffix' => '</div>',
      '#empty' => t('No forms available.'),
      '#multiple' => $multiple,
      '#attributes' => array('class' => $class),
      '#default_value' => NULL,
    );
  }
  return $form;
}

function update_status_submit(&$form, &$form_state) {
  $party_code = $form_state['values']['list_form_items'];
  foreach ($party_code as $code) {
    reverse_geocode_update_area_hook_status($code);
    if (isset($form_state['reverse_geocode']['rows'][$code])) {
      unset($form_state['reverse_geocode']['rows'][$code]);
    }
  }
  $i = 0;
  foreach ($form_state['reverse_geocode']['rows'] as $key => &$value) {
    $value['sr_no'] = $value['#attributes']['data-sr'] = ++$i;
  }
  $form_state['rebuild'] = TRUE;
}

function reverse_geocode_update_area_hook_status($code) {
  module_load_include('inc', 'change_feeds_import', 'form/upload_billing_data');
  $nid = check_node_title_exists($code, 'outlet_master');
  if ($nid) {
    $node = node_load($nid);
    $node->field_area_hook_group_hul['und'][0]['value'] = 1;
    node_save($node);
  }
}

function area_hook_ajax_callback($form, &$form_state) {
  $commands[] = ajax_command_replace('#address-selection-map', render($form));
  $commands[] = array
    (
    // The command will be used in our JavaScript file (see next section)
    'command' => 'filter_clicked',
      // We pass the value that the user selected in the select element to our
      // JavaScript function:
      //'selectedValue' => 123,
  );
  $commands[] = ajax_command_prepend('#address-selection-map', theme('status_messages'));
  // And finally, we will return all of our commands to the system to be executed:
  return array('#type' => 'ajax', '#commands' => $commands);
}

function filter_area_wise($form, &$form_state) {
  $commands[] = ajax_command_replace('#address-selection-map', render($form));
  $commands[] = array
    (
    // The command will be used in our JavaScript file (see next section)
    'command' => 'filter_clicked',
      // We pass the value that the user selected in the select element to our
      // JavaScript function:
      //'selectedValue' => 123,
  );
  $commands[] = ajax_command_prepend('#address-selection-map', theme('status_messages'));
  // And finally, we will return all of our commands to the system to be executed:
  return array('#type' => 'ajax', '#commands' => $commands);
}

function area_wise_filter_validate(&$form, &$form_state) {
  $sales_day = $form_state['values']['sales_day'];
  $van_pl_group = $form_state['values']['van_pl_group'];
  $area_hook_group = $form_state['values']['area_hook_group'];
  $area_status = $form_state['values']['area_status'];
  $type = $form_state['type'];
   $data = "";

  if (!empty($sales_day) && !empty($van_pl_group) && !empty($area_hook_group)) {
    $data = reverse_geocode_get_data_area_code_wise($sales_day, $van_pl_group, $area_hook_group, $area_status, $type);
  }
  if (empty($data)) {
    $message = "No Results Found";
    unset($form_state['complete form']['list_form_items']);
    unset($form_state['complete form']['submit']);
    form_set_error('area_hook_group', $message);
  }
  else {
    $form_state['values']['area_hook_data'] = $data;
  }
}

function area_wise_filters_submit(&$form, &$form_state) {
  unset($form_state['input']['list_form_items']);
  $sales_day = $form_state['values']['sales_day'];
  $van_pl_group = $form_state['values']['van_pl_group'];
  $area_hook_group = $form_state['values']['area_hook_group'];
  $data = $form_state['values']['area_hook_data'];
  $type = $form_state['type'];


  foreach ($data as $key => $val) {
    $lat = $val['field_hul_updated_lat_value'];
    $long = $val['field_hul_updated_long_value'];
    $party_code = $val['title'];
    $party_name = $val['field_party_name_value'];
    $hul_status = $val['field_area_hook_group_hul_value'];

    if ($type == "area-view") {
      $sequence = $val['field_sequence_value'];
    }

    $new_city = $val['field_updated_address_sub_administrative_area'];
    $new_district = $val['field_updated_address_premise'];
    $new_street = $val['field_updated_address_sub_premise'];
    $new_locality = $val['field_updated_address_locality'];
    $new_sublocality = $val['field_updated_address_dependent_locality'];
    $new_poi = $val['field_updated_address_data'];
    $new_lat_long = !empty($lat) ? $lat . ',' . $long : '';

    $marker_tooltip = '<div class="cell-address cell-hide"><table>'
        . '<tr><td style="white-space:nowrap;color:#222">Party code</td><td>' . $party_code . '</td></tr>'
        . '<tr><td style="white-space:nowrap;color:#222">Locality</td><td>' . $new_locality . '</td></tr>'
        . '<tr><td style="white-space:nowrap;color:#222">Sub Locality</td><td>' . $new_sublocality . '</td></tr>'
        . '<tr><td style="white-space:nowrap;color:#222">Street</td><td>' . $new_street . '</td></tr>'
        . '<tr><td style="white-space:nowrap;color:#222">POI</td><td>' . $new_poi . '</td></tr>'
        . '<tr><td style="white-space:nowrap;color:#222">Lat/Long</td><td>' . $new_lat_long . '</td></tr>'
        . '</table></div>';


    if ($hul_status == "0") {
      $hul_status = "Non-Confirmed";
    }
    elseif ($hul_status == "1") {
      $hul_status = "Confirmed";
    }

    if ($type == "area-view") {
      $no = floatval($sequence);
    }
    else {
      $no = $key + 1;
    }

    $options[$party_code] = array(
      'sr_no' => array('data' => $no, 'class' => 'sr-cell'),
      'party_code' => array('data' => $party_code, 'class' => 'party-code-cell'),
      'party_name' => array('data' => '<div class="party-name-data">' . $party_name . '</div>', 'class' => 'party-name-cell'),
      '#attributes' => array('data-lat' => $lat, 'data-lng' => $long, 'data-sr' => $no),
      'area_group_status' => array('data' => $hul_status, 'class' => 'status-cell'),
      'details' => array('data' => array('#markup' => '<a class="cell-details" style="display:none;">On Map</a>' . $marker_tooltip), 'class' => 'details-cell'),
    );
  }
  $form_state['reverse_geocode']['rows'] = $options;
  $form_state['rebuild'] = TRUE;
}

function reverse_geocode_get_data_area_code_wise($sales_day, $van_pl_group, $area_hook_group, $area_status, $type) {
  global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);

  $query = db_select('node', 'n');
  $query->condition('n.type', 'outlet_master');
  $query->addTag('node_uid');

  $query->join('field_data_field_party_name', 'fpn', 'fpn.entity_id = n.nid');
  $query->leftjoin('field_data_field_area_hook_group_hul', 'fagh', 'fagh.entity_id = n.nid');
  if ($type == "area-checkbox") {
    if (!is_null($area_status)) {
      if ($area_status === '0' || $area_status === '1') {
        $query->condition('fagh.field_area_hook_group_hul_value', $area_status, '=');
        $query->fields('fagh', array('field_area_hook_group_hul_value'));
      }
    }

    $query->orderBy('hul_lat_u.field_hul_updated_lat_value', 'asc');
    $query->orderBy('hul_long_u.field_hul_updated_long_value', 'asc');
  }
  else {
    if (!is_null($area_status)) {
      if ($area_status == '0' || $area_status == '1') {
        $query->condition('fagh.field_area_hook_group_hul_value', $area_status, '=');
        $query->fields('fagh', array('field_area_hook_group_hul_value'));
      }
    }

    if ($type == "revised-sequence") {

      $query->leftjoin('field_data_field_revised_sequence_status', 'fss', 'fss.entity_id = n.nid');
      $query->fields('fss', array('field_revised_sequence_status_value'));
      $query->join('field_data_field_revised_sequence', 'fsq', 'fsq.entity_id = n.nid');
      $query->fields('fsq', array('field_revised_sequence_value'));
      $query->orderBy('fsq.field_revised_sequence_value', 'asc');
      if (!is_null($area_status)) {
        if ($area_status === '0' || $area_status === '1') {
          $query->condition('fss.field_revised_sequence_status_value', $area_status, '=');
          $query->fields('fss', array('field_revised_sequence_status_value'));
        }
      }
    }
    else {
      $query->join('field_data_field_sequence', 'fsq', 'fsq.entity_id = n.nid');
      $query->fields('fsq', array('field_sequence_value'));
      $query->orderBy('fsq.field_sequence_value', 'asc');
    }
  }

  $query->join('field_data_field_hul_code', 'hc', 'hc.entity_id = n.nid');
  $query->join('node', 'hul', 'hc.field_hul_code_value = hul.title');
  $query->fields('hc', array('field_hul_code_value'));
  $query->condition('hul.uid', $distributor_id, '=');
  $query->condition('hul.type', 'hul_code_master');

  $query->Join('field_data_field_hul_updated_lat', 'hul_lat_u', 'hul_lat_u.entity_id = hul.nid');
  $query->fields('hul_lat_u', array('field_hul_updated_lat_value'));
  $query->Join('field_data_field_hul_updated_long', 'hul_long_u', 'hul_long_u.entity_id = hul.nid');
  $query->fields('hul_long_u', array('field_hul_updated_long_value'));
  $query->join('field_data_field_updated_address', 'new', 'new.entity_id = hul.nid');
  $query->fields('new');
  $query->fields('n', array('title'));
  $query->fields('fpn', array('field_party_name_value'));
  $query->fields('fagh', array('field_area_hook_group_hul_value'));

  $query->join('field_data_field_area_hook', 'fa', 'fa.entity_id = n.nid');
  $query->join('node', 'na', 'na.nid = fa.field_area_hook_nid');

  if (!is_null($sales_day)) {
    $query->join('field_data_field_dva_sales_day', 'sd', 'sd.entity_id = na.nid');
    $query->condition('sd.field_dva_sales_day_value', $sales_day, '=');
  }
  if (!is_null($area_hook_group)) {
    $query->join('field_data_field_dva_area_hook_group_name', 'agr', 'agr.entity_id = na.nid');
    $query->condition('agr.field_dva_area_hook_group_name_value', $area_hook_group, '=');
  }

  if (!is_null($van_pl_group)) {
    $query->join('field_data_field_dva_van_pl_group', 'ppplg', 'na.nid = ppplg.entity_id');
    $query->condition('ppplg.field_dva_van_pl_group_value', $van_pl_group, '=');
  }
  $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
  return $res;
}
