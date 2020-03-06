<?php
/**
 * File containing automailers
 */

/*
 * Function to send auto mailer of exception mis.
 */
function send_email_exception_mis() {
  $receiver_email_str = variable_get('exception_mis_to_email');
  $receiver_email_arr = explode(';', $receiver_email_str);
  
  foreach ($receiver_email_arr as $key => $value) {
    $dist_ids_str = $to_email_str = "";
    $individual_line_arr = $dist_ids_arr = array();
    if (strpos($value, ':')) {
      $individual_line_arr = explode(':', $value);
      $dist_ids_str = $individual_line_arr[0];
      $to_email_str = $individual_line_arr[1];
      $dist_ids_arr = explode(',', $dist_ids_str);
      if (count($dist_ids_arr) > 1) {
        send_email_exception_mis_callback($to_email_str, $dist_ids_arr);
      }
      else {
        $only_one_distributor_id = trim($dist_ids_arr[0]);
        send_email_exception_mis_callback($to_email_str, array($only_one_distributor_id));
      }
    }
    else {
      $active_distributors_id_arr = get_all_active_distributors_id();
      $receiver_email = $receiver_email_arr[0];
      send_email_exception_mis_callback($receiver_email, $active_distributors_id_arr); //array($dist_id)
    }
  }
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
  $query->condition('n.uid', $distributor_id, 'IN');
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
  $query->condition('n.uid', $distributor_id, 'IN');
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
  $query->condition('n.uid', $distributor_id, 'IN');
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
  $receiver_email_str = variable_get('return_vrs_to_email');
  $receiver_email_arr = explode(';', $receiver_email_str);
  
  foreach ($receiver_email_arr as $key => $value) {
    $dist_ids_str = $to_email_str = "";
    $individual_line_arr = $dist_ids_arr = array();
    if (strpos($value, ':')) {
      $individual_line_arr = explode(':', $value);
      $dist_ids_str = $individual_line_arr[0];
      $to_email_str = $individual_line_arr[1];
      $dist_ids_arr = explode(',', $dist_ids_str);
      if (count($dist_ids_arr) > 1) {
        send_email_return_vrs_prod_callback($to_email_str, $dist_ids_arr);
      }
      else {
        $only_one_distributor_id = trim($dist_ids_arr[0]);
        send_email_return_vrs_prod_callback($to_email_str, array($only_one_distributor_id));
      }
    }
    else {
      $active_distributors_id_arr = get_all_active_distributors_id();
      $receiver_email = $receiver_email_arr[0];
      send_email_return_vrs_prod_callback($receiver_email, $active_distributors_id_arr); //array($dist_id)
    }
  }
}

function send_email_del_app_adoption() {
  $receiver_email_str = variable_get('return_del_app_adoption');
  $receiver_email_arr = explode(';', $receiver_email_str);
  
  foreach ($receiver_email_arr as $key => $value) {
    $dist_ids_str = $to_email_str = "";
    $individual_line_arr = $dist_ids_arr = array();
    if (strpos($value, ':')) {
      $individual_line_arr = explode(':', $value);
      $dist_ids_str = $individual_line_arr[0];
      $to_email_str = $individual_line_arr[1];
      $dist_ids_arr = explode(',', $dist_ids_str);
      if (count($dist_ids_arr) > 1) {
        send_email_del_app_adoption_callback($to_email_str, $dist_ids_arr);
      }
      else {
        $only_one_distributor_id = trim($dist_ids_arr[0]);
        send_email_del_app_adoption_callback($to_email_str, array($only_one_distributor_id));
      }
    }
    else {
      $active_distributors_id_arr = get_all_active_distributors_id();
      $receiver_email = $receiver_email_arr[0];
      send_email_del_app_adoption_callback($receiver_email, $active_distributors_id_arr); //array($dist_id)
    }
  }
}

// Delivery Pending Report

function send_email_del_pending(){
 $receiver_email_str = variable_get('return_del_pending');
 $receiver_email_arr = explode(';', $receiver_email_str);
    foreach ($receiver_email_arr as $key => $value) {
     $dist_ids_str = $to_email_str = "";
     $individual_line_arr = $dist_ids_arr = array();
     if (strpos($value, ':')) {
       $individual_line_arr = explode(':', $value);
       $dist_ids_str = $individual_line_arr[0];
       $to_email_str = $individual_line_arr[1];
       $dist_ids_arr = explode(',', $dist_ids_str);
       if (count($dist_ids_arr) > 1) {
         send_email_del_pekding_callback($to_email_str, $dist_ids_arr);
       }
       else {
         $only_one_distributor_id = trim($dist_ids_arr[0]);
         send_email_del_pekding_callback($to_email_str, array($only_one_distributor_id));
       }
     }
     else {
       $active_distributors_id_arr = get_all_active_distributors_id();
       $receiver_email = $receiver_email_arr[0];
       send_email_del_pekding_callback($receiver_email, $active_distributors_id_arr); //array($dist_id)
     }
    }
 
}





