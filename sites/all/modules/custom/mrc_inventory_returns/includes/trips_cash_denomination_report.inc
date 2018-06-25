<?php

/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
module_load_include('inc', 'mrc_inventory_returns', 'includes/vrs_common');
function trips_denominations_form($form, &$form_state){
  global $user;  
  $dis_id = change_feeds_import_get_distributor_id($user->uid);
  $form['#prefix'] = '<div id="itemlist-div">';
  $form['#suffix'] = '</div>';
  $form['dispatch_date'] = array(
        '#type' => 'date_popup',
        '#required' => TRUE,
        '#title' => t('Dispatch Date'),
        '#date_format' => 'Y-m-d',
        '#attributes' => array('class' => array('dispatch_date')),
        '#date_label_position' => '',
        '#date_year_range' => '-20:+2',
        '#size' => 30,
        '#prefix' => '<div class="col_3">',
        '#suffix' => '</div>'
    );

    $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Download'),
    '#submit' => array('trips_denominations_form_submit'),
  );
  
  return $form;
}
function _get_vrs_trip_ids_using_dispatch_date($dispatch_date) {
    $res = array();
   if (!empty($dispatch_date)) {
      global $user;
      $dis_id = change_feeds_import_get_distributor_id($user->uid);
      $query = db_select('node', 'n');
      $query->condition('n.type', 'dispatch_data');
      $query->condition('n.uid', $dis_id);
      $query->join('field_data_field_dispatch_date', 't', 't.entity_id = n.nid');
      $query->join('field_data_field_vrs_status', 'vs', 'vs.entity_id = n.nid');
      $query->join('vrs_trip_denominations', 'deno', 'deno.trip_id = n.title');
      $query->condition('deno.distributor_id', $dis_id);
      $vrs_status = array('1','0');
      $query->condition('vs.field_vrs_status_value', $vrs_status,'NOT IN');
      $query->condition('t.field_dispatch_date_value', $dispatch_date);
      $query->fields('n', array('title'));
      $res = $query->execute()->fetchAllKeyed(0, 0);
   }
   return $res;
}

function trips_denominations_form_validate($form, &$form_state) {
    $dispatch_date = $form_state['values']['dispatch_date'];
    global $user;
    $dis_id = change_feeds_import_get_distributor_id($user->uid);
    if (!empty($dispatch_date)) {
         $trip_ids =  _get_vrs_trip_ids_using_dispatch_date($dispatch_date);
         if(empty($trip_ids)) {
              form_set_error('dispatch_date', "No Data Found");
         }
    }
    
}

function trips_denominations_form_submit($form,&$form_state) {
  $dispatch_date = $form_state['values']['dispatch_date'];
  global $user;  
  $dis_id = change_feeds_import_get_distributor_id($user->uid);
  if (!empty($dispatch_date)) {
    $trip_ids =  _get_vrs_trip_ids_using_dispatch_date($dispatch_date);
    $note_denominations = get_notes_denominations(); 
    if(!empty($trip_ids)) {
      $vrs_trips_denominations =  get_vrs_trips_denominations($trip_ids,$dis_id);
      $coins_sum = get_vrs_coins_value($trip_ids,$dis_id);
      $deno_details = get_vrs_trips_deno_details($trip_ids,$dis_id);
      $counter = count($note_denominations) + 2;
      if (!empty($deno_details)) {
          $table2 = '<table width="100%" border="1"><tr><th>Trip ID</th>';
          if (!empty($note_denominations)) {
             foreach($note_denominations as $key=>$value) {
                 $table2 .= '<th>'.$value['title'].'</th>';
             } 
             
          }
          $table2 .= '<th>Total</th>';
          $table2 .= '<th>COIN</th>';
          $table2 .= '<th>total</th></tr>';
          $sum_deno = 0;
          $sum_tot_without_coin = 0;
          $sum_tot_with_coin = 0;
              foreach($deno_details as $key=>$value) {
               $table2 .= '<tr>';
               $tot_deno = $value['no_notes'] * $key;
               $sum_deno += $tot_deno;
               $table2 .= '<td align="right"><b>'.$key .'</b></td>';
               $tot = 0;
               foreach($note_denominations as $key1=>$value1) {
                    $table2 .= '<td align="right"><b>'.$value[$value1['title']] .'</b></td>';
                    $tot = $tot + (int)$value[$value1['title']] * (int)$value1['title'];
               }
               $sum_tot_without_coin += $tot;
               $total = (int)$tot + $value['coins'];
               $sum_tot_with_coin += $total;
               $table2 .= '<td align="right"><b>'.$tot .'</b></td>';
               $table2 .= '<td align="right"><b>'.$value['coins'] .'</b></td>';
               $table2 .= '<td align="right"><b>'.$total .'</b></td>';
               $table2 .= '</tr>';
              
            
          }
           $table2 .= '<tr><td align="right" colspan="' . $counter . '"><b>' . $sum_tot_without_coin . '</b></td>';
           $table2 .= '<td>';
           $table2 .= '<td align="right"><b>'.$sum_tot_with_coin . '</b></td></tr>';
           $table2 .= '</table>';
          }

       
      if (!empty($vrs_trips_denominations)) {
          $table1 = '<table width="100%" border="1"><tr><th>NOTES</th><th>No OF NOTES</th><th>TOTAL</th></tr>';
          $sum_deno = 0;
          foreach($vrs_trips_denominations as $key=>$value) {
               $table1 .= '<tr>';
               $tot_deno = $value['no_notes'] * $key;
               $sum_deno += $tot_deno;
               $table1 .= '<td align="right"><b>'.$key .'</b></td>';
               $table1 .= '<td align="right"><b>'.$value['no_notes'] .'</b></td>';
               $table1 .= '<td align="right"><b>'.$tot_deno .'</b></td>';
               $table1 .= '</tr>';
          }
          $table1 .= '<tr><td></td><td></td><td><b>'.$sum_deno.'</b></td></tr>';
          $table1 .= '<tr><td><b>'.t('ADD Coins').'</b></td><td></td><td><b>'.$coins_sum.'</b></td></tr>';
          $table1 .= '<tr><td></td><td></td><td></td></tr>';
          $table1 .= '<tr><td></td><td></td><td><b>'.($coins_sum + $sum_deno).'</b></td></tr>';
          $table1 .= '</table>';
          
      }
    }
    $table = '<table width="100%">';
    $table .= '<tr><td><b>' .t('Dispatch Date: ').date('d/m/Y',strtotime($dispatch_date)).'</td></tr>';
    $table .= '<tr><td></td></tr>'; 
    $table .= '<tr><td>' .$table1.'</td>';
    $table .= '<td> </td>';
    $table .= '<td>' .$table2.'</td>';
    $table .= '</tr>';
    $table .= '</table>';
    cache_set('trips_denominations_report' . session_id(), $table, 'cache', time() + 60 * 10);
    $form_state['redirect'] = 'cash-denomination-report/export';
  }
}
function cash_denomination_report_export() {
  $file = "trips_cash_denomination_report.xls";
  $data = cache_get('trips_denominations_report' . session_id());
  if (!empty($data)) {
    $output = $data->data;
  }
  header("Content-Type: application/force-download");
  header("Content-type: application/vnd.ms-excel");
  header("Content-Disposition: attachment; filename=$file");
  echo $output;
}


