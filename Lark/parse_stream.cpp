
#include "parse.hpp"

namespace Lark {
  auto parse_stream (Cursor cursor) -> Match <Stream> {
    return parse_star (cursor, parse_decl);
  }

}
