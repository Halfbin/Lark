
#include "parse.hpp"

namespace Lark {
  namespace {
    bool is_key (Token token) {
      return token.spelling == "return";
    }

    auto parse_key (Cursor cursor) -> Match <bool> {
      return expect (is_key, cursor);
    }

    auto parse_value (Cursor cursor) -> Match <Expression*> {
      return parse_expression (cursor);
    }

  }

  auto parse_return (Cursor cursor) -> Match <Return*> {
    auto key = parse_key (cursor);
    if (!key) return cursor;

    auto expr = parse_value (key.end);

    auto end = expr ? expr.end : key.end;
    auto ret = new Return (expr.result);
    return { end, ret };
  }

}
