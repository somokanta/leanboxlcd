<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @param array $form
 * @param type $form_state
 * @return array
 */
function outlet_working_hours_form($form, &$form_state) {
  $form['#attached']['js'][] = drupal_get_path('module', 'change_feeds_import') . '/js/working_hours.js';

  $form['#prefix'] = '<div id="working-hours-itemlist-div" class="custom_form_wrap">';
  $form['#suffix'] = '</div>';

  $form['hul_code'] = array(
    '#type' => 'textfield',
    '#title' => t('HUL Code'),
    '#autocomplete_path' => 'hul-code/autocomplete',
    '#ajax' => array(
      'wrapper' => 'working-hours-itemlist-div',
      'callback' => 'hul_code_details_return_callback',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    '#validate' => array('hul_code_validation'),
    '#prefix' => '<div class="form_row">',
    '#suffix' => '</div>',
  );
  global $user;
  $distributor_id = change_feeds_import_get_distributor_id($user->uid);
  $hul_code = $form_state['values']['hul_code'];
  if ($form_state['triggering_element']['#name'] == 'hul_code') {
    for ($i = 0; $i<=6; $i++){
      unset($form_state[$i.'_field_deltas']);
      unset($form_state['input']['field_container_'.$i]);
    }
    unset($form_state['special_days_deltas']);
    unset($form_state['special_field_deltas']);
    unset($form_state['input']['special_field_container']);
  }

  if (!empty($hul_code) && empty($form_state['error'])) {
    $hul_party_name = get_hul_party_name($hul_code, $distributor_id);
    $hul_code_details = 'Working Hours for ' . '<b>' . $hul_party_name . '</b>' . ' :';
    $days = array('0' => 'Monday', '1' => 'Tuesday', '2' => 'Wednesday', '3' => 'Thursday', '4' => 'Friday', '5' => 'Saturday', '6' => 'Sunday');

    $form['hul_code_details'] = array(
      '#markup' => '<div class="main-wrapper"><div class="hul_code_details">' . $hul_code_details . '</div>',
    );

    foreach ($days as $key => $value) {

      $working_hours = get_working_hours($hul_code, $value);
      $class = '';
      $default_field_delta = array_keys($working_hours);
        if ($working_hours[0]->from_time == 'Closed' && $working_hours[0]->to_time == 'Closed') {
          $class = 'closed-day';
        }

        $form[$key]['field_container_' . $key] = array(
          '#type' => 'container',
          '#weight' => 10,
          '#tree' => TRUE,
          '#prefix' => '<div id="js-ajax-elements-wrapper_' . $key . '">',
          '#suffix' => '</div>',
        );
        
        $form_state[$key . '_field_deltas'] = isset($form_state[$key . '_field_deltas']) ? $form_state[$key . '_field_deltas'] : $default_field_delta;
        
        $form[$key]['field_container_' . $key]['days'] = array(
          '#markup' => "<div class='outlet days-field'>$value</div>",
        );
        
        $form[$key]['field_container_' . $key]['outlet_checkbox'] = array(
          '#type' => 'checkbox',
          '#title' => t(''),
          '#attributes' => array('class' => array('working-checkbox', 'outlet-checkbox-' . $key, 'checkbox-open-close', $class)),
          '#default_value' => TRUE,
          '#prefix' => '<div class="toggle_area checkbox-field-' . $key . '">',
          '#suffix' => '</div>',
        );

        $form[$key]['field_container_' . $key]['open_close_text'] = array(
          '#markup' => '<div class="outlet-text open-close-' . $key . '">Open</div>'
        );
        $counter = 0;
        foreach ($form_state[$key . '_field_deltas'] as $delta) {
          
          $form[$key]['field_container_' . $key][$delta] = array(
            '#type' => 'container',
            '#attributes' => array(
              'class' => array('container-inline'),
            ),
            '#tree' => TRUE,
            //'#prefix' => '<div class="time-field from-to-time-' . $key . '">',
          );
          if($counter ===0){
            $form[$key]['field_container_' . $key][$delta]['#prefix'] = '<div class="time-field from-to-time-' . $key . '">';
            $counter++;
          }
          
          $form[$key]['field_container_' . $key][$delta]['from_time_' . $key . '_' . $delta] = array(
            '#type' => 'textfield',
            '#title' => t(''),
            '#size' => 10,
            //'#name' => 'from_field1_' . $key . '_' . $delta,
            '#default_value' => $working_hours[$delta]->from_time ? $working_hours[$delta]->from_time : '',
            '#attributes' => array(
              'class' => array('from-field', 'from-' . $key),
            ),
          );

          $form[$key]['field_container_' . $key][$delta]['to_time_' . $key . '_' . $delta] = array(
            '#type' => 'textfield',
            '#title' => t('-'),
            '#size' => 10,
            //'#name' => 'to_field1_' . $key . '_' . $delta,
            '#default_value' => $working_hours[$delta]->to_time ? $working_hours[$delta]->to_time : '',
            '#attributes' => array(
              'class' => array('to-field', 'to-' . $key),
            ),
          );

          $form[$key]['field_container_' . $key][$delta]['remove_time'] = array(
            '#type' => 'submit',
            '#value' => t('X'),
            '#submit' => array('working_time_add_more_remove'),
            '#ajax' => array(
              'callback' => 'working_time_add_more_remove_callback',
              'wrapper' => 'js-ajax-elements-wrapper_' . $key
            ),
            '#weight' => 50,
            '#attributes' => array(
              'class' => array('remove-button'),
            ),
            //'#suffix' => '</div>',
            '#name' => 'remove_' . $key . '_' . $delta,
          );
        }
        
      $form[$key]['field_container_' . $key]['add_time'] = array(
        '#type' => 'submit',
        '#value' => t('ADD HOURS'),
        '#name' => 'add_' . $key . '_' . $delta,
        '#submit' => array('working_time_add_more_add_one'),
        '#ajax' => array(
          'callback' => 'working_time_add_more_add_one_callback',
          'wrapper' => 'js-ajax-elements-wrapper_' . $key,
        ),
        '#weight' => 100,
        '#prefix' => "<div class='add-button'>",
        '#suffix' => '</div></div>',
      );
    }

    // Special Days.
    $form['special_field_container'] = array(
      '#type' => 'container',
      '#weight' => 80,
      '#tree' => TRUE,
      '#prefix' => '<div id="special-elements-wrapper">',
      '#suffix' => '</div>',
    );

    $form['special_days'] = array(
      '#markup' => "<div class='special-days'>Add Special Days:</div>",
      '#prefix' => "<div class='special-days-wrapper'>",
    );

    $form['add_special_days'] = array(
      '#type' => 'submit',
      '#value' => t('+'),
      '#submit' => array('add_special_working_time_add_more_add_one'),
      '#ajax' => array(
        'callback' => 'add_special_working_time_add_more_add_one_callback',
        'wrapper' => 'special-elements-wrapper',
      ),
      '#attributes' => array('class' => array('add_special_days')),
      '#suffix' => '</div>',
    );
    
    $special_working_days = get_special_working_days($hul_code);
    $special_days_deltas = array_keys($special_working_days);

    $form_state['special_days_deltas'] = isset($form_state['special_days_deltas']) ? $form_state['special_days_deltas'] : $special_days_deltas;
    
    foreach ($form_state['special_days_deltas'] as $key => $value) {
      
      if (isset($special_working_days[$value]->special_days)) {
        $special_working_hours = get_special_working_hours($hul_code, $special_working_days[$value]->special_days);
        $special_hrs_delta_default = array_keys($special_working_hours);
      }
      else {
        $special_working_hours = array();
        $special_hrs_delta_default = array(0 => 0);
      }

      $special_class = '';
      if ($special_working_hours[0]->from_time == 'Closed' && $special_working_hours[0]->to_time == 'Closed') {
          $special_class = 'closed-special-day';
      }
      
      $form['special_field_container'][$value]['special_date_field'] = array(
        '#type' => 'date_popup',
        //'#title' => 'This is a date filter',
        '#date_format' => 'Y-m-d',
        '#date_year_range' => '-5:+5', //allow 5 years past and future
        '#attributes' => array('class' => array('special-date-container')),
        '#default_value' => $special_working_days[$value]->special_days ? date('Y-m-d',$special_working_days[$value]->special_days) : '',
        //'#required' => TRUE,
        '#prefix' => "<div class='special-days-container-wrapper'>",
      );

      $form['special_field_container'][$value]['special_outlet_checkbox'] = array(
        '#type' => 'checkbox',
        '#title' => t(''),
        '#attributes' => array('class' => array('special-working-checkbox', 'special-outlet-checkbox-' . $value, 'checkbox-open-close', $special_class, 'working-checkbox')),
        '#default_value' => TRUE,
        '#prefix' => "<div class='toggle_area checkbox-field'>",
        '#suffix' => '</div>',
      );
      $form['special_field_container'][$value]['special_open_close_text'] = array(
        '#markup' => '<div class="outlet-text special-open-close-' . $value . '">Open</div>'
      );

      $form['special_field_container'][$value]['special_time_container'] = array(
        '#type' => 'container',
        '#prefix' => '<div id="special-time-container">',
        '#suffix' => '</div>',
      );

      $form_state['special_field_deltas'][$value] = isset($form_state['special_field_deltas'][$value]) ? $form_state['special_field_deltas'][$value] : $special_hrs_delta_default;
      
       foreach ($form_state['special_field_deltas'][$value] as $special_delta) {
        $form['special_field_container'][$value]['special_time_container'][$special_delta] = array(
          '#type' => 'container',
          '#attributes' => array(
            'class' => array('container-inline'),
          ),
          '#tree' => TRUE,
          '#prefix' => '<div class="special-time-field special-from-to-time-' . $value . '">',
        );

        $form['special_field_container'][$value]['special_time_container'][$special_delta]['specialfromtime_' . $value . '_' . $special_delta] = array(
          '#type' => 'textfield',
          '#title' => t(''),
          '#size' => 10,
          //'#name' => 'specialfromtime_' . $value . '_' . $special_delta,
          '#default_value' => $special_working_hours[$special_delta]->from_time ? $special_working_hours[$special_delta]->from_time : '',
          '#attributes' => array(
            'class' => array('from-field'),
          ),
          //'#required' => TRUE,
        );

        $form['special_field_container'][$value]['special_time_container'][$special_delta]['specialtotime_' . $value . '_' . $special_delta] = array(
          '#type' => 'textfield',
          '#title' => t('-'),
          '#size' => 10,
          //'#name' => 'specialtotime_' . $value . '_' . $special_delta,
          '#default_value' => $special_working_hours[$special_delta]->to_time ? $special_working_hours[$special_delta]->to_time :'',
          '#attributes' => array(
            'class' => array('to-field'),
          ),
          //'#required' => TRUE,
        );

        $form['special_field_container'][$value]['special_time_container'][$special_delta]['special_remove_time'] = array(
          '#type' => 'submit',
          '#value' => t('X'),
          '#submit' => array('special_working_time_add_more_remove'),
          '#ajax' => array(
            'callback' => 'special_working_time_add_more_remove_callback',
            'wrapper' => 'special-elements-wrapper',
          ),
          '#weight' => 50,
          '#attributes' => array(
            'class' => array('special-button-small'),
          ),
          '#name' => 'removetime_' . $value . '_' . $special_delta,
          '#suffix' => '</div>',
        );
      }

      $form['special_field_container'][$value]['special_time_container'][$special_delta]['special_add_time'] = array(
        '#type' => 'submit',
        '#value' => t('ADD HOURS'),
        '#submit' => array('special_working_time_add_more_add_one'),
        '#name' => 'addtime_' . $value,
        '#ajax' => array(
          'callback' => 'special_working_time_add_more_add_one_callback',
          'wrapper' => 'special-elements-wrapper',
        ),
        '#weight' => 100,
      );

      $form['special_field_container'][$value]['special_delete'] = array(
        '#type' => 'submit',
        '#value' => t('DELETE'),
        '#submit' => array('special_working_time_add_more_delete_special'),
        '#ajax' => array(
          'callback' => 'special_working_time_add_more_delete_special_callback',
          'wrapper' => 'special-elements-wrapper',
        ),
        '#weight' => 50,
        '#attributes' => array(
          'class' => array('special-delete-button'),
        ),
        '#name' => 'delete_' . $value,
        '#suffix' => '</div>',
      );
    }

    // Save Button.
    $form['save'] = array(
      '#type' => 'submit',
      '#value' => 'Save',
      '#name' => 'save',
      '#attributes' => array('class' => array('save-working-hours')),
      '#submit' => array('outlet_working_hours_submit'),
      '#validate' => array('outlet_working_hours_validate'),
      '#weight' => 170,
      '#prefix' => "<div class='submit-wrapper' class='save-button'>",
    );

    // Cancel Button.
    $form['cancel'] = array(
      '#type' => 'submit',
      '#value' => 'Cancel',
      '#name' => 'cancel',
      '#attributes' => array('class' => array('cancel-working-hours'),
        'onclick' => 'if(!confirm("Are you sure you want to Cancel?")){return false;}',
        'style' => array('float: left;')),
      '#weight' => 180,
      '#prefix' => "<div class='cancel-submit-wrapper' class='cancel-button'>",
      '#suffix' => '</div></div></div>',
    );
  }
  return $form;
}

function working_time_add_more_remove($form, &$form_state) {
  $wrapper = $form_state['triggering_element']['#name'];
  $day_arr = explode('_', $wrapper);
  $days = $day_arr[1];
  $delta_remove = $day_arr[2];

  unset($form_state[$days . '_field_deltas'][$delta_remove]);

  $form_state['rebuild'] = TRUE;
}

function working_time_add_more_remove_callback($form, &$form_state) {
  $wrapper = explode('_', $form_state['triggering_element']['#name']);
  return $form[$wrapper[1]]['field_container_' . $wrapper[1]];
}

function working_time_add_more_add_one($form, &$form_state) {
  $wrapper = $form_state['triggering_element']['#name'];
  $day_arr = explode('_', $wrapper);
  $days = $day_arr[1];

  $m = max($form_state[$days . '_field_deltas']);
  $m = (empty($m) && $m !== 0) ? 0 : $m + 1;
  $form_state[$days . '_field_deltas'][$m] = $m;

  $form_state['rebuild'] = TRUE;
}

function working_time_add_more_add_one_callback($form, &$form_state) {
  $wrapper = explode('_', $form_state['triggering_element']['#name']);
  return $form[$wrapper[1]]['field_container_' . $wrapper[1]];
}

// Special Days.
function special_working_time_add_more_remove($form, &$form_state) {
  $wrapper = $form_state['triggering_element']['#name'];
  $day_arr = explode('_', $wrapper);
  $days = $day_arr[1];
  $delta_remove = $day_arr[2];

  unset($form_state['special_field_deltas'][$days][$delta_remove]);
  $form_state['rebuild'] = TRUE;
}

function special_working_time_add_more_remove_callback($form, &$form_state) {
  return $form['special_field_container'];
}

function special_working_time_add_more_add_one($form, &$form_state) {
  $wrapper = $form_state['triggering_element']['#name'];
  $day_arr = explode('_', $wrapper);
  $counts = $day_arr[1];

  $m = max($form_state['special_field_deltas'][$counts]);
  $m = (empty($m) && $m !== 0) ? 0 : $m + 1;
  $form_state['special_field_deltas'][$counts][$m] = $m;
  $form_state['rebuild'] = TRUE;
}

function special_working_time_add_more_add_one_callback($form, &$form_state) {
  return $form['special_field_container'];
}

// Add Special Days
function add_special_working_time_add_more_add_one($form, &$form_state) {
  $m = max($form_state['special_days_deltas']);
  $m = (empty($m) && $m !== 0) ? 0 : $m + 1;
  $form_state['special_days_deltas'][$m] = $m;

  $form_state['rebuild'] = TRUE;
}

function add_special_working_time_add_more_add_one_callback($form, &$form_state) {
  return $form['special_field_container'];
}

// Delete Special.
function special_working_time_add_more_delete_special($form, &$form_state) {
  $wrapper = $form_state['triggering_element']['#name'];
  $day_arr = explode('_', $wrapper);
  $days = $day_arr[1];

  unset($form_state['special_days_deltas'][$days]);
  $form_state['rebuild'] = TRUE;
}

function special_working_time_add_more_delete_special_callback($form, &$form_state) {
  return $form['special_field_container'];
}

function hul_code_details_return_callback($form, &$form_state) {
  $commands[] = ajax_command_replace('#working-hours-itemlist-div', render($form));
  $commands[] = ajax_command_replace('.alert-danger', '');
  $commands[] = ajax_command_replace('.alert-success', '');
  $commands[] = ajax_command_prepend('#working-hours-itemlist-div', theme('status_messages'));
  return array('#type' => 'ajax', '#commands' => $commands);
}

function hul_code_validation($form, &$form_state) {
  $form_state['error'] = 0;
  global $user;
  $distributor_id = change_feeds_import_get_distributor_id($user->uid);
  $hul_code = $form_state['values']['hul_code'];

  $query = db_select('node', 'n');
  $query->condition('n.type', 'hul_code_master');
  $query->condition('n.title', $hul_code);
  $query->condition('n.uid', $distributor_id);
  $query->fields('n', array('title'));
  $result = $query->execute()->fetchField();

  if (empty($result) && !empty($hul_code)) {
    drupal_set_message('HUL Code does not exist in HUL Code Master', 'error');
    $form_state['error'] = 1;
  }
}

function outlet_working_hours_validate($form, &$form_state) {
  $mess = '';
  $days = array('0' => 'Monday', '1' => 'Tuesday', '2' => 'Wednesday', '3' => 'Thursday', '4' => 'Friday', '5' => 'Saturday', '6' => 'Sunday');

  foreach ($days as $key => $value) {
    $field_container_value = $form_state['values']['field_container_' . $key];
    unset($field_container_value['outlet_checkbox']);
    unset($field_container_value['add_time']);
    foreach ($field_container_value as $k => $val) {
      unset($val['remove_time']);
      $from_field_val = $val['from_time_' . $key . '_' . $k];
      $to_field_val = $val['to_time_' . $key . '_' . $k];

      if (!empty($from_field_val) && !empty($to_field_val)) {
        // From time validation.
        $from_time_flag = 0;
        if (!empty($from_field_val) && $from_field_val != 'Closed') {
          $from_time = explode(':', $from_field_val);

          if (count($from_time) != 2) {
            $from_time_flag = 1;
          }

          if (!empty($from_time)) {
            $from_hour = (int) $from_time[0];
            $from_minutes_array = explode(' ', $from_time[1]);
            $from_minutes = (int) $from_minutes_array[0];
            $from_am_pm = strtolower(trim($from_minutes_array[1]));
          }

          if ($from_hour > 12 || !is_numeric($from_hour) || $from_hour <= 0) {
            $from_time_flag = 1;
          }
          if ($from_minutes > 60 && is_numeric($from_minutes)) {
            $from_time_flag = 1;
          }
          if ($from_minutes < 0 && !is_numeric($from_minutes)) {
            $from_time_flag = 1;
          }
          if ($from_minutes < 0) {
            $from_time_flag = 1;
          }
          if ($from_am_pm != 'am' && $from_am_pm != 'pm') {
            $from_time_flag = 1;
          }
        }

        // To time validation.
        $to_time_flag = 0;
        if (!empty($to_field_val) && $to_field_val != 'Closed') {
          $to_time = explode(':', $to_field_val);
          if (count($to_time) != 2) {
            $to_time_flag = 1;
          }
          if (!empty($to_time)) {
            $to_hour = (int) $to_time[0];
            $to_minutes_array = explode(' ', $to_time[1]);
            $to_minutes = (int) $to_minutes_array[0];
            $to_am_pm = strtolower(trim($to_minutes_array[1]));
          }

          if ($to_hour > 12 || !is_numeric($to_hour) || $to_hour <= 0) {
            $to_time_flag = 1;
          }
          if ($to_minutes > 60 && is_numeric($to_minutes)) {
            $to_time_flag = 1;
          }
          if ($to_minutes < 0 && !is_numeric($to_minutes)) {
            $to_time_flag = 1;
          }
          if ($to_minutes < 0) {
            $to_time_flag = 1;
          }
          if ($to_am_pm != 'am' && $to_am_pm != 'pm') {
            $to_time_flag = 1;
          }
        }
      }

      if (empty($from_field_val) && !empty($to_field_val)) {
        $mess .= t('Please enter From Time for ' . $value) . '<br>';
      }
      if (!empty($from_field_val) && empty($to_field_val)) {
        $mess .= t('Please enter To Time for ' . $value) . '<br>';
      }
    }

    if ($from_time_flag == 1) {
      $mess .= t('Please enter valid From Time for ' . $value . '. Allowed format is hh:mm AM/PM') . '<br>';
    }
    if ($to_time_flag == 1) {
      $mess .= t('Please enter valid To Time for ' . $value . '. Allowed format is hh:mm AM/PM') . '<br>';
    }
  }

  // Special Days Validation.
  $special_fields = $form_state['values']['special_field_container'];

  foreach ($special_fields as $spe_field_key => $spe_field_value) {

    $special_time = $form_state['input']['special_field_container'][$spe_field_key]['special_time_container'];

    unset($form_state['input']['special_field_container'][$spe_field_key]['special_outlet_checkbox']);
    foreach ($special_time as $spe_key => $spe_value) {
      $special_date = $form_state['values']['special_field_container'][$spe_field_key]['special_date_field'];
      $specialfromtime = $spe_value['specialfromtime_' . $spe_field_key . '_' . $spe_key];
      $specialtotime = $spe_value['specialtotime_' . $spe_field_key . '_' . $spe_key];
      if(empty($special_date)) {
        $mess .= t('Please enter Special Date') . '<br>';
      }
      if (!empty($specialfromtime) && !empty($specialtotime)) {
        // Special From time validation.

        $spe_from_time_flag = 0;
        if (!empty($specialfromtime) && ($specialfromtime != 'Closed')) {
          $spe_from_time = explode(':', $specialfromtime);
          if (count($spe_from_time) != 2) {
            $spe_from_time_flag = 1;
          }

          if (!empty($spe_from_time)) {
            $spe_from_hour = (int) $spe_from_time[0];
            $spe_from_minutes_array = explode(' ', $spe_from_time[1]);
            $spe_from_minutes = (int) $spe_from_minutes_array[0];
            $spe_from_am_pm = strtolower(trim($spe_from_minutes_array[1]));
          }
          if ($spe_from_hour > 12 || !is_numeric($spe_from_hour) || $spe_from_hour <= 0) {
            $spe_from_time_flag = 1;
          }
          if ($spe_from_minutes > 60 && is_numeric($spe_from_minutes)) {
            $spe_from_time_flag = 1;
          }
          if ($spe_from_minutes < 0 && !is_numeric($spe_from_minutes)) {
            $spe_from_time_flag = 1;
          }
          if ($spe_from_minutes < 0) {
            $spe_from_time_flag = 1;
          }
          if ($spe_from_am_pm != 'am' && $spe_from_am_pm != 'pm') {
            $spe_from_time_flag = 1;
          }
        }

        // Special To time validation.

        $spe_to_time_flag = 0;
        if (!empty($specialtotime) && ($specialtotime != 'Closed')) {
          $spe_to_time = explode(':', $specialtotime);
          if (count($spe_to_time) != 2) {
            $spe_to_time_flag = 1;
          }
          if (!empty($spe_to_time)) {
            $spe_to_hour = (int) $spe_to_time[0];
            $spe_to_minutes_array = explode(' ', $spe_to_time[1]);
            $spe_to_minutes = (int) $spe_to_minutes_array[0];
            $spe_to_am_pm = strtolower(trim($spe_to_minutes_array[1]));
          }
          if ($spe_to_hour > 12 || !is_numeric($spe_to_hour) || $spe_to_hour <= 0) {
            $spe_to_time_flag = 1;
          }
          if ($spe_to_minutes > 60 && is_numeric($spe_to_minutes)) {
            $spe_to_time_flag = 1;
          }
          if ($spe_to_minutes < 0 && !is_numeric($spe_to_minutes)) {
            $spe_to_time_flag = 1;
          }
          if ($spe_to_minutes < 0) {
            $spe_to_time_flag = 1;
          }
          if ($spe_to_am_pm != 'am' && $spe_to_am_pm != 'pm') {
            $spe_to_time_flag = 1;
          }
        }
      }
      if ($spe_from_time_flag == 1) {
        $mess .= t('Please enter valid From Time for ' . $special_date . '. Allowed format is hh:mm AM/PM') . '<br>';
      }
      if ($spe_to_time_flag == 1) {
        $mess .= t('Please enter valid To Time for ' . $special_date . '. Allowed format is hh:mm AM/PM') . '<br>';
      }
    }
  }


  if (!empty($mess)) {
    form_set_error('form', $mess);
  }
}

function outlet_working_hours_submit($form, &$form_state) {
  global $user;
  $distributor_id = change_feeds_import_get_distributor_id($user->uid);

  $days = array('0' => 'Monday', '1' => 'Tuesday', '2' => 'Wednesday', '3' => 'Thursday', '4' => 'Friday', '5' => 'Saturday', '6' => 'Sunday');
  $hul_code = $form_state['values']['hul_code'];
  $outlet_days = check_exist_in_outlet_working($hul_code);
  $hul_party_name = get_hul_party_name($hul_code, $distributor_id);
  
  db_delete('outlet_working_hours')
    ->condition('hul_code', $hul_code)
    ->execute();

  foreach ($days as $key => $value) {
    $field_container_value = $form_state['input']['field_container_' . $key];
    unset($field_container_value['outlet_checkbox']);
    unset($field_container_value['add_time']);
    foreach ($field_container_value as $k => $val) {
      unset($val['remove_time']);
      $from_field_val = $val['from_time_' . $key . '_' . $k];
      $to_field_val = $val['to_time_' . $key . '_' . $k];

      if ($form_state['input']['field_container_' . $key]['outlet_checkbox'] != 0) {
          $insert = db_insert('outlet_working_hours')
            ->fields(array(
            'hul_code' => $hul_code,
            'days' => $value,
            'from_time' => $from_field_val,
            'to_time' => $to_field_val,
            'updated_time' => time()
          ));
          $insert->execute();
      }
      else if ($form_state['input']['field_container_' . $key]['outlet_checkbox'] == 0) {
          $insert = db_insert('outlet_working_hours')
            ->fields(array(
            'hul_code' => $hul_code,
            'days' => $value,
            'from_time' => 'Closed',
            'to_time' => 'Closed',
            'updated_time' => time()
          ));
          $insert->execute();
      }  
    }
      }
    // Submit Special Days data.
    $special_fields = $form_state['input']['special_field_container'];
    foreach ($special_fields as $spe_field_key => $spe_field_value) {
      $special_time = $form_state['input']['special_field_container'][$spe_field_key]['special_time_container'];
      //unset($form_state['input']['special_field_container'][$spe_field_key]['special_outlet_checkbox']);
      foreach ($special_time as $spe_key => $spe_value) {
        $special_date = strtotime($form_state['input']['special_field_container'][$spe_field_key]['special_date_field']['date']);
        $specialfromtime = $spe_value['specialfromtime_' . $spe_field_key . '_' . $spe_key];
        $specialtotime = $spe_value['specialtotime_' . $spe_field_key . '_' . $spe_key];

        if (!empty($specialfromtime) && !empty($specialtotime) && $form_state['values']['special_field_container'][$spe_field_key]['special_outlet_checkbox'] != 0) {
            $insert = db_insert('outlet_working_hours')
              ->fields(array(
              'hul_code' => $hul_code,
              'special_days' => $special_date,
              'from_time' => $specialfromtime,
              'to_time' => $specialtotime,
              'updated_time' => time()
            ));
            $insert->execute();
        }else if ($form_state['values']['special_field_container'][$spe_field_key]['special_outlet_checkbox'] == 0) {
          $insert = db_insert('outlet_working_hours')
              ->fields(array(
              'hul_code' => $hul_code,
              'special_days' => $special_date,
              'from_time' => 'Closed',
              'to_time' => 'Closed',
              'updated_time' => time()
            ));
            $insert->execute();
        }   
        }
    }

  drupal_set_message(t('Data saved successfully for HUL Code : ' . $hul_code . ' & Party Name : ' . $hul_party_name));
}

function get_hul_party_name($hul_code, $distributor_id) {
  $query = db_select('node', 'n');
  $query->condition('n.type', 'hul_code_master');
  $query->condition('n.title', $hul_code);
  $query->condition('n.uid', $distributor_id);
  $query->join('field_data_field_hul_party_name', 'hpn', 'hpn.entity_id = n.nid');
  $query->fields('hpn', array('field_hul_party_name_value'));
  $result = $query->execute()->fetchField();

  return $result;
}

function check_exist_in_outlet_working($hul_code) {
  $query = db_select('outlet_working_hours', 'wh');
  $query->condition('wh.hul_code', $hul_code);
  $query->fields('wh', array('days'));
  $result = $query->execute()->fetchAll();

  return $result;
}

function get_working_hours($hul_code, $days) {
  $query = db_select('outlet_working_hours', 'wh');
  $query->condition('wh.hul_code', $hul_code);
  $query->condition('wh.days', $days);
  $query->fields('wh', array('days', 'from_time', 'to_time', 'special_days'));
  $result = $query->execute()->fetchAll();

  if (empty($result)) {
    $result[0]->days = $days;
    $result[0]->from_time = '';
    $result[0]->to_time = '';
    $result[0]->special_days = '';
  }
  return $result;
}

function get_special_working_days($hul_code, $days = NULL){
  $query = db_select('outlet_working_hours', 'wh');
  $query->condition('wh.hul_code', $hul_code);
  $query->condition('wh.days', $days);
  $query->addExpression('DISTINCT(special_days)','special_days');
//  $query->addExpression('from_time','from_time');
//  $query->addExpression('to_time','to_time');
  $result = $query->execute()->fetchAll();
  return $result;
}

function get_special_working_hours($hul_code, $special_days){
  $query = db_select('outlet_working_hours', 'wh');
  $query->condition('wh.hul_code', $hul_code);
  $query->condition('wh.special_days', $special_days);
  $query->fields('wh', array('from_time', 'to_time'));
  $result = $query->execute()->fetchAll();
  return $result;
}