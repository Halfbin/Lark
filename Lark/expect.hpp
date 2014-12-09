
#pragma once

#include "match.hpp"

namespace Lark {
  template <typename Pred>
  static auto expect (Cursor cursor, Pred pred)
    -> Match <Rk::cstring_ref>
  {
    if (!pred (*cursor)) return cursor;
    else return { cursor.next (), cursor->spelling };
  }

  static auto is_kind (TokenKind kind) {
    return [kind] (Token t) { return t.kind == kind; };
  }

  static auto is_identifier = is_kind (TokenKind::identifier);

  static auto is_spelled (Rk::cstring_ref spelling) {
    return [spelling] (Token t) { return t.spelling == spelling; };
  }

}
