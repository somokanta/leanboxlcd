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
      $form['#prefix'] = '<div id="itemlist-div">';
      $form['#suffix'] = '</div>';
      // wheather signed or bounced flag based on URL
//      $form['bill_flag'] = array(
//        '#type' => 'value',
//        '#value' => $arg1,
//      );
      $form['dis_id'] = array(
       '#type' => 'value',
        '#value' => $dis_id,
      );
      
//if(isset($form_state['input']['bill_no'])) {
//   
//}
      $form['bill_no'] = array(
        '#type' => 'textfield',
        '#maxlength' => 20,
        '#size' => 10,
        '#default_value' => '',
         '#prefix' => '<div id="bill-no">',
        '#suffix' => '</div>',
        //'#required' => TRUE,
        '#autocomplete_path' => 'bill-no/list',
        '#title' => t('Enter Bill No'),
          '#ajax' => array(
             'wrapper' => 'bill-no',
             'callback' => 'bill_no_remove_callback',
             'method' => 'replace',
             'effect' => 'fade',
           ),
      );
      $form['bill_details_wrapper'] = array(
        '#prefix' => '<div id="bill-details">',
        '#suffix' => '</div>',
      );

      $form['bill_details_wrapper']['add_bill'] = array(
        '#type' => 'submit',
        '#value' => t('Add Bill'),
        '#ajax' => array(
          'callback' => 'get_bill_details_using_bill_no',
          'wrapper' => 'bill-details',
          'method' => 'replace',
          'effect' => 'fade',
        ),
        '#submit' => array('add_bill_in_screen_submit'),
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



      $selected_bill_no = $form_state['values']['bill_no'];
      if (!empty($selected_bill_no) && !empty($form_state['bills']['header']) && !empty($form_state['bills']['rows'])) {

         $form['bill_details_wrapper']['bill_details'] = array(
           '#type' => 'tableselect',
           '#header' => $form_state['bills']['header'],
           '#options' => $form_state['bills']['rows'],
           '#empty' => t('No content available.'),
         );

         $form['bill_details_wrapper']['bill_remove'] = array(
           '#type' => 'submit',
           '#value' => t('Remove Flag'),
           '#submit' => array('remove_signed_bounced_flag'),        
           //'#limit_validation_errors' => array(array('bill_details')),
           '#ajax' => array(
             'wrapper' => 'bill-details',
             'callback' => 'bill_details_remove_callback',
             'method' => 'replace',
             'effect' => 'fade',
           ),
         );
         $form['bill_details_wrapper']['submit'] = array(
           '#type' => 'submit',
           '#value' => t('Save'),
         );
      }
      if (!isset($form_state['bills']['rows'])) {
         $form_state['bills']['rows'] = array();
      }
   }

   return $form;
}

/**
 * Validate Handler for upload_signed_bill_callback
 * @param type $form
 * @param type $form_state
 */
function upload_signed_bill_callback_validate($form, &$form_state) {
   $arg1 = arg(1); // passed arg from URL
   $dis_id = $form_state['values']['dis_id'];  // logged in users dis id stored in form
   // run foreach loop for all bills shown in table select
   // to fetch if any of these bill already a signed bill 
   // then show error
   foreach ($form_state['values']['bill_details'] as $bill_no => $val) {
      if (!empty($bill_no)) {
         $query = db_select('field_data_field_sales_bill_no_ref', 'br');
         $query->join('node', 'n', 'n.nid = br.entity_id');
         $query->condition('n.type', 'payment_collection');
         $query->fields('br', array('entity_id'));
         $query->condition('n.title', $bill_no);
         $query->condition('n.uid', $dis_id);
         $payment_collection_nid = $query->execute()->fetchField();

         $from_state['values']['bounced_nid'][] = $payment_collection_nid;

         if (!empty($payment_collection_nid) && $arg1 === 'signed') {
            $node = node_load($payment_collection_nid);
            $node_flag = $node->field_flag['und'][0]['value'];
            if ($node_flag === 'signed') {
               form_set_error('form', t("Bill no $bill_no already have signed flag."));
            }
         }
      }
   }
}

/**
 * Submit Handler for upload_signed_bill_callback
 * @param type $form
 * @param type $form_state
 */
