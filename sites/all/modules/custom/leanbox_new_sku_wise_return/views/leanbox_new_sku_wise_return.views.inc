<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function leanbox_new_sku_wise_return_views_data() {
  $data = array();
  
  // Excess Inventory Table
  $data['excess_inventory_table']['table'] = array(
      'group' => t('excess_inventory_table data'),
      'title' => t('Excess Inventory Table data'),
      'help' => t('excess_inventory_table data.'),
  );
    
  // Define this as a base table.
  $data['excess_inventory_table']['table']['base'] = array(
      'field' => 'id',
      'title' => t('Excess Inventory Table'),
      'help' => t("unique identifiers."),
          // 'weight' => -10,
  );
  
  $data['excess_inventory_table']['id'] = array(
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
  
  $data['excess_inventory_table']['trip_id'] = array(
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
    );
  
  $data['excess_inventory_table']['product_name'] = array(
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
  
  $data['excess_inventory_table']['mrp'] = array(
        'title' => t('MRP'),
        'help' => t('MRP'),
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
  
  $data['excess_inventory_table']['excess_qty'] = array(
        'title' => t('Excess Qty'),
        'help' => t('Excess Qty'),
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
  
  $data['excess_inventory_table']['credit_to_cashier'] = array(
        'title' => t('Credit to Cashier'),
        'help' => t('Credit to Cashier'),
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
  
  $data['excess_inventory_table']['excess_reason'] = array(
        'title' => t('Excess Reason'),
        'help' => t('Excess Reason'),
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
  
  $data['excess_inventory_table']['remarks'] = array(
        'title' => t('Remarks'),
        'help' => t('Remarks'),
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
  $base_table = 'excess_inventory_table';
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
