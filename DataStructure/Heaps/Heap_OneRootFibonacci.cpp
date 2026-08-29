#include <bits/stdc++.h>
using namespace std;
#define int long long

// one-root Fibonacci heap (single-root representation), mergeable min-heap
template <size_t N>
struct heap_one_root_fibonacci {
  int a[N], fa[N], vl[N], frm[N], rk[N], sn[N], bt[N];
  bool mk[N];  // lost-child marks
  int newnode(int x, int i) { return vl[i] = x, i; }
  int top(int x) { return vl[x]; }
  void join(int& x, int y) {  // hang y under the single root x
    if (vl[x] > vl[y]) swap(x, y);
    if (int z = bt[y] = exchange(sn[frm[y] = fa[y] = x], y)) frm[z] = y;
  }
  void decrease_key(int& h, int p, int v) {  // cascading cuts
    int f = fa[p];
    if (vl[p] = v, !f || vl[f] <= v) return;
    int g = frm[p], z = bt[p];
    if (((sn[g] == p ? sn[g] : bt[g]) = z)) frm[z] = g;
    do rk[f] && --rk[f];
    while (!(mk[f] ^= true) && (f = fa[f]));
    frm[p] = fa[p] = bt[p] = 0, mk[p] = false, join(h, p);
  }
  void erase(int& h, int x) {
    decrease_key(h, x, LLONG_MIN);
    int mx = -1, y = sn[x];
    while (y) {  // detach children, re-consolidate by rank
      int x = exchange(y, exchange(bt[y], 0));
      fa[x] = frm[x] = 0;
      while (int y = a[rk[x]]) join(x, y), a[rk[x]++] = 0;
      mx = max(mx, rk[x]), a[rk[x]] = x;
    }
    h = 0;
    for (int i = 0; i <= mx; ++i)
      if (int x = exchange(a[i], 0)) {
        if (!h)
          h = x;
        else
          join(h, x);
      }
  }
};
/*
 * ============================================================
 * Name: one-root Fibonacci heap (single-root form), mergeable min-heap
 * Complexity: newnode / top / join O(1) amortized; decrease_key O(1) amortized;
 *             erase O(log n) amortized
 * Usage: `heap_one_root_fibonacci<N>`: newnode / top / join / decrease_key /
 *        erase; the heap always has one root h (join hangs y directly under x's
 *        child list).
 * Source: Luogu article "In Praise of the Priority Queue"
 *         (the Luogu blog article "In Praise of the Priority Queue") section 12, wrapped into a struct
 * Notes: frm[] = previous sibling (sibling chain via bt); mk[] lost marks;
 *        erase decreases to LLONG_MIN, detaches all children and rebuilds the
 *        root by rank buckets; h by reference (empty = 0, but join does not
 *        handle x / y = 0)
 * ============================================================
 */
