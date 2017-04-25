<?php

module_load_include('inc', 'reverse_geocode', 'address_confirm');

function delivery_beat_create_form($form, &$form_state) {

  global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);

  $days = change_feeds_import_get_terms_using_vocubulary('Outlet Days');
  $sales_day_data = array_combine($days, $days);

  $form['sales_day'] = array(
    '#type' => 'select',
    '#title' => t('Sales Day'),
    '#options' => $sales_day_data,
    '#ajax' => array(
      'event' => 'change',
      'callback' => 'delivery_beat_sales_day_ajax_callback',
      'wrapper' => 'van-pl-wrapper',
    ),
    '#required' => TRUE,
  );


  $form['van_pl_group'] = array(
    '#type' => 'select',
    '#title' => t('Van PL Group'),
    '#options' => array(),
    '#ajax' => array(
      'event' => 'change',
      'callback' => 'delivery_beat_van_pl_group_ajax_callback',
      'wrapper' => 'area-hook-group-wrapper',
    ),
    '#prefix' => '<div id="van-pl-wrapper">',
    '#suffix' => '</div>',
      //'#required' => TRUE,
    "#empty_option" => t('- Select -'),
  );


  $form['area_hook_group'] = array(
    '#type' => 'select',
    '#title' => t('Area Hook Group'),
    '#options' => array(),
    '#prefix' => '<div id="area-hook-group-wrapper">',
    '#suffix' => '</div>',
      //'#required' => TRUE,
    "#empty_option" => t('- Select -'),
  );

  if (!empty($form_state['values']['sales_day'])) {
    $sales_day = $form_state['values']['sales_day'];
    $form['van_pl_group']['#options'] = reverse_geocode_get_data_from_sales_day($sales_day, $distributor_id);
  }

  if (!empty($form_state['values']['sales_day']) && !empty($form_state['values']['van_pl_group'])) {
    $sales_day = $form_state['values']['sales_day'];
    $van = $form_state['values']['van_pl_group'];
    $form['area_hook_group']['#options'] = reverse_geocode_get_data_for_area_group($sales_day, $van, $distributor_id);
  }

  $form['filter_submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
    '#ajax' => array(
      'wrapper' => 'delivery-beat-form-wrapper',
      'callback' => 'delivery_beat_filters_ajax',
    ),
    '#submit' => array('delivery_beat_filters_submit'),
  );

  $form['results_container'] = array(
    '#prefix' => '<div id="delivery-beat-form-wrapper">',
    '#suffix' => '</div>',
  );

  $rows = $form_state['reverse_geocode']['results'];
  if (!empty($rows)) {
    $header = array('nid', 'PARTY_CODE', 'PARTY_NAME', 'Beat Name', 'CHANNEL', 'PL_GROUP', 'Sales Day', 'Pl Van Group', 'Area Hook', 'Area Hook Group', 'Sequence', 'Delivery Beat Code');
    
    $form['results_container']['download'] = array(
      '#type' => 'submit',
      '#value' => t('Download'),
//      '#ajax' => array(
//        'wrapper' => 'delivery-beat-form-wrapper',
//        'callback' => 'delivery_beat_filters_ajax',
//      ),
      '#submit' => array('delivery_beat_download_submit'),
    );

    $form['results_container']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Submit'),
      '#submit' => array('delivery_beat_form_submit'),
    );
    
    $form['results_container']['table'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
    );
  }

  return $form;
}

function delivery_beat_filters_submit($form, &$form_state) {
  $sales_day = $form_state['values']['sales_day'];
  $van_pl_group = $form_state['values']['van_pl_group'] ? $form_state['values']['van_pl_group'] : NULL;
  $area_hook_group = $form_state['values']['area_hook_group'] ? $form_state['values']['area_hook_group'] : NULL;

  $results = delivery_beat_get_results($sales_day, $van_pl_group, $area_hook_group);

  if (empty($results)) {
    $form_state['reverse_geocode']['results'] = array();
    drupal_set_message('No results found', 'error');
  }
  else {
    $form_state['reverse_geocode']['results'] = $results;
  }

  $form_state['rebuild'] = TRUE;
}

function delivery_beat_download_submit($form, &$form_state) {
  $header = array('nid', 'PARTY_CODE', 'PARTY_NAME', 'Beat Name', 'CHANNEL', 'PL_GROUP', 'Sales Day', 'Pl Van Group', 'Area Hook', 'Area Hook Group', 'Sequence', 'Delivery Beat Code');

  header('Content-Type: text/csv; charset=utf-8');
  header('Content-Disposition: attachment; filename=RG.csv');
  $fp = fopen('php://output', 'w');
  fputcsv($fp, $header);

  $csv_rows = $form_state['reverse_geocode']['results'];
  foreach ($csv_rows as $k => $v) {
    fputcsv($fp, $v);
  }
  fclose($fp);
  exit;
}

function delivery_beat_sales_day_ajax_callback(&$form, &$form_state) {
  return $form['van_pl_group'];
}

