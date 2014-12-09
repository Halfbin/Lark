
#include "parse.hpp"

namespace Lark {
  // stream = function*
  auto parse_stream (Cursor cursor) -> Match <Stream*> {
    std::vector <Function*> functions;

    while (*cursor) {
      auto function = parse_function (cursor);
      if (!function) throw ParseError ("expected function");
      functions.push_back (function.result);
      cursor = function.end.skip_nl ();
    }

    auto stream = new Stream { std::move (functions) };
    return { cursor, stream };
  }

}
