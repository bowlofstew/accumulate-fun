#include <all.h>

#include <testinator.h>

#include <algorithm>
#include <functional>
#include <iterator>
#include <numeric>
#include <vector>

using namespace std;

// ---------------------------------------------------------------------------
// non-modifying sequence operations

DEF_PROPERTY(ForEach, NonModifyingSeqOps, const vector<unsigned int>& v)
{
  typename vector<unsigned int>::size_type x{};
  acc::for_each(v.cbegin(), v.cend(), [&x] (unsigned int) { ++x; });
  return x == v.size();
}

DEF_PROPERTY(Find, NonModifyingSeqOps, const vector<unsigned int>& v)
{
  unsigned int e = 0;
  if (v.size() > 0) e = v[v.size()/2];

  auto x = find(v.cbegin(), v.cend(), e);
  auto y = acc::find(v.cbegin(), v.cend(), e);
  return x == y;
}

DEF_PROPERTY(FindIf, NonModifyingSeqOps, const vector<unsigned int>& v)
{
  auto even = [] (unsigned int i) { return (i&1) == 0; };
  auto x = find_if(v.cbegin(), v.cend(), even);
  auto y = acc::find_if(v.cbegin(), v.cend(), even);
  return x == y;
}

DEF_PROPERTY(FindIfNot, NonModifyingSeqOps, const vector<unsigned int>& v)
{
  auto even = [] (unsigned int i) { return (i&1) == 0; };
  auto x = find_if_not(v.cbegin(), v.cend(), even);
  auto y = acc::find_if_not(v.cbegin(), v.cend(), even);
  return x == y;
}

DEF_PROPERTY(Mismatch, NonModifyingSeqOps, const vector<unsigned int>& v)
{
  auto w = v;
  if (w.size() > 0) ++w[w.size()/2];

  auto x = mismatch(v.cbegin(), v.cend(), w.cbegin(), equal_to<>());
  auto y = acc::mismatch(v.cbegin(), v.cend(), w.cbegin(), equal_to<>());
  return x == y;
}

DEF_PROPERTY(NoMismatch, NonModifyingSeqOps, const vector<unsigned int>& v)
{
  auto x = mismatch(v.cbegin(), v.cend(), v.cbegin(), equal_to<>());
  auto y = acc::mismatch(v.cbegin(), v.cend(), v.cbegin(), equal_to<>());
  return x == y;
}

DEF_PROPERTY(Mismatch2, NonModifyingSeqOps, const vector<unsigned int>& v)
{
  auto w = v;
  w.erase(w.begin() + w.size()/2, w.cend());

  auto x1 = mismatch(v.cbegin(), v.cend(), w.cbegin(), w.cend(), equal_to<>());
  auto y1 = acc::mismatch(v.cbegin(), v.cend(), w.cbegin(), w.cend(), equal_to<>());

  auto x2 = mismatch(w.cbegin(), w.cend(), v.cbegin(), v.cend(), equal_to<>());
  auto y2 = acc::mismatch(w.cbegin(), w.cend(), v.cbegin(), v.cend(), equal_to<>());

  return x1 == y1 && x2 == y2;
}

DEF_PROPERTY(NoMismatch2, NonModifyingSeqOps, const vector<unsigned int>& v)
{
  auto x = mismatch(v.cbegin(), v.cend(), v.cbegin(), v.cend(), equal_to<>());
  auto y = acc::mismatch(v.cbegin(), v.cend(), v.cbegin(), v.cend(), equal_to<>());
  return x == y;
}

DEF_PROPERTY(Equal, NonModifyingSeqOps, const vector<unsigned int>& v)
{
  return acc::equal(v.cbegin(), v.cend(), v.cbegin(), equal_to<>());
}

DEF_PROPERTY(NotEqual, NonModifyingSeqOps, const vector<unsigned int>& v)
{
  if (v.empty()) return true;

  auto w = v;
  ++w[w.size()/2];

  return !acc::equal(v.cbegin(), v.cend(), w.cbegin(), equal_to<>());
}

DEF_PROPERTY(Equal2, NonModifyingSeqOps, const vector<unsigned int>& v)
{
  return acc::equal(v.cbegin(), v.cend(), v.cbegin(), v.cend(), equal_to<>());
}

DEF_PROPERTY(NotEqual2, NonModifyingSeqOps, const vector<unsigned int>& v)
{
  if (v.empty()) return true;

  auto w = v;
  ++w[w.size()/2];

  return !acc::equal(v.cbegin(), v.cend(), w.cbegin(), w.cend(), equal_to<>());
}

DEF_PROPERTY(NotEqual2Lengths, NonModifyingSeqOps, const vector<unsigned int>& v)
{
  if (v.empty()) return true;

  return !acc::equal(v.cbegin(), v.cend(), v.cbegin(), v.cend()-1, equal_to<>())
    && !acc::equal(v.cbegin(), v.cend()-1, v.cbegin(), v.cend(), equal_to<>());
}

DEF_PROPERTY(FindFirstOf, NonModifyingSeqOps, const vector<unsigned int>& v)
{
  if (v.size() < 3) return true;
  unsigned int w[] = { v[2], v[1] };

  return acc::find_first_of(v.cbegin(), v.cend(), begin(w), end(w), equal_to<>()) == ++v.cbegin();
}

