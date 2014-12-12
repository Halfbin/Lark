
#include "parse.hpp"

#include <catch.hpp>

namespace Lark {
  auto BlockParser::parse (Cursor cursor) const -> Match <Block> {
    auto open = expect (cursor, "do");
    if (!open) return cursor;

    auto stmtseq = stmtseq_parser.parse (open.end);

    auto close = expect (stmtseq.end, "end");
    if (!close) throw ParseError ("expected `end` at end of block");

    Block block = std::make_unique <BlockNode> (std::move (stmtseq.result));
    return { close.end, std::move (block) };
  }

  using namespace TokenHelpers;

  TEST_CASE ("BlockParser") {
    StubParser <StmtSeq> stmtseq_stub (
      "<stmt*>",
      [] () -> StmtSeq { StmtSeq seq; seq.push_back (nullptr); return seq; }
    );
    BlockParser p (stmtseq_stub);

    SECTION ("parses blocks") {
      Token tokens [] = { kw("do"), id("<stmt*>"), kw("end"), t_end };
      auto m = p.parse (tokens);
      REQUIRE (m);
      REQUIRE (m.result->stmts.size () == 1);
      REQUIRE (m.end->kind == TK::end);
    }

    SECTION ("rejects non-blocks") {
      Token tokens [] = { kw("dont"), id("<stmt*>"), kw("bend"), t_end };
      auto m = p.parse (tokens);
      REQUIRE (!m);
    }

    SECTION ("accepts empty blocks") {
      Token tokens [] = { kw("do"), kw("end"), t_end };
      auto m = p.parse (tokens);
      REQUIRE (m);
      REQUIRE (m.result->stmts.empty ());
      REQUIRE (m.end->kind == TK::end);
    }

    SECTION ("fails for unclosed blocks") {
      Token tokens [] = { kw("do"), kw("<stmt*>"), kw("blend"), t_end };
      REQUIRE_THROWS (p.parse (tokens));
    }
  }
}
