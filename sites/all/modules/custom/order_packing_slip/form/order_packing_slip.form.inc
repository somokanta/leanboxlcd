<?php

function order_packing_slip_form($form, &$form_state){
  $form = array();
  $form['#prefix'] = '<div id="order-packing-slip-div">';
  $form['#suffix'] = '</div>';
  global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);
  $form_state['distributor_id'] = $distributor_id;
  $form['from_date'] = array(
    '#type' => 'date_popup',
    '#title' => 'Bill From date',
    '#require' => TRUE,
    '#date_label_position' => 'within',
    '#date_format' => 'Y-m-d',
  );
  
  $form['to_date'] = array(
    '#type' => 'date_popup',
    '#title' => 'Bill To date',
    '#require' => TRUE,
    '#date_label_position' => 'within',
    '#date_format' => 'Y-m-d',
  );
  
  $form['save'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
    '#ajax' => array(
        'callback' => 'order_packing_slip_ajax',
          'wrapper' => 'order-packing-slip-div',
          'method' => 'replace',
          'effect' => 'fade',
     ),
    '#validate' => array('order_packing_slip_form_validate'),
    '#submit' => array('order_packing_slip_form_submit'),
  );
  $table_header_data = $form_state['values']['order_numbers'];
  $table_row_data = $form_state['values']['table_data'];
  if(!empty($table_row_data)){
    $header = array(
      'sr_no' => 'Sr No.',
      'sku7_code' => 'SKU7 Code',
      'product_name' => 'Product Name',
      'mrp' => 'MRP',
      'total_put_qty' => 'Total Qty'
    );
    $main_title = '<table style ="margin-left:39%;"><tr><th><h1>Order Packing Slip</h1></th></tr></table>';
    foreach($table_row_data as $key => $val){
      $header_data_arr = json_decode($val['header_data'], true);
      $row_data_arr = json_decode($val['row_data'], true);   
      $main_header = '<table class = "preview_table" width = "100%" cellspacing = "0" style = "font-family:arial; font-size: 13px; color:#000;" cellpadding = "5" border = "0" align = "center">
        <tbody>
        <tr>
          <td>Invoice Number:</td>
          <td align = "left" style="color:#666">' . $table_header_data[$val['order_no']]['bill_number'] . '</td>
          <td>Bill Date</td>
          <td align = "left" style="color:#666">' . $table_header_data[$val['order_no']]['bill_date'] . '</td>
          <td>Beat Name:</td>
          <td align = "left" style="color:#666">' . $table_header_data[$val['order_no']]['beat_name'] . '</td>
        </tr>
        
        <tr>
          <td>Party Code:</td>
          <td align = "left" style="color:#666">' . $table_header_data[$val['order_no']]['party_code'] . '</td>
          <td>Box Id Number:</td>
          <td align = "left" style="color:#666">' . $val['box_id'] . '</td>
          <td>Seal Numbers</td>
          <td align = "left" style="color:#666">' . $header_data_arr['seal_number'] . '</td>
        </tr> 
        <tr>
          <td>Party Name:</td>
          <td align = "left" style="color:#666">' . $table_header_data[$val['order_no']]['party_name'] . '</td>
        </tr> 
        </tbody>
        </table><br>';
        $form['order_table_data'][$key] = array(
          '#prefix' => '<div>'.$main_header, //$main_title.
          '#theme' => 'table',
          '#header' => $header,
          '#rows' => $row_data_arr,
          '#empty' => t('No records found'),
          '#suffix' => '</div>'
        );
    } // end of foreach()  
     $from_bill_date = $form_state['values']['from_bill_date'];
     $to_bill_date = $form_state['values']['to_bill_date'];
    
    $open_pdf_link = l('Open PDF', "/download-order-packing-data/$from_bill_date/$to_bill_date" ,array('attributes'=>array("target" => "_blank",'class' => array('print-pdf'))));
    $form['dowmload_mark'] = array(
      '#type' => 'markup',
      '#markup' => $open_pdf_link,
    );
  }
  return $form;
}

function order_packing_slip_form_validate($form,&$form_state){
  $from_bill_date = $form_state['values']['from_date'];
  $to_bill_date = $form_state['values']['to_date'];
  if(empty($from_bill_date) || empty($to_bill_date)){
    form_set_error('form', "Please Select From and To date");
    unset($form_state['complete form']['order_table_data']);
    unset($form_state['complete form']['dowmload_mark']);
  }
}

