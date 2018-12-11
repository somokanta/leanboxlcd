<?php

/* 
 * Get Supervisor Name
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function get_ptl_supervisor_list($dis_id,$resource_type) {
  if (!empty($dis_id) && !empty($resource_type)) {
    $query = db_select('node', 'n');
    $query->condition('n.type', 'resource_master');
    $query->condition('n.uid', $dis_id);
    $query->join('field_data_field_resource_type', 'rt', 'rt.entity_id = n.nid');
    $query->condition('rt.field_resource_type_value', $resource_type, '=');
    $query->join('field_data_field_resource_name', 'rn', 'rn.entity_id = n.nid');
    $query->fields('n', array('nid'));
    $query->fields('rn', array('field_resource_name_value'));
    $result = $query->execute()->fetchAllKeyed(0, 1);
  }
  return $result; 
}
