
#include "parse.hpp"

namespace Lark {
  // statement-sequence = statement*
  auto parse_statement_sequence (Cursor cursor) -> Match <Statements> {
    Statements statements;
    for (;;) {
      auto statement = parse_statement (cursor);
      if (!statement) break;
      statements.push_back (statement.result);
      cursor = statement.end;
    }
    return { cursor, std::move (statements) };
  }

  // block = `do` statement-sequence? `end`
  auto parse_block (Cursor cursor) -> Match <Block*> {
    auto open = expect (cursor, "do");
    if (!open) return cursor;

    auto statements = parse_statement_sequence (open.end.skip_nl ());

    auto close = expect (statements.end.skip_nl (), "end");
    if (!close) throw ParseError ("expected `end`");

    auto block = new Block { std::move (statements.result) };
    return { close.end, block };
  }

}
