<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function dispatched_quantity_confirmation_views_data() {
    $data = array();

    // Dispatched Quantity Confirmation
    $data['dispatch_qty_confirmation']['table'] = array(
        'group' => t('dispatch_qty_confirmation data'),
        'title' => t('dispatch_qty_confirmation data'),
        'help' => t('dispatch_qty_confirmation data.'),
    );

    // Define this as a base table.
    $data['dispatch_qty_confirmation']['table']['base'] = array(
        'field' => 'id',
        'title' => t('dispatch_qty_confirmation'),
        'help' => t("unique identifiers."),
            // 'weight' => -10,
    );

    $data['dispatch_qty_confirmation']['id'] = array(
        'title' => t('ID'),
        'help' => t('ID'),
        'field' => array(
            'handler' => 'views_handler_field',
            'click sortable' => TRUE,
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_string',
        ),
        'argument' => array(
            'handler' => 'views_handler_argument_string',
        ),
    );

    $data['dispatch_qty_confirmation']['trip_id'] = array(
        'title' => t('Trip ID'),
        'help' => t('Trip ID'),
        'field' => array(
            'handler' => 'views_handler_field',
            'click sortable' => TRUE,
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_string',
        ),
        'argument' => array(
            'handler' => 'views_handler_argument_string',
        ),
        'relationship' => array(
            'title' => t('dispatch_qty_confirmation Trip id reference.'),
            'help' => t("dispatch_qty_confirmation Trip id reference."),
            'handler' => 'views_handler_relationship',
            'base' => 'node',
            'base field' => 'title',
            'extra' => array(
                array('field' => 'type', 'value' => 'dispatch_data'),
                array('field' => 'uid', 'left_field' => 'distributor_id'),
            ),
        ),
    );

    $data['dispatch_qty_confirmation']['bill_number'] = array(
        'title' => t('Bill Number'),
        'help' => t('Bill Number'),
        'field' => array(
            'handler' => 'views_handler_field',
            'click sortable' => TRUE,
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_string',
        ),
        'argument' => array(
            'handler' => 'views_handler_argument_string',
        ),
        'relationship' => array(
            'title' => t('Custom Billing Bill Sku reference.'),
            'help' => t("Custom Billing Bill Sku reference."),
            'handler' => 'views_handler_relationship',
            'base' => 'custom_billing_data',
            'base field' => 'bill_number',
            'extra' => array(
                array('field' => 'sku7_code', 'left_field' => 'sku_code'),
                array('field' => 'distributor_id', 'left_field' => 'distributor_id'),
            ),
        ),
    );

    $data['dispatch_qty_confirmation']['sku_code'] = array(
        'title' => t('SKU Code'),
        'help' => t('SKU Code'),
        'field' => array(
            'handler' => 'views_handler_field',
            'click sortable' => TRUE,
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_string',
        ),
        'argument' => array(
            'handler' => 'views_handler_argument_string',
        ),
    );

    $data['dispatch_qty_confirmation']['product_name'] = array(
        'title' => t('Product Name'),
        'help' => t('Product Name'),
        'field' => array(
            'handler' => 'views_handler_field',
            'click sortable' => TRUE,
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_string',
        ),
        'argument' => array(
            'handler' => 'views_handler_argument_string',
        ),
    );
    $data['dispatch_qty_confirmation']['mrp'] = array(
        'title' => t('MRP'),
        'help' => t('MRP'),
        'field' => array(
            'handler' => 'views_handler_field_numeric',
            'click sortable' => TRUE,
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_numeric',
        ),
        'argument' => array(
            'handler' => 'views_handler_argument_numeric',
        ),
    );
    $data['dispatch_qty_confirmation']['tur'] = array(
        'title' => t('TUR'),
        'help' => t('TUR'),
        'field' => array(
            'handler' => 'views_handler_field_numeric',
            'click sortable' => TRUE,
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_numeric',
        ),
        'argument' => array(
            'handler' => 'views_handler_argument_numeric',
        ),
    );    
    $data['dispatch_qty_confirmation']['original_loose_qty'] = array(
        'title' => t('Original loose qty'),
        'help' => t('Original loose qty'),
        'field' => array(
            'handler' => 'views_handler_field_numeric',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_numeric',
        ),
    );

    $data['dispatch_qty_confirmation']['ptl_put_qty'] = array(
        'title' => t('PLT Put QTY'),
        'help' => t('PLT Put QTY'),
        'field' => array(
            'handler' => 'views_handler_field_numeric',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_numeric',
        ),
    );

    $data['dispatch_qty_confirmation']['pplc_confirmed_qty'] = array(
        'title' => t('PPLC Confirmed qty'),
        'help' => t('PPLC confirmed qty'),
        'field' => array(
            'handler' => 'views_handler_field_numeric',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_numeric',
        ),
    );

    $data['dispatch_qty_confirmation']['short_reason'] = array(
        'title' => t('Short reason'),
        'help' => t('Short reason'),
        'field' => array(
            'handler' => 'views_handler_field',
            'click sortable' => TRUE,
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_string',
        ),
        'argument' => array(
            'handler' => 'views_handler_argument_string',
        ),
    );

    $data['dispatch_qty_confirmation']['cld_short_reason'] = array(
        'title' => t('CLD short reason'),
        'help' => t('CLD short reason'),
        'field' => array(
            'handler' => 'views_handler_field',
            'click sortable' => TRUE,
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_string',
        ),
        'argument' => array(
            'handler' => 'views_handler_argument_string',
        ),
    );

    $data['dispatch_qty_confirmation']['dispatch_loose_qty'] = array(
        'title' => t('Dispatch loose qty'),
        'help' => t('Dispatch loose qty'),
        'field' => array(
            'handler' => 'views_handler_field_numeric',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_numeric',
        ),
    );

    $data['dispatch_qty_confirmation']['original_cld_qty'] = array(
        'title' => t('Original cld qty'),
        'help' => t('Original cld qty'),
        'field' => array(
            'handler' => 'views_handler_field_numeric',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_numeric',
        ),
    );

    $data['dispatch_qty_confirmation']['loose_close_flag'] = array(
        'title' => t('Loose close flag'),
        'help' => t('Loose close flag'),
        'field' => array(
            'handler' => 'views_handler_field_numeric',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_numeric',
        ),
    );

    $data['dispatch_qty_confirmation']['cld_close_flag'] = array(
        'title' => t('CLD Close flag'),
        'help' => t('CLD Close flag'),
        'field' => array(
            'handler' => 'views_handler_field_numeric',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_numeric',
        ),
    );

    $data['dispatch_qty_confirmation']['dispatch_cld_qty'] = array(
        'title' => t('Dispatch cld qty'),
        'help' => t('Dispatch cld qty'),
        'field' => array(
            'handler' => 'views_handler_field_numeric',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_numeric',
        ),
    );

    $data['dispatch_qty_confirmation']['cld_confirmed_qty'] = array(
        'title' => t('CLD Confirmed qty'),
        'help' => t('CLD Confirmed qty'),
        'field' => array(
            'handler' => 'views_handler_field_numeric',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_numeric',
        ),
    );
    $data['dispatch_qty_confirmation']['declared_cld_qty'] = array(
        'title' => t('Declared CLD qty'),
        'help' => t('Declared CLD qty'),
        'field' => array(
            'handler' => 'views_handler_field_numeric',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_numeric',
        ),
    );
    
    $data['dispatch_qty_confirmation']['upc'] = array(
        'title' => t('UPC'),
        'help' => t('UPC'),
        'field' => array(
            'handler' => 'views_handler_field_numeric',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_numeric',
        ),
    );
    $data['dispatch_qty_confirmation']['original_qty'] = array(
        'title' => t('Original Quantity'),
        'help' => t('Original Quantity'),
        'field' => array(
            'handler' => 'views_handler_field_numeric',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_numeric',
        ),
    );
    $data['dispatch_qty_confirmation']['to_be_dispatch_qty'] = array(
        'title' => t('To be Dispatch Quantity'),
        'help' => t('To be Dispatch Quantity'),
        'field' => array(
            'handler' => 'views_handler_field_numeric',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_numeric',
        ),
    );
    $data['dispatch_qty_confirmation']['type'] = array(
        'title' => t('Type'),
        'help' => t('Type'),
        'field' => array(
            'handler' => 'views_handler_field',
            'click sortable' => TRUE,
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_string',
        ),
        'argument' => array(
            'handler' => 'views_handler_argument_string',
        ),
    );

    $data['dispatch_qty_confirmation']['created_date'] = array(
        'title' => t('Created date'),
        'help' => t('Created date'),
        'field' => array(
            'handler' => 'views_handler_field_date',
            'click sortable' => TRUE,
        ),
        'sort' => array(
            'handler' => 'views_handler_sort_date',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_date',
        ),
        'argument' => array(
            'handler' => 'views_handler_filter_date',
        ),
    );
    $data['dispatch_qty_confirmation']['loose_created_date'] = array(
        'title' => t('Loose Created date'),
        'help' => t('Loose Created date'),
        'field' => array(
            'handler' => 'views_handler_field_date',
            'click sortable' => TRUE,
        ),
        'sort' => array(
            'handler' => 'views_handler_sort_date',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_date',
        ),
        'argument' => array(
            'handler' => 'views_handler_filter_date',
        ),
    );    
    $data['dispatch_qty_confirmation']['cld_created_date'] = array(
        'title' => t('CLD Created date'),
        'help' => t('CLD Created date'),
        'field' => array(
            'handler' => 'views_handler_field_date',
            'click sortable' => TRUE,
        ),
        'sort' => array(
            'handler' => 'views_handler_sort_date',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_date',
        ),
        'argument' => array(
            'handler' => 'views_handler_filter_date',
        ),
    );
    
    $data['dispatch_qty_confirmation']['cld_retail_confirmed_flag'] = array(
        'title' => t('CLD Retail Confirmed Flag'),
        'help' => t('CLD Retail Confirmed Flag'),
        'field' => array(
            'handler' => 'views_handler_field_numeric',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_numeric',
        ),
    );
    
      $data['dispatch_qty_confirmation']['cld_loose_child_qty'] = array(
        'title' => t('CLD Loose child qty'),
        'help' => t('CLD Loose child qty'),
        'field' => array(
            'handler' => 'views_handler_field_numeric',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_numeric',
        ),
    );
      
    $data['dispatch_qty_confirmation']['masterptl_short_qty'] = array(
        'title' => t('Master PTL short qty'),
        'help' => t('Master PTL short qty'),
        'field' => array(
            'handler' => 'views_handler_field_numeric',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_numeric',
        ),
    );  

    $data['dispatch_qty_confirmation']['distributor_id'] = array(
        'title' => t('Distributor id'),
        'help' => t('Distributor id'),
        'field' => array(
            'handler' => 'views_handler_field_numeric',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_numeric',
        ),
    );
    //Date Filter
    $base_table = 'dispatch_qty_confirmation';
    $data[$base_table]['date_filter'] = array(
        'group' => t('Date'),
        'title' => t('Date (!base_table)', array('!base_table' => $base_table)),
        'help' => t('Filter any Views !base_table date field.', array('!base_table' => $base_table)),
        'filter' => array(
            'handler' => 'date_views_filter_handler',
            'empty field name' => t('Undated'),
            'is date' => TRUE,
        ),
    );

    /*
      //Host Revised Qty table view
      $data['host_revised_quantity']['table'] = array(
      'group' => t('host_revised_quantity data'),
      'title' => t('host_revised_quantity data'),
      'help' => t('host_revised_quantity data.'),
      );

      // Define this as a base table.
      $data['host_revised_quantity']['table']['base'] = array(
      'field' => 'id',
      'title' => t('host_revised_quantity'),
      'help' => t("unique identifiers."),
      // 'weight' => -10,
      );

      $data['host_revised_quantity']['id'] = array(
      'title' => t('ID'),
      'help' => t('ID'),
      'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
      ),
      'sort' => array(
      'handler' => 'views_handler_sort',
      ),
      'filter' => array(
      'handler' => 'views_handler_filter_string',
      ),
      'argument' => array(
      'handler' => 'views_handler_argument_string',
      ),
      );

      $data['host_revised_quantity']['bill_number'] = array(
      'title' => t('Bill Number'),
      'help' => t('Bill Number'),
      'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
      ),
      'sort' => array(
      'handler' => 'views_handler_sort',
      ),
      'filter' => array(
      'handler' => 'views_handler_filter_string',
      ),
      'argument' => array(
      'handler' => 'views_handler_argument_string',
      ),
      );

      $data['host_revised_quantity']['sku_code'] = array(
      'title' => t('SKU Code'),
      'help' => t('SKU Code'),
      'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
      ),
      'sort' => array(
      'handler' => 'views_handler_sort',
      ),
      'filter' => array(
      'handler' => 'views_handler_filter_string',
      ),
      'argument' => array(
      'handler' => 'views_handler_argument_string',
      ),
      );

      $data['host_revised_quantity']['revised_qty'] = array(
      'title' => t('Host Revised Quantity'),
      'help' => t('Host Revised Quantity'),
      'field' => array(
      'handler' => 'views_handler_field_numeric',
      ),
      'sort' => array(
      'handler' => 'views_handler_sort',
      ),
      'filter' => array(
      'handler' => 'views_handler_filter_numeric',
      ),
      );

      $data['host_revised_quantity']['created_date'] = array(
      'title' => t('created_date'),
      'help' => t('created_date'),
      'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
      ),
      'sort' => array(
      'handler' => 'views_handler_sort_date',
      ),
      'filter' => array(
      'handler' => 'views_handler_filter_date',
      ),
      'argument' => array(
      'handler' => 'views_handler_filter_date',
      ),
      );

      $data['host_revised_quantity']['distributor_id'] = array(
      'title' => t('distributor_id'),
      'help' => t('distributor_id'),
      'field' => array(
      'handler' => 'views_handler_field_numeric',
      ),
      'sort' => array(
      'handler' => 'views_handler_sort',
      ),
      'filter' => array(
      'handler' => 'views_handler_filter_numeric',
      ),
      );

      $base_table = 'host_revised_quantity';
      $data[$base_table]['date_filter'] = array(
      'group' => t('Date'),
      'title' => t('Date (!base_table)', array('!base_table' => $base_table)),
      'help' => t('Filter any Views !base_table date field.', array('!base_table' => $base_table)),
      'filter' => array(
      'handler' => 'date_views_filter_handler',
      'empty field name' => t('Undated'),
      'is date' => TRUE,
      ),
      );
     */

    //excess_qty Qty table view
    $data['excess_qty']['table'] = array(
        'group' => t('excess_qty data'),
        'title' => t('excess_qty data'),
        'help' => t('excess_qty data.'),
    );

    // Define this as a base table.
    $data['excess_qty']['table']['base'] = array(
        'field' => 'id',
        'title' => t('excess_qty'),
        'help' => t("unique identifiers."),
            // 'weight' => -10,
    );

    $data['excess_qty']['id'] = array(
        'title' => t('ID'),
        'help' => t('ID'),
        'field' => array(
            'handler' => 'views_handler_field',
            'click sortable' => TRUE,
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_string',
        ),
        'argument' => array(
            'handler' => 'views_handler_argument_string',
        ),
    );
    $data['excess_qty']['trip_id'] = array(
        'title' => t('Trip ID'),
        'help' => t('Trip ID'),
        'field' => array(
            'handler' => 'views_handler_field_numeric',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_numeric',
        ),
    );
    $data['excess_qty']['bill_number'] = array(
        'title' => t('Bill Number'),
        'help' => t('Bill Number'),
        'field' => array(
            'handler' => 'views_handler_field',
            'click sortable' => TRUE,
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_string',
        ),
        'argument' => array(
            'handler' => 'views_handler_argument_string',
        ),
    );

    $data['excess_qty']['sku_code'] = array(
        'title' => t('SKU Code'),
        'help' => t('SKU Code'),
        'field' => array(
            'handler' => 'views_handler_field',
            'click sortable' => TRUE,
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_string',
        ),
        'argument' => array(
            'handler' => 'views_handler_argument_string',
        ),
    );
    $data['excess_qty']['product_name'] = array(
        'title' => t('Product Name'),
        'help' => t('Product Name'),
        'field' => array(
            'handler' => 'views_handler_field',
            'click sortable' => TRUE,
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_string',
        ),
        'argument' => array(
            'handler' => 'views_handler_argument_string',
        ),
    );
    $data['excess_qty']['excess_qty'] = array(
        'title' => t('Excess Quantity'),
        'help' => t('Excess Quantity'),
        'field' => array(
            'handler' => 'views_handler_field_numeric',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_numeric',
        ),
    );
    $data['excess_qty']['reason_code'] = array(
        'title' => t('Reason Code'),
        'help' => t('Reason Code'),
        'field' => array(
            'handler' => 'views_handler_field',
            'click sortable' => TRUE,
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_string',
        ),
        'argument' => array(
            'handler' => 'views_handler_argument_string',
        ),
    );
    $data['excess_qty']['created_date'] = array(
        'title' => t('created_date'),
        'help' => t('created_date'),
        'field' => array(
            'handler' => 'views_handler_field_date',
            'click sortable' => TRUE,
        ),
        'sort' => array(
            'handler' => 'views_handler_sort_date',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_date',
        ),
        'argument' => array(
            'handler' => 'views_handler_filter_date',
        ),
    );

    $data['excess_qty']['distributor_id'] = array(
        'title' => t('distributor_id'),
        'help' => t('distributor_id'),
        'field' => array(
            'handler' => 'views_handler_field_numeric',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_numeric',
        ),
    );

    $base_table = 'excess_qty';
    $data[$base_table]['date_filter'] = array(
        'group' => t('Date'),
        'title' => t('Date (!base_table)', array('!base_table' => $base_table)),
        'help' => t('Filter any Views !base_table date field.', array('!base_table' => $base_table)),
        'filter' => array(
            'handler' => 'date_views_filter_handler',
            'empty field name' => t('Undated'),
            'is date' => TRUE,
        ),
    );
    return $data;
}