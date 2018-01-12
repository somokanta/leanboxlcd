<?php

/**
 *  Form for creating master dispatch planning
 */

/**
 * @param type $form
 * @param type $form_state
 * @return string
 */
function master_dispatch_planning_form($form, &$form_state) {

   $form['#prefix'] = '<div id="itemlist-div">';
   $form['#suffix'] = '</div>';

   global $user;
   // Store distributor id in author in each node
   $dis_id = change_feeds_import_get_distributor_id($user->uid);
   // store logged in users distributor id
   $form['dis_id'] = array(
     '#type' => 'value',
     '#value' => $dis_id,
   );

   $header = array(
     'trip' => 'Trip',
     'tripCreationDate' => 'Trip Creation date',
     'tripExecutionDate' => 'Trip Execution date',
     'hht' => 'HHT id',
     'billCount' => 'Bill count',
     'billValue' => 'Dispatch Bill value',
   );

   $form['#attached']['js'][] = drupal_get_path('module', 'dispatch_planning') . '/js/dispatch_planning_form.js';
   $form['#attached']['css'][] = drupal_get_path('module', 'dispatch_planning') . '/css/dispatch_planning.css';

   $form['trip_date'] = array(
     '#prefix' => '<div class="textfield-wrapper">',
     '#type' => 'date_popup',
     '#date_format' => 'm/d/y',
     '#title' => t('Trip date'),
     '#date_label_position' => '',
     '#date_year_range' => '-200:+0',
     '#size' => 30,
     '#required' => TRUE,
   );

   $form['trip_dispatch_submit'] = array(
     '#type' => 'submit',
     '#value' => t('Add Trip'),
     '#ajax' => array(
       'wrapper' => 'itemlist-div',
       'callback' => 'custom_trip_ajax_callback',
       'method' => 'replace',
       'effect' => 'fade',
     ),
     '#submit' => array('master_trip_dispatch_date_ajax_submit'),
     '#limit_validation_errors' => array(array('trip_date')),
     '#name' => 'trip',
     '#suffix' => '</div>',
   );


   $form['divider1'] = array(
     '#markup' => '<div class="or"><span><b>OR</b></span></div>',
   );

   $form['trip_number'] = array(
     '#prefix' => '<div class="textfield-wrapper">',
     '#type' => 'textfield',
     '#title' => t('Trip'),
     '#required' => TRUE,
     '#autocomplete_path' => 'trip/autocomplete'
   );

   $form['trip_submit'] = array(
     '#type' => 'submit',
     '#value' => t('Add Trip'),
     '#ajax' => array(
       'wrapper' => 'itemlist-div',
       'callback' => 'custom_trip_ajax_callback',
       'method' => 'replace',
       'effect' => 'fade',
     ),
     '#submit' => array('master_trip_id_ajax_submit'),
     '#limit_validation_errors' => array(array('trip_number')),
     '#name' => 'delta',
     '#suffix' => '</div>',
   );

   // initialize array
   if (!isset($form_state['dispatch_planning']['trips'])) {
      $form_state['dispatch_planning']['trips'] = array();
   }

   $options = $form_state['dispatch_planning']['trips'];

   if (!empty($options)) {

      $form['list_form_items'] = array(
        '#type' => 'tableselect',
        '#header' => $header,
        '#options' => $options,
        '#prefix' => '<div id="table-div">',
        '#suffix' => '</div>',
        '#empty' => t('No forms available.'),
        '#multiple' => TRUE,
      );

      $form['remove_selected'] = array(
        '#type' => 'submit',
        '#value' => t('Remove Checked items'),
        '#ajax' => array(
          'wrapper' => 'itemlist-div',
          'callback' => 'custom_ajax_callback',
          'method' => 'replace',
          'effect' => 'fade',
        ),
        '#submit' => array('master_trip_remove_selected_ajax_submit'),
        '#limit_validation_errors' => array(array('list_form_items')),
      );

//      $form['sequence'] = array('#type' => 'value');
//      $form['header'] = array('#type' => 'value');
//
//      $bill_count = count($options);
//      $bill_value_sum = array_sum(array_map(
//              function($item) {
//             return $item['bill_value'];
//          }, $options)
//      );
//      $form_state['dispatch_planning']['bill_value_sum'] = $bill_value_sum;
//      $form['bill_summary'] = array(
//        '#theme' => 'item_list',
//        '#items' => array(
//          'Bill Count : ' . $bill_count,
//          'Bill Value : ' . $bill_value_sum,
//        ),
//      );
//

      $form['submit'] = array(
        '#type' => 'submit',
        '#value' => 'Save',
        '#limit_validation_errors' => array(array('list_form_items')),
        '#submit' => array('dispatch_planning_master_form_save_submit'),
          // '#validate' => array('dispatch_planning_form_save_validate')
      );
   }
   return $form;
}

