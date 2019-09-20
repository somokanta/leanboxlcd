
<?php

/**
 * @file
 * Definition of example_handler_custom_field
 */

/**
 * Provides a custom views field.
 */
class total_travel_time_handler extends views_handler_field {

  function option_definition() {
    $options = parent::option_definition();
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
  }

  function query() {
    // do nothing -- to override the parent query.
  }

  function render($data) {
    global $user;
    $dis_id = change_feeds_import_get_distributor_id($user->uid);
    $trip_id = $data->node_title;
    
    $query = db_select('node', 'n');
    $query->condition('n.title', $trip_id, "=");
    $query->condition('n.uid', $dis_id);
    $query->condition('n.type', 'dispatch_data');
    $query->condition('n.status', 1);
    $query->leftjoin('field_data_field_total_travel_time', 'ttt', 'ttt.entity_id = n.nid');
    $query->fields('ttt', array('field_total_travel_time_value'));
    $res = $query->execute()->fetchField();
    
    $hour_minute = explode('.',$res);
    $hour = $hour_minute[0].'H';
    $minute = str_pad($hour_minute[1], 2, "0", STR_PAD_RIGHT);
    $minute = round($minute*0.6).'M';
    $total_travel_time = $hour <= 9 ? '0'.$hour.','.$minute : $hour.','.$minute;
    
    return $total_travel_time;
  }

}

