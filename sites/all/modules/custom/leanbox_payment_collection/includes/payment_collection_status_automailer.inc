<?php

/**
 * File containing automailer for payment collection status
 */
/*
 * Function to send auto mailer of Payment Collection Status.
 */
function send_email_payment_collection_status() {
  $distributor_id = _DISTRIBUTOR_ID;
  $distributor_name = change_feeds_import_get_distributor_name($distributor_id);

  // Pending For Collection
  $pending_for_collection_html = get_email_pending_for_collection($distributor_id);

  // Pending For Assignment
  $pending_for_assignment_html = get_email_pending_for_assignment($distributor_id);

  // Pending For Upload
  $pending_for_upload_html = get_email_pending_for_upload($distributor_id);

  // Yesterday Assignment and Today's Collection
  $pending_for_yesterday_assignment_html = get_email_pending_for_yesterday_assignment($distributor_id);

  // Today's Petty Cash TRansaction And Closing Balance
  $today_petty_cash_transaction_html = get_email_today_petty_cash_transaction($distributor_id);

  $params['subject'] = "SB And Bounce Chq Status";
  $params['body'] = $pending_for_collection_html . "<br>" . $pending_for_assignment_html . "<br>" . $pending_for_upload_html . "<br>" . $pending_for_yesterday_assignment_html . "<br>" . $today_petty_cash_transaction_html;
  $mail_to = variable_get('collection_status_to_email');
  $from = variable_get('from_email');

  $file_name = 'Pending_Collection.csv';

  $file_uri = 'public://COLLECTION_STATUS/' . $file_name;

  // create directory if not exist
  $dir = 'public://COLLECTION_STATUS';
  file_prepare_directory($dir, FILE_CREATE_DIRECTORY);

  $file = fopen($file_uri, 'w');

  $table = get_attachment_pending_for_collection($distributor_id);
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

  drupal_mail('email_status', 'collection_status', $mail_to, LANGUAGE_NONE, $params, $from);
}

/*
 * Returns table of Pending for Collection
 */

