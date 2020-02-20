<?php
/**
 * @file
 * Views definitions for user_hierarchy module.
 */

/**
 * Implements hook_views_data().
 */
function facility_code_filter_views_data() {
  $data = array();
  $data['facility_code_filter']['table']['group'] = t('Facility Code  Filter');
  $data['facility_code_filter']['table']['join'] = array(
    // Exist in all views.
    '#global' => array(),
  );
 
  $data['facility_code_filter']['term_hierarchy'] = array(
    'title' => t('Facility Code Filter'),
    'help' => t('Facility Code Filter'),
    'field' => array(
      'handler' => 'facility_code_filter_handler_term_hierarchy',
      'click sortable' => TRUE  
    ),
      
    'sort' => array(
      'handler' => 'facility_code_filter_handler_term_hierarchy',
    ),
    'filter' => array(
      'handler' => 'facility_code_filter_handler_term_hierarchy',
    ),
    'argument' => array(
      'handler' => 'facility_code_filter_handler_term_hierarchy',
    ),
  );
  
 
  return $data;
}
