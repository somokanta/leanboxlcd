<?php

/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

class dispatch_planning_bill_status extends views_handler_filter_in_operator {
  function get_value_options() {
    $this->value_title = t('DP res_status');
    //$field = field_info_field($this->definition['field_name']);
    $field = field_info_field('field_reason_type');
    $allowed_values = list_allowed_values($field);
//    $this->value_options = array(
//          // Put the list of your states/province/departments or whatever here.
//          // These are just examples
//  	  'deliver' => t('deliver'),
//  	  'undeliver' => t('undeliver'),
//  	  'partial_deliver' => t('partial_deliver'),
//  	);
    $this->value_options = $allowed_values;
  }
}
