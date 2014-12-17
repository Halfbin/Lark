
#include "parse.hpp"

#include <catch.hpp>

namespace Lark {
  auto StmtParser::parse (Cursor cursor) const -> Match <Stmt> {
    auto block = blockstmt_parser.parse (cursor);
    if (block) return block;

    auto line = linestmt_parser.parse (cursor);
    auto term = expect (line.end, ";");
    if (line && !term) throw ParseError ("expected `;`");
    else if (!term) return cursor;
    else return { term.end, std::move (line.result) };
  }

  using namespace TokenHelpers;

  TEST_CASE ("StmtParser") {
    auto make_stmt = [] () -> Stmt {
      return std::make_unique <StmtBase> (StmtType::test);
    };

    StubParser <Stmt> blockstmt_stub ("<block-stmt>", make_stmt);
    StubParser <Stmt> linestmt_stub  ("<line-stmt>",  make_stmt);

    StmtParser p (linestmt_stub, blockstmt_stub);

    SECTION ("parses block statements") {
      Token tokens [] = { id("<block-stmt>"), t_end };
      auto m = p.parse (tokens);
      REQUIRE (m);
      REQUIRE (m.end->kind == TK::end);
      REQUIRE (m.result);
      REQUIRE (m.result->ty == StmtType::test);
    }

    SECTION ("parses line statements") {
      Token tokens [] = { id("<line-stmt>"), pn(";"), t_end };
      auto m = p.parse (tokens);
      REQUIRE (m);
      REQUIRE (m.end->kind == TK::end);
      REQUIRE (m.result);
      REQUIRE (m.result->ty == StmtType::test);
    }

    SECTION ("fails for unterminated line statements") {
      Token tokens [] = { id("<line-stmt>"), id("not_a_semicolon"), t_end };
      REQUIRE_THROWS (p.parse (tokens));
    }

    SECTION ("rejects non-statements") {
      Token tokens [] = { id("<non-stmt>"), t_end };
      auto m = p.parse (tokens);
      REQUIRE (!m);
    }
  }

}
