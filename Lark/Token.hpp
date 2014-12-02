
#pragma once

#include <Rk/string_ref.hpp>

namespace Lark
{
  enum class TokenKind
  {
    garbage,
    end,
    space,
    newline,
    comment,
    keyword,
    identifier,
    integer,
    real,
    string,
    punct
  };

  struct Token
  {
    Rk::cstring_ref spelling;
    TokenKind       kind;

    Token () = default;

    Token (Rk::cstring_ref spelling, TokenKind kind) :
      spelling (spelling),
      kind     (kind)
    { }

  };

  inline bool operator == (Token a, Token b)
  {
    return a.kind == b.kind && a.spelling == b.spelling;
  }

}