function order_packing_slip_form_submit($form,&$form_state){
  $from_bill_date = strtotime($form_state['values']['from_date']);
  $to_bill_date = strtotime($form_state['values']['to_date']);
  $distributor_id = $form_state['distributor_id'];
  //dpm($distributor_id, 'distributor_id: ');
  if(!empty($from_bill_date) && !empty($to_bill_date)){
    $query = db_select('node', 'n');
    $query->condition('n.type', 'sales_register_data');
    $query->condition('n.uid', $distributor_id, '=');
    $query->leftjoin('field_data_field_sr_new_bill_date', 'bd', 'n.nid = bd.entity_id');
    $query->condition('bd.field_sr_new_bill_date_value',array($from_bill_date, $to_bill_date), 'BETWEEN');
    $query->leftjoin('field_data_field_crate_id', 'crate', 'n.nid = crate.entity_id');
    $query->leftjoin('field_data_field_outlet_order_no', 'oon', 'n.nid = oon.entity_id');
    $query->leftjoin('field_data_field_party_code_reference','party_code_ref','party_code_ref.entity_id = n.nid');
    $query->leftjoin('node','n1','n1.nid = party_code_ref.field_party_code_reference_nid');
    $query->condition('n1.type', 'outlet_master');
    $query->leftjoin('field_data_field_party_code','party_code','n1.nid = party_code.entity_id');
    $query->leftjoin('field_data_field_party_name','party_name','n1.nid = party_name.entity_id');
    $query->leftjoin('field_data_field_beat_name','beat_name','n1.nid = beat_name.entity_id');
    $query->fields('beat_name', array('field_beat_name_value'));
    $query->fields('party_code', array('field_party_code_value'));
    $query->fields('party_name', array('field_party_name_value'));
    $query->fields('n', array('title')); // bill number
    $query->fields('oon', array('field_outlet_order_no_value')); // order number
    $query->fields('crate', array('field_crate_id_value')); // box id comma separated  number
    $query->fields('bd', array('field_sr_new_bill_date_value'));
    $results = $query->execute()->fetchAll(PDO::FETCH_ASSOC);   
    $order_numbers =  array();
    foreach($results as $val){
      $boxids = explode(',', $val['field_crate_id_value']);
      if (count($boxids) > 1) {
        $order_numbers[$val['field_outlet_order_no_value']]['crate_id'] = $val['field_crate_id_value'];
        $order_numbers[$val['field_outlet_order_no_value']]['bill_number'] = $val['title'];
        $order_numbers[$val['field_outlet_order_no_value']]['bill_date'] = date('d-M-Y', $val['field_sr_new_bill_date_value']);
        $order_numbers[$val['field_outlet_order_no_value']]['party_code'] = $val['field_party_code_value'];
        $order_numbers[$val['field_outlet_order_no_value']]['beat_name'] = $val['field_beat_name_value'];
        $order_numbers[$val['field_outlet_order_no_value']]['party_name'] = $val['field_party_name_value'];
      }
    }
    
    $unique_orders_arr = array_keys($order_numbers);
    $table_data = array();
    if(!empty($unique_orders_arr)){
      $query4 = db_select('order_packing_slip_api_data', 'opad');
      $query4->fields('opad', array('order_no','party_name','box_id','seal_number','total_qty','header_data', 'row_data')); 
      $query4->condition('opad.order_no', $unique_orders_arr, 'IN');  //$unique_crate_ids
      $table_data = $query4->execute()->fetchAll(\PDO::FETCH_ASSOC);
    }
    $form_state['values']['table_data'] = $table_data;
    $form_state['values']['order_numbers'] = $order_numbers;
    $form_state['values']['from_bill_date'] = $from_bill_date;
    $form_state['values']['to_bill_date'] = $to_bill_date;
    $form_state['rebuild'] = TRUE;
  }
}

