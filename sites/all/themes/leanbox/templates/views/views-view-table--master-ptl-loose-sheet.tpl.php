<?php

/**
 * @file
 * Template to display a view as a table.
 *
 * - $title : The title of this group of rows.  May be empty.
 * - $header: An array of header labels keyed by field id.
 * - $caption: The caption for this table. May be empty.
 * - $header_classes: An array of header classes keyed by field id.
 * - $fields: An array of CSS IDs to use for each field id.
 * - $classes: A class or classes to apply to the table, based on settings.
 * - $row_classes: An array of classes to apply to each row, indexed by row
 *   number. This matches the index in $rows.
 * - $rows: An array of row items. Each row is an array of content.
 *   $rows are keyed by row number, fields within rows are keyed by field ID.
 * - $field_classes: An array of classes to apply to each field, indexed by
 *   field id, then row number. This matches the index in $rows.
 * @ingroup views_templates
 */
?>
<?php $page_break = &drupal_static(__FUNCTION__);
 if (!isset($page_break)) {
   $page_break = TRUE;
 }else{
   print '<pagebreak />';
 } ?>
<table border="1" cellpadding="5" cellspacing="0" width="100%" style="color:#000; font-size: 13px; font-family: arial" <?php if ($classes) { print 'class="'. $classes . '" '; } ?><?php print $attributes; ?>>
   <?php if (!empty($title) || !empty($caption)) : ?>
     <caption><?php print $caption . $title; ?></caption>
  <?php endif; ?>
  <?php if (!empty($header)) : ?>
    <thead>
      <tr>
        <?php foreach ($header as $field => $label): ?>
          <th <?php if (strpos(current_path(), 'download') !== false){ print 'style="background: #ccc; color: #000; border:0.5px solid gray;"'; } else { print 'style="background: #ccc; color: #000;"'; }?><?php if ($header_classes[$field]) { print 'class="'. $header_classes[$field] . '" '; } ?> scope="col">
            <?php print $label; ?>
          </th>
        <?php endforeach; ?>
      </tr>
    </thead>
  <?php endif; ?>
  <tbody>
    <?php $counter_ser = 1; $cld_total = 0; ?> <!-- Counter initialization. -->
    <?php foreach ($rows as $row_count => $row): ?>
      <tr <?php if ($row_classes[$row_count]) { print 'class="' . implode(' ', $row_classes[$row_count]) .'"';  } ?>>
        <?php $row['counter'] = $counter_ser; ?> <!-- Counter Reset -->
        <?php $cld_total += $row['php']; ?> <!-- Count cld -->
        <?php foreach ($row as $field => $content): ?>
          <td <?php if (strpos(current_path(), 'download') !== false){ print 'style="color:#666; border:0.5px solid gray;"'; } else { print 'style="color:#666;"'; }?><?php if ($field_classes[$field][$row_count]) { print 'class="'. $field_classes[$field][$row_count] . '" '; } ?><?php print drupal_attributes($field_attributes[$field][$row_count]); ?>>
            <?php print $content; ?>
          </td>
        <?php endforeach; ?>
      </tr>
      <?php $counter_ser++; ?> <!-- Counter Increment -->
    <?php endforeach; ?>
  </tbody>
</table>
<table border="0" width="100%" cellpadding="5"><tr> <td style="font-weight:bold; text-align:right"> <?php print ltrim($title, 'Category:').' CLDs '. $cld_total; ?></td></tr></table>
<?php   print '<table class = "preview_table" width = "100%" cellspacing = "0" style = "font-family:arial; font-size: 12px; color:#000;" cellpadding = "5" border = "0" align = "center">
      <tbody>
      <tr>
      <td></td>
      <td>Picker Name:</td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td>Loose Conf Name:</td>
      <td></td>
      <td></td>
      </tr>
      <tr>
      <td></td>
      <td>Start Time:</td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td>Start Time:</td>
      <td></td>
      <td></td>
      </tr>
      <tr>
      <td></td>
      <td>End Time:</td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td>End Time:</td>
      <td></td>
      <td></td>
      </tr>
      <tr>
      <td></td>
      <td>Sign:</td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      </tr>
      </tbody>
      </table>';
?>  <!--Add footer after every category table and comment footer in footer_master_ptl_loose_custom_field.-->