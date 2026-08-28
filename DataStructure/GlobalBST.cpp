#include <bits/stdc++.h>
using namespace std;
#define int long long

// Global balanced BST: static-tree path add / path max / point set in O(log n)
template <size_t N>
struct global_bst {
  static constexpr int INF = 0x3f3f3f3f3f3f3f3f;
  int n, idx;
  int val[N], fa[N], dep[N], sz[N], son[N], top[N], dfn[N], id[N], light[N];
  int ls[N], rs[N], f[N], mx[N], tag[N], w[N], Lp[N], Rp[N], root[N], pstk[N];
  vector<int> es[N];
  void dfs1(int x, int ff) {
    fa[x] = ff, dep[x] = dep[ff] + 1, sz[x] = 1;
    for (int y : es[x])
      if (y != ff) {
        dfs1(y, x), sz[x] += sz[y];
        if (sz[y] > sz[son[x]]) {
          if (son[x]) light[x] += sz[son[x]];
          son[x] = y;
        } else light[x] += sz[y];
      }
  }
  void dfs2(int x, int tp) {
    top[x] = tp, dfn[x] = ++idx, id[idx] = x;
    if (son[x]) dfs2(son[x], tp);
    for (int y : es[x])
      if (y != fa[x] && y != son[x]) dfs2(y, y);
  }
  void up(int x) {
    Lp[x] = ls[x] ? Lp[ls[x]] : dfn[x];
    Rp[x] = rs[x] ? Rp[rs[x]] : dfn[x];
    int lv = ls[x] ? mx[ls[x]] : -INF;
    int rv = rs[x] ? mx[rs[x]] : -INF;
    mx[x] = max(val[x], max(lv, rv));
  }
  void apply(int x, int d) {
    if (x) val[x] += d, mx[x] += d, tag[x] += d;
  }
  void pushdown(int x) {
    if (tag[x]) apply(ls[x], tag[x]), apply(rs[x], tag[x]), tag[x] = 0;
  }
  int build_chain(int l, int r) {  // weighted-median BST of one heavy chain
    if (l > r) return 0;
    int tot = 0;
    for (int i = l; i <= r; ++i) tot += w[id[i]];
    int acc = 0, mid = l;
    for (int i = l; i <= r; ++i) {
      acc += w[id[i]];
      if (acc * 2 >= tot) {
        mid = i;
        break;
      }
    }
    int x = id[mid];
    ls[x] = build_chain(l, mid - 1);
    rs[x] = build_chain(mid + 1, r);
    if (ls[x]) f[ls[x]] = x;
    if (rs[x]) f[rs[x]] = x;
    up(x);
    return x;
  }
  void build(int n_) {  // HLD + one BST per heavy chain
    n = n_, idx = 0;
    for (int h = 1; h <= n; ++h)
      if (!fa[h] && top[h] == 0) dfs1(h, 0), dfs2(h, h);
    for (int h = 1; h <= n; ++h)
      if (top[h] == h) {
        int len = 0;
        for (int x = h; x; x = son[x]) ++len;
        for (int i = 0; i < len; ++i) w[id[dfn[h] + i]] = light[id[dfn[h] + i]] + 1;
        root[h] = build_chain(dfn[h], dfn[h] + len - 1);
      }
  }
  void range_add(int x, int l, int r, int d) {
    if (!x || r < Lp[x] || Rp[x] < l) return;
    if (l <= Lp[x] && Rp[x] <= r) return apply(x, d), void();
    pushdown(x);
    if (l <= dfn[x] && dfn[x] <= r) val[x] += d;
    range_add(ls[x], l, r, d), range_add(rs[x], l, r, d);
    up(x);
  }
  int range_max(int x, int l, int r) {
    if (!x || r < Lp[x] || Rp[x] < l) return -INF;
    if (l <= Lp[x] && Rp[x] <= r) return mx[x];
    pushdown(x);
    int res = -INF;
    if (l <= dfn[x] && dfn[x] <= r) res = val[x];
    return max(res, max(range_max(ls[x], l, r), range_max(rs[x], l, r)));
  }
  void path_add(int u, int v, int d) {
    while (top[u] != top[v]) {
      if (dep[top[u]] < dep[top[v]]) swap(u, v);
      range_add(root[top[u]], dfn[top[u]], dfn[u], d);
      u = fa[top[u]];
    }
    if (dep[u] > dep[v]) swap(u, v);
    range_add(root[top[u]], dfn[u], dfn[v], d);
  }
  int path_max(int u, int v) {
    int res = -INF;
    while (top[u] != top[v]) {
      if (dep[top[u]] < dep[top[v]]) swap(u, v);
      res = max(res, range_max(root[top[u]], dfn[top[u]], dfn[u]));
      u = fa[top[u]];
    }
    if (dep[u] > dep[v]) swap(u, v);
    res = max(res, range_max(root[top[u]], dfn[u], dfn[v]));
    return res;
  }
  void point_set(int x, int v) {
    int tp2 = 0;  // reuse member stack array instead of a large local
    for (int y = x; y; y = f[y]) pstk[++tp2] = y;
    while (tp2) pushdown(pstk[tp2--]);
    val[x] = v;
    for (int y = x; y; y = f[y]) up(y);
  }
};

/*
 * ============================================================
 * Name: Global balanced BST (static-tree path operations)
 * Complexity: preprocessing O(n log n); path add / path max / point set O(log n) amortized
 * Usage: path updates and queries on a static tree, wrapped as global_bst<N>:
 *        fill es and val, build(n), then path_add / path_max / point_set;
 *        compared to "HLD + segment tree" O(log^2 n), in-chain operations run
 *        in O(log n) via weighted balanced BSTs; change up/apply to maintain
 *        sums, xor, etc.
 * Principle: after HLD, each heavy chain is built into a weighted-median BST
 *        (weight = light-subtree-size sum + 1; in-order = chain order; subtrees
 *        cover contiguous dfn ranges stored in Lp/Rp); in-chain range ops
 *        recurse by subtree coverage (O(log n)); path ops = chain climbing +
 *        in-chain BST range ops; weighted medians guarantee O(log n) chain height
 * Notes: forests are handled (build runs dfs1/dfs2 for every root); vertex
 *        weights (edge weights can be pushed down to the child); up relies on
 *        "in-order == dfn order"
 * ============================================================
 * Example (uncomment to compile; path add + path max + point set):
 * static global_bst<200009> gb;
 * signed main() {
 *   int n, m;
 *   cin >> n >> m;
 *   for (int i = 1; i <= n; ++i) cin >> gb.val[i];
 *   for (int i = 1, u, v; i < n; ++i) {
 *     cin >> u >> v;
 *     gb.es[u].push_back(v), gb.es[v].push_back(u);
 *   }
 *   gb.build(n);
 *   while (m--) {
 *     int o, u, v;
 *     cin >> o >> u >> v;
 *     if (o == 1) {
 *       int d;
 *       cin >> d;
 *       gb.path_add(u, v, d);
 *     }
 *     if (o == 2) cout << gb.path_max(u, v) << '\n';
 *     if (o == 3) gb.point_set(u, v);
 *   }
 * }
 * ============================================================
 */
