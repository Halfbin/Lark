
#pragma once

#include <stdexcept>

#include "Token.hpp"

namespace Lark {
  struct ParseError : public std::runtime_error {
    ParseError (const char* msg) : std::runtime_error (msg) { }
    virtual ~ParseError () throw ();
  };

  struct Node { };

  class  Parser;
  class  Yield;
  struct Match;

  typedef const Token* Cursor;
  typedef Yield (*Rule) (Node* node, Cursor cursor, Node* child);

  class Parser {
    Rule   resume_rule;
    Cursor resume_cursor;
    Node*  resume_node;
    Node*  child_node;

    friend class Yield;
    Parser (Rule rule, Cursor cursor, Node* node, Node* child) :
      resume_rule   (rule),
      resume_cursor (cursor),
      resume_node   (node),
      child_node    (child)
    { }

  public:
    Yield parse () const;

    static Parser begin (Rule rule, Cursor cursor);

  };

  class Yield {
    Rule   child_rule;
    Cursor cursor;
    Rule   resume_rule;
    Node*  resume_node;

    Yield (Rule rule, Cursor cursor, Rule resume, Node* node) :
      child_rule  (rule),
      cursor      (cursor),
      resume_rule (resume),
      resume_node (node)
    { }

  public:
    bool   returning () const;
    Match  match () const;
    Parser child () const;
    Parser resumption (Match child) const;

    static Yield to        (Rule child, Cursor cursor, Rule resume, Node* node);
    static Yield returning (Cursor end, Node* node);

    bool is_to (Rule child, Cursor cursor, Rule resume) const {
      return child_rule   == child
          && this->cursor == cursor
          && resume_rule  == resume;
    }

    bool is_returning (Cursor end) const {
      return returning () && cursor == end;
    }

  };

  struct Match {
    Node*  node;
    Cursor end_cursor;
  };

  Match parse_recursive (Parser p);

}