/*function send_email_pt_compliance() {
    

}*/

function send_email_pt_compliance(){
 $receiver_email_str = variable_get('pt_compliance');
 $receiver_email_arr = explode(';', $receiver_email_str);
    foreach ($receiver_email_arr as $key => $value) {
     $dist_ids_str = $to_email_str = "";
     $individual_line_arr = $dist_ids_arr = array();
     if (strpos($value, ':')) {
       $individual_line_arr = explode(':', $value);
       $dist_ids_str = $individual_line_arr[0];
       $to_email_str = $individual_line_arr[1];
       $dist_ids_arr = explode(',', $dist_ids_str);
       if (count($dist_ids_arr) > 1) {
         send_email_pt_compliance_callback($to_email_str, $dist_ids_arr);
       }
       else {
         $only_one_distributor_id = trim($dist_ids_arr[0]);
         send_email_pt_compliance_callback($to_email_str, array($only_one_distributor_id));
       }
     }
     else {
       $active_distributors_id_arr = get_all_active_distributors_id();
       $receiver_email = $receiver_email_arr[0];
       send_email_pt_compliance_callback($receiver_email, $active_distributors_id_arr); //array($dist_id)
     }
    }    
}

function send_email_bills_not_attached_to_trip(){
  $receiver_email_str = variable_get('bills_not_attached_to_trip_to_email');
  $receiver_email_arr = explode(';', $receiver_email_str);
  //if (!empty($receiver_email_arr)) {
    foreach ($receiver_email_arr as $key => $value) {
      $dist_ids_str = $to_email_str = "";
      $individual_line_arr = $dist_ids_arr = array();
      
      if (strpos($value, ':')) {
        $individual_line_arr = explode(':', $value);
        $dist_ids_str = $individual_line_arr[0];
        $to_email_str = $individual_line_arr[1];
        $dist_ids_arr = explode(',', $dist_ids_str);
        if (count($dist_ids_arr) > 1) {
          send_email_not_attached_trip($to_email_str, $dist_ids_arr); //$to_email_str = abc@.com,dfg@.com $dist_ids_arr = 101000,101000
        }
        else {
          $only_one_distributor_id = trim($dist_ids_arr[0]);
          send_email_not_attached_trip($to_email_str, array($only_one_distributor_id)); //$to_email_str = abc@.com/abc@.com/sdf@.com
        }
      }
      else {
        $active_distributors_id_arr = get_all_active_distributors_id();
        $receiver_email = $receiver_email_arr[0]; /// it is coming empty as no dis id is maintaine at config level
        send_email_not_attached_trip($receiver_email, $active_distributors_id_arr); //array($dist_id)
      }
    } //end of foreach()
  //}
}

// bill status not updated 

function send_email_bills_status_not_updated() {

  $receiver_email_str = variable_get('bills_with_status_not_updated_to_email');
  $receiver_email_arr = explode(';', $receiver_email_str); //explode(PHP_EOL, $receiver_email_str); //first exploding by new line (i.e PHP_EOL)
  foreach($receiver_email_arr as $key => $value){
    $dist_ids_str = $to_email_str = "";
    $individual_line_arr = $dist_ids_arr = array();

    if(strpos($value, ':')){
      $individual_line_arr = explode(':', $value);
      $dist_ids_str = $individual_line_arr[0];
      $to_email_str = $individual_line_arr[1];
      $dist_ids_arr = explode(',', $dist_ids_str);
      if(count($dist_ids_arr) > 1){
        send_email_status_not_updated($to_email_str, $dist_ids_arr);
      }else{
        $only_one_distributor_id = trim($dist_ids_arr[0]);
        send_email_status_not_updated($to_email_str, array($only_one_distributor_id));
      }
    }else{
      $active_distributors_id_arr = get_all_active_distributors_id();
      $receiver_email = $receiver_email_arr[0];
      send_email_status_not_updated($receiver_email, $active_distributors_id_arr); //array($dist_id)
    }  
  } //end of foreach()
}


