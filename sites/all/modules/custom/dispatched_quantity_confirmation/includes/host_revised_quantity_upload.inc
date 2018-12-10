<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function host_revised_qty_upload_form($form, $form_state) {
    $form['file'] = array(
        '#type' => 'file',
        '#title' => t('Choose a CSV file for host revised qty upload'),
    );

    $form['template_download'] = array(
        '#type' => 'markup',
        '#prefix' => '<div id="template_wrapper">',
        '#suffix' => '</div>',
        '#markup' => l("Download Template", drupal_get_path('module', 'dispatched_quantity_confirmation') . '/includes/host_revised_qty_upload_template.csv'),
    );

    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Upload'),
    );
    return $form;
}

function host_revised_qty_upload_form_validate($form, &$form_state) {
    $file = file_save_upload('file', array(
        'file_validate_extensions' => array('csv'),
    ));

    $header_array_from_xsd = array('Bill Number', 'SKU Code', 'Host Revised Quantity');
    if ($file) {
        if ($file = file_move($file, 'public://')) {
            $form_state['values']['file'] = $file;
            $filepath = $form_state['values']['file']->uri;
            $data = array_map("str_getcsv", file($filepath, FILE_SKIP_EMPTY_LINES));
            $header_from_csv = array_shift($data);
            if ($header_array_from_xsd != $header_from_csv) {
                $type = 'error';
                $message = 'Header mismatch.';
                form_set_error('file_upload', t($message));
            }

// Blank and attached to trip validation
            $datas = array();
            if (($handle = fopen($filepath, "r")) !== FALSE) {
                $header = fgetcsv($handle, 0, ',', '"');

                while (($row = fgetcsv($handle) ) !== FALSE) {

                    foreach ($row as $key => &$value) {
                        $value = mb_convert_encoding(trim($value), 'UTF-8', 'UTF-8');
                    }

                    $datas[] = array_combine($header, $row);
                }
                fclose($handle);
            }

            global $user;
            $dis_id = change_feeds_import_get_distributor_id($user->uid);

            foreach ($datas as $key => $val) {
                $bill_Number = $val['Bill Number'];
                $sku = $val['SKU Code'];
                $host_revised_qty = $val['Host Revised Quantity'];

                if (empty($bill_Number)) {
                    form_set_error('form', t("Bill Number cannot be empty for sku $sku"));
                }
                if (empty($sku)) {
                    form_set_error('form', t("Sku cannot be empty for bill number $bill_Number"));
                }
                if (empty($host_revised_qty)) {
                    form_set_error('form', t("Host revised qty cannot be empty for sku $sku"));
                }
                if (!empty($host_revised_qty)) {
                    if (!is_numeric($host_revised_qty)) {
                        form_set_error('form', t("Host revised qty should be numeric for sku $sku"));
                    }
                }

                $query = db_select('custom_billing_data', 'cb');
                $query->condition('cb.distributor_id', $dis_id);
                $query->condition('cb.bill_number', $bill_Number);
                $query->condition('cb.sku7_code', $sku);
                $query->fields('cb', array('id'));
                $result = $query->execute()->fetchField();
                if (empty($result)) {
                    form_set_error('form', t("Bill number: $bill_Number is not attached to sku: $sku"));
                }
            }
        } else {
            form_set_error('file_upload', t('Failed to write the uploaded file the site\'s file folder.'));
        }
    } else {
        form_set_error('file', t('No file was uploaded.'));
    }
}

function host_revised_qty_upload_form_submit($form, &$form_state) {

    $file = $form_state['values']['file'];
    $filepath = $file->uri;

    $datas = array();

    if (($handle = fopen($filepath, "r")) !== FALSE) {
        $header = fgetcsv($handle, 0, ',', '"');

        while (($row = fgetcsv($handle) ) !== FALSE) {

            foreach ($row as $key => &$value) {
                $value = mb_convert_encoding(trim($value), 'UTF-8', 'UTF-8');
            }

            $datas[] = array_combine($header, $row);
        }
        fclose($handle);
    }

    $operations = array();
    $data_chunk = array_chunk($datas, 100);
    foreach ($data_chunk as $data) {
        $operations[] = array('host_revised_qty_upload_form_process', array($data));
    }
    $batch = array(
        'title' => t('Uploading Host Revised Qty Data Csv'),
        'operations' => $operations,
        'init_message' => t("Please wait..."),
        'progress_message' => t('Processed @current out of @total.'),
        'error_message' => t('Upload data processing has encountered an error.'),
        'finished' => 'host_revised_qty_upload_form_finished',
        'file' => drupal_get_path('module', 'dispatched_quantity_confirmation') . '/includes/host_revised_quantity_upload.inc',
    );
    batch_set($batch);
}

function host_revised_qty_upload_form_process($data, &$context) {
    if (!isset($context['results']['mismatch_data_download'])) {
        $context['results']['mismatch_data_download'] = array();
    }

    global $user;
    $dis_id = change_feeds_import_get_distributor_id($user->uid);

    foreach ($data as $val) {
        $bill_Number = $val['Bill Number'];
        $sku = $val['SKU Code'];
        $host_revised_qty = $val['Host Revised Quantity'];

        db_merge('host_revised_quantity')
                ->key(array('bill_number' => $bill_Number, 'sku_code' => $sku))
                ->fields(array(
                    'bill_number' => $bill_Number,
                    'sku_code' => $sku,
                    'revised_qty' => $host_revised_qty,
                    'created_date' => REQUEST_TIME,
                    'distributor_id' => $dis_id
                ))
                ->execute();

        $query = db_select('dispatch_qty_confirmation', 'dqc');
        $query->condition('dqc.distributor_id', $dis_id, '=');
        $query->condition('dqc.bill_number', $bill_Number);
        $query->condition('dqc.sku_code', $sku);
        $query->fields('dqc', array('dispatch_loose_qty'));
        $dispatch_loose_qty = $query->execute()->fetchField();
        if ($host_revised_qty != $dispatch_loose_qty) {
            //$arr[] = $dispatch_loose_qty;


            $row['bill_number'] = $bill_Number;
            $row['sku'] = $sku;
            $row['host_revised_qty'] = $host_revised_qty;
            $row['dispatch_loose_qty'] = $dispatch_loose_qty;


            $context['results']['mismatch_data_download'][] = $row;
        }
    }

    $rows = $context['results']['mismatch_data_download'];
    $header = array('Bill No', 'Sku', 'Host Revised Qty', 'To Be Dispatched Qty');
    $output = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('width' => '100%')));

    if (!empty($rows)) {
        cache_set('mismatch_data_download' . session_id(), $output, 'cache', time() + 60 * 10);
        $html = l(t('XLS'), '/mismatch-data/export');
        drupal_set_message(t("Click here to download the mismatch data: $html"), 'warning');
    }
}

function host_revised_qty_upload_form_finished($success, $results, $operations) {
    if ($success) {
        drupal_set_message('All Data are Uploaded successfully');
        $_GET['destination'] = "host-revised-qty/import";
    } else {
        drupal_set_message('There is Error Occured in batch process');
    }
}

function mismatch_data_table_export() {
    $file = "mismatch_data.xls";
    $data = cache_get('mismatch_data_download' . session_id());
    if (!empty($data)) {
        $table = $data->data;
    }
    header("Content-type: application/vnd.ms-excel");
    header("Content-Disposition: attachment; filename=$file");
    echo $table;
}
