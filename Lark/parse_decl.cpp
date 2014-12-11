
#include "parse.hpp"

#include <catch.hpp>

namespace Lark {
  auto DeclParser::parse (Cursor cursor) const -> Match <Decl> {
    return func_parser.parse (cursor);
  }

  using namespace TokenHelpers;

  TEST_CASE ("DeclParser") {
    StubParser <Function> fp ("<func>", [] { return std::make_unique <FunctionNode> (); });
    DeclParser p { fp };

    SECTION ("parses functions") {
      Token tokens[] = { id("<func>"), t_end };
      auto s = p.parse (tokens);
      REQUIRE (s);
      REQUIRE (s.result->ty == DeclType::function);
      REQUIRE (s.end->kind == TK::end);
    }

    SECTION ("rejects non-functions") {
      Token tokens[] = { id("<non-func>"), t_end };
      auto s = p.parse (tokens);
      REQUIRE (!s);
    }

  }
}