function get_all_active_distributors_id(){
  $query = db_select('users', 'u');
  $query->join('field_data_field_third_party_username', 'rss_code', 'u.uid = rss_code.entity_id');
  $query->join('field_data_field_distributor', 'dis', 'u.uid = dis.entity_id');
  $query->condition('u.status','1','=');
  //$query->condition('rss_code.field_third_party_username_value','marico','='); //marico condition
  $query->fields('dis', array('field_distributor_value'))->distinct();
  //$query->fields('u', array('name'));
  $active_distributors_id_arr = $query->execute()->fetchCol(); //fetchAll(PDO::FETCH_ASSOC); 
  return $active_distributors_id_arr;
}


function send_email_not_attached_trip($receiver_email_str, $dist_ids_arr){
  $rows = array();
  $distributor_name_arr = array();
  foreach ($dist_ids_arr as $key => $id_value) {
    $distributor_id = $id_value;
    $distributor_name = change_feeds_import_get_distributor_name($distributor_id);
    if(!empty($distributor_name) && isset($distributor_name)){
      array_push($distributor_name_arr, $distributor_name);
    }

    $yesterday_date = strtotime('-1 day', strtotime(date('Y-m-d')));
    $query = db_select('node', 'n');
    $query->condition('n.type', 'sales_register_data');
    $query->condition('n.uid', $distributor_id, '=');
    $query->join('field_data_field_sr_bill_date', 'bd', 'bd.entity_id = n.nid');
    $query->condition('bd.field_sr_bill_date_value', $yesterday_date, '<=');
    $query->join('field_data_field_bill_value', 'bv', 'bv.entity_id = n.nid');
    $query->join('field_data_field_name', 'rs_name', 'rs_name.entity_id = n.uid');
    $query->leftjoin('field_data_field_previous_trip', 'pt', 'pt.entity_id = n.nid');
    $query->leftjoin('field_data_field_trip_id_reference', 'tr', 'tr.entity_id = n.nid');
    $query->isNULL('tr.field_trip_id_reference_nid');

    $query->fields('n', array('nid'));
    $query->fields('n', array('title'));
    $query->fields('bd', array('field_sr_bill_date_value'));
    $query->fields('bv', array('field_bill_value_value'));
    $query->fields('pt', array('field_previous_trip_value'));
    $query->fields('rs_name', array('field_name_value'));
    $all_bills = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
    $arr = build_not_attached_trip_table_rows($all_bills);
    if(!empty($arr)){
      foreach ($arr['rows'] as $key1 => $val1) {
      $rows[] = $val1;
      }      
    }
    else {
      $rows = array();
    }
    
  $header .= array('RS Name', 'Bill No.', 'Bill Date', 'Total Value', 'Previous Trip No');
  $output .= "<h3>Bills Not Attached To Trip $distributor_name</h3>";
  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('border' => '1', 'cellpadding' => '5', 'cellspacing' => '0'), 'empty' => t('No Records has been found')));    
  }
  $multiple_distributor_name = implode(', ', $distributor_name_arr);
  $params['subject'] = "Bills Not Attached To Trip - ".$multiple_distributor_name;

  $params['body'] = $output;
  $mail_to = $receiver_email_str; //variable_get('quantity_mismatched_to_email');
  $from = variable_get('from_email');
  drupal_mail('email_status', 'daily_data', $mail_to, LANGUAGE_NONE, $params, $from);   
}


