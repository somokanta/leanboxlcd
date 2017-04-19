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
    $form['start_date'] = array(
      '#type' => 'date_popup',
      '#title' => t('Billing Date From'),
      '#required' => TRUE,
      '#date_format' => 'Y-m-d',
      '#default_value' => $bill_date,
    );
    $form['end_date'] = array(
      '#type' => 'date_popup',
      '#title' => t('Billing Date To'),
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
function leanbox_dashboard_chart_filter_partypack_productivity_form($form, &$form_state) {
  $form = array();
  if (arg(1) == 'party_packing_productivity') {
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
      '#prefix' => '<div id ="party-packing-productivity-submit">',
      '#suffix' => '</div>',
      '#attributes' => array('class' => array('party-packing-productivity-submit')),
    );
  }

  $form['party_packing_productivity'] = array(
    '#markup' => '<a href = "/leanbox-chart-details/party_packing_productivity" target = "_blank"><div id="party_packing_productivity_div"></div></a>',
  );


  return $form;
}

/**
 * 
 * @param array $form
 * @param array $form_state
 * @return string
 */
function leanbox_dashboard_chart_filter_picking_productivity_form($form, &$form_state) {
  $form = array();
  if (arg(1) == 'picking_productivity') {
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
      '#prefix' => '<div id ="picking-productivity-submit">',
      '#suffix' => '</div>',
      '#attributes' => array('class' => array('picking-productivity-submit')),
    );
  }

  $form['picking_productivity'] = array(
    '#markup' => '<a href = "/leanbox-chart-details/picking_productivity" target = "_blank"><div id="picking_productivity_div"></div></a>',
  );


  return $form;
}

/**
 * 
 * @param array $form
 * @param array $form_state
 * @return string
 */
function leanbox_dashboard_chart_filter_billsperperson_form($form, &$form_state) {
  $form = array();
  if (arg(1) == 'bills_per_person') {
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
      '#prefix' => '<div id ="bills-person-submit">',
      '#suffix' => '</div>',
      '#attributes' => array('class' => array('bills-person-submit')),
    );
  }

  $form['bills_per_person'] = array(
    '#markup' => '<a href = "/leanbox-chart-details/bills_per_person" target = "_blank"><div id="bills_per_person_div"></div></a>',
  );


  return $form;
}

/**
 * 
 * @param array $form
 * @param array $form_state
 * @return string
 */
function leanbox_dashboard_chart_filter_billspervan_form($form, &$form_state) {
  $form = array();
  if (arg(1) == 'bills_per_van') {
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
      '#prefix' => '<div id ="bills-van-submit">',
      '#suffix' => '</div>',
      '#attributes' => array('class' => array('bills-van-submit')),
    );
  }

  $form['bills_per_van'] = array(
    '#markup' => '<a href = "/leanbox-chart-details/bills_per_van" target = "_blank"><div id="bills_per_van_div"></div></a>',
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
 * 
 * @param array $form
 * @param array $form_state
 * @return string
 */
function leanbox_dashboard_chart_filter_delivery_form($form, &$form_state) {
  $form = array();
  if (arg(1) == 'delivery') {
    $billing_date = strtotime(date('Y-m-d') . "- 7 days");
    $billing_date = date('Y-m-d', $billing_date);
    $form['start_date'] = array(
      '#type' => 'date_popup',
      '#title' => t('Bill Date From'),
      '#required' => TRUE,
      '#date_format' => 'Y-m-d',
      '#default_value' => $billing_date,
    );
    $form['end_date'] = array(
      '#type' => 'date_popup',
      '#title' => t('Bill Date To'),
      '#required' => TRUE,
      '#date_format' => 'Y-m-d',
      '#default_value' => $billing_date,
    );
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => 'Submit',
      '#prefix' => '<div id ="delivery-submit">',
      '#suffix' => '</div>',
      '#attributes' => array('class' => array('delivery-submit')),
    );
  }


  $form['unloading'] = array(
    '#markup' => '<a href = "/leanbox-chart-details/delivery"><div id="delivery_div"></div></a>',
  );

  return $form;
}

/**
 * Callback function for block party_packing
 * 
 * @return html
 */
