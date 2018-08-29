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
  
  // CRM Status of Signed Bills Edit
  $crm_status_signed_bill_html = get_email_crm_status_signed_bills($distributor_id);
  
  // CRM Status of Full Returned Bills
  $crm_status_full_returned_html = get_email_crm_status_full_returned_bills($distributor_id);
  
  $params['subject'] = "Exception MIS";
  $params['body'] = $pt_ct_not_maintained_html . "<br>" . $child_not_maintained_html . "<br>" . $crm_status_high_discounted_bills_html . "<br>" . $crm_status_signed_bill_html . "<br>" . $crm_status_full_returned_html;
  $mail_to = variable_get('exception_mis_to_email');
  $from = variable_get('from_email');
  
  // Attachment for Highly Discounted Bills
  // create directory if not exist
  $dir = 'public://CRM_STATUS';
  file_prepare_directory($dir, FILE_CREATE_DIRECTORY);
  
  $file_name = 'Highly_Discounted_Bills.csv';
  $file_uri = 'public://CRM_STATUS/' . $file_name;

  $file = fopen($file_uri, 'w');

  $table = get_attachment_high_discounted_bills($distributor_id);
  fputcsv($file, $table['header']);
  foreach ($table['rows'] as $line) {
    fputcsv($file, $line);
  }

  fclose($file);

  $params['attachment'] = array(
    'filecontent' => file_get_contents($file_uri),
    'filename' => $file_name,
    'filemime' => 'text/csv',
  );
  
  // Attachment for Signed Bills
  $file2_name = 'Signed_Bills.csv';
  $file2_uri = 'public://CRM_STATUS/' . $file2_name;


  $file2 = fopen($file2_uri, 'w');

  $table2 = get_attachment_signed_bills($distributor_id);
  fputcsv($file2, $table2['header']);
  foreach ($table2['rows'] as $line) {
    fputcsv($file2, $line);
  }

  fclose($file2);

  $params['attachment2'] = array(
    'filecontent' => file_get_contents($file2_uri),
    'filename' => $file2_name,
    'filemime' => 'text/csv',
  );
  
  
  // Attachment for Full Returned Bills
  $file3_name = 'Full_Returned_Bills.csv';
  $file3_uri = 'public://CRM_STATUS/' . $file3_name;

  $file3 = fopen($file3_uri, 'w');

  $table3 = get_attachment_full_returned_bills($distributor_id);
  fputcsv($file3, $table3['header']);
  foreach ($table3['rows'] as $line) {
    fputcsv($file3, $line);
  }

  fclose($file3);

  $params['attachment3'] = array(
    'filecontent' => file_get_contents($file3_uri),
    'filename' => $file3_name,
    'filemime' => 'text/csv',
  );

  drupal_mail('email_status', 'exception_mis', $mail_to, LANGUAGE_NONE, $params, $from);
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
  $start_date = strtotime('-7 day', strtotime(date('Y-m-d')));
  $start = date('Y-m-d', $start_date);
  $end = date('Y-m-d');
  
  $query = db_select('node', 'n');
  $query->condition('n.type', 'sales_register_data');
  $query->condition('n.uid', $distributor_id, '=');
  $query->leftjoin('field_data_field_sales_delivery_status', 'ds', 'ds.entity_id = n.nid');
  $query->join('field_data_field_trip_id_reference', 'ti', 'ti.entity_id = n.nid');
  $query->join('field_data_field_dispatch_date', 'dd', 'dd.entity_id = ti.field_trip_id_reference_nid');
  $query->isNotNull('dd.field_dispatch_date_value');
  $query->condition('dd.field_dispatch_date_value', array($start, $end), 'BETWEEN');
  $query->orderBy('dd.field_dispatch_date_value', 'DESC');
  $query->join('field_data_field_call_type', 'ct', 'ct.entity_id = n.nid');
  $query->condition('ct.field_call_type_value', 'hd', '=');

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
    $row[$dispatch_date]['per_delivered'] = round(($row[$dispatch_date]['delivered_status'] / $row[$dispatch_date]['total']) * 100).'%';
  }

  $header = array('Dispatch Date', 'Delivered', 'Undelivered', 'Not Dispatched', 'Blank', 'Total', '% Delivered');
  
  if($row) {
    foreach ($row as $date => $value) {
      $total_delivered += $value['delivered_status'];
      $total_undelivered += $value['undelivered_status'];
      $total_not_delivered += $value['notdispatched_status'];
      $total_blank += $value['blank_status'];
      $total += $value['total'];
      $per_delivered = round(($total_delivered / $total) * 100).'%';
    }
    $row[] = array('Total', $total_delivered, $total_undelivered, $total_not_delivered, $total_blank, $total, $per_delivered);
  }
  
  $output = "<h3>CRM Status of High Discounted Bills</h3>";
  $output .= theme('table', array('header' => $header, 'rows' => $row, 'attributes' => array('border' => '1', 'cellpadding' => '5', 'cellspacing' => '0'), 'empty' => t('No Records has been found')));
  
  return $output;
}

