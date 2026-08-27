#include <bits/stdc++.h>
using namespace std;

constexpr int N = 2e4 + 10;
int n, m, q, dfn[N], low[N], stk[N], tp, sq, idx, dep[N << 1], fa[N << 1][20];
std :: vector < int > g[N], e[N << 1];
void tarjan (int u) {
  dfn[u] = low[u] = ++idx;
  stk[++tp] = u;
  for (int v : g[u]) {
    if (!dfn[v]) {
      tarjan (v);
      low[u] = std :: min (low[u], low[v]);
      if (low[v] >= dfn[u]) {
        e[u].push_back (++sq);
        e[sq].push_back (u);
        for (int x = 0; x != v; ) {
          e[x = stk[tp--]].push_back (sq);
          e[sq].push_back (x);
        }
      }
    }
    else low[u] = std :: min (low[u], dfn[v]);
  }
}
void dfs (int u, int f) {
  dep[u] = dep[f] + 1;
  fa[u][0] = f;
  for (int i = 1; fa[u][i - 1]; i++) fa[u][i] = fa[fa[u][i - 1]][i - 1];  // 越顶即止（默认 0）
  for (int v : e[u]) {
    if (v == f) continue;
    dfs (v, u);
  }
}
int lca (int x, int y) {
  if (dep[x] < dep[y]) std :: swap (x, y);
  for (int i = 19; i >= 0; i--) {
    if (dep[fa[x][i]] >= dep[y]) {
      x = fa[x][i];
    }
  }
  if (x == y) return x;
  for (int i = 19; i >= 0; i--) {
    if (fa[x][i] != fa[y][i]) {
      x = fa[x][i], y = fa[y][i];
    }
  }
  return fa[x][0];
}

/*
 * ============================================================
 * 名称：Tarjan 点双连通分量（v-BCC）/ 圆方树（block-cut tree）
 * 复杂度：O(n + m)
 * 用途：无向图求点双连通分量并构建圆方树（BCC 缩成"方点"，原图点叫"圆点"，
 *       圆点与其所在 BCC 的方点连边）；割点 = 圆方树中度 >= 2 的圆点。
 *       注意：调用前需置 sq = n，使方点从 n+1 开始编号（圆方树节点总数为 sq，
 *       邻接表 e 按 2N 开）
 * 来源：all.cpp 行 55913-55931（tarjan 原样保留；sq = n 取自 55972；dfs/lca 55932-55955
 *       为同一代码块附带，一并保留；注释已统一移至文件尾部）
 * ============================================================
 * 使用示例（编译时取消注释）：
 * signed main() {
 *   cin.tie(nullptr)->sync_with_stdio(false);
 *   cin >> n >> m;
 *   for (int i = 1, x, y; i <= m; ++i) {
 *     cin >> x >> y;
 *     g[x].push_back (y), g[y].push_back (x);
 *   }
 *   sq = n;                               // 与原题 main（all.cpp 55972）一致：方点从 n+1 开始编号
 *   for (int i = 1; i <= n; ++i)
 *     if (!dfn[i]) tarjan (i);
 *   dfs (1, 0);   // 连通图示例；不连通时对每个分量分别 dfs
 *   cout << "BCC count: " << sq - n << '\n';
 *   cout << "cut vertices:";
 *   for (int i = 1; i <= n; ++i)
 *     if ((int)e[i].size() >= 2) cout << ' ' << i;   // 圆点与 >=2 个方点相邻（度 >= 2）即割点
 *   cout << '\n';
 *   cout.flush();   // 演示用：确保缓冲输出落盘（OJ 正常退出会自动 flush）
 * }
 * ============================================================
 */
