<?php

/**
 * This function syncs the London weather block to the cron, it's set to update every hour and store that value
 * in the database for 3 hours, in case the api openweathermap.org is unavailable.  Should the cached value expire
 * without being updated an error message is given instead (see weather_block_view),
 * so the stated weather is never too outdated.
 */
function weather_cronapi()
{
    return [
        'ultimate_weather_cron' => [
            'title' => 'Ultimate Weather Cron',
            'callback' => 'weather_retrieve_london_weather',
            'scheduler' => [
                'name' => 'crontab',
                'crontab' => [
                    'rules' => [
                        '0 * * * *'
                    ],
                ],
            ],
        ],
    ];
}

/**
 * This function is used to run the weather_contents function and make an api call to openweathermap.org,
 * for london weather results and cache_set those results for use in the block.
 */
function weather_retrieve_london_weather()
{
    try {
        $weather_results = weather_contents(51.5, 0.12);

        // Setting default cache expiry to ensure this doesn't change with the config options.
        cache_set('weather_london', $weather_results, 'cache_weather', (time() + 10800));
    } catch (WeatherApiException $e){
        watchdog_exception('weather', $e);
    }
}
