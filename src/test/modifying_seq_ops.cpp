#include <all.h>

#include <testinator.h>

#include <algorithm>
#include <functional>
#include <vector>

using namespace std;

// ---------------------------------------------------------------------------
// modifying sequence operations

DEF_PROPERTY(Copy, ModifyingSeqOps, const vector<unsigned int>& v)
{
  vector<unsigned int> outx(v.size(), 0);
  auto x = copy(v.cbegin(), v.cend(), outx.data());

  vector<unsigned int> outy(v.size(), 0);
  auto y = acc::copy(v.cbegin(), v.cend(), outy.data());

  using ST = typename vector<unsigned int>::size_type;
  auto sx = static_cast<ST>(x - outx.data());
  auto sy = static_cast<ST>(y - outy.data());

  return sx == outx.size() && sy == outy.size()
    && equal(outx.cbegin(), outx.cend(), outy.cbegin(), outy.cend());
}

DEF_PROPERTY(CopyIf, ModifyingSeqOps, const vector<unsigned int>& v)
{
  auto even = [] (unsigned int i) { return (i&1) == 0; };

  vector<unsigned int> outx(v.size(), 0);
  auto x = copy_if(v.cbegin(), v.cend(), outx.data(), even);

  vector<unsigned int> outy(v.size(), 0);
  auto y = acc::copy_if(v.cbegin(), v.cend(), outy.data(), even);

  auto sx = x - outx.data();
  auto sy = y - outy.data();

  return sx == sy
    && equal(outx.cbegin(), outx.cend(), outy.cbegin(), outy.cend());
}

DEF_PROPERTY(CopyN, ModifyingSeqOps, const vector<unsigned int>& v)
{
  vector<unsigned int> outx(v.size(), 0);
  auto x = copy_n(v.cbegin(), v.size(), outx.data());

  vector<unsigned int> outy(v.size(), 0);
  auto y = acc::copy_n(v.cbegin(), v.size(), outy.data());

  using ST = typename vector<unsigned int>::size_type;
  auto sx = static_cast<ST>(x - outx.data());
  auto sy = static_cast<ST>(y - outy.data());

  return sx == outx.size() && sy == outy.size()
    && equal(outx.cbegin(), outx.cend(), outy.cbegin(), outy.cend());
}

DEF_PROPERTY(CopyBackward, ModifyingSeqOps, const vector<unsigned int>& v)
{
  vector<unsigned int> outx(v.size(), 0);
  auto x = copy_backward(v.cbegin(), v.cend(), outx.end());

  vector<unsigned int> outy(v.size(), 0);
  auto y = acc::copy_backward(v.cbegin(), v.cend(), outy.end());

  return x == outx.begin() && y == outy.begin()
    && equal(outx.cbegin(), outx.cend(), outy.cbegin(), outy.cend());
}

DEF_TEST(Move, ModifyingSeqOps)
{
  vector<unique_ptr<int>> v;
  v.push_back(make_unique<int>(0));
  v.push_back(make_unique<int>(1));
  v.push_back(make_unique<int>(2));

  vector<unique_ptr<int>> w;
  acc::move(v.begin(), v.end(), back_inserter(w));
  return w.size() == 3
    && *w[0] == 0 && *w[1] == 1 && *w[2] == 2;
}

DEF_TEST(MoveBackward, ModifyingSeqOps)
{
  vector<unique_ptr<int>> v;
  v.push_back(make_unique<int>(0));
  v.push_back(make_unique<int>(1));
  v.push_back(make_unique<int>(2));

  vector<unique_ptr<int>> w;
  w.push_back(make_unique<int>(3));
  w.push_back(make_unique<int>(4));
  w.push_back(make_unique<int>(5));

  acc::move_backward(v.begin(), v.end(), w.end());
  return *w[0] == 0 && *w[1] == 1 && *w[2] == 2;
}

DEF_PROPERTY(Fill, ModifyingSeqOps, unsigned int i)
{
  vector<unsigned int> x(5, 0);
  acc::fill(x.begin(), x.end(), i);

  vector<unsigned int> y{i, i, i, i, i};
  return x == y;
}

DEF_PROPERTY(FillN, ModifyingSeqOps, unsigned int i)
{
  vector<unsigned int> x(5, 0);
  auto e = acc::fill_n(x.begin(), 5, i);

  vector<unsigned int> y{i, i, i, i, i};
  return e == x.end() && x == y;
}

