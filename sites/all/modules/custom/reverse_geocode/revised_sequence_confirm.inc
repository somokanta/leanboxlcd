<?php

module_load_include('inc', 'reverse_geocode', 'address_confirm');

function outlet_addition() {
  $license_key = variable_get('mapmyindia_map_key', '98thppnrdg3vpp7xupfg9eotrl2rliqu');
  $google_key = variable_get('google_places_key', 'AIzaSyBUpfJiBx6AmZY6vPa4nyYjOmU1kSsHxSQ');
  drupal_add_js("https://apis.mapmyindia.com/advancedmaps/v1/$license_key/map_load?v=0.1", 'external');
  drupal_add_js("https://maps.googleapis.com/maps/api/js?key=$google_key&libraries=places", 'external');
  drupal_add_js(drupal_get_path('module', 'reverse_geocode') . '/js/selectable-row.js', 'file');
  drupal_add_js(drupal_get_path('module', 'reverse_geocode') . '/js/leaflet.smoothmarkerbouncing.js', 'file');
  drupal_add_js(drupal_get_path('module', 'reverse_geocode') . '/js/multi-map.js', 'file');

  $form = drupal_render(drupal_get_form('revised_sequence_confirm_form'));
  $result['map-container'] = array(
    '#markup' => '<div id="map-container"></div>',
  );

  return $form . render($result);
}

function revised_sequence_confirm_form($form, &$form_state) {
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


  $header_array = array('sr_no' => 'Seq. No', 'party_code' => 'Party Code',
    'party_name' => 'Party Name', 'rev_seq_status' => 'Status',
    'details' => ''
  );

  $form['sales_day'] = array(
    '#type' => 'select',
    '#title' => t('Sales Day'),
//    '#prefix' => '<div id="address-selection-map">',
//    '#suffix' => '</div>',
    // '#default_value' => isset($form_state['values']['sales_day']) ? $form_state['values']['sales_day'] : NULL,
    '#options' => $sales_day_data,
    '#ajax' => array(
      'event' => 'change',
      'callback' => 'rev_sales_day_callback',
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
      'callback' => 'rev_sales_day_callback',
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

  $area_status = array(0 => "Non-Confirmed", 1 => "Confirmed");
  $form['rev_seq_status'] = array(
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
      'callback' => 'filter_rev_seq_wise',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    '#validate' => array('rev_seq_wise_filter_validate'),
    '#submit' => array('rev_seq_wise_filters_submit'),
    '#id' => 'area-filter',
  );


  if (!isset($form_state['reverse_geocode']['rows'])) {
    $form_state['reverse_geocode']['rows'] = array();
  }
  $options = $form_state['reverse_geocode']['rows'];
  if ($options) {

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Confirm Sequence'),
      '#submit' => array('update_rev_seq_submit'),
      '#ajax' => array(
        'wrapper' => 'address-selection-map',
        'callback' => 'rev_seq_hook_ajax_callback',
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

function rev_sales_day_callback($form, &$form_state) {
  return $form;
}

function filter_rev_seq_wise($form, &$form_state) {
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
  // return $form;
}

function rev_seq_wise_filter_validate(&$form, &$form_state) {
  $sales_day = $form_state['values']['sales_day'];
  $van_pl_group = $form_state['values']['van_pl_group'];
  $area_hook_group = $form_state['values']['area_hook_group'];
  $area_status = $form_state['values']['rev_seq_status'];
  $data = "";
  $type = "revised-sequence";
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

function rev_seq_hook_ajax_callback($form, &$form_state) {
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

function rev_seq_wise_filters_submit(&$form, &$form_state) {
  unset($form_state['input']['list_form_items']);
  $sales_day = $form_state['values']['sales_day'];
  $van_pl_group = $form_state['values']['van_pl_group'];
  $area_hook_group = $form_state['values']['area_hook_group'];
  $data = $form_state['values']['area_hook_data'];
  dpm($data,"data");
  $type = "revised-sequence";

  foreach ($data as $key => $val) {
    $lat = $val['field_hul_updated_lat_value'];
    $long = $val['field_hul_updated_long_value'];
    $party_code = $val['title'];
    $party_name = $val['field_party_name_value'];
    $rev_seq_status = $val['field_revised_sequence_status_value'];
    $sequence = $val['field_revised_sequence_value'];
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

    $no = floatval($sequence);
    
    $options[$party_code] = array(
      'sr_no' => $no,
      'party_code' => $party_code,
      'party_name' => $party_name,
      '#attributes' => array('data-lat' => $lat, 'data-lng' => $long, 'data-sr' => $no),
      'rev_seq_status' => $rev_seq_status,
      'details' => array('data' => array('#markup' => '<a class="cell-details" style="display:none;">On Map</a>' . $marker_tooltip)),
    );
  }
  $form_state['reverse_geocode']['rows'] = $options;
  $form_state['rebuild'] = TRUE;
}

function update_rev_seq_submit(&$form, &$form_state) {
 
  $party_code = $form_state['values']['list_form_items'];
  foreach ($party_code as $code) {
    reverse_geocode_update_revised_sequence_status($code);
      $form_state['reverse_geocode']['rows'][$code]['rev_seq_status'] = 'Confirmed';
  }
  unset($form_state['input']['list_form_items']);
  $form_state['rebuild'] = TRUE;
}

function reverse_geocode_update_revised_sequence_status($code) {
  module_load_include('inc', 'change_feeds_import', 'form/upload_billing_data');
  $nid = check_node_title_exists($code, 'outlet_master');
  if ($nid) {
    $node = node_load($nid);
    $node->field_revised_sequence_status['und'][0]['value'] = 1;
    node_save($node);
  }
}
