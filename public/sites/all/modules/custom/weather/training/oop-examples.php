<?php

class Myclass
{
    public $prop1 = "I'm a class property!";

    public static $count = 0;

    public function __construct()
    {
        echo 'The class "', __CLASS__, '" was initiated!' . "\n";
    }

    public function __destruct()
    {
        echo 'The class "', __CLASS__, '" was destroyed.' . "\n";
    }

    public function __toString()
    {
        echo "Using the toString method: ";
        return $this->getProperty();
    }

    public function setProperty($newval)
    {
        $this->prop1 = $newval;
    }

    private function getProperty()
    {
        return $this->prop1 . "\n";
    }

    public static function plusOne()
    {
        return "The count is " . ++self::$count . ".\n";
    }
}

class MyOtherClass extends MyClass
{
    public function __construct()
    {
        // Call the parent class's constructor.
        parent::__construct();
        echo "A new constructor in " . __CLASS__ . ".\n";
    }

    public function newMethod()
    {
        echo "From a new method in " . __CLASS__ .  ". \n";
    }

    public function callProtected()
    {
        return $this->getProperty();
    }
}

do {
    // Call plusOne without instantiating MyClass
    echo MyClass::plusOne();
} while ( MyClass::$count < 10);



//                        --------------------------- Example 1 ---------------------------

//$obj = new Myclass;

// ------- Get property value -------
//echo $obj->getProperty();

// ------- Set a new one -------
//$obj->setProperty("I'm a new property value");

// ------- Read it out again to show the change -------
//echo $obj->getProperty();

//                        --------------------------- Example 2 ---------------------------

// ------- Create two objects -------
//$obj = new MyClass;
//$obj2 = new MyClass;

// ------- Get the value of $prop1 from both objects -------
//echo $obj->getProperty();
//echo $obj2->getProperty();

// ------- Set new values for both objects -------
//$obj->setProperty("I'm a new property value!");
//$obj2->setProperty("I belong to the second instance");

// ------- Output both objects' $prop1 value -------
//echo $obj->getProperty();
//echo $obj2->getProperty();

//                        --------------------------- Example 3 ---------------------------

// ------- Create a new object -------
//$obj = new MyClass;

// ------- Get the value of $prop1 -------
//echo $obj;

// ------- Destroy the object -------
//unset($obj);

// ------- Output a message at the end of the file -------
//echo "End of file.<br>";

//                        --------------------------- Example 4 ---------------------------

// ------- Create a new object -------
//$newObj = new MyOtherClass;

// ------- Output the object as a string -------
//echo $newObj->newMethod();

// ------- Use a method from the parent class -------
//echo $newObj->callProtected();

//                        --------------------------- Example 5 ---------------------------

class foo {
    public $value = 42;

    public function &getValue()
    {
        return $this->value;
    }
}

$obj = new foo;
$myValue =& $obj->getValue();
$obj->value = 2;
echo $myValue;

//                       --------------------------- Test Examples ---------------------------

// A
$variable = "initial value";
echo "$variable\n";

function funcA($param)
{
    $param = "new value A";
    return $param;
}

$variable = funcA($variable);
echo "$variable\n";

// Answer: "initial value",
//         "new value A"

// B
$variable = "initial value";
echo "$variable\n";

function funcB(&$param)
{
    $param = "new value B";
}

funcB($variable);
echo "$variable\n";

// Answer: "initial value",
//         "new value B"

// Difference: $variable in 'A' only retains the effects of funcA because it's assigned outside of the function.
//             In B the '&' connects the two variables globally.

<?php

$fruits = ["peach", "grape", "banana", "orange", "apple", "grapefruit", "cherry"];

// Write some code here to sort the array alphabetically
// You are not allowed to use sort()
// Use usort() instead: https://secure.php.net/manual/en/function.usort.php


var_dump($fruits);
