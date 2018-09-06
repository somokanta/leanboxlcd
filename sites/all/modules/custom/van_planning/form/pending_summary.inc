<?php

/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
function pending_summary_form($form, &$form_state) {
  
  $billing_start_date = $_GET['from_date'] ? $_GET['from_date'] : date('Y-m-d', strtotime("-15 days")); // @todo replace 30 this line
  $billing_end_date = $_GET['to_date'] ? $_GET['to_date'] : date("Y-m-d"); 

 $form['bill_from_date'] = array(
    '#type' => 'date_popup',
    '#title' => 'Bill from date',
    '#require' => TRUE,
    '#date_label_position' => 'within',
    '#date_format' => 'Y-m-d',
    '#default_value' => $billing_start_date,
  );
  $form['bill_to_date'] = array(
    '#type' => 'date_popup',
    '#title' => 'Bill to date',
    '#require' => TRUE,
    '#date_label_position' => 'within',
    '#date_format' => 'Y-m-d',
    '#default_value' => $billing_end_date,
  );
 

  $form['save'] = array(
    '#type' => 'submit',
    '#value' => 'Submit'
  );
  
  return $form;
}

function pending_summary_form_submit($form, &$form_state) {
  $from_date = $form_state['values']['bill_from_date'];
  $to_date = $form_state['values']['bill_to_date'];
  if (($form_state['values']["op"] == "Submit") && (arg(0) == 'pending-summary')) {
    drupal_goto('pending-summary', array('query' => array('from_date'=>$from_date,'to_date'=>$to_date)));
  }
}

function pending_summary_callback() {
 if (isset($_GET['from_date'])) {
      $billing_start_date = strtotime($_GET['from_date']);
  }else{
     $billing_start_date = strtotime(date('Y-m-d', strtotime("-15 days")));
  }
  if (isset($_GET['to_date'])) {
      $billing_end_date = strtotime($_GET['to_date']);
  }else{
      $billing_end_date = strtotime(date('Y-m-d'));
  }
  
  $form = drupal_render(drupal_get_form('pending_summary_form'));
  
  $user_id = $GLOBALS['user']->uid;
  $dis_id = change_feeds_import_get_distributor_id($user_id);
  
  $query = db_select('node', 'n');
  $query->condition('n.type', 'hul_code_master');
  $query->leftjoin('field_data_field_area_code', 'ac', 'ac.entity_id = n.nid');
  $query->condition('n.uid', $dis_id);
  
  $query->leftjoin('node', 'n1', "n1.nid = ac.field_area_code_nid and n1.type ='area_code_master' and n1.uid = $dis_id ");
  $query->groupBy('n1.title');

  // join outlet master
  $query->join('field_data_field_hul_code_reference', 'hcr', 'hcr.field_hul_code_reference_nid = n.nid');
  $query->join('node', 'n2', 'n2.nid = hcr.entity_id');
  $query->condition('n2.type', 'outlet_master');
  $query->condition('n2.uid', $dis_id);
  $query->join('field_data_field_pl_group', 'fpg', 'fpg.entity_id = n2.nid');
  $query->groupBy('fpg.field_pl_group_value'); // group by Pl Group
  $query->addField('fpg', 'field_pl_group_value', 'pl_group');

  // join sales register data
  $query->join('field_data_field_party_code_reference', 'pcr', 'pcr.field_party_code_reference_nid = n2.nid');

  $query->join('node', 'n3', 'n3.nid = pcr.entity_id');
  $query->condition('n3.type', 'sales_register_data');
  $query->condition('n3.uid', $dis_id);

  $query->leftJoin('field_data_field_trip_id_reference', 'ft', 'ft.entity_id = n3.nid');
  $query->leftJoin('field_data_field_delivery_pending', 'dp', 'n3.nid = dp.entity_id');

  $db_or = db_or();
  $db_or->isNull('ft.field_trip_id_reference_nid');
  $db_or->condition('dp.field_delivery_pending_value', 1);
  $query->condition($db_or);

  //$query->addField('n3', 'nid', 'sales_nid');
  $query->join('field_data_field_sr_bill_date', 'sbd', 'sbd.entity_id = n3.nid');
  $query->condition('sbd.field_sr_bill_date_value', array($billing_start_date, $billing_end_date), 'BETWEEN');

  // custom billing data
  $query->join('custom_billing_data', 'cb', 'cb.bill_number = n3.title');
  $query->condition('cb.distributor_id', $dis_id);
  $query->groupBy('cb.bill_number');
  $query->addExpression('ROUND(SUM(cb.Derived_NET_SALES_KGS))', 'sum_kg');
  
  $query->join('field_data_field_bill_value', 'bv', 'bv.entity_id = n3.nid');
  $query->addField('n3', 'title', 'bill_no');
  $query->addExpression('ROUND(bv.field_bill_value_value)', 'bill_value');

  $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
 
  $total = array();
  foreach ($res as $key => $val) {
    foreach ($val as $key1 => $val1) {
      if ($key1 == 'bill_no') {
        if (!isset($res1[$val['pl_group']]['bill_count'])) {
          $res1[$val['pl_group']]['bill_count'] = 0;
        }
        $res1[$val['pl_group']]['bill_count'] += 1;
        $total['bill_count'] += 1;
      }
      else if ($key1 == 'bill_value') {
        $res1[$val['pl_group']][$key1] += $val1;
        $total['bill_value'] += $val1;
      }
      else if ($key1 == 'sum_kg') {
        $res1[$val['pl_group']][$key1] += $val1;
        $total['sum_kg'] += $val1;
      }
      else {
        $res1[$val['pl_group']][$key1] = $val1;
        $total['others'] = 'Total';
      }
    }
  }
  foreach($total as $key => $val){
      $res1['Total'][$key] = $val;
  }

  $header = array('PL Group','Bill Count','Weight (KGs)','Total Value');

  /*
  $per_page = 10;
  // Initialize the pager
  $current_page = pager_default_initialize(count($res1), $per_page);
  // Split your list into page sized chunks
  $chunks = array_chunk($res, $per_page, TRUE);
  // Show the appropriate items from the list
  $output = theme('table', array('header' => $header, 'rows' => $chunks[$current_page]));
  // Show the pager
  $output .= theme('pager', array('quantity', count($res1)));
*/
  $output = theme('table', array('header' => $header, 'rows' => $res1, 'attributes' => array('width' => '100%')));

  return $form.$output;
}