
#pragma once

#include "match.hpp"

namespace Lark {
  template <typename Cond>
  static auto expect (Cond pred, Cursor cursor)
    -> Match <StrRef>
  {
    if (!pred (*cursor)) return cursor;
    else return { cursor.next (), cursor->spelling };
  }

  static auto expect (Cursor cursor, StrRef spelling) -> Match <StrRef> {
    return expect ([=] (Token t) { return t.spelling == spelling; }, cursor);
  }

  static auto expect (Cursor cursor, TokenKind kind) -> Match <StrRef> {
    return expect ([=] (Token t) { return t.kind == kind; }, cursor);
  }

}
