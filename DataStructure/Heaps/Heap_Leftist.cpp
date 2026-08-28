#include <bits/stdc++.h>
using namespace std;

// leftist heap (mergeable, keyed by dist d), min-heap
template <size_t N>
struct heap_leftist {
  int fa[N], vl[N], d[N], ls[N], rs[N];
  int newnode(int x, int i) { return vl[i] = x, i; }
  int top(int x) { return vl[x]; }
  void join(int& x, int y) {
    if (!x || !y) return x |= y, void();
    if (vl[x] > vl[y]) swap(x, y);
    join(rs[x], y), fa[rs[x]] = x;
    if (d[ls[x]] > d[rs[x]]) swap(ls[x], rs[x]);
    d[x] = d[rs[x]] + 1;
  }
  void erase(int& h, int x) {
    int f = fa[x], l = ls[x], r = rs[x];
    int& s = f ? ls[f] == x ? ls[f] : rs[f] : h;
    join(s = l, r), fa[s] = f;
    while (f) {
      if (d[ls[f]] > d[rs[f]]) swap(ls[f], rs[f]);
      int nd = d[rs[f]] + 1;
      if (d[f] == nd) break;
      d[f] = nd, f = fa[f];
    }
  }
  void decrease_key(int& h, int p, int v) {
    erase(h, p), vl[p] = v, ls[p] = rs[p] = fa[p] = d[p] = 0, join(h, p);
  }
};
/*
 * ============================================================
 * Name: leftist heap (mergeable, dist-based merging), min-heap
 * Complexity: join/top O(1); merge/erase/decrease_key O(log n) worst case
 * Usage: newnode/top/join/decrease_key/erase, wrapped as heap_leftist<N>;
 *        a heap is identified by its root node (h passed by reference), empty heap = 0
 * Source: Luogu article "In Praise of the Priority Queue"
 *         (the Luogu blog article "In Praise of the Priority Queue") section 2, wrapped into a struct
 * Notes: arrays fa/vl/d/ls/rs inside the struct, size N as needed; join uses
 *        x|=y for empty heaps; self-merge x==y not handled; erase repairs d
 *        along the fa chain
 * ============================================================
 * Example (uncomment to compile):
 * static heap_leftist<1009> hp;
 * signed main() {
 *   int h = hp.newnode(5, 1);
 *   hp.join(h, hp.newnode(3, 2));
 *   hp.join(h, hp.newnode(8, 3));
 *   hp.join(h, hp.newnode(1, 4));
 *   hp.join(h, hp.newnode(7, 5));
 *   cout << hp.top(h) << '\n';      // 1
 *   hp.decrease_key(h, 2, 0);       // 3 -> 0
 *   cout << hp.top(h) << '\n';      // 0
 *   hp.erase(h, 2);                 // remove the 0
 *   cout << hp.top(h) << '\n';      // 1
 * }
 * ============================================================
 */