function upload_signed_bill_callback_submit($form, &$form_state) {
   // write node creation or update (in case of bounced) code here
   $dis_id = $form_state['values']['dis_id'];
   $arg1 = arg(1);

   foreach ($form_state['values']['bill_details'] as $bill_no => $val) {
      if (!empty($bill_no)) {
         $node = new stdClass();
         $node->title = $bill_no; // fetch or pass bill no here
         $node->type = "payment_collection";
         node_object_prepare($node); // Sets some defaults. Invokes hook_prepare() and hook_node_prepare().
         $node->language = LANGUAGE_NONE; // Or e.g. 'en' if locale is enabled
         $node->uid = $dis_id;
         $node->status = 1; //(1 or 0): published or not
         $node->comment = 0; // 0 = comments disabled, 1 = read only, 2 = read/write
         $node->field_declaration_date[LANGUAGE_NONE][0]['value'] = date('Y-m-d');
         if ($arg1 === 'bounced') {
            $node->field_cheque_date[$node->language][0]['value'] = $dispatch_bill_count;
            $node->field_cheque_number[$node->language][0]['value'] = $dispatch_bill_count;
            $node->field_cheque_amt[$node->language][0]['value'] = $dispatch_bill_count;
         }
         $node->field_flag[LANGUAGE_NONE][0]['value'] = $arg1;
         $node->field_sales_bill_no_ref[LANGUAGE_NONE][0]['nid'] = $bill_no_nid;
         node_save($node);
         drupal_set_message(t('Collection content uploded sucessfully.'));
      }
   }
}

/**
 * Menu callback for AJAX additions. Render bill details.
 */
function get_bill_details_using_bill_no($form, &$form_state) {
   return $form['bill_details_wrapper'];
}

/**
 * Menu callback for AJAX additions. Render bill details.
 */
function bill_details_remove_callback($form, &$form_state) {
   return $form['bill_details_wrapper'];
}

/**
 * Menu callback for AJAX additions. Render bill details.
 */
function bill_no_remove_callback() {
    return $form['bill_no'];
}
/**
 * submit handler when ajax is triggered
 * @param type $form
 * @param type $form_state
 */
function add_bill_in_screen_submit(&$form, &$form_state) {
   global $user;
   $arg1 = arg(1);
   // Store distributor id in author in each node
   $dis_id = change_feeds_import_get_distributor_id($user->uid);
   $bill_number = $form_state['values']['bill_no'];

   $bill_details_html = get_bill_details_using_bill_no_callback($bill_number, $dis_id, $arg1);
   $form_state['bills']['header'] = $bill_details_html['header'];
   $form_state['bills']['rows'] += $bill_details_html['rows'];
   $form_state['rebuild'] = TRUE;
}

function get_bill_details_using_bill_no_callback($bill_no_nid, $dis_id, $arg1) {
   // first parameter passed is nid of sales register
   $html = array();
   if (!empty($bill_no_nid) && !empty($dis_id)) {
      $query = db_select('node', 'n');
      $query->addField('n', 'title', 'title');
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
      $rows = $query->execute()->fetchAll(PDO::FETCH_GROUP | PDO::FETCH_UNIQUE | PDO::FETCH_ASSOC);
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
   //dpm($html, '$html');
   return $html;
}

/**
 * 
 * @param type $form
 * @param type $form_state
 */
function remove_signed_bounced_flag($form, &$form_state) {
   dpm($form_state, '$form_state');

   $checked = array_filter($form_state['values']['bill_details']);
   if (!empty($checked)) {
      $form_state['bills']['rows'] = array_diff_key($form_state['bills']['rows'], array_flip($checked));
   }
   drupal_get_messages();
   $form_state['rebuild'] = TRUE;
//   $query = db_select('field_data_field_sales_bill_no_ref', 'br');
//   $query->fields('br', array('entity_id'));
//   $query->condition('br.field_sales_bill_no_ref_nid', $bill_no_nid);
//   $payment_collection_nid = $query->execute()->fetchField();
//   if (!empty($payment_collection_nid)) {
//      $node = node_load($payment_collection_nid);
//      unset($node->field_flag['und']);
//      drupal_set_message(t('Removed flag'));
//   }
}
