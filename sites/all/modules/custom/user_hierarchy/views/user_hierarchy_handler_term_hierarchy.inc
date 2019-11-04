<?php

// This file must be at user_hierarchy/views directory.

/**
 * @file
 * Definition of handler_term_hierarchy.
 */

/**
 * Description of what my handler does.
 */
class user_hierarchy_handler_term_hierarchy extends views_handler_field {

  /**
   * Determine if User Hierarchy field can be exposed.
   */
  var $term_options = array();
  var $user_term_id;
  var $distributor_id;

  // required for expose form  
  function can_expose() {
    global $user;
    if (in_array("administrator", $user->roles) || in_array("central_distributor", $user->roles) || in_array("super_central_rs", $user->roles)) {
      return TRUE;
    }
    else {
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
    if (!empty($view) && !empty($selected_distributor_id)) {
      //Custom Table join query 
      $custom_tables = explode(',', variable_get('custom_tables'));
      if (!empty($custom_tables)) {
        if (false !== $table_key = array_search($view->base_table, $custom_tables)) {
          $this->query->add_where(0, "$custom_tables[$table_key].distributor_id", $selected_distributor_id, '=');
        }
      }
      if ($view->base_table == 'node') {
        $this->query->add_where(0, 'node.uid', $selected_distributor_id);
      }
    }
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
    if (in_array("administrator", $user->roles) || in_array("central_distributor", $user->roles) || in_array("super_central_rs", $user->roles)) {
      //$distributor_ids = get_distributor_ids();
      $distributor_id = change_feeds_import_get_distributor_id($user->uid);
      //unset($distributor_ids[$distributor_id]);
      //$distributor_ids = array(''=>'All') + $distributor_ids;
      $child_distributors = get_child_distributor_rsname($distributor_id);
      $distributor_ids = array('' => 'Select') + $child_distributors;
    }
    if (isset($_GET['user_hierarchy_values'])) {
      $default_value = $_GET['user_hierarchy_values'];
    }

    $form['expose']['user_hierarchy_values'] = array(
      '#title' => t('Select Distributor Name'),
      '#type' => 'select',
      '#description' => 'Select Distributor Name.',
      '#default_value' => $default_value,
      '#options' => $distributor_ids,
      '#weight' => -1,
    );
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
    }
    else {
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
