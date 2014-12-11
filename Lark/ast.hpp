
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

  using FuncArg = StrRef;

  using FuncArgList = std::vector <FuncArg>;

  struct FunctionNode : DeclBase {
    FuncArgList func_args;
    Block       body;

    FunctionNode (FuncArgList fargs, Block body) :
      DeclBase (DeclType::function),
      func_args (std::move (fargs)),
      body (std::move (body))
    { }

    FunctionNode () : DeclBase (DeclType::function) { }

  };

  using Function = std::unique_ptr <FunctionNode>;

  using Stream = std::vector <Decl>;

}
