#include <bits/stdc++.h>
using namespace std;

/*
 * 名称：LCA（倍增 / 二进制提升，Binary Lifting）
 * 复杂度：预处理 O(n log n)，单次查询 O(log n)
 * 用途：静态树上求最近公共祖先 lca(x, y)
 * 实现：dfs 预处理 dep 与倍增表 fa[u][i]（u 的 2^i 级祖先）；
 *       lca 先把较深的点抬到同一深度，再一起倍增上跳，最后返回 fa[x][0]。
 * 使用示例：
 *   dfs (1, 0);        // 预处理 dep / fa
 *   lca (x, y);
 * 来源：all.cpp 行 55932-55955（dfs 与 lca 原样保留；原代码块在圆方树上使用，
 *       故数组开 N<<1，普通树可把 dep/fa/e 改回 N 大小）
 */
constexpr int N = 2e4 + 10;
int n, dep[N << 1], fa[N << 1][20];       // 原代码在圆方树上用（节点数可达 n+BCC 数），普通树可改回 [N]
std :: vector < int > e[N << 1];          // 树邻接表
void dfs (int u, int f) {
  dep[u] = dep[f] + 1;
  fa[u][0] = f;
  for (int i = 1; i < 20; i++) fa[u][i] = fa[fa[u][i - 1]][i - 1];
  for (int v : e[u]) {
    if (v == f) continue;
    dfs (v, u);
  }
}
int lca (int x, int y) {
  if (dep[x] < dep[y]) std :: swap (x, y);
  for (int i = 19; i >= 0; i--) {
    if (dep[fa[x][i]] >= dep[y]) {        // 把 x 抬到与 y 同深度
      x = fa[x][i];
    }
  }
  if (x == y) return x;
  for (int i = 19; i >= 0; i--) {         // 一起倍增上跳，停在 LCA 的孩子处
    if (fa[x][i] != fa[y][i]) {
      x = fa[x][i], y = fa[y][i];
    }
  }
  return fa[x][0];
}

#ifdef DEMO
signed main() {
  cin.tie(nullptr)->sync_with_stdio(false);
  cin >> n;                               // n 个点的树
  for (int i = 1, x, y; i < n; ++i) {
    cin >> x >> y;
    e[x].push_back (y), e[y].push_back (x);
  }
  dfs (1, 0);
  int q;
  cin >> q;
  while (q--) {
    int x, y;
    cin >> x >> y;
    cout << lca (x, y) << '\n';
  }
  cout.flush();   // 演示用：确保缓冲输出落盘（OJ 正常退出会自动 flush）
}
#endif
