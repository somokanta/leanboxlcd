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
        $cor_images = '<a target="_blank" href="' . $path . '">' . $path ;
        $cor_images .= '</a> ,';
        $img = explode(",",$cor_images);
        foreach($img as $v){
         $url = rtrim('<a target="_blank" href="' . $v . '">' . $v. '</a> ,', ",");
        }        
    }

    return $url;
   }

}


