<?php

/**
 * Creates nodes from feed items.
 */
class FeedsNodeTargetMasterProcessor extends FeedsNodeProcessor {
 
  
  /**
   * Get nid of an existing feed item node if available.
   */
  protected function existingEntityId(FeedsSource $source, FeedsParserResult $result) {

    global $user;
    $user_id = $user->uid;
    $distributor_id = change_feeds_import_get_distributor_id($user_id);
    // Iterate through all unique targets and test whether they do already
    // exist in the database.
    foreach ($this->uniqueTargets($source, $result) as $target => $value) {
      switch ($target) {
        case 'title':
          $node_title = leanbox_mobile_get_distributor_using_rscode($value);
          $query1 = db_select('node', 'n');
          $query1->condition('n.type', 'van_stop_master');
          $query1->condition('n.title', $node_title);
          $query1->fields('n', array('nid'));
          $nid = $query1->execute()->fetchField();
          break;
      }
      if ($nid) {
        // Return with the first nid found.
        return $nid;
      }
    }
    return 0;
  }
}