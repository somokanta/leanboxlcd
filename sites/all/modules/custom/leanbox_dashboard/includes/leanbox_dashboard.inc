<?php

/**
 *  This file Includes callback function
 *  defined in for custom block
 */

/**
 * 
 * @param array $form
 * @param array $form_state
 * @return string
 */
function leanbox_dashboard_chart_filter_partypack_form($form, &$form_state) {
  $form = array();
  if (arg(1) == 'party_packing') {
    $form['start_date'] = array(
      '#type' => 'date_popup',
      '#title' => t('Loading Date From'),
      '#required' => TRUE,
      '#date_format' => 'Y-m-d',
      '#default_value' => date('Y-m-d'),
    );
    $form['end_date'] = array(
      '#type' => 'date_popup',
      '#title' => t('Loading Date To'),
      '#required' => TRUE,
      '#date_format' => 'Y-m-d',
      '#default_value' => date('Y-m-d'),
    );
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => 'Submit',
      '#prefix' => '<div id ="party-packing-submit">',
      '#suffix' => '</div>',
      '#attributes' => array('class' => array('party-packing-submit')),
    );
  }

  $form['party_packing'] = array(
    '#markup' => '<a href = "/leanbox-chart-details/party_packing" target = "_blank"><div id="party_packing_div"></div></a>',
  );


  return $form;
}

/**
 * 
 * @param array $form
 * @param array $form_state
 * @return string
 */
function leanbox_dashboard_chart_filter_picking_form($form, &$form_state) {
  $form = array();

  if (arg(1) == 'picking') {
    $form['start_date'] = array(
      '#type' => 'date_popup',
      '#title' => t('Loading Date From'),
      '#required' => TRUE,
      '#date_format' => 'Y-m-d',
      '#default_value' => date('Y-m-d'),
    );
    $form['end_date'] = array(
      '#type' => 'date_popup',
      '#title' => t('Loading Date To'),
      '#required' => TRUE,
      '#date_format' => 'Y-m-d',
      '#default_value' => date('Y-m-d'),
    );
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => 'Submit',
      '#prefix' => '<div id ="picking-submit">',
      '#suffix' => '</div>',
      '#attributes' => array('class' => array('picking-submit')),
    );
  }

  $form['picking'] = array(
    '#markup' => '<a href = "/leanbox-chart-details/picking" target = "_blank"><div id="picking_div"></div></a>',
  );

  return $form;
}

/**
 * 
 * @param array $form
 * @param array $form_state
 * @return string
 */
function leanbox_dashboard_chart_filter_unloading_form($form, &$form_state) {
  $form = array();
  if (arg(1) == 'unloading') {
    $form['start_date'] = array(
      '#type' => 'date_popup',
      '#title' => t('EGIR Date From'),
      '#required' => TRUE,
      '#date_format' => 'Y-m-d',
      '#default_value' => date('Y-m-d'),
    );
    $form['end_date'] = array(
      '#type' => 'date_popup',
      '#title' => t('EGIR Date To'),
      '#required' => TRUE,
      '#date_format' => 'Y-m-d',
      '#default_value' => date('Y-m-d'),
    );
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => 'Submit',
      '#prefix' => '<div id ="unloading-submit">',
      '#suffix' => '</div>',
      '#attributes' => array('class' => array('unloading-submit')),
    );
  }


  $form['unloading'] = array(
    '#markup' => '<a href = "/leanbox-chart-details/unloading"><div id="unloading_div"></div></a>',
  );

  return $form;
}

/**
 * Callback function for block party_packing
 * 
 * @return html
 */
