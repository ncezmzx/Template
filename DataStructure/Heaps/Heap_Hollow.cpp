#include <bits/stdc++.h>
using namespace std;
#define int long long

// hollow heap (mergeable; decrease_key leaves hollowed old nodes), min-heap
template <size_t N> struct heap_hollow {
  struct node {
    int vl, sn, bt, af, rk, ps;
  } nd[N];
  int pos[N], a[N], tot;
  int newnode(int w, int p) { return nd[++tot] = {w, 0, 0, 0, 0, p}, pos[p] = tot; }
  void join(int &u, int v) {
    if (nd[u].vl > nd[v].vl) swap(u, v);
    nd[v].bt = exchange(nd[u].sn, v);
  }
  void merge(int &u, int v) {
    if (!u) u = v;
    else if (v) join(u, v);
  }
  int top(int u) { return nd[u].vl; }
  void decrease_key(int &u, int x, int v) { // hollow the old node, insert a fresh one
    int y = pos[x];
    if (u == y) return nd[u].vl = v, void();
    int p = newnode(v, x);
    nd[y].ps = 0, nd[p].rk = max(nd[y].rk - 2, 0ll);
    return nd[p].sn = y, nd[y].af = p, join(u, p);
  }
  void erase(int &u, int x) {
    int y = exchange(pos[x], 0), mx = 0;
    if (nd[y].ps = 0, nd[u].ps) return;
    nd[u].bt = 0;
    while (u) { // sweep: drop hollow nodes, bucket the rest by rank
      int w = nd[u].sn, v = exchange(u, nd[u].bt);
      while (w) {
        int z = exchange(w, nd[w].bt);
        if (!nd[z].ps) {
          if (int &f = nd[z].af) (exchange(f, 0) == v ? w : nd[z].bt) = 0;
          else nd[z].bt = exchange(u, z);
        }
        else {
          int t = nd[z].rk;
          while (int &s = a[t]) join(z, exchange(s, 0)), nd[z].rk = ++t;
          a[t] = z, mx = max(mx, t);
        }
      }
    }
    for (int i = 0; i <= mx; ++i) merge(u, a[i]);
    memset(a, 0, (mx + 1) * sizeof(int));
  }
};
/*
 * ============================================================
 * Name: hollow heap (mergeable), min-heap
 * Complexity: newnode / top / join O(1) amortized; decrease_key O(1) amortized;
 *             erase O(log n) amortized
 * Usage: `heap_hollow<N>`: newnode / top / join / decrease_key / erase; nodes
 *        live in the nd[] pool, pos[] maps external id -> node.
 * Source: Luogu article "In Praise of the Priority Queue"
 *         (the Luogu blog article "In Praise of the Priority Queue") section 8, wrapped into a struct
 * Notes: node pool nd[N] (N = 2e6 recommended); tot allocates without recycling
 *        (each newnode creates a node); ps marks hollow nodes (invalidated by
 *        decrease_key); decrease_key edits the root in place when u == y
 * ============================================================
 */
