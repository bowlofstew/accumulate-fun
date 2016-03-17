#pragma once

#include "accumulate.h"

#include "modifying_seq_ops.h"
#include "non_modifying_seq_ops.h"

#include <iterator>

// ---------------------------------------------------------------------------
// 5 partitioning operations
//
// is_partitioned
// partition
// partition_copy
// stable_partition
// partition_point
//

namespace acc
{

  // ---------------------------------------------------------------------------
  // is_partitioned

  template <typename InputIt, typename UnaryPredicate>
  inline bool is_partitioned(
      InputIt first, InputIt last, UnaryPredicate p)
  {
    using T = typename std::iterator_traits<InputIt>::value_type;
    InputIt it = acc::find_if(
        first, last,
        [&] (const T& a) { return !p(a); });
    return acc::none_of(it, last, p);
  }

  // ---------------------------------------------------------------------------
  // partition and partition_copy

  template <typename ForwardIt, typename UnaryPredicate>
  ForwardIt partition(
      ForwardIt first, ForwardIt last, UnaryPredicate p)
  {
    first = acc::find_if_not(first, last, p);
    if (first == last) return first;
    return acc::accumulate_iter(
        std::next(first), last, first,
        [&] (ForwardIt i, ForwardIt f) {
          if (p(*f)) {
            std::iter_swap(i, f);
            return ++i;
          }
          return i;
        });
  }

  template <typename InputIt, typename OutputIt1,
            typename OutputIt2, typename UnaryPredicate>
  std::pair<OutputIt1, OutputIt2> partition_copy(
      InputIt first, InputIt last,
      OutputIt1 d_first_true, OutputIt2 d_first_false,
      UnaryPredicate p)
  {
    using T = typename std::iterator_traits<InputIt>::value_type;
    using P = std::pair<OutputIt1, OutputIt2>;

    return acc::accumulate(
        first, last, P{ d_first_true, d_first_false },
        [&] (P r, const T& a) {
          if (p(a)) {
            *r.first++ = a;
          } else {
            *r.second++ = a;
          }
          return r;
        });
  }

  // ---------------------------------------------------------------------------
  // stable_partition

  template <typename ForwardIt, typename UnaryPredicate>
  ForwardIt stable_partition(
      ForwardIt first, ForwardIt last, UnaryPredicate p)
  {
    auto n = std::distance(first, last);
    if (n == 0) return first;
    if (n == 1) return first + p(*first);
    auto m = first;
    std::advance(m, n/2);

    return acc::rotate(
        acc::stable_partition(first, m, p),
        m,
        acc::stable_partition(m, last, p));
  }

}
