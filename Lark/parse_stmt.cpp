
#include "parse.hpp"

#include <catch.hpp>

namespace Lark {
  auto parse_block_stmt (Cursor cursor) -> Match <Stmt> {
    Match <Stmt> match = parse_if (cursor); if (match) return match;

    return cursor;
  }

  auto parse_line_stmt (Cursor cursor) -> Match <Stmt> {
    return parse_expr (cursor);
  }

  auto parse_stmt (Cursor cursor) -> Match <Stmt> {
    auto block = parse_block_stmt (cursor);
    if (block) return block;

    auto line = parse_line_stmt (cursor);
    auto term = expect (line.end, ";");
    if (line && !term) throw ParseError ("expected `;`");
    else if (!term) return cursor;
    else return { term.end, std::move (line.result) };
  }

  auto parse_stmt_seq (Cursor cursor) -> Match <StmtSeq> {
    return parse_star (cursor, parse_stmt);
  }
}
