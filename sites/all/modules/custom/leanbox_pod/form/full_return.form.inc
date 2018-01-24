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
function _declare_fr_flag_callback($form, &$form_state) {
   $form = array();
   global $user;
   if (!isset($form_state['bills']['rows'])) {
      $form_state['bills']['rows'] = array();
   }

   // Store distributor id in author in each node
   $dis_id = change_feeds_import_get_distributor_id($user->uid);
   if (!empty($dis_id)) {
      $form['#prefix'] = '<div id="itemlist-div">';
      $form['#suffix'] = '</div>';

      // store logged in users distributor id
      $form['dis_id'] = array(
        '#type' => 'value',
        '#value' => $dis_id,
      );

      $form['bill_no_wrapper']['bill_no'] = array(
        '#type' => 'textfield',
        '#maxlength' => 20,
        '#size' => 10,
        '#autocomplete_path' => 'bill-no/list',
        '#title' => t('Enter Bill No'),
        '#ajax' => array(
          'callback' => 'get_sales_bill_details_using_bill_no',
          'wrapper' => 'itemlist-div',
          'method' => 'replace',
          'effect' => 'fade',
        ),
      );

      $bill_no = $form_state['input']['bill_no'];
      if (!empty($bill_no)) {
         $form['bill_no_wrapper']['add_bill'] = array(
           '#type' => 'submit',
           '#value' => t('Add Bill'),
           '#ajax' => array(
             'callback' => 'get_sales_bill_details_using_bill_no',
             'wrapper' => 'itemlist-div',
             'method' => 'replace',
             'effect' => 'fade',
           ),
           '#submit' => array('add_sales_bill_in_screen_submit'),
         );
      }
      if (!empty($form_state['bills']['rows'])) {
         $form['bill_no_wrapper']['bill_details_html'] = array(
           '#prefix' => '<div>',
           '#suffix' => '</div>',
         );
         $form['bill_details_wrapper']['bill_details'] = array(
           '#type' => 'tableselect',
           '#header' => $form_state['bills']['header'],
           '#options' => $form_state['bills']['rows'],
           '#empty' => t('No content available.'),
         );
         $form['bill_details_wrapper']['bill_remove'] = array(
           '#type' => 'submit',
           '#value' => t('Remove Bill'),
           '#submit' => array('remove_sales_bill_flag'),
           '#limit_validation_errors' => array(array('bill_details')),
           '#ajax' => array(
             'wrapper' => 'itemlist-div',
             'callback' => 'sales_bill_remove_callback',
             'method' => 'replace',
             'effect' => 'fade',
           ),
         );
         $form['bill_details_wrapper']['submit'] = array(
           '#type' => 'submit',
           '#value' => t('Save'),
         );
      }
   }
   return $form;
}

/**
 * Menu callback for AJAX additions. Render bill details.
 */
function get_sales_bill_details_using_bill_no($form, &$form_state) {
   return $form;
}

/**
 * Menu callback for AJAX additions. Render bill details.
 */
function sales_bill_remove_callback($form, &$form_state) {
   return $form;
}

/**
 * 
 * @param type $form
 * @param type $form_state
 */
function remove_sales_bill_flag($form, &$form_state) {
   // removing if bill_details array values is not selected (means having zero value)
   $checked = array_filter($form_state['values']['bill_details']);
   if (!empty($checked)) {
      $form_state['bills']['rows'] = array_diff_key($form_state['bills']['rows'], array_flip($checked));
   }
   $form_state['rebuild'] = TRUE;
}

/**
 * submit handler when ajax is triggered
 * @param type $form
 * @param type $form_state
 */
function add_sales_bill_in_screen_submit(&$form, &$form_state) {
   global $user;

   // Store distributor id in author in each node
   $dis_id = change_feeds_import_get_distributor_id($user->uid);
   $bill_number = $form_state['values']['bill_no'];
   unset($form_state['input']['bill_no']);
   $bill_details_html = get_sales_bill_details_using_bill_no_callback($bill_number, $dis_id);
   $form_state['bills']['header'] = $bill_details_html['header'];
   if(!empty($bill_details_html['rows'])) {
     $form_state['bills']['rows'] += $bill_details_html['rows'];
   }
   $form_state['rebuild'] = TRUE;
}

function get_sales_bill_details_using_bill_no_callback($bill_no, $dis_id) {
   $array = array();
   if (!empty($bill_no) && !empty($dis_id)) {
      $query = db_select('node', 'n');
      $query->condition('n.type', 'sales_register_data');
      $query->addField('n', 'title', 'title');
      $query->addField('n', 'title', 'Bill no');
            $query->addField('n', 'nid', 'nid');

      $query->join('field_data_field_sr_bill_date', 'sr', 'sr.entity_id = n.nid');
      $query->addExpression("DATE_FORMAT(CONVERT_TZ(FROM_UNIXTIME(sr.field_sr_bill_date_value),'+00:00','+05:30'), '%d-%b-%Y')", 'bill date');
      $query->leftjoin('field_data_field_party_code_reference', 'pc', 'pc.entity_id = n.nid');
      $query->join('node', 'n1', 'n1.nid = field_party_code_reference_nid');
      $query->addField('n1', 'title', 'Party code');
      $query->leftjoin('field_data_field_beat_name', 'bn', 'bn.entity_id = n1.nid');
      $query->addField('bn', 'field_beat_name_value', 'Beat name');
      $query->leftjoin('field_data_field_salesman', 'fs', 'fs.entity_id = n1.nid');
      $query->addField('fs', 'field_salesman_value', 'Salesman');
      $query->leftjoin('field_data_field_salesman_code', 'sc', 'sc.entity_id = n1.nid');
      $query->addField('sc', 'field_salesman_code_value', 'Salesman code');
      $query->join('field_data_field_bill_value', 'bv', 'bv.entity_id = n.nid');
      $query->addField('bv', 'field_bill_value_value', 'Bill value');
      $query->condition('n.title', $bill_no);
      $query->condition('n.uid', $dis_id);
      $rows = $query->execute()->fetchAll(PDO::FETCH_UNIQUE | PDO::FETCH_ASSOC);

      $header = array(
        'Billno' => 'Bill No',
        'nid' => 'nid',
        'billdate' => 'Bill date',
        'Partycode' => 'Party code',
        'Beatname' => 'Beat name',
        'Salesman' => 'Salesman',
        'Salesmancode' => 'Salesman code',
        'Billvalue' => 'Bill value',
      );


      $array['header'] = $header;
      $array['rows'] = $rows;
   }
   return $array;
}

/**
 * Submit Handler for upload_signed_bill_callback
 * @param type $form
 * @param type $form_state
 */
function _declare_fr_flag_callback_submit($form, &$form_state) {
   // write node creation code here
   $dis_id = $form_state['values']['dis_id'];
   foreach ($form_state['values']['bill_details'] as $bill_no => $val) {
      if (!empty($bill_no)) {
         $query = db_select('node', 'n');
         $query->condition('n.type', 'sales_register_data');
         $query->addField('n', 'nid', 'sales_register_nid');
         $query->condition('n.title', $bill_no);
         $query->condition('n.uid', $dis_id);
         $nid = $query->execute()->fetchField();
         if (!empty($nid)) {
            $node = node_load($nid);
            $node->field_call_type[LANGUAGE_NONE][0]['value'] = 'fr'; // full returned flag
            node_save($node);
            drupal_set_message(t("Bill Declared as Full Returned Flag Successfully."));
         }
      }
   }
}
