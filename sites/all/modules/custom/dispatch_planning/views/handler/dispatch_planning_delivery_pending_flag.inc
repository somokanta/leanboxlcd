<?php

class dispatch_planning_delivery_pending_flag extends views_handler_filter_in_operator {
  function get_value_options() {
    $this->value_title = t('Delivery Pending');
    $this->value_options = array(
  	  '0' => t('No'),
  	  '1' => t('Yes'),
   	);
  }
}

