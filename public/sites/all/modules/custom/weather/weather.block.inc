<?php

/**
 * Implements hook_block_info().
 * 'caching' => used to prevent the module sending data on each page load.
 */
function weather_block_info()
{
    $blocks = [];

    $blocks['weather'] = [
        'info' => t('London Weather'),
    ];
    return $blocks;
}

/**
 * Implements hook_block_view(). If cached results can be found they are stored in the $weather_cached_results variable,
 * and outputted to the block. Otherwise if api request is anything but 200 (successful), or no request has been made
 * through the cron and so returns a null value for the weather_results['message'],
 * 'Weather Unavailable - Unable to retrieve data...' is shown instead.
 */
function weather_block_view($delta = '')
{
    $block = [];

    switch ($delta) {
        case 'weather':
            $weather_cache_results = cache_get('weather_london', 'cache_weather');

            if (false === $weather_cache_results) {
                $block['subject'] = t('Weather unavailable');
                $block['content'] = [
                    '#markup' => '<p>' . t('Unable to retrieve data, please try again later.') . '</p>',
                ];
            } else {
                $block['subject'] = t('Weather in London');
                $block['content'] = [
                    '#theme' => 'weather_info',
                    '#temperature' => $weather_cache_results->data['temperature'],
                    '#current_weather' => $weather_cache_results->data['current_weather'],
                    '#humidity' => $weather_cache_results->data['humidity'],
                ];
            }
            break;
    }
    return $block;
}
