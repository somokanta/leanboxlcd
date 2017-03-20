<?php

function pdd_add_form($form, &$form_state) {
  require_once drupal_get_path('module', 'change_feeds_import') . '/form/upload_billing_data.inc';

  global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);
  $nid = $form_state['leanbox_misc']['nid'] = check_node_title_exists($distributor_id, 'pdd_master');

  if ($nid) {
    $node = node_load($nid);
    $pdd = $node->field_pdd[LANGUAGE_NONE][0]['value'];

    foreach ($node->field_day[LANGUAGE_NONE] as $value) {
      $wo[] = $value['value'];
    }
    $weekly_off = implode(',', $wo);
  }

  $form['weekly_off'] = array(
    '#type' => 'textfield',
    '#title' => t('Weekly Off (Enter comma separated days of the week)'),
    '#required' => TRUE,
    '#description' => 'Enter comma separated days of the week (eg: "Monday, Tuesday")',
    '#default_value' => $weekly_off,
  );

  $form['pdd'] = array(
    '#type' => 'textfield',
    '#title' => t('Delivery days'),
    '#required' => TRUE,
    '#element_validate' => array('element_validate_integer_positive'),
    '#default_value' => $pdd,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

function pdd_add_form_validate($form, &$form_state) {
  $weekly_off = $form_state['values']['weekly_off'];

  $days = array(
    'Sunday' => 'Sunday',
    'Monday' => 'Monday',
    'Tuesday' => 'Tuesday',
    'Wednesday' => 'Wednesday',
    'Thursday' => 'Thursday',
    'Friday' => 'Friday',
    'Saturday' => 'Saturday',
  );

  $entered_days = explode(',', $weekly_off);

  $entered_days = array_map('trim', $entered_days);
  $form_state['values']['entered_days'] = $entered_days;

  foreach ($entered_days as $day) {
    if (!isset($days[$day])) {
      form_set_error('weekly_off', t('Enter correct comma seperated days'));
    }
  }
}

function pdd_add_form_submit($form, &$form_state) {
  global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);

  if (empty($form_state['leanbox_misc']['nid'])) {
    $node = new stdClass();
    $node->title = $distributor_id;
    $node->type = "pdd_master";
    node_object_prepare($node); // Sets some defaults. Invokes hook_prepare() and hook_node_prepare().
    $node->language = LANGUAGE_NONE; // Or e.g. 'en' if locale is enabled
    $node->uid = $distributor_id;
    $node->status = 1; //(1 or 0): published or not
    $node->comment = 0; // 0 = comments disabled, 1 = read only, 2 = read/write

    $node->field_pdd[LANGUAGE_NONE][0]['value'] = $form_state['values']['pdd'];

    foreach ($form_state['values']['entered_days'] as $value) {
      $node->field_day[LANGUAGE_NONE][]['value'] = $value;
    }
    node_save($node);
  }
  else {
    $node = node_load($form_state['leanbox_misc']['nid']);

    $node->field_pdd[LANGUAGE_NONE][0]['value'] = $form_state['values']['pdd'];

    $node->field_day = array();
    foreach ($form_state['values']['entered_days'] as $value) {
      $node->field_day[LANGUAGE_NONE][]['value'] = $value;
    }
    node_save($node);
  }
}

function holiday_add_form($form, &$form_state) {
  require_once drupal_get_path('module', 'change_feeds_import') . '/form/upload_billing_data.inc';

  global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);
  $nid = $form_state['leanbox_misc']['nid'] = check_node_title_exists($distributor_id, 'holiday_master');
  if ($nid) {
    $node = node_load($nid);

    foreach ($node->field_holiday[LANGUAGE_NONE] as $value) {
      $urls[] = date('Y-m-d H:i:s', $value['value']);
    }
  }
  $count = count($urls);
  $form_state['holidays'] = empty($form_state['holidays']) ? empty($count) ? 1 : $count : $form_state['holidays'];

  $form['#tree'] = TRUE;
  $form['urls_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Holidays'),
    '#prefix' => '<div id="url-wrapper">',
    '#suffix' => '</div>',
  );

  for ($i = 0; $i < $form_state['holidays']; $i++) {
    $form['urls_fieldset'][$i]['url'] = array(
      '#type' => 'date_popup',
      '#date_label_position' => '',
        //'#title' => t('URL'),
        //'#title_display' => 'invisible'
    );
    if (!empty($urls[$i])) {
      $form['urls_fieldset'][$i]['url']['#default_value'] = $urls[$i];
    }
  }
  $form['urls_fieldset']['add_url'] = array(
    '#type' => 'submit',
    '#value' => t('Add one more'),
    '#submit' => array('holiday_add_form_add_more_add_one'),
    '#limit_validation_errors' => array(),
    '#ajax' => array(
      'callback' => 'holiday_add_form_add_more_callback',
      'wrapper' => 'url-wrapper',
    ),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

function holiday_add_form_add_more_add_one($form, &$form_state) {
  $form_state['holidays'] ++;
  $form_state['rebuild'] = TRUE;
}

function holiday_add_form_add_more_callback($form, $form_state) {
  return $form['urls_fieldset'];
}

function holiday_add_form_submit($form, &$form_state) {

  global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);

  foreach ($form_state['values']['urls_fieldset'] as $key => $value) {
    if (!empty($value['url']) && is_array($value))
      $values[] = $value['url'];
  }

  if (empty($form_state['leanbox_misc']['nid'])) {
    $node = new stdClass();
    $node->title = $distributor_id;
    $node->type = "holiday_master";
    node_object_prepare($node); // Sets some defaults. Invokes hook_prepare() and hook_node_prepare().
    $node->language = LANGUAGE_NONE; // Or e.g. 'en' if locale is enabled
    $node->uid = $distributor_id;
    $node->status = 1; //(1 or 0): published or not
    $node->comment = 0; // 0 = comments disabled, 1 = read only, 2 = read/write

    foreach ($values as $value) {
      $node->field_holiday[LANGUAGE_NONE][]['value'] = strtotime($value);
    }
    node_save($node);
  }
  else {
    $node = node_load($form_state['leanbox_misc']['nid']);

    $node->field_holiday = array();
    foreach ($values as $value) {
      $node->field_holiday[LANGUAGE_NONE][]['value'] = strtotime($value);
    }
    node_save($node);
  }
}
