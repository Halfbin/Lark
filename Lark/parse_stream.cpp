
#include "Stream.hpp"
#include "Function.hpp"

#include <catch.hpp>

namespace Lark {
  // stream = function*
  Yield parse_stream (Node* node, Cursor cur, Node* child) {
    auto stream = (Stream*) node;
    if (!stream) stream = new Stream;
    if (child) stream->add ((Function*) child);

    while (*cur) {
      switch (cur->kind) {
        case TokenKind::space:
        case TokenKind::newline:
        case TokenKind::comment:
          cur++;
          continue;

        case TokenKind::keyword:
          if (cur->spelling == "func")
            return Yield::to (parse_function, cur + 1, parse_stream, stream);
      }
    }

    return Yield::returning (cur, stream);
  }

  using TK = TokenKind;

  TEST_CASE ("parse_stream") {
    SECTION ("skips trash and recognizes `func`") {
      const Token tokens [] = {
        {" ",TK::space}, {"\n",TK::newline}, {"#c",TK::comment},
        {"func",TK::keyword}, {"TEST",TK::identifier}, {"",TK::end}
      };
      auto yield = parse_stream (nullptr, tokens, nullptr);
      REQUIRE (yield.is_to (parse_function, tokens+4, parse_stream));
    }

    SECTION ("returns at end-of-file") {
      const Token end = {"",TK::end};
      auto yield = parse_stream (nullptr, &end, nullptr);
      REQUIRE (yield.is_returning (&end));
    }

  }

}
