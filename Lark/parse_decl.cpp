
#include "parse.hpp"

#include <catch.hpp>

namespace Lark {
  auto parse_decl (Cursor cursor) -> Match <Decl> {
    return parse_function (cursor);
  }
}
