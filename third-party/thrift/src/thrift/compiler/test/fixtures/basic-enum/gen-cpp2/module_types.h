/**
 * Autogenerated by Thrift for src/module.thrift
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated @nocommit
 */
#pragma once

#include <thrift/lib/cpp2/gen/module_types_h.h>

#include <thrift/lib/cpp2/type/Tag.h>


namespace apache {
namespace thrift {
namespace ident {
struct myEnum;
struct myBigEnum;
} // namespace ident
namespace detail {
#ifndef APACHE_THRIFT_ACCESSOR_myEnum
#define APACHE_THRIFT_ACCESSOR_myEnum
APACHE_THRIFT_DEFINE_ACCESSOR(myEnum);
#endif
#ifndef APACHE_THRIFT_ACCESSOR_myBigEnum
#define APACHE_THRIFT_ACCESSOR_myBigEnum
APACHE_THRIFT_DEFINE_ACCESSOR(myBigEnum);
#endif
} // namespace detail
} // namespace thrift
} // namespace apache

// BEGIN declare_enums
namespace test { namespace fixtures { namespace enumstrict {

enum class EmptyEnum {
};



enum class MyEnum {
  ONE = 1,
  TWO = 2,
};



enum class MyBigEnum {
  UNKNOWN = 0,
  ONE = 1,
  TWO = 2,
  THREE = 3,
  FOUR = 4,
  FIVE = 5,
  SIX = 6,
  SEVEN = 7,
  EIGHT = 8,
  NINE = 9,
  TEN = 10,
  ELEVEN = 11,
  TWELVE = 12,
  THIRTEEN = 13,
  FOURTEEN = 14,
  FIFTEEN = 15,
  SIXTEEN = 16,
  SEVENTEEN = 17,
  EIGHTEEN = 18,
  NINETEEN = 19,
};



}}} // test::fixtures::enumstrict

namespace std {
template<> struct hash<::test::fixtures::enumstrict::EmptyEnum> :
  ::apache::thrift::detail::enum_hash<::test::fixtures::enumstrict::EmptyEnum> {};
template<> struct hash<::test::fixtures::enumstrict::MyEnum> :
  ::apache::thrift::detail::enum_hash<::test::fixtures::enumstrict::MyEnum> {};
template<> struct hash<::test::fixtures::enumstrict::MyBigEnum> :
  ::apache::thrift::detail::enum_hash<::test::fixtures::enumstrict::MyBigEnum> {};
} // std

namespace apache { namespace thrift {


template <> struct TEnumDataStorage<::test::fixtures::enumstrict::EmptyEnum>;

template <> struct TEnumTraits<::test::fixtures::enumstrict::EmptyEnum> {
  using type = ::test::fixtures::enumstrict::EmptyEnum;

  static constexpr std::size_t const size = 0;
  static folly::Range<type const*> const values;
  static folly::Range<folly::StringPiece const*> const names;

  static bool findName(type value, folly::StringPiece* out) noexcept;
  static bool findValue(folly::StringPiece name, type* out) noexcept;

#if FOLLY_HAS_STRING_VIEW
  static bool findName(type value, std::string_view* out) noexcept {
    folly::StringPiece outp;
    return findName(value, &outp) && ((*out = outp), true);
  }
#endif
  static char const* findName(type value) noexcept {
    folly::StringPiece ret;
    (void)findName(value, &ret);
    return ret.data();
  }
};


template <> struct TEnumDataStorage<::test::fixtures::enumstrict::MyEnum>;

template <> struct TEnumTraits<::test::fixtures::enumstrict::MyEnum> {
  using type = ::test::fixtures::enumstrict::MyEnum;

  static constexpr std::size_t const size = 2;
  static folly::Range<type const*> const values;
  static folly::Range<folly::StringPiece const*> const names;

  static bool findName(type value, folly::StringPiece* out) noexcept;
  static bool findValue(folly::StringPiece name, type* out) noexcept;

#if FOLLY_HAS_STRING_VIEW
  static bool findName(type value, std::string_view* out) noexcept {
    folly::StringPiece outp;
    return findName(value, &outp) && ((*out = outp), true);
  }
#endif
  static char const* findName(type value) noexcept {
    folly::StringPiece ret;
    (void)findName(value, &ret);
    return ret.data();
  }
  static constexpr type min() { return type::ONE; }
  static constexpr type max() { return type::TWO; }
};


template <> struct TEnumDataStorage<::test::fixtures::enumstrict::MyBigEnum>;

template <> struct TEnumTraits<::test::fixtures::enumstrict::MyBigEnum> {
  using type = ::test::fixtures::enumstrict::MyBigEnum;

  static constexpr std::size_t const size = 20;
  static folly::Range<type const*> const values;
  static folly::Range<folly::StringPiece const*> const names;

  static bool findName(type value, folly::StringPiece* out) noexcept;
  static bool findValue(folly::StringPiece name, type* out) noexcept;

#if FOLLY_HAS_STRING_VIEW
  static bool findName(type value, std::string_view* out) noexcept {
    folly::StringPiece outp;
    return findName(value, &outp) && ((*out = outp), true);
  }
#endif
  static char const* findName(type value) noexcept {
    folly::StringPiece ret;
    (void)findName(value, &ret);
    return ret.data();
  }
  static constexpr type min() { return type::UNKNOWN; }
  static constexpr type max() { return type::NINETEEN; }
};


}} // apache::thrift

