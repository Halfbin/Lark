
#pragma once

#include <stdexcept>
#include <vector>

#include "cursor.hpp"
#include "expect.hpp"
#include "match.hpp"
#include "ast.hpp"

namespace Lark {
  template <typename Value>
  using Parser = Match <Value> (*) (Cursor);

  struct ParseError : public std::runtime_error {
    template <typename... Args>
    ParseError (Args... args) : std::runtime_error (args...) { }
    virtual ~ParseError () throw ();
  };

  template <typename Elem>
  auto parse_star (Cursor cursor, Parser <Elem> inner) -> Match <std::vector <Elem>> {
    std::vector <Elem> elems;
    for (;;) {
      auto match = inner (cursor);
      if (!match) break;
      elems.push_back (std::move (match.result));
      cursor = match.end;
    }
    return { cursor, std::move (elems) };
  }

  auto parse_expr     (Cursor) -> Match <Expr>;
  auto parse_if       (Cursor) -> Match <If>;
  auto parse_stmt     (Cursor) -> Match <Stmt>;
  auto parse_stmt_seq (Cursor) -> Match <StmtSeq>;
  auto parse_oblock   (Cursor) -> Match <Block>;
  auto parse_block    (Cursor) -> Match <Block>;
  auto parse_function (Cursor) -> Match <Function>;
  auto parse_decl     (Cursor) -> Match <Decl>;
  auto parse_stream   (Cursor) -> Match <Stream>;
}
