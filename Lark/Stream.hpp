
#pragma once

#include "function.hpp"

#include <vector>

namespace Lark {
  class Stream : public Node {
    std::vector <Function*> funcs;
  public:
    void add (Function* func) { funcs.push_back (func); }

  };

}
