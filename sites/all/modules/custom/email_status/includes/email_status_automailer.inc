<?php
/**
 * File containing automailers
 */

/*
 * Function to send auto mailer of Monthly Trip Summary.
 */
function send_email_monthly_trip_summary() {
  $distributor_id = _DISTRIBUTOR_ID;
  //$distributor_name = change_feeds_import_get_distributor_name($distributor_id);
  
  $start_date = date('Y-m-01');
  $end_date = date('Y-m-d');

  $query = db_select('node', 'n');
  $query->condition('n.type', 'dispatch_data');
  $query->join('field_data_field_cashier_name', 'cn', 'cn.entity_id = n.nid');
  $query->condition('cn.field_cashier_name_value', '', '!=');
  $query->orderBy('field_cashier_name_value');
  $query->condition('n.uid', $distributor_id, '=');
  $query->join('field_data_field_vehicle_no', 'vn', 'vn.entity_id = n.nid');
  $query->orderBy('field_vehicle_no_value');
  $query->join('field_data_field_trip_id', 'td', 'td.entity_id = n.nid');
  $query->join('field_data_field_bill_count', 'bc', 'bc.entity_id = n.nid');
  $query->join('field_data_field_dispatch_bill_value', 'dbv', 'dbv.entity_id = n.nid');
  $query->join('field_data_field_dispatch_date', 'd', 'd.entity_id = n.nid');
  $query->condition('d.field_dispatch_date_value', array($start_date, $end_date), 'BETWEEN');
  $query->join('field_data_field_dispatch_loader_team_count', 'tc', 'tc.entity_id = n.nid');

  $query->fields('n', array('nid'));
  $query->fields('cn', array('field_cashier_name_value'));
  $query->fields('vn', array('field_vehicle_no_value'));
  $query->fields('td', array('field_trip_id_value'));
  $query->fields('bc', array('field_bill_count_value'));
  $query->fields('dbv', array('field_dispatch_bill_value_value'));
  $query->fields('d', array('field_dispatch_date_value'));
  $query->fields('tc', array('field_dispatch_loader_team_count_value'));

  $all_trip_data = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
  
  // For each trip fetch the van and loading date        
  foreach ($all_trip_data as $key => $one_trip) {
    $query = db_select('field_data_field_trip_id_reference', 'n');
    $query->condition('n.bundle', 'sales_register_data');
    $query->join('field_data_field_billing_van', 'bv', 'bv.entity_id = n.entity_id');
    $query->join('field_data_field_billing_loading_date', 'ld', 'ld.entity_id = n.entity_id');
    $query->fields('bv', array('field_billing_van_value'));
    $query->fields('ld', array('field_billing_loading_date_value'));
    $query->condition('n.field_trip_id_reference_nid', $one_trip['nid'], '=');

    $query->groupBy('bv.field_billing_van_value,ld.field_billing_loading_date_value');
    $van_loading_date = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
  
    // Get the Party Packing Loading Date on th basis of above combination of Van and Loading Date
    $party_packing_flag = 'NO';
    foreach ($van_loading_date as $one_van_loading) {
      $packing_loading_date = date('Y-m-d', $one_van_loading['field_billing_loading_date_value']);
      $query = db_select('node', 'n');
      $query->condition('n.type', 'party_packing');
      $query->join('field_data_field_party_packing_vehicle', 'pv', 'pv.entity_id = n.nid');
      $query->condition('pv.field_party_packing_vehicle_value', $one_van_loading['field_billing_van_value'], '=');
      $query->condition('n.uid', $distributor_id, '=');
      $query->join('field_data_field_party_packing_loading_date', 'ld', 'ld.entity_id = n.nid');
      $query->condition('ld.field_party_packing_loading_date_value', $packing_loading_date, '=');

      $query->leftjoin('field_data_field_party_packing_date', 'pd', 'pd.entity_id = n.nid');
      $query->condition('pd.field_party_packing_date_value', '1970-01-01 05:30:00', '<>');
      $query->fields('n', array('nid'));
      $query->fields('pv', array('field_party_packing_vehicle_value'));
      $query->fields('ld', array('field_party_packing_loading_date_value'));
      $query->fields('pd', array('field_party_packing_date_value'));
      
      $party_packing = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
      // If Packing Loading Date exists then flag will be "Yes" else "No"
      if ($party_packing) {
        $party_packing_flag = 'YES';        
      break;
      }
    }
  $all_trip_data[$key]['party_packing_flag'] = $party_packing_flag;
  }

  $full_table = array();
  foreach($all_trip_data as $value){
    $cashier = $value['field_cashier_name_value'];
    $vehicle_no = $value['field_vehicle_no_value'];
    $trip_id = $value['field_trip_id_value'];
    $bill_count = $value['field_bill_count_value'];
    $bill_value = $value['field_dispatch_bill_value_value'];
    $loader_team_count = $value['field_dispatch_loader_team_count_value'];
    $flag = $value['party_packing_flag'];
    
    // Create the table Group By Cashier , Vehicle No and Party Packing flag
    $full_table[$cashier.'/'.$vehicle_no.'/'.$flag]['cashier'] = $cashier;
    $full_table[$cashier.'/'.$vehicle_no.'/'.$flag]['vehicle_no'] = $vehicle_no;
    $full_table[$cashier.'/'.$vehicle_no.'/'.$flag]['flag'] = $flag;
    $full_table[$cashier.'/'.$vehicle_no.'/'.$flag]['trip_id_count'] += 1;
    $full_table[$cashier.'/'.$vehicle_no.'/'.$flag]['bill_count'] += $bill_count;
    $full_table[$cashier.'/'.$vehicle_no.'/'.$flag]['bill_value'] += $bill_value;
    $full_table[$cashier.'/'.$vehicle_no.'/'.$flag]['loader_team_count'] += $loader_team_count;
    $full_table[$cashier.'/'.$vehicle_no.'/'.$flag]['average_bills_per_trip'] = (int) ($full_table[$cashier.'/'.$vehicle_no.'/'.$flag]['bill_count']/$full_table[$cashier.'/'.$vehicle_no.'/'.$flag]['trip_id_count']);
    $full_table[$cashier.'/'.$vehicle_no.'/'.$flag]['average_bill_value_trip'] = (int) ($full_table[$cashier.'/'.$vehicle_no.'/'.$flag]['bill_value']/$full_table[$cashier.'/'.$vehicle_no.'/'.$flag]['trip_id_count']);    

    $total_trip_count +=  1;
    $total_bill_count += $bill_count;
    $total_bill_value += $bill_value;
    $total_team_count += $loader_team_count;
    $total_avg_bills_per_trip = (int) ($total_bill_count / $total_trip_count);
    $total_avg_bills_value = (int) ($total_bill_value / $total_trip_count);
  }
  
  if(!empty($full_table)){
    $full_table[]  = array('Total', '', '', $total_trip_count, $total_bill_count, $total_bill_value, $total_team_count, $total_avg_bills_per_trip, $total_avg_bills_value);
  }

  $header = array('Cashier Name', 'Vehicle No', 'Party Packing', 'Trip Count', 'Bill Count', 'Dispatch Bill Value', 'Dispatch Loader Team Count','Average Bill Count/Trip','Average Bill Value/Trip');
  
  $output = "<h3>Monthly Trip Summary</h3>";
  $output .= theme('table', array('header' => $header, 'rows' => $full_table, 'attributes' => array('border' => '1', 'cellpadding' => '5', 'cellspacing' => '0'), 'empty' => t('No Records has been found')));
  
  $params['subject'] = "Monthly Trip Summary";
  $params['body'] = $output;
  $mail_to = variable_get('monthly_trip_summary_to_email');
  $from = variable_get('from_email');

  drupal_mail('email_status', 'daily_data', $mail_to, LANGUAGE_NONE, $params, $from);
}

