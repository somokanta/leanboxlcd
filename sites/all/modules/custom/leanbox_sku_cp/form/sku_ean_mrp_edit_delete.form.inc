<?php
/**
 * 
 * @global type $user
 * @param type $form
 * @param type $form_state
 * @return int
 * To insert/update and delete ean and mrp on sku
 */
function sku_ean_add_edit_delete($form, &$form_state) {
  $form = array();
  $form['#prefix'] = '<div id="itemlist-div">';
  $form['#suffix'] = '</div>';

  // get logged in users distributor id
  global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);
  $rs_code = dispatch_planning_get_rscode_using_uid($distributor_id);


  $form['distributor_id'] = array(
    '#type' => 'value',
    '#value' => $distributor_id,
  );
  $form['rs_code'] = array(
    '#type' => 'value',
    '#value' => $rs_code,
  );

  $form['sku'] = array(
    '#type' => 'textfield',
    '#size' => 20,
    '#autocomplete_path' => 'sku-list',
    '#title' => t('Enter Sku No'),
    '#ajax' => array(
      'callback' => '_sku_ean_list_no_ajax',
      'wrapper' => 'itemlist-div',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );

  if (isset($form_state['values']['sku']) && !empty($form_state['values']['sku'])) {
    $sku = $form_state['values']['sku'];
    $sku_details = get_sku_ean_details_using_sku($sku, $rs_code);
    unset($form_state['input']['ean']);
    unset($form_state['input']['mrp']);
    $form['ean'] = array(
      '#type' => 'textfield',
      '#size' => 20,
      '#required' => TRUE,
      '#default_value' => $sku_details[0]->ean ? $sku_details[0]->ean : '',
      '#maxlength' => 20,
      '#title' => t('Enter EAN'),
    );
    $form['mrp'] = array(
      '#type' => 'textfield',
      '#size' => 20,
      '#default_value' => $sku_details[0]->mrp ? $sku_details[0]->mrp : '',
      '#maxlength' => 20,
      '#title' => t('Enter MRP'),
    );

    $form['submit'] = array(
      '#value' => 'Submit',
      '#type' => 'submit',
      '#size' => 20,
    );
    // Show delete button if data already present
//    if (!empty($sku_details[0]->ean)) {
//      $form['delete'] = array(
//        '#value' => 'Delete',
//        '#type' => 'submit',
//        '#size' => 20,
//      );
//    }
  }

  return $form;
}

/**
 * Menu callback for AJAX additions. Render sku's list.
 */
function _sku_ean_list_no_ajax($form, &$form_state) {
  return $form;
}

/**
 * 
 * @param type $form
 * @param type $form_state
 */
function sku_ean_add_edit_delete_submit($form, &$form_state) {
  $distributor_id = $form_state['values']['distributor_id'];
  $rs_code = $form_state['values']['rs_code'];
  $sku = $form_state['values']['sku'];
  $ean = $form_state['values']['ean'];
  $mrp = $form_state['values']['mrp'];

  if ($form_state['values']['op'] == 'Submit') {

    $result = db_merge('sku_ean_mrp')
      ->key(array('sku' => $sku, 'rs_code' => $rs_code))
      ->fields(array(
        'sku' => $sku,
        'ean' => $ean,
        'mrp' => !empty($mrp) ? $mrp : 0,
        'rs_code' => $rs_code,
        'flag' => 0, // enable for api
        'created_time' => REQUEST_TIME
      ))
      ->execute();
    drupal_set_message("SKU: $sku updated successfully!!");
  }
  else if ($form_state['values']['op'] == 'Delete') {
    db_delete('sku_ean_mrp')
      ->condition('sku', $sku)
      ->condition('rs_code', $rs_code)
      ->execute();
    drupal_set_message("SKU: $sku deleted successfully!!");
  }
}
/**
 * 
 * @param type $sku
 * @param type $rs_code
 * @return type
 * return ean and mrp of sku
 */
function get_sku_ean_details_using_sku($sku, $rs_code) {
  $query = db_select('sku_ean_mrp', 'sem');
  $query->condition('sem.sku', $sku, '=');
  $query->condition('sem.rs_code', $rs_code, '=');
  $query->fields('sem', array('ean', 'mrp'));
  $exist_sku_of_ean = $query->execute()->fetchAll();
  return $exist_sku_of_ean;
}
