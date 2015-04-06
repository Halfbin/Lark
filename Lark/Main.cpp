
#include "parse.hpp"
#include "lex.hpp"

#include <Rk/string_ref.hpp>
#include <Rk/file_stream.hpp>

#include <iostream>
#include <vector>

#define CATCH_CONFIG_RUNNER
#include <catch.hpp>

namespace Lark {
  void print_ast (std::ostream& s, const Stream& stream) {

  }

  auto filter_tokens (std::vector <Token> tokens) -> std::vector <Token> {
    std::vector <Token> out;
    
    for (auto token : tokens) {
      bool keep =  token.kind != TokenKind::comment
                && token.kind != TokenKind::newline
                && token.kind != TokenKind::space;
      if (keep) out.push_back (token);
    }

    return out;
  }

  template <typename P>
  auto read_file (const P& p) -> std::vector <char> {
    Rk::fio::in_stream file (p);
    std::vector <char> data ((size_t) file.size ());
    file.read (data.data (), data.size ());
    return data;
  }

  extern "C" int main (int arg_count, char** args) try {
    if (arg_count < 2)
      return 1;

    if (Rk::cstring_ref (args [1]) == "--test") {
      Catch::Session session;
      session.configData ().showSuccessfulTests = false;
      return session.run ();
    }

    auto source_data = read_file (args [1]);
    auto source = Rk::cstring_ref (source_data.data (), source_data.size ());
    auto tokens = filter_tokens (lex (source));

    auto ast = parse_stream (tokens.data ());
    print_ast (std::cout, ast.result);

    return 0;
  }
  catch (std::exception& e) {
    return 1;
  }
}
