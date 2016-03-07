#pragma once

namespace acc
{

  // ---------------------------------------------------------------------------
  // accumulate (ordinary value form)
  template <typename InputIt, typename T, typename BinaryOperation>
  inline auto accumulate(
      InputIt first, InputIt last, T init, BinaryOperation op)
  {
    for (; first != last; ++first) {
      init = op(init, *first);
    }
    return init;
  }

  // ---------------------------------------------------------------------------
  // accumulate (iterator form)
  template <typename InputIt, typename T, typename BinaryOperation>
  inline auto accumulate_iter(
      InputIt first, InputIt last, T init, BinaryOperation op)
  {
    for (; first != last; ++first) {
      init = op(init, first);
    }
    return init;
  }

}
