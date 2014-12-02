
#include "GrabToken.hpp"

#include <catch.hpp>

#include <algorithm>

namespace Lark
{
  template <typename Pred>
  Token grab_while (Rk::cstring_ref in, Pred pred, TokenKind kind)
  {
    auto end = std::find_if_not (in.begin (), in.end (), pred);
    return { {in.begin (), end}, kind };
  }

  template <typename Pred>
  Token grab_until (Rk::cstring_ref in, Pred pred, TokenKind kind)
  {
    auto end = std::find_if (in.begin (), in.end (), pred);
    return { {in.begin (), end}, kind };
  }

  bool is_space (char c)
  {
    return c == ' ';
  }

  bool is_line_comment_initial (char c)
  {
    return c == '#';
  }

  bool is_newline (char c)
  {
    return (c == '\r') || (c == '\n');
  }

  bool is_identifier_initial (char c)
  {
    return (c >= 'a' && c <= 'z')
        || (c >= 'A' && c <= 'Z')
        || (c == '_');
  }

  bool is_identifier_medial (char c)
  {
    return is_identifier_initial (c)
        || (c >= '0' && c <= '9');
  }

  bool is_identifier_suffix (char c)
  {
    return (c == '!') || (c == '?');
  }

  /* [a-zA-Z_][a-zA-Z_0-9]*[!\?]? */

  Token grab_identifier (Rk::cstring_ref in)
  {
    if (in.empty () || !is_identifier_initial (in [0]))
      throw std::logic_error ("oops");

    // Find the first character that's not medial
    auto end = std::find_if_not (in.begin () + 1, in.end (), is_identifier_medial);

    // Check if there's a ? or ! at the end
    if (end != in.end () && is_identifier_suffix (*end))
      end++;

    return { {in.begin (), end}, TokenKind::identifier };
  }

  Token grab_token (Rk::cstring_ref in)
  {
    if (in.empty ())
      return Token { in, TokenKind::end };

    char ch = in [0];

    if (is_space (ch))
    {
      return grab_while (in, is_space, TokenKind::space);
    }
    else if (is_newline (ch))
    {
      if (in.size () >= 2 && in.slice (0, 2) == "\r\n")
        return { in.slice (0, 2), TokenKind::newline };
      else
        return { in.slice (0, 1), TokenKind::newline };
    }
    else if (is_line_comment_initial (ch))
    {
      return grab_until (in, is_newline, TokenKind::comment);
    }
    else if (is_identifier_initial (ch))
    {
      auto token = grab_identifier (in);
      return token;
    }
    else
    {
      return { in.slice (0, 1), TokenKind::garbage };
    }
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
          == Token ("# bitter swearing", TokenKind::comment));
    }

    SECTION ("recognizes identifiers") {
      REQUIRE (
        grab_token ("identifiers end with spaces")
          == Token ("identifiers", TokenKind::identifier));
      REQUIRE (
        grab_token ("_identifiers can start with underscores")
          == Token ("_identifiers", TokenKind::identifier));
      REQUIRE (
        grab_token ("ident1234ifiers can have digits in them")
          == Token ("ident1234ifiers", TokenKind::identifier));
      REQUIRE (
        grab_token ("1dentifiers can't start with digits")
          .kind != TokenKind::identifier);
      REQUIRE (
        grab_token ("identifiers! can end with bangs (and queries)")
          == Token ("identifiers!", TokenKind::identifier));
      REQUIRE (
        grab_token ("but?! they can't have them before the end")
          == Token ("but?", TokenKind::identifier)
      );
    }
  }

}
