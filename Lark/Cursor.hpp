
#pragma once

#include "token.hpp"

namespace Lark {
  class Cursor {
    const Token* ptr;

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

    Token operator* () { return *ptr; }
    const Token* operator-> () { return ptr; }

  };

}