function get_email_pending_for_collection($distributor_id) {
  $query = db_select('node', 'n');
  $query->condition('n.type', 'payment_collection');
  $query->condition('n.uid', $distributor_id, '=');
  $query->join('field_data_field_declaration_date', 'dd', 'dd.entity_id = n.nid');
  $query->join('field_data_field_flag', 'ff', 'ff.entity_id = n.nid');
  $query->join('field_data_field_collection_status', 'cs', 'cs.entity_id = n.nid');
  $query->condition('cs.field_collection_status_value', '2', '!=');
  $query->join('field_data_field_remaining_amt_collection', 'ra', 'ra.entity_id = n.nid');
  $query->leftjoin('field_data_field_previous_cash_amt', 'pc', 'pc.entity_id = n.nid');
  $query->leftjoin('field_data_field_previous_cheque_amt', 'pch', 'pch.entity_id = n.nid');

  // Want record from 1st oct onwards
  $timestamp = strtotime(date('2017-10-01')) - 1;
  $query->join('field_data_field_sales_bill_no_ref', 'sbrf', 'sbrf.entity_id = n.nid');
  $query->join('field_data_field_sr_bill_date', 'sr', 'sr.entity_id = sbrf.field_sales_bill_no_ref_nid');
  $query->condition('sr.field_sr_bill_date_value', $timestamp, '>');

  // Remove records where Closure Flag = 1
  $query->leftjoin('field_data_field_terminated_bill_flag', 'tf', 'tf.entity_id = n.nid');
  $query->condition('tf.field_terminated_bill_flag_value', '1', '!=');

  $query->fields('n', array('nid'));
  $query->fields('n', array('title'));
  $query->fields('dd', array('field_declaration_date_value'));
  $query->fields('ff', array('field_flag_value'));
  $query->fields('cs', array('field_collection_status_value'));
  $query->fields('ra', array('field_remaining_amt_collection_value'));
  $query->fields('pc', array('field_previous_cash_amt_value'));
  $query->fields('pch', array('field_previous_cheque_amt_value'));
  $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

  $signed_counter = 1;
  $bounced_counter = 1;
  $no_due_signed_counter = 1;
  $no_due_bounced_counter = 1;
  $decl_signed_counter = 1;
  $decl_bounced_counter = 1;
  $fivedays_decl_signed_counter = 1;
  $fivedays_decl_bounced_counter = 1;
  $twelvedays_decl_signed_counter = 1;
  $twelvedays_decl_bounced_counter = 1;
  $thirtyonedays_decl_signed_counter = 1;
  $thirtyonedays_decl_bounced_counter = 1;
  $rows = array();
  $row[] = array('Type', 'Count', 'Value', 'Count', 'Value', 'Count', 'Value', 'Count', 'Value', 'Count', 'Value', 'Count', 'Value');
  $decl_date = strtotime(date('Y-m-d'));
  $ten_days_before = strtotime('-10 day', strtotime(date('Y-m-d')));
  $fifteen_days_before = strtotime('-15 day', strtotime(date('Y-m-d')));
  $thirty_days_before = strtotime('-30 day', strtotime(date('Y-m-d')));


  foreach ($res as $data) {
    $query = db_select('node', 'n');
    $query->condition('n.type', 'sales_register_data');
    $query->condition('n.title', $data['title'], '=');
    $query->join('field_data_field_party_code_reference', 'pc', 'pc.entity_id = n.nid');
    $query->join('field_data_field_credit_days', 'ct', 'ct.entity_id = pc.field_party_code_reference_nid');
    $query->fields('ct', array('field_credit_days_value'));
    $credit_days = $query->execute()->fetchField();
      
    $collection_date = strtotime("+$credit_days day", strtotime($data['field_declaration_date_value']));
    if ($data['field_flag_value'] == 'signed') {
      $row['signed']['type'] = 'Signed';
      $row['signed']['count'] = $signed_counter++;
      $row['signed']['value'] += $data['field_remaining_amt_collection_value'] - ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
      // Collection date is greater than today's date will be no-due 
      if ($collection_date >= $decl_date) {
        $row['signed']['no_due_count'] = $no_due_signed_counter++;
        $row['signed']['no_due_value'] += $data['field_remaining_amt_collection_value'] - ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
      }
      else {
        $row['signed']['no_due_count'] += 0;
        $row['signed']['no_due_value'] += 0;
      }
      // Collection date is between last 1-10 Days
      if ($collection_date < $decl_date && $collection_date >= $ten_days_before) {
        $row['signed']['decl_count'] = $decl_signed_counter++;
        $row['signed']['decl_value'] += $data['field_remaining_amt_collection_value'] - ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
      }
      else {
        $row['signed']['decl_count'] += 0;
        $row['signed']['decl_value'] += 0;
      }
      // Collection date is between last 11-15 Days
      if ($collection_date < $ten_days_before && $collection_date >= $fifteen_days_before) {
        $row['signed']['5days_decl_count'] = $fivedays_decl_signed_counter++;
        $row['signed']['5days_decl_value'] += $data['field_remaining_amt_collection_value'] - ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
      }
      else {
        $row['signed']['5days_decl_count'] += 0;
        $row['signed']['5days_decl_value'] += 0;
      }
      // Collection date is between last 16-30 Days
      if ($collection_date < $fifteen_days_before && $collection_date >= $thirty_days_before) {
        $row['signed']['12days_decl_count'] = $twelvedays_decl_signed_counter++;
        $row['signed']['12days_decl_value'] += $data['field_remaining_amt_collection_value'] - ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
      }
      else {
        $row['signed']['12days_decl_count'] += 0;
        $row['signed']['12days_decl_value'] += 0;
      }
      // Collection date is below last 30 Days
      if ($collection_date < $thirty_days_before) {
        $row['signed']['31days_decl_count'] = $thirtyonedays_decl_signed_counter++;
        $row['signed']['31days_decl_value'] += $data['field_remaining_amt_collection_value'] - ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
      }
      else {
        $row['signed']['31days_decl_count'] += 0;
        $row['signed']['31days_decl_value'] += 0;
      }
    }
    else {
      $row['bounced']['type'] = 'Bounced';
      $row['bounced']['count'] = $bounced_counter++;
      $row['bounced']['value'] += $data['field_remaining_amt_collection_value'] - ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
      // Collection date is greater than today's date will be no-due 
      if ($collection_date >= $decl_date) {
        $row['bounced']['no_due_count'] = $no_due_bounced_counter++;
        $row['bounced']['no_due_value'] += $data['field_remaining_amt_collection_value'] - ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
      }
      else {
        $row['bounced']['no_due_count'] += 0;
        $row['bounced']['no_due_value'] += 0;
      }
      // Collection date is between last 1-10 Days
      if ($collection_date < $decl_date && $collection_date >= $ten_days_before) {
        $row['bounced']['decl_count'] = $decl_bounced_counter++;
        $row['bounced']['decl_value'] += $data['field_remaining_amt_collection_value'] - ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
      }
      else {
        $row['bounced']['decl_count'] += 0;
        $row['bounced']['decl_value'] += 0;
      }
      // Collection date is between last 11-15 Days
      if ($collection_date < $ten_days_before && $collection_date >= $fifteen_days_before) {
        $row['bounced']['5days_decl_count'] = $fivedays_decl_bounced_counter++;
        $row['bounced']['5days_decl_value'] += $data['field_remaining_amt_collection_value'] - ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
      }
      else {
        $row['bounced']['5days_decl_count'] += 0;
        $row['bounced']['5days_decl_value'] += 0;
      }
      // Collection date is between last 16-30 Days
      if ($collection_date < $fifteen_days_before && $collection_date >= $thirty_days_before) {
        $row['bounced']['12days_decl_count'] = $twelvedays_decl_bounced_counter++;
        $row['bounced']['12days_decl_value'] += $data['field_remaining_amt_collection_value'] - ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
      }
      else {
        $row['bounced']['12days_decl_count'] += 0;
        $row['bounced']['12days_decl_value'] += 0;
      }
      // Collection date is below last 30 Days
      if ($collection_date < $thirty_days_before) {
        $row['bounced']['31days_decl_count'] = $thirtyonedays_decl_bounced_counter++;
        $row['bounced']['31days_decl_value'] += $data['field_remaining_amt_collection_value'] - ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
      }
      else {
        $row['bounced']['31days_decl_count'] += 0;
        $row['bounced']['31days_decl_value'] += 0;
      }
    }
  }

  $total_till_date_count = $row['signed']['count'] + $row['bounced']['count'];
  $total_till_date_value = $row['signed']['value'] + $row['bounced']['value'];
  $total_no_due_count = $row['signed']['no_due_count'] + $row['bounced']['no_due_count'];
  $total_no_due_value = $row['signed']['no_due_value'] + $row['bounced']['no_due_value'];
  $total_decl_date_count = $row['signed']['decl_count'] + $row['bounced']['decl_count'];
  $total_decl_date_value = $row['signed']['decl_value'] + $row['bounced']['decl_value'];
  $total_5days_decl_count = $row['signed']['5days_decl_count'] + $row['bounced']['5days_decl_count'];
  $total_5days_decl_value = $row['signed']['5days_decl_value'] + $row['bounced']['5days_decl_value'];
  $total_12days_decl_count = $row['signed']['12days_decl_count'] + $row['bounced']['12days_decl_count'];
  $total_12days_decl_value = $row['signed']['12days_decl_value'] + $row['bounced']['12days_decl_value'];
  $total_31days_decl_count = $row['signed']['31days_decl_count'] + $row['bounced']['31days_decl_count'];
  $total_31days_decl_value = $row['signed']['31days_decl_value'] + $row['bounced']['31days_decl_value'];
  $row[] = array('Total', $total_till_date_count, $total_till_date_value, $total_no_due_count, $total_no_due_value, $total_decl_date_count, $total_decl_date_value, $total_5days_decl_count, $total_5days_decl_value, $total_12days_decl_count, $total_12days_decl_value, $total_31days_decl_count, $total_31days_decl_value);

  $header = array('', array('data' => 'Till Date', 'colspan' => 2), array('data' => 'No Due', 'colspan' => 2), array('data' => 'Due 1-10 Days from Credit Days', 'colspan' => 2), array('data' => 'Due 11-15 Days from Credit Days', 'colspan' => 2), array('data' => 'Due 16-30 Days from Credit Days', 'colspan' => 2), array('data' => 'Due >30 Days from Credit Days', 'colspan' => 2));
  $output = "<h3>Pending for Collection</h3>";
  $output .= "<h3>Due Date is Calculated as Declaration Date + Credit Days</h3>";
  $output .= theme('table', array('header' => $header, 'rows' => $row, 'attributes' => array('border' => '1', 'cellpadding' => '5', 'cellspacing' => '0')));

  return $output;
}

/*
 * Function to attach with automailer
 */

