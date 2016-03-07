#pragma once

#include "accumulate.h"

#include "modifying_seq_ops.h"
#include "non_modifying_seq_ops.h"

#include <iostream>
#include <iterator>
#include <utility>

// ---------------------------------------------------------------------------
// 7 set operations
//
// merge
// inplace_merge
// includes
// set_difference
// set_intersection
// set_symmetric_difference
// set_union
//

namespace acc
{

  // ---------------------------------------------------------------------------
  // merge

  template<class InputIt1, class InputIt2,
           class OutputIt, class Compare>
  inline OutputIt merge(
      InputIt1 first1, InputIt1 last1,
      InputIt2 first2, InputIt2 last2,
      OutputIt d_first, Compare comp)
  {
    using P = std::pair<OutputIt, InputIt2>;
    using T = typename std::iterator_traits<InputIt2>::value_type;
    P p = acc::accumulate_iter(
        first1, last1, P{ d_first, first2 },
        [&] (P a, const InputIt1& b) {
          a = acc::copy_while(
              a.second, last2, a.first,
              [&] (const T& t) { return comp(t, *b); });
          *a.first = *b;
          ++a.first;
          return a;
        });
    return acc::copy(p.second, last2, p.first);
  }

  // ---------------------------------------------------------------------------
  // set_intersection

  template<class InputIt1, class InputIt2,
           class OutputIt, class Compare>
  inline OutputIt set_intersection(
      InputIt1 first1, InputIt1 last1,
      InputIt2 first2, InputIt2 last2,
      OutputIt d_first, Compare comp)
  {
    using P = std::pair<OutputIt, InputIt2>;
    using T = typename std::iterator_traits<InputIt2>::value_type;
    return acc::accumulate_iter(
        first1, last1, P{ d_first, first2 },
        [&] (P a, const InputIt1& b) {
          a.second = acc::find_if(
              a.second, last2,
              [&] (const T& t) { return !comp(t, *b); });
          if (a.second != last2 && !comp(*b, *a.second)) {
            *a.first++ = *b;
            ++a.second;
          }
          return a;
        }).first;
  }

}
