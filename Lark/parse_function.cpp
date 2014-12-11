
#include "parse.hpp"

#include <catch.hpp>

namespace Lark {
  auto FunctionParser::parse (Cursor cursor) const -> Match <Function> {
    auto key = expect (cursor, "func");
    if (!key) return cursor;

    auto name = expect (key.end, TokenKind::identifier);
    if (!name) throw ParseError ("expected name of function");

    auto fargs = fargs_parser.parse (name.end);

    auto body = block_parser.parse (fargs.end);
    if (!body) throw ParseError ("expected body of function");

    auto node = std::make_unique <FunctionNode> (
      std::move (fargs.result), std::move (body.result)
    );
    return { body.end, std::move (node) };
  }

  using namespace TokenHelpers;

  TEST_CASE ("FunctionParser") {
    StubParser <FormalArgList> fargs_stub (
      "<fargs>", [] () -> FormalArgList { return { "x", "y", "z" }; }
    );
    StubParser <Block> block_stub (
      "<block>", [] () -> Block { return std::make_unique <BlockNode> (); }
    );
    FunctionParser p { fargs_stub, block_stub };

    SECTION ("parses functions") {
      Token tokens [] = {
        kw("func"), id("foo"), id("<fargs>"), id("<block>"), t_end
      };
      auto m = p.parse (tokens);
      REQUIRE (m);
      REQUIRE (m.result->ty == DeclType::function);
      REQUIRE (m.result->formal_args.size () == 3);
      REQUIRE (m.result->body);
      REQUIRE (m.end->kind == TK::end);
    }

    SECTION ("rejects non-functions") {
      Token tokens [] = { id("funk"), t_end };
      auto m = p.parse (tokens);
      REQUIRE (!m);
    }

    SECTION ("fails for unnamed functions") {
      Token tokens [] = { kw("func"), t_end };
      REQUIRE_THROWS (p.parse (tokens));
    }

    SECTION ("fails for functions with no body") {
      Token tokens [] = { kw("func"), id("foo"), id("<fargs>"), t_end };
      REQUIRE_THROWS (p.parse (tokens));
    }

    SECTION ("allows omission of formal arg list") {
      Token tokens [] = { kw("func"), id("foo"), id("<block>"), t_end };
      auto m = p.parse (tokens);
      REQUIRE (m);
      REQUIRE (m.result->ty == DeclType::function);
      REQUIRE (m.result->formal_args.empty ());
      REQUIRE (m.result->body);
      REQUIRE (m.end->kind == TK::end);
    }
  }
}
