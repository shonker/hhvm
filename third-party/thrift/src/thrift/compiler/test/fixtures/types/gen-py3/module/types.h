/**
 * Autogenerated by Thrift
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */

#pragma once

#include <functional>
#include <folly/Range.h>

#include <thrift/lib/py3/enums.h>
#include "thrift/compiler/test/fixtures/types/src/gen-cpp2/module_data.h"
#include "thrift/compiler/test/fixtures/types/src/gen-cpp2/module_types.h"
#include "thrift/compiler/test/fixtures/types/src/gen-cpp2/module_metadata.h"
namespace thrift {
namespace py3 {


template<>
const std::vector<std::pair<std::string_view, std::string_view>>& PyEnumTraits<
    ::apache::thrift::fixtures::types::has_bitwise_ops>::namesmap() {
  static const folly::Indestructible<NamesMap> pairs {
    {
    }
  };
  return *pairs;
}


template<>
const std::vector<std::pair<std::string_view, std::string_view>>& PyEnumTraits<
    ::apache::thrift::fixtures::types::is_unscoped>::namesmap() {
  static const folly::Indestructible<NamesMap> pairs {
    {
    }
  };
  return *pairs;
}


template<>
const std::vector<std::pair<std::string_view, std::string_view>>& PyEnumTraits<
    ::apache::thrift::fixtures::types::MyForwardRefEnum>::namesmap() {
  static const folly::Indestructible<NamesMap> pairs {
    {
    }
  };
  return *pairs;
}



template<>
void reset_field<::apache::thrift::fixtures::types::decorated_struct>(
    ::apache::thrift::fixtures::types::decorated_struct& obj, uint16_t index) {
  switch (index) {
    case 0:
      obj.field_ref().copy_from(default_inst<::apache::thrift::fixtures::types::decorated_struct>().field_ref());
      return;
  }
}

template<>
void reset_field<::apache::thrift::fixtures::types::ContainerStruct>(
    ::apache::thrift::fixtures::types::ContainerStruct& obj, uint16_t index) {
  switch (index) {
    case 0:
      obj.fieldA_ref().copy_from(default_inst<::apache::thrift::fixtures::types::ContainerStruct>().fieldA_ref());
      return;
    case 1:
      obj.fieldB_ref().copy_from(default_inst<::apache::thrift::fixtures::types::ContainerStruct>().fieldB_ref());
      return;
    case 2:
      obj.fieldC_ref().copy_from(default_inst<::apache::thrift::fixtures::types::ContainerStruct>().fieldC_ref());
      return;
    case 3:
      obj.fieldD_ref().copy_from(default_inst<::apache::thrift::fixtures::types::ContainerStruct>().fieldD_ref());
      return;
    case 4:
      obj.fieldE_ref().copy_from(default_inst<::apache::thrift::fixtures::types::ContainerStruct>().fieldE_ref());
      return;
    case 5:
      obj.fieldF_ref().copy_from(default_inst<::apache::thrift::fixtures::types::ContainerStruct>().fieldF_ref());
      return;
    case 6:
      obj.fieldG_ref().copy_from(default_inst<::apache::thrift::fixtures::types::ContainerStruct>().fieldG_ref());
      return;
    case 7:
      obj.fieldH_ref().copy_from(default_inst<::apache::thrift::fixtures::types::ContainerStruct>().fieldH_ref());
      return;
  }
}

template<>
void reset_field<::apache::thrift::fixtures::types::CppTypeStruct>(
    ::apache::thrift::fixtures::types::CppTypeStruct& obj, uint16_t index) {
  switch (index) {
    case 0:
      obj.fieldA_ref().copy_from(default_inst<::apache::thrift::fixtures::types::CppTypeStruct>().fieldA_ref());
      return;
  }
}

template<>
void reset_field<::apache::thrift::fixtures::types::VirtualStruct>(
    ::apache::thrift::fixtures::types::VirtualStruct& obj, uint16_t index) {
  switch (index) {
    case 0:
      obj.MyIntField_ref().copy_from(default_inst<::apache::thrift::fixtures::types::VirtualStruct>().MyIntField_ref());
      return;
  }
}

template<>
void reset_field<::apache::thrift::fixtures::types::MyStructWithForwardRefEnum>(
    ::apache::thrift::fixtures::types::MyStructWithForwardRefEnum& obj, uint16_t index) {
  switch (index) {
    case 0:
      obj.a_ref().copy_from(default_inst<::apache::thrift::fixtures::types::MyStructWithForwardRefEnum>().a_ref());
      return;
    case 1:
      obj.b_ref().copy_from(default_inst<::apache::thrift::fixtures::types::MyStructWithForwardRefEnum>().b_ref());
      return;
  }
}

template<>
void reset_field<::apache::thrift::fixtures::types::TrivialNumeric>(
    ::apache::thrift::fixtures::types::TrivialNumeric& obj, uint16_t index) {
  switch (index) {
    case 0:
      obj.a_ref().copy_from(default_inst<::apache::thrift::fixtures::types::TrivialNumeric>().a_ref());
      return;
    case 1:
      obj.b_ref().copy_from(default_inst<::apache::thrift::fixtures::types::TrivialNumeric>().b_ref());
      return;
  }
}

template<>
void reset_field<::apache::thrift::fixtures::types::TrivialNestedWithDefault>(
    ::apache::thrift::fixtures::types::TrivialNestedWithDefault& obj, uint16_t index) {
  switch (index) {
    case 0:
      obj.z_ref().copy_from(default_inst<::apache::thrift::fixtures::types::TrivialNestedWithDefault>().z_ref());
      return;
    case 1:
      obj.n_ref().copy_from(default_inst<::apache::thrift::fixtures::types::TrivialNestedWithDefault>().n_ref());
      return;
  }
}

template<>
void reset_field<::apache::thrift::fixtures::types::ComplexString>(
    ::apache::thrift::fixtures::types::ComplexString& obj, uint16_t index) {
  switch (index) {
    case 0:
      obj.a_ref().copy_from(default_inst<::apache::thrift::fixtures::types::ComplexString>().a_ref());
      return;
    case 1:
      obj.b_ref().copy_from(default_inst<::apache::thrift::fixtures::types::ComplexString>().b_ref());
      return;
  }
}

template<>
void reset_field<::apache::thrift::fixtures::types::ComplexNestedWithDefault>(
    ::apache::thrift::fixtures::types::ComplexNestedWithDefault& obj, uint16_t index) {
  switch (index) {
    case 0:
      obj.z_ref().copy_from(default_inst<::apache::thrift::fixtures::types::ComplexNestedWithDefault>().z_ref());
      return;
    case 1:
      obj.n_ref().copy_from(default_inst<::apache::thrift::fixtures::types::ComplexNestedWithDefault>().n_ref());
      return;
  }
}

template<>
void reset_field<::apache::thrift::fixtures::types::MinPadding>(
    ::apache::thrift::fixtures::types::MinPadding& obj, uint16_t index) {
  switch (index) {
    case 0:
      obj.small_ref().copy_from(default_inst<::apache::thrift::fixtures::types::MinPadding>().small_ref());
      return;
    case 1:
      obj.big_ref().copy_from(default_inst<::apache::thrift::fixtures::types::MinPadding>().big_ref());
      return;
    case 2:
      obj.medium_ref().copy_from(default_inst<::apache::thrift::fixtures::types::MinPadding>().medium_ref());
      return;
    case 3:
      obj.biggish_ref().copy_from(default_inst<::apache::thrift::fixtures::types::MinPadding>().biggish_ref());
      return;
    case 4:
      obj.tiny_ref().copy_from(default_inst<::apache::thrift::fixtures::types::MinPadding>().tiny_ref());
      return;
  }
}

template<>
void reset_field<::apache::thrift::fixtures::types::MinPaddingWithCustomType>(
    ::apache::thrift::fixtures::types::MinPaddingWithCustomType& obj, uint16_t index) {
  switch (index) {
    case 0:
      obj.small_ref().copy_from(default_inst<::apache::thrift::fixtures::types::MinPaddingWithCustomType>().small_ref());
      return;
    case 1:
      obj.big_ref().copy_from(default_inst<::apache::thrift::fixtures::types::MinPaddingWithCustomType>().big_ref());
      return;
    case 2:
      obj.medium_ref().copy_from(default_inst<::apache::thrift::fixtures::types::MinPaddingWithCustomType>().medium_ref());
      return;
    case 3:
      obj.biggish_ref().copy_from(default_inst<::apache::thrift::fixtures::types::MinPaddingWithCustomType>().biggish_ref());
      return;
    case 4:
      obj.tiny_ref().copy_from(default_inst<::apache::thrift::fixtures::types::MinPaddingWithCustomType>().tiny_ref());
      return;
  }
}

template<>
void reset_field<::apache::thrift::fixtures::types::MyStruct>(
    ::apache::thrift::fixtures::types::MyStruct& obj, uint16_t index) {
  switch (index) {
    case 0:
      obj.MyIntField_ref().copy_from(default_inst<::apache::thrift::fixtures::types::MyStruct>().MyIntField_ref());
      return;
    case 1:
      obj.MyStringField_ref().copy_from(default_inst<::apache::thrift::fixtures::types::MyStruct>().MyStringField_ref());
      return;
    case 2:
      obj.majorVer_ref().copy_from(default_inst<::apache::thrift::fixtures::types::MyStruct>().majorVer_ref());
      return;
    case 3:
      obj.data_ref().copy_from(default_inst<::apache::thrift::fixtures::types::MyStruct>().data_ref());
      return;
  }
}

template<>
void reset_field<::apache::thrift::fixtures::types::MyDataItem>(
    ::apache::thrift::fixtures::types::MyDataItem& obj, uint16_t index) {
  switch (index) {
  }
}

template<>
void reset_field<::apache::thrift::fixtures::types::Renaming>(
    ::apache::thrift::fixtures::types::Renaming& obj, uint16_t index) {
  switch (index) {
    case 0:
      obj.bar_ref().copy_from(default_inst<::apache::thrift::fixtures::types::Renaming>().bar_ref());
      return;
  }
}

template<>
void reset_field<::apache::thrift::fixtures::types::AnnotatedTypes>(
    ::apache::thrift::fixtures::types::AnnotatedTypes& obj, uint16_t index) {
  switch (index) {
    case 0:
      obj.binary_field_ref().copy_from(default_inst<::apache::thrift::fixtures::types::AnnotatedTypes>().binary_field_ref());
      return;
    case 1:
      obj.list_field_ref().copy_from(default_inst<::apache::thrift::fixtures::types::AnnotatedTypes>().list_field_ref());
      return;
  }
}

template<>
void reset_field<::apache::thrift::fixtures::types::ForwardUsageRoot>(
    ::apache::thrift::fixtures::types::ForwardUsageRoot& obj, uint16_t index) {
  switch (index) {
    case 0:
      obj.ForwardUsageStruct_ref().copy_from(default_inst<::apache::thrift::fixtures::types::ForwardUsageRoot>().ForwardUsageStruct_ref());
      return;
    case 1:
      obj.ForwardUsageByRef_ref().reset();
      return;
  }
}

template<>
void reset_field<::apache::thrift::fixtures::types::ForwardUsageStruct>(
    ::apache::thrift::fixtures::types::ForwardUsageStruct& obj, uint16_t index) {
  switch (index) {
    case 0:
      obj.foo_ref().reset();
      return;
  }
}

template<>
void reset_field<::apache::thrift::fixtures::types::ForwardUsageByRef>(
    ::apache::thrift::fixtures::types::ForwardUsageByRef& obj, uint16_t index) {
  switch (index) {
    case 0:
      obj.foo_ref().reset();
      return;
  }
}

template<>
void reset_field<::apache::thrift::fixtures::types::IncompleteMap>(
    ::apache::thrift::fixtures::types::IncompleteMap& obj, uint16_t index) {
  switch (index) {
    case 0:
      obj.field_ref().copy_from(default_inst<::apache::thrift::fixtures::types::IncompleteMap>().field_ref());
      return;
  }
}

template<>
void reset_field<::apache::thrift::fixtures::types::IncompleteMapDep>(
    ::apache::thrift::fixtures::types::IncompleteMapDep& obj, uint16_t index) {
  switch (index) {
  }
}

template<>
void reset_field<::apache::thrift::fixtures::types::CompleteMap>(
    ::apache::thrift::fixtures::types::CompleteMap& obj, uint16_t index) {
  switch (index) {
    case 0:
      obj.field_ref().copy_from(default_inst<::apache::thrift::fixtures::types::CompleteMap>().field_ref());
      return;
  }
}

template<>
void reset_field<::apache::thrift::fixtures::types::CompleteMapDep>(
    ::apache::thrift::fixtures::types::CompleteMapDep& obj, uint16_t index) {
  switch (index) {
  }
}

template<>
void reset_field<::apache::thrift::fixtures::types::IncompleteList>(
    ::apache::thrift::fixtures::types::IncompleteList& obj, uint16_t index) {
  switch (index) {
    case 0:
      obj.field_ref().copy_from(default_inst<::apache::thrift::fixtures::types::IncompleteList>().field_ref());
      return;
  }
}

template<>
void reset_field<::apache::thrift::fixtures::types::IncompleteListDep>(
    ::apache::thrift::fixtures::types::IncompleteListDep& obj, uint16_t index) {
  switch (index) {
  }
}

template<>
void reset_field<::apache::thrift::fixtures::types::CompleteList>(
    ::apache::thrift::fixtures::types::CompleteList& obj, uint16_t index) {
  switch (index) {
    case 0:
      obj.field_ref().copy_from(default_inst<::apache::thrift::fixtures::types::CompleteList>().field_ref());
      return;
  }
}

template<>
void reset_field<::apache::thrift::fixtures::types::CompleteListDep>(
    ::apache::thrift::fixtures::types::CompleteListDep& obj, uint16_t index) {
  switch (index) {
  }
}

template<>
void reset_field<::apache::thrift::fixtures::types::AdaptedList>(
    ::apache::thrift::fixtures::types::AdaptedList& obj, uint16_t index) {
  switch (index) {
    case 0:
      obj.field_ref().copy_from(default_inst<::apache::thrift::fixtures::types::AdaptedList>().field_ref());
      return;
  }
}

template<>
void reset_field<::apache::thrift::fixtures::types::DependentAdaptedList>(
    ::apache::thrift::fixtures::types::DependentAdaptedList& obj, uint16_t index) {
  switch (index) {
    case 0:
      obj.field_ref().copy_from(default_inst<::apache::thrift::fixtures::types::DependentAdaptedList>().field_ref());
      return;
  }
}

template<>
void reset_field<::apache::thrift::fixtures::types::AllocatorAware>(
    ::apache::thrift::fixtures::types::AllocatorAware& obj, uint16_t index) {
  switch (index) {
    case 0:
      obj.aa_list_ref().copy_from(default_inst<::apache::thrift::fixtures::types::AllocatorAware>().aa_list_ref());
      return;
    case 1:
      obj.aa_set_ref().copy_from(default_inst<::apache::thrift::fixtures::types::AllocatorAware>().aa_set_ref());
      return;
    case 2:
      obj.aa_map_ref().copy_from(default_inst<::apache::thrift::fixtures::types::AllocatorAware>().aa_map_ref());
      return;
    case 3:
      obj.aa_string_ref().copy_from(default_inst<::apache::thrift::fixtures::types::AllocatorAware>().aa_string_ref());
      return;
    case 4:
      obj.not_a_container_ref().copy_from(default_inst<::apache::thrift::fixtures::types::AllocatorAware>().not_a_container_ref());
      return;
    case 5:
      obj.aa_unique_ref().reset();
      return;
    case 6:
      obj.aa_shared_ref().reset();
      return;
  }
}

template<>
void reset_field<::apache::thrift::fixtures::types::AllocatorAware2>(
    ::apache::thrift::fixtures::types::AllocatorAware2& obj, uint16_t index) {
  switch (index) {
    case 0:
      obj.not_a_container_ref().copy_from(default_inst<::apache::thrift::fixtures::types::AllocatorAware2>().not_a_container_ref());
      return;
  }
}

template<>
void reset_field<::apache::thrift::fixtures::types::TypedefStruct>(
    ::apache::thrift::fixtures::types::TypedefStruct& obj, uint16_t index) {
  switch (index) {
    case 0:
      obj.i32_field_ref().copy_from(default_inst<::apache::thrift::fixtures::types::TypedefStruct>().i32_field_ref());
      return;
    case 1:
      obj.IntTypedef_field_ref().copy_from(default_inst<::apache::thrift::fixtures::types::TypedefStruct>().IntTypedef_field_ref());
      return;
    case 2:
      obj.UintTypedef_field_ref().copy_from(default_inst<::apache::thrift::fixtures::types::TypedefStruct>().UintTypedef_field_ref());
      return;
  }
}

template<>
void reset_field<::apache::thrift::fixtures::types::StructWithDoubleUnderscores>(
    ::apache::thrift::fixtures::types::StructWithDoubleUnderscores& obj, uint16_t index) {
  switch (index) {
    case 0:
      obj.__field_ref().copy_from(default_inst<::apache::thrift::fixtures::types::StructWithDoubleUnderscores>().__field_ref());
      return;
  }
}

template<>
const std::unordered_map<std::string_view, std::string_view>& PyStructTraits<
    ::apache::thrift::fixtures::types::decorated_struct>::namesmap() {
  static const folly::Indestructible<NamesMap> map {
    {
    }
  };
  return *map;
}

template<>
const std::unordered_map<std::string_view, std::string_view>& PyStructTraits<
    ::apache::thrift::fixtures::types::ContainerStruct>::namesmap() {
  static const folly::Indestructible<NamesMap> map {
    {
    }
  };
  return *map;
}

template<>
const std::unordered_map<std::string_view, std::string_view>& PyStructTraits<
    ::apache::thrift::fixtures::types::CppTypeStruct>::namesmap() {
  static const folly::Indestructible<NamesMap> map {
    {
    }
  };
  return *map;
}

template<>
const std::unordered_map<std::string_view, std::string_view>& PyStructTraits<
    ::apache::thrift::fixtures::types::VirtualStruct>::namesmap() {
  static const folly::Indestructible<NamesMap> map {
    {
    }
  };
  return *map;
}

template<>
const std::unordered_map<std::string_view, std::string_view>& PyStructTraits<
    ::apache::thrift::fixtures::types::MyStructWithForwardRefEnum>::namesmap() {
  static const folly::Indestructible<NamesMap> map {
    {
    }
  };
  return *map;
}

template<>
const std::unordered_map<std::string_view, std::string_view>& PyStructTraits<
    ::apache::thrift::fixtures::types::TrivialNumeric>::namesmap() {
  static const folly::Indestructible<NamesMap> map {
    {
    }
  };
  return *map;
}

template<>
const std::unordered_map<std::string_view, std::string_view>& PyStructTraits<
    ::apache::thrift::fixtures::types::TrivialNestedWithDefault>::namesmap() {
  static const folly::Indestructible<NamesMap> map {
    {
    }
  };
  return *map;
}

template<>
const std::unordered_map<std::string_view, std::string_view>& PyStructTraits<
    ::apache::thrift::fixtures::types::ComplexString>::namesmap() {
  static const folly::Indestructible<NamesMap> map {
    {
    }
  };
  return *map;
}

template<>
const std::unordered_map<std::string_view, std::string_view>& PyStructTraits<
    ::apache::thrift::fixtures::types::ComplexNestedWithDefault>::namesmap() {
  static const folly::Indestructible<NamesMap> map {
    {
    }
  };
  return *map;
}

template<>
const std::unordered_map<std::string_view, std::string_view>& PyStructTraits<
    ::apache::thrift::fixtures::types::MinPadding>::namesmap() {
  static const folly::Indestructible<NamesMap> map {
    {
    }
  };
  return *map;
}

template<>
const std::unordered_map<std::string_view, std::string_view>& PyStructTraits<
    ::apache::thrift::fixtures::types::MinPaddingWithCustomType>::namesmap() {
  static const folly::Indestructible<NamesMap> map {
    {
    }
  };
  return *map;
}

template<>
const std::unordered_map<std::string_view, std::string_view>& PyStructTraits<
    ::apache::thrift::fixtures::types::MyStruct>::namesmap() {
  static const folly::Indestructible<NamesMap> map {
    {
    }
  };
  return *map;
}

template<>
const std::unordered_map<std::string_view, std::string_view>& PyStructTraits<
    ::apache::thrift::fixtures::types::MyDataItem>::namesmap() {
  static const folly::Indestructible<NamesMap> map {
    {
    }
  };
  return *map;
}

template<>
const std::unordered_map<std::string_view, std::string_view>& PyStructTraits<
    ::apache::thrift::fixtures::types::Renaming>::namesmap() {
  static const folly::Indestructible<NamesMap> map {
    {
    }
  };
  return *map;
}

template<>
const std::unordered_map<std::string_view, std::string_view>& PyStructTraits<
    ::apache::thrift::fixtures::types::AnnotatedTypes>::namesmap() {
  static const folly::Indestructible<NamesMap> map {
    {
    }
  };
  return *map;
}

template<>
const std::unordered_map<std::string_view, std::string_view>& PyStructTraits<
    ::apache::thrift::fixtures::types::ForwardUsageRoot>::namesmap() {
  static const folly::Indestructible<NamesMap> map {
    {
    }
  };
  return *map;
}

template<>
const std::unordered_map<std::string_view, std::string_view>& PyStructTraits<
    ::apache::thrift::fixtures::types::ForwardUsageStruct>::namesmap() {
  static const folly::Indestructible<NamesMap> map {
    {
    }
  };
  return *map;
}

template<>
const std::unordered_map<std::string_view, std::string_view>& PyStructTraits<
    ::apache::thrift::fixtures::types::ForwardUsageByRef>::namesmap() {
  static const folly::Indestructible<NamesMap> map {
    {
    }
  };
  return *map;
}

template<>
const std::unordered_map<std::string_view, std::string_view>& PyStructTraits<
    ::apache::thrift::fixtures::types::IncompleteMap>::namesmap() {
  static const folly::Indestructible<NamesMap> map {
    {
    }
  };
  return *map;
}

template<>
const std::unordered_map<std::string_view, std::string_view>& PyStructTraits<
    ::apache::thrift::fixtures::types::IncompleteMapDep>::namesmap() {
  static const folly::Indestructible<NamesMap> map {
    {
    }
  };
  return *map;
}

template<>
const std::unordered_map<std::string_view, std::string_view>& PyStructTraits<
    ::apache::thrift::fixtures::types::CompleteMap>::namesmap() {
  static const folly::Indestructible<NamesMap> map {
    {
    }
  };
  return *map;
}

template<>
const std::unordered_map<std::string_view, std::string_view>& PyStructTraits<
    ::apache::thrift::fixtures::types::CompleteMapDep>::namesmap() {
  static const folly::Indestructible<NamesMap> map {
    {
    }
  };
  return *map;
}

template<>
const std::unordered_map<std::string_view, std::string_view>& PyStructTraits<
    ::apache::thrift::fixtures::types::IncompleteList>::namesmap() {
  static const folly::Indestructible<NamesMap> map {
    {
    }
  };
  return *map;
}

template<>
const std::unordered_map<std::string_view, std::string_view>& PyStructTraits<
    ::apache::thrift::fixtures::types::IncompleteListDep>::namesmap() {
  static const folly::Indestructible<NamesMap> map {
    {
    }
  };
  return *map;
}

template<>
const std::unordered_map<std::string_view, std::string_view>& PyStructTraits<
    ::apache::thrift::fixtures::types::CompleteList>::namesmap() {
  static const folly::Indestructible<NamesMap> map {
    {
    }
  };
  return *map;
}

template<>
const std::unordered_map<std::string_view, std::string_view>& PyStructTraits<
    ::apache::thrift::fixtures::types::CompleteListDep>::namesmap() {
  static const folly::Indestructible<NamesMap> map {
    {
    }
  };
  return *map;
}

template<>
const std::unordered_map<std::string_view, std::string_view>& PyStructTraits<
    ::apache::thrift::fixtures::types::AdaptedList>::namesmap() {
  static const folly::Indestructible<NamesMap> map {
    {
    }
  };
  return *map;
}

template<>
const std::unordered_map<std::string_view, std::string_view>& PyStructTraits<
    ::apache::thrift::fixtures::types::DependentAdaptedList>::namesmap() {
  static const folly::Indestructible<NamesMap> map {
    {
    }
  };
  return *map;
}

template<>
const std::unordered_map<std::string_view, std::string_view>& PyStructTraits<
    ::apache::thrift::fixtures::types::AllocatorAware>::namesmap() {
  static const folly::Indestructible<NamesMap> map {
    {
    }
  };
  return *map;
}

template<>
const std::unordered_map<std::string_view, std::string_view>& PyStructTraits<
    ::apache::thrift::fixtures::types::AllocatorAware2>::namesmap() {
  static const folly::Indestructible<NamesMap> map {
    {
    }
  };
  return *map;
}

template<>
const std::unordered_map<std::string_view, std::string_view>& PyStructTraits<
    ::apache::thrift::fixtures::types::TypedefStruct>::namesmap() {
  static const folly::Indestructible<NamesMap> map {
    {
    }
  };
  return *map;
}

template<>
const std::unordered_map<std::string_view, std::string_view>& PyStructTraits<
    ::apache::thrift::fixtures::types::StructWithDoubleUnderscores>::namesmap() {
  static const folly::Indestructible<NamesMap> map {
    {
    }
  };
  return *map;
}
} // namespace py3
} // namespace thrift
