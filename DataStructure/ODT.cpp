#include <bits/stdc++.h>
using namespace std;

// Chtholly tree (ODT): map-based interval assignment
struct odt_tree {
  map<int, int> seg;  // seg[l] = value on [l, next_l)
  auto split(int p) {  // cut at p, return iterator to the segment starting at p
    auto it = prev(seg.upper_bound(p));
    return seg.insert(it, {p, it->second});
  }
  void assign(int l, int r, int x) {  // a[l..r] = x
    auto itl = split(l), itr = split(++r);
    seg.erase(itl, itr), seg[l] = x;
  }
  int query() {  // sum of value * segment length over [first, last)
    int t = 0;
    for (auto it = seg.begin(); next(it) != seg.end(); ++it)
      t += it->second * (next(it)->first - it->first);
    return t;
  }
};
/*
 * ============================================================
 * Name: Chtholly tree / ODT (std::map-based interval assignment)
 * Complexity: amortized O(n log n) on random data; best for assignment-
 *             dominated workloads
 * Usage: interval assignment plus per-segment statistics (e.g. sum of value *
 *        length), `odt_tree`:
 *        seed sentinel segments first (seg[1] = v, seg[n + 1] = 0), then split
 *        / assign / query.
 * Source: all.cpp lines 46715-46741 (wrapped into a struct, logic unchanged)
 * Notes: the amortized bound assumes random data; degenerate inputs degrade to
 *        brute force
 * ============================================================
 */
