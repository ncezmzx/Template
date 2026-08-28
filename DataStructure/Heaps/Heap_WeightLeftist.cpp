#include <bits/stdc++.h>
using namespace std;

// weighted leftist heap (merges by subtree size), mergeable min-heap
template <size_t N>
struct heap_weight_leftist {
  int fa[N], vl[N], sz[N], ls[N], rs[N];
  void cmin(int& x, int y) { x > y && (x = y); }
  int newnode(int x, int i) { return vl[i] = x, sz[i] = 1, i; }
  int top(int x) { return vl[x]; }
  void join(int& x, int y) {  // attach to the lighter side (by sz)
    if (!x || !y) return x |= y, void();
    if (vl[x] > vl[y]) swap(x, y);
    int& s = sz[ls[x]] > sz[rs[x]] ? rs[x] : ls[x];
    sz[x] += sz[y], join(s, y), fa[s] = x;
  }
  void erase(int& h, int x) {
    int f = fa[x], l = ls[x], r = rs[x];
    int& s = f ? ls[f] == x ? ls[f] : rs[f] : h;
    join(s = l, r), fa[s] = f;
  }
  void decrease_key(int& h, int p, int v) {
    erase(h, p), vl[p] = v, ls[p] = rs[p] = fa[p] = 0, sz[p] = 1, join(h, p);
  }
};
/*
 * ============================================================
 * Name: weighted leftist heap (merges by subtree size sz), mergeable min-heap
 * Complexity: newnode/top O(1); merge/erase/decrease_key O(log n) worst case
 *             (sz guarantees logarithmic depth)
 * Usage: newnode/top/join/decrease_key/erase, wrapped as heap_weight_leftist<N>;
 *        heap identified by its root (h by reference)
 * Source: Luogu article "In Praise of the Priority Queue"
 *         (the Luogu blog article "In Praise of the Priority Queue") section 15, wrapped into a struct
 * Notes: unlike the classic leftist heap this merges by subtree size (no d
 *        array); join uses x|=y for empty heaps; self-merge x==y not handled
 * ============================================================
 * Example (uncomment to compile):
 * static heap_weight_leftist<1009> hp;
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
