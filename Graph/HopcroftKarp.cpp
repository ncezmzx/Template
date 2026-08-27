#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr int N = 1e5 + 9;
int n, m, nl, nr, dis[N], match[N], vis[N];
vector<int> g[N];

bool bfs() {
  queue<int> q;
  bool f = false;
  for (int i = 1; i <= nl; ++i)
    if (!match[i]) dis[i] = 0, q.push(i);
    else dis[i] = -1;
  dis[0] = -1;
  while (!q.empty()) {
    int u = q.front();
    q.pop();
    for (int v : g[u]) {
      if (dis[match[v]] == -1) {
        dis[match[v]] = dis[u] + 1;
        if (match[v]) q.push(match[v]);
        else f = true;
      }
    }
  }
  return f;
}

bool dfs(int u) {
  for (int v : g[u]) {
    if (match[v] && dis[match[v]] != dis[u] + 1) continue;  // 层级检查先于 vis
    if (vis[v]) continue;
    vis[v] = 1;
    if (!match[v] || dfs(match[v])) {
      match[u] = v, match[v] = u;
      return true;
    }
  }
  dis[u] = -1;
  return false;
}

int hopcroft_karp() {
  int ans = 0;
  while (bfs()) {
    memset(vis, 0, sizeof vis);
    for (int i = 1; i <= nl; ++i)
      if (!match[i] && dfs(i)) ++ans;
  }
  return ans;
}

/*
 * ============================================================
 * 名称：Hopcroft–Karp（二分图最大匹配）
 * 复杂度：O(E sqrt(V))
 * 用途：二分图最大匹配（nl 个左部点、nr 个右部点，左部点 1..nl、右部点
 *       1..nr 或映射后使用）；匹配结果存 match[]（match[u]=v 且 match[v]=u）
 * 原理：BFS 按增广路长度分层（dis，dist 表示左部点），DFS 只沿"下一层"
 *       找增广路（多路增广），每轮找到当前长度的所有最短增广路；
 *       轮数 O(sqrt(V))，每轮 O(E)
 * 注意：右部点需与左部点编号不冲突（本模板 match 数组共用，右部点建议
 *       映射到 nl+1..nl+nr 或使用分离编号）；多组数据清空 g/match
 * ============================================================
 * 使用示例（编译时取消注释）：
 * signed main() {
 *   cin >> nl >> nr >> m;
 *   for (int i = 1, u, v; i <= m; ++i) {
 *     cin >> u >> v;
 *     g[u].push_back(v + nl);   // 右部点映射到 nl+1..nl+nr
 *   }
 *   nr += nl;
 *   cout << hopcroft_karp() << '\n';
 * }
 * ============================================================
 */
