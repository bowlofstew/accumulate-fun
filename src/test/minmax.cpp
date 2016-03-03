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

DEF_PROPERTY(MinElementSafe, MinMax, const vector<unsigned int>& v)
{
  auto x = min_element(v.cbegin(), v.cend(), std::less<>());
  auto y = acc::min_element_safe(v.cbegin(), v.cend(), std::less<>());
  return (v.empty() && y == std::experimental::nullopt) || (*x == y);
}

DEF_PROPERTY(MaxElement, MinMax, const vector<unsigned int>& v)
{
  auto x = max_element(v.cbegin(), v.cend(), std::less<>());
  auto y = acc::max_element(v.cbegin(), v.cend(), std::less<>());
  return x == y;
}

DEF_PROPERTY(MaxElementSafe, MinMax, const vector<unsigned int>& v)
{
  auto x = max_element(v.cbegin(), v.cend(), std::less<>());
  auto y = acc::max_element_safe(v.cbegin(), v.cend(), std::less<>());
  return (v.empty() && y == std::experimental::nullopt) || (*x == y);
}

DEF_PROPERTY(MinMaxElement, MinMax, const vector<unsigned int>& v)
{
  auto x = minmax_element(v.cbegin(), v.cend(), std::less<>());
  auto y = acc::minmax_element(v.cbegin(), v.cend(), std::less<>());
  return x == y;
}

DEF_PROPERTY(MinMaxElementSafe, MinMax, const vector<unsigned int>& v)
{
  auto x = minmax_element(v.cbegin(), v.cend(), std::less<>());
  auto y = acc::minmax_element_safe(v.cbegin(), v.cend(), std::less<>());
  return (v.empty() && y == std::experimental::nullopt) ||
    (*x.first == y->first && *x.second == y->second);
}
