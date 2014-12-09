
#include "parse.hpp"

namespace Lark {
  namespace {
    enum class Precedence : int {
      minimum = 0,
      relation,
      disjunction,
      conjunction,
      sum,
      product,
      prefix,
      postfix
    };

    Precedence operator + (Precedence prec, int rhs) {
      return Precedence (int (prec) + rhs);
    }

    auto parse_primary (Cursor cursor) -> Match <Primary*> {

    };

    bool is_relation (Rk::cstring_ref op) {
      return op == "<"
          || op == "<="
          || op == "=="
          || op == "!="
          || op == ">"
          || op == ">=";
    }

    bool is_disjunction (Rk::cstring_ref op) {
      return op == "or";
    }

    bool is_conjunction (Rk::cstring_ref op) {
      return op == "and";
    }

    bool is_sum (Rk::cstring_ref op) {
      return op == "+"
          || op == "-";
    }

    bool is_product (Rk::cstring_ref op) {
      return op == "*"
          || op == "/"
          || op == "%";
    }

    bool is_infix_op (Token token) {
      return is_relation    (token.spelling)
          || is_disjunction (token.spelling)
          || is_conjunction (token.spelling)
          || is_sum         (token.spelling)
          || is_product     (token.spelling);
    }

    auto parse_infix_op (Cursor cursor) -> Match <Rk::cstring_ref> {
      return expect (cursor, is_infix_op);
    }

    Precedence infix_precedence (Rk::cstring_ref op) {
      if      (is_relation    (op)) return Precedence::relation;
      else if (is_disjunction (op)) return Precedence::disjunction;
      else if (is_conjunction (op)) return Precedence::conjunction;
      else if (is_sum         (op)) return Precedence::sum;
      else if (is_product     (op)) return Precedence::product;
      else throw std::logic_error ("not an infix operator");
    }

    auto parse_infix (const Cursor cursor, Precedence min_precedence)
      -> Match <Infix*>
    {
      auto expression = parse_primary (cursor);
      if (!expression) return cursor;

      for (;;) {
        auto op = parse_infix_op (expression.end);
        if (!op) break;

        auto precedence = infix_precedence (op.result);
        if (precedence < min_precedence) break; // associate at higher level

        auto rhs = parse_infix (op.end, precedence + 1);
        if (!rhs) return cursor;

        auto node = new Infix { expression.result, op.result, rhs.result };
        expression = { rhs.end, node };
      }

      return expression;
    }

  }

  auto parse_expression (Cursor cursor) -> Match <Expression*> {
    return parse_infix (cursor, Precedence::minimum);
  }

}
