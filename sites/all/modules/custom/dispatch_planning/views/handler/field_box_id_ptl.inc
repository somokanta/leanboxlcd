<?php

/**
 * @file
 * Definition of example_handler_custom_field
 */

/**
 * Provides a custom views field.
 */
class field_box_id_ptl extends views_handler_field {

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
    $trip_id = $this->view->args[0];
    $bill_number = $data->node_title;
    $query1 = db_select('ptl_data_configuration', 'ptl');
    $query1->condition('ptl.trip_id', $trip_id, "=");
    $query1->condition('ptl.bill_number', $bill_number, "=");
    $query1->addExpression("GROUP_CONCAT(DISTINCT ptl.box_id separator ', ')", 'box_id');
    $query1->condition('ptl.box_id', '', "!=");
    $result = $query1->execute()->fetchField();
    //newly added
    $arr = explode(',', "$result");
    foreach ($arr as $val) {
      $bid = trim($val);
      $new_arr[$bid] =$bid; 
    }
    $box_id = implode(", ",$new_arr);
    return $box_id;
   }

}
