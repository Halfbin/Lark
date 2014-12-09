
#include "parse.hpp"

namespace Lark {
  namespace {
    bool is_key (Token token) {
      return token.spelling == "for";
    }

    auto parse_key (Cursor cursor) -> Match <Rk::cstring_ref> {
      return expect (cursor, is_key);
    }

    auto parse_spec (Cursor cursor) -> Match <For::Spec> {
      return parse_assignment (cursor);
    }

    auto parse_body (Cursor cursor) -> Match <For::Body> {
      return parse_statement (cursor);
    }

  }

  auto parse_for (Cursor cursor) -> Match <For*> {
    auto key = parse_key (cursor);
    if (!key) return cursor;

    auto spec = parse_spec (key.end.skip_nl ());
    auto body = parse_body (spec.end.skip_nl ());

    auto node = new For { spec.result, body.result };
    return { body.end, node };
  }

}
