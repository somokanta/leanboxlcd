<?php

/**
 * Files containing form for Van Map planning form
 */
function _get_van_map_callback($arg) {
   $result = '';
   $output = '';
   if (!empty($arg)) {
      $user_id = $GLOBALS['user']->uid;
      if (isset($_SESSION['van_wise_data'][$arg . '-' . $user_id]) && !empty($_SESSION['van_wise_data'][$arg . '-' . $user_id])) {
         $data = $_SESSION['van_wise_data'][$arg . '-' . $user_id];
         $license_key = variable_get('mapmyindia_map_key', '98thppnrdg3vpp7xupfg9eotrl2rliqu');
         $google_key = variable_get('google_places_key', 'AIzaSyBUpfJiBx6AmZY6vPa4nyYjOmU1kSsHxSQ');
         drupal_add_js("https://apis.mapmyindia.com/advancedmaps/v1/$license_key/map_load?v=0.1", 'external');
         drupal_add_js("https://maps.googleapis.com/maps/api/js?key=$google_key&libraries=places", 'external');
         drupal_add_js(drupal_get_path('module', 'van_planning') . '/js/selectable-row.js', 'file');
         //drupal_add_js(drupal_get_path('module', 'van_planning') . '/js/leaflet.smoothmarkerbouncing.js', 'file');
         drupal_add_js(drupal_get_path('module', 'van_planning') . '/js/livetrack-multi-map.js', 'file');

         $output = '';
         $form = drupal_get_form('_van_map_planning_callback_form', $data, $arg);
         $output = drupal_render($form);

         $result['map-container'] = array(
           '#markup' => '<div id="map-container"></div>',
         );
      }
   }

   return render($result) . $output;
}

/**
 * 
 * @param array $form
 * @param type $form_state
 * @return array
 */
function _van_map_planning_callback_form($form, &$form_state, $data, $arg) {
   $title = strtoupper($arg);
   drupal_set_title($title);
   drupal_add_js(drupal_get_path('module', 'van_planning') . '/js/live_tracking_page_refresh.js');

   $form = array();

   $array_data = json_decode($data, 1);
   $count = 0;
   foreach ($array_data as $key => $val) {
      $marker_tooltip = '<div class="cell-address cell-hide"><div>'
          . '<div style="white-space:nowrap;color:#222">Bill - ' . $val['bill_no'] . '</div>'
          . '</div></div>';
      $sr_data = ++$count;
      $val1['data'][$key]['bill_no']['data'] = $val['bill_no'];
      $val1['data'][$key]['party_code']['data'] = $val['party_code'];
      $val1['data'][$key]['seq']['data'] = $sr_data;

      $val1['data'][$key]['#attributes'] = array('class' => 'tracking', 'data-sr' => $sr_data, 'data-lat' => $val['latitude'], 'data-lng' => $val['longitude']);
      $val1['data'][$key]['details'] = array('data' => array('#markup' => '<a class="cell-details" style="display:none;">On Map</a>' . $marker_tooltip), 'class' => 'details-cell');
   }
   $form['#prefix'] = '<div id="address-selection-map">';
   $form['#suffix'] = '</div>';

   $header_array = array(
     'seq' => array('data' => t('Sequence'), 'field' => 'seq'),
     'bill_no' => array('data' => t('Bill NO'), 'field' => 'bill_no'),
     'party_code' => array('data' => t('Outlet Code'), 'field' => 'party_code'),
     'details' => array('data' => '', 'class' => 'details-cell'),
   );

   $class = array('selectable-row', 'table-checkbox');
   $form['list_form_items'] = array(
     '#type' => 'tableselect',
     '#header' => $header_array,
     '#options' => $val1['data'],
     '#prefix' => '<div id="table-div" style="min-height: 100px;>',
     '#suffix' => '</div>',
     '#empty' => t(''),
     '#attributes' => array('class' => $class),
     '#multiple' => TRUE,
   );
   return $form;
}
