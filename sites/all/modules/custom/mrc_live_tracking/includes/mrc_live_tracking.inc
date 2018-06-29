<?php

/**
 * Inc file implementing trip creation form for dispatch planning
 */
function van_live_tracking() {
  //for page refresh after 2min
  //$url = $_SERVER['REQUEST_URI'];
  //drupal_add_http_header('Refresh', "120; URL=$url");
  
  $license_key = variable_get('mapmyindia_map_key', '98thppnrdg3vpp7xupfg9eotrl2rliqu');
  $google_key = variable_get('google_places_key', 'AIzaSyBUpfJiBx6AmZY6vPa4nyYjOmU1kSsHxSQ');
  drupal_add_js("https://apis.mapmyindia.com/advancedmaps/v1/$license_key/map_load?v=0.1", 'external');
  drupal_add_js("https://maps.googleapis.com/maps/api/js?key=$google_key&libraries=places", 'external');
  drupal_add_js(drupal_get_path('module', 'mrc_live_tracking') . '/js/selectable-row.js', 'file');
  drupal_add_js(drupal_get_path('module', 'mrc_live_tracking') . '/js/leaflet.smoothmarkerbouncing.js', 'file');
  //drupal_add_js(drupal_get_path('module', 'mrc_live_tracking') . '/js/livetrack-multi-map.js', 'file');//ForGraph
  drupal_add_js(drupal_get_path('module', 'mrc_live_tracking') . '/js/multi-map.js', 'file');
  $output = '';
  $form = drupal_get_form('van_live_tracking_form');
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
function van_live_tracking_form($form, &$form_state) {
  global $user;
  $distributor_id = change_feeds_import_get_distributor_id($user->uid);
  $rs_code = dispatch_planning_get_rscode_using_uid($user->uid);
  
  drupal_add_js(drupal_get_path('module', 'mrc_live_tracking') . '/js/live_tracking_page_refresh.js');
  $form = array();
  $form['#prefix'] = '<div id="address-selection-map">';
  $form['#suffix'] = '</div>';
  $form['todays-date'] = array(
    '#markup' => '<div style="background-color:#cccccc;margin-bottom: 2px;width: 30%;padding: 5px;">Date: '.  date('d-M-Y').'</div>',
    '#title' => t('Date'),
  );
  $form['refresh_page'] = array(
    '#type' => 'submit',
    '#value' => 'Refresh Page',
  );
  $today_timestamp = strtotime(date("Y-m-d"));
  $query = db_select('van_live_tracking', 'vlt');
  $query->condition('vlt.distributor_id', $distributor_id, '=');
  $query->condition('vlt.tracktime', $today_timestamp, '>=');
  $query->condition('vlt.employeecode', db_like($rs_code) . '%', 'LIKE');  
  $query->fields('vlt', array('employeecode', 'tracktime','battery','speed','latitude', 'longitude'));
  $query->orderBy('vlt.employeecode', 'asc');
  //$query->orderBy('vlt.id', 'asc'); //ForGraph
  $result = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

  $options = array();
  $drivername_arr = array('');
  //$latlong = array();//ForGraph
  foreach ($result as $k => $val) {
    $employeecode = $val['employeecode'];
    $driver = explode("_", $employeecode);
    $drivername_arr[] = ucfirst($driver[1]);
    $tracktime = date('d-M-Y H:i',$val['tracktime']);
    $battery = round($val['battery']);
    $speed = round($val['speed']);
    $latitude = $val['latitude'];
    $long = $val['longitude'];

    $marker_name = $employeecode;

    $new_lat_long = $latitude . ',' . $long;
    //$latlong[$employeecode][] = $new_lat_long;//ForGraph
    $marker_tooltip = '<div class="cell-address cell-hide"><table>'
      . '<tr><td style="white-space:nowrap;color:#222">Van</td><td>' . $employeecode . '</td></tr>'
      . '<tr><td style="white-space:nowrap;color:#222">Lat/Long</td><td>' . $new_lat_long . '</td></tr>'
      . '</table></div>';

//'#attributes' => array('class' => 'tracking', 'latlong'=>$latlong[$employeecode] , 'data-lat' => $latitude, 'data-lng' => $long, 'data-sr' => $marker_name),//ForGraph
 
    $options[$employeecode] = array(
      'employeecode' => array('data' => $employeecode, 'class' => 'employeecode-cell'),
      'tracktime' => array('data' => '<div class="tracktime-data">' . $tracktime . '</div>', 'class' => 'party-name-cell'),
      'battery' => array('data' => $battery, 'class' => 'battery-cell'),
      'speed' => array('data' => $speed, 'class' => 'speed-cell'),
      '#attributes' => array('data-lat' => $latitude, 'data-lng' => $long, 'data-sr' => $marker_name),
      'details' => array('data' => array('#markup' => '<a class="cell-details" style="display:none;">On Map</a>' . $marker_tooltip), 'class' => 'details-cell'),
    );
  }
  if ($options) {
    $headers = array(
      'employeecode' => 'Van',
      'tracktime' => 'Last Datetime',
      'battery' => 'Battery %',
      'speed' => 'Speed',
      'details' => array('data' => '', 'class' => 'details-cell'),
    );

    $class = array('selectable-row', 'table-checkbox');
    $form['list_form_items'] = array(
      '#type' => 'tableselect',
      '#header' => $headers,
      '#options' => $options,
      '#prefix' => '<div id="table-div" style="min-height: 100px;>',
      '#suffix' => '</div>',
      '#empty' => t(''),
      '#attributes' => array('class' => $class),
      '#multiple' => TRUE,
    );
  }
  //Pending bills details cashier wise

  $dispatch_from_date = $dispatch_to_date = date('Y-m-d'); // uncomment while testing on dev

  $query = db_select('dispatch_planning_api_data', 'ad');
  $query->condition('ad.distributor_id', $distributor_id, '=');
  $query->condition('ad.fieldExecutiveName', $drivername_arr, 'IN');
  $query->fields('ad', array('fieldExecutiveName'));
  $query->groupBy('ad.fieldExecutiveName');
  
  if (!empty($dispatch_from_date) && !empty($dispatch_to_date)) {
    $query->where("(DATE_FORMAT(ad.date, '%Y-%m-%d') >= :from AND DATE_FORMAT(ad.date, '%Y-%m-%d') <= :to)", array(':from' => $dispatch_from_date, ':to' => $dispatch_to_date));
  }
  $query->addExpression('(COUNT (ad.bill_number))', 'bill_count');  
  $query->addExpression('SUM(CASE WHEN(ad.res_status IS NULL OR ad.res_status = \'\' ) THEN 1 ELSE 0 END)', 'pending_bill_count');
  $total_res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
  
  foreach($total_res as &$val){
    $val['pendency'] = round(($val['pending_bill_count']/$val['bill_count'])*100);
  }
  $header = array('Cashier Name', 'Bill Count','Pending Bill Count','Pendency %');

  $html = theme('table', array('header' => $header, 'rows' => $total_res, 'attributes' => array('width' => '100%'), 'empty' => t('No Records has been found')));
  $form['bill_pending_details'] = array(
    '#markup' => $html,
    '#title' => t('Bill Pending Details'),
  );

  return $form;
}

function van_live_tracking_form_submit($form, &$form_state) {
  drupal_goto('van-live-track');
}
