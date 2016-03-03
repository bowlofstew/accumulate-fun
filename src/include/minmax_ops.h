#pragma once

#include "accumulate.h"

#include <experimental/optional>
#include <initializer_list>
#include <iterator>
#include <utility>

// ---------------------------------------------------------------------------
// 10 minimum/maximum operations
//
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
//

namespace acc
{

  // ---------------------------------------------------------------------------
  // min_element

  template <typename ForwardIt, typename Compare>
  ForwardIt min_element(ForwardIt first, ForwardIt last, Compare cmp)
  {
    return acc::accumulate_iter(
        first, last, first,
        [&] (const ForwardIt& a, const ForwardIt& b) {
          return cmp(*a, *b) ? a : b;
        });
  }

  // ---------------------------------------------------------------------------
  // max_element

  template <typename ForwardIt, typename Compare>
  ForwardIt max_element(ForwardIt first, ForwardIt last, Compare cmp)
  {
    return acc::accumulate_iter(
        first, last, first,
        [&] (const ForwardIt& a, const ForwardIt& b) {
          return cmp(*a, *b) ? b : a;
        });
  }

  // ---------------------------------------------------------------------------
  // minmax_element

  template <typename ForwardIt, typename Compare>
  std::pair<ForwardIt, ForwardIt>
  minmax_element(ForwardIt first, ForwardIt last, Compare cmp)
  {
    using P = std::pair<ForwardIt, ForwardIt>;
    return acc::accumulate_iter(
        first, last, P{ first, first },
        [&] (const P& a, const ForwardIt& b) -> P {
          auto& f = cmp(*a.first, *b) ? a.first : b;
          auto& s = cmp(*a.second, *b) ? b : a.second;
          return { f, s };
        });
  }

  // ---------------------------------------------------------------------------
  // min, max, minmax (initializer_list forms)

  template <typename T, typename Compare>
  T min(std::initializer_list<T> ilist, Compare cmp)
  {
    return *acc::min_element(ilist.begin(), ilist.end(), cmp);
  }

  template <typename T, typename Compare>
  T max(std::initializer_list<T> ilist, Compare cmp)
  {
    return *acc::max_element(ilist.begin(), ilist.end(), cmp);
  }

  template <typename T, typename Compare>
  std::pair<T, T> minmax(std::initializer_list<T> ilist, Compare cmp)
  {
    auto p = acc::minmax_element(ilist.begin(), ilist.end(), cmp);
    return { *p.first, *p.second };
  }

  // ---------------------------------------------------------------------------
  // min_element (safe value form)
  template <typename InputIt, typename Compare>
  std::experimental::optional<typename std::iterator_traits<InputIt>::value_type>
  min_element_safe(InputIt first, InputIt last, Compare cmp)
  {
    if (first == last) return std::experimental::nullopt;
    using T = typename std::iterator_traits<InputIt>::value_type;
    auto init = *first;
    return {
      acc::accumulate(
          ++first, last, init,
          [&] (const T& a, const T& b) {
            return cmp(a, b) ? a : b;
          }) };
  }

  // ---------------------------------------------------------------------------
  // max_element (safe value form)
  template <typename InputIt, typename Compare>
  std::experimental::optional<typename std::iterator_traits<InputIt>::value_type>
  max_element_safe(InputIt first, InputIt last, Compare cmp)
  {
    if (first == last) return std::experimental::nullopt;
    using T = typename std::iterator_traits<InputIt>::value_type;
    auto init = *first;
    return {
      acc::accumulate(
          ++first, last, init,
          [&] (const T& a, const T& b) {
            return cmp(a, b) ? b : a;
          }) };
  }

  // ---------------------------------------------------------------------------
  // minmax_element (safe value form)
  template <typename InputIt, typename Compare>
  std::experimental::optional<
    std::pair<typename std::iterator_traits<InputIt>::value_type,
              typename std::iterator_traits<InputIt>::value_type>>
  minmax_element_safe(InputIt first, InputIt last, Compare cmp)
  {
    if (first == last) return std::experimental::nullopt;
    using T = typename std::iterator_traits<InputIt>::value_type;
    using P = std::pair<T, T>;
    auto init = *first;
    return {
      acc::accumulate(
          ++first, last, P{ init, init },
          [&] (const P& a, const T& b) -> P {
            auto& f = cmp(a.first, b) ? a.first : b;
            auto& s = cmp(a.second, b) ? b : a.second;
            return { f, s };
          }) };
  }

}
