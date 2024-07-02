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

#pragma once

#include <hphp/runtime/base/datatype.h>
#include "hphp/runtime/base/annot-type.h"
#include "hphp/runtime/base/runtime-option.h"
#include "hphp/runtime/base/string-data.h"
#include "hphp/runtime/base/tv-val.h"
#include "hphp/runtime/vm/hhbc.h"
#include "hphp/runtime/vm/type-constraint-flags.h"
#include "hphp/runtime/vm/type-profile.h"
#include "hphp/runtime/vm/containers.h"

#include "hphp/util/check-size.h"
#include "hphp/util/configs/eval.h"
#include "hphp/util/functional.h"
#include "hphp/util/trace.h"

#include <functional>
#include <ranges>
#include <string>

namespace HPHP {
struct Func;
struct NamedType;
struct StringData;
}

namespace HPHP {

//////////////////////////////////////////////////////////////////////

/*
 * TypeConstraint represents the metadata required to check a PHP
 * function or method parameter typehint at runtime.
 */
struct TypeConstraint {
  /*
   * Special type constraints use a "Type", instead of just
   * underlyingDataType().
   *
   * See underlyingDataType().
   */
  using Type = AnnotType;
  using MetaType = AnnotMetaType;

  static const int32_t ReturnId = -1;

  /// An ClassConstraint is used to represent class-specific (or
  /// enum-specific) information about a TypeConstraint.
  struct ClassConstraint {
    enum class ClsNameKind : uint8_t {
      Unset = 0,
      Object = 1,
      Class = 2
    };
    using NamePtr = TaggedLowStringPtr<ClsNameKind>;
    /// Resolved class name. Only valid if this TypeConstraint represents a
    /// resolved object or enum.
    NamePtr m_clsName;
    /// Source type name. In general this is the name from the source code.
    LowStringPtr m_typeName;
    /// The NamedType is used to differentiate implementations of
    /// TypeConstraints between requests. The m_typeName is common (it's a
    /// 'Foo') and the NamedType is specialized for each request to indicate the
    /// actual underlying implementation of that Class* for each request.
    LowPtr<const NamedType> m_namedType;

    ClassConstraint() = default;
    explicit ClassConstraint(LowStringPtr typeName);
    ClassConstraint(NamePtr clsName,
                    LowStringPtr typeName,
                    LowPtr<const NamedType> namedType);
    explicit ClassConstraint(Class& cls);

    size_t stableHash() const;
    bool operator==(const ClassConstraint& o) const;

    void serdeHelper(BlobDecoder& sd, bool isSubObject);
    void serdeHelper(BlobEncoder& sd, bool isSubObject) const;

    void init(AnnotType type);
  };

  static_assert(CheckSize<ClassConstraint, use_lowptr ? 12 : 24>(), "");

  using UnionTypeMask = uint16_t;
  // These are ordered such that the "simplest" ones (in terms of default value)
  // are first.
  constexpr static UnionTypeMask kUnionTypeBool      = 1 << 0;
  constexpr static UnionTypeMask kUnionTypeInt       = 1 << 1;
  constexpr static UnionTypeMask kUnionTypeFloat     = 1 << 2;
  constexpr static UnionTypeMask kUnionTypeCallable  = 1 << 3;
  constexpr static UnionTypeMask kUnionTypeObject    = 1 << 4;
  constexpr static UnionTypeMask kUnionTypeResource  = 1 << 5;
  constexpr static UnionTypeMask kUnionTypeThis      = 1 << 6;
  constexpr static UnionTypeMask kUnionTypeString    = 1 << 7;
  constexpr static UnionTypeMask kUnionTypeVec       = 1 << 8;
  constexpr static UnionTypeMask kUnionTypeKeyset    = 1 << 9;
  constexpr static UnionTypeMask kUnionTypeDict      = 1 << 10;
  constexpr static UnionTypeMask kUnionTypeClassname = 1 << 11;
  // Class should be the last flag because it indicates a list of classnames in
  // the repr which we want to handle last and repeat until we're out of
  // classes.
  constexpr static UnionTypeMask kUnionTypeClass = 1 << 15;