function leanbox_dashboard_chart_query_details($activity_type, $start = '', $end = '') {
  global $user;
// Store distributor id in author in each node
  $distributor_id = change_feeds_import_get_distributor_id($user->uid);
  if (!empty($start)) {
    $start_date = strtotime($start);
  }
  else {
    $start = date('Y-m-d'); // dnt change format as date are stored in same format in db
    $time = strtotime(date('Y-m-d'));
    $start_date = strtotime(date("Y-m-d", $time));
  }
  if (!empty($end)) {
    $end_date = strtotime($end) + 86399;
  }
  else {
    $end = date('Y-m-d');
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
  else if ($activity_type == 'party_packing_productivity') {

    $party_packing = get_productivity_data_for_party_packing($start, $end, $distributor_id);
    $data1 = floor($party_packing['total']['pp_bill_lines'] / $party_packing['total']['pp_time']);
    if (is_nan($data1) || empty($data1) || $data1 == FALSE) {
      $data1 = 0;
    }
    $data2 = floor($party_packing['total']['non_pp_bill_lines'] / $party_packing['total']['non_time']);
    if (is_nan($data2) || empty($data2) || $data2 == FALSE) {
      $data2 = 0;
    }
    $output[] = array('Group', 'Productivity');
    $output[] = array('PP', $data1);
    $output[] = array('NON-PP', $data2);
    return $output;
  }
  else if ($activity_type == 'picking_productivity') {

    $output[] = array('Group', 'Productivity');
    $picking_data = get_productivity_data_for_picking($start, $end, $distributor_id);

    if (!empty($picking_data)) {
      foreach ($picking_data['godown'] as $key => $val) {
        foreach ($val as $key1 => $val1) {
          $data = floor($val1['loading_sheet_line'] / $val1['total_tme']);
          if (is_nan($data) || empty($data) || $data == FALSE) {
            $data = 0;
          }

          $output[] = array($key . ' ' . $key1, $data);
        }
      }
    }
    else {
      $output[] = array('No data found', 0);
    }
    return $output;
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


// code for delivery
  else if ($activity_type == 'delivery') {

    $query = db_select('taxonomy_term_data', 't');
    $query->join('taxonomy_vocabulary', 'v', 'v.vid = t.vid');
    $query->fields('t', array('tid', 'name'));
    $query->condition('v.name', 'Delivery Status');
    $res1 = $query->execute()->fetchAllKeyed(0, 1);


    $query = db_select('node', 'n');
    $query->condition('n.type', 'sales_register_data');
    $query->condition('n.uid', $distributor_id);
    $query->leftjoin('field_data_field_delivery_status', 's', 's.entity_id = n.nid');
    $query->fields('s', array('field_delivery_status_tid'));
    $query->join('field_data_field_sr_bill_date', 'i', 'i.entity_id = n.nid');
    if (!empty($start_date) && !empty($end_date)) {
      $query->condition('i.field_sr_bill_date_value', array($start_date, $end_date), 'BETWEEN');
    }
    $query->join('field_data_field_bill_value', 'b', 'b.entity_id = n.nid');
    $query->leftjoin('field_data_field_sales_return_amt', 'r', 'r.entity_id = n.nid');
    $query->fields('b', array('field_bill_value_value'));
    $query->fields('r', array('field_sales_return_amt_value'));
    $res2 = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

    $output[0] = array('Sales Delivery', 'Return %');
    $total_bill_value = 0;

    foreach ($res2 as $key2 => $val2) {
      // calculate the sum of bill value for all bills for given day
      $total_bill_value += $val2['field_bill_value_value'];
    }
    if (!empty($res2)) {
      foreach ($res1 as $key1 => $val1) {
        foreach ($res2 as $key2 => $val2) {
          if (!isset($data[$val1]['data'])) {
            $data[$val1]['data'] = 0;
          }

          if ($key1 == $val2['field_delivery_status_tid']) {
            $data[$val1]['data'] += $val2['field_sales_return_amt_value'];
          }
        }
      }

      if (!empty($total_bill_value)) {
        foreach ($data as $key => $val) {
          if ($key == 'OK') {
            continue;
          }
          foreach ($val as $key1 => $val1) {
            $data = ($val1 / $total_bill_value) * 100;
            $output[] = array($key, round($data, 2));
          }
        }
      }
      else {
        $output[] = array('No Result Found for Return', 0);
      }
    }
    else {
      $output[] = array('No Result Found for Return', 0);
    }

    return $output;
  }
  else if ($activity_type == 'bills_per_person') {
    $query = db_select('node', 'n');
    $query->condition('n.type', 'dispatch_data');
    $query->addTag('node_uid');
    $query->join('field_data_field_dispatch_supervisor', 'su', 'su.entity_id = n.nid');
    $query->join('field_data_field_bill_count', 'v', 'v.entity_id = n.nid');
    $query->join('field_data_field_dispatch_date', 'd', 'd.entity_id = n.nid');
    $query->join('field_data_field_dispatch_loader_team_count', 'tc', 'tc.entity_id = n.nid');
    $query->fields('d', array('field_dispatch_date_value'));
    $query->fields('su', array('field_dispatch_supervisor_value'));
    $query->addExpression('SUM(tc.field_dispatch_loader_team_count_value)', 'Team_count');
    $query->addExpression('SUM(v.field_bill_count_value)', 'bill_value');
    $query->groupBy('su.field_dispatch_supervisor_value');

    $query->where("(DATE_FORMAT(d.field_dispatch_date_value, '%Y-%m-%d') >= :start)", array(':start' => $start));
    $query->where("(DATE_FORMAT(d.field_dispatch_date_value, '%Y-%m-%d') <= :to)", array(':to' => $end));

    $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
    $data1[] = array('PriceList Group', 'COUNT');

    if (!empty($res)) {
      foreach ($res as $key => $val) {

        $supervisor = $val['field_dispatch_supervisor_value'];
        $team_count = $val['Team_count'];
        $bill_value = $val['bill_value'];
        $count_per_person = ($bill_value / $team_count);
        $data1[] = array($supervisor, $count_per_person);
      }
    }
    else {
      $data1[] = array('No Result Found for Dispatch', 0);
    }
    return $data1;
  }
  else if ($activity_type == 'bills_per_van') {
    $query = db_select('node', 'n');
    $query->condition('n.type', 'dispatch_data');
    $query->addTag('node_uid');
    $query->join('field_data_field_dispatch_supervisor', 'su', 'su.entity_id = n.nid');
    $query->join('field_data_field_bill_count', 'v', 'v.entity_id = n.nid');
    $query->join('field_data_field_dispatch_date', 'd', 'd.entity_id = n.nid');
    $query->join('field_data_field_dispatch_loader_team_count', 'tc', 'tc.entity_id = n.nid');
    //$query->fields('d', array('field_dispatch_date_value'));
    $query->fields('su', array('field_dispatch_supervisor_value'));

    $query->where("(DATE_FORMAT(d.field_dispatch_date_value, '%Y-%m-%d') >= :start)", array(':start' => $start));
    $query->where("(DATE_FORMAT(d.field_dispatch_date_value, '%Y-%m-%d') <= :to)", array(':to' => $end));

    $query->addExpression('COUNT(n.title)', 'trip_count');
    $query->addExpression('SUM(v.field_bill_count_value)', 'bill_count_value');
    $query->groupBy('su.field_dispatch_supervisor_value');
    $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

    $data1[] = array('PriceList Group', 'COUNT');

    if (!empty($res)) {
      foreach ($res as $key => $val) {
        $supervisor = $val['field_dispatch_supervisor_value'];
        $trip_count = $val['trip_count'];
        $bill_count_value = $val['bill_count_value'];
        $count_per_van = ($bill_count_value / $trip_count);
        $data1[] = array($supervisor, $count_per_van);
      }
    }
    else {
      $data1[] = array('No Result Found for Dispatch', 0);
    }
    return $data1;
  }
}

function leanbox_dashboard_gauge_chart_query_details($type, $start_date = '', $end_date = '') {
  require_once drupal_get_path('module', 'change_feeds_import') . '/form/upload_billing_data.inc';

  $output1 = array();
  $output = 0;
  global $user;
  // Store distributor id in author in each node
  $uid = change_feeds_import_get_distributor_id($user->uid);
  if (!empty($start_date)) {
    $start_date = strtotime($start_date);
  }
  else {
    $start_date = leanbox_dashboard_get_default_pdd_date_for_chart();
  }
  if (!empty($end_date)) {
    $end_date = strtotime($end_date) + 86399;
  }
  else {
    $end_date = leanbox_dashboard_get_default_pdd_date_for_chart();
  }

// Query to calculate total bill value
  $query = db_select('node', 'n');
  $query->join('field_data_field_bill_value', 'v', 'v.entity_id = n.nid');
  $query->join('field_data_field_sr_bill_date', 'b', 'b.entity_id = n.nid');
  $query->addExpression('SUM(v.field_bill_value_value)', 'bill_value');
  $query->condition('n.type', 'sales_register_data');
  if (!empty($start_date) && !empty($end_date)) {
    $query->condition('b.field_sr_bill_date_value', array($start_date, $end_date), 'BETWEEN');
  }
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
  if (!empty($start_date) && !empty($end_date)) {
    $query->condition('b.field_sr_bill_date_value', array($start_date, $end_date), 'BETWEEN');
  }
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
  }

  if (arg(1) == 'otif') {
    $todays_date_date = date('Y-m-d');
    //$todays_date_date = "2017-03-20";
    // $todays_date_date = "2017-03-17 00:00:00";
    $todays_date_date_timestamp = strtotime(date($todays_date_date));
    $current_day = date("d", $todays_date_date_timestamp);
    $yesterday = strtotime('-1 day', strtotime($todays_date_date));
    //$day_before_yesterday = strtotime('-2 day', strtotime($todays_date_date));
    if ($current_day == 22) {
      $this_month = date("m", $todays_date_date_timestamp);
      $this_date = $yesterday;
    }
    elseif ($current_day <= 21) {
      $old_date = strtotime('-1 month', strtotime($todays_date_date));
      $last_month = date("m", $old_date);
      $this_date = strtotime(date("Y-$last_month-21"));
    }
    else {
      $this_month = date("m", $todays_date_date_timestamp);
      $this_date = strtotime(date("Y-$this_month-21"));
    }

    $yesterday_html = "<h3>Yesterday</h3>";
    $yesterday_data = get_yesterday_data_channel($yesterday, $yesterday, $uid);
    if ($yesterday_data == "No Result Found") {
      $header = get_header_of_chhanel();
      $yesterday_rows[] = array("<td colspan='10' style='text-align: center;'> There is No Activity</td>");
      $yesterday_html .= theme('table', array('header' => $header, 'rows' => $yesterday_rows, 'attributes' => array('border' => '1', 'cellpadding' => '5', 'cellspacing' => '0')));
    }
    else {
      $yesterday_html .= $yesterday_data;
    }

    $mtd_html = "<h3>MTD</h3>";
    $mtd_data .= get_mtd_data_channel($this_date, $yesterday, $uid);
    if ($mtd_data == "No Result Found") {
      $header = get_header_of_chhanel();
      $mtd_rows[] = array("<td colspan='10' style='text-align: center;'> There is No Activity</td>");
      $mtd_html .= theme('table', array('header' => $header, 'rows' => $mtd_rows, 'attributes' => array('border' => '1', 'cellpadding' => '5', 'cellspacing' => '0')));
    }
    else {
      $mtd_html .= $mtd_data;
    }

    $table = $yesterday_html . "<br>" . $mtd_html;
    $output1['table'] = $table;
  }

  $output1['output'] = $output;

  return $output1;
}

/**
 * logic to calculate bill date
 * @param string $pdd
 * @return time in string
 */
function leanbox_dashboard_get_default_pdd_date_for_chart($pdd = "1 days") {
  require_once drupal_get_path('module', 'change_feeds_import') . '/form/upload_billing_data.inc';

  $uid = $GLOBALS['user']->uid;
  $distributor_id = change_feeds_import_get_distributor_id($uid);
  $nid = check_node_title_exists($distributor_id, 'pdd_master');

  if (!empty($nid)) {
    $node = node_load($nid);
    $pdd = $node->field_pdd[LANGUAGE_NONE][0]['value'] . " days";
  }
  $bill_date = date('Y-m-d');
  $bill_date = strtotime($bill_date . "-$pdd");
  return $bill_date;
}
