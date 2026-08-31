#include <bits/stdc++.h>
using namespace std;
#define int long long

// Kruskal rebuild tree: bottleneck path values and weight-limited reachability
template <size_t N, size_t M> struct kruskal_rebuild_tree {
  struct edge {
    int u, v, w;
    bool operator<(const edge &r) const { return w < r.w; }
  } e[M];
  int n, m, tot, val[N << 1], fa[N << 1], f[N << 1][20], dep[N << 1];
  vector<int> T[N << 1];
  int get(int x) { return x == fa[x] ? x : fa[x] = get(fa[x]); }
  void build(int n_, int m_) {
    n = n_, m = m_;
    sort(e + 1, e + m + 1);
    tot = n;
    for (int i = 1; i <= 2 * n; ++i) fa[i] = i;
    for (int i = 1; i <= m; ++i) {
      int u = get(e[i].u), v = get(e[i].v);
      if (u == v) continue;
      ++tot;
      fa[u] = fa[v] = tot;
      T[tot].push_back(u), T[tot].push_back(v);
      val[tot] = e[i].w;
      if (tot == 2 * n - 1) break;
    }
  }
  void dfs(int x, int ff) { // root each tree of the rebuild forest
    f[x][0] = ff, dep[x] = dep[ff] + 1;
    for (int i = 1; f[x][i - 1]; ++i) f[x][i] = f[f[x][i - 1]][i - 1]; // stop at 0
    for (int y : T[x])
      if (y != ff) dfs(y, x);
  }
  int lca(int x, int y) {
    if (dep[x] < dep[y]) swap(x, y);
    for (int i = 19; ~i; --i)
      if (dep[f[x][i]] >= dep[y]) x = f[x][i];
    if (x == y) return x;
    for (int i = 19; ~i; --i)
      if (f[x][i] != f[y][i]) x = f[x][i], y = f[y][i];
    return f[x][0];
  }
};

/*
 * ============================================================
 * Name: Kruskal rebuild tree
 * Complexity: build O(m log m) + O(m alpha); preprocessing O(n log n); query
 *             O(log n)
 * Usage: `kruskal_rebuild_tree<N, M>`: bottleneck properties of the MST (built
 *        in ascending edge order).
 *        fill e[1..m], build(n, m), then dfs(i, 0) from every root (fa[i] ==
 *        i); the bottleneck value is val[lca(u, v)].
 *        among all u-v paths, the one minimizing the maximum edge weight has
 *        max-edge weight val[lca(u, v)];
 *        the vertices reachable from u using only edges <= w are the whole
 *        subtree of the highest ancestor with val <= w (combine with lifting +
 *        subtree sizes for weight-limited connectivity);
 *        building in descending order gives max-spanning / bottleneck semantics
 *        instead.
 * Principle: whenever Kruskal merges two components, a new virtual node (weight
 *            = the current edge weight) becomes the parent of both, giving a
 *            2n-1 node heap-ordered binary tree whose leaves are the original
 *            vertices
 * Notes: a disconnected graph yields a rebuild forest, so dfs each root; the
 *        original vertices' val[1..n] are uninitialized (set them to 0 if you
 *        compare them)
 * ============================================================
 */
