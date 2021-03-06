<?php

/**
 * @file
 * Definition of example_handler_custom_field
 */
/**
 * Provides a custom views field.
 */

/**
 * Field handler to translate a Status into its readable form.
 */
class hover_data_payment_coll_custom_field extends views_handler_field {

  function option_definition() {
    $options = parent::option_definition();
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
  }

  function query() {
    // do nothing -- to override the parent query.
  }

  function render($values) {
    return get_collection_history_for_hover($values->node_title);
  }
}

function get_collection_history_for_hover($bill_no) {
  global $user;
  $distributor_id = change_feeds_import_get_distributor_id($user->uid);
  $html = '';
  $query = db_select('node', 'n');
  $query->leftjoin('field_data_field_collection_cash_amount', 'cca', 'cca.entity_id = n.nid');
  $query->leftjoin('field_data_field_collection_cheque_num', 'nv', 'nv.entity_id = n.nid');
  $query->leftjoin('field_data_field_collection_cheque_amount', 'chea', 'chea.entity_id = n.nid');
  $query->leftjoin('field_data_field_collection_chq_date', 'chqd', 'chqd.entity_id = n.nid');
  $query->leftjoin('field_data_field_collection_history_date', 'chd', 'chd.entity_id = n.nid');
  $query->addExpression("DATE_FORMAT(chd.field_collection_history_date_value, '%Y-%m-%d')", 'collection_date');
  $query->addExpression('cca.field_collection_cash_amount_value', 'field_collection_cash_amount_value');
  $query->addExpression('chea.field_collection_cheque_amount_value', 'field_collection_cheque_amount_value');
  $query->addExpression('nv.field_collection_cheque_num_value', 'field_collection_cheque_num_value');
  $query->addExpression("DATE_FORMAT(chqd.field_collection_chq_date_value, '%Y-%m-%d')", 'chq_date');
  $query->condition('n.title', $bill_no);
  $query->condition('n.uid', $distributor_id);
  $query->condition('n.type', 'collection_history');
  $result = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
  
  foreach($result as $data) {
    $total_cash_amt += $data['field_collection_cash_amount_value'];
    $total_chq_amt += $data['field_collection_cheque_amount_value'];   
  }

  if(!empty($result)){
    $result['Total'] = array('Total', $total_cash_amt, $total_chq_amt, '', '');
  }
    $header = array('Collection Date', 'Cash Amt', 'Cheque Amt', 'Cheque No', 'Cheque Date');
    $html .= theme('table', array('header' => $header, 'rows' => $result, 'attributes' => array('width' => '50%'), 'empty' => t('No Records has been found')));
  return $html;
}