function leanbox_dashboard_chart_query_details($activity_type, $start_date = '', $end_date = '') {
  global $user;
// Store distributor id in author in each node
  $distributor_id = change_feeds_import_get_distributor_id($user->uid);
  if (!empty($start_date)) {
    $start_date = strtotime($start_date);
  }
  else {
    // $time = strtotime('-20 days');
    $time = strtotime(date('Y-m-d'));
    $start_date = strtotime(date("Y-m-d", $time));
  }
  if (!empty($end_date)) {
    $end_date = strtotime($end_date) + 86399;
  }
  else {
    $end_date = strtotime(date('Y-m-d'));
  }

  if ($activity_type == 'party_packing') {

    $query = db_select('node', 'n');
    $query->fields('n', array('nid'));
    $query->condition('n.type', 'party_packing');
    $query->condition('n.uid', $distributor_id);
    $query->join('field_data_field_party_packing_check', 'pc', 'pc.entity_id = n.nid');
    $query->condition('pc.field_party_packing_check_value', 1); // 1 => YES
    $query->join('field_data_field_party_packing_type', 'pt', 'pt.entity_id = n.nid');
    $query->fields('pt', array('field_party_packing_type_value'));
    $query->leftjoin('field_data_field__party_packingstatus', 's', 's.entity_id = n.nid');
    $query->fields('s', array('field__party_packingstatus_value'));
    $query->join('field_data_field_party_packing_loading_date', 'd', 'd.entity_id = n.nid');
    $query->fields('d', array('field_party_packing_loading_date_value'));
    $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

    //$pp['not_start'] = $pp['in_prg'] = $pp['comp'] = $non_pp['not_start'] = $non_pp['in_prg'] = $non_pp['comp'] = 0;
    $data1[] = array('PriceList Group', 'Not Started', 'In Progress', 'Completed');
    if (!empty($res)) {
      $field = field_info_field('field_party_packing_type');
      $allowed_values = list_allowed_values($field);
      foreach ($res as $key => $val) {

        $type = $val['field_party_packing_type_value'];
        if (!isset($output[$allowed_values[$type]])) {
          $output[$allowed_values[$type]] = array(
            'not_start' => 0,
            'in_prg' => 0,
            'comp' => 0
          );
        }
        $loading_date = strtotime($val['field_party_packing_loading_date_value']);

        if ($loading_date >= $start_date && $loading_date <= $end_date) {

          if (empty($val['field__party_packingstatus_value'])) {
            $output[$allowed_values[$type]]['not_start'] += 1;
          }
          else if ($val['field__party_packingstatus_value'] == 1) {
            $output[$allowed_values[$type]]['in_prg'] += 1;
          }
          else {
            $output[$allowed_values[$type]]['comp'] += 1;
          }
        }
      }

      foreach ($output as $key => $val) {
        $data1[] = array($key, $val['not_start'], $val['in_prg'], $val['comp']);
      }
    }
    else {
      $data1[] = array('No Result Found for Party Packing Type', 0, 0, 0);
    }
    return $data1;
  }
  else if ($activity_type == 'picking') {


    $query = db_select('field_data_field_picking_godown_area_id', 'g');
    $query->addExpression('DISTINCT(field_picking_godown_area_id_value)', 'uid_count');
    $query->condition('g.bundle', 'picking');
    $list = $query->execute()->fetchCol();

    $query = db_select('node', 'n');
    $query->fields('n', array('nid'));
    $query->condition('n.type', 'picking');
    $query->condition('n.uid', $distributor_id);
    $query->join('field_data_field_picking_godown_area_id', 'pt', 'pt.entity_id = n.nid');
    $query->fields('pt', array('field_picking_godown_area_id_value'));
    $query->leftjoin('field_data_field_picking_status', 's', 's.entity_id = n.nid');
    $query->fields('s', array('field_picking_status_value'));
    $query->join('field_data_field_picking_picking_type', 'pp', 'pp.entity_id = n.nid');
    $query->fields('pp', array('field_picking_picking_type_value'));
    $query->join('field_data_field_picking_loading_date', 'd', 'd.entity_id = n.nid');
    $query->fields('d', array('field_picking_loading_date_value'));
    $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

    $data1[] = array('PriceList Group', 'Not Started', 'In Progress', 'Completed');
    if (!empty($res)) {
      foreach ($list as $data => $data_list) {

        if (!isset($output[$data_list]['loose'])) {
          $output[$data_list]['loose'] = array(
            'not_start' => 0,
            'in_prg' => 0,
            'comp' => 0
          );
        }
        if (!isset($output[$data_list]['bulk'])) {
          $output[$data_list]['bulk'] = array(
            'not_start' => 0,
            'in_prg' => 0,
            'comp' => 0
          );
        }

        foreach ($res as $key => $val) {
          $loading_date = strtotime($val['field_picking_loading_date_value']);


          if ($loading_date >= $start_date && $loading_date <= $end_date) {
            if ($val['field_picking_godown_area_id_value'] == $data_list) {

              $type = (strtolower($val['field_picking_picking_type_value']) == 'Loose') ? 'loose' : 'bulk';
              if (empty($val['field_picking_status_value'])) {
                $output[$data_list][$type]['not_start'] += 1;
              }
              else if ($val['field_picking_status_value'] == 1) {
                $output[$data_list][$type]['in_prg'] += 1;
              }
              else {
                $output[$data_list][$type]['comp'] += 1;
              }
            }
          }
        }
      }

      foreach ($output as $key => $val) {
        foreach ($val as $key1 => $val1) {
          $data1[] = array($key . ' ' . $key1, $val1['not_start'], $val1['in_prg'], $val1['comp']);
        }
      }
    }
    else {
      $data1[] = array('No Result Found for Pricelist Group', 0, 0, 0);
    }
    return $data1;
  }
  else if ($activity_type == 'unloading') {
    $query = db_select('node', 'n');
    $query->fields('n', array('nid'));
    $query->condition('n.type', 'egir');
    $query->condition('n.uid', $distributor_id);
    $query->leftjoin('field_data_field_unloading_status', 'pt', 'pt.entity_id = n.nid');
    $query->fields('pt', array('field_unloading_status_value'));
    $query->join('field_data_field_egir_date', 'd', 'd.entity_id = n.nid');
    $query->fields('d', array('field_egir_date_value'));
    $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

    $output1[0] = array('PriceList Group', 'Not Started', 'In Progress', 'Completed');
    $output['not_start'] = $output['in_prg'] = $output['comp'] = 0;
    if (!empty($res)) {
      foreach ($res as $key => $val) {
        $loading_date = strtotime($val['field_egir_date_value']);

        if ($loading_date >= $start_date && $loading_date <= $end_date) {

          if (empty($val['field_unloading_status_value'])) {
            $output['not_start'] += 1;
          }
          else if ($val['field_unloading_status_value'] == 1) {
            $output['in_prg'] += 1;
          }
          else {
            $output['comp'] += 1;
          }
        }
      }
      $output1[] = array('Unloading', $output['not_start'], $output['in_prg'], $output['comp']);
    }
    else {
      $output1[] = array('No Result Found for Unloading', 0, 0, 0);
    }
    return $output1;
  }
}
