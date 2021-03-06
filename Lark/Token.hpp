
#pragma once

#include "types.hpp"

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
    StrRef    spelling;
    TokenKind kind;

    Token () :
      kind (TokenKind::end)
    { }

    Token (StrRef spelling, TokenKind kind) :
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

  template <typename OStream>
  inline OStream& operator << (OStream& os, Token tok) {
    return os << tok.spelling;
  }

  namespace TokenHelpers {
    inline Token id (StrRef spelling) {
      return { spelling, TokenKind::identifier };
    }

    inline Token kw (StrRef spelling) {
      return { spelling, TokenKind::keyword };
    }

    inline Token pn (StrRef spelling) {
      return { spelling, TokenKind::punct };
    }

    static const Token t_end { "<end>", TokenKind::end };

    using TK = TokenKind;

  }

}