function get_attachment_pending_for_collection($distributor_id) {
  $query = db_select('node', 'n');
  $query->condition('n.type', 'payment_collection');
  $query->condition('n.uid', $distributor_id, '=');
  $query->join('field_data_field_declaration_date', 'dd', 'dd.entity_id = n.nid');
  $query->join('field_data_field_flag', 'ff', 'ff.entity_id = n.nid');
  $query->join('field_data_field_collection_status', 'cs', 'cs.entity_id = n.nid');
  $query->condition('cs.field_collection_status_value', '2', '!=');
  $query->join('field_data_field_remaining_amt_collection', 'ra', 'ra.entity_id = n.nid');
  $query->leftjoin('field_data_field_previous_cash_amt', 'pc', 'pc.entity_id = n.nid');
  $query->leftjoin('field_data_field_previous_cheque_amt', 'pch', 'pch.entity_id = n.nid');

  // Want record from 1st oct onwards
  $timestamp = strtotime(date('2017-10-01')) - 1;
  $query->join('field_data_field_sales_bill_no_ref', 'sbrf', 'sbrf.entity_id = n.nid');
  $query->join('field_data_field_sr_bill_date', 'sr', 'sr.entity_id = sbrf.field_sales_bill_no_ref_nid');
  $query->condition('sr.field_sr_bill_date_value', $timestamp, '>');

  // Remove records where Closure Flag = 1
  $query->leftjoin('field_data_field_terminated_bill_flag', 'tf', 'tf.entity_id = n.nid');
  $query->condition('tf.field_terminated_bill_flag_value', '1', '!=');

  $query->leftjoin('field_data_field_party_code_reference', 'pcr', 'pcr.entity_id = sbrf.field_sales_bill_no_ref_nid');
  $query->join('node', 'pp', 'pp.nid = pcr.field_party_code_reference_nid');
  
  $query->leftjoin('field_data_field_party_name', 'opn', 'opn.entity_id = pcr.field_party_code_reference_nid');
  $query->leftjoin('field_data_field_payment_terms', 'pt', 'pt.entity_id = pcr.field_party_code_reference_nid');
  $query->leftjoin('field_data_field_credit_days', 'ct', 'ct.entity_id = pcr.field_party_code_reference_nid');
  $query->leftjoin('field_data_field_beat_name', 'bn', 'bn.entity_id = pcr.field_party_code_reference_nid');
  $query->leftjoin('field_data_field_salesman', 'sm', 'sm.entity_id = pcr.field_party_code_reference_nid');
  $query->leftjoin('field_data_field_salesman_code', 'smc', 'smc.entity_id = pcr.field_party_code_reference_nid');

  $query->leftjoin('field_data_field_bill_value', 'bv', 'bv.entity_id = sbrf.field_sales_bill_no_ref_nid');

  $query->leftjoin('field_data_field_cheque_number', 'ocn', 'ocn.entity_id = n.nid');
  $query->leftjoin('field_data_field_cheque_date', 'cd', 'cd.entity_id = n.nid');
  $query->leftjoin('field_data_field_file_upload_date', 'fd', 'fd.entity_id = n.nid');
  $query->leftjoin('field_data_field_assigned_person', 'ap', 'ap.entity_id = n.nid');
  $query->leftjoin('field_data_field_assignment_date', 'ad', 'ad.entity_id = n.nid');
  $query->leftjoin('field_data_field_collection_date', 'lcd', 'lcd.entity_id = n.nid');
  $query->leftjoin('field_data_field_returned_bill_flag', 'rbf', 'rbf.entity_id = n.nid');
  $query->leftjoin('field_data_field_declaration_remarks', 'dr', 'dr.entity_id = n.nid');
  $query->leftjoin('field_data_field_return_remarks', 'rr', 'rr.entity_id = n.nid');
  $query->leftjoin('field_data_field_partial_collection_remarks', 'pr', 'pr.entity_id = n.nid');
  $query->leftjoin('field_data_field_fine_amount', 'fa', 'fa.entity_id = n.nid');
  $query->leftjoin('field_data_field_collection_closure_remarks', 'cr', 'cr.entity_id = n.nid');

  $query->fields('n', array('nid'));
  $query->fields('n', array('title'));
  $query->fields('sr', array('field_sr_bill_date_value'));
  $query->fields('ff', array('field_flag_value'));
  $query->fields('pp', array('title'));
  $query->fields('opn', array('field_party_name_value'));
  $query->fields('pt', array('field_payment_terms_value'));
  $query->fields('ct', array('field_credit_days_value'));
  $query->fields('bn', array('field_beat_name_value'));
  $query->fields('sm', array('field_salesman_value'));
  $query->fields('smc', array('field_salesman_code_value'));
  $query->fields('bv', array('field_bill_value_value'));
  $query->fields('ra', array('field_remaining_amt_collection_value'));
  $query->fields('dd', array('field_declaration_date_value'));
  $query->fields('ocn', array('field_cheque_number_value'));
  $query->fields('cd', array('field_cheque_date_value'));
  $query->fields('fd', array('field_file_upload_date_value'));
  $query->fields('ap', array('field_assigned_person_value'));
  $query->fields('ad', array('field_assignment_date_value'));
  $query->fields('cs', array('field_collection_status_value'));
  $query->fields('pc', array('field_previous_cash_amt_value'));
  $query->fields('pch', array('field_previous_cheque_amt_value'));
  $query->fields('lcd', array('field_collection_date_value'));
  $query->fields('rbf', array('field_returned_bill_flag_value'));
  $query->fields('dr', array('field_declaration_remarks_value'));
  $query->fields('rr', array('field_return_remarks_value'));
  $query->fields('pr', array('field_partial_collection_remarks_value'));
  $query->fields('fa', array('field_fine_amount_value'));
  $query->fields('tf', array('field_terminated_bill_flag_value'));
  $query->fields('cr', array('field_collection_closure_remarks_value'));
  $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

  $rows = array();
  $decl_date = strtotime(date('Y-m-d'));
  $ten_days_before = strtotime('-10 day', strtotime(date('Y-m-d')));
  $fifteen_days_before = strtotime('-15 day', strtotime(date('Y-m-d')));
  $thirty_days_before = strtotime('-30 day', strtotime(date('Y-m-d')));


  foreach ($res as $data) {
    $query = db_select('node', 'n');
    $query->condition('n.type', 'sales_register_data');
    $query->condition('n.title', $data['title'], '=');
    $query->join('field_data_field_party_code_reference', 'pc', 'pc.entity_id = n.nid');
    $query->join('field_data_field_credit_days', 'ct', 'ct.entity_id = pc.field_party_code_reference_nid');
    $query->fields('ct', array('field_credit_days_value'));
    $credit_days = $query->execute()->fetchField();
    
    $collection_date = strtotime("+$credit_days day", strtotime($data['field_declaration_date_value']));
    $declaration_date = strtotime($data['field_declaration_date_value']);
    $row['nid'] = $data['nid'];
    $row['bill_no'] = $data['title'];
    $row['bill_date'] = date('d-M-Y', $data['field_sr_bill_date_value']);
    $row['type'] = $data['field_flag_value'];
    $row['party_code'] = $data['pp_title'];
    $row['party_name'] = $data['field_party_name_value'];
    $row['payment_terms'] = $data['field_payment_terms_value'];
    $row['credit_days'] = $data['field_credit_days_value'];
    $row['beat_name'] = $data['field_beat_name_value'];
    $row['salesman'] = $data['field_salesman_value'];
    $row['salesman_code'] = $data['field_salesman_code_value'];
    $row['bill_value'] = $data['field_bill_value_value'];
    $row['to_be_collected'] = $data['field_remaining_amt_collection_value'];
    $row['declaration_date'] = date('d-M-Y', $declaration_date);
    $row['old_chq_no'] = $data['field_cheque_number_value'];
    if (isset($data['field_cheque_date_value'])) {
      $old_chq_date = strtotime($data['field_cheque_date_value']);
      $row['old_chq_date'] = date('d-M-Y', $old_chq_date);
    }
    else {
      $row['old_chq_date'] = '';
    }
    if (isset($data['field_file_upload_date_value'])) {
      $file_upload_date = strtotime($data['field_file_upload_date_value']);
      $row['file_upload_date'] = date('d-M-Y', $file_upload_date);
    }
    else {
      $row['file_upload_date'] = '';
    }
    $row['assigned_to'] = $data['field_assigned_person_value'];
    if (isset($data['field_assignment_date_value'])) {
      $assigned_date = strtotime($data['field_assignment_date_value']);
      $row['assigned_date'] = date('d-M-Y', $assigned_date);
    }
    else {
      $row['assigned_date'] = '';
    }
    if ($data['field_collection_status_value'] == 0) {
      $row['collection_status'] = 'Not Collected';
    }
    else if ($data['field_collection_status_value'] == 1) {
      $row['collection_status'] = 'Partially Collected';
    }
    else if ($data['field_collection_status_value'] == 2) {
      $row['collection_status'] = 'Fully Collected';
    }
    $row['cash_collected_amt'] = $data['field_previous_cash_amt_value'];
    $row['chq_collected_amt'] = $data['field_previous_cheque_amt_value'];
    $row['total_amt'] = ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
    $row['difference_amt'] = $data['field_remaining_amt_collection_value'] - ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
    if (isset($data['field_collection_date_value'])) {
      $latest_collection_date = strtotime($data['field_collection_date_value']);
      $row['latest_collection_date'] = date('d-M-Y', $latest_collection_date);
    }
    else {
      $row['latest_collection_date'] = '';
    }
    $row['returned_flag'] = $data['field_returned_bill_flag_value'];
    $row['dec_remarks'] = $data['field_declaration_remarks_value'];
    $row['return_remarks'] = $data['field_return_remarks_value'];
    $row['partial_coll_remarks'] = $data['field_partial_collection_remarks_value'];
    $row['fine_amt'] = $data['field_fine_amount_value'];
    $row['final_closure'] = $data['field_terminated_bill_flag_value'];
    $row['final_closure_remarks'] = $data['field_collection_closure_remarks_value'];
    
    // Pending Days From Collection Days
    if ($collection_date >= $decl_date) {
      $row['ageing'] = 'No Due';
    }
    // Pending Days From 1-10 Days Collection Days
    if ($collection_date < $decl_date && $collection_date >= $ten_days_before) {
      $row['ageing'] = '1-10 Days';
    }
    // Pending Days From 11-15 Days Collection Days
    if ($collection_date < $ten_days_before && $collection_date >= $fifteen_days_before) {
      $row['ageing'] = '11-15 Days';
    }
    // Pending Days From 16-30 Days Collection Days
    if ($collection_date < $fifteen_days_before && $collection_date >= $thirty_days_before) {
      $row['ageing'] = '16-30 Days';
    }
    // Pending Days below 30 Days Collection Days
    if ($collection_date < $thirty_days_before) {
      $row['ageing'] = '>30 Days';
    }
    $rows[] = $row;
  }

  $header = array('Nid', 'Bill No.', 'Bill Date', 'Type', 'Party Code', 'Party Name', 'PT', 'CT', 'Beat Name', 'Salesman', 'Salesman Code', 'Bill Value', 'To Be Collected', 'Declaration Date', 'Old Cheque No.', 'Old Cheque Date', 'File Upload Date', 'Assigned To', 'Assignment Date', 'Collection Status', 'Cash Collected Amt', 'Cheque Collected Amt', 'Total Amt Collected', 'Difference Amt', 'Latest Collection Date', 'Returned Bill Flag', 'Declaration Remarks', 'Return Remarks', 'Partial Collection Remarks', 'Fine Amt', 'Final Closure', 'Final Closure Remarks', 'Ageing');

  $output1['header'] = $header;
  $output1['rows'] = $rows;

  return $output1;
}