/*
 * Returns table for CRM Status of Signed Bills Edit
 */
function get_email_crm_status_signed_bills($distributor_id) {
  $start_date = strtotime('-7 day', strtotime(date('Y-m-d')));
  $start = date('Y-m-d', $start_date);
  $end = date('Y-m-d');
  
  $query = db_select('node', 'n');
  $query->condition('n.type', 'sales_register_data');
  $query->condition('n.uid', $distributor_id, '=');
  $query->leftjoin('field_data_field_sales_delivery_status', 'ds', 'ds.entity_id = n.nid');
  $query->join('field_data_field_trip_id_reference', 'ti', 'ti.entity_id = n.nid');
  $query->join('field_data_field_dispatch_date', 'dd', 'dd.entity_id = ti.field_trip_id_reference_nid');
  $query->isNotNull('dd.field_dispatch_date_value');
  $query->condition('dd.field_dispatch_date_value', array($start, $end), 'BETWEEN');
  $query->orderBy('dd.field_dispatch_date_value', 'DESC');
  $query->join('field_data_field_call_type', 'ct', 'ct.entity_id = n.nid');
  $query->condition('ct.field_call_type_value', 'sd', '=');

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
    $row[$dispatch_date]['per_delivered'] = round(($row[$dispatch_date]['delivered_status'] / $row[$dispatch_date]['total']) * 100).'%';
  }

  $header = array('Dispatch Date', 'Delivered', 'Undelivered', 'Not Dispatched', 'Blank', 'Total', '% Delivered');
  
  if($row) {
    foreach ($row as $date => $value) {
      $total_delivered += $value['delivered_status'];
      $total_undelivered += $value['undelivered_status'];
      $total_not_delivered += $value['notdispatched_status'];
      $total_blank += $value['blank_status'];
      $total += $value['total'];
      $per_delivered = round(($total_delivered / $total) * 100).'%';
    }
    $row[] = array('Total', $total_delivered, $total_undelivered, $total_not_delivered, $total_blank, $total, $per_delivered);
  }
  
  $output = "<h3>CRM Status of Signed Bills Edit</h3>";
  $output .= theme('table', array('header' => $header, 'rows' => $row, 'attributes' => array('border' => '1', 'cellpadding' => '5', 'cellspacing' => '0'), 'empty' => t('No Records has been found')));
  
  return $output;
}

/*
 * Returns table for CRM Status of Full Returned Bills
 */
function get_email_crm_status_full_returned_bills($distributor_id) {
  $start_date = strtotime('-7 day', strtotime(date('Y-m-d')));
  $start = date('Y-m-d', $start_date);
  $end = date('Y-m-d');
  
  $query = db_select('node', 'n');
  $query->condition('n.type', 'sales_register_data');
  $query->condition('n.uid', $distributor_id, '=');
  $query->leftjoin('field_data_field_sales_delivery_status', 'ds', 'ds.entity_id = n.nid');
  $query->join('field_data_field_trip_id_reference', 'ti', 'ti.entity_id = n.nid');
  $query->join('field_data_field_dispatch_date', 'dd', 'dd.entity_id = ti.field_trip_id_reference_nid');
  $query->isNotNull('dd.field_dispatch_date_value');
  $query->condition('dd.field_dispatch_date_value', array($start, $end), 'BETWEEN');
  $query->orderBy('dd.field_dispatch_date_value', 'DESC');
  $query->join('field_data_field_call_type', 'ct', 'ct.entity_id = n.nid');
  $query->condition('ct.field_call_type_value', 'fr', '=');

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
    $row[$dispatch_date]['per_delivered'] = round(($row[$dispatch_date]['delivered_status'] / $row[$dispatch_date]['total']) * 100).'%';
  }

  $header = array('Dispatch Date', 'Delivered', 'Undelivered', 'Not Dispatched', 'Blank', 'Total', '% Delivered');
  
  if($row) {
    foreach ($row as $date => $value) {
      $total_delivered += $value['delivered_status'];
      $total_undelivered += $value['undelivered_status'];
      $total_not_delivered += $value['notdispatched_status'];
      $total_blank += $value['blank_status'];
      $total += $value['total'];
      $per_delivered = round(($total_delivered / $total) * 100).'%';
    }
    $row[] = array('Total', $total_delivered, $total_undelivered, $total_not_delivered, $total_blank, $total, $per_delivered);
  }
  
  $output = "<h3>CRM Status of Full Returned Bills</h3>";
  $output .= theme('table', array('header' => $header, 'rows' => $row, 'attributes' => array('border' => '1', 'cellpadding' => '5', 'cellspacing' => '0'), 'empty' => t('No Records has been found')));
  
  return $output;
}

