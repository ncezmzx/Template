/*
 * 名称：数位 DP（二进制版；计数 [0, x] 内二进制表示恰有 k 个 1 的数的个数）
 * 复杂度：O(位数 * k * 2)（记忆化 dp[p][q][top]，p ~ 60，q <= k）
 * 用途：数位 DP 的通用骨架——dfs(p, q, top)：从高位向低位填，p 为当前处理到的位，
 *       q 为已使用状态，top 表示之前各位是否都顶到上界（限制下一位取值上限 up）；
 *       sol(x) 把 x 拆成二进制位 a[1..c]（a[1] 为最低位，dp 从高到低枚举），
 *       返回满足条件的个数；记忆化只对非顶格（top == 0）的状态生效。
 *       本题即"1..x 中恰好 k 个 1"的计数，配合二分/容斥可解决
 *       "区间 [l, r] 内满足某数位性质的数"一类问题。
 * 使用示例（DEMO main）：求最小的 x，使 (x, 2x] 内恰有 k 个二进制 1 的数不少于 m 个。
 *       输入：m k 回车；输出：最小 x。
 * 来源：all.cpp 2602-2631（原样抽取）。十进制数位 DP（含前导零 pre 标记、聚合
 *       和的版本）见文件末尾注释（all.cpp 50486-50519）。
 */
#include <bits/stdc++.h>
using namespace std;
#define int long long
int m, k, dp[100][100][2], c, a[100];
int dfs(int p, int q, int top) {
  if (q < 0) return 0;
  if (!p) return q == 0;
  auto& ret = dp[p][q][top];
  if (~ret) return ret;
  ret = 0;
  for (int i = 0, up = top ? a[p] : 1; i <= up; ++i) ret += dfs(p - 1, q - i, top && i == up);
  return ret;
}
int sol(int x) {
  memset(dp, -1, sizeof dp);
  for (c = 0; x; x >>= 1) a[++c] = x & 1;
  return dfs(c, k, 1);
}
bool chk(int x) {
  return sol(2 * x) - sol(x) >= m;
}
#ifdef DEMO
// 使用示例：输入 m k，求最小 x 满足 (x, 2x] 内恰含 k 个二进制 1 的数 >= m 个
signed main() {
  cin.tie(nullptr)->sync_with_stdio(false);
  cin >> m >> k;
  int l = 1, r = 1e18;
  while (l < r) {
    int mid = (l + r) >> 1;
    if (chk(mid)) r = mid;
    else l = mid + 1;
  }
  cout << l;
  return 0;
}
#endif

/*
 * ── 十进制数位 DP 变体（all.cpp 50486-50519，含 pre 前导零标记与聚合求和）──────
 * 十进制逐位拆解用 a[++c] = x % 10；状态除 top 外还有 pre（是否仍为前导零），
 * 记忆化条件 !top && !pre；返回结构体 p0/p1/p2 分别聚合 个数/和/平方和。
 *
 * int a[20], c, pw[20];
 * struct info { int p0, p1, p2; info() = default; } dp[20][10][1 << 10];
 * info dfs(int p, bool top, bool pre, int S) {
 *   if (!p) return {__builtin_popcount(S) == s, 0, 0};
 *   if (!top && !pre && ~dp[p][s][S].p0) return dp[p][s][S];
 *   info ret = {0, 0, 0};
 *   int up = (top ? a[p] : 9);
 *   for (int i = 0; i <= up; ++i) {
 *     int nS = prg(S, i);
 *     auto nw = dfs(p - 1, top && (i == up), pre && (i == 0), (pre && (i == 0)) ? 0 : nS);
 *     int d = i * pw[p - 1] % md;
 *     (ret.p0 += nw.p0) %= md;
 *     (ret.p1 += nw.p1 + nw.p0 * d) %= md;
 *     (ret.p2 += nw.p2 + 2 * nw.p1 * d + d * d % md * nw.p0) %= md;
 *   }
 *   if (!top && !pre) dp[p][s][S] = ret;
 *   return ret;
 * }
 * int f(int x) {
 *   for (c = 0; x; x /= 10) a[++c] = x % 10;
 *   auto nw = dfs(c, true, true, 0);
 *   ...
 * }
 */
