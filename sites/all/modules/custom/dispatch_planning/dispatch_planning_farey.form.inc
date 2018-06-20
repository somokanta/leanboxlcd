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
 //$data[$val][] = array('data' => $val1, 'data-value' => $val1, 'class' => array("$val-$key1-val"));
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
   $form['#attached']['js'][] = drupal_get_path('module', 'dispatch_planning') . '/js/dispatch_planning_farey.js';

   // get logged in users rs id
   $user_id = $GLOBALS['user']->uid;
   $rs_code = dispatch_planning_get_rscode_using_uid($user_id);
   $distributor_id = change_feeds_import_get_distributor_id($user_id);

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
   if (isset($form_state['triggering_element']['#title']) && ($form_state['triggering_element']['#title'] == 'Bill Number') && !empty($form_state['input']['bill_number'])) {
     unset($form_state['storage']['trip_id']);
   }
   if (isset($form_state['storage']['trip_id']) && !empty($form_state['storage']['trip_id'])) {
      $trip_no = $form_state['storage']['trip_id'];
      
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
        '#attributes' => array('class' => array('order_status_list')),
      );

      $form['bill_number'] = array(
        '#prefix' => '<div class="textfield-wrapper">',
        '#type' => 'textfield',
        '#title' => t('Bill Number'),
        '#required' => TRUE,
        //'#options' => $options,
        '#autocomplete_path' => "bills/autocomplete/$trip_no",
        '#ajax' => array(
          'wrapper' => 'itemlist-div',
          'callback' => 'trip_details_remove_callback',
          'method' => 'replace',
          'effect' => 'fade',
        ),
      );


      $order_status = $form_state['input']['order_status'];
   }
   else {
      if (!empty($trip_number) && !empty($rs_code)) {
         //$options = _get_all_tripsbill($trip_number);
         //if (!empty($options)) {
         // order status
        
        // check trip if it is Returns Confirmed
        // check trip if it is VRS Confirmed
        $trip_detail = check_trip_id_exists_or_not($distributor_id, $trip_number);
        $vrs_status = $trip_detail->field_vrs_status_value;
  
        if($vrs_status == 1) {
           drupal_set_message(t('You cannot edit because this trip is returns confirmed.'), 'error');
        }
        else if($vrs_status == 2) {
          drupal_set_message(t('You cannot edit because this trip is VRS confirmed.'), 'error');
        }
        else if($vrs_status == 3) {
          drupal_set_message(t('You cannot edit because this trip is VRS pending.'), 'error');
        }
        else if($vrs_status == 4) {
          drupal_set_message(t('You cannot edit because this trip is Handover pending.'), 'error');
        }
        else if($vrs_status == 5) {
          drupal_set_message(t('You cannot edit because this trip is Handover confirmed.'), 'error');
        }
        else {
          $link = l('Enter Received Assets', "received-assets-screen?title=$trip_number", array('html' => TRUE, 'attributes' => array('target' => '_blank')));
          $form['return_assets_link'] = array(
            '#markup' => rawurldecode($link),
          );
          
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
           '#attributes' => array('class' => array('order_status_list')),
         );


         $form['bill_number'] = array(
           '#prefix' => '<div class="textfield-wrapper">',
           '#type' => 'textfield',
           '#title' => t('Bill Number'),
           '#required' => TRUE,
           //'#options' => $options,
           '#autocomplete_path' => "bills/autocomplete/$trip_number",
           '#ajax' => array(
             'wrapper' => 'itemlist-div',
             'callback' => 'trip_details_remove_callback',
             'method' => 'replace',
             'effect' => 'fade',
           ),
         );

         $order_status = $form_state['input']['order_status'];

         //}
         
         $bill_number = $form_state['input']['bill_number'];
         if (!empty($trip_number) && !empty($rs_code) && !empty($bill_number)) {
  
            $bill_details = _get_noneditable_delivered_trip_details($trip_number, $rs_code, $bill_number);
            //dpm($bill_details, '$bill_details');
            if (!empty($bill_details)) {
              
               // get default values from header level api_data table 
               $reason_for_failure = $bill_details[0]['res_reason_for_failure'];
               $res_confirm_signed_bill_amount = $bill_details[0]['res_confirm_signed_bill_amount'];
               $res_photo_of_signed_bill_delivery = $bill_details[0]['res_photo_of_signed_bill_delivery'];
               $res_bank_name = $bill_details[0]['res_bank_name'];
               $res_cheque_amount = $bill_details[0]['res_cheque_amount'];
               $res_cheque_date = $bill_details[0]['res_cheque_date'];
               $res_photo_of_cheque = $bill_details[0]['res_photo_of_cheque'];
               $res_cash_amount = $bill_details[0]['res_cash_amount'];
               $res_money_collect2 = strtolower($bill_details[0]['res_money_collect2']);
               $res_money_collect2_edit = strtolower($bill_details[0]['res_money_collect2_edit']);
               $res_confirm_signed_bill_amount_edit = $bill_details[0]['res_confirm_signed_bill_amount_edit'];
               $res_photo_of_signed_bill_delivery_edit = $bill_details[0]['res_photo_of_signed_bill_delivery_edit'];
               $res_bank_name_edit = $bill_details[0]['res_bank_name_edit'];
               $res_cheque_amount_edit = $bill_details[0]['res_cheque_amount_edit'];
               $res_cheque_date_edit = $bill_details[0]['res_cheque_date_edit'];
               $res_photo_of_cheque_edit = $bill_details[0]['res_photo_of_cheque_edit'];
               $res_cash_amount_edit = $bill_details[0]['res_cash_amount_edit'];
               $res_signed_bill_delivery = $bill_details[0]['res_signed_bill_delivery'];
               $res_signed_bill_partial = $bill_details[0]['res_signed_bill_partial'];
               $res_signed_bill_delivery_edit = $bill_details[0]['res_signed_bill_delivery_edit'];
               $res_signed_bill_partial_edit = $bill_details[0]['res_signed_bill_partial_edit'];
               $res_cheque_no = $bill_details[0]['res_cheque_no'];
               $res_cheque_no_edit = $bill_details[0]['res_cheque_no_edit'];

               // unset all keys 
               unset($bill_details[0]['res_reason_for_failure']);
               unset($bill_details[0]['res_confirm_signed_bill_amount']);
               unset($bill_details[0]['res_photo_of_signed_bill_delivery']);
               unset($bill_details[0]['res_bank_name']);
               unset($bill_details[0]['res_cheque_amount']);
               unset($bill_details[0]['res_cheque_date']);
               unset($bill_details[0]['res_photo_of_cheque']);
               unset($bill_details[0]['res_cash_amount']);
               unset($bill_details[0]['res_money_collect2']);
               unset($bill_details[0]['res_money_collect2_edit']);
               unset($bill_details[0]['res_confirm_signed_bill_amount_edit']);
               unset($bill_details[0]['res_photo_of_signed_bill_delivery_edit']);
               unset($bill_details[0]['res_bank_name_edit']);
               unset($bill_details[0]['res_cheque_amount_edit']);
               unset($bill_details[0]['res_cheque_date_edit']);
               unset($bill_details[0]['res_photo_of_cheque_edit']);
               unset($bill_details[0]['res_cash_amount_edit']);
               unset($bill_details[0]['res_signed_bill_delivery']);
               unset($bill_details[0]['res_signed_bill_partial']);
               unset($bill_details[0]['res_signed_bill_delivery_edit']);
               unset($bill_details[0]['res_signed_bill_partial_edit']);
               unset($bill_details[0]['res_cheque_no']);
               unset($bill_details[0]['res_cheque_no_edit']);
               
               //unset form input for signed,cheque and cash amounts
               unset($form_state['input']['collection_mode']);
               unset($form_state['input']['cash_amt']);
               unset($form_state['input']['bank_name']);
               unset($form_state['input']['cheque_amt']);
               unset($form_state['input']['cheque_date']);
               unset($form_state['input']['res_photo_of_cheque']);
               unset($form_state['input']['sb_amt']);
               unset($form_state['input']['sb_file']);
               unset($form_state['input']['cheque_no']);

               $bill_details_header = array('Bill value', 'Party name');

               $form['bill_details'] = array(
                 '#theme' => 'table',
                 '#header' => $bill_details_header,
                 '#rows' => $bill_details,
               );

               // order status
//            $form['order_status'] = array(
//              '#type' => 'select',
//              '#options' => array('' => '-select-', 'undeliver' => 'undeliver', 'deliver' => 'deliver', 'partial_deliver' => 'partial deliver'),
//              '#title' => t('Order Status'),
//              '#required' => TRUE,
//              '#ajax' => array(
//                'wrapper' => 'itemlist-div',
//                'callback' => 'trip_details_remove_callback',
//                'method' => 'replace',
//                'effect' => 'fade',
//              ),
//              '#attributes' => array('class' => array('order_status_list')),
//            );
//            
//            $order_status = $form_state['input']['order_status'];
//            
               $undeliver_reason_list = _get_dispatch_planning_reason_master_list('undeliver');
               $form['undeliver_reason'] = array(
                 '#type' => 'select',
                 '#options' => $undeliver_reason_list,
                 '#title' => t('Undeliver reason'),
                 '#states' => array(
                   'visible' => array(
                     ':input[name="order_status"]' => array('value' => 'undeliver'),
                   ),
                 ),
                 '#default_value' => !empty($reason_for_failure) ? $reason_for_failure : '',
               );
               // collection mode
               $lists = array('' => '-select-', 'signed' => 'Signed', 'cheque' => 'Cheque', 'cash' => 'Cash');

               if (!empty($order_status) && $order_status == 'undeliver') {
                  $form['collection_mode'] = array(
                    '#type' => 'value',
                    '#value' => '',
                  );
               }
               else { 
                 
                if(!empty($res_money_collect2_edit)) {
                  $col_mode = $res_money_collect2_edit;
                }else if (!empty($res_money_collect2)) {
                  $col_mode = $res_money_collect2;
                }else if ($res_signed_bill_delivery_edit == 'Yes') {
                  $col_mode = 'signed';
                }else if ($res_signed_bill_delivery == 'Yes') {
                  $col_mode = 'signed';
                }else if ($res_signed_bill_partial_edit == 'Yes') {
                  $col_mode = 'signed';
                }else if ($res_signed_bill_partial == 'Yes') {
                  $col_mode = 'signed';
                }else{
                  $col_mode = '';
                }

            $form['collection_mode'] = array(
                    '#type' => 'select',
                    '#options' => $lists,
                    '#title' => t('Collection Mode'),
                    
                      //'#required' => TRUE,
                     '#default_value' => $col_mode,
                  );


                  $form['bank_name'] = array(
                    '#type' => 'textfield',
                    '#title' => t('Bank Name'),
                    '#states' => array(
                      'visible' => array(
                        ':input[name="collection_mode"]' => array('value' => 'cheque'),
                      ),
                    ),
                    '#default_value' => !empty($res_bank_name_edit) ? $res_bank_name_edit : $res_bank_name,
                  );
                  $form['cheque_amt'] = array(
                    '#type' => 'textfield',
                    '#title' => t('Cheque Amt'),
                    '#states' => array(
                      'visible' => array(
                        ':input[name="collection_mode"]' => array('value' => 'cheque'),
                      ),
                    ),
                    '#default_value' => isset($res_cheque_amount_edit) ? $res_cheque_amount_edit : $res_cheque_amount,
                  );
                  $form['cheque_no'] = array(
                    '#type' => 'textfield',
                    '#title' => t('Cheque Number'),
                    '#states' => array(
                      'visible' => array(
                        ':input[name="collection_mode"]' => array('value' => 'cheque'),
                      ),
                    ),
                    '#default_value' => isset($res_cheque_no_edit) ? $res_cheque_no_edit : $res_cheque_no,
                  );
                  
                  if(!empty($res_cheque_date)) {
                     if(empty(strtotime($res_cheque_date))) {
                        $res_cheque_date = date('Y-m-d', $res_cheque_date);
                     }
                  }
                  $form['cheque_date'] = array(
                    '#type' => 'date_popup',
                    '#title' => t('Cheque Date'),
                    '#states' => array(
                      'visible' => array(
                        ':input[name="collection_mode"]' => array('value' => 'cheque'),
                      ),
                    ),
                    '#default_value' => !empty($res_cheque_date_edit) ? $res_cheque_date_edit : $res_cheque_date,
                    '#date_format' => 'Y-m-d',
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
                  );
                  if (!empty($res_photo_of_cheque_edit)) {
                    $url = file_create_url($res_photo_of_cheque_edit);
                  }else{
                    $url = file_create_url($res_photo_of_cheque);
                  }
                  if (!empty($res_photo_of_cheque) || !empty($res_photo_of_cheque_edit)) {
                     $file = 'Open File';
                     $form['res_photo_of_cheque'] = array(
                       '#type' => 'item',
                       '#markup' => l("$file", $url,  array('attributes' => array('target'=> '_blank'))) . '<div></div></br>',
                       '#states' => array(
                         'visible' => array(
                           ':input[name="collection_mode"]' => array('value' => 'cheque'),
                         ),
                       ),
                     );
                  }

                  $form['sb_amt'] = array(
                    '#type' => 'textfield',
                    '#title' => t('Signed bill Amt'),
                    '#states' => array(
                      'visible' => array(
                        ':input[name="collection_mode"]' => array('value' => 'signed'),
                      ),
                    ),
                    '#default_value' => isset($res_confirm_signed_bill_amount_edit) ? $res_confirm_signed_bill_amount_edit : $res_confirm_signed_bill_amount,
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
                  );
                  if (!empty($res_photo_of_signed_bill_delivery_edit)) {
                    $url = file_create_url($res_photo_of_signed_bill_delivery_edit);
                  }else{
                  $url = file_create_url($res_photo_of_signed_bill_delivery);
                  }
                  if (!empty($res_photo_of_signed_bill_delivery) || !empty($res_photo_of_signed_bill_delivery_edit)) {
                     $file = 'Open File';
                     $form['res_photo_of_signed_bill_delivery'] = array(
                       '#type' => 'item',
                       '#markup' => l("$file", $url, array('attributes' => array('target'=> '_blank'))) . '<div></div></br>',
                       '#states' => array(
                         'visible' => array(
                           ':input[name="collection_mode"]' => array('value' => 'signed'),
                         ),
                       ),
                     );
                  }

                  $form['cash_amt'] = array(
                    '#type' => 'textfield',
                    '#title' => t('Cash Amt'),
                    '#states' => array(
                      'visible' => array(
                        ':input[name="collection_mode"]' => array('value' => 'cash'),
                      ),
                    ),
                    '#default_value' => isset($res_cash_amount_edit) ? $res_cash_amount_edit : $res_cash_amount,
                  );
               }
               $form['error_message'] = array(
                 '#prefix' => '<div class ="error-msg">',
                 '#markup' => t('Actual Qty should not be greater than Net Qty.'),
                 '#suffix' => '</div>',
               );

               $item_details = _get_editable_delivered_trip_details($trip_number, $rs_code, $bill_number);
               $query = db_select('dispatch_planning_api_data', 'ad');
                                  $query->condition('ad.runsheet_number', $trip_number);
                                  $query->condition('ad.hub', $rs_code);
                                  $query->condition('ad.bill_number', $bill_number);
                                  $query->fields('ad', array('res_status'));
               $original_order_status = $query->execute()->fetchField();
               $item_details_header = array(
                 'SKU7Code' => 'SKU7Code',
                 'Description' => 'Description',
                 //'Batch Code' => 'Batch Code',
                 'MRP' => 'MRP',
                 'sum_net_sales_quantity' => 'Total Sales Qty',
                 'res_godown_short' => 'Godown Short',
                 'dispatch_qty' => 'Dispatch Qty',
                 'Actual_qty' => 'Return qty',
                 'Sku_reason' => 'Sku reason',
                 'cashier_short' => 'Cashier Short',
               );

               // initialize editable elements here
               $form['actual_qty'] = array('#type' => 'value');
               $form['sku_reason'] = array('#type' => 'value');
               $form['cashier_short'] = array('#type' => 'value');

               $partial_reason_list = _get_dispatch_planning_reason_master_list('partial_deliver');
               // editable elemenst
               foreach ($item_details as $key => $value) {
                  $item_details[$key]['actual_qty']['data'] = array(
                    '#type' => 'textfield',
                    '#title' => t('Actual qty'),
                    '#title_display' => 'invisible',
                    '#name' => "actual_qty[$key]",
                    '#attributes' => array('class' => array('actual_qty_value')),
                    '#size' => 10,
                    '#required' => TRUE,
                      //  '#value' => !empty($key['Actual_qty']) ? $key['Actual_qty'] : '',
                  );
                  if (!empty($order_status) && $order_status == 'deliver') {
                     // $item_details[$key]['actual_qty']['data']['#value'] = $item_details[$key]['dispatch_qty'];
                     $item_details[$key]['actual_qty']['data']['#value'] = 0;
                     $item_details[$key]['actual_qty']['data']['#attributes']['readonly'] = 'readonly';
                  }
                  if (!empty($order_status) && $order_status == 'partial_deliver') {
                     $item_details[$key]['actual_qty']['data']['#value'] = 0;
                     // $item_details[$key]['actual_qty']['data']['#attributes'] = array('readonly' => 'readonly');
                  }

                  if (!empty($order_status) && $order_status == 'undeliver') {
                     $item_details[$key]['actual_qty']['data']['#value'] = $item_details[$key]['dispatch_qty'];
                     $item_details[$key]['actual_qty']['data']['#attributes']['readonly'] = 'readonly';
                  }
                  
                  // default value for return Qty
                  if(!empty($item_details[$key]['return_qty']) && $original_order_status == $order_status) {
                    $item_details[$key]['actual_qty']['data']['#value'] = $item_details[$key]['return_qty'];
                  }
                  unset($item_details[$key]['return_qty']);
                 
                  // default value for sku_reason
                  $item_details[$key]['sku_reason']['data'] = array(
                    '#type' => 'select',
                    '#title' => t('Sku reason'),
                    '#title_display' => 'invisible',
                    '#name' => "sku_reason[$key]",
                    '#attributes' => array('class' => array('sku_reason_list')),
                    '#options' => $partial_reason_list,
                    '#required' => TRUE,
                      //  '#value' => !empty($key['Sku_reason']) ? $key['Sku_reason'] : '',
                  );
                  
                  // default value for sku_reason
                  if(!empty($item_details[$key]['res_sku_reason']) && $original_order_status == $order_status) {
                    $item_details[$key]['sku_reason']['data']['#value'] = $item_details[$key]['res_sku_reason'];
                  }
                  unset($item_details[$key]['res_sku_reason']);
                 
                  $item_details[$key]['cashier_short']['data'] = array(
                    '#type' => 'textfield',
                    '#title' => t('Cashier Short'),
                    '#title_display' => 'invisible',
                    '#size' => 10,
                    '#name' => "cashier_short[$key]",
                    '#attributes' => array('class' => array('cashier_short_list')),
                  );
                  
                  // default value for cashier short
                  if(!empty($item_details[$key]['res_cashier_short']) && $original_order_status == $order_status) {
                    $item_details[$key]['cashier_short']['data']['#value'] = $item_details[$key]['res_cashier_short'];
                  }
                  unset($item_details[$key]['res_cashier_short']);
                 
                  
                  
                  if (!empty($order_status) && $order_status == 'deliver') {
                     // $item_details[$key]['actual_qty']['data']['#value'] = $item_details[$key]['dispatch_qty'];
                     $item_details[$key]['cashier_short']['data']['#value'] = 0;
                     $item_details[$key]['cashier_short']['data']['#attributes']['readonly'] = 'readonly';
                     //$item_details[$key]['sku_reason']['data']['#attributes']['readonly'] = 'readonly';
                  }
               }
               $og_total = 0;
               foreach ($item_details as $key => $val) {
                  foreach ($val as $key1 => $val1) {
                     if ($key1 == 'dispatch_qty') {
                        $og_total = $og_total + $val1;
                        $og_array[$key] = $val1;
                     }
                     $form['og_array'] = array(
                       '#type' => 'value',
                       '#value' => $og_array,
                     );
                     $form['og_total'] = array(
                       '#type' => 'value',
                       '#value' => $og_total,
                     );
                     if ($key1 == 'actual_qty' || $key1 == 'sku_reason' || $key1 == 'cashier_short') {
                        $item_details_new[$key][$key1] = array('data' => $val1['data'], 'data-value' => $val1, 'class' => array("$key1-val"));
                     }
                     else {
                        $item_details_new[$key][$key1] = array('data' => $val1, 'data-value' => $val1, 'class' => array("$key1-val"));
                     }
                  }
               }

               $form['trip_details'] = array(
                 '#theme' => 'table',
                 '#header' => $item_details_header,
                 '#rows' => $item_details_new,
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
    }
   }
   //dpm($form, 'form');

   return $form;
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
   $query->fields('ad', array('bill_value', 'party_name', 
     'res_reason_for_failure', 'res_confirm_signed_bill_amount', 
     'res_photo_of_signed_bill_delivery', 'res_bank_name', 'res_cheque_amount', 
     'res_cheque_date', 'res_photo_of_cheque', 'res_cash_amount', 'res_money_collect2',
     'res_money_collect2_edit','res_confirm_signed_bill_amount_edit','res_photo_of_signed_bill_delivery_edit',
     'res_bank_name_edit','res_cheque_amount_edit','res_cheque_date_edit','res_photo_of_cheque_edit','res_cash_amount_edit',
     'res_signed_bill_delivery','res_signed_bill_partial','res_signed_bill_delivery_edit','res_signed_bill_partial_edit','res_cheque_no','res_cheque_no_edit'));
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

   $query = db_select('dispatch_planning_item_details', 'dpi');
   $query->join('dispatch_planning_api_data', 'ad', 'dpi.trip_bill_reference = ad.id');
   $query->condition('ad.runsheet_number', $trip_number);
   $query->condition('ad.hub', $rs_code);
   $query->condition('ad.bill_number', $bill_no);
   $query->fields('dpi', array('id', 'sku_code', 'sku_description', 
     'mrp', 'sum_net_sales_quantity',
     'res_godown_short', 'dispatch_qty', 'return_qty', 
     'res_sku_reason', 'res_cashier_short'));
   $rows = $query->execute()->fetchAll(PDO::FETCH_UNIQUE | PDO::FETCH_ASSOC);
   return $rows;
}


/**
 * 
 * @param type $form
 * @param type $form_state
 */
function _edit_trip_delivered_callback_form_validate($form, &$form_state) {
    foreach ($form_state['values']['actual_qty'] as $sku_no => $val) {
      $form_state['complete form']['trip_details']['#rows'][$sku_no]['actual_qty']['data']['#value'] = $val;
   }
   foreach ($form_state['values']['sku_reason'] as $sku_no => $val) {
      $form_state['complete form']['trip_details']['#rows'][$sku_no]['sku_reason']['data']['#value'] = $val;
   }
   foreach ($form_state['values']['cashier_short'] as $sku_no => $val) {
      $form_state['complete form']['trip_details']['#rows'][$sku_no]['cashier_short']['data']['#value'] = $val;
   }
   // dpm($form_state, '$form_state');
   $og_total = $form_state['values']['og_total'];
   $og_array = $form_state['values']['og_array'];
   $sku_reason = $form_state['values']['sku_reason'];
   $cash_short = $form_state['values']['cashier_short'];
   $actual_qty_array = $form_state['values']['actual_qty'];

//   $actual_qty = 0;
//   foreach ($actual_qty_array as $key => $value) {
//      $actual_qty = $actual_qty + $value;
//   }
//   if ($actual_qty > $og_total) {
//      form_set_error('actual_qty', t('Sum of actual quantity can not be greater than sum of Total Sales Quantityy.'));
//   }
   $order_status = $form_state['values']['order_status'];
   if ($order_status == 'undeliver') {
      $undeliver_reason = $form_state['values']['undeliver_reason'];
      if (empty($undeliver_reason)) {
         form_set_error('undeliver_reason', t('Undeliover reason can not be empty'));
      }
   }
   //  if ($order_status == 'partial_deliver') {
//      if ($actual_qty == $og_total) {
//         form_set_error('actual_qty', t('Sum of actual quantity can not be equal to sum of Total Sales Quantity.'));
//      }
   // dpm($og_array, '$og_array');
   // dpm($actual_qty_array, '$actual_qty_array');
   // dpm($sku_reason, '$sku_reason');
   foreach ($og_array as $key => $val) {
      $dispatch_qty = $og_array[$key];
      $user_input_qty = $actual_qty_array[$key] + $cash_short[$key];
      if ($user_input_qty > $dispatch_qty) {
         form_set_error('actual_qty', t('Sum of cashier short and return Qty can not be greater than dispatch Qty.'));
      }
      if ($order_status == 'partial_deliver') {
         if (($actual_qty_array[$key] != 0) && (empty($sku_reason[$key]))) {
            form_set_error('actual_qty', t('Kindly select the reason for partial delivery.'));
         }
//         if (array_sum($actual_qty_array) == 0) {
//            form_set_error('actual_qty', t('Sum of return qty cannot be zero'));
//         }
      }
   }
   // }

   $collc_mode = $form_state['values']['collection_mode'];

   if ($collc_mode == 'cash') {
      $cash_amt = $form_state['values']['cash_amt'];
      if (empty($cash_amt)) {
         // form_set_error('cash_amt', t('Cash amt can not be empty'));
      }
   }
   if ($collc_mode == 'signed') {
      $sb_amt = $form_state['values']['sb_amt'];
      if (empty($sb_amt)) {
         // form_set_error('sb_amt', t('Signed bill amt can not be empty'));
      }
   }

   if ($collc_mode == 'cheque') {
      $cheque_date = $form_state['values']['cheque_date'];
      if (empty($cheque_date)) {
         // form_set_error('cheque_date', t('Cheque date can not be empty'));
      }
      $cheque_amt = $form_state['values']['cheque_amt'];
      if (empty($cheque_amt)) {
         // form_set_error('cheque_amt', t('Cheque amt can not be empty'));
      }
      $bank_name = $form_state['values']['bank_name'];
      if (empty($bank_name)) {
         // form_set_error('bank_name', t('Bank name can not be empty'));
      }


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
   // dpm($trip_number, '$trip_number');
   $bill_number = $form_state['values']['bill_number'];
   // dpm($bill_number, '$bill_number');
   // get logged in users rs id
   $user_id = $GLOBALS['user']->uid;
   $rs_code = dispatch_planning_get_rscode_using_uid($user_id);
   // dpm($rs_code, '$rs_code');
   if (!empty($trip_number) && !empty($bill_number) && !empty($rs_code)) {
      $order_status = $form_state['values']['order_status'];
      $collc_mode = $form_state['values']['collection_mode'];

      $cheque_file = ($form_state['storage']['cheque_file']);
      $sb_file = ($form_state['storage']['sb_file']);
      $cheque_file_uri = $cheque_file->uri;
      $sb_file_uri = $sb_file->uri;
      $reason_for_failure = isset($form_state['values']['undeliver_reason']) ? $form_state['values']['undeliver_reason'] : '';
      $og_array = $form_state['values']['og_array']; // dispatch_qty 

      $updated_bill_rows = db_update('dispatch_planning_api_data') // Table name no longer needs {}
          ->fields(array(
            'res_status' => $order_status,
            'res_cash_amount_edit' => ($collc_mode == 'cash') ? $form_state['values']['cash_amt'] : '',
            'res_cheque_amount_edit' => ($collc_mode == 'cheque') ? $form_state['values']['cheque_amt'] : '',
            'res_cheque_no_edit' => ($collc_mode == 'cheque') ? $form_state['values']['cheque_no'] : '',
            'res_bank_name_edit' => ($collc_mode == 'cheque') ? $form_state['values']['bank_name'] : '',
            'res_cheque_date_edit' => ($collc_mode == 'cheque') ? $form_state['values']['cheque_date'] : '',
            'res_photo_of_cheque_edit' => ($collc_mode == 'cheque') ? $cheque_file_uri : '',
            'res_confirm_signed_bill_amount_edit' => ($collc_mode == 'signed') ? $form_state['values']['sb_amt'] : '',
            'res_photo_of_signed_bill_delivery_edit' => ($collc_mode == 'signed') ? $sb_file_uri : '',
            'res_signed_bill_delivery_edit' => ($collc_mode == 'signed' && $order_status == 'deliver') ? 'Yes' : '',
            'res_signed_bill_partial_edit' => ($collc_mode == 'signed' && $order_status == 'partial_deliver') ? 'Yes' : '',
            'res_reason_for_failure' => ($order_status == 'undeliver') ? $reason_for_failure : '',
            'updated_flag' => 1,
            'updated_time' => REQUEST_TIME,  
            'res_money_collect2_edit' =>  isset($form_state['values']['collection_mode']) ? $form_state['values']['collection_mode'] : '',
            
          ))
          ->condition('runsheet_number', $trip_number, '=')
          ->condition('bill_number', $bill_number, '=')
          ->condition('hub', $rs_code, '=')
          ->execute();
      // dpm($updated_bill_rows, '$updated_bill_rows');
      // if (!empty($updated_bill_rows)) {
      $reference_no = $trip_number . '_' . $rs_code . '_' . $bill_number;
      $bill_row_id = _get_bill_row_id_using_reference_no($reference_no);
      //  dpm($bill_row_id, '$bill_row_id');

      if ($order_status == 'deliver') {
         foreach ($form_state['values']['actual_qty'] as $key => $value) {

            db_update('dispatch_planning_item_details') // Table name no longer needs {}
                ->fields(array(
                  'res_sku_actual_quantity1' => $og_array[$key],
                  'return_qty' => $value,
                  'res_sku_reason' => $form_state['values']['sku_reason'][$key],
                  'res_cashier_short' => $form_state['values']['cashier_short'][$key],
                  'line_level_status'=> 'line_deliver', // line level status
                ))
                ->condition('trip_bill_reference', $bill_row_id, '=')
                ->condition('id', $key, '=')
                ->execute();
         }
      }
      else if ($order_status == 'undeliver') {
         foreach ($form_state['values']['actual_qty'] as $key => $value) {

            db_update('dispatch_planning_item_details') // Table name no longer needs {}
                ->fields(array(
                  'res_sku_actual_quantity1' => 0,
                  'return_qty' => $value,
                  'res_sku_reason' => $form_state['values']['sku_reason'][$key],
                  'res_cashier_short' => $form_state['values']['cashier_short'][$key],
                  'line_level_status'=> 'line_undeliver', // line level status
                ))
                ->condition('trip_bill_reference', $bill_row_id, '=')
                ->condition('id', $key, '=')
                ->execute();
         }
      }
      else if ($order_status == 'partial_deliver') {
         foreach ($form_state['values']['actual_qty'] as $key => $value) {
          
          $query = db_select('dispatch_planning_item_details','it')
           ->fields('it',array('res_godown_short'))
             ->condition('trip_bill_reference', $bill_row_id, '=')
             ->condition('id', $key, '='); 
           $godown_short = $query->execute()->fetchField();   
           
           $ret_cash = $value + $form_state['values']['cashier_short'][$key]; //Return Qty + Cashier Short  
           $ret_cash_godown = $ret_cash + $godown_short; //Return Qty + Cashier Short + Godown Short
           $line_status = '';
           if ($ret_cash == $og_array[$key] ) {
             $line_status = 'line_undeliver';
           }else if ($ret_cash_godown > 0) {
             $line_status = 'line_partial_deliver';
           }else if ($ret_cash_godown == 0) {
             $line_status = 'line_deliver';
           }
            db_update('dispatch_planning_item_details') // Table name no longer needs {}
                ->fields(array(
                  'res_sku_actual_quantity1' => $og_array[$key] - $value - $form_state['values']['cashier_short'][$key],
                  'return_qty' => $value,
                  'res_sku_reason' => $form_state['values']['sku_reason'][$key],
                  'res_cashier_short' => $form_state['values']['cashier_short'][$key],
                  'line_level_status' => $line_status, // line level status
                ))
                ->condition('trip_bill_reference', $bill_row_id, '=')
                ->condition('id', $key, '=')
                ->execute();
         }
      }

      drupal_set_message(t('Data submitted successfully'));
      $form_state['storage']['trip_id'] = $trip_number;
      $form_state['rebuild'] = TRUE;
      // }
   }
}


function _get_dispatch_planning_reason_master_list($reason_type) {
   $query = db_select('node', 'n');
   $query->join('field_data_field_reason_type', 'rt', 'rt.entity_id = n.nid');
   $query->join('field_data_field_dp_reason', 'dr', 'dr.entity_id = n.nid');
   $query->condition('rt.field_reason_type_value', $reason_type);
   $query->fields('dr', array('field_dp_reason_value'));
   $query->addTag('node_distributor_access');
   $res = $query->execute()->fetchAllKeyed(0,0);
   $res1 = array('' => '- select -');
   $response = $res1 + $res;
   return $response;

}