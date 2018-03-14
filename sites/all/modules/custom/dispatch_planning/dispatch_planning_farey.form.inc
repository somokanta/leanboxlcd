<?php

/**
 * Files containing form for Trip returned from Farey Delivered Trip API
 */

/**
 * 
 * @param array $form
 * @param type $form_state
 * @return array
 */
function _delivered_trip_data_callback($form, &$form_state) {
   $form = array();
   $form['#prefix'] = '<div id="itemlist-div">';
   $form['#suffix'] = '</div>';

   // get logged in users rs id
   $user_id = $GLOBALS['user']->uid;
   $rs_code = dispatch_planning_get_rscode_using_uid($user_id);

   $form['trip_number'] = array(
     '#prefix' => '<div class="textfield-wrapper">',
     '#type' => 'textfield',
     '#title' => t('Trip'),
     '#required' => TRUE,
     '#autocomplete_path' => 'trip/autocomplete',
     '#ajax' => array(
       'wrapper' => 'itemlist-div',
       'callback' => 'trip_details_remove_callback',
       'method' => 'replace',
       'effect' => 'fade',
     ),
   );

   $trip_number = $form_state['input']['trip_number'];

   if (!empty($trip_number) && !empty($rs_code)) {
      $trip_details = _get_farey_delivered_trip_details($trip_number, $rs_code);

      if (!empty($trip_details['rows'])) {
         $trip_details['header'] = array('SKU7Code', 'Description', 'Free Product', 'Value/Unit', 'Sum of Net Sales Qty', 'Actual Qty', 'Returned Qty');

         $form['trip_details'] = array(
           '#theme' => 'table',
           '#header' => $trip_details['header'],
           '#rows' => $trip_details['rows'],
         );

         
    $data = array();

      $data['trip_number'] = $trip_number;
    


         $output = l(t('XLS'), 'delivered-trip-data/export', array('query' => array($data)));
         $form['trip_details_download'] = array(
           '#markup' => $output,
         );
      }
      else {
         drupal_set_message(t('No result Found'), 'error');
      }
   }
   

   return $form;
}

function _delivered_trip_data_download_callback() {

   if (isset($_GET['trip_number']) && !empty($_GET['trip_number'])) {
      $trip_number = $_GET['trip_number'];
      // get logged in users rs id
      $user_id = $GLOBALS['user']->uid;
      $rs_code = dispatch_planning_get_rscode_using_uid($user_id);
      $trip_details = _get_farey_delivered_trip_details($trip_number, $rs_code);
      
      $header = array('SKU7Code', 'Description', 'Free Product', 'Value/Unit', 'Sum of Net Sales Qty', 'Actual Qty', 'Returned Qty');
      $rows = $trip_details['rows'];
      $output = theme('table', array(
        'header' => $header,
        'rows' => $rows,
        'attributes' => array('width' => '100%')
      ));
      $file = "fareydeliveredtrip.xls";
      header("Content-type: application/vnd.ms-excel");
      header("Content-Disposition: attachment; filename=$file");
      echo $output;
      exit;
   }
}


/**
 * 
 * @param type $trip_number
 * @param type $rs_code
 */
