<?php

function change_feeds_import_views_data_alter(&$data) {
  $data['field_data_field_picking_picking_type']['field_picking_picking_type']['field']['handler'] = 'custom_for_picking_type';
  $data['field_data_field_combined_nid']['field_combined_nid']['field']['handler'] = 'custom_for_combine_nid';

  $data['field_data_field_sales_party_code']['table']['group'] = t('Example table');
  $data['field_data_field_sales_party_code']['dummy_name'] = array(
    'title' => t('Example relationship'),
    'help' => t('Example help'),
    'relationship' => array(
      'title' => 'partycode',
      'base' => 'node', // Table we're joining to.
      'base field' => 'title', // Field on the joined table.
      'field' => 'field_sales_party_code_value', // Real field name on the 'foo' table.
      'handler' => 'views_handler_relationship',
      'label' => t('party code to outlet master relationship'),
      'title' => t('party code to outlet master relationship'),
      'help' => t('More information about relationship.'),
      'extra' => array(
        array('field' => 'type', 'value' => 'outlet_master'),
      ),
    ),
  );

  
  $data['field_data_field_dva_area_hook_group_name']['table']['group'] = t('Area Hook Grp Name');
  $data['field_data_field_dva_area_hook_group_name']['area_hook_grp'] = array(
    'title' => t('Area Hook Grp relationship'),
    'help' => t('Area Hook Grp help'),
    'relationship' => array(
      'title' => 'area_hook_grp',
      'base' => 'node', // Table we're joining to.
      'base field' => 'title', // Field on the joined table.
      'field' => 'field_dva_area_hook_group_name_value', // Real field name on the 'foo' table.
      'handler' => 'views_handler_relationship',
      'label' => t('Area Hook Grp Name to Area Hook Grp  master relationship'),
      'title' => t('Area Hook Grp Name to Area Hook Grp  master relationship'),
      'help' => t('More information about relationship.'),
      'extra' => array(
        array('field' => 'type', 'value' => 'area_hook_group_master'),
      ),
    ),
  );
  
  $data['field_data_field_dva_sales_day']['table']['group'] = t('Sales Day');
  $data['field_data_field_dva_sales_day']['sales_day'] = array(
    'title' => t('Sales Day'),
    'relationship' => array(
      'title' => 'Sales Day master relationship',
      'base' => 'node', // Table we're joining to.
      'base field' => 'title', // Field on the joined table.
      'field' => 'field_dva_sales_day_value', // Real field name on the 'foo' table.
      'handler' => 'views_handler_relationship',
      'label' => t('Sales day to Sales delivery day master relationship'),
      'title' => t('Sales day to Sales delivery day  master relationship'),
      'help' => t('More information about relationship.'),
      'extra' => array(
        array('field' => 'type', 'value' => 'sales_delivery_day_master'),
      ),
    ),
  );
  
  
  $data['field_data_field_hul_code']['table']['group'] = t('Hul table');
  $data['field_data_field_hul_code']['dummy_name1'] = array(
    'title' => t('Outlet to Hul relationship'),
    'help' => t('Outlet to Hul relationship'),
    'relationship' => array(
      'title' => 'hul_code',
      'base' => 'node', // Table we're joining to.
      'base field' => 'title', // Field on the joined table.
      'field' => 'field_hul_code_value', // Real field name on the 'foo' table.
      'handler' => 'views_handler_relationship',
      'label' => t('outlet master to Hul Code relationship'),
      'title' => t('outlet master to Hul Code relationship'),
      'help' => t('More information about relationship.'),
      'extra' => array(
        array('field' => 'type', 'value' => 'hul_code_master'),
      ),
    ),
  );

  $data['node']['pre_date_ageing'] = array(
    'title' => t('Previous Date Ageing'),
    'help' => t('date difference'),
    'real field' => 'nid',
    'field' => array(
      'handler' => 'change_feeds_import_handler_pre_date_ageing',
    ),
  );
  $data['node']['pen_date_ageing'] = array(
    'title' => t('Pending Date Ageing'),
    'help' => t('date difference'),
    'real field' => 'nid',
    'field' => array(
      'handler' => 'change_feeds_import_handler_pen_date_ageing',
    ),
  );

  $data['node']['pl_van_group'] = array(
    'title' => t('Pl Van Group'),
    'help' => t('Pl Van Group in Party Pack Master'),
    'real field' => 'nid',
    'field' => array(
      'handler' => 'change_feeds_import_handler_val_pan_group',
    ),
  );
}

