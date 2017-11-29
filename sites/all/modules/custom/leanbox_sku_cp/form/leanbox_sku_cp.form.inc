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
function _assign_sku_cp_callback($form, &$form_state) {
   $form = array();
   $form['#prefix'] = '<div id="petty-cash-div">';
   $form['#suffix'] = '</div>';

   // get logged in users distributor id
   $user_id = $GLOBALS['user']->uid;
   $distributor_id = change_feeds_import_get_distributor_id($user_id);


   $form['distributor_id'] = array(
     '#type' => 'value',
     '#value' => $distributor_id,
   );

   $form['sku'] = array(
     '#type' => 'textfield',
     '#size' => 20,
     '#autocomplete_path' => 'sku-list',
     '#title' => t('Enter SKU No'),
     '#ajax' => array(
       'callback' => '_sku_list_no_ajax',
       'wrapper' => 'itemlist-div',
       'method' => 'replace',
       'effect' => 'fade',
     ),
   );

   if (isset($form_state['values']['sku']) && !empty($form_state['values']['sku'])) {
      $sku = $form_state['values']['sku'];
      $sku_details = _get_sku_details_using_sku_id($sku, $distributor_id);

      if (!empty($sku_details)) {
         $form['sku_details'] = array(
           '#markup' => $sku_details,
           '#title' => t('Sku details'),
         );

         $form['sku_cp'] = array(
           '#type' => 'textfield',
           '#size' => 20,
           '#autocomplete_path' => 'sku-list',
           '#title' => t('Enter SKU No'),
           '#ajax' => array(
             'callback' => '_sku_list_no_ajax',
             'wrapper' => 'itemlist-div',
             'method' => 'replace',
             'effect' => 'fade',
           ),
         );
         if (isset($form_state['values']['sku_cp']) && !empty($form_state['values']['sku_cp'])) {
            $sku_cp = $form_state['values']['sku_cp'];
            $sku_cp_details = _get_sku_details_using_sku_id($sku_cp, $distributor_id);

            if (!empty($sku_cp_details)) {
               $form['sku_details'] = array(
                 '#markup' => $sku_cp_details,
                 '#title' => t('Child Sku details'),
               );

               $form['sku_cp_qty'] = array(
                 '#type' => 'textfield',
                 '#size' => 10,
                 '#maxlength' => 5,
                 '#title' => t('Enter Child SKU Quantity'),
               );

               $form['submit'] = array(
                 '#value' => 'submit',
                 '#type' => 'submit',
                 '#size' => 20,
               );
            }
         }
      }
   }

   return $form;
}

/**
 *  Submit Handler for _assign_sku_cp_callback
 * @param type $form
 * @param type $form_state
 */
function _assign_sku_cp_callback_submit($form, &$form_state) {
   $distributor_id = $form_state['values']['distributor_id'];
   $pod_threshold = $form_state['values']['pod_threshold'];

   if (!empty($distributor_id) && !empty($pod_threshold)) {
      
   }
}

/**
 * Menu callback for AJAX additions. Render sku's list.
 */
function _sku_list_no_ajax($form, &$form_state) {
   return $form;
}

/**
 * 
 * @param type $sku
 */
function _get_sku_details_using_sku_id($sku, $distributor_id) {
   $output = '';
   if (!empty($sku) && !empty($distributor_id)) {
      $query = db_select('node', 'n');
      $query->join('field_data_field_product_name', 'pn', 'pn.entity_id = n.nid');
      $query->condition('n.type', 'sku7_code_master');
      $query->condition('n.uid', $distributor_id);
      $query->condition('n.title', $sku);
      $query->fields('n', array('title'));
      $query->fields('pn', array('field_product_name_value'));
      $query->range(0, 10);
      $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
      if (!empty($res)) {
         $header = array(
           'Billno' => 'Bill No',
           'billdate' => 'Bill date',
         );

         $output = theme('table', array('header' => $header, 'rows' => $rows));
      }
   }
   return $output;
}
