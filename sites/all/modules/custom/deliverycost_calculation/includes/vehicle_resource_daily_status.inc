<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function vehicle_resource_daily_status_form($form, &$form_state) {
    $form = array();
    $form['#attached']['js'][] = drupal_get_path('module', 'deliverycost_calculation') . '/js/resource_vehicle_daily_status_update.js';
    $form['#attached']['css'][] = drupal_get_path('module', 'deliverycost_calculation') . '/css/vehicle_resource_daily_status.css';
    $form['#prefix'] = '<div id="itemlist-div" class="custom_form_wrapper">';
    $form['#suffix'] = '</div>';
    $form['trip_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Trip ID'),
      '#size' => 20,
      '#autocomplete_path' => 'sku-wise-trip/autocomplete',
      '#maxlength' => 50,
      '#attributes' => array('class' => array('trip-id')),
      '#prefix' => '<div class="form_row form_row2"><div class="col_3" style="width:200px">',
      '#suffix' => '</div>'
    );

    /* $form['vehicle_number_1'] = array(
      '#type' => 'textfield',
      '#title' => t('Vehicle Number'),
      '#attributes' => array('class' => array('vehicle_number')),
      '#autocomplete_path' => 'vehicle_maintenance/autocomplete',
      '#size' => 30,
      '#prefix' => '<div class="col_3" style="width:200px">',
      '#suffix' => '</div></div>'
      ); */

    $form['vehicle_number'] = array(
      '#type' => 'textfield',
      '#title' => t('Vehicle Number'),
      '#size' => 20,
      '#autocomplete_path' => 'vehicle_maintenance/autocomplete',
      '#maxlength' => 50,
      '#attributes' => array('class' => array('vehicle_number')),
      '#prefix' => '<div class="col_3" style="width:200px">',
      '#suffix' => '</div></div>'
    );


    $form['resource_type'] = array(
      '#type' => 'select',
      '#title' => 'Resource Type',
      '#prefix' => '<div class="form_row form_row2"><div id="get_tripid_dropdown" class="col_3" style="width:200px">',
      '#options' => array('' => t('Select'), 'Delivery Boy' => 'Delivery Boy', 'Cashier' => 'Cashier'),
      '#suffix' => '</div>',
      '#ajax' => array(
        'callback' => 'resource_type_callback',
        'event' => 'change',
        'wrapper' => 'itemlist-div',
        'method' => 'replace',
      ),
      '#attributes' => array('class' => array('trip-dropdown', 'col-lg-6')),
    );
    $form['resource_name'] = array(
      '#type' => 'textfield',
      '#title' => 'Resource Name',
      '#attributes' => array('class' => array('cashier_names')),
      '#prefix' => '<div class="col_2 col_2_2 cashier_name" id="resource_name" style="width:200px">',
      '#autocomplete_path' => 'get_active_resource_type/autocomplete/' . $form_state['input']['resource_type'],
      '#suffix' => '</div><div id ="get_tripid_dropdown"></div>'
    );
    $form['declaration_date'] = array(
      '#type' => 'date_popup',
      '#title' => t('Declaration Date'),
      '#required' => TRUE,
      '#date_format' => 'Y-m-d',
      '#attributes' => array('class' => array('declaration_date')),
      '#date_label_position' => '',
      '#date_year_range' => '-20:+2',
      '#default_value' => $trip_details['declaration_date'] ? $trip_details['declaration_date'] : date('Y-m-d'), //date('Y-m-d'),
      '#size' => 30,
      '#prefix' => '<div class="col_3 declaration_date">',
      '#suffix' => '</div>'
    );
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => 'Show',
      '#name' => 'show',
      '#ajax' => array(
        'callback' => 'resource_status_callback',
        'wrapper' => 'itemlist-div',
        'method' => 'replace',
        'effect' => 'fade',
      ),
      '#attributes' => array('class' => array('vrs-trip-submit')),
      '#submit' => array('display_resource_vehicle_daily_status'),
      '#validate' => array('validate_resource_vehicle_daily_status'),
      '#prefix' => '<div style="clear:both;float:left;align:center;margin-left:23px;">',
      '#suffix' => '</div></div>'
    );


    if ($form_state['apply_clicked'] == 1) {
        global $user;
        $distributor_id = change_feeds_import_get_distributor_id($user->uid);
        $trip_id = $form_state['values']['trip_id'];
        $vehicle_number = $form_state['values']['vehicle_number'];
        $resource_type = $form_state['values']['resource_type'];
        $resource_name = $form_state['values']['resource_name'];
        $declaration_date = $form_state['values']['declaration_date'];
        //unset($form_state['input']);
        $form_state['input']['vehicle_number'] = $vehicle_number;
        $form_state['input']['trip_id'] = $trip_id;
        $vehicle_reason = get_resons('vehicle');
        $trip_id = $form_state['values']['trip_id'];
        $resource_type = $form_state['values']['resource_type'];
        if (!empty($trip_id)) {
            $trip_details = get_trip_details($trip_id, '', '', '',  $declaration_date);
        } else if (!empty($form_state['values']['vehicle_number'])) {
            $trip_details = get_trip_details(null, $form_state['values']['vehicle_number'], NULL, NULL, $declaration_date);
        } else if (!empty($form_state['values']['resource_name'])) {
            $trip_details = get_trip_details('', '', $form_state['values']['resource_name'], $resource_type, $declaration_date);
        }
        $resource_reason = get_resons('resource');
        $form['daily_status_update'] = array(
          '#type' => 'fieldset',
          '#title' => t('Vehicle Resource Daily Status Update'),
          '#prefix' => '<div class="width_daily_status_update">',
          '#suffix' => '</div>',
        );
        $error_flag = 1;
        if (!empty($form_state['values']['trip_id']) || !empty($form_state['values']['vehicle_number']) || !empty($form_state['values']['resource_name'])) {
            if (!empty($form_state['values']['trip_id'])) {
                $node_obj = node_load_multiple(NULL, array('title' => $trip_id, 'type' => 'dispatch_data', 'uid' => $distributor_id));
                if (!empty($node_obj)) {
                    $key = key($node_obj);
                    $dispatch_date = date('d-M-Y', strtotime($node_obj[$key]->field_dispatch_date[LANGUAGE_NONE][0]['value']));
                    $vehicle_no = $node_obj[$key]->field_vehicle_no[LANGUAGE_NONE][0]['value'];
                    $field_cashier_name = $node_obj[$key]->field_cashier_name[LANGUAGE_NONE][0]['value'];
                    $delivery_boys = $node_obj[$key]->field_delivery_boy[LANGUAGE_NONE];
                }
                if (!empty($node_obj)) {
                    $error_flag = 0;
                    $form['daily_status_update']['html'] = array('#type' => 'markup',
                      '#markup' => '<h5>Dispatch Date: ' . $dispatch_date . ' </h5>');
                    unset($form_state['input']['trip_vehicle_number']);
                    $form['daily_status_update']['trip_vehicle_number'] = array(
                      '#type' => 'textfield',
                      '#title' => t('Vehicle Number'),
                      '#size' => 30,
                      '#prefix' => '<div class="form_row form_row2"><div class="col_3" style="width:200px">',
                      '#suffix' => '</div>',
                      '#attributes' => array('readonly' => 'readonly'),
                      '#default_value' => $vehicle_no
                    );
                    unset($form_state['input']['trip_vehicle_reason']);
                    $form['daily_status_update']['trip_vehicle_reason'] = array(
                      '#type' => 'select',
                      '#title' => t('Reason'),
                      '#attributes' => array('class' => array('reason_height')),
                      '#default_value' => $trip_details['vehicle_number'],
                      '#options' => array('0' => t('Select')) + $vehicle_reason,
                      '#prefix' => '<div class="col_3" style="width:200px;">',
                      '#suffix' => '</div></div>'
                    );
                    unset($form_state['input']['trip_cashier']);
                    $form['daily_status_update']['trip_cashier'] = array(
                      '#type' => 'textfield',
                      '#title' => t('Cashier'),
                      '#prefix' => '<div class="form_row form_row2"><div class="col_3" style="width:200px">',
                      '#suffix' => '</div>',
                      '#attributes' => array('readonly' => 'readonly'),
                      '#default_value' => $field_cashier_name
                    );
                    unset($form_state['input']['trip_cashier_reason']);
                    $form['daily_status_update']['trip_cashier_reason'] = array(
                      '#type' => 'select',
                      '#title' => t('Reason'),
                      '#options' => array('0' => t('Select')) + $resource_reason,
                      '#default_value' => $trip_details[$field_cashier_name],
                      '#prefix' => '<div class="col_3" style="width:200px;">',
                      '#suffix' => '</div></div>'
                    );
                    foreach ($delivery_boys as $nid => $value) {
                        $resource_obj = node_load($value['nid']);
                        $resource_name = $resource_obj->field_resource_name[LANGUAGE_NONE][0]['value'];
                        $resource_nid = $resource_obj->nid;
                        unset($form_state['input']['trip_delivery_boy_' . $resource_nid]);
                        $form['daily_status_update']['trip_delivery_boy_' . $resource_nid] = array(
                          '#type' => 'textfield',
                          '#title' => t('Delivery Boy'),
                          '#prefix' => '<div class="form_row form_row2"><div class="col_3" style="width:200px">',
                          '#suffix' => '</div>',
                          '#attributes' => array('readonly' => 'readonly'),
                          '#default_value' => $resource_name
                        );
                        unset($form_state['input']['trip_delivery_boy_reason_' . $resource_nid]);
                        $form['daily_status_update']['trip_delivery_boy_reason_' . $resource_nid] = array(
                          '#type' => 'select',
                          '#title' => t('Reason'),
                          '#options' => array('0' => t('Select')) + $resource_reason,
                          '#default_value' => $trip_details[$resource_nid],
                          '#prefix' => '<div class="col_3" style="width:200px;">',
                          '#suffix' => '</div></div>'
                        );
                    }
                }
            } else if (!empty($form_state['values']['vehicle_number'])) {
                $vh_no = $form_state['values']['vehicle_number'];
                $node_obj = node_load_multiple(NULL, array('title' => $vh_no, 'type' => 'vehicle_master', 'uid' => $distributor_id));
                unset($form_state['input']['trip_vehicle_number']);
                if (!empty($node_obj)) {
                    $error_flag = 0;
                    $form['daily_status_update']['trip_vehicle_number'] = array(
                      '#type' => 'textfield',
                      '#title' => t('Vehicle Number'),
                      '#prefix' => '<div class="form_row form_row2"><div class="col_3" style="width:200px">',
                      '#suffix' => '</div>',
                      '#attributes' => array('readonly' => 'readonly'),
                      '#default_value' => $form_state['values']['vehicle_number']
                    );
                    unset($form_state['input']['trip_vehicle_reason']);
                    $form['daily_status_update']['trip_vehicle_reason'] = array(
                      '#type' => 'select',
                      '#title' => t('Reason'),
                      '#options' => array('0' => t('Select')) + $vehicle_reason,
                      '#default_value' => $trip_details['vehicle_number'],
                      '#prefix' => '<div class="col_3" style="width:200px;">',
                      '#suffix' => '</div></div>'
                    );
                }
            } else if (!empty($form_state['values']['resource_name']) && !empty($form_state['values']['resource_type'])) {
                $resource_nid = check_resource_exists($form_state['values']['resource_type'], $form_state['values']['resource_name']);
                if (!empty($resource_nid)) {
                    $error_flag = 0;
                    $default_value = $trip_details[$form_state['values']['resource_name']];
                    unset($form_state['input']['trip_resource_name']);
                    $form['daily_status_update']['trip_resource_name'] = array(
                      '#type' => 'textfield',
                      '#title' => t('Resource Name'),
                      '#prefix' => '<div class="form_row form_row2"><div class="col_3" style="width:200px">',
                      '#suffix' => '</div>',
                      '#attributes' => array('readonly' => 'readonly'),
                      '#default_value' => $form_state['values']['resource_name']
                    );
                    unset($form_state['input']['trip_reason']);
                    $form['daily_status_update']['trip_reason'] = array(
                      '#type' => 'select',
                      '#title' => t('Reason'),
                      '#options' => array('0' => t('Select')) + $resource_reason,
                      '#default_value' => $default_value,
                      '#prefix' => '<div class="col_3" style="width:200px;">',
                      '#suffix' => '</div></div>'
                    );
                }
            }
            if ($error_flag == 0) {
                $form['daily_status_update']['save'] = array(
                  '#type' => 'submit',
                  '#value' => 'Submit',
                  '#name' => 'save',
                  '#attributes' => array('class' => array('vrs-trip-submit')),
                  '#validate'=>array('validate_input_vehicle_resource_daily_status'),
                  '#submit' => array('save_vehicle_resource_daily_status'),
                  '#prefix' => '<div style="clear:both;float:left;">',
                  '#suffix' => '</div></div>'
                );
            }
        }
    }


    return $form;
}