function get_attachment_high_discounted_bills($distributor_id) {
  $start_date = strtotime('-7 day', strtotime(date('Y-m-d')));
  $start = date('Y-m-d', $start_date);
  $end = date('Y-m-d');

  $query = db_select('node', 'n');
  $query->condition('n.type', 'sales_register_data');
  $query->condition('n.uid', $distributor_id, '=');
  $query->leftjoin('field_data_field_sales_delivery_status', 'ds', 'ds.entity_id = n.nid');
  $query->join('field_data_field_trip_id_reference', 'ti', 'ti.entity_id = n.nid');
  $query->join('field_data_field_dispatch_date', 'dd', 'dd.entity_id = ti.field_trip_id_reference_nid');
  $query->join('node', 'nw', 'nw.nid = ti.field_trip_id_reference_nid');
  $query->isNotNull('dd.field_dispatch_date_value');
  $query->condition('dd.field_dispatch_date_value', array($start, $end), 'BETWEEN');
  $query->orderBy('dd.field_dispatch_date_value', 'DESC');
  $query->join('field_data_field_call_type', 'ct', 'ct.entity_id = n.nid');
  $query->condition('ct.field_call_type_value', 'hd', '=');

  $query->fields('n', array('title'));
  $query->fields('nw', array('title'));
  $query->fields('dd', array('field_dispatch_date_value'));
  $query->fields('ds', array('field_sales_delivery_status_tid'));
  $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

  foreach ($res as $data) {
    $row['bill_no'] = $data['title'];
    $row['trip_id'] = $data['nw_title'];
    $row['dispatch_data'] = date('d-m-Y', strtotime($data['field_dispatch_date_value']));
    if ($data['field_sales_delivery_status_tid'] == '59') {
      $row['status'] = 'Delivered';
    }
    else if ($data['field_sales_delivery_status_tid'] == '60') {
      $row['status'] = 'Undelivered';
    }
    else if ($data['field_sales_delivery_status_tid'] == '66') {
      $row['status'] = 'Not Dispatched';
    }
    else {
      $row['status'] = 'Blank';
    }
    $rows[] = $row;
  }
  $header = array('Bill No.', 'Trip ID', 'Dispatch date', 'Status');

  $output1['header'] = $header;
  $output1['rows'] = $rows;

  return $output1;
}


function get_attachment_signed_bills($distributor_id) {
  $start_date = strtotime('-7 day', strtotime(date('Y-m-d')));
  $start = date('Y-m-d', $start_date);
  $end = date('Y-m-d');
  
  $query = db_select('node', 'n');
  $query->condition('n.type', 'sales_register_data');
  $query->condition('n.uid', $distributor_id, '=');
  $query->leftjoin('field_data_field_sales_delivery_status', 'ds', 'ds.entity_id = n.nid');
  $query->join('field_data_field_trip_id_reference', 'ti', 'ti.entity_id = n.nid');
  $query->join('field_data_field_dispatch_date', 'dd', 'dd.entity_id = ti.field_trip_id_reference_nid');
  $query->join('node', 'nw', 'nw.nid = ti.field_trip_id_reference_nid');
  $query->isNotNull('dd.field_dispatch_date_value');
  $query->condition('dd.field_dispatch_date_value', array($start, $end), 'BETWEEN');
  $query->orderBy('dd.field_dispatch_date_value', 'DESC');
  $query->join('field_data_field_call_type', 'ct', 'ct.entity_id = n.nid');
  $query->condition('ct.field_call_type_value', 'sd', '=');
  
  $query->fields('n', array('title'));
  $query->fields('nw', array('title'));
  $query->fields('dd', array('field_dispatch_date_value'));
  $query->fields('ds', array('field_sales_delivery_status_tid'));
  $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
  
  foreach ($res as $data) {
    $row['bill_no'] = $data['title'];
    $row['trip_id'] = $data['nw_title'];
    $row['dispatch_data'] = date('d-m-Y', strtotime($data['field_dispatch_date_value']));
    if ($data['field_sales_delivery_status_tid'] == '59') {
      $row['status'] = 'Delivered';
    }
    else if ($data['field_sales_delivery_status_tid'] == '60') {
      $row['status'] = 'Undelivered';
    }
    else if ($data['field_sales_delivery_status_tid'] == '66') {
      $row['status'] = 'Not Dispatched';
    }
    else {
      $row['status'] = 'Blank';
    }
    $rows[] = $row;
  }
  $header = array('Bill No.', 'Trip ID', 'Dispatch date', 'Status');

  $output1['header'] = $header;
  $output1['rows'] = $rows;

  return $output1;
}
 
