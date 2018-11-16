<?php
// to pass values from one autocomplete field to another
// url will be admin/dispatch_planning/delivery_pending/trip_id
// or admin/dispatch-planning/delivery-pending/trip_id
function trip_id_ajaxcallback($form, &$form_state) {
    $path_segment = explode('/',$form_state['request_uri']);
    $path = $path_segment[0].'/'.$path_segment[1].'/'.$path_segment[2];
    $trip_id = $form_state['values']['trip_id'];
    ajax_redirect($path.'/'.$trip_id);
    return $form;
}

function _get_dp_reasons($reason_type) {
   $query = db_select('node', 'n');
   $query->join('field_data_field_reason_type', 'rt', 'rt.entity_id = n.nid');
   $query->join('field_data_field_dp_reason', 'dr', 'dr.entity_id = n.nid');
   $query->condition('rt.field_reason_type_value', $reason_type);
   $query->condition('n.type', 'dispatch_planning_reason_master');
   $query->fields('n', array('nid'));
   $query->fields('dr', array('field_dp_reason_value'));
   $query->addTag('node_distributor_access');
   $res = $query->execute()->fetchAllKeyed(0,1);
   $res1 = array('' => '- select -');
   $response = $res1 + $res;
   return $response;

}
function delivery_pending_form($form, &$form_state) {
  $form['#prefix'] = '<div id="itemlist-div">';
  $form['#suffix'] = '</div>';
  $form['#attached']['js'][] = drupal_get_path('module', 'dispatch_planning') . '/js/dispatch_planning_form.js';
  $form['#attached']['js'][] = drupal_get_path('module', 'dispatch_planning') . '/js/delivery_pending.js';
  $form['#attached']['css'][] = drupal_get_path('module', 'dispatch_planning') . '/css/dispatch_planning.css';
  $form['#attached']['css'][] = drupal_get_path('module', 'dispatch_planning') . '/css/delivery_pending.css';
  global $base_url;
  if(empty(strpos(request_uri(),'system/ajax'))) {
   $request_uri = ltrim(request_uri(),'/');
   $form_state['request_uri'] = $request_uri;
  }
  drupal_add_js(array('baseUrl' => $base_url), 'setting');
  $form['trip_id'] = array(
    '#prefix' => '<div class ="dp_filter"><div class="dp_trip_id">',
    '#type' => 'textfield',
    '#title' => t('Trip ID'),
    '#suffix' => '</div>',
    '#required' => TRUE,
    '#default_value'=>arg(3),
    '#autocomplete_path' => 'dp_trip/autocomplete',
    '#ajax' => array(
      'callback' => 'trip_id_ajaxcallback',
      'wrapper' => 'itemlist-div',
    ),  
  );
  $trip_id = arg(3);
  if (!empty($trip_id)) {
      $form_state['input']['trip_id'] = $trip_id;
  }
  $trip_id = $form_state['input']['trip_id'];
  $form['bill_number'] = array(
    '#prefix' => '<div class="textfield-wrapper dp_bill_number">',
    '#type' => 'textfield',
    '#title' => t('Bill number*'),
    '#suffix' => '</div>',    
    '#size'=>20,
    '#attributes'=>array('class'=>array('dp_bill_number')),  
    '#autocomplete_path' => 'bill/pending/autocomplete/'.$trip_id,
    
  );
  $form['bill_number_submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add Bill'),
    '#prefix' => '<div class="add_bill">',  
    '#suffix' => '</div></div>',  
    '#ajax' => array(
      'wrapper' => 'itemlist-div',
      'callback' => 'custom_ajax_callback',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    '#submit' => array('delivery_pending_bill_number_ajax_submit'),
    '#validate' => array('bill_number_ajax_validate'),
    '#limit_validation_errors' => array(array('bill_number'),array('trip_id')),
   
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
    'delivery_pending' => 'Delivery Pending Reason',
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
    
    $dp_reason_list = _get_dp_reasons('delivery_pending');
    $form['dp_pending_reason_all'] = array(
            '#type' => 'select',
            '#title' => t('Select Delivery Pending Reason for all Bills'),
            '#attributes' => array('class' => array('dp_pending_reason_all'),'id'=>'dp_pending_reason_all'),
            '#prefix'=>'<div class="delivery_pending_all">',
            '#suffix'=>'</div>', 
            '#options' => $dp_reason_list,
            '#default_value' => $form['input']['dp_pending_reason_all']
        );
       $form['list_form_items'] = array(
      '#type' => 'tableselect',
      '#header' => $headers,
      '#options' => $options,
      '#prefix' => '<div id="table-div" class="dp_list_table">',
      '#suffix' => '</div>',
      '#empty' => t('No forms available.'),
      '#multiple' => TRUE,
      '#js_select' => FALSE, //Dont give Select All checkbox
    );

    $form['remove_selected'] = array(
      '#type' => 'submit',
      '#value' => t('Remove Checked items'),
      '#prefix'=>'<div class="remove_checked_items_outer">',  
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
    $field_is_party_pack = field_info_field('field_is_party_pack');
    $no_pp_values= list_allowed_values($field_is_party_pack);
    $form['is_party_pack'] = array(
            '#type' => 'radios',
            '#required'=>true,
            '#title' => t('Party-Packed Van Or Non-PP Van'),
            '#options' => $no_pp_values,
            '#suffix'=>'</div>'    
        );

    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => 'Save',
      //'#limit_validation_errors' => array(array('list_form_items'), array('delivery_pending')),
      '#submit' => array('delivery_pending_form_save_submit'),
      '#validate' => array('delivery_pending_form_save_validate')
    );

  }
  return $form;
}

