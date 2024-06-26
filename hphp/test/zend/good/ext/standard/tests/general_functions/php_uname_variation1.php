<?hh
/* Prototype: string php_uname  ([ string $mode  ] )
 * Description:  Returns information about the operating system PHP is running on
*/

class fooClass {
   function __toString() :mixed{
       return "m";
   }
}
<<__EntryPoint>> function main(): void {
echo "*** Testing php_uname() - usage variations\n";
// Prevent notices about undefines variables
error_reporting(E_ALL & ~E_NOTICE);


$values = dict[

          // int data
          "0" => 0,
          "1" => 1,
          "12345" =>  12345,
          "-2345" =>  -2345,

          // float data
          "10.5" => 10.5,
          "-10.5" => -10.5,
          "10.1234567e10" => 10.1234567e10,
          "10.7654321E-10" => 10.7654321E-10,
          ".5" => .5,

          // null data
          "NULL" => NULL,
          "null" =>  null,

          // boolean data
          "true" => true,
          "false" => false,
          "TRUE" => TRUE,
          "FALSE" => FALSE,

          // empty data
          "\"\"" => "",
          "''" => '',

          // object data
          "new fooClass()" => new fooClass(),


];

// loop through each element of the array for data

foreach($values as $key => $value) {
      echo "-- Iterator $key --\n";
      try { var_dump( php_uname($value) ); } catch (Exception $e) { echo 'ERROR: '; var_dump($e->getMessage()); }
};

echo "===DONE===\n";
}
