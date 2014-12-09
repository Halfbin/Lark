
#include "parse.hpp"

namespace Lark {
  auto parse_assignment_target (Cursor cursor) -> Match <Assignment::Target> {
    return parse_identifier (cursor);
  }

  bool is_assignment_op (Token token) {
    return token.spelling == "=";
  }

  auto parse_assignment_op (Cursor cursor) -> Match <Assignment::Op> {
    if (!is_assignment_op (*cursor)) return cursor;
    else return { cursor.next (), cursor->spelling };
  }

  auto parse_assignment_rhs (Cursor cursor) -> Match <Assignment::RHS> {
    return parse_expression (cursor);
  }

  auto parse_assign (Cursor cursor) -> Match <Assignment*> {
    auto target = parse_assignment_target (cursor);
    if (!target) return cursor;

    auto op = parse_assignment_op (target.end.skip_nl ());
    if (!op) return cursor;

    auto rhs = parse_assignment_rhs (op.end.skip_nl ());
    if (!rhs)
      throw ParseError ("expected expression on right-side of assignment");

    auto assignment = new Assignment (target.result, op.result, rhs.result);
    return { rhs.end, assignment };
  }

}
