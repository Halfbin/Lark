
#pragma once

#include <Rk/string_ref.hpp>

namespace Lark {
  enum class TokenKind {
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

  struct Token {
    Rk::cstring_ref spelling;
    TokenKind       kind;

    Token () :
      kind (TokenKind::end)
    { }

    Token (Rk::cstring_ref spelling, TokenKind kind) :
      spelling (spelling),
      kind     (kind)
    { }

    explicit operator bool () const {
      return kind != TokenKind::end;
    }

  };

  inline bool operator == (Token a, Token b) {
    return a.kind == b.kind && a.spelling == b.spelling;
  }

  inline bool operator != (Token a, Token b) {
    return !(a == b);
  }

  template <typename OStream>
  inline OStream& operator << (OStream& os, Token tok) {
    return os << tok.spelling;
  }

  inline bool is_literal (Token token) {
    return token.kind == TokenKind::integer
        || token.kind == TokenKind::real
        || token.kind == TokenKind::string;
  }

  inline bool is_identifier (Token token) {
    return token.kind == TokenKind::identifier;
  }

}