/*
 * Function to send auto mailer of exception mis.
 */
function send_email_exception_mis() {
  $distributor_id = _DISTRIBUTOR_ID;
  $distributor_name = change_feeds_import_get_distributor_name($distributor_id);
  
  // PT-CT Not Maintained
  $pt_ct_not_maintained_html = get_email_pt_ct_not_maintained($distributor_id);
  
  // Child Not Maintained
  $child_not_maintained_html = get_email_child_not_maintained($distributor_id);
  
  // CRM Status of High Discounted Bills
  $crm_status_high_discounted_bills_html = get_email_crm_status_high_discounted_bills($distributor_id);
  
  $params['subject'] = "Exception MIS";
  $params['body'] = $pt_ct_not_maintained_html . "<br>" . $child_not_maintained_html . "<br>" . $crm_status_high_discounted_bills_html;
  $mail_to = variable_get('exception_mis_to_email');
  $from = variable_get('from_email');

  drupal_mail('email_status', 'daily_data', $mail_to, LANGUAGE_NONE, $params, $from);
}

/*
 * Returns table for PT-CT Not Maintained
 */
function get_email_pt_ct_not_maintained($distributor_id) {
  $query = db_select('node', 'n');
  $query->condition('n.type', 'outlet_master');
  $query->condition('n.uid', $distributor_id, '=');
  $query->join('field_data_field_party_code', 'pc', 'pc.entity_id = n.nid');
  $query->join('field_data_field_hul_code', 'hc', 'hc.entity_id = n.nid');
  $query->leftjoin('field_data_field_party_name', 'pn', 'pn.entity_id = n.nid');
  $query->leftjoin('field_data_field_payment_terms', 'pt', 'pt.entity_id = n.nid');
  $query->leftjoin('field_data_field_credit_days', 'cd', 'cd.entity_id = n.nid');
  $db_or = db_or();
  $db_or->isNull('pt.field_payment_terms_value');
  $db_or->isNull('cd.field_credit_days_value');
  $query->condition($db_or);

  $query->fields('pc', array('field_party_code_value'));
  $query->fields('pn', array('field_party_name_value'));
  $query->fields('hc', array('field_hul_code_value')); 
  $query->fields('pt', array('field_payment_terms_value'));
  $query->fields('cd', array('field_credit_days_value'));
  $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
 
  $header = array('Party Code', 'Party Name', 'Hul Code', 'Payment Terms', 'Credit Days');
  
  $output = "<h3>PT-CT Not Maintained</h3>";
  $output .= theme('table', array('header' => $header, 'rows' => $res, 'attributes' => array('border' => '1', 'cellpadding' => '5', 'cellspacing' => '0'), 'empty' => t('No Records has been found')));
  return $output;
}

