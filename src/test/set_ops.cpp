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

DEF_PROPERTY(Includes, SetOps, vector<unsigned int> v, unsigned long int i)
{
  if (v.empty()) return true;

  std::mt19937 g(i);
  std::uniform_int_distribution<int> dis(0, v.size()-1);

  vector<unsigned int> w;
  copy(v.begin(), v.begin()+dis(g), back_inserter(w));

  sort(v.begin(), v.end());
  sort(w.begin(), w.end());

  return acc::includes(v.cbegin(), v.cend(), w.cbegin(), w.cend(), std::less<>{});
}

DEF_PROPERTY(SetDifferenceSelf, SetOps, vector<unsigned int> v)
{
  if (v.empty()) return true;

  sort(v.begin(), v.end());

  vector<unsigned int> outx;
  acc::set_difference(v.cbegin(), v.cend(),
                      v.cbegin(), v.cend(), back_inserter(outx), less<>{});
  return outx.empty();
}

DEF_PROPERTY(SetDifference, SetOps, vector<unsigned int> v)
{
  if (v.empty()) return true;

  sort(v.begin(), v.end());

  auto even = [] (unsigned int i) { return (i&1) == 0; };
  vector<unsigned int> w;
  copy_if(v.cbegin(), v.cend(), back_inserter(w), even);

  vector<unsigned int> x;
  acc::set_difference(v.cbegin(), v.cend(),
                      w.cbegin(), w.cend(), back_inserter(x), less<>{});

  vector<unsigned int> y;
  merge(w.cbegin(), w.cend(), x.cbegin(), x.cend(), back_inserter(y));
  return y == v;
}

DEF_PROPERTY(SetSymmetricDifferenceSelf, SetOps, vector<unsigned int> v)
{
  if (v.empty()) return true;

  sort(v.begin(), v.end());

  vector<unsigned int> outx;
  acc::set_symmetric_difference(v.cbegin(), v.cend(),
                                v.cbegin(), v.cend(), back_inserter(outx), less<>{});
  return outx.empty();
}

DEF_PROPERTY(SetSymmetricDifference, SetOps, vector<unsigned int> v)
{
  if (v.empty()) return true;

  sort(v.begin(), v.end());

  auto even = [] (unsigned int i) { return (i&1) == 0; };
  vector<unsigned int> w;
  copy_if(v.cbegin(), v.cend(), back_inserter(w), even);

  auto odd = [] (unsigned int i) { return (i&1) != 0; };
  vector<unsigned int> x;
  copy_if(v.cbegin(), v.cend(), back_inserter(x), odd);

  vector<unsigned int> y;
  acc::set_symmetric_difference(w.cbegin(), w.cend(),
                                x.cbegin(), x.cend(), back_inserter(y), less<>{});

  return y == v;
}

DEF_TEST(SetSymmetricDifferenceExample, SetOps)
{
  vector<int> v1{1,2,3,4,5,6,7,8     };
  vector<int> v2{        5,  7,  9,10};

  vector<int> v_symDiff;
  acc::set_symmetric_difference(
      v1.cbegin(), v1.cend(),
      v2.cbegin(), v2.cend(),
      back_inserter(v_symDiff), less<>{});

  vector<int> v_expected{1,2,3,4,6,8,9,10};

  return v_symDiff == v_expected;
}

DEF_PROPERTY(SetUnionSelf, SetOps, vector<unsigned int> v)
{
  if (v.empty()) return true;

  sort(v.begin(), v.end());

  vector<unsigned int> outx;
  acc::set_union(v.cbegin(), v.cend(),
                 v.cbegin(), v.cend(), back_inserter(outx), less<>{});
  return outx == v;
}

DEF_PROPERTY(SetUnionDisjoint, SetOps, vector<unsigned int> v)
{
  if (v.empty()) return true;

  sort(v.begin(), v.end());
  unique(v.begin(), v.end());

  auto even = [] (unsigned int i) { return (i&1) == 0; };
  vector<unsigned int> w;
  copy_if(v.cbegin(), v.cend(), back_inserter(w), even);

  auto odd = [] (unsigned int i) { return (i&1) != 0; };
  vector<unsigned int> x;
  copy_if(v.cbegin(), v.cend(), back_inserter(x), odd);

  vector<unsigned int> y;
  acc::set_union(w.cbegin(), w.cend(),
                 x.cbegin(), x.cend(), back_inserter(y), less<>{});

  return y == v;
}

DEF_PROPERTY(SetUnionRandom, SetOps, vector<unsigned int> v, unsigned long int i)
{
  if (v.empty()) return true;

  sort(v.begin(), v.end());
  unique(v.begin(), v.end());

  std::mt19937 g(i);
  std::uniform_int_distribution<int> dis(0, 63);

  auto select = [&] (unsigned int) { return (dis(g)&1) == 0; };

  vector<unsigned int> w;
  copy_if(v.cbegin(), v.cend(), back_inserter(w), select);

  vector<unsigned int> x;
  copy_if(v.cbegin(), v.cend(), back_inserter(x), select);

  vector<unsigned int> y;
  acc::set_union(w.cbegin(), w.cend(),
                 x.cbegin(), x.cend(), back_inserter(y), less<>{});

  vector<unsigned int> z;
  set_union(w.cbegin(), w.cend(),
            x.cbegin(), x.cend(), back_inserter(z), less<>{});

  return y == z;
}

DEF_TEST(SetUnionRepeated1, SetOps)
{
  vector<int> v1 = {1, 2, 3, 4, 5, 5};
  vector<int> v2 = {      3, 4, 5, 5, 5, 6, 7};
  vector<int> dest;

  acc::set_union(v1.begin(), v1.end(),
                 v2.begin(), v2.end(),
                 back_inserter(dest), std::less<>{});

  vector<int> v_expected{1,2,3,4,5,5,5,6,7};

  return dest == v_expected;
}

DEF_TEST(SetUnionRepeated2, SetOps)
{
  vector<int> v2 = {      3, 4, 5, 5, 5, 6, 7};
  vector<int> v1 = {1, 2, 3, 4, 5, 5};
  vector<int> dest;

  acc::set_union(v1.begin(), v1.end(),
                 v2.begin(), v2.end(),
                 back_inserter(dest), std::less<>{});

  vector<int> v_expected{1,2,3,4,5,5,5,6,7};

  return dest == v_expected;
}

DEF_TEST(MergeRepeated1, SetOps)
{
  vector<int> v1 = {1, 2, 3, 4, 5, 5};
  vector<int> v2 = {      3, 4, 5, 5, 5, 6, 7};
  vector<int> dest;

  acc::merge(v1.begin(), v1.end(),
             v2.begin(), v2.end(),
             back_inserter(dest), std::less<>{});

  vector<int> v_expected{1,2,3,3,4,4,5,5,5,5,5,6,7};

  return dest == v_expected;
}

DEF_TEST(MergeRepeated2, SetOps)
{
  vector<int> v2 = {      3, 4, 5, 5, 5, 6, 7};
  vector<int> v1 = {1, 2, 3, 4, 5, 5};
  vector<int> dest;

  acc::merge(v1.begin(), v1.end(),
             v2.begin(), v2.end(),
             back_inserter(dest), std::less<>{});

  vector<int> v_expected{1,2,3,3,4,4,5,5,5,5,5,6,7};

  return dest == v_expected;
}
