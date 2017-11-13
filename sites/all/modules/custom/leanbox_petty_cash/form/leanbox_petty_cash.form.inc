<?php

/**
 * Files containing form for petty cash
 */

/**
 * 
 * @param array $form
 * @param type $form_state
 * @return array
 */
function upload_petty_cash_callback($form, &$form_state) {
   $form = array();
   $form['#prefix'] = '<div id="petty-cash-div">';
   $form['#suffix'] = '</div>';

   // get logged in users distributor id
   $user_id = $GLOBALS['user']->uid;
   $distributor_id = change_feeds_import_get_distributor_id($user_id);

   $arg1 = arg(1); // wheather credit or debit based on URL
   // override arg1 variable
   if (isset($form_state['values']['arg'])) {
      $arg1 = $form_state['values']['arg'];
   }

   // store arg in form_state because arg1 gets change in ajax callback
   $form['arg'] = array(
     '#type' => 'value',
     '#value' => $arg1,
   );
   $form['distributor_id'] = array(
     '#type' => 'value',
     '#value' => $distributor_id,
   );

   $header_options = get_list_of_head_option($arg1, $distributor_id);
   if (!empty($header_options)) {
      $form['head'] = array(
        '#title' => t('Head'),
        '#type' => 'select',
        '#size' => 1,
        '#ajax' => array(
          'callback' => '_petty_cash_subhead_ajax',
          'wrapper' => 'petty-cash-div',
          'method' => 'replace',
          'effect' => 'fade',
        ),
        '#options' => $header_options,
        '#default_value' => '',
        '#required' => TRUE,
      );

      if (isset($form_state['values']['head']) && !empty($form_state['values']['head'])) {

         $selected_header = $form_state['values']['head'];
         $subhead_options = get_list_of_subhead_option_using_head($arg1, $distributor_id, $selected_header);

         $form['subhead'] = array(
           '#title' => t('Subhead'),
           '#type' => 'select',
           '#size' => 1,
           '#ajax' => array(
             'callback' => '_petty_cash_subhead_ajax',
             'wrapper' => 'petty-cash-div',
             'method' => 'replace',
             'effect' => 'fade',
           ),
           '#options' => $subhead_options,
           '#default_value' => '',
           '#required' => TRUE,
         );

         if (isset($form_state['values']['subhead']) && !empty($form_state['values']['head']) && !empty($form_state['values']['subhead'])) {
            $form['description'] = array(
              '#title' => t('Description'),
              '#type' => 'textfield',
              '#size' => 60,
              '#required' => TRUE,
            );
            $form['amount'] = array(
              '#title' => t('Enter Amount'),
              '#type' => 'textfield',
              '#size' => 15,
              '#element_validate' => array('element_validate_integer_positive'),
              '#required' => TRUE,
            );
            $form['person_name'] = array(
              '#title' => t('Person name'),
              '#type' => 'textfield',
              '#size' => 20,
              '#required' => TRUE,
            );
            if ($arg1 === 'debit') {
               $form['bill_available'] = array(
                 '#title' => t('Bill Available'),
                 '#type' => 'radios',
                 '#size' => 20,
                 '#options' => array('yes' => 'Yes', 'no' => 'No'),
                 '#required' => TRUE,
               );
               $form['bill_no'] = array(
                 '#title' => t('Bill No'),
                 '#type' => 'textfield',
                 '#size' => 20,
                 //'#required' => TRUE,
                 '#states' => array(
                   // Hide the settings when the cancel notify checkbox is disabled.
                   'visible' => array(
                     ':input[name="bill_available"]' => array('value' => 'yes'),
                   ),
                 ),
                 '#element_validate' => array('element_validate_require_bill_no'),
               );
            }
            $form['submit'] = array(
              '#value' => 'submit',
              '#type' => 'submit',
              '#size' => 20,
            );
         }
      }
   }
   else {
      drupal_set_message(t('No header found.Kindly upload header first.'), 'error');
   }


   return $form;
}

/**
 * 
 * @param type $element
 * @param type $form_state
 */
function element_validate_require_bill_no($element, &$form_state) {
   $value = $element['#value'];
   if (empty($value) && ($form_state['values']['bill_available'] === 'yes')) {
      form_error($element, t('%name is required.', array(
        '%name' => $element['#title'],
      )));
   }
}

