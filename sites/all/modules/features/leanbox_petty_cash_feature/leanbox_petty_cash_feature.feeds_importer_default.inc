<?php
/**
 * @file
 * leanbox_petty_cash_feature.feeds_importer_default.inc
 */

/**
 * Implements hook_feeds_importer_default().
 */
function leanbox_petty_cash_feature_feeds_importer_default() {
  $export = array();

  $feeds_importer = new stdClass();
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'petty_cash_master';
  $feeds_importer->config = array(
    'name' => 'Petty Cash Master',
    'description' => 'Petty Cash taxonomy Importer',
    'fetcher' => array(
      'plugin_key' => 'FeedsFileFetcher',
      'config' => array(
        'allowed_extensions' => 'txt csv tsv ',
        'delete_uploaded_file' => 1,
        'direct' => 0,
        'directory' => 'public://feeds',
        'allowed_schemes' => array(
          'public' => 'public',
        ),
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsCSVParser',
      'config' => array(
        'delimiter' => ',',
        'encoding' => 'UTF-8',
        'no_headers' => 0,
      ),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsTermProcessor',
      'config' => array(
        'vocabulary' => 0,
        'mappings' => array(
          0 => array(
            'source' => 'Type',
            'target' => 'field_petty_cash_type',
            'unique' => FALSE,
            'language' => 'und',
          ),
          1 => array(
            'source' => 'Head',
            'target' => 'field_petty_cash_head',
            'unique' => FALSE,
            'language' => 'und',
          ),
          2 => array(
            'source' => 'Sub head',
            'target' => 'field_petty_cash_subhead',
            'unique' => FALSE,
            'language' => 'und',
          ),
          3 => array(
            'source' => 'Name',
            'target' => 'name',
            'unique' => FALSE,
          ),
        ),
        'insert_new' => '1',
        'update_existing' => '0',
        'update_non_existent' => 'skip',
        'input_format' => 'plain_text',
        'skip_hash_check' => 0,
        'bundle' => 'petty_cash',
        'language' => 'und',
      ),
    ),
    'content_type' => '',
    'update' => 0,
    'import_period' => '-1',
    'expire_period' => 3600,
    'import_on_create' => 1,
    'process_in_background' => 0,
  );
  $export['petty_cash_master'] = $feeds_importer;

  return $export;
}
