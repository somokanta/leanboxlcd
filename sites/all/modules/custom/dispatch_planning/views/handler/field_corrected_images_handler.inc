<?php

/**
 * @file
 * Definition of example_handler_custom_field
 */

/**
 * Provides a custom views field.
 */
class field_corrected_images_handler extends views_handler_field {

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
    $corrected_latlong_images = $data->dispatch_planning_api_data_corrected_latlong_images;
    
    if(!empty($corrected_latlong_images)) {
        $wrapper = file_stream_wrapper_get_instance_by_uri($photo_cheque);
        if (!empty($wrapper)) {
          $path = $wrapper->getExternalUrl();
        }
        elseif(empty($path) && !empty($corrected_latlong_images)) {
             $path = $corrected_latlong_images;
        }
        
        $cor_images = '<a target="_blank" href="' . $path . '">' . $path ;
                $cor_images .= '</a>';
    }

    return $cor_images;
   }

}


