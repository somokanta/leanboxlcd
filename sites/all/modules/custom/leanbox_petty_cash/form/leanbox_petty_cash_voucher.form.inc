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
     //'#maxlength' => 20,
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
      if (!empty($output['rows'])) {
         $form['old_voucher_id'] = array(
           '#type' => 'value',
           '#value' => $form_state['values']['voucher'],
         );
         $form['old_voucher_table_id'] = array(
           '#type' => 'value',
           '#value' => $output['rows'][0]['id'],
         );

         unset($output['rows'][0]['id']);
         $html = theme('table', array(
           'header' => $output['header'],
           'rows' => $output['rows'],
           'attributes' => array('width' => '100%')
         ));
         $form['voucher_details'] = array(
           '#markup' => $html,
         );
         $form['voucher_rows'] = array(
           '#type' => 'value',
           '#value' => $output['rows'],
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
   if (!empty($form_state['values']['old_voucher_id']) && !empty($form_state['values']['distributor_id'])) {
      $old_voucher = $form_state['values']['old_voucher_id'];
      $distributor_id = $form_state['values']['distributor_id'];
      $balance = _get_latest_balance($distributor_id);
      $coupon_id = _update_unique_coupon($distributor_id);
      $arg1 = $form_state['values']['arg'];
      $user_details = leanbox_dashboard_get_username_and_rs_code($distributor_id);
      $rs_code = $user_details['field_third_party_username_value'];
      $complete_voucher_no = "delete-$rs_code-$coupon_id";

      $old_voucher_details = $form_state['values']['voucher_rows'];
      $old_amount = $old_voucher_details[0]['amount'];

      if ($old_voucher_details[0]['type'] === 'credit') {
         $type = 'delete';
         // credit voucher is deleted
         $balance = $balance - $old_amount;
      }
      else {
         $type = 'delete';
         $balance = $balance + $old_amount;
      }

      $old_voucher_serial_id = $form_state['values']['old_voucher_table_id'];
      if (!empty($old_voucher_serial_id)) {
         db_insert('petty_cash') // Table name no longer needs {}
             ->fields(array(
               'type' => $type,
               'head' => $old_voucher_details[0]['head'],
               'subhead' => $old_voucher_details[0]['subhead'],
               'description' => $old_voucher_details[0]['description'],
               'amount' => $old_amount,
               'voucher_no' => $complete_voucher_no,
               'voucher_date' => time(),
               'voucher_reference_no' => $old_voucher,
               'balance' => $balance,
               'person_name' => $old_voucher_details[0]['person_name'],
               'distributor_id' => $distributor_id,
             ))
             ->execute();
         drupal_set_message(t("$old_amount Reverted sucessfully."));
         drupal_set_message(t("$complete_voucher_no generated sucessfully."));
         //  update file id in petty cash table

         db_update('petty_cash')
             ->fields(array('deleted' => 'yes'))
             ->condition('id', $old_voucher_serial_id)
             ->execute();
      }
   }
}
