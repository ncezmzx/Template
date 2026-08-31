#include <bits/stdc++.h>
using namespace std;

// LCA by binary lifting
template <size_t N> struct lca_binary_lifting {
  int dep[N], fa[N][20]; // fa[u][i] = 2^i-th ancestor of u
  vector<int> e[N];
  void dfs(int u, int f) { // preprocess dep / fa
    dep[u] = dep[f] + 1;
    fa[u][0] = f;
    for (int i = 1; i < 20; i++) fa[u][i] = fa[fa[u][i - 1]][i - 1];
    for (int v : e[u]) {
      if (v == f) continue;
      dfs(v, u);
    }
  }
  int lca(int x, int y) {
    if (dep[x] < dep[y]) swap(x, y);
    for (int i = 19; i >= 0; i--) {
      if (dep[fa[x][i]] >= dep[y]) { // lift x to y's depth
        x = fa[x][i];
      }
    }
    if (x == y) return x;
    for (int i = 19; i >= 0; i--) { // lift together, stop at LCA's children
      if (fa[x][i] != fa[y][i]) {
        x = fa[x][i], y = fa[y][i];
      }
    }
    return fa[x][0];
  }
};

/*
 * ============================================================
 * Name: LCA (binary lifting)
 * Complexity: preprocessing O(n log n), query O(log n)
 * Usage: lowest common ancestor on a static tree, wrapped as
 *        lca_binary_lifting<N>: build the adjacency e, dfs(1, 0), then
 *        lca(x, y)
 * Source: all.cpp lines 55932-55955 (dfs and lca kept verbatim; the original
 *         ran on a block-cut tree, hence arrays of size N<<1 — the template
 *         parameter sizes them; wrapped into a struct)
 * ============================================================
 * Example (uncomment to compile):
 * static lca_binary_lifting<40020> t;
 * signed main() {
 *   cin.tie(nullptr)->sync_with_stdio(false);
 *   int n;
 *   cin >> n;                               // tree on n vertices
 *   for (int i = 1, x, y; i < n; ++i) {
 *     cin >> x >> y;
 *     t.e[x].push_back(y), t.e[y].push_back(x);
 *   }
 *   t.dfs(1, 0);
 *   int q;
 *   cin >> q;
 *   while (q--) {
 *     int x, y;
 *     cin >> x >> y;
 *     cout << t.lca(x, y) << '\n';
 *   }
 * }
 * ============================================================
 */
