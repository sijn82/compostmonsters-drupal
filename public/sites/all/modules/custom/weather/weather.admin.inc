<?php

//     ------------------------------------- Admin/Config/Batch-Weather-Data -----------------------------------------

/**
 * Implements custom page callback function to display both a form and generate a table with the inputted results.
 * A $_SESSION is set (and unset) to refresh the data displayed with each import.
 * @return mixed
 *
 */
function weather_batch_display()
{
    if (isset($_SESSION['weather_batch_table'])) {
        $batch_table = $_SESSION['weather_batch_table'];
        unset($_SESSION['weather_batch_table']);
        $batch_processing_display['results'] = $batch_table;
    }

    $batch_processing_display['form'] = drupal_get_form('weather_batch_processing_form');

    return $batch_processing_display;
}

/**
 * Implements hook_form().
 * This function will create the bulk weather data processing form, allowing the admin user to make several
 * latitude/longitude searches in one submission.
 */
function weather_batch_processing_form($form, &$form_state)
{
    if (isset($form_state['input']['batch_processing_entry_field'])) {
        $default_values = $form_state['input']['batch_processing_entry_field'];
    } elseif (isset($_SESSION['weather_data_entered'])) {
        $default_values = $_SESSION['weather_data_entered'];
        unset($_SESSION['weather_data_entered']);
    } else {
        $default_values = "";
    }


    $form['introduction'] = [
        '#markup' => <<< HEREDOC
            <div>
                <h4>Welcome to the City Weather Data Bulk Processing page</h4>
                <p>Searching for the weather of more than one place can be a time consuming process,<br>
                so this page is here to allow you to add as many coordinates (one per line) as you <br>
                wish and then create a table of the results on submission.</p>
            </div>
HEREDOC
    ];

    $form['batch_processing_entry_field'] = [
        '#type' => 'textarea',
        '#description' => t("Enter your coordinates here, one per line. Latitude then Longitude, separated by a comma."),
        '#title' => t("Weather Data Bulk Entry Field"),
        '#default_value' => $default_values,
    ];

    $form['csv_importer'] = [
        '#name' => 'files[csv_importer]',
        '#title' => t('Import CSV file.'),
        '#type' => 'file',
        '#description' => t("Import your CSV files here, 2 column format only, 'Lat', 'Long'."),
    ];

    $form['submit'] = [
        '#type' => 'submit',
        '#value' => 'Submit',
    ];

    return $form;
}

/**
 * Implements hook_validate() - This function checks the inputted data against a regular expression to unsure invalid
 * submissions are caught and the user is given an idea of why and how to fix their search terms.
 * @param $form
 * @param $form_state
 */
function weather_batch_processing_form_validate($form, &$form_state)
{
    $file_type_validation = [
        'file_validate_extensions' => ['csv'],
    ];

    $file = (file_save_upload('csv_importer', $file_type_validation, 'temporary://', FILE_EXISTS_REPLACE));

    $form_state['values']['csv_upload'] = $file;

    // If both input fields are empty - show an error.
    if (empty($form_state['values']['batch_processing_entry_field']) && null === $file) {
        form_set_error('batch_processing_entry_field', t('No data has been entered.'));
        form_set_error('csv_importer', t('Please enter some data, in one of these fields before clicking on submit.'));
    }

    // If both input fields contain data - show an error.
    if (!empty($form_state['values']['batch_processing_entry_field']) && null !== $file) {
        form_set_error('batch_processing_entry_field', t('Both fields have been entered.'));
        form_set_error('csv_importer', t('Please remove the data, in one of these fields before clicking on submit.'));
    }

    if (!empty($form_state['values']['batch_processing_entry_field'])) {
        if (1 !== preg_match(
            '/^(\ *[\d+\-\.]+\ *,\ *[\d+\-\.]+\ *(\r?\n)?)+$/i',
            $form_state['values']['batch_processing_entry_field']
        )) {
            form_set_error(
                'batch_processing_entry_field',
                t("Input error. Please check for empty lines, or invalid characters. Valid examples (per line) include '12, 32' or '12.333, -32.333'.")
            );
        }
    }
}

