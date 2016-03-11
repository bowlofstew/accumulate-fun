#pragma once

#include "accumulate.h"

#include <iterator>
#include <functional>
#include <random>
#include <utility>

// ---------------------------------------------------------------------------
// 27 modifying sequence operations
// (iter_swap, swap, random_shuffle omitted)
//
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
// swap_ranges
// reverse
// reverse_copy
// rotate
// rotate_copy
// shuffle
// unique
// unique_copy
//

namespace acc
{

  // ---------------------------------------------------------------------------
  // copy, copy_if and copy_n

  template <typename InputIt, typename OutputIt>
  inline OutputIt copy(
      InputIt first, InputIt last,
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
  inline OutputIt copy_if(
      InputIt first, InputIt last,
      OutputIt d_first, UnaryPredicate p)
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

  template <typename InputIt, typename Size, typename OutputIt>
  inline OutputIt copy_n(
      InputIt first, Size count, OutputIt result)
  {
    if (count <= Size{0}) return result;
    using P = std::pair<OutputIt, InputIt>;
    return acc::accumulate_iter(
        Size{0}, count, P{ result, first },
        [] (P p, Size) {
          *p.first++ = *p.second++;
          return p;
        }).first;
  }

  // ---------------------------------------------------------------------------
  // copy_while - a useful extra algorithm (and a helper for merge)

  template <typename InputIt, typename OutputIt, typename UnaryPredicate>
  inline std::pair<OutputIt, InputIt> copy_while(
      InputIt first, InputIt last,
      OutputIt d_first,
      UnaryPredicate p)
  {
    using P = std::pair<OutputIt, InputIt>;
    try
    {
      OutputIt o = acc::accumulate_iter(
          first, last, d_first,
          [&] (OutputIt d, InputIt a) {
            if (!p(*a)) throw P{ d, a };
            *d = *a;
            return ++d;
          });
      return P{ o, last };
    }
    catch (P& p)
    {
      return p;
    }
  }

  // ---------------------------------------------------------------------------
  // copy_backward

  template <typename BidirIt1, typename BidirIt2>
  inline BidirIt2 copy_backward(
      BidirIt1 first, BidirIt1 last,
      BidirIt2 d_last)
  {
    using T = typename std::iterator_traits<BidirIt1>::reference;
    using F = std::function<BidirIt2(BidirIt2)>;
    return acc::accumulate<BidirIt1, F, std::function<F(F, T)>>(
        first, last, [] (BidirIt2 d) { return d; },
        [] (F a, const T b) {
          return [a = std::move(a), &b] (BidirIt2 d) {
            *(--d) = b;
            return a(d); };
        })(d_last);
  }

  // ---------------------------------------------------------------------------
  // move and move_backward

  template <typename InputIt, typename OutputIt>
  inline OutputIt move(
      InputIt first, InputIt last,
      OutputIt d_first)
  {
    using T = typename std::iterator_traits<InputIt>::value_type;
    return acc::accumulate(
        first, last, d_first,
        [] (OutputIt d, T& a) {
          *d = std::move(a);
          return ++d;
        });
  }

  template <typename BidirIt1, typename BidirIt2>
  inline BidirIt2 move_backward(
      BidirIt1 first, BidirIt1 last,
      BidirIt2 d_last)
  {
    using T = typename std::iterator_traits<BidirIt1>::reference;
    using F = std::function<BidirIt2(BidirIt2)>;
    return acc::accumulate<BidirIt1, F, std::function<F(F, T)>>(
        first, last, [] (BidirIt2 d) { return d; },
        [] (F a, T& b) {
          return [a = std::move(a), &b] (BidirIt2 d) {
            *(--d) = std::move(b);
            return a(d); };
        })(d_last);
  }

  // ---------------------------------------------------------------------------
  // fill and fill_n

  template <typename ForwardIt, typename T>
  inline void fill(
      ForwardIt first, ForwardIt last,
      const T& value)
  {
    acc::accumulate_iter(
        first, last, value,
        [] (const T& a, ForwardIt b) {
          *b = a;
          return a;
        });
  }

  template <typename OutputIt, typename Size, typename T>
  inline OutputIt fill_n(
      OutputIt first, Size count, const T& value)
  {
    if (count <= Size{0}) return first;
    using P = std::pair<OutputIt, const T*>;
    return acc::accumulate_iter(
        Size{0}, count, P{ first, &value },
        [] (P p, Size) {
          *p.first++ = *p.second;
          return p;
        }).first;
  }

  // ---------------------------------------------------------------------------
  // transform

  template <typename InputIt, typename OutputIt, typename UnaryOperation>
  inline OutputIt transform(
      InputIt first1, InputIt last1, OutputIt d_first,
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
  inline OutputIt transform(
      InputIt1 first1, InputIt1 last1, InputIt2 first2,
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
  // generate and generate_n

  template <typename ForwardIt, typename Generator>
  inline void generate(
      ForwardIt first, ForwardIt last, Generator g)
  {
    acc::accumulate_iter(
        first, last, &g,
        [] (Generator* g, ForwardIt b) {
          *b = (*g)();
          return g;
        });
  }

  template <typename OutputIt, typename Size, typename Generator>
  inline OutputIt generate_n(
      OutputIt first, Size count, Generator g)
  {
    if (count < Size{0}) return first;
    using P = std::pair<OutputIt, Generator*>;
    return acc::accumulate_iter(
        Size{0}, count, P{ first, &g },
        [] (P p, Size) {
          *p.first++ = (*p.second)();
          return p;
        }).first;
  }

  // ---------------------------------------------------------------------------
  // remove_copy and remove_copy_if

  template <typename InputIt, typename OutputIt, typename T>
  inline OutputIt remove_copy(
      InputIt first, InputIt last,
      OutputIt d_first, const T& value)
  {
    using U = typename std::iterator_traits<InputIt>::value_type;
    return acc::accumulate(
        first, last, d_first,
        [&] (OutputIt d, const U& a) {
          if (a != value)
            *d++ = a;
          return d;
        });
  }

  template <typename InputIt, typename OutputIt, typename UnaryPredicate>
  inline InputIt remove_copy_if(
      InputIt first, InputIt last,
      OutputIt d_first, UnaryPredicate p)
  {
    using T = typename std::iterator_traits<InputIt>::value_type;
    return acc::accumulate(
        first, last, d_first,
        [&] (OutputIt d, const T& a) {
          if (!p(*d))
            *d++ = a;
          return d;
        });
  }

  // ---------------------------------------------------------------------------
  // remove and remove_if

  template <typename ForwardIt, typename T>
  inline ForwardIt remove(
      ForwardIt first, ForwardIt last,
      const T& value)
  {
    return acc::remove_copy(first, last, first, value);
  }

  template <typename ForwardIt, typename UnaryPredicate>
  inline ForwardIt remove_if(
      ForwardIt first, ForwardIt last,
      UnaryPredicate p)
  {
    return acc::remove_copy_if(first, last, first, p);
  }

  // ---------------------------------------------------------------------------
  // replace_copy and replace_copy_if

  template <typename InputIt, typename OutputIt, typename T>
  inline OutputIt replace_copy(
      InputIt first, InputIt last,
      OutputIt d_first,
      const T& old_value, const T& new_value)
  {
    using U = typename std::iterator_traits<InputIt>::value_type;
    return acc::accumulate(
        first, last, d_first,
        [&] (OutputIt d, const U& a) {
          *d = (a == old_value) ? new_value : a;
          return ++d;
        });
  }

  template <typename InputIt, typename OutputIt,
            typename UnaryPredicate, typename T>
  inline InputIt replace_copy_if(
      InputIt first, InputIt last,
      OutputIt d_first, UnaryPredicate p,
      const T& new_value)
  {
    using U = typename std::iterator_traits<InputIt>::value_type;
    return acc::accumulate(
        first, last, d_first,
        [&] (OutputIt d, const U& a) {
          *d = p(a) ? new_value : a;
          return ++d;
        });
  }

  // ---------------------------------------------------------------------------
  // replace and replace_if

  template <typename ForwardIt, typename T>
  inline void replace(
      ForwardIt first, ForwardIt last,
      const T& old_value, const T& new_value)
  {
    acc::replace_copy(first, last, first, old_value, new_value);
  }

  template <typename ForwardIt, typename UnaryPredicate, typename T>
  inline void replace_if(
      ForwardIt first, ForwardIt last,
      UnaryPredicate p, const T& old_value)
  {
    acc::replace_copy_if(first, last, first, p, old_value);
  }

  // ---------------------------------------------------------------------------
  // swap_ranges

  template <typename ForwardIt1, typename ForwardIt2>
  inline ForwardIt2 swap_ranges(
      ForwardIt1 first1, ForwardIt1 last1,
      ForwardIt2 first2)
  {
    return acc::accumulate_iter(
        first1, last1, first2,
        [&] (ForwardIt2 a, ForwardIt1 b) {
          std::iter_swap(a, b);
          return ++a;
        });
  }

  // ---------------------------------------------------------------------------
  // reverse (note weakened iterator category)

  template<class ForwardIt>
  inline void reverse(ForwardIt first, ForwardIt last)
  {
    using diff_t = typename std::iterator_traits<ForwardIt>::difference_type;
    diff_t d = std::distance(first, last);
    ForwardIt mid = first;
    std::advance(mid, d/2);

    using T = typename std::iterator_traits<ForwardIt>::reference;
    using F = std::function<ForwardIt(ForwardIt)>;
    using std::swap;
    auto f = acc::accumulate<ForwardIt, F, std::function<F(F, T)>>(
        first, mid, [] (ForwardIt i) { return i; },
        [&] (F a, const T b) {
          return [a = std::move(a), &b] (ForwardIt i) {
            swap(*i, b);
            return a(++i); };
        });
    if (d&1) ++mid;
    f(mid);
  }
  
  // ---------------------------------------------------------------------------
  // reverse_copy (note weakened iterator category)

  template <typename ForwardIt, typename OutputIt>
  inline OutputIt reverse_copy(ForwardIt first, ForwardIt last, OutputIt d_first)
  {
    using T = typename std::iterator_traits<ForwardIt>::reference;
    using F = std::function<OutputIt(OutputIt)>;
    return acc::accumulate<ForwardIt, F, std::function<F(F, T)>>(
        first, last, [] (OutputIt d) { return d; },
        [] (F a, const T b) {
          return [a = std::move(a), &b] (OutputIt d) {
            *d = b;
            return a(++d); };
        })(d_first);
  }

  // ---------------------------------------------------------------------------
  // rotate

  template <class ForwardIt>
  inline ForwardIt rotate(ForwardIt first, ForwardIt n_first, ForwardIt last)
  {
    if (n_first == last) return last;
    ForwardIt ret = first;
    acc::accumulate_iter(
        first, last, n_first,
        [&] (ForwardIt a, ForwardIt b) {
          iter_swap(a++, b++);
          if (a == last) {
            if (ret == first) ret = b;
            a = n_first;
          } else if (b == n_first) {
            n_first = a;
          }
          return a;
        });
    return ret;
  }

  // ---------------------------------------------------------------------------
  // rotate_copy

  template <typename ForwardIt, typename OutputIt>
  inline OutputIt rotate_copy(
      ForwardIt first, ForwardIt n_first,
      ForwardIt last, OutputIt d_first)
  {
    d_first = acc::copy(n_first, last, d_first);
    return acc::copy(first, n_first, d_first);
  }

  // ---------------------------------------------------------------------------
  // shuffle

  template <typename RandomIt, typename URNG>
  inline void shuffle(RandomIt first, RandomIt last, URNG&& g)
  {
    using diff_t = typename std::iterator_traits<RandomIt>::difference_type;
    using udiff_t = typename std::make_unsigned<diff_t>::type;
    using distr_t = typename std::uniform_int_distribution<udiff_t>;
    using param_t = typename distr_t::param_type;

    distr_t D;
    udiff_t d = last - first - 1;
    acc::accumulate_iter(
        first, last, d,
        [&] (udiff_t d, RandomIt i) {
          RandomIt r = i + D(g, param_t{0, d});
          std::iter_swap(i, r);
          return --d;
        });
  }

  // ---------------------------------------------------------------------------
  // unique and unique_copy

  template <typename InputIt, typename OutputIt, typename BinaryPredicate>
  inline OutputIt unique_copy(
      InputIt first, InputIt last,
      OutputIt d_first, BinaryPredicate p)
  {
    using U = typename std::iterator_traits<InputIt>::value_type;
    using P = std::pair<OutputIt, U>;
    if (first == last) return d_first;
    U init = *first;
    *d_first = init;
    return acc::accumulate(
        ++first, last, P{ ++d_first, std::move(init) },
        [&] (P a, const U& b) {
          if (!p(a.second, b)) *a.first++ = b;
          return P{ a.first, b };
        }).first;
  }

  template <typename ForwardIt, typename BinaryPredicate>
  inline ForwardIt unique(
      ForwardIt first, ForwardIt last, BinaryPredicate p)
  {
    return acc::unique_copy(first, last, first, p);
  }

}
