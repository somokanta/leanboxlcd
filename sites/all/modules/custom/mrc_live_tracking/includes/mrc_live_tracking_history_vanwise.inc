<?php

/**
 * Inc file implementing trip creation form for dispatch planning
 */
function van_live_tracking_history_vanwise() {
  //for page refresh after 2min
  //$url = $_SERVER['REQUEST_URI'];
  //drupal_add_http_header('Refresh', "120; URL=$url");

  $license_key = variable_get('mapmyindia_map_key', '98thppnrdg3vpp7xupfg9eotrl2rliqu');
  $google_key = variable_get('google_places_key', 'AIzaSyBUpfJiBx6AmZY6vPa4nyYjOmU1kSsHxSQ');
  drupal_add_js("https://apis.mapmyindia.com/advancedmaps/v1/$license_key/map_load?v=0.1", 'external');
  drupal_add_js("https://maps.googleapis.com/maps/api/js?key=$google_key&libraries=places", 'external');
  drupal_add_js(drupal_get_path('module', 'mrc_live_tracking') . '/js/selectable-row.js', 'file');
  drupal_add_js(drupal_get_path('module', 'mrc_live_tracking') . '/js/leaflet.smoothmarkerbouncing.js', 'file');
  drupal_add_js(drupal_get_path('module', 'mrc_live_tracking') . '/js/livetrack-multi-map-vanwise.js', 'file'); //ForGraph
  drupal_add_js(drupal_get_path('module', 'mrc_live_tracking') . '/js/tooltip/jquery.tinytooltip.js', 'file');
  drupal_add_css(drupal_get_path('module', 'mrc_live_tracking') . '/js/tooltip/jquery.tinytooltip.css', 'file');
  //drupal_add_js(drupal_get_path('module', 'mrc_live_tracking') . '/js/multi-map.js', 'file');
  $output = '';
  $form = drupal_get_form('van_live_tracking_history_vanwise_form');
  $output = drupal_render($form);

  $result['map-container'] = array(
    '#markup' => '<div id="map-container"></div>',
  );

  return render($result) . $output;
}

/**
 * @param type $form
 * @param type $form_state
 * @return string
 */
