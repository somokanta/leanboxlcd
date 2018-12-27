<?php

/**
 * @file
 * Definition of example_handler_custom_field
 */

/**
 * Provides a custom views field.
 */
class field_asset_dispatch_count extends views_handler_field {

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
    $trip_id = $this->view->result[0]->field_field_trip_id[0]['raw']['value'];
    
    $query = db_select('node', 'n');
    $query->condition('n.type', 'assets_count');
    $query->condition('n.uid', $dis_id);
    $query->condition('n.title', $trip_id);
    $query->leftjoin('field_data_field_dispatch_count', 'dc', 'dc.entity_id = n.nid');
    $query->addExpression("SUM(dc.field_dispatch_count_value)", 'total_dispatch_count_value');
    $res = $query->execute()->fetchField();
    
    return $res;
  }
}
