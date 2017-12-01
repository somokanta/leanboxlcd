<?php

/**
 * Files containing form for sku cp
 */

/**
 * 
 * @param array $form
 * @param type $form_state
 * @return array
 */
function _assign_sku_cp_callback($form, &$form_state) {
   $form = array();
   $form['#prefix'] = '<div id="itemlist-div">';
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
     '#title' => t('Enter Sku No'),
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

      if (!empty($sku_details['rows'][0])) {
         $form['sku_nid'] = array(
           '#type' => 'value',
           '#value' => $sku_details['rows'][0]['nid'],
         );
         unset($sku_details['rows'][0]['nid']);
         $default_sku_cp = $sku_details['rows'][0]['field_child_sku_nid'];
         unset($sku_details['rows'][0]['field_child_sku_nid']);
         $default_sku_cp_qty = $sku_details['rows'][0]['field_child_sku_qty_value'];
         unset($sku_details['rows'][0]['field_child_sku_qty_value']);

         $output = theme('table', array('header' => $sku_details['header'], 'rows' => $sku_details['rows']));
         $form['sku_details'] = array(
           '#markup' => $output,
           '#title' => t('Sku details'),
         );

         $form['sku_cp'] = array(
           '#type' => 'textfield',
           '#size' => 20,
           '#autocomplete_path' => 'sku-list',
           '#title' => t('Enter Child Sku No'),
           '#default_value' => $default_sku_cp,
           '#ajax' => array(
             'callback' => '_sku_list_no_ajax',
             'wrapper' => 'itemlist-div',
             'method' => 'replace',
             'effect' => 'fade',
           ),
         );
         if ((isset($form_state['values']['sku_cp']) && !empty($form_state['values']['sku_cp'])) || !empty($default_sku_cp)) {

            $sku_cp = $form_state['values']['sku_cp'];
            if (!empty($default_sku_cp)) {
               $sku_cp = sku_cp_get_title_using_nid($default_sku_cp);
               $form['sku_cp']['#default_value'] = $sku_cp;
            }
            
            $sku_cp_details = _get_sku_details_using_sku_id($sku_cp, $distributor_id, TRUE);
            
            if (!empty($sku_cp_details['rows'][0])) {

               $form['sku_cp_nid'] = array(
                 '#type' => 'value',
                 '#value' => $sku_cp_details['rows'][0]['nid'],
               );
               unset($sku_cp_details['rows'][0]['nid']);

               $output = theme('table', array('header' => $sku_cp_details['header'], 'rows' => $sku_cp_details['rows']));

               $form['sku_cp_details'] = array(
                 '#markup' => $output,
                 '#title' => t('Child Sku details'),
               );

               // fix drupal issue in ajax to show input value as default value
               unset($form_state['input']['sku_cp_qty']);
               $form['sku_cp_qty'] = array(
                 '#type' => 'textfield',
                 '#size' => 10,
                 '#required' => TRUE,
                 '#default_value' => $default_sku_cp_qty,
                 '#maxlength' => 5,
                 '#element_validate' => array('element_validate_integer_positive'),
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
   $sku_nid = $form_state['values']['sku_nid'];
   $sku_cp_nid = $form_state['values']['sku_cp_nid'];
   $sku_cp_qty = $form_state['values']['sku_cp_qty'];

   if (!empty($distributor_id) && !empty($sku_nid) && !empty($sku_cp_nid) && !empty($sku_cp_qty)) {
      $sku_node = node_load($sku_nid);
      $sku_node->field_child_sku['und'][0]['nid'] = $sku_cp_nid;
      $sku_node->field_child_sku_qty['und'][0]['value'] = $sku_cp_qty;
      node_save($sku_node);
      drupal_set_message(t('Changes has been saved.'));
   }
}

/**
 * Menu callback for AJAX additions. Render sku's list.
 */
function _sku_list_no_ajax($form, &$form_state) {
   return $form;
}


function sku_cp_get_title_using_nid($nid) {
   $query = db_select('node', 'n');
   $query->condition('n.nid', $nid);
   $query->fields('n', array('title'));
   $field = $query->execute()->fetchField();
   return $field;
}