function validate_resource_vehicle_daily_status($form, $form_state) {
    $vehicle_number = $form_state['values']['vehicle_number'];
    $resource_name = $form_state['values']['resource_name'];
    $resource_type = $form_state['values']['resource_type'];
    $declaration_date = $form_state['values']['declaration_date'];
    $declaration_date_in_strtotime = strtotime($declaration_date);
    if (empty($form_state['values']['resource_type']) && !empty($form_state['values']['resource_name'])) {
        form_set_error('resource_type', t('Please select resource type.'));
        $form_state['redirect'] = 'node';
    }

    if (empty($form_state['values']['trip_id']) && empty($form_state['values']['vehicle_number']) && empty($form_state['values']['resource_name'])) {
        form_set_error('trip_id', t('Please enter either Trip ID or Vehicle Number or Resource Name.'));
    }
    if (!empty($resource_name) && !empty($resource_type)) {
      $check_active_resource = get_all_active_resources_on_date_range($declaration_date_in_strtotime, $declaration_date_in_strtotime,$resource_name,$resource_types= array($resource_type=>$resource_type));
      if (empty($check_active_resource)) {
        form_set_error('resource_name', t("Resource:$resource_name contract not valid on ". date('d-M-Y',$declaration_date_in_strtotime)));
      }
    }
    if (!empty($vehicle_number)) {
      $check_active_vehicle = get_all_active_vehicles_on_date_range($declaration_date_in_strtotime, $declaration_date_in_strtotime,$vehicle_number);
      if (empty($check_active_vehicle)) {
        form_set_error('vehicle_number', t("Vehicle Number:$vehicle_number contract not valid on ". date('d-M-Y',$declaration_date_in_strtotime)));
      }
    }
   
    }

