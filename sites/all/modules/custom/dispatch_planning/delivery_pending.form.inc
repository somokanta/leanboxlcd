<?php

function delivery_pending_form($form, &$form_state) {

  $form['#prefix'] = '<div id="itemlist-div">';
  $form['#suffix'] = '</div>';

  $form['#attached']['js'][] = drupal_get_path('module', 'dispatch_planning') . '/js/dispatch_planning_form.js';
  $form['#attached']['css'][] = drupal_get_path('module', 'dispatch_planning') . '/css/dispatch_planning.css';


  $form['bill_number'] = array(
    '#prefix' => '<div class="textfield-wrapper">',
    '#type' => 'textfield',
    '#title' => t('Bill number'),
    '#required' => TRUE,
    '#autocomplete_path' => 'bill/pending/autocomplete/1'
  );

  $form['bill_number_submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add Bill'),
    '#ajax' => array(
      'wrapper' => 'itemlist-div',
      'callback' => 'custom_ajax_callback',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    '#submit' => array('delivery_pending_bill_number_ajax_submit'),
    '#validate' => array('bill_number_ajax_validate'),
    '#limit_validation_errors' => array(array('bill_number')),
    '#suffix' => '</div>',
  );

  if (!isset($form_state['dispatch_planning']['bills'])) {
    $form_state['dispatch_planning']['bills'] = array();
  }
  $options = $form_state['dispatch_planning']['bills'];

  $headers = array(
    'bill_number' => 'Bill Number',
    'party_name' => 'Party name',
    'bill_value' => 'Value',
    'van_loading_date' => 'Existing Van/Loading Date',
    //'sequence' => 'Delivery Sequence',
    //'trip_id' => 'Trip ID',
    'sales_beat' => 'Sales Beat',
    'delivery_pending' => 'Delivery Pending',
  );


  foreach ($headers as $key => $value) {
    $sort_options[$key . ':asc'] = $value . ' - Asc';
    $sort_options[$key . ':desc'] = $value . ' - Desc';
  }


  $form['sort'] = array(
    '#type' => 'select',
    '#options' => $sort_options,
    '#ajax' => array(
      'wrapper' => 'itemlist-div',
      'callback' => 'custom_ajax_callback',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    '#attributes' => array(
      'class' => array('sort-select', 'element-invisible'),
    ),
  );

  if (isset($form_state['triggering_element']) && ($form_state['triggering_element']['#name'] == 'sort')) {
    $active = explode(':', $form_state['values']['sort']);
    $form_state['dispatch_planning']['sort_field'] = $active[0];
    $form_state['dispatch_planning']['sort_order'] = $active[1];
  }

  if (isset($form_state['dispatch_planning']['sort_field'])) {
    $active_sort = $form_state['dispatch_planning']['sort_field'];
  }
  if (isset($form_state['dispatch_planning']['sort_order'])) {
    $active_order = $form_state['dispatch_planning']['sort_order'];
  }
  if ($active_order == 'asc') {
    $next_order = 'desc';
  }
  elseif ($active_order == 'desc') {
    $next_order = 'asc';
  }

  foreach ($headers as $k => $header) {
    $header_array[$k] = array(
      'data' => array(
        '#type' => 'link',
        '#title' => $header . (($active_sort == $k && $next_order) ? theme('tablesort_indicator', array('style' => $next_order)) : ''),
        '#href' => '',
        '#attributes' => array(
          'class' => ($active_sort == $k) ? array('active', 'header-link') : array('header-link'),
          'id' => $k,
          'data-sort' => ($active_sort == $k && $next_order) ? $next_order : '',
        ),
        '#options' => array(
          'html' => true,
        )
      ),
    );
  }

  if ($options) {
    if (isset($active_sort)) {
      $sort = ($active_sort == 'van_loading_date') ? 'van_loading_date_timestamp' : $active_sort;
      $options = _get_header_sort($options, $sort, $active_order);
    }

    $form['list_form_items'] = array(
      '#type' => 'tableselect',
      '#header' => $header_array,
      '#options' => $options,
      '#prefix' => '<div id="table-div">',
      '#suffix' => '</div>',
      '#empty' => t('No forms available.'),
      '#multiple' => TRUE,
      '#js_select' => FALSE, //Dont give Select All checkbox
    );

    $form['remove_selected'] = array(
      '#type' => 'submit',
      '#value' => t('Remove Checked items'),
      '#ajax' => array(
        'wrapper' => 'itemlist-div',
        'callback' => 'custom_ajax_callback',
        'method' => 'replace',
        'effect' => 'fade',
      ),
      '#submit' => array('remove_selected_ajax_submit'),
      '#limit_validation_errors' => array(array('list_form_items')),
    );

    $form['delivery_pending'] = array('#type' => 'value');
    $form['header'] = array('#type' => 'value');

//    $bill_count = count($options);
//    $bill_value_sum = array_sum(array_map(
//            function($item) {
//          return $item['bill_value'];
//        }, $options)
//    );
//    $form_state['dispatch_planning']['bill_value_sum'] = $bill_value_sum;
//
//    $form['bill_summary'] = array(
//      '#theme' => 'item_list',
//      '#items' => array(
//        'Bill Count : ' . $bill_count,
//        'Bill Value : ' . $bill_value_sum,
//      ),
//    );
    $form['actions'] = array('#type' => 'actions');

    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => 'Save',
      '#limit_validation_errors' => array(array('list_form_items'), array('delivery_pending')),
      '#submit' => array('delivery_pending_form_save_submit'),
      '#validate' => array('delivery_pending_form_save_validate')
    );
  }
  return $form;
}

