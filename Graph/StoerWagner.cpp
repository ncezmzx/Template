#include <bits/stdc++.h>
using namespace std;
#define int long long

// Stoer-Wagner global minimum cut (undirected, non-negative weights, multi-edges ok)
template <size_t N>
struct stoer_wagner {
  int n_;
  int edge[N][N];          // adjacency matrix (1-indexed, multi-edges accumulated)
  int dist[N], vis[N], bin[N];  // bin[i]: vertex i already contracted
  void init(int n) {
    n_ = n;
    for (int i = 1; i <= n; ++i) {
      bin[i] = 0;
      for (int j = 1; j <= n; ++j) edge[i][j] = 0;
    }
  }
  void add(int u, int v, int w) { edge[u][v] += w, edge[v][u] += w; }
  // one phase: maximum-adjacency ordering; s = second last, t = last added
  int contract_(int& s, int& t) {
    memset(dist, 0, sizeof dist), memset(vis, 0, sizeof vis);
    int mincut = 0;
    s = t = -1;
    for (int i = 0; i < n_; ++i) {
      int k = -1, mx = -1;
      for (int j = 1; j <= n_; ++j)
        if (!bin[j] && !vis[j] && dist[j] > mx) k = j, mx = dist[j];
      if (k == -1) return mincut;
      s = t, t = k, mincut = mx, vis[k] = 1;
      for (int j = 1; j <= n_; ++j)
        if (!bin[j] && !vis[j]) dist[j] += edge[k][j];
    }
    return mincut;
  }
  int solve() {
    int ans = LLONG_MAX, s = 0, t = 0;
    for (int i = 1; i < n_; ++i) {
      int cur = contract_(s, t);
      bin[t] = 1;
      ans = min(ans, cur);
      if (ans == 0) return 0;  // disconnected graph: min cut is 0
      for (int j = 1; j <= n_; ++j)
        if (!bin[j]) edge[s][j] = (edge[j][s] += edge[j][t]);  // merge t into s
    }
    return ans;
  }
};

/*
 * ============================================================
 * ============================================================
 * Name: Stoer-Wagner global minimum cut
 * Complexity: O(nm + n^2 log n) (naive form O(n^3); fine for n <= 600)
 * Usage: global min cut of an undirected positive-weight graph without
 *        fixing source/sink, wrapped as stoer_wagner<N>: init(n),
 *        add(u, v, w) (multi-edges accumulate), solve() returns the global
 *        min cut; much faster than running max flow for all source/sink
 *        pairs; disconnected graphs answer 0
 * Principle: any two vertices s, t are either on the same side of a cut or
 *        separated by some s-t cut. Each phase runs a maximum-adjacency
 *        ordering (Prim-like greedy by w(A, .)) to obtain the cut-of-phase,
 *        then merges t into s (edge weights add up); the answer is the
 *        minimum over all n-1 phases
 * Notes: 1-indexed; weights must be non-negative (a 0 cut returns early);
 *        size N per vertex count; n < 2 is meaningless
 * Source: OI-Wiki "Stoer-Wagner" (https://oi-wiki.org/graph/stoer-wagner/)
 * ============================================================
 * Example (uncomment to compile; Luogu P5632):

 * static stoer_wagner<605> sw;
 * signed main() {
 *   int n, m;
 *   cin >> n >> m;
 *   sw.init(n);
 *   for (int i = 0; i < m; ++i) {
 *     int u, v, w;
 *     cin >> u >> v >> w;
 *     sw.add(u, v, w);
 *   }
 *   cout << sw.solve() << '\n';
 * }
 * ============================================================
 */
