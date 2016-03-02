#pragma once

#include <experimental/optional>
#include <functional>
#include <utility>

namespace acc
{

  // ---------------------------------------------------------------------------
  // accumulate (ordinary value form)
  template <typename InputIt, typename T, typename BinaryOperation>
  auto accumulate(InputIt first, InputIt last, T init, BinaryOperation op)
  {
    for (; first != last; ++first) {
      init = op(init, *first);
    }
    return init;
  }

  // ---------------------------------------------------------------------------
  // accumulate (iterator form)
  template <typename InputIt, typename T, typename BinaryOperation>
  auto accumulate_iter(InputIt first, InputIt last, T init, BinaryOperation op)
  {
    for (; first != last; ++first) {
      init = op(init, first);
    }
    return init;
  }

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

  // ---------------------------------------------------------------------------
  // partitioning operations
  // is_partitioned
  // partition
  // partition_copy
  // stable_partition
  // partition_point

  // ---------------------------------------------------------------------------
  // sorting operations
  // is_sorted
  // is_sorted_until
  // sort
  // partial_sort
  // partial_sort_copy
  // stable_sort
  // nth_element

  // ---------------------------------------------------------------------------
  // binary search operations
  // lower_bound
  // upper_bound
  // binary_search
  // equal_range

  // ---------------------------------------------------------------------------
  // set operations
  // merge
  // inplace_merge
  // includes
  // set_difference
  // set_intersection
  // set_symmetric_difference
  // set_union

  // ---------------------------------------------------------------------------
  // heap operations
  // is_heap
  // is_heap_until
  // make_heap
  // push_heap
  // pop_heap
  // sort_heap

  // ---------------------------------------------------------------------------
  // minimum/maximum operations
  // max
  // max_element
  // min
  // min_element
  // minmax
  // minmax_element
  // lexicographical_compare
  // is_permutation
  // next_permutation
  // prev_permutation

  // ---------------------------------------------------------------------------
  // numeric operations
  // iota
  // accumulate
  // inner_product
  // adjacent_difference
  // partial_sum

  // ---------------------------------------------------------------------------
  // min_element (normal iterator form)
  template <typename ForwardIt, typename Compare>
  ForwardIt min_element(ForwardIt first, ForwardIt last, Compare cmp)
  {
    return acc::accumulate_iter(
        first, last, first,
        [&cmp] (const ForwardIt& a, const ForwardIt& b) {
          return cmp(*a, *b) ? a : b;
        });
  }

  // ---------------------------------------------------------------------------
  // min_element (value form)
  template <typename InputIt, typename Compare>
  std::experimental::optional<typename std::iterator_traits<InputIt>::value_type>
  min_element_value(InputIt first, InputIt last, Compare cmp)
  {
    if (first == last) return std::experimental::nullopt;
    using T = typename std::iterator_traits<InputIt>::value_type;
    auto init = *first;
    return {
      acc::accumulate(
          ++first, last, init,
          [&cmp] (const T& a, const T& b) {
            return cmp(a, b) ? a : b;
          }) };
  }

  // ---------------------------------------------------------------------------
  // max_element (normal iterator form)
  template <typename ForwardIt, typename Compare>
  ForwardIt max_element(ForwardIt first, ForwardIt last, Compare cmp)
  {
    return acc::accumulate_iter(
        first, last, first,
        [&cmp] (const ForwardIt& a, const ForwardIt& b) {
          return cmp(*a, *b) ? b : a;
        });
  }

  // ---------------------------------------------------------------------------
  // max_element (value form)
  template <typename InputIt, typename Compare>
  std::experimental::optional<typename std::iterator_traits<InputIt>::value_type>
  max_element_value(InputIt first, InputIt last, Compare cmp)
  {
    if (first == last) return std::experimental::nullopt;
    using T = typename std::iterator_traits<InputIt>::value_type;
    auto init = *first;
    return {
      acc::accumulate(
          ++first, last, init,
          [&cmp] (const T& a, const T& b) {
            return cmp(a, b) ? b : a;
          }) };
  }

