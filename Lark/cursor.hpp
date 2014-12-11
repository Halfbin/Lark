
#pragma once

#include "token.hpp"

namespace Lark {
  class Cursor {
    const Token* ptr;

  public:
    Cursor (const Token* ptr) : ptr (ptr) { }
    Cursor (const Cursor&) = default;

    Cursor next () const { return ptr + 1; }

    Token        operator * () const { return *ptr; }
    const Token* operator-> () const { return  ptr; }

  };

}
