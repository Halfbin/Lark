
#pragma once

#include "Token.hpp"

namespace Lark {
  class Cursor {
    const Token* ptr;

    static const Token* skip_nl (const Token* p) {
      while (p->kind == TokenKind::newline) p++;
      return p;
    }

  public:
    Cursor (const Token* ptr) : ptr (ptr) { }
    Cursor (const Cursor&) = default;

    Cursor& operator = (const Cursor& other) {
      ptr = other.ptr;
      return *this;
    }

    Cursor next () const {
      return ptr + 1;
    }

    Cursor skip_nl () const {
      return skip_nl (ptr);
    }

    Token operator* () { return *ptr; }
    const Token* operator-> () { return ptr; }

  };

}
