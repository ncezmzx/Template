#include <bits/stdc++.h>
using namespace std;
#define int long long


constexpr int INF = 0x3f3f3f3f3f3f3f3fLL;


int steiner_tree(int n, const vector<array<int, 3>> &edges, const vector<int> &keys) {
  int k = keys.size(), m = 1 << k;
  vector<vector<pair<int, int>>> g(n + 1);
  for (auto &e : edges) g[e[0]].push_back({e[1], e[2]}), g[e[1]].push_back({e[0], e[2]});
  vector<vector<int>> dp(n + 1, vector<int>(m, INF));
  for (int i = 0; i < k; ++i) dp[keys[i]][1 << i] = 0;
  for (int s = 1; s < m; ++s) {
    for (int i = 1; i <= n; ++i)
      for (int sub = s & (s - 1); sub; sub = s & (sub - 1)) dp[i][s] = min(dp[i][s], dp[i][sub] + dp[i][s ^ sub]);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> q;
    for (int i = 1; i <= n; ++i)
      if (dp[i][s] < INF) q.push({dp[i][s], i});
    while (!q.empty()) {
      int d = q.top().first, u = q.top().second;
      q.pop();
      if (d != dp[u][s]) continue;
      for (auto &e : g[u])
        if (dp[e.first][s] > d + e.second) dp[e.first][s] = d + e.second, q.push({dp[e.first][s], e.first});
    }
  }
  return dp[keys[0]][m - 1];
}

