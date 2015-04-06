
#pragma once

#include "grab_token.hpp"

#include <vector>

namespace Lark {
  auto lex (StrRef source) -> std::vector <Token> {
    std::vector <Token> tokens;
    for (;;) {
      auto token = grab_token (source);
      source = source.slice (token.spelling.length (), source.size ());
      tokens.push_back (token);
      if (!token) break; // check after adding; ensures null token at end of stream
    }
    return std::move (tokens);
  }
}
