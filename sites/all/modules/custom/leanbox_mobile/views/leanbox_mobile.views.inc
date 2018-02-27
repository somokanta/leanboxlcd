<?php

/*
  Implementation in Hook View Data
 */

function leanbox_mobile_views_data() {
  $data = array();
  
  // Adding custom handler for Order Report
  $data['impact_data']['table'] = array(
    'group' => t('Impact data'),
    'title' => t('Impact data'),
    'help' => t('Impact data'),
  );
  
  $data['impact_data']['table']['base'] = array(
    'field' => 'no',
    'title' => t('Impact data'),
    'help' => t("unique identifiers."),
    // 'weight' => -10,
  );
  
  // Define this as a base table.
  $data['impact_data']['no'] = array(
    'title' => t('Impact data'),
    'help' => t("unique identifiers."),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['impact_data']['bill_num'] = array(
    'title' => t('Bill number'),
    'help' => t("unique identifiers."),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );  
  $data['impact_data']['distributor_id'] = array(
    'title' => t('Distributor Id'),
    'help' => t("Distributor Id"),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );  
   $data['impact_data']['impact_date'] = array(
    'title' => t('Impact Date'),
    'help' => t("Impact Date"),
     'field' => array(
       'handler' => 'views_handler_field_date',
       'click sortable' => TRUE,
     ),
     'sort' => array(
       'handler' => 'views_handler_sort_date',
     ),
     'filter' => array(
       'handler' => 'views_handler_filter_date',
     ),
     'argument' => array(
       'handler' => 'views_handler_filter_date',
     ),
  );  
  $data['impact_data']['flag'] = array(
    'title' => t('Flag'),
    'help' => t("Flag"),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );  
  
    $base_table = 'impact_data';
    $data[$base_table]['date_filter'] = array(
     'group' => t('Date'),
     'title' => t('Date (!base_table)', array('!base_table' => $base_table)),
     'help' => t('Filter any Views !base_table date field.', array('!base_table' => $base_table)),
     'filter' => array(
       'handler' => 'date_views_filter_handler',
       'empty field name' => t('Undated'),
       'is date' => TRUE,
     //'skip base' => $base_table,
     ),
   );
    // 'weight' => -10,
  //$data['order_summary']['table']['group'] = t('Assign nodes to users');
  return $data;
}
