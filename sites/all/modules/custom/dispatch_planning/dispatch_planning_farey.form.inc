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

function trip_details_remove_callback($form, &$form_state) {
   return $form;
}