/*
 * Returns table of Pending for Assignment
 */

function get_email_pending_for_assignment($distributor_id) {
  $todays_date = date('Y-m-d');
  $forty_five_days_before = date('Y-m-d', strtotime('-45 day', strtotime(date('Y-m-d'))));

  $query = db_select('node', 'n');
  $query->condition('n.type', 'payment_collection');
  $query->condition('n.uid', $distributor_id, '=');
  $query->join('field_data_field_declaration_date', 'dd', 'dd.entity_id = n.nid');
  $query->condition('dd.field_declaration_date_value', array($forty_five_days_before, $todays_date), 'BETWEEN');
  $query->join('field_data_field_flag', 'ff', 'ff.entity_id = n.nid');
  $query->join('field_data_field_remaining_amt_collection', 'ra', 'ra.entity_id = n.nid');
  $query->leftjoin('field_data_field_assignment_date', 'ad', 'ad.entity_id = n.nid');
  $query->isNull('ad.field_assignment_date_value');
  $query->leftjoin('field_data_field_previous_cash_amt', 'pc', 'pc.entity_id = n.nid');
  $query->leftjoin('field_data_field_previous_cheque_amt', 'pch', 'pch.entity_id = n.nid');

  // Want records from 1st oct onwards
  $timestamp = strtotime(date('2017-10-01')) - 1;
  $query->join('field_data_field_sales_bill_no_ref', 'sbrf', 'sbrf.entity_id = n.nid');
  $query->join('field_data_field_sr_bill_date', 'sr', 'sr.entity_id = sbrf.field_sales_bill_no_ref_nid');
  $query->condition('sr.field_sr_bill_date_value', $timestamp, '>');

  // Remove records where Closure Flag = 1
  $query->leftjoin('field_data_field_terminated_bill_flag', 'tf', 'tf.entity_id = n.nid');
  $query->condition('tf.field_terminated_bill_flag_value', '1', '!=');

  // Remove records where status status if Fully Collected
  $query->leftjoin('field_data_field_collection_status', 'cs', 'cs.entity_id = n.nid');
  $query->condition('cs.field_collection_status_value', '2', '!=');

  $query->fields('n', array('nid'));
  $query->fields('n', array('title'));
  $query->fields('dd', array('field_declaration_date_value'));
  $query->fields('ff', array('field_flag_value'));
  $query->fields('ra', array('field_remaining_amt_collection_value'));
  $query->fields('ad', array('field_assignment_date_value'));
  $query->fields('pc', array('field_previous_cash_amt_value'));
  $query->fields('pch', array('field_previous_cheque_amt_value'));
  $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

  $signed_counter = 1;
  $bounced_counter = 1;
  $no_due_signed_counter = 1;
  $no_due_bounced_counter = 1;
  $decl_signed_counter = 1;
  $decl_bounced_counter = 1;
  $fivedays_decl_signed_counter = 1;
  $fivedays_decl_bounced_counter = 1;
  $twelvedays_decl_signed_counter = 1;
  $twelvedays_decl_bounced_counter = 1;
  $thirtyonedays_decl_signed_counter = 1;
  $thirtyonedays_decl_bounced_counter = 1;
  $rows = array();
  $row[] = array('Type', 'Count', 'Value', 'Count', 'Value', 'Count', 'Value', 'Count', 'Value', 'Count', 'Value', 'Count', 'Value');
  $decl_date = strtotime(date('Y-m-d'));
  $ten_days_before = strtotime('-10 day', strtotime(date('Y-m-d')));
  $fifteen_days_before = strtotime('-15 day', strtotime(date('Y-m-d')));
  $thirty_days_before = strtotime('-30 day', strtotime(date('Y-m-d')));

  foreach ($res as $data) {
    $query = db_select('node', 'n');
    $query->condition('n.type', 'sales_register_data');
    $query->condition('n.title', $data['title'], '=');
    $query->join('field_data_field_party_code_reference', 'pc', 'pc.entity_id = n.nid');
    $query->join('field_data_field_credit_days', 'ct', 'ct.entity_id = pc.field_party_code_reference_nid');
    $query->fields('ct', array('field_credit_days_value'));
    $credit_days = $query->execute()->fetchField();
    
    $collection_date = strtotime("+$credit_days day", strtotime($data['field_declaration_date_value']));
    if ($data['field_flag_value'] == 'signed') {
      $row['signed']['type'] = 'Signed';
      $row['signed']['count'] = $signed_counter++;
      $row['signed']['value'] += $data['field_remaining_amt_collection_value'] - ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
      // Collection date is greater than today's date will be no-due 
      if ($collection_date >= $decl_date) {
        $row['signed']['no_due_count'] = $no_due_signed_counter++;
        $row['signed']['no_due_value'] += $data['field_remaining_amt_collection_value'] - ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
      }
      else {
        $row['signed']['no_due_count'] += 0;
        $row['signed']['no_due_value'] += 0;
      }
      // Collection date is between last 1-10 Days
      if ($collection_date < $decl_date && $collection_date >= $ten_days_before) {
        $row['signed']['decl_count'] = $decl_signed_counter++;
        $row['signed']['decl_value'] += $data['field_remaining_amt_collection_value'] - ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
      }
      else {
        $row['signed']['decl_count'] += 0;
        $row['signed']['decl_value'] += 0;
      }
      // Collection date is between last 11-15 Days
      if ($collection_date < $ten_days_before && $collection_date >= $fifteen_days_before) {
        $row['signed']['5days_decl_count'] = $fivedays_decl_signed_counter++;
        $row['signed']['5days_decl_value'] += $data['field_remaining_amt_collection_value'] - ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
      }
      else {
        $row['signed']['5days_decl_count'] += 0;
        $row['signed']['5days_decl_value'] += 0;
      }
      // Collection date is between last 16-30 Days
      if ($collection_date < $fifteen_days_before && $collection_date >= $thirty_days_before) {
        $row['signed']['12days_decl_count'] = $twelvedays_decl_signed_counter++;
        $row['signed']['12days_decl_value'] += $data['field_remaining_amt_collection_value'] - ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
      }
      else {
        $row['signed']['12days_decl_count'] += 0;
        $row['signed']['12days_decl_value'] += 0;
      }
      // Collection date is below last 30 Days
      if ($collection_date < $thirty_days_before) {
        $row['signed']['31days_decl_count'] = $thirtyonedays_decl_signed_counter++;
        $row['signed']['31days_decl_value'] += $data['field_remaining_amt_collection_value'] - ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
      }
      else {
        $row['signed']['31days_decl_count'] += 0;
        $row['signed']['31days_decl_value'] += 0;
      }
    }
    else {
      $row['bounced']['type'] = 'Bounced';
      $row['bounced']['count'] = $bounced_counter++;
      $row['bounced']['value'] += $data['field_remaining_amt_collection_value'] - ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
      // Collection date is greater than today's date will be no-due 
      if ($collection_date >= $decl_date) {
        $row['bounced']['no_due_count'] = $no_due_bounced_counter++;
        $row['bounced']['no_due_value'] += $data['field_remaining_amt_collection_value'] - ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
      }
      else {
        $row['bounced']['no_due_count'] += 0;
        $row['bounced']['no_due_value'] += 0;
      }
      // Collection date is between last 1-10 Days
      if ($declaration_date < $decl_date && $declaration_date >= $ten_days_before) {
        $row['bounced']['decl_count'] = $decl_bounced_counter++;
        $row['bounced']['decl_value'] += $data['field_remaining_amt_collection_value'] - ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
      }
      else {
        $row['bounced']['decl_count'] += 0;
        $row['bounced']['decl_value'] += 0;
      }
      // Collection date is between last 11-15 Days
      if ($declaration_date < $ten_days_before && $declaration_date >= $fifteen_days_before) {
        $row['bounced']['5days_decl_count'] = $fivedays_decl_bounced_counter++;
        $row['bounced']['5days_decl_value'] += $data['field_remaining_amt_collection_value'] - ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
      }
      else {
        $row['bounced']['5days_decl_count'] += 0;
        $row['bounced']['5days_decl_value'] += 0;
      }
      // Collection date is between last 16-30 Days
      if ($declaration_date < $fifteen_days_before && $declaration_date >= $thirty_days_before) {
        $row['bounced']['12days_decl_count'] = $twelvedays_decl_bounced_counter++;
        $row['bounced']['12days_decl_value'] += $data['field_remaining_amt_collection_value'] - ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
      }
      else {
        $row['bounced']['12days_decl_count'] += 0;
        $row['bounced']['12days_decl_value'] += 0;
      }
      // Collection date is below last 30 Days
      if ($declaration_date < $thirty_days_before) {
        $row['bounced']['31days_decl_count'] = $thirtyonedays_decl_bounced_counter++;
        $row['bounced']['31days_decl_value'] += $data['field_remaining_amt_collection_value'] - ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
      }
      else {
        $row['bounced']['31days_decl_count'] += 0;
        $row['bounced']['31days_decl_value'] += 0;
      }
    }
  }

  $total_till_date_count = $row['signed']['count'] + $row['bounced']['count'];
  $total_till_date_value = $row['signed']['value'] + $row['bounced']['value'];
  $total_no_due_count = $row['signed']['no_due_count'] + $row['bounced']['no_due_count'];
  $total_no_due_value = $row['signed']['no_due_value'] + $row['bounced']['no_due_value'];
  $total_decl_date_count = $row['signed']['decl_count'] + $row['bounced']['decl_count'];
  $total_decl_date_value = $row['signed']['decl_value'] + $row['bounced']['decl_value'];
  $total_5days_decl_count = $row['signed']['5days_decl_count'] + $row['bounced']['5days_decl_count'];
  $total_5days_decl_value = $row['signed']['5days_decl_value'] + $row['bounced']['5days_decl_value'];
  $total_12days_decl_count = $row['signed']['12days_decl_count'] + $row['bounced']['12days_decl_count'];
  $total_12days_decl_value = $row['signed']['12days_decl_value'] + $row['bounced']['12days_decl_value'];
  $total_31days_decl_count = $row['signed']['31days_decl_count'] + $row['bounced']['31days_decl_count'];
  $total_31days_decl_value = $row['signed']['31days_decl_value'] + $row['bounced']['31days_decl_value'];
  $row[] = array('Total', $total_till_date_count, $total_till_date_value, $total_no_due_count, $total_no_due_value, $total_decl_date_count, $total_decl_date_value, $total_5days_decl_count, $total_5days_decl_value, $total_12days_decl_count, $total_12days_decl_value, $total_31days_decl_count, $total_31days_decl_value);

  $header = array('', array('data' => 'Till Date', 'colspan' => 2), array('data' => 'No Due', 'colspan' => 2), array('data' => 'Due 1-10 Days from Credit Days', 'colspan' => 2), array('data' => 'Due 11-15 Days from Credit Days', 'colspan' => 2), array('data' => 'Due 16-30 Days from Credit Days', 'colspan' => 2), array('data' => 'Due >30 Days from Credit Days', 'colspan' => 2));
  $output = "<h3>Pending for Assignment(For Last 45 days from Declaration Date)Excluding Fully Collected or Forced Closed Cases</h3>";
  $output .= "<h3>Due Date is Calculated as Declaration Date + Credit Days</h3>";
  $output .= theme('table', array('header' => $header, 'rows' => $row, 'attributes' => array('border' => '1', 'cellpadding' => '5', 'cellspacing' => '0')));

  return $output;
}

