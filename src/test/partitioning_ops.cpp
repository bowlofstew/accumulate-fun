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

DEF_PROPERTY(Partition, PartitioningOps, vector<unsigned int> v)
{
  auto even = [] (unsigned int i) { return (i&1) == 0; };
  auto x = acc::partition(v.begin(), v.end(), even);
  auto y = partition_point(v.begin(), v.end(), even);
  return is_partitioned(v.begin(), v.end(), even) && x == y;
}

DEF_PROPERTY(PartitionCopy, PartitioningOps, const vector<unsigned int>& v)
{
  auto even = [] (unsigned int i) { return (i&1) == 0; };

  vector<unsigned int> x;
  vector<unsigned int> y;
  acc::partition_copy(v.cbegin(), v.cend(),
                      back_inserter(x), back_inserter(y), even);

  return all_of(x.cbegin(), x.cend(), even)
    && none_of(y.cbegin(), y.cend(), even)
    && x.size() + y.size() == v.size();
}

DEF_PROPERTY(StablePartition, PartitioningOps, vector<unsigned int> v)
{
  auto even = [] (unsigned int i) { return (i&1) == 0; };
  auto x = acc::stable_partition(v.begin(), v.end(), even);
  auto y = partition_point(v.begin(), v.end(), even);
  return is_partitioned(v.begin(), v.end(), even) && x == y;
}

