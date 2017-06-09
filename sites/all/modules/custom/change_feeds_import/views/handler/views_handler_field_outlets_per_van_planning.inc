<?php

/**
 * @file
 * Definition of views_handler_field_outlets_per_van.
 */

/**
 * Returns outlets_per_van
 * @ingroup views_field_handlers
 */
class views_handler_field_outlets_per_van_planning extends views_handler_field {

  function render($values) {
    $van_pl_group = $values->field_field_dva_van_pl_group[0]['raw']['value'];
    if (!empty($van_pl_group)) {
      $seq = $values->_field_data[$this->field_alias]['entity']->field_revised_sequence['und'][0]['value'];

      $query = db_select('node', 'n');
      $query->addTag('node_uid');
      $query->join('field_data_field_stops_per_van', 'fsv', 'fsv.entity_id = n.nid');
      $query->condition('n.type', 'van_pl_group_master');
      $query->condition('n.title', $van_pl_group);
      $query->fields('fsv', array('field_stops_per_van_value'));
      $stops_per_van = $query->execute()->fetchField();

      return ceil($seq / $stops_per_van);
    }
    else {
      return "";
    }
  }

}