function delivery_beat_van_pl_group_ajax_callback(&$form, &$form_state) {
  return $form['area_hook_group'];
}

function delivery_beat_filters_ajax(&$form, &$form_state) {
  return $form['results_container'];
}

function delivery_beat_form_submit($form, &$form_state) {
  $outlets = $form_state['reverse_geocode']['results'];

  $operations = array();
  $data_chunk = array_chunk($outlets, 100);
  foreach ($data_chunk as $data) {
    $operations[] = array('delivery_beat_batch_process', array($data));
  }
  $batch = array(
    'title' => t('Updating'),
    'operations' => $operations,
    'init_message' => t("Batch is starting."),
    'progress_message' => t('Processed @current out of @total.'),
    'error_message' => t('Processing has encountered an error.'),
    'finished' => 'delivery_beat_batch_finished',
    'file' => drupal_get_path('module', 'reverse_geocode') . '/delivery_beat_form.inc',
  );

  batch_set($batch);
}

function delivery_beat_get_results($sales_day, $van_pl_group = NULL, $area_hook_group = NULL) {
  require_once drupal_get_path('module', 'change_feeds_import') . '/form/upload_billing_data.inc';
  
  global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);
  
  $stop_size = 10;
  $nid = check_node_title_exists($distributor_id, 'van_stop_master');
  if ($nid) {
    $node = node_load($nid);
    $stop_size = $node->field_stop_size[LANGUAGE_NONE][0]['value'];
  }
  
  $query = db_select('node', 'n');

  $query->addTag('node_uid');

  //Outlet Master
  $query->join('field_data_field_sales_day', 'fsd', 'fsd.entity_id = n.nid');
  $query->join('field_data_field_revised_sequence', 'fs', 'fs.entity_id = n.nid');
  $query->join('field_data_field_channel', 'fc1', 'fc1.entity_id = n.nid');
  $query->join('field_data_field_pl_group', 'fp1', 'fp1.entity_id = n.nid');
  $query->join('field_data_field_area_hook', 'fa', 'fa.entity_id = n.nid');
  $query->join('field_data_field_hul_code', 'fh', 'fh.entity_id = n.nid');
  $query->join('field_data_field_party_name', 'fp', 'fp.entity_id = n.nid');
  $query->join('field_data_field_beat_name', 'fb', 'fb.entity_id = n.nid');

  $query->join('field_data_field_area_hook_group_hul', 'faconf', 'faconf.entity_id = n.nid');

  $query->condition('faconf.field_area_hook_group_hul_value', 1);
  $query->condition('fsd.field_sales_day_value', $sales_day);
  $query->condition('n.type', 'outlet_master');
  $query->fields('n', array('nid', 'title'));
  $query->fields('fp', array('field_party_name_value'));
  $query->fields('fb', array('field_beat_name_value'));
  $query->fields('fc1', array('field_channel_value'));
  $query->fields('fp1', array('field_pl_group_value'));
  $query->fields('fsd', array('field_sales_day_value'));
  
  
  //Area hook Master
  $query->join('node', 'na', 'na.nid = fa.field_area_hook_nid');

  $query->join('field_data_field_dva_sales_day', 'sd', 'sd.entity_id = na.nid');
  $query->condition('sd.field_dva_sales_day_value', $sales_day, '=');

  $query->join('field_data_field_dva_area_hook_group_name', 'agr', 'agr.entity_id = na.nid');
  $query->join('field_data_field_dva_van_pl_group', 'ppplg', 'na.nid = ppplg.entity_id');
  $query->fields('ppplg', array('field_dva_van_pl_group_value'));
  if (!is_null($area_hook_group)) {
    $query->condition('agr.field_dva_area_hook_group_name_value', $area_hook_group, '=');
  }
  if (!is_null($van_pl_group)) {
    $query->condition('ppplg.field_dva_van_pl_group_value', $van_pl_group, '=');
  }
  
  $query->join('node', 'nag', 'agr.field_dva_area_hook_group_name_value = nag.title');
  $query->join('field_data_field_area_hook_group_id', 'fagi', 'fagi.entity_id = nag.nid');
  $query->condition('nag.type', 'area_hook_group_master');
  $query->condition('nag.uid', $distributor_id, '=');

  //Sales Delivery Day Master
  $query->join('node', 'nsd', 'nsd.title = fsd.field_sales_day_value');
  $query->join('field_data_field_day_number', 'fdn', 'fdn.entity_id = nsd.nid');
  $query->condition('nsd.type', 'sales_delivery_day_master');
  $query->condition('nsd.uid', $distributor_id, '=');

//HUL code Master
  $query->join('node', 'nh', 'nh.title = fh.field_hul_code_value');
  $query->join('field_data_field_hul_status', 'fhs', 'fhs.entity_id = nh.nid');
  $query->condition('nh.uid', $distributor_id, '=');

