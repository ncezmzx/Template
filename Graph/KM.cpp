#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr int N = 505;
constexpr int INF = 0x3f3f3f3f3f3f3f3f;
int n, w[N][N], lx[N], ly[N], match[N], slack[N], pre[N], visx[N], visy[N];

void augment(int s) {
  for (int i = 1; i <= n; ++i) visx[i] = visy[i] = 0, slack[i] = INF;
  int y = 0, ny = 0;
  match[0] = s;
  while (match[y]) {
    int x = match[y], d = INF;
    visy[y] = 1;
    for (int i = 1; i <= n; ++i)
      if (!visy[i]) {
        if (lx[x] + ly[i] - w[x][i] < slack[i])
          slack[i] = lx[x] + ly[i] - w[x][i], pre[i] = y;
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

int km() {
  for (int i = 1; i <= n; ++i) lx[i] = -INF, ly[i] = 0, match[i] = 0;
  for (int i = 1; i <= n; ++i)
    for (int j = 1; j <= n; ++j) lx[i] = max(lx[i], w[i][j]);
  for (int i = 1; i <= n; ++i) augment(i);
  int res = 0;
  for (int i = 1; i <= n; ++i) res += w[match[i]][i];
  return res;
}

/*
 * ============================================================
 * 名称：KM 算法（二分图最大权完美匹配，DFS + slack 优化）
 * 复杂度：O(n^3)
 * 用途：n×n 带权二分图求最大权完美匹配（权值可为负：lx 初始 -INF 的写法
 *       自动支持）；结果 match[i] = 与右部点 i 匹配的左部点，总权值和返回
 * 原理：顶标法（Kuhn–Munkres）：维护左部顶标 lx、右部顶标 ly 满足
 *       lx[u]+ly[v] >= w[u][v]，相等子图（取等号的边）中找完美匹配；
 *       augment 沿交替树调整顶标（min slack）并扩展相等子图，O(n) 轮
 * 注意：本实现针对稠密 n ≤ 500 的方阵；n 不相等的二分图请补零权虚点；
 *       求最小权完美匹配把权值取负即可
 * ============================================================
 * 使用示例（编译时取消注释；完整 n×n 权值矩阵，权值可为负）：
 * signed main() {
 *   cin >> n;
 *   for (int i = 1; i <= n; ++i)
 *     for (int j = 1; j <= n; ++j) cin >> w[i][j];
 *   cout << km() << '\n';
 *   for (int i = 1; i <= n; ++i) cout << match[i] << ' ';
 * }
 * ============================================================
 */
