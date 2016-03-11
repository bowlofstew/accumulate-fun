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

  template <typename InputIt1, typename InputIt2,
            typename OutputIt, typename Compare>
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
  // includes

  template <typename InputIt1, typename InputIt2, typename Compare>
  inline bool includes(
      InputIt1 first1, InputIt1 last1,
      InputIt2 first2, InputIt2 last2,
      Compare cmp)
  {
    using T = typename std::iterator_traits<InputIt1>::value_type;
    if (first2 == last2) return true;
    try
    {
      acc::accumulate(
          first1, last1, first2,
          [&] (InputIt2 i, const T& t) {
            if (cmp(*i, t)) throw false;
            if (!cmp(t, *i)) ++i;
            if (i == last2) throw true;
            return i;
          });
      return false;
    }
    catch (bool b)
    {
      return b;
    }
  }

  // ---------------------------------------------------------------------------
  // set_difference

  template <typename InputIt1, typename InputIt2,
            typename OutputIt, typename Compare>
  inline OutputIt set_difference(
      InputIt1 first1, InputIt1 last1,
      InputIt2 first2, InputIt2 last2,
      OutputIt d_first, Compare cmp)
  {
    using T = typename std::iterator_traits<InputIt1>::value_type;
    using U = typename std::iterator_traits<InputIt2>::value_type;
    if (first2 == last2) return acc::copy(first1, last1, d_first);

    using P = std::pair<OutputIt, InputIt2>;
    return acc::accumulate(
        first1, last1, P{ d_first, first2 },
          [&] (P p, const T& t) {
            if (p.second == last2 || cmp(t, *p.second)) {
              *p.first = t;
              return P{ ++p.first, p.second };
            }
            InputIt2 it2 = acc::find_if_not(
                p.second, last2,
                [&] (const U& u) { return cmp(t, u); });
            return P { p.first, ++it2 };
          }).first;
  }

  // ---------------------------------------------------------------------------
  // set_intersection

  template <typename InputIt1, typename InputIt2,
            typename OutputIt, typename Compare>
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

  // ---------------------------------------------------------------------------
  // set_symmetric_difference

  template <typename InputIt1, typename InputIt2,
            typename OutputIt, typename Compare>
  inline OutputIt set_symmetric_difference(
      InputIt1 first1, InputIt1 last1,
      InputIt2 first2, InputIt2 last2,
      OutputIt d_first, Compare cmp)
  {
    using T = typename std::iterator_traits<InputIt1>::value_type;
    using U = typename std::iterator_traits<InputIt2>::value_type;
    if (first1 == last1) return acc::copy(first2, last2, d_first);

    using P = std::pair<OutputIt, InputIt2>;
    P p = acc::accumulate(
        first1, last1, P{ d_first, first2 },
          [&] (P p, const T& t) {
            p = acc::copy_while(
                p.second, last2, p.first,
                [&] (const U& u) { return cmp(u, t); });
            if (p.second == last2 || cmp(t, *p.second)) {
              *p.first++ = t;
            }
            else if (p.second != last2) {
              ++p.second;
            }
            return p;
          });
    return acc::copy(p.second, last2, p.first);
  }

  // ---------------------------------------------------------------------------
  // set_union

  template <typename InputIt1, typename InputIt2,
            typename OutputIt, typename Compare>
  inline OutputIt set_union(
      InputIt1 first1, InputIt1 last1,
      InputIt2 first2, InputIt2 last2,
      OutputIt d_first, Compare cmp)
  {
    using T = typename std::iterator_traits<InputIt1>::value_type;
    using U = typename std::iterator_traits<InputIt2>::value_type;
    if (first1 == last1) return acc::copy(first2, last2, d_first);

    using P = std::pair<OutputIt, InputIt2>;
    P p = acc::accumulate(
        first1, last1, P{ d_first, first2 },
          [&] (P p, const T& t) {
            p = acc::copy_while(
                p.second, last2, p.first,
                [&] (const U& u) { return cmp(u, t); });
            if (p.second == last2 || cmp(t, *p.second)) {
              *p.first++ = t;
            }
            else if (p.second != last2) {
              *p.first++ = *p.second++;
            }
            return p;
          });
    return acc::copy(p.second, last2, p.first);
  }
}
