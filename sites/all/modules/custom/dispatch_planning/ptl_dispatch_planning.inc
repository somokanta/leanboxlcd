<?php

/*
 * it will call from, "ptl-api-data" services
 * 
 */

function fetch_ptl_data($rs_code, $api_time) {
  $time = time() - 60 * 5; // send data which is created 5min befor, to avoid data loose of trip

  $query = db_select('ptl_data_configuration', 'cb');
  $query->addExpression('MAX(id)', 'max');
  $query->condition('api_flag', '0');
  $query->condition('rs_code', $rs_code, '=');
  $query->condition('created_time', $time, '<');
  $max_ptl_data = $query->execute()->fetchField();

  $query = db_select('ptl_data_configuration', 'p');
  $query->fields('p', array('ptl_job', 'rs_code', 'trip_id', 'vehicle', 'bill_number', 'loading_date', 'sequence_no', 'basepack_code', 'sku_code', 'product_name', 'MRP', 'TUR'));
  $query->addExpression('loose', 'sum_sales_qty'); // send loose value in sum_sales_qty
  $query->condition('p.id', $max_ptl_data, '<=');
  $query->condition('p.api_flag', '0', '=');
  $query->condition('p.rs_code', $rs_code, '=');
  $query->condition('p.loose', 1, '>=');

  $result = $query->execute()->fetchAll();

  if (!empty($result)) {
    $results['result'] = json_encode($result);
    $results['flag'] = '1';

    db_update('ptl_data_configuration') // Table name no longer needs {}
      ->fields(array(
        'fetch_time' => $api_time,
      ))
      ->condition('id', $max_ptl_data, '<=')
      ->condition('api_flag', '0', '=')
      ->condition('rs_code', $rs_code, '=')
      ->condition('loose', 1, '>=')
      ->execute();
  }
  else {
    $results['flag'] = '0';
    $results['result'] = 'No Result';
  }
  // log entry and is remaining

  return $results;
}

/*
 * it will call from, "ptl-api-data" services to update the time
 * 
 */

function update_ptl_data($rs_code, $trip_id, $update_time) {

  $query = db_select('ptl_data_configuration', 'cb');
  $query->addExpression('MAX(id)', 'max');
  $query->condition('trip_id', $trip_id, '=');
  $query->condition('rs_code', $rs_code, '=');
  $max_ptl_data = $query->execute()->fetchField();

  $results['flag'] = '0';
  $results['result'] = 'Data not found. Please check RS Code and Trip Id';

  if ($max_ptl_data) {
    db_update('ptl_data_configuration') // Table name no longer needs {}
      ->fields(array(
        'update_time' => $update_time,
        'api_flag' => 1,
      ))
      ->condition('rs_code', $rs_code, '=')
      ->condition('trip_id', $trip_id, '=')
      ->condition('loose', 1, '>=')
      ->execute();

    $results['flag'] = '1';
    $results['result'] = 'Updated Successfully';
  }
  return $results;
}

// PTL Loose PDF REPORT
function ptl_loose_sheet_form($form, &$form_state) {
  $form['#prefix'] = '<div id="itemlist-div">';
  $form['#suffix'] = '</div>';

  $form['trip_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Trip Id'),
    '#size' => 10,
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('submit'),
    '#ajax' => array(
      'wrapper' => 'itemlist-div',
      'callback' => 'get_data_of_ptl_sheet_form_submit',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    '#submit' => array('ptl_loose_sheet_form_submit'),
  );
  if (!empty($form_state['views_data'])) {
    $form['hhh'] = array(
      '#type' => 'markup',
      '#markup' => $form_state['views_data'],
    );
    $trip_id = $form_state['values']['trip_id'];
    $rr = l('Open PDF', "/download-ptl-loose-sheet/$trip_id");
    $form['dowmload_mark'] = array(
      '#type' => 'markup',
      '#markup' => $rr,
    );
  }
  return $form;
}

function get_data_of_ptl_sheet_form_submit($form, &$form_state) {
  return $form;
}

function ptl_loose_sheet_form_submit($form, &$form_state) {
  $trip_id = $form_state['values']['trip_id'];
  $view_name = "ptl_loose_sheet";
  $view = views_get_view($view_name, '');
  $view->set_arguments(array($trip_id));
  $rendered_html = $view->render();
  $form_state['views_data'] = $rendered_html;
  $form_state['rebuild'] = TRUE;
}

function download_ptl_loose_sheet($args) {
  $view_name = "ptl_loose_sheet";
  $view = views_get_view($view_name, '');
  $view->set_arguments(array($args));
  $rendered_html = $view->render();
  pdf_using_mpdf_api($rendered_html);
}


//*************PTL PDF CLD REPORT***************
function ptl_cld_sheet_form($form, &$form_state) {
  $form['#prefix'] = '<div id="itemlist-div">';
  $form['#suffix'] = '</div>';

  $form['trip_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Trip Id'),
    '#size' => 10,
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('submit'),
    '#ajax' => array(
      'wrapper' => 'itemlist-div',
      'callback' => 'get_data_of_ptl_sheet_form_submit',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    '#submit' => array('ptl_cld_sheet_form_submit'),
  );
  if (!empty($form_state['views_data'])) {
    $form['hhh'] = array(
      '#type' => 'markup',
      '#markup' => $form_state['views_data'],
    );
    $trip_id = $form_state['values']['trip_id'];
    $rr = l('Open PDF', "/download-ptl-cld-sheet/$trip_id");
    $form['dowmload_mark'] = array(
      '#type' => 'markup',
      '#markup' => $rr,
    );
  }
  return $form;
}

function ptl_cld_sheet_form_submit($form, &$form_state) {
  $trip_id = $form_state['values']['trip_id'];
  $view_name = "ptl_cld_sheet";
  $view = views_get_view($view_name, '');
  $view->set_arguments(array($trip_id));
  $rendered_html = $view->render();
  $form_state['views_data'] = $rendered_html;
  $form_state['rebuild'] = TRUE;
}

function download_ptl_cld_sheet($args) {
  $view_name = "ptl_cld_sheet";
  $view = views_get_view($view_name, '');
  $view->set_arguments(array($args));
  $rendered_html = $view->render();
  pdf_using_mpdf_api($rendered_html);
}
