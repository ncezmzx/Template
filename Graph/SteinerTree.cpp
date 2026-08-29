#include <bits/stdc++.h>
using namespace std;
#define int long long

// minimum Steiner tree: min total weight connecting k terminals (undirected, positive weights)
constexpr int INF = 0x3f3f3f3f3f3f3f3fLL;

// edges = {u, v, w} (1-indexed), keys = k terminals; returns the min cost (INF if unreachable)
int steiner_tree(int n, const vector<array<int, 3>>& edges, const vector<int>& keys) {
  int k = keys.size(), m = 1 << k;
  vector<vector<pair<int, int>>> g(n + 1);
  for (auto& e : edges) g[e[0]].push_back({e[1], e[2]}), g[e[1]].push_back({e[0], e[2]});
  vector<vector<int>> dp(n + 1, vector<int>(m, INF));
  for (int i = 0; i < k; ++i) dp[keys[i]][1 << i] = 0;
  for (int s = 1; s < m; ++s) {
    for (int i = 1; i <= n; ++i)  // subset merge over proper subsets of s
      for (int sub = s & (s - 1); sub; sub = s & (sub - 1))
        dp[i][s] = min(dp[i][s], dp[i][sub] + dp[i][s ^ sub]);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> q;
    for (int i = 1; i <= n; ++i)
      if (dp[i][s] < INF) q.push({dp[i][s], i});
    while (!q.empty()) {  // Dijkstra relaxation across edges for this state
      int d = q.top().first, u = q.top().second;
      q.pop();
      if (d != dp[u][s]) continue;
      for (auto& e : g[u])
        if (dp[e.first][s] > d + e.second)
          dp[e.first][s] = d + e.second, q.push({dp[e.first][s], e.first});
    }
  }
  return dp[keys[0]][m - 1];
}

/*
 * ============================================================
 * Name: minimum Steiner tree
 * Complexity: O(3^k * n + 2^k * (m + n) log n)
 * Usage: minimum total edge weight connecting all k terminals of an undirected
 *        positive-weight connected graph (it may route through non-terminals; k
 *        is usually <= 10):
 *        steiner_tree(n, {{u, v, w}, ...}, {key1, ..., keyk}) returns the
 *        minimum cost, INF if unreachable.
 * Principle: dp[i][S] = minimum cost of a tree rooted at i connecting the
 *            terminals in S; transition 1 merges subsets (dp[i][T] +
 *            dp[i][S^T]), transition 2 relaxes edges as one Dijkstra pass per S
 * Notes: 0x3f3f3f3f3f3f3f3f is the infinity sentinel; dedupe multi-edges and
 *        self-loops yourself; for vertex-weighted variants (e.g. WC2008 tour
 *        planning) subtract double-counted vertex weights during merges
 * Source: OI-Wiki "Steiner tree" (https://oi-wiki.org/graph/steiner-tree/)
 * ============================================================
 */