function get_attachment_full_returned_bills($distributor_id) {
  $start_date = strtotime('-7 day', strtotime(date('Y-m-d')));
  $start = date('Y-m-d', $start_date);
  $end = date('Y-m-d');
  
  $query = db_select('node', 'n');
  $query->condition('n.type', 'sales_register_data');
  $query->condition('n.uid', $distributor_id, '=');
  $query->leftjoin('field_data_field_sales_delivery_status', 'ds', 'ds.entity_id = n.nid');
  $query->join('field_data_field_trip_id_reference', 'ti', 'ti.entity_id = n.nid');
  $query->join('field_data_field_dispatch_date', 'dd', 'dd.entity_id = ti.field_trip_id_reference_nid');
  $query->join('node', 'nw', 'nw.nid = ti.field_trip_id_reference_nid');
  $query->isNotNull('dd.field_dispatch_date_value');
  $query->condition('dd.field_dispatch_date_value', array($start, $end), 'BETWEEN');
  $query->orderBy('dd.field_dispatch_date_value', 'DESC');
  $query->join('field_data_field_call_type', 'ct', 'ct.entity_id = n.nid');
  $query->condition('ct.field_call_type_value', 'fr', '=');
  
  $query->fields('n', array('title'));
  $query->fields('nw', array('title'));
  $query->fields('dd', array('field_dispatch_date_value'));
  $query->fields('ds', array('field_sales_delivery_status_tid'));
  $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
  
  foreach ($res as $data) {
    $row['bill_no'] = $data['title'];
    $row['trip_id'] = $data['nw_title'];
    $row['dispatch_data'] = date('d-m-Y', strtotime($data['field_dispatch_date_value']));
    if ($data['field_sales_delivery_status_tid'] == '59') {
      $row['status'] = 'Delivered';
    }
    else if ($data['field_sales_delivery_status_tid'] == '60') {
      $row['status'] = 'Undelivered';
    }
    else if ($data['field_sales_delivery_status_tid'] == '66') {
      $row['status'] = 'Not Dispatched';
    }
    else {
      $row['status'] = 'Blank';
    }
    $rows[] = $row;
  }
  $header = array('Bill No.', 'Trip ID', 'Dispatch date', 'Status');

  $output1['header'] = $header;
  $output1['rows'] = $rows;

  return $output1;
}

