
#include "grab_token.hpp"

#include <catch.hpp>

namespace Lark
{
  /* [a-zA-Z_][a-zA-Z_0-9]*[!\?]? */

  Rk::cstring_ref grab_identifier (Rk::cstring_ref in)
  {
    if (in.empty () || !is_id_start (in [0]))
      throw std::logic_error ("oops");

    // Find the first character that's not medial
    auto end = std::find_if_not (in.begin () + 1, in.end (), is_id_medial);

    // Check if there's a ? or ! at the end
    if (end != in.end () && is_id_end (*end))
      end++;

    return { in.begin (), end };
  }

  TEST_CASE ("grab_identifier") {
    REQUIRE (
      grab_identifier ("identifiers end with spaces")
                    == "identifiers");
    REQUIRE (
      grab_identifier ("_identifiers can start with underscores")
                    == "_identifiers");
    REQUIRE (
      grab_identifier ("id3nt1fier5 can have digits in them")
                    == "id3nt1fier5");
    REQUIRE_THROWS (
      grab_identifier ("1dentifiers can't start with digits"));
    REQUIRE (
      grab_identifier ("identifiers! can end with bangs (and queries)")
                    == "identifiers!");
    REQUIRE (
      grab_identifier ("but?! they can't have them before the end")
                    == "but?");
  }

}
