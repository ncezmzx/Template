/*
 * 名称：斜率优化 DP（凸包技巧 CHT，单调队列维护）+ WQS 二分
 * 复杂度：calc(x) 单次 O(n)（每个点进出队一次），WQS 二分 O(log V) 次，总 O(n log V)
 * 用途：形如 f[i] = min_j { f[j] + w(j,i) } 且 w 满足四边形不等式的分段 DP；
 *       WQS 二分用于去掉"恰好分成 k 段"的段数限制：给每段代价减去 x（calc 中 - x），
 *       二分 x 使最优分段数 g[n] 收敛到 k，最终答案 = calc(l).first + l * k。
 * 使用示例（DEMO main）：把 a[1..n] 分成恰好 k 段，最小化 sum(段和 + 1)^2。
 *       输入：n k，随后 n 个数 a[i]；输出：最小总代价。
 * 来源：all.cpp 28756-28807（原样抽取；注释掉的 cerr 调试代码一并保留）
 * 变体：本文件末尾注释给出另外两种斜率优化写法
 *       （all.cpp 26430-26480 分数形式 calc；all.cpp 54539-54610 经典 y/x 斜率）。
 */
#include <bits/stdc++.h>
using namespace std;
#define int long long
using d64 = long double;
constexpr int N = 1e5 + 9;
constexpr d64 eps = 1e-9;
int n, k, a[N], s[N], f[N], g[N], q[N], hd, tl;
d64 x(int i) { return s[i]; }
d64 y(int i) { return f[i] + s[i] * s[i]; }
d64 slope(int i, int j) {
  return (y(j) - y(i)) / (x(j) - x(i));
}
pair<int, int> calc(int x) {
  for (int i = 0; i <= n; ++i) f[i] = g[i] = 0;
  hd = tl = 1, q[1] = 0;
  for (int i = 1; i <= n; ++i) {
    while (hd < tl && slope(q[hd], q[hd + 1]) < eps + 2 * (s[i] + 1)) ++hd;
    int j = q[hd];
    g[i] = g[j] + 1, f[i] = f[j] + (s[i] - s[j] + 1) * (s[i] - s[j] + 1) - x;
    while (hd < tl && slope(q[tl - 1], q[tl]) + eps > slope(q[tl], i)) --tl;
    q[++tl] = i;
    // cerr << f[i] << ' ' << j << ' ';
  }
  // cerr << '\n';
  // for (int i = 1; i <= n; ++i) {
  //   f[i] = 0x3f3f3f3f3f3f3f3f, g[i] = 0;
  //   int p = 0;
  //   for (int j = 0; j < i; ++j) {
  //     if (f[j] + (s[i] - s[j] + 1) * (s[i] - s[j] + 1) - x < f[i])
  //       f[i] = f[j] + (s[i] - s[j] + 1) * (s[i] - s[j] + 1) - x, p = j;
  //     if (f[j] + (s[i] - s[j] + 1) * (s[i] - s[j] + 1) - x == f[i] && g[j] + 1 > g[i]) p= j, g[i] = max(g[i], g[j] + 1);
  //   }
  //   cerr << f[i] << ' ' << p << ' ';
  // }
  // cerr << '\n';
  return {f[n], g[n]};
}
#ifdef DEMO
// 使用示例：n 个数分成恰好 k 段，每段代价 = (段和 + 1)^2，求最小总代价
// 输入：n k 回车，随后 n 个整数
signed main() {
  cin.tie(nullptr)->sync_with_stdio(false);
  cin >> n >> k;
  for (int i = 1; i <= n; ++i) cin >> a[i], s[i] = s[i - 1] + a[i];
  // auto [p, q] = calc(-485);
  // cerr << p << ' ' << q << '\n';
  int l = -2e16, r = 2e16;
  while (l < r) {
    int m = (l + r) >> 1;
    if (calc(m).second >= k) r = m;
    else l = m + 1;
  }
  // cerr << l;
  cout << calc(l).first + l * k;
}
#endif

/*
 * ── 变体 1（all.cpp 26430-26480，分数形式的斜率优化）────────────────────────
 * 分组均值类问题：calc(l, j) 直接比较两个候选端点的"分数值"，
 * 配合 cmax 更新（原文用 cmax 而非直接赋值）：
 *
 * int n, k, p, h[N], q[N], hd, tl, H, s[N];
 * pair<short, bool> g[N][N];
 * d64 f[N], F[N];
 * d64 x(int i) { return i - 1; }
 * d64 y(int i) { return F[i] - s[i]; }
 * d64 slope(int i, int j) {
 *   return (y(j) - y(i)) / (x(j) - x(i));
 * }
 * d64 calc(int l, int j) {
 *   return (F[l] - s[l] + s[j]) / (j - l + 1);
 * }
 * // 主循环骨架：
 * //   q[hd = tl = 1] = 0;
 * //   for (int j = 1; j <= n; ++j) {
 * //     while (hd < tl && calc(q[hd], j) < calc(q[hd + 1], j)) ++hd;
 * //     g[i][j] = {j - 1, 0}, f[j] = f[j - 1];
 * //     if (cmax(f[j], calc(q[hd], j))) g[i][j] = {q[hd], 1};
 * //     while (hd < tl && slope(q[tl - 1], q[tl]) + eps <= slope(q[tl], j)) --tl;
 * //     q[++tl] = j;
 * //   }
 * // 依赖的辅助函数（原文 all.cpp 26427）：
 * //   template<class T> bool cmax(T& x, const T &y) { if (x <= y) return x = y, true; return false; }
 *
 * ── 变体 2（all.cpp 54539-54610，经典 y/x 斜率 + 单调队列）─────────────────
 * 2a. 对 m 个斜率询问的静态凸包（54539-54560，处理 x 相同即竖直线的情况）：
 *
 * double slope(int x, int y) {
 *   return a[y].x == a[x].x ? (a[y].y < a[x].y ? -inf : inf) : 1. * (a[y].y - a[x].y) / (a[y].x - a[x].x);
 * }
 * // 建下凸包：for (int i = 1; i <= n; ++i) {
 * //             while (l < r && slope(q[r - 1], q[r]) <= slope(q[r], i)) --r;
 * //             q[++r] = i;
 * //           }
 * // 按斜率排序后查询：while (l < r && slope(q[l], q[l + 1]) >= b[i].k) ++l;
 * //                  ans[b[i].i] = -b[i].k * a[q[l]].x + a[q[l]].y;
 *
 * 2b. 决策单调 DP 版（54571-54604，getx/gety 封装坐标，斜率随 b[i].w 单调）：
 *
 * int getx(int x) { return -b[x + 1].h; }
 * int gety(int x) { return dp[x]; }
 * double slope(int x, int y) {
 *   return 1. * (gety(y) - gety(x)) / (getx(y) - getx(x));
 * }
 * // 主循环：
 * //   q[++r] = 0;
 * //   for (int i = 1; i <= m; ++i) {
 * //     while (l < r && slope(q[l], q[l + 1]) <= b[i].w) ++l;
 * //     dp[i] = b[i].w * b[q[l] + 1].h + dp[q[l]];
 * //     while (l < r && slope(q[r - 1], q[r]) >= slope(q[r], i)) --r;
 * //     q[++r] = i;
 * //   }
 */
