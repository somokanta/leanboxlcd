<?php

/**
 * @file
 * Definition of example_handler_custom_field
 */

/**
 * Provides a custom views field.
 */
class basepack_sku7_code_custom_field extends views_handler_field {

   function option_definition() {
      $options = parent::option_definition();
      return $options;
   }

   function options_form(&$form, &$form_state) {
      parent::options_form($form, $form_state);
   }

   function query() {
      // do nothing -- to override the parent query.
   }

   function render($data) {
      $sku_code = $data->node_title;
      $node_uid = $data->node_uid;
      $query = db_select('custom_billing_data', 'b');
      $query->fields('b', array('BASEPACK_CODE'));
      $query->condition('b.distributor_id', $node_uid);
      $query->condition('b.sku7_code', $sku_code);
      $query->range(0, 1);
      $res = $query->execute()->fetchField();
      return $res;
   }

}
