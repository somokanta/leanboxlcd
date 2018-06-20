<?php

class dispatch_planning_updated_flag extends views_handler_filter_in_operator {
  function get_value_options() {
    $this->value_title = t('Updated Flag');
  //  $field = field_info_field($this->definition['field_name']);
    $this->value_options = array(
          // Put the list of your states/province/departments or whatever here.
          // These are just examples
  	  '0' => t('Blank'),
  	  '1' => t('Manual'),
  	  '2' => t('Api'),
  	);
  }
}

