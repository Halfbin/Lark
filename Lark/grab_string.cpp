
#include "grab_token.hpp"

#include <catch.hpp>

namespace Lark
{
  Rk::cstring_ref grab_string (Rk::cstring_ref in)
  {
    if (in.empty () || !is_quote (in [0]))
      throw std::logic_error ("oops");

    auto quot = in.begin ();

    do
    {
      // Find the next quote
      quot = std::find_if (quot + 1, in.end (), is_quote);

      if (quot == in.end ())
        throw std::runtime_error ("unclosed string");
    }
    while (quot [-1] == '\\');

    return { in.begin (), quot + 1 };
  }

  TEST_CASE ("grab_string")
  {
    REQUIRE (
      grab_string ("\"\" is an empty string")
                == "\"\"");
    REQUIRE (
      grab_string ("\"this string has a \\\" in\" x")
                == "\"this string has a \\\" in\"");
    REQUIRE_THROWS (
      grab_string ("\" this is an unclosed string!"));
  }

}
