
#pragma once

#include <Rk/string_ref.hpp>

#include <vector>

namespace Lark {
  struct Statement { };

  using Statements = std::vector <Statement*>;

  struct Expression : Statement { };

  struct Return : Statement {
    using Value = Expression*;
    Value value;
    Return (Value value) : value (value) { }
  };

  struct Assignment : Statement {
    using Target = Rk::cstring_ref;
    using Op     = Rk::cstring_ref;
    using RHS    = Expression*;

    Target target;
    Op     op;
    RHS    rhs;

    Assignment (Target target, Op op, RHS rhs) :
      target (target), op (op), rhs (rhs)
    { }

  };

  struct If : Statement {
    using Condition = Expression*;
    using Body      = Block*;

    Condition condition;
    Body      body;

    If (Condition condition, Body body) :
      condition (condition), body (body)
    { }

  };

  struct While : Statement {
    using Condition = Expression*;
    using Body      = Block*;

    Condition condition;
    Body      body;

    If (Condition condition, Body body) :
      condition (condition), body (body)
    { }

  };

  struct Loop : Statement {
    using Body = Block*;
    Body body;
    Loop (Body body) : body (body) { }
  };

  struct For : Statement {
    using Assignment = Assignment*;
    using Body       = Block*;
    
    Assignment assignment;
    
  };

  struct Block : Statement {
    Statements statements;
    Block (Statements statements) : statements (statements) { }
  };

  struct Function {
    using Name = Rk::cstring_ref;
    using Argument = Rk::cstring_ref;
    using Arguments = std::vector <Argument>;
    using Body = Block*;

    Name      name;
    Arguments arguments;
    Body      body;

    Function (Name name, Arguments arguments, Body body) :
      name (name), arguments (arguments), body (body)
    { }

  };

  struct Stream {
    std::vector <Function*> functions;
  };

}
