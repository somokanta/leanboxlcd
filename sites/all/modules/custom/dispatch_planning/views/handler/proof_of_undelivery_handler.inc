<?php

/**
 * @file
 * Definition of example_handler_custom_field
 */

/**
 * Provides a custom views field.
 */
class proof_of_undelivery_handler extends views_handler_field {

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
     $proof_of_undelivery_photo = '';
    $proof_of_undelivery_photo_comma= $data->dispatch_planning_api_data_res_proof_of_undelivery;
    if (!empty($proof_of_undelivery_photo_comma) && $proof_of_undelivery_photo_comma != 'null') {
      $proof_of_undelivery_photo_arr = explode(',', $proof_of_undelivery_photo_comma);
      foreach ($proof_of_undelivery_photo_arr as $url) {
        if($url){
          $proof_of_undelivery_photo .= '<a target="_blank" href="' . $url . '">' . $url;
          $proof_of_undelivery_photo .= '</a>' . '<br>';
        }
      }
    }

    return $proof_of_undelivery_photo;
  }

}

/*
 *     if(!empty($image_urls)){
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
 */
