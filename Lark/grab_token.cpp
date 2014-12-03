
#include "grab_token.hpp"

#include <catch.hpp>

#include <algorithm>

namespace Lark
{
  template <typename Range, typename Pred>
  Range grab_while (const Range& in, Pred pred)
  {
    return { in.begin (), std::find_if_not (in.begin (), in.end (), pred) };
  }

  template <typename Range, typename Pred>
  Range grab_until (const Range& in, Pred pred)
  {
    return { in.begin (), std::find_if (in.begin (), in.end (), pred) };
  }

  bool is_space (char c)
  {
    return c == ' ';
  }

  bool is_comment (char c)
  {
    return c == '#';
  }

  bool is_newline (char c)
  {
    return (c == '\r') || (c == '\n');
  }

  Token handle_space (Rk::cstring_ref in)
  {
    return { grab_while (in, is_space), TokenKind::space }; 
  }

  Token handle_newline (Rk::cstring_ref in)
  {
    if (in.size () >= 2 && in.slice (0, 2) == "\r\n")
      return { in.slice (0, 2), TokenKind::newline };
    else
      return { in.slice (0, 1), TokenKind::newline };
  }

  Token handle_comment (Rk::cstring_ref in)
  {
    return { grab_until (in, is_newline), TokenKind::comment }; 
  }

  bool is_keyword (Rk::cstring_ref in)
  {
    std::initializer_list <Rk::cstring_ref> kws = {
      "and", "or", "not", "if", "else", "func"
    };

    return std::find (kws.begin (), kws.end (), in) != kws.end ();
  }

  Token handle_word (Rk::cstring_ref in)
  {
    auto spelling = grab_identifier (in);
    if (is_keyword (spelling))
      return { spelling, TokenKind::keyword };
    else
      return { spelling, TokenKind::identifier };
  }

  Token handle_number (Rk::cstring_ref in)
  {
    return { grab_while (in, is_digit), TokenKind::integer };
  }

  Token handle_string (Rk::cstring_ref in)
  {
    return { grab_string (in), TokenKind::string };
  }

  bool is_punct (char c)
  {
    static const Rk::cstring_ref puncts = "`$^&*()-=+[]{};:@~,./<>-\\|";
    return std::find (puncts.begin (), puncts.end (), c) != puncts.end ();
  }

  Token handle_punct (Rk::cstring_ref in)
  {
    if (in.size () < 2)
      return { in, TokenKind::punct };

    std::initializer_list <Rk::cstring_ref> twos = {
      "++", "--", "==", "::", "<<", ">>", "<=", ">=", "->", "=>"
    };

    if (std::find (twos.begin (), twos.end (), in.slice (0, 2)) != twos.end ())
      return { in.slice (0, 2), TokenKind::punct };
    else
      return { in.slice (0, 1), TokenKind::punct };
  }

  Token grab_token (Rk::cstring_ref in)
  {
    if      (in.empty ())          return { in, TokenKind::end };
    else if (is_space    (in [0])) return handle_space   (in);
    else if (is_newline  (in [0])) return handle_newline (in);
    else if (is_comment  (in [0])) return handle_comment (in);
    else if (is_id_start (in [0])) return handle_word    (in);
    else if (is_digit    (in [0])) return handle_number  (in);
    else if (is_quote    (in [0])) return handle_string  (in);
    else if (is_punct    (in [0])) return handle_punct   (in);
    else                           return { in.slice (0, 1), TokenKind::garbage };
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

    SECTION ("recognizes integers") {
      REQUIRE (grab_token ("12345 x") == Token ("12345", TokenKind::integer));
    }

    SECTION ("recognizes punctuation") {
      REQUIRE (grab_token ("+> are two separate puncts")
                 == Token ("+", TokenKind::punct));
      REQUIRE (grab_token ("++ is a single punct")
                 == Token ("++", TokenKind::punct));
    }

    SECTION ("recognizes keywords") {
      REQUIRE (grab_token ("if keywords end with a space, we're ok")
                 == Token ("if", TokenKind::keyword));
    }
  }

}
