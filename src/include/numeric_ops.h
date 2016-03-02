#pragma once

#include "accumulate.h"

#include <iterator>
#include <utility>

// ---------------------------------------------------------------------------
// numeric operations
// iota
// accumulate
// inner_product
// adjacent_difference
// partial_sum

namespace acc
{
  // ---------------------------------------------------------------------------
  // iota

  template <typename ForwardIt, typename T>
  void iota(ForwardIt first, ForwardIt last, T value)
  {
    acc::accumulate_iter(
        first, last, value,
        [] (T v, const ForwardIt& i) {
          *i = v; return ++v;
        });
  }

  // ---------------------------------------------------------------------------
  // inner_product

  template <typename InputIt1, typename InputIt2,
            typename T,
            typename BinaryOp1, typename BinaryOp2>
  T inner_product(InputIt1 first1, InputIt1 last1,
                  InputIt2 first2, T value,
                  BinaryOp1 op1, BinaryOp2 op2)
  {
    using U = typename std::iterator_traits<InputIt1>::value_type;
    return acc::accumulate(
        first1, last1, value,
        [&] (const T& v, const U& a) {
          return op1(v, op2(a, *first2++));
        });
  }

  // ---------------------------------------------------------------------------
  // adjacent_difference

  template <typename InputIt, typename OutputIt, typename BinaryOperation>
  OutputIt adjacent_difference(InputIt first, InputIt last,
                               OutputIt d_first, BinaryOperation op)
  {
    if (first == last) return d_first;
    using T = typename std::iterator_traits<InputIt>::value_type;
    using P = std::pair<OutputIt, T>;
    T acc = *first;
    *d_first = acc;
    return acc::accumulate(
        ++first, last, P{ ++d_first, std::move(acc) },
        [&] (P& p, const T& a) {
          *p.first = op(a, std::move(p.second));
          return P{ ++p.first, a };
        }).first;
  }

  // ---------------------------------------------------------------------------
  // partial_sum

  template <typename InputIt, typename OutputIt, typename BinaryOperation>
  OutputIt partial_sum(InputIt first, InputIt last,
                       OutputIt d_first, BinaryOperation op)
  {
    if (first == last) return d_first;
    using T = typename std::iterator_traits<InputIt>::value_type;
    using P = std::pair<OutputIt, T>;
    T acc = *first;
    *d_first = acc;
    return acc::accumulate(
        ++first, last, P{ ++d_first, std::move(acc) },
        [&] (P& p, const T& a) {
          T acc = op(std::move(p.second), a);
          *p.first = acc;
          return P{ ++p.first, std::move(acc) };
        }).first;
  }

}
