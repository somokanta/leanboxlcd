<?php
function tester_invoice_memo_form($form, &$form_state) {
  $form = array();
  $form['#prefix'] = '<div id="tester-invoice-memo-div">';
  $form['#suffix'] = '</div>';
  global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);
  $logged_in_distributor_id = $distributor_id;
    if (in_array("central_distributor", $user->roles)) {
        $distributor_ids = get_distributor_ids();
        unset($distributor_ids[$distributor_id]);
        $child_distributors = get_child_distributor_rsname($logged_in_distributor_id);
        $distributor_ids = array('' => 'Select') + $child_distributors;
        $form['central_outer'] = array(
            '#type' => 'markup',
            '#prefix' => '<div style="color:red"><h3>Note: Select any one  Distributor Name</h3></div>',
        );
        $form['distributor_id'] = array(
            '#title' => t('Select Distributor Name'),
            '#type' => 'select',
            '#options' => $distributor_ids,
            '#required' => TRUE,
        );
        $distributor_id = $form_state['input']['distributor_id'];
    }
    // Child distributor logged in
    else {
          $form['distributor_id'] = array(
            '#type' => 'hidden',
            '#value' => $distributor_id,
          );
    }
    $form['from_date'] = array(
        '#type' => 'date_popup',
        '#title' => 'Order From Date',
        '#require' => TRUE,
        '#date_label_position' => 'within',
        '#date_format' => 'Y-m-d',
    );
  
    $form['to_date'] = array(
        '#type' => 'date_popup',
        '#title' => 'Order To Date',
        '#require' => TRUE,
        '#date_label_position' => 'within',
        '#date_format' => 'Y-m-d',
    );
    $form['order_number'] = array(
        '#title' => t('Tester Order Number'),
        '#type' => 'textfield',
    );
  
    $form['save'] = array(
      '#type' => 'submit',
      '#value' => 'Submit',
      '#ajax' => array(
          'callback' => 'tester_invoice_form_ajax',
            'wrapper' => 'tester-invoice-memo-div',
            'method' => 'replace',
            'effect' => 'fade',
       ),
      '#validate' => array('tester_invoice_memo_form_validate'),
      '#submit' => array('tester_invoice_memo_form_submit'),
    );
    $table_data = $form_state['values']['table_data'];
    if (!empty($table_data)) {
        $header_array = array(
            'sr_no' => 'Sr No.',
            'basepack_code' => 'Basepack Code',
            'shade_code' => 'Shade Code',
            'shade_name' => 'Shade Name',
            'product_name' => 'Product Name',
            'qty' => 'Qty'
        );
    $main_title = '<table style ="margin-left:39%;"><tr><th><h1>Tester Invoice Memo</h1></th></tr></table>';
    foreach($table_data as $key => $val){
      $header_data_arr = json_decode($val['header_data'], true);
      $row_data_arr = json_decode($val['row_data'], true);
      $unique_box_ids= array_unique(explode(',', $header_data_arr['box_id']));
      $main_header = '<table class = "preview_table" width = "100%" cellspacing = "0" style = "font-family:arial; font-size: 13px; color:#000;" cellpadding = "5" border = "0" align = "center">
        <tbody>
        <tr>
          <td>Invoice Number:</td>
          <td align = "left" style="color:#666">' . $header_data_arr['tester_order_number'] . '</td>
          <td>Beat Name:</td>
          <td align = "left" style="color:#666">' . $header_data_arr['beat_name'] . '</td>
        </tr>
        
        <tr>
          <td>Invoice Date::</td>
          <td align = "left" style="color:#666">' . $header_data_arr['order_date'] . '</td>
          <td>Party Name:</td>
          <td align = "left" style="color:#666">' . $header_data_arr['party_name'] . '</td>
        </tr> 
        <tr>
          <td> Distributor Name:</td>
          <td align = "left" style="color:#666">' . $header_data_arr['distributor_name'] . '</td>
          <td> 	Party Code:</td>
          <td align = "left" style="color:#666">' . $header_data_arr['party_code'] . '</td>              
        </tr> 
        <tr>
          <td> Distributor Address:</td>
          <td align = "left" style="color:#666">' . $header_data_arr['distributor_address'] . '</td>
          <td> Tester Quantity (Sum):</td>
          <td align = "left" style="color:#666">' . $header_data_arr['total_picked_qty']. '</td>              
        </tr> 
        <tr>
          <td> Box IDs:</td>
          <td align = "left" style="color:#666">' . implode(',',$unique_box_ids) . '</td>
          <td> Box Count:</td>
          <td align = "left" style="color:#666">' . count($unique_box_ids) . '</td>              
            
        </tr>          
        </tbody>
        </table><br>';
        $form['order_table_data'][$key] = array(
          '#prefix' => '<div>'.$main_header, //$main_title.
          '#theme' => 'table',
          '#header' => $header_array,
          '#rows' => $row_data_arr,
          '#empty' => t('No records found'),
        );
        $form['order_table_data'][$key]['#suffix'] = "<h4 align='center'>*Non saleable material (Tester)</h4></div>";
        $from_date = $form_state['values']['from_date'];
        $to_date = $form_state['values']['to_date'];
        $order_number = $form_state['values']['order_number'];
        $distributor_id = $form_state['values']['distributor_id'];
       
        $open_pdf_link = l('Open PDF', "/download-tester-invoice-data/$from_date/$to_date/$order_number/$distributor_id" ,array('attributes'=>array("target" => "_blank",'class' => array('print-pdf'))));
        $form['dowmload_mark'] = array(
          '#type' => 'markup',
          '#markup' => $open_pdf_link,
        );
    }        
        
    }
    
    return $form;  
  
    
}