namespace test { namespace fixtures { namespace enumstrict {

using _EmptyEnum_EnumMapFactory = apache::thrift::detail::TEnumMapFactory<EmptyEnum>;
#ifndef ANDROID
[[deprecated("use apache::thrift::util::enumNameSafe, apache::thrift::util::enumName, or apache::thrift::TEnumTraits")]]
extern const _EmptyEnum_EnumMapFactory::ValuesToNamesMapType _EmptyEnum_VALUES_TO_NAMES;
[[deprecated("use apache::thrift::TEnumTraits")]]
extern const _EmptyEnum_EnumMapFactory::NamesToValuesMapType _EmptyEnum_NAMES_TO_VALUES;
#endif
using _MyEnum_EnumMapFactory = apache::thrift::detail::TEnumMapFactory<MyEnum>;
#ifndef ANDROID
[[deprecated("use apache::thrift::util::enumNameSafe, apache::thrift::util::enumName, or apache::thrift::TEnumTraits")]]
extern const _MyEnum_EnumMapFactory::ValuesToNamesMapType _MyEnum_VALUES_TO_NAMES;
[[deprecated("use apache::thrift::TEnumTraits")]]
extern const _MyEnum_EnumMapFactory::NamesToValuesMapType _MyEnum_NAMES_TO_VALUES;
#endif
using _MyBigEnum_EnumMapFactory = apache::thrift::detail::TEnumMapFactory<MyBigEnum>;
#ifndef ANDROID
[[deprecated("use apache::thrift::util::enumNameSafe, apache::thrift::util::enumName, or apache::thrift::TEnumTraits")]]
extern const _MyBigEnum_EnumMapFactory::ValuesToNamesMapType _MyBigEnum_VALUES_TO_NAMES;
[[deprecated("use apache::thrift::TEnumTraits")]]
extern const _MyBigEnum_EnumMapFactory::NamesToValuesMapType _MyBigEnum_NAMES_TO_VALUES;
#endif
}}} // test::fixtures::enumstrict

// END declare_enums
// BEGIN forward_declare
namespace test { namespace fixtures { namespace enumstrict {
class MyStruct;
}}} // test::fixtures::enumstrict
// END forward_declare
// BEGIN hash_and_equal_to
// END hash_and_equal_to
namespace test { namespace fixtures { namespace enumstrict {
using ::apache::thrift::detail::operator!=;
using ::apache::thrift::detail::operator>;
using ::apache::thrift::detail::operator<=;
using ::apache::thrift::detail::operator>=;


class MyStruct final  {
 private:
  friend struct ::apache::thrift::detail::st::struct_private_access;
  template<class> friend struct ::apache::thrift::detail::invoke_reffer;

  //  used by a static_assert in the corresponding source
  static constexpr bool __fbthrift_cpp2_gen_json = false;
  static const folly::StringPiece __fbthrift_get_field_name(::apache::thrift::FieldOrdinal ord);

