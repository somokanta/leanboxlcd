<?php
function deliverycost_calculation_views_data() {
  $data = array();
  $data['views']['vehicle_contract_entry'] = array(
    'title' => t('Vehicle Contract Entry '),
    'help' => t('Vehicle Contract Entry.'),
    'field' => array(
      'handler' => 'handler_deliverycost_calculation',
    ),
  );
 $data['views']['vehicle_maintenace_entry'] = array(
    'title' => t('Vehicle maintenace'),
    'help' => t('Vehicle maintenace'),
    'field' => array(
      'handler' => 'handler_deliverycost_calculation',
    ),
  );
 $data['views']['resource_contract_entry'] = array(
    'title' => t('Resource contract'),
    'help' => t('Resource contract'),
    'field' => array(
      'handler' => 'handler_deliverycost_calculation',
    ),
  );
 $data['views']['resource_leave_entry'] = array(
    'title' => t('Resource Leave Entry'),
    'help' => t('Resource Leave Entry.'),
    'field' => array(
      'handler' => 'handler_deliverycost_calculation',
    ),
  );
 $data['views']['resource_extra_working'] = array(
    'title' => t('Resource Extra Working'),
    'help' => t('Resource Extra Working Entry.'),
    'field' => array(
      'handler' => 'handler_deliverycost_calculation',
    ),
  );
 
 $data['views']['vehicle_extra_working'] = array(
    'title' => t('Vehicle Extra Working'),
    'help' => t('Vehicle Extra Working Entry.'),
    'field' => array(
      'handler' => 'handler_deliverycost_calculation',
    ),
  );
    
  return $data;
}
