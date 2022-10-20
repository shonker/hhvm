/**
 * Autogenerated by Thrift for thrift/compiler/test/fixtures/qualified/src/module2.thrift
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated @nocommit
 */

#include "thrift/compiler/test/fixtures/qualified/gen-cpp2/module2_data.h"

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

THRIFT_DATA_SECTION const std::array<folly::StringPiece, 2> TStructDataStorage<::module2::Struct>::fields_names = {{
  "first",
  "second",
}};
THRIFT_DATA_SECTION const std::array<int16_t, 2> TStructDataStorage<::module2::Struct>::fields_ids = {{
  1,
  2,
}};
THRIFT_DATA_SECTION const std::array<protocol::TType, 2> TStructDataStorage<::module2::Struct>::fields_types = {{
  TType::T_STRUCT,
  TType::T_STRUCT,
}};
THRIFT_DATA_SECTION const std::array<folly::StringPiece, 2> TStructDataStorage<::module2::Struct>::storage_names = {{
  "__fbthrift_field_first",
  "__fbthrift_field_second",
}};
THRIFT_DATA_SECTION const std::array<int, 2> TStructDataStorage<::module2::Struct>::isset_indexes = {{
  0,
  1,
}};

THRIFT_DATA_SECTION const std::array<folly::StringPiece, 2> TStructDataStorage<::module2::BigStruct>::fields_names = {{
  "s",
  "id",
}};
THRIFT_DATA_SECTION const std::array<int16_t, 2> TStructDataStorage<::module2::BigStruct>::fields_ids = {{
  1,
  2,
}};
THRIFT_DATA_SECTION const std::array<protocol::TType, 2> TStructDataStorage<::module2::BigStruct>::fields_types = {{
  TType::T_STRUCT,
  TType::T_I32,
}};
THRIFT_DATA_SECTION const std::array<folly::StringPiece, 2> TStructDataStorage<::module2::BigStruct>::storage_names = {{
  "__fbthrift_field_s",
  "__fbthrift_field_id",
}};
THRIFT_DATA_SECTION const std::array<int, 2> TStructDataStorage<::module2::BigStruct>::isset_indexes = {{
  0,
  1,
}};

} // namespace thrift
} // namespace apache
