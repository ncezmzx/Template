#include <bits/stdc++.h>
using namespace std;
#define int long long

// lazy binomial heap (mergeable, O(1) join), min-heap
template <size_t N> struct heap_lazy_binomial {
  int a[N], fa[N], vl[N], tp[N], rk[N], pos[N], msk[N];
  list<int> nd[N], sn[N];
  typename list<int>::iterator it[N];
  void add(list<int> &x, int y) { it[y] = x.insert(x.end(), y); }
  void cmin(int &x, int y) { x > y && (x = y); }
  int newnode(int x, int i) { return add(nd[i], i), tp[i] = vl[i] = x, pos[i] = msk[i] = i; }
  int top(int x) { return tp[x]; }
  void join(int x, int y) { // lazy: concatenate root lists only
    cmin(tp[x], tp[y]), nd[x].splice(nd[x].end(), nd[y]);
  }
  void decrease_key(int h, int p, int v) { // bubble up swapping masked values
    cmin(tp[h], vl[p = pos[p]] = v);
    for (int f = fa[p]; f && vl[f] > v; f = fa[p = f])
      swap(msk[p], msk[f]), swap(pos[msk[p]], pos[msk[f]]), swap(vl[p], vl[f]);
  }
  void erase(int h, int x) {
    decrease_key(h, x, LLONG_MIN);
    for (int y : sn[x = pos[x]]) add(nd[h], y), fa[y] = 0;
    nd[h].erase(it[x]), sn[x].clear();
    int mx = -1; // consolidate by rank
    for (int x : nd[h]) {
      while (int y = a[rk[x]]) {
        if (vl[x] > vl[y]) swap(x, y);
        add(sn[fa[y] = x], y), a[rk[x]++] = 0;
      }
      mx = max(mx, rk[x]), a[rk[x]] = x;
    }
    nd[h].clear(), tp[h] = LLONG_MAX;
    for (int i = 0; i <= mx; ++i)
      if (int &x = a[i]) add(nd[h], x), cmin(tp[h], vl[x]), x = 0;
  }
};
/*
 * ============================================================
 * Name: lazy binomial heap (mergeable), min-heap
 * Complexity: newnode / top / join O(1) amortized; decrease_key / erase O(log
 *             n) amortized
 * Usage: `heap_lazy_binomial<N>`: newnode / top / join / decrease_key / erase;
 *        heaps are identified by their container index.
 * Source: Luogu article "In Praise of the Priority Queue"
 *         (the Luogu blog article "In Praise of the Priority Queue") section 11, wrapped into a struct
 * Notes: join only concatenates root lists (lazy, the binomial structure is not
 *        maintained immediately); decrease_key bubbles up swapping msk / pos /
 *        vl; erase uses the LLONG_MIN sentinel then re-consolidates; tp uses
 *        the LLONG_MAX sentinel; empty-heap / self-merge not handled
 * ============================================================
 */
