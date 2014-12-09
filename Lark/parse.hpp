
#pragma once

#include <stdexcept>

#include "Cursor.hpp"
#include "Match.hpp"
#include "AST.hpp"

namespace Lark {
  struct ParseError : public std::runtime_error {
    ParseError (const char* msg) : std::runtime_error (msg) { }
    virtual ~ParseError () throw ();
  };

  static auto expect (Cursor cursor, Rk::cstring_ref spelling)
    -> Match <Rk::cstring_ref>
  {
    if (cursor->spelling != spelling) return cursor;
    else return { cursor.next (), cursor->spelling };
  }

  static auto expect (Cursor cursor, TokenKind kind)
    -> Match <Rk::cstring_ref>
  {
    if (cursor->kind != kind) return cursor;
    else return { cursor.next (), cursor->spelling };
  }

  static auto expect (Cursor cursor, TokenKind kind, Rk::cstring_ref spelling)
    -> Match <Rk::cstring_ref>
  {
    if (cursor->kind != kind || cursor->spelling != spelling) return cursor;
    else return { cursor.next (), cursor->spelling };
  }

  template <typename Parser>
  auto parse_first_of (Cursor cursor, Parser parser)
  {
    return parser (cursor);
  }

  template <typename Parser, typename... Parsers>
  auto parse_first_of (Cursor cursor, Parser parser, Parsers... rest)
  {
    auto match = parser (cursor);
    if (!match) return parse_first_of (cursor, rest...);
    else return match;
  }

  auto parse_identifier (Cursor) -> Match <Rk::cstring_ref>;
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
