#include <bits/stdc++.h>
using namespace std;
#define int long long

// static Top Tree: cluster decomposition via HLD + in-chain splay-style BSTs
template <size_t N> struct top_tree {
  static constexpr int INF = 0x3f3f3f3f3f3f3f3f;
  int n, idx;
  int val[N], fa[N], dep[N], sz[N], son[N], top[N], dfn[N], id[N], tail[N];
  int ch[N][2], f[N], Lp[N], Rp[N];
  int smx[N], rake[N], ck[N], crt[N];
  vector<int> es[N];
  void dfs1(int x, int ff) {
    fa[x] = ff, dep[x] = dep[ff] + 1, sz[x] = 1;
    for (int y : es[x])
      if (y != ff) {
        dfs1(y, x), sz[x] += sz[y];
        if (sz[y] > sz[son[x]]) son[x] = y;
      }
  }
  void dfs2(int x, int tp) {
    top[x] = tp, dfn[x] = ++idx, id[idx] = x, tail[tp] = x;
    if (son[x]) dfs2(son[x], tp);
    for (int y : es[x])
      if (y != fa[x] && y != son[x]) dfs2(y, y);
  }
  void pull(int x) {
    smx[x] = max(val[x], max(ch[x][0] ? smx[ch[x][0]] : -INF, ch[x][1] ? smx[ch[x][1]] : -INF));
    ck[x] = max({smx[x], rake[x], ch[x][0] ? ck[ch[x][0]] : -INF, ch[x][1] ? ck[ch[x][1]] : -INF});
    Lp[x] = ch[x][0] ? Lp[ch[x][0]] : dfn[x];
    Rp[x] = ch[x][1] ? Rp[ch[x][1]] : dfn[x];
  }
  int build_chain(int l, int r) {
    if (l > r) return 0;
    int m = (l + r) >> 1, x = id[m];
    ch[x][0] = build_chain(l, m - 1);
    ch[x][1] = build_chain(m + 1, r);
    if (ch[x][0]) f[ch[x][0]] = x;
    if (ch[x][1]) f[ch[x][1]] = x;
    pull(x);
    return x;
  }
  void build(int n_) { // HLD, then clusters chain by chain, deepest heads first
    n = n_, idx = 0;
    for (int h = 1; h <= n; ++h)
      if (!fa[h] && top[h] == 0) dfs1(h, 0), dfs2(h, h);
    for (int i = 1; i <= n; ++i) rake[i] = -INF;
    vector<int> heads;
    for (int h = 1; h <= n; ++h)
      if (top[h] == h) heads.push_back(h);
    sort(heads.begin(), heads.end(), [&](int a, int b) { return dep[a] > dep[b]; });
    for (int h : heads) {
      for (int x = h; x; x = son[x])
        for (int y : es[x])
          if (y != fa[x] && y != son[x]) rake[x] = max(rake[x], ck[crt[y]]);
      int len = 0;
      for (int x = h; x; x = son[x]) ++len;
      crt[h] = build_chain(dfn[h], dfn[h] + len - 1);
    }
  }
  int ck_query(int x, int l, int r) { // cluster aggregate (includes light rakes)
    if (!x || r < Lp[x] || Rp[x] < l) return -INF;
    if (l <= Lp[x] && Rp[x] <= r) return ck[x];
    int res = -INF;
    if (l <= dfn[x] && dfn[x] <= r) res = max(val[x], rake[x]);
    return max(res, max(ck_query(ch[x][0], l, r), ck_query(ch[x][1], l, r)));
  }
  int pt_query(int x, int l, int r) { // plain path aggregate (no rakes)
    if (!x || r < Lp[x] || Rp[x] < l) return -INF;
    if (l <= Lp[x] && Rp[x] <= r) return smx[x];
    int res = (l <= dfn[x] && dfn[x] <= r) ? val[x] : -INF;
    return max(res, max(pt_query(ch[x][0], l, r), pt_query(ch[x][1], l, r)));
  }
  int path_query(int u, int v) { // max vertex weight on path u-v
    int res = -INF;
    while (top[u] != top[v]) {
      if (dep[top[u]] < dep[top[v]]) swap(u, v);
      res = max(res, pt_query(crt[top[u]], dfn[top[u]], dfn[u]));
      u = fa[top[u]];
    }
    if (dep[u] > dep[v]) swap(u, v);
    res = max(res, pt_query(crt[top[u]], dfn[u], dfn[v]));
    return res;
  }
  int subtree_query(int x) { // max vertex weight in the subtree of x
    int h = top[x];
    return ck_query(crt[h], dfn[x], dfn[tail[h]]);
  }
  void point_set(int x, int v) { // update value, propagate along splay + light edges
    val[x] = v;
    while (true) {
      for (int u = x; u; u = f[u]) pull(u);
      int h = top[x], p = fa[h];
      if (!p) break;
      int nr = -INF;
      for (int y : es[p])
        if (y != fa[p] && y != son[p]) nr = max(nr, ck[crt[y]]);
      if (nr == rake[p]) break;
      rake[p] = nr;
      x = p;
    }
  }
};

/*
 * ============================================================
 * Name: static Top Tree (heavy-chain decomposition + cluster aggregation)
 * Complexity: preprocessing O(n log n); path_query O(log^2 n); subtree_query
 *             O(log n); point_set O(log^2 n + sum of light degrees)
 * Usage: cluster aggregate queries on a static tree, `top_tree<N>`:
 *        path_query(u, v): max vertex weight on the path (HLD semantics, no
 *        light rakes);
 *        subtree_query(x): max over the whole subtree in O(log n) via cluster
 *        rakes, not O(subtree size);
 *        point_set(x, v): updates a value and propagates along the splay and
 *        the light edges.
 * Principle: one splay per heavy chain (in-order = chain order, subtrees cover
 *            contiguous dfn ranges); each node keeps smx (in-chain range max),
 *            rake (aggregate of light-child chain clusters) and ck (cluster
 *            aggregate); chains are built from the deepest heads up so rake
 *            dependencies are ready
 * Notes: vertex weights; forests are handled; tail[h] is the bottom of chain h;
 *        rake depends on ck and vice versa, see point_set for the propagation
 *        loop
 * ============================================================
 */
