<?php

/**
 *  This file Includes callback function
 *  defined in for custom block
 */


/**
 * 
 * @param array $form
 * @param array $form_state
 * @return array
 */
function leanbox_dashboard_chart_filter_bill_pendency_form($form, &$form_state) {
  $form = array();
  if (arg(1) == 'bill_pendency') {
    $form['start_date'] = array(
      '#type' => 'date_popup',
      '#title' => t('Billing Date From'),
      '#required' => TRUE,
      '#date_format' => 'Y-m-d',
      '#default_value' => date('Y-m-d', strtotime("-1 days")),
    );
    $form['end_date'] = array(
      '#type' => 'date_popup',
      '#title' => t('Billing Date To'),
      '#required' => TRUE,
      '#date_format' => 'Y-m-d',
      '#default_value' => date('Y-m-d', strtotime("-1 days")),
    );

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => 'Submit',
      '#prefix' => '<div id ="bill-pendency-submit">',
      '#suffix' => '</div>',
      '#attributes' => array('class' => array('bill-pendency-submit')),
    );
  }

  $form['bill_pendency'] = array(
    '#markup' => '<a href = "/leanbox-chart-details/bill_pendency" target = "_blank"><div id="bill_pendency_div"></div></a>',
  );


  return $form;
}

/**
 * 
 * @param array $form
 * @param array $form_state
 * @return array
 */
function leanbox_dashboard_chart_filter_dispatch_ota_form($form, &$form_state) {
  $form = array();
  if (arg(1) == 'dispatch_ota') {
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
      '#prefix' => '<div id ="dispatch-ota-submit">',
      '#suffix' => '</div>',
      '#attributes' => array('class' => array('dispatch-ota-submit')),
    );
  }

  $form['dispatch_ota'] = array(
    '#markup' => '<a href = "/leanbox-chart-details/dispatch_ota" target = "_blank"><div id="dispatch_ota_div"></div></a>',
  );


  return $form;
}

/**
 * 
 * @param array $form
 * @param array $form_state
 * @return array
 */
function leanbox_dashboard_chart_filter_unloading_ota_form($form, &$form_state) {
  $form = array();
  if (arg(1) == 'unloading_ota') {
    $form['start_date'] = array(
      '#type' => 'date_popup',
      '#title' => t('Egir Date From'),
      '#required' => TRUE,
      '#date_format' => 'Y-m-d',
      '#default_value' => date('Y-m-d'),
    );
    $form['end_date'] = array(
      '#type' => 'date_popup',
      '#title' => t('Egir Date To'),
      '#required' => TRUE,
      '#date_format' => 'Y-m-d',
      '#default_value' => date('Y-m-d'),
    );
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => 'Submit',
      '#prefix' => '<div id ="unloading-ota-submit">',
      '#suffix' => '</div>',
      '#attributes' => array('class' => array('unloading-ota-submit')),
    );
  }

  $form['unloading_ota'] = array(
    '#markup' => '<a href = "/leanbox-chart-details/unloading_ota" target = "_blank"><div id="unloading_ota_div"></div></a>',
  );


  return $form;
}

/**
 * 
 * @param array $form
 * @param array $form_state
 * @return array
 */
function leanbox_dashboard_chart_filter_pp_ota_form($form, &$form_state) {
  $form = array();
  if (arg(1) == 'pp_ota') {
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
      '#prefix' => '<div id ="pp-ota-submit">',
      '#suffix' => '</div>',
      '#attributes' => array('class' => array('pp-ota-submit')),
    );
  }

  $form['pp_ota'] = array(
    '#markup' => '<a href = "/leanbox-chart-details/pp_ota" target = "_blank"><div id="pp_ota_div"></div></a>',
  );


  return $form;
}

/**
 * 
 * @param array $form
 * @param array $form_state
 * @return array
 */
function leanbox_dashboard_chart_filter_picking_ota_form($form, &$form_state) {
  $form = array();
  if (arg(1) == 'picking_ota') {
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
      '#prefix' => '<div id ="picking-ota-submit">',
      '#suffix' => '</div>',
      '#attributes' => array('class' => array('picking-ota-submit')),
    );
  }

  $form['picking_ota'] = array(
    '#markup' => '<a href = "/leanbox-chart-details/picking_ota" target = "_blank"><div id="picking_ota_div"></div></a>',
  );


  return $form;
}

