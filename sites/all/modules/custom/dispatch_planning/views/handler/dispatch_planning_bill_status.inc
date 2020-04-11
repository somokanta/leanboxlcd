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
    $allowed_values['empty_bill_status'] = 'empty_bill_status';
    //unset($allowed_values['delivery_pending']);// This status is not present in farEye
//    $this->value_options = array(
//          // Put the list of your states/province/departments or whatever here.
//          // These are just examples
//  	  'deliver' => t('deliver'),
//  	  'undeliver' => t('undeliver'),
//  	  'partial_deliver' => t('partial_deliver'),
//  	);
    $this->value_options = $allowed_values;
  }
  function query() {
      
        global $user;
        $view = $this->view;
        $bill_status = $view->exposed_input['res_status'];
        //dpm($bill_status);
        $blll_status_input = array();
        $empty_filter = 0;
        if (!empty($view) && !empty($bill_status) && $bill_status != 'All') {
            $combined_bill_status = explode(',', variable_get('combined_res_status'));
            if (!empty($combined_bill_status[0])) {
                //foreach ($bill_status as $key => $res_status_value) {
                if (in_array($bill_status, $combined_bill_status)) {
                    $this->query->add_where(0, "dispatch_planning_api_data.res_status", $combined_bill_status, 'IN');
                }
                else if ($bill_status == 'empty_bill_status') {

                    $this->query->add_where_expression(0, 'dispatch_planning_api_data.res_status IS NULL');
                    $empty_filter = 1;
                }
                else if ($bill_status == 'delivery_pending') {
                    $this->query->add_where(0, "dispatch_planning_api_data.delivery_pending_flag", 1, '=');
                    $this->query->add_where(0, "dispatch_planning_api_data.res_status", 'undeliver', '=');
                }
                else if ($bill_status == 'undeliver') {
                    $this->query->add_where(0, "dispatch_planning_api_data.delivery_pending_flag", 0, '=');
                    $this->query->add_where(0, "dispatch_planning_api_data.res_status", 'undeliver', '=');
                }
                else {
                    $this->query->add_where(0, "dispatch_planning_api_data.res_status", $bill_status, '=');
                }
                //}
            }
            else if ($bill_status == 'empty_bill_status') {

                $this->query->add_where_expression(0, 'dispatch_planning_api_data.res_status IS NULL');
                $empty_filter = 1;
            }
            else if ($bill_status == 'delivery_pending') {
                $this->query->add_where(0, "dispatch_planning_api_data.delivery_pending_flag", 1, '=');
                $this->query->add_where(0, "dispatch_planning_api_data.res_status", 'undeliver', '=');
            }
            else if ($bill_status == 'undeliver') {
                $this->query->add_where(0, "dispatch_planning_api_data.delivery_pending_flag", 0, '=');
                $this->query->add_where(0, "dispatch_planning_api_data.res_status", 'undeliver', '=');
            }
            else {
                $this->query->add_where(0, "dispatch_planning_api_data.res_status", $bill_status, '=');
            }
        }
    }
}
