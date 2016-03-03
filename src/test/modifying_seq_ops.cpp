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