function _get_farey_delivered_trip_details($trip_number, $rs_code) {
   global $user;
   $dis_id = change_feeds_import_get_distributor_id($user->uid);

   $query = db_select('dispatch_planning_item_details', 'id');
   $query->join('node', 'n', 'n.title = id.sku_code');
   $query->condition('n.uid', $dis_id);
   $query->condition('n.type', 'sku7_code_master');
   $query->leftjoin('field_data_field_child_sku', 'chs', 'chs.entity_id = n.nid');
   $query->leftjoin('node', 'n1', 'n1.nid = chs.field_child_sku_nid');
   $query->join('dispatch_planning_api_data', 'ad', 'id.trip_bill_reference = ad.id');
   $query->condition('ad.runsheet_number', $trip_number, '=');
   $query->condition('ad.hub', $rs_code, '=');
   $query->condition('id.og_sum_net_sales_quantity', 0, '!=');
   $query->condition('id.res_sku_actual_quantity1', '', '!=');
   $query->condition('id.res_sku_actual_quantity1', 'NULL', '!=');
   $query->fields('id', array('sku_code', 'sku_description'));
   $query->addField('n1', 'title', 'Child squ');
   $query->addField('id', 'sum_net_sales_value', 'value per unit');
   $query->groupBy('id.sku_code');
   $query->addExpression('SUM(og_sum_net_sales_quantity)', 'Sum_of_net_sales_qty');
   $query->addExpression('SUM(res_sku_actual_quantity1)', 'Actual_qty_delivered');
   $query->addExpression('(SUM(og_sum_net_sales_quantity) - SUM(res_sku_actual_quantity1))', 'Returned_qty');
   $query->havingCondition('Returned_qty', 0, '>');
   $rows['rows'] = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
   return $rows;
}

/**
 * 
 * @param type $form
 * @param type $form_state
 * @return array
 */
function trip_details_remove_callback($form, &$form_state) {
   return $form;
}


/**
 * 
 * @param array $form
 * @param type $form_state
 * @return array
 */
