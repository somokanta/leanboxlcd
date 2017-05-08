<?php

/**
 * @file
 * Definition of views_handler_field_outlets_per_van.
 */

/**
 * Returns outlets_per_van
 * @ingroup views_field_handlers
 */
class views_handler_field_outlets_per_van extends views_handler_field {

  function render($values) {

    $van_pl_group = $values->van_pl_group;
    $seq = $values->_field_data[$this->field_alias]['entity']->field_revised_sequence['und'][0]['value'];
    
    $query = db_select('node', 'n');
    $query->addTag('node_uid');
    $query->join('field_data_field_stops_per_van', 'fsv', 'fsv.entity_id = n.nid');
    $query->condition('n.type', 'van_pl_group_master');
    $query->condition('n.title', $van_pl_group);
    $query->fields('fsv', array('field_stops_per_van_value'));
    $stops_per_van = $query->execute()->fetchField();

    return ($seq/$stops_per_van);
  }

}