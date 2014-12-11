
#include "parse.hpp"

namespace Lark {
  namespace {
    bool is_key (Token token) {
      return token.spelling == "if"
          || token.spelling == "unless";
    }

    auto parse_key (Cursor cursor) -> Match <Rk::cstring_ref> {
      if (!is_key (*cursor)) return cursor;
      else return { cursor.next (), cursor->spelling };
    }

    auto parse_condition (Cursor cursor) -> Match <If::Condition> {
      auto condition = parse_expression (cursor);
      if (!condition) throw ParseError ("expected condition");
      else return condition;
    }

    auto parse_body (Cursor cursor) -> Match <If::Body> {
      auto body = parse_block (cursor);
      if (!body) throw ParseError ("expected block");
      else return body;
    }

  }

  auto parse_if (Cursor cursor) -> Match <If*> {
    auto key = parse_key (cursor);
    if (!key) return cursor;

    auto condition = parse_condition (key.end);
    auto body      = parse_body      (condition.end);

    auto node = new If { condition.result, body.result };
    return { body.end, node };
  }

}