function delivery_pending_bill_number_ajax_submit($form, &$form_state) {
  $bill_number = $form_state['values']['bill_number'];
  $trip_id = $form_state['values']['trip_id'];
  unset($form_state['input']['bill_number']);
  $rows = $form_state['input']['list_form_items'];
    if (!empty($rows)) {
        foreach ($rows as $key => $value) {
            foreach ($form_state['dispatch_planning']['bills'][$key]['delivery_pending']['data'][$key] as $k => &$v) {
                if ($v['#name'] == "delivery_pending[$key]") {
                    $v['#value'] = $form_state['input']['delivery_pending'][$key];
                }
                
            }
        }
        //$form_state['complete form']['dp_pending_reason_all']['#value'] = $form_state['input']['dp_pending_reason_all'];
        //$form_state['dp_pending_reason_all']['#value']= $form_state['input']['dp_pending_reason_all'];
    }
  $res_status = array('undeliver','');
  $bill_info = get_bill_info($bill_number, $van = NULL, $loading_date = NULL, $beat_name = NULL, $trip_id , $delivery_pending = TRUE,$res_status);
  $dp_reason_list = _get_dp_reasons('delivery_pending');
  foreach ($bill_info as $key => $value) {
    
    unset($bill_info[$key]['field_delivery_pending_value']);
    /*$bill_info[$key]['delivery_pending']['data'] = array(
      '#type' => 'select',
      '#title' => 'Delivery pending',
      '#title_display' => 'invisible',
      '#value' => $value['field_delivery_pending_value'],
      '#name' => "delivery_pending[$key]",
      '#options' => array('' => 'Select', 1 => 'Yes')
    );*/
    $bill_info[$key]['delivery_pending']['data'][$key]['dp_pending'] = array(
                    '#type' => 'select',
                    '#title' => t('Delivery Pending Reason'),
                    '#title_display' => 'invisible',
                    '#attributes' => array('class' => array('dp_reason_bills')),
                    '#name' => "delivery_pending[$key]",
                    '#required' => TRUE,
                    '#default_value' =>$form_state['input']['dp_pending_reason_all'],
                    '#options' => $dp_reason_list,
                    '#value' =>$form_state['input']['dp_pending_reason_all'],
                  );
    
    
  }
   if (!empty($bill_info)) {
    $form_state['dispatch_planning']['bills'] += $bill_info;
  }

  $form_state['rebuild'] = TRUE;
}