/**
 * 
 * @param array $form
 * @param array $form_state
 * @return array
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
 * @return array
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
 * @return array
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
 * @return array
 */
function leanbox_dashboard_chart_filter_billsperperson_form($form, &$form_state) {
  $form = array();
  if (arg(1) == 'bills_per_person') {
    $form['start_date'] = array(
      '#type' => 'date_popup',
      '#title' => t('Dispatch Date From'),
      '#required' => TRUE,
      '#date_format' => 'Y-m-d',
      '#default_value' => date('Y-m-d'),
    );
    $form['end_date'] = array(
      '#type' => 'date_popup',
      '#title' => t('Dispatch Date To'),
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
 * @return array
 */
function leanbox_dashboard_chart_filter_billspervan_form($form, &$form_state) {
  $form = array();
  if (arg(1) == 'bills_per_van') {
    $form['start_date'] = array(
      '#type' => 'date_popup',
      '#title' => t('Dispatch Date From'),
      '#required' => TRUE,
      '#date_format' => 'Y-m-d',
      '#default_value' => date('Y-m-d'),
    );
    $form['end_date'] = array(
      '#type' => 'date_popup',
      '#title' => t('Dispatch Date To'),
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
 * @return array
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
 * @return array
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
 * @return array
 */
function leanbox_dashboard_chart_filter_picking_bulk_form($form, &$form_state) {
  $form = array();

  if (arg(1) == 'picking_bulk') {
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
      '#prefix' => '<div id ="picking-bulk-submit">',
      '#suffix' => '</div>',
      '#attributes' => array('class' => array('picking-bulk-submit')),
    );
  }

  $form['picking_bulk'] = array(
    '#markup' => '<a href = "/leanbox-chart-details/picking_bulk" target = "_blank"><div id="picking_bulk_div"></div></a>',
  );

  return $form;
}

/**
 * 
 * @param array $form
 * @param array $form_state
 * @return array
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
 * @return array
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
function leanbox_dashboard_chart_query_details($activity_type, $start = '', $end = '', $ajax_type = '') {
  global $user;
// Store distributor id in author in each node
  $distributor_id = change_feeds_import_get_distributor_id($user->uid);
  $distributor_name = change_feeds_import_get_distributor_name($distributor_id);
  if (!empty($start)) {
    $start_date = strtotime($start);
  }
  else {
    $start = date('Y-m-d'); // dnt change format as date are stored in same format in db
    $start_date = strtotime(date("Y-m-d"));
  }
  if (!empty($end)) {
    $end_date = strtotime($end) + 86399;
  }
  else {
    $end = date('Y-m-d');
    $end_date = strtotime(date('Y-m-d')) + 86399;
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

    $pp_table = '';
    $party_packing = get_productivity_data_for_party_packing($start, $end, $distributor_id);
    $output[] = array('Group', 'Productivity', 'Target');

    if (!empty($party_packing)) {
      $data1 = floor($party_packing['total']['pp_bill_lines'] / $party_packing['total']['pp_time']);
      if (is_nan($data1) || empty($data1) || $data1 == FALSE) {
        $data1 = 0;
      }
      $data2 = floor($party_packing['total']['non_pp_bill_lines'] / $party_packing['total']['non_time']);
      if (is_nan($data2) || empty($data2) || $data2 == FALSE) {
        $data2 = 0;
      }
      $target_data = leanbox_dashboard_target_value_using_voc_name('productivity target');
      $output[] = array('PP', $data1, (int) $target_data['PP']);
      $output[] = array('NON-PP', $data2, (int) $target_data['NON-PP']);
    }
    else {
      $output[] = array('No data found', 0, 0);
    }

    if (arg(1) == 'party_packing_productivity' || $ajax_type == 'party_packing_ajax') {
      $pp_table = send_email_productivity_emailer($start, $distributor_name, $end, TRUE, 'party_packing');
    }

    $output1['output'] = $output;
    $output1['table'] = $pp_table;
    return $output1;
  }
  else if ($activity_type == 'picking_productivity') {

    $pp_table = '';
    $output[] = array('Group', 'Productivity', 'Target');
    $picking_data = get_productivity_data_for_picking($start, $end, $distributor_id);

    if (!empty($picking_data)) {
      $target_data = leanbox_dashboard_target_value_using_voc_name('productivity target');
      foreach ($picking_data['godown'] as $key => $val) {
        foreach ($val as $key1 => $val1) {
          $data = floor($val1['loading_sheet_line'] / $val1['total_tme']);
          if (is_nan($data) || empty($data) || $data == FALSE) {
            $data = 0;
          }
          $label = $key . ' ' . $key1;
          $label_key = strtolower($label);
          $output[] = array($label, $data, (int) $target_data[$label_key]);
        }
      }
    }
    else {
      $output[] = array('No data found', 0, 0);
    }

    if (arg(1) == 'picking_productivity' || $ajax_type == 'picking_ajax') {
      $pp_table = send_email_productivity_emailer($start, $distributor_name, $end, TRUE, 'picking');
    }

    $output1['output'] = $output;
    $output1['table'] = $pp_table;
    return $output1;
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

          if ($key1 == 'loose') {
            $picking_flag = TRUE;

            $data1[] = array($key . ' ' . $key1, $val1['in_prg'], $val1['not_start'], $val1['comp']);
          }
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
  else if ($activity_type == 'picking_bulk') {


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

          if ($key1 == 'bulk') {
            $picking_flag = TRUE;

            $data1[] = array($key . ' ' . $key1, $val1['in_prg'], $val1['not_start'], $val1['comp']);
          }
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

    $query = db_select('node', 'n');
    $query->condition('n.type', 'sales_register_data');
    $query->condition('n.uid', $distributor_id);
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
      foreach ($res2 as $key2 => $val2) {
        if ($val2['field_sales_return_amt_value'] != NULL) {

        $sales_ret = (( $val2['field_bill_value_value'] - $val2['field_sales_return_amt_value']) / $val2['field_bill_value_value']) * 100;
        if ($sales_ret >= 99) {
          $data['Full Return']['data'] += $val2['field_bill_value_value'] - $val2['field_sales_return_amt_value'];
        }
        else if ($sales_ret > 1 && $sales_ret < 99) {
          $data['Partial Delivery']['data'] += $val2['field_bill_value_value'] - $val2['field_sales_return_amt_value'];
        }
        else {
          continue;
        }
        }
      }

      if (!empty($total_bill_value) && !empty($data)) {
        foreach ($data as $key => $val) {
          foreach ($val as $key1 => $val1) {
            $data1 = (($val1) / $total_bill_value) * 100;
            $output[] = array($key, round($data1, 2));
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
        $total_bill_value += $bill_value;
        $total_team_count += $team_count;
        $count_per_person = round($bill_value / $team_count);
        $data1[] = array($supervisor, $count_per_person);
      }
      $data1[] = array('Total', round($total_bill_value / $total_team_count));
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
        $count_per_van = round($bill_count_value / $trip_count);
        $total_bill_value += $bill_count_value;
        $total_trip_count += $trip_count;
        $data1[] = array($supervisor, $count_per_van);
      }
      $data1[] = array('Total', round($total_bill_value / $total_trip_count));
    }
    else {
      $data1[] = array('No Result Found for Dispatch', 0);
    }
    return $data1;
  }
  else if ($activity_type == 'pp_ota') {
    $query = db_select('node', 'n');
    $query->addTag('node_uid');
    $query->condition('n.type', 'party_packing');
    $query->join('field_data_field_party_packing_loading_date', 'l', 'l.entity_id = n.nid');
    $query->leftjoin('field_data_field_party_packing_check', 'pc', 'pc.entity_id = n.nid');
    $query->condition('pc.field_party_packing_check_value', 1); // 1 => YES
    $query->leftjoin('field_data_field_party_packing_date', 'pcd', 'pcd.entity_id = n.nid');
    $query->fields('l', array('field_party_packing_loading_date_value'));
    $query->fields('pcd', array('field_party_packing_date_value'));
    $query->leftjoin('field_data_field__party_packingstatus', 'ps', 'ps.entity_id = n.nid');
    $query->leftjoin('field_data_field_party_packing_end_time', 'ppt', 'ppt.entity_id = n.nid');
    $query->fields('ppt', array('field_party_packing_end_time_value'));
    $query->fields('ps', array('field__party_packingstatus_value'));

    $query->where("(DATE_FORMAT(l.field_party_packing_loading_date_value, '%Y-%m-%d') >= :start)", array(':start' => $start));
    $query->where("(DATE_FORMAT(l.field_party_packing_loading_date_value, '%Y-%m-%d') <= :to)", array(':to' => $end));
    $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
    $data[] = array('PriceList Group', 'Fail', 'Success');

    $query = db_select('node', 'n');
    $query->addTag('node_uid');
    $query->condition('n.type', 'van_stop_master');
    $query->join('field_data_field_pp_finish_time', 'pp', 'pp.entity_id = n.nid');
    $query->fields('pp', array('field_pp_finish_time_value'));
    $dis_time = $query->execute()->fetchField();

    if (!empty($dis_time)) {
      if (!empty($res)) {

        $total_cont = count($res);
        $dis_on_time = strtotime(date("H:i", strtotime($dis_time)));
        $success_count = 0;
        foreach ($res as $key => $val) {
          $pp_on_time = strtotime(date("H:i", strtotime($val['field_party_packing_end_time_value'])));

          if (($val['field_party_packing_loading_date_value'] == $val['field_party_packing_date_value']) && ($val['field__party_packingstatus_value'] == 2)) {
            if ($pp_on_time <= $dis_on_time) {
              $success_count ++;
            }
          }
        }

        $fail_count = $total_cont - $success_count;
        $data[] = array('Result', $fail_count, $success_count);
      }
      else {
        $data[] = array('No Result Found', 0, 0);
      }
    }
    else {
      $data[] = array('No Finish Time Found', 0, 0);
    }
    return $data;
  }
  else if ($activity_type == 'picking_ota') {
    $query = db_select('node', 'n');
    $query->addTag('node_uid');
    $query->condition('n.type', 'picking');
    $query->join('field_data_field_picking_loading_date', 'l', 'l.entity_id = n.nid');
    $query->leftjoin('field_data_field_picking_picking_date', 'pcd', 'pcd.entity_id = n.nid');
    $query->leftjoin('field_data_field_picking_status', 'ps', 'ps.entity_id = n.nid');

    $query->fields('l', array('field_picking_loading_date_value'));
    $query->fields('pcd', array('field_picking_picking_date_value'));

    $query->leftjoin('field_data_field_picking_picking_type', 'pt', 'pt.entity_id = n.nid');
    $query->leftjoin('field_data_field_picking_van_id', 'v', 'v.entity_id = n.nid');
    $query->leftjoin('field_data_field_picking_end_time', 'e', 'e.entity_id = n.nid');

    $query->fields('ps', array('field_picking_status_value'));
    $query->fields('e', array('field_picking_end_time_value'));
    $query->fields('v', array('field_picking_van_id_value'));
    $query->addExpression('GROUP_CONCAT(IFNULL(e.field_picking_end_time_value, 0))', 'time');
    $query->groupBy('v.field_picking_van_id_value');
    $query->groupBy('l.field_picking_loading_date_value');
    $query->condition('pt.field_picking_picking_type_value', 'Loose');
    $query->where("(DATE_FORMAT(l.field_picking_loading_date_value, '%Y-%m-%d') >= :start)", array(':start' => $start));
    $query->where("(DATE_FORMAT(l.field_picking_loading_date_value, '%Y-%m-%d') <= :to)", array(':to' => $end));
    $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

    $data[] = array('PriceList Group', 'Fail', 'Success');

    $query = db_select('node', 'n');
    $query->addTag('node_uid');
    $query->condition('n.type', 'van_stop_master');
    $query->join('field_data_field_picking_finish_time', 'pp', 'pp.entity_id = n.nid');
    $query->fields('pp', array('field_picking_finish_time_value'));
    $dis_time = $query->execute()->fetchField();

    if (!empty($dis_time)) {
      if (!empty($res)) {

        $total_cont = count($res);
        $dis_on_time = strtotime(date("H:i", strtotime($dis_time)));
        $success_count = 0;
        foreach ($res as $key => $val) {
          if (($val['field_picking_loading_date_value'] == $val['field_picking_picking_date_value']) && ($val['field_picking_status_value'] == 2)) {
            $count = FALSE;
            $time = explode(',', $val['time']);

            foreach ($time as $key1 => $val1) {
              $pp_on_time = strtotime(date("H:i", strtotime($val1)));

              if (($pp_on_time <= $dis_on_time) && ($val1 != 0)) {
                $count = TRUE;
              }
              else {
                $count = FALSE;
                break;
              }
            }
            if ($count == TRUE) {
              $success_count ++;
            }
          }
        }
        $fail_count = $total_cont - $success_count;
        $data[] = array('result', $fail_count, $success_count);
      }
      else {
        $data[] = array('No Result Found', 0, 0);
      }
    }
    else {
      $data[] = array('No Finish Time Found', 0, 0);
    }

    return $data;
  }
  else if ($activity_type == 'unloading_ota') {
    $query = db_select('node', 'n');
    $query->addTag('node_uid');
    $query->condition('n.type', 'egir');
    $query->join('field_data_field_egir_date', 'l', 'l.entity_id = n.nid');
    $query->leftjoin('field_data_field_unloading_date', 'pcd', 'pcd.entity_id = n.nid');
    $query->fields('l', array('field_egir_date_value'));
    $query->fields('pcd', array('field_unloading_date_value'));

    $query->leftjoin('field_data_field_unloading_status', 'ps', 'ps.entity_id = n.nid');
    $query->leftjoin('field_data_field_unloading_end_time', 'ppt', 'ppt.entity_id = n.nid');
    $query->fields('ppt', array('field_unloading_end_time_value'));
    $query->fields('ps', array('field_unloading_status_value'));

    $query->where("(DATE_FORMAT(l.field_egir_date_value, '%Y-%m-%d') >= :start)", array(':start' => $start));
    $query->where("(DATE_FORMAT(l.field_egir_date_value, '%Y-%m-%d') <= :to)", array(':to' => $end));
    $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

    $data[] = array('PriceList Group', 'Fail', 'Success');

    $query = db_select('node', 'n');
    $query->addTag('node_uid');
    $query->condition('n.type', 'van_stop_master');
    $query->join('field_data_field_unloading_finish_time', 'pp', 'pp.entity_id = n.nid');
    $query->fields('pp', array('field_unloading_finish_time_value'));
    $dis_time = $query->execute()->fetchField();

    if (!empty($dis_time)) {
      if (!empty($res)) {

        $total_cont = count($res);
        $dis_on_time = strtotime(date("H:i", strtotime($dis_time)));
        $success_count = 0;
        foreach ($res as $key => $val) {
          $pp_on_time = strtotime(date("H:i", strtotime($val['field_unloading_end_time_value'])));

          if (($val['field_egir_date_value'] == $val['field_unloading_date_value']) && ($val['field_unloading_status_value'] == 2)) {
            if ($pp_on_time <= $dis_on_time) {
              $success_count ++;
            }
          }
        }

        $fail_count = $total_cont - $success_count;
        $data[] = array('result', $fail_count, $success_count);
      }
      else {
        $data[] = array('No Result Found', 0, 0);
      }
    }
    else {
      $data[] = array('No Finish Time Found', 0, 0);
    }
    return $data;
  }
  else if ($activity_type == 'dispatch_ota') {

// Query to calculate total bill value
    $query = db_select('node', 'n');
    $query->fields('n');
    $query->join('field_data_field_sr_bill_date', 'b', 'b.entity_id = n.nid');
    $query->join('field_data_field_promised_date', 'pro_date', 'pro_date.entity_id = n.nid');
    $query->join('field_data_field_billing_van', 'bva', 'bva.entity_id = n.nid');
    $query->condition('n.type', 'sales_register_data');
    if (!empty($start_date) && !empty($end_date)) {
      $query->condition('b.field_sr_bill_date_value', array($start_date, $end_date), 'BETWEEN');
    }
    $query->condition('n.uid', $distributor_id);
    $total_cont = $query->execute()->rowCount();

// query to calculate total dispatched bill value

    $query = db_select('node', 'n');
    $query->join('field_data_field_sr_bill_date', 'b', 'b.entity_id = n.nid');
    $query->join('field_data_field_promised_date', 'pd', 'pd.entity_id = n.nid');
    $query->fields('pd', array('field_promised_date_value'));
    $query->join('field_data_field_billing_van', 'bva', 'bva.entity_id = n.nid');
    $query->join('field_data_field_trip_id_reference', 't', 't.entity_id = n.nid');
    $query->join('node', 'n1', 't.field_trip_id_reference_nid = n1.nid');
    $query->join('field_data_field_dispatch_date', 'd1', 'd1.entity_id= n1.nid');
    $query->join('field_data_field_dispatch_start_time', 'st', 'st.entity_id = n1.nid');
    $query->fields('st', array('field_dispatch_start_time_value'));
    $query->fields('d1', array('field_dispatch_date_value'));
    $query->condition('n.uid', $distributor_id);
    $query->condition('n1.uid', $distributor_id);
    $query->condition('n.type', 'sales_register_data');
    if (!empty($start_date) && !empty($end_date)) {
      $query->condition('b.field_sr_bill_date_value', array($start_date, $end_date), 'BETWEEN');
    }
    $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);


    $data[] = array('PriceList Group', 'Fail', 'Success');

    $query = db_select('node', 'n');
    $query->addTag('node_uid');
    $query->condition('n.type', 'van_stop_master');
    $query->join('field_data_field_otif_finish_time', 'pp', 'pp.entity_id = n.nid');
    $query->fields('pp', array('field_otif_finish_time_value'));
    $dis_time = $query->execute()->fetchField();

    if (!empty($dis_time)) {
      if (!empty($res)) {

        $dis_on_time = strtotime(date("H:i", strtotime($dis_time)));
        $success_count = 0;
        foreach ($res as $key => $val) {
          $dispatch_date = strtotime($val['field_dispatch_date_value']);
          $pdd = $val['field_promised_date_value'];

          if (!empty($dispatch_date) && !empty($pdd)) {
            $date_diff = $pdd - $dispatch_date;
            if ($date_diff >= 0) {
              $pp_on_time = strtotime(date("H:i", strtotime($val['field_dispatch_start_time_value'])));

              $dispatch_date = date("y-m-d", ($val['field_dispatch_date_value']));
              $bill_date = date("y-m-d", ($val['field_sr_bill_date_value']));
              if (($bill_date == $dispatch_date)) {
                if ($pp_on_time <= $dis_on_time) {
                  $success_count ++;
                }
              }
            }
          }
        }

        $fail_count = $total_cont - $success_count;
        $data[] = array('result', $fail_count, $success_count);
      }
      else {
        $data[] = array('No Result Found', 0, 0);
      }
    }
    else {
      $data[] = array('No Finish Time Found', 0, 0);
    }
    return $data;
  }
  
  else if ($activity_type == 'outlet_status') {
    
    $header = array(
      array('data' => 'Status',),
      array('data' => 'Count',),
    );
    $query = db_select('node', 'n');
    $query->join('field_data_field_hul_code', 'h', 'h.entity_id = n.nid');
    $query->join('node', 'n1', 'h.field_hul_code_value = n1.title');
    $query->leftjoin('field_data_field_hul_status', 'hs', 'hs.entity_id = n1.nid');
    $query->leftjoin('field_data_field_area_hook_group_hul', 't', 't.entity_id = n.nid');
    $query->leftjoin('field_data_field_static_delivery_beat_code', 'd1', 'd1.entity_id= n.nid');
    $query->leftjoin('field_data_field_revised_sequence_status', 'st', 'st.entity_id = n.nid');
    $query->addExpression('COUNT(n.nid)', 'outlet');

    $query->addExpression('SUM(if((hs.field_hul_status_value = 1), 1, 0))', 'address');
    $query->addExpression('SUM(if((t.field_area_hook_group_hul_value), 1, 0))', 'ahg');
    $query->addExpression('SUM(if((st.field_revised_sequence_status_value = 1), 1, 0))', 'revised_seq');
    $query->addExpression('COUNT(d1.field_static_delivery_beat_code_value)', 'beat');
    $query->condition('n.uid', $distributor_id);
    $query->condition('n1.uid', $distributor_id);
    $query->condition('n.type', 'outlet_master');
    $query->condition('n1.type', 'hul_code_master');
    
    $res = $query->execute()->fetch();

    $row1 [] = 'Outlet Count';
    $row1 [] = $res->outlet;
    $rows [0] = $row1;
    
    $row2 [] = 'Address confirmed';
    $row2 [] = $res->address;
    $rows [1] = $row2;

    $row3 [] = 'Area Hook Confirmation';
    $row3 [] = $res->ahg;
    $rows [2] = $row3;

    $row4 [] = 'User Defined Seq Confirmation';
    $row4 [] = $res->revised_seq;
    $rows [3] = $row4;

    $row5 [] = 'Static Deliver Beat';
    $row5 [] = $res->beat;
    $rows [4] = $row5;
    
    $output = theme('table', array('header' => $header, 'rows' => $rows));
    return $output;
  }
  
  
  if ($activity_type == 'bill_pendency') {
    $output1 = array();

    $pp_table = '';
    $data[] = array('Group', 'Non-Dispatched', 'Dispatched');
    
    $query = db_select('node', 'n');
    $query->join('field_data_field_sr_bill_date', 'b', 'b.entity_id = n.nid');
    $query->join('field_data_field_billing_van', 'bva', 'bva.entity_id = n.nid');
    $query->leftjoin('field_data_field_trip_id_reference', 't', 't.entity_id = n.nid');
    $query->condition('n.uid', $distributor_id);
    $query->condition('n.type', 'sales_register_data');
    $query->addExpression('COUNT(n.nid)', 'total_count');
    $query->addExpression('ifnull(COUNT(t.field_trip_id_reference_nid), 0)', 'non_trip_count');
    if (!empty($start_date) && !empty($end_date)) {
      $query->condition('b.field_sr_bill_date_value', array($start_date, $end_date), 'BETWEEN');
    }
    $res = $query->execute()->fetch(PDO::FETCH_ASSOC);

    if (!empty($res)) {
      $total_count = $res['total_count'];
      $non_dis_count = $res['non_trip_count'];
      $dispatched_count = $total_count - $non_dis_count;
      $data[] = array('result', (int)$non_dis_count, (int)$dispatched_count);
    }
    else {
      $data[] = array('No data found', 0, 0);
    }

    if (arg(1) == 'bill_pendency' || $ajax_type == 'bill_pendency_ajax') {
      $pp_table = get_mtd_data_pl_groupwise($start_date, $end_date, $distributor_id);
    }

    $output1['output'] = $data;
    $output1['table'] = $pp_table;
    return $output1;

    
  }
}

function leanbox_dashboard_gauge_chart_query_details($type, $start_date = '', $end_date = '') {

  $output1 = array();
  $output = 0;
  $table = '';

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
  $query->join('field_data_field_promised_date', 'pro_date', 'pro_date.entity_id = n.nid');
  $query->join('field_data_field_billing_van', 'bva', 'bva.entity_id = n.nid');
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
  $query->join('field_data_field_billing_van', 'bva', 'bva.entity_id = n.nid');
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
  $output1['output'] = $output;

  if (arg(1) == 'otif' || $type == 'otif_ajax') {

    $mtd_html = "<h3>Channel Wise Otif</h3>";
    $mtd_data .= get_mtd_data_channel($start_date, $end_date, $uid);
    if ($mtd_data == "No Result Found") {
      $header = get_header_of_chhanel();
      $mtd_rows[] = array("<td colspan='10' style='text-align: center;'> There is No Activity</td>");
      $mtd_html .= theme('table', array('header' => $header, 'rows' => $mtd_rows, 'attributes' => array('border' => '1', 'cellpadding' => '5', 'cellspacing' => '0')));
    }
    else {
      $mtd_html .= $mtd_data;
    }
    $table = $mtd_html;
  }
  $output1['table'] = $table;

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
