// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the "hack" directory of this source tree.

use crate::pos::Pos;
use crate::typing_reason::*;

impl Reason {
    pub fn rev_pos(&self) -> Option<&Pos> {
        match self {
            T_::Rflow(r, _) => r.rev_pos(),
            T_::RprjSymm(_, r) => r.rev_pos(),
            T_::RprjAsymmLeft(_, r) => r.rev_pos(),
            T_::RprjAsymmRight(_, r) => r.rev_pos(),
            T_::Rrev(r) => r.pos(),
            _ => self.pos(),
        }
    }

    pub fn pos(&self) -> Option<&Pos> {
        use T_::*;
        match self {
            Rnone => None,
            Rinvalid => None,
            RmissingField => None,
            Rwitness(p)
            | RwitnessFromDecl(p)
            | Ridx(p, _)
            | RidxVector(p)
            | RidxVectorFromDecl(p)
            | Rforeach(p)
            | Rasyncforeach(p)
            | Rarith(p)
            | RarithRet(p)
            | RarithDynamic(p)
            | Rcomp(p)
            | RconcatRet(p)
            | RlogicRet(p)
            | Rbitwise(p)
            | RbitwiseRet(p)
            | RnoReturn(p)
            | RnoReturnAsync(p)
            | RretFunKind(p, _)
            | RretFunKindFromDecl(p, _)
            | Rhint(p)
            | Rthrow(p)
            | Rplaceholder(p)
            | RretDiv(p)
            | RyieldGen(p)
            | RyieldAsyncgen(p)
            | RyieldAsyncnull(p)
            | RyieldSend(p)
            | Rformat(p, _, _)
            | RclassClass(p, _)
            | RunknownClass(p)
            | RvarParam(p)
            | RvarParamFromDecl(p)
            | RunpackParam(p, _, _)
            | RinoutParam(p)
            | Rtypeconst(box Rnone, (p, _), _, _)
            | RnullsafeOp(p)
            | RtconstNoCstr((p, _))
            | Rpredicated(p, _)
            | RisRefinement(p)
            | RasRefinement(p)
            | Requal(p)
            | RvarrayOrDarrayKey(p)
            | RvecOrDictKey(p)
            | Rusing(p)
            | RdynamicProp(p)
            | RdynamicCall(p)
            | RdynamicConstruct(p)
            | RidxDict(p)
            | RidxSetElement(p)
            | RmissingOptionalField(p, _)
            | RunsetField(p, _)
            | Rregex(p)
            | RimplicitUpperBound(p, _)
            | RarithRetFloat(p, _, _)
            | RarithRetNum(p, _, _)
            | RarithRetInt(p)
            | RbitwiseDynamic(p)
            | RincdecDynamic(p)
            | RtypeVariable(p)
            | RtypeVariableGenerics(p, _, _)
            | RtypeVariableError(p)
            | RglobalTypeVariableGenerics(p, _, _)
            | RsolveFail(p)
            | RcstrOnGenerics(p, _)
            | RlambdaParam(p, _)
            | Rshape(p, _)
            | RshapeLiteral(p)
            | Renforceable(p)
            | Rdestructure(p)
            | RkeyValueCollectionKey(p)
            | RglobalClassProp(p)
            | RglobalFunParam(p)
            | RglobalFunRet(p)
            | Rsplice(p)
            | RetBoolean(p)
            | RdefaultCapability(p)
            | RconcatOperand(p)
            | RinterpOperand(p)
            | RsupportDynamicType(p)
            | RdynamicPartialEnforcement(p, _, _)
            | RrigidTvarEscape(p, _, _, _)
            | RmissingClass(p)
            | RcapturedLike(p)
            | RpessimisedInout(p)
            | RpessimisedReturn(p)
            | RpessimisedProp(p)
            | RpessimisedThis(p)
            | RunsafeCast(p)
            | Rpattern(p) => Some(p),
            RlostInfo(_, r, _)
            | Rinstantiate(_, _, r)
            | Rtypeconst(r, _, _, _)
            | RtypeAccess(r, _)
            | RexprDepType(r, _, _)
            | RcontravariantGeneric(r, _)
            | RinvariantGeneric(r, _) => r.pos(),
            RopaqueTypeFromModule(p, _, _) => Some(p),
            RdynamicCoercion(r) => r.pos(),
            Rflow(r, _) => r.pos(),
            RprjSymm(_, r) => r.pos(),
            RprjAsymmLeft(_, r) => r.pos(),
            RprjAsymmRight(_, r) => r.pos(),
            Rrev(r) => r.rev_pos(),
        }
    }
}
