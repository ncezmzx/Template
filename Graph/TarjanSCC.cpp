#include <bits/stdc++.h>
using namespace std;

#define int long long
template<class T> inline bool cmin(T &x, T y) { if (x > y) return x = y, true; return false; }
constexpr int N = 5e5 + 9;
int n, m, dfn[N], stk[N], low[N], col[N], vst[N], cl, idx, tp;
vector<int> es[N];
void tarjan(int x) {
  low[x] = dfn[x] = ++idx, stk[++tp] = x, vst[x] = true;
  for (int y : es[x]) {
    if (!dfn[y]) tarjan(y), cmin(low[x], low[y]);
    else if (vst[y]) cmin(low[x], dfn[y]);
  }
  if (low[x] == dfn[x]) {
    ++cl;
    for (int u = 0; u != x; vst[u] = false) col[u = stk[tp--]] = cl;
  }
}

/*
 * ============================================================
 * 名称：Tarjan 强连通分量（SCC）+ 缩点
 * 复杂度：O(n + m)
 * 用途：有向图求强连通分量；缩点后可得到 DAG 做拓扑 DP 等；
 *       缩点方法：对每条原边 (x, y)，若 col[x] != col[y]，在新图中加边 col[x] -> col[y]
 * 来源：all.cpp 行 14111-14129（原样保留；注释已统一移至文件尾部）
 * ============================================================
 * 使用示例（编译时取消注释）：
 * signed main() {
 *   cin.tie(nullptr)->sync_with_stdio(false);
 *   cin >> n >> m;
 *   for (int i = 1, x, y; i <= m; ++i) {
 *     cin >> x >> y;
 *     es[x].push_back(y);              // 有向边
 *   }
 *   for (int i = 1; i <= n; ++i)
 *     if (!dfn[i]) tarjan(i);
 *   cout << cl << '\n';                // SCC 个数
 *   for (int i = 1; i <= n; ++i) cout << col[i] << " \n"[i == n];
 *   cout.flush();   // 演示用：确保缓冲输出落盘（OJ 正常退出会自动 flush）
 * }
 * ============================================================
 */
