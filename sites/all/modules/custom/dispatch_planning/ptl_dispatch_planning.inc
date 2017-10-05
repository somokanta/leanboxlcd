<?php

/* 
 * it will call from, "ptl-api-data" services
 * 
 */

function fetch_ptl_data($time){
   $query = db_select('ptl_data_configuration', 'p');
   $query->fields('p', array('ptl_job','trip_id','vehicle', 'bill_number','loading_date','sequence_no','basepack_code','sku_code','product_name','MRP','TUR','sum_sales_qty'));
   $query->condition('p.api_flag', '0', '=');

   $result = $query->execute()->fetchAll();

   $results['data'] = json_encode($result);
   
   // log entry and is remaining
   
   return $results;
}