function _edit_trip_delivered_callback_form($form, &$form_state) {
   $form = array();
   $form['#prefix'] = '<div id="itemlist-div">';
   $form['#suffix'] = '</div>';

   // get logged in users rs id
   $user_id = $GLOBALS['user']->uid;
   $rs_code = dispatch_planning_get_rscode_using_uid($user_id);

   $form['trip_number'] = array(
     '#prefix' => '<div class="textfield-wrapper">',
     '#type' => 'textfield',
     '#title' => t('Trip'),
     '#required' => TRUE,
     '#autocomplete_path' => 'trip/autocomplete',
     '#ajax' => array(
       'wrapper' => 'itemlist-div',
       'callback' => 'trip_details_remove_callback',
       'method' => 'replace',
       'effect' => 'fade',
     ),
   );
   

   $trip_number = $form_state['input']['trip_number'];

   if (!empty($trip_number) && !empty($rs_code)) {
      $options = _get_all_tripsbill($trip_number);
      if (!empty($options)) {
         $form['bill_number'] = array(
           '#prefix' => '<div class="textfield-wrapper">',
           '#type' => 'select',
           '#options' => $options,
           '#required' => TRUE,
           '#title' => t('Bill Number'),
           '#ajax' => array(
             'wrapper' => 'itemlist-div',
             'callback' => 'trip_details_remove_callback',
             'method' => 'replace',
             'effect' => 'fade',
           ),
         );
      }
      
      $bill_number = $form_state['input']['bill_number'];
      if (!empty($trip_number) && !empty($rs_code) && !empty($bill_number)) {
      
         $bill_details = _get_noneditable_delivered_trip_details($trip_number, $rs_code, $bill_number);

         if (!empty($bill_details)) {

            $bill_details_header = array('Bill value', 'Party name');

            $form['bill_details'] = array(
              '#theme' => 'table',
              '#header' => $bill_details_header,
              '#rows' => $bill_details,
            );

            // order status
            $form['order_status'] = array(
              '#type' => 'select',
              '#options' => array('' => '-select-', 'undeliver' => 'undeliver', 'deliver' => 'deliver', 'partial_deliver' => 'partial deliver'),
              '#title' => t('Order Status'),
              '#required' => TRUE,
              '#ajax' => array(
                'wrapper' => 'itemlist-div',
                'callback' => 'trip_details_remove_callback',
                'method' => 'replace',
                'effect' => 'fade',
              ),
            );
            
            $order_status = $form_state['input']['order_status'];

            // collection mode
            $lists = array('' => '-select-', 'signed' => 'Signed', 'cheque' => 'Cheque', 'cash' => 'Cash');
            $form['collection_mode'] = array(
              '#type' => 'select',
              '#options' => $lists,
              '#title' => t('Collection Mode'),
              '#required' => TRUE,
            );


            $form['bank_name'] = array(
              '#type' => 'textfield',
              //'#options' => $lists,
              '#title' => t('Bank Name'),
              '#states' => array(
                'visible' => array(
                  ':input[name="collection_mode"]' => array('value' => 'cheque'),
                ),
              ),
             // '#required' => TRUE,
            );
            $form['cheque_amt'] = array(
              '#type' => 'textfield',
              //'#options' => $lists,
              '#title' => t('Cheque Amt'),
              '#states' => array(
               'visible' => array(
                  ':input[name="collection_mode"]' => array('value' => 'cheque'),
                ),
              ),
             // '#required' => TRUE,
            );
            $form['cheque_date'] = array(
              '#type' => 'date_popup',
              // '#options' => $lists,
              '#title' => t('Cheque Date'),
              '#states' => array(
                'visible' => array(
                  ':input[name="collection_mode"]' => array('value' => 'cheque'),
                ),
              ),
              '#date_format' => 'Y-m-d',
             // '#required' => TRUE,
            );
            $form['cheque_file'] = array(
              '#title' => t('Photo of Cheque Copy'),
              '#type' => 'file',
              '#description' => t('Allowed extensions: pdf png jpg jpeg txt'),
              '#states' => array(
                 'visible' => array(
                  ':input[name="collection_mode"]' => array('value' => 'cheque'),
                ),
              ),
             // '#required' => TRUE,
            );

            
            $form['sb_amt'] = array(
              '#type' => 'textfield',
              '#title' => t('Signed bill Amt'),
              '#states' => array(
                'visible' => array(
                  ':input[name="collection_mode"]' => array('value' => 'signed'),
                ),
              ),
             // '#required' => TRUE,
            );

            $form['sb_file'] = array(
              '#title' => t('Photo of Signed bill Copy'),
              '#type' => 'file',
              '#description' => t('Allowed extensions: pdf png jpg jpeg txt'),
              '#states' => array(
                'visible' => array(
                  ':input[name="collection_mode"]' => array('value' => 'signed'),
                ),
              ),
              // '#required' => TRUE,
            );
            
            $form['cash_amt'] = array(
              '#type' => 'textfield',
              '#title' => t('Cash Amt'),
              '#states' => array(
              'visible' => array(
                  ':input[name="collection_mode"]' => array('value' => 'cash'),
                ),
              ),
              // '#required' => TRUE,
            );

            $item_details = _get_editable_delivered_trip_details($trip_number, $rs_code, $bill_number);
            $item_details_header = array('SKU7Code' => 'SKU7Code', 
              'Description' => 'Description',
               'Sum_of_Net_Sales_Qty' => 'Sum of Net Sales Qty', 
              'Actual_qty' => 'Actual qty', 
              'Sku_reason' => 'Sku reason');

            // initialize editable elements here
            $form['actual_qty'] = array('#type' => 'value');
            $form['sku_reason'] = array('#type' => 'value');

            // editable elemenst
            foreach ($item_details as $key => $value) {
               $item_details[$key]['actual_qty']['data'] = array(
                 '#type' => 'textfield',
                 '#title' => t('Actual qty'),
                 '#title_display' => 'invisible',
                 '#name' => "actual_qty[$key]",
                 '#required' => TRUE,
                 '#value' => !empty($key['Actual_qty']) ? $key['Actual_qty'] : '',
               );
               if (!empty($order_status) && $order_status == 'deliver') {
                  $item_details[$key]['actual_qty']['data']['#value'] = $item_details[$key]['og_sum_net_sales_quantity'];
                  $item_details[$key]['actual_qty']['data']['#attributes'] = array('readonly' => 'readonly');
               }
               if (!empty($order_status) && $order_status == 'undeliver') {
                  $item_details[$key]['actual_qty']['data']['#value'] = 0;
                  $item_details[$key]['actual_qty']['data']['#attributes'] = array('readonly' => 'readonly');
               }
               $item_details[$key]['sku_reason']['data'] = array(
                 '#type' => 'textfield',
                 '#title' => t('Sku reason'),
                 '#title_display' => 'invisible',
                 '#name' => "sku_reason[$key]",
                 '#required' => TRUE,
                 '#value' => !empty($key['Sku_reason']) ? $key['Sku_reason'] : '',
               );
            }
            $form['trip_details'] = array(
              '#theme' => 'table',
              '#header' => $item_details_header,
              '#rows' => $item_details,
            );
            $form['submit'] = array(
              '#type' => 'submit',
              '#value' => 'submit',
            );
         }
         else {
            drupal_set_message(t('No result Found'), 'error');
         }
      }
   }

   return $form;
}


