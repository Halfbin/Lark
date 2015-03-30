
#include <Rk/string_ref.hpp>

#define CATCH_CONFIG_RUNNER
#include <catch.hpp>

namespace Lark {
  extern "C" int main (int arg_count, char** args) {
    if (arg_count > 1 && Rk::cstring_ref (args [1]) == "test") {
      Catch::Session session;
      session.configData ().showSuccessfulTests = false;
      return session.run ();
    }
  }

}