function delivery_pending_form_save_validate(&$form, &$form_state) { 
  $rows = $form_state['values']['list_form_items'];      
  $today = strtotime(date('Y-m-d 00:00:00'));
  if (!empty(array_filter($rows))) {
    form_set_error('list_form_items', 'Please uncheck all rows');
  }
  //print_r($form_state['dispatch_planning']['bills'][$key]['delivery_pending']['#options']);
  if (!empty($rows)) {
        foreach ($rows as $key => $value) {
            foreach ($form_state['complete form']['list_form_items']['#options'][$key]['delivery_pending']['data'][$key] as $k => &$v1) {
                if ($v1['#name'] == "delivery_pending[$key]") {
                    $v1['#value'] = $form_state['values']['delivery_pending'][$key];
                }
            }
        }

        $bills_nids = array_keys($rows);
      $sales_register_nodes = node_load_multiple($bills_nids);
      foreach($rows as $key=>$val) {
          if(empty($form_state['values']['delivery_pending'][$key])) {
              $mess.=t('Please select delivery pending reason for Bill Number:'.$sales_register_nodes[$key]->title).'<br>';
          }
      }
      if (!empty($mess)) {
          form_set_error('list_form_items', $mess);
      }
  }
}
// update trip id with party pack van type
function update_trip_data($trip_id,$is_party_pack,$distributor_id){
    if (!empty($trip_id)  && !empty($distributor_id)) {
    $node_obj = node_load_multiple(NULL, array('title' => $trip_id, 'type' => 'dispatch_data', 'uid' => $distributor_id));
    $key = key($node_obj);
    $node = $node_obj[$key];
    if (!empty($node)) {
      // field_is_party_pack  0- Non-PP Van
      // 1- Party-Packed Van
      $node->field_is_party_pack[LANGUAGE_NONE][0]['value'] = $is_party_pack;
      node_save($node);
      return $node;
    }
  }
}
function update_farery_delivery_pending_flag($bill_number,$trip_id,$distributor_id,$dp_reason){
    if (!empty($bill_number) && !empty($trip_id) && !empty($distributor_id) && !empty($dp_reason)) {
           $status_updated = db_update('dispatch_planning_api_data') // Table name no longer needs {}
                ->fields(array(
                    'delivery_pending_flag' => 1,
                    'dp_declaration_time' => REQUEST_TIME,
                    'delivery_pending_reason'=>$dp_reason,
                ))
                ->condition('bill_number', $bill_number, '=')
                ->condition('runsheet_number', $trip_id, '=')
                ->condition('distributor_id', $distributor_id, '=')   
                ->execute();
    }
}
// Update farey bill status as undeliver if res_staus = blank
function update_farery_bill_status($bill_number,$trip_id,$distributor_id,$dp_reason){
    if (!empty($bill_number) && !empty($trip_id) && !empty($distributor_id) && !empty($dp_reason)) {
           $status_updated = db_update('dispatch_planning_api_data') // Table name no longer needs {}
                ->fields(array(
                    'res_status' => 'undeliver',
                    'delivery_pending_flag' => 1,
                    'updated_flag' => 1, //manual
                    'res_reason_for_failure' =>'Outlet not attempted',
                    'dp_declaration_time' => REQUEST_TIME,
                    'updated_time' => REQUEST_TIME,
                    'delivery_pending_reason'=>$dp_reason,
                ))
                ->condition('bill_number', $bill_number, '=')
                ->condition('runsheet_number', $trip_id, '=')
                ->condition('distributor_id', $distributor_id, '=')   
                ->execute();
    }
}
function fetch_dispatch_planning_api_id($trip_id,$bill_number,$distributor_id) {
    $dp_rows = array();
    if (!empty($trip_id) && !empty($bill_number) && !empty($distributor_id)) {
        $query = db_select('dispatch_planning_api_data', 'ad');
        $query->fields('ad', array('id','res_status'));
        $query->condition('bill_number', $bill_number, '=');
        $query->condition('runsheet_number', $trip_id, '=');
        $query->condition('distributor_id', $distributor_id, '=');
        $dp_rows = $query->execute()->fetchAllKeyed(0,1);
    }
    return $dp_rows;
}
// If res_status blank then res_status become delivery_pending &  return qty = dispatched qty 
function update_dp_return_qty($bill_row_id) {
    if (is_numeric($bill_row_id)) {
        db_update('dispatch_planning_item_details') // Table name no longer needs {}
           ->expression('return_qty', "dispatch_qty")
           ->fields(array('res_sku_actual_quantity1'=>0,
               'farey_delivered_qty'=>0,
               'line_level_status'=>"line_undeliver"
               ))
           ->condition('trip_bill_reference', $bill_row_id, '=')
           ->execute();
    }
}

