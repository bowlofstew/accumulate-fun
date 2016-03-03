#include <all.h>

#include <testinator.h>

#include <algorithm>
#include <functional>
#include <numeric>
#include <vector>

using namespace std;

// ---------------------------------------------------------------------------
// numeric operations

DEF_PROPERTY(Iota, Numeric, unsigned int i)
{
  vector<unsigned int> v;
  vector<unsigned int> w;

  iota(v.begin(), v.end(), i);
  acc::iota(w.begin(), w.end(), i);

  return equal(v.cbegin(), v.cend(), w.cbegin());
}

DEF_PROPERTY(Accumulate, Numeric, const vector<unsigned int>& v)
{
  auto x = accumulate(v.cbegin(), v.cend(), 0, std::plus<>());
  auto y = acc::accumulate(v.cbegin(), v.cend(), 0, std::plus<>());
  return x == y;
}

DEF_PROPERTY(InnerProduct, Numeric, const vector<unsigned int>& v)
{
  auto x = inner_product(v.cbegin(), v.cend(), v.cbegin(), 0, plus<>(), multiplies<>());
  auto y = acc::inner_product(v.cbegin(), v.cend(), v.cbegin(), 0, plus<>(), multiplies<>());

  return x == y;
}

DEF_PROPERTY(AdjacentDifference, Numeric, const vector<int>& v)
{
  vector<int> outx(v.size(), 0);
  auto x = adjacent_difference(v.cbegin(), v.cend(), outx.data(), minus<>());

  vector<int> outy(v.size(), 0);
  auto y = acc::adjacent_difference(v.cbegin(), v.cend(), outy.data(), minus<>());

  using ST = typename vector<unsigned int>::size_type;
  auto sx = static_cast<ST>(x - outx.data());
  auto sy = static_cast<ST>(y - outy.data());

  return sx == outx.size() && sy == outy.size()
    && equal(outx.cbegin(), outx.cend(), outy.cbegin(), outy.cend());
}

DEF_PROPERTY(PartialSum, Numeric, const vector<int>& v)
{
  vector<int> outx(v.size(), 0);
  auto x = partial_sum(v.cbegin(), v.cend(), outx.data(), plus<>());

  vector<int> outy(v.size(), 0);
  auto y = acc::partial_sum(v.cbegin(), v.cend(), outy.data(), plus<>());

  using ST = typename vector<unsigned int>::size_type;
  auto sx = static_cast<ST>(x - outx.data());
  auto sy = static_cast<ST>(y - outy.data());

  return sx == outx.size() && sy == outy.size()
    && equal(outx.cbegin(), outx.cend(), outy.cbegin(), outy.cend());
}