  // ---------------------------------------------------------------------------
  // minmax_element (normal iterator form)
  template <typename ForwardIt, typename Compare>
  std::pair<ForwardIt, ForwardIt>
  minmax_element(ForwardIt first, ForwardIt last, Compare cmp)
  {
    using P = std::pair<ForwardIt, ForwardIt>;
    return acc::accumulate_iter(
        first, last, P{ first, first },
        [&cmp] (const P& a, const ForwardIt& b) -> P {
          auto& f = cmp(*a.first, *b) ? a.first : b;
          auto& s = cmp(*a.second, *b) ? b : a.second;
          return {f, s};
        });
  }

  // ---------------------------------------------------------------------------
  // minmax_element (value form)
  template <typename InputIt, typename Compare>
  std::experimental::optional<
    std::pair<typename std::iterator_traits<InputIt>::value_type,
              typename std::iterator_traits<InputIt>::value_type>>
  minmax_element_value(InputIt first, InputIt last, Compare cmp)
  {
    if (first == last) return std::experimental::nullopt;
    using T = typename std::iterator_traits<InputIt>::value_type;
    using P = std::pair<T, T>;
    auto init = *first;
    return {
      acc::accumulate(
          ++first, last, P{init, init},
          [&cmp] (const P& a, const T& b) -> P {
            auto& f = cmp(a.first, b) ? a.first : b;
            auto& s = cmp(a.second, b) ? b : a.second;
            return {f, s};
          }) };
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
        [&value] (const DT& c, const T& a) {
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
        [&p] (const DT& c, const T& a) {
          return p(a) ? c+1 : c;
        });
  }

  // ---------------------------------------------------------------------------
  // copy and copy_if

  template <typename InputIt, typename OutputIt>
  OutputIt copy(InputIt first, InputIt last,
                OutputIt d_first)
  {
    using T = typename std::iterator_traits<InputIt>::value_type;
    return acc::accumulate(
        first, last, d_first,
        [] (const OutputIt& d, const T& a) {
          *d = a;
          return d+1;
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
        [&p] (const OutputIt& d, const T& a) {
          if (p(a)) {
            *d = a;
            return d+1;
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
        [&unary_op] (const OutputIt& d, const T& a) {
          *d = unary_op(a);
          return d+1;
        });
  }

  template <typename InputIt1, typename InputIt2,
            typename OutputIt, typename BinaryOperation>
  OutputIt transform(InputIt1 first1, InputIt1 last1, InputIt2 first2,
                     OutputIt d_first,
                     BinaryOperation binary_op)
  {
    using T = typename std::iterator_traits<InputIt1>::value_type;
    return acc::accumulate(
        first1, last1, d_first,
        [&first2, &binary_op] (const OutputIt& d, const T& a) {
          *d = binary_op(a, *first2);
          ++first2;
          return d+1;
        });
  }

  // ---------------------------------------------------------------------------
  // find and friends
  template <typename InputIt, typename T>
  InputIt find(InputIt first, InputIt last, const T& value)
  {
    try
    {
      return acc::accumulate_iter(
          first, last, last,
          [&value] (const InputIt& a, const InputIt& b) {
            if (*b == value) throw b;
            return a;
          });
    }
    catch (InputIt e)
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
          [&p] (const InputIt& a, const InputIt& b) {
            if (p(*b)) throw b;
            return a;
          });
    }
    catch (InputIt e)
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
          [&p] (const InputIt& a, const InputIt& b) {
            if (!p(*b)) throw b;
            return a;
          });
    }
    catch (InputIt e)
    {
      return e;
    }
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
          [&p] (const P& a, const InputIt1& b) {
            if (!p(*b, *a.second)) throw P{b, a.second};
            return P{b, a.second+1};
          });
    }
    catch (P r)
    {
      return r;
    }
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
