<?php

//$a = "variable";
//$variable = "second variable";
//
//echo $$a;

//$numbers = [1,2,3,4,5,6];
//
//echo print_r($numbers) . "\n";
//
//$a = array_shift($numbers);
//echo 'array_shift($numbers) $a = ' . $a . "\n";
//echo print_r($numbers) . "\n";
//
//$b = array_unshift($numbers, 'first new item');
//echo 'array_unshift($numbers, first new item) $b = ' . $b . "\n";
//echo print_r($numbers) . "\n";
//
//$c = array_pop($numbers);
//echo 'array_pop($numbers) $c = ' . $c . "\n";
//echo print_r($numbers) . "\n";
//
//$d = array_push($numbers, 'second new item');
//echo 'array_push($numbers, second new item) $d = ' . $d . "\n";
//echo print_r($numbers) . "\n";

//$timestamp = time();
//echo strftime("The date today %d %m %Y.", $timestamp);

interface TableInterface {
    public function save(array $data);
}

interface LogInterface {
    public function log($message);
}

class Table implements TableInterface, LogInterface, Countable {
    public function save(array $data)
    {
        return "foo";
    }
    public function log($message)
    {
        return "bar";
    }
    public function count() {
    return 10;
    }
}

echo (new Table())->save([]);
echo (new Table())->log('hello');
echo (new Table())->count();
/**
 * Built in Interfaces
 *
 * Countable, OuterIterator, SplObserver, RecursiveIterator, SplSubject, SeekableIterator.
 */
