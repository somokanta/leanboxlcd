<?php

/**
 * @file
 * Definition of example_handler_custom_field
 */

/**
 * Provides a custom views field.
 */
class photo_url_handler extends views_handler_field {

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
    $money_collect_edit = $data->dispatch_planning_api_data_res_money_collect2_edit;

    if (!empty($money_collect_edit)) {
      $res_photo_of_cheque = $data->dispatch_planning_api_data_res_photo_of_cheque_edit;
      $photo_of_signed_bill_partial = $data->dispatch_planning_api_data_photo_of_signed_bill_partial_edit;
      $res_photo_of_signed_bill_delivery = $data->dispatch_planning_api_data_res_photo_of_signed_bill_delivery_1;
    }
    else {
      $res_photo_of_cheque = $data->dispatch_planning_api_data_res_photo_of_cheque;
      $photo_of_signed_bill_partial = $data->dispatch_planning_api_data_photo_of_signed_bill_partial;
      $res_photo_of_signed_bill_delivery = $data->dispatch_planning_api_data_res_photo_of_signed_bill_delivery;
    }
    
    $na_position = strpos($res_photo_of_cheque, '/NA');
    if (!empty($na_position)) {
      unset($res_photo_of_cheque);
    }
    
    $na_position = strpos($photo_of_signed_bill_partial, '/NA');
    if (!empty($na_position)) {
      unset($photo_of_signed_bill_partial);
    }
    
    $na_position = strpos($res_photo_of_signed_bill_delivery, '/NA');
    if (!empty($na_position)) {
      unset($res_photo_of_signed_bill_delivery);
    }
    
    if (!empty($res_photo_of_cheque)) {
      $photo_url = $res_photo_of_cheque;
    }
    else if (!empty($photo_of_signed_bill_partial)) {
      $photo_url = $photo_of_signed_bill_partial;
    }
    else if (!empty($res_photo_of_signed_bill_delivery)) {
      $photo_url = $res_photo_of_signed_bill_delivery;
    }

    if (!empty($photo_url)) {
      $url = $photo_url;
      $path = $url;
      $na_position = strpos($url, '/NA');
      $wrapper = file_stream_wrapper_get_instance_by_uri($url);
      if (empty($na_position)) {
        if (!empty($wrapper)) {
          $path = $wrapper->getExternalUrl();
        }
        $bill_photo .= '<a target="_blank" href="' . $path . '">'.$path;
        $bill_photo .= '</a>';
      }
    }

    return $bill_photo;
  }

}
