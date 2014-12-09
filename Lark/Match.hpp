
#pragma once

#include "cursor.hpp"

namespace Lark {
  template <typename Result>
  struct Match {
    using ResultType = Result;

    Cursor end;
    Result result;

    Match (Cursor end, Result result = Result ()) :
      end (end), result (std::move (result))
    { }

    template <typename OtherResult>
    Match (const Match <OtherResult>& other) :
      end (other.end),
      result (static_cast <Result> (other.result))
    { }

    explicit operator bool () const {
      return !!result;
    }

  };

}