  struct UnionClassList {
    TinyVector<ClassConstraint, 1> m_list;

    UnionClassList() = default;
    explicit UnionClassList(TinyVector<ClassConstraint, 1> list) : m_list(std::move(list)) { }

    size_t stableHash() const;
    bool operator==(const UnionClassList& o) const { return m_list == o.m_list; }
  };

  /// A UnionConstraint is used to represent a union of type
  /// constraints. Primitive types are stored as bitfields and classes are
  /// stored as an array of ClassConstraints.
  struct UnionConstraint {
    // TypeConstaints are global objects (they're used and cached by the JIT) so
    // thye must only point at persistant global objects. Use allocObjects() to
    // uniquify/allocate.
    LowPtr<const UnionClassList> m_classes;
    LowStringPtr m_typeName;
    UnionTypeMask m_mask;

    UnionConstraint() = default;
    UnionConstraint(UnionTypeMask mask,
                    LowStringPtr typeName,
                    LowPtr<const UnionClassList> classes)
      : m_classes(classes)
      , m_typeName(typeName)
      , m_mask(mask)
    { }

    size_t stableHash() const;
    bool operator==(const UnionConstraint& o) const;

    static LowPtr<const UnionClassList> allocObjects(UnionClassList objects);
  };

  static_assert(CheckSize<UnionConstraint, use_lowptr ? 12 : 24>(), "");

  TypeConstraint();
  TypeConstraint(const StringData* typeName, TypeConstraintFlags flags);
  TypeConstraint(Type type, TypeConstraintFlags flags, ClassConstraint class_);
  TypeConstraint(Type type,
                 TypeConstraintFlags flags,
                 const LowStringPtr typeName);
  TypeConstraint(Type type, TypeConstraintFlags flags);

  template<std::ranges::sized_range R>
  static TypeConstraint makeUnion(LowStringPtr typeName, R&& tcs) {
    assertx(typeName != nullptr);
    if (tcs.empty()) {
      return TypeConstraint{typeName, TypeConstraintFlags::NoFlags};
    } else if (tcs.size() == 1) {
      return TypeConstraint{*tcs.cbegin()};
    }

    UnionBuilder builder(typeName, tcs.size());
    if (auto result = builder.recordConstraints(tcs)) return *result;
    return std::move(builder).finish();
  }

  static TypeConstraint makeMixed();

  template<class SerDe> void serde(SerDe& sd);
  template<class SerDe> void serdeUnion(SerDe& sd);
  template<class SerDe> void serdeSingle(SerDe& sd);

  TypeConstraint(const TypeConstraint&) = default;
  TypeConstraint& operator=(const TypeConstraint&) = default;

  bool operator==(const TypeConstraint& o) const;

  size_t stableHash() const;

  void resolveType(AnnotType t, bool nullable, LowStringPtr clsName);
  void unresolve();

  void addFlags(TypeConstraintFlags flags) {
    m_flags = static_cast<TypeConstraintFlags>(m_flags | flags);
  }

  /*
   * Returns: whether this constraint implies any runtime checking at
   * all.  If this function returns false, it means the parameter type
   * verification would be a no-op.
   */
  bool hasConstraint() const {
    if (isUnion()) return true;
    switch (m_u.single.type) {
      case AnnotType::Null:
      case AnnotType::Bool:
      case AnnotType::Int:
      case AnnotType::Float:
      case AnnotType::String:
      case AnnotType::Object:
      case AnnotType::Resource:
      case AnnotType::Dict:
      case AnnotType::Vec:
      case AnnotType::Keyset:
      case AnnotType::Nonnull:
      case AnnotType::Callable:
      case AnnotType::Number:
      case AnnotType::ArrayKey:
      case AnnotType::This:
      case AnnotType::VecOrDict:
      case AnnotType::ArrayLike:
      case AnnotType::NoReturn:
      case AnnotType::Nothing:
      case AnnotType::Classname:
        return true;
      case AnnotType::SubObject:
      case AnnotType::Unresolved:
        return (bool)m_u.single.class_.m_typeName;
      case AnnotType::Mixed:
        return false;
    }
    not_reached();
  }

