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
    $channel = $values->_field_data[$this->field_alias]['entity']->field_channel['und'][0]['value'];
    $pl_group = $values->_field_data[$this->field_alias]['entity']->field_pl_group['und'][0]['value'];

    $query = db_select('node', 'n');
    $query->addTag('node_uid');
    $query->join('field_data_field_channel', 'ppmch', 'ppmch.entity_id = n.nid');
    $query->join('field_data_field_pl_group', 'ppmpl', 'ppmpl.entity_id = n.nid');
    $query->join('field_data_field_van_pl_group', 'ppplg', 'n.nid = ppplg.entity_id');
    $query->condition('n.type', 'party_pack_master');
    $query->condition('ppmch.field_channel_value', $channel);
    $query->condition('ppmpl.field_pl_group_value', $pl_group);
    $query->fields('ppplg', array('field_van_pl_group_value'));
    $van_pl_grp = $query->execute()->fetchField();


    $values->outlets_per_van = 50;

    return $van_pl_grp;
  }

}
