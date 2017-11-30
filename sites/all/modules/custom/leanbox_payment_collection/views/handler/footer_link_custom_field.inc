<?php

/**
 * @file
 * Definition of example_handler_custom_field
 */

/**
 * Provides a custom views field.
 */
class footer_link_custom_field extends views_handler_field {

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

  function render($empty = FALSE) {
    $params = drupal_get_query_parameters();
  
    $assigned_person = $params['field_assigned_person_value'];
    
    $start_assigned_date = strtotime($params['field_assignment_date_value']['value']['date']);
    $start_date = date('Ymd',$start_assigned_date);
    
//    $end_assigned_date = strtotime($params['field_assignment_date_value']['max']['date']);
//    $end_date = date('Ymd',$end_assigned_date);
//    
    if(!empty($assigned_person) && !empty($start_date)) {
      $result = l('Download', 'download-bills-acknowledgement/'.$assigned_person.'/'.$start_date);
    }
    return $result;
  }

}