/**
 * Field handler to translate a Status into its readable form.
 */
class change_feeds_import_handler_pre_date_ageing extends views_handler_field {

  function render($values) {
    $out_date = $values->field_field_outst_bill_date[0]['raw']['value'];
    if (!empty($out_date)) {
      $current_date = time();
      $date_ageing = floor(((($current_date - $out_date) / 60) / 60) / 24);
      if (empty($date_ageing)) {
        $date_ageing = 0;
      }
      return $date_ageing;
    }
    else {
      return '-';
    }
  }

}

/**
 * Field handler to translate a Status into its readable form.
 */
class change_feeds_import_handler_pen_date_ageing extends views_handler_field {

  function render($values) {

    $out_date = $values->custom_billing_data_bill_date;
    if (!empty($out_date)) {
      $current_date = time();
      $date_ageing = floor(((($current_date - $out_date) / 60) / 60) / 24);
      if (empty($date_ageing)) {
        $date_ageing = 0;
      }
      return $date_ageing;
    }
    else {
      return '-';
    }
  }

}

/**
 * Field handler to translate a Status into its readable form.
 */
class change_feeds_import_handler_val_pan_group extends views_handler_field {

  function render($values) {
    $res = '-';
    $channel = $values->field_field_channel[0]['raw']['value'];
    $pl_group = $values->field_field_pl_group[0]['raw']['value'];
    $query = db_select('node', 'n');
    $query->join('field_data_field_van_pl_group', 'vpg', 'vpg.entity_id = n.nid');
    $query->fields('vpg', array('field_van_pl_group_value'));
    $query->join('field_data_field_pl_group', 'pg', 'pg.entity_id = n.nid');
    $query->join('field_data_field_channel', 'c',  'c.entity_id = n.nid');
    $query->condition('pg.field_pl_group_value', $pl_group);
    $query->condition('c.field_channel_value', $channel);
    $query->condition('n.type', 'party_pack_master');
    $query->addTag('node_uid');
    $res = $query->execute()->fetchField();
    return $res;
  }

}

/**
 * Implements hook_views_data().
 */
