
#pragma once

#include "match.hpp"

namespace Lark {
  template <typename Cond>
  static auto expect (Cond pred, Cursor cursor)
    -> Match <Rk::cstring_ref>
  {
    if (!pred (*cursor)) return cursor;
    else return { cursor.next (), cursor->spelling };
  }

  static auto expect (Cursor cursor, Rk::cstring_ref spelling)
    -> Match <Rk::cstring_ref>
  {
    return expect (
      [spelling] (Token t) { return t.spelling == spelling; },
      cursor
    );
  }

  static auto expect (Cursor cursor, TokenKind kind)
    -> Match <Rk::cstring_ref>
  {
    return expect (
      [kind] (Token t) { return t.kind == kind; },
      cursor
    );
  }

}