/**
 * Implements hook_submit() - After data has been inputted, submission will begin the batch operations.
 * @param $form
 * @param $form_state
 *
 */
function weather_batch_processing_form_submit($form, &$form_state)
{
    // If the data has been entered by hand, then do the following.
    if (!empty($form_state['values']['batch_processing_entry_field'])) {
        // Retrieve the values from $form_state and put them into a variable.
        $results = $form_state['values']['batch_processing_entry_field'];
        // Store data entered, into a session variable so that it can be
        // restored to the entry field after the submission.
        $_SESSION['weather_data_entered'] = $results;
        // Explode the results on a line break so that each entry can be processed individually.
        $results = explode("\r\n", $results);
    } else {
        // use ini_set to ensure that if line endings are broken (if the csv file was made in excel for example)
        // then this function will judge where the line endings should be, when file() is used on it.
        ini_set('auto_detect_line_endings', true);
        // If data was imported (uploaded file) then do the following.
        // Retrieve the values from $form_state and put them into a variable.  Specifically the location of the file.
        $file = $form_state['values']['csv_upload'];
        // Load the file from this location and store the array of data (created by file()) into another variable.
        $results = (file($file->destination));
    }

    $coordinates = [];

    // Loop through the results, which can now be treated the same regardless of origin.
    foreach ($results as $key => $result) {
        $key = $key+1;

        $result = trim($result);

        // Check to see if the data is empty but not at the end of the file.
        if (empty($result)) {
            if ($key < count($results)) {
                drupal_set_message(t("Line {$key} - holding data ('{$result}'), was empty."), 'error');
                continue;
            // If the last result is empty, don't bother to return an error message, just continue (in effect end.)
            } else {
                continue;
            }
        }

        //
        $result = str_getcsv($result, ",");

        // Trim the results and enter them into a new array of Lat and Long coordinates
        // to pass through some more validation checks.
        $inner['lat'] = trim($result[0]);
        $inner['long'] = trim($result[1]);

        // Check to see if the data is not numeric.
        if (!is_numeric($inner['lat']) || !is_numeric($inner['long'])) {
            drupal_set_message(t("Line {$key} - holding data ('{$inner['lat']}', '{$inner['long']}'), wasn't numeric and was omitted."), 'error');
        // If numeric, check to see that the values are within valid Latitudinal and Longitudinal range.
        } elseif (floatval($inner['lat']) >= -90
            && floatval($inner['lat']) <= 90
            && floatval($inner['long']) >= -180
            && floatval($inner['long']) <= 180) {
            // If it passes all the checks, enter the results into the coordinates array.
            $coordinates[] = $inner;
        } else {
            drupal_set_message(t("Line {$key} - holding data ('{$inner['lat']}', '{$inner['long']}'), was out of latitudinal and/or longitudinal range."), 'error');
        }
    }
    // Pass successful coordinates through to the batch importer.
    weather_batch_importer($coordinates);
}

/**
 * Implements the Batch API - This handles the importing process, connecting the functions used on batch processing,
 * and batch completion as well as how progress and errors are displayed.
 * @param $coordinates
 */
function weather_batch_importer($coordinates)
{
    $batch = [
        'title' => t('Importing'),
        'finished' => 'weather_batch_complete',
        'init_message' => t('Batch processing is starting.'),
        'progress_message' => t('Processed @current out of @total, @percentage% complete.'),
        'error_message' => t('Batch processing has encountered an error.'),
    ];

    // Loop through coordinates array and enter each coordinate into the batch processing function.
    foreach ($coordinates as $location) {
        $batch['operations'][] =
            [
                'weather_batch_processing', [$location]
            ];
    }
    batch_set($batch);
}

/**
 * Implements a Batch API processing function.  After the bulk coordinates of Latitude and Longitude are submitted
 * into the form, this handles the multiple requests to the openweathermap.org API.
 * @param $coordinates
 * @param $context
 */
