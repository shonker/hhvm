// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the "hack" directory of this source tree.
use triomphe::OffsetArc;

/// This definition exists for ffi_cbindgen C++ header generation. Any
/// attempt to call this function will result in an intentional
/// unresolved symbol at link time.
#[no_mangle]
pub extern "C" fn no_call_compile_only_USED_TYPES_ffi(
    _: Maybe<i32>,
    _: Vector<u8>,
    _: OffsetArc<Vector<u8>>,
) {
    unimplemented!()
}
