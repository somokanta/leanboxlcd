<?php

/**
 * @file
 * Definition of views_handler_field_order_upload_datetime.
 */

/**
 * Returns outlets_per_van
 * @ingroup views_field_handlers
 */
class views_handler_field_order_upload_datetime extends views_handler_field {

  function render($values) {
   $outlet_order_number= $values->field_field_outlet_order_no[0]['raw']['value'];
   $node_uid = $values->node_uid;
   $order_upload_datetime_result = '';
   if(!empty($outlet_order_number) && !empty($node_uid)){
    $query = db_select('outlet_order_crate', 'ooc');
    $query->condition('ooc.outlet_order_no', $outlet_order_number);
    $query->condition('ooc.distributor_id', $node_uid);
    $query->fields('ooc', array('order_upload_datetime'));
    $order_upload_datetime = $query->execute()->fetchField();
    if($order_upload_datetime != 0){
      $order_upload_datetime_result = date('d-M-y H:i',$order_upload_datetime); 
    }
   }
   return $order_upload_datetime_result;
  }

}
