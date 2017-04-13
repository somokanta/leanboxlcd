<?php

// upload form for billing data

function leanbox_dashboard_delivery_per_van_form($form, $form_state) {
  global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);

  $form['file'] = array(
    '#type' => 'file',
    '#title' => t('Choose a CSV file for Delivery Per Van upload'),
  );
  $form['template_download'] = array(
    '#type' => 'markup',
    '#prefix' => '<div id="template_wrapper">',
    '#suffix' => '</div>',
    '#markup' => l("Download Template", drupal_get_path('module', 'leanbox_dashboard') . '/delivery_per_van_template.csv'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Upload'),
  );



  return $form;
}

function leanbox_dashboard_delivery_per_van_form_validate($form, &$form_state) {
  $file = file_save_upload('file', array(
    'file_validate_extensions' => array('csv'),
  ));

  $header_array_from_xsd = array(
    'Van Pl Group', 'Delivery Per Van', 'Outlets Per Stop',
  );
  if ($file) {
    if ($file = file_move($file, 'public://')) {
      $form_state['values']['file'] = $file;
      $filepath = $form_state['values']['file']->uri;
      $data = array_map("str_getcsv", file($filepath, FILE_SKIP_EMPTY_LINES));
      $header_from_csv = array_shift($data);
      if ($header_array_from_xsd != $header_from_csv) {
        $type = 'error';
        $message = 'Header mismatch.';
        // form_set_error('file_upload', t($message));
      }
    }
    else {
      form_set_error('file_upload', t('Failed to write the uploaded file the site\'s file folder.'));
    }
  }
  else {
    form_set_error('file', t('No file was uploaded.'));
  }
}

function leanbox_dashboard_delivery_per_van_form_submit($form, &$form_state) {
  global $user;
  $user_id = $user->uid;
  $distributor_id = change_feeds_import_get_distributor_id($user_id);
  $file = $form_state['values']['file'];
  $file_name = $file->filename;
  $fid = $file->fid;
  $filepath = $file->uri;

  $datas = array();
  // Making data as associative array with header.
  if (($handle = fopen($filepath, "r")) !== FALSE) {
    $header = fgetcsv($handle, 0, ',', '"');
    while (($row = fgetcsv($handle) ) !== FALSE) {
      $datas[] = array_combine($header, $row);

      foreach ($datas as $key => &$value) {
        $empty_flag = FALSE;
        $pl_grp = $value['Van Pl Group'];
        $del1 = $value['Delivery Per Van'];
        $del2 = $value['Outlets Per Stop'];

        foreach ($value as $key1 => $value1) {
          $value1 = mb_convert_encoding($value1, 'UTF-8', 'UTF-8');
          if (empty($value1)) {
            drupal_set_message(t("Value can not be empty for $key1"), 'error');
            $empty_flag = TRUE;
          }
        }
        if (!is_numeric($del1)) {
          drupal_set_message(t("$del1 Value should be numeric for Pl Group $pl_grp"), 'error');
          $empty_flag = TRUE;
        }
        if (!is_numeric($del2)) {
          drupal_set_message(t("$del2 Value should be numeric for Pl Group $pl_grp"), 'error');
          $empty_flag = TRUE;
        }
        if (!$empty_flag) {
          $query = db_select('node', 'n');
          $query->addTag('node_uid');
          $query->condition('n.type', 'party_pack_master');
          $query->join('field_data_field_van_pl_group', 'g', 'g.entity_id = n.nid');
          $query->condition('g.field_van_pl_group_value', $pl_grp);
          $query->fields('n', array('nid'));
          $res = $query->execute()->fetch(PDO::FETCH_ASSOC);
          if (empty($res)) {
            drupal_set_message(t("Van Pl Group doesn't exist in our system."), 'error');
            $empty_flag = TRUE;
          }
          else {
            foreach ($res as $nid) {
              $node = node_load($nid);
              $node->field_delivery_per_van['und'][0]['value'] = $value['Delivery Per Van'];
              $node->field_delivery_beats['und'][0]['value'] = $value['Outlets Per Stop'];
              field_attach_update('node', $node);
              entity_get_controller('node')->resetCache(array($nid));
            }
          }
        }
        if ($empty_flag) {
          continue;
        }
      }
    }
    fclose($handle);
  }
}
