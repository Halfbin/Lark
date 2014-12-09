
#pragma once

#include <Rk/string_ref.hpp>

#include <vector>

namespace Lark {
  struct Statement { };

  using Statements = std::vector <Statement*>;

  struct Block : Statement {
    Statements statements;
    Block (Statements statements) : statements (statements) { }
  };

  struct Expression : Statement { };

  struct Infix : Expression {
    using Operand = Expression*;
    using Op      = Rk::cstring_ref;

    Operand lhs;
    Op      op;
    Operand rhs;

    Infix (Operand lhs, Op op, Operand rhs) :
      lhs (lhs), op (op), rhs (rhs)
    { }

  };

  struct Return : Statement {
    using Value = Expression*;
    Value value;
    Return (Value value) : value (value) { }
  };

  struct Assignment : Statement {
    using LHS = Rk::cstring_ref;
    using Op  = Rk::cstring_ref;
    using RHS = Expression*;

    LHS lhs;
    Op  op;
    RHS rhs;

    Assignment (LHS lhs, Op op, RHS rhs) :
      lhs (lhs), op (op), rhs (rhs)
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

    While (Condition condition, Body body) :
      condition (condition), body (body)
    { }

  };

  struct Loop : Statement {
    using Body = Block*;
    Body body;
    Loop (Body body) : body (body) { }
  };

  struct For : Statement {
    using Spec = Assignment*;
    using Body = Block*;
    
    Spec spec;
    Body body;

    For (Spec spec, Body body) : spec (spec), body (body) { }

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