function resource_type_callback(&$form, &$form_state) {
    return $form;
//  $commands[] = ajax_command_replace('#itemlist-div', render($form));
//  $commands[] = ajax_command_replace('.alert-danger', '');
//  $commands[] = ajax_command_replace('.alert-success', '');
//  $commands[] = ajax_command_prepend('#itemlist-div', theme('status_messages'));
//  return array('#type' => 'ajax', '#commands' => $commands);
}

function resource_status_callback(&$form, &$form_state) {


    //return $form;  
    $commands[] = ajax_command_replace('#itemlist-div', render($form));
    $commands[] = ajax_command_replace('.alert-danger', '');
    $commands[] = ajax_command_replace('.alert-success', '');
    $commands[] = ajax_command_prepend('#itemlist-div', theme('status_messages'));
    return array('#type' => 'ajax', '#commands' => $commands);
}

function display_resource_vehicle_daily_status(&$form, &$form_state) {
    $form_state['apply_clicked'] = 1;
    $form_state['rebuild'] = true;
    return $form;
}

function get_resons($type) {
    $name = 'vehicle_resource_daily_status';
    $voc_object = taxonomy_vocabulary_machine_name_load($name);
    if (!empty($voc_object)) {
        $vid = $voc_object->vid;
    }

    $query = db_select('taxonomy_term_data', 'term_data');
    $query->join('field_data_field_vehicle_resource_type', 'type', 'type.entity_id = term_data.tid');
    $query->fields('type', array('field_vehicle_resource_type_value'));
    $query->fields('term_data', array('tid', 'name'));
    $result = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
    $term_datas = array();
    if (!empty($result)) {
        foreach ($result as $key => $value) {
            $term_datas[$value['field_vehicle_resource_type_value']][$value['tid']] = $value['name'];
        }
    }
    if ($type == 'vehicle') {
        return $term_datas['vehicle'];
    }
    if ($type == 'resource') {
        return $term_datas['resource'];
    }
}

