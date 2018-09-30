<?php

/**
 * @file
 * Definition of views_handler_field_bill_value.
 */

/**
 * Returns crate count
 * @ingroup views_field_handlers
 */
class handler_deliverycost_calculation extends views_handler_field {

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
        $views_current_field = $this->field;
        $page = arg();
        $view='';
        $action = t('Add');
        if (($page[0] == 'vehicle-master' || $page[0] =='resource-master-report') && $page[1]!='edit') {
            $view='view';
            $action = t('View');
        }
         switch ($views_current_field) {
                case 'vehicle_maintenace_entry' :
                       $output = '<div class="maintenance_entry">'; 
                       $output.= ctools_modal_text_button($action, 'leave_entry/nojs/add/vehicle/'.$data->nid.'/'.$view, 'maintenace entry');
                       $output.= '</div>'; 
                       break;
                case 'resource_leave_entry';
                       $output = '<div class="maintenance_entry">'; 
                       $output .= ctools_modal_text_button($action, 'leave_entry/nojs/add/resource/'.$data->nid.'/'.$view, 'resource leave entry');
                       $output.= '</div>'; 
                       break;
                    
                  case 'vehicle_contract_entry';
                       $output = '<div class="maintenance_entry">'; 
                       $output.= ctools_modal_text_button($action, 'contract_entry/nojs/add/vehicle/'.$data->nid.'/'.$view, 'maintenace entry');
                       $output.= '</div>'; 
                       break; 
                 case 'resource_contract_entry';
                       $output = '<div class="maintenance_entry">'; 
                       $output.= ctools_modal_text_button($action, 'contract_entry/nojs/add/resource/'.$data->nid.'/'.$view, 'maintenace entry');
                       $output.= '</div>'; 
                       break;
                 case 'resource_extra_working';
                       $output = '<div class="maintenance_entry">'; 
                       $output.= ctools_modal_text_button($action, 'extra_working/nojs/add/resource/'.$data->nid.'/'.$view, 'maintenace entry');
                       $output.= '</div>'; 
                       break; 
                   
                case 'vehicle_extra_working';
                       $output = '<div class="maintenance_entry">'; 
                       $output.= ctools_modal_text_button($action, 'extra_working/nojs/add/vehicle/'.$data->nid.'/'.$view, 'maintenace entry');
                       $output.= '</div>'; 
                       break;    
                       
         }

        
        return array('markup' => array('#markup' => $output));
    }

}
