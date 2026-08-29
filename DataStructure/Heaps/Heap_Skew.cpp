#include <bits/stdc++.h>
using namespace std;
#define int long long

// skew heap (mergeable, swaps children on every merge), min-heap
template <size_t N>
struct heap_skew {
  int fa[N], vl[N], ls[N], rs[N];
  int newnode(int x, int i) { return vl[i] = x, i; }
  int top(int x) { return vl[x]; }
  void join(int& x, int y) {
    if (!x || !y) return x |= y, void();
    if (vl[x] > vl[y]) swap(x, y);
    join(rs[x], y), fa[rs[x]] = x, swap(ls[x], rs[x]);
  }
  void decrease_key(int& h, int p, int v) {
    int f = fa[p], &s = f ? ls[f] == p ? ls[f] : rs[f] : h;
    s = fa[p] = 0, vl[p] = v, join(h, p);
  }
  void erase(int& h, int x) {
    decrease_key(h, x, LLONG_MIN);
    int l = ls[h], r = rs[h];
    join(h = l, r), fa[h] = 0;
  }
};
/*
 * ============================================================
 * Name: skew heap (mergeable, swaps children on merge), min-heap
 * Complexity: newnode / top O(1); merge / erase / decrease_key O(log n)
 *             amortized
 * Usage: `heap_skew<N>`: newnode / top / join / decrease_key / erase; heap
 *        identified by its root (h by reference), empty heap = 0.
 * Source: Luogu article "In Praise of the Priority Queue"
 *         (the Luogu blog article "In Praise of the Priority Queue") section 3, wrapped into a struct
 * Notes: with #define int long long the LLONG_MIN sentinel in erase behaves
 *        correctly; self-merge x == y not handled; no rank information
 * ============================================================
 */
