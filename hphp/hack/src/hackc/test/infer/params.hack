// RUN: %hackc compile-infer --fail-fast --keep-going %s | FileCheck %s

class Internal {}

class InternalGeneric<T> {}

// TEST-CHECK-BAL: define $root.internalClassParam
// CHECK: define $root.internalClassParam($this: *void, $a: .notnull *HackInt, $b: *Internal) : *Internal {
// CHECK: local $0: *void, $1: *void, $2: *void
// CHECK: #b0:
// CHECK:   n0 = __sil_lazy_class_initialize(<Internal>)
// CHECK:   store &$0 <- n0: *HackMixed
// CHECK:   n1 = __sil_allocate(<Internal>)
// CHECK:   n2 = Internal._86pinit(n1)
// CHECK:   store &$2 <- n1: *HackMixed
// CHECK:   n3: *HackMixed = load &$0
// CHECK:   n4: *HackMixed = load &$0
// CHECK:   n5: *HackMixed = load &$2
// CHECK:   jmp b2
// CHECK:   .handlers b1
// CHECK: #b1(n6: *HackMixed):
// CHECK:   store &$0 <- null: *HackMixed
// CHECK:   store &$1 <- null: *HackMixed
// CHECK:   store &$2 <- null: *HackMixed
// CHECK:   throw n6
// CHECK: #b2:
// CHECK:   store &$0 <- null: *HackMixed
// CHECK:   store &$1 <- null: *HackMixed
// CHECK:   store &$2 <- null: *HackMixed
// CHECK:   n7 = n5.?.__construct()
// CHECK:   n8 = $builtins.hhbc_lock_obj(n5)
// CHECK:   n9 = $builtins.hack_bool(__sil_instanceof(n5, <Internal>, 0))
// CHECK:   n10 = $builtins.hhbc_verify_type_pred(n5, n9)
// CHECK:   ret n5
// CHECK: }
function internalClassParam(int $a, Internal $b) : Internal {
  return new Internal();
}

// TEST-CHECK-BAL: define $root.externalClassParam
// CHECK: define $root.externalClassParam($this: *void, $a: .notnull *HackBool, $b: *External) : *External {
// CHECK: local $0: *void, $1: *void, $2: *void
// CHECK: #b0:
// CHECK:   n0 = __sil_lazy_class_initialize(<External>)
// CHECK:   store &$0 <- n0: *HackMixed
// CHECK:   n1 = __sil_allocate(<External>)
// CHECK:   n2 = External._86pinit(n1)
// CHECK:   store &$2 <- n1: *HackMixed
// CHECK:   n3: *HackMixed = load &$0
// CHECK:   n4: *HackMixed = load &$0
// CHECK:   n5: *HackMixed = load &$2
// CHECK:   jmp b2
// CHECK:   .handlers b1
// CHECK: #b1(n6: *HackMixed):
// CHECK:   store &$0 <- null: *HackMixed
// CHECK:   store &$1 <- null: *HackMixed
// CHECK:   store &$2 <- null: *HackMixed
// CHECK:   throw n6
// CHECK: #b2:
// CHECK:   store &$0 <- null: *HackMixed
// CHECK:   store &$1 <- null: *HackMixed
// CHECK:   store &$2 <- null: *HackMixed
// CHECK:   n7 = n5.?.__construct()
// CHECK:   n8 = $builtins.hhbc_lock_obj(n5)
// CHECK:   n9 = $builtins.hack_bool(__sil_instanceof(n5, <External>, 0))
// CHECK:   n10 = $builtins.hhbc_verify_type_pred(n5, n9)
// CHECK:   ret n5
// CHECK: }
function externalClassParam(bool $a, External $b): External {
  return new External();
}

// TEST-CHECK-BAL: define .async $root.genericParams
// CHECK: define .async $root.genericParams($this: *void, $a: .notnull *HackString, $b: *InternalGeneric) : .awaitable .notnull *HackInt {
// CHECK: #b0:
// CHECK:   n0 = $builtins.hack_new_dict($builtins.hack_string("kind"), $builtins.hack_int(101), $builtins.hack_string("classname"), $builtins.hack_string("InternalGeneric"), $builtins.hack_string("generic_types"), $builtins.hhbc_new_vec($builtins.hack_new_dict($builtins.hack_string("kind"), $builtins.hack_int(4))))
// CHECK:   n1: *HackMixed = load &$b
// CHECK:   n2 = $builtins.hhbc_verify_param_type_ts(n1, n0)
// CHECK:   n3 = $builtins.hhbc_is_type_int($builtins.hack_int(42))
// CHECK:   n4 = $builtins.hhbc_verify_type_pred($builtins.hack_int(42), n3)
// CHECK:   ret $builtins.hack_int(42)
// CHECK: }
async function genericParams(string $a, InternalGeneric<string> $b): Awaitable<int> {
  return 42;
}

// TEST-CHECK-BAL: define $root.softParam
// CHECK: define $root.softParam($this: *void, $a: *HackMixed) : *void {
// CHECK: #b0:
// CHECK:   n0: *HackMixed = load &$a
// CHECK:   n1 = $builtins.hhbc_print(n0)
// CHECK:   ret null
// CHECK: }
function softParam(<<__Soft>> string $a): void {
  echo $a;
}

// TEST-CHECK-BAL: define $root.likeParam
// CHECK: define $root.likeParam($this: *void, $a: .notnull *HackString) : *void {
// CHECK: #b0:
// CHECK:   n0: *HackMixed = load &$a
// CHECK:   n1 = $builtins.hhbc_print(n0)
// CHECK:   ret null
// CHECK: }
function likeParam(~string $a): void {
  echo $a;
}

// TEST-CHECK-BAL: define $root.genericRet
// CHECK: define $root.genericRet($this: *void, $a: .typevar="T" *HackMixed) : .typevar="T" *HackMixed {
// CHECK: #b0:
// CHECK:   n0: *HackMixed = load &$a
// CHECK:   ret n0
// CHECK: }
function genericRet<T>(T $a): T {
  return $a;
}

// TEST-CHECK-BAL: define .async $root.genericAsyncRet
// CHECK: define .async $root.genericAsyncRet($this: *void, $a: .typevar="T" *HackMixed) : .awaitable .typevar="T" *HackMixed {
// CHECK: #b0:
// CHECK:   n0: *HackMixed = load &$a
// CHECK:   ret n0
// CHECK: }
async function genericAsyncRet<T>(T $a): Awaitable<T> {
  return $a;
}
