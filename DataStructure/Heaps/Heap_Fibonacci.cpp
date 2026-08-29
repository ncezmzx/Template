#include <bits/stdc++.h>
using namespace std;
#define int long long

// Fibonacci heap (mergeable), min-heap
template <size_t N>
struct heap_fibonacci {
  int a[N], fa[N], vl[N], tp[N], dg[N];
  bool mk[N];  // "lost a child" marks
  list<int> nd[N], sn[N];
  typename list<int>::iterator it[N];
  void add(list<int>& x, int y) { it[y] = x.insert(x.end(), y); }
  void cmin(int& x, int y) { x > y && (x = y); }
  int newnode(int x, int i) { return add(nd[i], i), tp[i] = vl[i] = x, i; }
  int top(int x) { return tp[x]; }
  void join(int x, int y) {  // concatenate root lists
    cmin(tp[x], tp[y]), nd[x].splice(nd[x].end(), nd[y]);
  }
  void decrease_key(int h, int p, int v) {  // cascading cuts
    cmin(tp[h], vl[p] = v);
    int f = fa[p];
    if (!f || vl[f] <= v) return;
    do sn[f].erase(it[p]), --dg[f], add(nd[h], p), fa[p] = 0;
    while ((mk[p = f] ^= true) && (f = fa[p]));
  }
  void erase(int h, int x) {
    decrease_key(h, x, LLONG_MIN);
    for (int y : sn[x]) add(nd[h], y), fa[y] = 0;
    nd[h].erase(it[x]), sn[x].clear();
    int mx = -1;  // consolidate by degree
    for (int x : nd[h]) {
      while (int y = a[dg[x]]) {
        if (vl[x] > vl[y]) swap(x, y);
        add(sn[fa[y] = x], y), a[dg[x]++] = 0;
      }
      mx = max(mx, dg[x]), a[dg[x]] = x;
    }
    nd[h].clear(), tp[h] = LLONG_MAX;
    for (int i = 0; i <= mx; ++i)
      if (int& x = a[i]) add(nd[h], x), cmin(tp[h], vl[x]), x = 0;
  }
};
/*
 * ============================================================
 * Name: Fibonacci heap (mergeable), min-heap
 * Complexity: newnode / top / join O(1) amortized; decrease_key O(1) amortized;
 *             erase O(log n) amortized
 * Usage: `heap_fibonacci<N>`: newnode / top / join / decrease_key / erase;
 *        heaps are identified by their container index.
 * Source: Luogu article "In Praise of the Priority Queue"
 *         (the Luogu blog article "In Praise of the Priority Queue") section 7, wrapped into a struct
 * Notes: mk[] = lost-child marks; tp uses the LLONG_MAX sentinel; empty-heap /
 *        self-merge boundaries not handled
 * ============================================================
 */
