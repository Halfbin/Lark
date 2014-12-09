
#include "parse.hpp"

namespace Lark {
  namespace {
    auto parse_key (Cursor cursor) -> Match <bool> {
      return expect (cursor, "return");
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
