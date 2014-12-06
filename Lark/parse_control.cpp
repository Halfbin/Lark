
#include "parse_control.hpp"

namespace Lark {
  ParseError::~ParseError () throw ()
  { }

  Yield Parser::parse () const {
    return resume_rule (resume_node, resume_cursor, child_node);
  }

  Parser Parser::begin (Rule rule, Cursor cursor) {
    return { rule, cursor, nullptr, nullptr };
  }

  bool Yield::returning () const {
    return !child_rule;
  }

  Match Yield::match () const {
    return { resume_node, cursor };
  }

  Parser Yield::child () const {
    return Parser::begin (child_rule, cursor);
  }

  Parser Yield::resumption (Match child) const {
    return Parser (resume_rule, child.end_cursor, resume_node, child.node);
  }

  Yield Yield::to (Rule child, Cursor cursor, Rule resume, Node* node) {
    return { child, cursor, resume, node };
  }

  Yield Yield::returning (Cursor cursor, Node* node) {
    return { nullptr, cursor, nullptr, node };
  }

  Match parse_recursive (Parser parser) {
    for (;;) {
      Yield yield = parser.parse ();
      if (yield.returning ())
        return yield.match ();
      Match child_match = parse_recursive (yield.child ());
      parser = yield.resumption (child_match);
    }
  }

}