function master_dispatch_planning_form_validate($form, &$form_state) {
  dpm($form_state);
  foreach ($form_state['values']['list_form_items'] as $nid => $value) {
    $trip_node = node_load($nid);
    dpm($trip_node);
    if (!empty($trip_node->field_master_trip_id_reference['und'][0]['nid'])) {
      $child_trip = $trip_node->title;
      $node = node_load($trip_node->field_master_trip_id_reference['und'][0]['nid']);
      $master_trip = $node->title;
      dpm("Trip: $child_trip is already presend in master Trip:$master_trip ");
      form_set_error('form', "Trip: $child_trip is already presend in master Trip:$master_trip ");
    }
  }
}


/**
 * 
 * @param type $form
 * @param type $form_state
 * @return type
 */
function custom_trip_ajax_callback($form, &$form_state) {
   return $form;
}

function master_trip_dispatch_date_ajax_submit(&$form, &$form_state) {
   $trip_date = ($form_state['values']['trip_date']);
   unset($form_state['input']['trip_date']);
   global $user;
   // Store distributor id in author in each node
   $dis_id = change_feeds_import_get_distributor_id($user->uid);
   if (!empty($trip_date) && !empty($dis_id)) {
      $trips = get_trip_info($trip_date, $dis_id);
      if (!empty($trips)) {
         $form_state['dispatch_planning']['trips'] += $trips;
      }
      else {
         drupal_set_message(t('No result Found.'));
      }
   }
   $form_state['rebuild'] = TRUE;
}

function master_trip_id_ajax_submit(&$form, &$form_state) {
   $trip_number = ($form_state['values']['trip_number']);
   unset($form_state['input']['trip_number']);
   global $user;
   // Store distributor id in author in each node
   $dis_id = change_feeds_import_get_distributor_id($user->uid);
   if (!empty($trip_number) && !empty($dis_id)) {
      $trips = get_trip_info(NULL, $dis_id, $trip_number);
      $form_state['dispatch_planning']['trips'] += $trips;
   }
   $form_state['rebuild'] = TRUE;
}

/**
 * 
 * @param type $trip_date
 * @param type $dis_id
 * @return type
 */
function get_trip_info($trip_date = NULL, $dis_id, $trip_id = NULL) {

   $rows = array();
   $header = array(
     'trip' => 'Trip',
     'tripCreationDate' => 'Trip Creation date',
     'tripExecutionDate' => 'Trip Execution date',
     'hht' => 'HHT id',
     'billCount' => 'Bill count',
     'billValue' => 'Dispatch Bill value',
   );
   if (!empty($dis_id)) {
      $query = db_select('node', 'n');
      $query->condition('n.type', 'dispatch_data');
      $query->addField('n', 'nid', 'nid');
      if ($trip_id) {
         $query->condition('n.title', $trip_id);
      }
      $query->addField('n', 'title', 'trip');
      $query->leftjoin('field_data_field_trip_creation_date', 'tr', 'tr.entity_id = n.nid');
      $query->addExpression("DATE_FORMAT(CONVERT_TZ(FROM_UNIXTIME(tr.field_trip_creation_date_value),'+00:00','+05:30'), '%d-%b-%Y')", 'tripCreationDate');
      $query->leftjoin('field_data_field_trip_executon_date', 'te', 'te.entity_id = n.nid');
      $query->addExpression("DATE_FORMAT(CONVERT_TZ(FROM_UNIXTIME(te.field_trip_executon_date_value),'+00:00','+05:30'), '%d-%b-%Y')", 'tripExecutionDate');
      $query->leftjoin('field_data_field_hht_id', 'pc', 'pc.entity_id = n.nid');
      $query->leftjoin('node', 'n1', 'n1.nid = pc.field_hht_id_nid');
      $query->addExpression('n1.title', 'hht');
      $query->leftjoin('field_data_field_bill_count', 'bn', 'bn.entity_id = n.nid');
      $query->addExpression('bn.field_bill_count_value', 'billCount');
      $query->leftjoin('field_data_field_dispatch_bill_value', 'bv', 'bv.entity_id = n.nid');
      $query->addExpression('bv.field_dispatch_bill_value_value', 'billValue');
      if ($trip_date) {
         $query->leftjoin('field_data_field_dispatch_date', 'td', 'td.entity_id = n.nid');
         $query->condition('td.field_dispatch_date_value', $trip_date);
      }
      $query->condition('n.uid', $dis_id);
      $rows = $query->execute()->fetchAll(PDO::FETCH_GROUP | PDO::FETCH_UNIQUE | PDO::FETCH_ASSOC);
   }
   return $rows;
}

/**
 * 
 * @param type $form
 * @param type $form_state
 */
function master_trip_remove_selected_ajax_submit($form, &$form_state) {

   $checked = array_filter($form_state['values']['list_form_items']);

   if (!empty($checked)) {
      $form_state['dispatch_planning']['trips'] = array_diff_key($form_state['dispatch_planning']['trips'], array_flip($checked));
   }
   $form_state['rebuild'] = TRUE;
}

