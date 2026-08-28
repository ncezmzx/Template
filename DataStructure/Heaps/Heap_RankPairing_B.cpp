#include <bits/stdc++.h>
using namespace std;
#define int long long

// rank-pairing heap, type B (multi-way tree form), mergeable min-heap
template <size_t N>
struct heap_rank_pairing_b {
  int a[N], fa[N], vl[N], tp[N], rk[N];
  list<int> nd[N], sn[N];
  typename list<int>::iterator it[N];
  void add(list<int>& x, int y) { it[y] = x.insert(x.end(), y); }
  void cmin(int& x, int y) { x > y && (x = y); }
  int newnode(int x, int i) { return add(nd[i], i), tp[i] = vl[i] = x, i; }
  int top(int x) { return tp[x]; }
  void join(int x, int y) {
    cmin(tp[x], tp[y]), nd[x].splice(nd[x].end(), nd[y]);
  }
  int getls(int x) { return sn[x].empty() ? 0 : sn[x].front(); }
  int getrs(int x) {  // next sibling via list order
    int f = fa[x];
    if (!f || it[x] == prev(sn[f].end())) return 0;
    return *next(it[x]);
  }
  int getrk(int x) { return x ? rk[x] : -1; }
  int getfa(int x) {  // previous sibling acts as heap-parent in this form
    int f = fa[x];
    if (!f || it[x] == sn[f].begin()) return f;
    return *prev(it[x]);
  }
  void decrease_key(int h, int p, int v) {
    cmin(tp[h], vl[p] = v);
    int f = getfa(p);
    if (!f || vl[f] <= v) return;
    sn[f].erase(it[p]), add(nd[h], p), fa[p] = 0, rk[p] = getrk(getls(p)) + 1;
    while (f) {
      int rkf = getrk(f), rkl = getrk(getls(f)), rkr = getrk(getrs(f));
      if (rkf == rkl || (rkf == rkl + 1 && rkr <= rkl + 1)) break;
      rk[f] = rkr <= rkl + 1 ? rkl + 1 : rkl, f = getfa(f);
    }
  }
  void erase(int h, int x) {
    decrease_key(h, x, LLONG_MIN);
    for (int y : sn[x]) add(nd[h], y), fa[y] = 0;
    nd[h].erase(it[x]), sn[x].clear();
    int mx = -1;
    for (int x : nd[h]) {
      while (int y = a[rk[x]]) {
        if (vl[x] > vl[y]) swap(x, y);
        add(sn[fa[y] = x], y), a[rk[x]++] = 0;
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
 * Name: rank-pairing heap type B (multi-way tree form), mergeable min-heap
 * Complexity: newnode/top/join O(1) amortized; decrease_key/erase O(log n) amortized
 * Usage: newnode/top/join/decrease_key/erase, wrapped as heap_rank_pairing_b<N>;
 *        heaps identified by their container index
 * Source: Luogu article "In Praise of the Priority Queue"
 *         (the Luogu blog article "In Praise of the Priority Queue") section 6 (type B + multi-way tree), wrapped into a struct
 * Notes: child lists sn[] are std::lists; getls/getrs/getfa derive left/right
 *        siblings from it[] iterators and list order; KNOWN ISSUE: the erase
 *        rebuild (nd[h].clear() + re-insert) occasionally misbehaves/crashes
 *        on some operation sequences (suspected iterator invalidation) —
 *        validate with stress tests; empty-heap / self-merge not handled
 * ============================================================
 * Example (uncomment to compile; join/top only — see the known-issue note above):
 * static heap_rank_pairing_b<1009> hp;
 * signed main() {
 *   int h = hp.newnode(5, 1);
 *   hp.join(h, hp.newnode(3, 2));
 *   hp.join(h, hp.newnode(8, 3));
 *   hp.join(h, hp.newnode(1, 4));
 *   hp.join(h, hp.newnode(7, 5));
 *   cout << hp.top(h) << '\n';      // 1
 * }
 * ============================================================
 */
