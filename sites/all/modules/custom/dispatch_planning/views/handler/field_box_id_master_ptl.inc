<?php

/**
 * @file
 * Definition of example_handler_custom_field
 */

/**
 * Provides a custom views field.
 */
class field_box_id_master_ptl extends views_handler_field {

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
    $ptl_job = $data->field_field_master_trip_id_reference[0]['raw']['node']->title;
    $trip_id = $data->node_title;
    $query1 = db_select('master_ptl_data', 'mptl');
    $query1->condition('mptl.ptl_job', $ptl_job, "=");
    $query1->condition('mptl.trip_id', $trip_id, "=");
    $query1->addExpression('GROUP_CONCAT(DISTINCT mptl.box_id)', 'box_id');
    $query1->condition('mptl.box_id', '', "!=");
    $box_id = $query1->execute()->fetchField();
    return $box_id;
   }

}
