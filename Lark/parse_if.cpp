
#include "parse.hpp"

#include <catch.hpp>

namespace Lark {
  auto parse_if_clause (Cursor cursor) -> Match <IfClause> {
    auto keyword = expect (cursor, "if");
    if (!keyword) return cursor;

    auto cond = parse_expr (keyword.end);
    if (!cond) throw ParseError ("if-clause without a condition expression");

    auto block = parse_oblock (cond.end);
    if (!block) throw ParseError ("if-clause without a body");

    auto clause = IfClause { std::move (cond.result), std::move (block.result) };
    return { block.end, std::move (clause) };
  }

  auto parse_if (Cursor cursor) -> Match <If> {
    std::vector <IfClause> clauses;

    auto head = parse_if_clause (cursor);
    if (!head) return cursor;
    else clauses.push_back (std::move (head.result));
    cursor = head.end;

    for (;;) {
      auto else_key = expect (cursor, "else");
      if (!else_key) break;

      auto clause = parse_if_clause (else_key.end);
      if (!clause) break;
      else clauses.push_back (std::move (clause.result));

      cursor = clause.end;
    }

    Stmt else_block;
    auto else_key = expect (cursor, "else");
    if (else_key) {
      auto else_match = parse_oblock (else_key.end);
      if (!else_match) throw ParseError ("else-clause without a body");
      else else_block = std::move (else_match.result);
      cursor = else_match.end;
    }

    auto end_key = expect (cursor, "end");
    if (!end_key) throw ParseError ("if-statement without terminating `end`");

    auto node = std::make_unique <IfNode> (
      std::move (clauses), std::move (else_block)
    );
    return { cursor, std::move (node) };
  }
}