/**
 * Menu callback for AJAX additions. Render subhead details.
 */
function _petty_cash_subhead_ajax($form, &$form_state) {
   return $form;
}

/**
 * 
 * @param type $arg1
 * @param type $distributor_id
 * @return array
 */
function get_list_of_head_option($arg1, $distributor_id) {
   $header = array();
   if (!empty($arg1)) {
      if (empty($distributor_id)) {
         $user_id = $GLOBALS['user']->uid;
         $distributor_id = change_feeds_import_get_distributor_id($user_id);
      }
      $query = db_select('field_data_field_petty_cash_type', 'ct');
      $query->condition('ct.field_petty_cash_type_value', $arg1);
      $query->join('field_data_field_petty_cash_head', 'ch', 'ch.entity_id = ct.entity_id');
      $query->join('field_data_field_petty_cash_distributor', 'cds', 'cds.entity_id = ct.entity_id');
      $query->condition('cds.field_petty_cash_distributor_uid', $distributor_id);
      $query->fields('ch', array('field_petty_cash_head_value'));
      $header = $query->execute()->fetchAllKeyed(0, 0);
      $header[''] = t('-Select-');
   }
   return $header;
}

/**
 * 
 * @param type $arg1
 * @param type $distributor_id
 * @param type $head
 * @return array
 */
function get_list_of_subhead_option_using_head($arg1, $distributor_id, $head) {
   $subheader = array();
   if (!empty($arg1) && !empty($head)) {
      if (empty($distributor_id)) {
         $user_id = $GLOBALS['user']->uid;
         $distributor_id = change_feeds_import_get_distributor_id($user_id);
      }

      $query = db_select('field_data_field_petty_cash_type', 'ct');
      $query->condition('ct.field_petty_cash_type_value', $arg1);
      $query->join('field_data_field_petty_cash_head', 'ch', 'ch.entity_id = ct.entity_id');
      $query->join('field_data_field_petty_cash_subhead', 'csu', 'csu.entity_id = ct.entity_id');
      $query->join('field_data_field_petty_cash_distributor', 'cds', 'cds.entity_id = ct.entity_id');
      $query->condition('cds.field_petty_cash_distributor_uid', $distributor_id);
      $query->condition('ch.field_petty_cash_head_value', $head);

      $query->fields('csu', array('field_petty_cash_subhead_value'));
      $subheader = $query->execute()->fetchAllKeyed(0, 0);
      $subheader[''] = t('-Select-');
   }
   return $subheader;
}

function upload_petty_cash_callback_submit($form, &$form_state) {
   $distributor_id = $form_state['values']['distributor_id'];

   // fetch latest coupon id and update for next id
   $coupon_id = _update_unique_coupon($distributor_id);
   $user_details = leanbox_dashboard_get_username_and_rs_code($distributor_id);
   $rs_code = $user_details['field_third_party_username_value'];
   $arg1 = $form_state['values']['arg'];
   $complete_voucher_no = "$arg1-$rs_code-$coupon_id";
   $balance = _get_latest_balance($distributor_id);

   if ($arg1 === 'credit') {
      $balance = $balance + $form_state['values']['amount'];
   }
   else {
      $balance = $balance - $form_state['values']['amount'];
   }
   
   $bill_available = !empty($form_state['values']['bill_available']) ? $form_state['values']['bill_available'] : NULL;
   $bill_no = !empty($form_state['values']['bill_no']) ? $form_state['values']['bill_no'] : NULL;

   $amount = $form_state['values']['amount'];
   db_insert('petty_cash') // Table name no longer needs {}
       ->fields(array(
         'type' => $arg1,
         'head' => $form_state['values']['head'],
         'subhead' => $form_state['values']['subhead'],
         'description' => $form_state['values']['description'],
         'amount' => $amount,
         'voucher_no' => $complete_voucher_no,
         'person_name' => $form_state['values']['person_name'],
         'voucher_date' => date('Y-m-d'),
         'balance' => $balance,
         'distributor_id' => $distributor_id,
         'bill_available' => $bill_available,
         'bill_no' => $bill_no,
       ))
       ->execute();
   drupal_set_message(t("$amount $arg1 sucessfully"));
}
