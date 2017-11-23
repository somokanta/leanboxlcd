<?php
/**
 * @file
 * leanbox_pod_features.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function leanbox_pod_features_taxonomy_default_vocabularies() {
  return array(
    'pod' => array(
      'name' => 'pod',
      'machine_name' => 'pod',
      'description' => 'pod terms',
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
