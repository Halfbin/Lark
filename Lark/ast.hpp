
#pragma once

#include "types.hpp"

#include <memory>
#include <vector>

namespace Lark {
  struct BlockNode {

  };

  using Block = std::unique_ptr <BlockNode>;

  enum class DeclType {
    function
  };

  struct DeclBase {
    const DeclType ty;
    DeclBase (DeclType ty) : ty (ty) { }
  };

  using Decl = std::unique_ptr <DeclBase>;

  using FormalArg = StrRef;

  using FormalArgList = std::vector <FormalArg>;

  struct FunctionNode : DeclBase {
    FormalArgList formal_args;
    Block         body;

    FunctionNode (FormalArgList fargs, Block body) :
      DeclBase (DeclType::function),
      formal_args (std::move (fargs)),
      body (std::move (body))
    { }

    FunctionNode () : DeclBase (DeclType::function) { }

  };

  using Function = std::unique_ptr <FunctionNode>;

  using Stream = std::vector <Decl>;

}
