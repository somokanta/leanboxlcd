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
  
  $params['subject'] = "SB And Bounce Chq Status";
  $params['body'] = $pending_for_collection_html . "<br>" . $pending_for_assignment_html . "<br>" . $pending_for_upload_html;
  $mail_to = variable_get('to_email');
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
  $rows = array();
  $row[] = array('Type', 'Count', 'Value', 'Count', 'Value');
  $decl_date = strtotime('-8 day', strtotime(date('Y-m-d')));

  foreach($res as $data) {
    if($data['field_flag_value']  == 'signed') {
      $row['signed']['type']  = 'Signed';
      $row['signed']['count']  = $signed_counter++;
      $row['signed']['value'] += $data['field_remaining_amt_collection_value'] - ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
      if(strtotime($data['field_declaration_date_value']) <= $decl_date) {
        $row['signed']['decl_count'] = $decl_signed_counter++;
        $row['signed']['decl_value'] += $data['field_remaining_amt_collection_value'] - ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
      }
      else {
        $row['signed']['decl_count'] = 0;
        $row['signed']['decl_value'] = 0;
      }
    }
    else{
      $row['bounced']['type']  = 'Bounced';
      $row['bounced']['count']  = $bounced_counter++;
      $row['bounced']['value'] += $data['field_remaining_amt_collection_value'] - ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
      if(strtotime($data['field_declaration_date_value']) <= $decl_date) {
        $row['bounced']['decl_count'] = $decl_bounced_counter++;
        $row['bounced']['decl_value'] += $data['field_remaining_amt_collection_value'] - ($data['field_previous_cash_amt_value'] + $data['field_previous_cheque_amt_value']);
      }
      else {
        $row['bounced']['decl_count'] = 0;
        $row['bounced']['decl_value'] = 0;
      }
    }
  }
  
  $total_till_date_count = $row['signed']['count'] + $row['bounced']['count'];
  $total_till_date_value = $row['signed']['value'] + $row['bounced']['value'];
  $total_decl_date_count = $row['signed']['decl_count'] + $row['bounced']['decl_count'];
  $total_decl_date_value = $row['signed']['decl_value'] + $row['bounced']['decl_value'];
  $row[] = array('Total',$total_till_date_count,$total_till_date_value,$total_decl_date_count,$total_decl_date_value);
  
  $header = array('', array('data' => 'Till Date', 'colspan' => 2), array('data' => '>8Days from Declaration Date', 'colspan' => 2));
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
  
  $query->fields('n', array('nid'));
  $query->fields('dd', array('field_declaration_date_value'));
  $query->fields('ff', array('field_flag_value'));
  $query->fields('ra', array('field_remaining_amt_collection_value'));
  $query->fields('ad', array('field_assignment_date_value'));
  $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

  $signed_counter  = 1;
  $bounced_counter  = 1;
  $decl_signed_counter = 1;
  $decl_bounced_counter = 1;
  $rows = array();
  $row[] = array('Type', 'Count', 'Value', 'Count', 'Value');
  $decl_date = strtotime('-7 day', strtotime(date('Y-m-d')));

  foreach($res as $data) {
    if($data['field_flag_value']  == 'signed') {
      $row['signed']['type']  = 'Signed';
      $row['signed']['count']  = $signed_counter++;
      $row['signed']['value'] += $data['field_remaining_amt_collection_value'];
      if(strtotime($data['field_declaration_date_value']) <= $decl_date) {
        $row['signed']['decl_count'] = $decl_signed_counter++;
        $row['signed']['decl_value'] += $data['field_remaining_amt_collection_value'];
      }
      else {
        $row['signed']['decl_count'] = 0;
        $row['signed']['decl_value'] = 0;
      }
    }
    else{
      $row['bounced']['type']  = 'Bounced';
      $row['bounced']['count']  = $bounced_counter++;
      $row['bounced']['value'] += $data['field_remaining_amt_collection_value'];
      if(strtotime($data['field_declaration_date_value']) <= $decl_date) {
        $row['bounced']['decl_count'] = $decl_bounced_counter++;
        $row['bounced']['decl_value'] += $data['field_remaining_amt_collection_value'];
      }
      else {
        $row['bounced']['decl_count'] = 0;
        $row['bounced']['decl_value'] = 0;
      }
    }
  }
  
  $total_till_date_count = $row['signed']['count'] + $row['bounced']['count'];
  $total_till_date_value = $row['signed']['value'] + $row['bounced']['value'];
  $total_decl_date_count = $row['signed']['decl_count'] + $row['bounced']['decl_count'];
  $total_decl_date_value = $row['signed']['decl_value'] + $row['bounced']['decl_value'];
  $row[] = array('Total',$total_till_date_count,$total_till_date_value,$total_decl_date_count,$total_decl_date_value);
  
  $header = array('', array('data' => 'Till Date', 'colspan' => 2), array('data' => '>7Days from Declaration Date', 'colspan' => 2));
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
  
  $query->fields('n', array('nid'));
  $query->fields('dd', array('field_declaration_date_value'));
  $query->fields('ff', array('field_flag_value'));
  $query->fields('ra', array('field_remaining_amt_collection_value'));
  $query->fields('ud', array('field_file_upload_date_value'));
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
      $row['signed']['value'] += $data['field_remaining_amt_collection_value'];
      if(strtotime($data['field_declaration_date_value']) <= $decl_date) {
        $row['signed']['decl_count']  = $decl_signed_counter++;
        $row['signed']['decl_value'] += $data['field_remaining_amt_collection_value'];
      }
      else {
        $row['signed']['decl_count']  = 0;
        $row['signed']['decl_value']  = 0;
      }
    }
    else {
      $row['bounced']['type']  = 'Bounced';
      $row['bounced']['count']  = $bounced_counter++;
      $row['bounced']['value'] += $data['field_remaining_amt_collection_value'];
      if(strtotime($data['field_declaration_date_value']) <= $decl_date) {    
        $row['bounced']['decl_count']  = $decl_bounced_counter++;
        $row['bounced']['decl_value'] += $data['field_remaining_amt_collection_value'];
      }
      else {
        $row['bounced']['decl_count']  = 0;
        $row['bounced']['decl_value'] = 0;
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
