<?php

/**
 * Inc file implementing trip creation form for dispatch planning
 */
function park_stop_plotting() {

  $license_key = variable_get('mapmyindia_map_key', '98thppnrdg3vpp7xupfg9eotrl2rliqu');
  $google_key = variable_get('google_places_key', 'AIzaSyBUpfJiBx6AmZY6vPa4nyYjOmU1kSsHxSQ');
  drupal_add_js("https://apis.mapmyindia.com/advancedmaps/v1/$license_key/map_load?v=0.1", 'external');
  drupal_add_js("https://maps.googleapis.com/maps/api/js?key=$google_key&libraries=places", 'external');
  drupal_add_js(drupal_get_path('module', 'parkstop_calculation') . '/js/selectable-row.js', 'file');
  drupal_add_js(drupal_get_path('module', 'parkstop_calculation') . '/js/leaflet.smoothmarkerbouncing.js', 'file');
  drupal_add_js(drupal_get_path('module', 'parkstop_calculation') . '/js/multi-map-parkstop-plot.js', 'file');
  $output = '';
  $args[1] = arg(1);
  $args[2] = arg(2);
  $form = drupal_get_form('park_stop_plotting_form',$args);
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
function park_stop_plotting_form($form, &$form_state, $args) {
  global $user;
  $distributor_id = change_feeds_import_get_distributor_id($user->uid);

  $form = array();
  $form['#prefix'] = '<div id="address-selection-map">';
  $form['#suffix'] = '</div>';

  $prevan = $args[1];
  $trip_id = $args[2];
  $session_id = session_id();

  $query = db_select('parkstop_api_log', 'ps');
  $query->condition('ps.distributor_id', $distributor_id, '=');
  if (!empty($trip_id)) {
    $query->condition('ps.trip_id', $trip_id, '=');
  }
  else {
    $query->condition('ps.session_id', $session_id, 'LIKE');
  }
  $query->condition('ps.prevan', $prevan, '=');
  $query->fields('ps', array('prevan', 'api_response'));
  $query->orderBy('ps.id', 'asc');
  $result = $query->execute()->fetch(PDO::FETCH_ASSOC);
  $res_decode = json_decode($result['api_response']);
  $stops = $res_decode->Van;
  foreach ($stops as $key => $value) {
    $completion_time = $value->ApproxCompletionTime;
    foreach ($completion_time as $k1 => $val1) {
      foreach ($val1 as $k2 => $val2) {
        $del_boy[(int) $k2] = $k2;
        break;
      }
    }
  }

  $form_state['api_response'] = $result['api_response'];

  $form['delivery_boy'] = array(
    '#type' => 'select',
    '#prefix' => '<div>',
    '#suffix' => '</div>',
    '#title' => t('Delivery Boy:'),
    '#options' => $del_boy,
    '#default_value' => 2,
    '#ajax' => array(
      'wrapper' => 'address-selection-map',
      'callback' => 'deliveryboy_callback',
      'method' => 'replace',
      'effect' => 'fade',
      'event' => 'change',
    ),
  );

  if ((isset($form_state['values']['delivery_boy']) && !empty($form_state['values']['delivery_boy'])) || empty($form_state['triggering_element'])) {
    if (empty($form_state['triggering_element'])) {
      $form_state['values']['delivery_boy'] = 2;
    }
    
    $conf_nid = check_node_title_exists($distributor_id, 'van_stop_master');
    $conf_node = node_load($conf_nid);
    $origin_lat = $conf_node->field_distributor_latitude[LANGUAGE_NONE][0]['value'];
    $origin_long = $conf_node->field_distributor_longitude[LANGUAGE_NONE][0]['value'];
    $latlongnew[] = $origin_lat . ',' . $origin_long;
    $selected_del_boy = $form_state['values']['delivery_boy'];
    $api_response = $form_state['api_response'];
    $res_decode = json_decode($api_response);
    $stops = $res_decode->Van;
    $n = 0;
    $k = 1;
    $vantime = $res_decode->VanInAreaTime;
    foreach ($vantime as $key => $val) {
      $k++;
      $delboy[$k]['count'] = $k;
      $delboy[$k]['VanInAreaKMs'] = round($res_decode->VanInAreaKMs,2);
      $delboy[$k]['VanTotalKMs'] = round($res_decode->VanTotalKMs,2);
      $delboy[$k]['time'] = round(($val / 60), 2);
    }
    $form_state['total_distance'] = $delboy;
    //total time of delivery boy

    foreach ($stops as $key => $value) {
      $n++;
      $bills = $value->OptimalStop;
      $seq1[$n] = $bills;

      //child
      $completion_time = $value->ApproxCompletionTime;
      foreach ($completion_time as $child => $v) {
        if ($child == ($selected_del_boy - 2)) {
          $clusterStops = $v->ClusterStops;
          if (!empty($clusterStops)) {
            foreach ($clusterStops as $child => $v2) {
              $child_bills = $v2->ChildStop;
              $seq2[$n][$child_bills] = $child_bills;
              $seq3[$n] = $bills;
            }
          }
        }
      }
      //child
    }
    
    foreach ($seq1 as $key1 => $bill_nid) {
      $latlong = get_latlong_from_bills($bill_nid, $distributor_id);
      $lat = $latlong['lat'];
      $long = $latlong['lng'];
      $bill_number = $latlong['bill_num'];
      $party_code = $latlong['party_code'];
      $party_name = $latlong['party_name_value'];
      $park_name = 'Park Stop ' . ($key1);
      $new_lat_long = $lat . ',' . $long;
      $latlongnew[] = $new_lat_long;

      $marker_name = $park_name;
      $marker_tooltip = '<div class="cell-address cell-hide"><table>'
        . '<tr><td style="white-space:nowrap;color:#222">Bill Number</td><td> ' . $bill_number . '</td></tr>'
        . '<tr><td style="white-space:nowrap;color:#222">Sequence</td><td>' . $key1 . '</td></tr>'
        . '<tr><td style="white-space:nowrap;color:#222">Party Code</td><td>' . $party_code . '</td></tr>'
        . '<tr><td style="white-space:nowrap;color:#222">Party Name</td><td>' . $party_name . '</td></tr>'
        . '<tr><td style="white-space:nowrap;color:#222">Lat Long</td><td>' . $lat . ',' . $long . '</td></tr>'
        . '</table></div>';


      $options[$key1] = array(
        'parkname' => array('data' => $park_name, 'class' => 'park_name-cell'),
        'partyname' => array('data' => $party_name, 'class' => 'park_name-cell'),
        '#attributes' => array('data-lat' => $lat, 'data-lng' => $long, 'data-sr' => $marker_name),
        'details' => array('data' => array('#markup' => '<a class="cell-details" style="display:none;">On Map</a>' . $marker_tooltip), 'class' => 'details-cell'),
      );
      if (count($seq1) == ($key1)) {
        $options[$key1]['#attributes'] = array('latlong' => $latlongnew, 'data-lat' => $lat, 'data-lng' => $long, 'data-sr' => $marker_name);
      }
    }

    $latlongchild = array();
    //child

    foreach ($seq2 as $key3 => $bills) {
      $i = 0;
      unset($latlongchild);
      $parent_lat_long = get_latlong_from_bills($seq3[$key3], $distributor_id);
      $lat_parent = $parent_lat_long['lat'];
      $long_parent = $parent_lat_long['lng'];
      $new_lat_long_parent = $lat_parent . ',' . $long_parent;
      $latlongchild[] = $new_lat_long_parent;

      foreach ($bills as $final_bill_num) {
        $i++;
        $latlong_child = get_latlong_from_bills($final_bill_num, $distributor_id);
        $lat = $latlong_child['lat'];
        $lng = $latlong_child['lng'];
        $child_bill_no = $latlong_child['bill_num'];
        $party_code_child = $latlong_child['party_code'];
        $party_name_child = $latlong_child['party_name_value'];
        $child_stop_name = 'Child Stop ' . $i;
        $marker_name = $key3 . '.' . $i;
        $new_lat_long_child = $lat . ',' . $lng;
        $latlongchild[] = $new_lat_long_child;

        $marker_tooltip_child = '<div class="cell-address cell-hide"><table>'
          . '<tr><td style="white-space:nowrap;color:#222">Bill Number</td><td> ' . $child_bill_no . '</td></tr>'
          . '<tr><td style="white-space:nowrap;color:#222">Party Code</td><td>' . $party_code_child . '</td></tr>'
          . '<tr><td style="white-space:nowrap;color:#222">Party Name</td><td>' . $party_name_child . '</td></tr>'
          . '<tr><td style="white-space:nowrap;color:#222">Lat Long</td><td>' . $lat . ',' . $long . '</td></tr>'
          . '</table></div>';


        $options1[$final_bill_num] = array(
          'parkname' => array('data' => 'Park Stop ' . ($key3), 'class' => 'park_name-park-'.$key3),
          'child_stop_name' => array('data' => $child_stop_name, 'class' => 'park_name-cell'),
          'partyname' => array('data' => $party_name_child, 'class' => 'park_name-cell'),
          '#attributes' => array('data-lat' => $lat, 'data-lng' => $lng, 'data-sr' => $marker_name),
          'details' => array('data' => array('#markup' => '<a class="cell-details" style="display:none;">On Map</a>' . $marker_tooltip_child), 'class' => 'details-cell'),
        );
        if (count($bills) == $i) {
          $options1[$final_bill_num]['#attributes'] = array('latlongchild' => $latlongchild, 'data-lat' => $lat, 'data-lng' => $lng, 'data-sr' => $marker_name);
        }
      }
      $form_state['child_stop']['rows'] = $options1;
    }
  //child
    
    //add warehouse as first element in park stop
    $first_element_marker_tooltip = '<div class="cell-address cell-hide"><table>'
    . '<tr><td style="white-space:nowrap;color:#222">Lat Long</td><td>' . $origin_lat . ',' . $origin_long . '</td></tr>'
    . '</table></div>';
          
    $first_element = array(
      'parkname' => array('data' => 'Warehouse', 'class' => 'park_name-cell'),
      '#attributes' => array('data-lat' => $origin_lat, 'data-lng' => $origin_long, 'data-sr' => 'Warehouse'),
      'details' => array('data' => array('#markup' => '<a class="cell-details" style="display:none;">On Map</a>'.$first_element_marker_tooltip),'class' => 'details-cell'),
    );
    array_unshift($options, $first_element);
    $form_state['park_stop']['rows'] = $options;
    
    //Bill Count
    $stops = $res_decode->Van;
    $bill_seq = array();
    foreach ($stops as $key => $value) {
      $bills = $value->OptimalStop;
      if (array_key_exists($bills, $bill_seq) === FALSE) {//ignor if present at other place
        $bill_seq[$bills]['sequence'] = ++$n;
      }
      $completion_time = $value->ApproxCompletionTime;
      $clusterStops = $completion_time[$selected_del_boy - 2]->ClusterStops;
      foreach ($clusterStops as $child => $v2) {
        $child_bills = $v2->ChildStop;
        if (array_key_exists($child_bills, $bill_seq) === FALSE) {//ignor if present at other place
          $bill_seq[$child_bills]['sequence'] = ++$n;
        }
      }
    }

    $form['bill_count'] = array(
         '#markup' => '<div style="float:left;width:100%;clear:both;padding:10px 0 10px 0"><b>Bill Count</b> :'.count($bill_seq).'</div>'
    );
    //Bill Count
    
    //theme table
    $header2 = array('Delivery boy count', 'In Area (Kms)', 'Total (Kms)', 'In-Area Time(Hrs)');

    if (!isset($form_state['total_distance'])) {
      $form_state['total_distance'] = array();
    }
    $total_distance = $form_state['total_distance'];
    if (!empty($total_distance)) {
      $form["total_distance"] = array(
        '#theme' => 'table',
        '#caption' => "<b>Total Distance & Time: </b>",
        '#header' => $header2,
        '#rows' => $total_distance,
        '#prefix' => '<div style="float:left;">',
        '#suffix' => '</div>',
      );
    }
//
    if (!isset($form_state['park_stop']['rows'])) {
      $form_state['park_stop']['rows'] = array();
    }
    $park_stop_data = $form_state['park_stop']['rows'];
    if ($park_stop_data) {

      if (empty($trip_id)) {
        //delivery boy actual selected
        $form['delivery_boy_actual'] = array(
          '#type' => 'select',
          '#prefix' => '<div>',
          '#suffix' => '</div>',
          '#required' => TRUE,
          '#title' => t('Assign Delivery Boy:'),
          '#options' => array('' => 'Select') + $del_boy,
        );

        $form['submit'] = array(
          '#type' => 'submit',
          '#value' => 'Save',
          '#prefix' => '<div style = "float:left;">',
          '#suffix' => '</div>',
          '#submit' => array('deliveryboy_actual_form_submit'),
        );
      }

      //parkstops  
      $form['head1'] = array(
        '#markup' => '<div style="background-color:#cccccc;margin-bottom: 2px;width: 20%;padding: 5px;clear: both;">' . t('Park Stops') . '</div>',
      );
      $headers = array(
        'parkname' => 'Park Stop',
        'partyname' => 'Party Name',
        'details' => array('data' => '', 'class' => 'details-cell'),
      );

      $class = array('selectable-row', 'table-checkbox','park-stop-click');
      $form['list_form_items'] = array(
        '#type' => 'tableselect',
        '#header' => $headers,
        '#options' => $park_stop_data,
        '#prefix' => '<div class = "park-stop-main" id="table-div" style="min-height: 100px;>',
        '#suffix' => '</div>',
        '#empty' => t(''),
        '#attributes' => array('class' => $class),
      );
    }

   //child
    if (!isset($form_state['child_stop']['rows'])) {
      $form_state['child_stop']['rows'] = array();
    }
    $child_stop_data = $form_state['child_stop']['rows'];
    if ($child_stop_data) {
      $form['head2'] = array(
        '#markup' => '<div style="background-color:#cccccc;margin-bottom: 2px;width: 20%;padding: 5px;">' . t('Child Stops') . '</div>',
      );
      $headers1 = array(
        'parkname' => 'Park Stop',
        'child_stop_name' => 'Child Stop',
        'partyname' => 'Party Name',
        'details' => array('data' => '', 'class' => 'details-cell'),
      );
//
     $class1 = array('selectable-row', 'table-checkbox','child-stop-click');
      $form['list_form_items1'] = array(
        '#type' => 'tableselect',
        '#header' => $headers1,
        '#options' => $child_stop_data,
        '#prefix' => '<div class = "park-stop-child" id="table-div">',
        '#suffix' => '</div>',
        '#empty' => t(''),
        '#attributes' => array('class' => $class1),
      );
    }
    //child
  }
  return $form;
}

function deliveryboy_callback($form, &$form_state) {
  $commands[] = ajax_command_replace('#address-selection-map', render($form));
  $commands[] = array
  (
    'command' => 'filter_click_ajax',
  );
  $commands[] = ajax_command_replace('.alert-danger', '');
  $commands[] = ajax_command_replace('.alert-success', '');
  $commands[] = ajax_command_prepend('#address-selection-map', theme('status_messages'));
  return array('#type' => 'ajax', '#commands' => $commands);
}

function deliveryboy_actual_form_submit($form, &$form_state) {
  $del_boy_actual = $form_state['values']['delivery_boy_actual'];
  $api_response = $form_state['api_response'];
  $res_decode = json_decode($api_response);
  $van = arg(1);

  //call function and store in session
  save_parkstop_time_distance($res_decode, $van, $del_boy_actual);
  drupal_set_message('Data saved successfully');
  $form_state['rebuild'] = TRUE;
}

function get_latlong_from_bills($bills_nid, $distributor_id) {
  $query = db_select('node', 'n');
  $query->condition('n.type', 'sales_register_data');
  $query->condition('n.nid', $bills_nid, '=');
  $query->condition('n.uid', $distributor_id);
  $query->join('field_data_field_bill_value', 'bv', 'bv.entity_id = n.nid');
  $query->join('field_data_field_party_code_reference', 'pc', 'pc.entity_id = n.nid');
  $query->join('node', 'n1', 'n1.nid = pc.field_party_code_reference_nid');

  $query->condition('n1.type', 'outlet_master');
  $query->join('field_data_field_party_name', 'pn', 'pn.entity_id = n1.nid');
  $query->join('field_data_field_hul_code_reference', 'hcr', 'hcr.entity_id = n1.nid');
  $query->join('node', 'n2', 'hcr.field_hul_code_reference_nid = n2.nid');
  $query->leftjoin('field_data_field_hul_updated_lat', 'la', 'la.entity_id = n2.nid');
  $query->leftjoin('field_data_field_hul_updated_long', 'lg', 'lg.entity_id = n2.nid');
  $query->addfield('n', 'title', 'bill_num');
  $query->addfield('n1', 'title', 'party_code');
  $query->addfield('pn', 'field_party_name_value', 'party_name_value');
  $query->addfield('bv', 'field_bill_value_value', 'bill_val');
  $query->addfield('la', 'field_hul_updated_lat_value', 'lat');
  $query->addfield('lg', 'field_hul_updated_long_value', 'lng');
  $res = $query->execute()->fetch(PDO::FETCH_ASSOC);
  return $res;
}
