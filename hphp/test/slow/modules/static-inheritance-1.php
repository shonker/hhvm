<?hh


module foo;

<<__EntryPoint>>
function main(): void {
  include "static-inheritance-1.inc";
  include "static-inheritance-1.inc1";
  $x = new Child();
  $x->doStuff();
  var_dump($x->doStuff2()->get2());
}