/*
 * Returns table of Pending for Upload
 */

function get_email_pending_for_upload($distributor_id) {
  $todays_date = date('Y-m-d');
  $forty_five_days_before = date('Y-m-d', strtotime('-45 day', strtotime(date('Y-m-d'))));

  $query = db_select('node', 'n');
  $query->condition('n.type', 'payment_collection');
  $query->condition('n.uid', $distributor_id, '=');
  $query->join('field_data_field_declaration_date', 'dd', 'dd.entity_id = n.nid');
  $query->condition('dd.field_declaration_date_value', array($forty_five_days_before, $todays_date), 'BETWEEN');
  $query->join('field_data_field_flag', 'ff', 'ff.entity_id = n.nid');
  $query->join('field_data_field_remaining_amt_collection', 'ra', 'ra.entity_id = n.nid');
  $query->leftjoin('field_data_field_file_upload_date', 'ud', 'ud.entity_id = n.nid');
  $query->isNull('ud.field_file_upload_date_value');
  $query->leftjoin('field_data_field_previous_cash_amt', 'pc', 'pc.entity_id = n.nid');
  $query->leftjoin('field_data_field_previous_cheque_amt', 'pch', 'pch.entity_id = n.nid');

  // Want records from 1st oct onwards
  $timestamp = strtotime(date('2017-10-01')) - 1;
  $query->join('field_data_field_sales_bill_no_ref', 'sbrf', 'sbrf.entity_id = n.nid');
  $query->join('field_data_field_sr_bill_date', 'sr', 'sr.entity_id = sbrf.field_sales_bill_no_ref_nid');
  $query->condition('sr.field_sr_bill_date_value', $timestamp, '>');

  // Remove records where Closure Flag = 1
  $query->leftjoin('field_data_field_terminated_bill_flag', 'tf', 'tf.entity_id = n.nid');
  $query->condition('tf.field_terminated_bill_flag_value', '1', '!=');

  // Remove records where status status if Fully Collected
  $query->leftjoin('field_data_field_collection_status', 'cs', 'cs.entity_id = n.nid');
  $query->condition('cs.field_collection_status_value', '2', '!=');

  $query->fields('n', array('nid'));
  $query->fields('dd', array('field_declaration_date_value'));
  $query->fields('ff', array('field_flag_value'));
  $query->fields('ra', array('field_remaining_amt_collection_value'));
  $query->fields('ud', array('field_file_upload_date_value'));
  $query->fields('pc', array('field_previous_cash_amt_value'));
  $query->fields('pch', array('field_previous_cheque_amt_value'));
  $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

  $signed_counter = 1;
  $bounced_counter = 1;
  $decl_signed_counter = 1;
  $decl_bounced_counter = 1;
  $rows = array();
  $row[] = array('Type', 'Count', 'Value', 'Count', 'Value');
  $decl_date = strtotime('-2 day', strtotime(date('Y-m-d')));

  foreach ($res as $data) {
    if ($data['field_flag_value'] == 'signed') {
      $row['signed']['type'] = 'Signed';
      $row['signed']['count'] = $signed_counter++;
      $row['signed']['value'] += $data['field_remaining_amt_collection_value'] - ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
      if (strtotime($data['field_declaration_date_value']) <= $decl_date) {
        $row['signed']['decl_count'] = $decl_signed_counter++;
        $row['signed']['decl_value'] += $data['field_remaining_amt_collection_value'] - ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
      }
      else {
        $row['signed']['decl_count'] += 0;
        $row['signed']['decl_value'] += 0;
      }
    }
    else {
      $row['bounced']['type'] = 'Bounced';
      $row['bounced']['count'] = $bounced_counter++;
      $row['bounced']['value'] += $data['field_remaining_amt_collection_value'] - ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
      if (strtotime($data['field_declaration_date_value']) <= $decl_date) {
        $row['bounced']['decl_count'] = $decl_bounced_counter++;
        $row['bounced']['decl_value'] += $data['field_remaining_amt_collection_value'] - ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
      }
      else {
        $row['bounced']['decl_count'] += 0;
        $row['bounced']['decl_value'] += 0;
      }
    }
  }

  $total_till_date_count = $row['signed']['count'] + $row['bounced']['count'];
  $total_till_date_value = $row['signed']['value'] + $row['bounced']['value'];
  $total_decl_date_count = $row['signed']['decl_count'] + $row['bounced']['decl_count'];
  $total_decl_date_value = $row['signed']['decl_value'] + $row['bounced']['decl_value'];
  $row[] = array('Total', $total_till_date_count, $total_till_date_value, $total_decl_date_count, $total_decl_date_value);

  $header = array('', array('data' => 'Till Date', 'colspan' => 2), array('data' => '>2Days from Declaration Date', 'colspan' => 2));
  $output = "<h3>Pending for Upload(For Last 45 days from Declaration Date)Excluding Fully Collected or Forced Closed Cases</h3>";
  $output .= theme('table', array('header' => $header, 'rows' => $row, 'attributes' => array('border' => '1', 'cellpadding' => '5', 'cellspacing' => '0')));

  return $output;
}

