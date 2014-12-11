
#include "parse.hpp"

#include <catch.hpp>

namespace Lark {
  auto DeclParser::parse (Cursor cursor) const -> Match <Decl> {
    return func_parser.parse (cursor);
  }

  using namespace TokenHelpers;

  TEST_CASE ("DeclParser") {
    StubParser <Function> function_stub (
      "<func>", [] { return std::make_unique <FunctionNode> (); }
    );
    DeclParser p { function_stub };

    SECTION ("parses functions") {
      Token tokens[] = { id("<func>"), t_end };
      auto m = p.parse (tokens);
      REQUIRE (m);
      REQUIRE (m.result->ty == DeclType::function);
      REQUIRE (m.end->kind == TK::end);
    }

    SECTION ("rejects non-functions") {
      Token tokens[] = { id("<non-func>"), t_end };
      auto m = p.parse (tokens);
      REQUIRE (!m);
    }

  }
}
