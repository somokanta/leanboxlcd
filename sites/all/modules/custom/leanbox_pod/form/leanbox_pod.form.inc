<?php

/**
 * Files containing form for petty cash
 */

/**
 * 
 * @param array $form
 * @param type $form_state
 * @return array
 */
function _set_pod_threshold_callback($form, &$form_state) {
   $form = array();
   $form['#prefix'] = '<div id="petty-cash-div">';
   $form['#suffix'] = '</div>';

   // get logged in users distributor id
   $user_id = $GLOBALS['user']->uid;
   $distributor_id = change_feeds_import_get_distributor_id($user_id);
   // get default threshold callback here
   $default_pod_threshold = _get_pod_threshold_value($distributor_id);

   $form['distributor_id'] = array(
     '#type' => 'value',
     '#value' => $distributor_id,
   );
   $form['pod_threshold'] = array(
     '#type' => 'textfield',
     '#title' => t('Pod Threshold'),
     '#default_value' => $default_pod_threshold,
     '#element_validate' => array('element_validate_integer_positive'),
   );
   $form['submit'] = array(
     '#value' => 'submit',
     '#type' => 'submit',
     '#size' => 20,
   );
   return $form;
}

/**
 *  Submit Handler for _set_pod_threshold_callback
 * @param type $form
 * @param type $form_state
 */
function _set_pod_threshold_callback_submit($form, &$form_state) {
   $distributor_id = $form_state['values']['distributor_id'];
   $pod_threshold = $form_state['values']['pod_threshold'];
   
   if (!empty($distributor_id) && !empty($pod_threshold)) {
      db_merge('pod_flat_table')
          ->key(array('distributor_id' => $distributor_id))
          ->fields(array(
            'pod_threshold' => $pod_threshold,
          ))
          ->execute();
   }
}
