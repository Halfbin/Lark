
#include "parse.hpp"

namespace Lark {
  namespace {
    auto parse_lhs (Cursor cursor) -> Match <Assignment::LHS> {
      return expect (is_identifier, cursor);
    }

    bool is_op (Token token) {
      return token.spelling == "=";
    }

    auto parse_op (Cursor cursor) -> Match <Assignment::Op> {
      return expect (is_op, cursor);
    }

    auto parse_rhs (Cursor cursor) -> Match <Assignment::RHS> {
      auto rhs = parse_expression (cursor);
      if (!rhs)
        throw ParseError ("expected expression on right-side of assignment");
      return rhs;
    }

  }

  auto parse_assignment (Cursor cursor) -> Match <Assignment*> {
    auto lhs = parse_lhs (cursor);
    if (!lhs) return cursor;

    auto op = parse_op (lhs.end);
    if (!op) return cursor;

    auto rhs = parse_rhs (op.end);

    auto assignment = new Assignment (lhs.result, op.result, rhs.result);
    return { rhs.end, assignment };
  }

}
