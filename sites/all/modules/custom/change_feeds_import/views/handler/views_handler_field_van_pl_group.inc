<?php

/**
 * @file
 * Definition of views_handler_field_van_pl_group.
 */

/**
 * Returns van pl group from outlet master
 * @ingroup views_field_handlers
 */
class views_handler_field_van_pl_group extends views_handler_field {

  function render($values) {
    
    dpm($values);
    $value = $this->get_value($values);
    
  $query = db_select('node', 'n');
  $query->addTag('node_uid');
  $query->join('field_data_field_channel', 'ppmch', 'ppmch.entity_id = n.nid');
  $query->join('field_data_field_pl_group', 'ppmpl', 'ppmpl.entity_id = n.nid');
  $query->join('field_data_field_van_pl_group', 'ppplg', 'n.nid = ppplg.entity_id');
  $query->condition('n.type', 'party_pack_master');
  $query->condition('n.type', 'party_pack_master');
  $query->condition('n.type', 'party_pack_master');
  $query->fields('ppplg', array('field_van_pl_group_value'));
  //$van_pl_grp = $query->execute()->fetchField();
  
  return '00';
  
  }
}