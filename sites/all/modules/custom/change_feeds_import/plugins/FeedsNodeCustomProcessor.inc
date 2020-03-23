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
        $central_distributor_contents = array('resource_master', 'vehicle_master', 'area_code_master', 'asset_type_master', 'hht_master', 'resources_contract', 'vehicle_contract', 'vehicle_extra_working', 'resource_extra_working', 'resource_leaves');
        $subadmin_contents = array('sku7_code_master', 'product_category_master', 'product_division_master', 'product_sub_category_master', 'product_validation_master', 'putaway_group_master', 'resource_type', 'reason_for_update_master', 'short_reason_master', 'skill_master', 'trip_undispatch_reasons', 'pricelist_group_master', 'notes_denominations', 'non_bankable_reason_master', 'lb_product_master', 'godown_short_reason_master', 'godown_master', 'gs_claim_reason_master', 'excess_reason_master', 'dispatch_short_reason', 'dispatch_planning_reason_master', 'cancel_discard_reason_master', 'cancel_discard_bills', 'channel_master', 'brand_master', 'bill_not_edited_reason_master', 'bank_master');
        // Iterate through all unique targets and test whether they do already
        // exist in the database.
        foreach ($this->uniqueTargets($source, $result) as $target => $value) {
            switch ($target) {
                case 'title':
                    // uploading distributor wise data
                    if (in_array('central_distributor', $user->roles)) {
                        $dist = validate_distributor_name(trim($result->current_item['distributor name']));
                        $distributor_id = !empty($dist) ? $dist : $distributor_id;
                    }
                     // if any of the child logged in to update central master content
                    elseif (!in_array('central_distributor', $user->roles) && in_array($node->type, $central_distributor_contents)) {
                        $distributor_id = get_central_distributor_id($distributor_id);
                    }
                    elseif (in_array($node->type, $subadmin_contents)) {
                        $subadmin_distributor_id = get_uid_by_role('sub_admin');
                        $distributor_id = $subadmin_distributor_id;
                    }
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
