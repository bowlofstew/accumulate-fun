#pragma once

#include "accumulate.h"

#include <iterator>
#include <functional>
#include <utility>

// ---------------------------------------------------------------------------
// modifying sequence operations
// copy
// copy_if
// copy_n
// copy_backward
// move
// move_backward
// fill
// fill_n
// transform
// generate
// generate_n
// remove
// remove_if
// remove_copy
// remove_copy_if
// replace
// replace_if
// replace_copy
// replace_copy_if
// swap
// swap_ranges
// iter_swap
// reverse
// reverse_copy
// rotate
// rotate_copy
// shuffle
// unique
// unique_copy

namespace acc
{

  // ---------------------------------------------------------------------------
  // copy and copy_if

  template <typename InputIt, typename OutputIt>
  OutputIt copy(InputIt first, InputIt last,
                OutputIt d_first)
  {
    using T = typename std::iterator_traits<InputIt>::value_type;
    return acc::accumulate(
        first, last, d_first,
        [] (OutputIt d, const T& a) {
          *d = a;
          return ++d;
        });
  }

  template <typename InputIt, typename OutputIt, typename UnaryPredicate>
  OutputIt copy_if(InputIt first, InputIt last,
                   OutputIt d_first,
                   UnaryPredicate p)
  {
    using T = typename std::iterator_traits<InputIt>::value_type;
    return acc::accumulate(
        first, last, d_first,
        [&] (OutputIt d, const T& a) {
          if (p(a)) {
            *d = a;
            return ++d;
          }
          return d;
        });
  }

  // ---------------------------------------------------------------------------
  // transform

  template <typename InputIt, typename OutputIt, typename UnaryOperation>
  OutputIt transform(InputIt first1, InputIt last1, OutputIt d_first,
                     UnaryOperation unary_op)
  {
    using T = typename std::iterator_traits<InputIt>::value_type;
    return acc::accumulate(
        first1, last1, d_first,
        [&] (OutputIt d, const T& a) {
          *d = unary_op(a);
          return ++d;
        });
  }

  template <typename InputIt1, typename InputIt2,
            typename OutputIt, typename BinaryOperation>
  OutputIt transform(InputIt1 first1, InputIt1 last1, InputIt2 first2,
                     OutputIt d_first,
                     BinaryOperation binary_op)
  {
    using T = typename std::iterator_traits<InputIt1>::value_type;
    using P = std::pair<InputIt2, OutputIt>;
    return acc::accumulate(
        first1, last1, P{ first2, d_first },
        [&] (P d, const T& a) {
          *d.second = binary_op(a, *d.first);
          return P{ ++d.first, ++d.second };
        }).second;
  }

  // ---------------------------------------------------------------------------
  // reverse_copy

  template <typename BidirIt, typename OutputIt>
  OutputIt reverse_copy(BidirIt first, BidirIt last, OutputIt d_first)
  {
    using T = typename std::iterator_traits<BidirIt>::reference;
    using F = std::function<OutputIt(OutputIt)>;
    return acc::accumulate<BidirIt, F, std::function<F(F, T)>>(
        first, last, [] (OutputIt d) { return d; },
        [] (F a, const T b) {
          return [a = std::move(a), &b] (OutputIt d) { *d = b; return a(++d); };
        })(d_first);
  }

}