DEF_PROPERTY(FindFirstOfFail, NonModifyingSeqOps, const vector<unsigned int>& v)
{
  if (v.empty()) return true;
  unsigned int i = v[0] + 1;
  while (find(v.cbegin(), v.cend(), i) != v.end()) ++i;
  unsigned int* pi = &i;

  return acc::find_first_of(v.cbegin(), v.cend(), pi, pi+1, equal_to<>()) == v.cend();
}

DEF_PROPERTY(Count, NonModifyingSeqOps, const vector<unsigned int>& v)
{
  auto i = v.empty() ? 0 : v[0];
  auto x = count(v.cbegin(), v.cend(), i);
  auto y = acc::count(v.cbegin(), v.cend(), i);
  return x == y;
}

DEF_PROPERTY(CountIf, NonModifyingSeqOps, const vector<unsigned int>& v)
{
  auto even = [] (unsigned int i) { return (i&1) == 0; };
  auto x = count_if(v.cbegin(), v.cend(), even);
  auto y = acc::count_if(v.cbegin(), v.cend(), even);
  return x == y;
}

DEF_PROPERTY(AdjacentFind, NonModifyingSeqOps, const vector<unsigned int>& v)
{
  auto l = [] (unsigned int i, unsigned int j) { return ((i+j)&1) == 0; };
  auto x = adjacent_find(v.cbegin(), v.cend(), l);
  auto y = acc::adjacent_find(v.cbegin(), v.cend(), l);
  return x == y;
}

DEF_PROPERTY(SearchEmpty, NonModifyingSeqOps, const vector<unsigned int>& v)
{
  vector<unsigned int> s;
  auto x = search(v.cbegin(), v.cend(), s.cbegin(), s.cend(), equal_to<>());
  auto y = acc::search(v.cbegin(), v.cend(), s.cbegin(), s.cend(), equal_to<>());
  return x == y;
}

DEF_PROPERTY(SearchFull, NonModifyingSeqOps, const vector<unsigned int>& v)
{
  auto x = search(v.cbegin(), v.cend(), v.cbegin(), v.cend(), equal_to<>());
  auto y = acc::search(v.cbegin(), v.cend(), v.cbegin(), v.cend(), equal_to<>());
  return x == y;
}

DEF_PROPERTY(SearchNotFound, NonModifyingSeqOps, const vector<unsigned int>& v)
{
  if (v.empty()) return true;
  unsigned int i = v[0] + 1;
  while (find(v.cbegin(), v.cend(), i) != v.end()) ++i;
  unsigned int* pi = &i;

  auto x = search(v.cbegin(), v.cend(), pi, pi+1, equal_to<>());
  auto y = acc::search(v.cbegin(), v.cend(), pi, pi+1, equal_to<>());
  return x == y;
}

DEF_PROPERTY(SearchFound, NonModifyingSeqOps, const vector<unsigned int>& v)
{
  if (v.empty()) return true;
  unsigned int i = v[v.size()/2];
  unsigned int* pi = &i;

  auto x = search(v.cbegin(), v.cend(), pi, pi+1, equal_to<>());
  auto y = acc::search(v.cbegin(), v.cend(), pi, pi+1, equal_to<>());
  return x == y;
}

DEF_PROPERTY(SearchNZero, NonModifyingSeqOps, const vector<unsigned int>& v)
{
  return acc::search_n(v.cbegin(), v.cend(), 0, 0, equal_to<>()) == v.cbegin();
}

DEF_PROPERTY(SearchNOne, NonModifyingSeqOps, const vector<unsigned int>& v)
{
  if (v.empty()) return true;
  return acc::search_n(v.cbegin(), v.cend(), 1, v[0], equal_to<>()) == v.cbegin();
}

DEF_PROPERTY(SearchNTwo, NonModifyingSeqOps, vector<unsigned int> v)
{
  if (v.size() < 3) return true;
  v[2] = v[1];
  return acc::search_n(v.cbegin(), v.cend(), 2, v[1], equal_to<>()) == ++v.cbegin();
}

DEF_PROPERTY(FindIfAll, NonModifyingSeqOps, const vector<unsigned int>& v)
{
  auto even = [] (unsigned int i) { return (i&1) == 0; };

  using I = vector<unsigned int>::const_iterator;
  vector<I> its;
  acc::find_if_all(v.cbegin(), v.cend(), back_inserter(its), even);

  vector<I>::size_type index{};
  for (I i = find_if(v.cbegin(), v.cend(), even);
       i != v.cend();
       i = find_if(++i, v.cend(), even))
  {
    if (its.size() <= index) return false;
    if (its[index++] != i) return false;
  }
  return its.size() == index;
}

DEF_PROPERTY(AdjacentFindAll, NonModifyingSeqOps, const vector<unsigned int>& v)
{
  auto l = [] (unsigned int i, unsigned int j) { return ((i+j)&1) == 0; };

  using I = vector<unsigned int>::const_iterator;
  vector<I> its;
  acc::adjacent_find_all(v.cbegin(), v.cend(), back_inserter(its), l);

  vector<I>::size_type index{};
  for (I i = adjacent_find(v.cbegin(), v.cend(), l);
       i != v.cend();
       i = adjacent_find(++i, v.cend(), l))
  {
    if (its.size() <= index) return false;
    if (its[index++] != i) return false;
  }
  return its.size() == index;
}
