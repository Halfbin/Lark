
#include "parse.hpp"

#include <catch.hpp>

namespace Lark {
  auto parse_func_arg (Cursor cursor) -> Match <FuncArg> {
    return expect (cursor, TokenKind::identifier);
  }

  auto parse_func_arg_list (Cursor cursor) -> Match <FuncArgList> {
    auto obracket = expect (cursor, "(");
    if (!obracket) return cursor;
    auto args = parse_star (obracket.end, parse_func_arg);
    auto cbracket = expect (args.end, ")");
    if (!cbracket) throw ParseError ("expected `)` after function argument list");
    return { cbracket.end, std::move (args.result) };
  }

  auto parse_function (Cursor cursor) -> Match <Function> {
    auto key = expect (cursor, "func");
    if (!key) return cursor;

    auto name = expect (key.end, TokenKind::identifier);
    if (!name) throw ParseError ("expected name of function");

    auto fargs = parse_func_arg_list (name.end);

    auto body = parse_block (fargs.end);
    if (!body) throw ParseError ("expected body of function");

    auto node = std::make_unique <FunctionNode> (
      std::move (fargs.result), std::move (body.result)
    );
    return { body.end, std::move (node) };
  }
}