/**
 * 
 * @param type $string
 * @param type $trip_id
 */
function _get_all_tripsbill($trip_id) {
  $result = array();

  if (!empty($trip_id)) {
    $query = db_select('node', 'n');
    $query->Join('field_data_field_trip_id_reference', 'ft', 'ft.entity_id = n.nid');
    $query->Join('node', 'n1', 'n1.nid = ft.field_trip_id_reference_nid');
    $query->condition('n.type', 'sales_register_data');
    $query->condition('n1.title', $trip_id);
    $query->fields('n', array('title'));
    $query->range(0, 15);
    $query->addTag('node_distributor_access');
    $result = $query->execute()->fetchAllKeyed(0, 0);
  }

  return $result;
}



/**
 * 
 * @global type $user
 * @param type $trip_number
 * @param type $rs_code
 * @param type $bill_no
 * @return type
 */
function _get_noneditable_delivered_trip_details($trip_number, $rs_code, $bill_no) {
   global $user;
   $dis_id = change_feeds_import_get_distributor_id($user->uid);

   $query = db_select('dispatch_planning_api_data', 'ad');
   $query->condition('ad.runsheet_number', $trip_number);
   $query->condition('ad.hub', $rs_code);
   $query->condition('ad.bill_number', $bill_no);
   $query->fields('ad', array('bill_value', 'party_name'));    
   $rows = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
   return $rows;
}

/**
 * 
 * @global type $user
 * @param type $trip_number
 * @param type $rs_code
 * @param type $bill_no
 * @return type
 */
function _get_editable_delivered_trip_details($trip_number, $rs_code, $bill_no) {
   global $user;
   $dis_id = change_feeds_import_get_distributor_id($user->uid);

   $query = db_select('dispatch_planning_item_details', 'id');
   $query->join('dispatch_planning_api_data', 'ad', 'id.trip_bill_reference = ad.id');
   $query->condition('ad.runsheet_number', $trip_number);
   $query->condition('ad.hub', $rs_code);
   $query->condition('ad.bill_number', $bill_no);
   $query->fields('id', array('sku_code', 'sku_code', 'sku_description', 'og_sum_net_sales_quantity'));
   $rows = $query->execute()->fetchAll(PDO::FETCH_UNIQUE | PDO::FETCH_ASSOC);
    
   return $rows;
}


/**
 * 
 * @param type $form
 * @param type $form_state
 */
function _edit_trip_delivered_callback_form_validate($form, &$form_state) {
  // dpm($form_state, '$form_state');
   $collc_mode = $form_state['values']['collection_mode'];
   if ($collc_mode == 'cheque') {
      $file = file_save_upload('cheque_file', array(
        'file_validate_extensions' => array('pdf png jpg jpeg txt'), // Validate extensions.
      ));
      // If the file passed validation:
      if ($file) {
         // Change the status
         $file->status = FILE_STATUS_PERMANENT;
         // Update the file status into the database
         file_save($file);
         // Move the file, into the Drupal file system
         if ($file = file_move($file, 'public://')) {
            // Save the file for use in the submit handler.
            $form_state['storage']['cheque_file'] = $file;
         }
         else {
            form_set_error('cheque_file', t('Failed to write the uploaded bill file the site\'s file folder.'));
         }
      }
      else {
         form_set_error('cheque_file', t('No Cheque file was uploaded.'));
      }
   }
   if ($collc_mode == 'signed') {
      $file = file_save_upload('sb_file', array(
        'file_validate_extensions' => array('pdf png jpg jpeg txt'), // Validate extensions.
      ));
      // If the file passed validation:
      if ($file) {
         // Change the status
         $file->status = FILE_STATUS_PERMANENT;
         // Update the file status into the database
         file_save($file);
         // Move the file, into the Drupal file system
         if ($file = file_move($file, 'public://')) {
            // Save the file for use in the submit handler.
            $form_state['storage']['sb_file'] = $file;
         }
         else {
            form_set_error('sb_file', t('Failed to write the uploaded bill file the site\'s file folder.'));
         }
      }
      else {
         form_set_error('sb_file', t('No Signed file was uploaded.'));
      }
   }
}