//Party pack Master
  //$query->join('field_data_field_channel', 'fc2', 'fc1.field_channel_value = fc2.field_channel_value');
  //$query->join('field_data_field_pl_group', 'fp2', 'fp2.field_pl_group_value = fp1.field_pl_group_value AND fp2.entity_id=fc2.entity_id');
  //$query->join('field_data_field_van_pl_group', 'fvp', 'fvp.entity_id = fp2.entity_id');
  //$query->join('field_data_field_delivery_beats', 'fdb', 'fdb.entity_id = fp2.entity_id');
  //$query->join('field_data_field_delivery_per_van', 'fdpv', 'fdpv.entity_id = fp2.entity_id');
  //$query->join('node', 'npp', 'npp.nid = fp2.entity_id');

  //$query->condition('npp.uid', $distributor_id, '=');
  //$query->condition('fc2.bundle', 'party_pack_master');
  //$query->condition('fp2.bundle', 'party_pack_master');
  //$query->fields('fvp', array('field_van_pl_group_value'));



  $query->fields('fs', array('field_revised_sequence_value'));
  $expression = "CONCAT_WS('-',"
      . "LPAD(fdn.field_day_number_value, 2, '0'), "
      . "LPAD(ppplg.field_dva_van_pl_group_value, 2, '0'),"
      . "fagi.field_area_hook_group_id_value,"
      . "sub.area,"
      . "LPAD(CEIL(fs.field_revised_sequence_value/$stop_size), 3, '0')"
      . ")";
  
  $query->addExpression($expression, 'exp');

  $query->orderBy('ppplg.field_dva_van_pl_group_value');
  $query->orderBy('agr.field_dva_area_hook_group_name_value');
  $query->orderBy('fs.field_revised_sequence_value');
  
  
  $subquery = db_select('node', 'n');
  $subquery->addTag('node_uid');

  //Outlet Master
  $subquery->join('field_data_field_sales_day', 'fsd', 'fsd.entity_id = n.nid');
  $subquery->join('field_data_field_revised_sequence', 'fs', 'fs.entity_id = n.nid');
  $subquery->join('field_data_field_area_hook', 'fa', 'fa.entity_id = n.nid');
  $subquery->join('node', 'na', 'na.nid = fa.field_area_hook_nid');
  
  $subquery->join('node', 'na', 'na.nid = fa.field_area_hook_nid');
  $subquery->join('field_data_field_dva_sales_day', 'sd', 'sd.entity_id = na.nid');
  $subquery->condition('sd.field_dva_sales_day_value', $sales_day, '=');
  $subquery->join('field_data_field_dva_van_pl_group', 'ppplg', 'na.nid = ppplg.entity_id');
  
  
  //HUL Code Master
  $subquery->join('field_data_field_hul_code', 'hc', 'hc.entity_id = n.nid');
  $subquery->join('node', 'hul', 'hc.field_hul_code_value = hul.title');
  $subquery->condition('hul.type', 'hul_code_master');
  $subquery->condition('hul.uid', $distributor_id, '=');
  $subquery->join('field_data_field_updated_address', 'addr', 'addr.entity_id = hul.nid');
  
  //Party pack Master
  //$subquery->join('field_data_field_channel', 'fc2', 'fc1.field_channel_value = fc2.field_channel_value');
  //$subquery->join('field_data_field_pl_group', 'fp2', 'fp2.field_pl_group_value = fp1.field_pl_group_value AND fp2.entity_id=fc2.entity_id');
  //$subquery->join('field_data_field_delivery_beats', 'fdb', 'fdb.entity_id = fp2.entity_id');
  //$subquery->join('node', 'npp', 'npp.nid = fp2.entity_id');
  //$subquery->condition('npp.uid', $distributor_id, '=');
  
  $subquery->fields('ppplg', array('field_dva_van_pl_group_value'));
  $subquery->fields('fs', array('field_revised_sequence_value'));
  $subquery->addExpression('COALESCE(addr.field_updated_address_sub_premise, addr.field_updated_address_dependent_locality, addr.field_updated_address_locality)', 'area');
  
  ############ CHANGE 10 to DYNAMIC####################
  $subquery->where("((fs.field_revised_sequence_value - 1)%($stop_size) = 0)");
  
  $query->join($subquery, 'sub', "sub.field_revised_sequence_value = (FLOOR((fs.field_revised_sequence_value - 1)/$stop_size)*$stop_size + 1) AND sub.field_dva_van_pl_group_value = ppplg.field_dva_van_pl_group_value");

  $res = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

  return $res;
}

function delivery_beat_batch_process($data, &$context) {
  foreach ($data as $key => $value) {
    $node = node_load($value['nid']);
    if ($node) {
      //Save static delivery beat
      $node->field_static_delivery_beat_code‎ = $value['exp'];
      node_save($node);
    }
  }
}

function delivery_beat_batch_finished($success, $results, $operations) {

  if ($success) {
    
  }
  else {
    drupal_set_message('Error Occured while updating');
  }
}
