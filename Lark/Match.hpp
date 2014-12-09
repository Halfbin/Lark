
#pragma once

#include "Cursor.hpp"

namespace Lark {
  template <typename Result>
  struct Match {
    Cursor end;
    Result result;

    Match (Cursor end, Result result = Result ()) :
      end (end), result (std::move (result))
    { }

    template <typename OtherResult>
    Match (const Match <OtherResult>& other) :
      end (other.end),
      result (std::static_cast <Result> (other.result))
    { }

    explicit operator bool () const {
      return result;
    }

  };

}