function delivery_pending_bill_number_ajax_submit($form, &$form_state) {
  $bill_number = $form_state['values']['bill_number'];

  unset($form_state['input']['bill_number']);

  $bill_info = get_bill_info($bill_number, $van = NULL, $loading_date = NULL, $beat_name = NULL, $trip_id = NULL, $delivery_pending = TRUE);

  foreach ($bill_info as $key => $value) {
    
    unset($bill_info[$key]['field_delivery_pending_value']);
    $bill_info[$key]['delivery_pending']['data'] = array(
      '#type' => 'select',
      '#title' => 'Delivery pending',
      '#title_display' => 'invisible',
      '#value' => $value['field_delivery_pending_value'],
      '#name' => "delivery_pending[$key]",
      '#options' => array('' => 'Select', 1 => 'Yes')
    );
  }

  if (!empty($bill_info)) {
    $form_state['dispatch_planning']['bills'] += $bill_info;
  }

  $form_state['rebuild'] = TRUE;
}

function delivery_pending_form_save_validate($form, &$form_state) {
  $rows = $form_state['values']['list_form_items'];

  $today = strtotime(date('Y-m-d 00:00:00'));
  if (!empty(array_filter($rows))) {
    form_set_error('list_form_items', 'Please uncheck all rows');
  }
}

function delivery_pending_form_save_submit($form, &$form_state) {
  $rows = $form_state['values']['delivery_pending'];
  $bill_nids = $rows;

  foreach ($bill_nids as $nid => $value) {
    $node = node_load($nid);
    if ($node) {
      if ($value) {
        $node->field_delivery_pending[LANGUAGE_NONE][0]['value'] = 1;
        //Maintain previous trip
        unset($node->field_bill_sequence[LANGUAGE_NONE]);
        unset($node->field_drs_sequence[LANGUAGE_NONE]);
        $node->field_previous_trip[LANGUAGE_NONE][0]['value'] = $node->field_trip_id_reference[LANGUAGE_NONE][0]['nid'];
        unset($node->field_trip_id_reference[LANGUAGE_NONE]);
        unset($node->field_dispatch_n_value[LANGUAGE_NONE]);
      }
      else {
        $node->field_delivery_pending = array();
      }
      node_save($node);
    }
  }

  drupal_set_message('Successfully updated.');
}
