<?php

/**
 * @file
 * Definition of example_handler_custom_field
 */

/**
 * Provides a custom views field.
 */
class field_bill_date_handler extends views_handler_field {

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
    $user_id = $user->uid;
    $distributor_id = change_feeds_import_get_distributor_id($user_id);
    $trip_id = $data->node_title;
    
    $query = db_select('field_data_field_trip_id_reference', 'tr');
    $query->join('node', 'n', 'n.nid = tr.field_trip_id_reference_nid');
    $query->condition('n.title', $trip_id);
    $query->condition('n.uid', $distributor_id);
    $query->join('field_data_field_sr_new_bill_date', 'bd', 'tr.entity_id = bd.entity_id');
    $query->addExpression("GROUP_CONCAT(DISTINCT(DATE_FORMAT(CONVERT_TZ(FROM_UNIXTIME(bd.field_sr_new_bill_date_value),'+00:00','+05:30'), '%d-%b-%y')))", 'bill_date');
    $bill_dates = $query->execute()->fetchField();
    return $bill_dates;
  }

}
