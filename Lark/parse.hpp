
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

  template <typename Elem>
  struct SequenceParser : Parser <std::vector <Elem>> {
    StrRef         separator;
    Parser <Elem>& elem_parser;

    SequenceParser (StrRef separator, Parser <Elem>& elem) :
      separator   (separator),
      elem_parser (elem)
    { }

    auto parse (Cursor cursor) const -> Match <std::vector <Elem>> {
      auto elem = elem_parser.parse (cursor);
      if (!elem) return cursor;

      std::vector <Elem> seq { std::move (elem.result) };

      for (;;) {
        auto sep = expect (elem.end, separator);
        if (!sep) break;

        elem = elem_parser.parse (sep.end);
        if (!elem) throw ParseError ("expected element after separator");
        else seq.push_back (std::move (elem.result));
      }

      return { elem.end, std::move (seq) };
    }

  };

  template <typename Inner>
  struct BracketParser : Parser <Inner> {
    StrRef          open_delim, close_delim;
    Parser <Inner>& inner_parser;

    BracketParser (StrRef open, StrRef close, Parser <Inner>& inner) :
      open_delim   (open),
      close_delim  (close),
      inner_parser (inner)
    { }

    auto parse (Cursor cursor) const -> Match <Inner> {
      auto open = expect (cursor, open_delim);
      if (!open) return cursor;

      auto inner = inner_parser.parse (open.end);

      auto close = expect (inner.end, close_delim);
      if (!close)
        throw ParseError ("expected closing delimiter at end of bracket");

      return { close.end, std::move (inner.result) };
    }

  };

  template <typename Result>
  struct StubParser : Parser <Result> {
    using Factory = Result (*) ();

    StrRef  spelling;
    Factory factory;

    StubParser (StrRef spelling, Factory factory) :
      spelling (spelling),
      factory  (factory)
    { }

    auto parse (Cursor cursor) const -> Match <Result> {
      if (cursor->spelling != spelling) return cursor;
      else return Match <Result> { cursor.next (), factory () };
    }

  };

  struct StreamParser : Parser <Stream> {
    Parser <Decl>& decl_parser;
    StreamParser (Parser <Decl>& decl) : decl_parser (decl) { }
    auto parse (Cursor) const -> Match <Stream>;
  };

  struct DeclParser : Parser <Decl> {
    Parser <Function>& func_parser;
    DeclParser (Parser <Function>& func) : func_parser (func) { }
    auto parse (Cursor) const -> Match <Decl>;
  };

  struct FunctionParser : Parser <Function> {
    Parser <FuncArgList>& fargs_parser;
    Parser <Block>&       block_parser;
    FunctionParser (Parser <FuncArgList>& fargs, Parser <Block>& block) :
      fargs_parser (fargs), block_parser (block)
    { }
    auto parse (Cursor) const -> Match <Function>;
  };

  struct BlockParser : Parser <Block> {
    Parser <StmtSeq>& stmtseq_parser;
    BlockParser (Parser <StmtSeq>& stmtseq) : stmtseq_parser (stmtseq) { }
    auto parse (Cursor) const -> Match <Block>;
  };

  class IfClauseParser : public Parser <IfClause> {
  public:
    Parser <Expr>&  expr_parser;
    Parser <Block>& block_parser;
    IfClauseParser (Parser <Expr>& expr, Parser <Block>& block) :
      expr_parser (expr), block_parser (block)
    { }
    auto parse (Cursor) const -> Match <IfClause>;
  };

  class IfParser : public Parser <If> {
  public:
    Parser <IfClause>& clause_parser;
    Parser <Block>&    block_parser;
    IfParser (Parser <IfClause>& clause, Parser <Block>& block) :
      clause_parser (clause), block_parser (block)
    { }
    auto parse (Cursor) const -> Match <If>;
  };

  struct StmtParser : Parser <Stmt> {
    Parser <Stmt>& linestmt_parser;
    Parser <Stmt>& blockstmt_parser;
    StmtParser (Parser <Stmt>& linestmt, Parser <Stmt>& blockstmt) :
      linestmt_parser (linestmt), blockstmt_parser (blockstmt)
    { }
    auto parse (Cursor) const -> Match <Stmt>;
  };

}