  /*
   * Read access to various members.
   */
  const StringData* clsName() const {
    assertx(!isUnion());
    return m_u.single.class_.m_clsName;
  }
  const StringData* typeName() const {
    return isUnion()
      ? m_u.union_.m_typeName
      : m_u.single.class_.m_typeName;
  }
  const NamedType* clsNamedType() const {
    assertx(isSubObject());
    return m_u.single.class_.m_namedType;
  }
  const NamedType* typeNamedType() const {
    assertx(isUnresolved() && !isUnion());
    return m_u.single.class_.m_namedType;
  }
  const NamedType* anyNamedType() const {
    if (isUnion()) return nullptr;
    return m_u.single.class_.m_namedType;
  }
  TypeConstraintFlags flags() const { return m_flags; }

  /*
   * Access to the "meta type" for this TypeConstraint.
   */
  MetaType metaType() const {
    assertx(!isUnion());
    return getAnnotMetaType(m_u.single.type);
  }

  /*
   * If this->isUnresolved(), resolve it using the autoloader.
   */
  TypeConstraint resolvedWithAutoload() const;

  /*
   * Returns the underlying DataType for this TypeConstraint.
   */
  MaybeDataType underlyingDataType() const {
    if (isPrecise()) return MaybeDataType(getAnnotDataType(m_u.single.type));
    if (isSubObject() &&
        !interface_supports_non_objects(m_u.single.class_.m_clsName)) {
      return MaybeDataType(KindOfObject);
    }
    return std::nullopt;
  }

  /*
   * Returns the underlying DataType for this TypeConstraint,
   * chasing down type aliases.
   */
  MaybeDataType underlyingDataTypeResolved() const;

  /*
   * Check if this TypeConstraint *might* imply a runtime check. If it returns
   * false, it definitely doesn't. If it returns true, it might still be a mixed
   * type at runtime (because of a type-alias for example).
   */
  bool isCheckable() const {
    return (hasConstraint()
            && !isMixed()
            && !isTypeVar()
            && !isTypeConstant())
      || isUnion();
  }

  /*
   * Check if this TypeConstraint definitely does not require a runtime check
   * (equivalent to mixed). This may invoke the autoloader and will resolve
   * type-aliases, so is exact.
   */
  bool isMixedResolved() const;

  /*
   * Check if this TypeConstraint may not require a runtime check (equivalent to
   * mixed). If it returns false, it definitely is not. If it returns true, it
   * still might not be (because of a type-alias for example). This will not
   * invoke the autoloader.
   */
  bool maybeMixed() const;

  /*
   * Predicates for various properties of the type constraint.
   */
  bool isNullable() const { return contains(m_flags, TypeConstraintFlags::Nullable); }
  bool isSoft()     const { return contains(m_flags, TypeConstraintFlags::Soft); }
  bool isExtended() const { return contains(m_flags, TypeConstraintFlags::ExtendedHint); }
  bool isTypeVar()  const { return contains(m_flags, TypeConstraintFlags::TypeVar); }
  bool isTypeConstant() const { return contains(m_flags, TypeConstraintFlags::TypeConstant); }
  bool isUpperBound() const { return contains(m_flags, TypeConstraintFlags::UpperBound); }
  bool isUnion() const { return contains(m_flags, TypeConstraintFlags::Union); }

  bool isPrecise()  const { return !isUnion() && metaType() == MetaType::Precise; }
  bool isMixed()    const { return !isUnion() && m_u.single.type == Type::Mixed; }
  bool isThis()     const { return !isUnion() && m_u.single.type == Type::This; }
  bool isCallable() const { return !isUnion() && m_u.single.type == Type::Callable; }
  bool isNumber()   const { return !isUnion() && m_u.single.type == Type::Number; }
  bool isNothing()  const { return !isUnion() && m_u.single.type == Type::Nothing; }
  bool isNoReturn() const { return !isUnion() && m_u.single.type == Type::NoReturn; }
  bool isArrayKey() const { return !isUnion() && m_u.single.type == Type::ArrayKey; }
  bool isDict()     const { return !isUnion() && m_u.single.type == Type::Dict; }
  bool isVec()      const { return !isUnion() && m_u.single.type == Type::Vec; }
  bool isKeyset()   const { return !isUnion() && m_u.single.type == Type::Keyset; }
  bool isAnyObject() const { return !isUnion() && m_u.single.type == Type::Object; }
  bool isSubObject() const { return !isUnion() && m_u.single.type == Type::SubObject; }
  bool isInt()       const { return !isUnion() && m_u.single.type == Type::Int; }
  bool isString()    const { return !isUnion() && m_u.single.type == Type::String; }
  bool isArrayLike() const { return !isUnion() && m_u.single.type == Type::ArrayLike; }
  bool isVecOrDict() const { return !isUnion() && m_u.single.type == Type::VecOrDict; }
  bool isClassname() const { return !isUnion() && m_u.single.type == Type::Classname; }

