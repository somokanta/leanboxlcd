<?php

/**
 * @file
 * Definition of example_handler_custom_field
 */

/**
 * Provides a custom views field.
 */
class vrs_printout_total_bill_value extends views_handler_field {

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
        $distributor_id = change_feeds_import_get_distributor_id($user->uid);
        $trip_id = $data->vrs_trip_total_trip_id;
        if(!empty($trip_id) && !empty($distributor_id)) {
            $query = db_select('node', 'disp_data');
            $query->condition('disp_data.type', 'dispatch_data');
            $query->condition('disp_data .uid', $distributor_id);
            $query->condition('disp_data.title',$trip_id);
            $query->join('field_data_field_trip_id_reference', 'tr', 'tr.field_trip_id_reference_nid = disp_data.nid');
            $query->join('node', 'sr', 'sr.nid =tr.entity_id');
            $query->condition('sr.type', 'sales_register_data');
            $query->condition('sr.uid', $distributor_id);
            $query->join('field_data_field_bill_value', 'bv', 'bv.entity_id = sr.nid');
            $query->addExpression("SUM(bv.field_bill_value_value)", 'field_bill_value_value');
            $res = $query->execute()->fetchField();
        }
        return $res;
    }
    

}
