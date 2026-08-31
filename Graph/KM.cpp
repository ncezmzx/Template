#include <bits/stdc++.h>
using namespace std;
#define int long long

// Kuhn-Munkres: maximum-weight perfect matching on an n x n bipartite graph
template <size_t N> struct km_matching {
  static constexpr int INF = 0x3f3f3f3f3f3f3f3f;
  int n, w[N][N], lx[N], ly[N], match[N], slack[N], pre[N], visx[N], visy[N];
  void augment(int s) { // BFS-like augment from left vertex s
    for (int i = 1; i <= n; ++i) visx[i] = visy[i] = 0, slack[i] = INF;
    int y = 0, ny = 0;
    match[0] = s;
    while (match[y]) {
      int x = match[y], d = INF;
      visy[y] = 1;
      for (int i = 1; i <= n; ++i)
        if (!visy[i]) {
          if (lx[x] + ly[i] - w[x][i] < slack[i]) slack[i] = lx[x] + ly[i] - w[x][i], pre[i] = y;
          d = min(d, slack[i]);
        }
      for (int i = 0; i <= n; ++i) {
        if (visy[i]) lx[match[i]] -= d, ly[i] += d;
        else slack[i] -= d;
      }
      ny = 0;
      for (int i = 1; i <= n; ++i)
        if (!visy[i] && !slack[i]) ny = i;
      y = ny;
    }
    while (y) match[y] = match[pre[y]], y = pre[y];
  }
  // returns max total weight; match[i] = left vertex matched to right vertex i
  int solve(int n_) {
    n = n_;
    for (int i = 1; i <= n; ++i) lx[i] = -INF, ly[i] = 0, match[i] = 0;
    for (int i = 1; i <= n; ++i)
      for (int j = 1; j <= n; ++j) lx[i] = max(lx[i], w[i][j]);
    for (int i = 1; i <= n; ++i) augment(i);
    int res = 0;
    for (int i = 1; i <= n; ++i) res += w[match[i]][i];
    return res;
  }
};

/*
 * ============================================================
 * Name: KM algorithm (maximum-weight perfect bipartite matching, slack optimization)
 * Complexity: O(n^3)
 * Usage: `km_matching<N>`: maximum-weight perfect matching on an n x n weighted
 *        bipartite graph.
 *        fill w[1..n][1..n], solve(n) returns the total weight; match[i] is the
 *        left vertex matched to right vertex i.
 * Principle: Kuhn-Munkres labels lx / ly with lx[u] + ly[v] >= w[u][v]; find a
 *            perfect matching in the equality subgraph, augmenting adjusts the
 *            labels by the minimum slack and expands the subgraph over O(n)
 *            rounds
 * Notes: dense square matrices with n <= 500; negative weights work (lx starts
 *        at -INF); for unequal sides pad with zero-weight dummy vertices; for a
 *        minimum-weight perfect matching negate all weights
 * ============================================================
 */
