
#include "parse.hpp"

namespace Lark {
  namespace {
    bool is_terminator (Token token) {
      return token.kind     == TokenKind::newline
          || token.spelling == ";";
    }

    auto parse_terminator (Cursor cursor) -> Match <Rk::cstring_ref> {
      if (!is_terminator (*cursor)) return cursor;
      else return { cursor.next (), cursor->spelling };
    }

  }

  auto parse_line_statement (Cursor cursor) -> Match <Statement*> {
    auto statement = parse_first_of <Statement*> (cursor,
      parse_return,
      parse_assignment,
      parse_expression);

    auto term = parse_terminator (statement.end);
    if (!term) throw ParseError ("expected `;` or newline");
    return { term.end, statement.result };
  }

  auto parse_block_statement (Cursor cursor) -> Match <Statement*> {
    return parse_first_of <Statement*> (cursor,
      parse_block,
      parse_if,
      parse_while,
      parse_loop,
      parse_for);
  }

  auto parse_statement (Cursor cursor) -> Match <Statement*> {
    return parse_first_of <Statement*> (cursor,
      parse_block_statement,
      parse_line_statement);
  }

}