function send_email_return_vrs_prod() {
  $distributor_id = _DISTRIBUTOR_ID;
  
  $get_subdivision_nids = check_product_validation($distributor_id);
 
  $yesterday_date = date('Y-m-d', strtotime("-1 days"));
  $query = db_select('dispatch_planning_api_data', 'ad');
  $query->condition('ad.distributor_id', $distributor_id, '=');
  $query->fields('ad', array('date'));
  $query->condition('ad.date', $yesterday_date, '=');
  $query->fields('ad', array('runsheet_number'));
  $query->join('node', 'n', 'n.title = ad.runsheet_number');
  $query->condition('n.type', 'dispatch_data', '=');
  $query->condition('n.uid',$distributor_id);
  $query->join('field_data_field_vrs_status', 'vs', 'vs.entity_id = n.nid');
  $query->fields('vs', array('field_vrs_status_value'));
  $query->leftjoin('field_data_field_returned_version', 'rv', 'rv.entity_id = n.nid');
  $query->fields('rv', array('field_returned_version_value'));
  $query->leftjoin('field_data_field_vrs_version', 'vv', 'vv.entity_id = n.nid');
  $query->fields('vv', array('field_vrs_version_value'));
  $query->fields('ad', array('fieldExecutiveName'));
  $query->addExpression('((MAX(CASE WHEN (ad.updated_flag = 1) THEN ad.updated_time END) - MIN(CASE WHEN (ad.updated_flag = 1) THEN ad.updated_time END)) / 60)', 'total_return_time');
  $query->groupBy('ad.runsheet_number');
  $query->join('dispatch_planning_item_details', 'dpi', 'ad.id = dpi.trip_bill_reference');
  $query->addexpression("SUM(dpi.return_qty * dpi.sum_net_sales_value)", 'return_value');
  $query->addexpression("(SUM(CASE WHEN (ad.updated_flag = 1) THEN (dpi.return_qty * dpi.sum_net_sales_value * 10) END)) / ((MAX(CASE WHEN (ad.updated_flag = 1) THEN ad.updated_time END) - MIN(CASE WHEN (ad.updated_flag = 1) THEN ad.updated_time END)) / 60)", 'return_prod');

  $query->addExpression('(COUNT(DISTINCT(CASE WHEN (dpi.line_level_status = \'line_partial_deliver\'  or dpi.line_level_status = \'line_undeliver\') THEN dpi.sku_code END)) * 10) /  ((MAX(CASE WHEN (ad.updated_flag = 1) THEN ad.updated_time END) - MIN(CASE WHEN (ad.updated_flag = 1) THEN ad.updated_time END)) / 60)', 'product_lines');

  $query->leftjoin('vrs_trip_total_history', 'vtth', 'ad.runsheet_number = vtth.trip_id AND ad.distributor_id = vtth.distributor_id');
  //$query->condition('vtth.vrs_version_no', 'V1.0', '=');
  $query->addexpression("(CASE WHEN vtth.vrs_version_no = 'V1.0' THEN vtth.vrs_start_time END) - MAX(CASE WHEN (ad.updated_flag = 1) THEN ad.updated_time END)", 'vrs_start_return_end');
  $query->leftjoin('vrs_trip_total', 'vtt', 'ad.runsheet_number = vtt.trip_id AND ad.distributor_id = vtt.distributor_id');
  $query->addexpression("((vtt.vrs_end_time - (CASE WHEN vtth.vrs_version_no = 'V1.0' THEN vtth.vrs_start_time END)) / 60)", 'vrs_time_difference');
  $query->addexpression("((vtt.total_to_be_collected_amt * 10) / ((vtt.vrs_end_time - (CASE WHEN vtth.vrs_version_no = 'V1.0' THEN vtth.vrs_start_time END)) / 60))", 'vrs_value');
  $query->addexpression("(vtt.total_actual_cash_amount * 100) / (vtt.total_actual_cash_amount + vtth.total_actual_cheque_amount + vtt.total_actual_signed_bill_amount)", 'cash_percentage');
  $query->addexpression("(vtt.total_actual_cheque_amount * 100) / (vtt.total_actual_cash_amount + vtth.total_actual_cheque_amount + vtt.total_actual_signed_bill_amount)", 'cheque_percentage');
  $query->addexpression("(vtt.total_actual_signed_bill_amount * 100) / (vtt.total_actual_cash_amount + vtth.total_actual_cheque_amount + vtt.total_actual_signed_bill_amount)", 'signed_percentage');
  $query->orderBy('vs.field_vrs_status_value', 'DESC');
  
  $query->join('node', 'n1', "n1.title = dpi.sku_code");
  $query->condition('n1.type', 'sku7_code_master', '=');
  $query->condition('n1.uid',$distributor_id);
  $query->leftjoin('field_data_field_product_devision_reference', 'pdr', 'pdr.entity_id = n1.nid');
  $query->addexpression("SUM(CASE WHEN pdr.field_product_devision_reference_nid IN ($get_subdivision_nids) THEN dpi.return_qty END)", 'return_sku_qty_sachets');
  $query->addexpression("SUM(CASE WHEN pdr.field_product_devision_reference_nid NOT IN ($get_subdivision_nids) THEN dpi.return_qty END)", 'return_sku_qty_wo_sachets');
  
  $result = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
  $rows = array();
  foreach ($result as $val) {
    $row['dispatch_date'] = $val['date'];
    $row['trip_id'] = $val['runsheet_number'];

    $field = field_info_field('field_vrs_status');
    $allowed_values = list_allowed_values($field);
    foreach ($allowed_values as $key => $value) {
      if ($key == $val['field_vrs_status_value']) {
        $row['status'] = $value;
      }
    }
    $row['cashier_name'] = $val['fieldExecutiveName'];
    $row['arrival_to_asset_returns'] = '';
    $row['arrival_to_returns_start'] = '';

    $return_version_value = $val['field_returned_version_value'];
    if (empty($return_version_value) || ($val['field_vrs_status_value'] < 1 && !empty($return_version_value))) {
      $row['returns_time_difference'] = '';
      $row['return_prod'] = '';
      $row['product_lines'] = '';
    }
    else {
      $row['returns_time_difference'] = round($val['total_return_time']);
      $row['return_prod'] = round($val['return_prod']);
      $row['product_lines'] = round($val['product_lines']);
    }
    
    $row['return_value'] = round($val['return_value']);
    $row['return_qty_wo_sachets'] = $val['return_sku_qty_wo_sachets'];
    $row['return_qty_sachets'] = $val['return_sku_qty_sachets'];

    $vrs_version_value = $val['field_vrs_version_value'];
    if (empty($return_version_value) || ($val['field_vrs_status_value'] < 2 && !empty($vrs_version_value))) {
      $row['returns_end_to_vrs_start'] = '';
      $row['vrs_time_difference'] = '';
      $row['vrs_value'] = '';
      $row['cash_percentage'] = '';
      $row['cheque_percentage'] = '';
      $row['signed_percentage'] = '';
    }
    else {

      $row['returns_end_to_vrs_start'] = gmdate('i', $val['vrs_start_return_end']);
      $row['vrs_time_difference'] = round($val['vrs_time_difference']);
      $row['vrs_value'] = round($val['vrs_value']);
      $row['cash_percentage'] = round($val['cash_percentage']);
      $row['cheque_percentage'] = round($val['cheque_percentage']);
      $row['signed_percentage'] = round($val['signed_percentage']);
    }

    $rows[] = $row;
  }
  $header = array('Dispatch Date', 'Trip ID', 'Status', 'Cashier', 'Arrival to Asset Returns (min)', 'Arrival to Returns Start (min)', 'Returns Start to Returns End (min)', 'Returns Value(Rs.)/10 Min', 'Product Lines/10 Min','Total Returned Value(Rs.)','Returned SKU Quantity(w/o Sachets)','Returned Quantity of Sachets','Returns End to VRS Start (Min)', 'VRS Start to VRS End (Min)', 'VRS Value(Rs.)/10 Min', 'Cash %', 'Cheque %', 'Signed Bill %');

  $html = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('border' => '1', 'cellpadding' => '5', 'cellspacing' => '0')));

  $params['subject'] = "Returns & VRS Productivity";
  $params['body'] = $html;
  $mail_to = variable_get('return_vrs_to_email');
  $from = variable_get('from_email');

  drupal_mail('email_status', 'daily_data', $mail_to, LANGUAGE_NONE, $params, $from);
}