  bool isSoftOrBuiltinSoft(const Func* func) const;

  bool isUnresolved() const {
    return isUnion()
      ? !contains(m_flags, TypeConstraintFlags::Resolved)
      : (m_u.single.type == Type::Unresolved);
  }

  bool unionHasThis() const {
    assertx(isUnion());
    return m_u.union_.m_mask & kUnionTypeThis;
  }

  bool unionHasString() const {
    assertx(isUnion());
    return m_u.union_.m_mask & kUnionTypeString;
  }

  AnnotType type() const {
    assertx(!isUnion());
    return m_u.single.type;
  }

  void setType(AnnotType ty) {
    assertx(!isUnion());
    m_u.single.type = ty;
  }

  bool validForProp() const;

  void validForPropResolved(const Class* declCls,
                            const StringData* propName) const;

  bool validForEnumBase() const {
    auto const resolved = resolvedWithAutoload();
    return resolved.isInt() || resolved.isString() ||
           resolved.isArrayKey() || resolved.isClassname() ||
           resolved.isNothing();
  }

  /*
   * Format this TypeConstraint for display to the user. Context is used to
   * optionally resolve This to its class name. Extra will cause the resolved
   * type (if any) to be appended to the name.
   */
  std::string displayName(const Class* context = nullptr,
                          bool extra = false) const;

  std::string debugName() const;

  /*
   * Obtain an initial value suitable for this type-constraint. Where possible,
   * the initial value is chosen to satisfy the type-constraint, but this isn't
   * always possible (for example, for objects).
   */
  TypedValue defaultValue() const;

  /*
   * Returns whether this and another type-constraint might not be equivalent at
   * runtime. Two type-constraints are equivalent if they allow exactly the same
   * values. This function is conservative and will return true if not
   * sure. This function will not autoload or check loaded classes of
   * type-aliases. Only meant for property type-hints.
   */
  bool maybeInequivalentForProp(const TypeConstraint& other) const;

  /*
   * Returns whether this and another type-constraint are definitely
   * equivalent. Unlike maybeInequivalentForProp(), this function is exact and
   * can autoload. Only meant for property type-hints.
   */
  bool equivalentForProp(const TypeConstraint& other) const;

  /*
   * Normal check if this type-constraint is compatible with the given value
   * (using the given context). This can invoke the autoloader and is always
   * exact. This should not be used for property type-hints (which behave
   * slightly differently) and the context is required.  Instead use checkProp.
   * The context determines the meaning of This type-constraint.
   */
  bool check(tv_rval val, const Class* context) const {
    return checkImpl<CheckMode::Exact>(val, context);
  }
  bool checkProp(tv_rval val, const Class* context) const {
    return checkImpl<CheckMode::ExactProp>(val, context);
  }

  /*
   * Assert that this type-constraint is compatible with the given value. This
   * is meant for use in assertions and is conservative. It will not invoke the
   * autoloader, but can consult already loaded classes or type-aliases. It will
   * only return false if the value definitely does not satisfy the
   * type-constraint, true otherwise.
   */
  bool assertCheck(tv_rval val) const {
    return checkImpl<CheckMode::Assert>(val, nullptr);
  }

