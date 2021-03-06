<?php

/**
 * @file
 * Definition of example_handler_custom_field
 */

/**
 * Provides a custom views field.
 */

class field_fresh_dp_van_handler extends views_handler_field {
  
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
    
    $trip_id = $data->node_title;
    $distributor_id = get_distributorid_of_trip($trip_id);
    
    $query = db_select('node', 'n');
    $query->condition('n.type', 'sales_register_data');
    $query->leftjoin('field_data_field_trip_id_reference', 'tir', 'tir.entity_id = n.nid');
    $query->leftjoin('node', 'n1', 'tir.field_trip_id_reference_nid = n1.nid');
    $query->condition('n1.title', $trip_id);
    $query->condition('n1.uid', $distributor_id);
    $query->join('field_data_field_previous_trip', 'pt', 'pt.entity_id = n.nid');
    $query->fields('pt', array('field_previous_trip_value'));
    $result = $query->execute()->fetchAll();

    if (!empty($result)) {
        $query = db_select('node', 'n');
        $query->condition('n.type', 'sales_register_data');
        $query->leftjoin('field_data_field_trip_id_reference', 'tir', 'tir.entity_id = n.nid');
        $query->leftjoin('node', 'n1', 'tir.field_trip_id_reference_nid = n1.nid');
        $query->condition('n1.title', $trip_id);
        $query->condition('n1.uid', $distributor_id);
        $query->leftjoin('field_data_field_previous_trip', 'pt', 'pt.entity_id = n.nid');
        $query->isnull('pt.field_previous_trip_value');
        $query->fields('n', array('title'));
        $result2 = $query->execute()->fetchAll();
        if (!empty($result2)) {
            return 'Fresh,DP Van';
        }
      return 'DP Van';
    }
    else {
      return 'Fresh Van';
    }
  }
}