DEF_TEST(Generate, ModifyingSeqOps)
{
  int count = 5;
  vector<unsigned int> x(count, 0);
  acc::generate(x.begin(), x.end(), [&count] () { return count--;});

  vector<unsigned int> y{5,4,3,2,1};
  return count == 0 && x == y;
}

DEF_TEST(GenerateN, ModifyingSeqOps)
{
  int count = 5;
  vector<unsigned int> x(count, 0);
  acc::generate_n(x.begin(), count, [&count] () { return count--;});

  vector<unsigned int> y{5,4,3,2,1};
  return count == 0 && x == y;
}

DEF_PROPERTY(Remove, ModifyingSeqOps, vector<unsigned int> v)
{
  if (v.empty()) return true;

  unsigned int i = v[v.size()/2];

  v.erase(acc::remove(v.begin(), v.end(), i), v.end());
  return find(v.cbegin(), v.cend(), i) == v.cend();
}

DEF_PROPERTY(RemoveIf, ModifyingSeqOps, vector<unsigned int> v)
{
  auto even = [] (unsigned int i) { return (i&1) == 0; };
  v.erase(acc::remove_if(v.begin(), v.end(), even), v.end());
  return none_of(v.cbegin(), v.cend(), even);
}

DEF_PROPERTY(Replace, ModifyingSeqOps, vector<unsigned int> v)
{
  if (v.empty()) return true;
  unsigned int i = v[v.size()/2];

  acc::replace(v.begin(), v.end(), i, i+1);
  return find(v.cbegin(), v.cend(), i) == v.cend();
}

DEF_PROPERTY(ReplaceIf, ModifyingSeqOps, vector<unsigned int> v)
{
  auto even = [] (unsigned int i) { return (i&1) == 0; };
  unsigned int i = 1;
  while (find(v.cbegin(), v.cend(), i) != v.end()) i+=2;

  acc::replace_if(v.begin(), v.end(), even, i);
  return none_of(v.cbegin(), v.cend(), even);
}

DEF_PROPERTY(SwapRanges, ModifyingSeqOps,
             const vector<unsigned int>& v, const vector<unsigned int>& w)
{
  vector<unsigned int> v2{v};
  vector<unsigned int> w2{w};

  auto x = acc::swap_ranges(v2.begin(), v2.end(), w2.begin());
  return x == w2.end() && w2 == v && v2 == w;
}

DEF_PROPERTY(Transform, ModifyingSeqOps, const vector<unsigned int>& v)
{
  auto add1 = [] (unsigned int i) { return i+1; };

  vector<unsigned int> outx(v.size(), 0);
  auto x = transform(v.cbegin(), v.cend(), outx.data(), add1);

  vector<unsigned int> outy(v.size(), 0);
  auto y = acc::transform(v.cbegin(), v.cend(), outy.data(), add1);

  using ST = typename vector<unsigned int>::size_type;
  auto sx = static_cast<ST>(x - outx.data());
  auto sy = static_cast<ST>(y - outy.data());

  return sx == outx.size() && sy == outy.size()
    && equal(outx.cbegin(), outx.cend(), outy.cbegin(), outy.cend());
}

DEF_PROPERTY(Transform2, ModifyingSeqOps, const vector<unsigned int>& v)
{
  vector<unsigned int> outx(v.size(), 0);
  auto x = transform(v.cbegin(), v.cend(), v.cbegin(), outx.data(), plus<>());

  vector<unsigned int> outy(v.size(), 0);
  auto y = acc::transform(v.cbegin(), v.cend(), v.cbegin(), outy.data(), plus<>());

  using ST = typename vector<unsigned int>::size_type;
  auto sx = static_cast<ST>(x - outx.data());
  auto sy = static_cast<ST>(y - outy.data());

  return sx == outx.size() && sy == outy.size()
    && equal(outx.cbegin(), outx.cend(), outy.cbegin(), outy.cend());
}

DEF_PROPERTY(ReverseCopy, ModifyingSeqOps, const vector<unsigned int>& v)
{
  vector<unsigned int> outx(v.size(), 0);
  auto x = reverse_copy(v.cbegin(), v.cend(), outx.data());

  vector<unsigned int> outy(v.size(), 0);
  auto y = acc::reverse_copy(v.cbegin(), v.cend(), outy.data());

  using ST = typename vector<unsigned int>::size_type;
  auto sx = static_cast<ST>(x - outx.data());
  auto sy = static_cast<ST>(y - outy.data());

  return sx == outx.size() && sy == outy.size()
    && equal(outx.cbegin(), outx.cend(), outy.cbegin(), outy.cend());
}