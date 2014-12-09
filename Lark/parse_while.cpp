
#include "parse.hpp"

namespace Lark {
  namespace {
    bool is_key (Token token) {
      return token.spelling == "while"
          || token.spelling == "until";
    }

    auto parse_key (Cursor cursor) -> Match <Rk::cstring_ref> {
      if (!is_key (*cursor)) return cursor;
      else return { cursor.next (), cursor->spelling };
    }

    auto parse_condition (Cursor cursor) -> Match <While::Condition> {
      return parse_expression (cursor);
    }

    auto parse_body (Cursor cursor) -> Match <While::Body> {
      return parse_statement (cursor);
    }

  }

  auto parse_while (Cursor cursor) -> Match <While*> {
    auto key = parse_key (cursor);
    if (!key) return cursor;

    auto condition = parse_condition (key.end.skip_nl ());
    auto body = parse_body (condition.end.skip_nl ());

    auto node = new While { condition.result, body.result };
    return { body.end, node };
  }

}

