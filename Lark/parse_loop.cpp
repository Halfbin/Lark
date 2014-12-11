
#include "parse.hpp"

namespace Lark {
  namespace {
    bool is_key (Token token) {
      return token.spelling == "loop";
    }

    auto parse_key (Cursor cursor) -> Match <Rk::cstring_ref> {
      if (!is_key (*cursor)) return cursor;
      else return { cursor.next (), cursor->spelling };
    }

    auto parse_body (Cursor cursor) -> Match <Loop::Body> {
      return parse_block (cursor);
    }

  }

  auto parse_loop (Cursor cursor) -> Match <Loop*> {
    auto key = parse_key (cursor);
    if (!key) return cursor;

    auto body = parse_body (key.end);

    auto node = new Loop { body.result };
    return { body.end, node };
  }

}

