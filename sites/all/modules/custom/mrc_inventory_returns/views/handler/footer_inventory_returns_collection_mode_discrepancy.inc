<?php

/**
 * @file
 * Definition of example_handler_custom_field
 */

/**
 * Provides a custom views field.
 */
class footer_inventory_returns_collection_mode_discrepancy extends views_handler_field {

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

  function render($empty = FALSE) {
    $trip_number = $this->view->args[0];

    $user_id = $GLOBALS['user']->uid;
    $distributor_id = change_feeds_import_get_distributor_id($user_id);
    $payment_terms_not_allowed = array('sign bill','cash/chq/sb');
    $query = db_select('dispatch_planning_api_data', 'ad');
    $query->condition('ad.delivery_pending_flag', 1, '!=');
    $query->condition('ad.distributor_id', $distributor_id, '=');
    $query->condition('ad.runsheet_number', $trip_number, '=');
    $query->leftjoin('node', 'n', 'ad.party_code = n.title');
    $query->condition('n.type', 'outlet_master');
    $query->condition('n.uid', $distributor_id, '=');
    $query->leftjoin('field_data_field_payment_terms', 'pt', 'pt.entity_id = n.nid ');

    $query->addExpression("case when pt.field_payment_terms_value !=  '' then LOWER(pt.field_payment_terms_value) else 'NA' END", "field_payment_terms_value");
    //$query->havingCondition('field_payment_terms_value', 'sign bill', '!=');
    $query->havingCondition('field_payment_terms_value', $payment_terms_not_allowed, 'NOT IN');
    
    //$query->addExpression('CONVERT(ad.res_confirm_signed_bill_amount, SIGNED)', 'res_confirm_signed_bill_amount');
    //$query->havingCondition('res_confirm_signed_bill_amount', 0, '>');
    $query->fields('ad', array('bill_number'));
    $query->fields('ad', array('party_code'));
    $query->fields('ad', array('party_name'));
    $query->fields('pt', array('field_payment_terms_value'));
    $query->fields('ad', array('res_confirm_signed_bill_amount'));
    $query->fields('ad', array('res_confirm_signed_bill_amount_edit'));
    $query->fields('ad', array('res_money_collect2_edit'));
    $query->fields('ad', array('res_signed_bill_delivery'));
    $query->fields('ad', array('res_signed_bill_partial'));
    $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

    $count = 1;
    $rows = array();
    foreach ($res as $val) {
      if (empty($val['res_money_collect2_edit']) && ($val['res_signed_bill_delivery'] == 'Yes' || $val['res_signed_bill_partial'] == 'Yes')) {
        $row['counter'] = $count++;
        $row['bill_number'] = $val['bill_number'];
        $row['party_code'] = $val['party_code'];
        $row['party_name'] = $val['party_name'];
        $row['payment_term'] = $val['field_payment_terms_value'];
        $row['collection_mode'] = 'Signed Bill';
        $row['collected_value'] = $val['res_confirm_signed_bill_amount'];

        $total_collected_value += $val['res_confirm_signed_bill_amount'];
        $rows[] = $row;
      }
      else if ($val['res_money_collect2_edit'] == 'signed') {
        $row['counter'] = $count++;
        $row['bill_number'] = $val['bill_number'];
        $row['party_code'] = $val['party_code'];
        $row['party_name'] = $val['party_name'];
        $row['payment_term'] = $val['field_payment_terms_value'];
        $row['collection_mode'] = 'Signed Bill';
        $row['collected_value'] = (!empty($val['res_confirm_signed_bill_amount_edit'])) ? $val['res_confirm_signed_bill_amount_edit'] : $val['res_confirm_signed_bill_amount'];

        $total_collected_value += (!empty($val['res_confirm_signed_bill_amount_edit'])) ? $val['res_confirm_signed_bill_amount_edit'] : $val['res_confirm_signed_bill_amount'];
        $rows[] = $row;
      }
    }

    if (!empty($rows)) {
      $rows['total'] = array('Total', '', '', '', '', '', $total_collected_value);
      $header = array('Sr No.', 'Bill Number', 'Party Code', 'Party Name', 'Payment Terms', 'Collection Mode', 'Collected Value');
      $output = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('border' => '1', 'cellpadding' => '5', 'cellspacing' => '0')));
    }
    else {
      $output = 'No Result Found';
    }

    return $output . "<br/>\n";
  }

}