function validate_input_vehicle_resource_daily_status($form,&$form_state) {
    global $user;
    $mess ='';
    $distributor_id = change_feeds_import_get_distributor_id($user->uid);
    $delivery_boy_nids = get_delivery_boy_nid();
    $trip_vehicle_number = $form_state['values']['trip_vehicle_number'];
    $trip_vehicle_reason = $form_state['values']['trip_vehicle_reason'];
    $trip_cashier = $form_state['values']['trip_cashier'];
    $trip_cashier_reason = $form_state['values']['trip_cashier_reason'];
    $trip_id = $form_state['values']['trip_id'];
    $vehicle_number = $form_state['values']['vehicle_number'];
    $resource_type = $form_state['values']['resource_type'];
    $resource_name = $form_state['values']['resource_name'];
    $declaration_date = strtotime($form_state['values']['declaration_date']);
    $trip_resource_name = $form_state['values']['trip_resource_name'];
    $trip_reason = $form_state['values']['trip_reason'];
    if (!empty($trip_id)) {
        $resource_type = 'Delivery Boy';
        foreach ($delivery_boy_nids as $key => $value) {
            $delivery_boy_name = $form_state['values']['trip_delivery_boy_' . $value];
            $delivery_boy_reason = $form_state['values']['trip_delivery_boy_reason_' . $value];
            if (empty($delivery_boy_reason) && !empty($delivery_boy_name)) {
                $mess.=t('Please select reason for Delivery Boy: '.$delivery_boy_name).'<br>';
            }
        }
        $resource_type = 'Cashier';
        if (empty($trip_cashier_reason) && !empty($trip_cashier)) {
            $mess.=t('Please select reason for Cashier: '.$trip_cashier).'<br>';
        }
        if (!empty($trip_vehicle_number) && empty($trip_vehicle_reason)) {
            $mess.=t('Please select reason for Vehicle Number: '.$trip_vehicle_number).'<br>';
        }
        $existing_trip_title =   check_trip_id_already_exist_in_declaration_date($form_state['values']['declaration_date'],$trip_id,$vehicle_number=null,$resource_type=null,$resource_name=null,$delivery_boy_nid=null);
        if (!empty($existing_trip_title)) {
            $mess.=t("Trip has already been created for Trip ID  for date ".date('d-M-Y',$declaration_date));
        }
    } 
    else if (!empty($trip_vehicle_number)) {
        if (!empty($trip_vehicle_number) && empty($trip_vehicle_reason)) {
            $mess.=t('Please select reason for Vehicle Number: '.$trip_vehicle_number).'<br>';
        }
        $existing_trip_title =   check_trip_id_already_exist_in_declaration_date($form_state['values']['declaration_date'],$trip_id=null,$vehicle_number = $vehicle_number ,$resource_type=null,$resource_name=null,$delivery_boy_nid=null);
        if (!empty($existing_trip_title)) {
            $mess.=t("Trip has already been created for Vehicle Number: $vehicle_number for date ".date('d-M-Y',$declaration_date));
        }
        
    } 
    else if (!empty($trip_resource_name)) {
     
        if (!empty($trip_resource_name) && empty($trip_reason)) {
            $mess.=t('Please select reason for Resource Name: '.$trip_resource_name).'<br>';
        }
        if ($resource_type == 'Cashier') {
          $existing_trip_title =   check_trip_id_already_exist_in_declaration_date($form_state['values']['declaration_date'],$trip_id=null,$vehicle_number = null ,$resource_type='cashier',$resource_name=$trip_resource_name,$delivery_boy_nid=null);
          if (!empty($existing_trip_title)) {
              $mess.=t("Trip has already been created for Cashier: $trip_resource_name for date ".date('d-M-Y',$declaration_date));
          }
        }
        elseif ($resource_type == 'Delivery Boy') {
          $delivery_boy_nid = check_resource_exist($resource_type, $trip_resource_name);
          $existing_trip_title =   check_trip_id_already_exist_in_declaration_date($form_state['values']['declaration_date'],$trip_id=null,$vehicle_number = null ,$resource_type='delivery boy',$resource_name=$trip_resource_name,$delivery_boy_nid);
          if (!empty($existing_trip_title)) {
              $mess.=t("Trip has already been created for Delivery Boy: $trip_resource_name for date ".date('d-M-Y',$declaration_date));
          }
        }
    }
    if (!empty($mess)) {
        form_set_error('form',$mess);
        $form_state['rebuild'] = TRUE;
    }
}