function send_email_del_app_adoption() {
  $distributor_id = _DISTRIBUTOR_ID;
  $todays_date = date('Y-m-d');
  $todays_date_timestamp = strtotime($todays_date);
  $current_day = date("d", $todays_date_timestamp);
  if ($current_day <= '21') {
    $old_date = strtotime('-1 month', strtotime($todays_date));
    $last_month = date("m", $old_date);
    $last_year = date("Y", $old_date);
    $from_this_date = strtotime(date("$last_year-$last_month-21"));
  }
  else {
    $this_month = date("m", $todays_date_timestamp);
    $from_this_date = strtotime(date("Y-$this_month-21"));
  }
  $yesterday_date = strtotime(date('Y-m-d', strtotime("-1 days"))) ;
  $query = db_select('dispatch_planning_api_data', 'ad');
  $query->condition('ad.distributor_id', $distributor_id, '=');
  $query->condition('ad.dispatch_date', array($from_this_date, $yesterday_date), 'BETWEEN');
  $query->fields('ad', array('fieldExecutiveName'));
  $query->addexpression("ROUND(COUNT(CASE WHEN ad.dispatch_date =  $yesterday_date AND  ad.res_updated_time != 0 THEN ad.bill_number END) / COUNT(CASE WHEN ad.dispatch_date =  $yesterday_date THEN ad.bill_number END) *100)", 'app_adoption');
  $query->orderBy('app_adoption','DESC');
  $query->addexpression("ROUND(COUNT(CASE WHEN ad.dispatch_date = $yesterday_date AND ad.res_updated_time !=0 AND ad.res_updated_time < (UNIX_TIMESTAMP(from_unixtime(ad.res_updated_time, '%Y-%m-%d'))+60*60*18-19800) THEN ad.bill_number END) / COUNT(CASE WHEN  ad.dispatch_date = $yesterday_date AND ad.res_updated_time != 0 THEN ad.bill_number END)*100)", 'entrybefsix');

  $query->addexpression("ROUND(COUNT(CASE WHEN ad.dispatch_date = $yesterday_date AND ad.res_updated_time !=0 AND ad.res_updated_time >= (UNIX_TIMESTAMP(from_unixtime(ad.res_updated_time, '%Y-%m-%d'))+60*60*18-19800) THEN ad.bill_number END) / COUNT(CASE WHEN ad.dispatch_date = $yesterday_date AND ad.res_updated_time != 0 THEN ad.bill_number END)*100)", 'entryaftsix');

  $query->addexpression("ROUND(COUNT(CASE WHEN ad.res_updated_time != 0 THEN ad.bill_number END) / COUNT(ad.bill_number) *100)", 'mtd_app_adoption');
  $query->addexpression("ROUND(COUNT(CASE WHEN ad.res_updated_time !=0 AND ad.res_updated_time < (UNIX_TIMESTAMP(from_unixtime(ad.res_updated_time, '%Y-%m-%d'))+60*60*18-19800) THEN ad.bill_number END) / COUNT(CASE WHEN ad.res_updated_time != 0 THEN ad.bill_number END)*100)", 'mtd_entrybefsix');
  $query->addexpression("ROUND(COUNT(CASE WHEN ad.res_updated_time !=0 AND ad.res_updated_time >= (UNIX_TIMESTAMP(from_unixtime(ad.res_updated_time, '%Y-%m-%d'))+60*60*18-19800) THEN ad.bill_number END) / COUNT(CASE WHEN ad.res_updated_time != 0 THEN ad.bill_number END)*100)", 'mtd_entryaftsix');
  $query->groupBy('ad.fieldExecutiveName');
  $result = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
  $rows[] = array('Cashier', 'Delivery App Adoption(%)', '% Entries before 6:00 PM', '% Entries after 6:00 PM', 'Delivery App Adoption(%)', '% Entries before 6:00 PM', '% Entries after 6:00 PM');
  foreach ($result as $value) {
    $row['fieldExecutiveName'] = $value['fieldExecutiveName'];
    $total['fieldExecutiveName'] = 'Total';
    $row['app_adoption'] = $value['app_adoption'];
    $total['app_adoption'] += $value['app_adoption'];
    $row['entrybefsix'] = $value['entrybefsix'];
    $total['entrybefsix'] += $value['entrybefsix'];
    $row['entryaftsix'] = $value['entryaftsix'];
    $total['entryaftsix'] += $value['entryaftsix'];
    $row['mtd_app_adoption'] = $value['mtd_app_adoption'];
    $total['mtd_app_adoption'] += $value['mtd_app_adoption'];
    $row['mtd_entrybefsix'] = $value['mtd_entrybefsix'];
    $total['mtd_entrybefsix'] += $value['mtd_entrybefsix'];
    $row['mtd_entryaftsix'] = $value['mtd_entryaftsix'];
    $total['mtd_entryaftsix'] += $value['mtd_entryaftsix'];
    $rows[] = $row;
  }
  $rows[] = $total;
  $header = array(array('data' => 'Dispatch Date:' . date('d-M-y', strtotime("-1 days")), 'colspan' => 4), array('data' => 'MTD', 'colspan' => 3));
  $output = "<h3>Delivery App Adoption - RS Jogs</h3>";
  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('border' => '1', 'cellpadding' => '5', 'cellspacing' => '0'), 'empty' => t('No Records has been found')));
  $params['subject'] = "Delivery App Adoption - RS Jogs";
  $params['body'] = $output;
  $mail_to = variable_get('return_del_app_adoption');
  $from = variable_get('from_email');

  drupal_mail('email_status', 'daily_data', $mail_to, LANGUAGE_NONE, $params, $from);
}