  void __fbthrift_clear();
  void __fbthrift_clear_terse_fields();
  bool __fbthrift_is_empty() const;

 public:
  using __fbthrift_cpp2_type = MyStruct;
  static constexpr bool __fbthrift_cpp2_is_union =
    false;


 public:

  MyStruct() :
      __fbthrift_field_myEnum(),
      __fbthrift_field_myBigEnum(static_cast<::test::fixtures::enumstrict::MyBigEnum>( ::test::fixtures::enumstrict::MyBigEnum::ONE)) {
  }
  // FragileConstructor for use in initialization lists only.
  [[deprecated("This constructor is deprecated")]]
  MyStruct(apache::thrift::FragileConstructor, ::test::fixtures::enumstrict::MyEnum myEnum__arg, ::test::fixtures::enumstrict::MyBigEnum myBigEnum__arg);

  MyStruct(MyStruct&&) = default;

  MyStruct(const MyStruct&) = default;


  MyStruct& operator=(MyStruct&&) = default;

  MyStruct& operator=(const MyStruct&) = default;
 private:
  ::test::fixtures::enumstrict::MyEnum __fbthrift_field_myEnum;
 private:
  ::test::fixtures::enumstrict::MyBigEnum __fbthrift_field_myBigEnum;
 private:
  apache::thrift::detail::isset_bitset<2, apache::thrift::detail::IssetBitsetOption::Unpacked> __isset;

 public:

  bool operator==(const MyStruct&) const;
  bool operator<(const MyStruct&) const;

  template <typename..., typename T = ::test::fixtures::enumstrict::MyEnum>
  FOLLY_ERASE ::apache::thrift::field_ref<const T&> myEnum_ref() const& {
    return {this->__fbthrift_field_myEnum, __isset.at(0), __isset.bit(0)};
  }

  template <typename..., typename T = ::test::fixtures::enumstrict::MyEnum>
  FOLLY_ERASE ::apache::thrift::field_ref<const T&&> myEnum_ref() const&& {
    return {static_cast<const T&&>(this->__fbthrift_field_myEnum), __isset.at(0), __isset.bit(0)};
  }

  template <typename..., typename T = ::test::fixtures::enumstrict::MyEnum>
  FOLLY_ERASE ::apache::thrift::field_ref<T&> myEnum_ref() & {
    return {this->__fbthrift_field_myEnum, __isset.at(0), __isset.bit(0)};
  }

  template <typename..., typename T = ::test::fixtures::enumstrict::MyEnum>
  FOLLY_ERASE ::apache::thrift::field_ref<T&&> myEnum_ref() && {
    return {static_cast<T&&>(this->__fbthrift_field_myEnum), __isset.at(0), __isset.bit(0)};
  }

  template <typename..., typename T = ::test::fixtures::enumstrict::MyEnum>
  FOLLY_ERASE ::apache::thrift::field_ref<const T&> myEnum() const& {
    return {this->__fbthrift_field_myEnum, __isset.at(0), __isset.bit(0)};
  }

  template <typename..., typename T = ::test::fixtures::enumstrict::MyEnum>
  FOLLY_ERASE ::apache::thrift::field_ref<const T&&> myEnum() const&& {
    return {static_cast<const T&&>(this->__fbthrift_field_myEnum), __isset.at(0), __isset.bit(0)};
  }

  template <typename..., typename T = ::test::fixtures::enumstrict::MyEnum>
  FOLLY_ERASE ::apache::thrift::field_ref<T&> myEnum() & {
    return {this->__fbthrift_field_myEnum, __isset.at(0), __isset.bit(0)};
  }

  template <typename..., typename T = ::test::fixtures::enumstrict::MyEnum>
  FOLLY_ERASE ::apache::thrift::field_ref<T&&> myEnum() && {
    return {static_cast<T&&>(this->__fbthrift_field_myEnum), __isset.at(0), __isset.bit(0)};
  }

  template <typename..., typename T = ::test::fixtures::enumstrict::MyBigEnum>
  FOLLY_ERASE ::apache::thrift::field_ref<const T&> myBigEnum_ref() const& {
    return {this->__fbthrift_field_myBigEnum, __isset.at(1), __isset.bit(1)};
  }

