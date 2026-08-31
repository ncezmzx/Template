#include <bits/stdc++.h>
using namespace std;
#define int long long

// rerooting DP: f[x] = sum over marked u of (dist(x, u) + 1), two DFS passes
template <size_t N> struct tree_reroot {
  int n, m;
  int sz[N], key[N], dst[N], f[N];
  vector<int> se[N]; // rooted tree: children lists (dst[y] = edge weight to y)
  // pass 1: subtree DP. sz[x] = #marks in subtree, f[x] = subtree contribution
  void dfs2(int x) {
    sz[x] = f[x] = key[x];
    for (int y : se[x]) dfs2(y), f[x] += f[y] + dst[y] * sz[y], sz[x] += sz[y];
  }
  // pass 2: reroot. f[y] = f[x] + dst[y] * (m - 2 * sz[y])
  void dfs3(int x) {
    for (int y : se[x]) f[y] = f[x] + dst[y] * (m - 2 * sz[y]), dfs3(y);
  }
};

/*
 * ============================================================
 * Name: tree rerooting DP (reroot / two-pass scan)
 * Complexity: O(n) (two DFS passes)
 * Usage: given a tree and marked vertices (key[x] = 1), wrapped as
 *        tree_reroot<N>: build the rooted children lists se[] with edge
 *        weights dst[], then dfs2(root) + dfs3(root) give f[x] for every x,
 *        where f[x] = sum over marks u of (dist(x, u) + 1) — the original
 *        initialized f[x] = key[x], adding 1 per mark; subtract m if pure
 *        distances are wanted
 * Reroot formula: moving from x to child y,
 *        f[y] = f[x] + dst[y] * (m - 2 * sz[y])
 *   with sz[y] = marks inside y's subtree and m = total marks: the sz[y]
 *   marks inside get dst[y] closer, the other m - sz[y] get dst[y] farther.
 *   Note: the original ran on a virtual tree (se[x] = virtual children,
 *   dst[y] = original-tree distance); it works on plain trees too.
 * Source: all.cpp 7568-7577 (dfs2/dfs3 extracted verbatim; declarations from
 *         the original block 7488-7491, trimmed to what this template uses)
 * ============================================================
 * Example (uncomment to compile; n vertices, m marks, weighted tree -> per-vertex sums):
 * static tree_reroot<400009> tr;
 * vector<pair<int, int>> g[400009];  // demo-only weighted adjacency
 * signed main() {
 *   cin.tie(nullptr)->sync_with_stdio(false);
 *   cin >> tr.n >> tr.m;
 *   for (int i = 1; i <= tr.m; ++i) {
 *     int x;
 *     cin >> x;
 *     tr.key[x] = 1;
 *   }
 *   for (int i = 1, u, v, w; i < tr.n; ++i) {
 *     cin >> u >> v >> w;
 *     g[u].push_back({v, w}), g[v].push_back({u, w});
 *   }
 *   // root the tree at 1 (se[x] = children, dst[y] = edge weight)
 *   function<void(int, int)> build = [&](int x, int ff) {
 *     for (auto [y, w] : g[x])
 *       if (y != ff) tr.se[x].push_back(y), tr.dst[y] = w, build(y, x);
 *   };
 *   build(1, 0);
 *   tr.dfs2(1);
 *   tr.dfs3(1);
 *   for (int i = 1; i <= tr.n; ++i) cout << tr.f[i] << " \n"[i == tr.n];
 *   return 0;
 * }
 * ============================================================
 */
