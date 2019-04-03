<?php

/**
 * @file
 * Definition of example_handler_custom_field
 */

/**
 * Provides a custom views field.
 */
class vrs_printout_excess_value extends views_handler_field {

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
            $query = db_select('excess_inventory_table', 'eit');
            $query->condition('eit.trip_id', $trip_id);
            $query->condition('eit.distributor_id', $distributor_id);
            $query->condition('eit.credit_to_cashier', 1);
            $query->addExpression('ROUND(SUM(eit.excess_qty * eit.mrp * 0.9),2)', 'excess_value');
            $result = $query->execute()->fetchField();
        }
        return $result;
    }
    

}

