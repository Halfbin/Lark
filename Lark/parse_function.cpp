
#include "Function.hpp"
#include "Block.hpp"

namespace Lark {
  // function = `func` identifier formal-args? block
  Yield parse_function (Node* node, Cursor cur, Node* child) {
    return Yield::returning (cur, nullptr);
  }

}
