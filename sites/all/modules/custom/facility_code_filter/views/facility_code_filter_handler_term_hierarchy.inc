<?php

// This file must be at user_hierarchy/views directory.

/**
 * @file
 * Definition of handler_term_hierarchy.
 */

/**
 * Description of what my handler does.
 */
class facility_code_filter_handler_term_hierarchy extends views_handler_field {

    /**
     * Determine if User Hierarchy field can be exposed.
     */
    var $term_options = array();
    var $user_term_id;
    var $distributor_id;

    // required for expose form  
    function can_expose() {
        global $user;
        if (in_array("administrator", $user->roles)|| in_array("central_distributor", $user->roles) || in_array("super_central_distributor", $user->roles)) {
            return TRUE;
        } else {
            return FALSE;
        }
    }

    /**
     * Called to add the sort to a query.
     */
    function query() {
        global $user;
        $view = $this->view;
        $selected_distributor_id = $view->exposed_input['user_hierarchy_values'];
        $selected_facility_code = $view->exposed_input['facility_code'];
        if (!empty($view) && !empty($selected_facility_code) && $selected_facility_code!= 'All') {
            // Fetch all distributor ids of selected facility code
            $query = db_select('field_data_field_central_distributorid_ref', 'cdr');
            $query->join('field_data_field_distributor', 'distributor', 'cdr.entity_id = distributor.entity_id');
            $query->join('field_data_field_facility_code', 'facility_code', 'facility_code.entity_id = distributor.entity_id');
            $query->condition('facility_code.field_facility_code_tid', $selected_facility_code, '=');
            $query->fields('distributor', array('field_distributor_value'));
            $facility_distributor_ids = $query->execute()->fetchAllKeyed(0, 0); // will get child uids=>rs_name
            $facility_distributor_ids = $facility_distributor_ids?$facility_distributor_ids:array('');
           // print_r($facility_distributor_ids);
            //if (!empty($facility_distributor_ids)) {
                $custom_tables = explode(',', variable_get('custom_tables'));
                if (!empty($custom_tables)) {
                    if (false !== $table_key = array_search($view->base_table, $custom_tables)) {
                        $this->query->add_where(0, "$custom_tables[$table_key].distributor_id", $facility_distributor_ids, 'IN');
                    }
                }
                if ($view->base_table == 'node') {
                    $this->query->add_where(0, 'node.uid', $facility_distributor_ids);
                }
            }
        //}
    }

    function render($values) {
        /* // Return the text, so the code never thinks the value is empty.
          $form['expose']['label'] = array(
          '#type' => 'textfield',
          '#default_value' => $this->options['expose']['label'],
          '#title' => t('Label'),
          '#required' => TRUE,
          '#size' => 40,
          '#weight' => -1,
          );
          return $form; */
    }

    /**
     * Render our chunk of the exposed handler form when selecting
     */
    // required for expose form  
    function exposed_form(&$form, &$form_state) {
        global $user;
        if (in_array("administrator", $user->roles) || in_array("super_central_distributor", $user->roles) || in_array("administrator", $user->roles)) {

            $vocabulary = taxonomy_vocabulary_machine_name_load('facility_code');
            $terms_obj = entity_load('taxonomy_term', FALSE, array('vid' => $vocabulary->vid));
            if (!empty($terms_obj)) {
                foreach ($terms_obj as $key => $value) {
                   //$facility[$value->tid] = $value->name;
                    $facility[$value->tid] = $value->field_facility_name[LANGUAGE_NONE][0]['value'];
                }
                $facility = array('All' => 'Select') + $facility;
            }
            $form['expose']['facility_code'] = array(
                '#title' => t('Select Facility Code'),
                '#type' => 'select',
                '#description' => 'Facility Code.',
               // '#attributes'=>array('style'=>"height:70px;"),
               // '#multiple' => TRUE,
                '#default_value' => $default_value,
                '#options' => $facility,
                '#weight' => -25,
            );
        }
        return $form;
    }

    // Required to display in form
    function options_form(&$form, &$form_state) {
        parent::options_form($form, $form_state);
        if ($this->can_expose()) {
            $this->show_expose_button($form, $form_state);
        }
        if ($this->can_expose()) {
            $this->show_expose_form($form, $form_state);
        }
    }

    /**
     * Shortcut to display the expose/hide button.
     */
    // required for expose form  
    function show_expose_button(&$form, &$form_state) {

        $form['expose_button'] = array(
            '#prefix' => '<div class="views-expose clearfix">',
            '#suffix' => '</div>',
            // Should always come first
            '#weight' => -1000,
        );

        // Add a checkbox for JS users, which will have behavior attached to it
        // so it can replace the button.
        $form['expose_button']['checkbox'] = array(
            '#theme_wrappers' => array('container'),
            '#attributes' => array('class' => array('js-only')),
        );
        $form['expose_button']['checkbox']['checkbox'] = array(
            '#title' => t('Expose this field to visitors, to allow them to change it'),
            '#type' => 'checkbox',
        );

        // Then add the button itself.
        if (empty($this->options['exposed'])) {
            $form['expose_button']['markup'] = array(
                '#markup' => '<div class="description exposed-description" style="float: left; margin-right:10px">' . t('This sort is not exposed. Expose it to allow the users to change it.') . '</div>',
            );
            $form['expose_button']['button'] = array(
                '#limit_validation_errors' => array(),
                '#type' => 'submit',
                '#value' => t('Expose sort'),
                '#submit' => array('views_ui_config_item_form_expose'),
            );
            $form['expose_button']['checkbox']['checkbox']['#default_value'] = 0;
        } else {
            $form['expose_button']['markup'] = array(
                '#markup' => '<div class="description exposed-description">' . t('This sort is exposed. If you hide it, users will not be able to change it.') . '</div>',
            );
            $form['expose_button']['button'] = array(
                '#limit_validation_errors' => array(),
                '#type' => 'submit',
                '#value' => t('Hide sort'),
                '#submit' => array('views_ui_config_item_form_expose'),
            );
            $form['expose_button']['checkbox']['checkbox']['#default_value'] = 1;
        }
    }

}
