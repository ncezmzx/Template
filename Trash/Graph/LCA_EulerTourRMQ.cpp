#include <bits/stdc++.h>
using namespace std;
#define int long long

// LCA via Euler tour + sparse-table RMQ: O(1) queries
template <size_t N>
struct lca_euler_rmq {
  int n, idx;
  int mi[20][N], dfn[N], dep[N];
  vector<int> es[N];
  void dfs(int x, int ff) {  // Euler order: mi[0][dfn[x]] stores the parent
    mi[0][dfn[x] = ++idx] = ff, dep[x] = dep[ff] + 1;
    for (int y : es[x])
      if (y ^ ff) dfs(y, x);
  }
  int get(int x, int y) { return dfn[x] < dfn[y] ? x : y; }  // shallower (earlier) vertex
  void build(int n_) {  // sparse table over the Euler tour
    n = n_;
    for (int i = 1; i < 20; ++i)
      for (int j = 1; j + (1 << i) - 1 <= n; ++j)
        mi[i][j] = get(mi[i - 1][j], mi[i - 1][j + (1 << (i - 1))]);
  }
  int lca(int x, int y) {  // shallowest vertex on the Euler interval
    if (x == y) return x;
    if ((x = dfn[x]) > (y = dfn[y])) swap(x, y);
    int d = __lg(y - ++x + 1);
    return get(mi[d][x], mi[d][y - (1 << d) + 1]);
  }
  int dist(int x, int y) { return dep[x] + dep[y] - 2 * dep[lca(x, y)]; }
};

/*
 * ============================================================
 * Name: LCA (Euler tour + sparse-table RMQ)
 * Complexity: preprocessing O(n log n), query O(1)
 * Usage: lowest common ancestor lca(x, y) and tree distance dist(x, y),
 *        wrapped as lca_euler_rmq<N>: build es, dfs(1, 0), build(n), then
 *        lca / dist queries
 * Implementation: dfs records mi[0][dfn[x]] = parent (Euler order); the
 *        sparse table takes the shallowest vertex over [dfn[x], dfn[y]]
 * Source: all.cpp lines 7965-7990 (kept verbatim; see also 51062-51068,
 *         24960-24966); wrapped into a struct with build()
 * ============================================================
 * Example (uncomment to compile):
 * static lca_euler_rmq<100009> t;
 * signed main() {
 *   cin.tie(nullptr)->sync_with_stdio(false);
 *   int n;
 *   cin >> n;                              // tree on n vertices
 *   for (int i = 1, x, y; i < n; ++i) {
 *     cin >> x >> y;
 *     t.es[x].push_back(y), t.es[y].push_back(x);
 *   }
 *   t.dfs(1, 0);
 *   t.build(n);
 *   int q;
 *   cin >> q;
 *   while (q--) {
 *     int x, y;
 *     cin >> x >> y;
 *     cout << "lca=" << t.lca(x, y) << " dist=" << t.dist(x, y) << '\n';
 *   }
 * }
 * ============================================================
 */
