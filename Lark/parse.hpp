
#pragma once

#include "parse_control.hpp"

namespace Lark {
  Yield parse_block    (Node*, Cursor, Node*);
  Yield parse_function (Node*, Cursor, Node*);
  Yield parse_stream   (Node*, Cursor, Node*);

  Match parse (Cursor);

}
