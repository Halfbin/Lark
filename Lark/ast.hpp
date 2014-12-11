
#pragma once

#include <memory>
#include <vector>

namespace Lark {
  enum class DeclType {
    function
  };

  struct DeclBase {
    const DeclType ty;
    DeclBase (DeclType ty) : ty (ty) { }
  };

  using Decl = std::unique_ptr <DeclBase>;

  struct FunctionNode : DeclBase {
    FunctionNode () : DeclBase (DeclType::function) { }
  };

  using Function = std::unique_ptr <FunctionNode>;

  using Stream = std::vector <Decl>;

}
