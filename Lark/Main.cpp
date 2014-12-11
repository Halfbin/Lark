
#define CATCH_CONFIG_RUNNER
#include <catch.hpp>

namespace Lark {
  extern "C" int main () {
    Catch::Session session;
    return session.run ();
  }

}
