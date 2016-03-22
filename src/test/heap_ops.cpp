#include <all.h>

#include <testinator.h>

#include <algorithm>
#include <functional>
#include <vector>

using namespace std;

// ---------------------------------------------------------------------------
// heap operations

DEF_PROPERTY(IsHeap, HeapOps, vector<unsigned int> v)
{
  make_heap(v.begin(), v.end());
  return acc::is_heap(v.cbegin(), v.cend(), std::less<>{});
}

DEF_PROPERTY(IsHeap2, HeapOps, vector<unsigned int> v)
{
  make_heap(v.begin(), v.end(), std::greater<>{});
  return v.empty() || !acc::is_heap(v.cbegin(), v.cend(), std::less<>{});
}

DEF_PROPERTY(IsHeapUntil, HeapOps, vector<unsigned int> v)
{
  auto d = v.size()/2;
  make_heap(v.begin(), v.begin()+d);

  auto i = acc::is_heap_until(v.cbegin(), v.cend());
  auto j = is_heap_until(v.cbegin(), v.cend());

  return i == j && i >= v.cbegin()+d;
}
