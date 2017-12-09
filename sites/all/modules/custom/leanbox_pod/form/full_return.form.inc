<?php

/**
 * Files containing form for payment collection
 */

/**
 * 
 * @param array $form
 * @param type $form_state
 * @return array
 */
function upload_signed_bill_callback($form, &$form_state) {

   $form = array();
   global $user;

   $arg1 = arg(1); // wheather signed or bounced flag based on URL

   $form['#attached']['library'][] = array('system', 'ui.datepicker');
   $form['#attached']['js'][] = drupal_get_path('module', 'leanbox_payment_collection') . "/js/datepicker.js";

   // Store distributor id in author in each node
   $dis_id = change_feeds_import_get_distributor_id($user->uid);
   if (!empty($dis_id) && !empty($arg1)) {
      $form['#prefix'] = '<div id="itemlist-div">';
      $form['#suffix'] = '</div>';

      // store logged in users distributor id
      $form['dis_id'] = array(
        '#type' => 'value',
        '#value' => $dis_id,
      );
      if (isset($form_state['values']['arg']) == 'ajax') {
         $arg = explode("/", $form_state['complete form']['#action']);
         foreach ($arg as $key => $val) {
            if ($val === 'signed' || $val === 'bounced') {
               $arg1 = $val;
            }
         }
      }
      $form['arg'] = array(
        '#type' => 'value',
        '#value' => $arg1,
      );


      $form['bill_no_wrapper'] = array(
        '#prefix' => '<div id="bill-no">',
        '#suffix' => '</div>',
      );
      $form['bill_no_wrapper1'] = array(
        '#prefix' => '<div id="bill-no-wrapper">',
        '#suffix' => '</div>',
      );
      $form['bill_details_wrapper'] = array(
        '#prefix' => '<div id="bill-details">',
        '#suffix' => '</div>',
      );
//      if ($arg1 === 'signed') {
//         $form['bill_no_wrapper']['bill_no'] = array(
//           '#type' => 'textfield',
//           '#maxlength' => 20,
//           '#size' => 10,
//           //'#required' => TRUE,
//           '#autocomplete_path' => 'bill-no/list',
//           '#title' => t('Enter Bill No'),
//         );
//      }
      // else {
      $form['bill_no_wrapper']['bill_no'] = array(
        '#type' => 'textfield',
        '#maxlength' => 20,
        '#size' => 10,
        //'#required' => TRUE,
        '#autocomplete_path' => 'bill-no/list',
        '#title' => t('Enter Bill No'),
        '#ajax' => array(
          'callback' => '_bounce_bill_no_ajax',
          'wrapper' => 'itemlist-div',
          'method' => 'replace',
          'effect' => 'fade',
        ),
      );
      //   }
      $form['bill_no_wrapper']['bill_html'] = array(
        '#prefix' => '<div>',
        '#suffix' => '</div>',
      );
      }
      }
      
      