  /*
   * Check if this type-constraint is compatible with the given value in *all
   * contexts*. That is, regardless of which classes or type-aliases might be
   * loaded. A type which passes this check will never need to be checked at
   * run-time. This will utilize persistent classes but nothing else.
   */
  bool alwaysPasses(tv_rval val) const {
    if (!isCheckable()) return true;
    return checkImpl<CheckMode::AlwaysPasses>(val, nullptr);
  }
  // Same as the above, but uses a type instead of an actual value. The
  // StringData* variant is for objects.
  bool alwaysPasses(const StringData* clsName) const;
  bool alwaysPasses(DataType dt) const;

  bool checkTypeAliasObj(const Class* cls) const {
    assertx(!isUnion());
    return checkTypeAliasImpl<false>(m_u.single.class_, cls);
  }

  // NB: Can throw if the check fails.
  void verifyParam(tv_lval val,
                   const Class* ctx,
                   const Func* func,
                   int paramNum) const;
  void verifyReturn(TypedValue* tv, const Class* ctx, const Func* func) const;
  void verifyReturnNonNull(TypedValue* tv,
                           const Class* ctx,
                           const Func* func) const;
  void verifyOutParam(TypedValue* tv,
                      const Class* ctx,
                      const Func* func,
                      int paramNum) const;
  // TODO(T61738946): We can take a tv_rval here once we remove support for
  // coercing class_meth types.
  void verifyProperty(tv_lval val,
                      const Class* thisCls,
                      const Class* declCls,
                      const StringData* propName) const;
  void verifyStaticProperty(tv_lval val,
                            const Class* thisCls,
                            const Class* declCls,
                            const StringData* propName) const;

  void verifyParamFail(tv_lval val,
                       const Class* ctx,
                       const Func* func,
                       int paramNum) const;
  void verifyOutParamFail(TypedValue* tv,
                          const Class* ctx,
                          const Func* func,
                          int paramNum) const;
  void verifyReturnFail(tv_lval val,
                        const Class* ctx,
                        const Func* func) const;
  // TODO(T61738946): We can take a tv_rval here once we remove support for
  // coercing class_meth types.
  void verifyPropFail(const Class* thisCls, const Class* declCls,
                      tv_lval val, const StringData* propName,
                      bool isStatic) const;

  bool maybeStringCompatible() const;

  // Whether input may need to be coerced.
  bool mayCoerce() const {
    return maybeStringCompatible();
  }

  /**
   * Return the correct `DataType` that represents this type constraint as used
   * as a type within systemlib, specifically in the context of a `__Native`
   * function. For example:
   * - Nullable and soft types are always represented as `KindOfMixed`
   * - Unresolved types are always represented as `KindOfObejct`
   * - Otherwise, grab the result of `underlyingDataType`
   */
  MaybeDataType asSystemlibType() const;

private:
  void initSingle();
  void initUnion();
  ClassConstraint makeClass(Type type, const LowStringPtr typeName);

  TypeConstraint(TypeConstraintFlags flags,
                 UnionTypeMask mask,
                 LowStringPtr typeName,
                 LowPtr<const UnionClassList> classes);

  // There are a few cases where a type constraint does not pass, but we don't
  // raise an error. Some of these cases are resolved by mutating val instead.
  //
  // If this method returns true, we don't need to raise the type error.
  template <typename F>
  bool tryCommonCoercions(tv_lval val, const Class* ctx,
                          const Class* propDecl, F tcInfo) const;

  enum class CheckMode {
    Exact, // Do an exact check with autoloading
    ExactProp, // Do an exact prop check with autoloading
    AlwaysPasses, // Don't check environment at all. Return false if not sure.
    Assert // Check loaded classes/type-aliases, but don't autoload. Return true
           // if not sure.
  };

  template <CheckMode>
  bool checkImpl(tv_rval val, const Class* context) const;

  template <bool, bool>
  bool checkNamedTypeNonObj(tv_rval val) const;

  template <bool> static bool checkTypeAliasImpl(const ClassConstraint& oc, const Class* type);

  bool checkStringCompatible() const;

