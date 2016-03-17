#include <all.h>

#include <testinator.h>

#include <algorithm>
#include <functional>
#include <vector>

using namespace std;

// ---------------------------------------------------------------------------
// sort operations

DEF_PROPERTY(IsSorted, SortOps, vector<unsigned int> v)
{
  sort(v.begin(), v.end(), std::less<>{});
  return acc::is_sorted(v.cbegin(), v.cend(), std::less<>{});
}

DEF_PROPERTY(IsSorted2, SortOps, vector<unsigned int> v)
{
  sort(v.begin(), v.end(), std::greater<>{});
  return v.empty() || !acc::is_sorted(v.cbegin(), v.cend(), std::less<>{});
}

DEF_PROPERTY(IsSortedUntil, SortOps, vector<unsigned int> v)
{
  auto d = v.size()/2;
  sort(v.begin(), v.begin()+d, std::less<>{});

  auto i = acc::is_sorted_until(v.cbegin(), v.cend(), std::less<>{});
  return i >= v.cbegin()+d;
}

DEF_PROPERTY(Sort, SortingOps, vector<unsigned int> v)
{
  vector<unsigned int> w{v};
  sort(v.begin(), v.end());
  acc::sort(w.begin(), w.end());
  return w == v;
}