function pending_for_yesterday_assignment($distributor_id) {
  // for MTD LOGIC
  $todays_date = date('Y-m-d');
  $todays_date_timestamp = strtotime($todays_date);
  $current_day = date("d", $todays_date_timestamp);
  if ($current_day <= '21') {
    $old_date = strtotime('-1 month', strtotime($todays_date));
    $last_month = date("m", $old_date);
    $last_year = date("Y", $old_date);
    $from_this_date = date("$last_year-$last_month-21");
  }
  else {
    $this_month = date("m", $todays_date_timestamp);
    $from_this_date = date("Y-$this_month-21");
  }

  // Query For Yesterday Assignment
  $assignment_date = strtotime('-1 day', strtotime(date('Y-m-d')));
  $yesterday_date = date('Y-m-d', $assignment_date);

  $query = db_select('node', 'n');
  $query->condition('n.type', 'payment_collection');
  $query->condition('n.uid', $distributor_id, '=');
  $query->join('field_data_field_flag', 'ff', 'ff.entity_id = n.nid');
  $query->join('field_data_field_remaining_amt_collection', 'ra', 'ra.entity_id = n.nid');
  $query->leftjoin('field_data_field_assignment_date', 'ad', 'ad.entity_id = n.nid');
  $query->condition('ad.field_assignment_date_value', $yesterday_date, '=');
  $query->leftjoin('field_data_field_assigned_person', 'ap', 'ap.entity_id = n.nid');

  $query->leftjoin('field_data_field_previous_cash_amt', 'pc', 'pc.entity_id = n.nid');
  $query->leftjoin('field_data_field_previous_cheque_amt', 'pch', 'pch.entity_id = n.nid');

  // Remove records where Closure Flag = 1
  $query->leftjoin('field_data_field_terminated_bill_flag', 'tf', 'tf.entity_id = n.nid');
  $query->condition('tf.field_terminated_bill_flag_value', '1', '!=');

  $query->groupBy('ap.field_assigned_person_value');
  $query->groupBy('ff.field_flag_value');

  $query->addExpression('COUNT(n.title)', 'bill_count');
  $query->addExpression('SUM(ra.field_remaining_amt_collection_value)', 'bill_value');
  $query->fields('n', array('nid'));
  $query->fields('ff', array('field_flag_value'));
  $query->fields('ra', array('field_remaining_amt_collection_value'));
  $query->fields('ap', array('field_assigned_person_value'));
  $query->fields('ad', array('field_assignment_date_value'));
  $query->fields('pc', array('field_previous_cash_amt_value'));
  $query->fields('pch', array('field_previous_cheque_amt_value'));
  $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

  $row = array();
  $row[] = array('Type', 'Assigned To', 'Count', 'Value', 'Count', 'Value', 'Count', 'Value', 'Count', 'Value');
  foreach ($res as $data) {
    $person_signed = $data['field_assigned_person_value'] . '-' . 'signed';
    $person_bounced = $data['field_assigned_person_value'] . '-' . 'bounced';
    if ($data['field_flag_value'] == 'signed') {
      $row[$person_signed]['type'] = 'Signed';
      $row[$person_signed]['assigned_to'] = $data['field_assigned_person_value'];
      $row[$person_signed]['bill_count'] = $data['bill_count'];
      $row[$person_signed]['bill_value'] = $data['bill_value'] - ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
    }
    else {
      $row[$person_bounced]['type'] = 'Bounced';
      $row[$person_bounced]['assigned_to'] = $data['field_assigned_person_value'];
      $row[$person_bounced]['bill_count'] = $data['bill_count'];
      $row[$person_bounced]['bill_value'] = $data['bill_value'] - ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
    }
  }
  // MTD Query For Assignment
  $query = db_select('node', 'n');
  $query->condition('n.type', 'payment_collection');
  $query->condition('n.uid', $distributor_id, '=');
  $query->join('field_data_field_flag', 'ff', 'ff.entity_id = n.nid');
  $query->join('field_data_field_remaining_amt_collection', 'ra', 'ra.entity_id = n.nid');
  $query->leftjoin('field_data_field_assignment_date', 'ad', 'ad.entity_id = n.nid');
  $query->condition('ad.field_assignment_date_value', array($from_this_date, $yesterday_date), 'BETWEEN');
  $query->leftjoin('field_data_field_assigned_person', 'ap', 'ap.entity_id = n.nid');

  $query->leftjoin('field_data_field_previous_cash_amt', 'pc', 'pc.entity_id = n.nid');
  $query->leftjoin('field_data_field_previous_cheque_amt', 'pch', 'pch.entity_id = n.nid');

  // Remove records where Closure Flag = 1
  $query->leftjoin('field_data_field_terminated_bill_flag', 'tf', 'tf.entity_id = n.nid');
  $query->condition('tf.field_terminated_bill_flag_value', '1', '!=');

  $query->groupBy('ap.field_assigned_person_value');
  $query->groupBy('ff.field_flag_value');

  $query->addExpression('COUNT(n.title)', 'bill_count');
  $query->addExpression('SUM(ra.field_remaining_amt_collection_value)', 'bill_value');
  $query->fields('n', array('nid'));
  $query->fields('ff', array('field_flag_value'));
  $query->fields('ra', array('field_remaining_amt_collection_value'));
  $query->fields('ap', array('field_assigned_person_value'));
  $query->fields('ad', array('field_assignment_date_value'));
  $query->fields('pc', array('field_previous_cash_amt_value'));
  $query->fields('pch', array('field_previous_cheque_amt_value'));
  $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);


  foreach ($res as $data) {
    $person_signed = $data['field_assigned_person_value'] . '-' . 'signed';
    $person_bounced = $data['field_assigned_person_value'] . '-' . 'bounced';
    if ($data['field_flag_value'] == 'signed') {
      $row[$person_signed]['type'] = 'Signed';
      $row[$person_signed]['assigned_to'] = $data['field_assigned_person_value'];
      $row[$person_signed]['mtd_bill_count'] = $data['bill_count'];
      $row[$person_signed]['mtd_bill_value'] = $data['bill_value'];
    }
    else {
      $row[$person_bounced]['type'] = 'Bounced';
      $row[$person_bounced]['assigned_to'] = $data['field_assigned_person_value'];
      $row[$person_bounced]['mtd_bill_count'] = $data['bill_count'];
      $row[$person_bounced]['mtd_bill_value'] = $data['bill_value'];
    }
  }


  // Query For Today's Collection
  $collection_date = strtotime(date('Y-m-d'));
  $todaydate = date('Y-m-d', $collection_date);

  $query = db_select('node', 'n');
  $query->condition('n.type', 'payment_collection');
  $query->condition('n.uid', $distributor_id, '=');
  $query->join('field_data_field_flag', 'ff', 'ff.entity_id = n.nid');
  $query->join('field_data_field_remaining_amt_collection', 'ra', 'ra.entity_id = n.nid');
  $query->leftjoin('field_data_field_collection_date', 'cd', 'cd.entity_id = n.nid');
  $query->condition('cd.field_collection_date_value', $todaydate, '=');
  $query->leftjoin('field_data_field_assigned_person', 'ap', 'ap.entity_id = n.nid');

  $query->leftjoin('field_data_field_previous_cash_amt', 'pc', 'pc.entity_id = n.nid');
  $query->leftjoin('field_data_field_previous_cheque_amt', 'pch', 'pch.entity_id = n.nid');

  // Remove records where Closure Flag = 1
  $query->leftjoin('field_data_field_terminated_bill_flag', 'tf', 'tf.entity_id = n.nid');
  $query->condition('tf.field_terminated_bill_flag_value', '1', '!=');

  $query->groupBy('ap.field_assigned_person_value');
  $query->groupBy('ff.field_flag_value');

  $query->addExpression('COUNT(n.title)', 'bill_count');
  $query->addExpression('SUM(ra.field_remaining_amt_collection_value)', 'bill_value');
  $query->fields('n', array('nid'));
  $query->fields('ff', array('field_flag_value'));
  $query->fields('ra', array('field_remaining_amt_collection_value'));
  $query->fields('ap', array('field_assigned_person_value'));
  $query->fields('cd', array('field_collection_date_value'));
  $query->fields('pc', array('field_previous_cash_amt_value'));
  $query->fields('pch', array('field_previous_cheque_amt_value'));
  $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

  foreach ($res as $data) {
    $person_signed = $data['field_assigned_person_value'] . '-' . 'signed';
    $person_bounced = $data['field_assigned_person_value'] . '-' . 'bounced';
    if ($data['field_flag_value'] == 'signed') {
      $row[$person_signed]['type'] = 'Signed';
      $row[$person_signed]['assigned_to'] = $data['field_assigned_person_value'];
      $row[$person_signed]['bill_counts'] = $data['bill_count'];
      $row[$person_signed]['bill_values'] = $data['bill_value'] - ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
    }
    else {
      $row[$person_bounced]['type'] = 'Bounced';
      $row[$person_bounced]['assigned_to'] = $data['field_assigned_person_value'];
      $row[$person_bounced]['bill_counts'] = $data['bill_count'];
      $row[$person_bounced]['bill_values'] = $data['bill_value'] - ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
    }
  }

  // MTD Query For Today's Collection

  $query = db_select('node', 'n');
  $query->condition('n.type', 'payment_collection');
  $query->condition('n.uid', $distributor_id, '=');
  $query->join('field_data_field_flag', 'ff', 'ff.entity_id = n.nid');
  $query->join('field_data_field_remaining_amt_collection', 'ra', 'ra.entity_id = n.nid');
  //$query->leftjoin('field_data_field_collection_date', 'cd', 'cd.entity_id = n.nid');
  //$query->condition('cd.field_collection_date_value', array($from_this_date, $date), 'BETWEEN');
  $query->leftjoin('field_data_field_assignment_date', 'ad', 'ad.entity_id = n.nid');
  $query->condition('ad.field_assignment_date_value', array($from_this_date, $yesterday_date), 'BETWEEN');
  $query->leftjoin('field_data_field_assigned_person', 'ap', 'ap.entity_id = n.nid');
  $query->leftjoin('field_data_field_collection_status', 'cs', 'cs.entity_id = n.nid');
  $query->condition('cs.field_collection_status_value', '0', '!=');

  $query->leftjoin('field_data_field_previous_cash_amt', 'pc', 'pc.entity_id = n.nid');
  $query->leftjoin('field_data_field_previous_cheque_amt', 'pch', 'pch.entity_id = n.nid');

  // Remove records where Closure Flag = 1
  $query->leftjoin('field_data_field_terminated_bill_flag', 'tf', 'tf.entity_id = n.nid');
  $query->condition('tf.field_terminated_bill_flag_value', '1', '!=');

  $query->groupBy('ap.field_assigned_person_value');
  $query->groupBy('ff.field_flag_value');

  $query->addExpression('COUNT(n.title)', 'bill_count');
  $query->addExpression('SUM(pc.field_previous_cash_amt_value + pch.field_previous_cheque_amt_value)', 'bill_value');
  $query->fields('n', array('nid'));
  $query->fields('ff', array('field_flag_value'));
  $query->fields('ra', array('field_remaining_amt_collection_value'));
  $query->fields('ap', array('field_assigned_person_value'));
  //$query->fields('cd', array('field_collection_date_value'));
  $query->fields('ad', array('field_assignment_date_value'));
  $query->fields('pc', array('field_previous_cash_amt_value'));
  $query->fields('pch', array('field_previous_cheque_amt_value'));
  $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);


  foreach ($res as $data) {
    $person_signed = $data['field_assigned_person_value'] . '-' . 'signed';
    $person_bounced = $data['field_assigned_person_value'] . '-' . 'bounced';
    if ($data['field_flag_value'] == 'signed') {
      $row[$person_signed]['type'] = 'Signed';
      $row[$person_signed]['assigned_to'] = $data['field_assigned_person_value'];
      $row[$person_signed]['mtd_bill_counts'] = $data['bill_count'];
      $row[$person_signed]['mtd_bill_values'] = $data['bill_value'];
    }
    else {
      $row[$person_bounced]['type'] = 'Bounced';
      $row[$person_bounced]['assigned_to'] = $data['field_assigned_person_value'];
      $row[$person_bounced]['mtd_bill_counts'] = $data['bill_count'];
      $row[$person_bounced]['mtd_bill_values'] = $data['bill_value'];
    }
  }

  $final_row = array();
  foreach ($row as $k => $value) {
    if ($k !== 0) {
      $final_row[$k]['type'] = $value['type'];
      $final_row[$k]['assigned_to'] = $value['assigned_to'];
      $final_row[$k]['bill_count'] = isset($value['bill_count']) ? $value['bill_count'] : '';
      $final_row[$k]['bill_value'] = isset($value['bill_value']) ? $value['bill_value'] : '';
      $final_row[$k]['bill_counts'] = isset($value['bill_counts']) ? $value['bill_counts'] : '';
      $final_row[$k]['bill_values'] = isset($value['bill_values']) ? $value['bill_values'] : '';
      $final_row[$k]['mtd_bill_count'] = isset($value['mtd_bill_count']) ? $value['mtd_bill_count'] : '';
      $final_row[$k]['mtd_bill_value'] = isset($value['mtd_bill_value']) ? $value['mtd_bill_value'] : '';
      $final_row[$k]['mtd_bill_counts'] = isset($value['mtd_bill_counts']) ? $value['mtd_bill_counts'] : '';
      $final_row[$k]['mtd_bill_values'] = isset($value['mtd_bill_values']) ? $value['mtd_bill_values'] : '';
    }
    else {
      $final_row[$k] = $value;
    }
  }

  foreach ($row as $v) {
    $final_row['total']['type'] = 'Total';
    $final_row['total']['assigned_to'] = '';
    $final_row['total']['bill_count'] += $v['bill_count'];
    $final_row['total']['bill_value'] += $v['bill_value'];
    $final_row['total']['bill_counts'] += $v['bill_counts'];
    $final_row['total']['bill_values'] += $v['bill_values'];
    $final_row['total']['mtd_bill_count'] += $v['mtd_bill_count'];
    $final_row['total']['mtd_bill_value'] += $v['mtd_bill_value'];
    $final_row['total']['mtd_bill_counts'] += $v['mtd_bill_counts'];
    $final_row['total']['mtd_bill_values'] += $v['mtd_bill_values'];
  }

  $header = array('', '', array('data' => 'Yesterday Assignment', 'colspan' => 2), array('data' => 'Today\'s Collection', 'colspan' => 2), array('data' => 'MTD Assignment', 'colspan' => 2), array('data' => 'MTD Collection', 'colspan' => 2));
  $output = "<h3>Yesterday Assignment Today's Collection and MTD Assignment MTD Collection</h3>";
  $output .= theme('table', array('header' => $header, 'rows' => $final_row, 'attributes' => array('border' => '1', 'cellpadding' => '5', 'cellspacing' => '0')));

  return $output;
}

