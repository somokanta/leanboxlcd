<?php

function change_feeds_import_views_data_alter(&$data) {
  $data['field_data_field_picking_picking_type']['field_picking_picking_type']['field']['handler'] = 'custom_for_picking_type';
  $data['field_data_field_combined_nid']['field_combined_nid']['field']['handler'] = 'custom_for_combine_nid';
}

/**
 * Implements hook_views_data().
 */
function change_feeds_import_views_data() {
  $data = array();
  // Add Example group
  $data['ccfot']['table']['group'] = t('ccfot');
  $data['ccfot']['table']['join'] = array(
    // #global is a special flag which let's a table appear all the time.
    '#global' => array(),
  );
  // Add custom field
  $data['ccfot']['Order_Basepack'] = array(
    'title' => t('Order Number - Basepack Code'),
    'help' => t('Provides a custom field.'),
    'field' => array(
      'handler' => 'order_basepack_handler_custom_field',
    ),
  );

  $data['custom_billing_data']['table']['group'] = t('Custom Billing Data');

  $data['custom_billing_data']['table']['base'] = array(
    'title' => t('Custom Billing Data'),
    'help' => t('Contains records we want exposed to Views.'),
  );

  $data['custom_billing_data']['id'] = array(
    'title' => t('ID'),
    'help' => t('The record ID.'),
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


  $data['custom_billing_data']['vehicle'] = array(
    'title' => t('vehicle'),
    'help' => t('The record vehicle.'),
    'field' => array(
      'handler' => 'views_handler_field',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );


  $data['custom_billing_data']['loading_date'] = array(
    'title' => t('loading_date'),
    'help' => t('The record loading_date.'),
    'field' => array(
      'handler' => 'views_handler_field',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );


  $data['custom_billing_data']['party_code'] = array(
    'title' => t('party_code'),
    'help' => t('The record party_code.'),
    'field' => array(
      'handler' => 'views_handler_field',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );




  $data['custom_billing_data']['party_name'] = array(
    'title' => t('party_name'),
    'help' => t('The record party_name.'),
    'field' => array(
      'handler' => 'views_handler_field',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  $data['custom_billing_data']['bill_number'] = array(
    'title' => t('bill_number'),
    'help' => t('The record bill_number.'),
    'field' => array(
      'handler' => 'views_handler_field',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );


  $data['custom_billing_data']['bill_date'] = array(
    'title' => t('bill_date'),
    'help' => t('The record bill_date.'),
    'field' => array(
      'handler' => 'views_handler_field',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );


  $data['custom_billing_data']['product_name'] = array(
    'title' => t('product_name'),
    'help' => t('The record product_name.'),
    'field' => array(
      'handler' => 'views_handler_field',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );


  $data['custom_billing_data']['sku7_code'] = array(
    'title' => t('sku7_code'),
    'help' => t('The record sku7_code.'),
    'field' => array(
      'handler' => 'views_handler_field',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );


  $data['custom_billing_data']['mrp'] = array(
    'title' => t('mrp'),
    'help' => t('The record mrp.'),
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


  $data['custom_billing_data']['tur'] = array(
    'title' => t('tur'),
    'help' => t('The record tur.'),
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


  $data['custom_billing_data']['Sum_of_TOTAL_SALES_QTY'] = array(
    'title' => t('Sum_of_TOTAL_SALES_QTY'),
    'help' => t('The record Sum_of_TOTAL_SALES_QTY.'),
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


  $data['custom_billing_data']['Sum_of_TOTAL_SALES_RET_QTY'] = array(
    'title' => t('Sum_of_TOTAL_SALES_RET_QTY'),
    'help' => t('The record Sum_of_TOTAL_SALES_RET_QTY.'),
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


  $data['custom_billing_data']['Sum_of_NET_SALES_QTY'] = array(
    'title' => t('Sum_of_NET_SALES_QTY'),
    'help' => t('The record Sum_of_NET_SALES_QTY.'),
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


  $data['custom_billing_data']['Sum_of_SALES_FREE_QTY'] = array(
    'title' => t('Sum_of_SALES_FREE_QTY'),
    'help' => t('The record Sum_of_SALES_FREE_QTY.'),
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


  $data['custom_billing_data']['Sum_of_SALES_RET_FREE_QTY'] = array(
    'title' => t('Sum_of_SALES_RET_FREE_QTY'),
    'help' => t('The record Sum_of_SALES_RET_FREE_QTY.'),
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


  $data['custom_billing_data']['Sum_of_SALES_VALUE'] = array(
    'title' => t('Sum_of_SALES_VALUE'),
    'help' => t('The record Sum_of_SALES_VALUE.'),
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


  $data['custom_billing_data']['Sum_of_SALES_RET_VALUE'] = array(
    'title' => t('Sum_of_SALES_RET_VALUE'),
    'help' => t('The record Sum_of_SALES_RET_VALUE.'),
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


  $data['custom_billing_data']['Sum_of_NET_SALES_VALUE'] = array(
    'title' => t('Sum_of_NET_SALES_VALUE'),
    'help' => t('The record Sum_of_NET_SALES_VALUE.'),
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




  $data['custom_billing_data']['promise_date'] = array(
    'title' => t('promise_date'),
    'help' => t('The record promise_date.'),
    'field' => array(
      'handler' => 'views_handler_field',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );
  $data['custom_billing_data']['distributor_id'] = array(
    'title' => t('distributor_id'),
    'help' => t('The record distributor_id.'),
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


  $data['custom_billing_data']['created_date'] = array(
    'title' => t('created_date'),
    'help' => t('The record created_date.'),
    'field' => array(
      'handler' => 'views_handler_field',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  $data['custom_billing_data']['updated_date'] = array(
    'title' => t('updated_date'),
    'help' => t('The record updated_date.'),
    'field' => array(
      'handler' => 'views_handler_field',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  /*

    // The Deadline field
    $data['custom_billing_data']['loading_date'] = array(
    'title' => t('Loading Date'),
    'help' => t('The record Loading Date.'),
    'field' => array(
    'handler' => 'views_handler_field_date',
    ),
    'sort' => array(
    'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
    'handler' => 'views_handler_filter_date',
    ),
    );
   */
  return $data;
}
