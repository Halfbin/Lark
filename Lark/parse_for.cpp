
#include "parse.hpp"

namespace Lark {
  namespace {
    bool is_key (Token token) {
      return token.spelling == "for";
    }

    auto parse_key (Cursor cursor) -> Match <Rk::cstring_ref> {
      return expect (is_key, cursor);
    }

    auto parse_spec (Cursor cursor) -> Match <For::Spec> {
      return parse_assignment (cursor);
    }

    auto parse_body (Cursor cursor) -> Match <For::Body> {
      return parse_block (cursor);
    }

  }

  auto parse_for (Cursor cursor) -> Match <For*> {
    auto key = parse_key (cursor);
    if (!key) return cursor;

    auto spec = parse_spec (key.end);
    auto body = parse_body (spec.end);

    auto node = new For { spec.result, body.result };
    return { body.end, node };
  }

}
