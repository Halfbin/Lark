
#include "parse.hpp"

#include <catch.hpp>

namespace Lark {
  auto StreamParser::parse (Cursor cursor) const -> Match <Stream> {
    auto decls = parse_any_number (decl_parser, cursor);
    auto end = expect (decls.end, TokenKind::end);
    if (!end) throw ParseError ("expected decl or end-of-file");
    return { decls.end, std::move (decls.result) };
  }

  using namespace TokenHelpers;

  TEST_CASE ("StreamParser") {
    StubParser <Decl> dp ("<decl>");
    StreamParser p { dp };

    SECTION ("parses decls") {
      Token tokens[] = { id("<decl>"), id("<decl>"), id("<decl>"), t_end };
      auto s = p.parse (tokens);
      REQUIRE (s.result.size () == 3);
      REQUIRE (s.end->kind == TK::end);
    }

    SECTION ("accepts empty streams") {
      Token tokens[] = { t_end };
      auto s = p.parse (tokens);
      REQUIRE (s.end->kind == TK::end);
    }

    SECTION ("rejects non-decls") {
      Token tokens[] = { id("<not-a-decl>"), t_end };
      REQUIRE_THROWS (p.parse (tokens));
    }

  }

}
