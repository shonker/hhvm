<?hh

class C {}

<<__EntryPoint>>
function main(): void {
  $w = HH\class_to_classname(
    __hhvm_intrinsics\launder_value("C")
  );
  __hhvm_intrinsics\debug_var_dump_lazy_class($w);
  $x = HH\class_to_classname(
    __hhvm_intrinsics\launder_value(trim("C "))
  );
  __hhvm_intrinsics\debug_var_dump_lazy_class($x);

  $y = C::class;
  $y = HH\class_to_classname(
    __hhvm_intrinsics\launder_value($y)
  );
  __hhvm_intrinsics\debug_var_dump_lazy_class($y);

  $z = HH\classname_to_class(C::class);
  $z = HH\class_to_classname(
    __hhvm_intrinsics\launder_value($z)
  );
  __hhvm_intrinsics\debug_var_dump_lazy_class($z);

  HH\class_to_classname(
    __hhvm_intrinsics\launder_value(3)
  );
}
