
#include "parse.hpp"

#include <catch.hpp>

namespace Lark {
  using namespace TokenHelpers;

  TEST_CASE ("SequenceParser") {
    StubParser <int> elem_stub ("<elem>", [] () -> int { return 9; });
    SequenceParser <int> p (",", elem_stub);

    SECTION ("parses sequences") {
      Token tokens [] = {
        id("<elem>"), pn(","), id("<elem>"), pn(","), id("<elem>"), t_end
      };
      auto m = p.parse (tokens);
      REQUIRE (m);
      REQUIRE (m.result.size () == 3);
      REQUIRE (m.end->kind == TK::end);
    }

    SECTION ("rejects sequences of other things") {
      Token tokens [] = { id("<other>"), pn(","), id("<other>"), t_end };
      auto m = p.parse (tokens);
      REQUIRE(!m);
    }

    SECTION ("fails when there's a trailing separator") {
      Token tokens [] = { id("<elem>"), pn (","), id("<other>"), t_end };
      REQUIRE_THROWS (p.parse (tokens));
    }
  }

}