function build_not_attached_trip_table_rows($all_bills){
  $row = array();
  if(!empty($all_bills)){
    foreach ($all_bills as $value) {
      $row['rs_name'] = $value['field_name_value'];
      $row['bill_no'] = $value['title'];
      $row['bill_date'] = date('d-M-y', $value['field_sr_bill_date_value']);
      $row['bill_value'] = $value['field_bill_value_value'];
      $row['prev_trip'] = $value['field_previous_trip_value'];

      $rows[] = $row;
    }    
  }
  else{
    $rows = $row;
  }
  return array("rows"=>$rows);
}



 function send_email_status_not_updated($receiver_email_str , $dist_ids_arr){
  $rows1 = array();
  $distributor_name_arr = array();
  foreach ($dist_ids_arr as $key => $id_value) {
    $distributor_id = $id_value; //_MARICO_DISTRIBUTOR_ID;    
    $distributor_name = change_feeds_import_get_distributor_name($distributor_id);
    if(!empty($distributor_name) && isset($distributor_name)){
      array_push($distributor_name_arr, $distributor_name);
    }
  
    $end = strtotime(date('Y-m-d', strtotime("-2 days"))); //5 march
    $start = strtotime(date('Y-m-d', strtotime("-8 days"))); // 27 march

    $check_todays_day = date('N');
    $end_date_for_monday = strtotime(date('Y-m-d', strtotime("-3 days")));

    // Table OF Summary Of Bills Count
    $query = db_select('dispatch_planning_api_data', 'dpi');
    $query->join('field_data_field_name', 'rs_name', 'dpi.distributor_id = rs_name.entity_id');

    $query->condition('dpi.distributor_id', $distributor_id, '=');

    // check if today is Monday
    if ($check_todays_day == 1) {
      $query->condition('dpi.bill_date', array($start, $end_date_for_monday), 'BETWEEN');
    }
    else {
      $query->condition('dpi.bill_date', array($start, $end), 'BETWEEN');
    }
    //$query->fields('dpi', array('runsheet_number'));
    //$query->fields('dpi', array('fieldExecutiveName'));
    $query->fields('dpi', array('bill_date'));
    $query->fields('rs_name', array('field_name_value'));
    $query->addExpression('(COUNT(dpi.bill_number))', 'Total_Bills_Count');
    $query->addExpression('SUM(CASE WHEN (dpi.res_status IS NULL) THEN 1 ELSE 0 END)', 'Bills_Count_Empty_Status');

    //$query->groupBy('dpi.runsheet_number');
    $query->groupBy('dpi.bill_date');

    //$query->orderBy('dpi.runsheet_number', 'DESC');
    $query->orderBy('dpi.bill_date', 'DESC');
    $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
    watchdog('bill-status-notipdated-query', '<pre>' . print_r($res, TRUE) . '</pre>');
    $arr = build_status_not_updated_table_rows($res);
    foreach($arr['rows1'] as $key2 => $val2){
      $rows1[] = $val2;
    }
    
    $header1 .= array('RS Name', 'Bill Date', 'Total Bill Count', 'Bill Count-Status Not Updated');
    $output1 .= "<h3>Summary (Date Range from N-8 to N-2 Bill Date (N is Today's Date)) $distributor_name </h3>";
    $output1 .= theme('table', array('header' => $header1, 'rows' => $rows1, 'attributes' => array('border' => '1', 'cellpadding' => '5', 'cellspacing' => '0'), 'empty' => t('No Records has been found')));    

  }
    $file_name = 'Bills_With_Status_Not_Updated.csv';
    $file_uri = 'public://Bills_Status/' . $file_name;
    // create directory if not exist
    $dir = 'public://Bills_Status';
    file_prepare_directory($dir, FILE_CREATE_DIRECTORY);
  
    $file = fopen($file_uri, 'w');
    $table = get_attachment_bills_status_not_updated($dist_ids_arr);
    fputcsv($file, $table['header']);
    foreach ($table['rows'] as $line) {
      fputcsv($file, $line);
    }
  
    fclose($file);
    $params['attachment']= array(
      'filecontent' => file_get_contents($file_uri),
      'filename' => $file_name,
      'filemime' => 'text/csv',
    );
  $multiple_distributor_name = implode(', ', $distributor_name_arr);
  $params['subject'] = "Bills With Status Not Updated - ".$multiple_distributor_name;
  
  $params['body'] = $output1;
  $mail_to = $receiver_email_str; //variable_get('quantity_mismatched_to_email');
  $from = variable_get('from_email');
  watchdog('bill-status-not-updated-email-data', '<pre>' . print_r($from,$params,$mail_to,$output1, TRUE) . '</pre>');
  drupal_mail('email_status', 'daily_data', $mail_to, LANGUAGE_NONE, $params, $from);    
 }
 
 function get_attachment_bills_status_not_updated($distributor_id) {

  $end = strtotime(date('Y-m-d', strtotime("-2 days")));
  $start = strtotime(date('Y-m-d', strtotime("-8 days")));

  $check_todays_day = date('N');
  $end_date_for_monday = strtotime(date('Y-m-d', strtotime("-3 days")));

  $query = db_select('dispatch_planning_api_data', 'dpi');
  $query->condition('dpi.distributor_id', $distributor_id, 'IN');
  $query->isNull('dpi.res_status');
  $query->join('dispatch_planning_item_details', 'ad', 'ad.trip_bill_reference = dpi.id');
  $query->join('node', 'n', 'n.title = dpi.bill_number');
  $query->join('field_data_field_name', 'rs_name', 'dpi.distributor_id = rs_name.entity_id');
  $query->condition('n.uid', $distributor_id);
  $query->condition('n.type', 'sales_register_data');
  $query->join('field_data_field_bill_value', 'bv', 'bv.entity_id = n.nid');

  $query->orderBy('dpi.runsheet_number', 'DESC');
  $query->orderBy('dpi.bill_number', 'DESC');

  // check if today is Monday
  if ($check_todays_day == 1) {
    $query->condition('dpi.bill_date', array($start, $end_date_for_monday), 'BETWEEN');
  }
  else {
    $query->condition('dpi.bill_date', array($start, $end), 'BETWEEN');
  }

  $query->fields('dpi', array('runsheet_number'));
  $query->fields('dpi', array('bill_number'));
  $query->fields('dpi', array('bill_date'));
  $query->fields('rs_name', array('field_name_value'));
  $query->fields('dpi', array('fieldExecutiveName'));
  $query->addExpression('(SUM(ad.sum_net_sales_quantity))', 'Total_Sales_Qty');
  $query->fields('bv', array('field_bill_value_value'));
  $query->groupBy('ad.trip_bill_reference');
  $all_trips = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

  $row = array();
  foreach ($all_trips as $value) {
    $row['rs_name'] = $value['field_name_value'];
    $row['trip_id'] = $value['runsheet_number'];
    $row['bill_no'] = $value['bill_number'];
    $row['bill_date'] = date('d-M-y', $value['bill_date']);
    $row['total_sales_qty'] = $value['Total_Sales_Qty'];
    $row['total_value'] = $value['field_bill_value_value'];
    $row['cashier_name'] = $value['fieldExecutiveName'];
    $rows[] = $row;
  }

  $header = array('RS Name', 'Trip ID', 'Bill No.', 'Bill Date', 'Total Sales Qty', 'Total Value', 'Cashier Name');
  $output['header'] = $header;
  $output['rows'] = $rows;

  return $output;
}



