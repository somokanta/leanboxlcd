<?php

/**
 * Files containing form for delete petty cash voucher
 */

/**
 * 
 * @param array $form
 * @param type $form_state
 * @return array
 */
function revert_petty_cash_callback($form, &$form_state) {
   $form = array();
   $form['#prefix'] = '<div id="petty-cash-div">';
   $form['#suffix'] = '</div>';

   // get logged in users distributor id
   $user_id = $GLOBALS['user']->uid;
   $distributor_id = change_feeds_import_get_distributor_id($user_id);

   $form['voucher'] = array(
     '#title' => t('Voucher No'),
     '#type' => 'textfield',
     '#size' => 20,
     '#maxlength' => 20,
     '#autocomplete_path' => 'voucher-no/list',
     '#ajax' => array(
       'callback' => '_petty_cash_revert_ajax',
       'wrapper' => 'petty-cash-div',
       'method' => 'replace',
       'effect' => 'fade',
     ),
   );
   $form['distributor_id'] = array(
     '#type' => 'value',
     '#value' => $distributor_id,
   );
   if (isset($form_state['values']['voucher']) && !empty($form_state['values']['voucher'])) {
      $output = _get_voucher_details_distributor_wise($distributor_id, $form_state['values']['voucher']);

      if (!empty($output)) {
         $html = theme('table', array(
           'header' => $header,
           'rows' => $rows,
           'attributes' => array('width' => '100%')
         ));
         $form['voucher_details'] = array(
           '#markup' => $html,
         );
         $form['voucher_rows'] = array(
           '#type' => 'value',
           '#value' => $rows,
         );
         $form['amount'] = array(
           '#title' => t('Enter Amount'),
           '#type' => 'textfield',
           '#size' => 15,
           '#element_validate' => array('element_validate_integer_positive'),
           '#required' => TRUE,
         );
         $form['submit'] = array(
           '#value' => 'delete',
           '#type' => 'submit',
           '#size' => 20,
         );
      }
   }
   return $form;
}

/**
 * Menu callback for AJAX additions. Render voucher details.
 */
function _petty_cash_revert_ajax($form, &$form_state) {
   return $form;
}

/**
 * Submit handler for form revert_petty_cash_callback
 * @param type $form
 * @param type $form_state
 */
function revert_petty_cash_callback_submit($form, &$form_state) {
   dpm($form_state, 'state');
   if (!empty($form_state['values']['voucher']) && !empty($form_state['values']['distributor_id'])) {
      $old_voucher = $form_state['values']['voucher'];
      $distributor_id = $form_state['values']['distributor_id'];
      $balance = _get_latest_balance($distributor_id);
      $balance = $balance + $form_state['values']['amount'];
      dpm($balance, 'updated $balance');
      $coupon_id = _update_unique_coupon($distributor_id);
      dpm($coupon_id, 'received $coupon_id');
      $arg1 = $form_state['values']['arg'];
      $complete_voucher_no = "revert-$distributor_id-credit-$coupon_id";
      dpm($complete_voucher_no, '$complete_voucher_no');

      $old_voucher_details = $form_state['values']['voucher_rows'];
      dpm($old_voucher_details, '$old_voucher_details');
      /**
        db_insert('petty_cash') // Table name no longer needs {}
        ->fields(array(
        'type' => 'credit',
        'head' => $old_voucher_details['head'],
        'subhead' => $old_voucher_details['subhead'],
        'description' => $old_voucher_details['description'],
        'amount' => $form_state['values']['amount'],
        'voucher_no' => $complete_voucher_no,
        //'voucher_date' => date('Y-m-d'),
        'deleted' => 'yes',
        'voucher_reference_no' => $old_voucher,
        'balance' => $balance,
        'distributor_id' => $distributor_id,
        ))
        ->execute();
       * */
   }
}
