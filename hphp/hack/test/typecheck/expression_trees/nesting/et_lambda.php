<?hh
<<file: __EnableUnstableFeatures('expression_trees')>>
<<file: __EnableUnstableFeatures('expression_tree_nest')>>

<<__EntryPoint>>
function f(): void {
  $y = ExampleDsl`() ==> ${ExampleDsl`1`}`; // Ok
  $y = ExampleDsl`() ==> ${`1`}`; // Error: nested ET without DSL
  $y = ExampleDsl`() ==> {$x = 1; return ${ExampleDsl`$x`};}`; // Error: unbound $x
  $y = ExampleDsl`() ==> {$x = 1; return ${$x};}`; // Error: unbound $x
  $y = ExampleDsl`() ==> {$x = 1; return $x;}`; // Ok
}