function tester_invoice_memo_form_validate ($form, &$form_state) {
    $from_date = $form_state['values']['from_date'];
    $to_date = $form_state['values']['to_date'];
    $order_number = $form_state['values']['order_number'];
    if(empty($from_date) && empty($to_date) && empty($order_number)){
    form_set_error('form', "Please Select From and To date");
    }    
    
}

function tester_invoice_memo_form_submit ($form, &$form_state) {
    $from_date = $form_state['values']['from_date'];
    $to_date = $form_state['values']['to_date'];
    $order_number = $form_state['values']['order_number'];  
    $distributor_id = $form_state['values']['distributor_id'];
    $invoice_data = get_tester_invoice_memo ($from_date,$to_date,$order_number,$distributor_id);
    $form_state['values']['table_data'] = $invoice_data;
    $form_state['values']['from_fate'] = $from_date;
    $form_state['values']['to_date'] = $to_date;
    $form_state['values']['order_number'] = $order_number;
    $form_state['rebuild'] = TRUE;
  }

function tester_invoice_form_ajax ($form, &$form_state) {
    return $form;
}


function download_tester_invoice_slip($from_date,$to_date,$order_number,$distributor_id) {
   $distributor_id = arg(4);
   $invoice_data = get_tester_invoice_memo($from_date,$to_date,$order_number,$distributor_id);
   $order_count = count($invoice_data);
   $header_array = array(
        'sr_no' => 'Sr No.',
        'basepack_code' => 'Basepack Code',
        'shade_code' => 'Shade Code',
        'shade_name' => 'Shade Name',
        'product_name' => 'Product Name',
        'qty' => 'Qty'
   );
    $main_title = '<table style ="margin-left:39%;"><tr><th><h1>Tester Invoice Memo</h1></th></tr></table>';
    $i = 1;
    $table = '';
    $table.= $main_title;
    foreach($invoice_data as $key => $val){
      $header_data_arr = json_decode($val['header_data'], true);
      $row_data_arr = json_decode($val['row_data'], true);
      $unique_box_ids= array_unique(explode(',', $header_data_arr['box_id']));
      $main_header = '<table class = "preview_table" width = "100%" cellspacing = "0" style = "font-family:arial; font-size: 13px; color:#000;" cellpadding = "5" border = "0" align = "center">
        <tbody>
        <tr>
          <td>Invoice Number:</td>
          <td align = "left" style="color:#666">' . $header_data_arr['tester_order_number'] . '</td>
          <td>Beat Name:</td>
          <td align = "left" style="color:#666">' . $header_data_arr['beat_name'] . '</td>
        </tr>
        
        <tr>
          <td>Invoice Date::</td>
          <td align = "left" style="color:#666">' . $header_data_arr['order_date'] . '</td>
          <td>Party Name:</td>
          <td align = "left" style="color:#666">' . $header_data_arr['party_name'] . '</td>
        </tr> 
        <tr>
          <td> Distributor Name:</td>
          <td align = "left" style="color:#666">' . $header_data_arr['distributor_name'] . '</td>
          <td> 	Party Code:</td>
          <td align = "left" style="color:#666">' . $header_data_arr['party_code'] . '</td>              
        </tr> 
        <tr>
          <td> Distributor Address:</td>
          <td align = "left" style="color:#666">' . $header_data_arr['distributor_address'] . '</td>
          <td> Tester Quantity (Sum):</td>
          <td align = "left" style="color:#666">' . $header_data_arr['total_picked_qty']. '</td>              
        </tr> 
        <tr>
          <td> Box IDs:</td>
          <td align = "left" style="color:#666">' . implode(',',$unique_box_ids) . '</td>
          <td> Box Count:</td>
          <td align = "left" style="color:#666">' . count($unique_box_ids) . '</td>              
            
        </tr>          
        </tbody>
        </table><br>';
        $table .= get_html_table($header_array, $row_data_arr);
        if($order_count !== $i){
          $table .= "<pagebreak />";
        }
        $i++;
    }
    pdf_using_mpdf_api($table);
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



function get_tester_invoice_memo ($from_date,$to_date,$order_number,$distributor_id) {
    $invoice_data = [];
    if (!empty($from_date) && !empty($to_date) || !empty($order_number) || !empty($distributor_id)) {
        $query = db_select('tester_invoice_memo', 'invoice');
        if(!empty($from_date) && !empty($to_date)) {
            $query->condition('invoice.order_date',array(strtotime($from_date), strtotime($to_date)), 'BETWEEN');
        }
        if(!empty($order_number)) {
            $query->condition('invoice.order_no', $order_number);
        }
        $query->condition('invoice.distributor_id', $distributor_id, '=');
        $query->fields('invoice', array('header_data','row_data'));
        $invoice_data = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
    }
    return $invoice_data;
}


function get_tester_invoice_memo_one ($from_date,$to_date,$order_number,$distributor_id) {
    return 1;
}