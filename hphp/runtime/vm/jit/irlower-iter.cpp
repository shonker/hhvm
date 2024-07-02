/*
   +----------------------------------------------------------------------+
   | HipHop for PHP                                                       |
   +----------------------------------------------------------------------+
   | Copyright (c) 2010-present Facebook, Inc. (http://www.facebook.com)  |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
*/

#include "hphp/runtime/vm/jit/irlower-internal.h"

#include "hphp/runtime/base/builtin-functions.h"
#include "hphp/runtime/base/execution-context.h"
#include "hphp/runtime/base/object-data.h"
#include "hphp/runtime/base/tv-mutate.h"
#include "hphp/runtime/base/tv-variant.h"
#include "hphp/runtime/base/typed-value.h"
#include "hphp/runtime/base/vanilla-vec.h"
#include "hphp/runtime/vm/act-rec.h"
#include "hphp/runtime/vm/iter.h"

#include <folly/container/Array.h>

#include "hphp/runtime/vm/jit/abi.h"
#include "hphp/runtime/vm/jit/arg-group.h"
#include "hphp/runtime/vm/jit/array-iter-profile.h"
#include "hphp/runtime/vm/jit/bc-marker.h"
#include "hphp/runtime/vm/jit/call-spec.h"
#include "hphp/runtime/vm/jit/extra-data.h"
#include "hphp/runtime/vm/jit/ir-instruction.h"
#include "hphp/runtime/vm/jit/ir-opcode.h"
#include "hphp/runtime/vm/jit/ssa-tmp.h"
#include "hphp/runtime/vm/jit/target-profile.h"
#include "hphp/runtime/vm/jit/translator-inline.h"
#include "hphp/runtime/vm/jit/type.h"
#include "hphp/runtime/vm/jit/types.h"
#include "hphp/runtime/vm/jit/vasm-gen.h"
#include "hphp/runtime/vm/jit/vasm-instr.h"
#include "hphp/runtime/vm/jit/vasm-reg.h"

#include "hphp/util/trace.h"

