
#pragma once

#include <type_traits>
#include <stdexcept>

#include "cursor.hpp"
#include "expect.hpp"
#include "match.hpp"
#include "ast.hpp"

namespace Lark {
  struct ParseError : public std::runtime_error {
    ParseError (const char* msg) : std::runtime_error (msg) { }
    virtual ~ParseError () throw ();
  };

  template <typename Result, typename Parser>
  auto parse_first_of (Cursor cursor, Parser parser) -> Match <Result> {
    return parser (cursor);
  }

  template <typename Result, typename Parser, typename... Parsers>
  auto parse_first_of (Cursor cursor, Parser parser, Parsers... rest)
    -> Match <Result>
  {
    Match <Result> match = parser (cursor);
    if (!match) return parse_first_of <Result> (cursor, rest...);
    else return match;
  }

  auto parse_call       (Cursor) -> Match <Call*>;
  auto parse_primary    (Cursor) -> Match <Expression*>;
  auto parse_infix      (Cursor) -> Match <Infix*>;
  auto parse_affix      (Cursor) -> Match <Affix*>;
  auto parse_expression (Cursor) -> Match <Expression*>;
  auto parse_assignment (Cursor) -> Match <Assignment*>;
  auto parse_return     (Cursor) -> Match <Return*>;
  auto parse_if         (Cursor) -> Match <If*>;
  auto parse_while      (Cursor) -> Match <While*>;
  auto parse_loop       (Cursor) -> Match <Loop*>;
  auto parse_for        (Cursor) -> Match <For*>;
  auto parse_statement  (Cursor) -> Match <Statement*>;
  auto parse_block      (Cursor) -> Match <Block*>;
  auto parse_function   (Cursor) -> Match <Function*>;
  auto parse_stream     (Cursor) -> Match <Stream*>;

}