function van_live_tracking_history_vanwise_form($form, &$form_state) {
  global $user;
  $distributor_id = change_feeds_import_get_distributor_id($user->uid);
  $rs_code = dispatch_planning_get_rscode_using_uid($user->uid);

  drupal_add_js(drupal_get_path('module', 'mrc_live_tracking') . '/js/live_tracking_page_refresh.js');
  $form = array();
  $form['#prefix'] = '<div id="address-selection-map">';
  $form['#suffix'] = '</div>';
     $form['a'] = array(
     '#type' => 'fieldset',
     '#title' => t(''),
     '#collapsible' => FALSE,
     '#collapsed' => FALSE,
   );
  $form['a']['dispatch_date'] = array(
    '#type' => 'date_popup',
    '#title' => t('Dispatch Date'),
    '#date_format' => 'Y-m-d',
    '#attributes' => array('class' => array('dispatch_date')),
    '#date_label_position' => '',
    '#date_year_range' => '-20:+2',
    '#size' => 30,
    '#ajax' => array(
      'callback' => 'vanwise_form_callback',
      'event' => 'change',
      'wrapper' => 'address-selection-map',
      'method' => 'replace',
    ),
    //'#default_value' => $_GET['dispatch_date'] ? $_GET['dispatch_date'] :'',
  );
  $dispatch_date = $form_state['input']['dispatch_date'];
  $trips = _get_trip_ids_using_dispatch_date($dispatch_date);
  $select = array('' => '- Any -');
  $trips = $select + $trips;
  $form['a']['trip_ids'] = array(
    '#type' => 'select',
    '#title' => t('Select Trip ID'),
    '#options' => $trips,
    '#ajax' => array(
      'callback' => 'vanwise_form_callback',
      'event' => 'change',
      'wrapper' => 'address-selection-map',
      'method' => 'replace',
    ),
  );
  
  $trip_ids = $form_state['input']['trip_ids'];
  $cashiername = _get_cashier_name_using_trip_id($trip_ids);
  $select = array('' => '- Any -');
  $cashiername = $select + $cashiername;
  $form['a']['cashier_name'] = array(
    '#type' => 'select',
    '#title' => t('Cashier name'),
    '#options' => $cashiername
  );
  $form['trip_id_wrapper'] = array(
    '#type' => 'markup',
    '#title' => t('OR'),
    '#prefix' => '<div id="after_trip_id" class="text-center font-weight-bold or_txt" width=100%> OR',
    '#suffix' => '</div>'
  );
  $form['trip_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Trip ID*'),
    '#size' => 20,
    //'#prefix' => '<div class="text-center font-weight-bold or_txt" width=100%> OR</div>',
    '#maxlength' => 50,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
    '#prefix' => '<div>',
    '#suffix' => '</div>'
  );
  if (!isset($form_state['vanwise_tracking']['rows'])) {
    $form_state['vanwise_tracking']['rows'] = array();
  }
  $driver_data = $form_state['vanwise_tracking']['rows'];
  $class = array('selectable-row', 'table-checkbox');
  if ($driver_data) {
    $form['head'] = array(
      '#markup' => '<div style="background-color:#cccccc;margin-bottom: 2px;width: 35%;padding: 5px;">' . t('History Driver Wise') . '</div>',
    );
    $header_array = array(
      'employeecode' => 'Van',
      'tracktime' => 'Last Datetime',
      'battery' => 'Battery %',
      'speed' => 'Speed',
      'details' => array('data' => '', 'class' => 'details-cell'),
    );

    $form['list_form_items'] = array(
      '#type' => 'tableselect',
      '#header' => $header_array,
      '#options' => $driver_data,
      '#prefix' => '<div id="table-div" style="min-height:10px">',
      '#suffix' => '</div>',
      '#empty' => t(''),
      '#attributes' => array('class' => $class),
      '#multiple' => TRUE,
    );
  }
  if (!isset($form_state['vanwise_tracking_trip']['rows'])) {
    $form_state['vanwise_tracking_trip']['rows'] = array();
  }
  $trip_data = $form_state['vanwise_tracking_trip']['rows'];
  if ($trip_data) {
    $form['head1'] = array(
      '#markup' => '<div style="background-color:#cccccc;margin-bottom: 2px;width: 35%;padding: 5px;">' . t('Planned Route') . '</div>',
    );
    $header_array1 = array(
      'sr_no' => 'Sr.no',
      'party_code' => 'Party code',
      'party_name' => 'Party name',
      'seq' => 'Sequence',
      'details' => array('data' => '', 'class' => 'details-cell'),
    );
    $form['list_form_items1'] = array(
      '#type' => 'tableselect',
      '#header' => $header_array1,
      '#options' => $trip_data,
      '#prefix' => '<div id="table-div" style="min-height: auto;max-height: 350px;overflow-y: auto;">',
      '#suffix' => '</div>',
      '#empty' => t(''),
      '#attributes' => array('class' => $class),
      '#multiple' => TRUE,
    );
  }

  if (!isset($form_state['vanwise_tracking_farey']['rows'])) {
    $form_state['vanwise_tracking_farey']['rows'] = array();
  }
  $farey_data = $form_state['vanwise_tracking_farey']['rows'];
  if ($farey_data) {
    $form['head2'] = array(
      '#markup' => '<div style="background-color:#cccccc;margin-bottom: 2px;width: 40%;padding: 5px;">' . t('Actual Route Markers') . '</div>',
      '#title' => t('Fareye Data'),
    );
    $header_array2 = array(
      'sr_no' => 'Sr.no',
      'party_code' => 'Party code',
      'party_name' => 'Party name',
      'details' => array('data' => '', 'class' => 'details-cell'),
    );

    $form['list_form_items2'] = array(
      '#type' => 'tableselect',
      '#header' => $header_array2,
      '#options' => $farey_data,
      '#prefix' => '<div id="table-div" style="min-height: auto;max-height: 350px;overflow-y: auto;">',
      '#suffix' => '</div>',
      '#empty' => t(''),
      '#attributes' => array('class' => $class),
      '#multiple' => TRUE,
    );
  }


  return $form;
}

