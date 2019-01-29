<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function parkstop_check_form($form, $form_state) {
    $form['trip_number'] = array(
      '#type' => 'textfield',
      '#title' => t('Trip ID'),
      '#required' => TRUE,
  );

    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Show Trip Details'),
    );
    return $form;
}

function parkstop_check_form_validate($form, &$form_state) {

}

function parkstop_check_form_submit($form, &$form_state) {
  $trip_number = $form_state['values']['trip_number'];
  $prevan = get_tp_van_by_trip_id($trip_number);
  $url = 'park-stop-plotting/'.$prevan.'/'. $trip_number;
  $form_state['redirect'] = $url;
}
