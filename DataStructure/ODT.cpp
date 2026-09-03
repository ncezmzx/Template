#include <bits/stdc++.h>
using namespace std;


struct odt_tree {
  map<int, int> seg;
  auto split(int p) {
    auto it = prev(seg.upper_bound(p));
    return seg.insert(it, {p, it->second});
  }
  void assign(int l, int r, int x) {
    auto itl = split(l), itr = split(++r);
    seg.erase(itl, itr), seg[l] = x;
  }
  int query() {
    int t = 0;
    for (auto it = seg.begin(); next(it) != seg.end(); ++it) t += it->second * (next(it)->first - it->first);
    return t;
  }
};

