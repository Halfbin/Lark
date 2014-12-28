
#include "parse.hpp"

#include <catch.hpp>

namespace Lark {
  auto IfClauseParser::parse (Cursor cursor) const -> Match <IfClause> {
    auto key = expect (cursor, "if");
    if (!key) return cursor;

    auto cond = expr_parser.parse (key.end);
    if (!cond) throw ParseError ("expected if-clause condition");

    auto body = block_parser.parse (cond.end);
    if (!body) throw ParseError ("expected if-clause body");

    auto clause = IfClause {
      std::move (cond.result),
      std::move (body.result)
    };
    return { body.end, std::move (clause) };
  }

  using namespace TokenHelpers;

  TEST_CASE ("IfClauseParser") {
    StubParser <Expr> expr_stub (
      "<expr>", [] () -> Expr { return std::make_unique <ExprBase> (); }
    );
    StubParser <Block> block_stub (
      "<block>", [] () -> Block { return std::make_unique <BlockNode> (); }
    );
    IfClauseParser p { expr_stub, block_stub };

    SECTION ("parses if clauses") {
      Token tokens [] = { kw("if"), id("<expr>"), id("<block>"), t_end };
      auto m = p.parse (tokens);
      REQUIRE (m);
      REQUIRE (m.result.cond);
      REQUIRE (m.result.body);
      REQUIRE (m.end->kind == TK::end);
    }

    SECTION ("rejects non-ifs") {
      Token tokens [] = { id("derp"), t_end };
      auto m = p.parse (tokens);
      REQUIRE (!m);
    }

    SECTION ("fails for clauses with no condition") {
      Token tokens [] = { kw("if"), id("<block>"), t_end };
      REQUIRE_THROWS (p.parse (tokens));
    }

    SECTION ("fails for clauses with no body") {
      Token tokens [] = { kw("if"), id("<expr>"), t_end };
      REQUIRE_THROWS (p.parse (tokens));
    }
  }
}
