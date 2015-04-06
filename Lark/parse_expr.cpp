
#include "parse.hpp"

namespace Lark {
  auto parse_paren (Cursor cursor) -> Match <Expr> {
    auto obracket = expect (cursor, "(");
    if (!obracket) return cursor;

    auto inner = parse_expr (obracket.end);
    if (!inner) throw ParseError ("empty parenthesis");

    auto cbracket = expect (inner.end, ")");
    if (!cbracket) throw ParseError ("unclosed parenthesis");

    return { cbracket.end, std::move (inner.result) };
  }

  auto parse_literal (Cursor cursor) -> Match <Expr> {
    if (cursor->kind != TokenKind::integer) return cursor;
    auto token = *cursor;
    auto node = std::make_unique <LiteralExprNode> (token);
    return { cursor.next (), std::move (node) };
  }

  auto parse_arg_list (Cursor cursor) -> Match <ArgList> {
    auto obracket = expect (cursor, "(");
    if (!obracket) return cursor;

    ArgList args;

    auto arg = parse_expr (obracket.end);
    if (arg) {
      args.push_back (std::move (arg.result));

      for (;;) {
        auto comma = expect (arg.end, ",");
        if (!comma) break;
        arg = parse_expr (comma.end);
        if (!arg) throw ParseError ("missing last argument in list");
        args.push_back (std::move (arg.result));
      }
    }

    auto cbracket = expect (arg.end, ")");
    if (!cbracket) throw ParseError ("unclosed argument list");

    return { cbracket.end, std::move (args) };
  }

  auto parse_function_call (Cursor cursor) -> Match <Expr> {
    auto name = expect (cursor, TokenKind::identifier);
    if (!name) return cursor;

    auto args = parse_arg_list (name.end);
    if (!args) return cursor;

    auto node = std::make_unique <CallNode> (name.result, std::move (args.result));
    return { args.end, std::move (node) };
  }

  auto parse_id_expr (Cursor cursor) -> Match <Expr> {
    auto match = expect (cursor, TokenKind::identifier);
    if (!match) return cursor;
    auto node = std::make_unique <SymExprNode> (match.result);
    return { match.end, std::move (node) };
  }

  auto parse_primary_expr (Cursor cursor) -> Match <Expr> {
    Match <Expr> match = parse_paren         (cursor); if (match) return match;
                 match = parse_literal       (cursor); if (match) return match;
                 match = parse_function_call (cursor); if (match) return match;
                 match = parse_id_expr       (cursor);
    return match;
  }

  enum InfixOpPrec {
    prec_eq,
    prec_add,
    prec_mul
  };

  struct InfixOpParse {
    StrRef op;
    int precedence;
  };

  auto parse_infix_op (Cursor cursor, int left_precedence) -> Match <InfixOpParse> {
    static const InfixOpParse infix_ops [] = {
      { "+",  prec_add },
      { "-",  prec_add },
      { "*",  prec_mul },
      { "/",  prec_mul },
      { "=",  prec_eq  },
      { "/=", prec_eq  },
      { "<",  prec_eq  },
      { ">",  prec_eq  },
      { "<=", prec_eq  },
      { ">=", prec_eq  }
    };

    for (auto& op : infix_ops) {
      auto match = expect (cursor, op.op);
      if (match && op.precedence >= left_precedence)
        return { match.end, op };
    }

    return cursor;
  }

  auto parse_infix_expr (Cursor cursor, int left_precedence) -> Match <Expr> {
    Expr expr = nullptr;

    auto lhs_match = parse_primary_expr (cursor);
    if (!lhs_match) return cursor;
    cursor = lhs_match.end;
    expr = std::move (lhs_match.result);

    for (;;) {
      auto op_match = parse_infix_op (cursor, left_precedence);
      if (!op_match) break;

      auto rhs_match = parse_infix_expr (cursor, op_match.result.precedence);
      if (!rhs_match) throw ParseError ("infix operator with no right-hand-side");
      cursor = rhs_match.end;

      expr = std::make_unique <InfixExprNode> (
        std::move (expr),
        std::move (rhs_match.result),
        std::move (op_match.result.op)
      );
    }

    return { cursor, std::move (expr) };
  }

  auto parse_expr (Cursor cursor) -> Match <Expr> {
    return parse_infix_expr (cursor, 0);
  }
}
