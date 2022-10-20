/**
 * Autogenerated by Thrift for thrift/compiler/test/fixtures/no-legacy-apis/src/module.thrift
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated @nocommit
 */

#include "thrift/compiler/test/fixtures/no-legacy-apis/gen-cpp2/module_data.h"

#include <thrift/lib/cpp2/gen/module_data_cpp.h>

#if defined(__GNUC__) && defined(__linux__) && !FOLLY_MOBILE
// This attribute is applied to the static data members to ensure that they are
// not stripped from the compiled binary, in order to keep them available for
// use by debuggers at runtime.
//
// The attribute works by forcing all of the data members (both used and unused
// ones) into the same section. This stops the linker from stripping the unused
// data, as it works on a per-section basis and only removes sections if they
// are entirely unused.
#define THRIFT_DATA_SECTION [[gnu::section(".rodata.thrift.data")]]
#else
#define THRIFT_DATA_SECTION
#endif

namespace apache {
namespace thrift {

THRIFT_DATA_SECTION const std::array<::test::fixtures::basic::MyEnum, 2> TEnumDataStorage<::test::fixtures::basic::MyEnum>::values = {{
  type::MyValue1,
  type::MyValue2,
}};
THRIFT_DATA_SECTION const std::array<folly::StringPiece, 2> TEnumDataStorage<::test::fixtures::basic::MyEnum>::names = {{
  "MyValue1",
  "MyValue2",
}};

THRIFT_DATA_SECTION const std::array<::test::fixtures::basic::MyUnion::Type, 2> TEnumDataStorage<::test::fixtures::basic::MyUnion::Type>::values = {{
  type::myEnum,
  type::myDataItem,
}};
THRIFT_DATA_SECTION const std::array<folly::StringPiece, 2> TEnumDataStorage<::test::fixtures::basic::MyUnion::Type>::names = {{
  "myEnum",
  "myDataItem",
}};

THRIFT_DATA_SECTION const std::array<folly::StringPiece, 2> TStructDataStorage<::test::fixtures::basic::MyStruct>::fields_names = {{
  "myIntField",
  "myStringField",
}};
THRIFT_DATA_SECTION const std::array<int16_t, 2> TStructDataStorage<::test::fixtures::basic::MyStruct>::fields_ids = {{
  1,
  2,
}};
THRIFT_DATA_SECTION const std::array<protocol::TType, 2> TStructDataStorage<::test::fixtures::basic::MyStruct>::fields_types = {{
  TType::T_I64,
  TType::T_STRING,
}};
THRIFT_DATA_SECTION const std::array<folly::StringPiece, 2> TStructDataStorage<::test::fixtures::basic::MyStruct>::storage_names = {{
  "__fbthrift_field_myIntField",
  "__fbthrift_field_myStringField",
}};
THRIFT_DATA_SECTION const std::array<int, 2> TStructDataStorage<::test::fixtures::basic::MyStruct>::isset_indexes = {{
  0,
  1,
}};

THRIFT_DATA_SECTION const std::array<folly::StringPiece, 2> TStructDataStorage<::test::fixtures::basic::MyUnion>::fields_names = {{
  "myEnum",
  "myDataItem",
}};
THRIFT_DATA_SECTION const std::array<int16_t, 2> TStructDataStorage<::test::fixtures::basic::MyUnion>::fields_ids = {{
  1,
  2,
}};
THRIFT_DATA_SECTION const std::array<protocol::TType, 2> TStructDataStorage<::test::fixtures::basic::MyUnion>::fields_types = {{
  TType::T_I32,
  TType::T_STRUCT,
}};
THRIFT_DATA_SECTION const std::array<folly::StringPiece, 2> TStructDataStorage<::test::fixtures::basic::MyUnion>::storage_names = {{
  "myEnum",
  "myDataItem",
}};
THRIFT_DATA_SECTION const std::array<int, 2> TStructDataStorage<::test::fixtures::basic::MyUnion>::isset_indexes = {{
  0,
  1,
}};

} // namespace thrift
} // namespace apache