function download_order_packing_slip($from_bill_date,$to_bill_date){
  global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);
   if(!empty($from_bill_date) && !empty($to_bill_date)){
    $query = db_select('node', 'n');
    $query->condition('n.type', 'sales_register_data');
    $query->condition('n.uid', $distributor_id, '=');
    $query->leftjoin('field_data_field_sr_new_bill_date', 'bd', 'n.nid = bd.entity_id');
    $query->condition('bd.field_sr_new_bill_date_value',array($from_bill_date, $to_bill_date), 'BETWEEN');
    $query->leftjoin('field_data_field_crate_id', 'crate', 'n.nid = crate.entity_id');
    $query->leftjoin('field_data_field_outlet_order_no', 'oon', 'n.nid = oon.entity_id');
    $query->leftjoin('field_data_field_party_code_reference','party_code_ref','party_code_ref.entity_id = n.nid');
    $query->leftjoin('node','n1','n1.nid = party_code_ref.field_party_code_reference_nid');
    $query->condition('n1.type', 'outlet_master');
    $query->leftjoin('field_data_field_party_code','party_code','n1.nid = party_code.entity_id');
    $query->leftjoin('field_data_field_party_name','party_name','n1.nid = party_name.entity_id');
    $query->leftjoin('field_data_field_beat_name','beat_name','n1.nid = beat_name.entity_id');
    $query->fields('beat_name', array('field_beat_name_value'));
    $query->fields('party_code', array('field_party_code_value'));
    $query->fields('party_name', array('field_party_name_value'));
    $query->fields('n', array('title')); // bill number
    $query->fields('oon', array('field_outlet_order_no_value')); // order number
    $query->fields('crate', array('field_crate_id_value')); // box id comma separated  number
    $query->fields('bd', array('field_sr_new_bill_date_value'));
    $results = $query->execute()->fetchAll(PDO::FETCH_ASSOC);   
    $order_numbers =  array();
    foreach($results as $val){
      $boxids = explode(',', $val['field_crate_id_value']);
      if (count($boxids) > 1) {
        $order_numbers[$val['field_outlet_order_no_value']]['crate_id'] = $val['field_crate_id_value'];
        $order_numbers[$val['field_outlet_order_no_value']]['bill_number'] = $val['title'];
        $order_numbers[$val['field_outlet_order_no_value']]['bill_date'] = date('d-M-Y', $val['field_sr_new_bill_date_value']);
        $order_numbers[$val['field_outlet_order_no_value']]['party_code'] = $val['field_party_code_value'];
        $order_numbers[$val['field_outlet_order_no_value']]['beat_name'] = $val['field_beat_name_value'];
        $order_numbers[$val['field_outlet_order_no_value']]['party_name'] = $val['field_party_name_value'];
      }
    }
    $table_data = array();
    $unique_orders_arr = array_keys($order_numbers);
    if(!empty($unique_orders_arr)){
      $query4 = db_select('order_packing_slip_api_data', 'opad');
      $query4->fields('opad', array('order_no','party_name','box_id','seal_number','total_qty','header_data', 'row_data')); 
      $query4->condition('opad.order_no', $unique_orders_arr, 'IN');  //$unique_crate_ids
      $table_data = $query4->execute()->fetchAll(\PDO::FETCH_ASSOC);
    }
    $table_row_data = $table_data;
    //dpm($table_row_data);
    $table_header_data = $order_numbers;
    //dpm($table_header_data,'Header');
    $header = array(
      'sr_no' => 'Sr No.',
      'sku7_code' => 'SKU7 Code',
      'product_name' => 'Product Name',
      'mrp' => 'MRP',
      'total_put_qty' => 'Total Qty'
    );
    $main_title = '<table style ="margin-left:39%;"><tr><th><h1>Order Packing Slip</h1></th></tr></table>';
    $i = 1;
    $table = '';
    $table.= $main_title;
    foreach($table_row_data as $key => $val){
      $header_data_arr = json_decode($val['header_data'], true);
      $row_data_arr = json_decode($val['row_data'], true);
      $table .= '<table class = "preview_table" width = "100%" cellspacing = "0" style = "font-family:arial; font-size: 13px; color:#000;" cellpadding = "5" border = "0" align = "center">
        <tbody>
        <tr>
          <td>Invoice Number:</td>
          <td align = "left" style="color:#666">' . $table_header_data[$val['order_no']]['bill_number'] . '</td>
          <td>Bill Date</td>
          <td align = "left" style="color:#666">' . $table_header_data[$val['order_no']]['bill_date'] . '</td>
          <td>Beat Name:</td>
          <td align = "left" style="color:#666">' . $table_header_data[$val['order_no']]['beat_name'] . '</td>
        </tr>
        
        <tr>
          <td>Party Code:</td>
          <td align = "left" style="color:#666">' . $table_header_data[$val['order_no']]['party_code'] . '</td>
          <td>Box Id Number:</td>
          <td align = "left" style="color:#666">' . $val['box_id'] . '</td>
          <td>Seal Numbers</td>
          <td align = "left" style="color:#666">' . $header_data_arr['seal_number'] . '</td>
        </tr> 
        <tr>
          <td>Party Name:</td>
          <td align = "left" style="color:#666">' . $table_header_data[$val['order_no']]['party_name'] . '</td>
        </tr> 
        </tbody>
        </table><br>';
      $table .= get_html_table($header, $row_data_arr);
      if(count($table_row_data) !== $i){
        $table .= "<pagebreak />";
      }
      $i++;
    }
    pdf_using_mpdf_api($table);
  }
}


function get_html_table($headers, $datas) {
  if (!empty($headers) && !empty($datas)) {
    $border_class = " style='border:0.1pt solid black;border-collapse: collapse;'";
    $table = '<table ' . $border_class . ' cellspadding="0" cellspacing="0"><tr>';
    foreach ($headers as $key => $value) {
      $table .= '<th' . $border_class . '>' . $value . '</th>';
    }
    $table .= '</tr>';
    foreach ($datas as $key => $row) {
      $table .= '<tr>';
      foreach ($row as $row_id => $row_value) {
        $table .= '<td' . $border_class . '>' . $row_value . '</td>';
      }
      $table .= '</tr>';
    }

    $table .= '</table>';
  }
  return $table;
 }
 
 function order_packing_slip_ajax($form, &$form_state) {
   return $form;
}