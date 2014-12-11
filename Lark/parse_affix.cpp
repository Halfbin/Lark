
#include "parse.hpp"

namespace Lark {
  namespace {
    bool is_prefix (Rk::cstring_ref spelling) {
      return spelling == "+"
          || spelling == "-"
          || spelling == "not"
          || spelling == "++"
          || spelling == "--";
    }

    bool is_postfix (Rk::cstring_ref spelling) {
      return spelling == "++"
          || spelling == "--";
    }

    template <typename Pred>
    auto parse_affixes (Cursor cursor, Pred pred) -> Match <Affix::Ops> {
      Affix::Ops ops;
      while (pred (cursor->spelling)) {
        ops.push_back (cursor->spelling);
        cursor = cursor.next ();
      }
      return { cursor, std::move (ops) };
    }

  }

  auto parse_affix (Cursor cursor) -> Match <Affix*> {
    auto prefixes = parse_affixes (cursor, is_prefix);

    auto inner = parse_primary (prefixes.end);
    if (!inner) return cursor;

    auto postfixes = parse_affixes (inner.end, is_postfix);

    auto affix = new Affix { prefixes.result, inner.result, postfixes.result };
    return { postfixes.end, affix };
  }

}
