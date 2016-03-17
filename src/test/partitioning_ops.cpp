#include <all.h>

#include <testinator.h>

#include <algorithm>
#include <functional>
#include <vector>

using namespace std;

// ---------------------------------------------------------------------------
// partitioning operations

DEF_PROPERTY(IsPartitioned, PartitioningOps, vector<unsigned int> v)
{
  auto even = [] (unsigned int i) { return (i&1) == 0; };
  partition(v.begin(), v.end(), even);
  return acc::is_partitioned(v.cbegin(), v.cend(), even);
}
