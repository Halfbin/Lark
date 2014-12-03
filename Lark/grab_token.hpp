
#pragma once

#include "Token.hpp"

namespace Lark
{
  static bool is_quote (char c)
  {
    return c == '\"';
  }
  
  static bool is_id_start (char c)
  {
    return (c >= 'a' && c <= 'z')
        || (c >= 'A' && c <= 'Z')
        || (c == '_');
  }

  static bool is_digit (char c)
  {
    return (c >= '0' && c <= '9');
  }

  static bool is_id_medial (char c)
  {
    return is_id_start (c)
        || is_digit (c);
  }

  static bool is_id_end (char c)
  {
    return (c == '!') || (c == '?');
  }

  Rk::cstring_ref grab_string     (Rk::cstring_ref);
  Rk::cstring_ref grab_identifier (Rk::cstring_ref);

  Token grab_token (Rk::cstring_ref);

}
