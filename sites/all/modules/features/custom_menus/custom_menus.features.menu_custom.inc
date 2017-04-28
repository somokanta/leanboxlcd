<?php
/**
 * @file
 * custom_menus.features.menu_custom.inc
 */

/**
 * Implements hook_menu_default_menu_custom().
 */
function custom_menus_menu_default_menu_custom() {
  $menus = array();

  // Exported menu: menu-custom-menus.
  $menus['menu-custom-menus'] = array(
    'menu_name' => 'menu-custom-menus',
    'title' => 'Custom Menus',
    'description' => 'All Custom Menus',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('All Custom Menus');
  t('Custom Menus');

  return $menus;
}