function save_vehicle_resource_daily_status($form, $form_state) {
    global $user;
    $distributor_id = change_feeds_import_get_distributor_id($user->uid);
    $delivery_boy_nids = get_delivery_boy_nid();
    $trip_vehicle_number = $form_state['values']['trip_vehicle_number'];
    $trip_vehicle_reason = $form_state['values']['trip_vehicle_reason'];
    $trip_cashier = $form_state['values']['trip_cashier'];
    $trip_cashier_reason = $form_state['values']['trip_cashier_reason'];
    $trip_id = $form_state['values']['trip_id'];
    $vehicle_number = $form_state['values']['vehicle_number'];
    $resource_type = $form_state['values']['resource_type'];
    $resource_name = $form_state['values']['resource_name'];
    $declaration_date = strtotime($form_state['values']['declaration_date']);
    $trip_resource_name = $form_state['values']['trip_resource_name'];
    $trip_reason = $form_state['values']['trip_reason'];
    if (!empty($trip_id)) {
        $resource_type = 'Delivery Boy';
        foreach ($delivery_boy_nids as $key => $value) {
            $delivery_boy_name = $form_state['values']['trip_delivery_boy_' . $value];
            $delivery_boy_reason = $form_state['values']['trip_delivery_boy_reason_' . $value];
            if (!empty($delivery_boy_name) && !empty($delivery_boy_reason) && !empty($value)) {
                $result = db_merge('vehicle_resource_daily_status')
                    ->key(array('resource_nid' => $value, 'resource_name' => $delivery_boy_name,'declaration_date' => $declaration_date))
                    ->fields(array(
                      'resource_nid' => $value,
                      'trip_id' => $trip_id,
                      'reason' => $delivery_boy_reason,
                      'resource_name' => $delivery_boy_name,
                      'vehicle_number' => '',
                      'resource_type' => $resource_type,
                      'declaration_date' => $declaration_date,
                      'created_date' => time(),
                      'distributor_id' => $distributor_id ? $distributor_id : 0
                    ))
                    ->execute();
            }
        }
        // Vehicle reason update
        $result = db_merge('vehicle_resource_daily_status')
            ->key(array('trip_id' => $trip_id, 'vehicle_number' => $trip_vehicle_number,'declaration_date' => $declaration_date))
            ->fields(array(
              'resource_nid' => 0,
              'reason' => $trip_vehicle_reason,
              'resource_name' => '',
              'resource_type' => '',
              'declaration_date' => $declaration_date,
              'vehicle_number' => $trip_vehicle_number, // same table, diff is trip and master trip
              'created_date' => time(),
              'distributor_id' => $distributor_id ? $distributor_id : 0
            ))
            ->execute();

        // cashier insert
        $resource_type = 'Cashier';
        $result = db_merge('vehicle_resource_daily_status')
            ->key(array('trip_id' => $trip_id, 'resource_name' => $trip_cashier,'declaration_date' => $declaration_date))
            ->fields(array(
              'resource_nid' => 0,
              'reason' => $trip_cashier_reason,
              'resource_name' => $trip_cashier,
              'resource_type' => $resource_type,
              'declaration_date' => $declaration_date,
              'vehicle_number' => '', // same table, diff is trip and master trip
              'created_date' => time(),
              'distributor_id' => $distributor_id ? $distributor_id : 0
            ))
            ->execute();
    } 
    else if (!empty($trip_vehicle_number)) {
        $result = db_merge('vehicle_resource_daily_status')
            ->key(array('vehicle_number' => $trip_vehicle_number,'declaration_date' => $declaration_date))
            ->fields(array(
              'reason' => $trip_vehicle_reason,
              'resource_nid' => 0,
              'declaration_date' => $declaration_date,
              'vehicle_number' => $trip_vehicle_number, // same table, diff is trip and master trip
              'created_date' => time(),
              'distributor_id' => $distributor_id ? $distributor_id : 0
            ))
            ->execute();
    } 
    else if (!empty($trip_resource_name)) {
        $result = db_merge('vehicle_resource_daily_status')
            ->key(array('resource_name' => $trip_resource_name, 'resource_type' => $resource_type,'declaration_date' => $declaration_date))
            ->fields(array(
              'reason' => $trip_reason,
              'resource_nid' => 0,
              'declaration_date' => $declaration_date,
              'created_date' => time(),
              'distributor_id' => $distributor_id ? $distributor_id : 0
            ))
            ->execute();
    }
    drupal_set_message(t('Successfully Saved.'));
}