  template <typename..., typename T = ::test::fixtures::enumstrict::MyBigEnum>
  FOLLY_ERASE ::apache::thrift::field_ref<const T&&> myBigEnum_ref() const&& {
    return {static_cast<const T&&>(this->__fbthrift_field_myBigEnum), __isset.at(1), __isset.bit(1)};
  }

  template <typename..., typename T = ::test::fixtures::enumstrict::MyBigEnum>
  FOLLY_ERASE ::apache::thrift::field_ref<T&> myBigEnum_ref() & {
    return {this->__fbthrift_field_myBigEnum, __isset.at(1), __isset.bit(1)};
  }

  template <typename..., typename T = ::test::fixtures::enumstrict::MyBigEnum>
  FOLLY_ERASE ::apache::thrift::field_ref<T&&> myBigEnum_ref() && {
    return {static_cast<T&&>(this->__fbthrift_field_myBigEnum), __isset.at(1), __isset.bit(1)};
  }

  template <typename..., typename T = ::test::fixtures::enumstrict::MyBigEnum>
  FOLLY_ERASE ::apache::thrift::field_ref<const T&> myBigEnum() const& {
    return {this->__fbthrift_field_myBigEnum, __isset.at(1), __isset.bit(1)};
  }

  template <typename..., typename T = ::test::fixtures::enumstrict::MyBigEnum>
  FOLLY_ERASE ::apache::thrift::field_ref<const T&&> myBigEnum() const&& {
    return {static_cast<const T&&>(this->__fbthrift_field_myBigEnum), __isset.at(1), __isset.bit(1)};
  }

  template <typename..., typename T = ::test::fixtures::enumstrict::MyBigEnum>
  FOLLY_ERASE ::apache::thrift::field_ref<T&> myBigEnum() & {
    return {this->__fbthrift_field_myBigEnum, __isset.at(1), __isset.bit(1)};
  }

  template <typename..., typename T = ::test::fixtures::enumstrict::MyBigEnum>
  FOLLY_ERASE ::apache::thrift::field_ref<T&&> myBigEnum() && {
    return {static_cast<T&&>(this->__fbthrift_field_myBigEnum), __isset.at(1), __isset.bit(1)};
  }

  ::test::fixtures::enumstrict::MyEnum get_myEnum() const {
    return __fbthrift_field_myEnum;
  }

  [[deprecated("Use `FOO.myEnum_ref() = BAR;` instead of `FOO.set_myEnum(BAR);`")]]
  ::test::fixtures::enumstrict::MyEnum& set_myEnum(::test::fixtures::enumstrict::MyEnum myEnum_) {
    myEnum_ref() = myEnum_;
    return __fbthrift_field_myEnum;
  }

  ::test::fixtures::enumstrict::MyBigEnum get_myBigEnum() const {
    return __fbthrift_field_myBigEnum;
  }

  [[deprecated("Use `FOO.myBigEnum_ref() = BAR;` instead of `FOO.set_myBigEnum(BAR);`")]]
  ::test::fixtures::enumstrict::MyBigEnum& set_myBigEnum(::test::fixtures::enumstrict::MyBigEnum myBigEnum_) {
    myBigEnum_ref() = myBigEnum_;
    return __fbthrift_field_myBigEnum;
  }

  template <class Protocol_>
  unsigned long read(Protocol_* iprot);
  template <class Protocol_>
  uint32_t serializedSize(Protocol_ const* prot_) const;
  template <class Protocol_>
  uint32_t serializedSizeZC(Protocol_ const* prot_) const;
  template <class Protocol_>
  uint32_t write(Protocol_* prot_) const;

 private:
  template <class Protocol_>
  void readNoXfer(Protocol_* iprot);

  friend class ::apache::thrift::Cpp2Ops<MyStruct>;
  friend void swap(MyStruct& a, MyStruct& b);
};

template <class Protocol_>
unsigned long MyStruct::read(Protocol_* iprot) {
  auto _xferStart = iprot->getCursorPosition();
  readNoXfer(iprot);
  return iprot->getCursorPosition() - _xferStart;
}


}}} // test::fixtures::enumstrict
