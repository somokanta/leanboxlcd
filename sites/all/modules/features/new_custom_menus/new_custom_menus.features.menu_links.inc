<?php
/**
 * @file
 * new_custom_menus.features.menu_links.inc
 */

/**
 * Implements hook_menu_default_menu_links().
 */
function new_custom_menus_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: management_new-custom-menus:admin/structure/menu/manage/menu-new-custom-menus.
  $menu_links['management_new-custom-menus:admin/structure/menu/manage/menu-new-custom-menus'] = array(
    'menu_name' => 'management',
    'link_path' => 'admin/structure/menu/manage/menu-new-custom-menus',
    'router_path' => 'admin/structure/menu/manage/%',
    'link_title' => 'New Custom Menus',
    'options' => array(
      'alter' => TRUE,
      'identifier' => 'management_new-custom-menus:admin/structure/menu/manage/menu-new-custom-menus',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 0,
    'parent_identifier' => 'management_menus:admin/structure/menu',
  );
  // Exported menu link: menu-new-custom-menus_-impact-return-analysis-count-and-value-central:central-impact-return-analys.
  $menu_links['menu-new-custom-menus_-impact-return-analysis-count-and-value-central:central-impact-return-analys'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'central-impact-return-analys',
    'router_path' => 'central-impact-return-analys',
    'link_title' => ' Impact Return Analysis Count and Value (Central)',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_-impact-return-analysis-count-and-value-central:central-impact-return-analys',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_output:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_address-confirmation:address/confirm.
  $menu_links['menu-new-custom-menus_address-confirmation:address/confirm'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'address/confirm',
    'router_path' => 'address/confirm',
    'link_title' => 'Address Confirmation',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_address-confirmation:address/confirm',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_input:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_agent-master:<nolink>.
  $menu_links['menu-new-custom-menus_agent-master:<nolink>'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => '<nolink>',
    'router_path' => '<nolink>',
    'link_title' => 'Agent Master',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_agent-master:<nolink>',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 1,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_general:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_area-hook-group-confirmation:area-confirm.
  $menu_links['menu-new-custom-menus_area-hook-group-confirmation:area-confirm'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'area-confirm',
    'router_path' => 'area-confirm',
    'link_title' => 'Area Hook Group Confirmation',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_area-hook-group-confirmation:area-confirm',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_input:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_area-hook-group-master:<nolink>.
  $menu_links['menu-new-custom-menus_area-hook-group-master:<nolink>'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => '<nolink>',
    'router_path' => '<nolink>',
    'link_title' => 'Area Hook Group Master',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_area-hook-group-master:<nolink>',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 1,
    'weight' => -8,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_routing:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_area-hook-master:<nolink>.
  $menu_links['menu-new-custom-menus_area-hook-master:<nolink>'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => '<nolink>',
    'router_path' => '<nolink>',
    'link_title' => 'Area Hook Master',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_area-hook-master:<nolink>',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 1,
    'weight' => -10,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_routing:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_assignment-history:assignment-history.
  $menu_links['menu-new-custom-menus_assignment-history:assignment-history'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'assignment-history',
    'router_path' => 'assignment-history',
    'link_title' => 'Assignment History',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_assignment-history:assignment-history',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_output:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_bill-pendency:leanbox-chart-details/bill_pendency.
  $menu_links['menu-new-custom-menus_bill-pendency:leanbox-chart-details/bill_pendency'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'leanbox-chart-details/bill_pendency',
    'router_path' => 'leanbox-chart-details/%',
    'link_title' => 'Bill Pendency',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_bill-pendency:leanbox-chart-details/bill_pendency',
    ),
    'module' => 'menu',
    'hidden' => 1,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_progress:dashboard/Progress',
  );
  // Exported menu link: menu-new-custom-menus_bill-vs-trip-central:central-billing-report.
  $menu_links['menu-new-custom-menus_bill-vs-trip-central:central-billing-report'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'central-billing-report',
    'router_path' => 'central-billing-report',
    'link_title' => 'Bill Vs Trip (Central)',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_bill-vs-trip-central:central-billing-report',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_output:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_bill-vs-trip-report:billing-report.
  $menu_links['menu-new-custom-menus_bill-vs-trip-report:billing-report'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'billing-report',
    'router_path' => 'billing-report',
    'link_title' => 'Bill Vs Trip Report',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_bill-vs-trip-report:billing-report',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_output:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_billing-data:<nolink>.
  $menu_links['menu-new-custom-menus_billing-data:<nolink>'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => '<nolink>',
    'router_path' => '<nolink>',
    'link_title' => 'Billing Data',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_billing-data:<nolink>',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_input:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_bills-per-person-daywise:leanbox-chart-details/bills_per_person_trends.
  $menu_links['menu-new-custom-menus_bills-per-person-daywise:leanbox-chart-details/bills_per_person_trends'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'leanbox-chart-details/bills_per_person_trends',
    'router_path' => 'leanbox-chart-details/%',
    'link_title' => 'Bills Per Person DayWise',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_bills-per-person-daywise:leanbox-chart-details/bills_per_person_trends',
    ),
    'module' => 'menu',
    'hidden' => 1,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_productivity:dashboard/Productivity',
  );
  // Exported menu link: menu-new-custom-menus_bills-per-person:leanbox-chart-details/bills_per_person.
  $menu_links['menu-new-custom-menus_bills-per-person:leanbox-chart-details/bills_per_person'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'leanbox-chart-details/bills_per_person',
    'router_path' => 'leanbox-chart-details/%',
    'link_title' => 'Bills Per Person',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_bills-per-person:leanbox-chart-details/bills_per_person',
    ),
    'module' => 'menu',
    'hidden' => 1,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_productivity:dashboard/Productivity',
  );
  // Exported menu link: menu-new-custom-menus_bills-per-van-central:leanbox-chart-details/central_dispatch_daywise.
  $menu_links['menu-new-custom-menus_bills-per-van-central:leanbox-chart-details/central_dispatch_daywise'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'leanbox-chart-details/central_dispatch_daywise',
    'router_path' => 'leanbox-chart-details/%',
    'link_title' => 'Bills Per Van (Central)',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_bills-per-van-central:leanbox-chart-details/central_dispatch_daywise',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_output:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_bills-per-van-daywise:leanbox-chart-details/bills_per_van_trends.
  $menu_links['menu-new-custom-menus_bills-per-van-daywise:leanbox-chart-details/bills_per_van_trends'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'leanbox-chart-details/bills_per_van_trends',
    'router_path' => 'leanbox-chart-details/%',
    'link_title' => 'Bills Per Van DayWise',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_bills-per-van-daywise:leanbox-chart-details/bills_per_van_trends',
    ),
    'module' => 'menu',
    'hidden' => 1,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_productivity:dashboard/Productivity',
  );
  // Exported menu link: menu-new-custom-menus_bills-per-van:leanbox-chart-details/bills_per_van.
  $menu_links['menu-new-custom-menus_bills-per-van:leanbox-chart-details/bills_per_van'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'leanbox-chart-details/bills_per_van',
    'router_path' => 'leanbox-chart-details/%',
    'link_title' => 'Bills Per Van',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_bills-per-van:leanbox-chart-details/bills_per_van',
    ),
    'module' => 'menu',
    'hidden' => 1,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_productivity:dashboard/Productivity',
  );
  // Exported menu link: menu-new-custom-menus_bounced-check-declaration:upload-collection-bill/bounced.
  $menu_links['menu-new-custom-menus_bounced-check-declaration:upload-collection-bill/bounced'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'upload-collection-bill/bounced',
    'router_path' => 'upload-collection-bill/bounced',
    'link_title' => 'Bounced Check declaration',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_bounced-check-declaration:upload-collection-bill/bounced',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 1,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_input:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_bounced-cheque-upload:bounced-cheques-upload.
  $menu_links['menu-new-custom-menus_bounced-cheque-upload:bounced-cheques-upload'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'bounced-cheques-upload',
    'router_path' => 'bounced-cheques-upload',
    'link_title' => 'Bounced Cheque Upload',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_bounced-cheque-upload:bounced-cheques-upload',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_input:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_bounced-cheques-assignment:bounced-cheques-assignment.
  $menu_links['menu-new-custom-menus_bounced-cheques-assignment:bounced-cheques-assignment'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'bounced-cheques-assignment',
    'router_path' => 'bounced-cheques-assignment',
    'link_title' => 'Bounced Cheques Assignment',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_bounced-cheques-assignment:bounced-cheques-assignment',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_input:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_central-delivery-data:central-sales-delivery-table.
  $menu_links['menu-new-custom-menus_central-delivery-data:central-sales-delivery-table'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'central-sales-delivery-table',
    'router_path' => 'central-sales-delivery-table',
    'link_title' => 'Central delivery data',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_central-delivery-data:central-sales-delivery-table',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_output:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_central-report-authorisation:central-rs-configuration.
  $menu_links['menu-new-custom-menus_central-report-authorisation:central-rs-configuration'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'central-rs-configuration',
    'router_path' => 'central-rs-configuration',
    'link_title' => 'Central report Authorisation',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_central-report-authorisation:central-rs-configuration',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
  );
  // Exported menu link: menu-new-custom-menus_centrally-managed:<nolink>.
  $menu_links['menu-new-custom-menus_centrally-managed:<nolink>'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => '<nolink>',
    'router_path' => '<nolink>',
    'link_title' => 'Centrally Managed',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_centrally-managed:<nolink>',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 1,
    'weight' => -49,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_masters:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_channel-master:<nolink>.
  $menu_links['menu-new-custom-menus_channel-master:<nolink>'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => '<nolink>',
    'router_path' => '<nolink>',
    'link_title' => 'Channel Master',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_channel-master:<nolink>',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 1,
    'weight' => -50,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_centrally-managed:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_collection-history:collection-history.
  $menu_links['menu-new-custom-menus_collection-history:collection-history'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'collection-history',
    'router_path' => 'collection-history',
    'link_title' => 'Collection History',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_collection-history:collection-history',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_output:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_collection-update:payment-collection.
  $menu_links['menu-new-custom-menus_collection-update:payment-collection'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'payment-collection',
    'router_path' => 'payment-collection',
    'link_title' => 'Collection Update',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_collection-update:payment-collection',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_input:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_collection:<nolink>.
  $menu_links['menu-new-custom-menus_collection:<nolink>'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => '<nolink>',
    'router_path' => '<nolink>',
    'link_title' => 'Collection',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_collection:<nolink>',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 1,
    'weight' => 0,
    'customized' => 1,
  );
  // Exported menu link: menu-new-custom-menus_dashboard:<nolink>.
  $menu_links['menu-new-custom-menus_dashboard:<nolink>'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => '<nolink>',
    'router_path' => '<nolink>',
    'link_title' => 'Dashboard',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_dashboard:<nolink>',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 1,
    'weight' => -46,
    'customized' => 1,
  );
  // Exported menu link: menu-new-custom-menus_day-van-pl-group-area-grouping:<nolink>.
  $menu_links['menu-new-custom-menus_day-van-pl-group-area-grouping:<nolink>'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => '<nolink>',
    'router_path' => '<nolink>',
    'link_title' => 'Day-Van Pl Group Area Grouping',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_day-van-pl-group-area-grouping:<nolink>',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 1,
    'weight' => -6,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_routing:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_day-wise-bill-count-and-value:email/weekly-billing-report.
  $menu_links['menu-new-custom-menus_day-wise-bill-count-and-value:email/weekly-billing-report'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'email/weekly-billing-report',
    'router_path' => 'email/weekly-billing-report',
    'link_title' => 'Day Wise Bill Count And Value',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_day-wise-bill-count-and-value:email/weekly-billing-report',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_general-report:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_delete-billing-data:delete-billing-data.
  $menu_links['menu-new-custom-menus_delete-billing-data:delete-billing-data'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'delete-billing-data',
    'router_path' => 'delete-billing-data',
    'link_title' => 'Delete Billing Data',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_delete-billing-data:delete-billing-data',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
  );
  // Exported menu link: menu-new-custom-menus_delivery-data:<nolink>.
  $menu_links['menu-new-custom-menus_delivery-data:<nolink>'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => '<nolink>',
    'router_path' => '<nolink>',
    'link_title' => 'Delivery Data',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_delivery-data:<nolink>',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 1,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_input:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_dispatch-on-time-adherence:leanbox-chart-details/dispatch_ota.
  $menu_links['menu-new-custom-menus_dispatch-on-time-adherence:leanbox-chart-details/dispatch_ota'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'leanbox-chart-details/dispatch_ota',
    'router_path' => 'leanbox-chart-details/%',
    'link_title' => 'Dispatch On Time Adherence',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_dispatch-on-time-adherence:leanbox-chart-details/dispatch_ota',
    ),
    'module' => 'menu',
    'hidden' => 1,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_on-time-adherence:dashboard/ota',
  );
  // Exported menu link: menu-new-custom-menus_dispatch-planning:admin/dispatch-planning.
  $menu_links['menu-new-custom-menus_dispatch-planning:admin/dispatch-planning'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'admin/dispatch-planning',
    'router_path' => 'admin/dispatch-planning',
    'link_title' => 'Dispatch Planning',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_dispatch-planning:admin/dispatch-planning',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_input:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_dispatch:<nolink>.
  $menu_links['menu-new-custom-menus_dispatch:<nolink>'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => '<nolink>',
    'router_path' => '<nolink>',
    'link_title' => 'Dispatch',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_dispatch:<nolink>',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 1,
    'weight' => -48,
    'customized' => 1,
  );
  // Exported menu link: menu-new-custom-menus_download-central:central-outlet-master-report/view.
  $menu_links['menu-new-custom-menus_download-central:central-outlet-master-report/view'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'central-outlet-master-report/view',
    'router_path' => 'central-outlet-master-report/view',
    'link_title' => 'Download (Central)',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_download-central:central-outlet-master-report/view',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_outlet-master:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_download:area-hook-group-master-report.
  $menu_links['menu-new-custom-menus_download:area-hook-group-master-report'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'area-hook-group-master-report',
    'router_path' => 'area-hook-group-master-report',
    'link_title' => 'Download',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_download:area-hook-group-master-report',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -49,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_area-hook-group-master:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_download:area-hook-master-report.
  $menu_links['menu-new-custom-menus_download:area-hook-master-report'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'area-hook-master-report',
    'router_path' => 'area-hook-master-report',
    'link_title' => 'Download',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_download:area-hook-master-report',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_area-hook-master:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_download:channle-master-report.
  $menu_links['menu-new-custom-menus_download:channle-master-report'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'channle-master-report',
    'router_path' => 'channle-master-report',
    'link_title' => 'Download',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_download:channle-master-report',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -49,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_channel-master:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_download:custom-billing-report.
  $menu_links['menu-new-custom-menus_download:custom-billing-report'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'custom-billing-report',
    'router_path' => 'custom-billing-report',
    'link_title' => 'Download',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_download:custom-billing-report',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_billing-data:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_download:day-van-pl-group-report.
  $menu_links['menu-new-custom-menus_download:day-van-pl-group-report'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'day-van-pl-group-report',
    'router_path' => 'day-van-pl-group-report',
    'link_title' => 'Download',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_download:day-van-pl-group-report',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -49,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_day-van-pl-group-area-grouping:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_download:egir-report.
  $menu_links['menu-new-custom-menus_download:egir-report'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'egir-report',
    'router_path' => 'egir-report',
    'link_title' => 'Download',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_download:egir-report',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_egir-data:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_download:hht-master.
  $menu_links['menu-new-custom-menus_download:hht-master'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'hht-master',
    'router_path' => 'hht-master',
    'link_title' => 'Download',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_download:hht-master',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -49,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_hht-master:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_download:hul-code-master/view.
  $menu_links['menu-new-custom-menus_download:hul-code-master/view'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'hul-code-master/view',
    'router_path' => 'hul-code-master/view',
    'link_title' => 'Download',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_download:hul-code-master/view',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_gps-master:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_download:outlet-master-report.
  $menu_links['menu-new-custom-menus_download:outlet-master-report'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'outlet-master-report',
    'router_path' => 'outlet-master-report',
    'link_title' => 'Download',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_download:outlet-master-report',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_outlet-master:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_download:party-pack-master.
  $menu_links['menu-new-custom-menus_download:party-pack-master'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'party-pack-master',
    'router_path' => 'party-pack-master',
    'link_title' => 'Download',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_download:party-pack-master',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -49,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_van-plg-master:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_download:pdd-master.
  $menu_links['menu-new-custom-menus_download:pdd-master'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'pdd-master',
    'router_path' => 'pdd-master',
    'link_title' => 'Download',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_download:pdd-master',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_pdd-master:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_download:pl-group-master-report.
  $menu_links['menu-new-custom-menus_download:pl-group-master-report'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'pl-group-master-report',
    'router_path' => 'pl-group-master-report',
    'link_title' => 'Download',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_download:pl-group-master-report',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_plgroup-master:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_download:pricelist-group-master/view.
  $menu_links['menu-new-custom-menus_download:pricelist-group-master/view'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'pricelist-group-master/view',
    'router_path' => 'pricelist-group-master/view',
    'link_title' => 'Download',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_download:pricelist-group-master/view',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_pricelist-group-master:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_download:product-division-master/view.
  $menu_links['menu-new-custom-menus_download:product-division-master/view'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'product-division-master/view',
    'router_path' => 'product-division-master/view',
    'link_title' => 'Download',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_download:product-division-master/view',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_product-division-master:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_download:resource-master-report/view.
  $menu_links['menu-new-custom-menus_download:resource-master-report/view'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'resource-master-report/view',
    'router_path' => 'resource-master-report/view',
    'link_title' => 'Download',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_download:resource-master-report/view',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_agent-master:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_download:resource-type/view.
  $menu_links['menu-new-custom-menus_download:resource-type/view'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'resource-type/view',
    'router_path' => 'resource-type/view',
    'link_title' => 'Download',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_download:resource-type/view',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_resource-type-master:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_download:sales-delivery-day-report.
  $menu_links['menu-new-custom-menus_download:sales-delivery-day-report'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'sales-delivery-day-report',
    'router_path' => 'sales-delivery-day-report',
    'link_title' => 'Download',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_download:sales-delivery-day-report',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -1,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_sales-delivery-day-master:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_download:sales-delivery-table.
  $menu_links['menu-new-custom-menus_download:sales-delivery-table'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'sales-delivery-table',
    'router_path' => 'sales-delivery-table',
    'link_title' => 'Download',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_download:sales-delivery-table',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_delivery-data:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_download:sales-register-data.
  $menu_links['menu-new-custom-menus_download:sales-register-data'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'sales-register-data',
    'router_path' => 'sales-register-data',
    'link_title' => 'Download',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_download:sales-register-data',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_sales-register-data:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_download:sku7-code-master-report.
  $menu_links['menu-new-custom-menus_download:sku7-code-master-report'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'sku7-code-master-report',
    'router_path' => 'sku7-code-master-report',
    'link_title' => 'Download',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_download:sku7-code-master-report',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -49,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_sku7code-master:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_download:van-pl-group-master-report.
  $menu_links['menu-new-custom-menus_download:van-pl-group-master-report'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'van-pl-group-master-report',
    'router_path' => 'van-pl-group-master-report',
    'link_title' => 'Download',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_download:van-pl-group-master-report',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_van-stop-master:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_download:vehicle-master/view.
  $menu_links['menu-new-custom-menus_download:vehicle-master/view'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'vehicle-master/view',
    'router_path' => 'vehicle-master/view',
    'link_title' => 'Download',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_download:vehicle-master/view',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_vehicle-master:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_drs-printout---2:drs-report2.
  $menu_links['menu-new-custom-menus_drs-printout---2:drs-report2'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'drs-report2',
    'router_path' => 'drs-report2',
    'link_title' => 'DRS PrintOut - 2',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_drs-printout---2:drs-report2',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_output:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_drs-report:drs-report.
  $menu_links['menu-new-custom-menus_drs-report:drs-report'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'drs-report',
    'router_path' => 'drs-report',
    'link_title' => 'DRS Report',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_drs-report:drs-report',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_output:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_egir-data:<nolink>.
  $menu_links['menu-new-custom-menus_egir-data:<nolink>'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => '<nolink>',
    'router_path' => '<nolink>',
    'link_title' => 'EGIR Data',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_egir-data:<nolink>',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 1,
    'weight' => -4,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_input:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_general-report:<nolink>.
  $menu_links['menu-new-custom-menus_general-report:<nolink>'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => '<nolink>',
    'router_path' => '<nolink>',
    'link_title' => 'General Report',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_general-report:<nolink>',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 0,
    'weight' => 7,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_centrally-managed:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_general:<nolink>.
  $menu_links['menu-new-custom-menus_general:<nolink>'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => '<nolink>',
    'router_path' => '<nolink>',
    'link_title' => 'General',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_general:<nolink>',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 1,
    'weight' => -50,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_masters:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_gps-master:<nolink>.
  $menu_links['menu-new-custom-menus_gps-master:<nolink>'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => '<nolink>',
    'router_path' => '<nolink>',
    'link_title' => 'GPS Master',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_gps-master:<nolink>',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 1,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_general:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_hht-master:<nolink>.
  $menu_links['menu-new-custom-menus_hht-master:<nolink>'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => '<nolink>',
    'router_path' => '<nolink>',
    'link_title' => 'HHT Master',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_hht-master:<nolink>',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 1,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_general:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_impact-delivery-report-2:impact-delivery-report-two.
  $menu_links['menu-new-custom-menus_impact-delivery-report-2:impact-delivery-report-two'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'impact-delivery-report-two',
    'router_path' => 'impact-delivery-report-two',
    'link_title' => 'Impact Delivery Report 2',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_impact-delivery-report-2:impact-delivery-report-two',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_output:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_incremental-outlet-addition:incremental-outlet.
  $menu_links['menu-new-custom-menus_incremental-outlet-addition:incremental-outlet'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'incremental-outlet',
    'router_path' => 'incremental-outlet',
    'link_title' => 'Incremental Outlet Addition',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_incremental-outlet-addition:incremental-outlet',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_input:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_input:<nolink>.
  $menu_links['menu-new-custom-menus_input:<nolink>'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => '<nolink>',
    'router_path' => '<nolink>',
    'link_title' => 'Input',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_input:<nolink>',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 1,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_warehousing:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_masters:<nolink>.
  $menu_links['menu-new-custom-menus_masters:<nolink>'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => '<nolink>',
    'router_path' => '<nolink>',
    'link_title' => 'Masters',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_masters:<nolink>',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 1,
    'weight' => -50,
    'customized' => 1,
  );
  // Exported menu link: menu-new-custom-menus_metrics:<nolink>.
  $menu_links['menu-new-custom-menus_metrics:<nolink>'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => '<nolink>',
    'router_path' => '<nolink>',
    'link_title' => 'Metrics',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_metrics:<nolink>',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 1,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_dashboard:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_metrics:dashboard/Metrics.
  $menu_links['menu-new-custom-menus_metrics:dashboard/Metrics'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'dashboard/Metrics',
    'router_path' => 'dashboard/Metrics',
    'link_title' => 'Metrics',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_metrics:dashboard/Metrics',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 1,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_dashboard:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_on-time-adherence:<nolink>.
  $menu_links['menu-new-custom-menus_on-time-adherence:<nolink>'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => '<nolink>',
    'router_path' => '<nolink>',
    'link_title' => 'On-Time Adherence',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_on-time-adherence:<nolink>',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 1,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_dashboard:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_on-time-adherence:dashboard/ota.
  $menu_links['menu-new-custom-menus_on-time-adherence:dashboard/ota'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'dashboard/ota',
    'router_path' => 'dashboard/ota',
    'link_title' => 'On-Time Adherence',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_on-time-adherence:dashboard/ota',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 1,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_dashboard:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_original-outlet-sequence:<nolink>.
  $menu_links['menu-new-custom-menus_original-outlet-sequence:<nolink>'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => '<nolink>',
    'router_path' => '<nolink>',
    'link_title' => 'Original Outlet Sequence',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_original-outlet-sequence:<nolink>',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 0,
    'weight' => -5,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_routing:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_original-outlet-sequencing-api:area-confirm-outlet-sequence.
  $menu_links['menu-new-custom-menus_original-outlet-sequencing-api:area-confirm-outlet-sequence'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'area-confirm-outlet-sequence',
    'router_path' => 'area-confirm-outlet-sequence',
    'link_title' => 'Original Outlet Sequencing-API',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_original-outlet-sequencing-api:area-confirm-outlet-sequence',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_routing:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_otif-day-wise:leanbox-chart-details/otif_trends.
  $menu_links['menu-new-custom-menus_otif-day-wise:leanbox-chart-details/otif_trends'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'leanbox-chart-details/otif_trends',
    'router_path' => 'leanbox-chart-details/%',
    'link_title' => 'OTIF Day Wise',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_otif-day-wise:leanbox-chart-details/otif_trends',
    ),
    'module' => 'menu',
    'hidden' => 1,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 1,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_metrics:dashboard/Metrics',
  );
  // Exported menu link: menu-new-custom-menus_otif-value:leanbox-chart-details/otif.
  $menu_links['menu-new-custom-menus_otif-value:leanbox-chart-details/otif'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'leanbox-chart-details/otif',
    'router_path' => 'leanbox-chart-details/%',
    'link_title' => 'OTIF Value',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_otif-value:leanbox-chart-details/otif',
    ),
    'module' => 'menu',
    'hidden' => 1,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 1,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_metrics:dashboard/Metrics',
  );
  // Exported menu link: menu-new-custom-menus_outlet-area-hook:<nolink>.
  $menu_links['menu-new-custom-menus_outlet-area-hook:<nolink>'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => '<nolink>',
    'router_path' => '<nolink>',
    'link_title' => 'Outlet Area Hook',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_outlet-area-hook:<nolink>',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 1,
    'weight' => -9,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_routing:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_outlet-master:<nolink>.
  $menu_links['menu-new-custom-menus_outlet-master:<nolink>'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => '<nolink>',
    'router_path' => '<nolink>',
    'link_title' => 'Outlet Master',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_outlet-master:<nolink>',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 1,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_general:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_output:<nolink>.
  $menu_links['menu-new-custom-menus_output:<nolink>'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => '<nolink>',
    'router_path' => '<nolink>',
    'link_title' => 'Output',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_output:<nolink>',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 1,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_warehousing:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_party-packing-central:central-party-packing-report/view.
  $menu_links['menu-new-custom-menus_party-packing-central:central-party-packing-report/view'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'central-party-packing-report/view',
    'router_path' => 'central-party-packing-report/view',
    'link_title' => 'Party Packing (Central)',
    'options' => array(
      'attributes' => array(
        'title' => 'central-party-packing-report/view',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_party-packing-central:central-party-packing-report/view',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_output:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_party-packing-on-time-adherence:leanbox-chart-details/pp_ota.
  $menu_links['menu-new-custom-menus_party-packing-on-time-adherence:leanbox-chart-details/pp_ota'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'leanbox-chart-details/pp_ota',
    'router_path' => 'leanbox-chart-details/%',
    'link_title' => 'Party Packing On Time Adherence',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_party-packing-on-time-adherence:leanbox-chart-details/pp_ota',
    ),
    'module' => 'menu',
    'hidden' => 1,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_on-time-adherence:dashboard/ota',
  );
  // Exported menu link: menu-new-custom-menus_party-packing-productivity:leanbox-chart-details/party_packing_productivity.
  $menu_links['menu-new-custom-menus_party-packing-productivity:leanbox-chart-details/party_packing_productivity'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'leanbox-chart-details/party_packing_productivity',
    'router_path' => 'leanbox-chart-details/%',
    'link_title' => 'Party Packing Productivity',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_party-packing-productivity:leanbox-chart-details/party_packing_productivity',
    ),
    'module' => 'menu',
    'hidden' => 1,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_productivity:dashboard/Productivity',
  );
  // Exported menu link: menu-new-custom-menus_party-packing-van-progress:leanbox-chart-details/party_packing.
  $menu_links['menu-new-custom-menus_party-packing-van-progress:leanbox-chart-details/party_packing'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'leanbox-chart-details/party_packing',
    'router_path' => 'leanbox-chart-details/%',
    'link_title' => 'Party Packing Van Progress',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_party-packing-van-progress:leanbox-chart-details/party_packing',
    ),
    'module' => 'menu',
    'hidden' => 1,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_progress:dashboard/Progress',
  );
  // Exported menu link: menu-new-custom-menus_party-packing:party-packing-report/view.
  $menu_links['menu-new-custom-menus_party-packing:party-packing-report/view'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'party-packing-report/view',
    'router_path' => 'party-packing-report/view',
    'link_title' => 'Party Packing',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_party-packing:party-packing-report/view',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_output:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_payment-terms:<nolink>.
  $menu_links['menu-new-custom-menus_payment-terms:<nolink>'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => '<nolink>',
    'router_path' => '<nolink>',
    'link_title' => 'Payment Terms',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_payment-terms:<nolink>',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 1,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_general:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_pdd-master:<nolink>.
  $menu_links['menu-new-custom-menus_pdd-master:<nolink>'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => '<nolink>',
    'router_path' => '<nolink>',
    'link_title' => 'PDD Master',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_pdd-master:<nolink>',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 1,
    'weight' => 1,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_general:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_pick-confirm:pick-confirm-report/view.
  $menu_links['menu-new-custom-menus_pick-confirm:pick-confirm-report/view'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'pick-confirm-report/view',
    'router_path' => 'pick-confirm-report/view',
    'link_title' => 'Pick Confirm',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_pick-confirm:pick-confirm-report/view',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_output:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_picking-on-time-adherence-van-loose-:leanbox-chart-details/picking_ota.
  $menu_links['menu-new-custom-menus_picking-on-time-adherence-van-loose-:leanbox-chart-details/picking_ota'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'leanbox-chart-details/picking_ota',
    'router_path' => 'leanbox-chart-details/%',
    'link_title' => 'Picking On Time Adherence VAN Loose ',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_picking-on-time-adherence-van-loose-:leanbox-chart-details/picking_ota',
    ),
    'module' => 'menu',
    'hidden' => 1,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_on-time-adherence:dashboard/ota',
  );
  // Exported menu link: menu-new-custom-menus_picking-productivity:leanbox-chart-details/picking_productivity.
  $menu_links['menu-new-custom-menus_picking-productivity:leanbox-chart-details/picking_productivity'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'leanbox-chart-details/picking_productivity',
    'router_path' => 'leanbox-chart-details/%',
    'link_title' => 'Picking Productivity',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_picking-productivity:leanbox-chart-details/picking_productivity',
    ),
    'module' => 'menu',
    'hidden' => 1,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_productivity:dashboard/Productivity',
  );
  // Exported menu link: menu-new-custom-menus_picking-progress-task-bulk:leanbox-chart-details/picking_bulk.
  $menu_links['menu-new-custom-menus_picking-progress-task-bulk:leanbox-chart-details/picking_bulk'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'leanbox-chart-details/picking_bulk',
    'router_path' => 'leanbox-chart-details/%',
    'link_title' => 'Picking Progress Task Bulk',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_picking-progress-task-bulk:leanbox-chart-details/picking_bulk',
    ),
    'module' => 'menu',
    'hidden' => 1,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_progress:dashboard/Progress',
  );
  // Exported menu link: menu-new-custom-menus_picking-progress-task-loose:leanbox-chart-details/picking.
  $menu_links['menu-new-custom-menus_picking-progress-task-loose:leanbox-chart-details/picking'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'leanbox-chart-details/picking',
    'router_path' => 'leanbox-chart-details/%',
    'link_title' => 'Picking Progress Task Loose',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_picking-progress-task-loose:leanbox-chart-details/picking',
    ),
    'module' => 'menu',
    'hidden' => 1,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_progress:dashboard/Progress',
  );
  // Exported menu link: menu-new-custom-menus_picking:picking-report/view.
  $menu_links['menu-new-custom-menus_picking:picking-report/view'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'picking-report/view',
    'router_path' => 'picking-report/view',
    'link_title' => 'Picking',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_picking:picking-report/view',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -3,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_output:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_plgroup-master:<nolink>.
  $menu_links['menu-new-custom-menus_plgroup-master:<nolink>'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => '<nolink>',
    'router_path' => '<nolink>',
    'link_title' => 'PL_Group Master',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_plgroup-master:<nolink>',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 1,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_centrally-managed:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_pricelist-group-master:<nolink>.
  $menu_links['menu-new-custom-menus_pricelist-group-master:<nolink>'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => '<nolink>',
    'router_path' => '<nolink>',
    'link_title' => 'Pricelist Group Master',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_pricelist-group-master:<nolink>',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 1,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_centrally-managed:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_product-division-master:<nolink>.
  $menu_links['menu-new-custom-menus_product-division-master:<nolink>'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => '<nolink>',
    'router_path' => '<nolink>',
    'link_title' => 'Product Division Master',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_product-division-master:<nolink>',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 1,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_centrally-managed:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_productivity:<nolink>.
  $menu_links['menu-new-custom-menus_productivity:<nolink>'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => '<nolink>',
    'router_path' => '<nolink>',
    'link_title' => 'Productivity',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_productivity:<nolink>',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 1,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_dashboard:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_productivity:dashboard/Productivity.
  $menu_links['menu-new-custom-menus_productivity:dashboard/Productivity'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'dashboard/Productivity',
    'router_path' => 'dashboard/Productivity',
    'link_title' => 'Productivity',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_productivity:dashboard/Productivity',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 1,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_dashboard:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_progress:<nolink>.
  $menu_links['menu-new-custom-menus_progress:<nolink>'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => '<nolink>',
    'router_path' => '<nolink>',
    'link_title' => 'Progress',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_progress:<nolink>',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 1,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_dashboard:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_progress:dashboard/Progress.
  $menu_links['menu-new-custom-menus_progress:dashboard/Progress'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'dashboard/Progress',
    'router_path' => 'dashboard/Progress',
    'link_title' => 'Progress',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_progress:dashboard/Progress',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 1,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_dashboard:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_resource-type-master:<nolink>.
  $menu_links['menu-new-custom-menus_resource-type-master:<nolink>'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => '<nolink>',
    'router_path' => '<nolink>',
    'link_title' => 'Resource Type Master',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_resource-type-master:<nolink>',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 1,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_centrally-managed:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_return-value-:leanbox-chart-details/delivery.
  $menu_links['menu-new-custom-menus_return-value-:leanbox-chart-details/delivery'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'leanbox-chart-details/delivery',
    'router_path' => 'leanbox-chart-details/%',
    'link_title' => 'Return Value %',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_return-value-:leanbox-chart-details/delivery',
    ),
    'module' => 'menu',
    'hidden' => 1,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 1,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_metrics:dashboard/Metrics',
  );
  // Exported menu link: menu-new-custom-menus_routing:<nolink>.
  $menu_links['menu-new-custom-menus_routing:<nolink>'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => '<nolink>',
    'router_path' => '<nolink>',
    'link_title' => 'Routing',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_routing:<nolink>',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 1,
    'weight' => -50,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_masters:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_sales-delivery-day-master:<nolink>.
  $menu_links['menu-new-custom-menus_sales-delivery-day-master:<nolink>'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => '<nolink>',
    'router_path' => '<nolink>',
    'link_title' => 'Sales Delivery Day Master',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_sales-delivery-day-master:<nolink>',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 1,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_general:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_sales-register-data:<nolink>.
  $menu_links['menu-new-custom-menus_sales-register-data:<nolink>'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => '<nolink>',
    'router_path' => '<nolink>',
    'link_title' => 'Sales Register Data',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_sales-register-data:<nolink>',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 1,
    'weight' => -3,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_input:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_signed-bill-assignment:signed-bills-assignment.
  $menu_links['menu-new-custom-menus_signed-bill-assignment:signed-bills-assignment'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'signed-bills-assignment',
    'router_path' => 'signed-bills-assignment',
    'link_title' => 'Signed Bill Assignment',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_signed-bill-assignment:signed-bills-assignment',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_input:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_signed-bill-declaration:upload-collection-bill/signed.
  $menu_links['menu-new-custom-menus_signed-bill-declaration:upload-collection-bill/signed'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'upload-collection-bill/signed',
    'router_path' => 'upload-collection-bill/signed',
    'link_title' => 'Signed Bill Declaration',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_signed-bill-declaration:upload-collection-bill/signed',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_input:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_signed-bill-upload-:signed-bills-upload.
  $menu_links['menu-new-custom-menus_signed-bill-upload-:signed-bills-upload'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'signed-bills-upload',
    'router_path' => 'signed-bills-upload',
    'link_title' => 'Signed Bill Upload ',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_signed-bill-upload-:signed-bills-upload',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_input:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_sku7code-master:<nolink>.
  $menu_links['menu-new-custom-menus_sku7code-master:<nolink>'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => '<nolink>',
    'router_path' => '<nolink>',
    'link_title' => 'SKU7_Code Master',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_sku7code-master:<nolink>',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_general:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_static-beat-creation:delivery-beat.
  $menu_links['menu-new-custom-menus_static-beat-creation:delivery-beat'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'delivery-beat',
    'router_path' => 'delivery-beat',
    'link_title' => 'Static Beat Creation',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_static-beat-creation:delivery-beat',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_input:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_target-master:<nolink>.
  $menu_links['menu-new-custom-menus_target-master:<nolink>'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => '<nolink>',
    'router_path' => '<nolink>',
    'link_title' => 'Target Master',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_target-master:<nolink>',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 1,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_centrally-managed:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_trip-status-report:dispatch-data-report/view.
  $menu_links['menu-new-custom-menus_trip-status-report:dispatch-data-report/view'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'dispatch-data-report/view',
    'router_path' => 'dispatch-data-report/view',
    'link_title' => 'Trip Status Report',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_trip-status-report:dispatch-data-report/view',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_output:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_trip-vs-van:trip-van-data.
  $menu_links['menu-new-custom-menus_trip-vs-van:trip-van-data'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'trip-van-data',
    'router_path' => 'trip-van-data',
    'link_title' => 'Trip Vs Van',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_trip-vs-van:trip-van-data',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_output:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_unloading-central:central-unloading-form/view.
  $menu_links['menu-new-custom-menus_unloading-central:central-unloading-form/view'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'central-unloading-form/view',
    'router_path' => 'central-unloading-form/view',
    'link_title' => 'Unloading (Central)',
    'options' => array(
      'attributes' => array(
        'title' => 'central-unloading-form/view',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_unloading-central:central-unloading-form/view',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_output:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_unloading-on-time-adherence:leanbox-chart-details/unloading_ota.
  $menu_links['menu-new-custom-menus_unloading-on-time-adherence:leanbox-chart-details/unloading_ota'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'leanbox-chart-details/unloading_ota',
    'router_path' => 'leanbox-chart-details/%',
    'link_title' => 'Unloading On Time Adherence',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_unloading-on-time-adherence:leanbox-chart-details/unloading_ota',
    ),
    'module' => 'menu',
    'hidden' => 1,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_on-time-adherence:dashboard/ota',
  );
  // Exported menu link: menu-new-custom-menus_unloading-progress:leanbox-chart-details/unloading.
  $menu_links['menu-new-custom-menus_unloading-progress:leanbox-chart-details/unloading'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'leanbox-chart-details/unloading',
    'router_path' => 'leanbox-chart-details/%',
    'link_title' => 'Unloading Progress',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_unloading-progress:leanbox-chart-details/unloading',
    ),
    'module' => 'menu',
    'hidden' => 1,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_progress:dashboard/Progress',
  );
  // Exported menu link: menu-new-custom-menus_unloading:unloading-form/view.
  $menu_links['menu-new-custom-menus_unloading:unloading-form/view'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'unloading-form/view',
    'router_path' => 'unloading-form/view',
    'link_title' => 'Unloading',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_unloading:unloading-form/view',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -4,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_output:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_upload:area-hook-grp/import.
  $menu_links['menu-new-custom-menus_upload:area-hook-grp/import'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'area-hook-grp/import',
    'router_path' => 'area-hook-grp/import',
    'link_title' => 'Upload',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_upload:area-hook-grp/import',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -50,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_area-hook-group-master:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_upload:billing_data/import.
  $menu_links['menu-new-custom-menus_upload:billing_data/import'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'billing_data/import',
    'router_path' => 'billing_data/import',
    'link_title' => 'Upload',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_upload:billing_data/import',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -1,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_billing-data:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_upload:day-van-area/import.
  $menu_links['menu-new-custom-menus_upload:day-van-area/import'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'day-van-area/import',
    'router_path' => 'day-van-area/import',
    'link_title' => 'Upload',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_upload:day-van-area/import',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -50,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_day-van-pl-group-area-grouping:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_upload:import/area_hook_master.
  $menu_links['menu-new-custom-menus_upload:import/area_hook_master'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'import/area_hook_master',
    'router_path' => 'import/%',
    'link_title' => 'Upload',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_upload:import/area_hook_master',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -50,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_area-hook-master:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_upload:import/channel_master.
  $menu_links['menu-new-custom-menus_upload:import/channel_master'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'import/channel_master',
    'router_path' => 'import/%',
    'link_title' => 'Upload',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_upload:import/channel_master',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -50,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_channel-master:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_upload:import/egir.
  $menu_links['menu-new-custom-menus_upload:import/egir'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'import/egir',
    'router_path' => 'import/%',
    'link_title' => 'Upload',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_upload:import/egir',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -1,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_egir-data:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_upload:import/hht_importer.
  $menu_links['menu-new-custom-menus_upload:import/hht_importer'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'import/hht_importer',
    'router_path' => 'import/%',
    'link_title' => 'Upload',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_upload:import/hht_importer',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -50,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_hht-master:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_upload:import/hul_code_master.
  $menu_links['menu-new-custom-menus_upload:import/hul_code_master'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'import/hul_code_master',
    'router_path' => 'import/%',
    'link_title' => 'Upload',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_upload:import/hul_code_master',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -1,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_gps-master:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_upload:import/outlet_master.
  $menu_links['menu-new-custom-menus_upload:import/outlet_master'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'import/outlet_master',
    'router_path' => 'import/%',
    'link_title' => 'Upload',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_upload:import/outlet_master',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -1,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_outlet-master:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_upload:import/outlet_sequence.
  $menu_links['menu-new-custom-menus_upload:import/outlet_sequence'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'import/outlet_sequence',
    'router_path' => 'import/%',
    'link_title' => 'Upload',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_upload:import/outlet_sequence',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_original-outlet-sequence:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_upload:import/party_pack_master.
  $menu_links['menu-new-custom-menus_upload:import/party_pack_master'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'import/party_pack_master',
    'router_path' => 'import/%',
    'link_title' => 'Upload',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_upload:import/party_pack_master',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -50,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_van-plg-master:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_upload:import/pl_group_master.
  $menu_links['menu-new-custom-menus_upload:import/pl_group_master'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'import/pl_group_master',
    'router_path' => 'import/%',
    'link_title' => 'Upload',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_upload:import/pl_group_master',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -1,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_plgroup-master:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_upload:import/resource_master.
  $menu_links['menu-new-custom-menus_upload:import/resource_master'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'import/resource_master',
    'router_path' => 'import/%',
    'link_title' => 'Upload',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_upload:import/resource_master',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -1,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_agent-master:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_upload:import/sales_delivery_day_importer.
  $menu_links['menu-new-custom-menus_upload:import/sales_delivery_day_importer'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'import/sales_delivery_day_importer',
    'router_path' => 'import/%',
    'link_title' => 'Upload',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_upload:import/sales_delivery_day_importer',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -2,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_sales-delivery-day-master:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_upload:import/sku7_code_master.
  $menu_links['menu-new-custom-menus_upload:import/sku7_code_master'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'import/sku7_code_master',
    'router_path' => 'import/%',
    'link_title' => 'Upload',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_upload:import/sku7_code_master',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -50,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_sku7code-master:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_upload:import/user_defined_outlet_sequence.
  $menu_links['menu-new-custom-menus_upload:import/user_defined_outlet_sequence'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'import/user_defined_outlet_sequence',
    'router_path' => 'import/%',
    'link_title' => 'Upload',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_upload:import/user_defined_outlet_sequence',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 1,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_user-defined-outlet-sequence:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_upload:import/van_pl_group_stop_importer.
  $menu_links['menu-new-custom-menus_upload:import/van_pl_group_stop_importer'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'import/van_pl_group_stop_importer',
    'router_path' => 'import/%',
    'link_title' => 'Upload',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_upload:import/van_pl_group_stop_importer',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -2,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_van-stop-master:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_upload:import/vehicle_master.
  $menu_links['menu-new-custom-menus_upload:import/vehicle_master'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'import/vehicle_master',
    'router_path' => 'import/%',
    'link_title' => 'Upload',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_upload:import/vehicle_master',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -1,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_vehicle-master:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_upload:node/add/pricelist-group-master.
  $menu_links['menu-new-custom-menus_upload:node/add/pricelist-group-master'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'node/add/pricelist-group-master',
    'router_path' => 'node/add/pricelist-group-master',
    'link_title' => 'Upload',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_upload:node/add/pricelist-group-master',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -1,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_pricelist-group-master:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_upload:node/add/product-division-master.
  $menu_links['menu-new-custom-menus_upload:node/add/product-division-master'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'node/add/product-division-master',
    'router_path' => 'node/add/product-division-master',
    'link_title' => 'Upload',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_upload:node/add/product-division-master',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -1,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_product-division-master:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_upload:node/add/resource-type.
  $menu_links['menu-new-custom-menus_upload:node/add/resource-type'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'node/add/resource-type',
    'router_path' => 'node/add/resource-type',
    'link_title' => 'Upload',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_upload:node/add/resource-type',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -1,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_resource-type-master:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_upload:outlet-area-hook/import.
  $menu_links['menu-new-custom-menus_upload:outlet-area-hook/import'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'outlet-area-hook/import',
    'router_path' => 'outlet-area-hook/import',
    'link_title' => 'Upload',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_upload:outlet-area-hook/import',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_outlet-area-hook:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_upload:pdd/add.
  $menu_links['menu-new-custom-menus_upload:pdd/add'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'pdd/add',
    'router_path' => 'pdd/add',
    'link_title' => 'Upload',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_upload:pdd/add',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -1,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_pdd-master:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_upload:sales-delivery-data/import.
  $menu_links['menu-new-custom-menus_upload:sales-delivery-data/import'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'sales-delivery-data/import',
    'router_path' => 'sales-delivery-data/import',
    'link_title' => 'Upload',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_upload:sales-delivery-data/import',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -1,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_delivery-data:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_upload:sales-register/import.
  $menu_links['menu-new-custom-menus_upload:sales-register/import'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'sales-register/import',
    'router_path' => 'sales-register/import',
    'link_title' => 'Upload',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_upload:sales-register/import',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -1,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_sales-register-data:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_upload:van-stop-master.
  $menu_links['menu-new-custom-menus_upload:van-stop-master'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'van-stop-master',
    'router_path' => 'van-stop-master',
    'link_title' => 'Upload',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_upload:van-stop-master',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_target-master:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_user-defined-outlet-sequence:<nolink>.
  $menu_links['menu-new-custom-menus_user-defined-outlet-sequence:<nolink>'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => '<nolink>',
    'router_path' => '<nolink>',
    'link_title' => 'User Defined Outlet Sequence',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_user-defined-outlet-sequence:<nolink>',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 1,
    'weight' => -4,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_routing:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_user-defined-sequence-confirmation:revised-sequence-confirm.
  $menu_links['menu-new-custom-menus_user-defined-sequence-confirmation:revised-sequence-confirm'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'revised-sequence-confirm',
    'router_path' => 'revised-sequence-confirm',
    'link_title' => 'User Defined Sequence Confirmation',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_user-defined-sequence-confirmation:revised-sequence-confirm',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_input:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_user-manual:user-manual.
  $menu_links['menu-new-custom-menus_user-manual:user-manual'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'user-manual',
    'router_path' => 'user-manual',
    'link_title' => 'User Manual',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_user-manual:user-manual',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
  );
  // Exported menu link: menu-new-custom-menus_van-cld-report:van-cld-report.
  $menu_links['menu-new-custom-menus_van-cld-report:van-cld-report'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'van-cld-report',
    'router_path' => 'van-cld-report',
    'link_title' => 'Van Cld Report',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_van-cld-report:van-cld-report',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_output:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_van-planning-report:van-planning-report.
  $menu_links['menu-new-custom-menus_van-planning-report:van-planning-report'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'van-planning-report',
    'router_path' => 'van-planning-report',
    'link_title' => 'Van Planning Report',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_van-planning-report:van-planning-report',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_output:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_van-plg-master:<nolink>.
  $menu_links['menu-new-custom-menus_van-plg-master:<nolink>'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => '<nolink>',
    'router_path' => '<nolink>',
    'link_title' => 'Van PLG master',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_van-plg-master:<nolink>',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 1,
    'weight' => -7,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_routing:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_van-stop-master:<nolink>.
  $menu_links['menu-new-custom-menus_van-stop-master:<nolink>'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => '<nolink>',
    'router_path' => '<nolink>',
    'link_title' => 'Van stop master',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_van-stop-master:<nolink>',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 1,
    'weight' => -3,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_routing:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_vehicle-master:<nolink>.
  $menu_links['menu-new-custom-menus_vehicle-master:<nolink>'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => '<nolink>',
    'router_path' => '<nolink>',
    'link_title' => 'Vehicle Master',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_vehicle-master:<nolink>',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 1,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_general:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_view-area-hook-group-wise-outlets-sequence:area-confirm-view.
  $menu_links['menu-new-custom-menus_view-area-hook-group-wise-outlets-sequence:area-confirm-view'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'area-confirm-view',
    'router_path' => 'area-confirm-view',
    'link_title' => 'View Area Hook Group wise Outlets Sequence',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_view-area-hook-group-wise-outlets-sequence:area-confirm-view',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_output:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_view-area-hook-wise-outlets:area-hook-confirm-view.
  $menu_links['menu-new-custom-menus_view-area-hook-wise-outlets:area-hook-confirm-view'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'area-hook-confirm-view',
    'router_path' => 'area-hook-confirm-view',
    'link_title' => 'View Area Hook Wise Outlets',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_view-area-hook-wise-outlets:area-hook-confirm-view',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_output:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_view-trip-on-map:address/confirm/trip-wise.
  $menu_links['menu-new-custom-menus_view-trip-on-map:address/confirm/trip-wise'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => 'address/confirm/trip-wise',
    'router_path' => 'address/confirm/trip-wise',
    'link_title' => 'View Trip On Map',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_view-trip-on-map:address/confirm/trip-wise',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
    'parent_identifier' => 'menu-new-custom-menus_output:<nolink>',
  );
  // Exported menu link: menu-new-custom-menus_warehousing:<nolink>.
  $menu_links['menu-new-custom-menus_warehousing:<nolink>'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'link_path' => '<nolink>',
    'router_path' => '<nolink>',
    'link_title' => 'Warehousing',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-new-custom-menus_warehousing:<nolink>',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 1,
    'weight' => -49,
    'customized' => 1,
  );

  // Translatables
  // Included for use with string extractors like potx.
  t(' Impact Return Analysis Count and Value (Central)');
  t('Address Confirmation');
  t('Agent Master');
  t('Area Hook Group Confirmation');
  t('Area Hook Group Master');
  t('Area Hook Master');
  t('Assignment History');
  t('Bill Pendency');
  t('Bill Vs Trip (Central)');
  t('Bill Vs Trip Report');
  t('Billing Data');
  t('Bills Per Person');
  t('Bills Per Person DayWise');
  t('Bills Per Van');
  t('Bills Per Van (Central)');
  t('Bills Per Van DayWise');
  t('Bounced Check declaration');
  t('Bounced Cheque Upload');
  t('Bounced Cheques Assignment');
  t('Central delivery data');
  t('Central report Authorisation');
  t('Centrally Managed');
  t('Channel Master');
  t('Collection');
  t('Collection History');
  t('Collection Update');
  t('DRS PrintOut - 2');
  t('DRS Report');
  t('Dashboard');
  t('Day Wise Bill Count And Value');
  t('Day-Van Pl Group Area Grouping');
  t('Delete Billing Data');
  t('Delivery Data');
  t('Dispatch');
  t('Dispatch On Time Adherence');
  t('Dispatch Planning');
  t('Download');
  t('Download (Central)');
  t('EGIR Data');
  t('GPS Master');
  t('General');
  t('General Report');
  t('HHT Master');
  t('Impact Delivery Report 2');
  t('Incremental Outlet Addition');
  t('Input');
  t('Masters');
  t('Metrics');
  t('New Custom Menus');
  t('OTIF Day Wise');
  t('OTIF Value');
  t('On-Time Adherence');
  t('Original Outlet Sequence');
  t('Original Outlet Sequencing-API');
  t('Outlet Area Hook');
  t('Outlet Master');
  t('Output');
  t('PDD Master');
  t('PL_Group Master');
  t('Party Packing');
  t('Party Packing (Central)');
  t('Party Packing On Time Adherence');
  t('Party Packing Productivity');
  t('Party Packing Van Progress');
  t('Payment Terms');
  t('Pick Confirm');
  t('Picking');
  t('Picking On Time Adherence VAN Loose ');
  t('Picking Productivity');
  t('Picking Progress Task Bulk');
  t('Picking Progress Task Loose');
  t('Pricelist Group Master');
  t('Product Division Master');
  t('Productivity');
  t('Progress');
  t('Resource Type Master');
  t('Return Value %');
  t('Routing');
  t('SKU7_Code Master');
  t('Sales Delivery Day Master');
  t('Sales Register Data');
  t('Signed Bill Assignment');
  t('Signed Bill Declaration');
  t('Signed Bill Upload ');
  t('Static Beat Creation');
  t('Target Master');
  t('Trip Status Report');
  t('Trip Vs Van');
  t('Unloading');
  t('Unloading (Central)');
  t('Unloading On Time Adherence');
  t('Unloading Progress');
  t('Upload');
  t('User Defined Outlet Sequence');
  t('User Defined Sequence Confirmation');
  t('User Manual');
  t('Van Cld Report');
  t('Van PLG master');
  t('Van Planning Report');
  t('Van stop master');
  t('Vehicle Master');
  t('View Area Hook Group wise Outlets Sequence');
  t('View Area Hook Wise Outlets');
  t('View Trip On Map');
  t('Warehousing');

  return $menu_links;
}
