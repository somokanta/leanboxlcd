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
function leanbox_dashboard_chart_filter_otif_form($form, &$form_state) {
  $form = array();

  if (arg(1) == 'otif') {
    $bill_date = leanbox_dashboard_get_default_pdd_date_for_chart();
    $bill_date = date('Y-m-d', $bill_date);
    $form['bill_date'] = array(
      '#type' => 'date_popup',
      '#title' => t('Billing Date From'),
      '#required' => TRUE,
      '#date_format' => 'Y-m-d',
      '#default_value' => $bill_date,
    );

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => 'Submit',
      '#prefix' => '<div id ="otif-submit">',
      '#suffix' => '</div>',
      '#attributes' => array('class' => array('otif-submit')),
    );
  }

  $form['otif'] = array(
    '#markup' => '<a href = "/leanbox-chart-details/otif" target = "_blank"><div id="otif_div"></div></a>',
  );

  return $form;
}

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
    // $time = strtotime('-60 days');
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

    $data1[] = array('PriceList Group', 'In Progress', 'Not Started', 'Completed');
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

      $party_pack_flag = FALSE;
      foreach ($output as $key => $val) {
        if ($val['not_start'] == 0 && $val['in_prg'] == 0 && $val['comp'] == 0) {
          continue;
        }
        $party_pack_flag = TRUE;
        $data1[] = array($key, $val['in_prg'], $val['not_start'], $val['comp']);
      }
      if ($party_pack_flag == FALSE) {
        $data1[] = array('No Result Found for Party Packing Type', 0, 0, 0);
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

    $data1[] = array('PriceList Group', 'In Progress', 'Not Started', 'Completed');
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

              $type = (strtolower($val['field_picking_picking_type_value']));
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
      $picking_flag = FALSE;
      foreach ($output as $key => $val) {
        foreach ($val as $key1 => $val1) {
          if ($val1['not_start'] == 0 && $val1['in_prg'] == 0 && $val1['comp'] == 0) {
            continue;
          }
          $picking_flag = TRUE;
          $data1[] = array($key . ' ' . $key1, $val1['in_prg'], $val1['not_start'], $val1['comp']);
        }
      }
      if ($picking_flag == FALSE) {
        $data1[] = array('No Result Found for Pricelist Group', 0, 0, 0);
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

    $output1[0] = array('PriceList Group', 'In Progress', 'Not Started', 'Completed');
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
      if ($output['not_start'] == 0 && $output['in_prg'] == 0 && $output['comp'] == 0) {
        $output1[] = array('No Result Found for Unloading', 0, 0, 0);
      }
      else {
        $output1[] = array('Unloading', $output['in_prg'], $output['not_start'], $output['comp']);
      }
    }
    else {
      $output1[] = array('No Result Found for Unloading', 0, 0, 0);
    }
    return $output1;
  }
}

function leanbox_dashboard_gauge_chart_query_details($type, $bill_date = '') {
  require_once drupal_get_path('module', 'change_feeds_import') . '/form/upload_billing_data.inc';
  
  $output = 0;
  $uid = $GLOBALS['user']->uid;
  $uid = change_feeds_import_get_distributor_id($uid);
  if ($type == 'otif') {

    if (!empty($bill_date)) {
      $bill_date = strtotime($bill_date);
    }
    else {
      $bill_date = leanbox_dashboard_get_default_pdd_date_for_chart();
    }

    // Query to calculate total bill value
    $query = db_select('node', 'n');
    $query->join('field_data_field_bill_value', 'v', 'v.entity_id = n.nid');
    $query->join('field_data_field_sr_bill_date', 'b', 'b.entity_id = n.nid');
    $query->addExpression('SUM(v.field_bill_value_value)', 'bill_value');
    $query->condition('n.type', 'sales_register_data');
    $query->condition('b.field_sr_bill_date_value', $bill_date);
    $query->condition('n.uid', $uid);
    $total = $query->execute()->fetch(PDO::FETCH_ASSOC);

    // query to calculate total dispatched bill value

    $query = db_select('node', 'n');
    $query->join('field_data_field_bill_value', 'v', 'v.entity_id = n.nid');
    $query->fields('v', array('field_bill_value_value'));
    $query->join('field_data_field_sr_bill_date', 'b', 'b.entity_id = n.nid');
    $query->join('field_data_field_promised_date', 'pd', 'pd.entity_id = n.nid');
    $query->fields('pd', array('field_promised_date_value'));
    $query->join('field_data_field_trip_id_reference', 't', 't.entity_id = n.nid');
    $query->join('node', 'n1', 't.field_trip_id_reference_nid = n1.nid');
    $query->join('field_data_field_dispatch_date', 'd1', 'd1.entity_id= n1.nid');
    $query->fields('d1', array('field_dispatch_date_value'));
    $query->condition('n.uid', $uid);
    $query->condition('n1.uid', $uid);
    $query->condition('n.type', 'sales_register_data');
    $query->condition('b.field_sr_bill_date_value', $bill_date);
    $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
    
    if (!empty($res)) {
      $total_dis_bill_val = 0;
      foreach ($res as $key => $val) {
        $dispatch_date = strtotime($val['field_dispatch_date_value']);
        $pdd = $val['field_promised_date_value'];

        if (!empty($dispatch_date) && !empty($pdd)) {
          $date_diff = $pdd - $dispatch_date;
          if ($date_diff >= 0) {
            $total_dis_bill_val += $val['field_bill_value_value'];
          }
        }
      }

      $output = round(($total_dis_bill_val / $total['bill_value'] ) * 100);
      return $output;
    }
  }
  return $output;
}

/**
 * logic to calculate bill date
 * @param string $pdd
 * @return time in string
 */
function leanbox_dashboard_get_default_pdd_date_for_chart($pdd = "4 days") {
  require_once drupal_get_path('module', 'change_feeds_import') . '/form/upload_billing_data.inc';

  $uid = $GLOBALS['user']->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);
  $nid = check_node_title_exists($distributor_id, 'pdd_master');

  if (!empty($nid)) {
    $node = node_load($nid);
    $pdd = $node->field_pdd[LANGUAGE_NONE][0]['value'] . " days";
  }
  $bill_date = date('Y-m-d');
  $bill_date = strtotime($bill_date . "-$pdd");
  return $bill_date;
}
