#include <all.h>

#include <testinator.h>

#include <algorithm>
#include <functional>
#include <vector>

using namespace std;

// ---------------------------------------------------------------------------
// minimum/maximum operations

DEF_PROPERTY(MinElement, MinMax, const vector<unsigned int>& v)
{
  auto x = min_element(v.cbegin(), v.cend(), std::less<>());
  auto y = acc::min_element(v.cbegin(), v.cend(), std::less<>());
  return x == y;
}

DEF_PROPERTY(MaxElement, MinMax, const vector<unsigned int>& v)
{
  auto x = max_element(v.cbegin(), v.cend(), std::less<>());
  auto y = acc::max_element(v.cbegin(), v.cend(), std::less<>());
  return x == y;
}

DEF_PROPERTY(MinMaxElement, MinMax, const vector<unsigned int>& v)
{
  auto x = minmax_element(v.cbegin(), v.cend(), std::less<>());
  auto y = acc::minmax_element(v.cbegin(), v.cend(), std::less<>());
  return x == y;
}

#ifndef _MSC_VER
DEF_PROPERTY(MinElementSafe, MinMax, const vector<unsigned int>& v)
{
  auto x = min_element(v.cbegin(), v.cend(), std::less<>());
  auto y = acc::min_element_safe(v.cbegin(), v.cend(), std::less<>());
  return (v.empty() && y == std::experimental::nullopt) || (*x == y);
}

DEF_PROPERTY(MaxElementSafe, MinMax, const vector<unsigned int>& v)
{
  auto x = max_element(v.cbegin(), v.cend(), std::less<>());
  auto y = acc::max_element_safe(v.cbegin(), v.cend(), std::less<>());
  return (v.empty() && y == std::experimental::nullopt) || (*x == y);
}

DEF_PROPERTY(MinMaxElementSafe, MinMax, const vector<unsigned int>& v)
{
  auto x = minmax_element(v.cbegin(), v.cend(), std::less<>());
  auto y = acc::minmax_element_safe(v.cbegin(), v.cend(), std::less<>());
  return (v.empty() && y == std::experimental::nullopt) ||
    (*x.first == y->first && *x.second == y->second);
}
#endif

DEF_PROPERTY(LexicographicalCompare, MinMax, const vector<unsigned int>& v)
{
  bool a = acc::lexicographical_compare(v.cbegin(), v.cend(),
                                        v.cbegin(), v.cend(), std::less<>());
  if (v.empty()) return true;

  bool b = acc::lexicographical_compare(v.cbegin(), v.cend()-1,
                                        v.cbegin(), v.cend(), std::less<>());
  bool c = acc::lexicographical_compare(v.cbegin(), v.cend(),
                                        v.cbegin(), v.cend()-1, std::less<>());
  return !a && b && !c;
}

DEF_PROPERTY(LexicographicalCompare2, MinMax,
             const vector<unsigned int>& v, const vector<unsigned int>& w)
{
  bool a = lexicographical_compare(v.cbegin(), v.cend(),
                                   w.cbegin(), w.cend(), std::less<>());
  bool b = acc::lexicographical_compare(v.cbegin(), v.cend(),
                                        w.cbegin(), w.cend(), std::less<>());
  return a == b;
}

DEF_PROPERTY(IsPermutation, MinMax, const vector<unsigned int>& v, unsigned long int i)
{
  vector<unsigned int> w{v};

  std::mt19937 g(i);
  shuffle(w.begin(), w.end(), g);

  return acc::is_permutation(v.cbegin(), v.cend(), w.cbegin());
}
