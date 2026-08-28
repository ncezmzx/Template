#include <bits/stdc++.h>
using namespace std;
#define int long long

// Stoer-Wagner 全局最小割（无向正权图，允许重边）
constexpr int N = 605;
int edge[N][N];        // 邻接矩阵（1-indexed，重边累加）
int dist[N], vis[N], bin[N];  // bin[i]：i 已被合并掉
int n_;

void sw_init(int n) {
  n_ = n;
  for (int i = 1; i <= n; ++i) {
    bin[i] = 0;
    for (int j = 1; j <= n; ++j) edge[i][j] = 0;
  }
}
void sw_add(int u, int v, int w) { edge[u][v] += w, edge[v][u] += w; }

// 一轮 contract：求当前图的 cut-of-phase；s=倒数第二个加入，t=最后加入
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
int stoer_wagner() {
  int ans = LLONG_MAX, s = 0, t = 0;
  for (int i = 1; i < n_; ++i) {
    int cur = contract_(s, t);
    bin[t] = 1;
    ans = min(ans, cur);
    if (ans == 0) return 0;  // 图不连通：全局最小割为 0
    for (int j = 1; j <= n_; ++j)
      if (!bin[j]) edge[s][j] = (edge[j][s] += edge[j][t]);
  }
  return ans;
}

/*
 * ============================================================
 * 名称：Stoer-Wagner 全局最小割
 * 复杂度：O(nm + n² log n)（朴素写法 O(n³)，n ≤ 600 可用）
 * 用途：无向正权图的最小割（不指定源汇），比"枚举源汇跑 n 次
 *       最大流"快得多；图不连通时答案为 0
 * 接口：sw_init(n)；sw_add(u, v, w)（重边自动累加）；
 *       stoer_wagner() 返回全局最小割
 * 原理：任意两点 s,t 要么同属一个割块、要么被某个 s-t 割分开。
 *       每轮用"最大邻接权"次序（类似 Prim 地按 w(A,·) 贪心）求出
 *       s-t 最小割（cut-of-phase），再把 t 合并进 s（边权叠加），
 *       答案取各轮最小值。共 n-1 轮。
 * 注意：1-indexed；边权应为非负（0 权会被提前 return 0）；N 按点数
 *       调整；n < 2 时无意义
 * 来源：OI-Wiki《Stoer–Wagner 算法》（https://oi-wiki.org/graph/stoer-wagner/）
 * ============================================================
 * 使用示例（编译时取消注释；洛谷 P5632）：
 * signed main() {
 *   int n, m;
 *   cin >> n >> m;
 *   sw_init(n);
 *   for (int i = 0; i < m; ++i) {
 *     int u, v, w;
 *     cin >> u >> v >> w;
 *     sw_add(u, v, w);
 *   }
 *   cout << stoer_wagner() << '\n';
 * }
 * ============================================================
 */
