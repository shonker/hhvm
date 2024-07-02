<?hh

class A {}
class B extends A {}

<<__EntryPoint>>
function main(): void {
  $a = new A();
  // small test exercising the class(A) var_dump behavior
  var_dump(get_class($a));
  var_dump(get_parent_class(B::class));
}