/*
 * Returns table of Today's Petty Cash Transaction and Closing Balance
 */

function get_email_today_petty_cash_transaction($distributor_id) {
  $start_date = strtotime(date('Y-m-d'));

  $query = db_select('petty_cash', 'pc');
  $query->condition('pc.voucher_date', $start_date, '>');
  $query->condition('pc.distributor_id', $distributor_id, '=');
  $query->fields('pc');
  $result = $query->execute()->fetchAll();

  $header = array('Serial ID', 'Voucher no', 'Type', 'Voucher date', 'Head', 'Subhead', 'Description', 'Person name', 'Amount', 'Balance');
  $row = array();
  foreach ($result as $data) {
    $row['serial_no'] = $data->id;
    $row['voucher_no'] = $data->voucher_no;
    $row['type'] = $data->type;
    $row['voucher_date'] = date('d-M-y', $data->voucher_date);
    $row['head'] = $data->head;
    $row['subhead'] = $data->subhead;
    $row['description'] = $data->description;
    $row['person_name'] = $data->person_name;
    $row['amount'] = $data->amount;
    $row['balance'] = $data->balance;

    $rows[] = $row;
  }

  $output = "<h3>Today's Petty Cash Transaction and Closing Balance</h3>";
  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('border' => '1', 'cellpadding' => '5', 'cellspacing' => '0'), 'empty' => t('No Records has been found')));

  return $output;
}
