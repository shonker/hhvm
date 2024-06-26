<?hh
/* Prototype  : array array_combine(array $keys, array $values)
 * Description: Creates an array by using the elements of the first parameter as keys
 *              and the elements of the second as the corresponding values
 * Source code: ext/standard/array.c
*/

/*
* Testing the behavior of array_combine() by passing array with
* binary values for $keys and $values argument.
*/
<<__EntryPoint>> function main(): void {
echo "*** Testing array_combine() : binary safe checking ***\n";

// array with binary values
$arr_binary = vec[b"hello", b"world"];
$arr_normal = vec["hello", "world"];

// array with binary value for $keys and $values argument
var_dump( array_combine($arr_binary, $arr_binary) );

// array with binary value for $values argument
var_dump( array_combine($arr_normal, $arr_binary) );

// array with binary value for $keys argument
var_dump( array_combine($arr_binary, $arr_normal) );

echo "Done";
}
