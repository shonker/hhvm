// RUN: %hackc compile-infer --fail-fast %s | FileCheck %s

// TEST-CHECK-BAL: define $root.check_foreach
// CHECK: define $root.check_foreach($this: *void, $x: .notnull *HackVec) : *void {
// CHECK: local $index: *void, iter0: *void, $0: *void
// CHECK: #b0:
// CHECK:   n0 = $builtins.hack_new_dict($builtins.hack_string("kind"), $builtins.hack_int(20), $builtins.hack_string("generic_types"), $builtins.hhbc_new_vec($builtins.hack_new_dict($builtins.hack_string("kind"), $builtins.hack_int(4))))
// CHECK:   n1: *HackMixed = load &$x
// CHECK:   n2 = $builtins.hhbc_verify_param_type_ts(n1, n0)
// CHECK:   n3: *HackMixed = load &$x
// CHECK:   n4 = $builtins.hhbc_iter_base(n3)
// CHECK:   store &$0 <- n4: *HackMixed
// CHECK:   jmp b1
// CHECK: #b1:
// CHECK:   n5: *HackMixed = load &$0
// CHECK:   n6 = $builtins.hhbc_iter_init(&iter0, null, &$index, n5)
// CHECK:   jmp b2, b7
// CHECK:   .handlers b9
// CHECK: #b2:
// CHECK:   prune $builtins.hack_is_true(n6)
// CHECK:   jmp b3
// CHECK: #b3:
// CHECK:   n7: *HackMixed = load &$index
// CHECK:   n8 = $builtins.hhbc_print(n7)
// CHECK:   n9: *HackMixed = load &$0
// CHECK:   n10: *HackMixed = load &iter0
// CHECK:   n11 = $builtins.hhbc_iter_next(n10, null, &$index, n9)
// CHECK:   jmp b5, b6
// CHECK:   .handlers b4
// CHECK: #b4(n12: *HackMixed):
// CHECK:   n13: *HackMixed = load &iter0
// CHECK:   n14 = $builtins.hhbc_iter_free(n13)
// CHECK:   throw n12
// CHECK:   .handlers b9
// CHECK: #b5:
// CHECK:   prune $builtins.hack_is_true(n11)
// CHECK:   jmp b8
// CHECK: #b6:
// CHECK:   prune ! $builtins.hack_is_true(n11)
// CHECK:   jmp b3
// CHECK: #b7:
// CHECK:   prune ! $builtins.hack_is_true(n6)
// CHECK:   jmp b8
// CHECK: #b8:
// CHECK:   jmp b10
// CHECK:   .handlers b9
// CHECK: #b9(n15: *HackMixed):
// CHECK:   store &$0 <- null: *HackMixed
// CHECK:   throw n15
// CHECK: #b10:
// CHECK:   store &$0 <- null: *HackMixed
// CHECK:   store &$0 <- null: *HackMixed
// CHECK:   ret null
// CHECK: }
function check_foreach(vec<string> $x): void {
  foreach ($x as $index) {
    echo $index;
  }
}
