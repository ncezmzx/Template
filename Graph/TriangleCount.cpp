#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr int N = 1e5 + 9, M = 2e5 + 9;
int n, m, deg[N];
pair<int, int> e[M];
vector<int> g[N];

long long count_triangles() {
  for (int i = 1; i <= m; ++i) {
    int u = e[i].first, v = e[i].second;
    if (deg[u] > deg[v] || (deg[u] == deg[v] && u > v)) swap(u, v);
    g[u].push_back(v);
  }
  vector<char> tag(n + 1, 0);
  long long ans = 0;
  for (int u = 1; u <= n; ++u) {
    for (int v : g[u]) tag[v] = 1;
    for (int v : g[u])
      for (int w : g[v])
        if (tag[w]) ++ans;
    for (int v : g[u]) tag[v] = 0;
  }
  return ans;
}

/*
 * ============================================================
 * 名称：三元环计数（无向图，O(m√m)）
 * 复杂度：O(m√m)（重定向后每个点出度 O(√m)，总复杂度 O(m√m)）
 * 用途：统计无向图中长度为 3 的环（三角形）个数；思路可推广到四元环、
 *       特殊结构计数；也是"图定向技巧"的入门题
 * 原理：给每条边定向：度数小 → 度数大（度数相同按编号小 → 大），得到 DAG；
 *       每个三元环在"度最小的顶点"处被唯一统计：枚举 u 的邻点 v，再枚举 v 的
 *       邻点 w，若 w 也是 u 的邻点（tag 标记）则 (u,v,w) 构成三元环
 * 注意：重边需先合并（本模板默认无重边）；e[1..m] 为无向边表；
 *       大根堆式 tag 数组每次清零是 O(出度)，总代价 O(∑出度) = O(m)
 * 用法：读入边后 count_triangles() 返回三角形个数
 * ============================================================
 * 使用示例（编译时取消注释）：
 * signed main() {
 *   cin >> n >> m;
 *   for (int i = 1; i <= m; ++i) {
 *     cin >> e[i].first >> e[i].second;
 *     ++deg[e[i].first], ++deg[e[i].second];
 *   }
 *   cout << count_triangles() << '\n';
 * }
 * ============================================================
 */