function get_delivery_boy_nid() {
    global $user;
    $dis_id = change_feeds_import_get_distributor_id($user->uid);
    $resource_type = 'Delivery Boy';
    $query = db_select('node', 'n');
    $query->condition('n.type', 'resource_master');
    $query->condition('n.uid', $dis_id);
    $query->join('field_data_field_resource_type', 'rt', 'rt.entity_id = n.nid');
    if (!empty($resource_type)) {
        $query->condition('rt.field_resource_type_value', $resource_type);
    }
    $query->join('field_data_field_resource_name', 'rn', 'rn.entity_id = n.nid');
    $query->leftjoin('field_data_field_is_active', 'is_active', 'is_active.entity_id = n.nid');
    // $query->condition('is_active.field_is_active_value', 1);
    $query->fields('n', array('nid'));
    $result = $query->execute()->fetchAllKeyed(0, 0);
    return $result;
}

function get_trip_details($trip_id = null, $vehilce_number = null, $resource_name = null, $resource_type = null, $declaration_date) {
    $data = array();
    $declaration_date = strtotime($declaration_date);
    if (empty($trip_id) && empty($vehilce_number) && empty($resource_name)) {
        return $data;
    }
    global $user;
    $dis_id = change_feeds_import_get_distributor_id($user->uid);
    $query = db_select('vehicle_resource_daily_status', 'vs');
    $query->condition('distributor_id', $dis_id);
    $query->condition('declaration_date', $declaration_date);
    $query->fields('vs');
    if (!empty($trip_id)) {
        $query->condition('trip_id', $trip_id);
    } else if (!empty($vehilce_number)) {
        $query->condition('vehicle_number', $vehilce_number);
    } else if (!empty($resource_name) && !empty($resource_type)) {
        $query->condition('resource_name', $resource_name);
    }
    $result = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
    $data = array();
    if (!empty($result)) {
        foreach ($result as $key => $value) {
            if (!empty($value['resource_nid']) && $value['resource_type'] == 'Delivery Boy') {
                $data[$value['resource_nid']] = $value['reason'];
            }
            if (!empty($value['vehicle_number'])) {
                $data['vehicle_number'] = $value['reason'];
            }
            if (!empty($value['resource_name'])) {
                $data[$value['resource_name']] = $value['reason'];
            }
            $data['declaration_date'] = date('Y-m-d', $value['declaration_date']);
        }
    }
    return $data;
}

