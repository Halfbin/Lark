
#include "GrabToken.hpp"

#include <catch.hpp>

#include <algorithm>

namespace Lark
{
  template <typename Pred>
  Token grab_while (Rk::cstring_ref in, Pred pred, TokenKind kind)
  {
    auto end = std::find_if_not (in.begin (), in.end (), pred);
    return Token (Rk::cstring_ref (in.begin (), end), kind);
  }

  template <typename Pred>
  Token grab_until (Rk::cstring_ref in, Pred pred, TokenKind kind)
  {
    auto end = std::find_if (in.begin (), in.end (), pred);
    return Token (Rk::cstring_ref (in.begin (), end), kind);
  }

  Token grab_token (Rk::cstring_ref in)
  {
    if (in.empty ())
      return Token { in, TokenKind::end };

    switch (in [0])
    {
      case ' ':
        return grab_while (in, [] (char c) {return c==' ';}, TokenKind::space);

      case '\r':
        if (in.size () >= 2 && in [1] == '\n')
          return Token (in.slice (0, 2), TokenKind::newline);
      case '\n':
        return Token (in.slice (0, 1), TokenKind::newline);

      case '#':
        return grab_until (in, [] (char c) {return c=='\r'||c=='\n';}, TokenKind::comment);
    }

    return Token { "", TokenKind::garbage };
  }

  TEST_CASE ("grab_token")
  {
    SECTION ("recognizes whitespace") {
      REQUIRE (grab_token ("   x") == Token ("   ", TokenKind::space));
    }

    SECTION ("recognizes newlines") {
      REQUIRE (grab_token ("\n x")   == Token ("\n",   TokenKind::newline));
      REQUIRE (grab_token ("\r x")   == Token ("\r",   TokenKind::newline));
      REQUIRE (grab_token ("\r\n x") == Token ("\r\n", TokenKind::newline));
    }

    SECTION ("recognizes eof") {
      REQUIRE (grab_token ("") == Token ("", TokenKind::end));
    }

    SECTION ("recognizes line comments") {
      REQUIRE (
        grab_token ("# bitter swearing\r\nnext line")
          == Token ("# bitter swearing", TokenKind::comment)
      );
    }
  }

}
