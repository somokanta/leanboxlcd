<?php
/**
 * @file
 * Views definitions for user_hierarchy module.
 */

/**
 * Implements hook_views_data().
 */
function user_hierarchy_views_data() {
  $data = array();
  $data['user_hierarchy']['table']['group'] = t('DistributorID Filter');
  $data['user_hierarchy']['table']['join'] = array(
    // Exist in all views.
    '#global' => array(),
  );
 
  $data['user_hierarchy']['term_hierarchy'] = array(
    'title' => t('DistributorID Filter'),
    'help' => t('DistributorID Filter'),
    'field' => array(
      'handler' => 'user_hierarchy_handler_term_hierarchy',
      'click sortable' => TRUE  
    ),
      
    'sort' => array(
      'handler' => 'user_hierarchy_handler_term_hierarchy',
    ),
    'filter' => array(
      'handler' => 'user_hierarchy_handler_term_hierarchy',
    ),
    'argument' => array(
      'handler' => 'user_hierarchy_handler_term_hierarchy',
    ),
  );
  
 
  return $data;
}
