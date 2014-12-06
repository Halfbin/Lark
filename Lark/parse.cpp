
#include "parse.hpp"

namespace Lark {
  Match parse (Cursor tokens) {
    return parse_recursive (Parser::begin (parse_stream, tokens));
  }

}
