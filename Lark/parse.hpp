
#pragma once

#include <stdexcept>

#include "cursor.hpp"
#include "expect.hpp"
#include "match.hpp"
#include "ast.hpp"

namespace Lark {
  struct ParseError : public std::runtime_error {
    template <typename... Args>
    ParseError (Args... args) : std::runtime_error (args...) { }
    virtual ~ParseError () throw ();
  };

  template <typename Result>
  struct Parser {
    virtual auto parse (Cursor) const -> Match <Result> = 0;
  };

  template <typename Result>
  static auto parse_any_number (const Parser <Result>& parser, Cursor cursor)
    -> Match <std::vector <Result>>
  {
    std::vector <Result> results;
    for (;;) {
      auto match = parser.parse (cursor);
      if (!match) break;
      results.push_back (std::move (match.result));
      cursor = match.end;
    }
    return { cursor, std::move (results) };
  }

  template <typename Result>
  struct StubParser : Parser <Result> {
    StrRef spelling;
    StubParser (StrRef spelling) : spelling (spelling) { }
    auto parse (Cursor cursor) const -> Match <Result> {
      if (cursor->spelling != spelling) return cursor;
      else return Match <Result> { cursor.next (), Result () };
    }
  };

  struct StreamParser : Parser <Stream> {
    Parser <Decl>& decl_parser;
    StreamParser (Parser <Decl>& decl) : decl_parser (decl) { }
    auto parse (Cursor) const -> Match <Stream>;
  };

}
