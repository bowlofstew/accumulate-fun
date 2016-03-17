#pragma once

#include "accumulate.h"

// ---------------------------------------------------------------------------
// 5 partitioning operations
//
// is_partitioned
// partition
// partition_copy
// stable_partition
// partition_point
//

namespace acc
{

  // ---------------------------------------------------------------------------
  // is_partitioned
  template <typename InputIt, typename UnaryPredicate>
  inline bool is_partitioned(
      InputIt first, InputIt last, UnaryPredicate p)
  {
    using T = typename std::iterator_traits<InputIt>::value_type;
    InputIt it = acc::find_if(
        first, last,
        [&] (const T& a) { return !p(a); });
    return acc::none_of(it, last, p);
  }

}
