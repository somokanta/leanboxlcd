<?php

/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function update_crate_id_data($outlet_order_ref, $crate_id) {
   $result = db_merge('outlet_order_crate')
    ->key(array('outlet_order_no' => $outlet_order_ref))
    ->fields(array(
         'outlet_order_no' => $outlet_order_ref,
         'crate_id' => $crate_id,
         'created_time' => REQUEST_TIME,
       ))
       ->execute();

   $results['flag'] = '1';
   $results['result'] = 'Updated Successfully';
   return $results;
}