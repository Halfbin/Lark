
#include "parse.hpp"

#include <catch.hpp>

namespace Lark {
  auto parse_oblock (Cursor cursor) -> Match <Block> {
    auto begin = expect (cursor, "do");
    if (!begin) return cursor;

    auto stmts = parse_stmt_seq (begin.end);

    auto node = std::make_unique <BlockNode> (std::move (stmts.result));
    return { stmts.end, std::move (node) };
  }

  auto parse_block (Cursor cursor) -> Match <Block> {
    auto oblock = parse_oblock (cursor);
    if (!oblock) return cursor;

    auto end = expect (oblock.end, "end");
    if (!end) throw ParseError ("expected `end` at end of block");

    return { end.end, std::move (oblock.result) };
  }
}
