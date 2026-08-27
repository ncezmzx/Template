#include <bits/stdc++.h>
using namespace std;

#define int long long
constexpr int N = 3e5 + 9;
int n, m, dfn[N], idx, stk[N], tp, low[N], vis[N], cl, col[N];
vector<pair<int, int>> es[N];
int etot;
void add(int x, int y) {
  es[x].emplace_back(y, ++etot);
  es[y].emplace_back(x, etot);
}
void tarjan(int x, int lst) {
  dfn[x] = low[x] = ++idx, vis[x] = 1, stk[++tp] = x;
  for (auto [y, z] : es[x]) {
    if (lst == z) continue;
    if (!dfn[y]) tarjan(y, z), low[x] = min(low[x], low[y]);
    else if (vis[y]) low[x] = min(low[x], dfn[y]);
  }
  if (dfn[x] == low[x]) {
    ++cl;
    while (stk[tp] != x) col[stk[tp--]] = cl;
    col[stk[tp--]] = cl;
  }
}

/*
 * ============================================================
 * 名称：Tarjan 桥 / 边双连通分量（e-DCC）
 * 复杂度：O(n + m)
 * 用途：无向图按"删去桥后"的连通分量（边双连通分量）缩点；桥的判定：
 *       边 (x, fa) 是桥 当且仅当处理完 x 后 dfn[x] == low[x]；tarjan(int x, int lst)
 *       中 lst 为到父亲的边编号，可正确处理重边（重边编号不同，不会被 lst 跳过）
 * 来源：all.cpp 行 51899-51912（原样保留；另见 13981-13995 的类似写法；注释已统一移至文件尾部）
 * ============================================================
 * 使用示例（编译时取消注释）：
 * signed main() {
 *   cin.tie(nullptr)->sync_with_stdio(false);
 *   cin >> n >> m;
 *   for (int i = 1, x, y; i <= m; ++i) {
 *     cin >> x >> y;
 *     add(x, y);
 *   }
 *   for (int i = 1; i <= n; ++i)
 *     if (!dfn[i]) tarjan(i, 0);
 *   cout << cl << '\n';                    // 边双连通分量个数
 *   for (int i = 1; i <= n; ++i) cout << col[i] << " \n"[i == n];
 *   cout.flush();   // 演示用：确保缓冲输出落盘（OJ 正常退出会自动 flush）
 * }
 * ============================================================
 */
