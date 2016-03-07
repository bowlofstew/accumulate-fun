#pragma once

#include "accumulate.h"

#include <iterator>
#include <utility>

// ---------------------------------------------------------------------------
// 16 non-modifying sequence operations
//
// find
// find_if
// find_if_not
// all_of
// any_of
// none_of
// for_each
// count
// count_if
// mismatch
// equal
// find_end
// find_first_of
// adjacent_find
// search
// search_n
//

namespace acc
{
  // ---------------------------------------------------------------------------
  // find, find_if, find_if_not and derivatives

  template <typename InputIt, typename T>
  inline InputIt find(
      InputIt first, InputIt last, const T& value)
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
    catch (InputIt& i)
    {
      return i;
    }
  }

  template <typename InputIt, typename UnaryPredicate>
  inline InputIt find_if(
      InputIt first, InputIt last, UnaryPredicate p)
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
    catch (InputIt& i)
    {
      return i;
    }
  }

  template <typename InputIt, typename UnaryPredicate>
  inline InputIt find_if_not(
      InputIt first, InputIt last, UnaryPredicate p)
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
    catch (InputIt& i)
    {
      return i;
    }
  }

  template <class InputIt, class UnaryPredicate>
  inline bool all_of(
      InputIt first, InputIt last, UnaryPredicate p)
  {
    return acc::find_if_not(first, last, p) == last;
  }

  template <class InputIt, class UnaryPredicate>
  inline bool any_of(
      InputIt first, InputIt last, UnaryPredicate p)
  {
    return acc::find_if(first, last, p) != last;
  }

  template <class InputIt, class UnaryPredicate>
  inline bool none_of(
      InputIt first, InputIt last, UnaryPredicate p)
  {
    return acc::find_if(first, last, p) == last;
  }

  // ---------------------------------------------------------------------------
  // for_each

  template <typename InputIt, typename UnaryFunction>
  inline UnaryFunction for_each(
      InputIt first, InputIt last, UnaryFunction f)
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
  inline typename std::iterator_traits<InputIt>::difference_type
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
  inline typename std::iterator_traits<InputIt>::difference_type
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
  inline std::pair<InputIt1, InputIt2>
  mismatch(InputIt1 first1, InputIt1 last1,
           InputIt2 first2,
           BinaryPredicate p)
  {
    if (first1 == last1) return { first1, first2 };
    using P = std::pair<InputIt1, InputIt2>;
    try
    {
     return acc::accumulate_iter(
         first1, last1, P{ last1, first2 },
         [&] (P a, const InputIt1& b) {
           if (!p(*b, *a.second)) throw P{ b, a.second };
           return P{ a.first, ++a.second };
         });
    }
    catch (P& r)
    {
      return r;
    }
  }

  template <typename InputIt1, typename InputIt2,
            typename BinaryPredicate>
  inline std::pair<InputIt1, InputIt2>
  mismatch(InputIt1 first1, InputIt1 last1,
           InputIt2 first2, InputIt2 last2,
           BinaryPredicate p)
  {
    if (first1 == last1 || first2 == last2) return { first1, first2 };
    using P = std::pair<InputIt1, InputIt2>;
    try
    {
     return acc::accumulate_iter(
         first1, last1, P{ last1, first2 },
         [&] (P a, const InputIt1& b) {
           if (a.second == last2 || !p(*b, *a.second)) throw P{ b, a.second };
           return P{ a.first, ++a.second };
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
  inline bool equal(
      InputIt1 first1, InputIt1 last1,
      InputIt2 first2,
      BinaryPredicate p)
  {
    try
    {
      using P = std::pair<bool, InputIt2>;
      using T = typename std::iterator_traits<InputIt1>::value_type;
      return acc::accumulate(
          first1, last1, P{ true, first2 },
          [&] (P a, const T& b) {
            if (!p(b, *a.second)) throw false;
            return P{ true, ++a.second };
          }).first;
    }
    catch (...)
    {
      return false;
    }
  }

  template <typename InputIt1, typename InputIt2,
            typename BinaryPredicate>
  inline bool equal(
      InputIt1 first1, InputIt1 last1,
      InputIt2 first2, InputIt2 last2,
      BinaryPredicate p)
  {
    try
    {
      using P = std::pair<bool, InputIt2>;
      return acc::accumulate_iter(
          first1, last1, P{ true, first2 },
          [&] (P a, const InputIt1& b) {
            if (a.second == last2 || !p(*b, *a.second)) throw false;
            auto next = ++a.second;
            return P{ next == last2, next };
          }).first;
    }
    catch (...)
    {
      return false;
    }
  }

  // ---------------------------------------------------------------------------
  // find_end

  template <typename ForwardIt1, typename ForwardIt2, typename BinaryPredicate>
  inline ForwardIt1 find_end(
      ForwardIt1 first, ForwardIt1 last,
      ForwardIt2 s_first, ForwardIt2 s_last,
      BinaryPredicate p)
  {
    if (s_first == s_last) return last;
    return acc::accumulate_iter(
        first, last, last,
        [&] (ForwardIt1 l, ForwardIt1 i) {
          if (acc::mismatch(i, last, s_first, s_last, p).second == s_last) return i;
          return l;
        });
  }

  // ---------------------------------------------------------------------------
  // find_first_of

  template <typename InputIt, typename ForwardIt, typename BinaryPredicate>
  inline InputIt find_first_of(
      InputIt first, InputIt last,
      ForwardIt s_first, ForwardIt s_last,
      BinaryPredicate p)
  {
    try
    {
      using FT = typename std::iterator_traits<ForwardIt>::value_type;
      return acc::accumulate_iter(
          first, last, last,
          [&] (const InputIt& l, const InputIt& i) {
            if (acc::any_of(s_first, s_last,
                            [&] (const FT& s) { return p(*i, s); })) throw i;
            return l;
          });
    }
    catch (InputIt& i)
    {
      return i;
    }
  }

  // ---------------------------------------------------------------------------
  // adjacent_find

  template <typename ForwardIt, typename BinaryPredicate>
  inline ForwardIt adjacent_find(
      ForwardIt first, ForwardIt last,
      BinaryPredicate p)
  {
    if (first == last) return last;
    ForwardIt prev = first;
    try
    {
      return ++acc::accumulate_iter(
          ++first, last, prev,
          [&] (const ForwardIt& i, ForwardIt a) {
            if (p(*i, *a)) throw i;
            return a;
          });
    }
    catch (ForwardIt& i)
    {
      return i;
    }
  }

  // ---------------------------------------------------------------------------
  // search

  template <typename ForwardIt1, typename ForwardIt2, typename BinaryPredicate>
  inline ForwardIt1 search(
      ForwardIt1 first, ForwardIt1 last,
      ForwardIt2 s_first, ForwardIt2 s_last,
      BinaryPredicate p)
  {
    try
    {
      return acc::accumulate_iter(
          first, last, first,
          [&] (ForwardIt1 i, ForwardIt1) {
            if (acc::mismatch(i, last, s_first, s_last, p).second == s_last) throw i;
            return ++i;
          });
    }
    catch (ForwardIt1& i)
    {
      return i;
    }
  }

  // ---------------------------------------------------------------------------
  // search_n

  template <typename ForwardIt, typename Size,
            typename T, typename BinaryPredicate>
  inline ForwardIt search_n(
      ForwardIt first, ForwardIt last,
      Size count, const T& val, BinaryPredicate p)
  {
    using P = std::pair<ForwardIt, Size>;
    return acc::accumulate_iter(
        first, last, P{ first, count },
        [&] (P a, ForwardIt i) {
          if (a.second == 0) return a;
          if (p(*i, val)) return P{ a.first, a.second-1 };
          return P{ ++i, count };
        }).first;
  }

  // ---------------------------------------------------------------------------
  // As seen above, many of the find-type algorithms work on input iterators,
  // which means that accumulate is really just working as a glorified for loop
  // and the return has to be handled with an exception. This is unsatisfying.
  //
  // But: if we relax the iterator category to a forward iterator, we can write
  // _all versions of the same algorithms which find all the positions in the
  // sequence that satisfy the conditions. And these algorithms can be
  // parallelized by partitioning the sequence to subjobs. A couple of examples
  // follow.

  // ---------------------------------------------------------------------------
  // find_if_all
  //
  // find all positions in the sequence that satisfy the predicate

  template <typename ForwardIt, typename OutputIt, typename UnaryPredicate>
  inline OutputIt find_if_all(
      ForwardIt first, ForwardIt last,
      OutputIt dest, UnaryPredicate p)
  {
    return acc::accumulate_iter(
        first, last, dest,
        [&] (OutputIt d, const ForwardIt& a) {
          if (p(*a)) *d++ = a;
          return d;
        });
  }

  // ---------------------------------------------------------------------------
  // adjacent_find_all
  //
  // find all positions in the sequence that satisfy the binary predicate
  // applied to adjacent values

  template <typename ForwardIt, typename OutputIt, typename BinaryPredicate>
  inline OutputIt adjacent_find_all(
      ForwardIt first, ForwardIt last,
      OutputIt dest, BinaryPredicate p)
  {
    if (first == last) return dest;
    using P = std::pair<OutputIt, ForwardIt>;
    ForwardIt prev = first;
    return acc::accumulate_iter(
        ++first, last, P{ dest, prev },
        [&] (P a, const ForwardIt& b) {
          if (p(*a.second, *b)) *a.first++ = a.second;
          return P{ a.first, b };
        }).first;
  }

}
