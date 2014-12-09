
#include "parse.hpp"

namespace Lark {
  ParseError::~ParseError () throw ()
  { }

  auto parse_identifier (Cursor cursor) -> Match <Rk::cstring_ref> {
    if (cursor->kind == TokenKind::identifier)
      return { cursor.next (), cursor->spelling };
    else return cursor;
  }

}