  void validForPropFail(const Class*, const StringData*) const;

private:
#ifdef USE_LOWPTR
#pragma pack(push, 2)
#else
#pragma pack(push, 4)
#endif
  TypeConstraintFlags m_flags;
  union {
    struct {
      // `type` represents the type to check on.  We don't know whether a
      // bare name is a class/interface name or a type alias or an enum,
      // so when this is set to Type::Unresolved we may have to resolve a type
      // alias or enum and test for a different DataType (see annotCompat()
      // for details).
      Type type;
      ClassConstraint class_;
    } single;
    UnionConstraint union_;
  } m_u;
#pragma pack(pop)

  friend struct TcUnionPieceIterator;
  friend struct TcUnionPieceView;

  enum class ResolvedType {
    // We haven't yet specified a resolved or unresolved class.
    Unspecified,
    // We only have unresolved classes.
    Unresolved,
    // We only have resolved classes.
    Resolved,
    // We have a mix of resolved and unresolved classes. This isn't legal in a
    // TypeConstraint so it has to be dealt with before finishing.
    Mixed,
  };

  struct UnionBuilder {
    const LowStringPtr m_typeName;
    TypeConstraintFlags m_flags = TypeConstraintFlags::Union;
    UnionTypeMask m_preciseTypeMask = 0;
    ResolvedType m_resolved = ResolvedType::Unspecified;
    UnionClassList m_classes;
    bool m_containsNonnull = false;

    UnionBuilder(LowStringPtr typeName, size_t capacity);
    Optional<TypeConstraint> recordConstraint(const TypeConstraint& tc);
    TypeConstraint finish() &&;

    template<std::ranges::input_range R>
    Optional<TypeConstraint> recordConstraints(R&& range) {
      for (const auto& tc : range) {
        // We only need to do this one level because a union cannot contain a
        // sub-union.
        for (auto& sub : eachTypeConstraintInUnion(tc)) {
          auto result = recordConstraint(sub);
          if (result) return *result;
        }
      }
      return std::nullopt;
    }
  };
};

static_assert(CheckSize<TypeConstraint, use_lowptr ? 16 : 32>(), "");

/// This is an iterator for TypeConstraint - see eachTypeConstraintInUnion().
struct TcUnionPieceIterator {
  using difference_type = std::ptrdiff_t;
  using value_type = TypeConstraint;
  const TypeConstraint& operator*() const { return m_outTc; }
  TcUnionPieceIterator& operator++();
  TcUnionPieceIterator operator++(int);
  bool operator==(const TcUnionPieceIterator&) const;
  bool at_end() const;

private:
  TypeConstraint m_outTc;
  LowPtr<const TypeConstraint::UnionClassList> m_classes;
  TypeConstraintFlags m_flags;
  size_t m_nextClass;
  TypeConstraint::UnionTypeMask m_mask;

  void buildUnionTypeConstraint();

  friend struct TcUnionPieceView;
};
static_assert(std::forward_iterator<TcUnionPieceIterator>);

/// This is a "view" of a TypeConstraint which contains begin() and end()
/// support. We could have TypeConstraint support begin() and end() directly but
/// that seems somewhat non-intuitive what raw begin() and end() would mean.
struct TcUnionPieceView {
  enum class Kind { All, ClassesOnly };
  TcUnionPieceView(const TypeConstraint& tc, Kind kind) : m_tc(tc), m_kind(kind) { }
  TcUnionPieceIterator begin() const;
  TcUnionPieceIterator end() const;
private:
  const TypeConstraint m_tc;
  const Kind m_kind;
};

static_assert(std::ranges::input_range<TcUnionPieceView>);

/// For a single TypeConstraint returns a view which iterates over the single
/// TC.  For a union returns a view which iterates over the individual members
/// of the union.
inline TcUnionPieceView eachTypeConstraintInUnion(const TypeConstraint& tc) {
  return TcUnionPieceView(tc, TcUnionPieceView::Kind::All);
}
// Like eachTypeConstraintInUnion() but only yields the classes.
inline TcUnionPieceView eachClassTypeConstraintInUnion(const TypeConstraint& tc) {
  return TcUnionPieceView(tc, TcUnionPieceView::Kind::ClassesOnly);
}

/// TypeIntersectionConstraintT is generally used for upper-bounds. It's
/// templated so we can have a common implementation with different vector
/// representation (std::vector, TinyVector, VMCompactVector) based on need.
template<typename VectorT>
struct TypeIntersectionConstraintT {
  VectorT m_constraints;