/** Function to send auto mailer of quantity mismatch
 */
function send_email_dispatched_quantity_mismatch() {

  $receiver_email_str = variable_get('quantity_mismatched_to_email');
  $receiver_email_arr = explode(';', $receiver_email_str); //explode(PHP_EOL, $receiver_email_str); //first exploding by new line (i.e PHP_EOL)

  foreach($receiver_email_arr as $key => $value){
    $dist_ids_str = $to_email_str = "";
    $individual_line_arr = $dist_ids_arr = array();

    if(strpos($value, ':')){
      $individual_line_arr = explode(':', $value);
      $dist_ids_str = $individual_line_arr[0];
      $to_email_str = $individual_line_arr[1];
      $dist_ids_arr = explode(',', $dist_ids_str);
      if(count($dist_ids_arr) > 1){
        send_email_quantity_mismatch($to_email_str, $dist_ids_arr);
      }else{
        $only_one_distributor_id = trim($dist_ids_arr[0]);  
        send_email_quantity_mismatch($to_email_str, array($only_one_distributor_id));
      }
    }else{
      $active_distributors_id_arr = get_all_active_distributors_id();
      $receiver_email = $receiver_email_arr[0];
      //$dist_id = _MARICO_DISTRIBUTOR_ID;
      send_email_quantity_mismatch($receiver_email, $active_distributors_id_arr); //array($dist_id)
    }  
  } //end of foreach()
}


/** Function to send mails to receiver for dispatched_quantity_mismatch
 */
