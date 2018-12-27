<?php

/* 
 * Get Supervisor Name
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function get_ptl_supervisor_list($dis_id,$resource_type) {
  $result = array();
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

// Save surpervisor nameeeeeeee Saving Packing Auditor Name

function save_dispatch_qty_supervisor_name($trip_id,$supervisor_resource_nid,$distributor_id,$field) {
  if(!empty($trip_id) && !empty($supervisor_resource_nid) && !empty($distributor_id) && !empty($field)) {
    $query = db_select('node', 'n');
    $query->condition('n.title', $trip_id);
    $query->condition('n.type', 'dispatch_data');
    $query->condition('n.uid', $distributor_id);
    $query->fields('n', array('nid'));
    $nid = $query->execute()->fetchField();
    if (!empty($nid)) {
      $node_obj = node_load($nid);
      if(!empty($node_obj)) {
        //  Used this field in below two screens
        /* 	http://dev.leanbox.in/ptl-loose-confirmation
         *  http://dev.leanbox.in/master-ptl-loose-confirmation
         */
        if($field == 'field_packing_auditor_loose') {
          $node_obj->field_packing_auditor_loose[LANGUAGE_NONE][0]['nid'] = $supervisor_resource_nid;
        }
        // field used in http://dev.leanbox.in/ptl-retail-cld-confirmation
        if($field == 'field_packing_auditor_cld') {
          $node_obj->field_packing_auditor_cld[LANGUAGE_NONE][0]['nid'] = $supervisor_resource_nid;
        }
        // 	http://dev.leanbox.in/bill-wise-editing-cld
        if($field == 'field_bill_cld_confirm_resource') {
          $node_obj->field_bill_cld_confirm_resource[LANGUAGE_NONE][0]['nid'] = $supervisor_resource_nid;
        }
        
        node_save($node_obj);
      }
    }
        
  }
  
}
function get_auditor_name($dis_id,$trip_id,$field) {
  $result = 0;
  if (!empty($dis_id) && !empty($trip_id) && !empty($field)) { 
    $query = db_select('node', 'n');
    $query->condition('n.type', 'dispatch_data');
    $query->condition('n.uid', $dis_id);
    if($field == 'field_packing_auditor_loose') {
       $query->join('field_data_field_packing_auditor_loose', 'packing_auditor', 'packing_auditor.entity_id = n.nid');
       $query->fields('packing_auditor', array('field_packing_auditor_loose_nid'));
    }
    else if($field == 'field_packing_auditor_cld') {
       $query->join('field_data_field_packing_auditor_cld', 'packing_auditor', 'packing_auditor.entity_id = n.nid');
       $query->fields('packing_auditor', array('field_packing_auditor_cld_nid'));
    }
    else if($field == 'field_bill_cld_confirm_resource') {
       $query->join('field_data_field_bill_cld_confirm_resource', 'packing_auditor', 'packing_auditor.entity_id = n.nid');
       $query->fields('packing_auditor', array('field_bill_cld_confirm_resource_nid'));
    }
    
    $query->condition('n.title', $trip_id, '=');
    $result = $query->execute()->fetchField();
  }
  return $result; 
}