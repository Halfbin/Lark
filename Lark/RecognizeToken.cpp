
#include "RecognizeToken.hpp"

#include <catch.hpp>

#include <algorithm>

namespace Lark
{
  template <typename Pred>
  Token grab_while (Rk::cstring_ref in, Pred pred, TokenKind kind)
  {
    auto ws_end = std::find_if (in.begin (), in.end (), pred);
    return Token { Rk::cstring_ref (in.begin (), ws_end), kind };
  }

  Token recognize_token (Rk::cstring_ref in)
  {
    if (in.empty ())
      return Token { in, TokenKind::end };

    switch (in [0])
    {
      case ' ':
        return grab_while (in, [] (char c) { return c != ' '; }, TokenKind::space);
    }

    return Token { "", TokenKind::garbage };
  }

  TEST_CASE ("recognize_token")
  {
    SECTION ("recognizes whitespace") {
      REQUIRE (recognize_token ("   words words") == (Token { "   ", TokenKind::space }));
    }

    SECTION ("recognizes eof") {
      REQUIRE (recognize_token ("") == (Token { "", TokenKind::end }));
    }
  }

}
