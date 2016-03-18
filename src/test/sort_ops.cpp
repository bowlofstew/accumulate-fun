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

DEF_PROPERTY(StableSort, SortingOps, vector<unsigned int> v)
{
  vector<unsigned int> w{v};
  stable_sort(v.begin(), v.end());
  acc::stable_sort(w.begin(), w.end());
  return w == v;
}

DEF_PROPERTY(NthElement, SortingOps, vector<unsigned int> v, unsigned long int i)
{
  if (v.empty()) return true;

  std::mt19937 g(i);
  std::uniform_int_distribution<int> dis(0, v.size()-1);

  auto n = dis(g);

  vector<unsigned int> w(v);

  auto vnth = v.begin() + n;
  nth_element(v.begin(), vnth, v.end());

  auto wnth = w.begin() + n;
  acc::nth_element(w.begin(), wnth, w.end());

  return wnth - w.begin() == vnth - v.begin() && *wnth == *vnth;
}
