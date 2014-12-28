
#include "parse.hpp"

#include <catch.hpp>

namespace Lark {
  auto IfParser::parse (Cursor cursor) const -> Match <If> {
    std::vector <IfClause> clauses;

    auto head = clause_parser.parse (cursor);
    if (!head) return cursor;
    else clauses.push_back (std::move (head.result));
    cursor = head.end;

    for (;;) {
      auto else_key = expect (cursor, "else");
      if (!else_key) break;

      auto clause = clause_parser.parse (else_key.end);
      if (!clause) break;
      else clauses.push_back (std::move (clause.result));

      cursor = clause.end;
    }

    Stmt else_block;
    auto else_key = expect (cursor, "else");
    if (else_key) {
      auto else_match = block_parser.parse (else_key.end);
      if (!else_match) throw ParseError ("expected else body");
      else else_block = std::move (else_match.result);
      cursor = else_match.end;
    }

    auto node = std::make_unique <IfNode> (
      std::move (clauses), std::move (else_block)
    );
    return { cursor, std::move (node) };
  }

  using namespace TokenHelpers;

  TEST_CASE ("IfParser") {
    StubParser <IfClause> clause_stub (
      "<clause>", [] () -> IfClause { return { }; }
    );
    StubParser <Block> block_stub (
      "<block>", [] () -> Block { return std::make_unique <BlockNode> (); }
    );
    IfParser p { clause_stub, block_stub };

    SECTION ("parses if-statements with one clause") {
      Token tokens [] = {
        id("<clause>"), t_end
      };
      auto m = p.parse (tokens);
      REQUIRE (m);
      REQUIRE (m.result->ty == StmtType::if_else);
      REQUIRE (m.result->clauses.size () == 1);
      REQUIRE (!m.result->else_stmt);
      REQUIRE (m.end->kind == TK::end);
    }

    SECTION ("parses if-statements with many clauses") {
      Token tokens [] = {
        id("<clause>"), kw("else"), id("<clause>"), t_end
      };
      auto m = p.parse (tokens);
      REQUIRE (m);
      REQUIRE (m.result->ty == StmtType::if_else);
      REQUIRE (m.result->clauses.size () == 2);
      REQUIRE (!m.result->else_stmt);
      REQUIRE (m.end->kind == TK::end);
    }

    SECTION ("parses if-statements with else-clauses") {
      Token tokens [] = {
        id("<clause>"), kw("else"), id("<block>"), t_end
      };
      auto m = p.parse (tokens);
      REQUIRE (m);
      REQUIRE (m.result->ty == StmtType::if_else);
      REQUIRE (m.result->clauses.size () == 1);
      REQUIRE (m.result->else_stmt);
      REQUIRE (m.end->kind == TK::end);
    }

    SECTION ("rejects non-if-statements") {
      Token tokens [] = { id("derp"), t_end };
      auto m = p.parse (tokens);
      REQUIRE (!m);
    }

    SECTION ("fails for trailing elses") {
      Token tokens [] = { id("<clause>"), kw("else"), t_end };
      REQUIRE_THROWS (p.parse (tokens));
    }
  }
}
