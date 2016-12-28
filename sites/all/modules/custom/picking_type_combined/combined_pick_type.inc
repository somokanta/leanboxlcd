<?php

function fetch_combined_status() {
     
    $query = db_select('node', 'n');
    $query->condition('n.type', 'picking', '=');
    $query->join('field_data_field_picking_van_id', 'vanid', 'vanid.entity_id = n.nid');
    $query->condition('vanid.bundle', 'picking', '=');
    $query->join('field_data_field_picking_picking_type', 'ptype', 'ptype.entity_id = n.nid');
    $query->condition('ptype.bundle', 'picking', '=');
    $query->join('field_data_field_picking_godown_area_id', 'goid', 'goid.entity_id = n.nid');
    $query->condition('goid.bundle', 'picking', '=');
    $query->join('field_data_field_picking_loading_date', 'ldate', 'ldate.entity_id = n.nid');
    $query->condition('ldate.bundle', 'picking', '=');
    $query->join('field_data_field_pick_units', 'punits', 'punits.entity_id = n.nid');
    $query->condition('punits.bundle', 'picking', '=');
    $query->join('field_data_field_pick_clds', 'pclds', 'pclds.entity_id = n.nid');
    $query->condition('pclds.bundle', 'picking', '=');
    $query->join('field_data_field_picking_pick_supervisor', 'psuper', 'psuper.entity_id = n.nid');
    $query->condition('psuper.bundle', 'picking', '=');
    $query->join('field_data_field_combined_nid', 'cid', 'cid.entity_id = n.nid');
    $query->condition('cid.bundle', 'picking', '=');
    $query->join('field_data_field_picking_picker_name', 'picker', 'picker.entity_id = n.nid');
    $query->condition('picker.bundle', 'picking', '=');
    $query->join('field_data_field_picking_picking_team_count', 'tcount', 'tcount.entity_id = n.nid');
    $query->condition('tcount.bundle', 'picking', '=');
    $query->join('field_data_field_picking_status', 'pstatus', 'pstatus.entity_id = n.nid');
    $query->condition('pstatus.bundle', 'picking', '=');
    $query->join('field_data_field_picking_start_time', 'pstart', 'pstart.entity_id = n.nid');
    $query->condition('pstart.bundle', 'picking', '=');
    $query->fields('n', array('nid'));
    $query->fields('vanid', array('field_picking_van_id_value'));
    $query->fields('ptype', array('field_picking_picking_type_value'));
    $query->fields('goid', array('field_picking_godown_area_id_value'));
    $query->fields('ldate', array('field_picking_loading_date_value'));
    $query->fields('punits', array('field_pick_units_value'));
    $query->fields('pclds', array('field_pick_clds_value'));
    $query->fields('psuper', array('field_picking_pick_supervisor_value'));
    $query->fields('cid', array('field_combined_nid_value'));
    $query->fields('picker', array('field_picking_picker_name_value'));
    $query->fields('tcount', array('field_picking_picking_team_count_value'));
    $query->fields('pstatus', array('field_picking_status_value'));
    $query->fields('pstart', array('field_picking_start_time_value'));
    $query->groupBy('field_combined_nid_value');
    $results = $query->execute()->fetchAll();

    foreach($results as $key => $val) {
      if(!is_numeric($val->field_combined_nid_value)) {
        $val->field_picking_picking_type_value = 'combined';
      }
      // if($val->field_picking_status_value == 1) {
      //   $val->field_picking_status_value = 'In Process';
      // }
      // if($val->field_picking_status_value == 2) {
      //   $val->field_picking_status_value = 'Completed';
      // }
      $result_final[] =  array('godown area id' => $val->field_picking_godown_area_id_value , 'picking type' => $val->field_picking_picking_type_value , 'loading date'=> date("d-m-y", strtotime($val->field_picking_loading_date_value)) , 'van id' => $val->field_picking_van_id_value , 'pick units' => $val->field_pick_units_value, 'pick clds' => $var->field_pick_clds_value, 'pick supervisor' => $val->field_picking_pick_supervisor_value, 'picker name' => $val->field_picking_picker_name_value, 'picking team count' => $val->field_picking_picking_team_count_value, 'status' => $val->field_picking_status_value, 'combined nid' => $val->field_combined_nid_value, 'picking start time' => date( 'g:i A', strtotime($val->field_picking_start_time_value)) , 'nid' => $val->nid);
    }

    $result['data'] =$result_final;
    return $result;
//dpm(node_load(374590));
//dsm($results);
}

