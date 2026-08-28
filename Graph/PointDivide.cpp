#include <bits/stdc++.h>
using namespace std;
#define int long long

// Dynamic centroid tree: distance-based subtree statistics with point updates
template <size_t N>
struct centroid_tree {
  int n, idx, rt, cnt, lca_built = -1;
  int a[N], mi[20][N], dfn[N], fa[N], ban[N], sz[N], dep[N];
  vector<int> es[N], c[2][N];  // c[0]: per-distance BIT, c[1]: toward parent centroid
  void bit_add(int u, int x, int y, int o) {
    for (++x; x < (int)c[o][u].size(); x += x & -x) c[o][u][x] += y;
  }
  int bit_sum(int u, int x, int o, int r = 0) {
    for (++x, x = min(x, (int)c[o][u].size() - 1); x > 0; x -= x & -x) r += c[o][u][x];
    return r;
  }
  void dfs(int x, int ff) {  // Euler order for LCA sparse table
    mi[0][dfn[x] = ++idx] = ff, dep[x] = dep[ff] + 1;
    for (int y : es[x])
      if (y ^ ff) dfs(y, x);
  }
  int get(int x, int y) { return dfn[x] < dfn[y] ? x : y; }
  void build_mi() {  // lazy build (levels d >= 1 must exist before lca)
    for (int k = 1; k < 20; ++k)
      for (int i = 1; i + (1 << k) - 1 <= idx; ++i)
        mi[k][i] = get(mi[k - 1][i], mi[k - 1][i + (1 << (k - 1))]);
    lca_built = idx;
  }
  int lca(int x, int y) {
    if (x == y) return x;
    if (lca_built != idx) build_mi();
    if ((x = dfn[x]) > (y = dfn[y])) swap(x, y);
    int d = __lg(y - ++x + 1);
    return get(mi[d][x], mi[d][y - (1 << d) + 1]);
  }
  int dist(int x, int y) { return dep[x] + dep[y] - 2 * dep[lca(x, y)]; }
  void dfs3(int x, int ff) {
    sz[x] = 1;
    for (int y : es[x])
      if (y ^ ff && !ban[y]) dfs3(y, x), sz[x] += sz[y];
  }
  void dfs2(int x, int ff) {
    int mx = cnt - sz[x];
    for (int y : es[x])
      if (y ^ ff && !ban[y]) dfs2(y, x), mx = max(mx, sz[y]);
    if (mx * 2 <= cnt) rt = x;
  }
  void build(int x, int ff) {  // centroid decomposition
    dfs3(x, 0), cnt = sz[x], dfs2(x, 0);
    ban[rt] = 1, fa[rt] = ff, c[0][rt].resize(cnt + 2), c[1][rt].resize(cnt + 2);
    int u = rt;
    for (int y : es[u])
      if (y ^ ff && !ban[y]) build(y, u);
  }
  int query(int x, int y) {  // sum of weights within distance y from x
    int res = 0;
    for (int pr = 0, i = x; i; pr = i, i = fa[i]) {
      int d = dist(i, x);
      res += bit_sum(i, y - d, 0);
      if (pr) res -= bit_sum(pr, y - d, 1);
    }
    return res;
  }
  void update(int x, int y) {  // add y to the weight of x along the centroid path
    for (int i = x; i; i = fa[i]) bit_add(i, dist(x, i), y, 0);
    for (int i = x; fa[i]; i = fa[i]) bit_add(i, dist(x, fa[i]), y, 1);
  }
};

/*
 * ============================================================
 * ============================================================
 * Name: centroid decomposition (dynamic centroid tree)
 * Complexity: centroid-tree build O(n log n) (each level O(current component size));
 *             one query/update O(log^2 n)
 * Usage: distance-based statistics on a tree with point updates (e.g. sum of
 *        weights within distance k of x), wrapped as centroid_tree<N>:
 *        es holds the tree, dfs(1, 0) builds the Euler order (the LCA sparse
 *        table is built lazily), build(1, 0) builds the centroid tree,
 *        update(x, w) adds weight, query(x, k) sums weights within distance
 *        k of x; each centroid rt owns two BITs c[0][rt] (per-distance
 *        contributions) and c[1][rt] (toward the parent centroid, for
 *        inclusion-exclusion)
 * Source: all.cpp lines 7965-8041 (wrapped into a struct, logic unchanged)
 * ============================================================
 * Example (uncomment to compile):

 * static centroid_tree<100009> cd;
 * signed main() {
 *   cin.tie(nullptr)->sync_with_stdio(false);
 *   int n, m;
 *   cin >> n >> m;
 *   for (int i = 1; i <= n; ++i) cin >> cd.a[i];
 *   for (int i = 1, x, y; i < n; ++i) {
 *     cin >> x >> y, cd.es[x].push_back(y), cd.es[y].push_back(x);
 *   }
 *   cd.dfs(1, 0);
 *   cd.build(1, 0);
 *   for (int i = 1; i <= n; ++i) cd.update(i, cd.a[i]);   // initialize weights
 *   while (m--) {
 *     int o, x, y;
 *     cin >> o >> x >> y;
 *     if (o == 0) cout << cd.query(x, y) << '\n';          // sum of weights within distance y of x
 *     else cd.update(x, y - cd.a[x]), cd.a[x] = y;         // set x's weight to y
 *   }
 * }
 * ============================================================
 */
