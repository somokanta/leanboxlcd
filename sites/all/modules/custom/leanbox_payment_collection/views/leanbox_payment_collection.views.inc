<?php

/**
 * Implements hook_views_data().
 */
function leanbox_payment_collection_views_data() {
   
  $data = array();
  
  $data['views']['footer_area_link_custom'] = array(
    'title' => t('Footer field link'),
    'help' => t('Add custom link in footer.'),
    'area' => array(
      'handler' => 'footer_link_custom_field',
    ),
  );
  
  $data['views']['footer_area_total_custom'] = array(
    'title' => t('Footer field total'),
    'help' => t('Add custom total in footer.'),
    'area' => array(
      'handler' => 'footer_total_custom_field',
    ),
  );
  
  return $data;
}