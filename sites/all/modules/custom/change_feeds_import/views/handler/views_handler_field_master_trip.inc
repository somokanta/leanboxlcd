<?php

/**
 * @file
 * Definition of example_handler_custom_field
 */

/**
 * Provides a custom views field.
 */
class views_handler_field_master_trip extends views_handler_field {

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
      // $van = $data->field_field_billing_van[0]['raw']['value'];
      // $trip_id = $data->node_title;
      // $uid = $node->node_uid;
      $trip_nid = $data->nid;

      $query = db_select('field_data_field_master_trip_id_reference', 'tr');
      $query->condition('field_master_trip_id_reference_nid', $trip_nid);
      $query->join('node', 'n', 'n.nid = tr.entity_id');
      $query->addExpression('GROUP_CONCAT(n.title)', 'titles');
      $query->condition('n.type', "dispatch_data");
      $trip_titles = $query->execute()->fetch(PDO::FETCH_ASSOC);
      return $trip_titles['titles'];
   }

}
