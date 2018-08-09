<?php

/*
  Implementation in Hook View Data
 */

function party_packing_ptl_views_data_alter(&$data) {

  //Custom field handler for Sequence
  $data['ptl_data_configuration']['sequence_quantity'] = array(
    'title' => t('Sequence quantity field'),
    'help' => t('Just a field created for Sequence quantity.'),
    //'real field' => 'nid',
    'field' => array(
      'handler' => 'views_handler_field_sequence',
    ),
  );
  
   //Custom field handler for Sequence
  $data['ptl_data_configuration']['total_quantity'] = array(
    'title' => t('Total quantity field'),
    'help' => t('Just a field created for Total quantity.'),
    //'real field' => 'nid',
    'field' => array(
      'handler' => 'views_handler_field_total_quantity',
    ),
  );

  return $data;
}
