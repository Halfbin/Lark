
#pragma once

#include "cursor.hpp"

namespace Lark {
  template <typename Result>
  struct Match {
    Cursor end;
    Result result;
    bool   matched;

    Match (Cursor end, Result result) :
      end (end), result (std::move (result)), matched (true)
    { }

    Match (Cursor end) :
      end (end), matched (false)
    { }

    template <typename Other>
    Match (const Match <Other>& other) :
      end     (other.end),
      result  (static_cast <Result> (other.result)),
      matched (other.matched)
    { }

    template <typename Other>
    Match (Match <Other>&& other) :
      end     (other.end),
      result  (std::move (other.result)),
      matched (other.matched)
    { }

    explicit operator bool () const {
      return matched;
    }

  };

}
