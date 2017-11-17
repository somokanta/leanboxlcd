<?php
/**
 * @file
 * leanbox_petty_cash_feature.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function leanbox_petty_cash_feature_taxonomy_default_vocabularies() {
  return array(
    'petty_cash' => array(
      'name' => 'Petty Cash',
      'machine_name' => 'petty_cash',
      'description' => 'Petty cash type',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
      'rdf_mapping' => array(
        'rdftype' => array(
          0 => 'skos:ConceptScheme',
        ),
        'name' => array(
          'predicates' => array(
            0 => 'dc:title',
          ),
        ),
        'description' => array(
          'predicates' => array(
            0 => 'rdfs:comment',
          ),
        ),
      ),
    ),
  );
}
