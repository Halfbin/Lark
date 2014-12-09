
#include "parse.hpp"

namespace Lark {
  namespace {
    bool is_key (Token token) {
      return token.spelling == "func";
    }

    auto parse_key (Cursor cursor) -> Match <bool> {
      return expect (cursor, is_key);
    }

    auto parse_name (Cursor cursor) -> Match <Function::Name> {
      auto name = expect (cursor, is_identifier);
      if (!name) throw ParseError ("expected function name");
      else return name;
    }

    auto parse_argument (Cursor cursor) -> Match <Function::Argument> {
      return expect (cursor, is_identifier);
    }

    auto parse_argument_sequence (Cursor cursor) -> Match <Function::Arguments> {
      Function::Arguments arguments;

      auto first = parse_argument (cursor);
      if (!first) return cursor;
      arguments.push_back (first.result);
      cursor = first.end.skip_nl ();

      while (cursor->spelling == ",") {
        auto argument = parse_argument (cursor.next ().skip_nl ());
        if (!argument) throw ParseError ("expected formal argument");
        arguments.push_back (argument.result);
        cursor = argument.end.skip_nl ();
      }

      return { cursor, std::move (arguments) };
    }

    auto parse_arguments (Cursor cursor) -> Match <Function::Arguments> {
      auto open = expect (cursor, is_spelled ("("));
      if (!open) return cursor;

      auto arguments = parse_argument_sequence (open.end.skip_nl ());

      auto close = expect (arguments.end.skip_nl (), is_spelled (")"));
      if (!close) throw ParseError ("expected `)`");

      return { close.end.skip_nl (), std::move (arguments.result) };
    }

    auto parse_body (Cursor cursor) -> Match <Function::Body> {
      auto block = parse_block (cursor);
      if (!block) throw ParseError ("expected function body");
      else return block;
    }

  }

  // function = `func` identifier formal-argument-list? block
  auto parse_function (Cursor cursor) -> Match <Function*> {
    auto key  = parse_key (cursor);
    if (!key) return cursor;

    auto name = parse_name      (key.end.skip_nl ());
    auto args = parse_arguments (name.end.skip_nl ());
    auto body = parse_body      (args.end.skip_nl ());

    auto func = new Function (name.result, std::move (args.result), body.result);
    return { body.end, func };
  }

}
