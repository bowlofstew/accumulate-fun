#pragma once

#include "accumulate.h"

#include <iterator>
#include <utility>

// ---------------------------------------------------------------------------
// non-modifying sequence operations
// all_of
// any_of
// none_of
// for_each
// count
// count_if
// mismatch
// equal
// find
// find_if
// find_if_not
// find_end
// find_first_of
// adjacent_find
// search
// search_n

namespace acc
{
  // ---------------------------------------------------------------------------
  // find, find_if, find_if_not

  template <typename InputIt, typename T>
  InputIt find(InputIt first, InputIt last, const T& value)
  {
    try
    {
      return acc::accumulate_iter(
          first, last, last,
          [&] (const InputIt& a, const InputIt& b) {
            if (*b == value) throw b;
            return a;
          });
    }
    catch (InputIt& e)
    {
      return e;
    }
  }

  template <typename InputIt, typename UnaryPredicate>
  InputIt find_if(InputIt first, InputIt last, UnaryPredicate p)
  {
    try
    {
      return acc::accumulate_iter(
          first, last, last,
          [&] (const InputIt& a, const InputIt& b) {
            if (p(*b)) throw b;
            return a;
          });
    }
    catch (InputIt& e)
    {
      return e;
    }
  }

  template <typename InputIt, typename UnaryPredicate>
  InputIt find_if_not(InputIt first, InputIt last, UnaryPredicate p)
  {
    try
    {
      return acc::accumulate_iter(
          first, last, last,
          [&] (const InputIt& a, const InputIt& b) {
            if (!p(*b)) throw b;
            return a;
          });
    }
    catch (InputIt& e)
    {
      return e;
    }
  }

  template <class InputIt, class UnaryPredicate>
  bool all_of(InputIt first, InputIt last, UnaryPredicate p)
  {
    return acc::find_if_not(first, last, p) == last;
  }

  template <class InputIt, class UnaryPredicate>
  bool any_of(InputIt first, InputIt last, UnaryPredicate p)
  {
    return acc::find_if(first, last, p) != last;
  }

  template <class InputIt, class UnaryPredicate>
  bool none_of(InputIt first, InputIt last, UnaryPredicate p)
  {
    return acc::find_if(first, last, p) == last;
  }

  // ---------------------------------------------------------------------------
  // for_each

  template <typename InputIt, typename UnaryFunction>
  UnaryFunction for_each(InputIt first, InputIt last, UnaryFunction f)
  {
    using T = typename std::iterator_traits<InputIt>::value_type;
    return *acc::accumulate(
        first, last, &f,
        [] (UnaryFunction* f, const T& a) {
          (*f)(a); return f;
        });
  }

  // ---------------------------------------------------------------------------
  // count and count_if

  template <typename InputIt, typename T>
  typename std::iterator_traits<InputIt>::difference_type
  count(InputIt first, InputIt last, const T& value)
  {
    using DT = typename std::iterator_traits<InputIt>::difference_type;
    auto init = DT{};
    return acc::accumulate(
        first, last, init,
        [&] (const DT& c, const T& a) {
          return a == value ? c+1 : c;
        });
  }

  template <typename InputIt, typename UnaryPredicate>
  typename std::iterator_traits<InputIt>::difference_type
  count_if(InputIt first, InputIt last, UnaryPredicate p)
  {
    using DT = typename std::iterator_traits<InputIt>::difference_type;
    using T = typename std::iterator_traits<InputIt>::value_type;
    auto init = DT{};
    return acc::accumulate(
        first, last, init,
        [&] (const DT& c, const T& a) {
          return p(a) ? c+1 : c;
        });
  }

  // ---------------------------------------------------------------------------
  // mismatch

  template <typename InputIt1, typename InputIt2,
            typename BinaryPredicate>
  std::pair<InputIt1, InputIt2>
  mismatch(InputIt1 first1, InputIt1 last1,
           InputIt2 first2,
           BinaryPredicate p)
  {
    using P = std::pair<InputIt1, InputIt2>;
    try
    {
      return acc::accumulate_iter(
          first1, last1, P{ first1, first2 },
          [&] (P a, const InputIt1& b) {
            if (!p(*b, *a.second)) throw P{ b, a.second };
            return P{ b, ++a.second };
          });
    }
    catch (P& r)
    {
      return r;
    }
  }

  // ---------------------------------------------------------------------------
  // equal

  template <typename InputIt1, typename InputIt2,
            typename BinaryPredicate>
  bool equal(InputIt1 first1, InputIt1 last1,
             InputIt2 first2,
             BinaryPredicate p)
  {
    using P = std::pair<bool, InputIt2>;
    try
    {
      return acc::accumulate_iter(
          first1, last1, P{ true, first2 },
          [&] (P a, const InputIt1& b) {
            if (!p(*b, *a.second)) throw false;
            return P{ true, ++a.second };
          }).first;
    }
    catch (...)
    {
      return false;
    }
  }

}
