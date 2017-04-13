<?php

/**
 * callback function for area hook group
 */
function leanbox_dashboard_area_hook_grouping_callback($download = FALSE) {

  $sales_day_terms = change_feeds_import_get_terms_using_vocubulary('Outlet Days');

  $query = db_select('node', 'n');
  $query->addTag('node_uid');
  $query->condition('n.type', 'area_hook_master');
  $query->join('field_data_field_area_hook_day', 'a', 'a.entity_id = n.nid');
  $query->leftjoin('field_data_field_area_hook_group', 'g', 'g.entity_id = n.nid');
  $query->fields('n', array('title'));
  $query->fields('a', array('field_area_hook_day_tid'));
  $query->fields('g', array('field_area_hook_group_value'));
  $query->orderBy('n.title', 'ASC');
  $res = $query->execute()->fetchAll(PDO::FETCH_GROUP | PDO::FETCH_ASSOC); // result of area hook master data

  $header[] = 'AREA';
  foreach ($sales_day_terms as $key => $val) {
    $header[$key] = $val;
  }

  foreach ($res as $area => $info) {
    $row = array();
    $row[] = $area;
    foreach ($sales_day_terms as $key => $val) {
      $flag = 0;
      foreach ($info as $val2) {
        if ($val2['field_area_hook_day_tid'] == $key) {
          $row[] = !empty($val2['field_area_hook_group_value']) ? $val2['field_area_hook_group_value'] : '';

          $flag = 1;
          break;
        }
      }
      if ($flag != 1) {
        $row[] = '';
      }
    }
    $rows[] = $row;
  }

  $output .= theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array('width' => '100%')
  ));
  $output .= l(t('CSV'), 'area-hook-grouping-download');
  if ($download) {
    $output1['header'] = $header;
    $output1['rows'] = $rows;

    return $output1;
  }
  return $output;
}

function leanbox_dashboard_area_hook_grouping_download() {
  $table = leanbox_dashboard_area_hook_grouping_callback(TRUE);
  header('Content-Type: text/csv; charset=utf-8');
  header('Content-Disposition: attachment; filename=area_hook_group.csv');
  $file = fopen('php://output', 'w');
  fputcsv($file, $table['header']);
  foreach ($table['rows'] as $line) {
    fputcsv($file, $line);
  }

  fclose($file);
  exit();
}