namespace HPHP::jit::irlower {

TRACE_SET_MOD(irlower);

///////////////////////////////////////////////////////////////////////////////

namespace {

///////////////////////////////////////////////////////////////////////////////

int iterOffset(const BCMarker& marker, uint32_t id) {
  auto const func = marker.func();
  return -cellsToBytes(((id + 1) * kNumIterCells + func->numLocals()));
}

void implIterInit(IRLS& env, const IRInstruction* inst) {
  auto const isInitK = inst->is(IterInitArrK, IterInitObjK);
  auto const extra = &inst->extra<IterData>()->args;

  auto const src = inst->src(0);
  auto const fp = srcLoc(env, inst, 1).reg();
  auto const iterOff = iterOffset(inst->marker(), extra->iterId);
  auto const valOff = localOffset(extra->valId);

  auto& v = vmain(env);

  if (src->isA(TArrLike)) {
    auto args = argGroup(env, inst)
      .addr(fp, iterOff)
      .ssa(0 /* src */)
      .addr(fp, valOff);
    if (isInitK) {
      args.addr(fp, localOffset(extra->keyId));
    }

    auto const baseConst = has_flag(extra->flags, IterArgs::Flags::BaseConst);
    auto const target = isInitK
      ? CallSpec::direct(new_iter_array_key_helper(baseConst))
      : CallSpec::direct(new_iter_array_helper(baseConst));
    cgCallHelper(v, env, target, callDest(env, inst), SyncOptions::None, args);
    return;
  }

  always_assert(src->type() <= TObj);

  auto args = argGroup(env, inst)
    .ssa(0 /* src */)
    .addr(fp, valOff);
  if (isInitK) {
    args.addr(fp, localOffset(extra->keyId));
  } else {
    args.imm(0);
  }

  auto const target = CallSpec::direct(new_iter_object);
  cgCallHelper(v, env, target, callDest(env, inst), SyncOptions::Sync, args);
}

void implIterNext(IRLS& env, const IRInstruction* inst, CallSpec target) {
  always_assert(inst->is(IterNextArr, IterNextArrK,
                         IterNextObj, IterNextObjK));
  auto const isArr = inst->is(IterNextArr, IterNextArrK);
  auto const isKey = inst->is(IterNextArrK, IterNextObjK);
  auto const extra = &inst->extra<IterData>()->args;

  auto const sync = isArr ? SyncOptions::None : SyncOptions::Sync;
  auto const args = [&] {
    auto const fp = srcLoc(env, inst, 1).reg();
    auto ret = argGroup(env, inst);
    if (isArr) ret.addr(fp, iterOffset(inst->marker(), extra->iterId));
    ret.ssa(0);
    ret.addr(fp, localOffset(extra->valId));
    if (isKey) ret.addr(fp, localOffset(extra->keyId));
    return ret;
  }();

  auto& v = vmain(env);
  cgCallHelper(v, env, target, callDest(env, inst), sync, args);
}

///////////////////////////////////////////////////////////////////////////////

}

///////////////////////////////////////////////////////////////////////////////

namespace {

template<typename T>
Vptr iteratorPtr(IRLS& env, const IRInstruction* inst, const T* extra) {
  assertx(inst->src(0)->isA(TFramePtr));
  auto const fp = srcLoc(env, inst, 0).reg();
  return fp[iterOffset(inst->marker(), extra->iterId)];
}

}

void cgLdIterPos(IRLS& env, const IRInstruction* inst) {
  static_assert(IterImpl::posSize() == 8, "");
  auto const dst  = dstLoc(env, inst, 0).reg();
  auto const iter = iteratorPtr(env, inst, inst->extra<LdIterPos>());
  vmain(env) << load{iter + IterImpl::posOffset(), dst};
}

void cgLdIterEnd(IRLS& env, const IRInstruction* inst) {
  static_assert(IterImpl::endSize() == 8, "");
  auto const dst  = dstLoc(env, inst, 0).reg();
  auto const iter = iteratorPtr(env, inst, inst->extra<LdIterEnd>());
  vmain(env) << load{iter + IterImpl::endOffset(), dst};
}

void cgStIterPos(IRLS& env, const IRInstruction* inst) {
  static_assert(IterImpl::posSize() == 8, "");
  auto const src  = srcLoc(env, inst, 1).reg();
  auto const iter = iteratorPtr(env, inst, inst->extra<StIterPos>());
  vmain(env) << store{src, iter + IterImpl::posOffset()};
}

void cgStIterEnd(IRLS& env, const IRInstruction* inst) {
  static_assert(IterImpl::endSize() == 8, "");
  auto const src  = srcLoc(env, inst, 1).reg();
  auto const iter = iteratorPtr(env, inst, inst->extra<StIterEnd>());
  vmain(env) << store{src, iter + IterImpl::endOffset()};
}

void cgKillIter(IRLS& env, const IRInstruction* inst) {
  auto& v = vmain(env);
  v << killeffects{};
  if (!debug) return;

  int32_t trash;
  memset(&trash, kIterTrashFill, sizeof(trash));
  auto const iter = iteratorPtr(env, inst, inst->extra<KillIter>());
  for (auto i = 0; i < sizeof(IterImpl); i += sizeof(trash)) {
    v << storeli{trash, iter + i};
  }
}

///////////////////////////////////////////////////////////////////////////////

void cgProfileIterInit(IRLS& env, const IRInstruction* inst) {
  assertx(inst->src(0)->type().subtypeOfAny(TVec, TDict, TKeyset));
  auto const extra = inst->extra<RDSHandleData>();
  auto const args = argGroup(env, inst)
    .addr(rvmtl(), safe_cast<int32_t>(extra->handle))
    .ssa(0);
  cgCallHelper(vmain(env), env, CallSpec::method(&ArrayIterProfile::update),
               kVoidDest, SyncOptions::None, args);
}

void cgIterInitArr(IRLS& env, const IRInstruction* inst) {
  implIterInit(env, inst);
}

void cgIterInitArrK(IRLS& env, const IRInstruction* inst) {
  implIterInit(env, inst);
}

void cgIterInitObj(IRLS& env, const IRInstruction* inst) {
  implIterInit(env, inst);
}

void cgIterInitObjK(IRLS& env, const IRInstruction* inst) {
  implIterInit(env, inst);
}

void cgIterNextArr(IRLS& env, const IRInstruction* inst) {
  auto const flags = inst->extra<IterData>()->args.flags;
  auto const target = has_flag(flags, IterArgs::Flags::BaseConst)
    ? CallSpec::direct(iter_next_array<true>)
    : CallSpec::direct(iter_next_array<false>);
  implIterNext(env, inst, target);
}

void cgIterNextArrK(IRLS& env, const IRInstruction* inst) {
  auto const flags = inst->extra<IterData>()->args.flags;
  auto const target = has_flag(flags, IterArgs::Flags::BaseConst)
    ? CallSpec::direct(iter_next_array_key<true>)
    : CallSpec::direct(iter_next_array_key<false>);
  implIterNext(env, inst, target);
}

void cgIterNextObj(IRLS& env, const IRInstruction* inst) {
  implIterNext(env, inst, CallSpec::direct(iter_next_object));
}

void cgIterNextObjK(IRLS& env, const IRInstruction* inst) {
  implIterNext(env, inst, CallSpec::direct(iter_next_object_key));
}

IMPL_OPCODE_CALL(IterExtractBase)

///////////////////////////////////////////////////////////////////////////////

}
