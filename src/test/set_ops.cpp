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

  return sx == outx.size() && sy == outy.size()
    && equal(outx.cbegin(), outx.cend(), outy.cbegin(), outy.cend());
}
