#include <bits/stdc++.h>
using namespace std;
#define int long long


template <size_t N> struct heap_slim {
  int fa[N], vl[N], tp[N], sn[N], bt[N];
  list<int> nd[N];
  typename list<int>::iterator it[N];
  void add(list<int> &x, int y) { it[y] = x.insert(x.end(), y); }
  void cmin(int &x, int y) { x > y && (x = y); }
  int newnode(int x, int i) { return add(nd[i], i), tp[i] = vl[i] = x, i; }
  int top(int x) { return tp[x]; }
  void join(int x, int y) { cmin(tp[x], tp[y]), nd[x].splice(nd[x].end(), nd[y]); }
  void decrease_key(int h, int p, int v) {
    cmin(tp[h], vl[p] = v);
    int f = fa[p];
    if (!f) return;
    (p == sn[f] ? sn[f] : bt[f]) = exchange(bt[p], 0);
    add(nd[h], p), fa[p] = bt[p] = 0;
  }
  int merge(int x, int y) {
    return fa[bt[y] = exchange(sn[fa[y] = x], y)] = y, x;
  }
  void erase(int h, int x) {
    decrease_key(h, x, LLONG_MIN);
    for (int y = sn[x]; y; y = exchange(bt[y], 0)) add(nd[h], y), fa[y] = 0;
    nd[h].erase(it[x]);
    if (nd[h].empty()) return;
    auto ii = nd[h].begin();
    while (true)
      if (ii == prev(nd[h].end()) || vl[*ii] >= vl[*next(ii)])
        if (ii != prev(nd[h].end()) && (ii == nd[h].begin() || vl[*prev(ii)] < vl[*next(ii)]))
          *ii = merge(*next(ii), *ii), nd[h].erase(next(ii));
        else if (ii != nd[h].begin()) *ii = merge(*prev(ii), *ii), nd[h].erase(prev(ii));
        else break;
      else ++ii;
    it[*ii] = ii, tp[h] = vl[*ii];
  }
};