function weather_batch_processing($coordinates, &$context)
{
    // Try to complete api request, if errors are found in the process, move onto the catch statement
    // and populate the table with city not found and empty strings.
    try {
        // Pass the coordinates into the weather_contents() function.
        $results = weather_contents($coordinates['lat'], $coordinates['long']);
        $results['coord'] = "{$coordinates['lat']}, {$coordinates['long']}";
        $context['results'][] = $results;
        $context['finished'] = 1;
    } catch (WeatherApiException $e) {
        drupal_set_message(t("City not found."), 'error');
        $context['results'][] = [
            'location' => t("City not found at {$coordinates['lat']}, {$coordinates['long']}."),
            'temperature' => "N/A",
            'current_weather' => "N/A",
            'humidity' => "N/A",
        ];
    }
}

/**
 * Implements a Batch API function to handle the data that's been imported.  In this case it's going to display the
 * information in a table.
 * @param $success
 * @param $results
 */
function weather_batch_complete($success, $results)
{
    if ($success) {
        // Define the table headers.
        $header = [
            ['data' => t('Location'), 'field' => 'location', 'sort' => 'ASC'],
            ['data' => t('Temperature'), 'field' => 'temperature', 'sort' => 'ASC'],
            ['data' => t('Current Weather'), 'field' => 'current_weather', 'sort' => 'ASC'],
            ['data' => t('Humidity'), 'field' => 'humidity', 'sort' => 'ASC'],
            ['data' => t('Coordinates'), 'field' => 'coord', 'sort' => 'ASC'],
            ];
        // Build the table.
        $weather_batch_table = [
            '#theme' => 'table',
            '#header' => $header,
            '#rows' => [],
        ];

        // Build the rows of data to populate the table.
        foreach ($results as $row) {
            if (isset($row['message'])) {
                unset($row['message']);
            }
            $weather_batch_table['#rows'][] =  $row;
        }

        // Store this data in a session to initially display the results but to overwrite them with unset()
        // when a new set of results are created.
        $_SESSION['weather_batch_table'] = $weather_batch_table;
    }
}

//     ------------------------------------- Admin/Config/Weather-Data -----------------------------------------

/**
 * Implements hook_form().
 * This function will create the configuration form to change City Weather Data visibility and cache expiration.
 *
 */
function weather_config_form($form, &$form_state)
{
    $form['introduction'] = [
        '#markup' => <<< HEREDOC
            <div>
                <h4>Welcome to the City Weather Data Configuration page.</h4> 
                <p>From here you can toggle the visibility of the form on/off<br> 
                and determine how long the search data is cached in the database for.</p>
                <p>If you toggle the visibility off, access to the <strong>city-weather-data</strong> and<br>
                 <strong>city-weather-data/results</strong> pages will show access denied.</p>
            </div>
HEREDOC
    ];

    $form['form_visibility'] = [
        '#type' => 'checkbox',
        '#description' => 'Select city weather form visibility.',
        '#required' => false,
        '#default_value' => variable_get('weather_form_visibility', true),
        '#title' => t('City Weather Form Visibility.'),
    ];

    $form['cache_expiry'] = [
        '#type' => 'textfield',
        '#description' => 'Enter your cache expiry duration in seconds.',
        '#required' => true,
        '#title' => t('Cache Expiry'),
        '#default_value' => variable_get('weather_cache_expiry', WEATHER_CACHE_DEFAULT),
    ];

    $form['submit'] = [
        '#type' => 'submit',
        '#value' => 'Submit',
    ];

    return $form;
}

/**
 * Implements a submit function to update the current state of form visibility and cache expiry date
 * submitted from the configuration form at admin/config/weather-data.
 */
function weather_config_form_submit(&$form, &$form_state)
{
    variable_set('weather_form_visibility', (bool) $form_state['values']['form_visibility']);
    variable_set('weather_cache_expiry', $form_state['values']['cache_expiry']);
}

//     ------------------------------------- Admin/Reports/Weather-Log -----------------------------------------

/**
 * Implements the handling of group (all) row deletion from the admin/reports/weather-log page (table).
 */