function send_email_del_pending() {
  $distributor_id = _DISTRIBUTOR_ID;
  $start_date = strtotime(date('Y-m-d', strtotime("-15 days")));
  $todays_date = strtotime(date('Y-m-d'));

//Find all bills which are DP
  $query = db_select('dispatch_planning_api_data', 'ad');
  $query->condition('ad.distributor_id', $distributor_id, '=');
  $query->condition('ad.dispatch_date', array($start_date, $todays_date), 'BETWEEN');
  $query->condition('ad.delivery_pending_flag', 1, '=');
  $query->fields('ad', array('bill_number'));
  $query->groupBy('ad.bill_number');
  $bills = $query->execute()->fetchCol();

  $query = db_select('dispatch_planning_api_data', 'ad');
  $query->condition('ad.distributor_id', $distributor_id, '=');
  $query->condition('ad.dispatch_date', array($start_date, $todays_date), 'BETWEEN');
  if (!empty($bills)) {
      $query->condition('ad.bill_number', $bills, 'IN');
  }
  $query->fields('ad', array('bill_number'));
  $query->fields('ad', array('bill_date'));
  $query->addexpression("CASE WHEN (ad.delivery_pending_flag = 0 and (ad.res_status IS NOT NULL or ad.res_status !='')) THEN 'Completed' ELSE 'Pending' END", 'attempted_status');
  $query->addexpression("CASE WHEN ad.delivery_pending_flag = 0 THEN ad.res_status END", 'current_bill_status');
  $query->addexpression("CASE WHEN ad.delivery_pending_flag = 0 AND res_status ='undeliver'  THEN ad.res_reason_for_failure END", 'reason_for_failure');
  $query->addexpression("ad.runsheet_number", 'trip_number');
  $query->addexpression("ad.dispatch_date", 'dispatch_date');
  $query->leftjoin('field_data_field_dp_reason', 'dpr', 'dpr.entity_id = ad.delivery_pending_reason');
  $query->fields('dpr', array('field_dp_reason_value'));
  $query->addexpression("ad.fieldExecutiveName", 'fieldExecutiveName');
  $query->join('node', 'n', 'n.title = ad.runsheet_number');
  $query->condition('n.type', 'dispatch_data', '=');
  $query->condition('n.uid', $distributor_id);
  $query->join('field_data_field_vrs_status', 'vs', 'vs.entity_id = n.nid');
  $query->fields('vs', array('field_vrs_status_value'));
  $query->groupBy('ad.bill_number,ad.runsheet_number');
  $query->orderBy('ad.id');
  $result = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
  $arr = array();
  $header_new = array();

  foreach ($result as $val) {
    $arr[$val['bill_number']]['bill_number'] = $val['bill_number'];
    $arr[$val['bill_number']]['bill_date'] = date('d-M-y', $val['bill_date']);
    $arr[$val['bill_number']]['attempted_status'] = $val['attempted_status'];
    $arr[$val['bill_number']]['current_bill_status'] = $val['current_bill_status'];
    $arr[$val['bill_number']]['reason_for_failure'] = $val['reason_for_failure'];
    $arr[$val['bill_number']]['trip_count'] += 1;
    $counter = $arr[$val['bill_number']]['trip_count'];
    $arr[$val['bill_number']]['trip_number' . $counter] = $val['trip_number'];
    $arr[$val['bill_number']]['dispatch_date' . $counter] = date('d-M-y', $val['dispatch_date']);
    $arr[$val['bill_number']]['delivery_pending_reason' . $counter] = $val['field_dp_reason_value'];
    $arr[$val['bill_number']]['fieldExecutiveName' . $counter] = $val['fieldExecutiveName'];
    $field = field_info_field('field_vrs_status');
    $allowed_values = list_allowed_values($field);
    foreach ($allowed_values as $key => $value) {
      if ($key == $val['field_vrs_status_value']) {
        $arr[$val['bill_number']]['vrs_status' . $counter] = $value;
      }
    }
    $header_new['trip_id' . $counter] = 'Trip ' . $counter;
    $header_new['dispatch_date' . $counter] = 'Trip ' . $counter . ' Dispatch Date';
    $header_new['dp_reason' . $counter] = 'Trip ' . $counter . ' DP Reason';
    $header_new['cashier' . $counter] = 'Trip ' . $counter . ' Cashier';
    $header_new['status' . $counter] = 'Trip ' . $counter . ' Status';
  }
  $header = array('Bill Number', 'Bill Date', 'DP Attempted Status', 'Current Bill Status', 'Reason if full returned', 'Count of Trips');
  foreach ($header_new as $val) {
    $header[] = $val;
  }
  
  //Summary Table
  $header_summ = array('DP Attempted Status', 'Total Count of Trips', 'Bill Count','Remarks');
  foreach ($arr as $val) {
    $row[$val['attempted_status']][$val['trip_count']] += 1;
  }

  $p = $row['Pending'];
  $c = $row['Completed'];
  ksort($p);
  ksort($c);

   $i = 0;
  foreach ($p as $key => $val) {  
   $final_summ[$i][] = 'Pending';
    $final_summ[$i][] = $key;
    $final_summ[$i][] = $val;
    if ($key == 1) {
      $final_summ[$i][] = 'These are the count of DP Bills which have yet not been dispatched'; // change the message
    }
    elseif ($key == 2) {
      $final_summ[$i][] = 'These are the count of DP Bills which have been dispatched but delivery outcome has not been updated in TP'; // change the message
    }
    else if ($key == 3){
      $final_summ[$i][] = '- These are the count of DP Bills which have been dispatched but delivery outcome has not been updated in TP. <br> -These DP Bills were dispatched twice i.e. they were declared Delivery Pending twice'; // change the message
    }
    else{
      $final_summ[$i][] = ''; // change the message
    }
    $i++;
  }

  foreach ($c as $key => $val) {
    $final_summ[$i][] = 'Completed';
    $final_summ[$i][] = $key;
    $final_summ[$i][] = $val;
    if ($key == 2) {
      $final_summ[$i][] = 'These are the count of DP bills which have been dispatched and their delivery outcome has also been updated in TP'; // change the message
    }
    else {
      $final_summ[$i][] = ''; // change the message
    }
    $i++;
  }
  
  $output = "<h3>DP Bills Status Report</h3>";
  $output .= theme('table', array('header' => $header_summ, 'rows' => $final_summ, 'attributes' => array('border' => '1', 'cellpadding' => '5', 'cellspacing' => '0'), 'empty' => t('No Records has been found')));
  $output .= '<br><br>';
  //Summary Table
  
  foreach ($result as $val) {
    for ($i = 1; $i <= getMax($arr); $i++) {
      if (!isset($arr[$val['bill_number']]['trip_number' . $i])) {
        $arr[$val['bill_number']]['trip_number' . $i] = '';
      }
      if (!isset($arr[$val['bill_number']]['dispatch_date' . $i])) {
        $arr[$val['bill_number']]['dispatch_date' . $i] = '';
      }
      if (!isset($arr[$val['bill_number']]['delivery_pending_reason' . $i])) {
        $arr[$val['bill_number']]['delivery_pending_reason' . $i] = '';
      }
      if (!isset($arr[$val['bill_number']]['fieldExecutiveName' . $i])) {
        $arr[$val['bill_number']]['fieldExecutiveName' . $i] = '';
      }
      if (!isset($arr[$val['bill_number']]['vrs_status' . $i])) {
        $arr[$val['bill_number']]['vrs_status' . $i] = '';
      }
    }
  }
  $res_set1 = $res_set2 = array();
  foreach($arr as $key => $val){
    if($val['attempted_status'] == 'Pending'){
      $res_set1[$key] = $val; 
    }else{
      $res_set2[$key] = $val;
    }
  }
  
  $final_arr = $res_set1 + $res_set2;

  //$output = "<h3>Delivery Pending Report</h3>";
  $output .= theme('table', array('header' => $header, 'rows' => $final_arr, 'attributes' => array('border' => '1', 'cellpadding' => '5', 'cellspacing' => '0'), 'empty' => t('No Records has been found')));
  $params['subject'] = "DP Bills Status Report";
  $params['body'] = $output;
  $mail_to = variable_get('return_del_pending');
  $from = variable_get('from_email');
  drupal_mail('email_status', 'daily_data', $mail_to, LANGUAGE_NONE, $params, $from);
}

function getMax($arr) {
  $max = 0;
  foreach ($arr as $k => $v) {
    $max = max(array($max, $v['trip_count']));
  }
  return $max;
}
