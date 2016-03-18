#pragma once

#include "accumulate.h"
#include "non_modifying_seq_ops.h"
#include "partitioning_ops.h"
#include "set_ops.h"

#include <functional>
#include <iterator>
#include <utility>

// ---------------------------------------------------------------------------
// 7 sorting operations
//
// is_sorted
// is_sorted_until
// sort
// partial_sort
// partial_sort_copy
// stable_sort
// nth_element
//

namespace acc
{
  // ---------------------------------------------------------------------------
  // is_sorted and is_sorted_until

  template <typename ForwardIt, typename Compare>
  inline ForwardIt is_sorted_until(
      ForwardIt first, ForwardIt last, Compare cmp)
  {
    using T = typename std::iterator_traits<ForwardIt>::value_type;
    ForwardIt it = acc::adjacent_find(
        first, last,
        [&] (const T& a, const T& b) { return !cmp(a, b); });
    return it == last ? last : ++it;
  }

  template <typename ForwardIt, typename Compare>
  inline bool is_sorted(ForwardIt first, ForwardIt last, Compare cmp)
  {
    return acc::is_sorted_until(first, last, cmp) == last;
  }

  // ---------------------------------------------------------------------------
  // sort

  template <typename ForwardIt, typename Compare>
  void sort(ForwardIt first, ForwardIt last, Compare cmp)
  {
    auto n = std::distance(first, last);
    if (n <= 1) return;
    auto pivot = *std::next(first, n/2);

    using T = typename std::iterator_traits<ForwardIt>::value_type;
    ForwardIt middle1 = acc::partition(
        first, last,
        [pivot, &cmp](const T& a){ return cmp(a, pivot); });
    ForwardIt middle2 = acc::partition(
        middle1, last,
        [pivot, &cmp](const T& a){ return !cmp(pivot, a); });

    acc::sort(first, middle1, cmp);
    acc::sort(middle2, last, cmp);
  }

  template <typename ForwardIt>
  void sort(ForwardIt first, ForwardIt last)
  {
    acc::sort(first, last, std::less<>{});
  }

  // ---------------------------------------------------------------------------
  // stable sort

  template <typename ForwardIt, typename Compare>
  void stable_sort(ForwardIt first, ForwardIt last, Compare cmp)
  {
    auto n = std::distance(first, last);
    if (n > 1)
    {
      auto m = first + n/2;
      acc::stable_sort(first, m, cmp);
      acc::stable_sort(m, last, cmp);
      acc::inplace_merge(first, m, last, cmp);
    }
  }

  template <typename ForwardIt>
  void stable_sort(ForwardIt first, ForwardIt last)
  {
    acc::stable_sort(first, last, std::less<>{});
  }

}
