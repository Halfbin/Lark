
#pragma once

#include "types.hpp"

#include <memory>
#include <vector>

namespace Lark {
  enum class StmtType {
    test,
    block
  };

  struct StmtBase {
    const StmtType ty;
    StmtBase (StmtType ty) : ty (ty) { }
  };

  using Stmt = std::unique_ptr <StmtBase>;
  using StmtSeq = std::vector <Stmt>;

  struct BlockNode : StmtBase {
    StmtSeq stmts;

    explicit BlockNode (StmtSeq stmts) :
      StmtBase (StmtType::block),
      stmts    (std::move (stmts))
    { }

    BlockNode (BlockNode&& other) :
      StmtBase (StmtType::block),
      stmts    (std::move (other.stmts))
    { }

    BlockNode () :
      StmtBase (StmtType::block)
    { }

    BlockNode (const BlockNode&) = delete;

    BlockNode& operator = (const BlockNode&) = delete;

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