function check_resource_exists($resource_type, $resource_name) {
    $result = '';
    if (!empty($resource_type) && !empty($resource_name)) {
        global $user;
        $dis_id = change_feeds_import_get_distributor_id($user->uid);
        $query = db_select('node', 'n');
        $query->condition('n.type', 'resource_master');
        $query->condition('n.uid', $dis_id);
        $query->join('field_data_field_resource_type', 'rt', 'rt.entity_id = n.nid');
        $query->condition('rt.field_resource_type_value', $resource_type);
        $query->join('field_data_field_resource_name', 'rn', 'rn.entity_id = n.nid');
        $query->leftjoin('field_data_field_is_active', 'is_active', 'is_active.entity_id = n.nid');
        $query->condition('rn.field_resource_name_value', $resource_name);
        $query->fields('n', array('nid'));
        $result = $query->execute()->fetchAllKeyed(0, 0);
    }
    return $result;
}

function check_dispatch_date_trip_number($dispatch_date,$vehilce_number) {
    global $user;
    $dis_id = change_feeds_import_get_distributor_id($user->uid);
    $res = '';
    if (!empty($dispatch_date)) {
        $query = db_select('dispatch_planning_api_data', 'ad');
        $query->condition('ad.distributor_id', $dis_id, '=');
        if (!empty($vehilce_numbervehicle)) {
            $query->condition('ad.vehicle', $vehilce_numbervehicle, '=');
        }
        $query->condition('ad.date', $dispatch_date, '=');
        $query->join('node', 'n', 'n.title = ad.bill_number AND n.uid = ad.distributor_id');
        $query->condition('n.type', 'sales_register_data', '=');
        $query->condition('n.uid', $dis_id);
        $query->join('dispatch_planning_item_details', 'item', 'item.trip_bill_reference = ad.id');
        $query->join('node', 'disp_data', 'disp_data.title = ad.runsheet_number');
        $query->condition('disp_data.type', 'dispatch_data');
        $query->condition('disp_data .uid', $dis_id);
        $query->leftJoin('node', 'vehicle', 'vehicle.title = ad.vehicle');
        $query->condition('vehicle.type', 'vehicle_master');
        $query->condition('vehicle.uid', $dis_id);
        $query->groupBy('ad.runsheet_number');
        $query->addExpression('n.title', 'trip_id');
        $res = $query->execute()->fetchAllKeyed(0, 0);
    }
    return $res;
}
function check_trip_id_already_exist_in_declaration_date($declaration_date,$trip_id=null,$vehicle_number=null,$resource_type=null,$resource_name=null,$delivery_boy_nid=null){
 if (!empty($declaration_date)) {
    $trip_id_result = '';
    $query = db_select('node', 'n');
    if (!empty($trip_id)) {
       $query->condition('n.title', $trip_id, "=");
    }
    $query->condition('n.type', 'dispatch_data');
    $query->join('field_data_field_dispatch_date', 'dt', 'dt.entity_id = n.nid');
    $query->condition('dt.field_dispatch_date_value',$declaration_date);
    $query->addTag('node_distributor_access');
    if (!empty($vehicle_number)) {
      $query->join('field_data_field_vehicle_no', 've', 've.entity_id = n.nid');
      $query->condition('ve.field_vehicle_no_value', $vehicle_number);
    }
    if(!empty($resource_name) && $resource_type == 'cashier') {
      $query->join('field_data_field_cashier_name', 'cash', 'cash.entity_id = n.nid');
      $query->condition('cash.field_cashier_name_value', $resource_name); 
    }
    if(!empty($resource_name) && $resource_type == 'delivery boy') {
       $query->join('field_data_field_delivery_boy', 'delivery_boys', 'delivery_boys.entity_id = n.nid');
       $query->condition('delivery_boys.field_delivery_boy_nid', $delivery_boy_nid); 
    }
    $query->fields('n', array('title'));
    $trip_id_result = $query->execute()->fetchField();
   }
  return $trip_id_result;
}


