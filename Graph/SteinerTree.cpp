#include <bits/stdc++.h>
using namespace std;
#define int long long

// 最小斯坦纳树：连通 k 个关键点的最小边权和（无向正权连通图）
constexpr int INF = 0x3f3f3f3f3f3f3f3fLL;

// edges：{u, v, w}（1-indexed）；keys：k 个关键点（1-indexed）
// 返回连通全部关键点的最小边权和（不可达返回 INF）
int steiner_tree(int n, const vector<array<int, 3>>& edges, const vector<int>& keys) {
  int k = keys.size(), m = 1 << k;
  vector<vector<pair<int, int>>> g(n + 1);
  for (auto& e : edges) g[e[0]].push_back({e[1], e[2]}), g[e[1]].push_back({e[0], e[2]});
  vector<vector<int>> dp(n + 1, vector<int>(m, INF));
  for (int i = 0; i < k; ++i) dp[keys[i]][1 << i] = 0;
  for (int s = 1; s < m; ++s) {
    for (int i = 1; i <= n; ++i)  // 子树合并：枚举 s 的真子集
      for (int sub = s & (s - 1); sub; sub = s & (sub - 1))
        dp[i][s] = min(dp[i][s], dp[i][sub] + dp[i][s ^ sub]);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> q;
    for (int i = 1; i <= n; ++i)
      if (dp[i][s] < INF) q.push({dp[i][s], i});
    while (!q.empty()) {  // Dijkstra 松弛：跨边扩展同一连通状态
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
 * 名称：最小斯坦纳树（Steiner Tree）
 * 复杂度：O(3^k · n + 2^k · (m + n) log n)
 * 用途：给定无向正权连通图与 k 个关键点，求连通全部关键点的
 *       最小边权和（可借助非关键点"中转"，k 通常 ≤ 10）
 * 接口：steiner_tree(n, {{u, v, w}, ...}, {key1, ..., keyk})
 *       返回最小边权和（不可达返回 INF）
 * 原理：dp[i][S] = 以 i 为根、连通 S 内关键点的最小边权和。
 *       转移 1（子树合并）：dp[i][S] = min(dp[i][T] + dp[i][S^T])；
 *       转移 2（边松弛）：dp[v][S] = min(dp[v][S], dp[u][S] + w(u,v))，
 *       用 Dijkstra 对每个 S 一次性松弛。
 * 注意：边权 0x3f3f3f3f3f3f3f3f 视为"无穷大"哨兵；重边/自环自行去重；
 *       点权版（如 WC2008 游览计划）合并时减去重复计的点权即可
 * 来源：OI-Wiki《斯坦纳树》（https://oi-wiki.org/graph/steiner-tree/）
 * ============================================================
 * 使用示例（编译时取消注释；洛谷 P6192）：
 * signed main() {
 *   int n, m, k;
 *   cin >> n >> m >> k;
 *   vector<array<int, 3>> es;
 *   for (int i = 0; i < m; ++i) {
 *     int u, v, w;
 *     cin >> u >> v >> w;
 *     es.push_back({u, v, w});
 *   }
 *   vector<int> keys(k);
 *   for (int& x : keys) cin >> x;
 *   cout << steiner_tree(n, es, keys) << '\n';
 * }
 * ============================================================
 */
