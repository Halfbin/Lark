
#include "parse.hpp"

namespace Lark {
  namespace {
    auto parse_parenthesis (Cursor cursor) -> Match <Expression*> {
      auto open = expect (cursor, "(");
      if (!open) return cursor;

      auto inner = parse_expression (open.end);
      if (!inner) throw ParseError ("expected expression");

      auto close = expect (inner.end, ")");
      if (!close) throw ParseError ("expected `)`");

      return { close.end, inner.result };
    }

    auto parse_literal (Cursor cursor) -> Match <Literal*> {
      auto match = expect (is_literal, cursor);
      if (!match) return cursor;
      auto literal = new Literal { match.result };
      return { match.end, literal };
    }

    auto parse_reference (Cursor cursor) -> Match <Reference*> {
      auto match = expect (is_identifier, cursor);
      if (!match) return cursor;
      auto reference = new Reference { match.result };
      return { match.end, reference };
    }

  }

  auto parse_primary (Cursor cursor) -> Match <Expression*> {
    return parse_first_of <Expression*> (cursor,
      parse_parenthesis,
      parse_literal,
      parse_call,
      parse_reference
    );
  }

}