function send_email_quantity_mismatch($receiver_email_str, $dist_ids_arr){

  //$distributor_name = change_feeds_import_get_distributor_name($dist_ids_arr[0]);
  
  $rows = $rows1 = array();
  $distributor_name_arr = array();
  foreach ($dist_ids_arr as $key => $id_value) {

    $distributor_id = $id_value;//$dist_id; //_MARICO_DISTRIBUTOR_ID;
    $distributor_name = change_feeds_import_get_distributor_name($distributor_id);
    if(!empty($distributor_name) && isset($distributor_name)){
      array_push($distributor_name_arr, $distributor_name);
    }
  
    $bill_date = strtotime('-2 day', strtotime(date('Y-m-d')));
  
    $check_todays_day = date('N');
    $bill_date_for_monday = strtotime('-3 day', strtotime(date('Y-m-d')));
  
    $query = db_select('dispatch_planning_api_data', 'dpi');
    $query->condition('dpi.distributor_id', $distributor_id, '=');
    $query->join('dispatch_planning_item_details', 'ad', 'ad.trip_bill_reference = dpi.id');
    $query->join('field_data_field_name', 'rs_name', 'dpi.distributor_id = rs_name.entity_id');
    $query->isNotNull('dpi.res_status');
    // check if today is Monday
    if ($check_todays_day == 1) {
      $query->condition('dpi.bill_date', $bill_date_for_monday, '<=');
    }
    else {
      $query->condition('dpi.bill_date', $bill_date, '<=');
    }
  
    $query->fields('dpi', array('runsheet_number'));
    $query->fields('dpi', array('bill_number'));
    $query->fields('dpi', array('bill_date'));
    $query->fields('dpi', array('fieldExecutiveName'));
    $query->fields('rs_name', array('field_name_value'));
    $query->addExpression('(SUM(ad.sum_net_sales_quantity - ad.res_godown_short))', 'Dispatched_Sales_Qty');
    $query->addExpression('(SUM(ad.dispatch_qty))', 'Dispatch_Qty');
    $query->addExpression('(SUM(ad.dispatch_qty - ad.return_qty - ad.res_cashier_short))', 'Delivered_Sales_Qty');
    $query->addExpression('(SUM(ad.res_sku_actual_quantity1))', 'Delivered_Qty');
    $query->groupBy('ad.trip_bill_reference');
    $all_trips = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
    $arr = build_quntity_mismatch_table_rows($all_trips);
    foreach ($arr['rows'] as $key1 => $val1) {
      $rows[] = $val1;
    }

    foreach($arr['rows1'] as $key2 => $val2){
      $rows1[] = $val2;
    }
  }
  $multiple_distributor_name = implode(', ', $distributor_name_arr);
  $params['subject'] = "Quantity Mismatched - ".$multiple_distributor_name;
  
  $header = array('RS Name', 'Trip ID', 'Bill No.', 'Bill Date', 'Cashier Name');
  $output = "<h3>Quantity Mismatched (Dispatched quantity IS NOT equal to Total Sales quantity - Godown Short)</h3>";
  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('border' => '1', 'cellpadding' => '5', 'cellspacing' => '0'), 'empty' => t('No Records has been found')));

  $output1 = "<h3>Quantity Mismatched (Delivered quantity IS NOT equal to Dispatched Quantity - Returned Quantity - Cashier Short)</h3>";
  $output1 .= theme('table', array('header' => $header, 'rows' => $rows1, 'attributes' => array('border' => '1', 'cellpadding' => '5', 'cellspacing' => '0'), 'empty' => t('No Records has been found')));

  $params['body'] = $output.$output1; // build_quntity_mismatch_table_rows($id_value); 
  $mail_to = $receiver_email_str; //variable_get('quantity_mismatched_to_email');
  $from = variable_get('from_email');
  drupal_mail('email_status', 'daily_data', $mail_to, LANGUAGE_NONE, $params, $from);  
}

/* function to build mail body of dispatched quantity mismatch  */
function build_quntity_mismatch_table_rows($all_trips){
  $row = array();
  foreach ($all_trips as $value) {
    $row['rs_name'] = $value['field_name_value'];
    $row['trip_id'] = $value['runsheet_number'];
    $row['bill_no'] = $value['bill_number'];
    $row['bill_date'] = date('d-M-y', $value['bill_date']);
    $row['cashier_name'] = $value['fieldExecutiveName'];
    // Condition Of Dispatch qty is not equal to Total Sales Qty - Godown short
    if ($value['Dispatched_Sales_Qty'] != $value['Dispatch_Qty']) {
      $rows[] = $row;
    }
    // Condition Of Delivered quantity IS NOT equal to Dispatched Quantity - Returned Quantity - Cashier Short
    if ($value['Delivered_Sales_Qty'] != $value['Delivered_Qty']) {
      $rows1[] = $row;
    }
  }
  return array("rows"=>$rows, "rows1"=>$rows1);
}

/*
 * Function to send auto mailer of live tracking data
 */
function send_email_live_tracking_data() {
  $receiver_email_str = variable_get('live_tracking_data_to_email');
  $receiver_email_arr = explode(';', $receiver_email_str);
  
  foreach ($receiver_email_arr as $key => $value) {
    $dist_ids_str = $to_email_str = "";
    $individual_line_arr = $dist_ids_arr = array();
    if (strpos($value, ':')) {
      $individual_line_arr = explode(':', $value);
      $dist_ids_str = $individual_line_arr[0];
      $to_email_str = $individual_line_arr[1];
      $dist_ids_arr = explode(',', $dist_ids_str);
      if (count($dist_ids_arr) > 1) {
        send_email_live_tracking_data_callback($to_email_str, $dist_ids_arr);
      }
      else {
        $only_one_distributor_id = trim($dist_ids_arr[0]);
        send_email_live_tracking_data_callback($to_email_str, array($only_one_distributor_id));
      }
    }
    else {
      $active_distributors_id_arr = get_all_active_distributors_id();
      $receiver_email = $receiver_email_arr[0];
      send_email_live_tracking_data_callback($receiver_email, $active_distributors_id_arr); //array($dist_id)
    }
  }  

}


