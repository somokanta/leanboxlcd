<?php

/**
 * Creates nodes from feed items.
 */
class FeedsNodeCustomProcessor extends FeedsNodeProcessor {
 
  
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
          $nid = db_query("SELECT nid FROM {node} WHERE title = :title AND type = :type AND uid = :uid", array(':title' => $value, ':type' => $this->bundle(), ':uid' => $distributor_id))->fetchField();
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