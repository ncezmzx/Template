#include <bits/stdc++.h>
using namespace std;
#define int long long

// rank-pairing heap, type A (half-tree form), mergeable min-heap
template <size_t N>
struct heap_rank_pairing_a {
  int a[N], fa[N], vl[N], tp[N], rk[N], ls[N], rs[N];
  list<int> nd[N];
  typename list<int>::iterator it[N];
  void add(list<int>& x, int y) { it[y] = x.insert(x.end(), y); }
  void cmin(int& x, int y) { x > y && (x = y); }
  int newnode(int x, int i) { return add(nd[i], i), tp[i] = vl[i] = x, i; }
  int top(int x) { return tp[x]; }
  void join(int x, int y) {
    cmin(tp[x], tp[y]), nd[x].splice(nd[x].end(), nd[y]);
  }
  int getrk(int x) { return x ? rk[x] : -1; }
  void decrease_key(int h, int p, int v) {
    cmin(tp[h], vl[p] = v);
    int f = fa[p];
    if (!f || (p == ls[f] && vl[f] <= v)) return;
    (p == ls[f] ? ls[f] : rs[f]) = exchange(rs[p], 0);
    add(nd[h], p), fa[p] = 0, rk[p] = getrk(ls[p]) + 1;
    while (f) {  // repair ranks upward
      int rkf = getrk(f), rkl = getrk(ls[f]), rkr = getrk(rs[f]);
      if (rkf == rkl || (rkf == rkl + 1 && rkr == rkl)) break;
      rk[f] = rkr == rkl ? rkl + 1 : rkl, f = fa[f];
    }
  }
  void erase(int h, int x) {
    decrease_key(h, x, LLONG_MIN);
    for (int y = ls[x]; y; y = exchange(rs[y], 0)) add(nd[h], y), fa[y] = 0;
    nd[h].erase(it[x]);
    int mx = -1;
    for (int x : nd[h]) {
      while (int y = a[rk[x]]) {
        if (vl[x] > vl[y]) swap(x, y);
        fa[rs[y] = exchange(ls[fa[y] = x], y)] = y, a[rk[x]++] = 0;
      }
      mx = max(mx, rk[x]), a[rk[x]] = x;
    }
    nd[h].clear(), tp[h] = LLONG_MAX;
    for (int i = 0; i <= mx; ++i)
      if (int& x = a[i]) add(nd[h], x), cmin(tp[h], vl[x]), x = 0;
  }
};
/*
 * ============================================================
 * Name: rank-pairing heap type A (half-tree form), mergeable min-heap
 * Complexity: newnode / top / join O(1) amortized; decrease_key / erase O(log
 *             n) amortized
 * Usage: `heap_rank_pairing_a<N>`: newnode / top / join / decrease_key / erase;
 *        heaps are identified by their container index.
 * Source: Luogu article "In Praise of the Priority Queue"
 *         (the Luogu blog article "In Praise of the Priority Queue") section 6 (type A + half-tree), wrapped into a struct
 * Notes: newnode only registers into the root list nd[] (no parent-child tree
 *        until decrease_key).
 *        KNOWN DEFECTS (from the source article): erase-rebuild fa semantics
 *        conflict with decrease_key unlinking, non-first-child unlink is wrong,
 *        erase may leave LLONG_MIN ghost nodes. Reference only, do not use in
 *        contests
 * ============================================================
 */
