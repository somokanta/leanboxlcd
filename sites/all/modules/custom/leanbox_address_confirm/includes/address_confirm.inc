<?php

/* if (user_access('hul_address_confirm_upload')) {
  echo "you have permission";
  } */

function check_hul_address_confirmed_or_not($party_code = array()) {
    global $user;
    $distributor_id = change_feeds_import_get_distributor_id($user->uid);
    $res = array();
    if (!empty($party_code)) {
        $query = db_select('node', 'n');
        $query->condition('n.type', 'outlet_master');
        $query->condition('n.uid', $distributor_id, '=');
        // Party Code
        $query->join('field_data_field_party_code', 'pc', 'pc.entity_id = n.nid');
        $query->addField('pc', 'field_party_code_value', 'party_code');
        $query->condition('pc.field_party_code_value', $party_code, '=');
        // Sales Day $sales_day
        $query->join('field_data_field_sales_day', 'sd', 'sd.entity_id = n.nid');
        $query->addField('sd', 'field_sales_day_value', 'sales_day');
        // PL group $pl_group
        $query->join('field_data_field_pl_group', 'plg', 'plg.entity_id = n.nid');
        $query->addField('plg', 'field_pl_group_value', 'pl_group');
        // Beat name $beat_name
        $query->join('field_data_field_beat_name', 'bn', 'bn.entity_id = n.nid');
        $query->addField('bn', 'field_beat_name_value', 'beat_name');
        //Hul code  $hul_code reference,
        $query->join('field_data_field_hul_code_reference', 'hc', 'hc.entity_id = n.nid');
        $query->addField('hc', 'field_hul_code_reference_nid', 'hul_nid');
        $query->join('node', 'hul', 'hc.field_hul_code_reference_nid = hul.nid');
        $query->condition('hul.type', 'hul_code_master');
        $query->condition('hul.uid', $distributor_id, '=');
        $query->addField('hul','title','hul_code');
        // Hul Status Not confirmed   $hul_status!  =1
        $query->leftJoin('field_data_field_hul_status', 'hul_st', 'hul_st.entity_id = hul.nid');
        $query->condition('hul_st.field_hul_status_value', 1, '!=');
        $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
    }
    return $res;
}