function weather_delete_all()
{
    $delete_all = db_delete('weather')
        ->execute();

    if ($delete_all > 0) {
        drupal_set_message("All records have been removed, I hope that's what you intended.");
    } else {
        drupal_set_message("There was a problem deleting the records.  Please consult google... I mean an expert.", 'error');
    }

    drupal_goto('/admin/reports/weather-log');
}

/**
 * Implements a function to handle individual row deletion from the admin/reports/weather-log page (table).
 */
function weather_delete_row($id)
{
    $delete_row = db_delete('weather')
        ->condition('submission_id', $id)
        ->execute();

    if (1 === $delete_row) {
        drupal_set_message("Record {$id} has been successfully deleted.");
    } else {
        drupal_set_message("There was a problem deleting the row, please consult an expert.", 'error');
    }

    drupal_goto('/admin/reports/weather-log');
}

/**
 * Implements custom function to build a table in admin/reports/weather-log to give administrators
 * access to view all the recorded searches using the City Weather Data form.
 */
function weather_reports()
{
    $header = [
        ['data' => t('Submission ID'), 'field' => 'submission_id', 'sort' => 'ASC'],
        ['data' => t('Latitude'), 'field' => 'latitude', 'sort' => 'ASC'],
        ['data' => t('Longitude'), 'field' => 'longitude', 'sort' => 'ASC'],
        ['data' => t('Request Timestamp'), 'field' => 'request_timestamp', 'sort' => 'ASC'],
        ['data' => t('User IP Address'), 'field' => 'user_ip_address', 'sort' => 'ASC'],
        ['data' => t('Location'), 'field' => 'location', 'sort' => 'ASC'],
        ['data' => t('Operations'), 'field' => 'operations'],
    ];

    // Query to populate fields with data using extend() to add some additional functionality such as the
    // pagination and sortable headers.
    $query = db_select('weather', 'w')
        ->extend('PagerDefault')->limit(10)
        ->extend('TableSort')->OrderByHeader($header)
        ->fields('w', [
            'submission_id',
            'latitude',
            'longitude',
            'request_timestamp',
            'user_ip_address',
            'location',
        ]);

    // These following commands execute the initial query, fetching them via the primary key.
    $result = $query->execute();
    $result = $result->fetchAllAssoc('submission_id');

    if (empty($result) === true) {
        $empty_report = [
            'opening_statement' => [
                '#markup' => <<< HEREDOC
                    <div>
                        <h3>There are no Weather Reports to show.</h3> 
                        <p>This is likely because you have just deleted the last row or the module has been freshly installed.<br>
                        In which case please make use of the fantastic Weather module and try again later.</p>
                        <p>If you still don't find any data here, or you've already been using the module<br>
                         and were expecting to see some data here, then something might have gone wrong.<br>
                        In which case there's a problem, and a pretty damn serious one at that. Sorry.</p>
                    </div>
HEREDOC
            ],
        ];

        return $empty_report;
    } else {
        $render_report = [
            [
                '#theme' => 'table',
                '#header' => $header,
                '#rows' => [],
            ],
            [
                '#theme' => 'pager',
            ],
            [
                '#theme' => 'link',
                '#text' => t('Reset'),
                '#path' => 'admin/reports/weather-log',
                '#options' => [
                    'attributes' => ['class' => 'button'],
                    'html' => true,
                ],
            ],
            [
                '#theme' => ['link'],
                '#text' => t('Delete All'),
                '#path' => 'admin/reports/weather-log/delete-all',
                '#options' => [
                    'attributes' => ['class' => 'button'],
                    'html' => true,
                ],
            ],
        ];

        // Alternative to using the foreach approaches below to iterate over the $results variable,
        // using array_map and type juggling to break the data into rows. - Version 3.
        $result = array_map(
            function ($row) {
                $row = (array)$row;
                $row['operations'] = "<a href=\"/admin/reports/weather-log/delete/{$row['submission_id']}\">Delete Row</a>";

                return $row;
            },
            $result
        );

        $render_report[0]['#rows'] = $result;

        return $render_report;
    }
}