/*
 * Returns table for SKU7 Child Not Maintained
 */
function get_email_child_not_maintained($distributor_id) {
  $query = db_select('node', 'n');
  $query->condition('n.type', 'sku7_code_master');
  $query->condition('n.uid', $distributor_id, '=');
  $query->join('field_data_field_sku7_code', 'sc', 'sc.entity_id = n.nid');
  $query->join('field_data_field_product_name', 'pn', 'pn.entity_id = n.nid');
  $query->condition('field_product_name_value', db_like('X') . '%', 'LIKE');
  $query->leftjoin('field_data_field_child_sku', 'cs', 'cs.entity_id = n.nid');
  $query->isNULL('field_child_sku_nid'); 

  $query->fields('sc', array('field_sku7_code_value'));
  $query->fields('pn', array('field_product_name_value'));
  $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

  $header = array('SKU7 Code', 'SKU7 Description');
  
  $output = "<h3>Child Not Maintained</h3>";
  $output .= theme('table', array('header' => $header, 'rows' => $res, 'attributes' => array('border' => '1', 'cellpadding' => '5', 'cellspacing' => '0'), 'empty' => t('No Records has been found')));
  return $output;
}

/*
 * Returns table for CRM Status of High Discounted Bills
 */
function get_email_crm_status_high_discounted_bills($distributor_id) {
  //$start_date = strtotime('-7 day', strtotime(date('Y-m-d')));
  //$end_date = strtotime(date('Y-m-d'));
  $start_date = strtotime('-7 day', strtotime(date('Y-m-d')));
  $start = date('Y-m-d', $start_date);
  $end = date('Y-m-d');
  
  $query = db_select('node', 'n');
  $query->condition('n.type', 'sales_register_data');
  $query->condition('n.uid', $distributor_id, '=');
  //$query->join('field_data_field_sr_bill_date', 'bd', 'bd.entity_id = n.nid');
  //$query->condition('bd.field_sr_bill_date_value', array($start_date, $end_date), 'BETWEEN');
  //$query->orderBy('bd.field_sr_bill_date_value', 'DESC');
  $query->leftjoin('field_data_field_sales_delivery_status', 'ds', 'ds.entity_id = n.nid');
  $query->join('field_data_field_trip_id_reference', 'ti', 'ti.entity_id = n.nid');
  $query->join('field_data_field_dispatch_date', 'dd', 'dd.entity_id = ti.field_trip_id_reference_nid');
  $query->isNotNull('dd.field_dispatch_date_value');
  $query->condition('dd.field_dispatch_date_value', array($start, $end), 'BETWEEN');
  $query->orderBy('dd.field_dispatch_date_value', 'DESC');
  $query->join('field_data_field_pod_flag', 'pf', 'pf.entity_id = n.nid');
  $query->condition('pf.field_pod_flag_value', 1, '=');

  //$query->fields('bd', array('field_sr_bill_date_value'));
  $query->fields('dd', array('field_dispatch_date_value'));
  $query->fields('ds', array('field_sales_delivery_status_tid'));
  $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

  foreach ($res as $data) {
    //$bill_date = date('d-m-Y',$data['field_sr_bill_date_value']);
    $dispatch_date = date('d-m-Y',strtotime($data['field_dispatch_date_value']));
      
    if (!isset($row[$dispatch_date])) {
      $row[$dispatch_date]['dispatch_date'] = $dispatch_date;
      $row[$dispatch_date]['delivered_status'] = 0;
      $row[$dispatch_date]['undelivered_status'] = 0;
      $row[$dispatch_date]['notdispatched_status'] = 0;
      $row[$dispatch_date]['blank_status'] = 0;
    }

    if ($data['field_sales_delivery_status_tid'] == '59') {
      $row[$dispatch_date]['delivered_status'] += 1;
    }
    else if ($data['field_sales_delivery_status_tid'] == '60') {
      $row[$dispatch_date]['undelivered_status'] += 1;
    }
    else if ($data['field_sales_delivery_status_tid'] == '66') {
      $row[$dispatch_date]['notdispatched_status'] += 1;
    }
    else {
      $row[$dispatch_date]['blank_status'] += 1;
    }
  
    $row[$dispatch_date]['total'] +=1; 
  }

  $header = array('Dispatch Date', 'Delivered', 'Undelivered', 'Not Dispatched', 'Blank', 'Total');
  
  $output = "<h3>CRM Status of High Discounted Bills</h3>";
  $output .= theme('table', array('header' => $header, 'rows' => $row, 'attributes' => array('border' => '1', 'cellpadding' => '5', 'cellspacing' => '0'), 'empty' => t('No Records has been found')));
  
  return $output;
}