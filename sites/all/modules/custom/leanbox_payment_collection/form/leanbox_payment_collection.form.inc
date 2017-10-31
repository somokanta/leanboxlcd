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
   $arg1 = arg(1);

   // Store distributor id in author in each node
   $dis_id = change_feeds_import_get_distributor_id($user->uid);
   if (!empty($dis_id) && !empty($arg1)) {
      $form['bill_no'] = array(
        '#type' => 'textfield',
        '#maxlength' => 20,
        '#size' => 10,
        '#required' => TRUE,
        '#autocomplete_path' => 'bill-no/list',
        '#title' => t('Enter Bill No'),
        '#ajax' => array(
          'callback' => 'get_bill_details_using_bill_no',
          'wrapper' => 'bill-details',
          'method' => 'replace',
          'effect' => 'fade',
        ),
      );
      $form['bill_details_wrapper'] = array(
        '#prefix' => '<div id="bill-details">',
        '#suffix' => '</div>',
      );
      // wheather signed or bounced flag based on URL
      $form['bill_flag'] = array(
        '#hidden' => TRUE,
        '#value' => $arg1,
      );
      $form['dis_id'] = array(
        '#hidden' => TRUE,
        '#value' => $dis_id,
      );
      if ($arg1 === 'bounced') {
         // if arg is bounced then we have to collect cheque no and cheque date
         $form['cheque_no'] = array(
           '#type' => 'textfield',
           '#title' => t('Cheque number'),
           '#size' => 20,
           '#maxlength' => 15,
         );
         $form['cheque_date'] = array(
           '#type' => 'date_popup',
           '#default_value' => date('Y-m-d'),
           '#title' => t('Cheque date'),
         );
      }

      $selected_bill_no = $form_state['input']['bill_no'];
      if (!empty($selected_bill_no)) {

         $bill_details_html = get_bill_details_using_bill_no_callback($selected_bill_no, $dis_id, $arg1);
         $form['bill_details_wrapper']['bill_details'] = array(
           '#type' => 'tableselect',
           '#header' => $bill_details_html['header'],
           '#options' => $bill_details_html['rows'],
           '#empty' => t('No content available.'),
         );

         $form['bill_details_wrapper']['bill_detail_submit'] = array(
           '#type' => 'submit',
           '#value' => t('Remove Flag'),
           '#submit' => array('remove_signed_bounced_flag'),
         );
      }

      $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save'),
      );
   }

   return $form;
}

/**
 * Validate Handler for upload_signed_bill_callback
 * @param type $form
 * @param type $form_state
 */
function upload_signed_bill_callback_validate($form, &$form_state) {
   $bill_no_nid = $form_state['values']['bill_no'];
   $arg1 = $form_state['values']['bill_flag'];
   $query = db_select('field_data_field_sales_bill_no_ref', 'br');
   $query->fields('br', array('entity_id'));
   $query->condition('br.field_sales_bill_no_ref_nid', $bill_no_nid);
   $payment_collection_nid = $query->execute()->fetchField();
   $from_state['values']['bounced_nid'] = $payment_collection_nid;
   if (!empty($payment_collection_nid) && $arg1 === 'signed') {
      $node = node_load($payment_collection_nid);
      $node_flag = $node->field_flag['und'][0]['value'];
      if ($node_flag === 'signed') {
         form_set_error('bill_no', t('bill already have signed flag.'));
      }
   }
   if (($form_state['clicked_button']['#value'] === 'Remove Flag') && empty($payment_collection_nid)) {
      form_set_error('bill_no', t('No Bill Found in Collection.'));
   }
}

/**
 * Submit Handler for upload_signed_bill_callback
 * @param type $form
 * @param type $form_state
 */
function upload_signed_bill_callback_submit($form, &$form_state) {
   // write node creation or update (in case of bounced) code here
   $bill_no_nid = $form_state['values']['bill_no'];
   $dis_id = $form_state['values']['dis_id'];
   $arg1 = $form_state['values']['bill_flag'];
   $bounced_nid = $from_state['values']['bounced_nid'];
   if (!empty($bounced_nid) && $arg1 === 'bounced') {
      // write a code to update here
   }
   else {

      $node = new stdClass();

      // vvviiiimmmpppp
      $node->title = $bill_no; // fetch or pass bill no here


      $node->type = "payment_collection";
      node_object_prepare($node); // Sets some defaults. Invokes hook_prepare() and hook_node_prepare().
      $node->language = LANGUAGE_NONE; // Or e.g. 'en' if locale is enabled
      $node->uid = $dis_id;
      $node->status = 1; //(1 or 0): published or not
      $node->comment = 0; // 0 = comments disabled, 1 = read only, 2 = read/write
      $node->field_declaration_date[LANGUAGE_NONE][0]['value'] = date('Y-m-d');
      if($arg1 === 'bounced') {
          $node->field_bill_count[$node->language][0]['value'] = $dispatch_bill_count;
      $node->field_bill_count[$node->language][0]['value'] = $dispatch_bill_count;
      }
     
      $node->field_bill_count[$node->language][0]['value'] = $dispatch_bill_count;
      $node->field_bill_count[$node->language][0]['value'] = $dispatch_bill_count;
      $node->field_bill_count[$node->language][0]['value'] = $dispatch_bill_count;
      $node->field_bill_count[$node->language][0]['value'] = $dispatch_bill_count;
      $node->field_sales_bill_no_ref[LANGUAGE_NONE][0]['nid'] = $bill_no_nid;
   }
}

/**
 * Menu callback for AJAX additions. Render bill details.
 */
function get_bill_details_using_bill_no($form, $form_state) {
   return $form['bill_details_wrapper'];
}

function get_bill_details_using_bill_no_callback($bill_no_nid, $dis_id, $arg1) {
   // first parameter passed is nid of sales register
   $html = array();

   if (!empty($bill_no_nid) && !empty($dis_id)) {
      $query = db_select('node', 'n');
      $query->addField('n', 'title', 'Bill no');
      $query->join('field_data_field_sr_bill_date', 'sr', 'sr.entity_id = n.nid');
      $query->addExpression("DATE_FORMAT(CONVERT_TZ(FROM_UNIXTIME(sr.field_sr_bill_date_value),'+00:00','+05:30'), '%Y/%m/%d')", 'bill date');
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
      $query->condition('n.nid', $bill_no_nid);
      $query->condition('n.uid', $dis_id);
      $rows = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

      $header = array(
        'Billno' => 'Bill No',
        'billdate' => 'Bill date',
        'Partycode' => 'Party code',
        'Beatname' => 'Beat name',
        'Salesman' => 'Salesman',
        'Salesmancode' => 'Salesman code',
        'Billvalue' => 'Bill value');

      $html['header'] = $header;
      $html['rows'] = $rows;
   }
   return $html;
}

/**
 * 
 * @param type $form
 * @param type $form_state
 */
function remove_signed_bounced_flag($form, &$form_state) {
   $bill_no_nid = $form_state['values']['bill_no'];

   $query = db_select('field_data_field_sales_bill_no_ref', 'br');
   $query->fields('br', array('entity_id'));
   $query->condition('br.field_sales_bill_no_ref_nid', $bill_no_nid);
   $payment_collection_nid = $query->execute()->fetchField();
   if (!empty($payment_collection_nid)) {
      $node = node_load($payment_collection_nid);
      unset($node->field_flag['und']);
      drupal_set_message(t('Removed flag'));
   }
   $form_state['rebuild'] = TRUE;
}
