#include <bits/stdc++.h>
using namespace std;
#define int long long

// heavy-light decomposition + segment tree (point set / range sum) for path sums
template <size_t N>
struct hld {
  int n, idx;
  int a[N], dfn[N], sz[N], id[N], top[N], fa[N], son[N], dep[N];
  vector<int> es[N];
  int sgt[N << 2];
  int dfs1(int x, int ff) {  // fa/dep/subtree size/heavy child
    fa[x] = ff, dep[x] = dep[ff] + 1;
    for (int y : es[x])
      if (y ^ ff) sz[x] += dfs1(y, x), sz[y] > sz[son[x]] && (son[x] = y);
    return ++sz[x];
  }
  void dfs2(int x, int tp) {  // decompose: top = chain head, dfn order (heavy child first)
    top[x] = tp, id[dfn[x] = ++idx] = x;
    if (son[x]) dfs2(son[x], tp);
    for (int y : es[x])
      if ((y ^ son[x]) && (y ^ fa[x])) dfs2(y, y);
  }
  void up(int u) {
    sgt[u] = sgt[u << 1] + sgt[u << 1 | 1];
  }
  void build(int u, int l, int r) {
    if (l == r) return sgt[u] = a[id[l]], void();  // id[dfn] restores vertex weights
    int m = (l + r) >> 1;
    build(u << 1, l, m), build(u << 1 | 1, m + 1, r);
    up(u);
  }
  void update(int u, int l, int r, int x, int y) {  // point set: dfn x becomes y
    if (l == r) return sgt[u] = y, void();
    int m = (l + r) >> 1;
    if (m >= x) update(u << 1, l, m, x, y);
    else update(u << 1 | 1, m + 1, r, x, y);
    up(u);
  }
  int query(int u, int l, int r, int x, int y) {  // range sum over dfn interval
    if (x <= l && r <= y) return sgt[u];
    int m = (l + r) >> 1, res = 0;
    if (m >= x) res += query(u << 1, l, m, x, y);
    if (m < y) res += query(u << 1 | 1, m + 1, r, x, y);
    return res;
  }
  int query(int x, int y) {  // sum along the path x -> y
    int res = 0;
    while (top[x] != top[y]) {
      if (dep[top[x]] < dep[top[y]]) swap(x, y);
      res += query(1, 1, n, dfn[top[x]], dfn[x]);
      x = fa[top[x]];
    }
    if (dfn[x] < dfn[y]) swap(x, y);
    return query(1, 1, n, dfn[y], dfn[x]) + res;
  }
};

/*
 * ============================================================
 * Name: heavy-light decomposition (HLD) + segment tree for path sums
 * Complexity: preprocessing O(n), one path query / point update O(log^2 n)
 * Usage: static-tree path sums, subtree sums and point updates, wrapped as
 *        hld<N>: fill a[] and es[], then dfs1(1, 0), dfs2(1, 1),
 *        build(1, 1, n); query(x, y) is the x->y path sum;
 *        update(1, 1, n, dfn[x], y) sets vertex x's weight to y.
 *        The segment tree here is simplified to point-set/range-sum; swap in
 *        any other segment tree as needed.
 * Source: all.cpp lines 11059-11121 (HLD 11065-11076, path query 11111-11121
 *         kept verbatim; the original was a range-modulo segment tree);
 *         wrapped into a struct
 * ============================================================
 * Example (uncomment to compile):
 * static hld<100009> h;
 * signed main() {
 *   cin.tie(nullptr)->sync_with_stdio(false);
 *   int n, m;
 *   cin >> n >> m;
 *   h.n = n;
 *   for (int i = 1; i <= n; ++i) cin >> h.a[i];
 *   for (int i = 1, x, y; i < n; ++i) {
 *     cin >> x >> y;
 *     h.es[x].push_back(y), h.es[y].push_back(x);
 *   }
 *   h.dfs1(1, 0), h.dfs2(1, 1), h.build(1, 1, n);
 *   while (m--) {
 *     int o, x, y;
 *     cin >> o >> x >> y;
 *     if (o == 1) h.update(1, 1, n, h.dfn[x], y);   // point set
 *     else cout << h.query(x, y) << '\n';           // path sum
 *   }
 * }
 * ============================================================
 */
