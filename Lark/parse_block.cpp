
#include "Block.hpp"

namespace Lark {
  // block = `do` statement* `end`
  Yield parse_block (Node* node, Cursor cur, Node* child) {
    return Yield::returning (cur, nullptr);
  }

}
