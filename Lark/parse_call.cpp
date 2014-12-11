
#include "parse.hpp"

namespace Lark {
  namespace {
    auto parse_target (Cursor cursor) -> Match <Call::Target> {
      return expect (is_identifier, cursor);
    }

    auto parse_argument (Cursor cursor) -> Match <Call::Argument> {
      return parse_expression (cursor);
    }

    auto parse_arguments (Cursor cursor) -> Match <Call::Arguments> {
      auto argument = parse_argument (cursor);
      if (!argument) return cursor;

      Call::Arguments arguments { argument.result };

      for (;;) {
        auto separator = expect (argument.end, ",");
        if (!separator) break;

        auto argument = parse_argument (separator.end);
        if (!argument) throw ParseError ("expected argument");
        arguments.push_back (argument.result);
      }

      return { argument.end, std::move (arguments) };
    }

  }

  auto parse_call (Cursor cursor) -> Match <Call*> {
    auto target = parse_target (cursor);
    if (!target) return cursor;

    auto open = expect (target.end, "(");
    if (!open) return cursor;

    auto args = parse_arguments (open.end);

    auto close = expect (args.end, ")");
    if (!close) throw ParseError ("expected `)`");

    auto call = new Call { target.result, std::move (args.result) };
    return { close.end, call };
  }

}