  TypeIntersectionConstraintT() = default;

  explicit TypeIntersectionConstraintT(TypeConstraint tc) {
    m_constraints.emplace_back(std::move(tc));
  }

  template<class SerDe>
  void serde(SerDe& sd) {
    sd(m_constraints);
  }

  bool isTop() const { return m_constraints.empty(); }
  bool isSimple() const { return m_constraints.size() == 1; }

  const TypeConstraint& asSimple() const {
    assertx(isSimple());
    return m_constraints[0];
  }

  TypeConstraint& asSimpleMut() {
    assertx(isSimple());
    return m_constraints[0];
  }

  void add(TypeConstraint tc) {
    m_constraints.emplace_back(std::move(tc));
  }
};

using TinyTypeIntersectionConstraint = TypeIntersectionConstraintT<TinyVector<TypeConstraint>>;
using TypeIntersectionConstraint = TypeIntersectionConstraintT<CompactVector<TypeConstraint>>;
using StdTypeIntersectionConstraint = TypeIntersectionConstraintT<std::vector<TypeConstraint>>;
using VMTypeIntersectionConstraint = TypeIntersectionConstraintT<VMCompactVector<TypeConstraint>>;

//////////////////////////////////////////////////////////////////////

/*
 * Its possible to use type constraints on function parameters to devise better
 * memoization key generation schemes. For example, if we know the
 * type-constraint limits the parameter to only ever being an integer or string,
 * then the memoization key scheme can just be the identity. This is because
 * integers and strings won't collide with each other, and we know it won't ever
 * be anything else. Without such a constraint, the string would need escaping.
 *
 * This function takes a type-constraint and returns the suitable "memo-key
 * constraint" if it corresponds to one. Note: HHBBC, the interpreter, and the
 * JIT all need to agree exactly on the scheme for each constraint. It is the
 * caller's responsibility to actually verify that type-hints are being
 * enforced. If they are not, then none of this information can be used.
 */
enum class MemoKeyConstraint {
  Int,
  IntOrNull,
  Bool,
  BoolOrNull,
  Str,
  StrOrNull,
  IntOrStr,
  Dbl,
  DblOrNull,
  Object,
  ObjectOrNull,
  None
};
MemoKeyConstraint memoKeyConstraintFromTC(const TypeConstraint&);

std::string describe_actual_type(tv_rval val);

bool tcCouldBeReified(const Func*, uint32_t);

/*
 * Check if the result of a SetOp needs to be checked against the property's
 * type-hint. If we do, we'll have to perform the SetOp on a temporary, do the
 * check, then move it into the final location. Otherwise we can just do the
 * SetOp in place.
 *
 * For now we only elide the type-check for concats when the lhs is already a
 * string or the type-hint always allows strings (since a concat always produces
 * a string). This is more than a minor optimization. If we do the concat on a
 * temporary, it will increase the ref-count of the target, meaning the concat
 * will trigger a COW. This can be a major performance hit if the target is
 * large.
 */
inline bool setOpNeedsTypeCheck(const TypeConstraint& tc,
                                SetOpOp op,
                                tv_rval lhs) {
  if (Cfg::Eval::CheckPropTypeHints <= 0 || !tc.isCheckable()) {
    return false;
  }
  if (op != SetOpOp::ConcatEqual) return true;
  // If the target of the concat is already a string, or the type-hint always
  // allows a string, we don't need a check because the concat will always
  // produce a string, regardless of the rhs.
  if (LIKELY(isStringType(type(lhs)))) return false;
  return !tc.alwaysPasses(KindOfString);
}

// Add all flags in tc (except TypeVar) to ub.
// FIXME: applying random flags such as Resolved is super sketchy
void applyFlagsToUB(TypeConstraint& ub, const TypeConstraint& tc);
} // namespace HPHP

template<> inline constexpr bool std::ranges::enable_borrowed_range<HPHP::TcUnionPieceView> = true;

static_assert(std::ranges::borrowed_range<HPHP::TcUnionPieceView>);
static_assert(std::ranges::viewable_range<HPHP::TcUnionPieceView>);