function van_live_tracking_history_vanwise_form_validate($form, &$form_state) {
  $trip_id = $form_state['values']['trip_id'];
  $dispatch_date = $form_state['values']['dispatch_date'];
  $trip_ids_new = $form_state['values']['trip_ids'];
  $cashier_name = strtolower($form_state['values']['cashier_name']);
  if (!empty($dispatch_date) && empty($trip_ids_new)) {
    form_set_error('file', t('Please select trip id'));
  }
  if (!empty($dispatch_date) && !empty($trip_ids_new) && empty($cashier_name)) {
    form_set_error('file', t('Please select cashier name'));
  }
  if (empty($dispatch_date) && empty($trip_id)) {
    form_set_error('file', t('Please select trip id or dispatch date'));
  }
}

function van_live_tracking_history_vanwise_form_submit($form, &$form_state) {
  global $user;
  $distributor_id = change_feeds_import_get_distributor_id($user->uid);
  $rs_code = dispatch_planning_get_rscode_using_uid($user->uid);
  $trip_id = $form_state['values']['trip_id'];
  $trip_ids_new = $form_state['values']['trip_ids'];
  $dispatch_date = $form_state['values']['dispatch_date'];
  $cashier_name = strtolower($form_state['values']['cashier_name']);

  $query1 = db_select('dispatch_planning_api_data', 'ad');
  $query1->condition('ad.distributor_id', $distributor_id, '=');
  if (!empty($dispatch_date)) {
    $query1->condition('ad.date', $dispatch_date);
  }
  if (!empty($trip_ids_new)) {
    $query1->condition('ad.runsheet_number', $trip_ids_new);
  }else{
    $query1->condition('ad.runsheet_number', $trip_id);
  }
  if (!empty($cashier_name)) {
    $query1->condition('ad.fieldExecutiveName', $cashier_name, '=');
  }
  $query1->fields('ad', array('party_code', 'party_name', 'res_latitude', 'res_longitude', 'bill_number', 'res_status', 'runsheet_number', 'fieldExecutiveName','date'));
  $res_new = $query1->execute()->fetchAll(PDO::FETCH_ASSOC);
  $farey_data = array();
  foreach ($res_new as $key => $value) {
    $fieldExecutiveName = $value['fieldExecutiveName'];
    $res_latitude = $value['res_latitude'];
    $res_longitude = $value['res_longitude'];
    $party_code = $value['party_code'];
    $party_name = $value['party_name'];
    $bill_number = $value['bill_number'];
    $res_status = $value['res_status'];
    $dispatch_date = $value['date'];
    
    $dispatch_start_date = strtotime($dispatch_date);
    $dispatch_end_date = strtotime($dispatch_date) + 86399;

    $new_lat_long1 = !empty($res_latitude) ? $res_latitude . ',' . $res_longitude : '';
    
    //Farey data
    $marker_tooltip1 = '<div class="cell-address cell-hide" style="display:none"><table>'
      . '<tr><td style="white-space:nowrap;color:#222">Party code</td><td>' . $party_code . '</td></tr>'
      . '<tr><td style="white-space:nowrap;color:#222">Party name</td><td>' . $party_name . '</td></tr>'
      . '<tr><td style="white-space:nowrap;color:#222">Lat/Long</td><td>' . $new_lat_long1 . '</td></tr>'
      . '<tr><td style="white-space:nowrap;color:#222">Bill Number</td><td>' . $bill_number . '</td></tr>'
      . '<tr><td style="white-space:nowrap;color:#222">Bill Status</td><td>' . $res_status . '</td></tr>'
      . '</table></div>';

    $farey_data[$party_code] = array(
      'sr_no' => array('data' => $key + 1, 'class' => 'sr-cell'),
      'party_code' => array('data' => $party_code, 'class' => 'party-code-cell'),
      'party_name' => array('data' => '<div class="party-name-data">' . substr($party_name,0,10) . '</div>', 'class' => 'party-name-cell-'.$key, 'data-store' => $party_name,'data-trans' => $key),
      '#attributes' => array('data-lat' => $res_latitude, 'data-lng' => $res_longitude, 'data-sr' => 'Del-'.$party_code),
      'details' => array('data' => array('#markup' => '<a class="cell-details" style="display:none;">On Map</a>' . $marker_tooltip1)),
    );
    $form_state['vanwise_tracking_farey']['rows'] = $farey_data;
    
    //Driver wise data
    $query = db_select('van_live_tracking', 'vlt');
    $query->condition('vlt.distributor_id', $distributor_id, '=');
    $query->condition('vlt.employeecode', db_like($rs_code) . '%', 'LIKE');
    if (!empty($cashier_name)) {
      $query->condition('vlt.employeecode', '%' . db_like($cashier_name) . '%', 'LIKE');
    }
    else {
      $query->condition('vlt.employeecode', '%' . db_like($fieldExecutiveName) . '%', 'LIKE');
    }
    $query->condition('vlt.tracktime', array($dispatch_start_date, $dispatch_end_date), 'BETWEEN');
    $query->fields('vlt', array('employeecode', 'tracktime', 'battery', 'speed', 'latitude', 'longitude'));
    $query->orderBy('vlt.employeecode', 'asc');
    $query->orderBy('vlt.id', 'asc');
    $result = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

    $driver_data = array();
    $latlong = array(); //ForGraph
    foreach ($result as $k => $val) {
      $employeecode = $val['employeecode'];
      $tracktime = date('d-M-Y H:i', $val['tracktime']);
      $battery = round($val['battery']);
      $speed = round($val['speed']);
      $latitude = $val['latitude'];
      $longitude = $val['longitude'];

      $marker_name = $employeecode;

      $new_lat_long = $latitude . ',' . $longitude;
      $latlong[$employeecode][] = $new_lat_long; //ForGraph
      $marker_tooltip = '<div class="cell-address cell-hide"><table>'
        . '<tr><td style="white-space:nowrap;color:#222">Van</td><td>' . $employeecode . '</td></tr>'
        . '<tr><td style="white-space:nowrap;color:#222">Lat/Long</td><td>' . $new_lat_long . '</td></tr>'
        . '</table></div>';



      $driver_data[$employeecode] = array(
        'employeecode' => array('data' => $employeecode, 'class' => 'employeecode-cell'),
        'tracktime' => array('data' => '<div class="tracktime-data">' . $tracktime . '</div>', 'class' => 'party-name-cell'),
        'battery' => array('data' => $battery, 'class' => 'battery-cell'),
        'speed' => array('data' => $speed, 'class' => 'speed-cell'),
        '#attributes' => array('class' => 'tracking', 'latlong' => $latlong[$employeecode], 'data-lat' => $latitude, 'data-lng' => $longitude, 'data-sr' => $marker_name), //ForGraph
        'details' => array('data' => array('#markup' => '<a class="cell-details" style="display:none;">On Map</a>' . $marker_tooltip), 'class' => 'details-cell'),
      );
    }
    $form_state['vanwise_tracking']['rows'] = $driver_data;
  }

  //Trip Wise data
  if ($trip_ids_new) {
    $data = reverse_geocode_get_data_trip_wise($trip_ids_new);
  }else{
    $data = reverse_geocode_get_data_trip_wise($trip_id);
  }
  
  $conf_nid = check_node_title_exists($distributor_id, 'van_stop_master');
  $conf_node = node_load($conf_nid);
  $origin_lat = $conf_node->field_distributor_latitude[LANGUAGE_NONE][0]['value'];
  $origin_long = $conf_node->field_distributor_longitude[LANGUAGE_NONE][0]['value'];
  $lat_long_arr[] = $origin_lat . ',' . $origin_long;
  foreach ($data as $key => $val) {
    $party_code1 = $val['field_party_code_value'];
    $lat = $val['field_hul_updated_lat_value'];
    $long = $val['field_hul_updated_long_value'];

    $old_lat = $val['field_hul_lat_value'];
    $old_long = $val['field_hul_long_value'];

    $party_name1 = $val['field_party_name_value'];
    $sequence = (int) ($val['field_bill_sequence_value']);
    if ($sequence == "0" || empty($sequence)) {
      $seq = "NA";
    }
    else {
      $seq = "";
    }

    $sr_no = (int) ($val['field_drs_sequence_value']);
    $old_street = $val['field_original_address_sub_premise'];
    $old_locality = $val['field_original_address_locality'];
    $old_sublocality = $val['field_original_address_dependent_locality'];
    $old_poi = $val['field_original_address_data'];

    $old_lat_long = !empty($lat) ? $lat . ',' . $long : '';
    $lat_long_arr[] = $old_lat_long;
    $marker_tooltip2 = '<div class="cell-address cell-hide"><table>'
      . '<tr><td style="white-space:nowrap;color:#222">Party code</td><td>' . $party_code1 . '</td></tr>'
      . '<tr><td style="white-space:nowrap;color:#222">Party Name</td><td>' . $party_name1 . '</td></tr>'
      . '<tr><td style="white-space:nowrap;color:#222">Locality</td><td>' . $old_locality . '</td></tr>'
      . '<tr><td style="white-space:nowrap;color:#222">Sub Locality</td><td>' . $old_sublocality . '</td></tr>'
      . '<tr><td style="white-space:nowrap;color:#222">Street</td><td>' . $old_street . '</td></tr>'
      . '<tr><td style="white-space:nowrap;color:#222">POI</td><td>' . $old_poi . '</td></tr>'
      . '<tr><td style="white-space:nowrap;color:#222">Lat/Long</td><td>' . $old_lat_long . '</td></tr>'
      . '</table></div>';

    $trip_data[$key] = array(
      'sr_no' => array('data' => $sr_no, 'class' => 'sr-cell'),
      'party_code' => array('data' => $party_code1, 'class' => 'party-code-cell'),
      'party_name' => array('data' => '<div class="party-name-data">' . substr($party_name1,0,10) . '</div>', 'class' => 'party-name-cell-'.$sr_no,'data-store' => $party_name1,'data-trans' => $sr_no),
      'seq' => array('data' => $seq, 'class' => 'seq-cell'),
      '#attributes' => array('class' => 'party-tracking', 'data-lat' => $lat, 'data-lng' => $long, 'data-sr' => $party_code1),
      'details' => array('data' => array('#markup' => '<a class="cell-details" style="display:none;">On Map</a>' . $marker_tooltip2)),
    );
    if(count($data) == ($key + 1)){
      $trip_data[$key]['#attributes'] = array('class' => 'party-tracking', 'latlongnew' => $lat_long_arr, 'data-lat' => $lat, 'data-lng' => $long, 'data-sr' => $party_code1);
    }
  }
//  //Add first element for warehouse
  $first_trip_element = array('sr_no' => array('data' => 0, 'class' => 'sr-cell'),
    'party_code' => array('data' => 'Warehouse', 'class' => 'party-code-cell'),
    'party_name' =>array('data' => '<div class="party-name-data">' . substr('Warehouse',0,10) . '</div>', 'class' => 'party-name-cell-'.'0','data-store' => 'Warehouse','data-trans' => 0),
    'seq' => array('data' => '', 'class' => 'seq-cell'),
    '#attributes' => array('class' => 'party-tracking', 'data-lat' => $origin_lat, 'data-lng' => $origin_long, 'data-sr' => 'Warehouse'),
     'details' => array('data' => array('#markup' => '<a class="cell-details" style="display:none;">On Map</a>')),); 
  array_unshift($trip_data, $first_trip_element);
  $form_state['vanwise_tracking_trip']['rows'] = $trip_data;
  $form_state['rebuild'] = TRUE;
}

function vanwise_form_callback($form, &$form_state) {
  return $form;
}

function _get_cashier_name_using_dispatch_date($dispatch_date) {
  $res = array();
  if (!empty($dispatch_date)) {
    global $user;
    $dis_id = change_feeds_import_get_distributor_id($user->uid);
    $query = db_select('dispatch_planning_api_data', 'ad');
    $query->condition('ad.distributor_id', $dis_id);
    $query->condition('ad.date', $dispatch_date);
    $query->fields('ad', array('fieldExecutiveName'));
    $res = $query->execute()->fetchAllKeyed(0, 0);
  }
  return $res;
}
function _get_cashier_name_using_trip_id($trip_id) {
  $res = array();
  if (!empty($trip_id)) {
    global $user;
    $dis_id = change_feeds_import_get_distributor_id($user->uid);
    $query = db_select('node', 'n');
    $query->condition('n.type', 'dispatch_data');
    $query->condition('n.uid', $dis_id);
    $query->leftjoin('field_data_field_cashier_name', 'cn', 'cn.entity_id = n.nid');
    $query->fields('cn', array('field_cashier_name_value'));
    $query->condition('n.title', $trip_id);
    $res = $query->execute()->fetchAllKeyed(0, 0);
  }
  return $res;
}
