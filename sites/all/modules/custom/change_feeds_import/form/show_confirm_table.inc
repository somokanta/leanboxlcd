<?php

// show table after file is uploaded
function change_feeds_import_confirm_content($id) {
  global $base_url;
  $result = db_select('error_billing_data', 'bd')
          ->condition('bd.id', $id, '=')
          ->fields('bd')
          ->execute()->fetchAssoc();

  $header = array('Upload Date& Time', 'File', 'Total Bill Uploaded', 'Total Lines Uploaded', 'Sum Of Total Sales Qty', 'Error File', 'Action');

  $status = $result['status'];
  
  $rows['date'] = date("Y-m-d h:i:s", $result['date']);
    $rows['file_name'] = $result['file_name'];
    $rows['total_bill_uploaded'] = $result['total_bill_uploaded'];
    $rows['total_line_uploaded'] = $result['total_line_uploaded'];
    $rows['Sum_of_TOTAL_SALES_QTY'] = $result['Sum_of_TOTAL_SALES_QTY'];
    $discard_link = l(t('Discard'), 'discard/' . $id);
  if ($status == "Confirm") {
    $error_link = "";
    $action_link = l(t('Confirm'), 'confirm/create_content/' . $id);
    $rows['error_file'] = $error_link;
    $rows['action'] = $action_link.'/'.$discard_link;
  }elseif($status == "Error"){
    drupal_set_message('Validate Functionality is removed temporarily. Please discard and upload full data!!.','error');
    $error_link = l(t('Error file'), $result['error_file']);
    //$action_link = l(t('Validate'), 'validate/' . $id);
    $rows['error_file'] = $error_link;
    $rows['action'] = $discard_link;
  }elseif($status == "Validate"){
    drupal_set_message('Validate Functionality is removed temporarily. Please discard and upload full data!!.','error');
    //$error_link = ""; 
    $error_link = l(t('Error file'), $result['error_file']);
    //$action_link = l(t('Validate'), 'validate/' . $id);
    $rows['error_file'] = $error_link;
    //$rows['action'] = $action_link.'/'.$discard_link;
    $rows['action'] = $discard_link;
  }
  
  $build['pager_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => array($rows),
    '#empty' => t('There are no date formats found.'),
  );

  return $build;
}
