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
  
  // Want record where bill date is 1st oct onward
  $timestamp = strtotime(date('2017-10-01'))-1;
  $query->join('field_data_field_sales_bill_no_ref', 'sbrf', 'sbrf.entity_id = n.nid');
  $query->join('field_data_field_sr_bill_date', 'sr', 'sr.entity_id = sbrf.field_sales_bill_no_ref_nid');
  $query->condition('sr.field_sr_bill_date_value', $timestamp,'>');

  $query->fields('n', array('nid'));
  $query->fields('dd', array('field_declaration_date_value'));
  $query->fields('ff', array('field_flag_value'));
  $query->fields('cs', array('field_collection_status_value'));
  $query->fields('ra', array('field_remaining_amt_collection_value'));
  $query->fields('pc', array('field_previous_cash_amt_value'));
  $query->fields('pch', array('field_previous_cheque_amt_value'));
  $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

  $signed_counter  = 1;
  $bounced_counter  = 1;
  $decl_signed_counter = 1;
  $decl_bounced_counter = 1;
  $fivedays_decl_signed_counter = 1;
  $fivedays_decl_bounced_counter = 1;
  $twelvedays_decl_signed_counter = 1;
  $twelvedays_decl_bounced_counter = 1;
  $thirtyonedays_decl_signed_counter = 1;
  $thirtyonedays_decl_bounced_counter = 1;
  $rows = array();
  $row[] = array('Type', 'Count', 'Value', 'Count', 'Value', 'Count', 'Value', 'Count', 'Value', 'Count', 'Value');
  $decl_date = strtotime(date('Y-m-d'));
  $ten_days_before = strtotime('-10 day', strtotime(date('Y-m-d')));
  $fifteen_days_before = strtotime('-15 day', strtotime(date('Y-m-d')));
  $thirty_days_before = strtotime('-30 day', strtotime(date('Y-m-d')));
  

  foreach($res as $data) {
    $declaration_date = strtotime($data['field_declaration_date_value']);
    if($data['field_flag_value']  == 'signed') {
      $row['signed']['type']  = 'Signed';
      $row['signed']['count']  = $signed_counter++;
      $row['signed']['value'] += $data['field_remaining_amt_collection_value'] - ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
      // Collection from 1-10 Days from Declaration Date
      if($declaration_date <= $decl_date && $declaration_date >= $ten_days_before) {
        $row['signed']['decl_count'] = $decl_signed_counter++;
        $row['signed']['decl_value'] += $data['field_remaining_amt_collection_value'] - ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
      }
      else {
        $row['signed']['decl_count'] += 0;
        $row['signed']['decl_value'] += 0;
      }
      // Collection from 11-15 Days from Declaration Date
      if($declaration_date < $ten_days_before && $declaration_date >= $fifteen_days_before) {
        $row['signed']['5days_decl_count'] = $fivedays_decl_signed_counter++;
        $row['signed']['5days_decl_value'] += $data['field_remaining_amt_collection_value'] - ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
      }
      else {
        $row['signed']['5days_decl_count'] += 0;
        $row['signed']['5days_decl_value'] += 0;
      }
      // Collection from 16-30 Days from Declaration Date
      if($declaration_date < $fifteen_days_before && $declaration_date >= $thirty_days_before) {
        $row['signed']['12days_decl_count'] = $twelvedays_decl_signed_counter++;
        $row['signed']['12days_decl_value'] += $data['field_remaining_amt_collection_value'] - ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
      }
      else {
        $row['signed']['12days_decl_count'] += 0;
        $row['signed']['12days_decl_value'] += 0;
      }
      // Collection from below 30 Days from Declaration Date
      if($declaration_date < $thirty_days_before) {
        $row['signed']['31days_decl_count'] = $thirtyonedays_decl_signed_counter++;
        $row['signed']['31days_decl_value'] += $data['field_remaining_amt_collection_value'] - ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
      }
      else {
        $row['signed']['31days_decl_count'] += 0;
        $row['signed']['31days_decl_value'] += 0;
      }
    }
    else{
      $row['bounced']['type']  = 'Bounced';
      $row['bounced']['count']  = $bounced_counter++;
      $row['bounced']['value'] += $data['field_remaining_amt_collection_value'] - ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
      // Collection from 1-10 Days from Declaration Date
      if($declaration_date <= $decl_date && $declaration_date >= $ten_days_before) {
        $row['bounced']['decl_count'] = $decl_bounced_counter++;
        $row['bounced']['decl_value'] += $data['field_remaining_amt_collection_value'] - ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
      }
      else {
        $row['bounced']['decl_count'] += 0;
        $row['bounced']['decl_value'] += 0;
      }
      // Collection from 10-15 Days from Declaration Date
      if($declaration_date < $ten_days_before && $declaration_date >= $fifteen_days_before) {
        $row['bounced']['5days_decl_count'] = $fivedays_decl_bounced_counter++;
        $row['bounced']['5days_decl_value'] += $data['field_remaining_amt_collection_value'] - ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
      }
      else {
        $row['bounced']['5days_decl_count'] += 0;
        $row['bounced']['5days_decl_value'] += 0;
      }
      // Collection from 16-30 Days from Declaration Date
      if($declaration_date < $fifteen_days_before && $declaration_date >= $thirty_days_before) {
        $row['bounced']['12days_decl_count'] = $twelvedays_decl_bounced_counter++;
        $row['bounced']['12days_decl_value'] += $data['field_remaining_amt_collection_value'] - ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
      }
      else {
        $row['bounced']['12days_decl_count'] += 0;
        $row['bounced']['12days_decl_value'] += 0;
      }
      // Collection from below 30 Days from Declaration Date
      if($declaration_date < $thirty_days_before) {
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
  $total_decl_date_count = $row['signed']['decl_count'] + $row['bounced']['decl_count'];
  $total_decl_date_value = $row['signed']['decl_value'] + $row['bounced']['decl_value'];
  $total_5days_decl_count = $row['signed']['5days_decl_count'] + $row['bounced']['5days_decl_count'];
  $total_5days_decl_value = $row['signed']['5days_decl_value'] + $row['bounced']['5days_decl_value'];
  $total_12days_decl_count = $row['signed']['12days_decl_count'] + $row['bounced']['12days_decl_count'];
  $total_12days_decl_value = $row['signed']['12days_decl_value'] + $row['bounced']['12days_decl_value'];
  $total_31days_decl_count = $row['signed']['31days_decl_count'] + $row['bounced']['31days_decl_count'];
  $total_31days_decl_value = $row['signed']['31days_decl_value'] + $row['bounced']['31days_decl_value'];
  $row[] = array('Total',$total_till_date_count,$total_till_date_value,$total_decl_date_count,$total_decl_date_value,$total_5days_decl_count,$total_5days_decl_value,$total_12days_decl_count,$total_12days_decl_value,$total_31days_decl_count,$total_31days_decl_value);
  
  $header = array('', array('data' => 'Till Date', 'colspan' => 2), array('data' => '1-10 Days from Declaration Date', 'colspan' => 2), array('data' => '11-15 Days from Declaration Date', 'colspan' => 2), array('data' => '16-30 Days from Declaration Date', 'colspan' => 2), array('data' => '>30 Days from Declaration Date', 'colspan' => 2));
  $output = "<h3>Pending for Collection</h3>";
  $output .= theme('table', array('header' => $header, 'rows' => $row, 'attributes' => array('border' => '1', 'cellpadding' => '5', 'cellspacing' => '0')));

  return $output;
}

/*
 * Returns table of Pending for Assignment
 */
function get_email_pending_for_assignment($distributor_id) {
  $query = db_select('node', 'n');
  $query->condition('n.type', 'payment_collection');
  $query->condition('n.uid', $distributor_id, '=');
  $query->join('field_data_field_declaration_date', 'dd', 'dd.entity_id = n.nid');
  $query->join('field_data_field_flag', 'ff', 'ff.entity_id = n.nid');
  $query->join('field_data_field_remaining_amt_collection', 'ra', 'ra.entity_id = n.nid');
  $query->leftjoin('field_data_field_assignment_date', 'ad', 'ad.entity_id = n.nid');
  $query->isNull('ad.field_assignment_date_value');
  $query->leftjoin('field_data_field_previous_cash_amt', 'pc', 'pc.entity_id = n.nid');
  $query->leftjoin('field_data_field_previous_cheque_amt', 'pch', 'pch.entity_id = n.nid');

  // Want record where bill date is 1st oct onward
  $timestamp = strtotime(date('2017-10-01'))-1;
  $query->join('field_data_field_sales_bill_no_ref', 'sbrf', 'sbrf.entity_id = n.nid');
  $query->join('field_data_field_sr_bill_date', 'sr', 'sr.entity_id = sbrf.field_sales_bill_no_ref_nid');
  $query->condition('sr.field_sr_bill_date_value', $timestamp,'>');
  
  $query->fields('n', array('nid'));
  $query->fields('dd', array('field_declaration_date_value'));
  $query->fields('ff', array('field_flag_value'));
  $query->fields('ra', array('field_remaining_amt_collection_value'));
  $query->fields('ad', array('field_assignment_date_value'));
  $query->fields('pc', array('field_previous_cash_amt_value'));
  $query->fields('pch', array('field_previous_cheque_amt_value'));
  $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

  $signed_counter  = 1;
  $bounced_counter  = 1;
  $decl_signed_counter = 1;
  $decl_bounced_counter = 1;
  $fivedays_decl_signed_counter = 1;
  $fivedays_decl_bounced_counter = 1;
  $twelvedays_decl_signed_counter = 1;
  $twelvedays_decl_bounced_counter = 1;
  $thirtyonedays_decl_signed_counter = 1;
  $thirtyonedays_decl_bounced_counter = 1;
  $rows = array();
  $row[] = array('Type', 'Count', 'Value', 'Count', 'Value', 'Count', 'Value', 'Count', 'Value', 'Count', 'Value');
  $decl_date = strtotime(date('Y-m-d'));
  $ten_days_before = strtotime('-10 day', strtotime(date('Y-m-d')));
  $fifteen_days_before = strtotime('-15 day', strtotime(date('Y-m-d')));
  $thirty_days_before = strtotime('-30 day', strtotime(date('Y-m-d')));
  
  foreach($res as $data) {
    $declaration_date = strtotime($data['field_declaration_date_value']);
    if($data['field_flag_value']  == 'signed') {
      $row['signed']['type']  = 'Signed';
      $row['signed']['count']  = $signed_counter++;
      $row['signed']['value'] += $data['field_remaining_amt_collection_value'] - ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
      // Assignment from 1-10 Days from Declaration Date
      if($declaration_date <= $decl_date && $declaration_date >= $ten_days_before) {
        $row['signed']['decl_count'] = $decl_signed_counter++;
        $row['signed']['decl_value'] += $data['field_remaining_amt_collection_value'] - ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
      }
      else {
        $row['signed']['decl_count'] += 0;
        $row['signed']['decl_value'] += 0;
      }
      // Assignment from 11-15 Days from Declaration Date
      if($declaration_date < $ten_days_before && $declaration_date >= $fifteen_days_before) {
        $row['signed']['5days_decl_count'] = $fivedays_decl_signed_counter++;
        $row['signed']['5days_decl_value'] += $data['field_remaining_amt_collection_value'] - ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
      }
      else {
        $row['signed']['5days_decl_count'] += 0;
        $row['signed']['5days_decl_value'] += 0;
      }
      // Assignment from 16-30 Days from Declaration Date
      if($declaration_date < $fifteen_days_before && $declaration_date >= $thirty_days_before) {
        $row['signed']['12days_decl_count'] = $twelvedays_decl_signed_counter++;
        $row['signed']['12days_decl_value'] += $data['field_remaining_amt_collection_value'] - ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
      }
      else {
        $row['signed']['12days_decl_count'] += 0;
        $row['signed']['12days_decl_value'] += 0;
      }
      // Assignment from below 30 Days from Declaration Date
      if($declaration_date < $thirty_days_before) {
        $row['signed']['31days_decl_count'] = $thirtyonedays_decl_signed_counter++;
        $row['signed']['31days_decl_value'] += $data['field_remaining_amt_collection_value'] - ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
      }
      else {
        $row['signed']['31days_decl_count'] += 0;
        $row['signed']['31days_decl_value'] += 0;
      }
    }
    else{
      $row['bounced']['type']  = 'Bounced';
      $row['bounced']['count']  = $bounced_counter++;
      $row['bounced']['value'] += $data['field_remaining_amt_collection_value'] - ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
      // Assignment from 1-10 Days from Declaration Date
      if($declaration_date <= $decl_date && $declaration_date >= $ten_days_before) {
        $row['bounced']['decl_count'] = $decl_bounced_counter++;
        $row['bounced']['decl_value'] += $data['field_remaining_amt_collection_value'] - ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
      }
      else {
        $row['bounced']['decl_count'] += 0;
        $row['bounced']['decl_value'] += 0;
      }
      // Assignment from 10-15 Days from Declaration Date
      if($declaration_date < $ten_days_before && $declaration_date >= $fifteen_days_before) {
        $row['bounced']['5days_decl_count'] = $fivedays_decl_bounced_counter++;
        $row['bounced']['5days_decl_value'] += $data['field_remaining_amt_collection_value'] - ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
      }
      else {
        $row['bounced']['5days_decl_count'] += 0;
        $row['bounced']['5days_decl_value'] += 0;
      }
      // Assignment from 16-30 Days from Declaration Date
      if($declaration_date < $fifteen_days_before && $declaration_date >= $thirty_days_before) {
        $row['bounced']['12days_decl_count'] = $twelvedays_decl_bounced_counter++;
        $row['bounced']['12days_decl_value'] += $data['field_remaining_amt_collection_value'] - ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
      }
      else {
        $row['bounced']['12days_decl_count'] += 0;
        $row['bounced']['12days_decl_value'] += 0;
      }
      // Assignment from below 30 Days from Declaration Date
      if($declaration_date < $thirty_days_before) {
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
  $total_decl_date_count = $row['signed']['decl_count'] + $row['bounced']['decl_count'];
  $total_decl_date_value = $row['signed']['decl_value'] + $row['bounced']['decl_value'];
  $total_5days_decl_count = $row['signed']['5days_decl_count'] + $row['bounced']['5days_decl_count'];
  $total_5days_decl_value = $row['signed']['5days_decl_value'] + $row['bounced']['5days_decl_value'];
  $total_12days_decl_count = $row['signed']['12days_decl_count'] + $row['bounced']['12days_decl_count'];
  $total_12days_decl_value = $row['signed']['12days_decl_value'] + $row['bounced']['12days_decl_value'];
  $total_31days_decl_count = $row['signed']['31days_decl_count'] + $row['bounced']['31days_decl_count'];
  $total_31days_decl_value = $row['signed']['31days_decl_value'] + $row['bounced']['31days_decl_value'];
  $row[] = array('Total',$total_till_date_count,$total_till_date_value,$total_decl_date_count,$total_decl_date_value,$total_5days_decl_count,$total_5days_decl_value,$total_12days_decl_count,$total_12days_decl_value,$total_31days_decl_count,$total_31days_decl_value);
  
  $header = array('', array('data' => 'Till Date', 'colspan' => 2), array('data' => '1-10 Days from Declaration Date', 'colspan' => 2), array('data' => '11-15 Days from Declaration Date', 'colspan' => 2), array('data' => '16-30 Days from Declaration Date', 'colspan' => 2), array('data' => '>30 Days from Declaration Date', 'colspan' => 2));
  $output = "<h3>Pending for Assignment</h3>";
  $output .= theme('table', array('header' => $header, 'rows' => $row, 'attributes' => array('border' => '1', 'cellpadding' => '5', 'cellspacing' => '0')));

  return $output;
}

/*
 * Returns table of Pending for Upload
 */
function get_email_pending_for_upload($distributor_id) {
  $query = db_select('node', 'n');
  $query->condition('n.type', 'payment_collection');
  $query->condition('n.uid', $distributor_id, '=');
  $query->join('field_data_field_declaration_date', 'dd', 'dd.entity_id = n.nid');
  $query->join('field_data_field_flag', 'ff', 'ff.entity_id = n.nid');
  $query->join('field_data_field_remaining_amt_collection', 'ra', 'ra.entity_id = n.nid');
  $query->leftjoin('field_data_field_file_upload_date', 'ud', 'ud.entity_id = n.nid');
  $query->isNull('ud.field_file_upload_date_value');
  $query->leftjoin('field_data_field_previous_cash_amt', 'pc', 'pc.entity_id = n.nid');
  $query->leftjoin('field_data_field_previous_cheque_amt', 'pch', 'pch.entity_id = n.nid');

  // Want record where bill date is 1st oct onward
  $timestamp = strtotime(date('2017-10-01'))-1;
  $query->join('field_data_field_sales_bill_no_ref', 'sbrf', 'sbrf.entity_id = n.nid');
  $query->join('field_data_field_sr_bill_date', 'sr', 'sr.entity_id = sbrf.field_sales_bill_no_ref_nid');
  $query->condition('sr.field_sr_bill_date_value', $timestamp,'>');
  
  $query->fields('n', array('nid'));
  $query->fields('dd', array('field_declaration_date_value'));
  $query->fields('ff', array('field_flag_value'));
  $query->fields('ra', array('field_remaining_amt_collection_value'));
  $query->fields('ud', array('field_file_upload_date_value'));
  $query->fields('pc', array('field_previous_cash_amt_value'));
  $query->fields('pch', array('field_previous_cheque_amt_value'));
  $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

  $signed_counter  = 1;
  $bounced_counter  = 1;
  $decl_signed_counter = 1;
  $decl_bounced_counter = 1;
  $rows = array();
  $row[] = array('Type', 'Count', 'Value', 'Count', 'Value');
  $decl_date = strtotime('-2 day', strtotime(date('Y-m-d')));

  foreach($res as $data) {
    if($data['field_flag_value']  == 'signed') {
      $row['signed']['type']  = 'Signed';
      $row['signed']['count']  = $signed_counter++;
      $row['signed']['value'] += $data['field_remaining_amt_collection_value'] - ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
      if(strtotime($data['field_declaration_date_value']) <= $decl_date) {
        $row['signed']['decl_count']  = $decl_signed_counter++;
        $row['signed']['decl_value'] += $data['field_remaining_amt_collection_value'] - ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
      }
      else {
        $row['signed']['decl_count']  += 0;
        $row['signed']['decl_value']  += 0;
      }
    }
    else {
      $row['bounced']['type']  = 'Bounced';
      $row['bounced']['count']  = $bounced_counter++;
      $row['bounced']['value'] += $data['field_remaining_amt_collection_value'] - ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
      if(strtotime($data['field_declaration_date_value']) <= $decl_date) {    
        $row['bounced']['decl_count']  = $decl_bounced_counter++;
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
  $row[] = array('Total',$total_till_date_count,$total_till_date_value,$total_decl_date_count,$total_decl_date_value);
  
  $header = array('', array('data' => 'Till Date', 'colspan' => 2), array('data' => '>2Days from Declaration Date', 'colspan' => 2));
  $output = "<h3>Pending for Upload</h3>";
  $output .= theme('table', array('header' => $header, 'rows' => $row, 'attributes' => array('border' => '1', 'cellpadding' => '5', 'cellspacing' => '0')));

  return $output;
}

function get_email_pending_for_yesterday_assignment($distributor_id) {
  // Query For Yesterday Assignment
  $assignment_date = strtotime('-1 day', strtotime(date('Y-m-d')));
  $date = date('Y-m-d H:i:s', $assignment_date);
  
  $query = db_select('node', 'n');
  $query->condition('n.type', 'payment_collection');
  $query->condition('n.uid', $distributor_id, '=');
  $query->join('field_data_field_flag', 'ff', 'ff.entity_id = n.nid');
  $query->join('field_data_field_remaining_amt_collection', 'ra', 'ra.entity_id = n.nid');
  $query->leftjoin('field_data_field_assignment_date', 'ad', 'ad.entity_id = n.nid');
  $query->condition('ad.field_assignment_date_value', $date, '=');
  $query->leftjoin('field_data_field_assigned_person', 'ap', 'ap.entity_id = n.nid');

  $query->leftjoin('field_data_field_previous_cash_amt', 'pc', 'pc.entity_id = n.nid');
  $query->leftjoin('field_data_field_previous_cheque_amt', 'pch', 'pch.entity_id = n.nid');
  
  $query->groupBy('ap.field_assigned_person_value','ff.field_flag_value');
  
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
  $row[] = array('Type', 'Assigned To', 'Count', 'Value', 'Count', 'Value');
  foreach($res as $data) {
    $person_signed = $data['field_assigned_person_value'].'-'.'signed';
    $person_bounced = $data['field_assigned_person_value'].'-'.'bounced';
    if($data['field_flag_value']  == 'signed') {
      $row[$person_signed]['type']  = 'Signed';
      $row[$person_signed]['assigned_to']  = $data['field_assigned_person_value'];
      $row[$person_signed]['bill_count']  = $data['bill_count'];
      $row[$person_signed]['bill_value']  = $data['bill_value'] - ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
      $row[$person_signed]['bill_counts'] =  '';
      $row[$person_signed]['bill_values'] =  '' ;
    }
    else {
      $row[$person_bounced]['type']  = 'Bounced';
      $row[$person_bounced]['assigned_to']  = $data['field_assigned_person_value'];
      $row[$person_bounced]['bill_count']  = $data['bill_count'];
      $row[$person_bounced]['bill_value']  = $data['bill_value'] - ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
      $row[$person_bounced]['bill_counts'] =  '';
      $row[$person_bounced]['bill_values'] =  '' ;
    }
  }
  
  // Query For Today's Collection
  $collection_date = strtotime(date('Y-m-d'));
  $date = date('Y-m-d H:i:s', $collection_date);
  
  $query = db_select('node', 'n');
  $query->condition('n.type', 'payment_collection');
  $query->condition('n.uid', $distributor_id, '=');
  $query->join('field_data_field_flag', 'ff', 'ff.entity_id = n.nid');
  $query->join('field_data_field_remaining_amt_collection', 'ra', 'ra.entity_id = n.nid');
  $query->leftjoin('field_data_field_collection_date', 'cd', 'cd.entity_id = n.nid');
  $query->condition('cd.field_collection_date_value', $date, '=');
  $query->leftjoin('field_data_field_assigned_person', 'ap', 'ap.entity_id = n.nid');
  
  $query->leftjoin('field_data_field_previous_cash_amt', 'pc', 'pc.entity_id = n.nid');
  $query->leftjoin('field_data_field_previous_cheque_amt', 'pch', 'pch.entity_id = n.nid');
  
  $query->groupBy('ap.field_assigned_person_value','ff.field_flag_value');
  
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

  foreach($res as $data) {
    $person_signed = $data['field_assigned_person_value'].'-'.'signed';
    $person_bounced = $data['field_assigned_person_value'].'-'.'bounced';
    if($data['field_flag_value']  == 'signed') {
      $row[$person_signed]['type']  = 'Signed';
      $row[$person_signed]['assigned_to']  = $data['field_assigned_person_value'];
      $row[$person_signed]['bill_count'] = isset($row[$person_signed]['bill_count']) ? $row[$person_signed]['bill_count'] : '';
      $row[$person_signed]['bill_value'] = isset($row[$person_signed]['bill_value']) ? $row[$person_signed]['bill_value'] : '';
      $row[$person_signed]['bill_counts']  = $data['bill_count'];
      $row[$person_signed]['bill_values']  = $data['bill_value'] - ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
    }
    else {
      $row[$person_bounced]['type']  = 'Bounced';
      $row[$person_bounced]['assigned_to']  = $data['field_assigned_person_value'];
      $row[$person_bounced]['bill_count'] = isset($row[$person_bounced]['bill_count']) ? $row[$person_bounced]['bill_count'] : '';
      $row[$person_bounced]['bill_value'] = isset($row[$person_bounced]['bill_value']) ? $row[$person_bounced]['bill_value'] : '';
      $row[$person_bounced]['bill_counts']  = $data['bill_count'];
      $row[$person_bounced]['bill_values']  = $data['bill_value'] - ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
    }
  }

  $total = $row;
  foreach($total as $v){
    $row['total']['type']  = 'Total';
    $row['total']['person']  = '';
    $row['total']['bill_count']  += $v['bill_count'];
    $row['total']['bill_value']  += $v['bill_value'];
    $row['total']['bill_counts'] +=  $v['bill_counts'];
    $row['total']['bill_values'] +=  $v['bill_values'];
  }

  $header = array('','', array('data' => 'Yesterday Assignment', 'colspan' => 2), array('data' => 'Today\'s Collection', 'colspan' => 2));
  $output = "<h3>Yesterday Assignment Today's Collection</h3>";
  $output .= theme('table', array('header' => $header, 'rows' => $row, 'attributes' => array('border' => '1', 'cellpadding' => '5', 'cellspacing' => '0')));
  
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
  foreach($result as $data) {
    $row['serial_no'] = $data->id;
    $row['voucher_no'] = $data->voucher_no;
    $row['type'] = $data->type;
    $row['voucher_date'] = date('d-M-y',$data->voucher_date);
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