function change_feeds_import_views_data() {

  $data = array();

  $data['views']['header_area_text_custom'] = array(
    'title' => t('Header field'),
    'help' => t('Just a field created for header purposes.'),
    'area' => array(
      'handler' => 'header_handler_custom_field',
    ),
  );

  $data['views']['footer_area_text_custom'] = array(
    'title' => t('Footer field'),
    'help' => t('Just a field created for footer purposes.'),
    'area' => array(
      'handler' => 'footer_handler_custom_field',
    ),
  );

  
  
  $data['sequence_method']['table']['group'] = t('Dispatch Sequence Method');
  $data['sequence_method']['table']['join'] = array(
    // #global is a special flag which let's a table appear all the time.
    '#global' => array(),
  );
  // Add custom field
  $data['sequence_method']['field_sequence_method'] = array(
    'title' => t('Custom Sequence Method Field'),
    'help' => t('Provides a Custom Sequence Method Field.'),
    'field' => array(
      'handler' => 'sequence_method_handler_custom_field',
    ),
  );
  
  

  $data['beat_name']['table']['group'] = t('BEAT NAME');
  $data['beat_name']['table']['join'] = array(
    // #global is a special flag which let's a table appear all the time.
    '#global' => array(),
  );
  // Add custom field
  $data['beat_name']['field_beat_name'] = array(
    'title' => t('Custom Beat Name Field'),
    'help' => t('Provides a custom field.'),
    'field' => array(
      'handler' => 'beat_name_handler_custom_field',
    ),
  );
  
  
  $data['bill_sequence']['table']['group'] = t('Bill Sequence');
  $data['bill_sequence']['table']['join'] = array(
    // #global is a special flag which let's a table appear all the time.
    '#global' => array(),
  );
  // Add custom field
  $data['bill_sequence']['field_bill_sequence'] = array(
    'title' => t('Custom Bill Sequence'),
    'help' => t('Provides a custom field.'),
    'field' => array(
      'handler' => 'custom_for_bill_sequence',
    ),
  );


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
      'handler' => 'views_handler_field_date',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
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
    'relationship' => array(
      'title' => t('Party code in outlet master'),
      'help' => t("Party code in outlet master"),
      'base' => 'node',
      'base field' => 'title',
      'handler' => 'views_handler_relationship',
      'label' => t('Party code in outlet master'),
      'extra' => array(
        array('field' => 'type', 'value' => 'outlet_master'),
      ),
    ),
  );
  $data['custom_billing_data']['BEAT_NAME'] = array(
    'title' => t('BEAT_NAME'),
    'help' => t('The record BEAT_NAME.'),
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
    'relationship' => array(
      'title' => t('Bill number in sales register'),
      'help' => t("Bill number in sales register"),
      'base' => 'node',
      'base field' => 'title',
      'handler' => 'views_handler_relationship',
      'label' => t('Bill number in sales register'),
      'extra' => array(
        array('field' => 'type', 'value' => 'sales_register_data'),
      ),
    ),
  );

  $data['custom_billing_data']['bill_date'] = array(
    'title' => t('bill_date'),
    'help' => t('The record bill_date.'),
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

  $data['custom_billing_data']['BASEPACK_CODE'] = array(
    'title' => t('BASEPACK_CODE'),
    'help' => t('The record BASEPACK_CODE.'),
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

  $data['custom_billing_data']['PRICELIST_GROUP'] = array(
    'title' => t('PRICELIST_GROUP'),
    'help' => t('The record PRICELIST_GROUP.'),
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

  $data['custom_billing_data']['PRODUCT_DIVISION'] = array(
    'title' => t('PRODUCT_DIVISION'),
    'help' => t('The record PRODUCT_DIVISION.'),
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

  $data['custom_billing_data']['PRODUCT_CATEGORY'] = array(
    'title' => t('PRODUCT_CATEGORY'),
    'help' => t('The record PRODUCT_CATEGORY.'),
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

  $data['custom_billing_data']['PRODUCT_SUB_CATEGORY'] = array(
    'title' => t('PRODUCT_SUB_CATEGORY'),
    'help' => t('The record PRODUCT_SUB_CATEGORY.'),
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


//  $data['custom_billing_data']['Sum_of_TOTAL_SALES_RET_QTY'] = array(
//    'title' => t('Sum_of_TOTAL_SALES_RET_QTY'),
//    'help' => t('The record Sum_of_TOTAL_SALES_RET_QTY.'),
//    'field' => array(
//      'handler' => 'views_handler_field_numeric',
//    ),
//    'sort' => array(
//      'handler' => 'views_handler_sort',
//    ),
//    'filter' => array(
//      'handler' => 'views_handler_filter_numeric',
//    ),
//  );


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


//  $data['custom_billing_data']['Sum_of_SALES_RET_FREE_QTY'] = array(
//    'title' => t('Sum_of_SALES_RET_FREE_QTY'),
//    'help' => t('The record Sum_of_SALES_RET_FREE_QTY.'),
//    'field' => array(
//      'handler' => 'views_handler_field_numeric',
//    ),
//    'sort' => array(
//      'handler' => 'views_handler_sort',
//    ),
//    'filter' => array(
//      'handler' => 'views_handler_filter_numeric',
//    ),
//  );
//  $data['custom_billing_data']['Sum_of_SALES_RET_VALUE'] = array(
//    'title' => t('Sum_of_SALES_RET_VALUE'),
//    'help' => t('The record Sum_of_SALES_RET_VALUE.'),
//    'field' => array(
//      'handler' => 'views_handler_field_numeric',
//    ),
//    'sort' => array(
//      'handler' => 'views_handler_sort',
//    ),
//    'filter' => array(
//      'handler' => 'views_handler_filter_numeric',
//    ),
//  );


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

  $data['custom_billing_data']['Sum_of_NET_SALES_CASES'] = array(
    'title' => t('Sum_of_NET_SALES_CASES'),
    'help' => t('The record Sum_of_NET_SALES_CASES.'),
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

  $data['custom_billing_data']['Sum_of_NET_SALES_KGS'] = array(
    'title' => t('Sum_of_NET_SALES_KGS'),
    'help' => t('The record Sum_of_NET_SALES_KGS.'),
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

  $data['custom_billing_data']['Sum_of_SCHEME_DISCOUNT'] = array(
    'title' => t('Sum_of_SCHEME_DISCOUNT'),
    'help' => t('The record Sum_of_SCHEME_DISCOUNT.'),
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

  $data['custom_billing_data']['Sum_of_OTHER_DISCOUNT'] = array(
    'title' => t('Sum_of_OTHER_DISCOUNT'),
    'help' => t('The record Sum_of_OTHER_DISCOUNT.'),
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

  $data['custom_billing_data']['Sum_of_TAX_PERCENTAGE'] = array(
    'title' => t('Sum_of_TAX_PERCENTAGE'),
    'help' => t('The record Sum_of_TAX_PERCENTAGE.'),
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
      'handler' => 'views_handler_field_date',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
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
      'handler' => 'views_handler_field_date',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  $data['custom_billing_data']['updated_date'] = array(
    'title' => t('updated_date'),
    'help' => t('The record updated_date.'),
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
  $data['custom_billing_data']['bill_value'] = array(
    'title' => t('Custom bill_value'),
    'help' => t('Custom bill_value'),
    'real field' => 'id',
    'field' => array(
      'handler' => 'custom_billing_data_bill_value',
      'click sortable' => FALSE,
    ),
  );

  $data['van_cld_data_table']['table']['group'] = t('VAN CLD GROUP');
  $data['van_cld_data_table']['table']['base'] = array(
    'title' => t('VAN CLD Data'),
    'help' => t('Contains records from van_cld_data custom table.'),
  );



  $data['van_cld_data_table']['vehicle'] = array(
    'title' => t('Vehicle'),
    'help' => t('Vehicle'),
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

  $data['van_cld_data_table']['loading_date'] = array(
    'title' => t('Loading Date'),
    'help' => t('Loading Date'),
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


  $data['van_cld_data_table']['sku7_code'] = array(
    'title' => t('SKU7 CODE'),
    'help' => t('SKU7 CODE'),
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


  $data['van_cld_data_table']['product_name'] = array(
    'title' => t('Product Name'),
    'help' => t('Product Name'),
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


  $data['van_cld_data_table']['cld'] = array(
    'title' => t('CLD'),
    'help' => t('CLD'),
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

  $data['van_cld_data_table']['mrp'] = array(
    'title' => t('MRP'),
    'help' => t('MRP'),
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

  $data['van_cld_data_table']['pricelist_group'] = array(
    'title' => t('Pricelist Group'),
    'help' => t('Pricelist Group'),
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

  $base_table = 'custom_billing_data';
  $data[$base_table]['date_filter'] = array(
    'group' => t('Date'),
    'title' => t('Date (!base_table)', array('!base_table' => $base_table)),
    'help' => t('Filter any Views !base_table date field.', array('!base_table' => $base_table)),
    'filter' => array(
      'handler' => 'date_views_filter_handler',
      'empty field name' => t('Undated'),
      'is date' => TRUE,
    //'skip base' => $base_table,
    ),
  );
  $base_table2 = "van_cld_data_table";
  $data[$base_table2]['date_filter'] = array(
    'group' => t('Date'),
    'title' => t('Date (!base_table)', array('!base_table' => $base_table2)),
    'help' => t('Filter any Views !base_table date field.', array('!base_table' => $base_table2)),
    'filter' => array(
      'handler' => 'date_views_filter_handler',
      'empty field name' => t('Undated'),
      'is date' => TRUE,
    //'skip base' => $base_table,
    ),
  );
  
  
  ############ VAN-PL GRoup in Oulet master
  $data['node']['van_pl_group'] = array(
    'title' => t('van pl group'),
    'help' => t('van pl group From outlet master'),
    'real field' => 'nid',
    'field' => array(
      'handler' => 'views_handler_field_van_pl_group',
      'click sortable' => FALSE,
     ),
  );
  $data['node']['outlets_per_van'] = array(
    'title' => t('outlets per van'),
    'help' => t('outlets per van From van pl group'),
    'real field' => 'nid',
    'field' => array(
      'handler' => 'views_handler_field_outlets_per_van',
      'click sortable' => FALSE,
     ),
  );

  return $data;
}
