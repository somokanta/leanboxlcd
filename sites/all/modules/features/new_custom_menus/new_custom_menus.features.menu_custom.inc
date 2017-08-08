<?php
/**
 * @file
 * new_custom_menus.features.menu_custom.inc
 */

/**
 * Implements hook_menu_default_menu_custom().
 */
function new_custom_menus_menu_default_menu_custom() {
  $menus = array();

  // Exported menu: management.
  $menus['management'] = array(
    'menu_name' => 'management',
    'title' => 'Management',
    'description' => 'The <em>Management</em> menu contains links for administrative tasks.',
  );
  // Exported menu: menu-new-custom-menus.
  $menus['menu-new-custom-menus'] = array(
    'menu_name' => 'menu-new-custom-menus',
    'title' => 'New Custom Menus',
    'description' => 'New Custom Menus',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Management');
  t('New Custom Menus');
  t('The <em>Management</em> menu contains links for administrative tasks.');

  return $menus;
}