function build_status_not_updated_table_rows($res){
  $row1 = array();
  if(!empty($res)){
    foreach ($res as $val) {
      if ($val['Bills_Count_Empty_Status'] != 0) {
        //$row1['trip_id'] = $val['runsheet_number'];
        //$row1['cashier_name'] = $val['fieldExecutiveName'];
        $row1['rs_name'] = $val['field_name_value'];
        $row1['bill_date'] = date('d-M-y', $val['bill_date']);
        $row1['total_bills_count'] = $val['Total_Bills_Count'];
        $row1['bill_count_empty_status'] = $val['Bills_Count_Empty_Status'];

        $rows1[] = $row1;
      }
    } 
  }
  else {
    $rows1[] = $row1;
  }
  return array('rows1'=>$rows1); //$output1;
}



//PL Group Wise Bill Pendency mailer first in form configuration
function send_email_pl_group_emailer_callback() {
  $receiver_email_str = variable_get('pl_grp_wise_to_email');
  $receiver_email_arr = explode(';', $receiver_email_str);
  foreach ($receiver_email_arr as $key => $value) {
    $dist_ids_str = $to_email_str = "";
    $individual_line_arr = $dist_ids_arr = array();

    if (strpos($value, ':')) {
      $individual_line_arr = explode(':', $value);
      $dist_ids_str = $individual_line_arr[0];
      $to_email_str = $individual_line_arr[1];
      $dist_ids_arr = explode(',', $dist_ids_str);
      if (count($dist_ids_arr) > 1) {
        send_email_pl_group_emailer($to_email_str, $dist_ids_arr);
      }
      else {
        $only_one_distributor_id = trim($dist_ids_arr[0]);
        send_email_pl_group_emailer($to_email_str, array($only_one_distributor_id));
      }
    }
    else {
      $active_distributors_id_arr = get_all_active_distributors_id();
      $receiver_email = $receiver_email_arr[0];
      send_email_pl_group_emailer($receiver_email, $active_distributors_id_arr); //array($dist_id)
    }
  }
}
//PL Group Wise Bill Pendency mailer first in form configuration



//Productivity Progress Status second in form configuration
/**
 *  Send Particular Email 
 * @param type $todays_date_date
 * @param type $distributor_name
 * @param type $end_date
 * @param type $chart
 * @param type $type
 * @param type $distributor_id
 * @return type
 */
function send_party_packingand_picking_productivity_emailer_callback() {
  $receiver_email_str = variable_get('productivity_status_to_email');
  $receiver_email_arr = explode(';', $receiver_email_str);
    foreach($receiver_email_arr as $key => $value){
    $dist_ids_str = $to_email_str = "";
    $individual_line_arr = $dist_ids_arr = array();
    if(strpos($value, ':')){
      $individual_line_arr = explode(':', $value);
      $dist_ids_str = $individual_line_arr[0];
      $to_email_str = $individual_line_arr[1];
      $dist_ids_arr = explode(',', $dist_ids_str);
      if(count($dist_ids_arr) > 1){
        send_party_packingand_picking_productivity_mailer($to_email_str, $dist_ids_arr);
      }else{
        $only_one_distributor_id = trim($dist_ids_arr[0]);
        send_party_packingand_picking_productivity_mailer($to_email_str, array($only_one_distributor_id));
      }
    }else{
      $active_distributors_id_arr = get_all_active_distributors_id();
      $receiver_email = $receiver_email_arr[0];
      send_party_packingand_picking_productivity_mailer($receiver_email, $active_distributors_id_arr); //array($dist_id)
    }  
  } 

}

//Productivity Progress Status second in form configuration

//Loading Productivity Progress Status third in form configuration
function send_loading_productivity_emailer() {
  $receiver_email_str = variable_get('loading_productivity_status_to_email');
  $receiver_email_arr = explode(';', $receiver_email_str);
    foreach($receiver_email_arr as $key => $value){
    $dist_ids_str = $to_email_str = "";
    $individual_line_arr = $dist_ids_arr = array();
    if(strpos($value, ':')){
      $individual_line_arr = explode(':', $value);
      $dist_ids_str = $individual_line_arr[0];
      $to_email_str = $individual_line_arr[1];
      $dist_ids_arr = explode(',', $dist_ids_str);
      if(count($dist_ids_arr) > 1){
        send_loading_productivity_mailer($to_email_str, $dist_ids_arr);
      }else{
        $only_one_distributor_id = trim($dist_ids_arr[0]);
        send_loading_productivity_mailer($to_email_str, array($only_one_distributor_id));
      }
    }else{
      $active_distributors_id_arr = get_all_active_distributors_id();
      $receiver_email = $receiver_email_arr[0];
      send_loading_productivity_mailer($receiver_email, $active_distributors_id_arr); //array($dist_id)
    }  
  }
}
//Loading Productivity Progress Status third in form configuration


