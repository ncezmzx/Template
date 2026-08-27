#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr int N = 2e5 + 9;
int n, m, dfn[N], id[N], idx, fa[N];
vector<int> g[N], rg[N], bucket[N];
int sdom[N], idom[N], mn[N], f[N];

int find(int x) {
  if (f[x] == x) return x;
  int y = find(f[x]);
  if (sdom[y] < sdom[mn[x]]) mn[x] = y;
  f[x] = f[f[x]];
  return mn[x];
}

void dfs(int x) {
  dfn[x] = ++idx, id[idx] = x;
  for (int y : g[x])
    if (!dfn[y]) fa[y] = x, dfs(y);
}

void dominator(int s) {
  idx = 0;
  dfs(s);
  for (int i = 1; i <= n; ++i) sdom[i] = dfn[i], mn[i] = f[i] = i;
  for (int i = idx; i >= 2; --i) {
    int x = id[i];
    for (int y : rg[x]) {
      if (!dfn[y]) continue;
      int w = find(y);
      if (sdom[w] < sdom[x]) sdom[x] = sdom[w];
    }
    bucket[id[sdom[x]]].push_back(x);
    f[x] = fa[x];
    for (int y : bucket[fa[x]]) {
      int w = find(y);
      idom[y] = sdom[w] == sdom[y] ? fa[x] : w;
    }
    bucket[fa[x]].clear();
  }
  for (int i = 2; i <= idx; ++i) {
    int x = id[i];
    if (idom[x] != id[sdom[x]]) idom[x] = idom[idom[x]];
  }
  idom[s] = 0;
}

/*
 * ============================================================
 * 名称：支配树（Lengauer-Tarjan 算法）
 * 复杂度：O((n + m) log n)（find 带路径压缩的并查集），实现为 O((n+m) α)
 * 用途：给定有向图与源点 s，求每个点 x 的最近支配点 idom[x]：
 *       所有从 s 到 x 的路径都必须经过 idom[x]（x 的直接支配者）；
 *       idom 关系构成一棵树（支配树），x 的支配点 = 支配树上的祖先
 *       应用：必经点/必经边统计、程序控制流图、删点后连通性等
 * 原理：先在原图上 DFS 求 dfn/fa；按 dfn 逆序计算半支配点 sdom（利用
 *       路径压缩并查集维护"当前最小的 sdom 候选"），再修正为 idom
 * 注意：仅处理从 s 可达的点（dfn 非零）；不可达点不进入支配树；
 *       idom[s] = 0 表示源点无支配者；重边/自环不影响正确性
 * 用法：加图后 dominator(1)；idom[x] 即 x 的最近支配点（0 表示源点）
 * ============================================================
 * 使用示例（编译时取消注释；输出每个点的直接支配点）：
 * signed main() {
 *   cin >> n >> m;
 *   for (int i = 1, u, v; i <= m; ++i) {
 *     cin >> u >> v;
 *     g[u].push_back(v), rg[v].push_back(u);
 *   }
 *   dominator(1);
 *   for (int i = 1; i <= n; ++i) cout << idom[i] << " \n"[i == n];
 * }
 * ============================================================
 */
