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
function _petty_cash_pdf_details_form($form, &$form_state) {
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
       'callback' => '_petty_cash_pdf_ajax',
       'wrapper' => 'petty-cash-div',
       'method' => 'replace',
       'effect' => 'fade',
       'event' => 'change'
     ),
   );
   $form['distributor_id'] = array(
     '#type' => 'value',
     '#value' => $distributor_id,
   );

   if (isset($form_state['values']['voucher']) && !empty($form_state['values']['voucher'])) {
      $voucher = $form_state['values']['voucher'];
      $html = _leanbox_petty_cash_pdf_details($voucher, $distributor_id);
      if (!empty($html)) {
         $form['voucher_details'] = array(
           '#markup' => $html,
         );

         $form['pdf_link'] = array(
           '#markup' => l('Open PDF', "/petty-cash-voucher/generate-pdf/$voucher", array('attributes' => array('target' => '_blank'),)),
         );
         // cache_set($distributor_id . $voucher . '_petty_cash', $html, 'cache', time() + 60 * 10);
      }
   }

   return $form;
}

/**
 * Menu callback for AJAX additions. Render voucher details.
 */
function _petty_cash_pdf_ajax($form, &$form_state) {
   return $form;
}

/**
 * 
 */
function _petty_cash_pdf_generate_form($voucher_arg) {
   $html = '';
   if (!empty($voucher_arg)) {
      // get logged in users distributor id
      $user_id = $GLOBALS['user']->uid;
      $distributor_id = change_feeds_import_get_distributor_id($user_id);
      // fetch data from cache if available
      //$data = cache_get($distributor_id . $voucher_arg . '_petty_cash');
      //if (!empty($data)) {
      //  $html = $data->data;
      //}
      // else {
      $html = _leanbox_petty_cash_pdf_details($voucher_arg, $distributor_id);
      //}
      // return pdf by rendering html
      pdf_using_mpdf_api($html);
   }
   else {
      return $html;
   }
}
