#include <bits/stdc++.h>
using namespace std;

/*
 * 名称：LCA（欧拉序 + ST 表 RMQ）
 * 复杂度：预处理 O(n log n)，单次查询 O(1)
 * 用途：静态树上求最近公共祖先 lca(x, y) 与树上距离 dist(x, y)
 * 实现：dfs 时令 mi[0][dfn[x]] = 父亲（欧拉序），ST 表对"深度较浅的点"做 RMQ，
 *       区间 [dfn[x], dfn[y]] 中深度最浅的点即 lca。
 * 使用示例：
 *   dfs(1, 0);                              // 求 dfn / dep / mi[0]
 *   for (int i = 1; i < 20; ++i)            // 建 ST 表
 *     for (int j = 1; j + (1 << i) - 1 <= n; ++j)
 *       mi[i][j] = get(mi[i - 1][j], mi[i - 1][j + (1 << (i - 1))]);
 *   lca(x, y);  dist(x, y);
 * 来源：all.cpp 行 7965-7990（原样保留；另见 51062-51068、24960-24966 的相同实现）
 */
#define int long long
constexpr int N = 1e5 + 9;
int n, mi[20][N], dfn[N], idx, dep[N];
vector<int> es[N];
void dfs(int x, int ff) {
  mi[0][dfn[x] = ++idx] = ff, dep[x] = dep[ff] + 1;   // 欧拉序：mi[0][dfn[x]] 记父亲
  for (int y : es[x])
    if (y ^ ff) dfs(y, x);
}
int get(int x, int y) { return dfn[x] < dfn[y] ? x : y; }  // 取 dfn 更小者（深度更浅，即更靠祖先）
int lca(int x, int y) {
  if (x == y) return x;
  if ((x = dfn[x]) > (y = dfn[y])) swap(x, y);
  int d = __lg(y - ++x + 1);              // 区间 [x, y] 长度取对数
  return get(mi[d][x], mi[d][y - (1 << d) + 1]);
}
int dist(int x, int y) { return dep[x] + dep[y] - 2 * dep[lca(x, y)]; }  // 树上距离

#ifdef DEMO
signed main() {
  cin.tie(nullptr)->sync_with_stdio(false);
  cin >> n;                              // n 个点的树
  for (int i = 1, x, y; i < n; ++i) {
    cin >> x >> y;
    es[x].push_back(y), es[y].push_back(x);
  }
  dfs(1, 0);
  for (int i = 1; i < 20; ++i)
    for (int j = 1; j + (1 << i) - 1 <= n; ++j)
      mi[i][j] = get(mi[i - 1][j], mi[i - 1][j + (1 << (i - 1))]);
  int q;
  cin >> q;
  while (q--) {
    int x, y;
    cin >> x >> y;
    cout << "lca=" << lca(x, y) << " dist=" << dist(x, y) << '\n';
  }
  cout.flush();   // 演示用：确保缓冲输出落盘（OJ 正常退出会自动 flush）
}
#endif
