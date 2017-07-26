<?php

/**
 * Interface Shape - The parent template implemented by the classes below. The function getArea() must be called to
 * implement this interface.
 */
interface Shape
{
    public function getArea();
}

/**
 * Class Quadrangle
 * An abstract class used by (Classes) Square and Rectangle, to define those instances of the function getArea().
 */
abstract class Quadrangle implements Shape
{
    public $width;
    public $height;

    /**
     * @return number - returns a numerical value of the total calculated area for the chosen shape.
     */
    public function getArea()
    {
        return $this->width * $this->height;
    }
}

/**
 * Class Square
 */
class Square extends Quadrangle
{
    /**
     * Additional validity checks are made here using exceptions to catch unsuitable inputs
     * and display appropriate error messages.
     * @param number $width - takes numerical value(s) into the function getArea() to calculate the total area.
     */
    public function __construct($width)
    {
        if (!is_numeric($width)) {
            throw new InvalidArgumentException("Invalid parameter type: expected numeric, received " . gettype($width) . ".\n" . "Please use a positive, numerical value (I.e 1, not 'One').");
        }
        if (0 >= $width) {
            throw new ShapeException("Invalid Width: " . $width . " for " . __CLASS__ . ". Please use a positive number.");
        }
        $this->width = $this->height = $width;
    }
}

/**
 * Class Rectangle
 * Constructs an instance of the getArea() function inherited from the abstract class Quadrangle.
 * Additional validity checks are made here using exceptions to catch unsuitable inputs
 * and display appropriate error messages.
 */
class Rectangle extends Quadrangle
{

    /**
     * @param int $width & $height - takes numerical value(s) into the function getArea() to calculate the total area.
     */
    public function __construct($width, $height)
    {
        if (!is_numeric($width)) {
            throw new InvalidArgumentException("Invalid parameter type: expected numeric, received " . gettype($width) . ".\n" . "Please use a positive, numerical value (I.e 1, not 'One').");
        }
        if (!is_numeric($height)) {
            throw new InvalidArgumentException("Invalid parameter type: expected numeric, received " . gettype($height) . ".\n" . "Please use a positive, numerical value (I.e 1, not 'One').");
        }
        if (0 >= $width) {
            throw new ShapeException("Invalid Width: " . $width . " for " . __CLASS__ . ". Please use a positive number.");
        }
        if (0 >= $height) {
            throw new ShapeException("Invalid Height: " . $height . " for " . __CLASS__ . ". Please use a positive number.");
        }
        $this->width = $width;
        $this->height = $height;
    }
}

/**
 * Class Circle
 * Unlike classes Square and Rectangle (children of Quadrangle), Circle implements Shape directly.
 * It makes the same checks for valid data but also defines the getArea() function.
 */
class Circle implements Shape
{
    public $radius;

    /**
     * Circle constructor.
     * @param - number $radius - takes numerical value(s) into the function getArea() to calculate the total area.
     */
    public function __construct($radius)
    {
        if (!is_numeric($radius)) {
            throw new InvalidArgumentException("Invalid parameter type: expected numeric, received " . gettype($radius) . ".\n" . "Please use a positive, numerical value (I.e 1, not 'One').");
        }
        if (0 >= $radius) {
            throw new ShapeException("Invalid Radius: " . $radius . " for " . __CLASS__ . ". Please use a positive number.");
        }
        $this->radius = $radius;
    }

    /**
     * @return number - a numerical value of the total calculated area for the chosen shape.
     */
    public function getArea()
    {
        return pow($this->radius, 2) * pi();
    }
}

/**
 * Class ShapeException - filters the base Exception class to check specifically for OutOfRangeExceptions.
 * This means we can be clearer on which errors are caught and describe them back to the user/developer more accurately.
 */
class ShapeException extends OutOfRangeException {

}

/**
 * The code within the try parentheses will be run, while the catch blocks look for caught exceptions,
 * echoing out the results (if any) using the inherited object methods selected by the error statement.
 */
try {
    $square = new Square();
    //echo "The area of the square is " . $square->getArea() . "\n";

    $rectangle = new Rectangle(5, 4);
    //echo "The area of the rectangle is " . $rectangle->getArea() . "\n";

    $circle = new Circle(3);
    //echo "The area of the circle is " . $circle->getArea() . "\n";

    //$quadrangle = new Quadrangle(5);
    //echo "The area of the quadrangle is " . $quadrangle->getArea() . "\n";

    shapePrinter($square);
    shapePrinter($rectangle);
    shapePrinter($circle);

} catch (ShapeException $e) {
    echo "Caught " . get_class($e) . ": " . $e->getMessage() . "\n" . "This error was thrown on line " . $e->getLine() . " of file: " . $e->getFile() . ".\n";
} catch (InvalidArgumentException $e) {
    echo "Caught " . get_class($e) . ": " . $e->getMessage() . "\n" . "This error was thrown on line " . $e->getLine() . " of file: " . $e->getFile() . ".\n";
}

/**
 * This function can be used to run the getArea() function from any class that implements the Shape interface.
 * @param Shape $shape - accepts a new instance of the classes declared above and uses the appropriate
 * getArea() function to calculate the total area.
 * @return number - of the calculated area for the selected shape.
 */
function shapePrinter(Shape $shape)
{
    echo "The area of the " . get_class($shape) . " is " . $shape->getArea(). "\n";
}
