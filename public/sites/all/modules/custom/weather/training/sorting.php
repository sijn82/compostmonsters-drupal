<?php

$fruits = ["peach", "grape", "banana", "orange", "apple", "grapefruit", "cherry", "pomegranate", "coconut", "fig"];

// Write some code here to sort the array alphabetically
// You are not allowed to use sort()
// Use usort() instead: https://secure.php.net/manual/en/function.usort.php

//usort($fruits, 'strcasecmp'
//    function ($a, $b) {
//        return strcasecmp($a, $b);
//    }
//);

function sortinator3000($a, $b)
{
    if (strlen($a) < strlen($b)) {
        return -1;
    } elseif (strlen($a) > strlen($b)) {
        return 1;
    } else{
        return strcmp($a, $b);
    }
}

usort($fruits, 'sortinator3000');


var_dump($fruits);
