#include <all.h>

#include <testinator.h>

#include <algorithm>
#include <functional>
#include <vector>

using namespace std;

// ---------------------------------------------------------------------------
// set operations

DEF_PROPERTY(Merge, SetOps,
             vector<unsigned int> v, vector<unsigned int> w)
{
  sort(v.begin(), v.end());
  sort(w.begin(), w.end());

  vector<unsigned int> outx(v.size() + w.size(), 0);
  auto x = merge(v.cbegin(), v.cend(), w.cbegin(), w.cend(), outx.data(), less<>{});

  vector<unsigned int> outy(v.size() + w.size(), 0);
  auto y = acc::merge(v.cbegin(), v.cend(), w.cbegin(), w.cend(), outy.data(), less<>{});

  using ST = typename vector<unsigned int>::size_type;
  auto sx = static_cast<ST>(x - outx.data());
  auto sy = static_cast<ST>(y - outy.data());

  return sx == outx.size() && sy == outy.size() && outx == outy;
}

DEF_TEST(SetIntersectionEmpty, SetOps)
{
  vector<unsigned int> v = { 1,2,3 };
  vector<unsigned int> w;

  vector<unsigned int> outx(v.size(), 0);
  auto x = set_intersection(v.cbegin(), v.cend(),
                            w.cbegin(), w.cend(), outx.data(), less<>{});

  vector<unsigned int> outy(v.size(), 0);
  auto y = acc::set_intersection(v.cbegin(), v.cend(),
                                 w.cbegin(), w.cend(), outy.data(), less<>{});

  using ST = typename vector<unsigned int>::size_type;
  auto sx = static_cast<ST>(x - outx.data());
  auto sy = static_cast<ST>(y - outy.data());

  return sx == 0 && sx == sy && outx == outy;
}

DEF_TEST(SetIntersectionEmpty2, SetOps)
{
  vector<unsigned int> v;
  vector<unsigned int> w = { 1,2,3 };

  vector<unsigned int> outx(v.size(), 0);
  auto x = set_intersection(v.cbegin(), v.cend(),
                            w.cbegin(), w.cend(), outx.data(), less<>{});

  vector<unsigned int> outy(v.size(), 0);
  auto y = acc::set_intersection(v.cbegin(), v.cend(),
                                 w.cbegin(), w.cend(), outy.data(), less<>{});

  using ST = typename vector<unsigned int>::size_type;
  auto sx = static_cast<ST>(x - outx.data());
  auto sy = static_cast<ST>(y - outy.data());

  return sx == 0 && sx == sy && outx == outy;
}

DEF_TEST(SetIntersectionEmpty3, SetOps)
{
  vector<unsigned int> v;
  vector<unsigned int> w;

  vector<unsigned int> outx(5, 0);
  auto x = set_intersection(v.cbegin(), v.cend(),
                            w.cbegin(), w.cend(), outx.data(), less<>{});

  vector<unsigned int> outy(5, 0);
  auto y = acc::set_intersection(v.cbegin(), v.cend(),
                                 w.cbegin(), w.cend(), outy.data(), less<>{});

  using ST = typename vector<unsigned int>::size_type;
  auto sx = static_cast<ST>(x - outx.data());
  auto sy = static_cast<ST>(y - outy.data());

  return sx == 0 && sx == sy && outx == outy;
}

DEF_PROPERTY(SetIntersectionSelf, SetOps, vector<unsigned int> v)
{
  if (v.empty()) return true;

  sort(v.begin(), v.end());

  vector<unsigned int> outx(v.size(), 0);
  auto x = set_intersection(v.cbegin(), v.cend(),
                            v.cbegin(), v.cend(), outx.data(), less<>{});

  vector<unsigned int> outy(v.size(), 0);
  auto y = acc::set_intersection(v.cbegin(), v.cend(),
                                 v.cbegin(), v.cend(), outy.data(), less<>{});

  using ST = typename vector<unsigned int>::size_type;
  auto sx = static_cast<ST>(x - outx.data());
  auto sy = static_cast<ST>(y - outy.data());

  return sx == sy && outx == outy
    && equal(outx.cbegin(), outx.cbegin() + sx, v.cbegin(), v.cend());
}

DEF_TEST(SetIntersection, SetOps)
{
  vector<unsigned int> v = { 0,1,2,3,4,5,6 };
  vector<unsigned int> w = { 1,2,2,3,3 };

  vector<unsigned int> outx(v.size(), 0);
  auto x = set_intersection(v.cbegin(), v.cend(),
                            w.cbegin(), w.cend(), outx.data(), less<>{});

  vector<unsigned int> outy(v.size(), 0);
  auto y = acc::set_intersection(v.cbegin(), v.cend(),
                                 w.cbegin(), w.cend(), outy.data(), less<>{});

  using ST = typename vector<unsigned int>::size_type;
  auto sx = static_cast<ST>(x - outx.data());
  auto sy = static_cast<ST>(y - outy.data());

  return sx == sy && outx == outy;
}

DEF_TEST(SetIntersection2, SetOps)
{
  vector<unsigned int> v = { 1,2,2,2,3,4,5 };
  vector<unsigned int> w = { 0,2,2,3,3,6,7 };

  vector<unsigned int> outx(v.size() + w.size(), 0);
  auto x = set_intersection(v.cbegin(), v.cend(),
                            w.cbegin(), w.cend(), outx.data(), less<>{});

  vector<unsigned int> outy(v.size() + w.size(), 0);
  auto y = acc::set_intersection(v.cbegin(), v.cend(),
                                 w.cbegin(), w.cend(), outy.data(), less<>{});

  using ST = typename vector<unsigned int>::size_type;
  auto sx = static_cast<ST>(x - outx.data());
  auto sy = static_cast<ST>(y - outy.data());

  return sx == sy && outx == outy;
}

