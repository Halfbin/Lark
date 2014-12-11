
#pragma once

#include <memory>
#include <vector>

namespace Lark {
  struct DeclBase { };

  using Decl = std::unique_ptr <DeclBase>;

  using Stream = std::vector <Decl>;

  enum class SyntaxRule {
    decl
  };

}
