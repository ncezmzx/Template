/*
 * 名称：单调队列（滑动窗口最值，deque 实现）
 * 复杂度：O(n)（每个下标至多入队/出队一次）
 * 用途：长度为 k 的滑动窗口内取最小/最大值；也可用于单调队列优化 DP（决策点随窗口滑动）。
 * 维护规则：队首是当前窗口的最值下标；入队前先把窗口外（q.front() < 窗口左端点）的弹出，
 *       再把队尾"不优"（比新元素更小/更大）的元素弹出（保持队内单调），然后入队。
 * 使用示例（DEMO main）：对 a[1..n][1..m] 的每一行求长度为 y 的滑动窗口最大值，
 *       结果存 mx[i][j - y + 1]。
 * 来源：all.cpp 48897-48903（原为二维滑动窗口题中"逐行"的窗口维护循环，原样抽取；
 *       声明沿用原块 48882-48884）。数组版单调队列（q[hd..tl]）见文件末尾注释，
 *       取自 all.cpp 28770-28776（斜率优化源块），非本人编写。
 */
#include <bits/stdc++.h>
using namespace std;
#define int long long
constexpr int N = 1e3 + 9;
int n, m, a[N][N], t, s[N][N], mx[N][N];

#ifdef DEMO
// 使用示例：n 行 m 列矩阵，每行求长度 y 的滑动窗口最大值 → mx[i][j - y + 1]
// 输入：n m 回车，n 行每行 m 个数，最后一行 y
signed main() {
  cin.tie(nullptr)->sync_with_stdio(false);
  cin >> n >> m;
  for (int i = 1; i <= n; ++i)
    for (int j = 1; j <= m; ++j) cin >> a[i][j];
  int y;
  cin >> y;
  for (int i = 1; i <= n; ++i) {
    deque<int> q;
    for (int j = 1; j <= m; ++j) {
      while (!q.empty() && q.front() < j - y + 1) q.pop_front();
      while (!q.empty() && a[i][q.back()] < a[i][j]) q.pop_back();
      q.push_back(j);
      if (j >= y) mx[i][j - y + 1] = a[i][q.front()];
    }
  }
  for (int i = 1; i <= n; ++i)
    for (int j = 1; j + y - 1 <= m; ++j) cout << mx[i][j] << " \n"[j + y - 1 == m];
  return 0;
}
#endif

/*
 * ── 数组版单调队列（q[hd..tl]，源 all.cpp 28770-28776，斜率优化块内原样）──────
 * int q[N], hd, tl;
 * q[hd = tl = 1] = 0;                    // 初始化：队首队尾指向 1，先放入 0 号决策点
 * for (int i = 1; i <= n; ++i) {
 *   while (hd < tl && <队首不优条件>) ++hd;   // 弹出队首（队首不再是最优决策）
 *   int j = q[hd];                          // 取队首为最优决策点
 *   ... 用 j 转移 ...
 *   while (hd < tl && <队尾破坏凸性/单调性条件>) --tl;  // 弹出队尾保持单调
 *   q[++tl] = i;                            // 新决策点入队
 * }
 * 求最小值时队内下标对应的值单调不减（队首最小），比较条件符号相应取反。
 *
 * ── 其他相关写法 ──────────────────────────────────────────────────────────
 * 1) all.cpp 4936-4946：单调队列 push 封装（取滑动窗口最大值，交互题片段）：
 *    deque<int> q;
 *    void push(int x) { while (!q.empty() && a[q.back()] < a[x]) q.pop_back(); q.push_back(x); }
 *    // 过期弹出：while (!q.empty() && q.front() < lft) q.pop_front();
 * 2) all.cpp 54351-54366：单调队列优化 DP（跳 k 步的最少"惩罚"次数）：
 *    q.clear(); q.push_back(1);
 *    for (int i = 2; i <= n; ++i) {
 *      while (!q.empty() && i - q.front() > k) q.pop_front();
 *      dp[i] = dp[q.front()] + (d[i] >= d[q.front()]);
 *      while (!q.empty() && ((dp[q.back()] == dp[i] && d[q.back()] < d[i]) || dp[q.back()] > dp[i])) q.pop_back();
 *      q.push_back(i);
 *    }
 */