/**
 * 
 * @param type $form
 * @param type $form_state
 */
function dispatch_planning_master_form_save_submit($form, &$form_state) {
   global $user;
   // Store distributor id in author in each node
   $dis_id = change_feeds_import_get_distributor_id($user->uid);
   $total_bill_count = $total_bill_value = 0;
   if (!empty($dis_id)) {
      $serial_id = _get_highest_title_master_trip_distributor_wise($dis_id);
      // increase title by adding 1
      $serial_id = $serial_id + 1;
      $node = new stdClass();
      $node->title = $serial_id; // fetch or pass bill no here
      $node->type = "master_dispatch_data";
      node_object_prepare($node); // Sets some defaults. Invokes hook_prepare() and hook_node_prepare().
      $node->language = LANGUAGE_NONE; // Or e.g. 'en' if locale is enabled
      $node->uid = $dis_id;
      $node->status = 1; //(1 or 0): published or not
      $node->comment = 0; // 0 = comments disabled, 1 = read only, 2 = read/write
      //$node->field_declaration_date[LANGUAGE_NONE][0]['value'] = date('Y-m-d');

      node_save($node);
      $master_trip_nid = $node->nid;

      foreach ($form_state['values']['list_form_items'] as $nid => $value) {
         $trip_node = node_load($nid);
         if (!empty($trip_node)) {
            $total_bill_count += $trip_node->field_bill_count['und'][0]['value'];
            $total_bill_value += $trip_node->field_dispatch_bill_value['und'][0]['value'];
            $trip_node->field_master_trip_id_reference['und'][0]['nid'] = $master_trip_nid;
            node_save($trip_node);
         }
      }
      $node->field_master_bill_count['und'][0]['value'] = $total_bill_count;
      $node->field_master_bill_value['und'][0]['value'] = $total_bill_value;
      node_save($node); // save node of master_dispatch_data type
      drupal_set_message(t("Master Trip $serial_id Created Sucessfully"));
   }
}

/**
 * 
 * @param type $dis_id
 * @return int
 */
function _get_highest_title_master_trip_distributor_wise($dis_id) {
   $query = db_select('node', 'n');
   $query->condition('n.type', 'master_dispatch_data');
   $query->fields('n', array('title'));
   $query->orderBy('n.title', 'DESC');
   $query->condition('n.uid', $dis_id);
   $field = $query->execute()->fetchField();
   if (empty($field)) {
      $field = 0;
   }
   return $field;
}

/**
 * 
 * @param type $form
 * @param type $form_state
 * @return string
 */
function delete_master_dispatch_planning_form($form, &$form_state) {
  $form['master_trip_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Master Trip Id'),
    '#size' => 10,
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  return $form;
}

function delete_master_dispatch_planning_form_submit($form, &$form_state) {
  global $user;
  $dis_id = change_feeds_import_get_distributor_id($user->uid);

  $master_trip_id = $form_state['values']['master_trip_id'];

  $query = db_select('node', 'n');
  $query->condition('n.type', 'master_dispatch_data');
  $query->fields('n', array('nid'));
  $query->condition('n.title', $master_trip_id);
  $query->condition('n.uid', $dis_id);
  $master_trip_nid = $query->execute()->fetchField();

  if ($master_trip_nid) {
    $query = db_select('field_data_field_third_party_username', 'rs');
    $query->condition('rs.entity_id', $user->uid);
    $query->fields('rs', array('field_third_party_username_value'));
    $rs_code = $query->execute()->fetchField();

    $query = db_select('master_ptl_data', 'cb');
    $query->addExpression('MAX(id)', 'max');
    $query->condition('rs_code', $rs_code, '=');
    $query->condition('ptl_job', $master_trip_id, '=');
    $trip_data_exist = $query->execute()->fetchField();

    if (!$trip_data_exist) {
      $master_node = node_load($master_trip_nid);
      $master_node->status = 0;
      node_save($master_node);

      $query = db_select('field_data_field_master_trip_id_reference', 'tr');
      $query->condition('field_master_trip_id_reference_nid', $master_trip_nid);
      $query->fields('tr', array('entity_id'));
      $query->condition('tr.bundle', "dispatch_data");
      $trip_nids = $query->execute()->fetchAll();

      foreach ($trip_nids as $dis_nid) {
        $dis_node = node_load($dis_nid->entity_id);
        unset($dis_node->field_master_trip_id_reference['und']);
        node_save($dis_node);
      }
      drupal_set_message("Master Trip $master_trip_id is deleted successfully!");
    }
    else {
      drupal_set_message("Trip data is already created for Master Trip $master_trip_id. You can't delete the Master Trip!", 'warning');
    }
  }
  else {
    drupal_set_message("Master Trip $master_trip_id is not found!!");
  }
}
