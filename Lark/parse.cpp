
#include "parse_control.hpp"

#include <catch.hpp>

#include <vector>

namespace Lark
{
  struct Block : Node
  { };

  // block = `do` statement* `end`
  Yield parse_block (Node* node, Cursor cur, Node* child) {
    return Yield::returning (cur, nullptr);
  }

  class Function : Node
  { };

  // function = `func` identifier formal-args? block
  Yield parse_function (Node* node, Cursor cur, Node* child) {
    return Yield::returning (cur, nullptr);
  }

  class Stream : public Node {
    std::vector <Function*> funcs;
  public:
    void add (Function* func) { funcs.push_back (func); }

  };

  // stream = function*
  Yield parse_stream (Node* node, Cursor cur, Node* child) {
    auto stream = (Stream*) node;
    if (!stream) stream = new Stream;
    if (child) stream->add ((Function*) child);

    while (*cur) {
      switch (cur->kind) {
        case TokenKind::space:
        case TokenKind::newline:
        case TokenKind::comment:
          cur++;
          continue;

        case TokenKind::keyword:
          if (cur->spelling == "func")
            return Yield::to (parse_function, cur + 1, parse_stream, stream);
      }
    }

    return Yield::returning (cur, stream);
  }

  using TK = TokenKind;

  TEST_CASE ("parse_stream") {
    SECTION ("skips trash and recognizes `func`") {
      const Token tokens [] = {
        {" ",TK::space}, {"\n",TK::newline}, {"#c",TK::comment},
        {"func",TK::keyword}, {"TEST",TK::identifier}, {"",TK::end}
      };
      auto yield = parse_stream (nullptr, tokens, nullptr);
      REQUIRE (yield.is_to (parse_function, tokens+4, parse_stream));
    }

    SECTION ("returns at end-of-file") {
      const Token end = {"",TK::end};
      auto yield = parse_stream (nullptr, &end, nullptr);
      REQUIRE (yield.is_returning (&end));
    }

  }

  void parse (const Token* tokens) {
    parse_recursive (Parser::begin (parse_stream, tokens));
  }

}