function delivery_pending_form_save_submit($form, &$form_state) {
  global $user;
  global $base_url;
  $path_segment = explode('/',request_uri());
  $path = $path_segment[1].'/'.$path_segment[2].'/'.$path_segment[3];
  $dis_id = change_feeds_import_get_distributor_id($user->uid);  
  $trip_id = $form_state['values']['trip_id'];
  $is_party_pack = $form_state['values']['is_party_pack'];
  $rows = $form_state['values']['delivery_pending'];
  $bill_nids = $rows;
  $dp_status = t('delivery_pending');
  foreach ($bill_nids as $nid => $value) {
    $node = node_load($nid);
    if ($node) {
      if ($value) {
        $node->field_delivery_pending[LANGUAGE_NONE][0]['value'] = 1;
        //Maintain previous trip        
        $trip_nid = $node->field_trip_id_reference[LANGUAGE_NONE][0]['nid'];
        $query = db_select('node', 'n');
        $query->condition('n.nid', $trip_nid);
        $query->condition('n.type', 'dispatch_data');
        $query->fields('n', array('title'));
        $trip_title = $query->execute()->fetchField();
        $dp_reason = $value;
        $node->field_previous_trip[LANGUAGE_NONE][0]['value'] = $trip_title;
        unset($node->field_trip_id_reference[LANGUAGE_NONE]);
        unset($node->field_dispatch_n_value[LANGUAGE_NONE]);
        unset($node->field_bill_sequence[LANGUAGE_NONE]);
        unset($node->field_drs_sequence[LANGUAGE_NONE]);
        unset($node->field_deliver_n_value[LANGUAGE_NONE]);
      }
      else {
        $node->field_delivery_pending = array();
      }
       node_save($node);
       // update dispatch data with party van or NON PP van field
       $trip_node = update_trip_data($trip_id,$is_party_pack,$dis_id);
       // fetch farey dp_id & res_status, 
       $bill_rows = fetch_dispatch_planning_api_id($trip_id, $node->title, $dis_id);
       $bill_row_id = key($bill_rows);
       if (!empty($trip_node)) {
            // if res_status == undeliver then update delivery_pending flag = 1
            if (!empty(($bill_rows[$bill_row_id]))) {
                    update_farery_delivery_pending_flag($node->title,$trip_id,$dis_id,$dp_reason);
            }
           //if res_status == blank then return_qty = dispatch_qty no entry for this bill in farey so update return qty
           else {
                 update_farery_bill_status($node->title,$trip_id,$dis_id,$dp_reason);
                 $bill_row_id = update_dp_return_qty($bill_row_id) ;
           }
       }
    }
  }

  drupal_set_message('Successfully updated.');
  //$path = '/admin/dispatch-planning/delivery-pending';
  drupal_goto($path);
}