//

/*
 * Pending OFD Sales Beat Wise fourth in form configuration
 */

function send_email_pending_ofd_salesbeatwise_data() {
  $receiver_email_str = variable_get('ofd_salesbeatwise_to_email');
  $receiver_email_arr = explode(';', $receiver_email_str);
  foreach ($receiver_email_arr as $key => $value) {
    $dist_ids_str = $to_email_str = "";
    $individual_line_arr = $dist_ids_arr = array();
    if (strpos($value, ':')) {
      $individual_line_arr = explode(':', $value);
      $dist_ids_str = $individual_line_arr[0];
      $to_email_str = $individual_line_arr[1];
      $dist_ids_arr = explode(',', $dist_ids_str);
      if (count($dist_ids_arr) > 1) {
        send_email_pending_ofd_salesbeatwise_data_mailer($to_email_str, $dist_ids_arr);
      }
      else {
        $only_one_distributor_id = trim($dist_ids_arr[0]);
        send_email_pending_ofd_salesbeatwise_data_mailer($to_email_str, array($only_one_distributor_id));
      }
    }
    else {
      $active_distributors_id_arr = get_all_active_distributors_id();
      $receiver_email = $receiver_email_arr[0];
      send_email_pending_ofd_salesbeatwise_data_mailer($receiver_email, $active_distributors_id_arr); //array($dist_id)
    }
  }
}

// Pending OFD Sales Beat Wise fourth in form configuration





/*
 * OFD Report fifth in form configuration
 */

//function send_email_ofd_report() {
//  $receiver_email_str = variable_get('ofd_to_email');
//  $receiver_email_arr = explode(';', $receiver_email_str);
//  foreach ($receiver_email_arr as $key => $value) {
//    $dist_ids_str = $to_email_str = "";
//    $individual_line_arr = $dist_ids_arr = array();
//    if (strpos($value, ':')) {
//      $individual_line_arr = explode(':', $value);
//      $dist_ids_str = $individual_line_arr[0];
//      $to_email_str = $individual_line_arr[1];
//      $dist_ids_arr = explode(',', $dist_ids_str);
//      if (count($dist_ids_arr) > 1) {
//        send_email_ofd_report_callback($to_email_str, $dist_ids_arr);
//      }
//      else {
//        $only_one_distributor_id = trim($dist_ids_arr[0]);
//        send_email_ofd_report_callback($to_email_str, array($only_one_distributor_id));
//      }
//    }
//    else {
//      $active_distributors_id_arr = get_all_active_distributors_id();
//      $receiver_email = $receiver_email_arr[0];
//      send_email_ofd_report_callback($receiver_email, $active_distributors_id_arr); //array($dist_id)
//    }
//  }
//}

/*
 * OFD Report
 */ 

//   Function to send auto mailer of Monthly Trip Summary  
function send_email_monthly_trip_summary() {
  $receiver_email_str = variable_get('monthly_trip_summary_to_email');
  $receiver_email_arr = explode(';', $receiver_email_str);
  foreach ($receiver_email_arr as $key => $value) {
    $dist_ids_str = $to_email_str = "";
    $individual_line_arr = $dist_ids_arr = array();
    if (strpos($value, ':')) {
      $individual_line_arr = explode(':', $value);
      $dist_ids_str = $individual_line_arr[0];
      $to_email_str = $individual_line_arr[1];
      $dist_ids_arr = explode(',', $dist_ids_str);
      if (count($dist_ids_arr) > 1) {
        send_email_monthly_trip_summary_callback($to_email_str, $dist_ids_arr);
      }
      else {
        $only_one_distributor_id = trim($dist_ids_arr[0]);
        send_email_monthly_trip_summary_callback($to_email_str, array($only_one_distributor_id));
      }
    }
    else {
      $active_distributors_id_arr = get_all_active_distributors_id();
      $receiver_email = $receiver_email_arr[0];
      send_email_monthly_trip_summary_callback($receiver_email, $active_distributors_id_arr); //array($dist_id)
    }
  }
}