/**
 * 
 * @param type $form
 * @param type $form_state
 */
function _edit_trip_delivered_callback_form_submit($form, &$form_state) {
  // dpm($form_state, '$form_state');
   $trip_number = $form_state['values']['trip_number'];
   //dpm($trip_number, '$trip_number');
   $bill_number = $form_state['values']['bill_number'];
   //dpm($bill_number, '$bill_number');
   // get logged in users rs id
   $user_id = $GLOBALS['user']->uid;
   $rs_code = dispatch_planning_get_rscode_using_uid($user_id);
   //dpm($rs_code, '$rs_code');
   if (!empty($trip_number) && !empty($bill_number) && !empty($rs_code)) {
      $order_status = $form_state['values']['order_status'];
      $collc_mode = $form_state['values']['collection_mode'];
      
      $cheque_file = ($form_state['storage']['cheque_file']);
      $sb_file = ($form_state['storage']['sb_file']);
      $cheque_file_uri = $cheque_file->uri;
      $sb_file_uri = $sb_file->uri;

      $updated_bill_rows = db_update('dispatch_planning_api_data') // Table name no longer needs {}
          ->fields(array(
            'res_status' => $order_status,
            'res_cash_amount' => ($collc_mode == 'cash') ? $form_state['values']['cash_amt'] : '',
            'res_cheque_amount' => ($collc_mode == 'cheque') ? $form_state['values']['cheque_amt'] : '',
            'res_bank_name' => ($collc_mode == 'cheque') ? $form_state['values']['bank_name'] : '',
            'res_cheque_date' => ($collc_mode == 'cheque') ? $form_state['values']['cheque_date'] : '',
            'res_photo_of_cheque' => ($collc_mode == 'cheque') ? $cheque_file_uri : '',
            'res_confirm_signed_bill_amount' => ($collc_mode == 'signed') ? $form_state['values']['sb_amt'] : '',
            'res_photo_of_signed_bill_delivery' => ($collc_mode == 'signed') ? $sb_file_uri : '',
            'res_signed_bill_delivery' => ($collc_mode == 'signed' && $order_status == 'deliver') ? 1 : '',
            'res_signed_bill_partial' => ($collc_mode == 'signed' && $order_status == 'partial_deliver') ? 1 : '',
            'updated_flag' => 1,
          ))
          ->condition('runsheet_number', $trip_number, '=')
          ->condition('bill_number', $bill_number, '=')
          ->condition('hub', $rs_code, '=')
          ->execute();
      //dpm($updated_bill_rows, '$updated_bill_rows');
      if (!empty($updated_bill_rows)) {
         $reference_no = $trip_number . '_' . $rs_code . '_' . $bill_number;
         $bill_row_id = _get_bill_row_id_using_reference_no($reference_no);
         //dpm($bill_row_id, '$bill_row_id');


         foreach ($form_state['values']['actual_qty'] as $key => $value) {

            db_update('dispatch_planning_item_details') // Table name no longer needs {}
                ->fields(array(
                  'res_sku_actual_quantity1' => $value,
                  'res_sku_reason' => $form_state['values']['sku_reason'][$key],
                ))
                ->condition('trip_bill_reference', $bill_row_id, '=')
                ->condition('sku_code', $key, '=')
                ->execute();
         }
         
         drupal_set_message(t('Data submitted successfully'));
      }
   }
}
