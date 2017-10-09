<?php

module_load_include('inc', 'reverse_geocode', 'address_confirm');

function area_confirm_outlet_sequence() {
  $license_key = variable_get('mapmyindia_map_key', '98thppnrdg3vpp7xupfg9eotrl2rliqu');
  $google_key = variable_get('google_places_key', 'AIzaSyBUpfJiBx6AmZY6vPa4nyYjOmU1kSsHxSQ');
  drupal_add_js("https://apis.mapmyindia.com/advancedmaps/v1/$license_key/map_load?v=0.1", 'external');
  drupal_add_js("https://maps.googleapis.com/maps/api/js?key=$google_key&libraries=places", 'external');
  drupal_add_js(drupal_get_path('module', 'reverse_geocode') . '/js/selectable-row.js', 'file');
  drupal_add_js(drupal_get_path('module', 'reverse_geocode') . '/js/multi-map.js', 'file');
  drupal_add_js(drupal_get_path('module', 'reverse_geocode') . '/js/leaflet.smoothmarkerbouncing.js', 'file');
  

  $form = drupal_render(drupal_get_form('area_confirm_outlet_sequence_form'));
  $result['map-container'] = array(
    '#markup' => '<div id="map-container"></div>',
  );

  return $form . render($result);
}

function area_confirm_outlet_sequence_form($form, &$form_state) {
  $license_key = variable_get('mapmyindia_map_key', '98thppnrdg3vpp7xupfg9eotrl2rliqu');
  $google_key = variable_get('google_places_key', 'AIzaSyBUpfJiBx6AmZY6vPa4nyYjOmU1kSsHxSQ');
  drupal_add_js("https://apis.mapmyindia.com/advancedmaps/v1/$license_key/map_load?v=0.1", 'external');
  drupal_add_js("https://maps.googleapis.com/maps/api/js?key=$google_key&libraries=places", 'external');
  drupal_add_js(drupal_get_path('module', 'reverse_geocode') . '/js/leaflet.smoothmarkerbouncing.js', 'file');
  drupal_add_js(drupal_get_path('module', 'reverse_geocode') . '/js/multi-map.js', 'file');

  global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);
  $sales_day_data = reverse_geocode_get_data_for_sales_day($distributor_id);
  $form['#prefix'] = '<div id="address-selection-map">';
  $form['#suffix'] = '</div>';


  $header_array = array('sr_no' => array('data' => 'Seq. No', 'class' => 'sr-cell'), 'party_code' => array('data' => 'Party Code', 'class' => 'party-code-cell'),
    'party_name' => array('data' => 'Party Name', 'class' => 'party-name-cell'), 'original_seq' => array('data' => 'Original Seq', 'class' => 'original-seq-cell'), 'details' => array('data' => '', 'class' => 'details-cell'));

  $form['sales_day'] = array(
    '#type' => 'select',
    '#title' => t('Sales Day'),
    '#options' => $sales_day_data,
    '#ajax' => array(
      'event' => 'change',
      'callback' => 'area_confirm_outlet_sequence_callback',
      'wrapper' => 'address-selection-map',
    ),
    '#required' => TRUE,
  );


  $form['van_pl_group'] = array(
    '#type' => 'select',
    '#title' => t('Van PL Group'),
    '#options' => array(),
    '#ajax' => array(
      'event' => 'change',
      'callback' => 'area_confirm_outlet_sequence_callback',
      'wrapper' => 'address-selection-map',
    ),
    '#required' => TRUE,
  );


  $form['area_hook_group'] = array(
    '#type' => 'select',
    '#title' => t('Area Hook Group'),
    '#ajax' => array(
      'event' => 'change',
      'callback' => 'area_confirm_outlet_sequence_callback',
      'wrapper' => 'address-selection-map',
    ),
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

  $form['dummy'] = array(
    "#markup" => '<div class="clearance"></div>',
    //'#required' => TRUE,
  );

  $form['filter'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
    '#ajax' => array(
      'wrapper' => 'address-selection-map',
      'callback' => 'filter_area_confirm_outlet_sequence_wise',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    '#validate' => array('filter_area_confirm_outlet_sequence_wise_validate'),
    '#submit' => array('filter_area_confirm_outlet_sequence_wise_submit'),
    '#id' => 'area-filter',
  );

// CHANGE FROM HERE- Above validate&submit(table data to show) and below submit(actual logic) only
  if (!isset($form_state['reverse_geocode']['rows'])) {
    $form_state['reverse_geocode']['rows'] = array();
  }
  $options = $form_state['reverse_geocode']['rows'];
  if ($options) {

    $form['routific_submit'] = array(
      '#type' => 'submit',
      '#value' => t('Routific API'),
      '#submit' => array('area_confirm_outlet_sequence_api'),
      '#ajax' => array(
        'wrapper' => 'address-selection-map',
        'callback' => 'area_confirm_outlet_sequence_ajax_callback',
        'method' => 'replace',
        'effect' => 'fade',
      ),

      '#id' => 'area-hook-routific',
    );
/*    $form['routesavvy_submit'] = array(
      '#type' => 'submit',
      '#value' => t('RouteSavvy API'),
      '#submit' => array('area_confirm_outlet_sequence_api'),
      '#ajax' => array(
        'wrapper' => 'address-selection-map',
        'callback' => 'area_confirm_outlet_sequence_ajax_callback',
        'method' => 'replace',
        'effect' => 'fade',
      ),
      '#id' => 'area-hook-routesavvy',
    );
*/
    if ($form_state['reverse_geocode']['rows_save']) {
      $form['save_submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save Sequence'),
        '#submit' => array('area_confirm_outlet_sequence_save'),
        '#ajax' => array(
          'wrapper' => 'address-selection-map',
          'callback' => 'area_confirm_outlet_sequence_ajax_callback',
          'method' => 'replace',
          'effect' => 'fade',
        ),
        '#id' => 'area-hook-save',
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

function area_confirm_outlet_sequence_callback($form, &$form_state) {
  return $form;
}

function filter_area_confirm_outlet_sequence_wise($form, &$form_state) {
  $commands[] = ajax_command_replace('#address-selection-map', render($form));
  $commands[] = array
    (
    // The command will be used in our JavaScript file (see next section)
    'command' => 'filter_clicked',
  );
  $commands[] = ajax_command_prepend('#address-selection-map', theme('status_messages'));
  // And finally, we will return all of our commands to the system to be executed:
  return array('#type' => 'ajax', '#commands' => $commands);
  // return $form;
}

function filter_area_confirm_outlet_sequence_wise_validate(&$form, &$form_state) {
  unset($form_state['reverse_geocode']['rows_save']);
  unset($form_state['reverse_geocode']['filterd_rows']);
  $sales_day = $form_state['values']['sales_day'];
  $van_pl_group = $form_state['values']['van_pl_group'];
  $area_hook_group = $form_state['values']['area_hook_group'];

  $data = "";
  $type = "filter-sequence-api";
  $area_status = 1;
  global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);
  $nid = check_node_title_exists($distributor_id, 'van_stop_master');
  if (!$nid) {
    form_set_error('area_hook_group', 'Please create Van Stop Master for routing sequence');
  }
  if (!empty($sales_day) && !empty($van_pl_group) && !empty($area_hook_group)) {
    $data = area_confirm_outlet_sequence_get_data_area_code_wise($sales_day, $van_pl_group, $area_hook_group, $area_status, $type);
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

function area_confirm_outlet_sequence_ajax_callback($form, &$form_state) {
  $commands[] = ajax_command_replace('#address-selection-map', render($form));
  $commands[] = array
    (
    // The command will be used in our JavaScript file (see next section)
    'command' => 'filter_clicked',
  );
  $commands[] = ajax_command_prepend('#address-selection-map', theme('status_messages'));
  // And finally, we will return all of our commands to the system to be executed:
  return array('#type' => 'ajax', '#commands' => $commands);
}

function filter_area_confirm_outlet_sequence_wise_submit(&$form, &$form_state) {
  unset($form_state['input']['list_form_items']);
  $sales_day = $form_state['values']['sales_day'];
  $van_pl_group = $form_state['values']['van_pl_group'];
  $area_hook_group = $form_state['values']['area_hook_group'];
  $data = $form_state['values']['area_hook_data'];

  foreach ($data as $key => $val) {
    $lat = $val['field_hul_updated_lat_value'];
    $long = $val['field_hul_updated_long_value'];
    $party_code = $val['title'];
    $party_name = $val['field_party_name_value'];
    $rev_seq_status = $val['field_area_hook_group_hul_value'];
    $sequence = $val['field_sequence_value'];
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


    if ($rev_seq_status == "0") {
      $rev_seq_status = "Non-Confirmed";
    }
    elseif ($rev_seq_status == "1") {
      $rev_seq_status = "Confirmed";
    }

    $no = $key + 1;
    $original_sequence = floatval($sequence);

    $options[$party_code] = array(
      'sr_no' => array('data' => $no, 'class' => 'sr-cell'),
      'party_code' => array('data' => $party_code, 'class' => 'party-code-cell'),
      'party_name' => array('data' => '<div class="party-name-data">' . $party_name . '</div>', 'class' => 'party-name-cell'),
      '#attributes' => array('data-lat' => $lat, 'data-lng' => $long, 'data-sr' => $no),
      'original_seq' => array('data' => $original_sequence, 'class' => 'original_seq-cell'),
      'details' => array('data' => array('#markup' => '<a class="cell-details" style="display:none;">On Map</a>' . $marker_tooltip), 'class' => 'details-cell'),
    );
  }
  $form_state['reverse_geocode']['rows'] = $options;
  $form_state['rebuild'] = TRUE;
}

function area_confirm_outlet_sequence_get_data_area_code_wise($sales_day, $van_pl_group, $area_hook_group, $area_status, $type) {
  global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);

  $query = db_select('node', 'n');
  $query->condition('n.type', 'outlet_master');
  $query->addTag('node_uid');

  $query->join('field_data_field_party_name', 'fpn', 'fpn.entity_id = n.nid');
  $query->leftjoin('field_data_field_area_hook_group_hul', 'fagh', 'fagh.entity_id = n.nid');

  if (!is_null($area_status)) {
    if ($area_status === '0' || $area_status === '1') {
      $query->condition('fagh.field_area_hook_group_hul_value', $area_status, '=');
      $query->fields('fagh', array('field_area_hook_group_hul_value'));
    }
  }

  $query->leftjoin('field_data_field_sequence', 'fsq', 'fsq.entity_id = n.nid');
  $query->fields('fsq', array('field_sequence_value'));
  //$query->orderBy('fsq.field_sequence_value', 'asc');
  // Newly added below two line and above one commented
  $query->orderBy('hul_lat_u.field_hul_updated_lat_value', 'asc');
  $query->orderBy('hul_long_u.field_hul_updated_long_value', 'asc');

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

  $query->join('field_data_field_dva_van_pl_group', 'ppplg', 'na.nid = ppplg.entity_id');
  if (!is_null($van_pl_group)) {
    $query->condition('ppplg.field_dva_van_pl_group_value', $van_pl_group, '=');
  }

  $res = $query->groupBy('n.title')->execute()->fetchAll(PDO::FETCH_ASSOC);
  return $res;
}

function area_confirm_outlet_sequence_api(&$form, &$form_state) {
  $sales_day = $form_state['values']['sales_day'];
  $van_pl_group = $form_state['values']['van_pl_group'];
  $area_hook_group = $form_state['values']['area_hook_group'];
  $data = "";
  $type = "filter-sequence-api";
  $area_status = 1;
  if (!isset($form_state['reverse_geocode']['filterd_rows'])) {
    if (!empty($sales_day) && !empty($van_pl_group) && !empty($area_hook_group)) {
      $data = area_confirm_outlet_sequence_get_data_area_code_wise($sales_day, $van_pl_group, $area_hook_group, $area_status, $type);
    }
  }else{
   $data = $form_state['reverse_geocode']['filterd_rows'];
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

  unset($form_state['input']['list_form_items']);

  /**
   * Have run the API and show date on the fly and on the save button have to save the field
   */
  global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);
  $nid = check_node_title_exists($distributor_id, 'van_stop_master');

  if ($nid) {
    $node = node_load($nid);
  }

  $start_lat = $node->field_distributor_latitude['und'][0]['value'];
  $start_long = $node->field_distributor_longitude['und'][0]['value'];
  $start_name = 'START';

  $api_call_success = 0;
  if ($form_state['values']['op'] == 'RouteSavvy API') {
    $Locations[0]['LocationName'] = $start_name;
    $Locations[0]['Latitude'] = $start_lat;
    $Locations[0]['Longitude'] = $start_long;

    foreach ($data as $key => $val) {
      $Locations[$key + 1]['LocationName'] = $key . '#' . $val['title'];
      $Locations[$key + 1]['Latitude'] = $val['field_hul_updated_lat_value'];
      $Locations[$key + 1]['Longitude'] = $val['field_hul_updated_long_value'];
    }
    $parameters['AppId'] = "cdaccfeb03474c87ad9d1275e5877209";
    $parameters['RouteType'] = variable_get('api_routesavvy_routetype', 'FixedStartOpenEnd'); //RoundTrip
    drupal_set_message("Route Type: " . $parameters['RouteType']);
    $parameters['StartLocation'] = $Locations[0]['LocationName'];
    //$parameters['EndLocation'] = $Locations[0]['LocationName'];


    $optimizer_routesavvy_url = variable_get('api_routesavvy_url', 'http://optimizer.routesavvy.com/OnTerraStopOpt.svc?wsdl');
    $client = new SoapClient($optimizer_routesavvy_url);
    $request_data['Locations']['Location'] = $Locations;
    $request_data['Parameters'] = $parameters;
    $response_data = $client->GetStopOpt($request_data);
    ################SHORT THE ARRAY in sequence################### 
    if ($response_data->GetStopOptResult->ErrorCode === 0) {
      $api_call_success = 1;
      $response_array = $response_data->GetStopOptResult->OutputLocations->Location;
//    $lat = $response_data->GetStopOptResult->OutputLocations->Location[0]->Latitude; 
      foreach ($response_array as $k => $v) {
        $_response_name = explode('#', $v->LocationName);
        foreach ($data as $key => $val) {
          if ($key == $_response_name[0] && $_response_name[0] !== 'START') {
            $data_modified[$k - 1] = $val;
            $data_modified[$k - 1]['original_sequence'] = $val['field_sequence_value'];
            $data_modified[$k - 1]['field_sequence_value'] = $k;
          }
        }
      }
      $data = $data_modified; // now modified original data
      $form_state['reverse_geocode']['rows_save'] = 1;
    }
    else {
      drupal_set_message($response_data->GetStopOptResult->OutputMessage . ' (API ERROR)');
    }
  }
  if ($form_state['values']['op'] == 'Routific API') {
    foreach ($data as $key => $val) {
      $Locations['visits'][$key + 1]['location']['name'] = $key . '#' . $val['title'];
      $Locations['visits'][$key + 1]['location']['lat'] = $val['field_hul_updated_lat_value'];
      $Locations['visits'][$key + 1]['location']['lng'] = $val['field_hul_updated_long_value'];
    }

    $Locations['fleet']['hul_vehicle']['start_location'] = array(
      "id" => "hul_outlet",
      "name" => $start_name,
      "lat" => $start_lat,
      "lng" => $start_long
    );

    $Locations['options']['shortest_distance'] = true;
    $Locations['options']['polylines'] = true;
    $Locations_data = json_encode($Locations);
    $authorization = 'bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJfaWQiOiI1ODgzOWY4ZjYyZmQyMzkzMGE4NjQwYjEiLCJpYXQiOjE1MDQ2Nzg4NTl9.AzJLVp6HDk6lbXCWxI1T8Y7US6R0panNjkY5B-pkdqg';
    $request_headers = array('Content-Type' => 'application/json', 'Authorization' => $authorization);
    $roitufic_api_url = variable_get('api_routific_url', 'https://api.routific.com/v1/vrp');
    $routific_options = array('headers' => $request_headers, 'method' => 'POST', 'data' => $Locations_data);

    $response_data = drupal_http_request($roitufic_api_url, $routific_options);
    ################SHORT THE ARRAY in sequence################### 

    $response_data = json_decode($response_data->data);
    if ($response_data->num_unserved) {
      dpm($response_data->unserved);
    }
    if ($response_data->status == 'success') {
      $api_call_success = 1;
      $response_array = $response_data->solution->hul_vehicle;
      foreach ($response_array as $k => $v) {
        $_response_name = explode('#', $v->location_name);
        foreach ($data as $key => $val) {
          if ($key == $_response_name[0] && $_response_name[0] !== 'START') {
            $data_modified[$k - 1] = $val;
            $data_modified[$k - 1]['original_sequence'] = $val['field_sequence_value'];
            $data_modified[$k - 1]['field_sequence_value'] = $k;
          }
        }
      }
      $data = $data_modified; // now modified original data
      $form_state['reverse_geocode']['rows_save'] = 1;
    }
    else {
      drupal_set_message($response_data->error . ' (API ERROR)');
    }
  }
####################API call end###################################3 
  // to show hul outlet lat long
  $options['hul_outlet'] = array(
    'sr_no' => array('data' => 0, 'class' => 'sr-cell'),
    'party_code' => array('data' => 'Outlet Location', 'class' => 'party-code-cell'),
    'party_name' => array('data' => '<div class="party-name-data">' . 'Outlet Location' . '</div>', 'class' => 'party-name-cell'),
    '#attributes' => array('data-lat' => $start_lat, 'data-lng' => $start_long, 'data-sr' => 0),
    'original_seq' => array('data' => $original_sequence, 'class' => 'original_seq-cell'),
    'details' => array('data' => array('#markup' => '<a class="cell-details" style="display:none;">On Map</a>' . ' '), 'class' => 'details-cell'),
  );

  $form_state['reverse_geocode']['filterd_rows'] = $data;
  foreach ($data as $key => $val) {
    $lat = $val['field_hul_updated_lat_value'];
    $long = $val['field_hul_updated_long_value'];
    $party_code = $val['title'];
    $party_name = $val['field_party_name_value'];
    $rev_seq_status = $val['field_area_hook_group_hul_value'];
    $sequence = $val['field_sequence_value'];
    if ($api_call_success) {
      $original_sequence = $val['original_sequence']; // newly added for modified sequence
    }
    else {
      $original_sequence = $sequence;
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


    if ($rev_seq_status == "0") {
      $rev_seq_status = "Non-Confirmed";
    }
    elseif ($rev_seq_status == "1") {
      $rev_seq_status = "Confirmed";
    }

    //$no = floatval($sequence); //Commented because in some case sequence is blank
    $no = $key + 1;
    $original_sequence = floatval($original_sequence);

    $options[$party_code] = array(
      'sr_no' => array('data' => $no, 'class' => 'sr-cell'),
      'party_code' => array('data' => $party_code, 'class' => 'party-code-cell'),
      'party_name' => array('data' => '<div class="party-name-data">' . $party_name . '</div>', 'class' => 'party-name-cell'),
      '#attributes' => array('data-lat' => $lat, 'data-lng' => $long, 'data-sr' => $no),
      'original_seq' => array('data' => $original_sequence, 'class' => 'original_seq-cell'),
      'details' => array('data' => array('#markup' => '<a class="cell-details" style="display:none;">On Map</a>' . $marker_tooltip), 'class' => 'details-cell'),
    );
  }
  $form_state['reverse_geocode']['rows'] = $options;
  $form_state['rebuild'] = TRUE;
}

function area_confirm_outlet_sequence_save(&$form, &$form_state) {
  $party_code = $form_state['values']['list_form_items'];
  $sequence_no = 1;
  foreach ($party_code as $code => $val) {
    if ($code !== 'hul_outlet') {
      update_area_confirm_outlet_sequence_status($code, $sequence_no);
      $form_state['reverse_geocode']['rows'][$code]['original_seq'] = $sequence_no;
      $sequence_no ++;
    }
  }
  unset($form_state['input']['list_form_items']);
  unset($form_state['reverse_geocode']['rows_save']);
  $form_state['rebuild'] = TRUE;
}

function update_area_confirm_outlet_sequence_status($code, $sequence_no) {
  module_load_include('inc', 'change_feeds_import', 'form/upload_billing_data');
  $nid = check_node_title_exists($code, 'outlet_master');
  if ($nid) {
    $node = node_load($nid);
    $node->field_sequence['und'][0]['value'] = $sequence_no;
    node_save($node);
  }
}
