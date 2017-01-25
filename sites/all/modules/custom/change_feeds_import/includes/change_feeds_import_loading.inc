<?php

function change_feeds_import_check_party_packing_or_not($van_type, $nid) {
  if ($van_type == "0") {
    $sub_query2 = db_select('node', 'n');
    $sub_query2->condition('n.nid', $nid, '=');
    $sub_query2->innerjoin('field_data_field_loading_loose', 'll', 'll.entity_id = n.nid');
    $sub_query2->innerjoin('field_data_field_loading_bulk', 'lb', 'lb.entity_id = n.nid');
    $sub_query2->innerjoin('field_data_field_loading_lines', 'lin', 'lin.entity_id = n.nid');
    $sub_query2->innerjoin('field_data_field_loader_sheet_lines', 'lsl', 'lsl.entity_id = n.nid');
    $sub_query2->fields('n', array('nid'));
    $sub_query2->fields('ll', array('	field_loading_loose_value'));
    $sub_query2->fields('lb', array('field_loading_bulk_value'));
    $sub_query2->fields('lin', array('field_loading_lines_value'));
    $sub_query2->fields('lsl', array('	field_loader_sheet_lines_value'));
    $billing_result = $sub_query2->execute()->fetchASSOC();
    $output['result'] = $billing_result;
  }
  else {
    $sub_query1 = db_select('node', 'n');
    $sub_query1->condition('n.nid', 384835, '=');
    $sub_query1->innerjoin('field_data_field_loading_van_id', 'flv', 'flv.entity_id = n.nid');
    $sub_query1->innerjoin('field_data_field_distributor_loading_date', 'fld', 'fld.entity_id = n.nid');
    $sub_query1->innerjoin('field_data_field_loading_van_id', 'new_flv', 'new_flv.field_loading_van_id_value = flv.field_loading_van_id_value');
    $sub_query1->innerjoin('field_data_field_picking_van_id', 'pld', 'new_flv.field_loading_van_id_value = pld.field_picking_van_id_value');
    $sub_query1->innerjoin('field_data_field_distributor_loading_date', 'new_fld', 'new_fld.field_distributor_loading_date_value = fld.field_distributor_loading_date_value');
    $sub_query1->innerjoin('field_data_field_picking_loading_date', 'plo', 'new_fld.field_distributor_loading_date_value = plo.field_picking_loading_date_value');
    $sub_query1->innerjoin('field_data_field_pick_lines', 'pick', 'pick.entity_id = pld.entity_id');
    $sub_query1->innerjoin('field_data_field_picking_loading_sheet_line', 'lsl', 'lsl.entity_id = pld.entity_id');
    $sub_query1->groupBy('pld.field_picking_van_id_value', 'plo.field_picking_loading_date_value');
    $sub_query1->fields('plo', array('field_picking_loading_date_value'));
    $sub_query1->fields('pld', array('field_picking_van_id_value'));
    $sub_query1->fields('pick', array('field_pick_lines_value'));
    $sub_query1->fields('lsl', array('field_picking_loading_sheet_line_value'));
    $sub_query1->fields('n', array('nid'));
    $sub_query1->fields('pld', array('entity_id'));
    $result = $sub_query1->execute()->fetchAll(PDO::FETCH_ASSOC);
    $output['MESSAGE'] = $result;
  }

  return $output;
}
