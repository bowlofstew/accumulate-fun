#pragma once

#include "accumulate.h"

#include <functional>

// ---------------------------------------------------------------------------
// 6 heap operations
//
// is_heap
// is_heap_until
// make_heap
// push_heap
// pop_heap
// sort_heap
//

namespace acc
{

  // ---------------------------------------------------------------------------
  // is_heap_until and is_heap

  template <typename RandomIt, typename Compare>
  inline RandomIt is_heap_until(
      RandomIt first, RandomIt last, Compare cmp)
  {
    auto n = std::distance(first, last);
    if (n < 2) return last;
    try
    {
      return acc::accumulate_iter(
          first, first + n/2, first+1,
          [&] (RandomIt i, const RandomIt& a) {
            if (cmp(*a, *i)) throw i;
            if (++i == last) return i;
            if (cmp(*a, *i)) throw i;
            return ++i;
          });
    }
    catch (RandomIt& r)
    {
      return r;
    }
  }

  template <typename RandomIt>
  inline RandomIt is_heap_until(
      RandomIt first, RandomIt last)
  {
    return acc::is_heap_until(first, last, std::less<>{});
  }

  template <typename RandomIt, typename Compare>
  inline bool is_heap(
      RandomIt first, RandomIt last, Compare cmp)
  {
    return acc::is_heap_until(first, last, cmp) == last;
  }

  template <typename RandomIt>
  inline bool is_heap(
      RandomIt first, RandomIt last)
  {
    return acc::is_heap(first, last, std::less<>{});
  }

}
